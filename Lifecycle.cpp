#include "Lifecycle.h"

#include <driver/rtc_io.h>

#include "Constants.h"
#include "RecordStorage.h"
#include "SpreadSheetGateway.h"
#include "Logger.h"

void Lifecycle::enableRTCGPIOHold()
{
  rtc_gpio_hold_en(gpio_num_t(Constants::Pins::EPAPER_RST));
  rtc_gpio_hold_en(gpio_num_t(Constants::Pins::CCS811_HW_RESET));
  rtc_gpio_hold_en(gpio_num_t(Constants::Pins::CCS811_WAKE));
  rtc_gpio_hold_en(gpio_num_t(Constants::Pins::EPAPER_KEY4));
}

void Lifecycle::disableRTCGPIOHold()
{
  rtc_gpio_hold_dis(gpio_num_t(Constants::Pins::EPAPER_RST));
  rtc_gpio_hold_dis(gpio_num_t(Constants::Pins::CCS811_HW_RESET));
  rtc_gpio_hold_dis(gpio_num_t(Constants::Pins::CCS811_WAKE));
  rtc_gpio_hold_dis(gpio_num_t(Constants::Pins::EPAPER_KEY4));
}

void Lifecycle::sync()
{
  const auto recordExists = RecordStorage::NumberOfRecords() > 0;
  if (!recordExists)
    Println("No record found. Skipping some processes...");

  if (recordExists)
  {
    Println("Drawing sync page...");
    ePaper.drawSyncPage();
  }

  Println("Connecting to the Internet...");
  if (!WiFiManager::connect(10000)) {
    Println("Failed to connect to the Internet.");
    return;
  }

  syncClockWithNTP();

  Record latestRecord;
  if (recordExists)
  {
    Println("Loading latest record...");
    latestRecord = RecordStorage::ReadLatestRecord();

    postRecordsOnStorage();
  }

  Println("Disconnecting from Wi-Fi...");
  WiFiManager::disconnect();

  if (recordExists)
  {
    Println("Drawing status page...");
    ePaper.drawStatusPage(latestRecord.dateTime, latestRecord.value, timeProvider.isRTCBatteryLow());
  }
}

void Lifecycle::syncClockWithNTP()
{
  Println("Syncing with NTP...");
  timeProvider.syncWithNTP();
}

void Lifecycle::postRecordsOnStorage()
{
  Println("Sending records...");
  const auto postSucceeded = SpreadSheetGateway::PostRecords();

  if (postSucceeded)
  {
    Println("Post succeeded.");
    RecordStorage::RemoveAllRecords();
  }
  else
  {
    Println("Post failed.");
  }
}

void Lifecycle::startDeepSleep()
{
  Println("Disconnecting from Wi-Fi...");
  WiFiManager::disconnect();

  Println("Putting e-Paper to deep-sleep...");
  ePaper.sleep();

  enableRTCGPIOHold();

  const auto syncKey = gpio_num_t(Constants::Pins::EPAPER_KEY4);

  rtc_gpio_init(syncKey);
  rtc_gpio_set_direction(syncKey, RTC_GPIO_MODE_INPUT_ONLY);
  rtc_gpio_pullup_en(syncKey);
  rtc_gpio_pulldown_dis(syncKey);

  esp_sleep_enable_ext0_wakeup(syncKey, LOW);

  const auto now = timeProvider.fetchRTCDateTime();

  auto latestRecordTime = RecordStorage::GetLatestRecordTime();
  auto sleepUntil = latestRecordTime + TimeSpan(60);
  const auto sleepFor = (sleepUntil - now).totalseconds();

  delay(100);

  esp_sleep_enable_timer_wakeup(sleepFor * 1000 * 1000);
  esp_deep_sleep_start();
}

void Lifecycle::handleSetup()
{
  Serial.begin(115200);
  RecordStorage::Begin();

  disableRTCGPIOHold();

  Wire.begin(Constants::Pins::SDA, Constants::Pins::SCL);

  Println("Initializing e-Paper...");
  ePaper.begin();

  const auto wakeupCause = esp_sleep_get_wakeup_cause();

  if (wakeupCause == ESP_SLEEP_WAKEUP_UNDEFINED)
  {
    Println("Cold boot.");

    Println("Drawing boot page...");
    ePaper.drawBootPage();

    Println("Initializing sensors...");
    sensors.begin();
  }
  else
  {
    Println("Wake up from deep-sleep.");

    sensors.beginWithoutHardwareInit();
  }

  if (wakeupCause == ESP_SLEEP_WAKEUP_EXT0 || !timeProvider.isRTCRunning())
  {
    Println("Syncing...");
    sync();
  }

  Println("Fetching time from RTC...");
  auto currentRecordTime = timeProvider.fetchRTCDateTime();

  const auto latestRecordTime = RecordStorage::GetLatestRecordTime();
  const auto elapsedSecondsFromLatestRecord = (currentRecordTime - latestRecordTime).totalseconds();
  if (elapsedSecondsFromLatestRecord < 60)
  {
    Println("Going into deep-sleep...");
    startDeepSleep();
    return;
  }

  Println("Checking RTC's battery status...");
  const auto isBatteryLow = timeProvider.isRTCBatteryLow();

  Println("Measuring...");
  const auto value = sensors.measure();

  Println("Saving record on storage...");
  RecordStorage::SaveRecord({ currentRecordTime, value });

  Println("Drawing status page...");
  ePaper.drawStatusPage(currentRecordTime, value, isBatteryLow);

  Println("Going into deep-sleep...");
  startDeepSleep();
}

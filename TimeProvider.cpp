#include "TimeProvider.h"

#include <Arduino.h>
#include <HTTPClient.h>
#include "Logger.h"
#include "Constants.h"

using namespace Constants::Clock;

bool TimeProvider::fetchTimeWithHttp(const String url, DateTime& dateTime)
{
  HTTPClient http;
  http.begin(url);

  http.setConnectTimeout(10 * 1000);
  http.setTimeout(10 * 1000);

  if (http.GET() != 200) return false;

  const auto html = http.getString();
  const auto timeStr = html.substring(46, html.indexOf("\n", 46));

  const uint32_t unixTime = timeStr.toDouble() + 0.5;
  dateTime = DateTime(unixTime + 9 * 60 * 60);

  return true;
}

bool TimeProvider::isRTCRunning()
{
  return rtc.initialized();
}

bool TimeProvider::isRTCBatteryLow()
{
  return rtc.readIfBatteryIsLow();
}

void TimeProvider::syncWithNTP()
{
  DateTime dateTime;

  if (!fetchTimeWithHttp(UNIX_TIME_URL, dateTime))
    return;

  rtc.reset();
  rtc.adjust(dateTime);
}

DateTime TimeProvider::fetchRTCDateTime()
{
  if (!isRTCRunning()) Throw("RTC is not running.");

  return rtc.now();
}

void TimeProvider::GetDateStr(char *str, const DateTime dateTime)
{
  sprintf(str, "%04u/%02u/%02u", dateTime.year(), dateTime.month(), dateTime.day());
}

void TimeProvider::GetTimeStr(char *str, const DateTime dateTime, const bool withSeconds = true)
{
  if (withSeconds) sprintf(str, "%02u:%02u:%02u", dateTime.hour(), dateTime.minute(), dateTime.second());
  else sprintf(str, "%02u:%02u", dateTime.hour(), dateTime.minute());
}

String TimeProvider::GetDateTimeStr(const DateTime dateTime)
{
  String result;
  char temp[16];
  
  GetDateStr(temp, dateTime);
  result += String(temp) + String(' ');
  
  GetTimeStr(temp, dateTime);
  result += String(temp);

  return result;
}

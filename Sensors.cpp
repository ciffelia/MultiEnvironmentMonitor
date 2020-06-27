#include "Sensors.h"

#include "Constants.h"
#include "Logger.h"

// BME280の初期化
void Sensors::beginBME280()
{
  bme280.begin();
}

// CCS811の初期化
void Sensors::beginCCS811()
{
  ccs811.reset();
  // I²C通信を開始
  ccs811.wake();

  // 60秒に一度測定するモードで初期化
  const CCS811::status returnCode = ccs811.begin(3);
  if (returnCode != CCS811::SENSOR_SUCCESS)
  {
    ccs811.sleep();
    Throw("ccs811.begin() returned with an error: %x.", returnCode);
  }

  // 割り込みを無効化
  ccs811.disableInterrupts();
  
  // 値が読み出せるようになるまで待機
  while (!ccs811.dataAvailable()) delay(1000);
  
  // I²C通信を停止
  ccs811.sleep();
}

// BME280で値を計測し取得
void Sensors::measureBME280()
{
  bme280.forceMeasure();

  measuredValue.temperature = bme280.readTemperature();
  measuredValue.humidity = bme280.readHumidity();
  measuredValue.pressure = bme280.readPressure();
}

// CCS811で計測した値を取得
void Sensors::measureCCS811()
{
  ccs811.wake();

  // BME280で計測した値を使用して測定値を補正
  ccs811.setEnvironmentalData(measuredValue.humidity, measuredValue.temperature);
  
  // 値が読み出せるようになるまで待機
  while (!ccs811.dataAvailable()) delay(1000);
  
  ccs811.readAlgorithmResults();
  measuredValue.co2 = ccs811.getCO2();
  measuredValue.tvoc = ccs811.getTVOC();
  
  ccs811.sleep();
}

Sensors::Sensors()
  : ccs811(Constants::Addresses::CCS811, Constants::Pins::CCS811_HW_RESET, Constants::Pins::CCS811_WAKE)
{
}

// 各センサの初期化
void Sensors::begin()
{ 
  beginBME280();
  beginCCS811();
}

// 各センサから測定に必要なパラメータを取得
void Sensors::beginWithoutHardwareInit()
{
  bme280.beginWithoutHardwareInit();
}

// 各センサで測定
Sensors::MeasuredValue Sensors::measure()
{
  measureBME280();
  measureCCS811();
  return measuredValue;
}

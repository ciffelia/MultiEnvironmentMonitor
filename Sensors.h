#ifndef SENSORS_H
#define SENSORS_H

#include <BME280.h>
#include <CCS811.h>

class Sensors {
public:
  struct MeasuredValue {
    float temperature;
    float humidity;
    float pressure;
    uint16_t co2;
    uint16_t tvoc;
  };

private:
  BME280 bme280;
  CCS811 ccs811;

  // センサから取得した値
  MeasuredValue measuredValue;

  // BME280の初期化
  void beginBME280();
  // CCS811の初期化
  void beginCCS811();

  // BME280で値を計測し取得
  void measureBME280();
  // CCS811で計測した値を取得
  void measureCCS811();

public:
  Sensors();

  // 各センサの初期化
  void begin();

  // 各センサから測定に必要なパラメータを取得
  void beginWithoutHardwareInit();

  // 各センサで測定
  MeasuredValue measure();
};

#endif

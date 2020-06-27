#ifndef EPAPER_H
#define EPAPER_H

#include <WiFi.h>
#include <SPI.h>
#include <epd2in7.h>
#include <epdpaint.h>
#include <RTClib.h>

#include "Constants.h"
#include "Sensors.h"
#include "Image.h"

class EPaper {
  Epd epd;
  unsigned char imageBuffer[Constants::EPaper::WIDTH * Constants::EPaper::HEIGHT / 8];
  Paint paint;

  static void GetMeasuredValueStr(char *temperatureStr, char *humidityStr, char *pressureStr, char *co2Str, char *tvocStr, const Sensors::MeasuredValue value);

  void drawImage(const int x, const int y, const Image &image, const int colored);

public:
  EPaper();

  void begin();

  void sleep();

  void drawStatusPage(const DateTime dateTime, const Sensors::MeasuredValue measuredValue, const bool isRTCBatteryLow);

  void drawBootPage();

  void drawSyncPage();
};

#endif

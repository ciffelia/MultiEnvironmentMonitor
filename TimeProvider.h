#ifndef TIME_PROVIDER_H
#define TIME_PROVIDER_H

#include <RTClib.h>

class TimeProvider {
private:
  RTC_PCF8523 rtc;

  bool fetchTimeWithHttp(const String url, DateTime& dateTime);

public:
  bool isRTCRunning();

  bool isRTCBatteryLow();

  void syncWithNTP();

  DateTime fetchRTCDateTime();

  static void GetDateStr(char *str, const DateTime dateTime);
  static void GetTimeStr(char *str, const DateTime dateTime, const bool withSeconds);

  static String GetDateTimeStr(const DateTime dateTime);
};

#endif

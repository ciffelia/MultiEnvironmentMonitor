#ifndef LIFECYCLE_H
#define LIFECYCLE_H

#include "EPaper.h"
#include "TimeProvider.h"
#include "Sensors.h"
#include "Record.h"
#include "WiFiManager.h"

class Lifecycle {
private:
  EPaper ePaper;
  TimeProvider timeProvider;
  Sensors sensors;

  void enableRTCGPIOHold();
  void disableRTCGPIOHold();

  void sync();

  void syncClockWithNTP();

  void postRecordsOnStorage();

  void startDeepSleep();

public:
  void handleSetup();
};

#endif

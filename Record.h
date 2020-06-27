#ifndef RECORD_H
#define RECORD_H

#include <RTClib.h>
#include "Sensors.h"

struct Record {
  DateTime dateTime;
  Sensors::MeasuredValue value;
};

#endif

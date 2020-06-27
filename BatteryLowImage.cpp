#include "BatteryLowImage.h"

const bool BatteryLowImageData[] = 
{
  0, 0, 1, 1, 1, 0, 0,
  1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 1, 0, 0, 1,
  1, 0, 0, 1, 0, 0, 1,
  1, 0, 0, 1, 0, 0, 1,
  1, 0, 0, 1, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 1, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1
};

const Image BatteryLowImage =
{
  BatteryLowImageData,
  7,
  13
};

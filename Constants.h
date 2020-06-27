#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace Constants
{
  // GPIOピンの番号
  namespace Pins
  {
    const uint8_t SDA = 4;
    const uint8_t SCL = 23;

    const uint8_t CCS811_HW_RESET = 25;
    const uint8_t CCS811_WAKE = 26;

    const uint8_t EPAPER_RST = 13;
    const uint8_t EPAPER_DC = 18;
    const uint8_t EPAPER_CS = 19;
    const uint8_t EPAPER_BUSY = 17;

    const uint8_t EPAPER_KEY1 = 22;
    const uint8_t EPAPER_KEY2 = 27;
    const uint8_t EPAPER_KEY3 = 13;
    const uint8_t EPAPER_KEY4 = 14;
  }

  // I²C 7ビットアドレス
  namespace Addresses
  {
    const uint8_t CCS811 = 0x5B;
  }

  namespace Clock
  {
    const char UNIX_TIME_URL[] = "http://ntp-a1.nict.go.jp/cgi-bin/jst";
  }

  namespace EPaper {
    const uint16_t WIDTH = 176;
    const uint16_t HEIGHT = 264;
    const uint8_t PAINT_BLACK = 0;
    const uint8_t PAINT_WHITE = 1;
  }

  namespace Storage {
    namespace Files {
      const char RECORDS[] = "/records.dat";
      const char LATEST_RECORD_TIME[] = "/latest_record.dat";
    }
  }
}

#endif

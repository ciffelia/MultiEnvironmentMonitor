#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>

struct Image {
  const bool *data;
  const uint16_t width;
  const uint16_t height;
};

#endif

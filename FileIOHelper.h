#ifndef FILE_IO_HELPER_H
#define FILE_IO_HELPER_H

#include <SPIFFS.h>

template<typename T>
T ReadFromFile(File& file)
{
  T data;
  file.read(reinterpret_cast<uint8_t*>(&data), sizeof(T));
  return data;
}

template<typename T>
size_t WriteToFile(File& file, const T& data)
{
  return file.write(reinterpret_cast<const uint8_t*>(&data), sizeof(T));
}

#endif

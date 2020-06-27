#ifndef LOGGER_H
#define LOGGER_H

#include <HardwareSerial.h>
#include "WiFiManager.h"
#include "FilenameMacro.h"

namespace InternalLogger
{
  template <typename ...Args>
  void Print(const char file[], const int line, const char function[], const Args &...args)
  {
    Serial.printf("[Log][%s:%d %s()] ", file, line, function);
    Serial.print(args...);
  }

  template <typename ...Args>
  void Println(const char file[], const int line, const char function[], const Args &...args)
  {
    Serial.printf("[Log][%s:%d %s()] ", file, line, function);
    Serial.println(args...);
  }

  template <typename ...Args>
  void Printf(const char file[], const int line, const char function[], const Args &...args)
  {
    Serial.printf("[Log][%s:%d %s()] ", file, line, function);
    Serial.printf(args...);
  }

  template <typename ...Args>
  void Throw(const char file[], const int line, const char function[], const Args &...args)
  {
    Serial.printf("[Error][%s:%d %s()] ", file, line, function);
    Serial.printf(args...);

    WiFiManager::disconnect();

    // Wait until serial messages are sent.
    delay(100);

    esp_deep_sleep_start();
  }
}

#define Print(...) InternalLogger::Print(__FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define Println(...) InternalLogger::Println(__FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define Printf(...) InternalLogger::Printf(__FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define Throw(...) InternalLogger::Throw(__FILENAME__, __LINE__, __func__, __VA_ARGS__)

#endif

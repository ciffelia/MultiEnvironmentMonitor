#include "WiFiManager.h"

#include <WiFi.h>

#include "SecretConstants.h"

bool WiFiManager::connect(const int timeout)
{
  const auto start = millis();

  WiFi.begin(Constants::Wifi::SSID, Constants::Wifi::PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    if ((millis() - start) > timeout) break;
    delay(100);
  }

  // Terminate Wi-Fi if failed
  if (WiFi.status() != WL_CONNECTED) WiFi.disconnect();

  return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::disconnect()
{
  WiFi.disconnect();
}

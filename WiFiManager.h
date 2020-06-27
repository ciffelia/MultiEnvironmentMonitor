#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

class WiFiManager {
public:
  static bool connect(const int timeout);

  static void disconnect();
};

#endif

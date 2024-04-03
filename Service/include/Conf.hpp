#pragma once

#include <Config.hpp>

#include <chrono>
#include <string>

namespace DBUS {
class Connection;
}

class Conf : public Config::File {
public:
    explicit Conf(DBUS::Connection& a_DBusConnection);
    void Update();

    double backlightMin     = 0;
    double backlightMax     = 1;
    double backlightScale   = 1;
    double keyboardLedMin   = 0;
    double keyboardLedMax   = 1;
    double keyboardLedScale = 1;
    double sensorScale      = 1;
    double sensorOffset     = 0;
    int loopDelay           = 5000;

    DBUS::Connection& dBusConnection;

    std::chrono::high_resolution_clock::time_point lastUpdate;
};
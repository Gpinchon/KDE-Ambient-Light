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

    std::filesystem::path backlightPath = "/sys/class/backlight/amdgpu_bl1";
    bool backlightEnabled   = true;
    double backlightMin     = 0;
    double backlightMax     = 1;
    double backlightScale   = 1;

    std::filesystem::path keyboardLedPath = "/sys/class/leds/platform::kbd_backlight";
    bool keyboardLedEnabled = true;
    double keyboardLedMin   = 0;
    double keyboardLedMax   = 1;
    double keyboardLedScale = 1;

    std::filesystem::path sensorPath = "/sys/bus/iio/devices/iio:device0";
    double sensorScale      = 1;
    double sensorOffset     = 0;
    int loopDelay           = 5000;
    int updateDelay         = 5000;
    bool firstUpdate        = true;
    

    DBUS::Connection& dBusConnection;

    std::chrono::high_resolution_clock::time_point lastUpdate;
};
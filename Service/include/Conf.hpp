#pragma once

#include <string>
#include <chrono>

class DBUSConnection;

class Conf
{
public:
    explicit Conf(DBUSConnection &a_DBusConnection);
    void Update();

    int loopDelay = 0;

    int confUpdateDelay = 0;

    std::string sensorPath;
    float sensorScale = 0;
    float sensorOffset = 0;
    int sensorDelay = 0;

    bool backlightEnabled = true;
    float backlightMax = 0;
    float backlightMin = 0;
    int32_t backlightScale = 0;
    int backlightDelay = 0;

    bool keyboardLedEnabled = true;
    float keyboardLedMin = 0;
    float keyboardLedMax = 0;
    int32_t keyboardLedScale = 0;
    int keyboardLedDelay = 0;

    float maxLuxBreakpoint = 0;

    DBUSConnection &dBusConnection;

private:
    std::chrono::high_resolution_clock::time_point lastUpdate;
};
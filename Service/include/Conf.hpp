#pragma once

#include <string>
#include <chrono>

struct Conf
{
    Conf();
    void Update();

    int loopDelay = 0;

    int confUpdateDelay = 0;

    std::string sensorPath;
    float sensorScale = 0;
    float sensorOffset = 0;
    int sensorDelay = 0;

    std::string backlightPath;
    bool backlightEnabled = true;
    float backlightMax = 0;
    float backlightMin = 0;
    float backlightScale = 0;
    int backlightDelay = 0;

    std::string keyboardLedPath;
    bool keyboardLedEnabled = true;
    float keyboardLedMin = 0;
    float keyboardLedMax = 0;
    float keyboardLedScale = 0;
    int keyboardLedDelay = 0;

    float maxLuxBreakpoint = 0;

    std::chrono::high_resolution_clock::time_point lastUpdate;
};
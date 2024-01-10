#pragma once

#include <string>
#include <chrono>

struct Conf
{
    Conf()
    {
        Update();
    }
    void Update();

    int confUpdateDelay = 0;

    std::string sensorPath;

    std::string backlightPath;
    float backlightMax = 0;
    float backlightMin = 0;
    float backlightScale = 0;
    float backlightSteps = 0;
    int backlightDelay = 0;

    std::string keyboardLedPath;
    float keyboardScale = 0;

    float maxLuxBreakpoint = 0;

    std::chrono::high_resolution_clock::time_point lastUpdate;
};
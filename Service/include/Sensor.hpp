#pragma once

#include <chrono>

class Conf;

class Sensor
{
public:
    Sensor(Conf &a_Conf) : conf(a_Conf) {}
    void Update();
    float GetBrightness() const;

private:
    Conf &conf;
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    bool firstUpdate = true;
    float illuminance = 0;
};
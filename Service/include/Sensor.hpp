#pragma once

#include <chrono>
#include <filesystem>

class Conf;

class Sensor
{
public:
    Sensor(Conf &a_Conf);
    void Update();
    float GetBrightness() const;

private:
    Conf &conf;
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    bool firstUpdate = true;
    std::filesystem::path illuminancePath;
    float illuminance = 0;
};
#pragma once

#include <chrono>
#include <filesystem>

class Conf;

class Backlight {
public:
    Backlight(Conf& a_Conf);
    void Update();
    void SetBrightness(const double& a_Value);

private:
    Conf& conf;
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    bool firstUpdate                                          = true;
    const std::filesystem::path brightnessPath;
    double brightness;
    double lastBrightness;
};
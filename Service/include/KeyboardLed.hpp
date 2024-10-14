#pragma once

#include <chrono>
#include <filesystem>

class Conf;

class KeyboardLed
{
public:
    KeyboardLed(Conf &a_Conf);
    void Update();
    void SetBrightness(const double &a_Value);

private:
    Conf &conf;
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    bool firstUpdate = true;
    std::filesystem::path brightnessPath;
    float brightness;
};
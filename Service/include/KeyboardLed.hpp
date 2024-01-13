#pragma once

#include <chrono>

class Conf;

class KeyboardLed
{
public:
    KeyboardLed(const Conf &a_Conf) : conf(a_Conf) {}
    void Update();
    void SetBrightness(const float &a_Value);

private:
    const Conf &conf;
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    bool firstUpdate = true;
    float brightness;
};
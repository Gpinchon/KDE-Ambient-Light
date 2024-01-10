#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cmath>
#include <thread>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <algorithm>

#include <Conf.hpp>

Conf config;

float BrightessFromLuxReading(float a_Lux)
{
    if (a_Lux > config.maxLuxBreakpoint)
        return 1.0;
    else if (a_Lux > 0)
        return (9.9323 * log(a_Lux) + 27.059) / 100.0;
    else
        return 0;
}

class Sensor
{
public:
    Sensor()
    {
        std::ifstream(config.sensorPath + "/in_illuminance_scale") >> illuminanceScale;
        std::ifstream(config.sensorPath + "/in_illuminance_offset") >> illuminanceOffset;
    }
    //@return the sensor illuminance in lumens
    float GetIlluminance()
    {
        float illuminanceRaw;
        std::ifstream(config.sensorPath + "/in_illuminance_raw") >> illuminanceRaw;
        return illuminanceRaw * illuminanceScale + illuminanceOffset;
    }
    float illuminanceScale = 0;
    float illuminanceOffset = 0;
};

class Backlight
{
public:
    void Update()
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
        if (delta < config.backlightDelay)
            return;
        int curBrightness = 0;
        std::ifstream(config.backlightPath + "/brightness") >> curBrightness;
        auto newBrightness = int(brightness * config.backlightScale);
        newBrightness = curBrightness * 0.9f + newBrightness * 0.1f;
        if (curBrightness == newBrightness)
            return;
        std::ofstream(config.backlightPath + "/brightness") << newBrightness;
        std::cout << "Backlight brightness :\n"
                  << "Min Brightness   " << config.backlightMin << "\n"
                  << "Max Brightness   " << config.backlightMax << "\n"
                  << "Brightness Scale " << config.backlightScale << "\n"
                  << "New Brightness   " << newBrightness << "\n"
                  << "Brightness       " << brightness << std::endl;
        lastUpdate = std::chrono::high_resolution_clock::now();
    }
    void SetBrightness(const float &a_Value)
    {
        brightness = std::clamp(a_Value, config.backlightMin, config.backlightMax);
    }

private:
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    float brightness;
};

class KeyboardLed
{
public:
    void Update()
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
        if (delta < config.backlightDelay)
            return;
        float curBrightness = 0;
        std::ifstream(config.keyboardLedPath + "/brightness") >> curBrightness;
        auto newBrightness = int(brightness * config.keyboardLedScale);
        if (curBrightness == newBrightness)
            return;
        std::ofstream(config.keyboardLedPath + "/brightness") << newBrightness;
        std::cout << "Keyboard brightness :\n"
                  << "Min Brightness   " << config.keyboardLedMin << "\n"
                  << "Max Brightness   " << config.keyboardLedMax << "\n"
                  << "Brightness Scale " << config.keyboardLedScale << "\n"
                  << "New Brightness   " << newBrightness << "\n"
                  << "Brightness       " << brightness << std::endl;
        lastUpdate = std::chrono::high_resolution_clock::now();
    }

    void SetBrightness(const float &a_Value)
    {
        brightness = std::clamp(a_Value, config.keyboardLedMin, config.keyboardLedMax);
    }

private:
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    float brightness;
};

int main(int argc, char const *argv[])
{
    Sensor sensor;
    Backlight backlight;
    KeyboardLed keyboardLed;

    auto delay = std::chrono::milliseconds(16);
    std::cout << "BacklightPath   : " << config.backlightPath << "\n"
              << "SensorPath      : " << config.sensorPath << "\n"
              << "KeyboardLedPath : " << config.keyboardLedPath << "\n";
    while (true)
    {
        auto sensorIlluminance = sensor.GetIlluminance();
        auto brightness = BrightessFromLuxReading(sensorIlluminance);
        backlight.SetBrightness(brightness);
        keyboardLed.SetBrightness(1 - brightness);
        config.Update();
        backlight.Update();
        keyboardLed.Update();
        std::this_thread::sleep_for(delay);
    }
    return 0;
}

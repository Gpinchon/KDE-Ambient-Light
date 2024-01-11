#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <cmath>
#include <thread>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <algorithm>

#include <Tools.hpp>
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
    void Update()
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
        if (!firstUpdate && delta < config.sensorDelay)
            return;
        std::ifstream(config.sensorPath + "/in_illuminance_raw") >> illuminance;
        Log() << "Get sensor illuminance : " << illuminance << "\n";
        illuminance = illuminance * config.sensorScale + config.sensorOffset;
        lastUpdate = std::chrono::high_resolution_clock::now();
        firstUpdate = false;
    }
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    bool firstUpdate = true;
    float illuminance = 0;
};

class Backlight
{
public:
    void Update()
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
        if (!firstUpdate && delta < config.backlightDelay)
            return;
        int curBrightness = 0;
        std::ifstream(config.backlightPath + "/brightness") >> curBrightness;
        auto newBrightness = int(brightness * config.backlightScale);
        newBrightness = curBrightness * 0.9f + newBrightness * 0.1f;
        if (curBrightness == newBrightness)
            return;
        std::ofstream(config.backlightPath + "/brightness") << newBrightness;
        Log() << "Backlight brightness :\n"
              << "Min Brightness   " << config.backlightMin << "\n"
              << "Max Brightness   " << config.backlightMax << "\n"
              << "Brightness Scale " << config.backlightScale << "\n"
              << "New Brightness   " << newBrightness << "\n"
              << "Brightness       " << brightness << std::endl;
        lastUpdate = std::chrono::high_resolution_clock::now();
        firstUpdate = false;
    }
    void SetBrightness(const float &a_Value)
    {
        brightness = std::clamp(a_Value, config.backlightMin, config.backlightMax);
    }

private:
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    bool firstUpdate = true;
    float brightness;
};

class KeyboardLed
{
public:
    void Update()
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
        if (!firstUpdate && delta < config.sensorDelay)
            return;
        float curBrightness = 0;
        std::ifstream(config.keyboardLedPath + "/brightness") >> curBrightness;
        auto newBrightness = int(brightness * config.keyboardLedScale);
        if (curBrightness == newBrightness)
            return;
        std::ofstream(config.keyboardLedPath + "/brightness") << newBrightness;
        Log() << "Keyboard brightness :\n"
              << "Min Brightness   " << config.keyboardLedMin << "\n"
              << "Max Brightness   " << config.keyboardLedMax << "\n"
              << "Brightness Scale " << config.keyboardLedScale << "\n"
              << "New Brightness   " << newBrightness << "\n"
              << "Brightness       " << brightness << std::endl;
        lastUpdate = std::chrono::high_resolution_clock::now();
        firstUpdate = false;
    }

    void SetBrightness(const float &a_Value)
    {
        brightness = std::clamp(a_Value, config.keyboardLedMin, config.keyboardLedMax);
    }

private:
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    bool firstUpdate = true;
    float brightness;
};

int main(int argc, char const *argv[])
{
    Log() << "Starting...\n";
    Sensor sensor;
    Backlight backlight;
    KeyboardLed keyboardLed;

    Log() << "BacklightPath   : " << config.backlightPath << "\n"
          << "SensorPath      : " << config.sensorPath << "\n"
          << "KeyboardLedPath : " << config.keyboardLedPath << "\n";
    while (true)
    {
        config.Update();
        sensor.Update();
        auto brightness = BrightessFromLuxReading(sensor.illuminance);
        if (config.backlightEnabled)
        {
            backlight.SetBrightness(brightness);
            backlight.Update();
        }
        if (config.keyboardLedEnabled)
        {
            keyboardLed.SetBrightness(1 - brightness);
            keyboardLed.Update();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(config.loopDelay));
    }
    return 0;
}

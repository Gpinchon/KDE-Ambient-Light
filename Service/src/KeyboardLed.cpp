#include <Conf.hpp>
#include <DBUS.hpp>
#include <KeyboardLed.hpp>
#include <ServiceConfKeys.hpp>
#include <Tools.hpp>

#include <algorithm>
#include <cassert>
#include <fstream>

KeyboardLed::KeyboardLed(Conf &a_Conf)
    : conf(a_Conf)
    , brightnessPath(conf.keyboardLedPath / "brightness")
{
    assert(std::filesystem::exists(brightnessPath));
}

void KeyboardLed::Update()
{
    const auto now   = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (!firstUpdate && delta < conf.Get(KeyboardLedDelay, DefaultKeyboardLedDelay))
        return;
    int curBrightness = 0;
    {
        std::ifstream(brightnessPath) >> curBrightness;
    }
    lastUpdate = std::chrono::high_resolution_clock::now();
    if (auto newBrightness = int(brightness * conf.keyboardLedScale); curBrightness == newBrightness) {
        std::ofstream(brightnessPath) << newBrightness;
        Log() << "Keyboard brightness :\n"
              << "Min Brightness   " << conf.keyboardLedMin << "\n"
              << "Max Brightness   " << conf.keyboardLedMax << "\n"
              << "Brightness Scale " << conf.keyboardLedScale << "\n"
              << "New Brightness   " << newBrightness << "\n"
              << "Brightness       " << brightness << std::endl;
    }
    firstUpdate = false;
}

void KeyboardLed::SetBrightness(const double& a_Value)
{
    brightness = float(std::clamp(a_Value,
        conf.keyboardLedMin,
        conf.keyboardLedMax));
}
#include <Backlight.hpp>
#include <Conf.hpp>
#include <DBUS.hpp>
#include <ServiceConfKeys.hpp>
#include <Tools.hpp>

#include <algorithm>
#include <fstream>
#include <cassert>

Backlight::Backlight(Conf& a_Conf)
    : conf(a_Conf)
    , brightnessPath(conf.Get(BacklightPath, DefaultBacklightPath) + "brightness")
{
    assert(std::filesystem::exists(brightnessPath));
}

void Backlight::Update()
{
    const auto now   = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (!firstUpdate && delta < conf.Get(BacklightDelay, DefaultBacklightDelay))
        return;
    int curBrightness = 0;
    {
        std::ifstream(brightnessPath) >> curBrightness;
    }
    if (firstUpdate)
        lastBrightness = curBrightness / conf.backlightScale;
    auto brightnessTransitionTime = conf.Get(BacklightTransitionTime, DefaultBacklightTransitionTime);
    auto newBrightnessWeight      = std::min(delta / brightnessTransitionTime, 1.0);
    auto curBrightnessWeight      = 1 - newBrightnessWeight;
    auto newBrightness            = lastBrightness * curBrightnessWeight + brightness * newBrightnessWeight;
    lastUpdate                    = std::chrono::high_resolution_clock::now();
    lastBrightness                = newBrightness;
    if (auto newBrightnessInt = int(newBrightness * conf.backlightScale); curBrightness != newBrightnessInt) {
        std::ofstream(brightnessPath, std::ios_base::trunc) << newBrightnessInt;
        Log() << "Backlight brightness :\n"
              << "Min Brightness   " << conf.backlightMin << "\n"
              << "Max Brightness   " << conf.backlightMax << "\n"
              << "Brightness Scale " << conf.backlightScale << "\n"
              << "Brightness       " << brightness << "\n"
              << "New Brightness   " << (newBrightness * conf.backlightScale) << "\n"
              << std::endl;
    }
    firstUpdate = false;
}
void Backlight::SetBrightness(const double& a_Value)
{
    brightness = std::clamp(a_Value,
        conf.backlightMin,
        conf.backlightMax);
}
#include <Backlight.hpp>
#include <DBUS.hpp>
#include <Conf.hpp>
#include <Tools.hpp>

#include <algorithm>

void Backlight::Update()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (!firstUpdate && delta < conf.backlightDelay)
        return;
    int curBrightness = 0;

    {
        DBUS::MethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/BrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.BrightnessControl",
                                  "brightness");
        DBUS::Reply reply(conf.dBusConnection.Send(methodCall));
        curBrightness = std::any_cast<int32_t>(reply.GetArgs().front());
    }

    auto newBrightness = int(brightness * conf.backlightScale);
    newBrightness = curBrightness * 0.9f + newBrightness * 0.1f;
    if (curBrightness == newBrightness)
        return;
    {
        DBUS::MethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/BrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.BrightnessControl",
                                  "setBrightnessSilent");
        methodCall.SetArgs(DBUS_TYPE_INT32, &newBrightness);
        conf.dBusConnection.Send(methodCall);
    }
    Log() << "Backlight brightness :\n"
          << "Min Brightness   " << conf.backlightMin << "\n"
          << "Max Brightness   " << conf.backlightMax << "\n"
          << "Brightness Scale " << conf.backlightScale << "\n"
          << "New Brightness   " << newBrightness << "\n"
          << "Brightness       " << brightness << std::endl;
    lastUpdate = std::chrono::high_resolution_clock::now();
    firstUpdate = false;
}
void Backlight::SetBrightness(const float &a_Value)
{
    brightness = std::clamp(a_Value, conf.backlightMin, conf.backlightMax);
}
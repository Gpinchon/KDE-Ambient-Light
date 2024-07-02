#include <Backlight.hpp>
#include <Conf.hpp>
#include <DBUS.hpp>
#include <ServiceConfKeys.hpp>
#include <Tools.hpp>

#include <algorithm>

void Backlight::Update()
{
    const auto now   = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (!firstUpdate && delta < conf.Get(BacklightDelay, DefaultBacklightDelay))
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
    if (firstUpdate)
        lastBrightness = curBrightness / conf.backlightScale;
    auto brightnessTransitionTime = conf.Get(BacklightTransitionTime, DefaultBacklightTransitionTime);
    auto newBrightnessWeight      = std::min(delta / brightnessTransitionTime, 1.0);
    auto curBrightnessWeight      = 1 - newBrightnessWeight;
    auto newBrightness            = lastBrightness * curBrightnessWeight + brightness * newBrightnessWeight;
    lastUpdate                    = std::chrono::high_resolution_clock::now();
    lastBrightness                = newBrightness;
    if (auto newBrightnessInt = int(newBrightness * conf.backlightScale); curBrightness != newBrightnessInt) {
        DBUS::MethodCall methodCall("org.kde.Solid.PowerManagement",
            "/org/kde/Solid/PowerManagement/Actions/BrightnessControl",
            "org.kde.Solid.PowerManagement.Actions.BrightnessControl",
            "setBrightnessSilent");
        methodCall.SetArgs(DBUS_TYPE_INT32, &newBrightnessInt);
        conf.dBusConnection.SendNoReply(methodCall);
        Log() << "Backlight brightness :\n"
              << "Min Brightness   " << conf.backlightMin << "\n"
              << "Max Brightness   " << conf.backlightMax << "\n"
              << "Brightness       " << brightness
              << "Brightness Scale " << conf.backlightScale << "\n"
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
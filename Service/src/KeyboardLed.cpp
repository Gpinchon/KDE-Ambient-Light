#include <KeyboardLed.hpp>
#include <DBUS.hpp>
#include <Conf.hpp>
#include <Tools.hpp>

#include <algorithm>

void KeyboardLed::Update()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (!firstUpdate && delta < conf.sensorDelay)
        return;
    int curBrightness = 0;

    {
        DBUS::MethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/KeyboardBrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.KeyboardBrightnessControl",
                                  "keyboardBrightness");
        DBUS::Reply reply(conf.dBusConnection.Send(methodCall));
        curBrightness = std::any_cast<int32_t>(reply.GetArgs().front());
    }

    auto newBrightness = int(brightness * conf.keyboardLedScale);
    if (curBrightness == newBrightness)
        return;
    {
        DBUS::MethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/KeyboardBrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.KeyboardBrightnessControl",
                                  "setKeyboardBrightnessSilent");
        methodCall.SetArgs(DBUS_TYPE_INT32, &newBrightness);
        conf.dBusConnection.Send(methodCall);
    }
    Log() << "Keyboard brightness :\n"
          << "Min Brightness   " << conf.keyboardLedMin << "\n"
          << "Max Brightness   " << conf.keyboardLedMax << "\n"
          << "Brightness Scale " << conf.keyboardLedScale << "\n"
          << "New Brightness   " << newBrightness << "\n"
          << "Brightness       " << brightness << std::endl;
    lastUpdate = std::chrono::high_resolution_clock::now();
    firstUpdate = false;
}

void KeyboardLed::SetBrightness(const float &a_Value)
{
    brightness = std::clamp(a_Value, conf.keyboardLedMin, conf.keyboardLedMax);
}
#include <KeyboardLed.hpp>
#include <DBUS.hpp>
#include <Conf.hpp>
#include <ConfKeys.hpp>
#include <Tools.hpp>

#include <algorithm>

void KeyboardLed::Update()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (!firstUpdate && delta < conf.Get(KeyboardLedDelay, DefaultKeyboardLedDelay))
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

    auto newBrightness = int(brightness * conf.Get(KeyboardLedScale, DefaultKeyboardLedScale));
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
          << "Min Brightness   " << conf.Get(KeyboardLedMin, DefaultKeyboardLedMin) << "\n"
          << "Max Brightness   " << conf.Get(KeyboardLedMax, DefaultKeyboardLedMax) << "\n"
          << "Brightness Scale " << conf.Get(KeyboardLedScale, DefaultKeyboardLedScale) << "\n"
          << "New Brightness   " << newBrightness << "\n"
          << "Brightness       " << brightness << std::endl;
    lastUpdate = std::chrono::high_resolution_clock::now();
    firstUpdate = false;
}

void KeyboardLed::SetBrightness(const double &a_Value)
{
    brightness = float(std::clamp(a_Value,
        conf.Get(KeyboardLedMin, DefaultKeyboardLedMin),
        conf.Get(KeyboardLedMax, DefaultKeyboardLedMax)));
}
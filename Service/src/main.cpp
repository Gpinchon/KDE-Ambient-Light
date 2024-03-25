#include <DBUS.hpp>
#include <KeyboardLed.hpp>
#include <Backlight.hpp>
#include <Sensor.hpp>
#include <Tools.hpp>
#include <Conf.hpp>

#include <thread>

int main(int argc, char const *argv[])
{
    Log() << "Starting...\n";
    DBUS::Connection sessionDBusConnection(DBUS_BUS_SESSION);
    Conf config(sessionDBusConnection);
    Sensor sensor(config);
    Backlight backlight(config);
    KeyboardLed keyboardLed(config);

    Log() << "SensorPath : " << config.sensorPath << "\n";
    while (true)
    {
        config.Update();
        sensor.Update();
        auto brightness = sensor.GetBrightness();
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

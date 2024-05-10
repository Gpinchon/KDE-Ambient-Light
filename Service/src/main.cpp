#include <Backlight.hpp>
#include <Conf.hpp>
#include <DBUS.hpp>
#include <KeyboardLed.hpp>
#include <Sensor.hpp>
#include <ServiceConfKeys.hpp>
#include <Tools.hpp>

#include <thread>

int main(int, char const*[])
{
    Log() << "Starting...\n";
    DBUS::Connection sessionDBusConnection(DBUS_BUS_SESSION);
    Conf config(sessionDBusConnection);
    Sensor sensor(config);
    Backlight backlight(config);
    KeyboardLed keyboardLed(config);

    Log() << "SensorPath : " << config.Get(SensorPath, DefaultSensorPath) << "\n";
    while (true) {
        config.Update();
        sensor.Update();
        auto brightness = sensor.GetBrightness();
        if (config.backlightEnabled != 0) {
            backlight.SetBrightness(brightness);
            backlight.Update();
        }
        if (config.keyboardLedEnabled != 0) {
            keyboardLed.SetBrightness(1 - brightness);
            keyboardLed.Update();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(config.loopDelay));
    }
    return 0;
}

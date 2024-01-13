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

#include <DBUS.hpp>
#include <KeyboardLed.hpp>
#include <Backlight.hpp>
#include <Sensor.hpp>
#include <Tools.hpp>
#include <Conf.hpp>

int main(int argc, char const *argv[])
{
    Log() << "Starting...\n";
    DBUSConnection dBusConnection;
    Conf config;
    Sensor sensor(config, dBusConnection);
    Backlight backlight(config, dBusConnection);
    KeyboardLed keyboardLed(config, dBusConnection);

    Log() << "BacklightPath   : " << config.backlightPath << "\n"
          << "SensorPath      : " << config.sensorPath << "\n"
          << "KeyboardLedPath : " << config.keyboardLedPath << "\n";
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

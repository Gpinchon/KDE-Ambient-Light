#include <Sensor.hpp>
#include <Conf.hpp>
#include <DBUS.hpp>
#include <Tools.hpp>

#include <cmath>
#include <fstream>

void Sensor::Update()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (!firstUpdate && delta < conf.sensorDelay)
        return;
    std::ifstream(conf.sensorPath + "/in_illuminance_raw") >> illuminance;
    Log() << "Get sensor illuminance : " << illuminance << "\n";
    illuminance = illuminance * conf.sensorScale + conf.sensorOffset;
    lastUpdate = std::chrono::high_resolution_clock::now();
    firstUpdate = false;
}

// inspired by https://www.analog.com/en/design-notes/a-simple-implementation-of-lcd-brightness-control-using-the-max44009-ambientlight-sensor.html
float Sensor::GetBrightness() const
{
    if (illuminance > conf.maxLuxBreakpoint)
        return 1.0;
    else if (illuminance > 0)
        return (9.9323 * log(illuminance) + 27.059) / 100.0;
    else
        return 0;
}
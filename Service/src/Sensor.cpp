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

float Sensor::GetBrightness() const
{
    auto x = std::min(illuminance, conf.maxLuxBreakpoint) / conf.maxLuxBreakpoint;
    if (illuminance > 0)
        return 1 - powf(1 - x, conf.sensorSmoothing);
    else
        return 0;
}
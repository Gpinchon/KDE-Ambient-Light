#include <Conf.hpp>
#include <DBUS.hpp>
#include <Sensor.hpp>
#include <ServiceConfKeys.hpp>
#include <Tools.hpp>

#include <cmath>
#include <fstream>
#include <cassert>

Sensor::Sensor(Conf &a_Conf)
    : conf(a_Conf)
    , illuminancePath(conf.sensorPath / "in_illuminance_raw")
{
    assert(std::filesystem::exists(illuminancePath));
}

void Sensor::Update()
{
    const auto now   = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (!firstUpdate && delta < conf.Get(SensorDelay, DefaultSensorDelay))
        return;
    std::ifstream(illuminancePath) >> illuminance;
    Log() << "Sensor illuminance " << illuminance << "\n";
    illuminance = illuminance * conf.sensorScale + conf.sensorOffset;
    lastUpdate  = std::chrono::high_resolution_clock::now();
    firstUpdate = false;
}

constexpr auto FuncMinValue = 1.f;
constexpr auto FuncMaxValue = 1000.f;

auto GetFuncValue(float a_Value, float a_Smoothing)
{
    return pow(log(a_Value), a_Smoothing);
}

auto NormalizeValue(float a_X, float a_MinX, float a_MaxX, float a_NewMin, float a_NewMax)
{
    return (a_X - a_MinX) / (a_MaxX - a_MinX) * (a_NewMax - a_NewMin) + a_NewMin;
}

float Sensor::GetBrightness() const
{
    auto maxLuxBreakpoint = float(conf.Get(MaxLuxBreakpoint, DefaultMaxLuxBreakpoint));
    auto sensorSmoothing  = conf.Get(SensorSmoothing, DefaultSensorSmoothing);
    auto x                = std::min(illuminance, maxLuxBreakpoint);
    x                     = NormalizeValue(x, 0, maxLuxBreakpoint, FuncMinValue, FuncMaxValue);
    if (illuminance > 0)
        return NormalizeValue(
            GetFuncValue(x, sensorSmoothing),
            GetFuncValue(FuncMinValue, sensorSmoothing),
            GetFuncValue(FuncMaxValue, sensorSmoothing),
            0, 1);
    else
        return 0;
}
#pragma once

#include <chrono>

struct Conf;
class DBUSConnection;

class Backlight
{
public:
    Backlight(const Conf &a_Conf, DBUSConnection &a_DBUSConnection) : conf(a_Conf), dBusConnection(a_DBUSConnection) {}
    void Update();
    void SetBrightness(const float &a_Value);

private:
    const Conf &conf;
    DBUSConnection &dBusConnection;
    std::chrono::high_resolution_clock::time_point lastUpdate = std::chrono::high_resolution_clock::now();
    bool firstUpdate = true;
    float brightness;
};
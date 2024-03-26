#pragma once

#include <Config.hpp>

#include <string>
#include <chrono>

namespace DBUS {
class Connection;
}

class Conf : public Config::File
{
public:
    explicit Conf(DBUS::Connection &a_DBusConnection);
    void Update();

    int loopDelay = 5000;

    DBUS::Connection &dBusConnection;

    std::chrono::high_resolution_clock::time_point lastUpdate;
};
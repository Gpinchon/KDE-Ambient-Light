#pragma once

#include <string>
#include <chrono>

class DBUSConnection;

class PlasmaPowerSettings
{
public:
    PlasmaPowerSettings();
    float GetDisplayMaxBrightness() const;
    float GetKeyboardMaxBrightness() const;
    float GetDisplayACMaxBrightness() const;
    float GetDisplayBATMaxBrightness() const;
    float GetDisplayLowBATMaxBrightness() const;
    float GetKeyboardACMaxBrightness() const;
    float GetKeyboardBATMaxBrightness() const;
    float GetKeyboardLowBATMaxBrightness() const;

private:
    bool _OnBattery() const;
    bool _LowBattery() const;
    float _GetBrightness(const std::string& a_ValName, const std::string& a_Cmd) const;
    std::string _backlightACCmd;
    std::string _backlightBATCmd;
    std::string _backlightLowBATCmd;
    std::string _keyboardACCmd;
    std::string _keyboardBATCmd;
    std::string _keyboardLowBATCmd;
};

class Conf
{
public:
    explicit Conf(DBUSConnection &a_DBusConnection);
    void Update();

    int loopDelay = 0;

    int confUpdateDelay = 0;

    std::string sensorPath;
    float sensorScale = 0;
    float sensorOffset = 0;
    float sensorSmoothing = 0;
    int sensorDelay = 0;

    bool backlightEnabled = true;
    float backlightMax = 0;
    float backlightMin = 0;
    int32_t backlightScale = 0;
    int backlightDelay = 0;

    bool keyboardLedEnabled = true;
    float keyboardLedMin = 0;
    float keyboardLedMax = 0;
    int32_t keyboardLedScale = 0;
    int keyboardLedDelay = 0;

    float maxLuxBreakpoint = 0;

    DBUSConnection &dBusConnection;

    std::chrono::high_resolution_clock::time_point lastUpdate;

    PlasmaPowerSettings powerSettings;
};
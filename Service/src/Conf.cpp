#include <Conf.hpp>
#include <Config.hpp>
#include <Tools.hpp>

#include <iostream>
#include <fstream>
#include <glob.h>
#include <pwd.h>
#include <unistd.h>

constexpr auto DefaultConfUpdateDelay = 5000;

constexpr auto DefaultSensorPath = "/sys/bus/iio/devices/iio:device0";
constexpr auto DefaultSensorDelay = 500;

constexpr auto DefaultBacklightEnabled = true;
constexpr auto DefaultBacklightPath = "/sys/class/backlight/*";
constexpr auto DefaultBacklightDelay = 32; // default update frequency is 30 times/sec
constexpr auto DefaultBacklightMin = 0;
constexpr auto DefaultBacklightMax = 1;

constexpr auto DefaultKeyboardLedEnabled = true;
constexpr auto DefaultKeyboardLedPath = "/sys/class/leds/platform::kbd_backlight/";
constexpr auto DefaultKeyboardLedDelay = 1000;
constexpr auto DefaultKeyboardLedMin = 0;
constexpr auto DefaultKeyboardLedMax = 1;

constexpr auto DefaultMaxLuxBreakpoint = 1254;

std::string GetHomeDir()
{
    static std::string homeDir;
    if (!homeDir.empty())
        return homeDir;
    Log() << "Are we running a root ?\n";
    if (getuid() != 0)
    {
        Error() << "We need root access\n";
        exit(-1);
    }
    Log() << "Getting home directory...\n";
    std::string logName = exec("logname");
    Log() << "Logname " << logName << "\n";
    auto pw = getpwnam(logName.c_str());
    if (pw == nullptr)
    {
        throw std::runtime_error("Cannot retreive user");
    }
    if (pw->pw_name == nullptr || std::string(pw->pw_name) == "root")
    {
        Log() << "Running as root, figuring out sudoer\n";
        auto sudoer = getenv("SUDO_UID");
        pw = getpwuid(std::stoi(sudoer));
    }
    return homeDir = pw->pw_dir;
}

std::filesystem::path GetConfigPath()
{
    return GetHomeDir() + "/.config/ambient-light";
}

std::string GetBatteryStatus()
{
    return exec("cat /sys/class/power_supply/BAT0/status");
}

std::string GetBatteryCapacityLevel()
{
    return exec("cat /sys/class/power_supply/BAT0/capacity_level");
}

float GetBacklightACMaxBrightness()
{
    auto cmdRet = exec("kreadconfig5 --file " + GetHomeDir() + "/.config/powermanagementprofilesrc --group AC --group BrightnessControl --key value --default 100");
    try
    {
        Log() << "Max BrightnessControl on AC set to " << cmdRet;
        return std::stof(cmdRet) / 100.f;
    }
    catch (std::invalid_argument &)
    {
        return 1;
    }
}

float GetBacklightBatteryMaxBrightness()
{

    auto cmdRet = exec("kreadconfig5 --file " + GetHomeDir() + "/.config/powermanagementprofilesrc --group Battery --group BrightnessControl --key value --default 100");
    try
    {
        Log() << "Max BrightnessControl on battery set to " << cmdRet;
        return std::stof(cmdRet) / 100.f;
    }
    catch (std::invalid_argument &)
    {
        return 1;
    }
}

float GetBacklightLowBatteryMaxBrightness()
{
    auto cmdRet = exec("kreadconfig5 --file " + GetHomeDir() + "/.config/powermanagementprofilesrc --group LowBattery --group BrightnessControl --key value --default 100");
    try
    {
        Log() << "Max BrightnessControl on low battery set to " << cmdRet;
        return std::stof(cmdRet) / 100.f;
    }
    catch (std::invalid_argument &)
    {
        return 1;
    }
}

float GetKeyboardACMaxBrightness()
{
    auto cmdRet = exec("kreadconfig5 --file " + GetHomeDir() + "/.config/powermanagementprofilesrc --group AC --group KeyboardBrightnessControl --key value --default 100");
    try
    {
        Log() << "Max KeyboardBrightnessControl on AC set to " << cmdRet;
        return std::stof(cmdRet) / 100.f;
    }
    catch (std::invalid_argument &)
    {
        return 1;
    }
}

float GetKeyboardBatteryMaxBrightness()
{

    auto cmdRet = exec("kreadconfig5 --file " + GetHomeDir() + "/.config/powermanagementprofilesrc --group Battery --group KeyboardBrightnessControl --key value --default 100");
    try
    {
        Log() << "Max KeyboardBrightnessControl on battery set to " << cmdRet;
        return std::stof(cmdRet) / 100.f;
    }
    catch (std::invalid_argument &)
    {
        return 1;
    }
}

float GetKeyboardLowBatteryMaxBrightness()
{
    auto cmdRet = exec("kreadconfig5 --file " + GetHomeDir() + "/.config/powermanagementprofilesrc --group LowBattery --group KeyboardBrightnessControl --key value --default 100");
    try
    {
        Log() << "Max KeyboardBrightnessControl on low battery set to " << cmdRet;
        return std::stof(cmdRet) / 100.f;
    }
    catch (std::invalid_argument &)
    {
        return 1;
    }
}

float GetMaxKeyboardLed()
{
    auto batteryStatus = GetBatteryStatus();
    float backlightMax = Config::Global().Get("KeyboardLedMax", DefaultKeyboardLedMax);
    Log() << "Battery Status : " << batteryStatus;
    if (batteryStatus == "Discharging\n")
    {
        auto batteryCapacityLevel = GetBatteryCapacityLevel();
        Log() << "Battery Capacity : " << batteryCapacityLevel;
        if (batteryCapacityLevel == "Critical\n" || batteryCapacityLevel == "Low\n")
        {
            return std::min(backlightMax, GetKeyboardLowBatteryMaxBrightness());
        }
        else
        {
            return std::min(backlightMax, GetKeyboardBatteryMaxBrightness());
        }
    }
    else // we're connected to AC
    {
        return std::min(backlightMax, GetKeyboardACMaxBrightness());
    }
}

float GetMaxBacklight()
{
    auto batteryStatus = GetBatteryStatus();
    float backlightMax = Config::Global().Get("BacklightMax", DefaultBacklightMax);
    Log() << "Battery Status : " << batteryStatus;
    if (batteryStatus == "Discharging\n")
    {
        auto batteryCapacityLevel = GetBatteryCapacityLevel();
        Log() << "Battery Capacity : " << batteryCapacityLevel;
        if (batteryCapacityLevel == "Critical\n" || batteryCapacityLevel == "Low\n")
        {
            return std::min(backlightMax, GetBacklightLowBatteryMaxBrightness());
        }
        else
        {
            return std::min(backlightMax, GetBacklightBatteryMaxBrightness());
        }
    }
    else // we're connected to AC
    {
        return std::min(backlightMax, GetBacklightACMaxBrightness());
    }
}

bool BacklightPathValid(const std::filesystem::path &a_Path)
{
    if (std::filesystem::exists(a_Path))
    {
        auto enabledPath = a_Path / "device/enabled";
        auto brightnessPath = a_Path / "brightness";
        auto maxBrightnessPath = a_Path / "max_brightness";
        if (!std::filesystem::exists(enabledPath) || !std::filesystem::exists(brightnessPath) || !std::filesystem::exists(maxBrightnessPath))
            return false;
        std::ifstream file(enabledPath);
        std::string enabled;
        file >> enabled;
        return enabled == "enabled";
    }
    return false;
}

bool SensorPathValid(const std::filesystem::path &a_Path)
{
    auto illuRaw = a_Path / "in_illuminance_raw";
    auto illuScale = a_Path / "in_illuminance_scale";
    auto illuOffset = a_Path / "in_illuminance_offset";
    return std::filesystem::exists(a_Path) && std::filesystem::exists(illuRaw) && std::filesystem::exists(illuScale) && std::filesystem::exists(illuOffset);
}

bool KeyboardLedPathValid(const std::filesystem::path &a_Path)
{
    auto brightnessPath = a_Path / "brightness";
    auto maxBrightnessPath = a_Path / "max_brightness";
    return std::filesystem::exists(brightnessPath) && std::filesystem::exists(maxBrightnessPath);
}

std::string GetBacklightPath()
{
    auto configPath = Config::Global().Get("BacklightPath", std::string(DefaultBacklightPath));
    glob_t globStruct;
    glob(
        configPath.c_str(),
        0,
        nullptr,
        &globStruct);
    for (auto i = 0; i < globStruct.gl_pathc; ++i)
    {
        if (BacklightPathValid(globStruct.gl_pathv[i]))
            return globStruct.gl_pathv[i];
    }
    return "";
}

std::string GetSensorPath()
{
    if (auto configPath = Config::Global().Get("SensorPath", std::string(DefaultSensorPath)); SensorPathValid(configPath))
        return configPath;
    return "";
}

std::string GetKBLedPath()
{
    if (auto configPath = Config::Global().Get("KeyboardLedPath", std::string(DefaultKeyboardLedPath)); KeyboardLedPathValid(configPath))
        return configPath;
    return "";
}

Conf::Conf()
{
    Log() << "Creating Conf\n";
    Update();
}

void Conf::Update()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (delta < confUpdateDelay)
        return;
    auto configPath = std::filesystem::absolute(GetConfigPath());
    Log() << "Config Path : " << configPath << "\n";
    if (std::filesystem::exists(configPath))
        Config::Global().Parse(configPath);
    else
    {
        Error() << "No config file, using default settings and saving them.\n";

        Config::Global().Set("ConfUpdateDelay", DefaultConfUpdateDelay);

        Config::Global().Set("SensorPath", DefaultSensorPath);
        Config::Global().Set("SensorDelay", DefaultSensorDelay);

        Config::Global().Set("BacklightEnabled", DefaultBacklightEnabled);
        Config::Global().Set("BacklightPath", DefaultBacklightPath);
        Config::Global().Set("BacklightDelay", DefaultBacklightDelay);
        Config::Global().Set("BacklightMin", DefaultBacklightMin);
        Config::Global().Set("BacklightMax", DefaultBacklightMax);

        Config::Global().Set("KeyboardLedEnabled", DefaultKeyboardLedEnabled);
        Config::Global().Set("KeyboardLedPath", DefaultKeyboardLedPath);
        Config::Global().Set("KeyboardLedDelay", DefaultKeyboardLedDelay);
        Config::Global().Set("KeyboardLedMin", DefaultKeyboardLedMin);
        Config::Global().Set("KeyboardLedMax", DefaultKeyboardLedMax);

        Config::Global().Set("MaxLuxBreakpoint", DefaultMaxLuxBreakpoint);

        Config::Global().Save(configPath);
        std::filesystem::permissions(
            configPath,
            std::filesystem::perms::all,
            std::filesystem::perm_options::add);
    }

    confUpdateDelay = Config::Global().Get("ConfUpdateDelay", DefaultConfUpdateDelay);

    maxLuxBreakpoint = Config::Global().Get("MaxLuxBreakpoint", DefaultMaxLuxBreakpoint);

    sensorPath = GetSensorPath();
    sensorDelay = Config::Global().Get("SensorDelay", DefaultSensorDelay);
    std::ifstream(sensorPath + "/in_illuminance_scale") >> sensorScale;
    std::ifstream(sensorPath + "/in_illuminance_offset") >> sensorOffset;

    backlightEnabled = Config::Global().Get("BacklightEnabled", DefaultBacklightEnabled);
    backlightPath = GetBacklightPath();
    backlightDelay = Config::Global().Get("BacklightDelay", DefaultBacklightDelay);
    backlightMin = Config::Global().Get("BacklightMin", DefaultBacklightMin);
    backlightMax = GetMaxBacklight();
    std::ifstream(backlightPath + "/max_brightness") >> backlightScale;

    keyboardLedEnabled = Config::Global().Get("BacklightEnabled", DefaultBacklightEnabled);
    keyboardLedPath = GetKBLedPath();
    keyboardLedDelay = Config::Global().Get("KeyboardLedDelay", DefaultKeyboardLedDelay);
    keyboardLedMin = Config::Global().Get("BacklightMin", DefaultBacklightMin);
    keyboardLedMax = GetMaxKeyboardLed();
    std::ifstream(keyboardLedPath + "/max_brightness") >> keyboardLedScale;

    loopDelay = confUpdateDelay;
    loopDelay = std::min(loopDelay, sensorDelay);
    if (backlightEnabled)
        loopDelay = std::min(loopDelay, backlightDelay);
    if (keyboardLedEnabled)
        loopDelay = std::min(loopDelay, keyboardLedDelay);

    lastUpdate = std::chrono::high_resolution_clock::now();
}
#include <Conf.hpp>
#include <Config.hpp>
#include <Tools.hpp>
#include <DBUS.hpp>

#include <array>
#include <iostream>
#include <fstream>
#include <glob.h>
#include <pwd.h>
#include <unistd.h>

constexpr auto DefaultConfUpdateDelay = 5000;

constexpr auto DefaultSensorPath = "/sys/bus/iio/devices/iio:device0";
constexpr auto DefaultSensorDelay = 500;
constexpr auto DefaultSensorSmoothing = 1.5;

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

class ConfException : public std::exception
{
public:
    explicit ConfException(const std::string &a_Message) : _message("Conf exception : " + a_Message) {}
    const char *what() const noexcept override { return _message.c_str(); }
    const std::string _message;
};

std::string GetHomeDir()
{
    static std::string homeDir;
    if (!homeDir.empty())
        return homeDir;
    Log() << "Getting home directory...\n";
    std::string logName = exec("logname");
    Log() << "Logname " << logName << "\n";
    passwd pw;
    passwd *pwResult = nullptr;
    std::array<char, 4096> pwBuffer;
    getpwnam_r(logName.c_str(), &pw, pwBuffer.data(), pwBuffer.size(), &pwResult);
    if (pwResult == nullptr)
    {
        throw ConfException("Cannot retreive user");
    }
    if (pwResult->pw_name == nullptr || std::string(pwResult->pw_name) == "root")
    {
        Log() << "Running as root, figuring out sudoer\n";
        auto sudoer = getenv("SUDO_UID");
        getpwuid_r(std::stoi(sudoer), &pw,
                   pwBuffer.data(), pwBuffer.size(), &pwResult);
    }
    return homeDir = pwResult->pw_dir;
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

PlasmaPowerSettings::PlasmaPowerSettings() {
    auto homeDir = GetHomeDir();
    auto configFileName = exec("kreadconfig5 --file " + homeDir + "/.config/powermanagementprofilesrc --group Migration --key MigratedProfilesToPlasma6 --default powermanagementprofilesrc");
    auto configFilePath = homeDir + "/.config/"+ configFileName;
    Log() << "Config file path : " << configFilePath << std::endl;
    _backlightACCmd = "kreadconfig5 --file " + configFilePath + " --group AC --group Display --key DisplayBrightness --default 100";
    _backlightBATCmd = "kreadconfig5 --file " + configFilePath + " --group Battery --group Display --key DisplayBrightness --default 100";
    _backlightLowBATCmd = "kreadconfig5 --file " + configFilePath + " --group LowBattery --group Display --key DisplayBrightness --default 100";
    _keyboardACCmd = "kreadconfig5 --file " + configFilePath + " --group AC --group Keyboard --key KeyboardBrightness --default 100";
    _keyboardBATCmd = "kreadconfig5 --file " + configFilePath + " --group Battery --group Keyboard --key KeyboardBrightness --default 100";
    _keyboardBATCmd = "kreadconfig5 --file " + configFilePath + " --group LowBattery --group Keyboard --key KeyboardBrightness --default 100";
}

bool PlasmaPowerSettings::_LowBattery() const
{
    auto batteryCapacityLevel = GetBatteryCapacityLevel();
    Log() << "Battery Capacity : " << batteryCapacityLevel << std::endl;
    return (batteryCapacityLevel == "Critical\n" || batteryCapacityLevel == "Low\n");
}

bool PlasmaPowerSettings::_OnBattery() const {
    auto batteryStatus = GetBatteryStatus();
    Log() << "Battery Status : " << batteryStatus << std::endl;
    return batteryStatus == "Discharging";
}

float PlasmaPowerSettings::GetDisplayMaxBrightness() const {
    float backlightMax = Config::Global().Get("BacklightMax", DefaultKeyboardLedMax);
    float confMaxBacklight = 1;
    if (_OnBattery())
    {
        confMaxBacklight = _LowBattery() ? GetDisplayLowBATMaxBrightness() : GetDisplayBATMaxBrightness();
    }
    else // we're connected to AC
    {
        confMaxBacklight = GetDisplayACMaxBrightness();
    }
    return std::min(backlightMax, confMaxBacklight);
}

float PlasmaPowerSettings::GetKeyboardMaxBrightness() const {
    float backlightMax = Config::Global().Get("KeyboardLedMax", DefaultKeyboardLedMax);
    float confMaxBacklight = 1;
    if (_OnBattery())
    {
        confMaxBacklight = _LowBattery() ? GetKeyboardLowBATMaxBrightness() : GetKeyboardBATMaxBrightness();
    }
    else // we're connected to AC
    {
        confMaxBacklight = GetKeyboardACMaxBrightness();
    }
    return std::min(backlightMax, GetKeyboardACMaxBrightness());
}

float PlasmaPowerSettings::GetDisplayACMaxBrightness() const {
    return _GetBrightness("ACMaxDisplayBrightness", _backlightACCmd);
}

float PlasmaPowerSettings::GetDisplayBATMaxBrightness() const {
    return _GetBrightness("BATMaxDisplayBrightness", _backlightBATCmd);
}

float PlasmaPowerSettings::GetDisplayLowBATMaxBrightness() const {
    return _GetBrightness("LowBATMaxDisplayBrightness", _backlightLowBATCmd);
}

float PlasmaPowerSettings::GetKeyboardACMaxBrightness() const {
    return _GetBrightness("ACMaxKeyboardBrightness", _keyboardACCmd);
}

float PlasmaPowerSettings::GetKeyboardBATMaxBrightness() const {
    return _GetBrightness("BATMaxKeyboardBrightness", _keyboardBATCmd);
}

float PlasmaPowerSettings::GetKeyboardLowBATMaxBrightness() const {
    return _GetBrightness("LowBATMaxKeyboardBrightness", _keyboardLowBATCmd);
}

float PlasmaPowerSettings::_GetBrightness(const std::string& a_ValName, const std::string& a_Cmd) const {
        auto cmdRet = exec(a_Cmd);
        try
        {
            Log() << a_ValName << " : " << cmdRet  << std::endl;
            return std::stof(cmdRet) / 100.f;
        }
        catch (std::invalid_argument &)
        {
            return 1;
        }
    }

bool SensorPathValid(const std::filesystem::path &a_Path)
{
    auto illuRaw = a_Path / "in_illuminance_raw";
    auto illuScale = a_Path / "in_illuminance_scale";
    auto illuOffset = a_Path / "in_illuminance_offset";
    return std::filesystem::exists(a_Path) && std::filesystem::exists(illuRaw) && std::filesystem::exists(illuScale) && std::filesystem::exists(illuOffset);
}

std::string GetSensorPath()
{
    if (auto configPath = Config::Global().Get("SensorPath", std::string(DefaultSensorPath)); SensorPathValid(configPath))
        return configPath;
    return "";
}

Conf::Conf(DBUSConnection &a_DBusConnection) : dBusConnection(a_DBusConnection)
{
    Log() << "Creating Conf\n";
    Update();
    {
        DBUSMethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/BrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.BrightnessControl",
                                  "brightnessMax");
        DBUSReply reply(dBusConnection.Send(methodCall));
        reply.GetArgs(DBUS_TYPE_INT32, &backlightScale);
    }
}

void Conf::Update()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    if (delta < confUpdateDelay)
        return;
    auto configPath = std::filesystem::absolute(GetConfigPath());
    Log() << "Config Path : " << configPath << "\n";
    bool confFileExists = std::filesystem::exists(configPath);
    if (confFileExists)
        Config::Global().Parse(configPath);
    else
    {
        Error() << "No config file, using default settings and saving them.\n";
    }

    confUpdateDelay = Config::Global().Get("ConfUpdateDelay", DefaultConfUpdateDelay);

    maxLuxBreakpoint = Config::Global().Get("MaxLuxBreakpoint", DefaultMaxLuxBreakpoint);

    sensorPath = GetSensorPath();
    sensorDelay = Config::Global().Get("SensorDelay", DefaultSensorDelay);
    sensorSmoothing = Config::Global().Get("SensorSmoothing", DefaultSensorSmoothing);
    std::ifstream(sensorPath + "/in_illuminance_scale") >> sensorScale;
    std::ifstream(sensorPath + "/in_illuminance_offset") >> sensorOffset;

    backlightEnabled = Config::Global().Get("BacklightEnabled", DefaultBacklightEnabled);
    backlightDelay = Config::Global().Get("BacklightDelay", DefaultBacklightDelay);
    backlightMin = Config::Global().Get("BacklightMin", DefaultBacklightMin);
    backlightMax = powerSettings.GetDisplayMaxBrightness();
    {
        DBUSMethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/BrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.BrightnessControl",
                                  "brightnessMax");
        DBUSReply reply(dBusConnection.Send(methodCall));
        reply.GetArgs(DBUS_TYPE_INT32, &backlightScale);
    }

    keyboardLedEnabled = Config::Global().Get("KeyboardLedEnabled", DefaultKeyboardLedEnabled);
    keyboardLedDelay = Config::Global().Get("KeyboardLedDelay", DefaultKeyboardLedDelay);
    keyboardLedMin = Config::Global().Get("KeyboardLedMin", DefaultKeyboardLedMin);
    keyboardLedMax = powerSettings.GetKeyboardMaxBrightness();
    {
        DBUSMethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/KeyboardBrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.KeyboardBrightnessControl",
                                  "keyboardBrightnessMax");
        DBUSReply reply(dBusConnection.Send(methodCall));
        reply.GetArgs(DBUS_TYPE_INT32, &keyboardLedScale);
    }

    loopDelay = confUpdateDelay;
    loopDelay = std::min(loopDelay, sensorDelay);
    if (backlightEnabled)
        loopDelay = std::min(loopDelay, backlightDelay);
    if (keyboardLedEnabled)
        loopDelay = std::min(loopDelay, keyboardLedDelay);

    if (!confFileExists)
    {
        Log() << "Saving config file\n";
        Config::Global().Save(configPath);
        std::filesystem::permissions(
            configPath,
            std::filesystem::perms::all,
            std::filesystem::perm_options::add);
    }

    lastUpdate = std::chrono::high_resolution_clock::now();
}
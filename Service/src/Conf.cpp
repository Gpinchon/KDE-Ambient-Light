#include <Conf.hpp>
#include <ConfKeys.hpp>
#include <Config.hpp>
#include <Tools.hpp>
#include <DBUS.hpp>

#include <array>
#include <iostream>
#include <fstream>
#include <glob.h>
#include <pwd.h>
#include <unistd.h>

enum class BatteryLevel {
    Unknown,
    None = 1,
    Low = 3,
    Critical = 4,
    Normal = 6,
    High = 7,
    Full = 8,
    MaxValue
};

enum class BatteryState {
    Unknown,
    Charging,
    Discharging,
    Empty,
    FullyCharged,
    PendingCharge,
    PendingDischarge,
    MaxValue
};

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

bool OnLowBattery()
{
    DBUS::Connection dBusConnection(DBUS_BUS_SYSTEM);
    DBUS::MethodCall methodCall("org.freedesktop.UPower",
                              "/org/freedesktop/UPower/devices/battery_BAT0",
                              DBUS_INTERFACE_PROPERTIES,
                              "Get");
    auto iFace = "org.freedesktop.UPower.Device";
    auto property = "BatteryLevel";
    methodCall.SetArgs(DBUS_TYPE_STRING, &iFace,
                       DBUS_TYPE_STRING, &property);
    DBUS::Reply reply(dBusConnection.Send(methodCall));
    auto batteryLevel = BatteryLevel(std::any_cast<uint32_t>(reply.GetArgs().at(0)));
    Log() << "Battery level " << int(batteryLevel) << std::endl;
    return batteryLevel == BatteryLevel::Low || batteryLevel== BatteryLevel::Critical;
}

bool OnBattery() {
    DBUS::Connection dBusConnection(DBUS_BUS_SYSTEM);
    DBUS::MethodCall methodCall("org.freedesktop.UPower",
                              "/org/freedesktop/UPower",
                              DBUS_INTERFACE_PROPERTIES,
                              "Get");
    auto iFace = "org.freedesktop.UPower";
    auto property = "OnBattery";
    methodCall.SetArgs(DBUS_TYPE_STRING, &iFace,
                       DBUS_TYPE_STRING, &property);
    DBUS::Reply reply(dBusConnection.Send(methodCall));
    auto onBattery = std::any_cast<bool>(reply.GetArgs().at(0));
    Log() << "On battery " << onBattery << std::endl;
    return onBattery;
}

bool SensorPathValid(const std::filesystem::path &a_Path)
{
    auto illuRaw = a_Path / "in_illuminance_raw";
    auto illuScale = a_Path / "in_illuminance_scale";
    auto illuOffset = a_Path / "in_illuminance_offset";
    return std::filesystem::exists(a_Path) && std::filesystem::exists(illuRaw) && std::filesystem::exists(illuScale) && std::filesystem::exists(illuOffset);
}

Conf::Conf(DBUS::Connection &a_DBusConnection) : dBusConnection(a_DBusConnection)
{
    Log() << "Creating Conf\n";
    Update();
    {
        DBUS::MethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/BrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.BrightnessControl",
                                  "brightnessMax");
        DBUS::Reply reply(dBusConnection.Send(methodCall));
        Set("BacklightScale",std::any_cast<int32_t>(reply.GetArgs().front()));
    }
}

void Conf::Update()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto delta = std::chrono::duration<double, std::milli>(now - lastUpdate).count();
    auto confUpdateDelay = Get(ConfUpdateDelay, DefaultConfUpdateDelay);
    if (delta < confUpdateDelay)
        return;
    auto configPath = std::filesystem::absolute(GetConfigPath());
    Log() << "Config Path : " << configPath << "\n";
    bool confFileExists = std::filesystem::exists(configPath);
    if (confFileExists)
        Parse(configPath);
    else
    {
        Error() << "No config file, using default settings and saving them.\n";
    }
    Get(MaxLuxBreakpoint, DefaultMaxLuxBreakpoint);

    auto sensorPath = Get(SensorPath, std::string(DefaultSensorPath));
    if (!SensorPathValid(sensorPath))
        throw std::runtime_error("Invalid sensor path");
    auto sensorDelay = Get(SensorDelay, DefaultSensorDelay);
    Get(SensorSmoothing, DefaultSensorSmoothing);
    std::ifstream(sensorPath + "/in_illuminance_scale") >> sensorScale;
    std::ifstream(sensorPath + "/in_illuminance_offset") >> sensorOffset;

    auto backlightEnabled = Get(BacklightEnabled, DefaultBacklightEnabled) != 0;
    auto backlightDelay = Get(BacklightDelay, DefaultBacklightDelay);
    Get(BacklightMin, DefaultBacklightMin);
    auto backlightMaxAC = Get(BacklightMaxAC, DefaultBacklightMaxAC);
    auto backlightMaxBAT = Get(BacklightMaxBAT, DefaultBacklightMaxBAT);
    auto backlightMaxBATLow = Get(BacklightMaxBATLow, DefaultBacklightMaxBATLow);
    {
        DBUS::MethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/BrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.BrightnessControl",
                                  "brightnessMax");
        DBUS::Reply reply(dBusConnection.Send(methodCall));
        backlightScale = std::any_cast<int32_t>(reply.GetArgs().front());
    }

    auto keyboardLedEnabled = Get(KeyboardLedEnabled, DefaultKeyboardLedEnabled) != 0;
    auto keyboardLedDelay = Get(KeyboardLedDelay, DefaultKeyboardLedDelay);
    Get(KeyboardLedMin, DefaultKeyboardLedMin);
    auto keyboardLedMaxAC = Get(KeyboardLedMaxAC, DefaultKeyboardLedMaxAC);
    auto keyboardLedMaxBAT = Get(KeyboardLedMaxBAT, DefaultKeyboardLedMaxBAT);
    auto keyboardLedMaxBATLow = Get(KeyboardLedMaxBATLow, DefaultKeyboardLedMaxBATLow);
    {
        DBUS::MethodCall methodCall("org.kde.Solid.PowerManagement",
                                  "/org/kde/Solid/PowerManagement/Actions/KeyboardBrightnessControl",
                                  "org.kde.Solid.PowerManagement.Actions.KeyboardBrightnessControl",
                                  "keyboardBrightnessMax");
        DBUS::Reply reply(dBusConnection.Send(methodCall));
        keyboardLedScale = std::any_cast<int32_t>(reply.GetArgs().front());
    }

    if (OnBattery()) {
        if (OnLowBattery()) {
            backlightMax = backlightMaxBATLow;
            keyboardLedMax = keyboardLedMaxBATLow;
        }
        else {
            backlightMax = backlightMaxBAT;
            keyboardLedMax = keyboardLedMaxBAT;
        }
    }
    else { // we're connected to AC
        backlightMax = backlightMaxAC;
        keyboardLedMax = keyboardLedMaxAC;
    }

    loopDelay = int(confUpdateDelay);
    loopDelay = std::min(loopDelay, int(sensorDelay));
    if (backlightEnabled)
        loopDelay = std::min(loopDelay, int(backlightDelay));
    if (keyboardLedEnabled)
        loopDelay = std::min(loopDelay, int(keyboardLedDelay));
    if (loopDelay == 0) loopDelay = 5000; //Avoid looping infinitely fast...

    if (!confFileExists)
    {
        Log() << "Saving config file\n";
        Save(configPath);
        std::filesystem::permissions(
            configPath,
            std::filesystem::perms::all,
            std::filesystem::perm_options::add);
    }

    lastUpdate = std::chrono::high_resolution_clock::now();
}
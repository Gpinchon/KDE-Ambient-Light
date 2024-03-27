#pragma once

constexpr auto SensorPath = "SensorPath";
constexpr auto SensorDelay = "SensorDelay";
constexpr auto SensorSmoothing = "SensorSmoothing";

constexpr auto BacklightEnabled = "BacklightEnabled";
constexpr auto BacklightDelay = "BacklightDelay";
constexpr auto BacklightMin = "BacklightMin";
constexpr auto BacklightMaxBAT = "BacklightMaxBAT";
constexpr auto BacklightMaxBATLow = "BacklightMaxBATLow";
constexpr auto BacklightMaxAC = "BacklightMaxAC";

constexpr auto KeyboardLedEnabled = "KeyboardLedEnabled";
constexpr auto KeyboardLedDelay = "KeyboardLedDelay";
constexpr auto KeyboardLedMin = "KeyboardLedMin";
constexpr auto KeyboardLedMaxBAT = "KeyboardLedMaxBAT";
constexpr auto KeyboardLedMaxBATLow = "KeyboardLedMaxBATLow";
constexpr auto KeyboardLedMaxAC = "KeyboardLedMaxAC";

constexpr auto MaxLuxBreakpoint = "MaxLuxBreakpoint";

constexpr auto DefaultSensorPath = "/sys/bus/iio/devices/iio:device0";
constexpr auto DefaultSensorDelay = 500;
constexpr auto DefaultSensorSmoothing = 1.5;

constexpr auto DefaultBacklightEnabled = true;
constexpr auto DefaultBacklightDelay = 32; // default update frequency is 30 times/sec
constexpr auto DefaultBacklightMin = 0;
constexpr auto DefaultBacklightMaxBAT = 1;
constexpr auto DefaultBacklightMaxBATLow = 1;
constexpr auto DefaultBacklightMaxAC = 1;

constexpr auto DefaultKeyboardLedEnabled = true;
constexpr auto DefaultKeyboardLedDelay = 1000;
constexpr auto DefaultKeyboardLedMin = 0;
constexpr auto DefaultKeyboardLedMaxBAT = 1;
constexpr auto DefaultKeyboardLedMaxBATLow = 1;
constexpr auto DefaultKeyboardLedMaxAC = 1;

constexpr auto DefaultMaxLuxBreakpoint = 1254;

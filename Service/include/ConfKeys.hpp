#pragma once

constexpr auto ConfUpdateDelay = "ConfUpdateDelay";

constexpr auto SensorPath = "SensorPath";
constexpr auto SensorDelay = "SensorDelay";
constexpr auto SensorSmoothing = "SensorSmoothing";
constexpr auto SensorScale = "SensorScale";
constexpr auto SensorOffset = "SensorOffset";

constexpr auto BacklightEnabled = "BacklightEnabled";
constexpr auto BacklightScale = "BacklightScale";
constexpr auto BacklightDelay = "BacklightDelay";
constexpr auto BacklightMin = "BacklightMin";
constexpr auto BacklightMax = "BacklightMax";
constexpr auto BacklightMaxBAT = "BacklightMaxBAT";
constexpr auto BacklightMaxBATLow = "BacklightMaxBATLow";
constexpr auto BacklightMaxAC = "BacklightMaxAC";

constexpr auto KeyboardLedEnabled = "KeyboardLedEnabled";
constexpr auto KeyboardLedScale = "KeyboardLedScale";
constexpr auto KeyboardLedDelay = "KeyboardLedDelay";
constexpr auto KeyboardLedMin = "KeyboardLedMin";
constexpr auto KeyboardLedMax = "KeyboardLedMax";
constexpr auto KeyboardLedMaxBAT = "KeyboardLedMaxBAT";
constexpr auto KeyboardLedMaxBATLow = "KeyboardLedMaxBATLow";
constexpr auto KeyboardLedMaxAC = "KeyboardLedMaxAC";

constexpr auto MaxLuxBreakpoint = "MaxLuxBreakpoint";

constexpr auto DefaultConfUpdateDelay = 5000;

constexpr auto DefaultSensorPath = "/sys/bus/iio/devices/iio:device0";
constexpr auto DefaultSensorDelay = 500;
constexpr auto DefaultSensorSmoothing = 1.5;
constexpr auto DefaultSensorScale = 1;
constexpr auto DefaultSensorOffset = 0;

constexpr auto DefaultBacklightEnabled = true;
constexpr auto DefaultBacklightScale = 1;
constexpr auto DefaultBacklightDelay = 32; // default update frequency is 30 times/sec
constexpr auto DefaultBacklightMin = 0;
constexpr auto DefaultBacklightMax = 1;
constexpr auto DefaultBacklightMaxBAT = 1;
constexpr auto DefaultBacklightMaxBATLow = 1;
constexpr auto DefaultBacklightMaxAC = 1;

constexpr auto DefaultKeyboardLedEnabled = true;
constexpr auto DefaultKeyboardLedScale = 1;
constexpr auto DefaultKeyboardLedDelay = 1000;
constexpr auto DefaultKeyboardLedMin = 0;
constexpr auto DefaultKeyboardLedMax = 1;
constexpr auto DefaultKeyboardLedMaxBAT = 1;
constexpr auto DefaultKeyboardLedMaxBATLow = 1;
constexpr auto DefaultKeyboardLedMaxAC = 1;

constexpr auto DefaultMaxLuxBreakpoint = 1254;

#pragma once

constexpr auto SensorPath      = "SensorPath";
constexpr auto SensorDelay     = "SensorDelay";
constexpr auto SensorSmoothing = "SensorSmoothing";

constexpr auto BacklightEnabled   = "BacklightEnabled";
constexpr auto BacklightDelay     = "BacklightDelay";
constexpr auto BacklightMinAC     = "BacklightMinAC";
constexpr auto BacklightMinBAT    = "BacklightMinBAT";
constexpr auto BacklightMinBATLow = "BacklightMinBATLow";
constexpr auto BacklightMaxAC     = "BacklightMaxAC";
constexpr auto BacklightMaxBAT    = "BacklightMaxBAT";
constexpr auto BacklightMaxBATLow = "BacklightMaxBATLow";

constexpr auto KeyboardLedEnabled   = "KeyboardLedEnabled";
constexpr auto KeyboardLedDelay     = "KeyboardLedDelay";
constexpr auto KeyboardLedMinAC     = "KeyboardLedMinAC";
constexpr auto KeyboardLedMinBAT    = "KeyboardLedMinBAT";
constexpr auto KeyboardLedMinBATLow = "KeyboardLedMinBATLow";
constexpr auto KeyboardLedMaxAC     = "KeyboardLedMaxAC";
constexpr auto KeyboardLedMaxBAT    = "KeyboardLedMaxBAT";
constexpr auto KeyboardLedMaxBATLow = "KeyboardLedMaxBATLow";

constexpr auto MaxLuxBreakpoint = "MaxLuxBreakpoint";

constexpr auto DefaultSensorPath      = "/sys/bus/iio/devices/iio:device0";
constexpr auto DefaultSensorDelay     = 500;
constexpr auto DefaultSensorSmoothing = 1.5;

constexpr auto DefaultBacklightDelay         = 32; // default update frequency is 30 times/sec
constexpr auto DefaultBacklightEnabledAC     = true;
constexpr auto DefaultBacklightEnabledBAT    = true;
constexpr auto DefaultBacklightEnabledBATLow = true;
constexpr auto DefaultBacklightMinAC         = 0;
constexpr auto DefaultBacklightMinBAT        = 0;
constexpr auto DefaultBacklightMinBATLow     = 0;
constexpr auto DefaultBacklightMaxAC         = 1;
constexpr auto DefaultBacklightMaxBAT        = 1;
constexpr auto DefaultBacklightMaxBATLow     = 1;

constexpr auto DefaultKeyboardLedDelay         = 1000;
constexpr auto DefaultKeyboardLedEnabledAC     = true;
constexpr auto DefaultKeyboardLedEnabledBAT    = true;
constexpr auto DefaultKeyboardLedEnabledBATLow = true;
constexpr auto DefaultKeyboardLedMinAC         = 0;
constexpr auto DefaultKeyboardLedMinBAT        = 0;
constexpr auto DefaultKeyboardLedMinBATLow     = 0;
constexpr auto DefaultKeyboardLedMaxAC         = 1;
constexpr auto DefaultKeyboardLedMaxBAT        = 1;
constexpr auto DefaultKeyboardLedMaxBATLow     = 1;

constexpr auto DefaultMaxLuxBreakpoint = 1254;

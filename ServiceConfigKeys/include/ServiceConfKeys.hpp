#pragma once

constexpr auto ConfigUpdateDelay = "ConfigUpdateDelay";

constexpr auto SensorPath      = "SensorPath";
constexpr auto SensorDelay     = "SensorDelay";
constexpr auto SensorSmoothing = "SensorSmoothing";

constexpr auto BacklightPath           = "BacklightPath";
constexpr auto BacklightDelay          = "BacklightDelay";
constexpr auto BacklightTransitionTime = "BacklightTransitionTime";
constexpr auto BacklightEnabledAC      = "BacklightEnabledAC";
constexpr auto BacklightEnabledBAT     = "BacklightEnabledBAT";
constexpr auto BacklightEnabledBATLow  = "BacklightEnabledBATLow";
constexpr auto BacklightMinAC          = "BacklightMinAC";
constexpr auto BacklightMinBAT         = "BacklightMinBAT";
constexpr auto BacklightMinBATLow      = "BacklightMinBATLow";
constexpr auto BacklightMaxAC          = "BacklightMaxAC";
constexpr auto BacklightMaxBAT         = "BacklightMaxBAT";
constexpr auto BacklightMaxBATLow      = "BacklightMaxBATLow";

constexpr auto KeyboardLedPath          = "KeyboardLedPath";
constexpr auto KeyboardLedDelay         = "KeyboardLedDelay";
constexpr auto KeyboardLedEnabledAC     = "KeyboardLedEnabledAC";
constexpr auto KeyboardLedEnabledBAT    = "KeyboardLedEnabledBAT";
constexpr auto KeyboardLedEnabledBATLow = "KeyboardLedEnabledBATLow";
constexpr auto KeyboardLedMinAC         = "KeyboardLedMinAC";
constexpr auto KeyboardLedMinBAT        = "KeyboardLedMinBAT";
constexpr auto KeyboardLedMinBATLow     = "KeyboardLedMinBATLow";
constexpr auto KeyboardLedMaxAC         = "KeyboardLedMaxAC";
constexpr auto KeyboardLedMaxBAT        = "KeyboardLedMaxBAT";
constexpr auto KeyboardLedMaxBATLow     = "KeyboardLedMaxBATLow";

constexpr auto MaxLuxBreakpoint = "MaxLuxBreakpoint";

constexpr auto DefaultConfigUpdateDelay = 5000;

constexpr auto DefaultSensorPath      = "/sys/bus/iio/devices/iio:device0";
constexpr auto DefaultSensorDelay     = 500;
constexpr auto DefaultSensorSmoothing = 1.5;

constexpr auto DefaultBacklightPath           = "/sys/class/backlight/amdgpu_bl1/";
constexpr auto DefaultBacklightDelay          = 32; // default update frequency is 30 times/sec
constexpr auto DefaultBacklightTransitionTime = 5000.0;
constexpr auto DefaultBacklightEnabledAC      = true;
constexpr auto DefaultBacklightEnabledBAT     = true;
constexpr auto DefaultBacklightEnabledBATLow  = true;
constexpr auto DefaultBacklightMinAC          = 0;
constexpr auto DefaultBacklightMinBAT         = 0;
constexpr auto DefaultBacklightMinBATLow      = 0;
constexpr auto DefaultBacklightMaxAC          = 1;
constexpr auto DefaultBacklightMaxBAT         = 1;
constexpr auto DefaultBacklightMaxBATLow      = 1;

constexpr auto DefaultKeyboardLedPath          = "/sys/class/leds/platform::kbd_backlight";
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

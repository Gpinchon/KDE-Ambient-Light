# Description
This service attempts to bring automatic backlight for KDE.
It was originaly developped on a Yoga Pro 7 running OpenSUSE Tumbleweed with Plasma.
It might not work on other laptops depending on your configuration. Please profide feedback if it doesn't.

The config file used for this service is located in `~/.config/ambient-light`
If no config file is found the service generates it automatically using the default values.

# Default config values
```cpp
//Delay between config lookups, every updates in the conf file will be reflected without restarting the service
DefaultConfUpdateDelay = 5000

//Light sensor path, might vary depending on the configuration
DefaultSensorPath = /sys/bus/iio/devices/iio:device0
//Light sensor lookups delay in milliseconds, default 2 times/sec
DefaultSensorDelay = 500
//Influences the smoothing of sensor lumen to brightness function
//function is pow(log(x), SensorSmoothing), it's then normalized
DefaultSensorSmoothing = 1.5

//Enable/Disables automatic backlight
DefaultBacklightEnabled = true
//Backlight update delay in milliseconds, default update frequency is 30 times/sec
DefaultBacklightDelay = 32
//Backlight minimum value
DefaultBacklightMin = 0
//Backlight maximum value on battery
DefaultBacklightMaxBAT = 1
//Backlight maximum value on low battery
DefaultBacklightMaxBATLow = 1
//Backlight maximum value on AC
DefaultBacklightMaxAC = 1

//Enable/Disables automatic keyboard LED
DefaultKeyboardLedEnabled = 1
//Keyboard LED update delay in milliseconds, default update frequency is 1 times/sec
DefaultKeyboardLedDelay = 1000
//Keyboard LED minimum value
DefaultKeyboardLedMin = 0
//Keyboard LED maximum value on battery
DefaultKeyboardLedMaxBAT = 1
//Keyboard LED maximum value on low battery
DefaultKeyboardLedMaxBATLow = 1
//Keyboard LED maximum value on AC
DefaultKeyboardLedMaxAC = 1

//The lux (lumen) breakpoint. If the sensor detects this or a higher value it will set the Backlight to BacklightMax
DefaultMaxLuxBreakpoint = 1254
```

# Building
In order to build from source and install the service, simply run ./build_and_install.sh
## Dependencies
cmake, libc-dev, libcxx-dev, libdbus-1-dev, PkgConfig, FindPkgConfig.cmake

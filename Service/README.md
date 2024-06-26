# ambient-light service
This is the service source code

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
//Backlight transition time in milliseconds, default is 5 seconds
DefaultBacklightTransitionTime = 5000
//Backlight minimum value on AC
DefaultBacklightMinAC = 0
//Backlight minimum value on battery
DefaultBacklightMinBAT = 0
//Backlight minimum value on low battery
DefaultBacklightMinBATLow = 0
//Backlight maximum value on AC
DefaultBacklightMaxAC = 1
//Backlight maximum value on battery
DefaultBacklightMaxBAT = 1
//Backlight maximum value on low battery
DefaultBacklightMaxBATLow = 1


//Enable/Disables automatic keyboard LED
DefaultKeyboardLedEnabled = true
//Keyboard LED update delay in milliseconds, default update frequency is 1 times/sec
DefaultKeyboardLedDelay = 1000
//Keyboard LED minimum value on AC
DefaultKeyboardLedMinAC = 0
//Keyboard LED minimum value on battery
DefaultKeyboardLedMinBAT = 0
//Keyboard LED minimum value on low battery
DefaultKeyboardLedMinBATLow = 0
//Keyboard LED maximum value on battery
DefaultKeyboardLedMaxBAT = 1
//Keyboard LED maximum value on low battery
DefaultKeyboardLedMaxBATLow = 1
//Keyboard LED maximum value on AC
DefaultKeyboardLedMaxAC = 1

//The lux (lumen) breakpoint. If the sensor detects this or a higher value it will set the Backlight to BacklightMax
DefaultMaxLuxBreakpoint = 1254
```
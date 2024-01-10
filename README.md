This service attempts to bring automatic backlight for KDE.
It was originaly developped on a Yoga Pro 7 running OpenSUSE Tumbleweed with Plasma.
It might not work on other laptops depending on your configuration. Please profide feedback if it doesn't.

The max values for automatic brightness can be set using System Plasma's "Settings/Power Management". They're read from `~/.config/powermanagementprofilesrc`

The config file used for this service is located in `~/.config/ambient-light`
If no config file is found the service generates it automatically using the default values.

The default values for the config file are the following :
```
//Delay between config lookups, every updates in the conf file will be reflected withoug restarting the service
ConfUpdateDelay = 5000

//Light sensor path, might vary depending on the configuration
SensorPath      = /sys/bus/iio/devices/iio:device0
//Light sensor lookups delay in milliseconds
SensorDelay     = 500

//The backlight path, the wildcard can be replaced with the proper folder, otherwise the service will scan for proper folder
BacklightPath   = /sys/class/backlight/*
//Backlight update delay in milliseconds
BacklightDelay  = 32
//Backlight minimum value
BacklightMin    = 0.0
//Backlight maximum value, can be overloaded by System Settings/Power Management values
BacklightMax    = 1.0



//Keyboard LED path, might vary depending on the configuration
KeyboardLedPath  = /sys/class/leds/platform::kbd_backlight/
//Leyboard LED update delay in milliseconds
KeyboardLedDelay = 1000
//Keyboard LED minimum value
KeyboardLedMin   = 0
//Keyboard LED maximum value, can be overloaded by System Settings/Power Management values
KeyboardLedMax   = 1

//The lux (lumen) breakpoint. If the sensor detects this or a higher value it will set the Backlight to BacklightMax
MaxLuxBreakpoint = 1254.0
```

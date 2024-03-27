# Description
This service attempts to bring automatic backlight for KDE.
It was originaly developped on a Yoga Pro 7 running OpenSUSE Tumbleweed with Plasma.
It might not work on other laptops depending on your configuration. Please provide feedback if it doesn't.

# Building
In order to build from source and install the service, simply run ./build_and_install.sh
## Dependencies
### Service
cmake, libc-dev, libcxx-dev, libdbus-1-dev, PkgConfig, FindPkgConfig.cmake
### KCM
cmake, libc-dev, libcxx-dev, Qt6


cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0074 NEW)

include(FetchContent)

find_package(PkgConfig REQUIRED) # Include functions provided by PkgConfig module.

pkg_check_modules(DBUS REQUIRED dbus-1) # This calls pkgconfig with appropriate arguments
# Use results of pkg_check_modules() call.

FetchContent_Declare(
    CONFIG
    GIT_REPOSITORY  https://github.com/Gpinchon/Config.git
    GIT_TAG         origin/main
)
FetchContent_MakeAvailable(CONFIG)

set(SERVICE_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Backlight.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Conf.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Tools.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/DBUS.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyboardLed.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sensor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Tools.cpp)

set(SERVICE_INCLUDE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Backlight.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Conf.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/DBUS.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/KeyboardLed.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Sensor.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Tools.hpp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

add_executable(ambient-light
  ${SERVICE_SRC} ${SERVICE_INCLUDE})

target_include_directories(ambient-light PUBLIC
  ${DBUS_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_directories(ambient-light PUBLIC
  ${DBUS_LIBRARY_DIRS})
target_link_libraries(ambient-light
  ${DBUS_LIBRARIES}
  Config)

add_custom_command(
  TARGET ambient-light POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/*
          ${CMAKE_CURRENT_BINARY_DIR}/)
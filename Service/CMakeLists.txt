cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0074 NEW)

include(FetchContent)

FetchContent_Declare(
    CONFIG
    GIT_REPOSITORY  https://github.com/Gpinchon/Config.git
    GIT_TAG         origin/main
)
FetchContent_MakeAvailable(CONFIG)

set(SERVICE_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Conf.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Tools.cpp)
set(SERVICE_INCLUDE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Conf.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Tools.hpp)

add_executable(KDE-Ambient-Light-Service
  ${SERVICE_SRC} ${SERVICE_INCLUDE})

target_include_directories(KDE-Ambient-Light-Service PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(KDE-Ambient-Light-Service
  Config)
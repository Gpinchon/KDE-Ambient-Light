cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0074 NEW)

project(KDE-Ambient-Light LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ServiceConfigKeys)
add_subdirectory(Service)

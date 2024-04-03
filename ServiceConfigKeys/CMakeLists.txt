cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0074 NEW)

add_library(ServiceConfigKeys INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ServiceConfKeys.hpp)
set_target_properties(ServiceConfigKeys PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(ServiceConfigKeys INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/)
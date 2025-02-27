cmake_minimum_required(VERSION 3.1.0...3.27.0)

project(RTWeekend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENDIONS OFF)

file(GLOB_RECURSE rtw_header_files CONFIGURE_DEPENDS "rt-weekend/*.h")

add_executable(RT_weekend rt-weekend/main.cc ${rtw_header_files})

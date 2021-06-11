cmake_minimum_required(VERSION 3.0)
project(EMP-Lib)

set(CMAKE_CXX_STANDARD 11)

# To simulate the libray use as submodule, teh library are include using
# Symbolic-Link, in real case, copy or clone the EMPlibs Directory from source
add_subdirectory(EMPlibs)
add_executable(demoMain main.cpp)
target_link_libraries(demoMain PUBLIC EMPlibs)
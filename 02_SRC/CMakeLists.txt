# Cmake only to test and compile the library.
# To find test please go to "03_Test" Folder.
cmake_minimum_required(VERSION 3.0)
project(EMP-Lib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(EMPlibs)
add_executable(demoMain main.cpp)
target_link_libraries(demoMain PUBLIC EMPlibs)

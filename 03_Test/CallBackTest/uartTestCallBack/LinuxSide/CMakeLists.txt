# Cmake only to test and compile the library.
# To find test please go to "03_Test" Folder.
cmake_minimum_required(VERSION 3.0)
project(EMP-Lib_UART_DEMO-callBack)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(EMPlibs)
add_executable(uartDemoMain main.cpp)
target_link_libraries(uartDemoMain PUBLIC EMPlibs)

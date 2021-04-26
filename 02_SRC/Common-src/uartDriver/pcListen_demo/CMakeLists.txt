# Demo listen of the board
add_executable(pcListenDemo main.cpp)

#Add used library
target_link_libraries(pcListenDemo PUBLIC dataTransfertPrimitive)
target_link_libraries(pcListenDemo PUBLIC uartDriver)
message("\t[pcListenDemo] Demo executable add")

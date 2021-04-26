#Add src file
message("--> Common-src CmakeLists.txt start include Common-src Lib:")
add_subdirectory(timeOp)
add_subdirectory(dataTransfertPrimitive)
add_subdirectory(circularBuffer)
add_subdirectory(uartDriver)
add_subdirectory(utilityScorbot)

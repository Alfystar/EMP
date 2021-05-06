get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            # Core library file
            conf.h macro.h
            MP.h MP.cpp
            cobs-c/cobs.h cobs-c/cobs.c
            crc8/crc8.h crc8/crc8_inc.h crc8/crc8.cpp
            )

    target_include_directories(${libName} PUBLIC "${CMAKE_INCLUDE_CURRENT_DIR}")

    add_subdirectory(circularBuffer)
    target_link_libraries(${libName} PUBLIC circularBuffer)

    message("[${libName}] subsystem included")
endif ()

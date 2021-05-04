get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            CircularBuffer.h CircularBuffer.cpp )
    target_include_directories(${libName} PUBLIC "${CMAKE_INCLUDE_CURRENT_DIR}")
    if (${UNIX})
        target_compile_definitions(${libName} PUBLIC LinuxSide=1)
    endif ()
    message("[${libName}] subsystem included")
endif ()


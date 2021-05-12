get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            timeSpecOp.h timerSpecOp.cpp
            )
    target_include_directories(${libName} PUBLIC "${CMAKE_INCLUDE_CURRENT_DIR}")
    message("[${libName}] subsystem included")

endif ()

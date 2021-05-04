get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            conf.h macro.h includePlatform.h
            UARTdataTransfert.cpp UARTdataTransfert.h)

    target_include_directories(${libName} PUBLIC "${CMAKE_INCLUDE_CURRENT_DIR}")

    add_subdirectory(circularBuffer)
    target_link_libraries(${libName} PUBLIC circularBuffer)

    find_package(Threads REQUIRED)
    target_link_libraries(${libName} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    message("[${libName}] CMAKE_THREAD_LIBS_INIT: " ${CMAKE_THREAD_LIBS_INIT})

    message("[${libName}] subsystem included")
endif ()

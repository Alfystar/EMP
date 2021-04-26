# Le librerie Comuni DEVONO essere PRE-Incluse
get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            UartDriver.h UartDriver.cpp
            UartException.h
            )

    #aggiunge la libreria/eseguibile in modalità INTERFACE|PUBLIC|PRIVATE alla INCLUDE_DIRECTORIES property
    target_include_directories(${libName} PUBLIC ".")
    target_link_libraries(${libName} PUBLIC circularBuffer)
    target_link_libraries(${libName} PUBLIC dataTransfertPrimitive)

    find_package(Threads REQUIRED)
    target_link_libraries(${libName} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    message("[${libName}] CMAKE_THREAD_LIBS_INIT: " ${CMAKE_THREAD_LIBS_INIT})
    if (${UNIX})
        target_compile_definitions(${libName} PUBLIC LinuxSide=1)
        #    target_compile_definitions(${libName} PUBLIC RT_THREAD=1)
    endif ()
    message("[${libName}] subsystem included")

    add_subdirectory(pcListen_demo)

endif ()

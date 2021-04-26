#definisco da quali file è composta la libreria
get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            CircularBuffer.h CircularBuffer.cpp
            )

    #aggiunge la libreria/eseguibile in modalità INTERFACE|PUBLIC|PRIVATE alla INCLUDE_DIRECTORIES property
    target_include_directories(${libName} PUBLIC "${CMAKE_INCLUDE_CURRENT_DIR}")
    if (${UNIX})
        target_compile_definitions(${libName} PUBLIC LinuxSide=1)
    endif ()
    message("[${libName}] subsystem included")
endif ()


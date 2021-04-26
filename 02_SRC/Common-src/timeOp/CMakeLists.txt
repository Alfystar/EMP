#definisco da quali file è composta la libreria
get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            timeSpecOp.h timerSpecOp.cpp
            )

    #aggiunge la libreria/eseguibile in modalità INTERFACE|PUBLIC|PRIVATE alla INCLUDE_DIRECTORIES property
    target_include_directories(${libName} PUBLIC "./")
    message("[${libName}] subsystem included")

endif ()

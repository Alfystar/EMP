#definisco da quali file è composta la libreria
get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            dataTransfert_AllInclude.h      #Header con tutti gli include di questo sotto sistema
            dataPrimitive.h                 #File con le definizioni comuni
            DataFactory.h DataFactory.cpp
            dataManipolation/AllSensor.h dataManipolation/AllSensor.cpp
            dataManipolation/CurrentMot.h dataManipolation/CurrentMot.cpp
            dataManipolation/EncoderMot.h dataManipolation/EncoderMot.cpp
            dataManipolation/SpeedMot.h dataManipolation/SpeedMot.cpp
            dataManipolation/SettingBoard_C.h dataManipolation/SettingBoard_C.cpp
            )

    #aggiunge la libreria/eseguibile in modalità INTERFACE|PUBLIC|PRIVATE alla INCLUDE_DIRECTORIES property
    #target_include_directories(dataTransfert PUBLIC ${CMAKE_INCLUDE_CURRENT_DIR})
    target_include_directories(${libName} PUBLIC "./")
    message("[${libName}] subsystem included")
    if (${UNIX})
        target_compile_definitions(${libName} PUBLIC LinuxSide=1)
    endif ()
endif ()

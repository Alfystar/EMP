get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            #Specialization class
            MP_Fd.h MP_Fd.cpp
            MP_Uart.h MP_Uart.cpp
            exeptionMPLinux.h
            )

    target_include_directories(${libName} PUBLIC "${CMAKE_INCLUDE_CURRENT_DIR}")

    add_subdirectory(MPcore)
    target_link_libraries(${libName} PUBLIC MPcore)

    add_subdirectory(timeOp)
    target_link_libraries(${libName} PUBLIC timeOp)

    find_package(Threads REQUIRED)
    target_link_libraries(${libName} PUBLIC ${CMAKE_THREAD_LIBS_INIT})

    # Define to distining the compiler from Embedded Compiler
    target_compile_definitions(${libName} PUBLIC CMAKE_COMPILING=1)

    message("[${libName}] subsystem included")
endif ()

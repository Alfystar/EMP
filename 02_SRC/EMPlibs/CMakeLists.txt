get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName}
            #Specialization class
            MP_Pipe.h MP_Pipe.cpp
            )

    target_include_directories(${libName} PUBLIC "${CMAKE_INCLUDE_CURRENT_DIR}")

    add_subdirectory(MPcore)
    target_link_libraries(${libName} PUBLIC MPcore)

    find_package(Threads REQUIRED)
    target_link_libraries(${libName} PUBLIC ${CMAKE_THREAD_LIBS_INIT})

    message("[${libName}] subsystem included")
endif ()

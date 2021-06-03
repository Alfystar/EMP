get_filename_component(libName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if (NOT TARGET ${libName})
    add_library(${libName} cmakeEmpty.cpp) #Artifact to cheat cmake )

    target_include_directories(${libName} PUBLIC "${CMAKE_INCLUDE_CURRENT_DIR}")
    add_subdirectory(src)
    target_link_libraries(${libName} PUBLIC src)

    message("[${libName}] subsystem included")
endif ()


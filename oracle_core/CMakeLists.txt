cmake_minimum_required(VERSION 2.8)

project(oracle_core)

if (WIN32)
        message(STATUS "Defining win32 macro")
        add_definitions(-Doracle_win32)
endif(WIN32)

if(UNIX AND NOT APPLE)
        message(STATUS "Defining linux macro")
        add_definitions(-Doracle_linux)
endif(UNIX AND NOT APPLE)

if(APPLE)
        message(STATUS "Defining apple macro")
        add_definitions(-Doracle_apple)
endif(APPLE)

file(GLOB_RECURSE
        lib_srcs
        "src/*.cpp"
        "include/*.hpp"
        )

include_directories(include)

add_library(oracle_core ${lib_srcs}) 
target_link_libraries(oracle_core pthread)


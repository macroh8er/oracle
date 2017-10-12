cmake_minimum_required(VERSION 3.0) 
set(CMAKE_CXX_STANDARD 14)

project(oracle)

add_subdirectory(opt)
add_subdirectory(oracle_core)
add_subdirectory(oracle_overlay)

file(GLOB_RECURSE
        lib_srcs
        "src/*.cpp"
        "src/*.hpp"
        )

file(GLOB_RECURSE
        test_srcs
        "tests/*.cpp"
        "tests/*.hpp"
        )


include_directories(oracle_core/include)
include_directories(oracle_overlay/include)

add_executable(oracle ${lib_srcs})
target_link_libraries(oracle oracle_core oracle_overlay)


include_directories(oracle_core/include opt/googletest/googletest/include opt/googletest/googlemock/include)
add_executable(oracle_tests ${test_srcs})
target_link_libraries(oracle_tests oracle_core gtest)

# copy config and media
add_custom_command(TARGET oracle PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config
        ${CMAKE_CURRENT_BINARY_DIR}/config
        )


add_custom_command(TARGET oracle PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/media
        ${CMAKE_CURRENT_BINARY_DIR}/media
        )

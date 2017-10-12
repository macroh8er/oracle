
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)


file(GLOB_RECURSE
        overlay_srcs
        "src/*.cpp"
        "include/*.hpp"
        )

include_directories(oracle_overlay include)

add_library(oracle_overlay ${overlay_srcs})

target_link_libraries(oracle_overlay Qt5::Widgets)



set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
find_package(Qt5Quick)
find_package(Qt5Qml)
find_package(Qt5Gui)


file(GLOB_RECURSE
        overlay_srcs
        "src/*.cpp"
        "include/*.hpp"
        )

include_directories(oracle_overlay include)

add_library(oracle_overlay ${overlay_srcs})

target_link_libraries(oracle_overlay Qt5::Widgets Qt5::Qml Qt5::Quick)

message(STATUS ${CMAKE_CURRENT_BINARY_DIR})

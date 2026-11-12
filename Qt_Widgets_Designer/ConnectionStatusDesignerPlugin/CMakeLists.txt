cmake_minimum_required(VERSION 3.16)

project(ConnectionStatusDesignerPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    UiPlugin
)

add_library(connectionstatusplugin MODULE

    connectionstatus.h
    connectionstatus.cpp

    connectionstatusplugin.h
    connectionstatusplugin.cpp
)

target_link_libraries(connectionstatusplugin PRIVATE

    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::UiPlugin
)

set_target_properties(connectionstatusplugin PROPERTIES

    LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/designer"

    OUTPUT_NAME "connectionstatusplugin"
)
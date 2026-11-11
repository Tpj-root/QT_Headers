
cmake_minimum_required(VERSION 3.16)

project(MyCustomPlugin VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS
    Widgets
    UiPlugin
)

add_library(customwidgetplugin MODULE

    hellowidget.h
    hellowidget.cpp

    hellowidgetplugin.h
    hellowidgetplugin.cpp
)

target_link_libraries(customwidgetplugin PRIVATE
    Qt6::Widgets
    Qt6::UiPlugin
)

set_target_properties(customwidgetplugin PROPERTIES

    OUTPUT_NAME "customwidgetplugin"

    LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/designer"
)

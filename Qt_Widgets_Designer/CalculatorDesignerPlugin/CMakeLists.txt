
cmake_minimum_required(VERSION 3.16)

project(CalculatorDesignerPlugin VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    UiPlugin
)

add_library(calculatorplugin MODULE

    calculator.h
    calculator.cpp

    calculatorplugin.h
    calculatorplugin.cpp
)

target_link_libraries(calculatorplugin PRIVATE

    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::UiPlugin
)

set_target_properties(calculatorplugin PROPERTIES

    OUTPUT_NAME "calculatorplugin"

    LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/designer"
)

cmake_minimum_required(VERSION 3.16)

project(QT_Headers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Svg
)

add_executable(QT_Headers
    main.cpp
    resources.qrc
)

target_link_libraries(QT_Headers PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Svg
)



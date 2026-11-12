
cmake_minimum_required(VERSION 3.16)

project(MyQtSoftware VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Multimedia
    MultimediaWidgets
)

qt_standard_project_setup()

qt_add_executable(MyQtSoftware
    main.cpp
)

target_link_libraries(MyQtSoftware PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

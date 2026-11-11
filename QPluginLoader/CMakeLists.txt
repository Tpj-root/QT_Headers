cmake_minimum_required(VERSION 3.16)

project(MyQtSoftware VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

qt_standard_project_setup()

qt_add_executable(MyQtSoftware
    main.cpp
    mainwindow.h
    mainwindow.cpp
    plugininterface.h
)

target_link_libraries(MyQtSoftware
    PRIVATE
        Qt6::Widgets
)

# Build all plugins
add_subdirectory(plugins/HelloPlugin)
add_subdirectory(plugins/Calculator)
add_subdirectory(plugins/Chart)
add_subdirectory(plugins/Database)
cmake_minimum_required(VERSION 3.16)

# Project name and language.
project(Qt_Menu_Toolbar_Modular_Project LANGUAGES CXX)

# Use modern C++.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Qt 6 Widgets module.
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Build the application executable.
add_executable(Qt_Menu_Toolbar_Modular_Project
    main.cpp
    include/app_actions.h
    include/menu_bar.h
    include/tool_bar.h
    include/connection.h
    include/tool_tip.h
    include/enable_disable.h
    include/set_shortcut.h
    include/set_icon.h
    include/status_bar.h
    include/library.h
)

# Allow main.cpp to find headers in the include directory.
target_include_directories(Qt_Menu_Toolbar_Modular_Project
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link Qt Widgets (QApplication, QMainWindow, QAction, QPushButton, etc.).
target_link_libraries(Qt_Menu_Toolbar_Modular_Project
    PRIVATE
        Qt6::Widgets
)

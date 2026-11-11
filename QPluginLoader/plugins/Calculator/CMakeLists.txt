add_library(Calculator MODULE
    calculatorplugin.cpp
)

target_include_directories(Calculator PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(Calculator PRIVATE
    Qt6::Widgets
)

set_target_properties(Calculator PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)
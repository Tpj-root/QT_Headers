add_library(Chart MODULE
    chartplugin.cpp
)

target_include_directories(Chart PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(Chart PRIVATE
    Qt6::Widgets
)

set_target_properties(Chart PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)
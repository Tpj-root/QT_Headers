add_library(HelloPlugin MODULE
    helloplugin.cpp
)

target_include_directories(HelloPlugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(HelloPlugin PRIVATE
    Qt6::Widgets
)

set_target_properties(HelloPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)
add_library(Database MODULE
    databaseplugin.cpp
)

target_include_directories(Database PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(Database PRIVATE
    Qt6::Widgets
    Qt6::Sql
)

set_target_properties(Database PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

add_executable(example "main.cc")

target_link_libraries(example yloc)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(example PRIVATE)

set_property(TARGET example PROPERTY CXX_STANDARD 17)


add_executable(gpumonitor "main.cc")

target_link_libraries(gpumonitor yloc)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(gpumonitor PRIVATE)


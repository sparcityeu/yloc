
add_executable(gpumonitor "main.cc")
target_link_libraries(gpumonitor yloc)
target_include_directories(gpumonitor PRIVATE)

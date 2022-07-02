
add_executable(hwloc-example "main.cc" "print.h")

target_link_libraries(hwloc-example yloc)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(HWLOC REQUIRED)

target_include_directories(hwloc-example PRIVATE ${HWLOC_INCLUDE_DIRS})
target_link_libraries(hwloc-example ${HWLOC_LIBRARIES})

set_property(TARGET hwloc-example PROPERTY CXX_STANDARD 17)

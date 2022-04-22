
add_executable(example "main.cc" "print.h")

target_link_libraries(example yloc)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(HWLOC REQUIRED)

target_include_directories(example PRIVATE ${HWLOC_INCLUDE_DIRS})
target_link_libraries(example ${HWLOC_LIBRARIES})

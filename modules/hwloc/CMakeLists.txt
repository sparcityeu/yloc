
string(REPLACE "-" "_" MOD_CPPNAME "${MOD_TARGET}")

configure_file("interface_impl.cc.in" "interface_impl.cc")

set(MOD_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/interface_impl.cc" "interface_impl.h"
    "init_graph.cc" "init_graph.h"
    "query_graph.h" "predicates.h" "visitors.h")

add_library(${MOD_TARGET} "${MOD_SOURCES}")

# This include directory is required so that the generated file can include local headers
target_include_directories(${MOD_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${MOD_TARGET} yloc-module-interface)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(hwloc REQUIRED)

target_include_directories(${MOD_TARGET} PRIVATE ${HWLOC_INCLUDE_DIRS})
target_link_libraries(${MOD_TARGET} ${HWLOC_LIBRARIES})

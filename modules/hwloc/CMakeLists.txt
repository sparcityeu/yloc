
string(REPLACE "-" "_" MOD_CPPNAME "${MOD_TARGET}")

configure_file("interface_impl.cc.in" "interface_impl.cc")

set(MOD_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/interface_impl.cc" "interface_impl.h"
    "init_graph.cc" "init_graph.h"
    "query_graph.h" "predicates.h" "visitors.h")

add_library(${MOD_TARGET} "${MOD_SOURCES}")
target_include_directories(${MOD_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Todo add this directory from main cmake file, use interface/public scopes
target_include_directories(${MOD_TARGET} PRIVATE "${PROJECT_SOURCE_DIR}/src/include/modules/")

# Todo proper include and link against hwloc
target_link_libraries(${MOD_TARGET} hwloc)

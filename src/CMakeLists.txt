
# Provide module interface as cmake target, this allows the modules to find the include directories
add_library(yloc-module-interface INTERFACE)
target_include_directories(yloc-module-interface INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/modules")

# Write available modules to modules.cc to make them usable from code.
string(REPLACE "-" "_" MAIN_MODULE_CPPNAME "${MAIN_MODULE}")
list(TRANSFORM YLOC_MODULES REPLACE "-" "_" OUTPUT_VARIABLE YLOC_MODULES_CPPNAMES)
list(JOIN YLOC_MODULES_CPPNAMES ";\nextern YlocModule *" YLOC_MODULES_DECLARATIONS)
list(JOIN YLOC_MODULES_CPPNAMES ", " YLOC_MODULES_CPPNAMES)

configure_file("modules.cc.in" "modules.cc")

# Main library
add_library(yloc 
    "include/yloc.h"
    "init.cc" "include/init.h"
    "${CMAKE_CURRENT_BINARY_DIR}/modules.cc" "include/modules.h"
    "include/graph.h"
    "graph_vertex.cc" "include/graph_vertex.h"
    "include/property.h"
    "include/component_types.h"
    "include/modules/interface.h"
    "include/modules/adapter.h"
    )

target_link_libraries(yloc ${YLOC_MODULES})
#Todo properly manage public include directory
target_include_directories(yloc INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include/modules")
target_include_directories(yloc PRIVATE "include" "include/modules")

set_property(TARGET yloc PROPERTY CXX_STANDARD 17)

# Add Boost graph to the include directories
find_package(Boost REQUIRED COMPONENTS graph)

target_include_directories(yloc PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(yloc-module-interface INTERFACE ${Boost_INCLUDE_DIRS})

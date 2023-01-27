# Write available modules to modules.cc to make them usable from code.
string(REPLACE "-" "_" MAIN_MODULE_CPPNAME "${MAIN_MODULE}")
list(TRANSFORM YLOC_MODULES REPLACE "-" "_" OUTPUT_VARIABLE YLOC_MODULES_CPPNAMES)
list(JOIN YLOC_MODULES_CPPNAMES ";\nextern YlocModule *" YLOC_MODULES_DECLARATIONS)
list(JOIN YLOC_MODULES_CPPNAMES ", " YLOC_MODULES_CPPNAMES)

configure_file("modules.cc.in" "modules.cc")

# Main library
add_library(yloc
    "init.cc"
    "${CMAKE_CURRENT_BINARY_DIR}/modules.cc"
    "graph/vertex.cc"
)

target_link_libraries(yloc ${YLOC_MODULES})

find_package(Boost REQUIRED COMPONENTS graph)
target_include_directories(yloc PUBLIC ${Boost_INCLUDE_DIRS})

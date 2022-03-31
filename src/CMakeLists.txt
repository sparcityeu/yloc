
include_directories("include" "include/modules")

string(REPLACE "-" "_" MAIN_MODULE_CPPNAME "${MAIN_MODULE}")
list(TRANSFORM YLOC_MODULES REPLACE "-" "_" OUTPUT_VARIABLE YLOC_MODULES_CPPNAMES)
list(JOIN YLOC_MODULES_CPPNAMES ";\nextern YlocModule *" YLOC_MODULES_DECLARATIONS)
list(JOIN YLOC_MODULES_CPPNAMES ", " YLOC_MODULES_CPPNAMES)

configure_file("modules.cc.in" "modules.cc")

# Todo include boost properly

add_library(yloc "init.cc" "include/init.h"
    "${CMAKE_CURRENT_BINARY_DIR}/modules.cc" "include/modules.h" "include/modules/interface.h" "include/modules/graph_type.h")

target_link_libraries(yloc ${YLOC_MODULES})

#set_property(TARGET yloc PROPERTY CXX_STANDARD 17)

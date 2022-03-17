
include_directories("include")

add_library(yloc "init.cc" "include/init.h"
    "modules.cc" "include/modules.h" "include/modules/interface.h")

target_link_libraries(yloc ${YLOC_MODULES})

#set_property(TARGET yloc PROPERTY CXX_STANDARD 17)

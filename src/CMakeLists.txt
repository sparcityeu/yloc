# Write available modules to modules.cc to make them usable from code.
string(REPLACE "-" "_" MAIN_MODULE_CPPNAME "${MAIN_MODULE}")
list(TRANSFORM YLOC_MODULES REPLACE "-" "_" OUTPUT_VARIABLE YLOC_MODULES_CPPNAMES)
list(JOIN YLOC_MODULES_CPPNAMES ";\nextern Module *" YLOC_MODULES_DECLARATIONS)
list(JOIN YLOC_MODULES_CPPNAMES ", " YLOC_MODULES_CPPNAMES)

configure_file("modules.cc.in" "modules.cc")

set(YLOC_SOURCES
    "init.cc"
    "util.cc"
    "${CMAKE_CURRENT_BINARY_DIR}/modules.cc"
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Main library
add_library(yloc SHARED
    ${YLOC_SOURCES}
)

add_library(yloc-static STATIC
    ${YLOC_SOURCES}
)

set_target_properties(yloc PROPERTIES VERSION 1.0.0 SOVERSION 1)

target_link_libraries(yloc ${YLOC_MODULES})

find_package(Boost REQUIRED COMPONENTS graph)
target_include_directories(yloc PUBLIC ${Boost_INCLUDE_DIRS})

# install (public) yloc header
# TODO: this currently just copies ALL header files from include/yloc
install(DIRECTORY include/yloc DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        PATTERN "modules*" EXCLUDE)

# install yloc library
install(TARGETS yloc yloc-static
        LIBRARY DESTINATION lib # shared library
        ARCHIVE DESTINATION lib/yloc # static library
        # RUNTIME DESTINATION bin # executable
        # PUBLIC_HEADER DESTINATION include/yloc
        # PRIVATE_HEADER DESTINATION include/yloc
)

# install(TARGETS yloc-cli DESTINATION bin)

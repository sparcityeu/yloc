
string(REPLACE "-" "_" MOD_CPPNAME "${MOD_TARGET}")

find_package(MPI)

if(NOT ${MPI_FOUND})
    return()
endif()

configure_file("interface_impl.cc.in" "interface_impl.cc")

set(MOD_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/interface_impl.cc" "interface_impl.h" "init_graph.cc"
    "supermuc_adapter.h")

add_library(${MOD_TARGET} "${MOD_SOURCES}")

set_property(TARGET ${MOD_TARGET} PROPERTY CXX_STANDARD 17)

# This include directory is required so that the generated file can include local headers
target_include_directories(${MOD_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${MOD_TARGET} yloc-module-interface)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(${MOD_TARGET} PRIVATE ${MPI_CXX_INCLUDE_PATH})
target_link_libraries(${MOD_TARGET} ${MPI_CXX_LIBRARIES})


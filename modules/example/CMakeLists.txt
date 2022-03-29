
string(REPLACE "-" "_" MOD_CPPNAME "${MOD_TARGET}")

configure_file("interface_impl.cpp.in" "interface_impl.cpp")

set(MOD_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/interface_impl.cpp")

add_library(${MOD_TARGET} "${MOD_SOURCES}")
target_include_directories(${MOD_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Todo add this directory from main cmake file, use interface/public scopes
target_include_directories(${MOD_TARGET} PRIVATE "${PROJECT_SOURCE_DIR}/src/include/modules/")

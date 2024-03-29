
string(REPLACE "-" "_" MOD_CPPNAME "${MOD_TARGET}")

if(NOT DEFINED ENV{ROCM_DIR})
    return()
endif()

configure_file("interface_impl.cc.in" "interface_impl.cc")

set(MOD_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/interface_impl.cc" "rocm_impl.cc")

add_library(${MOD_TARGET} OBJECT "${MOD_SOURCES}")
set_property(TARGET ${MOD_TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

# This include directory is required so that the generated file can include local headers
target_include_directories(${MOD_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

########################
# rocm specific
########################

# Search for rocm in common locations
# list(APPEND CMAKE_PREFIX_PATH /opt/rocm/hip /opt/rocm)
# list(APPEND CMAKE_PREFIX_PATH /opt/rocm-4.5.0/hip /opt/rocm-4.5.0)

# find_package(hip) finds /opt/rocm directory on rome1 instead of /opt/rocm-4.5.0
# which does not have rocm-smi installed ...
# so library directory and include directory are set manual instead
# (ROCM_DIR) must be set by user
# 
# find_package(hip)
# target_include_directories(${MOD_TARGET} PUBLIC "${hip_INCLUDE_DIR}")
# target_link_directories(${MOD_TARGET} PUBLIC "${hip_LIB_INSTALL_DIR}")

# find_package(hip)

message(STATUS "$ENV{ROCM_DIR}/include")

target_include_directories(${MOD_TARGET} PUBLIC "$ENV{ROCM_DIR}/rocm_smi/include")
target_link_directories(${MOD_TARGET} PUBLIC "$ENV{ROCM_DIR}/rocm_smi/lib")

target_link_libraries(${MOD_TARGET} rocm_smi64) # hip::host 

# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

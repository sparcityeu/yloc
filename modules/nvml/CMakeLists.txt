
########################
# NVIDIA nvml specific
########################

# message(STATUS "To use a different installed version of the toolkit set the environment variable CUDA_BIN_PATH")
# message(STATUS "It might be necessary to set CUDA_TOOLKIT_ROOT_DIR manually on certain platforms")
if(${CMAKE_VERSION} VERSION_LESS "3.17")
    find_package(CUDA) # deprecated since cmake version 3.10
    if(NOT DEFINED CUDA_TOOLKIT_ROOT_DIR)
        message(WARNING "FindCUDA: CUDA toolkit not found")
        return()
    endif()
    set(CUDA_LIB_DIRS ${CUDA_LIBRARIES})
    set(CUDA_INC_DIRS ${CUDA_INCLUDE_DIRS})
else()
    find_package(CUDAToolkit) # since cmake version 3.17
    if(NOT ${CUDAToolkit_FOUND})
        message(WARNING "FindCUDAToolkit: CUDA toolkit not found")
        return()
    endif()
    set(CUDA_LIB_DIRS ${CUDAToolkit_LIBRARY_DIR})
    set(CUDA_INC_DIRS ${CUDAToolkit_INCLUDE_DIRS})
endif()

###################################
# yloc specific target definition
###################################

string(REPLACE "-" "_" MOD_CPPNAME "${MOD_TARGET}")

configure_file("interface_impl.cc.in" "interface_impl.cc")

set(MOD_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/interface_impl.cc" "nvml_impl.cc")

add_library(${MOD_TARGET} "${MOD_SOURCES}")

# This include directory is required so that the generated file can include local headers
target_include_directories(${MOD_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_directories(${MOD_TARGET} PUBLIC ${CUDA_LIB_DIRS})
target_include_directories(${MOD_TARGET} PUBLIC ${CUDA_INC_DIRS})

target_link_libraries(${MOD_TARGET} nvidia-ml) 

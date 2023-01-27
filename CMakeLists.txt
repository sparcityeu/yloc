cmake_minimum_required(VERSION 3.12)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# set C++ standard for project
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra)
add_compile_options(-Wno-unused) # TODO: we need to fix the unused parameters...
add_compile_options(-O2 -march=native)

project(yloc VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(modules)
add_subdirectory(yloc)
add_subdirectory(example)

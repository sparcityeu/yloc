cmake_minimum_required(VERSION 3.12)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# set C++ standard for project
set(CMAKE_CXX_STANDARD 17)

project(yloc CXX)

add_subdirectory(modules)

add_subdirectory(src)

add_subdirectory(example)

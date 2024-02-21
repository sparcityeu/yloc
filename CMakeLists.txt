cmake_minimum_required(VERSION 3.12)

project(yloc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# set C++ standard for project
set(CMAKE_CXX_STANDARD 17)

# add_compile_options(-Wall -Wextra)
# add_compile_options(-Weffc++)

# for compile-time bitset length
execute_process(COMMAND getconf _NPROCESSORS_ONLN OUTPUT_VARIABLE NPROCESSORS_ONLN OUTPUT_STRIP_TRAILING_WHITESPACE)
add_compile_definitions(NPROCESSORS_ONLN=${NPROCESSORS_ONLN})

# path for cmake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories("${CMAKE_SOURCE_DIR}/src/include")

add_subdirectory(modules)
add_subdirectory(src)
add_subdirectory(example)
# build the documentation
# add_subdirectory(docs)


add_executable(example "main.cc" "output_graph.h" "query_graph.h" "predicates.h" "visitors.h" "algo.h" "distances.h")

target_link_libraries(example yloc)

# Todo proper include directory from main CMake-file
target_include_directories(example PRIVATE "${PROJECT_SOURCE_DIR}/src/include" "${PROJECT_SOURCE_DIR}/src/include/modules")

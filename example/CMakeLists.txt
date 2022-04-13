
add_executable(example "main.cc" "output_graph.h" "query_graph.h" "predicates.h" "visitors.h" "algo.h" "distances.h")

target_link_libraries(example yloc)


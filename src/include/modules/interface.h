#pragma once

typedef void* graph_t; // Boost graph here? Yes actual type here, probably Boost graph containing a map.

void init_graph(graph_t * graph);               // init complete graph
void init_graph_secondary(graph_t * graph);     // init of secondary module (if graph already is initialized by another module)

void export_graph(graph_t * graph, void * output);
// optional function, not ever module requires this
// it is recommended that every module that can init a graph on its own also provides this function.

void update_graph(graph_t * graph);
// Manual(?) trigger for an update on the graph
// Thread logic in module or in init?
// Probably both, modules that require push logic need internal threads.
// Otherwise keep thread logic in init or modules
//
// An alternative would be to add a auto-update function to the interface,
// this would be a good place to add an per-module interval


template <typename T>
T query_graph(graph_t * graph, void * query);
// Todo this function does not belong here,
// it is only wriiten here to highlight the logic required inside the modules for this function



#pragma once

#include <graph_type.h>
#include <graph_object.h>
/** TODO: think about constness of arguments and functions **/

namespace yloc
{
    class YlocModule
    {
    public:
        YlocModule() : _subgraph(root_graph().create_subgraph()) {}

        virtual void init_graph(/* graph_t &graph */) = 0; // init module subgraph

        virtual void export_graph(graph_t &graph, void **output) = 0;
        // optional function, not ever module requires this
        // it is recommended that every module that can init a graph on its own also provides this function.

        virtual void update_graph(graph_t &graph) = 0;
        // Manual(?) trigger for an update on the graph
        // Thread logic in module or in init?
        // Probably both, modules that require push logic need internal threads.
        // Otherwise keep thread logic in init or modules
        //
        // An alternative would be to add a auto-update function to the interface,
        // this would be a good place to add an per-module interval

        // virtual graph_t & subgraph() = 0;
    protected:
        graph_t &_subgraph; // module subgraph
    };
}

// template <typename T>
// T query_graph(graph_t *graph, void *query);
// Todo this function does not belong here,
// it is only written here to highlight the logic required inside the modules for this function

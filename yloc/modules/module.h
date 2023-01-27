#pragma once

#include <yloc/yloc_status.h> // yloc_status_t

namespace yloc
{
    class Graph; // forward declaration

    class YlocModule
    {
    public:
        /** TODO: how to avoid duplicated code, but have "pure" interface class YlocModule? */
#if USE_SUBGRAPH
        YlocModule() : m_subgraph(root_graph().create_subgraph()) {}
#else
#endif /* USE_SUBGRAPH */

        virtual ~YlocModule() = default; // 0;

        virtual yloc_status_t init_graph(Graph &graph) = 0; // init module subgraph

        virtual yloc_status_t export_graph(const Graph &graph, void **output) = 0;
        // optional function, not ever module requires this
        // it is recommended that every module that can init a graph on its own also provides this function.

        virtual yloc_status_t update_graph(Graph &graph) = 0;
        // Manual(?) trigger for an update on the graph
        // Thread logic in module or in init?
        // Probably both, modules that require push logic need internal threads.
        // Otherwise keep thread logic in init or modules
        //
        // An alternative would be to add a auto-update function to the interface,
        // this would be a good place to add an per-module interval

        // virtual Graph & subgraph() = 0;

    protected:
#if USE_SUBGRAPH
        Graph &m_subgraph; // module subgraph
#else
#endif /* USE_SUBGRAPH */
    };
}

// template <typename T>
// T query_graph(Graph *graph, void *query);
// Todo this function does not belong here,
// it is only written here to highlight the logic required inside the modules for this function

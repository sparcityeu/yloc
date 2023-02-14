#pragma once

#include <yloc/yloc_status.h> // yloc_status_t

namespace yloc
{
    class Graph;

    class Module
    {
    public:
        /** TODO: how to avoid duplicated code, but have "pure" interface class Module? */
#if USE_SUBGRAPH
        Module() : m_subgraph(root_graph().create_subgraph()) {}
#else
#endif /* USE_SUBGRAPH */

        /** TODO: use pure virtual destructor ? then we must override in sub-classes */
        virtual ~Module() = default; // 0;

        /**
         * @brief Initializes the module's subgraph and attaches to the root graph.
         * 
         * @param graph The root graph
         * @return yloc_status_t 
         */
        virtual yloc_status_t init_graph(Graph &graph) = 0;

        virtual yloc_status_t export_graph(const Graph &graph, void **output) = 0;

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
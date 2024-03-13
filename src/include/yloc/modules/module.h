#pragma once

#include <yloc/status.h> // yloc_status_t

namespace yloc
{
    class Graph;

    class Module
    {
    public:
        enum class init_order : int { FIRST=0, SECOND=1 };

        /** TODO: use pure virtual destructor ? then we must override in sub-classes */
        virtual ~Module() = default; // 0;

        /**
         * @brief Initializes the module's subgraph and attaches to the root graph.
         *
         * @param graph The root graph
         * @return yloc_status_t
         */
        virtual yloc_status_t init_graph(Graph &graph) = 0;
        
        init_order m_init_order{init_order::FIRST};
        bool m_enabled{true};
    };
}

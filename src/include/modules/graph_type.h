#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>

// enum yloc_vertex_type { YLOC_VERTEX_TYPE_MAX };

enum yloc_edge_type { YLOC_EDGE_TYPE_PARENT,
                      YLOC_EDGE_TYPE_CHILD,
                      YLOC_EDGE_TYPE_MAX };

namespace yloc
{
    // using vertex_type = yloc_vertex_type;

    using edge_type = yloc_edge_type;

    /* bundled internal (common) vertex properties */
    struct Vertex {
        // yloc_vertex_type type;
        /* TODO: somehow we need to point to the vertex property maps here */
        // AdapterContainer ac;
    };

    /* bundled internal (common) edge properties */
    struct Edge {
        edge_type type;
        /* TODO: somehow we need to point to the edge property maps here */
        // AdapterContainer ac;
    };

    // using graph_t = boost::adjacency_list<
    //     boost::vecS,           //  out-edge container selector
    //     boost::vecS,           //  Vector container selector
    //     boost::bidirectionalS, // sets bidirectional edge access
    //     Vertex,                // vertex property
    //     Edge>;                 // edge property

    using graph_t = boost::subgraph<
        boost::adjacency_list<
            boost::vecS,           //  out-edge container selector
            boost::vecS,           //  Vector container selector
            boost::bidirectionalS, // sets bidirectional edge access
            Vertex,                // vertex property
            // edge property (see bundled properties, subgraph)
            boost::property<boost::edge_index_t, std::size_t, Edge>>>;

    using edge_descriptor_t = typename boost::graph_traits<graph_t>::edge_descriptor;
    using vertex_descriptor_t = typename boost::graph_traits<graph_t>::vertex_descriptor;
}


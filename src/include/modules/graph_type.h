#pragma once

#include <boost/graph/adjacency_list.hpp>

// TODO: use namespace yloc?

enum yloc_vertex_type { YLOC_VERTEX_TYPE_MAX };

enum yloc_edge_type { YLOC_EDGE_TYPE_PARENT,
                      YLOC_EDGE_TYPE_CHILD,
                      YLOC_EDGE_TYPE_MAX };

/* bundled internal (common) vertex properties */
struct Vertex {
    // yloc_vertex_type type;
};

/* bundled internal (common) edge properties */
struct Edge {
    yloc_edge_type type;
};

using graph_t = boost::adjacency_list<
    boost::vecS,           //  out-edge container selector
    boost::vecS,           //  Vector container selector
    boost::bidirectionalS, // sets bidirectional edge access
    Vertex,                // vertex property
    Edge>;                 // edge property

// using ndgraph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge>;

// graph internally used descriptors for the main graph, probably not derived types, can become invalidated
using ED = typename boost::graph_traits<graph_t>::edge_descriptor;
using VD = typename boost::graph_traits<graph_t>::vertex_descriptor;

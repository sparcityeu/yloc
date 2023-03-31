#pragma once

#include <yloc/graph.h>

#include <boost/graph/breadth_first_search.hpp>

#include <functional>
#include <type_traits>
#include <vector>

namespace yloc
{
    template <class Graph>
    using vertex = typename boost::graph_traits<Graph>::vertex_descriptor;

    template <class Graph>
    using edge = typename boost::graph_traits<Graph>::edge_descriptor;

    template <class Graph, class DistanceMap, class Fn>
    class bfs_tree_edge_visitor : public boost::default_bfs_visitor
    {
        using T = typename boost::property_traits<DistanceMap>::value_type;
        using E = edge<Graph>;

    public:
        bfs_tree_edge_visitor(Graph &graph, DistanceMap dmap, Fn on_tree_edge)
            : m_distmap{dmap}, m_on_tree_edge{on_tree_edge}
        {
        }

        void tree_edge(E e, const Graph &graph) const
        {
            vertex<Graph> u = boost::source(e, graph);
            vertex<Graph> v = boost::target(e, graph);
            boost::put(m_distmap, v, m_on_tree_edge(graph, e, boost::get(m_distmap, u)));
        }

    private:
        DistanceMap m_distmap;
        Fn m_on_tree_edge;
    };

    template <class Graph, class Fn>
    auto bfs_distance_vector(Graph &graph, vertex<Graph> v, Fn on_tree_edge)
    {
        using T = typename decltype(std::function{std::declval<Fn>()})::result_type;
        static_assert(std::is_invocable_r_v<T, Fn, Graph &, edge<Graph>, T>);

        std::vector<T> dist(boost::num_vertices(graph));
        auto dist_pmap = boost::make_iterator_property_map(dist.begin(), boost::get(boost::vertex_index, graph));
        auto visitor = bfs_tree_edge_visitor(graph, dist_pmap, on_tree_edge);
        boost::breadth_first_search(graph, v, boost::visitor(visitor));
        return dist;
    }

    // number of edges in the shortest path from the source vertex to other vertices
    template <class Graph>
    std::vector<int> bfs_distance_vector(Graph &graph, vertex<Graph> v)
    {
        return bfs_distance_vector(graph, v, [&](const Graph &g, edge<Graph> e, int val_pre) { return val_pre + 1; });
    }
} // namespace yloc

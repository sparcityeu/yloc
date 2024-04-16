#pragma once

#include <yloc/graph.h>
#include <yloc/util.h>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <functional>
#include <type_traits>
#include <unordered_map>
#include <vector>

/**
 * @brief Specialization of std::hash structure in the std namespace to use yloc::edge_t as key in associative
 * containers.
 *
 * @tparam
 */
template <>
struct std::hash<yloc::edge_t> {
    std::size_t operator()(const yloc::edge_t &f) const
    {
        return std::hash<std::size_t>()(f.m_source) ^ std::hash<std::size_t>()(f.m_target) << 1;
    }
};

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
            boost::put(m_distmap, v, m_on_tree_edge(e, boost::get(m_distmap, u)));
        }

    private:
        DistanceMap m_distmap;
        Fn m_on_tree_edge;
    };

    /**
     * @brief Performs a BFS on the graph starting from vertex vstart and computes distances to other vertices.
     *
     * @tparam Graph
     * @tparam Fn
     * @param graph The graph
     * @param vstart The starting vertex
     * @param on_tree_edge Distance function run on tree edges
     * @return std::vector The distance vector
     */
    template <class Graph, class Fn>
    auto bfs_distance_vector(Graph &graph, vertex<Graph> vstart, Fn on_tree_edge)
    {
        using T = typename decltype(std::function{std::declval<Fn>()})::result_type;
        static_assert(std::is_invocable_r_v<T, Fn, edge<Graph>, T>);

        std::vector<T> distances(boost::num_vertices(graph));
        auto dist_pmap = boost::make_iterator_property_map(distances.begin(), boost::get(boost::vertex_index, graph));
        auto visitor = bfs_tree_edge_visitor(graph, dist_pmap, on_tree_edge);
        boost::breadth_first_search(graph, vstart, boost::visitor(visitor));
        return distances;
    }

    // Computes number of edges in the shortest path from the source vertex to other vertices
    template <class Graph>
    std::vector<int> bfs_distance_vector(Graph &graph, vertex<Graph> vstart)
    {
        return bfs_distance_vector(
            graph, vstart, [&](edge<Graph> e, int val_pre) { return val_pre + 1; });
    }

    /**
     * @brief Builds a map from edges to edge weights defined by edge_weight_fn.
     * 
     * @tparam Graph 
     * @tparam Fn 
     * @param graph 
     * @param edge_weight_fn 
     * @return std::unordered_map 
     */
    template <class Graph, class Fn>
    auto make_edge_weight_map(Graph &graph, Fn edge_weight_fn)
    {
        using T = typename decltype(std::function{std::declval<Fn>()})::result_type;
        static_assert(std::is_invocable_r_v<T, Fn, edge<Graph>>);

        // fill edge weight map
        std::unordered_map<edge<Graph>, T> edge_weight_map{};
        for (auto e : yloc::edge_range(graph)) {
            edge_weight_map[e] = edge_weight_fn(e);
        }
        return edge_weight_map;
    }

    template <class Graph, class Map>
    auto dijkstra_path_distance_vector(Graph &graph, vertex<Graph> vstart, Map edge_weight_map)
    {
        using T = typename Map::mapped_type;

        std::vector<yloc::vertex_t> predecessors(boost::num_vertices(graph));
        std::vector<T> distances(boost::num_vertices(graph));

        boost::dijkstra_shortest_paths(
            graph,
            vstart,
            predecessor_map(boost::make_iterator_property_map(predecessors.begin(), get(boost::vertex_index, graph)))
                .weight_map(boost::make_assoc_property_map(edge_weight_map))
                .distance_map(boost::make_iterator_property_map(distances.begin(), get(boost::vertex_index, graph))));

        return std::make_pair(predecessors, distances);
    }

#if 0
template <class Graph, class Fn>
auto dijkstra_path_distance_vector(Graph &graph, yloc::vertex_t start, Fn edge_weight)
{
    using T = typename decltype(std::function{std::declval<Fn>()})::result_type;
    static_assert(std::is_invocable_r_v<T, Fn, yloc::Edge>);

    std::vector<yloc::vertex_t> predecessors(boost::num_vertices(graph)); /* TODO: num_vertices_view */
    std::vector<T> distances(boost::num_vertices(graph));

    // fill edge weight map
    std::unordered_map<yloc::edge_t, T> edge_weight_map{};
    for (auto e : yloc::edge_range(graph)) {
        edge_weight_map[e] = edge_weight(graph[e]);
    }

    boost::dijkstra_shortest_paths(
        graph,
        start,
        predecessor_map(boost::make_iterator_property_map(predecessors.begin(), get(boost::vertex_index, graph)))
            .weight_map(boost::make_assoc_property_map(edge_weight_map))
            .distance_map(boost::make_iterator_property_map(distances.begin(), get(boost::vertex_index, graph))));

    return std::make_pair(predecessors, distances);
}
#endif

#if 0
template <typename Graph>
auto dijkstra_path_distance_vector(Graph &graph, vertex<Graph> vstart, std::string edge_weight_property)
{
    return dijkstra_path_distance_vector(Graph &graph, vertex<Graph> vstart, [&](yloc::Edge edge) { return edge.template get<T>(edge_weight_property).value_or(?); } )
}
#endif

} // namespace yloc

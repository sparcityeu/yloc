#pragma once

#include <yloc/graph.h>

#include <boost/graph/breadth_first_search.hpp>

#include <functional>

namespace yloc
{
    template <class DistMap>
    class bfs_tree_edge_visitor : public boost::default_bfs_visitor
    {
        typedef typename boost::property_traits<DistMap>::value_type T;

    public:
        bfs_tree_edge_visitor(DistMap dmap, std::function<T(const Graph &, edge_descriptor_t, T)> on_tree_edge)
            : m_distmap{dmap}, m_on_tree_edge{on_tree_edge} {}

        void tree_edge(edge_descriptor_t e, const Graph &g) const
        {
            typename boost::graph_traits<Graph>::vertex_descriptor u = boost::source(e, g),
                                                                   v = boost::target(e, g);
            boost::put(m_distmap, v, m_on_tree_edge(g, e, boost::get(m_distmap, u)));
        }

    private:
        DistMap m_distmap;
        std::function<T(const Graph &, edge_descriptor_t, T)> m_on_tree_edge;
    };

    template <typename T>
    std::vector<T> bfs_distance_vector(vertex_descriptor_t v, std::function<T(const Graph &, edge_descriptor_t, T)> on_tree_edge)
    {
        auto &graph = yloc::root_graph();
        std::vector<T> dist(boost::num_vertices(graph));
        auto dist_pmap = boost::make_iterator_property_map(dist.begin(), boost::get(boost::vertex_index, graph));
        auto visitor = bfs_tree_edge_visitor(dist_pmap, on_tree_edge);

        boost::breadth_first_search(graph, v, boost::visitor(visitor));
        return dist;
    }

    // number of edges in the shortest path from the source vertex
    inline std::vector<int> bfs_distance_vector(vertex_descriptor_t v)
    {
        // auto &graph = yloc::root_graph();
        // std::vector<int> dist(boost::num_vertices(graph));
        // auto dist_pmap = boost::make_iterator_property_map(dist.begin(), boost::get(boost::vertex_index, graph));
        // auto visitor = boost::make_bfs_visitor(boost::record_distances(dist_pmap, boost::on_tree_edge()));
        // boost::breadth_first_search(graph, v, boost::visitor(visitor));

        return yloc::bfs_distance_vector(v, std::function{[&](const yloc::Graph &graph, yloc::edge_descriptor_t e, int val_pre) {
                                             return val_pre + 1;
                                         }});
    }

#if YLOC_NOT_IMPLEMENTED_YET
    class Query
    {
    };

    void distance(vertex_descriptor_t &vd_start, vertex_descriptor_t &vd_end)
    {
    }
#endif

}

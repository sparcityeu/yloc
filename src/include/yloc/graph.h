#pragma once

#include <unordered_map>

#define USE_SUBGRAPH 0

#include <boost/graph/adjacency_list.hpp>
#if USE_SUBGRAPH
#include <boost/graph/subgraph.hpp>
#endif

#include <yloc/vertex.h>
#include <yloc/edge.h>
#include <yloc/yloc_status.h>

namespace yloc
{
#if USE_SUBGRAPH
    using boost_graph_t = boost::subgraph<
        boost::adjacency_list<
            boost::vecS,           //  out-edge container selector
            boost::vecS,           //  Vector container selector
            boost::bidirectionalS, // sets bidirectional edge access
            Vertex,                // vertex property
            // edge property (see bundled properties, subgraph)
            boost::property<boost::edge_index_t, std::size_t, Edge>>>;
#else
    using boost_graph_t = boost::adjacency_list<
        boost::vecS,           //  out-edge container selector
        boost::vecS,           //  Vector container selector
        boost::bidirectionalS, // sets bidirectional edge access
        Vertex,                // vertex property
        Edge>;                 // edge property
#endif

    using edge_descriptor_t = typename boost::graph_traits<boost_graph_t>::edge_descriptor;
    using vertex_descriptor_t = typename boost::graph_traits<boost_graph_t>::vertex_descriptor;

    class Graph
    {
    public:
        using identifier_t = std::string;

        Graph() = default;

    private:
        Graph(Graph &) = default;

    public:
        Graph(Graph &&) = default;

        Graph(boost_graph_t _graph)
            : m_graph(_graph) {}

        ~Graph() = default;

    private:
        Graph &operator=(Graph &) = default;

    public:
        Graph &operator=(Graph &&) = default;

        /**
         * @brief Implicit conversion to boost graph. 
         */
        operator boost_graph_t &() { return m_graph; }
        operator const boost_graph_t &() const { return m_graph; }

        /**
         * @brief Provides access to the underlying boost graph.
         */
        boost_graph_t &boost_graph() { return m_graph; }

        /**
         * @brief Adds a new unnamed vertex to the graph.
         */
        vertex_descriptor_t add_vertex()
        {
            return boost::add_vertex(m_graph);
        }

        /**
         * @brief Get a vertex for the identifier id.
         *
         * Adds a new vertex to the graph if there is no vertex with id.
         * Otherwise the already existing vertex is returned.
         *
         * @param id The identifier of the vertex.
         */
        vertex_descriptor_t add_vertex(identifier_t id)
        {
            auto iter = m_identifier_map.find(id);
            if (iter != m_identifier_map.end()) {
                return iter->second;
            }
            auto vd = boost::add_vertex(m_graph);
            m_identifier_map.insert({id, vd});
            return vd;
        }

        /**
         * @brief Provides access to vertices of the underlying boost graph.
         */
        auto &operator[](vertex_descriptor_t vd)
        {
            return m_graph[vd];
        }

        /**
         * @brief Provides access to vertices of the underlying boost graph using an identifier.
         */
        auto &operator[](identifier_t id)
        {
            /** TODO: Error handling when id is not available, maybe call add_vertex in background */
            return m_graph[m_identifier_map[id]];
        }

#if USE_SUBGRAPH
        Graph create_subgraph();
#endif

    private:
        boost_graph_t m_graph;
        std::unordered_map<identifier_t, vertex_descriptor_t> m_identifier_map;
    };


    /**
     * @brief Returns yloc topology root graph.
     *
     * @return Graph&
     */
    Graph &root_graph();
}

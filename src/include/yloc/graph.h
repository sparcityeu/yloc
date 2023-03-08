#pragma once

#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>

#include <yloc/edge.h>
#include <yloc/vertex.h>
#include <yloc/yloc_status.h>

namespace yloc
{
    using boost_graph_t = boost::adjacency_list<
        boost::vecS,           //  out-edge container selector
        boost::vecS,           //  Vector container selector
        boost::bidirectionalS, // sets bidirectional edge access
        Vertex,                // vertex property
        Edge>;                 // edge property

    using edge_descriptor_t = typename boost::graph_traits<boost_graph_t>::edge_descriptor;
    using vertex_descriptor_t = typename boost::graph_traits<boost_graph_t>::vertex_descriptor;

    class Graph : public boost_graph_t
    {
    public:
        using identifier_t = std::string;

        Graph() = default;

    private:
        Graph(Graph &) = default;

    public:
        Graph(Graph &&) = default;

        ~Graph() = default;

    private:
        Graph &operator=(Graph &) = default;

    public:
        Graph &operator=(Graph &&) = default;

        /**
         * @brief Adds a new unnamed vertex to the graph.
         */
        vertex_descriptor_t add_vertex()
        {
            return boost::add_vertex(*this);
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
            auto vd = boost::add_vertex(*this);
            m_identifier_map.insert({id, vd});
            return vd;
        }

        /**
         * @brief Provides access to vertices of the underlying boost graph.
         */
        auto &operator[](vertex_descriptor_t vd)
        {
            return this->boost_graph_t::operator[](vd);
        }

        /**
         * @brief Provides access to vertices of the underlying boost graph.
         */
        const auto &operator[](vertex_descriptor_t vd) const
        {
            return this->boost_graph_t::operator[](vd);
        }

        /**
         * @brief Provides access to vertices of the underlying boost graph using an identifier.
         */
        auto &operator[](identifier_t id)
        {
            /** TODO: Error handling when id is not available, maybe call add_vertex in background */
            return (*this)[m_identifier_map[id]];
        }

    private:
        std::unordered_map<identifier_t, vertex_descriptor_t> m_identifier_map{};
    };

    /**
     * @brief Returns yloc topology root graph.
     *
     * @return Graph&
     */
    Graph &root_graph();
}

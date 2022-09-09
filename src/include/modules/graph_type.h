#pragma once

#define USE_SUBGRAPH 0

#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
// #include <adapter.h>
#include <adapter_container.h>

enum yloc_vertex_type { YLOC_VERTEX_TYPE_MAX };

enum yloc_edge_type { YLOC_EDGE_TYPE_PARENT,
                      YLOC_EDGE_TYPE_CHILD,
                      YLOC_EDGE_TYPE_MAX };
                      
namespace yloc
{
    using vertex_type = yloc_vertex_type;
    using edge_type = yloc_edge_type;

    /* bundled internal (common) vertex properties */
    struct Vertex {
        // yloc_vertex_type type;
        /* TODO: somehow we need to point to the vertex property maps here */
        AdapterContainer tinfo;
        // Adapter a;
    };

    /* bundled internal (common) edge properties */
    struct Edge {
        edge_type type;
        /* TODO: somehow we need to point to the edge property maps here */
        // AdapterContainer tinfo;
    };

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
        Graph(Graph &) = default;
        Graph(Graph &&) = default;
        
        Graph(boost_graph_t _graph)
        : m_graph(_graph) {}

        ~Graph() = default;

        Graph & operator=(Graph &) = default;
        Graph & operator=(Graph &&) = default;

        operator boost_graph_t&() { return m_graph; }
        operator const boost_graph_t&() const { return m_graph; }

        boost_graph_t& boost_graph() { return m_graph; }

        vertex_descriptor_t add_vertex() {
            return boost::add_vertex(m_graph);
        }

        vertex_descriptor_t add_vertex(identifier_t id) {
            if(m_identifier_map.count(id) > 0) {
                return m_identifier_map.at(id);
            }
            auto vd = boost::add_vertex(m_graph);
            m_identifier_map.insert({id,vd});
            register_pending(vd,id);
            return vd;
        }

        /** TODO: May depend on module initialization order */
        bool register_vertex(vertex_descriptor_t vd, identifier_t id) {
            auto result = m_identifier_map.insert({id,vd});
            register_pending(vd,id);
            return result.second;
        }

        /** TODO: Depends on module initialization order
         *        Pending list?
         *        Better: globally valid identifiers
         */
        bool register_vertex(identifier_t old_id, identifier_t id) {
            if(m_identifier_map.count(old_id) == 0) {
                m_pending_identifiers.insert({old_id,id});
                return false;
            }
            auto vd = m_identifier_map.at(old_id);
            auto result = m_identifier_map.insert({id,vd});
            register_pending(vd,id);
            return result.second;
        }

        auto operator[] (vertex_descriptor_t vd)
        {
            return m_graph[vd];
        }

        auto operator[] (identifier_t id)
        {
            /** TODO: Error handling when id is not available, maybe call add_vertex in background */
            return m_graph[m_identifier_map[id]];
        }

#if USE_SUBGRAPH
        Graph create_subgraph();
#endif

    private:

        /**
         * @brief Registration of pending identifiers for vertices that only became available later.
         * Has to be called after every action that can introduce a new id to the identifier map.
         * 
         * @param vd Vertex descriptor of the node identified by id
         * @param id Identifier that became available just now
         */
        void register_pending(vertex_descriptor_t vd, identifier_t id) {
            auto pending_it = m_pending_identifiers.find(id);
            while(pending_it != m_pending_identifiers.end()) {
                auto new_id = pending_it->second;
                m_pending_identifiers.erase(pending_it);
                /** TODO: if this returns false there may be a duplicated node */
                register_vertex(vd, new_id);
            }
        }

        boost_graph_t m_graph;
        std::unordered_map<identifier_t,vertex_descriptor_t> m_identifier_map;
        std::unordered_multimap<identifier_t,identifier_t> m_pending_identifiers;
    };

    using graph_t = Graph;
}

//using yloc::graph_t; /** FIXME: do not use yloc::graph_t by default **/

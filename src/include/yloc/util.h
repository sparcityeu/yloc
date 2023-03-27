#pragma once

#include <yloc/affinity.h>
#include <yloc/graph.h>

#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/graphviz.hpp>      // write_graphviz
#include <boost/property_map/property_map.hpp>

#include <functional> // move to implementation
#include <iostream>
#include <sstream>
#include <string>

namespace yloc
{
    vertex_descriptor_t lowest_containing_vertex(AffinityMask &mask, vertex_descriptor_t vertex)
    {
        Graph &g = yloc::root_graph();
        auto fg = boost::filtered_graph(
            g, [&](edge_descriptor_t edge) { return g[edge].type == edge_type::CHILD; }, boost::keep_all{});

        // loop over child edges and return result of child vertex, if child vertex fully contains the mask
        auto out_it = boost::out_edges(vertex, fg).first;
        auto out_end = boost::out_edges(vertex, fg).second;
        for (; out_it != out_end; ++out_it) {
            edge_descriptor_t e = *out_it;
            vertex_descriptor_t targ = boost::target(e, fg);

            auto target_mask = g[targ].get<AffinityMask>("cpu_affinity_mask");
            if (target_mask.has_value() && target_mask.value().is_containing(mask)) {
                return lowest_containing_vertex(mask, targ);
            }
        }
        return vertex;
    }

    vertex_descriptor_t lowest_containing_vertex(AffinityMask &mask)
    {
        vertex_descriptor_t root_vertex = yloc::root_graph().get_root_vertex();
        return lowest_containing_vertex(mask, root_vertex);
    }

    /* helper function because boost::num_vertices<GraphView> returns the number of vertices in the underlying graph.
       the number of vertices contained in a graph view must be calculated */
    template <class GraphView>
    size_t num_vertices_view(const GraphView &gv)
    {
        size_t num_vertices = 0u;
        for (auto vi = boost::vertices(gv).first; vi != boost::vertices(gv).second; vi++) {
            num_vertices++;
        }
        return num_vertices;
    }

    template <class Graph, class Descriptor>
    void print_property(const Graph &g, Descriptor &d, const char *property)
    {
        auto p = g[d].template get<std::string>(property);
        std::cout << property << "=";
        if (p.has_value()) {
            std::cout << p.value() << '\n';
        } else {
            std::cout << "has no value\n";
        }
    }

    template <class Graph>
    void write_graph_dot_file(const Graph &g, std::string dot_file_name, std::vector<std::string> vertex_properties)
    {
        std::ofstream ofs{dot_file_name};

        // we need to define how to transform the vertices/edges to string labels.
        // implemented using make_transform_value_property_map() before creating a label writer
        auto vpmt = boost::make_transform_value_property_map(
            [&](yloc::vertex_descriptor_t vd) {
                std::stringstream ss;
                ss << g[vd].to_string() + "\nVD=" + std::to_string(vd) << '\n';
                for (auto &property : vertex_properties) {
                    auto p = g[vd].template get<std::string>(property);
                    if (p.has_value()) {
                        ss << property << "=" << p.value() << '\n';
                    }
                }
                return ss.str();
            },
            boost::get(boost::vertex_index, g));

        auto epmt = boost::make_transform_value_property_map(
            [&](yloc::edge_type edgetype) {
                std::stringstream ss;
                ss << ((edgetype == edge_type::PARENT) ? "parent" : "child");
                return ss.str();
            },
            boost::get(&yloc::Edge::type, g));

        boost::write_graphviz(ofs, g, boost::make_label_writer(vpmt), boost::make_label_writer(epmt));
    }
}

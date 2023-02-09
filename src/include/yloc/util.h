#pragma once

#include <yloc/graph.h>


#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/graphviz.hpp>      // write_graphviz
#include <boost/property_map/property_map.hpp>



namespace yloc {
    // template <class G>
    // void write_graph_dot_file(G &g, std::string dot_file_name, std::vector<std::string> edge_properties);

    // template <class G>
    // void write_graph_dot_file(G &g, std::string dot_file_name, std::vector<std::string> vertex_properties, std::vector<std::string> edge_properties);

    template <class G>
    void write_graph_dot_file(G &g, std::string dot_file_name, std::vector<std::string> vertex_properties)
    {
        std::ofstream ofs{dot_file_name};

        // we need to define how to transform the vertices/edges to string labels.
        // implemented using make_transform_value_property_map() before creating a label writer
        auto vpmt = boost::make_transform_value_property_map(
            [&](yloc::vertex_descriptor_t vd) {
                std::stringstream ss;
                ss << g[vd].to_string() + "\nVD=" + std::to_string(vd) << '\n';
                for (auto &property : vertex_properties) {
                    auto p = g[vd].get(property);
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

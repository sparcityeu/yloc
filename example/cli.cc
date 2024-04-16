#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/graphviz.hpp>      // write_graphviz
#include <getopt.h>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <yloc/modules/adapter.h>
#include <thread>

#include "yloc/yloc.h"
#include "cli.h"

using namespace yloc;

bool dynamic_probing_flag = false;
int probing_interval = DEFAULT_PROBING_INTERVAL;
int probing_period;

void print_modules()
{
    auto modules = list_modules();

    if (!empty(modules)) {
        std::cout << "Available modules:\n";
    }

    for (auto *m : modules) {
        if (m->m_enabled) {
            std::cout << boost::core::demangled_name(typeid(*m)) << "\n";
        }
    }
}

/**
 * Queries the graph vertices for their component types.
 *
 * @param Graph g
 * @param bool print    (if true, result will be printed to stdout)
 * @return
 */
void print_component_types()
{
    std::cout << "Available component-types:\n";

    for (const auto& [k, v] : Component::map()) {
        std::cout << v->to_string() << "\n";
    }
}

/**
 * Queries the graph vertices for their properties.
 *
 * @param Graph g
 * @param bool print    (if true, result will be printed to stdout)
 * @return
 */
std::set<std::string> get_properties(Graph *g, bool print = true)
{
    auto vertices = g->m_vertices;

    std::set<std::string> properties;

    auto yloc_property_map = Adapter::map();
    for (const auto& p : yloc_property_map) {
        properties.insert((std::string)p.first);
    }

    for (const auto& v : vertices) {
        auto adapters = v.m_property.m_adapters;
        for (const auto& a : adapters) {
            auto module_adapter_map = a->module_map();
            for (const auto& p : module_adapter_map) {
                properties.insert((std::string)p.first);
            }
        }
    }

    if (print) {
        if (!empty(properties)) {
            std::cout << "Component-Properties:\n";
            for (const auto& p : properties) {
                std::cout << p << "\n";
            }
        }
    }

    return properties;
}

std::vector<std::string> parse_string(std::string& s, const std::string& delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

template <class Graph, typename VertexPropertiesWriter>
void write_vertex_info_to_csv(const Graph& g, std::ostream& out, VertexPropertiesWriter vpw)
{
    auto timestamp = date_time_in_milliseconds();

    typename boost::graph_traits< Graph >::vertex_iterator i, end;
    for (boost::tie(i, end) = vertices(g); i != end; ++i)
    {
        vpw(out, *i); // print vertex attributes
        out << "," << timestamp << std::endl;
    }
}

template <class Graph, typename VertexPropertiesWriter>
void write_csv(const Graph& g, const std::string& file_name, VertexPropertiesWriter vpw, const std::string& csv_header)
{
    int elapsed_time = 0;

    if (!file_name.empty()) {
        std::ofstream out{file_name};
        out << csv_header;

        if (dynamic_probing_flag) {
            // the user should be able to trigger an infinite loop by explicitly setting the probing_period to -1
            while(elapsed_time < probing_period || probing_period == -1) {
                write_vertex_info_to_csv(g, out, vpw);

                print_file_written(file_name);

                std::this_thread::sleep_for(std::chrono::milliseconds(probing_interval));
                elapsed_time += probing_interval;
            }
        } else {
            write_vertex_info_to_csv(g, out, vpw);
            print_file_written(file_name);
        }
    } else {
        std::ostream& out = std::cout;
        out << csv_header;

        if (dynamic_probing_flag) {
            // the user should be able to trigger an infinite loop by explicitly setting the probing_period to -1
            while(elapsed_time < probing_period || probing_period == -1) {
                write_vertex_info_to_csv(g, out, vpw);

                std::this_thread::sleep_for(std::chrono::milliseconds(probing_interval));

                elapsed_time += probing_interval;
            }
        } else {
            write_vertex_info_to_csv(g, out, vpw);
        }
    }
}

/**
 * Graph will be transformed to tabular comma-separated format and written to out.
 * @param out
 * @param g
 */
template<class Graph>
void write_graph_csv_format(const Graph& g, const std::vector<std::string> &vertex_properties, const std::string &file_name = "")
{
    std::string csv_header = "vertex_id,vertex_type";
    for (auto const& property : vertex_properties) {
        csv_header += ',';
        csv_header += property;
    }
    csv_header += ",timestamp\n";

    // we need to define how to transform the vertices/edges to string labels.
    // implemented using make_transform_value_property_map() before creating a label writer
    auto vpmt = boost::make_transform_value_property_map(
        [&](yloc::vertex_t vd) {
            std::stringstream ss;
            ss << std::to_string(vd) + "," + g[vd].m_type->to_string();
            for (auto &property : vertex_properties) {
                auto p = g[vd].template get<std::string>(property);
                if (p.has_value()) {
                    ss << "," + p.value();
                } else {
                    ss << ",";
                }
            }
            return ss.str();
        },
        boost::get(boost::vertex_index, g));

    write_csv(g, file_name, csv_writer(vpmt), csv_header);
}

template <class Graph, typename VertexPropertiesWriter, typename EdgePropertiesWriter>
void write_dot(const Graph& g, const std::string& file_name, VertexPropertiesWriter vpw, EdgePropertiesWriter epw)
{
    int elapsed_time = 0;

    if (!file_name.empty()) {
        std::ofstream ofs{file_name};

        if (dynamic_probing_flag) {
            // the user should be able to trigger an infinite loop by explicitly setting the probing_period to -1
            while(elapsed_time < probing_period || probing_period == -1) {
                // File is getting open and closed, so that content will be overwritten and not appended.
                if (!ofs.is_open()) {
                    ofs.open(file_name);
                }
                boost::write_graphviz(ofs, g, vpw, epw);
                ofs.close();
                print_file_written(file_name);

                std::this_thread::sleep_for(std::chrono::milliseconds(probing_interval));

                elapsed_time += probing_interval;
            }
        } else {
            boost::write_graphviz(ofs, g, vpw, epw);
            print_file_written(file_name);
        }
    } else {
        if (dynamic_probing_flag) {
            // the user should be able to trigger an infinite loop by explicitly setting the probing_period to -1
            while(elapsed_time < probing_period || probing_period == -1) {
                boost::write_graphviz(std::cout, g, vpw, epw);

                std::this_thread::sleep_for(std::chrono::milliseconds(probing_interval));

                elapsed_time += probing_interval;
            }
        } else {
            boost::write_graphviz(std::cout, g, vpw, epw);
        }
    }
}

template <class Graph>
void write_graph_dot_format(const Graph& g, const std::vector<std::string>& vertex_properties, const std::string& file_name = "")
{
    // we need to define how to transform the vertices/edges to string labels.
    // implemented using make_transform_value_property_map() before creating a label writer
    auto vpmt = boost::make_transform_value_property_map(
        [&](yloc::vertex_t vd) {
            std::stringstream ss;
            ss << g[vd].to_string() + "\nVD=" + std::to_string(vd) << '\n';
            for (auto& property : vertex_properties) {
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
        boost::get(&yloc::Edge::m_edgetype, g));

    write_dot(g, file_name, boost::make_label_writer(vpmt), boost::make_label_writer(epmt));
}

/**
 * Based on the output format, the method call is delegated to specialised methods.
 *
 * @tparam Graph
 * @param g
 * @param file_name     optional
 * @param vertex_properties optional
 */
template <class Graph>
void write_graph(const Graph& g, const std::vector<std::string>& vertex_properties = std::vector<std::string>{"memory", "numa_affinity"}, const std::string& output_format = "dot", const std::string& file_name = "")
{
    if (output_format == "dot") {
        write_graph_dot_format(g, vertex_properties, file_name);
    } else if (output_format == "csv") {
        write_graph_csv_format(g, vertex_properties, file_name);
    }
}

bool is_valid_format(const std::string& output_format)
{
    return std::find(VALID_OUTPUT_FORMATS.begin(), VALID_OUTPUT_FORMATS.end(), output_format)
                    != VALID_OUTPUT_FORMATS.end();
}

int main(int argc, char *argv[])
{
    yloc::init();

    Graph &g = yloc::root_graph();

    std::string component_filter_string, properties_filter_string;
    std::string output_file, output_file_extension, output_format;
    std::vector<std::string> file_parts;
    std::vector<std::string> properties_to_filter;
    bool filter_components_flag = false;

    // std::vector<std::string> vertex_properties = DEFAULT_VECTOR_PROPERTIES;
    auto all_vertex_properties = get_properties(&g, false);
    // Default
    std::vector<std::string> vertex_properties = std::vector<std::string>(all_vertex_properties.begin(), all_vertex_properties.end());

    int argument;
    int option_index = 0;
    while ((argument = getopt_long(argc, argv, "hHMCPOc:p:d:i:o:f:", long_options, &option_index)) != -1) {
        switch (argument) {
            case 'h':
            case 'H':
                show_help();
                return YLOC_STATUS_SUCCESS;
            case 'M':
                print_modules();
                return YLOC_STATUS_SUCCESS;
            case 'C':
                print_component_types();
                return YLOC_STATUS_SUCCESS;
            case 'P':
                get_properties(&g);
                return YLOC_STATUS_SUCCESS;
            case 'O':
                show_output_formats();
                return YLOC_STATUS_SUCCESS;
            case 'c':
                component_filter_string = optarg;
                filter_components_flag = true;
                break;
            case 'p':
                properties_filter_string = optarg;
                properties_to_filter = parse_string(properties_filter_string, ",");
                break;
            case 'd':
                dynamic_probing_flag = true;
                probing_period = (int) std::strtol(optarg, nullptr, 10);
                break;
            case 'i':
                probing_interval = (int) std::strtol(optarg, nullptr, 10);
                break;
            case 'o':
                output_file = optarg;
                file_parts = parse_string(output_file, ".");
                if (file_parts.size() > 1) {
                    auto file_extension = file_parts.back();
                    if (is_valid_format(file_extension)) {
                        output_file_extension = file_extension;
                    }
                }
                break;
            case 'f':
                output_format = optarg;
                if (!is_valid_format(output_format)) {
                    std::cerr << "Format [" << output_format << "] is not supported. Check option -O for supported output formats.\n";
                    return YLOC_STATUS_INVALID_ARGS;
                }
                break;
            default:
                std::cerr << "Incorrect Command-line parameter given.\n\n";
                show_help();
                return YLOC_STATUS_INVALID_ARGS;
        }
    }

    if (output_format.empty()) {
        /*
         * if output_format has not been specified with option -f,
         * the output_format should follow the output_file_extension (if given)
         * or default to "dot"
         */
        output_format = !output_file_extension.empty() ? output_file_extension : "dot";
    }

    if (!properties_to_filter.empty()) {
        auto properties = get_properties(&g, false);

            for (const auto& property_to_filter : properties_to_filter) {
                bool invalid_property_found = false;
                if (properties.find(property_to_filter) == properties.end()) {
                    invalid_property_found = true;
                    std::cerr << "Property [" << property_to_filter << "] is not available.\n";
                }
                if (invalid_property_found) {
                    std::cerr << "See `yloc-cli -P` for a list of available properties.\n";
                    return YLOC_STATUS_NOT_FOUND;
                }
        }
        // range constructor to get vector from set
        vertex_properties = std::vector<std::string>(properties_to_filter.begin(), properties_to_filter.end());
    }

    // Write graph
    if (filter_components_flag) {

        // filter_string can contain multiple component types separated by commas
        std::vector<std::string> components_to_filter = parse_string(component_filter_string, ",");

        // Try monolithic version
        const auto &component_types = yloc::Component::map();
        bool components_valid = true;

        // plausibility check of the components_to_filter argument
        for (const auto &component_to_filter : components_to_filter) {
            bool component_valid = component_types.find(component_to_filter) != component_types.end();
            // it is checked, whether any of the component_types actually contains the component name to filter by
            if (!component_valid) {
                components_valid = false;
                std::cerr << "Component type [" << component_to_filter << "] is not available.\n";
            }
        }

        if(!components_valid) {
            std::cerr << "See `yloc-cli -C` for a list of available component types.\n";
            return YLOC_STATUS_NOT_FOUND;
        }

        // use predicate (f(v) -> bool) to filter the graph by component type
        /*auto*/ std::function<bool(const vertex_t &)> vertex_predicate = [&](const vertex_t &v) -> bool {
            // filter if any of the component types contains a given string, e.g. Cache in L1DataCache etc.
            return std::any_of(components_to_filter.begin(), components_to_filter.end(), [&](auto component_str) -> bool {
                return component_types.find(component_str)->second->test_component(g[v].m_type);
            });
        };
        // all edges will be kept
        /*auto*/ std::function<bool(const edge_t &)> edge_predicate = [&](const edge_t &v) -> bool {
            return true;
            };

        auto fgv = boost::make_filtered_graph(g, edge_predicate, vertex_predicate);

        write_graph(fgv, vertex_properties, output_format, output_file);
    } else {
        write_graph(g, vertex_properties, output_format, output_file);
    }

    yloc::finalize();
    return YLOC_STATUS_SUCCESS;
}
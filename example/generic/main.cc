#include <fstream>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/graphviz.hpp>      // write_graphviz
#include <boost/property_map/property_map.hpp>

/** TODO: create combined header */
//#include <yloc.h>
#include "graph.h"
#include "init.h"

using namespace yloc;

/* writes graph with labels to file */
static void write_graph_dot_file(graph_t &g, std::string dot_file_name)
{
    std::ofstream ofs{dot_file_name};
    // we need to define how to transform the vertices/edges to string labels.
    // implemented using make_transform_value_property_map() before creating a label writer
    auto vpmt = boost::make_transform_value_property_map(
        [&](yloc::vertex_descriptor_t vd) {
            return g[vd].to_string() + "\nVD=" + std::to_string(vd);
        },
        boost::get(boost::vertex_index, g.boost_graph()));

    auto epmt = boost::make_transform_value_property_map(
        [&](yloc::edge_type edgetype) { return edgetype == yloc_edge_type::YLOC_EDGE_TYPE_PARENT ? "parent" : "child"; },
        boost::get(&yloc::Edge::type, g.boost_graph()));

    boost::write_graphviz(ofs, g.boost_graph(), boost::make_label_writer(vpmt), boost::make_label_writer(epmt));
}

/* example of a filter graph query */
static void filter_graph_example(graph_t &g)
{
    // use predicate (f(v) -> bool) to filter the graph by object type "Cache"
    /*auto*/ std::function<bool(const vertex_descriptor_t &)> predicate = [&](const vertex_descriptor_t &v) -> bool {
        return g[v].type->is_a<Cache>();
    };

    // edges are filtered by predicate "boost::keep_all{}" (keeping all edges)
    // vertices are filtered by predicate "predicate"
    auto fgv = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, predicate);

    // print vertices of resulting filtered graph view:
    std::cout << "writing filtered graph to filtered_graph.dot" << std::endl;
    // std::for_each(vertices(fgv).first, vertices(fgv).second, [&](const vertex_descriptor_t &v) {
        // std::cout << "VD=" << v << std::endl
                //   << g[v].to_string() << std::endl;
    // });

    // write_graph_dot_file(fgv, "filtered_graph.dot");

    std::ofstream ofs{"filtered_graph.dot"};
    auto vpmt = boost::make_transform_value_property_map(
        [&](yloc::vertex_descriptor_t vd) {
            return fgv[vd].to_string() + "\nVD=" + std::to_string(vd);
        },
        boost::get(boost::vertex_index, fgv));

    auto epmt = boost::make_transform_value_property_map(
        [&](yloc::edge_type edgetype) { return edgetype == yloc_edge_type::YLOC_EDGE_TYPE_PARENT ? "parent" : "child"; },
        boost::get(&yloc::Edge::type, fgv));

    boost::write_graphviz(ofs, fgv, boost::make_label_writer(vpmt), boost::make_label_writer(epmt));
}

/* helper function because boost::num_vertices<GraphView> returns the number of vertices in the underlying graph.
   the number of vertices contained in a graph view must be calculated */
template <typename GraphView>
size_t num_vertices_view(const GraphView &gv)
{
    size_t num_vertices = 0;
    for (auto vi = boost::vertices(gv).first; vi != boost::vertices(gv).second; vi++) {
        num_vertices++;
    }
    return num_vertices;
}

/* example of finding distances from one vertex to others */
static void find_distances(graph_t &g)
{
    // this example finds the distances (#hops as metric) from a (the first) co-processor to all PU's
    // and prints the PU's with the lowest distance.
    //
    // note: hwloc offers functionality e.g. in hwloc_opencl_get_device_cpuset()
    //       to get the closest cpu's to an opencl device

    // first we query the graph for ...
    auto predicate_accelerator = [&](const vertex_descriptor_t &v) -> bool {
        return g[v].type->is_a<Accelerator>();
    };
    auto fgv_accelerator = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, predicate_accelerator);

    size_t num_vertices = num_vertices_view(fgv_accelerator);
    std::cout << "number of found accelerators: " << num_vertices_view(fgv_accelerator) << std::endl;
    if (num_vertices < 1) {
        return;
    }

    auto vi = boost::vertices(fgv_accelerator).first;
    std::cout << "first accelerator:" << std::endl
              << g[*vi].to_string() << std::endl;

    // then we filter the graph to get all PU's
    auto predicate_pu = [&](const vertex_descriptor_t &v) -> bool {
        return g[v].type->is_a<LogicalCore>();
    };
    auto fgv_pu = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, predicate_pu);
    std::cout << "number of Cores: " << num_vertices_view(fgv_pu) << std::endl;

    // here we use a vector as the underlying container to the property map for the distances,
    // therefore we must use an iterator property map here.
    //
    // note: there must be space allocated for every vertex descriptor of the underlying graph
    //       using the default boost property map adaptor.
    std::vector<int> distances(boost::num_vertices(fgv_pu));
    auto dist_pmap = boost::make_iterator_property_map(distances.begin(), get(boost::vertex_index, fgv_pu));

    // boost::on_tree_edge() adds 1 per edge, could be any distance metric
    auto vis = boost::make_bfs_visitor(boost::record_distances(dist_pmap, boost::on_tree_edge()));
    boost::breadth_first_search(g.boost_graph(), *vi, visitor(vis));

    // get the minimum distance to the PU's
    auto mindist = boost::get(dist_pmap, *boost::vertices(fgv_pu).first);
    for (auto vd : boost::make_iterator_range(boost::vertices(fgv_pu))) {
        // boost::get(dist_pmap, vd) accesses distances[vd] via adaptor
        if (boost::get(dist_pmap, vd) < mindist) {
            mindist = boost::get(dist_pmap, vd);
        }
    }
    std::cout << "minimum distance: " << mindist << std::endl;

    // now filter out the PU's that have higher distance
    auto predicate_pu_min = [&](const vertex_descriptor_t &v) -> bool {
        return predicate_pu(v) && !(boost::get(dist_pmap, v) > mindist);
    };

    // TODO: using lambdas as predicates, the graph view cannot be further filtered because
    // lambda closure type has a deleted copy assignment operator.
    // using std::function instead of a lambda might enable queries on the graph view
    auto fgv_pu_min = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, predicate_pu_min);

    for (auto vd : boost::make_iterator_range(boost::vertices(fgv_pu_min))) {
        std::cout << "#hops [" << *vi << " -> " << vd << "] = " << distances[vd] << "\n";
        break;
    }
}

int main(int argc, char *argv[])
{
    // yloc::set_options/configure/...

    // MPI_Init(&argc, &argv);
    yloc::init(YLOC_FULL | YLOC_ONGOING);

    graph_t & g = yloc::root_graph();

    write_graph_dot_file(g, std::string{"graph.dot"});
    filter_graph_example(g);

    find_distances(g);

    yloc::finalize();
    // MPI_Finalize();

    return EXIT_SUCCESS;
}

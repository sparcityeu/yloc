#pragma once

#include <yloc/graph.h>

namespace yloc
{
    class Query
    {
    };

    boost_graph_t filtered_graph()
    {
        auto predicate_accelerator = [&](const vertex_descriptor_t &v) -> bool {
            return g[v].type->is_a<Accelerator>();
        };
        auto fgv_accelerator = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, predicate_accelerator);
    }

    void distance(vertex_descriptor_t &vd1, vertex_descriptor_t &vd1)
    {
        auto vi = boost::vertices(fgv_accelerator).first;

        size_t num_vertices = num_vertices_view(fgv_accelerator);
        std::cout << "number of found accelerators: " << num_vertices_view(fgv_accelerator) << std::endl;
        if (num_vertices < 1) {
            return;
        }
        // here we use a vector as the underlying container to the property map for the distances,
        // therefore we must use an iterator property map here.
        //
        // note: there must be space allocated for every vertex descriptor of the underlying graph
        //       using the default boost property map adaptor.
        std::vector<int> distances(boost::num_vertices(g1));
        auto dist_pmap = boost::make_iterator_property_map(distances.begin(), boost::get(boost::vertex_index, g1));

        // boost::on_tree_edge() adds 1 per edge, could be any distance metric
        auto vis = boost::make_bfs_visitor(boost::record_distances(dist_pmap, boost::on_tree_edge()));
        boost::breadth_first_search(g.boost_graph(), *vi, visitor(vis));

        // get the minimum distance to the PU's
        auto mindist = boost::get(dist_pmap, *boost::vertices(g1).first);
        for (auto vd : boost::make_iterator_range(boost::vertices(g1))) {
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

    template <class G1, class G2>
    void find_distances(G1 &g1, G2 &g2)
    {
        size_t num_vertices = num_vertices_view(fgv_accelerator);
        std::cout << "number of found accelerators: " << num_vertices_view(fgv_accelerator) << std::endl;
        if (num_vertices < 1) {
            return;
        }
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
}

/* example of finding distances from one vertex to others */
static void find_distances(Graph &g)
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
}
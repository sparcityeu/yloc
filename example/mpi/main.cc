#include <vector>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <mpi.h>

#include <yloc/yloc.h>

using namespace std::string_literals;

// this example finds the distances from mpi rank vertices in filtered graph to other vertices in graph
static std::vector<std::pair<int, std::vector<int>>> find_distances(yloc::Graph &graph, std::function<bool(const yloc::vertex_descriptor_t &)> predicate)
{
    auto graph_view = boost::make_filtered_graph(graph.boost_graph(), boost::keep_all{}, predicate);

    size_t num_vertices_view = yloc::num_vertices_view(graph_view);
    std::cout << "number of MPI processes: " << num_vertices_view << std::endl;
    if (num_vertices_view < 1) {
        return {};
    }

    int i = 0;
    std::vector<std::pair<int, std::vector<int>>> distance_matrix(num_vertices_view);

    std::for_each(boost::vertices(graph_view).first, boost::vertices(graph_view).second, [&](const yloc::vertex_descriptor_t &vd) {
        distance_matrix[i] = {graph[vd].get("mpi_rank").value(), std::vector<int>(boost::num_vertices(graph.boost_graph()))};
        auto dist_pmap = boost::make_iterator_property_map(distance_matrix[i].second.begin(), boost::get(boost::vertex_index, graph.boost_graph()));
        auto visitor = boost::make_bfs_visitor(boost::record_distances(dist_pmap, boost::on_tree_edge()));
        boost::breadth_first_search(graph.boost_graph(), vd, boost::visitor(visitor));
        ++i;
    });

    return distance_matrix;
}

// this example finds the distances (#hops as metric) between MPI processes
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    yloc::init(YLOC_FULL | YLOC_ONGOING);

    yloc::Graph &g = yloc::root_graph();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int p = 0;

    std::function<bool(const yloc::vertex_descriptor_t &)> predicate = [&](const yloc::vertex_descriptor_t &v) -> bool {
        return g[v].type->is_a<yloc::MPIProcess>();
    };
    auto graph_view = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, predicate);

    if (rank == 0) {
        auto distance_matrix = find_distances(g, predicate);
        std::for_each(distance_matrix.begin(), distance_matrix.end(), [&](const auto &pair) {
            std::cout << "distance from mpi rank " << pair.first << '\n';
            for(const auto &vd : boost::make_iterator_range(boost::vertices(graph_view))) {
                std::cout << "to " << g[vd].get("mpi_rank").value() << ": " << pair.second[vd] << '\n';
            }
        });

        yloc::write_graph_dot_file(g, "mpi_graph.dot"s, std::vector{"mpi_rank"s});
    }

    yloc::finalize();
    MPI_Finalize();

    return EXIT_SUCCESS;
}

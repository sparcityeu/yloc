#include <vector>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <mpi.h>

#include <yloc/yloc.h>

using namespace std::string_literals;

static std::vector<std::vector<int>> find_distances(yloc::Graph &g)
{
    // this example finds the distances (#hops as metric) between MPI processes
    std::function<bool(const yloc::vertex_descriptor_t &)> predicate = [&](const yloc::vertex_descriptor_t &v) -> bool {
        return g[v].type->is_a<yloc::MPIProcess>();
    };
    auto graph_view = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, predicate);

    size_t num_vertices = yloc::num_vertices_view(graph_view);
    std::cout << "number of MPI processes: " << num_vertices << std::endl;
    if (num_vertices < 1) {
        return {};
    }

    int i = 0;
    std::vector<std::vector<int>> distance_matrix(num_vertices);

    std::for_each(boost::vertices(graph_view).first, boost::vertices(graph_view).second, [&](const yloc::vertex_descriptor_t &v) {
        distance_matrix[i] = std::vector<int>(num_vertices);
        auto dist_pmap = boost::make_iterator_property_map(distance_matrix[i].begin(), get(boost::vertex_index, graph_view));
        auto visitor = boost::make_bfs_visitor(boost::record_distances(dist_pmap, boost::on_tree_edge()));
        boost::breadth_first_search(graph_view, v, boost::visitor(visitor));
    });

    return distance_matrix;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    yloc::init(YLOC_FULL | YLOC_ONGOING);

    yloc::Graph &g = yloc::root_graph();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::vector<std::vector<int>> distance_matrix = find_distances(g);
        std::for_each(distance_matrix.begin(), distance_matrix.end(), [&](auto d) {
            std::for_each(d.begin(), d.end(), [=](int i) { std::cout << i << '\n'; });
        });

        yloc::write_graph_dot_file(g, "mpi_graph.dot"s, std::vector{"mpi_rank"s});
    }

    yloc::finalize();
    MPI_Finalize();

    return EXIT_SUCCESS;
}

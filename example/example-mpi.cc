#include <vector>
#include <fstream>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/visitors.hpp>
#include <mpi.h>

#include <yloc/yloc.h>

using namespace std::string_literals;

// this example finds the distances (#hops as metric) between MPI processes
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    if (yloc::init() != YLOC_STATUS_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    yloc::Graph &g = yloc::root_graph();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto graph_view = boost::make_filtered_graph(
        g, boost::keep_all{}, [&](yloc::vertex_t v) { return g[v].is_a<yloc::MPIProcess>(); });

    if (rank == 0) {
        for (auto v1 : yloc::vertex_range(graph_view)) {
            auto dist = yloc::bfs_distance_vector(g, v1);
            std::cout << "distance from mpi rank " << g[v1].get("mpi_rank").value() << '\n';
            for (auto v2 : yloc::vertex_range(graph_view)) {
                std::cout << "to " << g[v2].get("mpi_rank").value() << ": " << dist[v2] << '\n';
            }
        }
        std::ofstream out_file{"mpi_graph.dot"s};
        yloc::write_graph_dot(g, out_file, std::vector{"mpi_rank"s});
    }

    yloc::finalize();
    MPI_Finalize();

    return EXIT_SUCCESS;
}

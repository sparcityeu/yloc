#include <vector>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <mpi.h>

#include <yloc/yloc.h>

using namespace std::string_literals;

// this example finds the distances (#hops as metric) between MPI processes
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    assert(yloc::init() == YLOC_STATUS_SUCCESS);

    yloc::Graph &g = yloc::root_graph();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto graph_view = boost::make_filtered_graph(g, boost::keep_all{}, [&](yloc::vertex_descriptor_t v) {
        return g[v].type->is_a<yloc::MPIProcess>();
    });

    if (rank == 0) {
        for (auto v1 : boost::make_iterator_range(boost::vertices(graph_view))) {
            auto dist = yloc::bfs_distance_vector(v1);
            std::cout << "distance from mpi rank " << g[v1].get<uint64_t>("mpi_rank").value() << '\n';
            for (auto v2 : boost::make_iterator_range(boost::vertices(graph_view))) {
                std::cout << "to " << g[v2].get<uint64_t>("mpi_rank").value() << ": " << dist[v2] << '\n';
            }
        }

        yloc::write_graph_dot_file(g, "mpi_graph.dot"s, std::vector{"mpi_rank"s});
    }

    yloc::finalize();
    MPI_Finalize();

    return EXIT_SUCCESS;
}

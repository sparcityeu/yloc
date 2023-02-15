#include <vector>

#include <mpi.h>

#include <yloc/yloc.h>

using namespace std::string_literals;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    yloc::init(YLOC_FULL | YLOC_ONGOING);

    yloc::Graph &g = yloc::root_graph();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        yloc::write_graph_dot_file(g, "mpi_graph.dot"s, std::vector{"mpi_rank"s});
    }

    yloc::finalize();
    MPI_Finalize();

    return EXIT_SUCCESS;
}

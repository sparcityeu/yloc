#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <yloc/yloc.h>

using namespace yloc;

int main(int argc, char *argv[])
{
    // yloc::set_options/configure/...
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <dot file> [list of properties]\n";
    }

    // MPI_Init(&argc, &argv);
    yloc::init(YLOC_FULL | YLOC_ONGOING);

    Graph &g = yloc::root_graph();

    std::ofstream ofs{argv[1]};

    // write_graph_dot_file(g, ofs, argv, argc);

    yloc::finalize();
    // MPI_Finalize();

    return EXIT_SUCCESS;
}

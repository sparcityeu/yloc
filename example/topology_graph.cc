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
        return EXIT_FAILURE;
    }

    // MPI_Init(&argc, &argv);
    yloc::init();

    Graph &g = yloc::root_graph();

    std::ofstream ofs{argv[1]};
    std::vector<std::string> properties(argv+2,argv+argc);

    write_graph_dot(g, ofs, properties);

    yloc::finalize();
    // MPI_Finalize();

    return EXIT_SUCCESS;
}

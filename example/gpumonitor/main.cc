#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/graphviz.hpp>      // write_graphviz
#include <boost/property_map/property_map.hpp>
#include <fstream>

/** TODO: create combined header */
//#include <yloc.h>
#include "graph_object.h"
#include "graph_type.h"
#include "init.h"

using namespace yloc;

#define PRINT_PROPERTY(property) std::cout << #property << "=" << YLOC_GET(g, vd, property).value() << '\n'

int main(int argc, char *argv[])
{
    // yloc::set_options/configure/...

    yloc::init(YLOC_FULL | YLOC_ONGOING);

    graph_t &g = yloc::root_graph();

    auto fgv = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, [&](const vertex_descriptor_t &v) -> bool {
        return g[v].tinfo.type->is_a<GPU>();
    });

    while (1) {
        for (auto vd : boost::make_iterator_range(boost::vertices(fgv))) {
            // PRINT_PROPERTY(as_string);
            std::cout << "==========================\nGPU [vd=" << vd << "]\n==========================\n";
            PRINT_PROPERTY(memory);
            PRINT_PROPERTY(memory_usage);
            PRINT_PROPERTY(memory_load);
            PRINT_PROPERTY(memory_frequency);
            PRINT_PROPERTY(bdfid);
            PRINT_PROPERTY(numa_affinity);
            PRINT_PROPERTY(temperature);
            PRINT_PROPERTY(frequency);
            PRINT_PROPERTY(load);
            PRINT_PROPERTY(power);
        }
        sleep(1);
    }

    yloc::finalize();

    return EXIT_SUCCESS;
}

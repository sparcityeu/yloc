#include <iostream>
#include <optional>
#include <thread>

#include "boost/graph/filtered_graph.hpp"

/** TODO: create combined header */
//#include <yloc.h>
#include "graph.h"
#include "init.h"

using namespace yloc;

#define PRINT_PROPERTY(property)            \
    do {                                    \
        auto p = YLOC_GET(g, vd, property); \
        std::cout << #property << "=";      \
        if (p.has_value()) {                \
            std::cout << p.value() << '\n'; \
        } else {                            \
            std::cout << "has no value\n";  \
        }                                   \
    } while (0)

int main(int argc, char *argv[])
{
    // yloc::set_options/configure/...

    yloc::init(YLOC_FULL | YLOC_ONGOING);

    graph_t &g = yloc::root_graph();

    auto fgv = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, [&](const vertex_descriptor_t &v) -> bool {
        return g[v].type->is_a<GPU>();
    });

    while (1) {
        size_t gpu = 0;
        for (auto vd : boost::make_iterator_range(boost::vertices(fgv))) {
            // PRINT_PROPERTY(to_string);
            std::cout << "==========================\nGPU" << gpu << " [vd=" << vd << "]\n==========================\n";
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
            // these properties are blocking:
            // PRINT_PROPERTY(pci_throughput);
            // PRINT_PROPERTY(pci_throughput_read);
            // PRINT_PROPERTY(pci_throughput_write);
            gpu++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    yloc::finalize();

    return EXIT_SUCCESS;
}

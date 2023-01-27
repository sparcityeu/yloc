#include <iostream>
#include <optional>
#include <thread>

#include "boost/graph/filtered_graph.hpp"

#include <yloc/yloc.h>

using namespace yloc;

static void print_property(Graph &g, vertex_descriptor_t vd, const char *property)
{
    auto p = g[vd].get(property);
    std::cout << property << "=";
    if (p.has_value()) {
        std::cout << p.value() << '\n';
    } else {
        std::cout << "has no value\n";
    }
}

int main(int argc, char *argv[])
{
    // yloc::set_options/configure/...

    yloc::init(YLOC_FULL | YLOC_ONGOING);

    Graph &g = yloc::root_graph();

    auto fgv = boost::make_filtered_graph(g.boost_graph(), boost::keep_all{}, [&](const vertex_descriptor_t &v) -> bool {
        return g[v].type->is_a<GPU>();
    });

    while (1) {
        size_t gpu = 0;
        for (auto vd : boost::make_iterator_range(boost::vertices(fgv))) {
            // PRINT_PROPERTY(to_string);
            std::cout << "==========================\nGPU" << gpu << " [vd=" << vd << "]\n==========================\n";
            print_property(g, vd, "memory");
            print_property(g, vd, "memory_usage");
            print_property(g, vd, "memory_load");
            print_property(g, vd, "memory_frequency");
            print_property(g, vd, "bdfid");
            print_property(g, vd, "numa_affinity");
            print_property(g, vd, "temperature");
            print_property(g, vd, "frequency");
            print_property(g, vd, "load");
            print_property(g, vd, "power");
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

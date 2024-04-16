#include <iostream>
#include <optional>
#include <thread>

#include "boost/graph/filtered_graph.hpp"

#include <yloc/yloc.h>

int main(int argc, char *argv[])
{
    yloc::init();

    yloc::Graph &g = yloc::root_graph();

    auto fgv =
        boost::make_filtered_graph(g, boost::keep_all{}, [&](yloc::vertex_t v) { return g[v].is_a<yloc::GPU>(); });

    while (1) {
        size_t gpu = 0;
        for (auto vd : yloc::vertex_range(fgv)) {
            std::cout << "==========================\nGPU" << gpu << " [vd=" << vd << "]\n==========================\n";
            yloc::print_property(g, vd, "memory");
            yloc::print_property(g, vd, "memory_usage");
            yloc::print_property(g, vd, "memory_load");
            yloc::print_property(g, vd, "memory_frequency");
            yloc::print_property(g, vd, "bdfid");
            yloc::print_property(g, vd, "numa_affinity");
            yloc::print_property(g, vd, "temperature");
            yloc::print_property(g, vd, "frequency");
            yloc::print_property(g, vd, "load");
            yloc::print_property(g, vd, "power");
            // these properties are blocking:
            // yloc::print_property(g, vd, "pci_throughput");
            // yloc::print_property(g, vd, "pci_throughput_read");
            // yloc::print_property(g, vd, "pci_throughput_write");
            gpu++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    yloc::finalize();

    return EXIT_SUCCESS;
}

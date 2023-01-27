
#include <mpi.h>
#include <string.h>

#include <yloc/graph/graph.h>
#include <yloc/modules/adapter.h>
#include <yloc/modules/module.h>
#include <yloc/yloc_status.h>

#include "interface_impl.h"
#include "supermuc_adapter.h"

using namespace yloc;

/**
 * @brief Build boost graph structure from hostnames of supermuc.
 *
 * @param g The graph to add the entries to
 * @param hostname Own hostname
 * @param hostname_length Length of hostname
 */
static void make_supermuc_graph(Graph &g, vertex_descriptor_t vd_local_node, char *hostname, int hostname_length)
{
    /** TODO: check for structure of hostnames as expected for supermuc */
    assert(hostname_length < 32);

    int rank, nbproc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nbproc);

    char *recv_buffer = new char[32 * nbproc];
    char(*hostnames)[32] = (char(*)[32])(recv_buffer);

    // necessary?
    // strncpy(hostnames[rank],hostname,32);

    MPI_Allgather(hostname, 32, MPI_CHAR, recv_buffer, 32, MPI_CHAR, MPI_COMM_WORLD);

    for (int i = 0; i < nbproc; i++) {
        vertex_descriptor_t island_vd;
        vertex_descriptor_t rack_vd;
        vertex_descriptor_t column_vd;
        vertex_descriptor_t slot_vd;

#ifdef READABLE_CODE
        char island[4];
        strncpy(island, hostnames[i], 3);
        island[3] = '\0';
        island_vd = g.add_vertex("system:" + std::string{island});

        char rack[7];
        strncpy(rack, hostnames[i], 6);
        rack[6] = '\0';
        rack_vd = g.add_vertex("system:" + std::string{rack});

        char slot[10];
        strncpy(slot, hostnames[i], 9);
        slot[9] = '\0';
        column_vd = g.add_vertex("system:" + std::string{slot});

        char *node = hostnames[i];
        if (rank == i) {
            slot_vd = vd_local_node;
        } else {
            slot_vd = g.add_vertex("system:" + std::string{node});
        }
#else
        if (rank == i) {
            slot_vd = vd_local_node;
        } else {
            slot_vd = g.add_vertex("system:" + std::string{hostnames[i]});
        }
        g[slot_vd].add_adapter(new SuperMUCAdapter{hostnames[i]});

        // slot ends after 6 characters
        hostnames[i][9] = '\0';
        column_vd = g.add_vertex("system:" + std::string{hostnames[i]});
        g[column_vd].add_adapter(new SuperMUCAdapter{hostnames[i]});

        // rack ends after 4 characters
        hostnames[i][6] = '\0';
        rack_vd = g.add_vertex("system:" + std::string{hostnames[i]});
        g[rack_vd].add_adapter(new SuperMUCAdapter{hostnames[i]});

        // island ends after 2 characters
        hostnames[i][3] = '\0';
        island_vd = g.add_vertex("system:" + std::string{hostnames[i]});
        g[island_vd].add_adapter(new SuperMUCAdapter{hostnames[i]});
#endif

        /** TODO: Move this logic to g.add_edge-method */

        // Check if there is already an edge between the nodes
        if (!boost::edge(island_vd, rack_vd, g.boost_graph()).second) {
            /** TODO: Filter for only child/parent edges */
            boost::add_edge(island_vd, rack_vd, Edge{edge_type::PARENT}, g.boost_graph());
            boost::add_edge(rack_vd, island_vd, Edge{edge_type::CHILD}, g.boost_graph());
        }

        // Check if there is already an edge between the nodes
        if (!boost::edge(rack_vd, column_vd, g.boost_graph()).second) {
            /** TODO: Filter for only child/parent edges */
            boost::add_edge(rack_vd, column_vd, Edge{edge_type::PARENT}, g.boost_graph());
            boost::add_edge(column_vd, rack_vd, Edge{edge_type::CHILD}, g.boost_graph());
        }

        // Check if there is already an edge between the nodes
        if (!boost::edge(column_vd, slot_vd, g.boost_graph()).second) {
            /** TODO: Filter for only child/parent edges */
            boost::add_edge(column_vd, slot_vd, Edge{edge_type::PARENT}, g.boost_graph());
            boost::add_edge(slot_vd, column_vd, Edge{edge_type::CHILD}, g.boost_graph());
        }
    }
}

yloc_status_t ModuleSuperMUC::init_graph(Graph &g)
{
    int initialized;
    MPI_Initialized(&initialized);
    if (!initialized) {
        return YLOC_STATUS_INIT_ERROR;
    }

    char hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_length;
    MPI_Get_processor_name(hostname, &hostname_length);
    // hostname[hostname_length] = '\0'; // ??

    /** TODO: Only works if init_graph of hwloc is called before this one */
    auto vd_local_node = g.add_vertex("machine:" + std::string{std::getenv("HOSTNAME")});

    make_supermuc_graph(g, vd_local_node, hostname, hostname_length);

    return YLOC_STATUS_SUCCESS;
}

#include <yloc/yloc.h>

#include "interface_impl.h"
#include "supermuc_adapter.h"

#include <mpi.h>

#include <memory>
#include <string>

#include <cstdio>

using namespace yloc;

static vertex_descriptor_t make_supermuc_node(Graph &g, const char *name)
{
    vertex_descriptor_t vd = g.add_vertex("system:" + std::string{name});
    g[vd].m_type = Misc::ptr(); /** TODO: add component type for (logical) system interconnect components */
    g[vd].m_description = std::string{name};
    g[vd].add_adapter(std::make_shared<SuperMUCAdapter>(name));
    return vd;
}

/**
 * @brief Build boost graph structure from hostnames of supermuc.
 *
 * @param g The graph to add the entries to
 * @param hostname Own hostname
 * @param hostname_length Length of hostname
 */
static void make_supermuc_graph(Graph &g, const char *hostname)
{
    int rank, nbproc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nbproc);

    char hostnames[nbproc][32];

    MPI_Allgather(hostname, 32, MPI_CHAR, hostnames, 32, MPI_CHAR, MPI_COMM_WORLD);

    for (int i = 0; i < nbproc; i++) {
        vertex_descriptor_t island_vd;
        vertex_descriptor_t rack_vd;
        vertex_descriptor_t column_vd;
        vertex_descriptor_t slot_vd;

        // hostname: island,rack,column,slot
        slot_vd = g.add_vertex("machine:" + std::string{hostnames[i]});
        g[slot_vd].m_type = Node::ptr();
        g[slot_vd].m_description = std::string{hostnames[i]};
        g[slot_vd].add_adapter(std::make_shared<SuperMUCAdapter>(hostnames[i]));

        hostnames[i][9] = '\0'; // island,rack,column
        column_vd = make_supermuc_node(g, hostnames[i]);

        hostnames[i][6] = '\0'; // island,rack
        rack_vd = make_supermuc_node(g, hostnames[i]);

        hostnames[i][3] = '\0'; // island
        island_vd = make_supermuc_node(g, hostnames[i]);

        /** TODO: Move this logic to g.add_edge-method */

        // Check if there is already an edge between the nodes
        if (!boost::edge(island_vd, rack_vd, g).second) {
            /** TODO: Filter for only child/parent edges */
            boost::add_edge(island_vd, rack_vd, Edge{edge_type::CHILD}, g);
            boost::add_edge(rack_vd, island_vd, Edge{edge_type::PARENT}, g);
        }

        // Check if there is already an edge between the nodes
        if (!boost::edge(rack_vd, column_vd, g).second) {
            /** TODO: Filter for only child/parent edges */
            boost::add_edge(rack_vd, column_vd, Edge{edge_type::CHILD}, g);
            boost::add_edge(column_vd, rack_vd, Edge{edge_type::PARENT}, g);
        }

        // Check if there is already an edge between the nodes
        if (!boost::edge(column_vd, slot_vd, g).second) {
            /** TODO: Filter for only child/parent edges */
            boost::add_edge(column_vd, slot_vd, Edge{edge_type::CHILD}, g);
            boost::add_edge(slot_vd, column_vd, Edge{edge_type::PARENT}, g);
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

    // check whether hostname structure is same as supermuc's naming convention
    const int supermuc_hostname_len = 12;
    if (hostname_length != supermuc_hostname_len) {
        std::cerr << "hostname length does not match supermuc naming convention\n";
        return YLOC_STATUS_INIT_ERROR;
    }

    char i[3], r[3], c[3], s[3];
    int nscan = sscanf(hostname, "i%2[0-9]r%2[0-9]c%2[0-9]s%2[0-9]", i, r, c, s);
    if (nscan != 4) {
        std::cerr << "hostname pattern does not match supermuc naming convention\n";
        return YLOC_STATUS_INIT_ERROR;
    }

    make_supermuc_graph(g, hostname);

    return YLOC_STATUS_SUCCESS;
}


#include <mpi.h>
#include <string.h>

//#include <adapter.h>
// #include <adapter_container.h>

#include "interface_impl.h"

using namespace yloc;

/**
 * @brief Build boost graph structure from hostnames of supermuc.
 *
 * @param g The graph to add the entries to
 * @param hostname Own hostname
 * @param hostname_length Length of hostname
 */
static void make_supermuc_graph(graph_t &g, vertex_descriptor_t vd_local_node, char *hostname, int hostname_length)
{
    /** TODO: check for structure of hostnames as expected for supermuc */
    assert(hostname_length < 32);

    int rank,nbproc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nbproc);

    char *recv_buffer = new char[32*nbproc];
    char (*hostnames)[32] = (char (*)[32])(recv_buffer);

    // necessary?
    //strncpy(hostnames[rank],hostname,32);

    MPI_Allgather(hostname,32,MPI_CHAR,recv_buffer,32,MPI_CHAR,MPI_COMM_WORLD);

    for(int i=0; i < nbproc; i++)
    {
        vertex_descriptor_t island_vd;
        vertex_descriptor_t rack_vd;
        vertex_descriptor_t slot_vd;
        vertex_descriptor_t node_vd;

#ifdef READABLE_CODE
        char island[3];
        strncpy(island,hostnames[i],2);
        island[2] = '\0';
        island_vd = g.add_vertex("system:"+std::string{island});

        char rack[5];
        strncpy(rack,hostnames[i],4);
        rack[4] = '\0';
        rack_vd = g.add_vertex("system:"+std::string{rack});

        char slot[7];
        strncpy(slot,hostnames[i],6);
        slot[6] = '\0';
        slot_vd = g.add_vertex("system:"+std::string{slot});
        
        char *node = hostnames[i];
        if(rank == i)
        {
            node_vd = vd_local_node;
        }
        else
        {
            node_vd = g.add_vertex("system:"+std::string{node});
        }
#else
        if(rank == i)
        {
            node_vd = vd_local_node;
        }
        else
        {
            node_vd = g.add_vertex("system:"+std::string{hostnames[i]});
        }

        /** TODO: Check hostname structure on supermuc */
        // slot ends after 6 characters
        hostnames[i][6] = '\0';
        slot_vd = g.add_vertex("system:"+std::string{hostnames[i]});

        // rack ends after 4 characters
        hostnames[i][4] = '\0';
        rack_vd = g.add_vertex("system:"+std::string{hostnames[i]});

        // island ends after 2 characters
        hostnames[i][2] = '\0';
        island_vd = g.add_vertex("system:"+std::string{hostnames[i]});
#endif

        /** TODO: Move this logic to g.add_edge-method */

        // Check if there is already an edge between the nodes
        if(! boost::edge(island_vd, rack_vd, g.boost_graph()).second)
        {
            /** TODO: Filter for only child/parent edges */
            boost::add_edge(island_vd, rack_vd, Edge{edge_type::YLOC_EDGE_TYPE_PARENT}, g.boost_graph());
            boost::add_edge(rack_vd, island_vd, Edge{edge_type::YLOC_EDGE_TYPE_CHILD}, g.boost_graph());
        }

        // Check if there is already an edge between the nodes
        if(! boost::edge(island_vd, rack_vd, g.boost_graph()).second)
        {
            /** TODO: Filter for only child/parent edges */
            boost::add_edge(rack_vd, slot_vd, Edge{edge_type::YLOC_EDGE_TYPE_PARENT}, g.boost_graph());
            boost::add_edge(slot_vd, rack_vd, Edge{edge_type::YLOC_EDGE_TYPE_CHILD}, g.boost_graph());
        }

        // Check if there is already an edge between the nodes
        if(! boost::edge(island_vd, rack_vd, g.boost_graph()).second)
        {
            /** TODO: Filter for only child/parent edges */
            boost::add_edge(slot_vd, node_vd, Edge{edge_type::YLOC_EDGE_TYPE_PARENT}, g.boost_graph());
            boost::add_edge(node_vd, slot_vd, Edge{edge_type::YLOC_EDGE_TYPE_CHILD}, g.boost_graph());
        }
    }
}

void YlocSuperMUC::init_graph(graph_t &g)
{
    int initialized;
    MPI_Initialized(&initialized);
    if(!initialized) {
        /** TODO: Log MPI not available */
        return;
    }

    char hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_length;
    MPI_Get_processor_name(hostname, &hostname_length);
    //hostname[hostname_length] = '\0'; // ??

    /** TODO: Only works if init_graph of hwloc is called before this one */
    auto vd_local_node = g.add_vertex("machine:"+std::string{std::getenv("HOSTNAME")});

    make_supermuc_graph(g, vd_local_node, hostname, hostname_length);
}

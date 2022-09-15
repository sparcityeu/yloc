
#include <mpi.h>

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

    /** TODO: Create hierachie based on numbers in hostname */
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

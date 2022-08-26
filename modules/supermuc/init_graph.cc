
#include <hwloc.h>
#include <iostream>

//#include <adapter.h>
// #include <adapter_container.h>
#include "init_graph.h"
#include "interface_impl.h"

// hwloc hierarchy: machine -> numanode -> package -> cache -> core -> pu
using namespace yloc;

/**
 * @brief Build boost graph structure in hostnames of supermuc.
 *
 * @param g The graph to add the entries to
 * @param hostname Own hostname
 * @param hostname_length Length of hostname
 */
static void make_supermuc_graph(graph_t &g, char *hostname, int hostname_length)
{
    /** TODO: Allgatherv with hostnames
     * Create hierachie based on numbers in hostname
     */
}

void YlocSuperMUC::init_graph(/* const char *file */)
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
    root_graph().register_vertex("hwloc_root",hostname);

    make_supermuc_graph(root_graph(), hostname, hostname_length);
}

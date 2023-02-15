#include <string>

#include <mpi.h>

#include <yloc/yloc.h>

#include "interface_impl.h"
#include "mpi_adapter.h"

using namespace yloc;


static void make_mpi_graph(Graph &g, char *hostname)
{
    int nbproc;
    MPI_Comm_size(MPI_COMM_WORLD, &nbproc);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char hostnames[nbproc][MPI_MAX_PROCESSOR_NAME];
    MPI_Allgather(hostname, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, hostnames, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, MPI_COMM_WORLD);


    for (int i = 0; i < nbproc; i++) {
        vertex_descriptor_t node_vd = g.add_vertex("machine:" + std::string{hostnames[i]});
        vertex_descriptor_t proc_vd = g.add_vertex();
        g[proc_vd].type = MPIProcess::ptr();
        g[proc_vd].add_adapter(new MPIAdapter{i});

        // add edges from mpi process nodes to compute nodes
        boost::add_edge(node_vd, proc_vd, Edge{edge_type::PARENT}, g.boost_graph());
        boost::add_edge(proc_vd, node_vd, Edge{edge_type::CHILD}, g.boost_graph());
    }
}

yloc_status_t ModuleMPI::init_graph(Graph &g)
{
    int initialized;
    MPI_Initialized(&initialized);
    if (!initialized) {
        return YLOC_STATUS_INIT_ERROR;
    }

    char hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_length;
    MPI_Get_processor_name(hostname, &hostname_length);

    make_mpi_graph(g, hostname);

    return YLOC_STATUS_SUCCESS;
}

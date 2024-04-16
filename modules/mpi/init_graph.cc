#include "interface_impl.h"
#include "mpi_adapter.h"
#include <yloc/affinity.h>
#include <yloc/yloc.h>

#include <mpi.h>

#include <memory>
#include <string>

#include <cassert>
#include <cstring>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <sched.h> // cpu_set_t, sched_getaffinity
#endif

using namespace yloc;

static void make_mpi_graph(Graph &g, const char *hostname)
{
    int nbproc;
    MPI_Comm_size(MPI_COMM_WORLD, &nbproc);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char hostnames[nbproc][MPI_MAX_PROCESSOR_NAME];
    MPI_Allgather(hostname, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, hostnames, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, MPI_COMM_WORLD);

    cpu_set_t cpuset;
    sched_getaffinity(0, sizeof(cpu_set_t), &cpuset);
    cpu_set_t cpusets[nbproc];
    MPI_Allgather(&cpuset, sizeof(cpu_set_t), MPI_BYTE, cpusets, sizeof(cpu_set_t), MPI_BYTE, MPI_COMM_WORLD);

    for (int i = 0; i < nbproc; i++) {
        vertex_t node_vd = g.add_vertex("machine:" + std::string{hostnames[i]});
        vertex_t proc_vd = g.add_vertex("mpi_rank:" + std::to_string(i));

        g[proc_vd].m_type = MPIProcess::ptr();
        auto adapter = std::make_shared<MPIAdapter>(i);
        g[proc_vd].add_adapter(adapter);

        // get affinity of mpi processes on local node
        if (!strcmp(hostnames[i], hostname)) {
            AffinityMask mask{cpusets[i]};
            if (mask.any()) {
                adapter->set_cpu_affinity_mask(mask);
                node_vd = lowest_containing_vertex(mask);
            }
        }

        // add edges from mpi process nodes to compute nodes
        boost::add_edge(node_vd, proc_vd, Edge{edge_type::CHILD}, g);
        boost::add_edge(proc_vd, node_vd, Edge{edge_type::PARENT}, g);
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

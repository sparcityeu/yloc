#include <yloc/graph.h>
#include <yloc/modules/adapter.h>
#include <yloc/modules/module.h>
#include <yloc/status.h>

#include "interface_impl.h"
#include "nvml_adapter.h"
#include "nvml_util.h"

#include <nvml.h>

#include <iostream>
#include <memory>
#include <vector>

/** TODO: implement bandwidth and throughput properties */

/** @see https://docs.nvidia.com/deploy/nvml-api/index.html */

using namespace yloc;

/** TODO: */
static uint64_t yloc_nvml_gpu_interconnect(Graph &g, uint32_t num_devices, std::vector<vertex_descriptor_t> &vertices, std::vector<nvmlDevice_t> &devices)
{
    uint64_t num_interconnects = 0;
    // get connectivity and topology between devices: (assuming symmetric connection)
    for (uint32_t dev_ind_src = 0; dev_ind_src < num_devices; ++dev_ind_src) {
        for (uint32_t dev_ind_dst = dev_ind_src + 1; dev_ind_dst < num_devices; ++dev_ind_dst) {
            nvmlGpuP2PStatus_t status;
            // we add a new GPU_INTERCONNECT edge iff two GPUs are directly connected (via NVLINK for NVIDIA GPUs, 1 hop)
            // connection via PCIe is already modeled via graph

            /*
            nvmlGpuP2PCapsIndex_t not documented in NVML doc... @see nvml.h
            NVML_P2P_CAPS_INDEX_READ = 0,
            NVML_P2P_CAPS_INDEX_WRITE,
            NVML_P2P_CAPS_INDEX_NVLINK,
            NVML_P2P_CAPS_INDEX_ATOMICS,
            NVML_P2P_CAPS_INDEX_PROP,
            NVML_P2P_CAPS_INDEX_UNKNOWN;
            */
            nvmlDeviceGetP2PStatus(devices[dev_ind_src], devices[dev_ind_dst], NVML_P2P_CAPS_INDEX_NVLINK, &status);

            uint32_t hops = 0;
            if (NVML_P2P_STATUS_OK == status) {
                hops = 1;
            }

#if 0
            nvmlGpuTopologyLevel_t path;
            nvmlDeviceGetTopologyCommonAncestor(devices[dev_ind_src], devices[dev_ind_dst], &path);
            /* 
            nvmlGpuTopologyLevel_t not fully documented in NVML doc... @see nvml.h
            enum nvmlGpuTopologyLevel_t:
            NVML_TOPOLOGY_INTERNAL           = 0, // e.g. Tesla K80
            NVML_TOPOLOGY_SINGLE             = 10, // all devices that only need traverse a single PCIe switch
            NVML_TOPOLOGY_MULTIPLE           = 20, // all devices that need not traverse a host bridge
            NVML_TOPOLOGY_HOSTBRIDGE         = 30, // all devices that are connected to the same host bridge
            NVML_TOPOLOGY_NODE               = 40, // all devices that are connected to the same NUMA node but possibly multiple host bridges
            NVML_TOPOLOGY_SYSTEM             = 50, // all devices in the system
            */
            if(path == NVML_TOPOLOGY_INTERNAL) {
                hops = 1;
            }

            if (path == NVML_TOPOLOGY_SINGLE || NVML_TOPOLOGY_MULTIPLE || NVML_TOPOLOGY_HOSTBRIDGE || NVML_TOPOLOGY_NODE) {
                // std::cout << "at least same node\n";
            }
#endif

            if (hops == 1) {
                num_interconnects++;
                // std::cout << "link gpu indices: " << dev_ind_src << " <-> " << dev_ind_dst << '\n';
                // std::cout << "link graph vds: " << vertices[dev_ind_src] << " <-> " << vertices[dev_ind_dst] << '\n';
                auto ed = boost::add_edge(vertices[dev_ind_src], vertices[dev_ind_dst], Edge{edge_type::GPU_INTERCONNECT}, g);
                ed = boost::add_edge(vertices[dev_ind_dst], vertices[dev_ind_src], Edge{edge_type::GPU_INTERCONNECT}, g);
            }
        }
    }
    return num_interconnects;
}

yloc_status_t ModuleNvml::init_graph(Graph &g)
{
    EXIT_ERR_NVML(nvmlInit_v2());
    unsigned int num_units;
    unsigned int num_devices;
    uint16_t dev_id;
    uint64_t bdfid; // PCIe bus device function id
    CHECK_NVML_MSG(nvmlUnitGetCount(&num_units));
    // std::cout << "num nvml units: " << num_units << '\n';

    CHECK_NVML_MSG(nvmlDeviceGetCount_v2(&num_devices));
    // std::cout << "num nvml devices: " << num_devices << '\n';

    std::vector<vertex_descriptor_t> vertices(num_devices);
    std::vector<nvmlDevice_t> devices(num_devices);

    for (unsigned int dev_index = 0; dev_index < num_devices; ++dev_index) {
        // nvmlDeviceGetAttributes_v2 ??
        nvmlDevice_t device;
        CHECK_NVML_MSG(nvmlDeviceGetHandleByIndex_v2(dev_index, &device));
        devices[dev_index] = device;

        nvmlPciInfo_t pci;
        CHECK_NVML_MSG(nvmlDeviceGetPciInfo_v3(device, &pci));

        auto nvml_adapter = std::make_shared<NvmlAdapter>(device);
        bdfid = a->bdfid().value();
        // std::cout << "GPU BDFID=" << bdfid << '\n';
        // std::cout << "GPU NUMA AFFINITY=" << a->numa_affinity().value() << '\n';

        nvmlMemory_t memory;
        CHECK_NVML_MSG(nvmlDeviceGetMemoryInfo(device, &memory));
        // std::cout << "GPU MEMORY VRAM=" << memory.total << '\n';

        // associate nvml device with graph node by pcie bdfid:
        auto vd = g.add_vertex("bdfid:" + std::to_string(bdfid));
        g[vd].add_adapter(nvml_adapter);
        vertices[dev_index] = vd;

        assert(g[vd].is_a<PCIDevice>());
        g[vd].type = GPU::ptr();
    }
    /** TODO: store this interconnect information? */
    uint64_t num_interconnects = yloc_nvml_gpu_interconnect(g, num_devices, vertices, devices);
    return YLOC_STATUS_SUCCESS;
}

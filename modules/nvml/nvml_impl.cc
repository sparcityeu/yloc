
#include <adapter.h>
#include <iostream>
#include <vector>

#include <interface.h>

#include "interface_impl.h"
#include "nvml_adapter.h"
#include "nvml_util.h"
#include <nvml.h>

#include "yloc.h"

/** TODO: implement bandwidth and throughput properties */

/** @see https://docs.nvidia.com/deploy/nvml-api/index.html */

using namespace yloc;

/** TODO: */
static uint64_t yloc_nvml_gpu_interconnect(graph_t &g, uint32_t num_devices, std::vector<vertex_descriptor_t> vertices)
{
    uint64_t num_interconnects = 0;
    return num_interconnects;
}

yloc_status_t YlocNvml::init_graph(graph_t &g)
{
    EXIT_ERR_NVML(nvmlInit_v2());
    unsigned int num_units;
    unsigned int num_devices;
    uint16_t dev_id;
    uint64_t bdfid; // PCIe bus device function id
    CHECK_NVML_MSG(nvmlUnitGetCount(&num_units));
    std::cout << "num nvml units: " << num_units << '\n';

    CHECK_NVML_MSG(nvmlDeviceGetCount_v2(&num_devices));
    std::cout << "num nvml devices: " << num_devices << '\n';

    std::vector<vertex_descriptor_t> vertices(num_devices);
    for (unsigned int dev_index = 0; dev_index < num_devices; ++dev_index) {
        // nvmlDeviceGetAttributes_v2 ??
        nvmlDevice_t device;
        CHECK_NVML_MSG(nvmlDeviceGetHandleByIndex_v2(dev_index, &device));

        nvmlPciInfo_t pci;
        CHECK_NVML_MSG(nvmlDeviceGetPciInfo_v3(device, &pci));

        NvmlAdapter *a = new NvmlAdapter{device};
        bdfid = a->bdfid().value();
        std::cout << "GPU BDFID=" << bdfid << '\n';


        std::cout << "GPU NUMA AFFINITY=" << a->numa_affinity().value() << '\n';

        nvmlMemory_t memory;
        CHECK_NVML_MSG(nvmlDeviceGetMemoryInfo(device, &memory));
        std::cout << "GPU MEMORY VRAM=" << memory.total << '\n';

        // std::cout << "nvml\n";
        // associate nvml device with graph node by pcie bdfid:
        auto vd = g.add_vertex("bdfid:" + std::to_string(bdfid));
        g[vd].tinfo.push_back(new NvmlAdapter{device});
        vertices[dev_index] = vd;

        assert(g[vd].tinfo.type->is_a<PCIDevice>());
        g[vd].tinfo.type = GPU::ptr();
        assert(g[vd].tinfo.type->is_a<Accelerator>());
        assert(g[vd].tinfo.type->is_a<GPU>());
}
    /** TODO: store this interconnect information? */
    // uint64_t num_interconnects = yloc_nvml_gpu_interconnect(g, num_devices, vertices);
    return YLOC_STATUS_SUCCESS;

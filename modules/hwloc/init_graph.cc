#include <yloc/graph.h>
#include <yloc/modules/adapter.h>
#include <yloc/modules/module.h>
#include <yloc/status.h>

#include "hwloc_adapter.h"
#include "interface_impl.h"

#include <hwloc.h>

#include <iostream>
#include <memory>

#include <unistd.h> // gethostname

// enum aliases for backwards compatibility (from hwloc documentation)
#if HWLOC_API_VERSION < 0x00010b00
#define HWLOC_OBJ_NUMANODE HWLOC_OBJ_NODE
#define HWLOC_OBJ_PACKAGE HWLOC_OBJ_SOCKET
#endif /* HWLOC_API_VERSION */

// hwloc hierarchy: machine -> numanode -> package -> cache -> core -> pu
using namespace yloc;

static const yloc::Component *hwloc_2_yloc_type(hwloc_obj_t obj)
{
    /** TODO: move that logic elsewhere and/or move type info to adapter */
    /** TODO: implement missing hwloc types: (@see hwloc_compare_types) */

    /**
        HWLOC object type Enumerations:
           enum hwloc_obj_type_t { HWLOC_OBJ_MACHINE, HWLOC_OBJ_PACKAGE,
               HWLOC_OBJ_CORE, HWLOC_OBJ_PU, HWLOC_OBJ_L1CACHE, HWLOC_OBJ_L2CACHE,
               HWLOC_OBJ_L3CACHE, HWLOC_OBJ_L4CACHE, HWLOC_OBJ_L5CACHE,
               HWLOC_OBJ_L1ICACHE, HWLOC_OBJ_L2ICACHE, HWLOC_OBJ_L3ICACHE,
               HWLOC_OBJ_GROUP, HWLOC_OBJ_NUMANODE, HWLOC_OBJ_BRIDGE,
               HWLOC_OBJ_PCI_DEVICE, HWLOC_OBJ_OS_DEVICE, HWLOC_OBJ_MISC,
               HWLOC_OBJ_MEMCACHE, HWLOC_OBJ_DIE }
           enum hwloc_obj_cache_type_e { HWLOC_OBJ_CACHE_UNIFIED,
               HWLOC_OBJ_CACHE_DATA, HWLOC_OBJ_CACHE_INSTRUCTION }
           enum hwloc_obj_bridge_type_e { HWLOC_OBJ_BRIDGE_HOST,
               HWLOC_OBJ_BRIDGE_PCI }
           enum hwloc_obj_osdev_type_e { HWLOC_OBJ_OSDEV_BLOCK, HWLOC_OBJ_OSDEV_GPU,
               HWLOC_OBJ_OSDEV_NETWORK, HWLOC_OBJ_OSDEV_OPENFABRICS,
               HWLOC_OBJ_OSDEV_DMA, HWLOC_OBJ_OSDEV_COPROC }
           enum hwloc_compare_types_e { HWLOC_TYPE_UNORDERED }
    */

    if (hwloc_obj_type_is_cache(obj->type)) {
        /** TODO: implement unified cache type */
    }

    if (hwloc_obj_type_is_icache(obj->type)) {
        if (!hwloc_compare_types(obj->type, HWLOC_OBJ_L1ICACHE)) {
            return L1InstructionCache::ptr();
        } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_L2ICACHE)) {
            return L2InstructionCache::ptr();
        } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_L3ICACHE)) {
            return L3InstructionCache::ptr();
        } else {
            return InstructionCache::ptr();
        }
    } else if (hwloc_obj_type_is_dcache(obj->type)) {
        /* Check whether an object type is a CPU Data or Unified Cache. Memory-side caches are not CPU caches. */
        if (!hwloc_compare_types(obj->type, HWLOC_OBJ_L1CACHE)) {
            return L1DataCache::ptr();
        } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_L2CACHE)) {
            return L2DataCache::ptr();
        } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_L3CACHE)) {
            return L3DataCache::ptr();
        } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_L4CACHE)) {
            return L4DataCache::ptr();
        }
        // else if (!hwloc_compare_types(HWLOC_OBJ_L5CACHE)) {
        // return DataCache::ptr();
        // }
        else {
            return DataCache::ptr();
        }
    } else if (hwloc_obj_type_is_memory(obj->type)) {
        /* This currently includes NUMA nodes and Memory-side caches. */
        return Memory::ptr();
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_OS_DEVICE) && obj->attr != NULL) {
        switch (obj->attr->osdev.type) {
        case HWLOC_OBJ_OSDEV_GPU:
            return LogicalGPU::ptr();
        case HWLOC_OBJ_OSDEV_COPROC:
            return LogicalAccelerator::ptr();
        case HWLOC_OBJ_OSDEV_DMA:
            return Misc::ptr(); // yloc type not implemented yet
        case HWLOC_OBJ_OSDEV_NETWORK:
            return Misc::ptr(); // yloc type not implemented yet
        case HWLOC_OBJ_OSDEV_OPENFABRICS:
            return Misc::ptr(); // yloc type not implemented yet
        case HWLOC_OBJ_OSDEV_BLOCK:
            return Misc::ptr(); // yloc type not implemented yet
        default:
            assert(false); // sanity check
            return UnknownComponentType::ptr();
        }
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_CORE)) {
        return PhysicalCore::ptr();
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_PU)) {
        return LogicalCore::ptr();
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_MACHINE)) {
        return Node::ptr(); // yloc type not implemented yet
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_PACKAGE)) {
        return Misc::ptr(); // yloc type not implemented yet
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_NUMANODE)) {
        return Memory::ptr(); // yloc type not implemented yet
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_GROUP)) {
        return Misc::ptr(); // yloc type not implemented yet
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_MISC)) {
        return Misc::ptr(); // yloc type not implemented yet
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_MEMCACHE)) {
        return Misc::ptr(); // yloc type not implemented yet
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_BRIDGE)) {
        return Bridge::ptr();
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_PCI_DEVICE)) {
        return PCIDevice::ptr();
    } else if (!hwloc_compare_types(obj->type, HWLOC_OBJ_DIE)) {
        return Misc::ptr(); // yloc type not implemented yet
    } else {
        /** TODO: if this assert triggers, we miss a hwloc type here */
        assert(false); // sanity check
        return UnknownComponentType::ptr();
    }
}

/**
 * @brief Build boost graph from hwloc (sub)tree.
 *
 * @param g
 * @param t
 * @param vd
 * @param obj
 */
static void make_hwloc_graph(Graph &g, hwloc_topology_t t, vertex_t vd, hwloc_obj_t obj)
{
    // for all children of obj: add_adapter new vertex to graph and set edges
    hwloc_obj_t child = hwloc_get_next_child(t, obj, NULL);
    while (child) {
        auto adapter = std::make_shared<HwlocAdapter>(child);
        vertex_t child_vd;

        if (hwloc_2_yloc_type(child)->is_a<PCIDevice>()) {
            std::string id = "bdfid:" + std::to_string(adapter->bdfid().value());
            child_vd = g.add_vertex(id);
            // std::cout << "hwloc pcidevice " << id << " vd: " << child_vd << "\n";
        } else {
            child_vd = g.add_vertex();
        }

        if (g[child_vd].m_description.empty()) {
            g[child_vd].m_description = adapter->to_string();
        }

        g[child_vd].add_adapter(adapter);
        if (g[child_vd].m_type == UnknownComponentType::ptr()) { // has no component type yet
            g[child_vd].m_type = hwloc_2_yloc_type(child);
        } else {
            // sanity check /** TODO: implement is_a for runtime objects */
            // assert(g[child_vd].m_type == hwloc_2_yloc_type(obj));
        }
        auto ret = boost::add_edge(vd, child_vd, Edge{edge_type::CHILD}, g);
        ret = boost::add_edge(child_vd, vd, Edge{edge_type::PARENT}, g);

        make_hwloc_graph(g, t, child_vd, child);
        child = hwloc_get_next_child(t, obj, child);
    }
}

/* runtime check for matching hwloc abi from hwloc documentation */
yloc_status_t check_hwloc_api_version()
{
    /** TODO: probably move that piece of code to constructor of hwloc module */
    unsigned version = hwloc_get_api_version();
    if ((version >> 16) != (HWLOC_API_VERSION >> 16)) {
        fprintf(stderr,
                "%s compiled for hwloc API 0x%x but running on library API 0x%x.\n"
                "You may need to point LD_LIBRARY_PATH to the right hwloc library.\n"
                "Aborting since the new ABI is not backward compatible.\n",
                __func__, HWLOC_API_VERSION, version);
        return YLOC_STATUS_INIT_ERROR;
    }
    return YLOC_STATUS_SUCCESS;
}

static void set_hwloc_options(hwloc_topology_t &t)
{
    // some object types are filtered by default
    // see man hwlocality_object_types and hwlocality_configuration
    // control with hwloc_topology_set_flags() or
    // hwloc_topology_set_..._filter()
    hwloc_topology_set_all_types_filter(t, HWLOC_TYPE_FILTER_KEEP_IMPORTANT);

    // instruction cache and bridges are typically not important
    hwloc_topology_set_icache_types_filter(t, HWLOC_TYPE_FILTER_KEEP_NONE);
    hwloc_topology_set_type_filter(t, HWLOC_OBJ_BRIDGE, HWLOC_TYPE_FILTER_KEEP_NONE);

    // optionally load topology from xml file
    // hwloc_topology_set_xml(t, file)
}

ModuleHwloc::~ModuleHwloc()
{
    hwloc_topology_destroy(m_topology);
}

yloc_status_t ModuleHwloc::init_graph(Graph &g)
{
    yloc_status_t ret;
    if((ret = check_hwloc_api_version()) != YLOC_STATUS_SUCCESS) {
        return ret;
    }

    hwloc_topology_t &t = m_topology;
    hwloc_topology_init(&t);
    set_hwloc_options(t);
    hwloc_topology_load(t); // actual detection

    hwloc_obj_t root = hwloc_get_root_obj(t);
    assert(root->type == HWLOC_OBJ_MACHINE);

    // std::getenv("HOSTNAME") // HOSTNAME may be not set, so we use gethostname instead
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    auto root_vd = g.add_vertex("machine:" + std::string{hostname});
    g[root_vd].m_description = std::string{hostname};
    g.set_root_vertex(root_vd);

    g[root_vd].add_adapter(std::make_shared<HwlocAdapter>(root));
    if (g[root_vd].m_type == UnknownComponentType::ptr()) { // has no component type yet
        g[root_vd].m_type = hwloc_2_yloc_type(root);
    } else {
        // sanity check /** TODO: implement is_a for runtime objects */
        // assert(g[root_vd].m_type == hwloc_2_yloc_type(root));
    }

    make_hwloc_graph(g, t, root_vd, root);
    return YLOC_STATUS_SUCCESS;
}

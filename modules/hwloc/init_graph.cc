
#include <hwloc.h>
#include <iostream>

#include "hwloc_adapter.h"
#include <adapter.h>

#include <hwloc.h>
#include <interface.h>

#include "interface_impl.h"

// hwloc hierarchy: machine -> numanode -> package -> cache -> core -> pu
using namespace yloc;

static const yloc::Component *yloc_type(hwloc_obj_t obj)
{
    /** TODO: move that logic elsewhere and/or move type info to adapter */
    /** TODO: implement missing hwloc types: (@see hwloc_compare_types)
    Typedefs
       typedef enum hwloc_obj_cache_type_e hwloc_obj_cache_type_t
       typedef enum hwloc_obj_bridge_type_e hwloc_obj_bridge_type_t
       typedef enum hwloc_obj_osdev_type_e hwloc_obj_osdev_type_t

    Enumerations
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

    if (hwloc_obj_type_is_icache(obj->type)) {
        return InstructionCache::ptr();
    } else if (hwloc_obj_type_is_dcache(obj->type)) {
        /* Check whether an object type is a CPU Data or Unified Cache. Memory-side caches are not CPU caches. */
        return DataCache::ptr();
    } else if (hwloc_obj_type_is_memory(obj->type)) {
        /* This current includes NUMA nodes and Memory-side caches. */
        return Memory::ptr();
    } else if (hwloc_compare_types(obj->type, HWLOC_OBJ_OS_DEVICE) == 0 && obj->attr != NULL) {
        switch (obj->attr->osdev.type) {
        case HWLOC_OBJ_OSDEV_GPU:
            return GPU::ptr();
        case HWLOC_OBJ_OSDEV_COPROC:
            return Accelerator::ptr();
        case HWLOC_OBJ_OSDEV_DMA:
            return Misc::ptr(); // yloc type not implemented yet
        case HWLOC_OBJ_OSDEV_NETWORK:
            return Misc::ptr(); // yloc type not implemented yet
        case HWLOC_OBJ_OSDEV_OPENFABRICS:
            return Misc::ptr(); // yloc type not implemented yet
        case HWLOC_OBJ_OSDEV_BLOCK:
            return Misc::ptr(); // yloc type not implemented yet
        default:
            return UnknownComponentType::ptr();
        }
    } else if (hwloc_compare_types(obj->type, HWLOC_OBJ_CORE)) {
        return CPUCore::ptr();
    } else if (hwloc_compare_types(obj->type, HWLOC_OBJ_PU)) {
        return LogicalCore::ptr();
    } else if (hwloc_compare_types(obj->type, HWLOC_OBJ_MACHINE)) {
        return Misc::ptr(); // yloc type not implemented yet
    } else if (hwloc_compare_types(obj->type, HWLOC_OBJ_PACKAGE)) {
        return Misc::ptr(); // yloc type not implemented yet
    } else {
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
static void make_hwloc_graph(graph_t &g, hwloc_topology_t t, vertex_descriptor_t &vd, hwloc_obj_t obj)
{
    g[vd].tinfo.push_back(new HwlocAdapter{obj});
    if (g[vd].tinfo.type == UnknownComponentType::ptr()) { // has no component type yet
        g[vd].tinfo.type = yloc_type(obj);
    }

    // for all children of obj: add new vertex to graph and set edges
    hwloc_obj_t child = hwloc_get_next_child(t, obj, NULL);
    while (child) {
        /** TODO: proper add_vertex function with identifier_t */
        auto child_vd = g.add_vertex();
#if USE_SUBGRAPH
        auto ret = boost::add_edge(vd, child_vd, graph_t::edge_property_type{0, Edge{YLOC_EDGE_TYPE_CHILD}}, g.boost_graph());
        ret = boost::add_edge(child_vd, vd, graph_t::edge_property_type{0, Edge{YLOC_EDGE_TYPE_PARENT}}, g.boost_graph());
#else
        auto ret = boost::add_edge(vd, child_vd, {YLOC_EDGE_TYPE_PARENT}, g.boost_graph());
        ret = boost::add_edge(child_vd, vd, {YLOC_EDGE_TYPE_CHILD}, g.boost_graph());
#endif
        make_hwloc_graph(g, t, child_vd, child);
        child = hwloc_get_next_child(t, obj, child);
    }
}

/* runtime check for matching hwloc abi from hwloc documentation */
static void check_hwloc_api_version()
{
    /** TODO: probably move that piece of code to constructor of hwloc module */
    unsigned version = hwloc_get_api_version();
    if ((version >> 16) != (HWLOC_API_VERSION >> 16)) {
        fprintf(stderr,
                "%s compiled for hwloc API 0x%x but running on library API 0x%x.\n"
                "You may need to point LD_LIBRARY_PATH to the right hwloc library.\n"
                "Aborting since the new ABI is not backward compatible.\n",
                __func__, HWLOC_API_VERSION, version);
        exit(EXIT_FAILURE);
    }
}

void YlocHwloc::init_graph(/* const char *file */)
{
    check_hwloc_api_version();

    hwloc_topology_t t;
    hwloc_topology_init(&t); // initialization

    /**
     * TODO: move the settings logic to other function (set_options)
     *
     */

    // some object types are filtered by default
    // see man hwlocality_object_types and hwlocality_configuration
    // control with hwloc_topology_set_flags() or
    // hwloc_topology_set_..._filter()

    // hwloc_topology_set_io_types_filter(t, HWLOC_TYPE_FILTER_KEEP_ALL);
    // hwloc_topology_set_all_types_filter(t, HWLOC_TYPE_FILTER_KEEP_ALL);

    hwloc_topology_set_all_types_filter(t, HWLOC_TYPE_FILTER_KEEP_IMPORTANT);

    /*
    if (*file != '0') { // FIXME: check for NULL instead? review interface ...
        if (hwloc_topology_set_xml(t, file) == -1) {
            // TODO: error handling of hwloc functions ...
        }
    }
    */

    hwloc_topology_load(t); // actual detection

    hwloc_obj_t root = hwloc_get_root_obj(t);
    assert(root->type == HWLOC_OBJ_MACHINE);
#if USE_SUBGRAPH
    /** TODO: Subgraph-logic if it is supposed to stay */
    // printf("making hwloc graph...\n");
    auto root_vd = boost::add_vertex(m_subgraph);

    make_hwloc_graph(m_subgraph, t, root_vd, root);
#else
    // printf("making hwloc graph...\n");
    auto root_vd = root_graph().add_vertex("hwloc_root");

    make_hwloc_graph(root_graph(), t, root_vd, root);
#endif

    // TODO: lifetime of topology context?
    // hwloc_topology_destroy(t);
}

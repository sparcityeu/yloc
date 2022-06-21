
#include <hwloc.h>
#include <iostream>

#include "interface_impl.h"
#include "init_graph.h"


// hwloc hierarchy: machine -> numanode -> package -> cache -> core -> pu

// typedef boost::graph_traits<graph_t>::vertex_descriptor VD;
using graph_t = yloc::graph_t;
using VD = yloc::vertex_descriptor_t;

std::unordered_map<VD, hwloc_obj_t> vertex2hwloc_map;
// const auto pm = boost::make_assoc_property_map(std::unordered_map<VD, hwloc_obj_t>{});

/**
 * @brief Build boost graph from hwloc (sub)tree.
 *
 * @param g
 * @param t
 * @param vd
 * @param obj
 */
static void make_hwloc_graph(graph_t &g, hwloc_topology_t t, VD &vd, hwloc_obj_t obj)
{
    // TODO: remove this code and also the whole Vertex class property? ...
    // boost::put(&Vertex::type, g, vd, hwloc_obj_type_string(obj->type));

    // ... and add vertex properties to external property map instead:
    vertex2hwloc_map[vd] = obj;
    // or maybe use BGL property map:
    // boost::put(hwloc_obj_t, g, vd, obj);

    //g1.local_to_global(vd1)

    // for all children of obj: add new vertex to graph and set edges
    hwloc_obj_t child = hwloc_get_next_child(t, obj, NULL);
    while (child) {
        auto child_vd = boost::add_vertex(g);
#if USE_SUBGRAPH
        auto ret = boost::add_edge(vd, child_vd, graph_t::edge_property_type{0, Edge{YLOC_EDGE_TYPE_CHILD}}, g);
        ret = boost::add_edge(child_vd, vd, graph_t::edge_property_type{0, Edge{YLOC_EDGE_TYPE_PARENT}}, g);
#else
        auto ret = boost::add_edge(vd, child_vd, {YLOC_EDGE_TYPE_PARENT}, g);
        ret = boost::add_edge(child_vd, vd, {YLOC_EDGE_TYPE_CHILD}, g);
#endif
        make_hwloc_graph(g, t, child_vd, child);
        child = hwloc_get_next_child(t, obj, child);
    }
}

/* runtime check for matching hwloc abi from hwloc documentation */
static void check_hwloc_api_version()
{
    // TODO: probably move that piece of code to constructor of hwloc module
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

    // printf("making hwloc graph...\n");
    auto root_vd = boost::add_vertex(m_subgraph);

    make_hwloc_graph(m_subgraph, t, root_vd, root);

    // TODO: lifetime of topology context?
    // hwloc_topology_destroy(t);
}

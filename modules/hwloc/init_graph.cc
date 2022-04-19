
// Adopted from myloq by Jakub Orlowski (jakubo87)
// https://github.com/jakubo87/myLoq

#include "interface_impl.h"

#include <hwloc.h>
#include <iostream>

#include "init_graph.h"
#include "query_graph.h"

// machine -> numanode -> package -> cache -> core -> pu

// some object types are filtered by default
// see man hwlocality_object_types and hwlocality_configuration
// control with hwloc_topology_set_flags()

typedef boost::graph_traits<graph_t>::vertex_descriptor VD;
std::unordered_map<VD, hwloc_obj_t> hwloc_property_map;

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
    boost::put(&Vertex::vid, g, vd, getmax_vid(g)); // get new highest global index
    boost::put(&Vertex::type, g, vd, hwloc_obj_type_string(obj->type));
    boost::put(&Vertex::index, g, vd, obj->logical_index);
    boost::put(&Vertex::depth, g, vd, obj->depth);
    // ... and add vertex properties to external property map instead:
    hwloc_property_map[vd] = obj;
    // or maybe use BGL property map:
    // boost::put(hwloc_obj_t, g, vd, obj);

    // for all children of obj: add new vertex to graph and set edges
    hwloc_obj_t child = hwloc_get_next_child(t, obj, NULL);
    while (child) {
        auto child_vd = add_vertex(g);
        auto ret = add_edge(vd, child_vd, g);
        ret = add_edge(child_vd, vd, g);
        make_hwloc_graph(g, t, child_vd, child);
        child = hwloc_get_next_child(t, obj, child);
    }
}

graph_t init_graph_myloq(const char *file)
{
    // hwloc_init
    hwloc_topology_t t;
    hwloc_topology_init(&t); // initialization
    // hwloc_topology_set_io_types_filter(t, HWLOC_TYPE_FILTER_KEEP_ALL); // for all the devices, or...
    // hwloc_topology_set_io_types_filter(t,HWLOC_TYPE_FILTER_KEEP_IMPORTANT);

    hwloc_topology_set_all_types_filter(t, HWLOC_TYPE_FILTER_KEEP_ALL);
    hwloc_topology_set_all_types_filter(t, HWLOC_TYPE_FILTER_KEEP_IMPORTANT);

    if (*file != '0') { // FIXME: check for NULL instead? review interface ...
        if (hwloc_topology_set_xml(t, file) == -1) {
            // TODO: error handling of hwloc functions ...
        }
    }
    hwloc_topology_load(t); // actual detection

    hwloc_obj_t root = hwloc_get_root_obj(t);
    assert(root->type == HWLOC_OBJ_MACHINE);

    graph_t g;
    // printf("making hwloc graph...\n");
    auto root_vd = add_vertex(g);
    make_hwloc_graph(g, t, root_vd, root);

    // TODO: lifetime of topology context?
    // hwloc_topology_destroy(t);

    return g;
}

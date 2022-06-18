
#include <interface.h>

#include "init_graph.h"

// enum aliases for backwards compatibility from hwloc documentation
#if HWLOC_API_VERSION < 0x00010b00
#define HWLOC_OBJ_NUMANODE HWLOC_OBJ_NODE
#define HWLOC_OBJ_PACKAGE HWLOC_OBJ_SOCKET
#endif /* HWLOC_API_VERSION */

using yloc::graph_t;
using yloc::YlocModule;

// TODO: separate definition from declaration, remove init_graph_myloq

class YlocHwloc : public YlocModule
{
public:
    void init_graph(graph_t &graph)
    {
        graph = init_graph_myloq("0");
        return;
    }

    void export_graph(graph_t &graph, void **output)
    {
        output = nullptr;
        return;
    }

    void update_graph(graph_t &graph)
    {
        return;
    };

private:
};

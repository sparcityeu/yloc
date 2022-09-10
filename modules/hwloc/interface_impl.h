
#include <interface.h>

// enum aliases for backwards compatibility from hwloc documentation
#if HWLOC_API_VERSION < 0x00010b00
#define HWLOC_OBJ_NUMANODE HWLOC_OBJ_NODE
#define HWLOC_OBJ_PACKAGE HWLOC_OBJ_SOCKET
#endif /* HWLOC_API_VERSION */

using yloc::graph_t;
using yloc::YlocModule;

/** TODO: separate definition from declaration **/

class YlocHwloc : public YlocModule
{
public:
    void init_graph(/* graph_t &graph */) override;

    void export_graph(graph_t &graph, void **output) override
    {
        output = nullptr;
        return;
    }

    void update_graph(graph_t &graph) override
    {
        return;
    };

private:
};

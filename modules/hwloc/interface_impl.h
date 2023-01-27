
#include <yloc/modules/module.h>

// enum aliases for backwards compatibility from hwloc documentation
#if HWLOC_API_VERSION < 0x00010b00
#define HWLOC_OBJ_NUMANODE HWLOC_OBJ_NODE
#define HWLOC_OBJ_PACKAGE HWLOC_OBJ_SOCKET
#endif /* HWLOC_API_VERSION */

using yloc::Graph;
using yloc::YlocModule;

/** TODO: separate definition from declaration **/

class YlocHwloc : public YlocModule
{
public:
    yloc_status_t init_graph(Graph &graph) override;

    yloc_status_t export_graph(const Graph &graph, void **output) override
    {
        output = nullptr;
        return YLOC_STATUS_NOT_YET_IMPLEMENTED;
    }

    yloc_status_t update_graph(Graph &graph) override
    {
        return YLOC_STATUS_NOT_YET_IMPLEMENTED;
    };

private:
};

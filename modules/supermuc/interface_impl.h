
#include <yloc/modules/module.h>
#include <yloc/yloc_status.h>

using yloc::Graph;
using yloc::YlocModule;

class YlocSuperMUC : public YlocModule
{
public:
    yloc_status_t init_graph(Graph &graph) override;

    yloc_status_t export_graph(const Graph &graph, void **output) override
    {
        output = nullptr;
        return YLOC_STATUS_NOT_SUPPORTED;
    }

    yloc_status_t update_graph(Graph &graph) override
    {
        return YLOC_STATUS_NOT_SUPPORTED;
    };

private:
};

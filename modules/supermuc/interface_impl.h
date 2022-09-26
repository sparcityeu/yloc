
#include <interface.h>
#include <yloc.h>

using yloc::graph_t;
using yloc::YlocModule;

class YlocSuperMUC : public YlocModule
{
public:
    yloc_status_t init_graph(graph_t &graph) override;

    yloc_status_t export_graph(const graph_t &graph, void **output) override
    {
        output = nullptr;
        return;
    }

    yloc_status_t update_graph(graph_t &graph) override
    {
        return;
    };

private:
};

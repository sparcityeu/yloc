#pragma once

#include <interface.h>

using yloc::graph_t;
using yloc::YlocModule;

class YlocNvml : public YlocModule
{
public:
    yloc_status_t init_graph(graph_t &graph) override;

    yloc_status_t export_graph(const graph_t &graph, void **output) override
    {
        return YLOC_STATUS_NOT_SUPPORTED;
    }

    yloc_status_t update_graph(graph_t &graph) override
    {
        return YLOC_STATUS_NOT_SUPPORTED;
    }

private:
};

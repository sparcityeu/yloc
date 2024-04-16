#pragma once

#include <yloc/modules/module.h>

namespace yloc
{
    class ModuleNvml : public Module
    {
    public:
        yloc_status_t init_graph(Graph &graph) override;

    private:
    };
}
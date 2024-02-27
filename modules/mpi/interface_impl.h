#pragma once

#include <yloc/modules/module.h>

namespace yloc
{
    class ModuleMPI : public Module
    {
    public:
        ModuleMPI() { m_init_order = Module::init_order::SECOND; }
        
        yloc_status_t init_graph(Graph &graph) override;

        yloc_status_t export_graph(const Graph &graph, void **output) override
        {
            output = nullptr;
            return YLOC_STATUS_NOT_SUPPORTED;
        }

        yloc_status_t update_graph(Graph &graph) override
        {
            return YLOC_STATUS_NOT_SUPPORTED;
        }

    private:
    };
}
#pragma once

#include <yloc/modules/module.h>

namespace yloc
{
    class ModuleMPI : public Module
    {
    public:
        ModuleMPI() { m_init_order = Module::init_order::SECOND; }
        
        yloc_status_t init_graph(Graph &graph) override;

    private:
    };
}
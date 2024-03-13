#pragma once

#include <yloc/modules/module.h>

namespace yloc
{
    class ModuleSuperMUC : public Module
    {
    public:
        ModuleSuperMUC() { m_enabled = false; }

        yloc_status_t init_graph(Graph &graph) override;

    private:
    };
}
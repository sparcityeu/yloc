#pragma once

#include <yloc/modules/module.h>

struct hwloc_topology; // fwd decl

namespace yloc
{
    class ModuleHwloc : public Module
    {
    public:
        yloc_status_t init_graph(Graph &graph) override;

        ~ModuleHwloc();

    private:
        hwloc_topology *m_topology;
    };
}
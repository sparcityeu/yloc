#include <yloc/graph.h>
#include <yloc/init.h>
#include <yloc/modules/module.h>
#include <yloc/status.h>

namespace yloc
{
    Graph &root_graph()
    {
        static Graph s_graph;
        return s_graph;
    }

    yloc_status_t init()
    {
        auto modules = list_modules(); // list_modules creates new vector ...

        std::sort(modules.begin(), modules.end(), [](yloc::Module *a, yloc::Module *b) {
            return a->m_init_order < b->m_init_order;
        });

        for (auto *m : modules) {
            if (m->m_enabled) {
                // TODO: check return value
                m->init_graph(root_graph());
            }
        }
        return YLOC_STATUS_SUCCESS;
    }

    yloc_status_t finalize()
    {
        // free modules ?

        // exit threads of ongoing modules (nothing to do yet)

        return YLOC_STATUS_SUCCESS;
    }
}

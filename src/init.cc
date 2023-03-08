
#include <yloc/graph.h>
#include <yloc/init.h>
#include <yloc/modules/module.h>

namespace yloc
{
    Graph &root_graph()
    {
        static Graph s_graph;
        return s_graph;
    }

    int init(init_flags_t _flags)
    {

        // todo flag logic (probably combined with module logic)

        // initialize static graph
        //  Graph & graph = root_graph();

        for (auto *m : list_modules()) {
            m->init_graph(root_graph());
        }

        // store result of module init in global variable
        return 0;
    }

    int finalize()
    {
        // free modules ?

        // exit threads of ongoing modules (nothing to do yet)

        return 0;
    }

    int update()
    {
        // todo move to better file (currently fits best to these function)
        // trigger a manual update of the modules
        // Extra logic together with auto-updates from threads necessary

        // This function is probably a manual trigger that calls update_graph on the modules.

        // This function could also be called by an auto-update thread from the outer library,
        // but it is probably better to split the threads, so there is one thread per module.
        // More fine granualar thread functions would allow different timings.
        return 0;
    }
}


#include <interface.h>

#include "init_graph.h"

// actually used graph type by this module
// todo probably use centralized structure to maintain compatibility between modules
//typedef graph_t void *;

class YlocHwloc : public YlocModule {

public:

    void init_graph(graph_t * graph) {               // init complete graph
        // Todo proper lifetime of this object
        static graph_t _graph = init_graph_myloq("0"); // Todo use namespaces
        graph = &_graph;
        return;
    }

    void init_graph_secondary(graph_t * graph) {     // init of secondary module (if graph already is initialized by another module)
        return;
    }

    // Specifies wheter init_graph is implemented and if the module can be used standalone // todo better comment
    // Todo better module capability system
    bool is_main_module() {
        return true;
    }

    // optional function, not ever module requires this
    // it is recommended that every module that can init a graph on its own also provides this function.
    // Todo return value for feedback if export was successful/supported
    void export_graph(graph_t * graph, void ** output) {
        output = nullptr;
        return;
    }

    void update_graph(graph_t * graph) {
        return;
    };

private:

};
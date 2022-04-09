
#include <interface.h>

#include "init_graph.h"

// actually used graph type by this module
// todo probably use centralized structure to maintain compatibility between modules
//typedef graph_t void *;

// enum aliases for backwards compatibility from hwloc documentation
#if HWLOC_API_VERSION < 0x00010b00
    #define HWLOC_OBJ_NUMANODE HWLOC_OBJ_NODE
    #define HWLOC_OBJ_PACKAGE HWLOC_OBJ_SOCKET
#endif /* HWLOC_API_VERSION */

class YlocHwloc : public YlocModule {

public:

    void init_graph(graph_t * graph) {               // init complete graph

        // runtime check for matching hwloc abi from hwloc documentation
        // TODO: probably move that piece of code to constructor of hwloc module
        unsigned version = hwloc_get_api_version();
        if ((version >> 16) != (HWLOC_API_VERSION >> 16)) {
            fprintf(stderr,
               "%s compiled for hwloc API 0x%x but running on library API 0x%x.\n"
               "You may need to point LD_LIBRARY_PATH to the right hwloc library.\n"
               "Aborting since the new ABI is not backward compatible.\n",
               __func__, HWLOC_API_VERSION, version);
            exit(EXIT_FAILURE);
        }

        // Todo proper lifetime of this object
        *graph = init_graph_myloq("0"); // Todo use namespaces
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
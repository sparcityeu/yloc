
#include <interface.h>

using yloc::graph_t;
using yloc::YlocModule;

class ExampleModule : public YlocModule
{

public:
    void init_graph(graph_t &graph)
    { // init complete graph
        return;
    }

    // optional function, not ever module requires this
    // it is recommended that every module that can init a graph on its own also provides this function.
    // Todo return value for feedback if export was successful/supported
    void export_graph(graph_t &graph, void **output)
    {
        output = nullptr;
        return;
    }

    void update_graph(graph_t &graph)
    {
        return;
    }

private:
};

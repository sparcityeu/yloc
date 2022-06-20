#pragma once

#include <adapter.h>
#include <vector>

namespace yloc
{
    class AdapterContainer
    {
    public:
        //~AdapterContainer
    
    private:
        std::vector<Adapter *> adapters;
    };
}


#if 0
vertex_descriptor_t VD;

for(adapter in g[VD].ac) {
    adapter.source().subgraph();
    adapter.source_descriptor();
}

// nice syntax: g[VD].ac
#endif
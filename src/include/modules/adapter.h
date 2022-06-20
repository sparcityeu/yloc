#pragma once

#include <optional>
#include "interface.h"

namespace yloc
{
    class Adapter // TODO: Maybe separate between VertexAdapter and EdgeAdapter
    {
    public:
        // TODO: Deconstructor and lifetime of native objects
        
        // example for abstract machine model implementation
        //
        // TODO: do we want to provide a default implementation?
        // e.g. return std::optional false in default impl.
        virtual std::optional<int> capacity() = 0;

        /*virtual YlocModule *        source() = 0;
        // virtual vertex_descriptor_t source_descriptor() = 0;
        virtual vertex_descriptor_t source_descriptor() { return vd_ } = 0;*/

        // virtual ?type? native_obj() = 0; // TODO

    private:
        vertex_descriptor_t vd_;
    };

    // we can also think of an inheritance hierarchy to reflect the abstract machine model.
    //
    // for example:
    //
    class MemoryAdapter : Adapter
    {
    };

    class CacheAdapter : MemoryAdapter
    {
    };

    class RamAdapter : MemoryAdapter
    {
    };
}

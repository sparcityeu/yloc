#pragma once

#include <optional>
// #include <graph_type.h>

#define ADAPTER_PROP(type, name) virtual std::optional<type> name() const { return std::optional<type>{}; }

namespace yloc
{
    class Adapter // TODO: Maybe separate between VertexAdapter and EdgeAdapter
    {
    public:
        /** TODO: Deconstructor and lifetime of native objects */
        // Adapter() = default;
        virtual ~Adapter() = default;

        /** TODO: abstract machine model definition */
        // virtual std::optional<int> capacity() { return std::optional<int>{}; } 

        // virtual std::optional<yloc_vertex_type> type() { return std::optional<yloc_vertex_type>{}; }
        // yloc_vertex_type type() { return YLOC_VERTEX_TYPE_MAX; }
        /** abstract machine model end */

        /* virtual YlocModule *        source() = 0; */
        /* virtual vertex_descriptor_t source_descriptor() { return m_vd; } */

        // virtual std::optional<std::string> as_string() { return std::optional<std::string>{}; }
        ADAPTER_PROP(std::string, as_string)

    protected:
        // vertex_descriptor_t m_vd; // local vertex descriptor
    };    
}

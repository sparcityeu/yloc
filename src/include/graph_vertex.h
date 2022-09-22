#pragma once

/** TODO: edge class */

// enum class yloc_vertex_type : int { YLOC_VERTEX_TYPE_MAX };

enum class yloc_edge_type : int { YLOC_EDGE_TYPE_PARENT,
                                  YLOC_EDGE_TYPE_CHILD,
                                  YLOC_GPU_INTERCONNECT,
                                  YLOC_EDGE_TYPE_MAX };

#include <optional>
#include <string>
#include <vector>

#include <component_types.h>

namespace yloc
{
    // using vertex_type = yloc_vertex_type;
    using edge_type = yloc_edge_type;

    // forward declaration to avoid #include "modules/adapter.h"
    class Adapter;

    /* bundled internal (common) vertex properties */
    struct Vertex {
        const Component *type{UnknownComponentType::ptr()};

        std::optional<uint64_t> get(const char *property);

        std::string to_string() { return type->to_string() + ": " + description; }

        void add(Adapter *a)
        {
            m_adapters.push_back(a);
        }

        std::vector<Adapter *> m_adapters;
        std::string description{};
    };

    /* bundled internal (common) edge properties */
    struct Edge {
        edge_type type;
        /* TODO: somehow we need to point to the edge property maps here */
        // AdapterContainer tinfo;
    };
}
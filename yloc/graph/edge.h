#pragma once

namespace yloc
{
    enum class edge_type : int { PARENT,
                                 CHILD,
                                 GPU_INTERCONNECT,
                                 EDGE_TYPE_MAX };

    /* bundled internal (common) edge properties */
    struct Edge {
        edge_type type;
        /* TODO: somehow we need to point to the edge property maps here */
    };
}
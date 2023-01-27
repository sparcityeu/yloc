// #pragma once

#include <yloc/modules/module.h>
#include <iostream>
#include <rocm_smi/rocm_smi.h>

using yloc::Graph;
using yloc::YlocModule;

class YlocRocm : public YlocModule
{
public:
    yloc_status_t init_graph(Graph &graph) override;

    yloc_status_t export_graph(const Graph &graph, void **output) override
    {
        return YLOC_STATUS_NOT_SUPPORTED;
    }

    yloc_status_t update_graph(Graph &graph) override
    {
        return YLOC_STATUS_NOT_SUPPORTED;
    }

private:
};

#if FIXME

// #include "init_graph.h"

/** Bit mask stuff **/
static constexpr size_t BITS_PER_LONG = 8 * sizeof(long);
static constexpr size_t BITS_PER_LONG_LONG = 8 * sizeof(long long);

static_assert(BITS_PER_LONG_LONG == 64);

#define UL(x) x##UL
#define ULL(x) x##ULL

#define GENMASK(h, l)                  \
    (((~UL(0)) - (UL(1) << (l)) + 1) & \
     (~UL(0) >> (BITS_PER_LONG - 1 - (h))))

#define GENMASK_ULL(h, l)                \
    (((~ULL(0)) - (ULL(1) << (l)) + 1) & \
     (~ULL(0) >> (BITS_PER_LONG_LONG - 1 - (h))))

#define print_bits(x)                                     \
    do {                                                  \
        typeof(x) a__ = (x);                              \
        char *p__ = (char *)&a__ + sizeof(x) - 1;         \
        size_t bytes__ = sizeof(x);                       \
        printf(#x ": ");                                  \
        while (bytes__--) {                               \
            char bits__ = 8;                              \
            while (bits__--)                              \
                putchar(*p__ &(1 << bits__) ? '1' : '0'); \
            p__--;                                        \
        }                                                 \
        putchar('\n');                                    \
    } while (0)
#endif
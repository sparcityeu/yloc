// #pragma once

#include <interface.h>
#include <iostream>
#include <rocm_smi/rocm_smi.h>

using yloc::graph_t;
using yloc::YlocModule;

class YlocRocm : public YlocModule
{
public:
    void init_graph(graph_t &graph) override;

    void export_graph(const graph_t &graph, void **output) override
    {
        return;
    }

    void update_graph(graph_t &graph) override
    {
        return;
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
#pragma once

#include <bitset>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sched.h>

namespace yloc
{
    class AffinityMask : public std::bitset<NPROCESSORS_ONLN>
    {
    public:
        AffinityMask() {} // Default constructor. Constructs a bitset with all bits set to zero.

        // Linux cpu set.
        AffinityMask(cpu_set_t mask)
        {
            for (int cpu = 0; cpu < NPROCESSORS_ONLN; ++cpu) {
                (*this)[cpu] = CPU_ISSET(cpu, &mask);
            }
        }

        bool is_contained_in(AffinityMask &other) { return (other & *this) == *this; }
        bool is_containing(AffinityMask &other) { return other.is_contained_in(*this); }
    };
}

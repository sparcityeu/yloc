#pragma once

#include <bitset>

#ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#endif /* _GNU_SOURCE */
#include <sched.h>

#ifdef YLOC_WITH_HWLOC
#    include <hwloc.h>
#endif /* YLOC_WITH_HWLOC */

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

#if YLOC_WITH_HWLOC
        AffinityMask(hwloc_bitmap_t mask)
        {
            if (mask == nullptr) { /* TODO */ }
            AffinityMask yloc_mask{};
            unsigned id;
            hwloc_bitmap_foreach_begin(id, mask)
            {
                assert(id < NPROCESSORS_ONLN);
                yloc_mask[id] = hwloc_bitmap_isset(mask, id);
            }
            hwloc_bitmap_foreach_end();
        }
#endif /* YLOC_WITH_HWLOC */

        /**
         * @brief Type conversion from AffinityMask to cpu_set_t.
         *
         * @return cpu_set_t
         */
        operator cpu_set_t() const
        {
            cpu_set_t mask;
            CPU_ZERO(&mask);
            for (int cpu = 0; cpu < NPROCESSORS_ONLN; ++cpu) {
                if ((*this)[cpu]) {
                    CPU_SET(cpu, &mask);
                }
            }
            return mask;
        }

#if YLOC_WITH_HWLOC
        /**
         * @brief Type conversion from AffinityMask to hwloc_bitmap_t.
         * Note: it is the users responsibility to free the allocated 
         * memory with a call to hwloc_bitmap_free.
         *
         * @return hwloc_bitmap_t
         */
        operator hwloc_bitmap_t() const
        {
            hwloc_bitmap_t mask = hwloc_bitmap_alloc();
            if (!mask) { /* TODO */ }
            hwloc_bitmap_zero(mask);
            for (unsigned cpu = 0; cpu < NPROCESSORS_ONLN; ++cpu) {
                if ((*this)[cpu]) {
                    hwloc_bitmap_set(mask, cpu);
                }
            }
            return mask;
        }
#endif /* YLOC_WITH_HWLOC */

        bool is_contained_in(AffinityMask &other)
        {
            return (other & *this) == *this;
        }

        bool is_containing(AffinityMask &other)
        {
            return other.is_contained_in(*this);
        }
    };
} // namespace yloc

#pragma once

#include <boost/dynamic_bitset.hpp>

#ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#endif /* _GNU_SOURCE */
#include <sched.h>
#include <unistd.h>

#ifdef YLOC_WITH_HWLOC
#    include <hwloc.h>
#endif /* YLOC_WITH_HWLOC */

namespace yloc
{
    class AffinityMask : public boost::dynamic_bitset<>
    {
    public:
        /**
         * @brief Construct a new AffinityMask bitset with all bits set to zero.
         *
         */
        AffinityMask() : boost::dynamic_bitset<>{m_nprocs_online} {}

        /**
         * @brief Construct a new AffinityMask from a Linux cpu set.
         *
         * @param mask
         */
        AffinityMask(cpu_set_t mask) : boost::dynamic_bitset<>{m_nprocs_online}
        {
            for (int cpu = 0; cpu < m_nprocs_online; ++cpu) {
                (*this)[cpu] = CPU_ISSET(cpu, &mask);
            }
        }

#if YLOC_WITH_HWLOC
        /**
         * @brief Construct a new AffinityMask from an hwloc bitmap.
         *
         * @param mask
         */
        AffinityMask(hwloc_bitmap_t mask) : boost::dynamic_bitset<>{m_nprocs_online}
        {
            if (mask == nullptr) {
                /* TODO */
            }
            unsigned id;
            hwloc_bitmap_foreach_begin(id, mask)
            {
                assert(id < m_nprocs_online);
                (*this)[id] = hwloc_bitmap_isset(mask, id);
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
            for (int cpu = 0; cpu < m_nprocs_online; ++cpu) {
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
            if (!mask) { /* TODO */
            }
            hwloc_bitmap_zero(mask);
            for (unsigned cpu = 0; cpu < m_nprocs_online; ++cpu) {
                if ((*this)[cpu]) {
                    hwloc_bitmap_set(mask, cpu);
                }
            }
            return mask;
        }
#endif /* YLOC_WITH_HWLOC */

        bool is_contained_in(AffinityMask &other) const
        {
            return (other & *this) == *this;
        }

        bool is_containing(AffinityMask &other) const
        {
            return other.is_contained_in(*this);
        }

        inline const static size_t m_nprocs_online{static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN))};
    };
} // namespace yloc

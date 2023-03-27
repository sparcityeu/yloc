#pragma once

#include <yloc/affinity.h>
#include <yloc/modules/adapter.h>

namespace yloc
{
    class MPIAdapter : public Adapter
    {
        using obj_t = int;

    public:
        MPIAdapter(obj_t obj) : m_obj(obj) {}

        std::string to_string() const override
        {
            return std::to_string(m_obj);
        }

        /** TODO: abstract machine model implementation **/

        std::optional<uint64_t> mpi_rank() const override
        {
            return static_cast<uint64_t>(m_obj);
        }

        std::optional<uint64_t> numa_affinity() const override
        {
            return {};
        }

        std::optional<AffinityMask> cpu_affinity_mask() const override
        {
            if (m_cpu_affinity_mask.any()) {
                return m_cpu_affinity_mask;
            }
            return {};
        }

        /** abstract machine model end **/

        obj_t native_obj() const { return m_obj; }

        void set_cpu_affinity_mask(AffinityMask mask) { m_cpu_affinity_mask = mask; }

    private:
        obj_t m_obj;
        AffinityMask m_cpu_affinity_mask{};
    };
}
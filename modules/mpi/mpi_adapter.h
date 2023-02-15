#pragma once

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

        /** abstract machine model end **/

        obj_t native_obj() const { return m_obj; }

    private:
        obj_t m_obj;
    };
}
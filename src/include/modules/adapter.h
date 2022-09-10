#pragma once

#include <cstdint>
#include <optional>
#include <string>

#define ADAPTER_PROP(type, name) \
    virtual std::optional<type> name() const { return std::optional<type>{}; }

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
        ADAPTER_PROP(std::string, as_string) // String representation

        ADAPTER_PROP(int64_t, temperature) // in millidegrees Celcius

        ADAPTER_PROP(uint64_t, memory)       // in Bytes
        ADAPTER_PROP(uint64_t, memory_usage) // in Bytes
        ADAPTER_PROP(uint64_t, memory_load)  // in percent
        ADAPTER_PROP(uint64_t, memory_frequency)
        ADAPTER_PROP(uint64_t, bdfid) // Bus Device Function ID
        ADAPTER_PROP(uint64_t, numa_affinity)
        ADAPTER_PROP(uint64_t, bandwidth)
        ADAPTER_PROP(uint64_t, bandwidth_min)
        ADAPTER_PROP(uint64_t, bandwidth_max)
        ADAPTER_PROP(uint64_t, throughput)
        ADAPTER_PROP(uint64_t, latency)   // in nanoseconds
        ADAPTER_PROP(uint64_t, frequency) // in Hz
        // ADAPTER_PROP(uint64_t, frequency_min) // in Hz
        // ADAPTER_PROP(uint64_t, frequency_max) // in Hz
        ADAPTER_PROP(uint64_t, power) // in microwatts
        // ADAPTER_PROP(uint64_t, power_max)
        ADAPTER_PROP(uint64_t, usage)
        ADAPTER_PROP(uint64_t, load) // in percent

    protected:
        // vertex_descriptor_t m_vd; // local vertex descriptor
    };
}

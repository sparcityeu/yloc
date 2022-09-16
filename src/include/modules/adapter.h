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

        /** TODO: normalize to millidegrees Kelvin and change to return type uint64_t? */
        /**
         * @brief Gets temperature of component.
         *
         * @return Temperature in millidegrees Celcius or std::nullopt if component has no temperature.
         */
        ADAPTER_PROP(int64_t, temperature)

        /**
         * @brief Gets total memory capacity of component.
         *
         * @return Total memory capacity in bytes or std::nullopt if component has no memory capacity.
         */
        ADAPTER_PROP(uint64_t, memory)

        /**
         * @brief Gets used memory of component.
         *
         * @return Used memory in bytes or std::nullopt if component has no used memory.
         */
        ADAPTER_PROP(uint64_t, memory_usage)

        /**
         * @brief Gets memory load of component.
         *
         * @return Used memory in percent or std::nullopt if component has no memory load.
         */
        ADAPTER_PROP(uint64_t, memory_load)

        /**
         * @brief Gets memory frequency of component.
         *
         * @return Memory frequency in Hz or std::nullopt if component has no memory frequency.
         */
        ADAPTER_PROP(uint64_t, memory_frequency)

        /**
         * @brief Gets bus device function id of component.
         *
         * @return Bus device function id or std::nullopt if component has no bus device function id.
         */
        ADAPTER_PROP(uint64_t, bdfid)

        /**
         * @brief Gets NUMA affinity of component.
         *
         * @return NUMA affinity or std::nullopt if component has no NUMA affinity.
         */
        ADAPTER_PROP(uint64_t, numa_affinity)

        /**
         * @brief Gets bandwidth of component.
         *
         * @return Bandwidth in bytes per second or std::nullopt if component has no bandwidth.
         */
        ADAPTER_PROP(uint64_t, bandwidth)

        ADAPTER_PROP(uint64_t, bandwidth_min)

        ADAPTER_PROP(uint64_t, bandwidth_max)
        ADAPTER_PROP(uint64_t, throughput)

        /**
         * @brief Gets latency of component.
         *
         * @return Latency in nanoseconds or std::nullopt if component has no latency.
         */
        ADAPTER_PROP(uint64_t, latency)

        /**
         * @brief Gets frequency of component.
         *
         * @return Frequency in Hz or std::nullopt if component has no frequency.
         */
        ADAPTER_PROP(uint64_t, frequency) // in Hz

        // ADAPTER_PROP(uint64_t, frequency_min) // in Hz
        // ADAPTER_PROP(uint64_t, frequency_max) // in Hz

        /**
         * @brief Gets power of component.
         *
         * @return Power in microwatt or std::nullopt if component has no power.
         */
        ADAPTER_PROP(uint64_t, power)

        // ADAPTER_PROP(uint64_t, power_max)

        /**
         * @brief Gets usage of component.
         *
         * @return Usage in bytes per second or std::nullopt if component has no usage.
         */
        ADAPTER_PROP(uint64_t, usage)

        /**
         * @brief Gets load of component.
         *
         * @return Load in percent or std::nullopt if component has no load.
         */
        ADAPTER_PROP(uint64_t, load)

        ADAPTER_PROP(uint64_t, pci_throughput)       // read + write in bytes per second
        ADAPTER_PROP(uint64_t, pci_throughput_read)  // in bytes per second
        ADAPTER_PROP(uint64_t, pci_throughput_write) // in bytes per second

    protected:
        // vertex_descriptor_t m_vd; // local vertex descriptor
    };
}

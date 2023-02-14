#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include <yloc/modules/property.h>
#include <yloc/yloc_status.h> // yloc_status_t

/**
 * @brief Macro to define properties.
 * 
 */
#define ADAPTER_PROPERTY(type, name) \
    virtual std::optional<type> name() const { return std::optional<type>{}; }

namespace yloc
{
    class Adapter // TODO: Maybe separate between VertexAdapter and EdgeAdapter
    {
    public:
        /** TODO: Deconstructor and lifetime of native objects */
        // Adapter() = default;
        virtual ~Adapter() = default;

        /** predefined yloc-global property map */
        static std::unordered_map<std::string_view, Property> &map()
        {
            static std::unordered_map<std::string_view, Property> map{
                {make_property_pair("memory", &Adapter::memory)},
                {make_property_pair("memory_usage", &Adapter::memory_usage)},
                {make_property_pair("memory_load", &Adapter::memory_load)},
                {make_property_pair("memory_frequency", &Adapter::memory_frequency)},
                {make_property_pair("bdfid", &Adapter::bdfid)},
                {make_property_pair("numa_affinity", &Adapter::numa_affinity)},
                {make_property_pair("bandwidth", &Adapter::bandwidth)},
                {make_property_pair("bandwidth_min", &Adapter::bandwidth_min)},
                {make_property_pair("bandwidth_max", &Adapter::bandwidth_max)},
                {make_property_pair("throughput", &Adapter::throughput)},
                {make_property_pair("latency", &Adapter::latency)},
                {make_property_pair("frequency", &Adapter::frequency)},
                /** TODO: maybe support multiple return types in Vertex::get("property") (e.g. Vertex::get<return_type>("property")) 
                 * or change temperature scale to degree Kelvin and return type to uint64_t */
                // {make_property_pair("temperature", &Adapter::temperature)},
                {make_property_pair("power", &Adapter::power)},
                {make_property_pair("usage", &Adapter::usage)},
                {make_property_pair("load", &Adapter::load)},
                {make_property_pair("pci_throughput", &Adapter::pci_throughput)},
                {make_property_pair("pci_throughput_read", &Adapter::pci_throughput_read)},
                {make_property_pair("pci_throughput_write", &Adapter::pci_throughput_write)},
                {make_property_pair("mpi_rank", &Adapter::mpi_rank)}};
            return map;
        }

        /**
         * @brief 
         * 
         * @return std::unordered_map<std::string_view, Property>& 
         */
        virtual std::unordered_map<std::string_view, Property> &mmap()
        {
            return map();
        }

        /* virtual Module *        source() = 0; */
        /* virtual vertex_descriptor_t source_descriptor() { return m_vd; } */

        /**
         * @brief Gets string representation of component.
         *
         * @return String representation.
         */
        virtual std::string to_string() const { return {}; }

        /** abstract machine model begin */

        /** TODO: normalize to millidegrees Kelvin and change to return type uint64_t? */
        /**
         * @brief Gets temperature of component.
         *
         * @return Temperature in millidegrees Celcius or std::nullopt if component has no temperature.
         */
        ADAPTER_PROPERTY(int64_t, temperature)

        /**
         * @brief Gets total memory capacity of component.
         *
         * @return Total memory capacity in bytes or std::nullopt if component has no memory capacity.
         */
        ADAPTER_PROPERTY(uint64_t, memory)

        /**
         * @brief Gets used memory of component.
         *
         * @return Used memory in bytes or std::nullopt if component has no used memory.
         */
        ADAPTER_PROPERTY(uint64_t, memory_usage)

        /**
         * @brief Gets memory load of component.
         *
         * @return Used memory in percent or std::nullopt if component has no memory load.
         */
        ADAPTER_PROPERTY(uint64_t, memory_load)

        /**
         * @brief Gets memory frequency of component.
         *
         * @return Memory frequency in Hz or std::nullopt if component has no memory frequency.
         */
        ADAPTER_PROPERTY(uint64_t, memory_frequency)

        /**
         * @brief Gets bus device function id of component.
         *
         * @return Bus device function id or std::nullopt if component has no bus device function id.
         */
        ADAPTER_PROPERTY(uint64_t, bdfid)

        /**
         * @brief Gets NUMA affinity of component.
         *
         * @return NUMA affinity or std::nullopt if component has no NUMA affinity.
         */
        ADAPTER_PROPERTY(uint64_t, numa_affinity)

        /**
         * @brief Gets bandwidth of component.
         *
         * @return Bandwidth in bytes per second or std::nullopt if component has no bandwidth.
         */
        ADAPTER_PROPERTY(uint64_t, bandwidth)

        /** TODO: documentation */
        ADAPTER_PROPERTY(uint64_t, bandwidth_min)
        ADAPTER_PROPERTY(uint64_t, bandwidth_max)
        ADAPTER_PROPERTY(uint64_t, throughput)

        /**
         * @brief Gets latency of component.
         *
         * @return Latency in nanoseconds or std::nullopt if component has no latency.
         */
        ADAPTER_PROPERTY(uint64_t, latency)

        /**
         * @brief Gets frequency of component.
         *
         * @return Frequency in Hz or std::nullopt if component has no frequency.
         */
        ADAPTER_PROPERTY(uint64_t, frequency) // in Hz

        // ADAPTER_PROPERTY(uint64_t, frequency_min) // in Hz
        // ADAPTER_PROPERTY(uint64_t, frequency_max) // in Hz

        /**
         * @brief Gets power of component.
         *
         * @return Power in microwatt or std::nullopt if component has no power.
         */
        ADAPTER_PROPERTY(uint64_t, power)

        // ADAPTER_PROPERTY(uint64_t, power_max)

        /**
         * @brief Gets usage of component.
         *
         * @return Usage in bytes per second or std::nullopt if component has no usage.
         */
        ADAPTER_PROPERTY(uint64_t, usage)

        /**
         * @brief Gets load of component.
         *
         * @return Load in percent or std::nullopt if component has no load.
         */
        ADAPTER_PROPERTY(uint64_t, load)

        /** TODO: documentation */
        ADAPTER_PROPERTY(uint64_t, pci_throughput)       // read + write in bytes per second
        ADAPTER_PROPERTY(uint64_t, pci_throughput_read)  // in bytes per second
        ADAPTER_PROPERTY(uint64_t, pci_throughput_write) // in bytes per second

        ADAPTER_PROPERTY(uint64_t, mpi_rank) // rank in comm world

        /** abstract machine model end */
    };
}

#undef ADAPTER_PROPERTY

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

#include <property.h>
#include <yloc.h> // yloc_status_t

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

        /** predefined yloc-global property map */
        static std::unordered_map<const char *, AbstractProperty *> &map()
        {
            static std::unordered_map<const char *, AbstractProperty *> map{
                {"memory", make_property("memory", &Adapter::memory)},
                {"memory_usage", make_property("memory_usage", &Adapter::memory_usage)},
                {"memory_load", make_property("memory_load", &Adapter::memory_load)},
                {"memory_frequency", make_property("memory_frequency", &Adapter::memory_frequency)},
                {"bdfid", make_property("bdfid", &Adapter::bdfid)},
                {"numa_affinity", make_property("numa_affinity", &Adapter::numa_affinity)},
                {"bandwidth", make_property("bandwidth", &Adapter::bandwidth)},
                {"bandwidth_min", make_property("bandwidth_min", &Adapter::bandwidth_min)},
                {"bandwidth_max", make_property("bandwidth_max", &Adapter::bandwidth_max)},
                {"throughput", make_property("throughput", &Adapter::throughput)},
                {"latency", make_property("latency", &Adapter::latency)},
                {"frequency", make_property("frequency", &Adapter::frequency)},
                {"power", make_property("power", &Adapter::power)},
                {"usage", make_property("usage", &Adapter::usage)},
                {"load", make_property("load", &Adapter::load)},
                {"pci_throughput", make_property("pci_throughput", &Adapter::pci_throughput)},
                {"pci_throughput_read", make_property("pci_throughput_read", &Adapter::pci_throughput_read)},
                {"pci_throughput_write", make_property("pci_throughput_write", &Adapter::pci_throughput_write)}};

            return map;
        }

        /**
         * @brief TODO
         * 
         * @return std::unordered_map<const char *, AbstractProperty *>& 
         */
        virtual std::unordered_map<const char *, AbstractProperty *> &mmap()
        {
            return map();
        }

        /* virtual YlocModule *        source() = 0; */
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

        /** TODO: documentation */
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

        /** TODO: documentation */
        ADAPTER_PROP(uint64_t, pci_throughput)       // read + write in bytes per second
        ADAPTER_PROP(uint64_t, pci_throughput_read)  // in bytes per second
        ADAPTER_PROP(uint64_t, pci_throughput_write) // in bytes per second

        /** abstract machine model end */
    };
}

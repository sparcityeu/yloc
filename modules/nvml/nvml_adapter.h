#pragma once

#include "nvml_util.h"
#include <bits/stdc++.h>
#include <nvml.h>
#include <yloc/modules/adapter.h>

namespace yloc
{
    class NvmlAdapter : public Adapter
    {
        using obj_t = nvmlDevice_t; // NVML device type

    public:
        NvmlAdapter(obj_t obj) : m_obj(obj) {}

        std::optional<std::string> as_string() const override
        {
            constexpr size_t MAX_NAME_LEN = NVML_DEVICE_NAME_V2_BUFFER_SIZE;
            char name[MAX_NAME_LEN];
            CHECK_NVML(nvmlDeviceGetName(m_obj, name, MAX_NAME_LEN));
            name[MAX_NAME_LEN - 1] = '\0';
            return name;
        }
        /** TODO: abstract machine model implementation **/

        /** @see NVML documentation */

        std::optional<uint64_t> memory() const override
        {

            nvmlMemory_t memory;
            if (!CHECK_NVML(nvmlDeviceGetMemoryInfo(m_obj, &memory))) {
                return {};
            }
            return memory.total;
        }

        std::optional<uint64_t> memory_usage() const override
        {
            nvmlMemory_t memory;
            if (!CHECK_NVML(nvmlDeviceGetMemoryInfo(m_obj, &memory))) {
                return {};
            }
            return memory.used;
        }

        /** TODO: return rate or ratio here? */
        std::optional<uint64_t> memory_load() const override
        {
            nvmlUtilization_t load;
            if (!CHECK_NVML(nvmlDeviceGetUtilizationRates(m_obj, &load))) {
                return {};
            }
            return static_cast<uint64_t>(load.memory);
        }

#if 0
    std::optional<uint64_t> memory_load() const override
    {
        nvmlMemory_t memory;
        if (!CHECK_NVML(nvmlDeviceGetMemoryInfo(m_obj, &memory))) {
            return {};
        }
        return (100 * memory.used) / memory.total;
    }
#endif

        std::optional<uint64_t> memory_frequency() const override
        {
            unsigned int clockMHz;
            if (!CHECK_NVML(nvmlDeviceGetClock(m_obj, NVML_CLOCK_MEM, NVML_CLOCK_ID_CURRENT, &clockMHz))) {
                return {};
            }
            return static_cast<uint64_t>(clockMHz) * 1000 * 1000;
        }

        /** TODO: memory_busy ? */

        std::optional<uint64_t> bdfid() const override
        {
            nvmlPciInfo_t pci;
            if (!CHECK_NVML_MSG(nvmlDeviceGetPciInfo_v3(m_obj, &pci))) {
                return {};
            }
            return bdfid_get(pci);
        }

        std::optional<uint64_t> numa_affinity() const override
        {
            unsigned int nodeSetSize = 1;
            unsigned long nodeSet;
            if (!CHECK_NVML(nvmlDeviceGetMemoryAffinity(m_obj, nodeSetSize, &nodeSet, NVML_AFFINITY_SCOPE_NODE))) {
                return {};
            }
            // nodeSet is bit set
            return static_cast<uint64_t>(ffs(nodeSet));
        }

        std::optional<int64_t> temperature() const override
        {
            unsigned int temperature;
            if (!CHECK_NVML(nvmlDeviceGetTemperature(m_obj, NVML_TEMPERATURE_GPU, &temperature))) {
                return {};
            }
            return static_cast<int64_t>(temperature) * 1000;
        }

        std::optional<uint64_t> frequency() const override
        {
            // NVML_CLOCK_GRAPHICS = 0
            // Graphics clock domain.
            // NVML_CLOCK_SM = 1
            // SM clock domain.
            // NVML_CLOCK_MEM = 2
            // Memory clock domain.
            // NVML_CLOCK_VIDEO = 3
            // Video encoder/decoder clock domain.
            // NVML_CLOCK_COUNT
            // Count of clock types.
            /** TODO: or SM clock ? */
            unsigned int clockMHz;
            if (!CHECK_NVML(nvmlDeviceGetClock(m_obj, NVML_CLOCK_GRAPHICS, NVML_CLOCK_ID_CURRENT, &clockMHz))) {
                return {};
            }
            return static_cast<uint64_t>(clockMHz) * 1000 * 1000;
        }

        std::optional<uint64_t> load() const override
        {
            nvmlUtilization_t load;
            if (!CHECK_NVML(nvmlDeviceGetUtilizationRates(m_obj, &load))) {
                return {};
            }
            return static_cast<uint64_t>(load.gpu);
        }

        std::optional<uint64_t> power() const override
        {
            unsigned int power; // milliwatts
            if (!CHECK_NVML(nvmlDeviceGetPowerUsage(m_obj, &power))) {
                return {};
            }
            return static_cast<uint64_t>(power) * 1000;
        }

        /** abstract machine model end **/

        obj_t native_obj() const { return m_obj; }

    private:
        obj_t m_obj;
    };
}

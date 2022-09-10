#pragma once

#include <adapter.h>
#include <rocm_smi/rocm_smi.h>

#include "rocm_util.h"

class RocmAdapter : public yloc::Adapter
{
    using obj_t = uint32_t; // rocm device index

public:
    RocmAdapter(obj_t obj) : m_obj(obj) {}

    std::optional<std::string> as_string() const override
    {
        constexpr size_t MAX_NAME_LEN = 2048;
        char name[MAX_NAME_LEN];
        CHECK_ROCM(rsmi_dev_name_get(m_obj, name, MAX_NAME_LEN));
        name[MAX_NAME_LEN - 1] = '\0';
        return name;
    }

    /** TODO: abstract machine model implementation **/

    /** @see ROCm-SMI documentation */
    std::optional<uint64_t> memory() const override
    {
        /** TODO: what to return here? returning ?visible? VRAM like rocm-smi tool */
        // possible memory types:
        // RSMI_MEM_TYPE_VRAM, RSMI_MEM_TYPE_VIS_VRAM, RSMI_MEM_TYPE_GTT
        uint64_t memory_total;
        if (!CHECK_ROCM(rsmi_dev_memory_total_get(m_obj, RSMI_MEM_TYPE_VIS_VRAM, &memory_total))) {
            return {};
        }
        return memory_total;
    }

    std::optional<uint64_t> memory_usage() const override
    {
        uint64_t memory_usage;
        if (!CHECK_ROCM(rsmi_dev_memory_usage_get(m_obj, RSMI_MEM_TYPE_VIS_VRAM, &memory_usage))) {
            return {};
        }
        return memory_usage;
    }

    std::optional<uint64_t> memory_load() const override
    {
        uint32_t load;
        if(!CHECK_ROCM(rsmi_dev_memory_busy_percent_get(m_obj, &load))) {
            return {};
        }
        return static_cast<uint64_t>(load);
    }

    std::optional<uint64_t> memory_frequency() const override
    {
        rsmi_frequencies_t f;
        if (!CHECK_ROCM(rsmi_dev_gpu_clk_freq_get(m_obj, RSMI_CLK_TYPE_MEM, &f))) {
            return {};
        }
        return f.frequency[f.current];
    }

    /** TODO: memory_busy ? */

    std::optional<uint64_t> bdfid() const override
    {
        uint64_t bdfid;
        if (!CHECK_ROCM(rsmi_dev_pci_id_get(m_obj, &bdfid))) {
            return {};
        }
        return bdfid;
    }

    std::optional<uint64_t> numa_affinity() const override
    {
        uint32_t numa_affinity;
        if (!CHECK_ROCM(rsmi_topo_numa_affinity_get(m_obj, &numa_affinity))) {
            return {};
        }
        return static_cast<uint64_t>(numa_affinity);
    }

    std::optional<int64_t> temperature() const override
    {
        // RSMI_TEMP_TYPE_EDGE Edge GPU temperature.
        // RSMI_TEMP_TYPE_JUNCTION Junction/hotspot temperature
        // RSMI_TEMP_TYPE_MEMORY VRAM temperature.
        // RSMI_TEMP_TYPE_HBM_0 HBM temperature instance 0.
        // RSMI_TEMP_TYPE_HBM_1 HBM temperature instance 1.
        // RSMI_TEMP_TYPE_HBM_2 HBM temperature instance 2.
        // RSMI_TEMP_TYPE_HBM_3 HBM temperature instance 3.
        // RSMI_TEMP_TYPE_INVALID Invalid type.
        int64_t temperature;
        if(!CHECK_ROCM(rsmi_dev_temp_metric_get(m_obj, RSMI_TEMP_TYPE_EDGE, RSMI_TEMP_CURRENT, &temperature))) {
            return {};
        }
        return temperature;
    }

    std::optional<uint64_t> frequency() const override
    {
        // CLOCKS:
        // RSMI_CLK_TYPE_SYS System clock.
        // RSMI_CLK_TYPE_DF Data Fabric clock (for ASICs running on a separate clock)
        // RSMI_CLK_TYPE_DCEF Display Controller Engine clock.
        // RSMI_CLK_TYPE_SOC SOC clock.
        // RSMI_CLK_TYPE_MEM Memory clock.

        rsmi_frequencies_t f;
        if (!CHECK_ROCM(rsmi_dev_gpu_clk_freq_get(m_obj, RSMI_CLK_TYPE_SYS, &f))) {
            return {};
        }
        return f.frequency[f.current];
    }

    std::optional<uint64_t> load() const override
    {
        uint32_t load;
        if(!CHECK_ROCM(rsmi_dev_busy_percent_get(m_obj, &load))) {
            return {};
        }
        return static_cast<uint64_t>(load);
    }

    std::optional<uint64_t> power() const override
    {
        uint64_t power;
        uint32_t sensor_ind = 0;
        if (!CHECK_ROCM(rsmi_dev_power_ave_get(m_obj, sensor_ind, &power))) {
            return {};
        }
        return power;
    }

    /** abstract machine model end **/

    obj_t native_obj() const { return m_obj; }

private:
    obj_t m_obj;
};

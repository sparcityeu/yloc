#pragma once

#include <yloc/affinity.h>
#include <yloc/modules/adapter.h>

#include <hwloc.h>

#include <sstream>

#define VERBOSE_HWLOC_ATTRIBUTES 0

namespace yloc
{
    class HwlocAdapter : public Adapter
    {
        using obj_t = hwloc_obj_t;

    public:
        HwlocAdapter(obj_t obj) : m_obj(obj) {}

        std::string to_string() const override
        {
            std::stringstream ss;
            ss << hwloc_obj_type_string(m_obj->type); /*<< std::endl*/
#if VERBOSE_HWLOC_ATTRIBUTES
            int verbose = 1;
            const size_t attribute_len_max = 4096;
            char attributes[attribute_len_max];
            hwloc_obj_attr_snprintf(attributes, attribute_len_max, m_obj, "\n", verbose);
            ss << attributes;
#endif
            return ss.str();
        }

        /** TODO: abstract machine model implementation **/
        /** @see man hwloc_obj_attr_u */
        std::optional<uint64_t> memory() const override
        {
            if (hwloc_obj_type_is_cache(m_obj->type)) {
                return m_obj->attr->cache.size;
            } else if (hwloc_obj_type_is_memory(m_obj->type)) {
                return m_obj->attr->numanode.local_memory;
            } else if (hwloc_obj_type_is_io(m_obj->type)) {
                /** TODO: implementation for SSD, non-volatile memory etc... */
            }
            return {};
        }

        std::optional<uint64_t> bdfid() const override
        {
            // bdfid format:
            // BDFID = ((DOMAIN & 0xffffffff) << 32) | ((BUS & 0xff) << 8) |
            // ((DEVICE & 0x1f) <<3 ) | (FUNCTION & 0x7)
            if (!hwloc_compare_types(m_obj->type, HWLOC_OBJ_PCI_DEVICE) /** TODO: || or ... whatever has BDFID */) {
                return ((static_cast<uint64_t>(m_obj->attr->pcidev.domain) & 0xffffffff) << 32) | ((static_cast<uint64_t>(m_obj->attr->pcidev.bus) & 0xff) << 8) |
                       ((static_cast<uint64_t>(m_obj->attr->pcidev.dev) & 0x1f) << 3) | (static_cast<uint64_t>(m_obj->attr->pcidev.func) & 0x7);
            }
            return {};
        }

        std::optional<AffinityMask> cpu_affinity_mask() const override
        {
            if (m_obj->cpuset == nullptr) {
                return {};
            }
            return m_obj->cpuset;
        }

        /** abstract machine model end **/

        obj_t native_obj() const { return m_obj; }

    private:
        obj_t m_obj;
    };
}
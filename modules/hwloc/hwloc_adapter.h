#pragma once

#include <adapter.h>
#include <hwloc.h>
#include <sstream>

#define VERBOSE_HWLOC_ATTRIBUTES 0

class HwlocAdapter : public yloc::Adapter
{
    using obj_t = hwloc_obj_t;

public:
    HwlocAdapter(obj_t obj) : m_obj(obj) {}

    std::optional<std::string> as_string() const override
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
        return std::optional<std::string>{ss.str()};
    }

    /** TODO: abstract machine model implementation **/
    /** @see man hwloc_obj_attr_u */
    std::optional<uint64_t> size() const override
    {
        if(hwloc_obj_type_is_cache(m_obj->type)) {
            return std::optional<uint64_t>{m_obj->attr->cache.size};
        } else if(hwloc_obj_type_is_memory(m_obj->type)) {
            return std::optional<uint64_t>{m_obj->attr->numanode.local_memory};
        } else if(hwloc_obj_type_is_io(m_obj->type)) {
            /** TODO: implementation for SSD, non-volatile memory etc... */
        }
        return std::optional<uint64_t>{};
    }
    /** abstract machine model end **/

    obj_t native_obj() const { return m_obj; }

private:
    obj_t m_obj;
};



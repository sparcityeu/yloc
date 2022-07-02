#pragma once

#include <adapter.h>
#include <hwloc.h>
#include <sstream>

class HwlocAdapter : public yloc::Adapter
{
    using obj_t = hwloc_obj_t;

public:
    HwlocAdapter(obj_t obj) : m_obj(obj) {}

    std::optional<std::string> as_string() override
    {
        std::stringstream ss;
        ss << hwloc_obj_type_string(m_obj->type); /*<< std::endl*/
#if VERBOSE_HWLOC_ATTRIBUTES
        int verbose = 1;
        const char *sep = "\n";
        const size_t attribute_len_max = 4096;
        char attributes[attribute_len_max];
        hwloc_obj_attr_snprintf(attributes, attribute_len_max, obj, sep, verbose);
        ss << attributes;
#endif
        return std::optional<std::string>{ss.str()};
    }

    /** TODO: abstract machine model implementation **/
    /** abstract machine model end **/

    obj_t native_obj() const { return m_obj; }

private:
    obj_t m_obj;
};

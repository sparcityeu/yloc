#include <cinttypes> // uint64_t
#include <optional>
#include <string_view>

#include <yloc/vertex.h>
#include <yloc/modules/adapter.h>
#include <yloc/modules/property.h>

namespace yloc
{
    std::optional<uint64_t> Vertex::get(std::string_view property) const
    {
        // first search global yloc properties
        auto it = Adapter::map().find(property);
        if (it != Adapter::map().end()) {
            Property &p = it->second;
            for (Adapter *a : m_adapters) {
                auto ret = p(a);
                if (ret.has_value()) {
                    return ret;
                }
            }
        }
        // then search custom module-specific properties
        else {
            for (Adapter *a : m_adapters) {
                auto it = a->mmap().find(property);
                if (it != a->mmap().end()) {
                    Property &p = it->second;
                    auto ret = p(a);
                    if (ret.has_value()) {
                        return ret;
                    }
                }
            }
        }
        return {};
    }
}

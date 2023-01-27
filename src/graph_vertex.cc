#include <cinttypes> // uint64_t
#include <optional>
/** TODO: provide reason why you should include all headers that are required in implementation */
// #include <unordered_map>

#include <graph_vertex.h>
#include <modules/adapter.h>
#include <property.h>

namespace yloc
{
    std::optional<uint64_t> Vertex::get(const char *property)
    {
        // first search default yloc properties
        auto it = Adapter::map().find(property);
        if (it != Adapter::map().end()) {
            AbstractProperty *p = it->second;
            for (auto a : m_adapters) {
                auto ret = p->get(a);
                if (ret.has_value()) {
                    return ret;
                }
            }
        }
        // then search custom module properties
        else {
            for (auto a : m_adapters) {
                auto it = a->mmap().find(property);
                AbstractProperty *p = it->second;
                auto ret = p->get(a);
                if (ret.has_value()) {
                    return ret;
                }
            }
        }
        return {};
    }
}

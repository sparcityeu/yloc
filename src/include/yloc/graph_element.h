#pragma once

#include <yloc/component_types.h>
#include <yloc/modules/adapter.h>
#include <yloc/modules/property.h>

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace yloc
{
    /* vertex / edge properties */
    struct GraphElement {

        template <class RT>
        std::optional<RT> get(std::string_view property_name) const
        {
            // first search global yloc properties
            auto it = Adapter::map().find(property_name);
            if (it != Adapter::map().end()) {
                auto property = it->second;
                if (property->supports(typeid(RT))) {
                    for (std::shared_ptr<Adapter> a : m_adapters) {
                        auto ret = std::dynamic_pointer_cast<Property<RT>>(property)->value(a.get());
                        if (ret.has_value()) {
                            return ret;
                        }
                    }
                }
            }
            // then search custom module-specific properties
            else {
                for (std::shared_ptr<Adapter> a : m_adapters) {
                    auto it = a->module_map().find(property_name);
                    if (it != a->module_map().end()) {
                        auto property = it->second;
                        if (property->supports(typeid(RT))) {
                            auto ret = std::dynamic_pointer_cast<Property<RT>>(property)->value(a.get());
                            if (ret.has_value()) {
                                return ret;
                            }
                        }
                    }
                }
            }
            return {};
        }

        template <class ComponentType>
        bool is_a() const noexcept { return m_type->is_a<ComponentType>(); }

        std::string to_string() const { return std::string{m_type->to_string()} + ": " + m_description; }

        [[deprecated("use void add_adapter(std::shared_ptr<Adapter> a) instead")]] void add_adapter(Adapter *a)
        {
            m_adapters.push_back(std::shared_ptr<Adapter>{a});
        }

        void add_adapter(std::shared_ptr<Adapter> a)
        {
            m_adapters.push_back(a);
        }

        std::vector<std::shared_ptr<Adapter>> m_adapters{};
        std::string m_description{};
        const Component *m_type{UnknownComponentType::ptr()};
    };

    template <>
    inline std::optional<std::string> GraphElement::get<std::string>(std::string_view property_name) const
    {
        // first search global yloc properties
        auto it = Adapter::map().find(property_name);
        if (it != Adapter::map().end()) {
            auto property = it->second;
            for (std::shared_ptr<Adapter> a : m_adapters) {
                auto ret = property->value_to_string(a.get());
                if (ret.has_value()) {
                    return ret;
                }
            }
        }
        // then search custom module-specific properties
        else {
            for (std::shared_ptr<Adapter> a : m_adapters) {
                auto it = a->module_map().find(property_name);
                if (it != a->module_map().end()) {
                    auto property = it->second;
                    auto ret = property->value_to_string(a.get());
                    if (ret.has_value()) {
                        return ret;
                    }
                }
            }
        }
        return {};
    }

    struct Vertex : public GraphElement {
    };

    enum class edge_type : int { PARENT,
                                 CHILD,
                                 GPU_INTERCONNECT,
                                 EDGE_TYPE_MAX };

    struct Edge : public GraphElement {
        Edge(edge_type type) : m_edgetype{type} {}
        edge_type m_edgetype;
    };
}
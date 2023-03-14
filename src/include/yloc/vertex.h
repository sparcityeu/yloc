#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <yloc/component_types.h>
#include <yloc/modules/adapter.h>
#include <yloc/modules/property.h>

namespace yloc
{
    /* bundled internal (common) vertex properties */
    struct Vertex {
        const Component *type{UnknownComponentType::ptr()};

        template <class RT>
        std::optional<RT> get(std::string_view property_name) const
        {
            // first search global yloc properties
            auto it = Adapter::map().find(property_name);
            if (it != Adapter::map().end()) {
                auto *property = it->second;
                if(property->supports(typeid(RT))) {
                    for (Adapter *a : m_adapters) {
                        auto ret = dynamic_cast<Property<RT> *>(property)->value(a);
                        if (ret.has_value()) {
                            return ret;
                        }
                    }
                }
            }
            // then search custom module-specific properties
            else {
                for (Adapter *a : m_adapters) {
                    auto it = a->module_map().find(property_name);
                    if (it != a->module_map().end()) {
                        auto *property = it->second;
                        if(property->supports(typeid(RT))) {
                            auto ret = dynamic_cast<Property<RT> *>(property)->value(a);
                            if (ret.has_value()) {
                                return ret;
                            }
                        }
                    }
                }
            }
            return {};
        }

        std::string to_string() const { return std::string{type->to_string()} + ": " + m_description; }

        void add_adapter(Adapter *a)
        {
            m_adapters.push_back(a);
        }

        std::vector<Adapter *> m_adapters{};
        std::string m_description{};
    };

    template <>
    inline std::optional<std::string> Vertex::get<std::string>(std::string_view property_name) const
    {
        // first search global yloc properties
        auto it = Adapter::map().find(property_name);
        if (it != Adapter::map().end()) {
            auto *property = it->second;
            for (Adapter *a : m_adapters) {
                auto ret = property->value_to_string(a);
                if (ret.has_value()) {
                    return ret;
                }
            }
        }
        // then search custom module-specific properties
        else {
            for (Adapter *a : m_adapters) {
                auto it = a->module_map().find(property_name);
                if (it != a->module_map().end()) {
                    auto *property = it->second;
                    auto ret = property->value_to_string(a);
                    if (ret.has_value()) {
                        return ret;
                    }
                }
            }
        }
        return {};
    }
}

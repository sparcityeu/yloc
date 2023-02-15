#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <yloc/component_types.h>

namespace yloc
{
    class Adapter;

    /* bundled internal (common) vertex properties */
    struct Vertex {
        const Component *type{UnknownComponentType::ptr()};

        std::optional<uint64_t> get(std::string_view property) const;

        std::string to_string() const { return std::string{type->to_string()} + ": " + m_description; }

        void add_adapter(Adapter *a)
        {
            m_adapters.push_back(a);
        }

        std::vector<Adapter *> m_adapters{};
        std::string m_description{};
    };
}

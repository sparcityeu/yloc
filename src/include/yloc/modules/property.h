#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <sstream>
#include <type_traits>

namespace yloc
{
    class Adapter;

    struct AbstractProperty {
        virtual bool supports(const std::type_info &type) const = 0;
        virtual std::optional<std::string> value_to_string(Adapter *a) const = 0;
    };

    /**
     * @brief Class holding the name of a property and its member function pointer in a module's adapter.
     *
     */
    template <class RT>
    class Property final : public AbstractProperty
    {
    public:
        using property_fn_t = std::optional<RT> (Adapter::*)() const;

        Property(const char *name, property_fn_t pf) : m_name{name}, m_property_fn{pf} {}

        bool supports(const std::type_info &type) const override
        {
            return type == typeid(RT);
        }

        const char *name() const
        {
            return m_name;
        }

        /**
         * @brief Returns the module-specific property value of a graph element.
         *
         * @param a The module-specific adapter of a graph element
         * @return std::optional<RT> The property value
         */
        std::optional<RT> value(Adapter *a) const
        {
            return std::invoke(m_property_fn, a); // (a->*m_property_fn)();
        }

        std::optional<std::string> value_to_string(Adapter *a) const override
        {
            auto v = value(a);
            std::stringstream ss;
            if (v.has_value()) {
                ss << v.value();
                return ss.str();
            }
            return {};
        }

    private:
        const char *m_name;          // Property name
        property_fn_t m_property_fn; // Property member function
    };

    /**
     * @brief Creates a std::pair of (property string, property function).
     *
     * @tparam AdapterType The module-specific adapter type (derived from adapter class)
     * @tparam RT The type of the property
     * @param property_name The name of the the property
     * @param pf The property-retrieving member function of the adapter
     * @return std::pair<const char *, Property> The (property string, property function) pair
     */
    template <class AdapterType, class RT>
    std::pair<const char *, std::shared_ptr<AbstractProperty>>
    make_property_pair(const char *property_name, std::optional<RT> (AdapterType::*pf)() const)
    {
        static_assert(std::is_base_of_v<Adapter, AdapterType>);
        static_assert(std::is_invocable_r_v<std::optional<RT>, decltype(pf), Adapter>);

        return {property_name,
                // upcasting the member function pointer to base (Adapter) is safe if unambiguous
                std::make_shared<Property<RT>>(property_name, static_cast<typename Property<RT>::property_fn_t>(pf))};
    }
} // namespace yloc

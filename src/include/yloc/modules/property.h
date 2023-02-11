#pragma once

#include <cinttypes>
#include <optional>

namespace yloc
{
    class Adapter;

    /**
     * @brief Class holding the name of a property and its member function pointer in a module's adapter.
     * 
     */
    class Property
    {
    public:
        using property_fn_t = std::optional<uint64_t> (Adapter::*)() const;

        Property(const char *name, property_fn_t pf) : m_name{name}, m_pf{pf} {}
        

        const char *name() const { return m_name; }

        /**
         * @brief Returns the module-specific property value of a graph element.
         * 
         * @param a The module-specific adapter of a graph element
         * @return std::optional<uint64_t> The property value
         */
        std::optional<uint64_t> operator()(Adapter *a) const
        {
            return (a->*m_pf)();
        }

    private:
        const char *m_name; // Property name
        property_fn_t m_pf; // Property member function
    };

    /**
     * @brief Creates a std::pair of (property string, property function).
     * 
     * @tparam AdapterType The module-specific adapter type (derived from adapter class)
     * @param property_name The name of the the property
     * @param pf The property-retrieving member function of the adapter
     * @return std::pair<const char *, Property> The (property string, property function) pair
     */
    template <class AdapterType>
    std::pair<const char *, Property> make_property_pair(const char *property_name, std::optional<uint64_t> (AdapterType::*pf)() const)
    {
        return {property_name, Property{property_name,
            // upcasting the member function pointer to base (Adapter) is safe if unambiguous
            static_cast<Property::property_fn_t>(pf)}};
    }
}

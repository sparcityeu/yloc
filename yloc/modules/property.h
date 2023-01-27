#pragma once

#include <cinttypes>
#include <optional>

namespace yloc
{
    class Adapter;

    /**
     * @brief TODO
     *
     */
    class AbstractProperty
    {
    public:
        AbstractProperty(const char *property_name) : m_property_name{property_name} {}
        AbstractProperty() = delete;
        // ~AbstractProperty();

        const char *name() { return m_property_name; }
        virtual std::optional<uint64_t> get(Adapter *a) const = 0;

    protected:
        const char *m_property_name;
    };

    /** TODO: template on return type ? */

    /**
     * @brief TODO
     *
     * @tparam AdapterType
     */
    template <class AdapterType>
    class Property : public AbstractProperty
    {
        using property_mem_fn_t = std::optional<uint64_t> (AdapterType::*)() const;

    private:
        property_mem_fn_t m_pmfu64;

    public:
        Property(const char *property_name, property_mem_fn_t pmfu64) : AbstractProperty{property_name}, m_pmfu64{pmfu64} {}

        virtual std::optional<uint64_t> get(Adapter *a) const override
        {
            AdapterType *ca = dynamic_cast<AdapterType *>(a);
            if (ca == nullptr) {
                return {};
            }

            // CALL_MEMBER_FN(*ca, m_pmfu64)();
            return ((*ca).*(m_pmfu64))();
        }
    };

    /**
     * @brief TODO
     *
     * @tparam AdapterType
     * @param property_name
     * @param pmfu64
     * @return Property<AdapterType>*
     */
    template <class AdapterType>
    Property<AdapterType> *make_property(const char *property_name, std::optional<uint64_t> (AdapterType::*pmfu64)() const)
    {
        return new Property<AdapterType>{property_name, pmfu64};
    }
}

#pragma once

#include <optional>
#include <vector>
// #include <modules/adapter.h>
#include <adapter.h>

#define YLOC_PROPERTY(prop) &yloc::Adapter::prop

namespace yloc
{
    class AdapterContainer
    {
    public:
        //~AdapterContainer

        void push_back(Adapter *a)
        {
            m_adapters.push_back(a);
        }

        template <typename property_type>
        std::optional<property_type> get(std::optional<property_type> (Adapter::*mfun)())
        {
            for (auto a : m_adapters) {
                /** FIXME: better use std::invoke() than macro **/
#define CALL_MEMBER_FN(object, ptrToMember) ((object).*(ptrToMember))
                std::optional<property_type> res = CALL_MEMBER_FN(*a, mfun)();
#undef CALL_MEMBER_FN

                if (res.has_value()) {
                    return res;
                }
            }
            return std::optional<property_type>{};
        }

    private:
        std::vector<Adapter *> m_adapters;
        /** TODO: yloc vertex/edge type */
    };
}
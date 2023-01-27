#pragma once

#include <yloc/modules/adapter.h>

namespace yloc
{
    class SuperMUCAdapter : public Adapter
    {
        using obj_t = std::string;

    public:
        SuperMUCAdapter(obj_t obj) : m_obj(obj) {}

        std::string to_string() const override
        {
            return m_obj;
        }

        /** TODO: abstract machine model implementation **/

        /** abstract machine model end **/

        obj_t native_obj() const { return m_obj; }

    private:
        obj_t m_obj;
    };
}
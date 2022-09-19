#pragma once

#include <adapter.h>

class SuperMUCAdapter : public yloc::Adapter
{
    using obj_t = std::string; // rocm device index

public:
    SuperMUCAdapter(obj_t obj) : m_obj(obj) {}

    std::optional<std::string> as_string() const override
    {
        return m_obj;
    }

    /** TODO: abstract machine model implementation **/

    /** abstract machine model end **/

    obj_t native_obj() const { return m_obj; }

private:
    obj_t m_obj;
};

#pragma once

#include <adapter.h>
#include <hwloc.h>

class HwlocAdapter : public yloc::Adapter
{
    using obj_t = hwloc_obj_t;

public:
    HwlocAdapter(obj_t obj) : m_obj(obj) {}

    /** TODO: abstract machine model implementation **/

    /** abstract machine model end **/
    
    obj_t native_obj() { return m_obj; }

private:
    obj_t m_obj;
};

#include <adapter.h>

class HwlocAdapter : Adapter
{
    using native_obj_t = hwloc_obj_t;

    public:
    inline std::optional<int> capacity() { return std::make_optional(0) };

    native_obj_t native_obj();
    // ...

    private:
    native_obj_t obj;
};

// static_cast<HwlocAdapter::native_obj_t> adapter->native_obj();
# Writing a yloc Module

Writing an own module involves implementing two classes: the new module class that implements the YlocModule interface, and the module's adapter class.
The adapter class specifies the list of available properties, and how these properties are accessed in the module.

---

### Module Interface

```CPP
#include <yloc/modules/module.h>

using yloc::Graph;
using yloc::YlocModule;

class ExampleModule : public YlocModule
{

public:
    void init_graph(Graph &graph) override
    {
        return;
    }

    void export_graph(const Graph &graph, void **output) const override
    {
        output = nullptr; return;
    }

    void update_graph(Graph &graph) override
    {
        return;
    }

private:
};

```

### Module Adapter

```CPP
#include <yloc/modules/adapter.h>

class MyAdapter : public yloc::Adapter
{
    using obj_t = MyTopologyObjectType;

public:
    MyAdapter(obj_t obj) : m_obj(obj) {}

    std::optional<std::string> as_string() const override
    {
        /* string representation of object */
    }

    std::optional<uint64_t> memory() const override
    {
        /* implementation for memory property */
    }

    /* [...] */

    std::optional<uint64_t> my_custom_property() const override
    {
        /* implementation for custom property */
    }

    obj_t native_obj() const { return m_obj; }

private:
    obj_t m_obj;
};
```

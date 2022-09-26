# yloc - A Dynamic Topology Information System

## Introduction

TODO

---

## Documentation

### Requirements, Download, Build and Install

`yloc` uses the CMake build system.
In order to configure the software create a build folder and call `cmake <yloc-root>`  from within.

```
git clone <yloc-repo>/yloc.git
cd yloc
mkdir build ; cd build
cmake ..
```

CMake will check for dependencies and fail with according messages if they are not met.
Otherwise build files are generated that can be executed either with `make` or `cmake --build .`

By default `yloc` only depends on the Boost Graph Library.
There are further dependencies for the different modules, altough they aren't usually mandatory.
For the moment we strongly recommend `hwloc` as primary source of topology information.

There are CMake options to change the default build of yloc.
To list all available options use `cmake -L` from the build folder.
At the moment there are options to enable or disable specific modules: `ENABLE_<MODULE>`.
They can be set using `cmake -D`:

```
cmake -DENABLE_EXAMPLE=OFF ..
```

### API Reference

TODO

---

## yloc Modules

Writing an own module involves implementing two classes: the new module class that implements the YlocModule interface, and the module's adapter class.
The adapter class specifies the list of available properties, and how these properties are accessed in the module.

---

### Module Interface

```CPP
#include <interface.h>

using yloc::graph_t;
using yloc::YlocModule;

class ExampleModule : public YlocModule
{

public:
    void init_graph(graph_t &graph) override
    {
        return;
    }

    void export_graph(const graph_t &graph, void **output) const override
    {
        output = nullptr; return;
    }

    void update_graph(graph_t &graph) override
    {
        return;
    }

private:
};

```

### Module Adapter

```CPP
#include <adapter.h>

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

---

### Supported Topology Systems

TODO:
  - hwloc
  - netloc ?
  - nvlink ?
  - MPI ?
  - ... ?

### Supported Architectures

TODO:
  - tested on ?

---

<!--
## Funding

TODO?

-->

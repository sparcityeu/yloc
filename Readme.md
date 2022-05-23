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

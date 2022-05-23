# Extending yloc

`yloc` is designed in a modular way. This makes it simple to extend it by writing own modules.
As long as basic concepts are followed the graph can be extended and modified, while remaining compatible with existing software.

## Writing an own module

To get started create a copy of the example-module (folder in `<yloc-root>/modules/`).
CMake will automatically pick the new folder up and will try build it the next time.

Modify the `CMakeLists.txt`-file in the new module to include your dependencies and necessary build steps to build your module.
There are also comments in the CMake-file that will guide you through this process. <span style="color:red">**TODO** improve sample module</span>

The main implementation of the module is in `interface_impl.h` but it is also possible to add new source-files that will contain necessary code for the module.
These new files have to be added to the CMake configuration.
You can also compare to other available modules to see how they are implemented.

### Limitation for graph modifications.

TODO

---

## Modification of the outer library

TODO
    - Should be done with caution, remain compatible with other (third-party) modules
    - Best practices for pull requests
---

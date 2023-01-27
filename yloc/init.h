#pragma once

#include <vector>

typedef enum
{
    YLOC_MINIMAL    = 0b00000000,   // bare minimum
    YLOC_LIGHT      = 0b00000001,   // run light microbenchmarks
    YLOC_FULL       = 0b00000011,   // run everything, for usage as a tool instead of a library
    //YLOC_RESERVED = 0b00000100,
    YLOC_POLLED     = 0b00001000,   // initialize modules that provide information over time
    YLOC_ONGOING    = 0b00011000    // automatically updates polled modules
} yloc_init_flags_t;

inline yloc_init_flags_t operator|(yloc_init_flags_t a, yloc_init_flags_t b) {
    return static_cast<yloc_init_flags_t>(static_cast<int>(a) | static_cast<int>(b));
}

namespace yloc
{
    using init_flags_t = yloc_init_flags_t;

    // TODO: set_options

    // TODO: export as yloc_init for C-interface
    int init(init_flags_t _flags);
    // int init();

    int finalize();

    class Module;

    std::vector<Module *> list_modules();
}

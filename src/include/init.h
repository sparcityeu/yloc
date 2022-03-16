#pragma once

typedef enum {
    YLOC_MINIMAL    = 0b00000000,   // bare minimum
    YLOC_LIGHT      = 0b00000001,   // run light microbenchmarks
    YLOC_FULL       = 0b00000011,   // run everythin, for usage as a tool instead of a library
    //YLOC_RESERVED = 0b00000100,
    YLOC_POLLED     = 0b00001000,   // initialize modules that provide information over time
    YLOC_ONGOING    = 0b00011000    // automatically updates polled modules
} yloc_init_flags_t;


int yloc_init(yloc_init_flags_t _flags);

int yloc_finalize();

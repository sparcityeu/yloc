
#include <init.h>
#include <modules/interface.h>

int yloc_init(yloc_init_flags_t _flags) {
    // todo module logic
    // todo flag logic (probably combined with module logic)

    // store result of module init in global variable
}

int yloc_finalize() {
    // free global variables

    // exit threads of ongoing modules

    return;
}


int yloc_update() {
    // todo move to better file (currently fits best to these function)
    // trigger a manual update of the modules
    // Extra logic together with auto-updates from threads necessary

    // This function is probably a manual trigger that calls update_graph on the modules.

    // This function could also be called by an auto-update thread from the outer library,
    // but it is probably better to split the threads, so there is one thread per module.
    // More fine granualar thread functions would allow different timings.
}

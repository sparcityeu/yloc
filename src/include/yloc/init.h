#pragma once

#include <yloc/status.h>

#include <vector>

namespace yloc
{
    yloc_status_t init();
    yloc_status_t finalize();

    class Module;
    std::vector<Module *> list_modules();
} // namespace yloc

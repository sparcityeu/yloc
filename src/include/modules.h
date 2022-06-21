#pragma once

#include <vector>

#include <modules/interface.h>

namespace yloc
{
    std::vector<YlocModule *> list_modules();
}


// COM like module selection?
// Return pointer to object with virtual functions?
//
// No dynamic linkage as found with COM
// Not practical in HPC environment and serious security risk
// Instead use static linked libraries,
// Probably still with virtual functions to ease addtion of new modules

#pragma once

// Adopted from myloq by Jakub Orlowski (jakubo87)
// https://github.com/jakubo87/myLoq

#include <hwloc.h>

#include <interface.h>

using yloc::graph_t;
graph_t init_graph_myloq(const char *t);

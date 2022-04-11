#pragma once

// Adopted from myloq by Jakub Orlowski (jakubo87)
// https://github.com/jakubo87/myLoq

#include <hwloc.h>

#include <interface.h>

graph_t init_graph_myloq(const char* t);

template<typename G, typename V>
using Distance = std::function<double(V,V,const G&)>;

///enumerating combinatorics
std::vector<std::vector<int>>
comb(const int k, const std::vector<int>& vec);

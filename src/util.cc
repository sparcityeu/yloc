#include <yloc/affinity.h>
#include <yloc/util.h>

#include <boost/graph/filtered_graph.hpp>

#include <algorithm>
#include <vector>

namespace yloc
{
    static vertex_descriptor_t lowest_containing_vertex(AffinityMask &mask, vertex_descriptor_t vertex)
    {

        Graph &g = yloc::root_graph();
        auto fg = boost::filtered_graph(
            g, [&](edge_descriptor_t edge) { return g[edge].type == edge_type::CHILD; }, boost::keep_all{});

        std::vector<vertex_descriptor_t> candidates{};

        // loop over child edges and return result of child vertex, if child vertex fully contains the mask
        auto out_it = boost::out_edges(vertex, fg).first;
        auto out_end = boost::out_edges(vertex, fg).second;
        for (; out_it != out_end; ++out_it) {
            edge_descriptor_t e = *out_it;
            vertex_descriptor_t target = boost::target(e, fg);
            auto target_mask = g[target].get<AffinityMask>("cpu_affinity_mask");
            if (target_mask.has_value() && target_mask.value().is_containing(mask) && !g[target].type->is_a<MPIProcess>()) {
                candidates.push_back(target);
            }
        }

        if (candidates.empty()) {
            return vertex;
        }

        std::sort(candidates.begin(), candidates.end(), [&](vertex_descriptor_t a, vertex_descriptor_t b) {
            return g[a].get<AffinityMask>("cpu_affinity_mask").value().count() <
                   g[b].get<AffinityMask>("cpu_affinity_mask").value().count();
        });

        return lowest_containing_vertex(mask, candidates[0]);
    }

    vertex_descriptor_t lowest_containing_vertex(AffinityMask &mask)
    {
        vertex_descriptor_t root_vertex = yloc::root_graph().get_root_vertex();
        return lowest_containing_vertex(mask, root_vertex);
    }
}

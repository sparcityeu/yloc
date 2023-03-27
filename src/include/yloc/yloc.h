#pragma once

/** TODO:
 * What should be visible to the user application?
 * 1. initialization / memory reclamation
 * 2. setting options
 * 3. simplified graph queries
 * 4. underlying boost graph
 * 5. vertex/edge property interface
 * 6. is-a vertex/edge type relationship
 * 7. utility functions
 * 8. ?
 */

#include <yloc/affinity.h>
#include <yloc/component_types.h> // is-a vertex/edge type relationship
#include <yloc/graph.h>           // graph object and underlying boost graph
#include <yloc/init.h>            // initialization / memory reclamation
#include <yloc/query.h>           // simplified graph queries
#include <yloc/util.h>            // utility functions
#include <yloc/status.h>     // required ?

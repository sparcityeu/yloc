/**
 * @file yloc.h
 * @author Sergej Breiter (s.breiter@nm.ifi.lmu.de)
 * @brief Combined header for ease of include in user application.
 * @version 0.1
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

/** TODO:
 * What should be visible to the user application?
 * 1. initialization / memory reclamation
 * 2. setting options
 * 3. simplified graph queries
 * 4. underlying boost graph
 * 5. vertex/edge property interface
 * 6. is-a vertex/edge type relationship
 * 7. ?
 */

#include <yloc/graph/component_types.h> // is-a vertex/edge type relationship
#include <yloc/graph/graph.h>           // graph object and underlying boost graph
#include <yloc/init.h>                  // initialization / memory reclamation
#include <yloc/modules/module.h>        // required ?
// #include <yloc/query/query.h>           // simplified graph queries
#include <yloc/yloc_status.h>           // required ?

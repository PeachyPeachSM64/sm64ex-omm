#ifndef GEO_LAYOUT_H
#define GEO_LAYOUT_H

#include "types.h"
#include "game/memory.h" // struct AllocOnlyPool

struct GraphNode *geo_layout_to_graph_node(struct AllocOnlyPool *pool, const GeoLayout *geoLayout);

#endif

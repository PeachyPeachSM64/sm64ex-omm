#ifndef GRAPH_NODE_O_H
#define GRAPH_NODE_O_H

#include "types.h"
#include "game/object_list_processor.h" // OBJECT_POOL_CAPACITY

#define _O(x) x[OBJECT_POOL_CAPACITY + 1]

struct GraphNodeTranslationRotation {
    struct GraphNode node;
    void *displayList;
    Vec3s translation;
    Vec3s rotation;
    Vec3s_ts _O(_translation);
    Vec3s_ts _O(_rotation);
};

struct GraphNodeTranslation {
    struct GraphNode node;
    void *displayList;
    Vec3s translation;
    Vec3s_ts _O(_translation);
};

struct GraphNodeRotation {
    struct GraphNode node;
    void *displayList;
    Vec3s rotation;
    Vec3s_ts _O(_rotation);
};

struct GraphNodeAnimatedPart {
    struct GraphNode node;
    void *displayList;
    Vec3s translation;
    Vec3s_ts _O(_translation);
};

struct GraphNodeBillboard {
    struct GraphNode node;
    void *displayList;
    Vec3s translation;
    Vec3s_ts _O(_translation);
};

struct GraphNodeScale {
    struct GraphNode node;
    void *displayList;
    f32 scale;
    f32_ts _O(_scale);
};

#endif

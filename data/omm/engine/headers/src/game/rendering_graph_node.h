#ifndef RENDERING_GRAPH_NODE_H
#define RENDERING_GRAPH_NODE_H

#include "graph_node.h"

enum MarioAnimPart {
    MARIO_ANIM_PART_NONE = 0,

    MARIO_ANIM_PART_ROOT,
    MARIO_ANIM_PART_BUTT,
    MARIO_ANIM_PART_TORSO,
    MARIO_ANIM_PART_HEAD,
    MARIO_ANIM_PART_UPPER_LEFT,
    MARIO_ANIM_PART_LEFT_ARM,
    MARIO_ANIM_PART_LEFT_FOREARM,
    MARIO_ANIM_PART_LEFT_HAND,
    MARIO_ANIM_PART_UPPER_RIGHT,
    MARIO_ANIM_PART_RIGHT_ARM,
    MARIO_ANIM_PART_RIGHT_FOREARM,
    MARIO_ANIM_PART_RIGHT_HAND,
    MARIO_ANIM_PART_LOWER_LEFT,
    MARIO_ANIM_PART_LEFT_THIGH,
    MARIO_ANIM_PART_LEFT_LEG,
    MARIO_ANIM_PART_LEFT_FOOT,
    MARIO_ANIM_PART_LOWER_RIGHT,
    MARIO_ANIM_PART_RIGHT_THIGH,
    MARIO_ANIM_PART_RIGHT_LEG,
    MARIO_ANIM_PART_RIGHT_FOOT,

    MARIO_ANIM_PART_MAX,
};

void geo_get_marios_anim_part_pos(struct GraphNode *node, Vec3f dest, s32 animPart);
void geo_get_marios_anim_part_mtx(struct GraphNode *node, Mat4 dest, s32 animPart);
f32  geo_get_marios_anim_part_height(struct GraphNode *node, s32 animPart);
f32  geo_get_marios_anim_part_distance(struct GraphNode *node, s32 animPart1, s32 animPart2);
bool geo_compute_capture_cappy_obj_transform(struct Object *o, s32 animParts, Mat4 transform);
void geo_preprocess_object_graph_node(struct Object *o);
void geo_process_node_and_siblings(struct GraphNode *firstNode);
void geo_process_root(struct GraphNodeRoot *node, Vp *viewport1, Vp *viewport2, s32 clearColor);

#endif

#ifndef OMM_CAPPY_H
#define OMM_CAPPY_H

#include "types.h"

struct Object *omm_cappy_get_object();
struct Object *omm_cappy_get_object_play_as();
struct Object *omm_cappy_spawn(struct MarioState *m);
struct Object *omm_cappy_find_target(f32 *origin, struct Object *cappy, struct MarioState *m, Vec3f direction, f32 distanceMax);
void omm_cappy_update(struct MarioState *m);
void omm_cappy_return_to_mario(struct Object *cappy);
void omm_cappy_try_to_target_next_object(struct Object *cappy, struct MarioState *m);
void omm_cappy_bounce_back(struct Object *cappy);
void omm_cappy_unload();
bool omm_cappy_is_mario_available(struct MarioState *m, bool isCapture);
void omm_cappy_gfx_draw_eyes(struct GraphNode *node, void (*append)(void *, s16));
u32  omm_cappy_gfx_get_graph_node_identifier(struct GraphNode *node);
bool omm_cappy_gfx_has_eyes(struct GraphNode *node);

#endif // OMM_CAPPY_H

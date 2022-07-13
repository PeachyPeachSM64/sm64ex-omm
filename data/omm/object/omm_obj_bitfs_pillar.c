#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Collision
//

#define OMM_BITFS_PILLAR_RADIUS 110
#define OMM_BITFS_PILLAR_HEIGHT 3200

static const Collision omm_bitfs_pillar_collision[] = {
    COL_INIT(),
    COL_VERTEX_INIT(8),
    COL_VERTEX(-OMM_BITFS_PILLAR_RADIUS, 0, -OMM_BITFS_PILLAR_RADIUS),
    COL_VERTEX(-OMM_BITFS_PILLAR_RADIUS, 0, +OMM_BITFS_PILLAR_RADIUS),
    COL_VERTEX(-OMM_BITFS_PILLAR_RADIUS, OMM_BITFS_PILLAR_HEIGHT, +OMM_BITFS_PILLAR_RADIUS),
    COL_VERTEX(+OMM_BITFS_PILLAR_RADIUS, 0, +OMM_BITFS_PILLAR_RADIUS),
    COL_VERTEX(+OMM_BITFS_PILLAR_RADIUS, OMM_BITFS_PILLAR_HEIGHT, +OMM_BITFS_PILLAR_RADIUS),
    COL_VERTEX(+OMM_BITFS_PILLAR_RADIUS, 0, -OMM_BITFS_PILLAR_RADIUS),
    COL_VERTEX(+OMM_BITFS_PILLAR_RADIUS, OMM_BITFS_PILLAR_HEIGHT, -OMM_BITFS_PILLAR_RADIUS),
    COL_VERTEX(-OMM_BITFS_PILLAR_RADIUS, OMM_BITFS_PILLAR_HEIGHT, -OMM_BITFS_PILLAR_RADIUS),
    COL_TRI_INIT(SURFACE_DEFAULT, 12),
    COL_TRI(0, 1, 2),
    COL_TRI(1, 3, 4),
    COL_TRI(1, 4, 2),
    COL_TRI(5, 3, 1),
    COL_TRI(5, 1, 0),
    COL_TRI(6, 4, 3),
    COL_TRI(6, 3, 5),
    COL_TRI(7, 4, 6),
    COL_TRI(7, 2, 4),
    COL_TRI(0, 2, 7),
    COL_TRI(7, 6, 5),
    COL_TRI(7, 5, 0),
    COL_TRI_STOP(),
    COL_END(),
};

//
// Behavior
//

const BehaviorScript omm_bhv_bitfs_pillar[] = {
    OBJ_TYPE_SURFACE,
    0x2A000000, (uintptr_t) omm_bitfs_pillar_collision,
    0x08000000,
    0x0C000000, (uintptr_t) load_object_collision_model,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_bitfs_pillar(struct Object *o, f32 x, f32 y, f32 z) {
    struct Object *pillar = spawn_object(o, MODEL_NONE, omm_bhv_bitfs_pillar);
    pillar->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    obj_set_pos(pillar, x, y, z);
    obj_set_home(pillar, x, y, z);
    obj_set_angle(pillar, 0, 0, 0);
    obj_scale(pillar, 1.f);
    return pillar;
}

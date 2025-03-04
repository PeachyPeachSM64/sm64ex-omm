#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Collision
//

#define OMM_BITFS_PILLAR_RADIUS (110)
#define OMM_BITFS_PILLAR_HEIGHT (3200)

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

const BehaviorScript bhvOmmBitfsPillar[] = {
    OBJ_TYPE_SURFACE,
    BHV_LOAD_COLLISION_DATA(omm_bitfs_pillar_collision),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(load_object_collision_model),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_bitfs_pillar(struct Object *o, f32 x, f32 y, f32 z) {
    struct Object *pillar = spawn_object(o, MODEL_NONE, bhvOmmBitfsPillar);
    pillar->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    obj_set_xyz(pillar, x, y, z);
    obj_set_home(pillar, x, y, z);
    obj_set_angle(pillar, 0, 0, 0);
    obj_scale(pillar, 1.f);
    return pillar;
}

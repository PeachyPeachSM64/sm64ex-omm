#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Behavior
//

static void omm_bhv_damage_mario_update() {
    struct Object *o = gCurrentObject;
    if (o->oAction++ == 0) {
        obj_set_pos(o, gMarioState->pos[0], gMarioState->pos[1], gMarioState->pos[2]);
        obj_set_home(o, gMarioState->pos[0], gMarioState->pos[1], gMarioState->pos[2]);
    } else {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript omm_bhv_damage_mario[] = {
    OBJ_TYPE_GENACTOR,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_damage_mario_update,
    0x09000000
};

//
// Spawner
//

struct Object *omm_spawn_damage_mario(struct Object *o, s32 interactType, s32 damage) {
    struct Object *dmg = spawn_object(o, MODEL_NONE, omm_bhv_damage_mario);
    obj_set_pos(o, gMarioState->pos[0], gMarioState->pos[1], gMarioState->pos[2]);
    obj_set_home(o, gMarioState->pos[0], gMarioState->pos[1], gMarioState->pos[2]);
    obj_set_params(dmg, interactType, damage, 0, 0, true);
    obj_reset_hitbox(dmg, 300, 600, 300, 600, 0, 300);
    o->oAction = 0;
    return dmg;
}

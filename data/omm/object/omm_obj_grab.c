#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static const u32 sOmmGrabActions[][2] = {
    { ACT_PUNCHING, 5 },
    { ACT_MOVE_PUNCHING, 5 },
    { ACT_DIVE, 0xFF },
    { ACT_DIVE_SLIDE, 0xFF },
    { ACT_OMM_PEACH_ATTACK_GROUND, 3 }
};

OMM_INLINE bool is_grab_action(struct MarioState *m) {
    for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmGrabActions); ++i) {
        if (m->action == sOmmGrabActions[i][0] && m->actionArg <= sOmmGrabActions[i][1]) {
            return true;
        }
    }
    return false;
}

//
// Behavior
//

static void omm_bhv_grab_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    if (OMM_MOVESET_CLASSIC || !m->marioObj || m->heldObj || !is_grab_action(m)) {
        obj_mark_for_deletion(o);
        return;
    }

    // Update grab hitbox
    obj_set_scale(o, m->marioObj->oScaleX, m->marioObj->oScaleY, m->marioObj->oScaleZ);
    obj_set_pos(o, m->pos[0] + 30.f * o->oScaleX * sins(m->faceAngle[1]), m->pos[1], m->pos[2] + 30.f * o->oScaleX * coss(m->faceAngle[1]));
    obj_set_home(o, o->oPosX, o->oPosY, o->oPosZ);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 60, 80, 0, 0, 0, 0);

    // Interactions
    for_each_object_in_interaction_lists(obj) {
        if (obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX)) {
            if (omm_mario_check_grab(m, obj, false)) {
                obj_mark_for_deletion(o);
                return;
            }
        }
    }
}

const BehaviorScript omm_bhv_grab[] = {
    OBJ_TYPE_DEFAULT, // Needs to be updated after other objects
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_grab_update,
    0x09000000
};

//
// Spawner (auto)
//

OMM_ROUTINE_UPDATE(omm_spawn_grab) {
    struct MarioState *m = gMarioState;
    struct Object *o = obj_get_first_with_behavior(omm_bhv_grab);
    if (!o && OMM_MOVESET_ODYSSEY && m->marioObj && !m->heldObj && is_grab_action(m)) {
        spawn_object(m->marioObj, MODEL_NONE, omm_bhv_grab);
    }
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

static void bhv_omm_dorrie_update() {
    struct Object *o = gCurrentObject;
    struct MarioState *m = gMarioState;
    obj_approach_orbit(o, o->oTimer + o->oBehParams, 7000.f, 1800, -2500.f, 1.2f, 0x100, 0x100);
    if (m->marioObj->platform == o) {
        o->oSubAction++;
        if (o->oSubAction == 30) {
            omm_secrets_unlock(OMM_SECRET_FLYING_DORRIE);
        }
    } else {
        o->oSubAction = 0;
    }
}

const BehaviorScript bhvOmmDorrie[] = {
    OBJ_TYPE_SURFACE,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_LOAD_COLLISION_DATA(dorrie_seg6_collision_0600F644),
    BHV_LOAD_ANIMATIONS(oAnimations, dorrie_seg6_anims_0600F638),
    BHV_ANIMATE(1),
    BHV_SCALE(0, 50),
    BHV_SET_HOME(),
    BHV_SET_RANDOM_INT(oBehParams, 0, 1800),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_dorrie_update),
        BHV_CALL_NATIVE(load_object_collision_model),
    BHV_END_LOOP(),
};

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

extern void bhv_dorrie_update_pos(struct Object *obj, f32 radius);

//
// Init
//

bool omm_cappy_dorrie_init(struct Object *o) {
    o->collisionData = (void *) dorrie_seg6_collision_0600F644;
    o->oVelY = 0.f;
    o->oDorrieOffsetY = 0.f;
    o->oDorrieNeckAngle = -0x26F4;
    return true;
}

void omm_cappy_dorrie_end(struct Object *o) {
    o->collisionData = (void *) dorrie_seg6_collision_0600F644;
    pobj_reset_home();
    o->oAction = DORRIE_ACT_MOVE;
    o->oForwardVel = 0.f;
    o->oDorrieOffsetY = 0.f;
    o->oDorrieVelY = 0.f;
    o->oDorrieYawVel = 0;
    o->oDorrieNeckAngle = -0x26F4;
    o->oDorrieHeadRaiseSpeed = 0;
    o->oDorrieLiftingMario = FALSE;
    o->oDorrieGroundPounded = FALSE;
}

u64 omm_cappy_dorrie_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_DORRIE;
}

f32 omm_cappy_dorrie_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_dorrie_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_FLOATING;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_QUICKSAND;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
    }

    // Movement
    perform_object_step(o, OBJ_STEP_UPDATE_PREV_POS);
    pobj_reset_home();
    bhv_dorrie_update_pos(o, o->hitboxRadius);
    pobj_decelerate(o);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    obj_anim_play(o, 1, 1.f);

    // OK
    pobj_return_ok;
}

void omm_cappy_dorrie_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

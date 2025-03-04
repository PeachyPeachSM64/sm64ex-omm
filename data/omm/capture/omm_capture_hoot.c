#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_hoot_init(struct Object *o) {
    if (o->oHootAvailability != HOOT_AVAIL_READY_TO_FLY) {
        return false;
    }

    return true;
}

void omm_cappy_hoot_end(struct Object *o) {
    pobj_reset_home();
    obj_anim_play(o, 0, 1.f);
}

u64 omm_cappy_hoot_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_HOOT;
}

f32 omm_cappy_hoot_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_hoot_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_FLOATING;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 6) == POBJ_RESULT_JUMP_START) {
            o->oCurrAnim = NULL;
            obj_play_sound(o, POBJ_SOUND_WING_FLAP_1);
        }
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    obj_anim_play(o, 0, (o->oVelY > 0.f) ? 2.f : 1.f);

    // OK
    pobj_return_ok;
}

void omm_cappy_hoot_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}
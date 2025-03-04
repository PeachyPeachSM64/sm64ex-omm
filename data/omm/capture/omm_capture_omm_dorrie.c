#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_omm_dorrie_init(UNUSED struct Object *o) {
    return true;
}

void omm_cappy_omm_dorrie_end(struct Object *o) {
    pobj_reset_home();
    o->oTimer = (random_u16() % 1800);
}

u64 omm_cappy_omm_dorrie_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_DORRIE;
}

f32 omm_cappy_omm_dorrie_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_omm_dorrie_update(struct Object *o) {
    omm_secrets_unlock(OMM_SECRET_FLYING_DORRIE);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_QUICKSAND;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;

    // Inputs
    omm_capture_set_camera_behind_mario();
    if (pobj_process_inputs(o)) {
        pobj_move_3d(o, false, false, false, 0x3F00, 0x180, 0x180);
    }

    // Movement
    perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_UPDATE_PREV_POS);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // OK
    pobj_return_ok;
}

void omm_cappy_omm_dorrie_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_friendly_blargg_init(UNUSED struct Object *o) {
    return true;
}

void omm_cappy_friendly_blargg_end(struct Object *o) {
    audio_stop_shell_music();
    o->oAction = 0;
    o->oInteractStatus = 0;
}

u64 omm_cappy_friendly_blargg_get_type(UNUSED struct Object *o) {
    return 0;
}

f32 omm_cappy_friendly_blargg_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_friendly_blargg_update(struct Object *o) {

    // Init
    if (!gOmmObject->state.actionFlag) {
        audio_play_shell_music();
        gOmmObject->state.actionFlag = true;
    }

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;
    POBJ_SET_ATTACKING_BREAKABLE;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, POBJ_B_BUTTON_DOWN, false);
    }

    // Movement
    Vec3f previousPos;
    obj_pos_as_vec3f(o, previousPos);
    perform_object_step(o, POBJ_STEP_FLAGS);
    if (!obj_is_on_ground(o) || o->oFloor->type != SURFACE_BURNING) {
        vec3f_copy(&o->oPosX, previousPos);
        obj_drop_to_floor(o);
    }
    pobj_decelerate(o);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    obj_make_step_sound_and_particle(o, NULL, 0, 0, POBJ_SOUND_RIDING_SHELL_LAVA, OBJ_PARTICLE_FLAME);

    // OK
    pobj_return_ok;
}

void omm_cappy_friendly_blargg_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

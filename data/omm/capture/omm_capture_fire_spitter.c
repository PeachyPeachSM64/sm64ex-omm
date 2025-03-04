#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_fire_spitter_init(struct Object *o) {
    obj_scale(o, 0.25f);
    o->oGraphYOffset = 40.f;
    return true;
}

void omm_cappy_fire_spitter_end(struct Object *o) {
    obj_drop_to_floor(o);
    obj_scale(o, 0.2f);
    o->oGraphYOffset = 40.f;
    o->oAction = FIRE_SPITTER_ACT_IDLE;
    o->oTimer = 0;
}

u64 omm_cappy_fire_spitter_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_FIRE_SPITTER;
}

f32 omm_cappy_fire_spitter_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_fire_spitter_update(struct Object *o) {

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
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Fireball
        if (POBJ_B_BUTTON_PRESSED) {
            omm_obj_spawn_fire_spitter_flame(o, false);
            gOmmObject->state.actionTimer = 0;
        }

        // Flamethrower
        if (POBJ_B_BUTTON_DOWN) {
            if (gOmmObject->state.actionTimer++ >= 15) {
                omm_obj_spawn_fire_spitter_flame(o, true);
            }
        } else {
            gOmmObject->state.actionTimer = 0;
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

    // OK
    pobj_return_ok;
}

void omm_cappy_fire_spitter_update_gfx(struct Object *o) {

    // Gfx
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.tra_y = 250.f;
    gOmmObject->cappy.scale = 3.f;
}

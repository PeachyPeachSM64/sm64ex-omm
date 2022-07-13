#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_crazy_box_init(UNUSED struct Object *o) {
    gOmmData->object->state.actionState = 0;
    return true;
}

void cappy_crazy_box_end(UNUSED struct Object *o) {
}

f32 cappy_crazy_box_get_top(struct Object *o) {
    return 200.f * o->oScaleY;
}

//
// Update
//

s32 cappy_crazy_box_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        if (obj_is_on_ground(o) && (gOmmData->object->state.actionState == 3)) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 0);
            obj_destroy(o);
            POBJ_RETURN_UNPOSSESS;
        }
        pobj_move(o, false, false, false);
        switch (pobj_jump(o, 1.6f, 1)) {
            case POBJ_RESULT_HOP_SMALL: {
                obj_play_sound(o, SOUND_GENERAL_BOING1);
            } break;
            case POBJ_RESULT_HOP_LARGE: {
                if (POBJ_B_BUTTON_DOWN) {
                    o->oVelY = omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER * (1.50f + 0.25f * (gOmmData->object->state.actionState++)); // 1.50, 1.75, 2.00
                    obj_spawn_white_puff(o, SOUND_GENERAL_BOING2);
                } else {
                    obj_play_sound(o, SOUND_GENERAL_BOING1);
                }
            } break;
        }
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);

    // Cappy values
    gOmmData->object->cappy.offset[1] = 200.f;
    gOmmData->object->cappy.scale     = 2.f;

    // OK
    POBJ_RETURN_OK;
}

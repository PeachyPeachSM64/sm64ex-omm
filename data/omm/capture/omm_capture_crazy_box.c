#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_crazy_box_init(UNUSED struct Object *o) {
    gOmmMario->capture.lockTimer = 1;
    return true;
}

void omm_cappy_crazy_box_end(UNUSED struct Object *o) {
}

u64 omm_cappy_crazy_box_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_CRAZY_BOX;
}

f32 omm_cappy_crazy_box_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_crazy_box_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        if (obj_is_on_ground(o) && (gOmmObject->state.actionState == 3)) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 0);
            obj_destroy(o);
            pobj_return_unpossess;
        }
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 0) == POBJ_RESULT_JUMP_START) {
            if (POBJ_B_BUTTON_DOWN) {
                o->oVelY = pobj_get_jump_velocity(o) * (1.50f + 0.25f * (gOmmObject->state.actionState++)); // 1.50, 1.75, 2.00
                obj_spawn_white_puff(o, POBJ_SOUND_JUMP_BOING_2);
            } else {
                obj_play_sound(o, POBJ_SOUND_JUMP_BOING_1);
            }
        }
    }
    pobj_stop_if_unpossessed();

    // Movement
    if (pobj_hop(o, 1.6f)) {
        obj_play_sound(o, POBJ_SOUND_JUMP_BOING_1);
    }
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);

    // Lava boost
    if (o->oVelY > 0 && gOmmObject->state.actionTimer == 1) {
        spawn_object(o, MODEL_BURN_SMOKE, bhvBlackSmokeMario);
        obj_play_sound(o, POBJ_SOUND_LAVA_BURN);
    } else if (o->oFloor) {
        if (o->oFloor->type == SURFACE_BURNING && !OMM_CHEAT_WALK_ON_LAVA) {
            if (gOmmObject->state.actionTimer == 0 && o->oDistToFloor <= 10.f && POBJ_A_BUTTON_DOWN) {
                o->oVelY = max_f(o->oVelY, pobj_get_jump_velocity(o));
                o->oPosY = max_f(o->oPosY, o->oFloorHeight + 10.f);
                for (s32 i = 0; i != 8; ++i) spawn_object(o, MODEL_RED_FLAME, bhvKoopaShellFlame);
                obj_play_sound(o, POBJ_SOUND_RIDING_SHELL_LAVA);
                obj_play_sound(o, POBJ_SOUND_JUMP_BOING_1);
                gOmmObject->state.actionTimer = 1;
            }
        } else {
            gOmmObject->state.actionTimer = 0;
        }
    }

    // Special floors
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // OK
    pobj_return_ok;
}

void omm_cappy_crazy_box_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.tra_y = 200.f;
    gOmmObject->cappy.scale = 2.f;
}

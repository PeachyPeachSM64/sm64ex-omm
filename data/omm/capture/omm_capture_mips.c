#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_mips_init(struct Object *o) {
    if (o->oMipsStarStatus == MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR) {
        gOmmObject->state.actionState = 3;
    } else {
        gOmmObject->state.actionTimer = 15;
    }
    return true;
}

void omm_cappy_mips_end(struct Object *o) {
    obj_anim_play(o, 2, 1.f);
    o->oAction = MIPS_ACT_FALL_DOWN;
    if (gOmmObject->state.actionState >= 3) {
        o->oMipsStarStatus = MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR;
    } else {
        o->oMipsStarStatus = MIPS_STAR_STATUS_SHOULD_SPAWN_STAR;
    }
}

u64 omm_cappy_mips_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_MIPS;
}

f32 omm_cappy_mips_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_mips_update(struct Object *o) {

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
    POBJ_SET_ABLE_TO_OPEN_DOORS;
    POBJ_SET_TALKING * (gOmmObject->state.actionState >= 0 && gOmmObject->state.actionState <= 2);

    // States
    if (gOmmObject->state.actionTimer == 0) {

        // Start dialog
        if (gOmmObject->state.actionState == 0) {
            if (omm_mario_lock(gMarioState, -1) && obj_dialog_start(o->oBehParams2ndByte ? DIALOG_162 : DIALOG_084)) {
                gOmmObject->state.actionState = 1;
            }
        }

        // Wait for the dialog to end
        else if (gOmmObject->state.actionState == 1) {
            if (obj_dialog_update()) {
                gOmmObject->state.actionState = 2;
                gOmmObject->state.actionTimer = 5;
            }
        }

        // Spawn star
        else if (gOmmObject->state.actionState == 2) {
            if (omm_mario_unlock(gMarioState)) {
                bhv_spawn_star_no_level_exit(3 + o->oBehParams2ndByte);
                gOmmObject->state.actionState = 3;
            }
        }

        // Inputs
        else if (pobj_process_inputs(o)) {
            pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
            if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
                obj_play_sound(o, POBJ_SOUND_JUMP_MIPS);
            }
        }

    } else {
        gOmmObject->state.actionTimer = max_s(0, gOmmObject->state.actionTimer - 1);
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
    if (obj_is_on_ground(o)) {
        if (POBJ_IS_WALKING) {
            obj_anim_play(o, 1, max_f(1.f, POBJ_ABS_FORWARD_VEL / pobj_get_walk_speed(o)));
            if (obj_anim_is_near_end(o)) {
                f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);
                if (!obj_is_underwater(o, waterLevel) && o->oPosY < waterLevel) {
                    obj_play_sound(o, POBJ_SOUND_WALK_MIPS_WATER);
                    spawn_object(o, MODEL_NONE, bhvShallowWaterSplash);
                } else {
                    obj_play_sound(o, POBJ_SOUND_WALK_MIPS);
                }
            }
        } else {
            obj_anim_play(o, 0, 1.f);
        }
    } else if (o->oVelY > 0.f) {
        obj_anim_play(o, 1, 1.f);
        obj_anim_clamp_frame(o, 0, o->oCurrAnim->mLoopEnd * 0.3f);
    } else {
        obj_anim_play(o, 1, 1.f);
        obj_anim_clamp_frame(o, o->oCurrAnim->mLoopEnd * 0.3f, o->oCurrAnim->mLoopEnd * 0.6f);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_mips_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

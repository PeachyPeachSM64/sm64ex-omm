#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_HEAVE_HO_GRAB_POS     0, 0, 80
#define OMM_CAPPY_HEAVE_HO_THROW_FVEL   (-12)
#define OMM_CAPPY_HEAVE_HO_THROW_YVEL   (64)

//
// Init
//

bool omm_cappy_heave_ho_init(struct Object *o) {
    if (o->oAction == 0 ||
        o->oAction == 3) {
        return false;
    }

    return true;
}

void omm_cappy_heave_ho_end(struct Object *o) {
    pobj_throw_held_object(o, OMM_CAPPY_HEAVE_HO_THROW_FVEL, OMM_CAPPY_HEAVE_HO_THROW_YVEL);
    o->oHomeY = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
    o->oTimer = 0;
}

u64 omm_cappy_heave_ho_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_HEAVE_HO;
}

f32 omm_cappy_heave_ho_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_heave_ho_update(struct Object *o) {

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

    // Inputs
    if (pobj_process_inputs(o)) {
        obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Hold
        if (POBJ_B_BUTTON_PRESSED && pobj_try_to_hold_object(o, 250, 100, -50, 0x3000)) {
            gOmmObject->state.actionState = 1;
            obj_play_sound(o, POBJ_SOUND_HEAVE_HO_GRAB);
            omm_mario_lock(gMarioState, -1);
        }

        // Throw
        else if (gOmmObject->state.heldObj) {

            // Hold B to throw higher and further
            pobj_charge_attack(15, o, 200, 60);
            pobj_release_attack(15,
                pobj_throw_held_object(o,
                    OMM_CAPPY_HEAVE_HO_THROW_FVEL * lerp_f(_power_, 1.f, 1.5f),
                    OMM_CAPPY_HEAVE_HO_THROW_YVEL * lerp_f(_power_, 1.f, 1.5f)
                );
                obj_play_sound(o, POBJ_SOUND_HEAVE_HO_GRAB);
                obj_play_sound(o, POBJ_SOUND_JUMP_BOING_1);
                gOmmObject->state.actionState = 2;
                omm_mario_lock(gMarioState, -1);
            );
        }
    }

    // Hold/Throw
    else if (gOmmObject->state.actionState != 0) {
        if (obj_anim_is_at_end(o)) {
            omm_mario_unlock(gMarioState);
            gOmmObject->state.actionState = 0;
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

void omm_cappy_heave_ho_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    switch (gOmmObject->state.actionState) {

        // Idle, walk, jump
        case 0: {
            obj_anim_play(o, 0, POBJ_ABS_FORWARD_VEL / 20.f);
            pobj_update_held_object(o, OMM_CAPPY_HEAVE_HO_GRAB_POS, 0, 1);
            if (POBJ_IS_WALKING) {
                obj_play_sound(o, POBJ_SOUND_WALK_HEAVE_HO);
            }
        } break;

        // Hold
        case 1: {
            pobj_freeze_gfx_during_star_dance();
            obj_anim_play(o, 1, 1.f);
            pobj_update_held_object(o, OMM_CAPPY_HEAVE_HO_GRAB_POS, 3.f * o->hitboxHeight, (f32) o->oAnimFrame / (f32) o->oCurrAnim->mLoopEnd);
        } break;

        // Throw
        case 2: {
            pobj_freeze_gfx_during_star_dance();
            obj_anim_play(o, 1, 1.f);
        } break;
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}

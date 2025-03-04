#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_CHUCKYA_HANDS_POS_1   0, 120, 40
#define OMM_CAPPY_CHUCKYA_HANDS_POS_2   0, 130, -20
#define OMM_CAPPY_CHUCKYA_THROW_FVEL    (40)
#define OMM_CAPPY_CHUCKYA_THROW_YVEL    (40)

//
// Init
//

bool omm_cappy_chuckya_init(struct Object *o) {
    if (o->oAction == 1) {
        return false;
    }

    return true;
}

void omm_cappy_chuckya_end(struct Object *o) {
    pobj_throw_held_object(o, OMM_CAPPY_CHUCKYA_THROW_FVEL, OMM_CAPPY_CHUCKYA_THROW_YVEL);
    obj_anim_play(o, o->oAction == 2 ? 2 : 4, 1.f);
    o->oChuckyaUnk88 = 0;
    o->oGravity = -4.f;
}

u64 omm_cappy_chuckya_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_CHUCKYA;
}

f32 omm_cappy_chuckya_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_chuckya_update(struct Object *o) {

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
            obj_play_sound(o, POBJ_SOUND_CHUCKYA_GRAB);
            omm_mario_lock(gMarioState, -1);
        }

        // Throw
        else if (gOmmObject->state.heldObj) {

            // Hold B to throw higher and further
            pobj_charge_attack(15, o, 300, 140);
            pobj_release_attack(15,
                o->oObjectPower = _power_;
                gOmmObject->state.actionState = 2;
                omm_mario_lock(gMarioState, -1);
            );
        }
    }

    // Hold
    else if (gOmmObject->state.actionState == 1) {
        if (obj_anim_is_at_end(o)) {
            sound_stop(POBJ_SOUND_CHUCKYA_GRAB, o->oCameraToObject);
            omm_mario_unlock(gMarioState);
            gOmmObject->state.actionState = 0;
        }
    }

    // Throw
    else if (gOmmObject->state.actionState == 2) {
        if (gOmmObject->state.heldObj && o->oAnimFrame >= 16) {
            pobj_throw_held_object(o,
                OMM_CAPPY_CHUCKYA_THROW_FVEL * lerp_f(o->oObjectPower, 1.f, 1.5f),
                OMM_CAPPY_CHUCKYA_THROW_YVEL * lerp_f(o->oObjectPower, 1.f, 1.5f)
            );
            obj_play_sound(o, POBJ_SOUND_THROW_2);
        } else if (o->oAnimFrame >= 18) {
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

void omm_cappy_chuckya_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    switch (gOmmObject->state.actionState) {

        // Idle, walk, jump
        case 0: {
            obj_anim_play(o, gOmmObject->state.heldObj ? 1 : 4, 1.f);
            pobj_update_held_object(o, OMM_CAPPY_CHUCKYA_HANDS_POS_1, 0, 1);
            if (POBJ_IS_WALKING) {
                obj_play_sound(o, POBJ_SOUND_WALK_HEAVE_HO);
            }
        } break;

        // Hold
        case 1: {
            pobj_freeze_gfx_during_star_dance();
            obj_anim_play(o, 0, 1.f);
            f32 dt = clamp_0_1_f((f32) o->oAnimFrame * 2.5f / (f32) o->oCurrAnim->mLoopEnd);
            pobj_update_held_object(o, OMM_CAPPY_CHUCKYA_HANDS_POS_1, 0.5f * o->hitboxHeight, dt);
        } break;

        // Throw
        case 2: {
            pobj_freeze_gfx_during_star_dance();
            obj_anim_play(o, 3, 1.5f);
            Vec3f pos1 = { OMM_CAPPY_CHUCKYA_HANDS_POS_1 };
            Vec3f pos2 = { OMM_CAPPY_CHUCKYA_HANDS_POS_2 };
            f32 t = sins(relerp_0_1_f(o->oAnimFrame, 0, 16, 0, 0x8000));
            f32 x = lerp_f(t, pos1[0], pos2[0]);
            f32 y = lerp_f(t, pos1[1], pos2[1]);
            f32 z = lerp_f(t, pos1[2], pos2[2]);
            pobj_update_held_object(o, x, y, z, 0, 1);
        } break;
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}

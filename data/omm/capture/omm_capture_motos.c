#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define MOTOS_CONSTANTS_ONLY
#include "behaviors/motos.inc.c"
#undef MOTOS_CONSTANTS_ONLY

#define OMM_CAPPY_MOTOS_HAND_POS_1  40, 90, 30
#define OMM_CAPPY_MOTOS_HAND_POS_2  40, 60, -50
#define OMM_CAPPY_MOTOS_THROW_FVEL  (50)
#define OMM_CAPPY_MOTOS_THROW_YVEL  (24)

//
// Init
//

bool omm_cappy_motos_init(struct Object *o) {
    if (o->oAction == MOTOS_ACT_PLAYER_CARRY ||
        o->oAction == MOTOS_ACT_DEATH) {
        return false;
    }

    return true;
}

void omm_cappy_motos_end(struct Object *o) {
    pobj_throw_held_object(o, OMM_CAPPY_MOTOS_THROW_FVEL, OMM_CAPPY_MOTOS_THROW_YVEL);
    obj_anim_play(o, MOTOS_ANIM_WAIT, 1.f);
    obj_set_vel(o, 0.f, 0.f, 0.f);
    o->oAction = MOTOS_ACT_WAIT;
}

u64 omm_cappy_motos_get_type(UNUSED struct Object *o) {
    return 0;
}

f32 omm_cappy_motos_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_motos_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_INVULNERABLE;
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
            obj_play_sound(o, POBJ_SOUND_MOTOS_GRAB);
            omm_mario_lock(gMarioState, -1);
        }

        // Throw
        else if (gOmmObject->state.heldObj) {

            // Hold B to throw higher and further
            pobj_charge_attack(15, o, 250, 120);
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
            sound_stop(POBJ_SOUND_MOTOS_GRAB, o->oCameraToObject);
            omm_mario_unlock(gMarioState);
            gOmmObject->state.actionState = 0;
        }
    }

    // Throw
    else if (gOmmObject->state.actionState == 2) {
        if (gOmmObject->state.heldObj && o->oAnimFrame >= 14) {
            pobj_throw_held_object(o,
                OMM_CAPPY_MOTOS_THROW_FVEL * lerp_f(o->oObjectPower, 1.f, 1.5f),
                OMM_CAPPY_MOTOS_THROW_YVEL * lerp_f(o->oObjectPower, 1.f, 1.5f)
            );
            obj_play_sound(o, POBJ_SOUND_THROW_2);
        } else if (obj_anim_is_at_end(o)) {
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

void omm_cappy_motos_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    switch (gOmmObject->state.actionState) {

        // Idle, walk, jump
        case 0: {
            obj_anim_play(o, gOmmObject->state.heldObj ? MOTOS_ANIM_CARRY_RUN : MOTOS_ANIM_WALK, 1.f);
            pobj_update_held_object(o, OMM_CAPPY_MOTOS_HAND_POS_1, 0, 1);
            if (obj_is_on_ground(o)) {
                obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 16.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALK_BULLY_2, OBJ_PARTICLE_NONE);
            }
        } break;

        // Hold
        case 1: {
            pobj_freeze_gfx_during_star_dance();
            obj_anim_play(o, MOTOS_ANIM_CARRY_START, 1.5f);
            pobj_update_held_object(o, OMM_CAPPY_MOTOS_HAND_POS_1, 0.5f * o->hitboxHeight, (f32) o->oAnimFrame / (f32) o->oCurrAnim->mLoopEnd);
        } break;

        // Throw
        case 2: {
            pobj_freeze_gfx_during_star_dance();
            obj_anim_play(o, MOTOS_ANIM_PITCH, 1.5f);
            Vec3f pos1 = { OMM_CAPPY_MOTOS_HAND_POS_1 };
            Vec3f pos2 = { OMM_CAPPY_MOTOS_HAND_POS_2 };
            f32 t = sins(relerp_0_1_f(o->oAnimFrame, 0, 14, 0, 0x8000));
            f32 x = lerp_f(t, pos1[0], pos2[0]);
            f32 y = lerp_f(t, pos1[1], pos2[1]);
            f32 z = lerp_f(t, pos1[2], pos2[2]);
            pobj_update_held_object(o, x, y, z, 0, 1);
        } break;
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}

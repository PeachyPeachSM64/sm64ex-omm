#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_SPINDRIFT_SPIN_ATTACK_COOLDOWN    (20)

//
// Init
//

bool omm_cappy_spindrift_init(struct Object* o) {
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    gOmmObject->state.actionTimer = OMM_CAPPY_SPINDRIFT_SPIN_ATTACK_COOLDOWN;
    return true;
}

void omm_cappy_spindrift_end(struct Object *o) {
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    o->oGravity = -4.f;
}

u64 omm_cappy_spindrift_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_SPINDRIFT;
}

f32 omm_cappy_spindrift_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_spindrift_update(struct Object *o) {

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

        // Propeller states
        if (obj_is_on_ground(o)) {
            gOmmObject->state.actionState = 0;
        } else if (gOmmObject->state.actionState == 1 && o->oVelY < 0.f) {
            gOmmObject->state.actionState = 2;
        }

        // Move and jump
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Propeller jump
        else if (POBJ_A_BUTTON_PRESSED && !obj_is_on_ground(o) && gOmmObject->state.actionState == 0) {
            o->oVelY = pobj_get_jump_velocity(o) * 2.5f;
            f32 gravity = -pobj_get_gravity(o);
            f32 duration = o->oVelY / gravity;
            o->oGravity = 0x20000 / (gravity * (duration * (duration - 1)) / 2);
            omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_1, o->oCameraToObject);
            gOmmObject->state.actionState = 1;
        }

        // Float
        if (POBJ_A_BUTTON_DOWN && !obj_is_on_ground(o) && o->oVelY < 0.f) {
            if (!gOmmObject->state.actionFlag) {
                omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_2, o->oCameraToObject);
            }
            gOmmObject->state.actionFlag = true;
        } else {
            gOmmObject->state.actionFlag = false;
        }

        // Spin attack
        if (POBJ_B_BUTTON_PRESSED && gOmmObject->state.actionTimer >= OMM_CAPPY_SPINDRIFT_SPIN_ATTACK_COOLDOWN) {
            omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_3, o->oCameraToObject);
            o->oVelY = pobj_get_jump_velocity(o) / 2.f;
            gOmmObject->state.actionTimer = 0;
            gOmmObject->state.actionFlag = false;
            omm_mario_lock(gMarioState, 8);
            omm_obj_spawn_spindrift_shockwave(o);
        }
    }
    pobj_stop_if_unpossessed();

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, gOmmObject->state.actionFlag ? 0.25f : 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();
    if (POBJ_IS_BOUNCING || POBJ_IS_STAR_DANCING) {
        gOmmObject->state.actionState = 0;
    }

    // Spin attack
    if (++gOmmObject->state.actionTimer < 8) {
        o->oAngleVelYaw += 0x2000;
    }

    // Propeller jump
    else if (gOmmObject->state.actionState == 1) {
        o->oAngleVelYaw += max_f(0.f, o->oVelY * o->oGravity);
        spawn_object(o, MODEL_NONE, bhvMistParticleSpawner);
    }

    // Float
    else if (gOmmObject->state.actionFlag) {
        o->oAngleVelYaw += 0x1000;
    }

    // Default
    else {
        o->oAngleVelYaw = o->oFaceAngleYaw;
    }

    // Animation, sound and particles
    obj_anim_play(o, 0, 1.f);

    // OK
    pobj_return_ok;
}

void omm_cappy_spindrift_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    o->oGfxAngle[1] = o->oAngleVelYaw;

    // Cappy transform
    gOmmObject->cappy.object = o;
}

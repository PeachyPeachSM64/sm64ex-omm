#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_BEE_SPIN_ATTACK_COOLDOWN  (20)

//
// Init
//

bool omm_cappy_bee_init(struct Object* o) {
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    gOmmObject->state.actionTimer = OMM_CAPPY_BEE_SPIN_ATTACK_COOLDOWN;
    return true;
}

void omm_cappy_bee_end(struct Object *o) {
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
}

u64 omm_cappy_bee_get_type(UNUSED struct Object *o) {
    return 0;
}

f32 omm_cappy_bee_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_bee_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
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
        if (POBJ_B_BUTTON_PRESSED && gOmmObject->state.actionTimer >= OMM_CAPPY_BEE_SPIN_ATTACK_COOLDOWN) {
            omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_3, o->oCameraToObject);
            o->oVelY = pobj_get_jump_velocity(o) / 2.f;
            o->oAngleVelYaw = o->oFaceAngleYaw;
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

    // Animation, sound and particles
    gOmmObject->state.actionTimer++;
    obj_anim_play(o, 0, (gOmmObject->state.actionState == 1 || gOmmObject->state.actionFlag) ? 2.f : 1.f);
    if (gOmmObject->state.actionState == 1) {
        spawn_object(o, MODEL_NONE, bhvMistParticleSpawner);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_bee_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
#if OMM_GAME_IS_SMSR
    o->oAnimState++;
#else
    if (gOmmObject->state.actionTimer < 8) {
        o->oGfxAngle[1] = o->oAngleVelYaw + 0x2000 * gOmmObject->state.actionTimer;
    }
#endif

    // Cappy transform
#if OMM_GAME_IS_SMSR
    s16 angle = o->oFaceAngleYaw - gCamera->yaw + 0x4000;
    gOmmObject->cappy.tra_x = 15.f * sins(angle);
    gOmmObject->cappy.tra_y = 35.f;
    gOmmObject->cappy.tra_z = 15.f * coss(angle);
    gOmmObject->cappy.scale = 0.4f;
#else
    gOmmObject->cappy.object = o;
#endif
}

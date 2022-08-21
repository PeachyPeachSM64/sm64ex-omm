#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_spindrift_init(UNUSED struct Object* o) {
    gOmmObject->state.actionState = 0;
    gOmmObject->state.actionTimer = 0;
    return true;
}

void omm_cappy_spindrift_end(UNUSED struct Object *o) {
    o->oGravity = -4.f;
}

f32 omm_cappy_spindrift_get_top(struct Object *o) {
    return 160.f * o->oScaleY;
}

//
// Update
//

s32 omm_cappy_spindrift_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;

    // Inputs
    if (!obj_update_door(o) && !omm_mario_is_locked(gMarioState)) {

        // Propeller states
        if (obj_is_on_ground(o)) {
            gOmmObject->state.actionState = 0;
        } else if (gOmmObject->state.actionState == 1 && o->oVelY < 0.f) {
            gOmmObject->state.actionState = 2;
        }

        // Move and jump
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 0, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
        }

        // Propeller jump
        else if (POBJ_A_BUTTON_PRESSED && !obj_is_on_ground(o) && (gOmmObject->state.actionState == 0)) {
            o->oVelY = omm_capture_get_jump_velocity(o) * POBJ_PHYSICS_JUMP * 2.5f;
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
        if (POBJ_B_BUTTON_PRESSED && (gOmmObject->state.actionTimer == 0)) {
            omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_3, o->oCameraToObject);
            o->oVelY = omm_capture_get_jump_velocity(o) * POBJ_PHYSICS_JUMP / 2.f;
            gOmmObject->state.actionTimer = 30;
            gOmmObject->state.actionFlag = false;
            omm_mario_lock(gMarioState, 8);
            omm_spawn_shockwave_spindrift(o);
        }
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, gOmmObject->state.actionFlag ? 0.25f : 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS(

    // Doors
    obj_open_door(o, obj);
    
    );
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    vec3f_copy(o->oGfxPos, &o->oPosX);
    obj_anim_play(o, 0, 1.f);

    // Spin attack
    gOmmObject->state.actionTimer = max_s(0, gOmmObject->state.actionTimer - 1);
    if ((gOmmObject->state.actionTimer != 0) && omm_mario_is_locked(gMarioState)) {
        o->oGfxAngle[1] += 0x2000;
    }

    // Propeller jump
    else if (gOmmObject->state.actionState == 1) {
        o->oGfxAngle[1] += o->oVelY * 69;
        spawn_object(o, MODEL_NONE, bhvMistParticleSpawner);
    }

    // Float
    else if (gOmmObject->state.actionFlag) {
        o->oGfxAngle[1] += 0x1000;
    }

    // Default
    else {
        o->oGfxAngle[1] = o->oFaceAngleYaw;
    }

    // Cappy values
    gOmmObject->cappy.offset[1] = 160.f;
    gOmmObject->cappy.scale     = 1.2f;

    // OK
    POBJ_RETURN_OK;
}

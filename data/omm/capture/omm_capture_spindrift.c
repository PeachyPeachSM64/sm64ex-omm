#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_spindrift_init(UNUSED struct Object* o) {
    gOmmData->object->state.actionState = 0;
    gOmmData->object->state.actionTimer = 0;
    return true;
}

void cappy_spindrift_end(UNUSED struct Object *o) {
    o->oGravity = -4.f;
}

f32 cappy_spindrift_get_top(struct Object *o) {
    return 160.f * o->oScaleY;
}

//
// Update
//

s32 cappy_spindrift_update(struct Object *o) {

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
            gOmmData->object->state.actionState = 0;
        } else if (gOmmData->object->state.actionState == 1 && o->oVelY < 0.f) {
            gOmmData->object->state.actionState = 2;
        }

        // Move and jump
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 0, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
        }

        // Propeller jump
        else if (POBJ_A_BUTTON_PRESSED && !obj_is_on_ground(o) && (gOmmData->object->state.actionState == 0)) {
            o->oVelY = omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER * 2.5f;
            omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_1, o->oCameraToObject);
            gOmmData->object->state.actionState = 1;
        }

        // Float
        if (POBJ_A_BUTTON_DOWN && !obj_is_on_ground(o) && o->oVelY < 0.f) {
            if (!gOmmData->object->state.actionFlag) {
                omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_2, o->oCameraToObject);
            }
            gOmmData->object->state.actionFlag = true;
        } else {
            gOmmData->object->state.actionFlag = false;
        }

        // Spin attack
        if (POBJ_B_BUTTON_PRESSED && (gOmmData->object->state.actionTimer == 0)) {
            omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_3, o->oCameraToObject);
            o->oVelY = omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER / 2.f;
            gOmmData->object->state.actionTimer = 30;
            gOmmData->object->state.actionFlag = false;
            omm_mario_lock(gMarioState, 8);
            omm_spawn_shockwave_spindrift(o);
        }
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, gOmmData->object->state.actionFlag ? 0.25f : 1.f);
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
    gOmmData->object->state.actionTimer = max_s(0, gOmmData->object->state.actionTimer - 1);
    if ((gOmmData->object->state.actionTimer != 0) && omm_mario_is_locked(gMarioState)) {
        o->oGfxAngle[1] += 0x2000;
    }

    // Propeller jump
    else if (gOmmData->object->state.actionState == 1) {
        o->oGfxAngle[1] += o->oVelY * 69;
        spawn_object(o, MODEL_NONE, bhvMistParticleSpawner);
    }

    // Float
    else if (gOmmData->object->state.actionFlag) {
        o->oGfxAngle[1] += 0x1000;
    }

    // Default
    else {
        o->oGfxAngle[1] = o->oFaceAngleYaw;
    }

    // Cappy values
    gOmmData->object->cappy.offset[1] = 160.f;
    gOmmData->object->cappy.scale     = 1.2f;

    // OK
    POBJ_RETURN_OK;
}

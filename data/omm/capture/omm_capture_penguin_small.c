#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_penguin_small_init(UNUSED struct Object *o) {
    gOmmData->object->state.actionState = 0;
    gOmmData->object->state.actionTimer = 30;
    return true;
}

void cappy_penguin_small_end(UNUSED struct Object *o) {
    o->oAction = 0;
}

f32 cappy_penguin_small_get_top(UNUSED struct Object *o) {
    return 68.f * o->oScaleY;
}

//
// Update
//

s32 cappy_penguin_small_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;

    // Stop sliding
    if (gOmmData->object->state.actionState == 1 && o->oForwardVel < (omm_capture_get_walk_speed(o) / 2.f) && obj_is_on_ground(o)) {
        gOmmData->object->state.actionState = 2;
        omm_mario_lock(gMarioState, 20);
    }

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        gOmmData->object->state.actionState = (gOmmData->object->state.actionState & 1);
        if (pobj_jump(o, 0, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
        }

        // Walk
        if (gOmmData->mario->capture.stickMag > 0) {
            s32 incdec = 0x1000 * max_f(0.f, (1.0f - max_f(0, o->oForwardVel - omm_capture_get_walk_speed(o)) / 90.f));
            s32 faceYaw = gOmmData->mario->capture.stickYaw - approach_s32((s16) (gOmmData->mario->capture.stickYaw - o->oFaceAngleYaw), 0, incdec, incdec);
            obj_set_forward_vel(o, faceYaw, gOmmData->mario->capture.stickMag, gOmmData->object->state.actionState == 1 ? o->oForwardVel : omm_capture_get_walk_speed(o));
            o->oFaceAngleYaw = faceYaw;
            o->oFaceAnglePitch = 0;
            o->oFaceAngleRoll = 0;
            o->oMoveAngleYaw = faceYaw;
            o->oMoveAnglePitch = 0;
            o->oMoveAngleRoll = 0;
        } else if (gOmmData->object->state.actionState == 1) {
            obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, o->oForwardVel / 1.1f);
        }

        // Max speed multiplier
        f32 maxSpeedMult = 1.f;
        switch (mario_get_floor_class(gMarioState)) {
            case SURFACE_CLASS_NOT_SLIPPERY:  maxSpeedMult = 0.75f; break;
            case SURFACE_CLASS_DEFAULT:       maxSpeedMult = 1.00f; break;
            case SURFACE_CLASS_SLIPPERY:      maxSpeedMult = 1.25f; break;
            case SURFACE_CLASS_VERY_SLIPPERY: maxSpeedMult = 1.50f; break;
        }

        // Dive
        if (POBJ_B_BUTTON_PRESSED && (obj_is_on_ground(o) || gOmmData->object->state.actionState != 1)) {
            obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, clamp_f(o->oForwardVel * 1.5f, omm_capture_get_walk_speed(o), omm_capture_get_dash_speed(o) * maxSpeedMult));
            o->oVelY = max_f(o->oVelY, omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER / 2.f);
            o->oFloor = NULL;
            gOmmData->object->state.actionState = 1;
            omm_mario_lock(gMarioState, 15);
            play_sound(SOUND_OBJ_BABY_PENGUIN_DIVE, o->oCameraToObject);
        }

        // Slide
        if (POBJ_B_BUTTON_DOWN && obj_is_on_ground(o) && gOmmData->object->state.actionState == 1) {
            obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, clamp_f(o->oForwardVel * 1.05f, omm_capture_get_walk_speed(o), omm_capture_get_dash_speed(o) * maxSpeedMult));
        }
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, (gOmmData->object->state.actionState == 1) ? 0.99f : 0.80f, (gOmmData->object->state.actionState == 1) ? 0.99f : 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS(

    // Tuxie's mother
    // The timer is needed to prevent the mother's dialogs (and knockbacks)
    // from triggering as soon as Mario captures a small penguin
    if (obj->behavior == bhvTuxiesMother && (gOmmData->object->state.actionTimer == 0) && obj_is_on_ground(o)) {
        if (obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX)) {

            // Mother penguin is furious
            if (obj->oForwardVel != 0) {
                obj->oInteractStatus = INT_STATUS_INTERACTED;
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_KNOCKED_BACK, obj_is_object2_pushing_object1_backwards(o, obj, true), 0);
            }

            // Mother penguin is happy
            else {
                obj->oAction = 1;
                obj->oSubAction = 0;
                obj->prevObj = o;
                obj->oInteractStatus = INT_STATUS_INTERACTED;
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_NONE, false, 0);
            }
        }
    }

    );
    gOmmData->object->state.actionTimer = max_s(0, gOmmData->object->state.actionTimer - 1);
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_anim_play(o, gOmmData->object->state.actionState, 1.f);

    // Walk
    if (gOmmData->object->state.actionState == 0) {
        if (obj_is_on_ground(o)) {
            obj_make_step_sound_and_particle(o, &gOmmData->object->state.walkDistance, omm_capture_get_walk_speed(o) * 9.f, o->oForwardVel, SOUND_OBJ_BABY_PENGUIN_WALK, OBJ_PARTICLE_NONE);
        }
        gOmmData->object->cappy.offset[1] = 68.f;
        gOmmData->object->cappy.offset[2] = 12.f;
        gOmmData->object->cappy.scale     = 0.6f;
    }

    // Slide
    else if (gOmmData->object->state.actionState == 1) {
        if (obj_is_on_ground(o)) {
            obj_make_step_sound_and_particle(o, &gOmmData->object->state.walkDistance, 0.f, 0.f, SOUND_MOVING_TERRAIN_SLIDE + gMarioState->terrainSoundAddend, OBJ_PARTICLE_MIST);
        }
        gOmmData->object->cappy.offset[1] = 30.f;
        gOmmData->object->cappy.offset[2] = 50.f;
        gOmmData->object->cappy.scale     = 0.6f;
    }

    // Get up
    else if (gOmmData->object->state.actionState == 2) {
        gOmmData->object->cappy.offset[1] = 68.f;
        gOmmData->object->cappy.offset[2] = 12.f;
        gOmmData->object->cappy.scale     = 0.6f;
    }

    // OK
    POBJ_RETURN_OK;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

enum {
    CAPPY_KOOPA_STATE_SHELLED_IDLE,
    CAPPY_KOOPA_STATE_SHELLED_WALKING,
    CAPPY_KOOPA_STATE_SHELLED_SLIDING,
    CAPPY_KOOPA_STATE_SHELLED_JUMPING,
    CAPPY_KOOPA_STATE_UNSHELLED_IDLE,
    CAPPY_KOOPA_STATE_UNSHELLED_WALKING,
    CAPPY_KOOPA_STATE_UNSHELLED_RUNNING,
    CAPPY_KOOPA_STATE_UNSHELLED_JUMPING,
};

/* AnimIndex, Accel, Sfx, Model */
static s32 sCappyKoopaStateData[][4] = {
    {  7,   0, 0, MODEL_KOOPA_WITH_SHELL    },
    {  9,   1, 1, MODEL_KOOPA_WITH_SHELL    },
    {  0,   0, 2, MODEL_KOOPA_SHELL         },
    { 12, 100, 0, MODEL_KOOPA_WITH_SHELL    },
    {  7,   0, 0, MODEL_KOOPA_WITHOUT_SHELL },
    {  9,   1, 1, MODEL_KOOPA_WITHOUT_SHELL },
    {  3,   0, 1, MODEL_KOOPA_WITHOUT_SHELL },
    { 12, 100, 0, MODEL_KOOPA_WITHOUT_SHELL }
};

/* fdist, ldist, vdist, pitch, yaw, roll, scale */
static s32 sCappyKoopaCappyParams[][7] = {
    { 33, 0, 68, 0xE000, 0x0000, 0x0000, 65 },
    { 26, 0, 74, 0xDC00, 0x0000, 0x0000, 65 },
    {  0, 0,  0, 0x0000, 0x0000, 0x0000,  0 },
    {  6, 0, 70, 0xB400, 0x0000, 0x0000, 65 },
    { 33, 0, 68, 0xE000, 0x0000, 0x0000, 65 },
    { 26, 0, 74, 0xDC00, 0x0000, 0x0000, 65 },
    { -4, 0, 82, 0xD400, 0x0000, 0x0000, 65 },
    {  6, 0, 70, 0xB400, 0x0000, 0x0000, 65 },
};

//
// Init
//

bool cappy_koopa_init(struct Object *o) {

    // Can't possess KTQ
    if (o->oBehParams2ndByte == KOOPA_BP_KOOPA_THE_QUICK_BOB ||
        o->oBehParams2ndByte == KOOPA_BP_KOOPA_THE_QUICK_THI) {
        return false;
    }

    gOmmData->object->koopa.scale = o->oScaleY;
    return true;
}

void cappy_koopa_end(struct Object *o) {
    if (gOmmData->object->state.actionState < CAPPY_KOOPA_STATE_UNSHELLED_IDLE) {
        o->oKoopaMovementType = KOOPA_BP_NORMAL;
        o->oGraphNode = gLoadedGraphNodes[MODEL_KOOPA_WITH_SHELL];
    } else {
        o->oKoopaMovementType = KOOPA_BP_UNSHELLED;
        o->oGraphNode = gLoadedGraphNodes[MODEL_KOOPA_WITHOUT_SHELL];
    }
}

f32 cappy_koopa_get_top(struct Object *o) {
    return 70.f * o->oScaleY;
}

//
// Update
//

s32 cappy_koopa_update(struct Object *o) {
    obj_scale(o, gOmmData->object->koopa.scale);

    // Inputs
    if (!obj_update_door(o) && !omm_mario_is_locked(gMarioState)) {
        bool shelled = (o->oKoopaMovementType != KOOPA_BP_UNSHELLED);
        pobj_move(o, !shelled && POBJ_B_BUTTON_DOWN, shelled && POBJ_B_BUTTON_DOWN, false);
        if (pobj_jump(o, 0, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
        }
    }

    // States
    if (o->oKoopaMovementType != KOOPA_BP_UNSHELLED) {
        if (POBJ_B_BUTTON_DOWN) {
            gOmmData->object->state.actionState = CAPPY_KOOPA_STATE_SHELLED_SLIDING;
        } else if (obj_is_on_ground(o)) {
            if (o->oForwardVel > 1.f) {
                gOmmData->object->state.actionState = CAPPY_KOOPA_STATE_SHELLED_WALKING;
            } else {
                gOmmData->object->state.actionState = CAPPY_KOOPA_STATE_SHELLED_IDLE;
            }
        } else {
            gOmmData->object->state.actionState = CAPPY_KOOPA_STATE_SHELLED_JUMPING;
        }
    } else {
        if (obj_is_on_ground(o)) {
            if (o->oForwardVel > omm_capture_get_walk_speed(o)) {
                gOmmData->object->state.actionState = CAPPY_KOOPA_STATE_UNSHELLED_RUNNING;
            } else if (o->oForwardVel > 1.f) {
                gOmmData->object->state.actionState = CAPPY_KOOPA_STATE_UNSHELLED_WALKING;
            } else {
                gOmmData->object->state.actionState = CAPPY_KOOPA_STATE_UNSHELLED_IDLE;
            }
        } else {
            gOmmData->object->state.actionState = CAPPY_KOOPA_STATE_UNSHELLED_JUMPING;
        }
    }

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    bool shellSlide = (gOmmData->object->state.actionState == CAPPY_KOOPA_STATE_SHELLED_SLIDING);
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_IMMUNE_TO_LAVA * shellSlide;
    POBJ_SET_IMMUNE_TO_SAND * shellSlide;
    POBJ_SET_ABLE_TO_MOVE_ON_WATER * shellSlide;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES * shellSlide;
    POBJ_SET_ATTACKING * shellSlide;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS(

    // Doors
    obj_open_door(o, obj);

    // Koopa shell
    if (obj->behavior == bhvKoopaShell && o->oKoopaMovementType == KOOPA_BP_UNSHELLED) {
        if (obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX)) {
            o->oKoopaMovementType = KOOPA_BP_NORMAL;
            obj_spawn_white_puff(obj, SOUND_MENU_STAR_SOUND);
            obj_mark_for_deletion(obj);
            obj->oInteractStatus = INT_STATUS_INTERACTED;
        }
    }

    );
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    if (gOmmData->object->state.actionState == CAPPY_KOOPA_STATE_SHELLED_SLIDING) {
        obj_scale(o, gOmmData->object->koopa.scale / 1.5f);
        o->oGfxAngle[0] = 0;
        o->oGfxAngle[1] = o->oTimer * 0x2000;
        o->oGfxAngle[2] = 0;
    } else {
        obj_scale(o, gOmmData->object->koopa.scale);
    }

    // Animation
    s32 *animData = sCappyKoopaStateData[gOmmData->object->state.actionState];
    o->oGraphNode = gLoadedGraphNodes[animData[3]];
    obj_anim_play(o, animData[0], max_f(1.f, animData[1] * max_f(1.f, o->oForwardVel * 2.f / (omm_capture_get_walk_speed(o)))));
    obj_update_blink_state(o, &o->oKoopaBlinkTimer, 20, 50, 4);

    // Sound effect
    switch (animData[2]) {
        case 1:
            if (obj_is_on_ground(o)) {
                obj_make_step_sound_and_particle(o,
                    &gOmmData->object->state.walkDistance,
                    omm_capture_get_walk_speed(o) * 8.f, o->oForwardVel,
                    SOUND_OBJ_KOOPA_WALK, OBJ_PARTICLE_NONE
                );
            }
            break;

        case 2:
            switch (o->oFloorType) {
                case OBJ_FLOOR_TYPE_GROUND:
                    obj_make_step_sound_and_particle(o,
                        &gOmmData->object->state.walkDistance, 0.f, 0.f,
                        SOUND_MOVING_TERRAIN_RIDING_SHELL + gMarioState->terrainSoundAddend,
                        OBJ_PARTICLE_MIST
                    );
                    break;

                case OBJ_FLOOR_TYPE_WATER:
                    obj_make_step_sound_and_particle(o,
                        &gOmmData->object->state.walkDistance, 0.f, 0.f,
                        SOUND_MOVING_TERRAIN_RIDING_SHELL + SOUND_TERRAIN_WATER,
                        OBJ_PARTICLE_WATER_TRAIL | OBJ_PARTICLE_WATER_DROPLET
                    );
                    break;

                case OBJ_FLOOR_TYPE_LAVA:
                    obj_make_step_sound_and_particle(o,
                        &gOmmData->object->state.walkDistance, 0.f, 0.f,
                        SOUND_MOVING_RIDING_SHELL_LAVA,
                        OBJ_PARTICLE_FLAME
                    );
                    break;
            }
            break;
    }

    // Cappy values
    s32 *cappyParams = sCappyKoopaCappyParams[gOmmData->object->state.actionState];
    gOmmData->object->cappy.offset[0] = (f32) cappyParams[1];
    gOmmData->object->cappy.offset[1] = (f32) cappyParams[2];
    gOmmData->object->cappy.offset[2] = (f32) cappyParams[0];
    gOmmData->object->cappy.angle[0]  = (s16) cappyParams[3];
    gOmmData->object->cappy.angle[1]  = (s16) cappyParams[4];
    gOmmData->object->cappy.angle[2]  = (s16) cappyParams[5];
    gOmmData->object->cappy.scale     = (f32) cappyParams[6] / 100.f;

    // OK
    POBJ_RETURN_OK;
}

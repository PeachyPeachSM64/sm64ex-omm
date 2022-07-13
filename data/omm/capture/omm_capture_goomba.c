#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void set_goomba_as_standalone(struct Object *goomba) {
    if (goomba->parentObj && goomba->parentObj != goomba) {

        // Mark the goomba as dead inside the triplet
        set_object_respawn_info_bits(goomba->parentObj, (goomba->oBehParams2ndByte & GOOMBA_BP_TRIPLET_FLAG_MASK) >> 2);
        goomba->parentObj->oBehParams = goomba->parentObj->oBehParams | (goomba->oBehParams2ndByte & GOOMBA_BP_TRIPLET_FLAG_MASK) << 6;

        // Set the goomba as a standalone goomba
        goomba->oBehParams = (goomba->oGoombaSize & GOOMBA_BP_SIZE_MASK) << 16;
        goomba->oBehParams2ndByte = (goomba->oGoombaSize & GOOMBA_BP_SIZE_MASK);
        goomba->parentObj = goomba;
        goomba->prevObj = NULL;
    }
}

//
// Init
//

bool cappy_goomba_init(struct Object *o) {

    // Retrieve the nearest Goombas (up to OBJ_GOOMBA_STACK_MAX)
    // First Goomba is the one Mario's trying to capture
    struct Object *goombas[OBJ_GOOMBA_STACK_MAX] = { o };
    s32 goombaCount = 1;
    omm_array_for_each(omm_obj_get_goomba_behaviors(), p) {
        const BehaviorScript *bhv = (const BehaviorScript *) p->as_ptr;
        for_each_object_with_behavior(goomba, bhv) {
            if (goomba == o) continue;
            f32 dist = obj_get_horizontal_distance(o, goomba);
            f32 ydif = abs_f(goomba->oPosY - o->oPosY);
            if (dist < 20.f && ydif < (70.f * OBJ_GOOMBA_STACK_MAX * o->oScaleY)) {
                set_goomba_as_standalone(goomba);
                goombas[goombaCount++] = goomba;
                if (goombaCount >= OBJ_GOOMBA_STACK_MAX) {
                    break;
                }
            }
        }
    }

    // Store the lowest Y value, this will be the Y value of the base
    f32 lowestY = +20000.f;
    for (s32 i = 0; i != goombaCount; ++i) {
        lowestY = min_f(lowestY, goombas[i]->oPosY);
    }

    // Setup the first Goomba
    o->oPosY = lowestY;
    o->oGoombaStackParent = o;
    obj_update_gfx(o);
    set_goomba_as_standalone(o);

    // Setup the Goomba stack
    for (s32 i = 1; i != goombaCount; ++i) {
        gOmmData->object->goomba.stackObj[i - 1] = goombas[i];
        goombas[i]->oPosY = o->oPosY + i * o->oScaleY * 65.f;
        goombas[i]->curBhvCommand = omm_bhv_goomba_stack_capture;
        goombas[i]->bhvStackIndex = 0;
        goombas[i]->oInteractStatus = 0;
        goombas[i]->oIntangibleTimer = -1;
        goombas[i]->oGoombaStackParent = o;
        obj_update_gfx(goombas[i]);
    }
    gOmmData->object->goomba.stackCount = (goombaCount - 1);
    return true;
}

void cappy_goomba_end(struct Object *o) {
    struct MarioState *m = gMarioState;

    // Reset Goomba's state
    o->curBhvCommand = o->behavior;
    o->bhvStackIndex = 0;
    o->oAction = 0;
    o->oInteractStatus = 0;
    o->oIntangibleTimer = 3;
    o->oGoombaStackParent = o;
    set_goomba_as_standalone(o);

    // Stay stacked when released, but break the stack if Mario gets damaged
    bool stack = (m->action == ACT_IDLE || m->action == ACT_FREEFALL || m->action == ACT_WATER_IDLE || m->action == ACT_OMM_LEAVE_OBJECT_JUMP || m->action == ACT_OMM_LEAVE_OBJECT_WATER);
    for (u8 i = 0; i != gOmmData->object->goomba.stackCount; ++i) {
        struct Object *goomba = gOmmData->object->goomba.stackObj[i];
        goomba->curBhvCommand = (stack ? omm_bhv_goomba_stack : goomba->behavior);
        goomba->bhvStackIndex = 0;
        goomba->oAction = stack * (i + 1);
        goomba->oInteractStatus = 0;
        goomba->oIntangibleTimer = 3;
        goomba->oGoombaStackParent = o;
        set_goomba_as_standalone(goomba);
    }
}

f32 cappy_goomba_get_top(UNUSED struct Object *o) {
    return omm_capture_get_hitbox_height(o) * (1 + gOmmData->object->goomba.stackCount);
}

//
// Update
//

s32 cappy_goomba_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o) * (gOmmData->object->goomba.stackCount + 1);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;

    // Inputs
    if (!obj_update_door(o) && !omm_mario_is_locked(gMarioState)) {
        pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
        if (pobj_jump(o, 0, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
        }
    }

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

    // Goomba stack
    if (gOmmData->object->goomba.stackCount < (OBJ_GOOMBA_STACK_MAX - 1) && omm_obj_is_goomba(obj) && obj->oIntangibleTimer == 0) {

        // Fall check
        if ((!obj_is_on_ground(o) && o->oVelY <= 0.f) || (obj_is_on_ground(o) && o->oPosY > obj->oPosY + obj->hitboxHeight / 4.f)) {
            
            // Height check
            f32 y1 = obj->oPosY;
            f32 y2 = obj->oPosY + obj->hitboxHeight * 1.6f;
            if (y1 < o->oPosY && o->oPosY < y2) {

                // Radius check
                f32 r2 = sqr_f(o->hitboxRadius + obj->hitboxRadius);
                f32 d2 = sqr_f(o->oPosX - obj->oPosX) + sqr_f(o->oPosZ - obj->oPosZ);
                if (d2 < r2) {

                    // Add goomba to stack
                    gOmmData->object->goomba.stackObj[gOmmData->object->goomba.stackCount++] = obj;
                    obj->curBhvCommand = omm_bhv_goomba_stack_capture;
                    obj->bhvStackIndex = 0;
                    obj->oInteractStatus = INT_STATUS_INTERACTED;
                    obj->oIntangibleTimer = -1;
                    obj->oGoombaStackParent = o;
                    set_goomba_as_standalone(obj);
                    obj_spawn_white_puff(obj, SOUND_OBJ_DEFAULT_DEATH);
                }
            }
        }
    }

    );
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_anim_play(o, 0, (o->oVelY <= 0.f) * max_f(1.f, o->oForwardVel * 2.f / (omm_capture_get_walk_speed(o))));
    obj_update_blink_state(o, &o->oGoombaBlinkTimer, 30, 50, 5);
    if (obj_is_on_ground(o)) {
        obj_make_step_sound_and_particle(o, &gOmmData->object->state.walkDistance, omm_capture_get_walk_speed(o) * 8.f, o->oForwardVel, SOUND_OBJ_GOOMBA_WALK, OBJ_PARTICLE_NONE);
    }

    // Update Goomba stack
    for (u8 i = 0; i != gOmmData->object->goomba.stackCount; ++i) {
        struct Object *obj   = gOmmData->object->goomba.stackObj[i];
        obj->oPosX           = o->oPosX;
        obj->oPosY           = o->oPosY + omm_capture_get_hitbox_height(o) * (i + 1);
        obj->oPosZ           = o->oPosZ;
        obj->oHomeX          = o->oPosX;
        obj->oHomeY          = o->oPosY;
        obj->oHomeZ          = o->oPosZ;
        obj->oFaceAnglePitch = o->oFaceAnglePitch;
        obj->oFaceAngleYaw   = o->oFaceAngleYaw;
        obj->oFaceAngleRoll  = o->oFaceAngleRoll;
        obj->oMoveAnglePitch = o->oMoveAnglePitch;
        obj->oMoveAngleYaw   = o->oMoveAngleYaw;
        obj->oMoveAngleRoll  = o->oMoveAngleRoll;
        obj->oGfxPos[0]      = o->oPosX;
        obj->oGfxPos[1]      = o->oPosY;
        obj->oGfxPos[2]      = o->oPosZ;
        obj->oGfxAngle[0]    = o->oFaceAnglePitch;
        obj->oGfxAngle[1]    = o->oFaceAngleYaw;
        obj->oGfxAngle[2]    = o->oFaceAngleRoll;
        obj->oGfxScale[0]    = o->oGfxScale[0];
        obj->oGfxScale[1]    = o->oGfxScale[1];
        obj->oGfxScale[2]    = o->oGfxScale[2];
        obj_anim_play(obj, 0, 1.f);
        obj_update_blink_state(obj, &obj->oGoombaBlinkTimer, 30, 50, 5);
    }

    // Cappy values
    gOmmData->object->cappy.offset[1] = (omm_capture_get_hitbox_height(o) / o->oScaleY) * (1 + gOmmData->object->goomba.stackCount);
    gOmmData->object->cappy.scale     = 0.8f;

    // OK
    POBJ_RETURN_OK;
}

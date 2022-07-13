#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_bobomb_init(struct Object* o) {
    gOmmData->object->state.actionTimer = 0;
    gOmmData->object->state.actionState = 0;
    gOmmData->object->state.actionFlag = false;
    gOmmData->object->state.initialPos[0] = o->oHomeX;
    gOmmData->object->state.initialPos[1] = o->oHomeY;
    gOmmData->object->state.initialPos[2] = o->oHomeZ;
    o->oScaleX = 1.f;
    o->oScaleY = 1.f;
    o->oScaleZ = 1.f;
    return true;
}

void cappy_bobomb_end(struct Object *o) {
    o->oBehParams &= 0xFF00FFFF;
    o->oBehParams |= (BOBOMB_BP_STYPE_GENERIC << 16);
    o->oBehParams2ndByte = BOBOMB_BP_STYPE_GENERIC;
    o->oAnimInfo.curAnim = NULL;
    obj_anim_play(o, 0, 1.f);

    // As Bob-omb's respawn point is set to its home position,
    // reset the latter to its previous value if the floor
    // under it is not convenient
    struct Surface *floor = NULL;
    find_floor(o->oPosX, o->oPosY + 50.f, o->oPosZ, &floor);
    if (
#if OMM_GAME_IS_SM64
        (gCurrLevelNum == LEVEL_BITFS) ||
#endif
        (floor && (
        (floor->type == SURFACE_DEATH_PLANE) ||
        (floor->type == SURFACE_VERTICAL_WIND) ||
        (floor->type == SURFACE_BURNING)))) {
        o->oHomeX = gOmmData->object->state.initialPos[0];
        o->oHomeY = gOmmData->object->state.initialPos[1];
        o->oHomeZ = gOmmData->object->state.initialPos[2];
    }
}

f32 cappy_bobomb_get_top(struct Object *o) {
    return 94.f * o->oScaleY;
}

//
// Update
//

s32 cappy_bobomb_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;

    // Inputs
    if (!obj_update_door(o) && !omm_mario_is_locked(gMarioState)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 0, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
        }

        // Explosion
        if (POBJ_B_BUTTON_PRESSED && !gOmmData->object->state.actionFlag && gOmmData->object->state.actionTimer == 0) {
            omm_spawn_explosion(o);

            // Make the bob-omb bloat after each explosion
            o->oScaleX *= 1.2f;
            o->oScaleY *= 1.2f;
            o->oScaleZ *= 1.2f;
            gOmmData->object->state.actionFlag = true;
            gOmmData->object->state.actionTimer = 30;
            gOmmData->object->state.actionState++;

            // If airborne, do a double jump
            if (!obj_is_on_ground(o)) {
                o->oVelY = 1.6f * omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER;
            }
        } else {
            gOmmData->object->state.actionTimer = max_s(0, gOmmData->object->state.actionTimer - 1);
        }
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    o->oHomeX = gOmmData->object->state.initialPos[0];
    o->oHomeY = gOmmData->object->state.initialPos[1];
    o->oHomeZ = gOmmData->object->state.initialPos[2];
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Landing after 3 booms makes the bob-omb explode and ejects Mario
    if (obj_is_on_ground(o)) {
        gOmmData->object->state.actionFlag = false;
        if (gOmmData->object->state.actionState == 3) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 0);
            obj_destroy(o);
        }
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS(

    // Doors
    obj_open_door(o, obj);
    
    );
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_anim_play(o, 0, (o->oVelY <= 0.f) * max_f(1.f, o->oForwardVel * (2.f / (omm_capture_get_walk_speed(o)))));
    obj_random_blink(o, &o->oBobombBlinkTimer);
    if (obj_is_on_ground(o)) {
        obj_make_step_sound_and_particle(o, &gOmmData->object->state.walkDistance, omm_capture_get_walk_speed(o) * 11.f, o->oForwardVel, SOUND_OBJ_BOBOMB_WALK, OBJ_PARTICLE_NONE);
    }

    // Cappy values
    gOmmData->object->cappy.offset[1] = 94.f;
    gOmmData->object->cappy.scale     = 1.2f;

    // OK
    POBJ_RETURN_OK;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

enum {
    CAPPY_WHOMP_ACTION_DEFAULT = 0,
    CAPPY_WHOMP_ACTION_BEGIN_ATTACK,
    CAPPY_WHOMP_ACTION_ATTACK,
    CAPPY_WHOMP_ACTION_FALLING,
    CAPPY_WHOMP_ACTION_LAND,
    CAPPY_WHOMP_ACTION_END_ATTACK
};

//
// Init
//

bool cappy_whomp_init(struct Object *o) {
    if (o->oAction != 0 &&
        o->oAction != 1 &&
        o->oAction != 2 &&
        o->oAction != 3 &&
        o->oAction != 6 &&
        o->oAction != 7) {
        return false;
    }

    if (o->oAction == 6) {
        gOmmData->object->state.actionState = CAPPY_WHOMP_ACTION_END_ATTACK;
    } else {
        gOmmData->object->state.actionState = CAPPY_WHOMP_ACTION_DEFAULT;
    }
    gOmmData->object->state.actionTimer = 0;
    return true;
}

void cappy_whomp_end(struct Object *o) {
    o->oAction = 1;
    o->oSubAction = 0;
    o->oTimer = 0;
    o->oFaceAnglePitch = 0;
    o->oNumLootCoins = 0;
}

f32 cappy_whomp_get_top(struct Object *o) {
    return 430.f * o->oScaleY;
}

//
// Update
//

s32 cappy_whomp_update(struct Object *o) {

    // Collect the remaining coins from the Whomp
    if (o->oNumLootCoins > 0) {
        for (s32 i = 0; i != o->oNumLootCoins; ++i) {
            static struct Object coin[1];
            coin->oDamageOrCoinValue = 1;
            coin->respawnInfoType = 0;
            omm_mario_interact_coin(gMarioState, coin);
        }
        o->oNumLootCoins = 0;
    }

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 0, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
        }

        // Body slam
        if (POBJ_B_BUTTON_PRESSED) {
            gOmmData->object->state.actionState = CAPPY_WHOMP_ACTION_BEGIN_ATTACK;
        }
    }

    // Hitbox
    bool bodySlam = (gOmmData->object->state.actionState != CAPPY_WHOMP_ACTION_DEFAULT);
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o) * (bodySlam ? 0.25f : 1.f);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_INVULNERABLE * bodySlam;
    POBJ_SET_IMMUNE_TO_FIRE * bodySlam;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Begin Body slam
    if (gOmmData->object->state.actionState == CAPPY_WHOMP_ACTION_BEGIN_ATTACK) {
        omm_mario_lock(gMarioState, -1);
        obj_anim_play(o, 1, 2.0f);
        if (obj_anim_is_near_end(o)) {
            o->oVelY = 40.0f;
            gOmmData->object->state.actionState = CAPPY_WHOMP_ACTION_ATTACK;
        }
    }

    // Body slam
    else if (gOmmData->object->state.actionState == CAPPY_WHOMP_ACTION_ATTACK) {
        omm_mario_lock(gMarioState, -1);
        o->oFaceAnglePitch = min_s(o->oFaceAnglePitch + 0x400, 0x4000);
        if (o->oFaceAnglePitch == 0x4000) {
            gOmmData->object->state.actionState = CAPPY_WHOMP_ACTION_FALLING;
        }
    }

    // Falling
    else if (gOmmData->object->state.actionState == CAPPY_WHOMP_ACTION_FALLING) {
        omm_mario_lock(gMarioState, -1);
        if (obj_is_on_ground(o)) {
            f32 x = o->oPosX + sins(o->oFaceAngleYaw) * omm_capture_get_hitbox_height(o) / 2.f;
            f32 y = o->oPosY;
            f32 z = o->oPosZ + coss(o->oFaceAngleYaw) * omm_capture_get_hitbox_height(o) / 2.f;
            obj_spawn_white_puff_at(x, y, z, SOUND_OBJ_WHOMP_SLAM);
            cur_obj_shake_screen(SHAKE_POS_LARGE);
            gOmmData->object->state.actionState = CAPPY_WHOMP_ACTION_LAND;
            gOmmData->object->state.actionTimer = 0;
            omm_spawn_shockwave_whomp(o, x, y, z);
        }
    }

    // Landing
    else if (gOmmData->object->state.actionState == CAPPY_WHOMP_ACTION_LAND) {
        omm_mario_lock(gMarioState, -1);
        if (gOmmData->object->state.actionTimer++ == 30) {
            gOmmData->object->state.actionState = CAPPY_WHOMP_ACTION_END_ATTACK;
        }
    }

    // Getting up
    else if (gOmmData->object->state.actionState == CAPPY_WHOMP_ACTION_END_ATTACK) {
        omm_mario_lock(gMarioState, -1);
        o->oFaceAnglePitch = max_s(o->oFaceAnglePitch - 0x200, 0);
        if (o->oFaceAnglePitch == 0 && omm_mario_unlock(gMarioState)) {
            gOmmData->object->state.actionState = CAPPY_WHOMP_ACTION_DEFAULT;
        }
    }

    // Default
    else {
        obj_anim_play(o, 0, (o->oVelY <= 0.f) * max_f(1.f, o->oForwardVel * 3.f / (omm_capture_get_walk_speed(o))));
        if (obj_is_on_ground(o)) {
            obj_make_step_sound_and_particle(o, &gOmmData->object->state.walkDistance, omm_capture_get_walk_speed(o) * 9.f, o->oForwardVel, SOUND_OBJ_POUNDING1, OBJ_PARTICLE_NONE);
        }
    }

    // Gfx
    obj_update_gfx(o);

    // Cappy values
    gOmmData->object->cappy.offset[1] = 430.f;
    gOmmData->object->cappy.offset[2] = -45.f;
    gOmmData->object->cappy.scale     = 1.5f;

    // OK
    POBJ_RETURN_OK;
}

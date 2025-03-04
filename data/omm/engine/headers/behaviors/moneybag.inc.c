#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif
#include "data/omm/cappy/omm_cappy.h"

#define FAKE_MONEYBAG_COIN_ACT_HIDE (2)

static struct ObjectHitbox sMoneybagHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 120,
    /* height:            */ 120,
    /* hurtboxRadius:     */ 100,
    /* hurtboxHeight:     */ 100,
};

static struct ObjectHitbox sMoneybagHiddenHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 120,
    /* height:            */ 120,
    /* hurtboxRadius:     */ 100,
    /* hurtboxHeight:     */ 100,
};

void bhv_moneybag_init(void) {
    o->oGravity = 3.f;
    o->oFriction = 1.f;
    o->oBuoyancy = 2.f;
    o->oOpacity = 0;
    cur_obj_init_animation(0);
}

void moneybag_check_mario_collision(void) {
    obj_set_hitbox(o, &sMoneybagHitbox);
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (o->oInteractStatus & INT_STATUS_ATTACKED_MARIO) {
            o->oMoveAngleYaw = o->oAngleToMario + 0x8000;
            o->oVelY = 30.f;
        }
        if (o->oInteractStatus & INT_STATUS_WAS_ATTACKED) {
            cur_obj_become_intangible();
            o->oAction = MONEYBAG_ACT_DEATH;
        }
        o->oInteractStatus = 0;
    }
}

void moneybag_jump(s8 collisionFlags) {
    switch (o->oMoneybagJumpState) {
        case MONEYBAG_JUMP_PREPARE: {
            cur_obj_init_animation(1);
            if (o->oAnimFrame == 5) {
                o->oForwardVel = 20.f;
                o->oVelY = 40.f;
            }
            if (cur_obj_check_if_near_animation_end() == 1) {
                o->oMoneybagJumpState = MONEYBAG_JUMP_JUMP;
                cur_obj_play_sound_2(SOUND_GENERAL_BOING2_LOWPRIO);
            }
        } break;

        case MONEYBAG_JUMP_JUMP: {
            cur_obj_init_animation(2);
            if ((collisionFlags & OBJ_COL_FLAG_GROUNDED) == OBJ_COL_FLAG_GROUNDED) {
                o->oForwardVel = 0;
                o->oVelY = 0;
                o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
            }
        } break;

        case MONEYBAG_JUMP_JUMP_AND_BOUNCE: {
            cur_obj_init_animation(3);
            if (cur_obj_check_if_near_animation_end() == 1) {
                o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
            }
        } break;

        case MONEYBAG_JUMP_WALK_AROUND: {
            cur_obj_init_animation(4);
            o->oForwardVel = 10.f;
            if (o->oTimer >= 61) {
                o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
                o->oForwardVel = 0;
                o->oAnimFrame = 0;
            }
        } break;

        case MONEYBAG_JUMP_WALK_HOME: {
            cur_obj_init_animation(4);
            o->oForwardVel = 5.f;
        } break;
    }
}

static s32 moneybag_is_within_radius_of_mario_or_cappy(s32 dist) {
    if (is_point_within_radius_of_mario(o->oHomeX, o->oHomeY, o->oHomeZ, dist)) {
        return TRUE;
    }
    struct Object *cappy = omm_cappy_get_object();
    if (cappy && is_point_close_to_object(cappy, o->oHomeX, o->oHomeY, o->oHomeZ, dist)) {
        return TRUE;
    }
    return FALSE;
}

void moneybag_act_appear(void) {
    cur_obj_enable_rendering();
    o->oOpacity += 18;
    if (o->oOpacity >= 256) {
        o->oOpacity = 255;
        if (o->oMoneybagParentObj) {
            o->oMoneybagParentObj->oAction = FAKE_MONEYBAG_COIN_ACT_HIDE;
        }
        cur_obj_become_tangible();
        o->oAction = MONEYBAG_ACT_MOVE_AROUND;
    }
}

void moneybag_act_move_around(void) {
    obj_return_and_displace_home(o, o->oHomeX, o->oHomeY, o->oHomeZ, 200);

    s16 collisionFlags = object_step();
    if (((collisionFlags & OBJ_COL_FLAGS_LANDED) == OBJ_COL_FLAGS_LANDED) && (o->oMoneybagJumpState == MONEYBAG_JUMP_LANDING)) {
        if ((s32) (random_float() * 6.f) == 1) {
            o->oMoneybagJumpState = MONEYBAG_JUMP_WALK_AROUND;
            o->oTimer = 0;
        } else {
            o->oMoneybagJumpState = MONEYBAG_JUMP_PREPARE;
        }
    }

    moneybag_jump(collisionFlags);
    moneybag_check_mario_collision();

    if (!moneybag_is_within_radius_of_mario_or_cappy(800) && ((collisionFlags & OBJ_COL_FLAGS_LANDED) == OBJ_COL_FLAGS_LANDED)) {
        cur_obj_become_tangible();
        o->oAction = MONEYBAG_ACT_RETURN_HOME;
    }
}

void moneybag_act_return_home(void) {
    s16 targetYaw = atan2s(o->oHomeZ - o->oPosZ, o->oHomeX - o->oPosX);
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, targetYaw, 0x800);

    s16 collisionFlags = object_step();
    if (((collisionFlags & OBJ_COL_FLAGS_LANDED) == OBJ_COL_FLAGS_LANDED) && (o->oMoneybagJumpState == MONEYBAG_JUMP_LANDING)) {
        o->oMoneybagJumpState = MONEYBAG_JUMP_WALK_HOME;
    }

    moneybag_jump(collisionFlags);
    moneybag_check_mario_collision();

    if (is_point_close_to_object(o, o->oHomeX, o->oHomeY, o->oHomeZ, 100)) {
        if (!o->oMoneybagParentObj) {
            o->oMoneybagParentObj = spawn_object(o, MODEL_YELLOW_COIN, bhvMoneybagHidden);
            o->oMoneybagParentObj->oMoneybagParentObj = o;
        }
        cur_obj_play_sound_2(SOUND_GENERAL_VANISH_SFX);
        cur_obj_init_animation(0);
        cur_obj_become_intangible();
        o->oMoneybagParentObj->oAction = FAKE_MONEYBAG_COIN_ACT_IDLE;
        o->oAction = MONEYBAG_ACT_DISAPPEAR;
        o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
    }

    else if (moneybag_is_within_radius_of_mario_or_cappy(800)) {
        cur_obj_become_tangible();
        o->oAction = MONEYBAG_ACT_MOVE_AROUND;
        o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
    }
}

void moneybag_act_disappear(void) {
    o->oOpacity -= 12;
    if (o->oOpacity < 0) {
        o->oOpacity = 0;
        cur_obj_disable_rendering();
    }
}

void moneybag_act_death(void) {
    if (o->oTimer == 1) {
        obj_spawn_yellow_coins(o, 5);
        create_sound_spawner(SOUND_GENERAL_SPLATTERING);
        spawn_mist_particles();
        obj_mark_for_deletion(o);
        if (o->oMoneybagParentObj) {
            obj_mark_for_deletion(o->oMoneybagParentObj);
        }
    }
}

void bhv_moneybag_loop(void) {
    switch (o->oAction) {
        case MONEYBAG_ACT_APPEAR:
        case MONEYBAG_ACT_UNUSED_APPEAR: moneybag_act_appear(); break;
        case MONEYBAG_ACT_MOVE_AROUND: moneybag_act_move_around(); break;
        case MONEYBAG_ACT_RETURN_HOME: moneybag_act_return_home(); break;
        case MONEYBAG_ACT_DISAPPEAR: moneybag_act_disappear(); break;
        case MONEYBAG_ACT_DEATH: moneybag_act_death(); break;
    }
}

void bhv_moneybag_hidden_loop(void) {
    vec3f_copy(&o->oHomeX, &o->oPosX);
    obj_set_hitbox(o, &sMoneybagHiddenHitbox);
    switch (o->oAction) {
        case FAKE_MONEYBAG_COIN_ACT_IDLE: {
            cur_obj_become_tangible();
            cur_obj_enable_rendering();
            if (moneybag_is_within_radius_of_mario_or_cappy(400)) {
                if (!o->oMoneybagParentObj) {
                    o->oMoneybagParentObj = spawn_object(o, MODEL_MONEYBAG, bhvMoneybag);
                    o->oMoneybagParentObj->oMoneybagParentObj = o;
                }
                cur_obj_play_sound_2(SOUND_GENERAL_VANISH_SFX);
                o->oMoneybagParentObj->oAction = MONEYBAG_ACT_APPEAR;
                o->oAction = FAKE_MONEYBAG_COIN_ACT_TRANSFORM;
            }
        } break;

        case FAKE_MONEYBAG_COIN_ACT_TRANSFORM: {
        } break;

        case FAKE_MONEYBAG_COIN_ACT_HIDE: {
            cur_obj_become_intangible();
            cur_obj_disable_rendering();
        } break;
    }
    if (o->oMoneybagParentObj) {
        if (!o->oMoneybagParentObj->activeFlags || o->oMoneybagParentObj->behavior != bhvMoneybag) {
            obj_mark_for_deletion(o);
        } else {
            vec3f_copy(&o->oPosX, &o->oMoneybagParentObj->oPosX);
        }
    }
    o->oInteractStatus = 0;
}

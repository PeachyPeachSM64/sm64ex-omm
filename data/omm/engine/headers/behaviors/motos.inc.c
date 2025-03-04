#define MOTOS_ACT_WAIT              (0)
#define MOTOS_ACT_PLAYER_SEARCH     (1)
#define MOTOS_ACT_PLAYER_CARRY      (2)
#define MOTOS_ACT_THROWN            (3)
#define MOTOS_ACT_RECOVER           (4)
#define MOTOS_ACT_DEATH             (5)

#define MOTOS_ANIM_BASE             (0)
#define MOTOS_ANIM_CARRY            (1)
#define MOTOS_ANIM_CARRY_RUN        (2)
#define MOTOS_ANIM_CARRY_START      (3)
#define MOTOS_ANIM_DOWN_RECOVER     (4)
#define MOTOS_ANIM_DOWN_STOP        (5)
#define MOTOS_ANIM_PITCH            (6)
#define MOTOS_ANIM_SAFE_DOWN        (7)
#define MOTOS_ANIM_WAIT             (8)
#define MOTOS_ANIM_WALK             (9)

#ifndef MOTOS_CONSTANTS_ONLY
#if OMM_GAME_IS_R96X
#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

void bhv_motos_wait() {
    o->oForwardVel = 0.f;
    o->oVelY = 0.f;
    cur_obj_init_animation_with_sound(MOTOS_ANIM_WALK);
    if (o->oDistanceToMario < 500.f) {
        o->oAction = MOTOS_ACT_PLAYER_SEARCH;
    }
}

void bhv_motos_player_search() {
    o->oForwardVel = 6.f;
    cur_obj_init_animation_with_sound(MOTOS_ANIM_WALK);
    cur_obj_rotate_yaw_toward(o->oAngleToMario, 350);
    cur_obj_update_floor_and_walls();
    if (o->oInteractStatus & INT_STATUS_GRABBED_MARIO) {
        o->oAction = MOTOS_ACT_PLAYER_CARRY;
        o->oSubAction = 0;
        o->oChuckyaUnk88 = 1;
    }
}

void bhv_motos_player_carry() {
    switch (o->oSubAction) {
        case 0: {
            cur_obj_init_animation_with_sound(MOTOS_ANIM_CARRY_START);
            o->oForwardVel = 0.f;
            if (cur_obj_check_if_near_animation_end()) {
                o->oSubAction = 1;
                o->oTimer = 0;
            }
        } break;

        case 1: {
            o->oForwardVel = 15.f;
            cur_obj_init_animation_with_sound(MOTOS_ANIM_CARRY_RUN);
            if (o->oTimer > 45 || (o->oMoveFlags & OBJ_MOVE_HIT_EDGE)) {
                o->oSubAction = 2;
                o->oTimer = 0;
            }
        } break;

        case 2: {
            o->oForwardVel = 0.f;
            cur_obj_init_animation_with_sound(MOTOS_ANIM_PITCH);
            if (cur_obj_check_anim_frame(14)) {
                o->oChuckyaUnk88 = 2;
                o->numCollidedObjs = 20;
            }
            if (cur_obj_check_if_near_animation_end()) {
                o->oAction = MOTOS_ACT_WAIT;
                o->oInteractStatus &= ~INT_STATUS_GRABBED_MARIO;
            }
        } break;
    }
}

void bhv_motos_thrown(void) {
    cur_obj_init_animation_with_sound(MOTOS_ANIM_CARRY);
    if (o->oMoveFlags & OBJ_MOVE_LANDED) {
        o->oAction = MOTOS_ACT_RECOVER;
        cur_obj_play_sound_2(SOUND_OBJ2_LARGE_BULLY_ATTACKED);
    }
}

void bhv_motos_recover(void) {
    o->oForwardVel = 0.f;
    cur_obj_init_animation_with_sound(MOTOS_ANIM_DOWN_RECOVER);
    if (cur_obj_check_if_near_animation_end()) {
        o->oAction = MOTOS_ACT_WAIT;
    }
}

void moto_spawn_coin(void) {
    struct Object *coin = spawn_object(o, MODEL_BLUE_COIN, bhvBlueCoinMotos);
    cur_obj_play_sound_2(SOUND_GENERAL_COIN_SPURT_2);
    coin->oForwardVel = 10.f;
    coin->oVelY = 100.f;
    coin->oPosY += 310.f;
    coin->oMoveAngleYaw = o->oFaceAngleYaw + 0x8000 + (random_u16() & 0x3FF);
}

void bhv_motos_death(void) {
    if (obj_lava_death()) {
        moto_spawn_coin();
    }
}

void bhv_motos_main() {
    cur_obj_update_floor_and_walls();
    switch (o->oAction) {
        case MOTOS_ACT_WAIT: bhv_motos_wait(); break;
        case MOTOS_ACT_PLAYER_SEARCH: bhv_motos_player_search(); break;
        case MOTOS_ACT_PLAYER_CARRY: bhv_motos_player_carry(); break;
        case MOTOS_ACT_THROWN: bhv_motos_thrown(); break;
        case MOTOS_ACT_RECOVER: bhv_motos_recover(); break;
        case MOTOS_ACT_DEATH: bhv_motos_death(); return;
    }
    cur_obj_move_standard(-78);
    struct Surface *floor = NULL;
    f32 floorY = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
    if (floor && o->oPosY < floorY + 1.f && floor->type == SURFACE_BURNING) {
        o->oAction = MOTOS_ACT_DEATH;
    }
}

void bhv_motos_loop(void) {
    cur_obj_scale(2.f);
    o->oInteractionSubtype |= INT_SUBTYPE_GRABS_MARIO;
    u32 attackFlags = (o->oInteractStatus >> 24) & (OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_WEAK);
    if (attackFlags) {
        if (attackFlags & OBJ_INT_ATTACK_STRONG) {
            o->oHeldState = HELD_THROWN;
        } else {
            o->oMoveAngleYaw += 0x8000;
            o->oAction = MOTOS_ACT_PLAYER_SEARCH;
        }
        if (omm_mario_is_grabbed(gMarioState) && gMarioState->usedObj == o) {
            o->oChuckyaUnk88 = 2;
        }
        o->oInteractStatus = 0;
        cur_obj_play_sound_2(SOUND_OBJ2_LARGE_BULLY_ATTACKED);
    }
    switch (o->oHeldState) {
        case HELD_FREE: bhv_motos_main(); break;
        case HELD_HELD: cur_obj_unrender_and_reset_state(MOTOS_ANIM_CARRY, MOTOS_ACT_WAIT); break;
        case HELD_THROWN: cur_obj_get_thrown_or_placed(15.f, 35.f, MOTOS_ACT_THROWN); break;
        case HELD_DROPPED: cur_obj_get_thrown_or_placed(15.f, 35.f, MOTOS_ACT_THROWN); break;
    }
    o->oInteractStatus = 0;
}

#endif
#endif

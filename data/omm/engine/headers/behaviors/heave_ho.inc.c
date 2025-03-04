#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

void bhv_heave_ho_throw_mario_loop(void) {
    obj_mark_for_deletion(o);
}

void heave_ho_act_0(void) {
    cur_obj_set_pos_to_home();
    o->oAction = 1;
}

void heave_ho_act_1(void) {
    o->oForwardVel = 0.f;
    cur_obj_reverse_animation();
    if (o->oTimer >= 118) {
        o->oAction = 2;
    } else {
        f32 accel = (max_s(0, o->oTimer - 20) % 22) >= 10;
        cur_obj_init_animation_with_accel_and_sound(2, accel);
    }
}

void heave_ho_act_2(void) {
    if (1000.f < cur_obj_lateral_dist_from_mario_to_home()) {
        o->oAngleToMario = cur_obj_angle_to_home();
    }
    if (o->oTimer > 150) {
        o->oHeaveHoUnkF4 = (302 - o->oTimer) / 152.f;
        if (o->oHeaveHoUnkF4 < 0.1f) {
            o->oHeaveHoUnkF4 = 0.1f;
            o->oAction = 1;
        }
    } else {
        o->oHeaveHoUnkF4 = 1.f;
    }
    cur_obj_init_animation_with_accel_and_sound(0, o->oHeaveHoUnkF4);
    o->oForwardVel = o->oHeaveHoUnkF4 * 10.f;
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, o->oHeaveHoUnkF4 * 0x400);
}

void heave_ho_act_3(void) {
    o->oForwardVel = 0.f;
    if (o->oTimer == 0) {
        cur_obj_init_animation_with_accel_and_sound(1, 1.f);
        cur_obj_play_sound_2(SOUND_OBJ_HEAVEHO_TOSSED);
        gMarioObject->oInteractStatus |= INT_STATUS_MARIO_UNK2;
        gMarioState->forwardVel = -45.f;
        gMarioState->vel[1] = 95.f;
        o->numCollidedObjs = 20;
    }
    if (cur_obj_check_if_near_animation_end()) {
        o->oAction = 1;
    }
}

void (*sHeaveHoActions[])(void) = { heave_ho_act_0, heave_ho_act_1, heave_ho_act_2, heave_ho_act_3 };

void heave_ho_move(void) {
    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sHeaveHoActions);
    cur_obj_move_standard(-78);
    if (o->oForwardVel > 3.f && find_water_level(o->oPosX, o->oPosZ) < o->oPosY + 80.f) {
        cur_obj_play_sound_1(SOUND_AIR_HEAVEHO_MOVE);
    }
    if (o->oInteractStatus & INT_STATUS_GRABBED_MARIO) {
        o->oAction = 3;
    }
}

void bhv_heave_ho_loop(void) {
    cur_obj_scale(2.f);
    switch (o->oHeldState) {
        case HELD_FREE: heave_ho_move(); break;
        case HELD_HELD: cur_obj_unrender_and_reset_state(0, 1); break;
        case HELD_THROWN: cur_obj_get_thrown_or_placed(24.f, 36.f, 1); break;
        case HELD_DROPPED: cur_obj_get_dropped(); break;
    }
    o->oInteractStatus = 0;
    o->oGraphYOffset = 0.f;
    o->oDragStrength = 0.9f;
    o->oBuoyancy = 1.5f;
}

#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

#if OMM_GAME_IS_R96X
#include FILE_CHEATS_H
#endif

//
// Interaction
//

static void bhv_1up_interact(void) {
    if (obj_check_if_collided_with_object(o, gMarioObject)) {
#if OMM_GAME_IS_R96X
        if (Cheats.EnableCheats && Cheats.ChaosGreenDemon) {
            play_sound(SOUND_MENU_CAMERA_BUZZ, gGlobalSoundArgs);
            gMarioState->health = OMM_HEALTH_DEAD;
            gMarioState->healCounter = 0;
            gMarioState->hurtCounter = 0;
            obj_mark_for_deletion(o);
            return;
        }
#endif
        omm_health_life_up(gMarioState);
        obj_mark_for_deletion(o);
        omm_stats_increase(mushrooms1upCollected, 1);
    }
}

//
// Actions
//

// Update visibility, tangibility and sparkles
// `sparkles` is ignored if `tangible` is set to `false`
// `tangible` is ignored if `visible` is set to `false`
// `bhv_1up_interact()` is run only if `visible` and `tangible` are `true`
static void bhv_1up_common_update(bool visible, bool tangible, bool sparkles) {
    if (visible) {
        cur_obj_unhide();
        if (tangible) {
            cur_obj_become_tangible();
            bhv_1up_interact();
            if (sparkles) {
                spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
            }
        } else {
            cur_obj_become_intangible();
        }
    } else {
        cur_obj_hide();
        cur_obj_become_intangible();
    }
}

static void bhv_1up_loop_in_air(void) {
    if (o->oTimer < 5) {
        o->oVelY = 40.f;
    } else {
        o->oAngleVelPitch = -0x1000;
        o->oMoveAnglePitch += o->oAngleVelPitch;
        o->oVelY = coss(o->oMoveAnglePitch) * 30.f + 2.f;
        o->oForwardVel = -sins(o->oMoveAnglePitch) * 30.f;
    }
}

static void bhv_1up_move_towards_mario(void) {
    f32 dx = gMarioObject->oPosX - o->oPosX;
    f32 dy = gMarioObject->oPosY - o->oPosY + 120.f;
    f32 dz = gMarioObject->oPosZ - o->oPosZ;
    s16 pitch = atan2s(sqrtf(sqr(dx) + sqr(dz)), dy);
    obj_turn_toward_object(o, gMarioObject, 16, 0x1000);
    o->oMoveAnglePitch = approach_s16_symmetric(o->oMoveAnglePitch, pitch, 0x1000);
    o->oVelY = sins(o->oMoveAnglePitch) * 30.f;
    o->oForwardVel = coss(o->oMoveAnglePitch) * 30.f;
}

static void bhv_1up_move_away_from_mario(s16 colFlags) {
    o->oForwardVel = 8.f;
    o->oMoveAngleYaw = o->oAngleToMario + 0x8000;
    if (colFlags & OBJ_COL_FLAG_HIT_WALL) {
        o->oAction = 2;
    }
    if (!is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 3000)) {
        o->oAction = 2;
    }
}

static void bhv_1up_sliding_move(void) {
    s16 colFlags = object_step();
    if (colFlags & OBJ_COL_FLAG_GROUNDED) {
        o->oForwardVel += 25.f;
        o->oVelY = 0;
    } else {
        o->oForwardVel *= 0.98f;
    }
    if (o->oForwardVel > 40.f) {
        o->oForwardVel = 40.f;
    }
    if (!is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 5000)) {
        o->oAction = 2;
    }
}

//
// Init
//

void bhv_1up_common_init(void) {
    o->oMoveAnglePitch = -0x4000;
    o->oGravity = 3.f;
    o->oFriction = 1.f;
    o->oBuoyancy = 1.f;
}

void bhv_1up_init(void) {
    bhv_1up_common_init();
}

//
// Update
//

void bhv_1up_walking_loop(void) {
    bhv_1up_common_update(true, true, true);
    object_step();

    switch (o->oAction) {
        case 0: {
            bhv_1up_loop_in_air();
            if (o->oTimer == 0) {
                play_sound(SOUND_GENERAL2_1UP_APPEAR, gGlobalSoundArgs);
            }
            if (o->oTimer == 37) {
                o->oAction = 1;
                o->oForwardVel = 2.f;
            }
        } break;

        case 1: {
            if (o->oTimer > 300) {
                o->oAction = 2;
            }
        } break;

        case 2: {
            obj_flicker_and_disappear(o, 30);
        } break;
    }
}

void bhv_1up_running_away_loop(void) {
    bhv_1up_common_update(true, true, true);
    s16 colFlags = object_step();

    switch (o->oAction) {
        case 0: {
            bhv_1up_loop_in_air();
            if (o->oTimer == 0) {
                play_sound(SOUND_GENERAL2_1UP_APPEAR, gGlobalSoundArgs);
            }
            if (o->oTimer == 37) {
                o->oAction = 1;
                o->oForwardVel = 8.f;
            }
        } break;

        case 1: {
            bhv_1up_move_away_from_mario(colFlags);
        } break;

        case 2: {
            obj_flicker_and_disappear(o, 30);
        } break;
    }
}

void bhv_1up_sliding_loop(void) {
    bool sparkles = o->oAction >= 1;
    bhv_1up_common_update(true, true, sparkles);

    switch (o->oAction) {
        case 0: {
            if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 1000)) {
                o->oAction = 1;
            }
        } break;

        case 1: {
            bhv_1up_sliding_move();
        } break;

        case 2: {
            obj_flicker_and_disappear(o, 30);
        } break;
    }
}

void bhv_1up_loop(void) {
    bhv_1up_common_update(true, true, false);
}

void bhv_1up_jump_on_approach_loop(void) {
    bool sparkles = o->oAction >= 1;
    bhv_1up_common_update(true, true, sparkles);

    switch (o->oAction) {
        case 0: {
            if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 1000)) {
                o->oVelY = 40.f;
                o->oAction = 1;
            }
        } break;

        case 1: {
            s16 colFlags = object_step();
            bhv_1up_move_away_from_mario(colFlags);
        } break;

        case 2: {
            object_step();
            obj_flicker_and_disappear(o, 30);
        } break;
    }
}

void bhv_1up_hidden_loop(void) {
    bool hidden = o->oAction == 0;
    bhv_1up_common_update(!hidden, !hidden, !hidden);

    switch (o->oAction) {
        case 0: {
            if (o->o1UpHiddenUnkF4 == o->oBehParams2ndByte) {
                o->oVelY = 40.f;
                o->oAction = 3;
                play_sound(SOUND_GENERAL2_1UP_APPEAR, gGlobalSoundArgs);
            }
        } break;

        case 1: {
            s16 colFlags = object_step();
            bhv_1up_move_away_from_mario(colFlags);
        } break;

        case 2: {
            object_step();
            obj_flicker_and_disappear(o, 30);
        } break;

        case 3: {
            object_step();
            bhv_1up_loop_in_air();
            if (o->oTimer == 37) {
                o->oAction = 1;
                o->oForwardVel = 8.f;
            }
        } break;
    }
}

void bhv_1up_hidden_in_pole_loop(void) {
    bool hidden = o->oAction == 0;
    bhv_1up_common_update(!hidden, !hidden, !hidden);

    switch (o->oAction) {
        case 0: {
            if (o->o1UpHiddenUnkF4 == o->oBehParams2ndByte) {
                o->oVelY = 40.f;
                o->oAction = 3;
                play_sound(SOUND_GENERAL2_1UP_APPEAR, gGlobalSoundArgs);
            }
        } break;

        case 1: {
            bhv_1up_move_towards_mario();
            object_step();
        } break;

        case 3: {
            object_step();
            bhv_1up_loop_in_air();
            if (o->oTimer == 37) {
                o->oAction = 1;
                o->oForwardVel = 10.f;
            }
        } break;
    }
}

//
// Hidden triggers
//

void bhv_1up_hidden_trigger_loop(void) {
    if (obj_check_if_collided_with_object(o, gMarioObject)) {
        struct Object *hidden1up = cur_obj_nearest_object_with_behavior(bhvHidden1up);
        if (hidden1up) {
            hidden1up->o1UpHiddenUnkF4++;
        }
        obj_mark_for_deletion(o);
    }
}

void bhv_1up_hidden_in_pole_trigger_loop(void) {
    if (obj_check_if_collided_with_object(o, gMarioObject)) {
        struct Object *hidden1upInPole = cur_obj_nearest_object_with_behavior(bhvHidden1upInPole);
        if (hidden1upInPole) {
            hidden1upInPole->o1UpHiddenUnkF4++;
        }
        obj_mark_for_deletion(o);
    }
}

void bhv_1up_hidden_in_pole_spawner_loop(void) {
    if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 700)) {
        spawn_object_relative(2, 0, 50, 0, o, MODEL_1UP, bhvHidden1upInPole);
        for (s32 i = 0; i < 2; i++) {
            spawn_object_relative(0, 0, i * -200, 0, o, MODEL_NONE, bhvHidden1upInPoleTrigger);
        }
        obj_mark_for_deletion(o);
    }
}

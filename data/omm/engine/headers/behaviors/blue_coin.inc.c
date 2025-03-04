#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

#define BLUE_COIN_SWITCH_ACT_RISING 3

//
// Hidden blue coin
//

void bhv_hidden_blue_coin_loop(void) {
    o->oTransparency = 0;
    switch (o->oAction) {
        case HIDDEN_BLUE_COIN_ACT_INACTIVE: {
            cur_obj_hide();
            cur_obj_become_intangible();
            o->oAction = HIDDEN_BLUE_COIN_ACT_WAITING;
        } break;

        case HIDDEN_BLUE_COIN_ACT_WAITING: {
            struct Object *blueCoinSwitch = obj_get_nearest_with_behavior(o, bhvBlueCoinSwitch);
            if (blueCoinSwitch) {
                if (blueCoinSwitch->oAction == BLUE_COIN_SWITCH_ACT_TICKING) {
                    o->oAction = HIDDEN_BLUE_COIN_ACT_ACTIVE;
                } else if (gMarioObject->platform == blueCoinSwitch) {
                    o->oTransparency = 0x80;
                    cur_obj_unhide();
                } else {
                    cur_obj_hide();
                }
            }
        } break;

        case HIDDEN_BLUE_COIN_ACT_ACTIVE: {
            cur_obj_unhide();
            cur_obj_become_tangible();
            if (o->oInteractStatus & INT_STATUS_INTERACTED) {
                spawn_object(o, MODEL_SPARKLES, bhvGoldenCoinSparkles);
                obj_mark_for_deletion(o);
            }
            if (cur_obj_wait_then_blink(200, 20)) {
                cur_obj_hide();
                cur_obj_become_intangible();
                o->oAction = HIDDEN_BLUE_COIN_ACT_INACTIVE;
            }
        } break;
    }
    o->oInteractStatus = 0;
}

//
// Blue coin switch
//

void bhv_blue_coin_switch_loop(void) {
    cur_obj_scale(3.f);
    switch (o->oAction) {
        case BLUE_COIN_SWITCH_ACT_IDLE: {
            obj_load_collision_model(o);
            o->oHomeY = o->oPosY;
            if (gMarioObject->platform == o) {
                if (omm_mario_is_ground_pound_landing(gMarioState)) {
                    o->oAction = BLUE_COIN_SWITCH_ACT_RECEDING;
                } else if (!obj_get_first_with_behavior(bhvOmmBlueCoinsNumber)) {
                    omm_obj_spawn_blue_coins_number(gMarioObject);
                }
            }
        } break;

        case BLUE_COIN_SWITCH_ACT_RECEDING: {
            if (o->oTimer == 0) {
                o->oHomeY = o->oPosY;
                obj_play_sound(o, SOUND_GENERAL_SWITCH_DOOR_OPEN);
            } else if (o->oTimer > 5) {
                cur_obj_hide();
                o->oAction = BLUE_COIN_SWITCH_ACT_TICKING;
                spawn_mist_particles_variable(0, 0, 46);
            }
            o->oPosY = o->oHomeY - 20.f * o->oTimer;
            obj_load_collision_model(o);
        } break;

        case BLUE_COIN_SWITCH_ACT_TICKING: {
            if (o->oTimer < 200) {
                play_sound(SOUND_GENERAL2_SWITCH_TICK_FAST, gGlobalSoundArgs);
            } else {
                play_sound(SOUND_GENERAL2_SWITCH_TICK_SLOW, gGlobalSoundArgs);
            }
            if (!obj_get_nearest_with_behavior(o, bhvHiddenBlueCoin)) {
                obj_mark_for_deletion(o);
            } else if (o->oTimer > 240) {
                obj_play_sound(o, SOUND_GENERAL_SWITCH_DOOR_OPEN);
                cur_obj_unhide();
                o->oAction = BLUE_COIN_SWITCH_ACT_RISING;
            }
        } break;

        case BLUE_COIN_SWITCH_ACT_RISING: {
            if (o->oTimer == 0) {
                obj_play_sound(o, SOUND_GENERAL_SWITCH_DOOR_OPEN);
            } else if (o->oTimer == 5) {
                o->oAction = BLUE_COIN_SWITCH_ACT_IDLE;
            }
            o->oPosY = o->oHomeY - 20.f * (5 - o->oTimer);
            obj_load_collision_model(o);
        } break;
    }
}

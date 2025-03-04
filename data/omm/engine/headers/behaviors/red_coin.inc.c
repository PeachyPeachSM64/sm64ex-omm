#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

//
// Red coin star
//

static void bhv_hidden_red_coin_star_loop_common(bool isBowserCourse) {
    switch (o->oAction) {

        // Init
        case 0: {
            if (!OMM_GAME_IS_SM64 || gCurrCourseNum != COURSE_JRB) {
                spawn_object(o, MODEL_TRANSPARENT_STAR, bhvRedCoinStarMarker);
            }
            s32 numRedCoins = count_objects_with_behavior(bhvRedCoin);
            if (numRedCoins == 0) {
                struct Object *star = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStar, o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
                star->oBehParams = o->oBehParams;
                obj_mark_for_deletion(o);
            }
            if (gOmmArea->redCoins == 0) {
                gOmmArea->redCoins = numRedCoins;
            }
            if (gOmmArea->redCoinStarIndex == -1) {
                gOmmArea->redCoinStarIndex = (o->oBehParams >> 24);
            }
            gRedCoinsCollected = gOmmArea->redCoins - numRedCoins;
            o->oAction = 1;
        } break;

        // Update
        case 1: {
            if (gRedCoinsCollected >= gOmmArea->redCoins) {
                o->oAction = 2;
            }
        } break;

        // Spawn star
        case 2: {
            if (o->oTimer > 2) {
                if (isBowserCourse) {
                    spawn_no_exit_star(o->oPosX, o->oPosY, o->oPosZ);
                } else {
                    spawn_red_coin_cutscene_star(o->oPosX, o->oPosY, o->oPosZ);
                }
                spawn_mist_particles();
                obj_mark_for_deletion(o);
            }
        } break;
    }
}

void bhv_hidden_red_coin_star_init_(void) {
}

void bhv_hidden_red_coin_star_loop_(void) {
    bhv_hidden_red_coin_star_loop_common(false);
}

void bhv_bowser_course_red_coin_star_loop(void) {
    bhv_hidden_red_coin_star_loop_common(true);
}

//
// Red coin
//

static struct ObjectHitbox sRedCoinHitbox = {
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 100,
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_red_coin_init(void) {
    obj_set_hitbox(o, &sRedCoinHitbox);
}

void bhv_red_coin_loop(void) {
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (obj_get_first_with_behavior(bhvHiddenRedCoinStar) || obj_get_first_with_behavior(bhvBowserCourseRedCoinStar)) {
            gRedCoinsCollected++;
            s32 maxVariations = min_s(7, gOmmArea->redCoins - 1);
            s32 soundVariation = clamp_s(maxVariations - (gOmmArea->redCoins - gRedCoinsCollected), 0, maxVariations);
            omm_obj_spawn_orange_number(gMarioObject,
                gRedCoinsCollected, 80,
                SOUND_MENU_COLLECT_RED_COIN, soundVariation,
                omm_geo_number
            );
        }
        coin_collected();
        o->oInteractStatus = 0;
    }
}

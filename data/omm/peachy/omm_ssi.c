#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

bool omm_ssi_star(struct MarioState *m, struct Object *o) {
    if (o->behavior == omm_bhv_sparkly_star) {
        if (m->action != ACT_OMM_POSSESSION) {
            omm_ssd_collect_star(gOmmSSM, omm_ssd_get_star_index(gOmmSSM, gCurrLevelNum, gCurrAreaIndex));
            mario_stop_riding_and_holding(m);
            update_mario_sound_and_camera(m);
            play_sound(SOUND_MENU_STAR_SOUND, m->marioObj->oCameraToObject);
            spawn_object(o, MODEL_NONE, bhvStarKeyCollectionPuffSpawner);
            omm_ssc_state_set(OMM_SSC_INVALID, 0);
            omm_ssc_data_reset();
            omm_mario_set_action(m, ACT_OMM_SPARKLY_STAR_DANCE, (m->prevAction & ACT_FLAG_METAL_WATER) || ((m->prevAction & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED), 0);
            obj_mark_for_deletion(o);
        }
        return true;
    }
    return false;
}

void omm_ssi_grand_star(struct MarioState *m, struct Object *o) {
#if OMM_GAME_IS_SM64
    if (o->behavior == bhvGrandStar && (o->oInteractionSubtype & INT_SUBTYPE_GRAND_STAR)) {

        // Sparkly Grand Star
        // Collecting it unlocks the next mode
        // Triggers the secret Peach ending if Bowser 4 is defeated for the first time
        if (OMM_SSF_IS_GRAND_STAR) {
            gOmmSSE = (!omm_player_is_unlocked(OMM_PLAYER_PEACH) ? OMM_SSE_PEACH : OMM_SSE_REGULAR);
            omm_ssd_collect_grand_star(gOmmSSM);
            return;
        }

        // Regular Grand Star
        // Triggers the bad ending if Bowser 3 is defeated for the first time after completing
        // a save file (120 stars + regular Bowser 3 flag), or if the Sparkly Stars mode
        // is enabled and Peach is not unlocked yet
        if (m->numStars >= 120 && (!omm_ssd_is_unlocked(OMM_SSM_NORMAL) || (OMM_SSM_IS_ENABLED && !omm_player_is_unlocked(OMM_PLAYER_PEACH)))) {
            gOmmSSE = OMM_SSE_BAD;
            omm_ssd_unlock_mode(OMM_SSM_NORMAL);
            return;
        }
    }
#else
    OMM_UNUSED(m);
    OMM_UNUSED(o);
#endif
}

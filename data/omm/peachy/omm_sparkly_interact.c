#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

bool omm_sparkly_interact_star(struct MarioState *m, struct Object *o) {
    if (o->behavior == bhvOmmSparklyStar) {
        if (!omm_mario_is_capture(m)) {
            omm_sparkly_collect_star(gOmmSparklyMode, omm_sparkly_get_star_index(gOmmSparklyMode, gCurrLevelNum, gCurrAreaIndex));
            mario_stop_riding_and_holding(m);
            update_mario_sound_and_camera(m);
            SFX(SOUND_MENU_STAR_SOUND);
            spawn_object(o, MODEL_NONE, bhvStarKeyCollectionPuffSpawner);
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
            omm_sparkly_context_reset_data();
            omm_mario_set_action(m, ACT_OMM_SPARKLY_STAR_DANCE, (m->prevAction & ACT_FLAG_METAL_WATER) || ((m->prevAction & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED), 0);
            obj_mark_for_deletion(o);
            omm_stats_increase(sparklyStarsCollected, 1);
        }
        return true;
    }
    return false;
}

void omm_sparkly_interact_grand_star(struct MarioState *m, struct Object *o) {
#if OMM_GAME_IS_SM64
    if (o->oInteractionSubtype & INT_SUBTYPE_GRAND_STAR) {

        // Sparkly Grand Star
        // Collecting it unlocks the next mode
        // Triggers the secret Peach ending if Bowser 4 is defeated for the first time
        if (gOmmSparkly->grandStar) {
            gOmmSparklyEnding = (!OMM_REWARD_IS_PLAYABLE_PEACH_UNLOCKED ? OMM_SPARKLY_ENDING_PEACH : OMM_SPARKLY_ENDING_REGULAR);
            if (!omm_sparkly_is_completed(gOmmSparklyMode)) {
                gOmmSparklyStarsCompletionReward = gOmmSparklyMode;
            }
            omm_sparkly_collect_grand_star(gOmmSparklyMode);
            omm_stats_increase(sparklyStarsCollected, 1);
            return;
        }

        // Regular Grand Star
        // Triggers the bad ending if Bowser 3 is defeated for the first time after completing a permanent save file
        // (120 stars + regular Bowser 3 flag), or if the Sparkly Stars mode is enabled and Peach is not unlocked yet
        if (m->numStars >= 120 && gCurrSaveFileNum <= NUM_SAVE_FILES && (!OMM_REWARD_IS_SPARKLY_STARS_UNLOCKED || (OMM_SPARKLY_MODE_IS_ENABLED && !OMM_REWARD_IS_PLAYABLE_PEACH_UNLOCKED))) {
            gOmmSparklyEnding = OMM_SPARKLY_ENDING_BAD;
            omm_sparkly_unlock_mode(OMM_SPARKLY_MODE_NORMAL);
            return;
        }
    }
#endif
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
static s32 sOmmSparklyBowser4WarpTimer = 0;

bool omm_sparkly_check_bowser_4(struct MarioState *m) {
#if OMM_GAME_IS_SM64
    sOmmSparklyBowser4WarpTimer--;

    // Unlock Bowser 4
    // Display the "unlocked Bowser 4" message if the player has met the requirements to unlock the final fight (Vanilla only)
    if (!gOmmSparkly->gamePaused &&
        !gOmmSparkly->transition &&
         omm_mario_is_ready_for_dialog(m) &&
        !omm_sparkly_is_bowser_4_unlocked(gOmmSparklyMode) &&
        !omm_sparkly_is_grand_star_collected(gOmmSparklyMode) &&
         omm_sparkly_get_collected_count(gOmmSparklyMode) == omm_sparkly_get_bowser_4_index(gOmmSparklyMode)
    ) {
        omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, OMM_DIALOG_SPARKLY_BOWSER_4_UNLOCKED, 0);
        omm_sparkly_unlock_bowser_4(gOmmSparklyMode);
        audio_play_puzzle_jingle();
        return true;
    }

    // During Bowser 4 fight
    if (omm_sparkly_is_bowser_4_battle()) {

        // Rules: No cap power-up, no Vibe
        // Collecting all Sparkly Stars of the current difficulty allows the player to bypass the rules
        if (!omm_sparkly_is_completed(gOmmSparklyMode)) {
            m->capTimer = min_s(m->capTimer, 1);
            omm_peach_vibe_deactivate(m);
        }

        // Change the grand star to a Sparkly Grand Star
        struct Object *grandStar = obj_get_first_with_behavior(bhvGrandStar);
        if (grandStar) {
            grandStar->oGraphNode = geo_layout_to_graph_node(NULL, OMM_SPARKLY_STAR_GEO_OPAQUE[gOmmSparklyMode]);
            gOmmSparkly->grandStar = true;
            return true;
        }

        // Invalidate state to skip useless checks
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
        return false;
    }
    
    // Transition to Bowser 4
    if (sOmmSparklyBowser4WarpTimer > 0) {
        return true;
    }
    
    // Warping to Bowser 4
    if (sOmmSparklyBowser4WarpTimer == 0) {
        clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        if (OMM_SPARKLY_MODE_IS_ENABLED) {
            initiate_warp(LEVEL_GROUNDS, 2, 0x0A, 0);
        } else {
            omm_return_to_castle(false, true);
        }
        return true;
    }
    
    // Initiating warp to Bowser 4
    if (omm_sparkly_is_bowser_4_unlocked(gOmmSparklyMode) && !gOmmSparkly->gamePaused && gCurrLevelNum == LEVEL_GROUNDS && gCurrAreaIndex == 1) {
        static const Vec3f sBowser4WarpCenter = { 0, 2500, -2000 };
        static const Vec3f sBowser4WarpHitbox = { 250, 300, 50 };

        // Spawn sparkles from the window
        struct Object *sparkle = obj_spawn_from_geo(m->marioObj, OMM_SPARKLY_SPARKLE_GEO[gOmmSparklyMode], bhvOmmSparklyStarSparkle);
        sparkle->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        sparkle->oPosX = sBowser4WarpCenter[0] + sBowser4WarpHitbox[0] * ((2.f * random_float()) - 1.f);
        sparkle->oPosY = sBowser4WarpCenter[1] + sBowser4WarpHitbox[1] * ((2.f * random_float()) - 1.f);
        sparkle->oPosZ = sBowser4WarpCenter[2];
        sparkle->oVelX = 0;
        sparkle->oVelY = 0;
        sparkle->oVelZ = 20.f * random_float();
        sparkle->oAnimState = (random_u16() & 1);
        sparkle->oSparklyStarMode = gOmmSparklyMode;
        obj_scale_random(sparkle, 1.f, 0.5f);

        // Trigger the warp to Bowser 4 if Mario touches the window
        if (abs_f(m->pos[0] - sBowser4WarpCenter[0]) < sBowser4WarpHitbox[0] &&
            abs_f(m->pos[1] - sBowser4WarpCenter[1]) < sBowser4WarpHitbox[1] &&
            abs_f(m->pos[2] - sBowser4WarpCenter[2]) < sBowser4WarpHitbox[2] && m->vel[2] < 0.f) {
            sOmmSparklyBowser4WarpTimer = 40;
            omm_cappy_unload();
            omm_mario_set_action(m, ACT_OMM_WARPING, 0, 0xFFFF);
            play_sound(SOUND_MENU_ENTER_HOLE, gGlobalSoundArgs);
            play_transition(WARP_TRANSITION_FADE_INTO_CIRCLE, 20, 0, 0, 0);
            set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
            m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
            return true;
        }
    }
#else
    OMM_UNUSED(m);
#endif
    return false;
}

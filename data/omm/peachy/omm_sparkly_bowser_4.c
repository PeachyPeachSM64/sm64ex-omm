#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

bool omm_sparkly_check_bowser_4(struct MarioState *m) {
#if OMM_GAME_IS_SM64

    // Unlock Bowser 4
    // Display the "unlocked Bowser 4" message if the player has met the requirements to unlock the final fight (Vanilla only)
    if (!gOmmSparkly->gamePaused &&
        !gOmmSparkly->transition &&
         omm_mario_is_ready_for_dialog(m) &&
        !omm_sparkly_is_bowser_4_unlocked(gOmmSparklyMode) &&
        !omm_sparkly_is_grand_star_collected(gOmmSparklyMode) &&
         omm_sparkly_get_collected_count(gOmmSparklyMode) == omm_sparkly_get_bowser_4_index(gOmmSparklyMode)
    ) {
        omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, OMM_DIALOG_SPARKLY_BOWSER_4_UNLOCKED(gOmmSparklyMode), 0);
        omm_sparkly_unlock_bowser_4(gOmmSparklyMode);
        audio_play_puzzle_jingle();
        return true;
    }

    // During Bowser 4 fight
    if (omm_sparkly_is_bowser_4_battle()) {

        // Bowser rules
        // Collecting all Sparkly Stars of the current difficulty allows the player to bypass the rules
        if (!OMM_SPARKLY_BYPASS_BOWSER_RULES) {

            // No cap
            omm_mario_unset_cap(m);
            gOmmAllow->capModifier = false;

            // No capture other than flaming bob-ombs
            struct Object *capture = gOmmCapture;
            if (omm_mario_is_capture(m) && omm_capture_get_type(capture) != OMM_CAPTURE_FLAMING_BOBOMB) {
                omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
                obj_mark_for_deletion(capture);
            }
            gOmmAllow->captures = false;
            gOmmAllow->yoshiSummon = false;

            // No Vibe
            omm_peach_vibe_deactivate(m);
            gOmmAllow->vibes = false;
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

    // Warping to Bowser 4
    if (m->action == ACT_OMM_WARPING) {
        return true;
    }

    // Initiating warp to Bowser 4
    struct Object *warp = obj_get_first_with_behavior_and_field_s32(bhvOmmWallWarp, _FIELD(oWallWarpKind), 1);
    if (warp) {
        warp->oWallWarpSparkleGeo = OMM_SPARKLY_SPARKLE_GEO[gOmmSparklyMode];
    } else {
        omm_obj_spawn_wall_warp(
            m->marioObj, 1, 0, 2550, -2040, 300, 350, 0x0000,
            OMM_SPARKLY_BOWSER_4_LEVEL, OMM_SPARKLY_BOWSER_4_AREA, 0x0A, 30,
            WARP_TRANSITION_FADE_INTO_CIRCLE, 20, 0x000000,
            1.f, 1.f, 1.f, NULL, OMM_SPARKLY_SPARKLE_GEO[gOmmSparklyMode]
        );
    }
#endif
    return false;
}

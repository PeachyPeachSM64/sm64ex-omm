
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

bool omm_sparkly_check_requirements(struct MarioState *m) {
#if OMM_GAME_IS_SM64
    gAcousticReachPerLevel[OMM_SPARKLY_BOWSER_4_LEVEL] = 25000 + 35000 * (gCurrAreaIndex == OMM_SPARKLY_BOWSER_4_AREA);
    omm_sparkly_ending_cutscene();
#endif

    // Check Sparkly Stars mode option
    omm_opt_select_available(
        OMM_SPARKLY_STARS_MODE,
        OMM_SPARKLY_MODE_DISABLED,
        OMM_SPARKLY_MODE_COUNT,
        omm_sparkly_is_selectible(OMM_SPARKLY_STARS_MODE)
    );

    // Check Sparkly Stars reward option
    omm_opt_select_available(
        OMM_SPARKLY_STARS_COMPLETION_REWARD,
        OMM_SPARKLY_MODE_DISABLED,
        OMM_SPARKLY_MODE_COUNT,
        omm_sparkly_is_completed(OMM_SPARKLY_STARS_COMPLETION_REWARD)
    );

    // Reset the Sparkly Stars mode in the main menu, and return
    if (omm_is_main_menu()) {
        omm_sparkly_disable();
        gOmmSparklyEnding = OMM_SPARKLY_ENDING_REGULAR;
        gOmmSparkly->starBlock = false;
        gOmmSparkly->grandStar = false;
        gOmmSparkly->cheatDetected = false;
        return true;
    }

    // If Bowser 3 was never defeated, disable the Sparkly Stars mode, sets the option to 0 and return
    if (!OMM_REWARD_IS_SPARKLY_STARS_UNLOCKED) {
        omm_sparkly_set_mode(OMM_SPARKLY_MODE_DISABLED);
        omm_sparkly_disable();
        return true;
    }

    // If a save file has not all stars collected, disable the Sparkly Stars mode and return
    if (m->numStars < omm_stars_get_total_star_count(OMM_GAME_MODE)) {
        omm_sparkly_disable();
        return true;
    }

    // Disable the Sparkly Stars mode during Yoshi mode
    if (gOmmGlobals->yoshiMode) {
        omm_sparkly_disable();
        return true;
    }

    // Disable the Sparkly Stars mode if the option is set to 0 (this case doesn't need a level change)
    if (OMM_SPARKLY_STARS_MODE == OMM_SPARKLY_MODE_DISABLED) {
        if (OMM_SPARKLY_MODE_IS_ENABLED && gOmmSparklyContext->timerStarted) {
            gHudDisplay.flags &= ~HUD_DISPLAY_FLAG_TIMER;
        }
        omm_sparkly_disable();
    }

    // Disable the Sparkly Stars mode during the ending cutscenes and return
    if (m->action == ACT_END_WAVING_CUTSCENE ||
        m->action == ACT_END_PEACH_CUTSCENE ||
        m->action == ACT_CREDITS_CUTSCENE) {
        omm_sparkly_disable();
        return true;
    }

    // Stop the update here during the jumbo star cutscene
    if (m->action == ACT_JUMBO_STAR_CUTSCENE) {
        return true;
    }

    // Stop if the object lists or Mario's object are not loaded yet
    if (OMM_UNLIKELY(!gObjectLists || !gMarioObject || !m->marioObj)) {
        return true;
    }

    // Immediately return during regular Bowser fights
    if (OMM_LEVEL_IS_BOWSER_FIGHT(gCurrLevelNum)) {
        return true;
    }

    return false;
}

bool omm_sparkly_check_game_state(struct MarioState *m) {

    // Spawn Sparkly Stars blocks for available modes
    // Break it to enable the corresponding Sparkly Stars mode
    if (gCurrLevelNum == OMM_SPARKLY_BLOCK_LEVEL && gCurrAreaIndex == OMM_SPARKLY_BLOCK_AREA) {
        for (s32 i = -1, sparklyMode = OMM_SPARKLY_MODE_NORMAL; sparklyMode != OMM_SPARKLY_MODE_COUNT; ++sparklyMode) {
            if (omm_sparkly_is_available(sparklyMode) && (++i != -1) && omm_sparkly_is_unlocked(sparklyMode) && !omm_sparkly_is_selected(sparklyMode)) {
                struct Object *block = obj_get_first_with_behavior(OMM_SPARKLY_BLOCK_BHV[sparklyMode]);

                // If the block is not already spawned, spawn it
                if (!block) {
                    f32 d = (f32) i - (OMM_SPARKLY_BLOCK_COUNT - 1) / 2.f;
                    f32 x = OMM_SPARKLY_BLOCK_X + 360.f * d * sins(OMM_SPARKLY_BLOCK_ANGLE);
                    f32 y = OMM_SPARKLY_BLOCK_Y + 300.f;
                    f32 z = OMM_SPARKLY_BLOCK_Z + 360.f * d * coss(OMM_SPARKLY_BLOCK_ANGLE);
                    block = omm_obj_spawn_sparkly_star_block(m->marioObj, sparklyMode, x, y, z);
                }

                // If the block has been hit, change the current Sparkly Star mode
                if (block->oAction == 2) {
                    gOmmSparkly->starBlock = true;
                    obj_deactivate_all_with_behavior(bhvOmmSparklyStar);
                    obj_deactivate_all_with_behavior(bhvOmmSparklyStarHint);
                    omm_sparkly_set_mode(sparklyMode);
                    omm_sparkly_context_reset();
                    omm_sparkly_turn_off_cheats();
                    omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
                    obj_unload_all_with_behavior(bhvOmmYoshi);
                    block->oAction = 3;
                    block->oTimer = 0;
                }
            }
        }
    }

    // Unload everything if not Sparkly Stars mode, then return
    // Also ejects Mario from the Bowser 4 fight
    if (!OMM_SPARKLY_MODE_IS_ENABLED) {
        if (omm_sparkly_is_bowser_4_battle()) {
            omm_return_to_castle(true, false);
        }
        obj_deactivate_all_with_behavior(bhvOmmSparklyStar);
        obj_deactivate_all_with_behavior(bhvOmmSparklyStarHint);
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
        omm_sparkly_context_reset_data();
        gOmmSparklyCheats->currMsg = 0;
        gOmmSparklyCheats->introId = 0;
        gOmmSparklyCheats->messageId = 0;
        gOmmSparklyCheats->endingId = 0;
        gOmmSparkly->starBlock = false;
        gOmmSparkly->cheatDetected = false;
        return true;
    }

    // Update game state
    static s32 sPrevMarioTimer = -1;
    gOmmSparkly->gamePaused = omm_is_game_paused();
    gOmmSparkly->timeStop = (gTimeStopState & TIME_STOP_ENABLED) != 0;
    gOmmSparkly->transition = omm_is_transition_active();
    gOmmSparkly->marioUpdated = ((m->marioObj->oTimer != sPrevMarioTimer) && m->floor && m->action); // Mario does not update if he's out of bounds
    sPrevMarioTimer = m->marioObj->oTimer;
    return false;
}

bool omm_sparkly_check_star_block(struct MarioState *m) {

    // Broken Sparkly Stars block
    // Start the timer if not started
    // Display the Castle grounds hint message if Sparkly Star count is 0 (Vanilla only)
    if (gOmmSparkly->starBlock) {
        if (!omm_sparkly_is_timer_started(gOmmSparklyMode)) {
            omm_sparkly_start_timer(gOmmSparklyMode);
        }
#if OMM_GAME_IS_SM64
        if (gCurrLevelNum == LEVEL_CASTLE_GROUNDS) {
            if (!gOmmSparkly->gamePaused &&
                !gOmmSparkly->transition &&
                 gOmmSparkly->marioUpdated &&
                !omm_sparkly_get_collected_count(gOmmSparklyMode) &&
                 omm_mario_is_ready_for_dialog(m)
            ) {
                struct Object *sign = obj_get_first_with_behavior(bhvOmmSparklyStarHint);
                if (sign) {
                    m->marioObj->oMarioReadingSignDYaw  = (s16) (0x8000 - m->faceAngle[1]);
                    m->marioObj->oMarioReadingSignDPosX = 0;
                    m->marioObj->oMarioReadingSignDPosZ = 0;
                    m->interactObj = sign;
                    m->usedObj = sign;
                    omm_mario_set_action(m, ACT_READING_SIGN, 0, 0);
                    gOmmSparkly->starBlock = false;
                    return true;
                }
            }
        } else
#endif
        gOmmSparkly->starBlock = false;
    }
    return false;
}

bool omm_sparkly_check_anti_cheat(struct MarioState *m) {

    // Anti-cheat message
    // Read the messages and return
    if (gOmmSparklyCheats->currMsg >= 1) {
        if (m->action != ACT_READING_AUTOMATIC_DIALOG) {
            switch (gOmmSparklyCheats->currMsg) {
                case 1: omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, gOmmSparklyCheats->introId, 0); break;
                case 2: omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, gOmmSparklyCheats->messageId, 0); break;
                case 3: omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, gOmmSparklyCheats->endingId, 0); break;
            }
            gOmmSparklyCheats->currMsg = ((gOmmSparklyCheats->currMsg + 1) % 4);
        }
        return true;
    }

    // Anti-cheat trigger
    if (gOmmSparkly->cheatDetected) {
        omm_sparkly_turn_off_cheats();
        if (!omm_mario_is_dead(m) &&
             gCurrCourseNum == COURSE_NONE && // TODO: nebulas: check if this is valid in other games (probably not)
            !omm_sparkly_is_bowser_4_battle() &&
            !gOmmSparkly->gamePaused &&
            !gOmmSparkly->transition &&
             gOmmSparkly->marioUpdated &&
             omm_mario_is_ready_for_dialog(m)
        ) {
            play_sound(SOUND_OBJ_BOWSER_INTRO_LAUGH, gGlobalSoundArgs);
            play_sequence(SEQ_PLAYER_ENV, SEQ_EVENT_KOOPA_MESSAGE, 0);
            gOmmSparklyCheats->currMsg = 1;
            gOmmSparklyCheats->introId = OMM_DIALOG_SPARKLY_ANTI_CHEAT_INTRO;
            gOmmSparklyCheats->messageId = OMM_DIALOG_SPARKLY_ANTI_CHEAT_0 + (random_u16() % 8);
            gOmmSparklyCheats->endingId = OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_0 + (random_u16() % 3);
            gOmmSparkly->cheatDetected = false;
        }
        return true;
    }
    return false;
}

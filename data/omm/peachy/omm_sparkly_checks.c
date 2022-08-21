
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

bool omm_sparkly_check_requirements(struct MarioState *m) {
#if OMM_GAME_IS_SM64
    sAcousticReachPerLevel[LEVEL_GROUNDS] = 25000 + 35000 * (gCurrAreaIndex == 2);
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
        OMM_EXTRAS_SPARKLY_STARS_REWARD,
        OMM_SPARKLY_MODE_DISABLED,
        OMM_SPARKLY_MODE_COUNT,
        omm_sparkly_is_completed(OMM_EXTRAS_SPARKLY_STARS_REWARD)
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
    if (!omm_sparkly_is_unlocked(OMM_SPARKLY_MODE_NORMAL)) {
        omm_sparkly_set_mode(OMM_SPARKLY_MODE_DISABLED);
        omm_sparkly_disable();
        return true;
    }

    // If a save file with less than the minimum number of stars required is selected, disable the Sparkly Stars mode and return
    if (m->numStars < OMM_SPARKLY_REQUIREMENT) {
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
    if (gCurrLevelNum == LEVEL_BOWSER_1 ||
        gCurrLevelNum == LEVEL_BOWSER_2 ||
        gCurrLevelNum == LEVEL_BOWSER_3) {
        return true;
    }

    return false;
}

bool omm_sparkly_check_game_state(struct MarioState *m) {

    // Spawn Sparkly Stars blocks for available modes
    // Break it to enable the corresponding Sparkly Stars mode
    if (gCurrLevelNum == OMM_SPARKLY_BLOCK_LEVEL && gCurrAreaIndex == OMM_SPARKLY_BLOCK_AREA) {
        for (s32 i = -1, mode = OMM_SPARKLY_MODE_NORMAL; mode != OMM_SPARKLY_MODE_COUNT; ++mode) {
            if (omm_sparkly_is_available(mode) && (++i != -1) && omm_sparkly_is_unlocked(mode) && !omm_sparkly_is_selected(mode)) {
                struct Object *block = obj_get_first_with_behavior(OMM_SPARKLY_BLOCK_BHV[mode]);

                // If the block is not already spawned, spawn it
                if (!block) {
                    f32 d = (f32) i - (OMM_SPARKLY_BLOCK_COUNT - 1) / 2.f;
                    f32 x = OMM_SPARKLY_BLOCK_X + 360.f * d * sins(OMM_SPARKLY_BLOCK_ANGLE);
                    f32 y = OMM_SPARKLY_BLOCK_Y + 300.f;
                    f32 z = OMM_SPARKLY_BLOCK_Z + 360.f * d * coss(OMM_SPARKLY_BLOCK_ANGLE);
                    block = omm_spawn_sparkly_star_block(m->marioObj, mode, x, y, z);
                }
                
                // If the block has been hit, change the current Sparkly Star mode
                if (block->oAction == 2) {
                    gOmmSparkly->starBlock = true;
                    obj_deactivate_all_with_behavior(bhvOmmSparklyStar);
                    obj_deactivate_all_with_behavior(bhvOmmSparklyStarHint);
                    omm_sparkly_set_mode(mode);
                    omm_sparkly_context_reset();
                    omm_sparkly_turn_off_cheats();
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
        obj_deactivate_all_with_behavior(bhvOmmSparklyStarSparkle);
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
        omm_sparkly_context_reset_data();
        gOmmSparklyCheats[0] = 0;
        gOmmSparklyCheats[1] = 0;
        gOmmSparklyCheats[2] = 0;
        gOmmSparklyCheats[3] = 0;
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
    gPlayer1Controller->buttonPressed &= ~(gOmmSparkly->cheatDetected * START_BUTTON);
    gPlayer2Controller->buttonPressed &= ~(gOmmSparkly->cheatDetected * START_BUTTON);
    gPlayer3Controller->buttonPressed &= ~(gOmmSparkly->cheatDetected * START_BUTTON);
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
        if (gCurrLevelNum == LEVEL_GROUNDS) {
            if (!gOmmSparkly->gamePaused && !gOmmSparkly->transition && gOmmSparkly->marioUpdated && !omm_sparkly_get_collected_count(gOmmSparklyMode) &&
                omm_mario_is_ready_for_dialog(m)) {
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
#else
        OMM_UNUSED(m);
#endif
        gOmmSparkly->starBlock = false;
    }
    return false;
}

bool omm_sparkly_check_anti_cheat(struct MarioState *m) {

    // Anti-cheat message
    // Read the messages and return
    if (gOmmSparklyCheats[0] >= 1) {
        if (m->action != ACT_READING_AUTOMATIC_DIALOG) {
            omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, gOmmSparklyCheats[gOmmSparklyCheats[0]], 0);
            gOmmSparklyCheats[0] = ((gOmmSparklyCheats[0] + 1) % 4);
        }
        return true;
    }

    // Anti-cheat trigger
    // Kill Mario, disable the Pause menu, eject him from the current level
    // (bypassing the Non-Stop mode) and display a special dialog...
    static bool sDeathState = false;
    if (gOmmSparkly->cheatDetected) {

        // Turn off cheats, disable caps and reduce Mario's health to 0...
        if (!sDeathState) {
            gOmmSparklyContext->cheatDetected = false;
            omm_sparkly_turn_off_cheats();
            m->capTimer = 0;
            m->flags &= ~(MARIO_WING_CAP | MARIO_METAL_CAP | MARIO_VANISH_CAP);
            if (OMM_MOVESET_ODYSSEY) {
                m->health = OMM_HEALTH_ODYSSEY_DEAD;
            } else {
                m->healCounter = 0;
                m->hurtCounter = 30;
            }
        }

        // ...until Mario is in a death state
        if (m->action == ACT_STANDING_DEATH ||
            m->action == ACT_QUICKSAND_DEATH ||
            m->action == ACT_ELECTROCUTION ||
            m->action == ACT_SUFFOCATION ||
            m->action == ACT_DEATH_ON_STOMACH ||
            m->action == ACT_DEATH_ON_BACK ||
            m->action == ACT_EATEN_BY_BUBBA ||
            m->action == ACT_DROWNING ||
            m->action == ACT_WATER_DEATH) {
            sDeathState = true;
        }

        // When warped back to Castle, trigger a random Bowser anti-cheat dialog
        if (!omm_mario_is_dead(m) && gCurrCourseNum == COURSE_NONE && !omm_sparkly_is_bowser_4_battle() &&
            !gOmmSparkly->gamePaused && !gOmmSparkly->transition && gOmmSparkly->marioUpdated &&
            omm_mario_is_ready_for_dialog(m)) {
            play_sound(SOUND_OBJ_BOWSER_INTRO_LAUGH, gGlobalSoundArgs);
            play_sequence(SEQ_PLAYER_ENV, SEQ_EVENT_KOOPA_MESSAGE, 0);
            gOmmSparklyCheats[0] = 1;
            gOmmSparklyCheats[1] = OMM_DIALOG_SPARKLY_ANTI_CHEAT_INTRO;
            gOmmSparklyCheats[2] = OMM_DIALOG_SPARKLY_ANTI_CHEAT_0 + (random_u16() % 8);
            gOmmSparklyCheats[3] = OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_0 + (random_u16() % 3);
            gOmmSparkly->cheatDetected = false;
            sDeathState = false;
        }
        return true;
    }
    sDeathState = false;
    return false;
}

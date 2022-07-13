#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static bool omm_ssu_requirements(struct MarioState *m) {
#if OMM_GAME_IS_SM64
    sAcousticReachPerLevel[LEVEL_GROUNDS] = 25000 + 35000 * (gCurrAreaIndex == 2);
    omm_sse_update_cutscene();
#endif

    // Check Sparkly Stars mode option
    omm_opt_select_available(
        OMM_SPARKLY_STARS_MODE,
        OMM_SSM_DISABLED,
        OMM_SSM_COUNT,
        omm_ssd_is_selectible(OMM_SPARKLY_STARS_MODE)
    );
    
    // Reset the Sparkly Stars mode in the main menu, and return
    if (omm_is_main_menu()) {
        omm_ssd_disable();
        gOmmSSE = OMM_SSE_REGULAR;
        omm_ssf_set(OMM_SSF_STAR_BLOCK, 0);
        omm_ssf_set(OMM_SSF_GRAND_STAR, 0);
        omm_ssf_set(OMM_SSF_CHEAT_DETECTED, 0);
        return true;
    }

    // If Bowser 3 was never defeated, disable the Sparkly Stars mode, sets the option to 0 and return
    if (!omm_ssd_is_unlocked(OMM_SSM_NORMAL)) {
        omm_ssd_set_mode(OMM_SSM_DISABLED);
        omm_ssd_disable();
        return true;
    }

    // If a save file with less than the minimum number of stars required is selected, disable the Sparkly Stars mode and return
    if (m->numStars < OMM_SSX_REQUIRED) {
        omm_ssd_disable();
        return true;
    }

    // Disable the Sparkly Stars mode if the option is set to 0 (this case doesn't need a level change)
    if (OMM_SPARKLY_STARS_MODE == OMM_SSM_DISABLED) {
        if (OMM_SSM_IS_ENABLED && OMM_SSC_IS_TIMER_STARTED) {
            gHudDisplay.flags &= ~HUD_DISPLAY_FLAG_TIMER;
        }
        omm_ssd_disable();
    }

    // Disable the Sparkly Stars mode during the ending cutscenes and return
    if (m->action == ACT_END_WAVING_CUTSCENE ||
        m->action == ACT_END_PEACH_CUTSCENE ||
        m->action == ACT_CREDITS_CUTSCENE) {
        omm_ssd_disable();
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

    // Peach, Non-Stop mode and 1 HP mode are required in Lunatic mode
    // Disable the Sparkly Stars Lunatic mode if one of the conditions is not fulfilled
    if (OMM_SSM_IS_LUNATIC && (!OMM_PLAYER_IS_PEACH || !OMM_STARS_NON_STOP || !g1HPMode)) {
        omm_ssd_set_mode(OMM_SSM_DISABLED);
        omm_ssd_disable();
    }

    return false;
}

static bool omm_ssu_game_state(struct MarioState *m) {

    // Spawn Sparkly Stars blocks for available modes
    // Break it to enable the corresponding Sparkly Stars mode
    if (gCurrLevelNum == OMM_SSX_BLOCK_LEVEL && gCurrAreaIndex == OMM_SSX_BLOCK_AREA) {
        for (s32 i = -1, mode = OMM_SSM_NORMAL; mode != OMM_SSM_COUNT; ++mode) {
            if (omm_ssd_is_available(mode) && (++i != -1) && omm_ssd_is_unlocked(mode) && !omm_ssd_is_selected(mode)) {
                struct Object *block = obj_get_first_with_behavior(OMM_SSX_BLOCK_BHV[mode]);

                // If the block is not already spawned, spawn it
                if (!block) {
                    f32 d = (f32) i - (OMM_SSX_BLOCK_COUNT - 1) / 2.f;
                    f32 x = OMM_SSX_BLOCK_X + 360.f * d * sins(OMM_SSX_BLOCK_ANGLE);
                    f32 y = OMM_SSX_BLOCK_Y + 300.f;
                    f32 z = OMM_SSX_BLOCK_Z + 360.f * d * coss(OMM_SSX_BLOCK_ANGLE);
                    block = omm_spawn_sparkly_star_block(m->marioObj, mode, x, y, z);
                }
                
                // If the block has been hit, change the current Sparkly Star mode
                if (block->oAction == 2) {
                    omm_ssf_set(OMM_SSF_STAR_BLOCK, 1);
                    obj_deactivate_all_with_behavior(omm_bhv_sparkly_star);
                    obj_deactivate_all_with_behavior(omm_bhv_sparkly_star_hint);
                    omm_ssd_set_mode(mode);
                    omm_ssc_reset();
                    omm_ssd_turn_off_cheats();
                    block->oAction = 3;
                    block->oTimer = 0;

                    // Lunatic mode: Select Peach and enable Non-Stop mode and 1 HP mode
                    if (mode == OMM_SSM_LUNATIC) {
                        g1HPMode = true;
                        gOmmStarsMode = 1;
                        if (!OMM_PLAYER_IS_PEACH) {
                            omm_player_select(OMM_PLAYER_PEACH);
                            obj_spawn_white_puff(m->marioObj, -1);
                            obj_spawn_white_puff(m->marioObj, -1);
                            obj_spawn_white_puff(m->marioObj, -1);
                        }
                    }
                }
            }
        }
    }

    // Unload everything if not Sparkly Stars mode, then return
    // Also ejects Mario from the Bowser 4 fight
    if (!OMM_SSM_IS_ENABLED) {
        if (omm_ssd_is_bowser_4()) {
            omm_return_to_castle(true, false);
        }
        obj_deactivate_all_with_behavior(omm_bhv_sparkly_star);
        obj_deactivate_all_with_behavior(omm_bhv_sparkly_star_hint);
        obj_deactivate_all_with_behavior(omm_bhv_sparkly_star_sparkle);
        omm_ssc_state_set(OMM_SSC_INVALID, 0);
        omm_ssc_data_reset();
        omm_ssf_set(OMM_SSF_STAR_BLOCK, 0);
        omm_ssf_set(OMM_SSF_CHEAT_DETECTED, 0);
        gOmmSSH[0] = 0;
        gOmmSSH[1] = 0;
        gOmmSSH[2] = 0;
        gOmmSSH[3] = 0;
        return true;
    }

    // Update game state
    static s32 sPrevMarioTimer = -1;
    omm_ssf_set(OMM_SSF_GAME_PAUSED, omm_is_game_paused());
    omm_ssf_set(OMM_SSF_TIME_STOP, (gTimeStopState & TIME_STOP_ENABLED) != 0);
    omm_ssf_set(OMM_SSF_TRANSITION, omm_is_transition_active());
    omm_ssf_set(OMM_SSF_MARIO_UPDATED, (m->marioObj->oTimer != sPrevMarioTimer) && (m->floor != NULL) && (m->action != 0)); // Mario does not update if he's out of bounds
    sPrevMarioTimer = m->marioObj->oTimer;
    gPlayer1Controller->buttonPressed &= ~(OMM_SSF_IS_CHEAT_DETECTED * START_BUTTON);
    gPlayer2Controller->buttonPressed &= ~(OMM_SSF_IS_CHEAT_DETECTED * START_BUTTON);
    gPlayer3Controller->buttonPressed &= ~(OMM_SSF_IS_CHEAT_DETECTED * START_BUTTON);
    return false;
}

static bool omm_ssu_star_block(struct MarioState *m) {

    // Broken Sparkly Stars block
    // Start the timer if not started
    // Display the Castle grounds hint message if Sparkly Star count is 0 (Vanilla only)
    if (OMM_SSF_IS_STAR_BLOCK) {
        if (!omm_ssd_is_timer_started(gOmmSSM)) {
            omm_ssd_start_timer(gOmmSSM);
        }
#if OMM_GAME_IS_SM64
        if (gCurrLevelNum == LEVEL_GROUNDS) {
            if (!OMM_SSF_IS_GAME_PAUSED && !OMM_SSF_IS_TRANSITION && OMM_SSF_IS_MARIO_UPDATED && !omm_ssd_get_star_count(gOmmSSM) &&
                (m->action == ACT_IDLE || m->action == ACT_PANTING || m->action == ACT_WALKING)) {
                struct Object *sign = obj_get_first_with_behavior(omm_bhv_sparkly_star_hint);
                if (sign) {
                    m->marioObj->oMarioReadingSignDYaw  = (s16) (0x8000 - m->faceAngle[1]);
                    m->marioObj->oMarioReadingSignDPosX = 0;
                    m->marioObj->oMarioReadingSignDPosZ = 0;
                    m->interactObj = sign;
                    m->usedObj = sign;
                    omm_mario_set_action(m, ACT_READING_SIGN, 0, 0);
                    omm_ssf_set(OMM_SSF_STAR_BLOCK, 0);
                    return true;
                }
            }
        } else
#else
        OMM_UNUSED(m);
#endif
        omm_ssf_set(OMM_SSF_STAR_BLOCK, 0);
    }
    return false;
}

static bool omm_ssu_anti_cheat(struct MarioState *m) {

    // Anti-cheat message
    // Read the messages and return
    if (gOmmSSH[0] >= 1) {
        if (m->action != ACT_READING_AUTOMATIC_DIALOG) {
            omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, gOmmSSH[gOmmSSH[0]], 0);
            gOmmSSH[0] = ((gOmmSSH[0] + 1) % 4);
        }
        return true;
    }

    // Anti-cheat trigger
    // Kill Mario, disable the Pause menu, eject him from the current level
    // (bypassing the Non-Stop mode) and display a special dialog...
    static bool sDeathState = false;
    if (OMM_SSF_IS_CHEAT_DETECTED) {

        // Turn off cheats, disable caps and reduce Mario's health to 0...
        if (!sDeathState) {
            omm_ssc_clr(OMM_SSC_F_CHEAT_DETECTED);
            omm_ssd_turn_off_cheats();
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
        if (m->health > OMM_HEALTH_DEAD && gCurrCourseNum == COURSE_NONE && !omm_ssd_is_bowser_4() &&
            !OMM_SSF_IS_GAME_PAUSED && !OMM_SSF_IS_TRANSITION && OMM_SSF_IS_MARIO_UPDATED &&
            (m->action == ACT_IDLE || m->action == ACT_PANTING || m->action == ACT_WALKING)) {
            play_sound(SOUND_OBJ_BOWSER_INTRO_LAUGH, gGlobalSoundArgs);
            play_sequence(SEQ_PLAYER_ENV, SEQ_EVENT_KOOPA_MESSAGE, 0);
            gOmmSSH[0] = 1;
            gOmmSSH[1] = OMM_DIALOG_SPARKLY_ANTI_CHEAT_INTRO;
            gOmmSSH[2] = OMM_DIALOG_SPARKLY_ANTI_CHEAT_0 + (random_u16() % 8);
            gOmmSSH[3] = OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_0 + (random_u16() % 3);
            omm_ssf_set(OMM_SSF_CHEAT_DETECTED, 0);
            sDeathState = false;
        }
        return true;
    }
    sDeathState = false;
    return false;
}

static bool omm_ssu_bowser_4(struct MarioState *m) {
#if OMM_GAME_IS_SM64
    static s32 sBowser4WarpTimer = 0;
    sBowser4WarpTimer--;

    // Unlock Bowser 4
    // Display the "unlocked Bowser 4" message if the player has met the requirements to unlock the final fight (Vanilla only)
    if (!OMM_SSF_IS_GAME_PAUSED && !OMM_SSF_IS_TRANSITION && omm_ssd_get_star_count(gOmmSSM) == 29 &&
        !omm_ssd_is_bowser_4_unlocked(gOmmSSM) && !omm_ssd_is_grand_star_collected(gOmmSSM) &&
        (m->action == ACT_IDLE || m->action == ACT_PANTING || m->action == ACT_WALKING)) {
        omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, OMM_DIALOG_SPARKLY_BOWSER_4_UNLOCKED, 0);
        omm_ssd_unlock_bowser_4(gOmmSSM);
        audio_play_puzzle_jingle();
        return true;
    }

    // Bowser 4 fight
    if (omm_ssd_is_bowser_4()) {

        // Change the grand star to a Sparkly Grand Star
        struct Object *grandStar = obj_get_first_with_behavior(bhvGrandStar);
        if (grandStar) {
            grandStar->oGraphNode = geo_layout_to_graph_node(NULL, OMM_SSX_STAR_GEO_OPAQUE[gOmmSSM]);
            omm_ssf_set(OMM_SSF_GRAND_STAR, 1);
            return true;
        }
        return false;
    }
    
    // Transition to Bowser 4
    if (sBowser4WarpTimer > 0) {
        return true;
    }
    
    // Warping to Bowser 4
    if (sBowser4WarpTimer == 0) {
        clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        if (OMM_SSM_IS_ENABLED) {
            initiate_warp(LEVEL_GROUNDS, 2, 0x0A, 0);
        } else {
            omm_return_to_castle(false, true);
        }
        return true;
    }
    
    // Initiating warp to Bowser 4
    if (omm_ssd_is_bowser_4_unlocked(gOmmSSM) && !OMM_SSF_IS_GAME_PAUSED && gCurrLevelNum == LEVEL_GROUNDS && gCurrAreaIndex == 1) {
        static const Vec3f sBowser4WarpCenter = { 0, 2500, -2000 };
        static const Vec3f sBowser4WarpHitbox = { 250, 300, 50 };

        // Spawn sparkles from the window
        struct Object *sparkle = obj_spawn_from_geo(m->marioObj, OMM_SSX_SPARKLE_GEO[gOmmSSM], omm_bhv_sparkly_star_sparkle);
        sparkle->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        sparkle->oPosX = sBowser4WarpCenter[0] + sBowser4WarpHitbox[0] * ((2.f * random_float()) - 1.f);
        sparkle->oPosY = sBowser4WarpCenter[1] + sBowser4WarpHitbox[1] * ((2.f * random_float()) - 1.f);
        sparkle->oPosZ = sBowser4WarpCenter[2];
        sparkle->oVelX = 0;
        sparkle->oVelY = 0;
        sparkle->oVelZ = 20.f * random_float();
        sparkle->oAnimState = (random_u16() & 1);
        sparkle->oSparklyStarMode = gOmmSSM;
        obj_scale_random(sparkle, 1.f, 0.5f);

        // Trigger the warp to Bowser 4 if Mario touches the window
        if (abs_f(m->pos[0] - sBowser4WarpCenter[0]) < sBowser4WarpHitbox[0] &&
            abs_f(m->pos[1] - sBowser4WarpCenter[1]) < sBowser4WarpHitbox[1] &&
            abs_f(m->pos[2] - sBowser4WarpCenter[2]) < sBowser4WarpHitbox[2] && m->vel[2] < 0.f) {
            sBowser4WarpTimer = 40;
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

OMM_ROUTINE_UPDATE(omm_ssu_loop) {
    struct MarioState *m = gMarioState;
    
    // Check requirements
    if (omm_ssu_requirements(m)) {
        return;
    }
    
    // Update game state
    if (omm_ssu_game_state(m)) {
        return;
    }

    // Update star blocks
    if (omm_ssu_star_block(m)) {
        return;
    }

    // Update the anti-cheat
    if (omm_ssu_anti_cheat(m)) {
        return;
    }

    // Update Bowser 4
    if (omm_ssu_bowser_4(m)) {
        return;
    }
    
    // Update context
    omm_ssc_update(m);
    omm_ssc_spawn_star(m);
    omm_ssc_spawn_sign(m);
    omm_ssc_set(OMM_SSC_F_INITED, 1);
}

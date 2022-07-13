#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Data
//

static struct OmmWarpState {
    s32 level;
    s32 area;
    s32 act;
    bool exit;
    bool active;
} sOmmWarpState = { 0 };

//
// Warps
//

bool omm_warp_to_level(s32 level, s32 area, s32 act) {
    if (omm_level_can_warp(level) && omm_level_get_entry_warp(level, area)) {
        sOmmWarpState.level = level;
        sOmmWarpState.area = area;
        sOmmWarpState.act = act;
        sOmmWarpState.exit = false;
        sOmmWarpState.active = true;
        return true;
    }
    return false;
}

bool omm_restart_level() {
#if OMM_GAME_IS_SM74
    return omm_restart_area();
#else
    return omm_warp_to_level(gCurrLevelNum, 1, gCurrActNum);
#endif
}

bool omm_restart_area() {
    for (s32 i = gCurrAreaIndex; i != 0; --i) {
        if (omm_warp_to_level(gCurrLevelNum, i, gCurrActNum)) {
            return true;
        }
    }
    return false;
}

bool omm_exit_level() {
    if (omm_level_can_warp(gCurrLevelNum)) {

        // Close the pause menu if it was open
        if (optmenu_open) optmenu_toggle();
        level_set_transition(0, NULL);
        gDialogBoxState = 0;
        gMenuMode = -1;

        // Cancel out every music/sound/sequence
        music_unlower_volume(SEQ_PLAYER_LEVEL, 0);
        for (u16 seqid = 0; seqid != SEQ_COUNT; ++seqid) {
            stop_background_music(seqid);
        }
        audio_play_shell_music();
        audio_stop_shell_music();
        audio_stop_cap_music();
        func_80321080(0);
        fadeout_music(0);
        fadeout_level_music(0);

        // Play a fade-out transition, and trigger a "fake" warp
        play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 16, 0, 0, 0);
        level_set_transition(30, 0);
        warp_special(0);
        sWarpDest.type = 0;
        sOmmWarpState.level = gCurrLevelNum;
        sOmmWarpState.area = gCurrAreaIndex;
        sOmmWarpState.act = gCurrActNum;
        sOmmWarpState.exit = true;
        sOmmWarpState.active = true;
        return true;
    }
    return false;
}

bool omm_return_to_castle(bool fadeOut, bool force) {
    if (force || (!omm_is_game_paused() && !omm_is_transition_active() && !omm_is_warping())) {
        initiate_warp(OMM_LEVEL_RETURN_TO_CASTLE);
        if (fadeOut) {
            fade_into_special_warp(0, 0);
        } else {
            level_set_transition(0, NULL);
            warp_special(0);
        }
        gSavedCourseNum = COURSE_NONE;
        return true;
    }
    return false;
}

bool omm_is_warping() {
    return sOmmWarpState.active;
}

//
// Update
//

static void omm_exit_level_find_position(s16 *x0, s16 *y0, s16 *z0, s16 yaw, f32 dist) {
    for (f32 d = dist; d > 0.f; d -= 10.f) {
        f32 x = (f32) *x0 + d * sins(yaw + 0x8000);
        f32 z = (f32) *z0 + d * coss(yaw + 0x8000);
        for (f32 dy = 0.f; dy <= 5000.f; dy += 100.f) {
            f32 y = (f32) *y0 + dy;
            struct Surface *floor;
            f32 floorY = find_floor(x, y, z, &floor);
            if (floor &&
                floor->type != SURFACE_WARP &&
                floor->type != SURFACE_BURNING &&
                floor->type != SURFACE_DEATH_PLANE &&
                floor->type != SURFACE_VERTICAL_WIND &&
                floor->type != SURFACE_DEEP_QUICKSAND &&
                floor->type != SURFACE_INSTANT_QUICKSAND &&
                floor->type != SURFACE_INSTANT_MOVING_QUICKSAND) {
                *x0 = x;
                *y0 = floorY;
                *z0 = z;
                return;
            }
        }
    }
}

void *omm_update_warp(void *cmd, bool inited) {
    static s32 sExitTimer = 0;
    static s32 sTargetArea = -1;
    static s16 *sTargetWarp = NULL;
    if (sOmmWarpState.active) {

        // Exit level
        if (sOmmWarpState.exit) {

            // Phase 0 - Wait for the transition to end
            if (sTargetArea == -1 && omm_is_transition_active()) {
                sExitTimer = 14;
                return NULL;
            }

            // Phase 1 - Clear the previous level and set up the new level
            if (sTargetArea == -1) {
                
                // Wait 14 more frames...
                if (sExitTimer-- > 0) {
                    return NULL;
                }

                // Bowser levels
                if (sOmmWarpState.level == LEVEL_BOWSER_1) sOmmWarpState.level = LEVEL_BITDW;
                if (sOmmWarpState.level == LEVEL_BOWSER_2) sOmmWarpState.level = LEVEL_BITFS;
                if (sOmmWarpState.level == LEVEL_BOWSER_3) sOmmWarpState.level = LEVEL_BITS;

                // Exit warp to Castle warp
                // Uses the death warp, as it's the only warp that exists for every stage in the game
                s16 *warp = omm_level_get_death_warp(sOmmWarpState.level, sOmmWarpState.area);
                sTargetWarp = omm_level_get_warp(warp[7], warp[8], warp[9]);

                // Free everything from the current level
                clear_objects();
                clear_area_graph_nodes();
                clear_areas();
                main_pool_pop_state();

                // Reset Mario's state
                gMarioState->action = 0;
                gMarioState->healCounter = 0;
                gMarioState->hurtCounter = 0;
                gMarioState->numCoins = 0;
                gMarioState->input = 0;
                gMarioState->controller->buttonPressed = 0;
                gHudDisplay.coins = 0;
                gOmmData->reset();

                // Set up new level values
                gCurrLevelNum = warp[7];
                gCurrCourseNum = omm_level_get_course(gCurrLevelNum);
                gSavedCourseNum = gCurrCourseNum;
                gDialogCourseActNum = gCurrActNum;
                gCurrAreaIndex = warp[8];
                sTargetArea = warp[8];

                // Set up new level script
                sWarpDest.type = 0;
                sWarpDest.levelNum = 0;
                sWarpDest.areaIdx = gCurrAreaIndex;
                sWarpDest.nodeId = 0;
                sWarpDest.arg = 0;
                return (void *) omm_level_get_script(gCurrLevelNum);

            } else {
    
                // Phase 2 - Set Mario spawn info after the MARIO_POS command
                if (*((u8 *) cmd) == 0x2B) {
                    gMarioSpawnInfo->areaIndex = sTargetArea;
                    gCurrAreaIndex = sTargetArea;
                }

                // Phase 3 - End level initialization
                if (sTargetWarp && inited) {
            
                    // Find target position
                    s16 x = sTargetWarp[3];
                    s16 y = sTargetWarp[4];
                    s16 z = sTargetWarp[5];
                    s16 yaw = sTargetWarp[6];
                    omm_exit_level_find_position(&x, &y, &z, yaw, OMM_LEVEL_EXIT_DISTANCE);

                    // Init Mario
                    gMarioSpawnInfo->startPos[0] = x;
                    gMarioSpawnInfo->startPos[1] = y;
                    gMarioSpawnInfo->startPos[2] = z;
                    gMarioSpawnInfo->startAngle[0] = 0;
                    gMarioSpawnInfo->startAngle[1] = yaw;
                    gMarioSpawnInfo->startAngle[2] = 0;
                    gMarioSpawnInfo->areaIndex = gCurrAreaIndex;
                    init_mario();
                    set_mario_initial_action(gMarioState, MARIO_SPAWN_UNKNOWN_02, 0);

                    // Init transition
                    reset_camera(gCurrentArea->camera);
                    init_camera(gCurrentArea->camera);
                    sDelayedWarpOp = WARP_OP_NONE;
                    gTimeStopState &= ~(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                    play_transition(WARP_TRANSITION_FADE_FROM_STAR, 15, 0, 0, 0);
                    play_sound(SOUND_MENU_MARIO_CASTLE_WARP, gGlobalSoundArgs);

                    // Set music
                    music_stop();
                    set_background_music(gCurrentArea->musicParam, gCurrentArea->musicParam2, 0);
                    sTargetWarp = NULL;
                }

                // Phase 4 - Unlock Mario as soon as the second transition is ended
                if (!sTargetWarp && !omm_is_transition_active()) {
                    sOmmWarpState.active = false;
                    sTargetArea = -1;
                }
            }
        }
        
        // Warp to level
        else {
        
            // Phase 1 - Clear the previous level and set up the new level
            if (sTargetArea == -1) {

                // Close the pause menu if it was open
                if (optmenu_open) optmenu_toggle();
                level_set_transition(0, NULL);
                gDialogBoxState = 0;
                gMenuMode = -1;

                // Cancel out every music/sound/sequence
                audio_mute(FALSE);
                music_unlower_volume(SEQ_PLAYER_LEVEL, 0);
                for (u16 seqid = 0; seqid != SEQ_COUNT; ++seqid) {
                stop_background_music(seqid);
                }
                audio_play_shell_music();
                audio_stop_shell_music();
                audio_stop_cap_music();
                func_80321080(0);
                fadeout_music(0);
                fadeout_level_music(0);

                // Free everything from the current level
                clear_objects();
                clear_area_graph_nodes();
                clear_areas();
                main_pool_pop_state();

                // Reset Mario's state
                gMarioState->action = 0;
                gMarioState->healCounter = 0;
                gMarioState->hurtCounter = 0;
                gMarioState->numCoins = 0;
                gMarioState->input = 0;
                gMarioState->controller->buttonPressed = 0;
                gHudDisplay.coins = 0;
                gOmmData->reset();

                // Set up new level values
                gCurrLevelNum = sOmmWarpState.level;
                gCurrCourseNum = omm_level_get_course(gCurrLevelNum);
                gSavedCourseNum = gCurrCourseNum;
                gCurrActNum = max_s(1, sOmmWarpState.act * (gCurrCourseNum <= COURSE_STAGES_MAX));
                gDialogCourseActNum = gCurrActNum;
                gCurrAreaIndex = sOmmWarpState.area;
                sTargetArea = gCurrAreaIndex;

                // Set up new level script
                sWarpDest.type = 0;
                sWarpDest.levelNum = 0;
                sWarpDest.areaIdx = gCurrAreaIndex;
                sWarpDest.nodeId = 0;
                sWarpDest.arg = 0;
                return (void *) omm_level_get_script(gCurrLevelNum);

            } else {
    
                // Phase 2 - Set Mario spawn info after the MARIO_POS command
                if (*((u8 *) cmd) == 0x2B) {
                    gMarioSpawnInfo->areaIndex = sTargetArea;
                    gCurrAreaIndex = sTargetArea;
                }

                // Phase 3 - End level initialization
                if (inited) {

                    // Init Mario
                    s16 *warp = omm_level_get_entry_warp(gCurrLevelNum, gCurrAreaIndex);
                    s16 spawnType = sSpawnTypeFromWarpBhv[warp[2]];
                    gMarioSpawnInfo->startPos[0] = warp[3] + (spawnType == MARIO_SPAWN_DOOR_WARP) * 300.0f * sins(warp[6]);
                    gMarioSpawnInfo->startPos[1] = warp[4];
                    gMarioSpawnInfo->startPos[2] = warp[5] + (spawnType == MARIO_SPAWN_DOOR_WARP) * 300.0f * coss(warp[6]);
                    gMarioSpawnInfo->startAngle[0] = 0;
                    gMarioSpawnInfo->startAngle[1] = warp[6];
                    gMarioSpawnInfo->startAngle[2] = 0;
                    gMarioSpawnInfo->areaIndex = gCurrAreaIndex;
                    init_mario();
                    set_mario_initial_action(gMarioState, spawnType, 0);

                    // Init transition
                    reset_camera(gCurrentArea->camera);
                    init_camera(gCurrentArea->camera);
                    sDelayedWarpOp = WARP_OP_NONE;
                    switch (spawnType) {
                        case MARIO_SPAWN_UNKNOWN_03:           play_transition(WARP_TRANSITION_FADE_FROM_STAR,   0x10, 0x00, 0x00, 0x00); break;
                        case MARIO_SPAWN_DOOR_WARP:            play_transition(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00); break;
                        case MARIO_SPAWN_TELEPORT:             play_transition(WARP_TRANSITION_FADE_FROM_COLOR,  0x14, 0xFF, 0xFF, 0xFF); break;
                        case MARIO_SPAWN_SPIN_AIRBORNE:        play_transition(WARP_TRANSITION_FADE_FROM_COLOR,  0x1A, 0xFF, 0xFF, 0xFF); break;
                        case MARIO_SPAWN_SPIN_AIRBORNE_CIRCLE: play_transition(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00); break;
                        case MARIO_SPAWN_UNKNOWN_27:           play_transition(WARP_TRANSITION_FADE_FROM_COLOR,  0x10, 0x00, 0x00, 0x00); break;
                        default:                               play_transition(WARP_TRANSITION_FADE_FROM_STAR,   0x10, 0x00, 0x00, 0x00); break;
                    }

                    // Set music
                    music_stop();
                    set_background_music(gCurrentArea->musicParam, gCurrentArea->musicParam2, 0);
                    if (gMarioState->flags & MARIO_WING_CAP)   audio_play_wing_cap_music();
                    if (gMarioState->flags & MARIO_METAL_CAP)  audio_play_metal_cap_music();
                    if (gMarioState->flags & MARIO_VANISH_CAP) audio_play_vanish_cap_music();

                    if (gCurrLevelNum == LEVEL_BOWSER_1 ||
                        gCurrLevelNum == LEVEL_BOWSER_2 ||
                        gCurrLevelNum == LEVEL_BOWSER_3) {
                        sound_banks_enable(0, 0xFFFF); // Bowser levels sound fix
                    }

                    // Reset values
                    sOmmWarpState.active = false;
                    sTargetArea = -1;
                }
            }
        }
    }
    return NULL;
}

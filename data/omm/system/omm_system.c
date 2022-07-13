#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
s16 gOmm1HPMode = FALSE;

//
// Data
//

typedef void (*OmmRoutine)(void);
static OmmArray sOmmRoutines[OMM_ROUTINE_TYPES] = { omm_array_zero, omm_array_zero, omm_array_zero };
static bool sOmmIsMainMenu = true;
static bool sOmmIsLevelEntry = false;
static bool sOmmIsEndingCutscene = false;
static bool sOmmIsEndingCakeScreen = false;
static s32  sOmmTimerReturnToMainMenu = 0;

//
// Complete save cheat
// Press C-Up, C-Up, C-Down, C-Down, C-Left, C-Right, C-Left, C-Right, Z, R and select a file with A
// Collects all stars, sets all flags, opens all cannons and unlocks Peach
//

static const u16 sOmmCompleteSaveButtons[] = { U_CBUTTONS, U_CBUTTONS, D_CBUTTONS, D_CBUTTONS, L_CBUTTONS, R_CBUTTONS, L_CBUTTONS, R_CBUTTONS, Z_TRIG, R_TRIG, A_BUTTON, 0xFFFF };
static s32 sOmmCompleteSaveSequenceIndex = 0;
extern void omm_set_complete_save_file(s32 fileIndex);
extern void omm_stars_init_bits();

//
// Paths
//

const char *omm_exe_path() {
    static char *sOmmExePath = NULL;
    if (!sOmmExePath) {
        sOmmExePath = SDL_GetBasePath();
        if (sOmmExePath) {
            s32 length = strlen(sOmmExePath);
            for (char *sep = sOmmExePath + length - 1; *sep == '/' || *sep == '\\'; --sep) {
                *sep = 0;
            }
        }
    }
    return sOmmExePath;
}

const char *omm_user_path() {
    static char *sOmmUserPath = NULL;
    if (!sOmmUserPath) {
        sOmmUserPath = SDL_GetPrefPath("", "sm64ex");
        if (sOmmUserPath) {
            s32 length = strlen(sOmmUserPath);
            for (char *sep = sOmmUserPath + length - 1; *sep == '/' || *sep == '\\'; --sep) {
                *sep = 0;
            }
            if (!fs_sys_dir_exists(sOmmUserPath)) {
                fs_sys_mkdir(sOmmUserPath);
            }
        }
    }
    return sOmmUserPath;
}

//
// Routines
//

void omm_add_routine(s32 type, void (*func)(void)) {
    if (OMM_LIKELY(type >= 0 && type < OMM_ROUTINE_TYPES && func)) {
        if (omm_array_find(sOmmRoutines[type], ptr, func) == -1) {
            omm_array_add(sOmmRoutines[type], ptr, func);
        }
    }
}

static void omm_execute_routines(s32 type) {
    omm_array_for_each(sOmmRoutines[type], p) {
        OmmRoutine routine = (OmmRoutine) p->as_ptr;
        routine();
    }
}

//
// Main Menu
//

void omm_select_save_file(s32 saveFileNum) {
    gMarioState->health = omm_health_get_max(0);
    gCurrSaveFileNum = saveFileNum;
    sOmmIsMainMenu = false;
    sOmmIsLevelEntry = false;
    sOmmIsEndingCutscene = false;
    if (sOmmCompleteSaveSequenceIndex == OMM_ARRAY_SIZE(sOmmCompleteSaveButtons) - 1) {
        omm_set_complete_save_file(saveFileNum - 1);
    }
    sOmmCompleteSaveSequenceIndex = 0;
}

void omm_return_to_main_menu() {
    if (optmenu_open) optmenu_toggle();
    fade_into_special_warp(-2, 0);
    gDialogBoxState = 0;
    gMenuMode = -1;
}

//
// Update
//

void omm_update() {

    // Level entry
    if (sOmmIsLevelEntry) {
        omm_execute_routines(OMM_ROUTINE_TYPE_LEVEL_ENTRY);
        sOmmIsLevelEntry = false;
    }

    // Update routines
    omm_execute_routines(OMM_ROUTINE_TYPE_UPDATE);

    // Complete save cheat inputs
    if (sOmmIsMainMenu) {
        if (gPlayer1Controller->buttonPressed != 0) {
            u16 buttonPressed = gPlayer1Controller->buttonPressed;
            u16 buttonRequired = sOmmCompleteSaveButtons[sOmmCompleteSaveSequenceIndex];
            if ((buttonPressed & buttonRequired) == buttonRequired) {
                sOmmCompleteSaveSequenceIndex++;
                if (sOmmCompleteSaveButtons[sOmmCompleteSaveSequenceIndex] == A_BUTTON) {
#if OMM_GAME_IS_R96A
                    play_sound(SOUND_MENU_STAR_SOUND | 0xFF00, gGlobalSoundArgs); // For some reason the 'right answer' sound effect doesn't play
#else
                    play_sound(SOUND_GENERAL2_RIGHT_ANSWER | 0xFF00, gGlobalSoundArgs);
#endif
                }
            } else {
                sOmmCompleteSaveSequenceIndex = 0;
            }
        }
    } else {
        sOmmCompleteSaveSequenceIndex = 0;
    }

    // Inhibit inputs (except camera controls) during a transition
    if (omm_is_transition_active() || omm_is_warping()) {
        for (s32 i = 0; i != OMM_ARRAY_SIZE(gControllers); ++i) {
            gControllers[i].rawStickX      = 0;
            gControllers[i].rawStickY      = 0;
            gControllers[i].stickX         = 0;
            gControllers[i].stickY         = 0;
            gControllers[i].stickMag       = 0;
            gControllers[i].buttonDown    &= (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS | L_TRIG | R_TRIG);
            gControllers[i].buttonPressed &= (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS | L_TRIG | R_TRIG);
        }
    }

    // Misc stuff
    sOmmTimerReturnToMainMenu--;
    gPrevFrameObjectCount = 0;
}

//
// Update GFX
//

static void omm_pre_render_update_stars_models() {

    // Stars number
    if (OMM_EXTRAS_SHOW_STAR_NUMBER) {
        omm_array_for_each(omm_obj_get_star_or_key_behaviors(), p) {
            const BehaviorScript *bhv = (const BehaviorScript *) p->as_ptr;
            for_each_object_with_behavior(star, bhv) {
                if (!obj_is_dormant(star) &&
                    star->behavior != bhvBowserKey &&
                    star->behavior != bhvGrandStar &&
                    star->behavior != omm_bhv_sparkly_star) {
                    bool numberSpawned = false;
                    for_each_object_with_behavior(obj, omm_bhv_star_number) {
                        if (obj->activeFlags && obj->parentObj == star) {
                            numberSpawned = true;
                            break;
                        }
                    }
                    if (!numberSpawned) {
                        omm_spawn_star_number(star);
                    }
                }
            }
        }
    }

#if !OMM_GAME_IS_SMMS
    // Colored Stars
    static struct GraphNode *sOmmStarGraphNodes[34] = { NULL };
    if (OMM_UNLIKELY(!sOmmStarGraphNodes[0])) {
        sOmmStarGraphNodes[0]  = geo_layout_to_graph_node(NULL, omm_geo_star_0_opaque);
        sOmmStarGraphNodes[1]  = geo_layout_to_graph_node(NULL, omm_geo_star_1_opaque);
        sOmmStarGraphNodes[2]  = geo_layout_to_graph_node(NULL, omm_geo_star_2_opaque);
        sOmmStarGraphNodes[3]  = geo_layout_to_graph_node(NULL, omm_geo_star_3_opaque);
        sOmmStarGraphNodes[4]  = geo_layout_to_graph_node(NULL, omm_geo_star_4_opaque);
        sOmmStarGraphNodes[5]  = geo_layout_to_graph_node(NULL, omm_geo_star_5_opaque);
        sOmmStarGraphNodes[6]  = geo_layout_to_graph_node(NULL, omm_geo_star_6_opaque);
        sOmmStarGraphNodes[7]  = geo_layout_to_graph_node(NULL, omm_geo_star_7_opaque);
        sOmmStarGraphNodes[8]  = geo_layout_to_graph_node(NULL, omm_geo_star_8_opaque);
        sOmmStarGraphNodes[9]  = geo_layout_to_graph_node(NULL, omm_geo_star_9_opaque);
        sOmmStarGraphNodes[10] = geo_layout_to_graph_node(NULL, omm_geo_star_10_opaque);
        sOmmStarGraphNodes[11] = geo_layout_to_graph_node(NULL, omm_geo_star_11_opaque);
        sOmmStarGraphNodes[12] = geo_layout_to_graph_node(NULL, omm_geo_star_12_opaque);
        sOmmStarGraphNodes[13] = geo_layout_to_graph_node(NULL, omm_geo_star_13_opaque);
        sOmmStarGraphNodes[14] = geo_layout_to_graph_node(NULL, omm_geo_star_14_opaque);
        sOmmStarGraphNodes[15] = geo_layout_to_graph_node(NULL, omm_geo_star_15_opaque);
        sOmmStarGraphNodes[16] = geo_layout_to_graph_node(NULL, omm_geo_star_16_opaque);
        sOmmStarGraphNodes[17] = geo_layout_to_graph_node(NULL, omm_geo_star_0_transparent);
        sOmmStarGraphNodes[18] = geo_layout_to_graph_node(NULL, omm_geo_star_1_transparent);
        sOmmStarGraphNodes[19] = geo_layout_to_graph_node(NULL, omm_geo_star_2_transparent);
        sOmmStarGraphNodes[20] = geo_layout_to_graph_node(NULL, omm_geo_star_3_transparent);
        sOmmStarGraphNodes[21] = geo_layout_to_graph_node(NULL, omm_geo_star_4_transparent);
        sOmmStarGraphNodes[22] = geo_layout_to_graph_node(NULL, omm_geo_star_5_transparent);
        sOmmStarGraphNodes[23] = geo_layout_to_graph_node(NULL, omm_geo_star_6_transparent);
        sOmmStarGraphNodes[24] = geo_layout_to_graph_node(NULL, omm_geo_star_7_transparent);
        sOmmStarGraphNodes[25] = geo_layout_to_graph_node(NULL, omm_geo_star_8_transparent);
        sOmmStarGraphNodes[26] = geo_layout_to_graph_node(NULL, omm_geo_star_9_transparent);
        sOmmStarGraphNodes[27] = geo_layout_to_graph_node(NULL, omm_geo_star_10_transparent);
        sOmmStarGraphNodes[28] = geo_layout_to_graph_node(NULL, omm_geo_star_11_transparent);
        sOmmStarGraphNodes[29] = geo_layout_to_graph_node(NULL, omm_geo_star_12_transparent);
        sOmmStarGraphNodes[30] = geo_layout_to_graph_node(NULL, omm_geo_star_13_transparent);
        sOmmStarGraphNodes[31] = geo_layout_to_graph_node(NULL, omm_geo_star_14_transparent);
        sOmmStarGraphNodes[32] = geo_layout_to_graph_node(NULL, omm_geo_star_15_transparent);
        sOmmStarGraphNodes[33] = geo_layout_to_graph_node(NULL, omm_geo_star_16_transparent);
    }

    s32 starColor = clamp_s(gCurrCourseNum, 0, 16);
    omm_array_for_each(omm_obj_get_star_model_behaviors(), p) {
        const BehaviorScript *bhv = (const BehaviorScript *) p->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            if (OMM_EXTRAS_COLORED_STARS) {
                if (obj_check_model(obj, MODEL_STAR) || (obj->behavior == bhvCelebrationStar && !obj_check_model(obj, MODEL_BOWSER_KEY))) {
                    obj->oGraphNode = sOmmStarGraphNodes[starColor];
                } else if (obj_check_model(obj, MODEL_TRANSPARENT_STAR)) {
                    obj->oGraphNode = sOmmStarGraphNodes[starColor + 17 * !omm_is_ending_cutscene()];
                }
            } else {
                for (s32 i = 0; i != 34; ++i) {
                    if (obj_has_graph_node(obj, sOmmStarGraphNodes[i])) {
                        obj->oGraphNode = gLoadedGraphNodes[i < 17 ? MODEL_STAR : MODEL_TRANSPARENT_STAR];
                        break;
                    }
                }
            }
        }
    }
#endif
}

static void omm_pre_render_update_caps_models() {
    static s32 (*sCapFunctions[])(s32) = {
        omm_player_get_normal_cap,
        omm_player_get_wing_cap,
        omm_player_get_metal_cap,
        omm_player_get_winged_metal_cap,
    };
    s32 playerIndex = omm_player_get_selected_index();
    omm_array_for_each(omm_obj_get_cap_behaviors(), p) {
        const BehaviorScript *bhv = (const BehaviorScript *) p->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            for (s32 capType = 0; capType != 4; ++capType) {
                s32 playerCapModel = sCapFunctions[capType](playerIndex);
                for (s32 charIndex = 0; charIndex != OMM_NUM_PLAYABLE_CHARACTERS; ++charIndex) {
                    s32 charCapModel = sCapFunctions[capType](charIndex);
                    if (charCapModel != playerCapModel && obj_check_model(obj, charCapModel)) {
                        obj->oGraphNode = gLoadedGraphNodes[playerCapModel];
                        capType = 3; // break the other for loop
                        break;
                    }
                }
            }
            geo_preprocess_object_graph_node(obj);
        }
    }
}

void omm_pre_render() {
    omm_execute_routines(OMM_ROUTINE_TYPE_PRE_RENDER);

    // Mario
    if (gMarioState->marioBodyState && OMM_EXTRAS_INVISIBLE_MODE) {
        gMarioState->marioBodyState->modelState &= ~0xFF;
        gMarioState->marioBodyState->modelState |= 0x100;
    }

    // Crystal sparkles
    if (gMarioObject && OMM_EXTRAS_CRYSTAL_STARS_REWARD) {
        if (!omm_is_game_paused() && ((gGlobalTimer & 1) || vec3f_dist(gMarioState->pos, gOmmData->mario->state.previous.pos) > 20.f)) {
            omm_spawn_sparkly_star_sparkle_mario(gMarioObject, OMM_SSM_HARD, 60.f, 10.f, 0.5f, 30.f);
        }
    }

    // Object models
    omm_pre_render_update_stars_models();
    omm_pre_render_update_caps_models();

    // Invisible mode
    omm_array_for_each(omm_obj_get_player_behaviors(), p) {
        const BehaviorScript *bhv = (const BehaviorScript *) p->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            if (OMM_EXTRAS_INVISIBLE_MODE) {
                obj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
            } else {
                obj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            }
        }
    }
}

//
// Level commands
//

void *omm_update_cmd(void *cmd, s32 reg) {

    // Main Menu
    if (cmd == level_script_entry_point ||
        cmd == level_script_splash_screen ||
        cmd == level_script_goddard_regular ||
        cmd == level_script_goddard_game_over ||
        cmd == level_script_debug_level_select ||
        cmd == level_script_to_file_select ||
        cmd == level_script_to_debug_level_select ||
        cmd == level_script_to_star_select_1 ||
        cmd == level_script_to_star_select_2 ||
        cmd == level_script_to_splash_screen ||
        cmd == level_script_file_select) {
        gMarioState->action = 0;
        sOmmIsMainMenu = true;
        sOmmIsLevelEntry = false;
        sOmmIsEndingCutscene = false;
        sOmmIsEndingCakeScreen = false;
    }

    // Level Entry
    static const uintptr_t cmd_level_entry[] = { CALL(0, lvl_init_or_update) };
    if (OMM_MEMCMP(cmd, cmd_level_entry, sizeof(cmd_level_entry))) {
        sOmmIsMainMenu = false;
        sOmmIsLevelEntry = true;
        sOmmIsEndingCakeScreen = false;
        omm_stars_init_bits();
    }

    // Star Select
    static const uintptr_t cmd_star_select[] = { EXECUTE(0x14, _menuSegmentRomStart, _menuSegmentRomEnd, level_script_star_select) };
    if (OMM_MEMCMP(cmd, cmd_star_select, sizeof(cmd_star_select)) && OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) {
        gCurrLevelNum = reg;
        gCurrCourseNum = omm_level_get_course(reg);
        gCurrActNum = 1;
#if !OMM_GAME_IS_SM74
        gCurrAreaIndex = 1;
#endif
        gDialogCourseActNum = gCurrActNum;

        // Reset the coin counter and star flags unless Mario enters a Bowser fight or returns to the Castle
        if (gCurrLevelNum != LEVEL_BOWSER_1 &&
            gCurrLevelNum != LEVEL_BOWSER_2 &&
            gCurrLevelNum != LEVEL_BOWSER_3 &&
            gCurrLevelNum != LEVEL_CASTLE   &&
            gCurrLevelNum != LEVEL_GROUNDS  &&
            gCurrLevelNum != LEVEL_COURT) {
            gMarioState->numCoins = 0;
            gHudDisplay.coins = 0;
            gCurrCourseStarFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
        }

        // Return an EXIT command to skip the star select screen
        static const uintptr_t cmd_skip_star_select[] = { EXIT() };
        return (void *) cmd_skip_star_select;
    }

    // Ending Cutscene
    if (gMarioState->action == ACT_JUMBO_STAR_CUTSCENE ||
        gMarioState->action == ACT_END_WAVING_CUTSCENE ||
        gMarioState->action == ACT_END_PEACH_CUTSCENE ||
        gMarioState->action == ACT_CREDITS_CUTSCENE) {
        sOmmIsMainMenu = false;
        sOmmIsEndingCutscene = true;
    }

    // Ending return to Main Menu
    switch (sOmmTimerReturnToMainMenu) {

        // Sets sOmmIsEndingCakeScreen to true if loading the cake screen
        // Starts the timer when hearing the "Thank you so much"
        default: {
            if (cmd == level_script_cake_ending) {
                sOmmIsEndingCakeScreen = true;
            }
            static const uintptr_t cmd_ending_cake_sound[] = { CALL(0, lvl_play_the_end_screen_sound) };
            if (OMM_MEMCMP(cmd, cmd_ending_cake_sound, sizeof(cmd_ending_cake_sound))) {
                sOmmTimerReturnToMainMenu = 300;
            }
        } break;

        // After 5 seconds, stops the timer, and wait for the player to press A or Start
        case 150: {
            if (gPlayer1Controller->buttonPressed & (A_BUTTON | START_BUTTON)) {
                play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 90, 0, 0, 0);
            } else {
                sOmmTimerReturnToMainMenu++;
            }
        } break;

        // After 5 other seconds and a transition, return to the main menu
        case 0: {
            gHudDisplay.flags = 0;
            sOmmTimerReturnToMainMenu = -1;
            sOmmIsEndingCakeScreen = false;
            return (void *) level_script_goddard_regular;
        } break;
    }

    // Warp update
    return omm_update_warp(cmd, sOmmIsLevelEntry);
}

//
// Game states
//

bool omm_is_main_menu() {
    return sOmmIsMainMenu;
}

bool omm_is_game_paused() {
    return gMenuMode != -1;
}

bool omm_is_transition_active() {
    static s16 sOmmTransitionTimer = 0;
    if (sTransitionTimer > 0) sOmmTransitionTimer = sTransitionTimer;
    bool isTransitionActive = (gWarpTransition.isActive || sOmmTransitionTimer > 0);
    sOmmTransitionTimer = sTransitionTimer;
    return isTransitionActive;
}

bool omm_is_ending_cutscene() {
    return sOmmIsEndingCutscene;
}

bool omm_is_ending_cake_screen() {
    return sOmmIsEndingCakeScreen;
}

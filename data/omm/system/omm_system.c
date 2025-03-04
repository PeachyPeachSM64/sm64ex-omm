#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/omm_constants.h"
#include "level_commands.h"

typedef void (*OmmRoutine)(void);

static struct {
    OmmArray routines[OMM_ROUTINE_TYPES];
    bool skipIntro;
    bool isMainMenu;
    bool isEndingCutscene;
    bool isEndingCake;
    u32 returnToMainMenu;
    s32 warpToLastCourseNum;
    s16 transitionTimer;
} sOmmSystem[1];

//
// Routines
//

void omm_add_routine(s32 type, void (*func)(void)) {
    if (OMM_LIKELY(type >= 0 && type < OMM_ROUTINE_TYPES && func)) {
        if (omm_array_find(sOmmSystem->routines[type], ptr, func) == -1) {
            omm_array_add(sOmmSystem->routines[type], ptr, func);
        }
    }
}

void omm_execute_routines(s32 type) {
    omm_array_for_each(sOmmSystem->routines[type], p_routine) {
        OmmRoutine routine = (OmmRoutine) p_routine->as_ptr;
        routine();
    }
}

//
// Main Menu
//

void omm_select_save_file(s32 fileIndex, s32 modeIndex, s32 courseNum, bool skipIntro) {
    gMarioState->health = omm_health_get_max(0);
    gCurrSaveFileNum = fileIndex + 1;
    gCurrAreaIndex = modeIndex + 1;
    sWarpDest.areaIdx = modeIndex + 1;
    sOmmSystem->warpToLastCourseNum = courseNum;
    sOmmSystem->isMainMenu = false;
    sOmmSystem->isEndingCutscene = false;
    sOmmSystem->skipIntro = skipIntro || gOmmGlobals->yoshiMode;
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

    // Resume save file from last course
    if (!sOmmSystem->isMainMenu && sOmmSystem->warpToLastCourseNum != COURSE_NONE && gMarioObject) {
        s32 levelNum = omm_level_from_course(sOmmSystem->warpToLastCourseNum);
        initiate_warp(levelNum, gCurrAreaIndex, OMM_LEVEL_ENTRY_WARP(levelNum), 0);
        play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 1, 0xFF, 0xFF, 0xFF);
        level_set_transition(0, NULL);
        warp_special(0);
        gSavedCourseNum = COURSE_NONE;
        sOmmSystem->warpToLastCourseNum = COURSE_NONE;
        return;
    }

    // Update routines
    omm_execute_routines(OMM_ROUTINE_TYPE_UPDATE);

    // Inhibit inputs (except camera controls) during a transition
    if (omm_is_transition_active() || omm_is_warping()) {
        for (s32 i = 0; i != array_length(gControllers); ++i) {
            gControllers[i].rawStickX      = 0;
            gControllers[i].rawStickY      = 0;
            gControllers[i].stickX         = 0;
            gControllers[i].stickY         = 0;
            gControllers[i].stickMag       = 0;
            gControllers[i].buttonDown    &= (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS | L_TRIG | R_TRIG);
            gControllers[i].buttonPressed &= (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS | L_TRIG | R_TRIG);
        }
    }

    // Auto-update display options
    static ConfigWindow sConfigWindow;
    configWindow.settings_changed = sConfigWindow.fullscreen != configWindow.fullscreen || sConfigWindow.vsync != configWindow.vsync;
    sConfigWindow = configWindow;

    // Misc stuff
    gPrevFrameObjectCount = 0;
#if OMM_GAME_IS_SMSR
    gStarRoadHardMode = FALSE;
#endif
#if BETTER_CAM_IS_PUPPY_CAM
    if (omm_is_main_menu()) {
        gPuppyCam.opacity = 0xFF;
    }
#endif
}

//
// Update GFX
//

static void omm_pre_render_update_stars_models() {

    // Stars number
    if (OMM_EXTRAS_SHOW_STAR_NUMBER) {
        omm_array_for_each(omm_obj_get_star_or_key_behaviors(), p_bhv) {
            const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
            for_each_object_with_behavior(star, bhv) {
                if (!obj_is_dormant(star) &&
                    star->behavior != bhvBowserKey &&
                    star->behavior != bhvGrandStar &&
                    star->behavior != bhvOmmSparklyStar) {
                    bool numberSpawned = false;
                    for_each_object_with_behavior(obj, bhvOmmStarNumber) {
                        if (obj->activeFlags && obj->parentObj == star) {
                            numberSpawned = true;
                            break;
                        }
                    }
                    if (!numberSpawned) {
                        omm_obj_spawn_star_number(star);
                    }
                }
            }
        }
    }

#if !OMM_GAME_IS_SMMS
    // Colored Stars
    static struct GraphNode *sOmmStarGraphNodes[34] = {0};
    OMM_DO_ONCE {
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

    s32 starColor = OMM_STAR_COLOR_[clamp_s(gCurrCourseNum, 0, 16) + OMM_STAR_COLOR_OFFSET(OMM_GAME_MODE)];
    omm_array_for_each(omm_obj_get_star_model_behaviors(), p_bhv) {
        const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            if (OMM_EXTRAS_COLORED_STARS) {
                if (obj_has_model(obj, MODEL_STAR) || (obj->behavior == bhvCelebrationStar && !obj_has_model(obj, MODEL_BOWSER_KEY))) {
                    obj->oGraphNode = sOmmStarGraphNodes[starColor];
                } else if (obj_has_model(obj, MODEL_TRANSPARENT_STAR)) {
                    obj->oGraphNode = sOmmStarGraphNodes[starColor + 17 * !omm_is_ending_cutscene()];
                }
            } else {
                for (s32 i = 0; i != 34; ++i) {
                    if (obj_has_graph_node(obj, sOmmStarGraphNodes[i])) {
                        obj->oGraphNode = geo_layout_to_graph_node(NULL, i < 17 ? star_geo : transparent_star_geo);
                        break;
                    }
                }
            }
        }
    }
#endif
}

static void omm_pre_render_update_caps_models() {
    static s32 (*cap_functions[])(s32) = {
        omm_player_graphics_get_normal_cap,
        omm_player_graphics_get_wing_cap,
        omm_player_graphics_get_metal_cap,
        omm_player_graphics_get_winged_metal_cap,
    };
    s32 playerIndex = omm_player_get_selected_index_model_and_sounds();
    omm_array_for_each(omm_obj_get_cap_behaviors(), p_bhv) {
        const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            for (s32 capType = 0; capType != 4; ++capType) {
                s32 playerCapModel = cap_functions[capType](playerIndex);
                for (s32 charIndex = 0; charIndex != OMM_NUM_PLAYABLE_CHARACTERS; ++charIndex) {
                    s32 charCapModel = cap_functions[capType](charIndex);
                    if (charCapModel != playerCapModel && obj_has_model(obj, charCapModel)) {
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
    struct MarioState *m = gMarioState;
    struct Object *o = gMarioObject;

    // Always make Mario fully opaque on title screen
    if (m->marioBodyState && omm_is_main_menu()) {
        m->marioBodyState->modelState &= ~0x1FF;
    }

    // Sparkly Stars sparkles
    if (OMM_SPARKLY_STARS_COMPLETION_REWARD && !OMM_EXTRAS_INVISIBLE_MODE && o && !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE) && get_dialog_id() == -1 && !omm_is_game_paused()) {
        f32 vel = vec3f_dist(m->pos, gOmmMario->state.previous.pos);
        if (gGlobalTimer % (3 - clamp_s(vel / 25.f, 0, 2)) == 0) {
            struct Object *sparkle = omm_obj_spawn_sparkly_star_sparkle_mario(o, OMM_SPARKLY_STARS_COMPLETION_REWARD, 20.f, 10.f, 0.4f, 30.f);
            Vec3f marioRootPos; geo_get_marios_anim_part_pos(NULL, marioRootPos, MARIO_ANIM_PART_ROOT);
            sparkle->oPosX += (marioRootPos[0] - o->oPosX);
            sparkle->oPosY += (marioRootPos[1] - o->oPosY);
            sparkle->oPosZ += (marioRootPos[2] - o->oPosZ);
        }
    }

    // Object models
    omm_pre_render_update_stars_models();
    omm_pre_render_update_caps_models();

    // Routines
    omm_execute_routines(OMM_ROUTINE_TYPE_PRE_RENDER);

    // Invisible mode
    omm_array_for_each(omm_obj_get_player_behaviors(), p_bhv) {
        const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            if (OMM_EXTRAS_INVISIBLE_MODE) {
                obj->oFlags |= OBJ_FLAG_INVISIBLE_MODE;
            } else {
                obj->oFlags &= ~OBJ_FLAG_INVISIBLE_MODE;
            }
        }
    }

    // Cut the music before resuming save file from last course
    if (!sOmmSystem->isMainMenu && sOmmSystem->warpToLastCourseNum != COURSE_NONE && o) {
        music_fade_out(SEQ_PLAYER_LEVEL, 1);
        music_pause();
    }

    // Clear the spin bit
    gOmmMario->spin.pressed = false;
}

//
// Level commands
//

void *omm_update_cmd(void *cmd, UNUSED s32 reg) {

    // Main menu
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
        configSkipIntro = false;
        sOmmSystem->skipIntro = false;
        sOmmSystem->isEndingCutscene = false;
        sOmmSystem->isEndingCake = false;
        sOmmSystem->returnToMainMenu = 0;
        if (!sOmmSystem->isMainMenu) {
            sOmmSystem->isMainMenu = true;
            omm_execute_routines(OMM_ROUTINE_TYPE_LEVEL_ENTRY);
        }
    }

    // Loading screen
    if (cmd == level_script_entry_point) {
        extern void omm_loading_screen_start();
        omm_loading_screen_start();
        return NULL;
    }

    // Palette editor
    if (sOmmSystem->isMainMenu && !omm_is_transition_active()) {
        if (omm_palette_editor_is_opening()) { omm_palette_editor_set_open();   return (void *) omm_level_palette_editor; }
        if (omm_palette_editor_is_closing()) { omm_palette_editor_set_closed(); return (void *) level_script_file_select; }
    }

#if OMM_GAME_IS_SM64
    // Skip intro cutscene (NOT Lakitu and Bowser's laugh)
    // Oddly enough, the check for intro Lakitu and Bowser's laugh occurs BEFORE the check for the intro cutscene
    // Meaning we can enable the former, then set configSkipIntro to skip the latter
    static const uintptr_t cmd_lvl_init_from_save_file[] = { CALL(0, lvl_init_from_save_file) };
    if (gOmmGlobals->yoshiMode || mem_eq(cmd, cmd_lvl_init_from_save_file, sizeof(cmd_lvl_init_from_save_file))) {
        configSkipIntro = sOmmSystem->skipIntro;
    }
#endif

    // Level entry
    bool isLevelEntry = false;
    static const uintptr_t cmd_level_entry[] = { CALL(0, lvl_init_or_update) };
    if (mem_eq(cmd, cmd_level_entry, sizeof(cmd_level_entry))) {
        isLevelEntry = true;
        sOmmSystem->isMainMenu = false;
        sOmmSystem->isEndingCake = false;
        omm_execute_routines(OMM_ROUTINE_TYPE_LEVEL_ENTRY);
    }

#if OMM_GAME_IS_SM64 || OMM_GAME_IS_SM74
    if (!sOmmSystem->isMainMenu) {

        // Ending cutscene
        if (gMarioState->action == ACT_END_PEACH_CUTSCENE ||
            gMarioState->action == ACT_CREDITS_CUTSCENE ||
            gMarioState->action == ACT_END_WAVING_CUTSCENE) {
            sOmmSystem->isEndingCutscene = true;
        }

#if OMM_GAME_IS_SM64
        // Ending cake screen
        if (cmd == level_script_cake_ending) {
            sOmmSystem->isEndingCutscene = true;
            sOmmSystem->isEndingCake = true;
            // TODO: YOSHIMODE
            // if (gOmmGlobals->yoshiMode) {
            //     omm_secrets_unlock(OMM_SECRET_YOSHI_ENDING);
            // }
        }
#endif

        // Skip ending
        if (sOmmSystem->isEndingCutscene && !sOmmSystem->returnToMainMenu && (gPlayer1Controller->buttonPressed & START_BUTTON)
        // TODO: YOSHIMODE
// #if OMM_GAME_IS_SM64
//             && (!gOmmGlobals->yoshiMode || omm_secrets_is_unlocked(OMM_SECRET_YOSHI_ENDING))
// #endif
        ) {
            sOmmSystem->returnToMainMenu = gGlobalTimer;
            play_transition(WARP_TRANSITION_FADE_INTO_STAR, 30, 0, 0, 0);
            music_fade_out(SEQ_PLAYER_LEVEL, 190);
            return NULL;
        }

        // Return to main menu
        if (sOmmSystem->returnToMainMenu && (gGlobalTimer - sOmmSystem->returnToMainMenu) > 45) {
            gHudDisplay.flags = HUD_DISPLAY_NONE;
            sOmmSystem->isEndingCutscene = false;
            sOmmSystem->returnToMainMenu = 0;

            // Level stack is already cleared by the jump to cake ending, so just jump to the main menu entry
            if (sOmmSystem->isEndingCake) {
                return (void *) level_script_goddard_regular;
            }

            // The current level must be properly cleared before jumping to the main menu
            // Unset current transition, set warp to main menu, clear and exit
            static const uintptr_t cmd_return_to_main_menu[] = {
                CALL_LOOP(1, lvl_init_or_update),
                CLEAR_LEVEL(),
                SLEEP_BEFORE_EXIT(1),
                EXIT()
            };
            level_set_transition(0, NULL);
            warp_special(-2);
            return (void *) cmd_return_to_main_menu;
        }
    }
#endif

    // Warp update
    return omm_update_warp(cmd, isLevelEntry);
}

//
// Game states
//

bool omm_is_main_menu() {
    return sOmmSystem->isMainMenu;
}

bool omm_is_game_paused() {
    return sCurrPlayMode == PLAY_MODE_PAUSED || gMenuMode != -1;
}

bool omm_is_transition_active() {
    if (sTransitionTimer > 0) sOmmSystem->transitionTimer = sTransitionTimer;
    bool isTransitionActive = (gWarpTransition.isActive || sOmmSystem->transitionTimer > 0);
    sOmmSystem->transitionTimer = sTransitionTimer;
    return isTransitionActive;
}

bool omm_is_ending_cutscene() {
    return sOmmSystem->isEndingCutscene;
}

bool omm_is_ending_cake_screen() {
    return sOmmSystem->isEndingCake;
}

bool omm_is_warping_to_last_course() {
    return sOmmSystem->warpToLastCourseNum != COURSE_NONE;
}

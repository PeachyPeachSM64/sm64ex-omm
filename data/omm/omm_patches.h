#ifndef OMM_PATCHES_H
#define OMM_PATCHES_H

//
// Patches
//

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#error "Big Endian systems are not compatible with OMM."
#endif

#if defined(DYNOS) && !defined(r96x)
#error "Standalone DynOS is not compatible with OMM."
#endif

#if defined(CHEATER) && !defined(r96x)
#error "Standalone Cheater is not compatible with OMM."
#endif

#if defined(TIME_TRIALS)
#error "Time Trials is not compatible with OMM."
#endif

#if defined(RAPI_RT64)
#error "RT64 is not compatible with OMM."
#endif

#if defined(ACT_SPINDASH) || defined(ACT_TRANSFORM)
#error "Sonic Edition is not compatible with OMM."
#endif

#if defined(ANGLE_QUEUE_SIZE) || defined(ACT_LEDGE_PARKOUR) || defined(ACT_WATER_GROUND_POUND_STROKE)
#error "Extended Moveset is not compatible with OMM."
#endif

//
// Code patches
//

#define omm_patch__find_in_bounds_yaw_wdw_bob_thi__return_yaw                   return yaw;
#define omm_patch__common_anchor_mario_behavior__obj_update_held_mario          return obj_update_held_mario(gCurrentObject, sp28, sp2C);
#define omm_patch__play_sound__play_character_sound                             OMM_RETURN_IF_TRUE(omm_sound_play_character_sound_n64(soundBits, pos),,);
#define omm_patch__stop_background_music__fix_boss_music                        stop_background_music_fix_boss_music();
#define omm_patch__render_dialog_entries__fix_dialog_box_text_lower_bound       fix_dialog_box_text_lower_bound();
#define omm_patch__check_death_barrier__cheat_walk_on_death_barrier             OMM_RETURN_IF_TRUE(OMM_CHEAT_WALK_ON_DEATH_BARRIER,,);
#define omm_patch__check_lava_boost__cheat_walk_on_lava                         OMM_RETURN_IF_TRUE(OMM_CHEAT_WALK_ON_LAVA,,);
#define omm_patch__level_trigger_warp__check_death_warp                         OMM_RETURN_IF_TRUE(omm_mario_check_death_warp(m, warpOp), 0,);
#define omm_patch__mario_get_floor_class__cheat_walk_on_slope                   OMM_RETURN_IF_TRUE(OMM_CHEAT_WALK_ON_SLOPE, SURFACE_CLASS_NOT_SLIPPERY,);
#define omm_patch__mario_floor_is_slippery__cheat_walk_on_slope                 OMM_RETURN_IF_TRUE(OMM_CHEAT_WALK_ON_SLOPE, FALSE,);
#define omm_patch__set_water_plunge_action__check_flooded                       OMM_RETURN_IF_TRUE(omm_mario_check_flooded(m), TRUE,);
#define omm_patch__update_air_with_turn__update_air_with_turn                   OMM_RETURN_IF_TRUE(OMM_LIKELY(omm_mario_update_air_with_turn(m)),,);
#define omm_patch__update_air_without_turn__update_air_without_turn             OMM_RETURN_IF_TRUE(OMM_LIKELY(omm_mario_update_air_without_turn(m)),,);
#define omm_patch__common_air_action_step__check_wall_slide                     OMM_RETURN_IF_TRUE(omm_mario_try_to_perform_wall_slide(m), AIR_STEP_NONE, set_mario_animation(m, animation););
#define omm_patch__act_lava_boost__cheat_walk_on_lava                           && !OMM_CHEAT_WALK_ON_LAVA
#define omm_patch__set_mario_npc_dialog__check_npc_dialog                       OMM_RETURN_IF_TRUE(omm_mario_check_npc_dialog(gMarioState, actionArg, &dialogState), dialogState);
#define omm_patch__check_for_instant_quicksand__fix_downwarp                    OMM_RETURN_IF_TRUE(OMM_MOVESET_ODYSSEY, (find_floor_height(m->pos[0], m->pos[1], m->pos[2]) >= m->pos[1]) && mario_update_quicksand(m, 0.f),);
#define omm_patch__apply_slope_accel__not_peach_vibe_gloom                      if (!omm_peach_vibe_is_gloom())
#define omm_patch__update_walking_speed__update_walking_speed                   OMM_RETURN_IF_TRUE(OMM_LIKELY(omm_mario_update_walking_speed(m)),,);
#define omm_patch__cheats_play_as__disable                                      return FALSE;
#define omm_patch__obj_resolve_object_collisions__ignore_capture_goomba_stack   && otherObject != gOmmCapture && (!omm_obj_is_goomba_stack(otherObject) || otherObject->oGoombaStackParent != o)
#define omm_patch__cur_obj_apply_drag_xz__fix_xz_vel                            { obj_apply_drag_xz(o, dragStrength); return; }
#define omm_patch__obj_set_hitbox__obj_fix_hitbox                               obj_fix_hitbox(obj);
#define omm_patch__cur_obj_update_dialog__skip_if_capture                       OMM_RETURN_IF_TRUE(omm_mario_is_capture(gMarioState), TRUE, gDialogLineNum = 1; handle_special_dialog_text(dialogID); o->oDialogResponse = 1;);
#define omm_patch__cur_obj_update_dialog_with_cutscene__skip_if_capture         OMM_RETURN_IF_TRUE(omm_mario_is_capture(gMarioState), TRUE, gDialogLineNum = 1; handle_special_dialog_text(dialogID); o->oDialogResponse = 1;);
#define omm_patch__cur_obj_has_model__check_georef                              if (o->oGraphNode && gLoadedGraphNodes[modelID] && o->oGraphNode->georef == gLoadedGraphNodes[modelID]->georef) { return TRUE; } else
#define omm_patch__unload_objects_from_area__keep_held_ridden_capture_alive     obj_keep_held_ridden_capture_alive();
#define omm_patch__load_level_init_text__skip_entry_dialog_if_non_stop          && OMM_STARS_CLASSIC
#define omm_patch__render_painting__interpolate_painting                        extern void gfx_interpolate_painting(Vtx *, s32); gfx_interpolate_painting(verts, numVtx);
#define omm_patch__geo_painting_update__fix_floor_pointer                       geo_painting_update_fix_floor();
#define omm_patch__r96_get_intended_level_music__bowser_4_music                 OMM_RETURN_IF_TRUE(omm_sparkly_is_bowser_4_battle(), R96_LEVEL_BOWSER_3,);
#define omm_patch__r96_get_intended_level_music__fix_milk_music                 && !omm_mario_is_milk(gMarioState)

//
// Some preprocessor magic
// That's a really awful way of coding, but it avoids patching files for a couple diffs :)
//

// Prevent the game from playing the demo and inhibiting X and Y buttons
#define run_demo_inputs_0 ;
#define run_demo_inputs_1 run_demo_inputs_unused()
#define run_demo_inputs(...) CAT(run_demo_inputs_, N_ARGS(__VA_ARGS__))

// Prevent the game from applying the old 60 FPS interpolation
#define patch_interpolations_0 ;
#define patch_interpolations_1 patch_interpolations()
#define patch_interpolations(...) CAT(patch_interpolations_, N_ARGS(__VA_ARGS__))

// Configure properly the camera settings
#define newcam_init_settings_0 omm_camera_update_settings()
#define newcam_init_settings_1 newcam_init_settings()
#define newcam_init_settings(...) CAT(newcam_init_settings_, N_ARGS(__VA_ARGS__))
#define puppycam_default_config_0 omm_camera_update_settings()
#define puppycam_default_config_1 puppycam_default_config()
#define puppycam_default_config(...) CAT(puppycam_default_config_, N_ARGS(__VA_ARGS__))

// render_hud is replaced by omm_render_hud
#define render_hud_0 omm_render_hud()
#define render_hud_1 render_hud()
#define render_hud(...) CAT(render_hud_, N_ARGS(__VA_ARGS__))

// render_pause_courses_and_castle is replaced by omm_render_pause
#define render_pause_courses_and_castle_0 omm_render_pause()
#define render_pause_courses_and_castle_1 render_pause_courses_and_castle()
#define render_pause_courses_and_castle(...) CAT(render_pause_courses_and_castle_, N_ARGS(__VA_ARGS__))
#define render_pause_screen_0 omm_render_pause()
#define render_pause_screen_1 render_pause_screen()
#define render_pause_screen(...) CAT(render_pause_screen_, N_ARGS(__VA_ARGS__))

// render_course_complete_screen is replaced by omm_render_course_complete
#define render_course_complete_screen_0 omm_render_course_complete()
#define render_course_complete_screen_1 render_course_complete_screen()
#define render_course_complete_screen(...) CAT(render_course_complete_screen_, N_ARGS(__VA_ARGS__))

// SM64: do_cutscene_handler is replaced by omm_sparkly_ending_dialog
#if OMM_GAME_IS_SM64
#define do_cutscene_handler_0 omm_sparkly_ending_dialog()
#define do_cutscene_handler_1 do_cutscene_handler()
#define do_cutscene_handler(...) CAT(do_cutscene_handler_, N_ARGS(__VA_ARGS__))
#endif

// bhv_mario_update uses OMM Mario update
#define bhv_mario_update_0 bhv_mario_update()
#define bhv_mario_update_1 bhv_mario_update_unused(void)
#define bhv_mario_update(...) CAT(bhv_mario_update_, N_ARGS(__VA_ARGS__))

// mario_ready_to_speak is replaced by omm_mario_is_ready_to_speak
#define mario_ready_to_speak_0 omm_mario_is_ready_to_speak(gMarioState)
#define mario_ready_to_speak_1 mario_ready_to_speak()
#define mario_ready_to_speak(...) CAT(mario_ready_to_speak_, N_ARGS(__VA_ARGS__))

// update_mario_platform is replaced by omm_mario_update_platform
#define update_mario_platform_0 omm_mario_update_platform(gMarioState)
#define update_mario_platform_1 update_mario_platform()
#define update_mario_platform(...) CAT(update_mario_platform_, N_ARGS(__VA_ARGS__))

// find_unimportant_object is redefined in omm_object_helpers.c
#define find_unimportant_object_0 find_unimportant_object()
#define find_unimportant_object_1 find_unimportant_object_unused()
#define find_unimportant_object(...) CAT(find_unimportant_object_, N_ARGS(__VA_ARGS__))

// cur_obj_within_12k_bounds always returns TRUE
#define cur_obj_within_12k_bounds_0 TRUE
#define cur_obj_within_12k_bounds_1 cur_obj_within_12k_bounds_unused()
#define cur_obj_within_12k_bounds(...) CAT(cur_obj_within_12k_bounds_, N_ARGS(__VA_ARGS__))

// cur_obj_is_mario_ground_pounding_platform now works with any type of ground pound
#define cur_obj_is_mario_ground_pounding_platform_0 (gMarioObject->platform == o && omm_mario_is_ground_pound_landing(gMarioState))
#define cur_obj_is_mario_ground_pounding_platform_1 cur_obj_is_mario_ground_pounding_platform(void)
#define cur_obj_is_mario_ground_pounding_platform(...) CAT(cur_obj_is_mario_ground_pounding_platform_, N_ARGS(__VA_ARGS__))

// check_instant_warp is replaced by omm_check_instant_warp
#define check_instant_warp_0 omm_check_instant_warp()
#define check_instant_warp_1 check_instant_warp()
#define check_instant_warp(...) CAT(check_instant_warp_, N_ARGS(__VA_ARGS__))

// Update the number of defeated enemies
#define unload_deactivated_objects_0 omm_stats_update_defeated_enemies()
#define unload_deactivated_objects_1 \
omm_stats_update_defeated_enemies() { \
    for_each_object_in_interaction_lists(obj) { \
        if (omm_obj_is_enemy_defeated(obj)) { \
            omm_stats_increase(enemiesDefeated, 1); \
        } \
    } \
    extern void _unload_deactivated_objects(); \
    _unload_deactivated_objects(); \
} void _unload_deactivated_objects()
#define unload_deactivated_objects(...) CAT(unload_deactivated_objects_, N_ARGS(__VA_ARGS__))

// Trigger a level exit in Yoshi mode for star and death exits
// Star Road: if the player dies as Zero-life Yoshi, return to the Zero-life area
// TODO: YOSHIMODE
// #if OMM_GAME_IS_SMSR
// #define check_return_to_zero_life() { \
//     if (gOmmGlobals->booZeroLife && sDelayedWarpOp == WARP_OP_DEATH) { \
//         sDelayedWarpOp = WARP_OP_NONE; \
//         gOmmGlobals->yoshiMode = false; \
//         gOmmGlobals->booZeroLife = false; \
//         initiate_warp(LEVEL_ZERO_LIFE, 1, 0x0A, 0); \
//         return; \
//     } \
// }
// #else
// #define check_return_to_zero_life()
// #endif
// #define reset_dialog_render_state_0 \
// reset_dialog_render_state(); \
// if (gOmmGlobals->yoshiMode && (sDelayedWarpOp == WARP_OP_STAR_EXIT || sDelayedWarpOp == WARP_OP_DEATH)) { \
//     check_return_to_zero_life(); \
//     if (omm_exit_level(gCurrLevelNum, gCurrAreaIndex, true)) { \
//         sDelayedWarpOp = WARP_OP_NONE; \
//         return; \
//     } \
// }
// #define reset_dialog_render_state_1 reset_dialog_render_state()
// #define reset_dialog_render_state(...) CAT(reset_dialog_render_state_, N_ARGS(__VA_ARGS__))

// Star Road: Don't print the Hard Mode strings
#if OMM_GAME_IS_SMSR
#define print_hard_mode_strings_0
#define print_hard_mode_strings_1 print_hard_mode_strings(void)
#define print_hard_mode_strings(...) CAT(print_hard_mode_strings_, N_ARGS(__VA_ARGS__))
#endif

// If the boss sequence is missing, the level music is not going to restart...
// So, add the boss music in the queue to force the stop_background_music
// function to remove it from the queue and restart the level music.
#define stop_background_music_fix_boss_music() \
if ((seqId & 0xFF) == SEQ_EVENT_BOSS) { \
    for (u8 i = 0; i <= sBackgroundMusicQueueSize; ++i) { \
        if (i == sBackgroundMusicQueueSize) { \
            sBackgroundMusicQueueSize = min_s(sBackgroundMusicQueueSize + 1, MAX_BG_MUSIC_QUEUE_SIZE); \
            mem_mov(sBackgroundMusicQueue + 1, sBackgroundMusicQueue, sizeof(sBackgroundMusicQueue[0]) * (MAX_BG_MUSIC_QUEUE_SIZE - 1)); \
            sBackgroundMusicQueue[0].seqId = SEQ_EVENT_BOSS; \
            sBackgroundMusicQueue[0].priority = 4; \
            break; \
        } else if (sBackgroundMusicQueue[i].seqId == SEQ_EVENT_BOSS) { \
            break; \
        } \
    } \
}

// Compute the lower bound before resetting gDialogScrollOffsetY to not break frame interpolation
#define fix_dialog_box_text_lower_bound() \
lowerBound = (gDialogScrollOffsetY / 16) + 1; \
if (gDialogScrollOffsetY >= dialog->linesPerBox * DIAG_VAL1) { \
    gDialogTextPos = gLastDialogPageStrPos; \
    gDialogBoxState = 1; \
    gDialogScrollOffsetY = 0; \
} \
break

// Keep heldObj, riddenObj, Cappy, capture and their children alive when unloading the area
#define obj_keep_held_ridden_capture_alive() \
for (bool ok = false; !ok;) { ok = true; \
    for (s32 objList = 0; objList != NUM_OBJ_LISTS; ++objList) { \
        for_each_object_in_list(obj, objList) { \
            if (!(obj->oFlags & OBJ_FLAG_UPDATE_AREA_INDEX) && ( \
                obj == gMarioState->riddenObj || \
                obj == gMarioState->heldObj || \
                obj == omm_cappy_get_object() || \
                obj == gOmmCapture || \
                obj == gOmmObject->state.heldObj || \
                (obj->parentObj && (obj->parentObj->oFlags & OBJ_FLAG_UPDATE_AREA_INDEX))) \
            ) { \
                obj->oActiveAreaIndex = areaIndex - 1; \
                obj->oFlags |= OBJ_FLAG_UPDATE_AREA_INDEX; \
                ok = false; \
            } \
        } \
    } \
}

// Fix annoying crashes that can occur in rooms with paintings
#define geo_painting_update_fix_floor() \
gLastPaintingUpdateCounter = gPaintingUpdateCounter; \
gPaintingUpdateCounter = gAreaUpdateCounter; \
extern struct MarioState *gMarioState; \
if (gMarioState->floor) { \
    surface = gMarioState->floor; \
} else { \
    find_floor(gMarioObject->oPosX, gMarioObject->oPosY, gMarioObject->oPosZ, &surface); \
} \
if (surface) { \
    gPaintingMarioFloorType = surface->type; \
    gPaintingMarioXPos = gMarioObject->oPosX; \
    gPaintingMarioYPos = gMarioObject->oPosY; \
    gPaintingMarioZPos = gMarioObject->oPosZ; \
} \
return NULL;

// Call omm_camera_update and return if TRUE
#ifdef INCLUDED_FROM_CAMERA_C
#define update_camera \
update_camera(struct Camera *c) { \
    gOmmGlobals->cameraUpdate = true; \
    if (!omm_camera_update(c, gMarioState)) { \
        extern void update_camera_(struct Camera *c); \
        update_camera_(c); \
    } \
    gOmmGlobals->cameraUpdate = false; \
} \
void update_camera_
#endif

// For convenience
#define play_buzz_sound() \
    play_sound(SOUND_MENU_CAMERA_BUZZ | 0xFF00, gGlobalSoundArgs)

#endif

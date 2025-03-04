#ifndef OMM_INCLUDES_H
#define OMM_INCLUDES_H

// Required headers //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <SDL2/SDL.h>                       // SDL stuff
#include "math_util.h"                      // Math stuff
#include "game/camera.h"                    // struct Camera
#include "game/level_update.h"              // HUD_DISPLAY_* flags
#include "game/object_list_processor.h"     // OBJECT_POOL_CAPACITY, bhv_mario_update
#include "stb/stb_image.h"                  // stb_image_* functions
#include "stb/stb_image_write.h"            // stb_image_write_* functions

// OMM headers ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "data/omm/omm_macros.h"
#include "data/omm/omm_defines.h"
#include "data/omm/omm_patches.h"
#include "data/omm/system/omm_memory.h"
#include "data/omm/system/omm_system.h"
#include "data/omm/system/omm_render.h"
#include "data/omm/system/omm_options.h"
#include "data/omm/engine/omm_engine.h"
#include "data/omm/object/omm_object.h"
#include "data/omm/object/omm_object_data.h"
#include "data/omm/object/omm_object_fields.h"
#include "data/omm/object/omm_behavior_data.h"
#include "data/omm/models/omm_models.h"
#include "data/omm/mario/omm_mario.h"
#include "data/omm/mario/omm_mario_actions.h"
#include "data/omm/cappy/omm_cappy.h"
#include "data/omm/cappy/omm_cappy_data.h"
#include "data/omm/capture/omm_capture_all.h"
#include "data/omm/peachy/omm_peach.h"
#include "data/omm/peachy/omm_peach_actions.h"
#include "data/omm/peachy/omm_perry.h"
#include "data/omm/peachy/omm_sparkly.h"
#include "data/omm/time_trials/time_trials.h"

// All headers ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define OMM_ALL_HEADERS before including this file
// to include all headers, not only the OMM ones
#ifdef OMM_ALL_HEADERS
#include "sm64.h"
#include "behavior_data.h"
#include "config.h"
#include "course_table.h"
#include "dialog_ids.h"
#include "geo_commands.h"
#include "gfx_dimensions.h"
#include "level_misc_macros.h"
#include "macro_preset_names.h"
#include "special_preset_names.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "actors/common0.h"
#include "actors/common1.h"
#include "actors/group0.h"
#include "actors/group1.h"
#include "actors/group2.h"
#include "actors/group3.h"
#include "actors/group4.h"
#include "actors/group5.h"
#include "actors/group6.h"
#include "actors/group7.h"
#include "actors/group8.h"
#include "actors/group9.h"
#include "actors/group10.h"
#include "actors/group11.h"
#include "actors/group12.h"
#include "actors/group13.h"
#include "actors/group14.h"
#include "actors/group15.h"
#include "actors/group16.h"
#include "actors/group17.h"
#include "game/behavior_actions.h"
#include "game/camera.h"
#include "game/game_init.h"
#include "game/geo_misc.h"
#include "game/hud.h"
#include "game/ingame_menu.h"
#include "game/interaction.h"
#include "game/level_geo.h"
#include "game/mario.h"
#include "game/mario_actions_airborne.h"
#include "game/mario_actions_automatic.h"
#include "game/mario_actions_cutscene.h"
#include "game/mario_actions_moving.h"
#include "game/mario_actions_object.h"
#include "game/mario_actions_stationary.h"
#include "game/mario_actions_submerged.h"
#include "game/mario_misc.h"
#include "game/moving_texture.h"
#include "game/obj_behaviors.h"
#include "game/object_helpers.h"
#include "game/print.h"
#include "game/save_file.h"
#include "game/screen_transition.h"
#include "game/segment2.h"
#include "game/sound_init.h"
#include "game/spawn_object.h"
#include "game/spawn_sound.h"
#include "audio/external.h"
#include "audio/load.h"
#include "menu/file_select.h"
#include "menu/star_select.h"
#include "levels/intro/header.h"
#include "levels/bowser_2/header.h"
#include "levels/ending/header.h"
#include "levels/menu/header.h"
#include "levels/scripts.h"
#include "pc/pc_main.h"
#include "pc/cliopts.h"
#include "pc/configfile.h"
#include "pc/fs/dirtree.h"
#include "pc/gfx/gfx_pc.h"
#include "pc/gfx/gfx_cc.h"
#include "pc/gfx/gfx_screen_config.h"
#include "pc/gfx/gfx_window_manager_api.h"
#include "pc/gfx/gfx_rendering_api.h"
#include "pc/controller/controller_api.h"
#include "pc/controller/controller_sdl.h"
#include "pc/controller/controller_keyboard.h"
#include "pc/controller/controller_mouse.h"
#include FILE_OPTIONS_H
#include FILE_SOUNDS_H
#include FILE_CHEATS_H
#include FILE_MARIO_CHEATS_H
#include FILE_TITLE_H
#include FILE_BETTERCAM_H
#include FILE_TXT_CONV_H
#include FILE_TEXT_LOADER_H
#include FILE_R96_SYSTEM_H
#endif

// SM64 globals //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern s8 gDialogBoxState;
extern s8 gDialogBoxType;
extern s8 gDialogLineNum;
extern u8 gDialogCharWidths[];
extern u8 sCutsceneDialogResponse;
extern u8 sFramesSinceCutsceneEnded;
extern u8 sObjectCutscene;
extern u8 sSpawnTypeFromWarpBhv[];
extern u8 texture_hud_char_puppycam[];
extern u8 *gEndCutsceneStringsEn[];
extern s16 gCutsceneMsgIndex;
extern s16 gCutsceneTimer;
extern s16 gDialogID;
extern s16 gDialogTextPos;
extern s16 gDialogScrollOffsetY;
extern s16 gMenuMode;
extern s16 sCutsceneShot;
extern s16 sStatusFlags;
extern s16 sYawSpeed;
extern u16 gAcousticReachPerLevel[];
extern s32 sNumActiveFirePiranhaPlants;
extern s32 sNumKilledFirePiranhaPlants;
extern u32 unused80339F10;
extern u32 gKeyPressed;
extern u64 sCapFlickerFrames;
extern f32 gDialogBoxOpenTimer;
extern f32 gDialogBoxScale;
extern struct TransitionInfo sModeTransition;
extern struct PlayerGeometry sMarioGeometry;
extern struct PlayerCameraState *sMarioCamState;
extern const BehaviorScript *sWarpBhvSpawnTable[];
extern const char SM64_TO_ASCII_CHAR[];
extern const bool IS_BOWSER_INTERACTABLE[];

// SM64 functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern s32 analog_stick_held_back(struct MarioState *m);
extern s32 apply_slope_decel(struct MarioState *m, f32);
extern s32 begin_braking_action(struct MarioState *m);
extern s32 check_ground_dive_or_punch(struct MarioState *m);
extern s32 check_horizontal_wind(struct MarioState *m);
extern s32 check_object_step(struct Object *o, s32 step, u32 flags);
extern s32 lava_boost_on_wall(struct MarioState *m);
extern s32 perform_hang_step(struct MarioState *m);
extern s32 perform_water_step(struct MarioState *m);
extern s32 perform_object_step(struct Object *o, u32 flags);
extern s32 should_begin_sliding(struct MarioState *m);
extern u32 common_air_action_step(struct MarioState *, u32, s32, u32);
extern u32 determine_interaction(struct MarioState *m, struct Object *o);
extern u32 should_push_or_pull_door(struct MarioState *m, struct Object *o);
extern f32 get_buoyancy(struct MarioState *m);
extern void apply_slope_accel(struct MarioState *m);
extern void bhv_chain_chomp_update_chain_parts(struct Object *o, bool isFreed);
extern void bhv_hidden_red_coin_star_init_();
extern void bhv_hidden_red_coin_star_loop_();
extern void check_death_barrier(struct MarioState *m);
extern void check_lava_boost(struct MarioState *m);
extern void check_ledge_climb_down(struct MarioState *m);
extern void create_dl_rotation_matrix(s8 pushOp, f32 a, f32 x, f32 y, f32 z);
extern void create_dl_scale_matrix(s8 pushOp, f32 x, f32 y, f32 z);
extern void cutscene_ending_look_at_sky(struct Camera *c);
extern void cutscene_ending_mario_fall(struct Camera *c);
extern void cutscene_ending_mario_land(struct Camera *c);
extern void cutscene_ending_mario_land_closeup(struct Camera *c);
extern void cutscene_ending_reset_spline(struct Camera *c);
extern void cutscene_ending_stars_free_peach(struct Camera *c);
extern void cutscene_ending_stop(struct Camera *c);
extern void cutscene_ending_zoom_fov(struct Camera *c);
extern void cutscene_enter_painting(struct Camera *c);
extern void cutscene_exit_painting_end(struct Camera *c);
extern void debug_surface_list_info(f32 x, f32 z);
extern void handle_power_meter_actions(s16);
extern void handle_special_dialog_text(s16 dialogID);
extern void initiate_warp(s16 destLevel, s16 destArea, s16 destWarpNode, s32 warpArg);
extern void mario_reset_bodystate(struct MarioState *m);
extern void mario_update_hitbox_and_cap_model(struct MarioState *m);
extern void monty_mole_spawn_dirt_particles(s8 offsetY, s8 velYBase);
extern void obj_orient_graph(struct Object *obj, f32 normalX, f32 normalY, f32 normalZ);
extern void play_flip_sounds(struct MarioState *m, s16 frame1, s16 frame2, s16 frame3);
extern void play_sequence(u8, u8, u16);
extern void pss_begin_slide(struct MarioState *m);
extern void pss_end_slide(struct MarioState *m);
extern void push_mario_out_of_object(struct MarioState *, struct Object *, f32);
extern void push_or_sidle_wall(struct MarioState *m, Vec3f startPos);
extern void render_dialog_box_type(struct DialogEntry *dialog, s8 linesPerBox);
extern void set_mario_initial_action(struct MarioState *, u32, u32);
extern void set_play_mode(s16);
extern void set_submerged_cam_preset_and_spawn_bubbles(struct MarioState *m);
extern void sink_mario_in_quicksand(struct MarioState *m);
extern void squish_mario_model(struct MarioState *m);
extern void stop_cutscene(struct Camera *c);
extern void tilt_body_ground_shell(struct MarioState *m, s16 startYaw);
extern void tilt_body_walking(struct MarioState *m, s16 startYaw);
extern void update_air_with_turn(struct MarioState *m);
extern void update_air_without_turn(struct MarioState *m);
extern void update_lakitu(struct Camera *c);
extern void update_mario_health(struct MarioState *m);
extern void update_mario_info_for_cam(struct MarioState *m);
extern void update_mario_inputs(struct MarioState *m);
extern void update_walking_speed(struct MarioState *m);
extern void warp_special(s32 arg);

#endif // OMM_INCLUDES_H

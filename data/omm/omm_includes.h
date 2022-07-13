#ifndef OMM_INCLUDES_H
#define OMM_INCLUDES_H

// Required headers
#include <math.h>
#include <limits.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "libc/math.h"
#include "game/memory.h"
#include "game/camera.h"
#include "game/ingame_menu.h"
#include "game/mario_step.h"
#include "game/object_list_processor.h"
#include "pc/fs/fs.h"
#include "pc/configfile.h"
#include "stb/stb_image.h"

// OMM headers
#include "data/omm/omm_macros.h"
#include "data/omm/omm_defines.h"
#include "data/omm/omm_patches.h"
#include "data/omm/system/omm_memory.h"
#include "data/omm/system/omm_system.h"
#include "data/omm/engine/omm_engine.h"
#include "data/omm/object/omm_object.h"
#include "data/omm/mario/omm_mario.h"
#include "data/omm/mario/omm_mario_actions.h"
#include "data/omm/cappy/omm_cappy.h"
#include "data/omm/capture/omm_capture_all.h"
#include "data/omm/peachy/omm_sparkly.h"
#include "data/omm/peachy/omm_peach.h"
#include "data/omm/peachy/omm_peach_actions.h"

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
#include "level_commands.h"
#include "level_misc_macros.h"
#include "macro_preset_names.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "special_preset_names.h"
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
#include "game/game_init.h"
#include "game/geo_misc.h"
#include "game/hud.h"
#include "game/interaction.h"
#include "game/level_geo.h"
#include "game/level_update.h"
#include "game/mario.h"
#include "game/mario_actions_airborne.h"
#include "game/mario_actions_automatic.h"
#include "game/mario_actions_cutscene.h"
#include "game/mario_actions_moving.h"
#include "game/mario_actions_object.h"
#include "game/mario_actions_stationary.h"
#include "game/mario_actions_submerged.h"
#include "game/mario_misc.h"
#include "game/mario_step.h"
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
#include "levels/intro/header.h"
#include "levels/bowser_2/header.h"
#include "levels/ending/header.h"
#include "levels/menu/header.h"
#include "levels/scripts.h"
#include "pc/pc_main.h"
#include "pc/cliopts.h"
#include "pc/platform.h"
#include "pc/configfile.h"
#include "pc/gfx/gfx_pc.h"
#include "pc/gfx/gfx_cc.h"
#include "pc/gfx/gfx_screen_config.h"
#include "pc/gfx/gfx_window_manager_api.h"
#include "pc/gfx/gfx_rendering_api.h"
#include "pc/controller/controller_api.h"
#include "pc/controller/controller_sdl.h"
#include "pc/controller/controller_keyboard.h"
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

// SM64 globals
extern s8 gDialogBoxState;
extern s8 gDialogLineIndex;
extern u8 sFramesSinceCutsceneEnded;
extern u8 sSpawnTypeFromWarpBhv[];
extern u8 gDialogCharWidths[];
extern u8 texture_hud_char_puppycam[];
extern u8 *gEndCutsceneStringsEn[];
extern s16 gCutsceneMsgIndex;
extern s16 gDialogID;
extern s16 gMenuMode;
extern s16 g1HPMode;
extern s16 sStatusFlags;
extern s16 sYawSpeed;
extern u16 sAcousticReachPerLevel[];
extern s32 gFindFloorForCutsceneStar;
extern s32 sNumKilledFirePiranhaPlants;
extern u32 gKeyPressed;
extern f32 gDialogBoxAngle;
extern const Gfx null[];
extern const bool gIsBowserInteractible[];
extern const BehaviorScript *sWarpBhvSpawnTable[];
extern struct PlayerGeometry sMarioGeometry;
extern struct PlayerCameraState *sMarioCamState;

// SM64 functions
extern s32 analog_stick_held_back(struct MarioState *m);
extern s32 apply_slope_decel(struct MarioState *m, f32);
extern s32 begin_braking_action(struct MarioState *m);
extern s32 check_ground_dive_or_punch(struct MarioState *m);
extern s32 check_horizontal_wind(struct MarioState *m);
extern s32 lava_boost_on_wall(struct MarioState *);
extern s32 perform_hang_step(struct MarioState *m);
extern s32 perform_water_step(struct MarioState *m);
extern s32 perform_object_step(struct Object *o, u32 flags);
extern s32 should_begin_sliding(struct MarioState *m);
extern u32 common_air_action_step(struct MarioState *, u32, s32, u32);
extern u32 determine_interaction(struct MarioState *m, struct Object *o);
extern u32 interact_cap(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_coin(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_flame(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_warp(struct MarioState *m, u32 interactType, struct Object *o);
extern f32 get_buoyancy(struct MarioState *m);
extern void apply_slope_accel(struct MarioState *m);
extern void bhv_chain_chomp_update_chain_parts(struct Object *o, bool isFreed);
extern void check_ledge_climb_down(struct MarioState *m);
extern void create_dl_rotation_matrix(s8 pushOp, f32 a, f32 x, f32 y, f32 z);
extern void create_dl_scale_matrix(s8 pushOp, f32 x, f32 y, f32 z);
extern void cutscene_ending_look_at_sky(struct Camera *);
extern void cutscene_ending_mario_fall(struct Camera *);
extern void cutscene_ending_mario_land(struct Camera *);
extern void cutscene_ending_mario_land_closeup(struct Camera *);
extern void cutscene_ending_reset_spline(struct Camera *);
extern void cutscene_ending_stars_free_peach(struct Camera *);
extern void cutscene_ending_stop(struct Camera *);
extern void cutscene_ending_zoom_fov(struct Camera *);
extern void debug_surface_list_info(f32 x, f32 z);
extern void handle_power_meter_actions(s16);
extern void initiate_warp(s16, s16, s16, s32);
extern void mario_reset_bodystate(struct MarioState *m);
extern void mario_update_hitbox_and_cap_model(struct MarioState *m);
extern void monty_mole_spawn_dirt_particles(s8 offsetY, s8 velYBase);
extern void obj_orient_graph(struct Object *obj, f32 normalX, f32 normalY, f32 normalZ);
extern void play_flip_sounds(struct MarioState *m, s16 frame1, s16 frame2, s16 frame3);
extern void play_sequence(u8, u8, u16);
extern void push_mario_out_of_object(struct MarioState *, struct Object *, f32);
extern void push_or_sidle_wall(struct MarioState *m, Vec3f startPos);
extern void render_dialog_box_type(struct DialogEntry *dialog, s8 linesPerBox);
extern void run_display_list(struct SPTask *spTask);
extern void set_mario_initial_action(struct MarioState *, u32, u32);
extern void set_play_mode(s16);
extern void set_submerged_cam_preset_and_spawn_bubbles(struct MarioState *m);
extern void sink_mario_in_quicksand(struct MarioState *m);
extern void spawn_particle(u32 activeParticleFlag, s16 model, const BehaviorScript *behavior);
extern void squish_mario_model(struct MarioState *m);
extern void tilt_body_walking(struct MarioState *m, s16 startYaw);
extern void update_lakitu(struct Camera *c);
extern void update_mario_health(struct MarioState *);
extern void update_mario_info_for_cam(struct MarioState *m);
extern void update_mario_inputs(struct MarioState *m);
extern void update_walking_speed(struct MarioState *m);
extern void warp_special(s32 arg);

#endif // OMM_INCLUDES_H

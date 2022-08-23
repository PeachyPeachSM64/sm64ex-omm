#ifndef OMM_SYSTEM_H
#define OMM_SYSTEM_H

#include "types.h"
#include "pc/fs/fs.h"
#include "game/mario_step.h" // declare get_additive_y_vel_for_jumps before macro-ing it

//
// System
//

void omm_add_routine(s32 type, void (*func)(void));
void omm_select_save_file(s32 saveFileNum);
void omm_return_to_main_menu();
void omm_update();
void omm_pre_render();
void *omm_update_cmd(void *cmd, s32 reg);
bool omm_is_main_menu();
bool omm_is_game_paused();
bool omm_is_transition_active();
bool omm_is_ending_cutscene();
bool omm_is_ending_cake_screen();
void omm_speedrun_split(s32 numStars);

#if OMM_GAME_IS_SM74
void omm_opt_sm74_change_mode(UNUSED void *opt, s32 arg);
#define sm74_mode__omm_save                     ((omm_is_main_menu() ? (s32) sWarpDest.areaIdx : (s32) gCurrAreaIndex) - 1)
#define sm74_mode__omm_level_script             sWarpDest.areaIdx
#define sm74_mode__omm_opt_init_warp_to_level   sWarpDest.areaIdx
#define sm74_mode__omm_stars_get_bits_total     sWarpDest.areaIdx
#define sm74_mode__omm_level_get_name           sWarpDest.areaIdx
#define sm74_mode__omm_level_get_act_name       sWarpDest.areaIdx
#define sm74_mode__omm_render_pause_castle      gCurrAreaIndex
#define sm74_mode__omm_dialog_get_table         gCurrAreaIndex
#endif

//
// Profiling
//

#define OMM_PRF_OMM 0 // Count the elapsed time spent by omm_update().
#define OMM_PRF_LVL 1 // Count the elapsed time spent by the level script update.
#define OMM_PRF_PRE 2 // Count the elapsed time spent by omm_pre_render().
#define OMM_PRF_GEO 3 // Count the elapsed time spent by render_game().
#define OMM_PRF_GFX 4 // Count the elapsed time spent by gfx_run_dl() during 1 game update (1/30 second).
#define OMM_PRF_RDR 5 // Count the elapsed time spent by the scene rendering during 1 game update (1/30 second).
#define OMM_PRF_FRM 6 // Count the elapsed time between every call to gfx_start_frame() and gfx_end_frame() during 1 game update (1/30 second).
#define OMM_PRF_FPS 7 // Count the elapsed time between 2 successive game updates. Used to compute the actual frame rate.
#define OMM_PRF_MAX 8

void omm_profiler_start(s32 prf);
void omm_profiler_stop(s32 prf);
void omm_profiler_start_gfx_op(u8 op);
void omm_profiler_stop_gfx_op(u8 op);
void omm_profiler_frame_drawn();
void omm_profiler_display();

//
// Player
//

bool omm_player_is_unlocked(s32 index);
bool omm_player_is_selected(s32 index);
void omm_player_select(s32 index);
s32  omm_player_get_selected_index();

const char *omm_player_properties_get_name(s32 index);
u32 omm_player_properties_get_color(s32 index);
const void *omm_player_properties_get_anims(s32 index);

f32 omm_player_physics_get_ground(s32 index);
f32 omm_player_physics_get_air(s32 index);
f32 omm_player_physics_get_swim(s32 index);
f32 omm_player_physics_get_jump(s32 index);
f32 omm_player_physics_get_gravity(s32 index);
f32 omm_player_physics_get_slide(s32 index);

s32 omm_player_graphics_get_model(s32 index);
s32 omm_player_graphics_get_normal_cap(s32 index);
s32 omm_player_graphics_get_wing_cap(s32 index);
s32 omm_player_graphics_get_metal_cap(s32 index);
s32 omm_player_graphics_get_winged_metal_cap(s32 index);
s32 omm_player_graphics_get_cap(s32 index, bool wing, bool metal);

#define __OMM_PLAYER_SELECTED_INDEX__                           (omm_peach_vibe_is_active() ? (gOmmPeach->vibeType + OMM_NUM_PLAYABLE_CHARACTERS - 1) : omm_player_get_selected_index())

#define omm_player_properties_get_selected_name()               omm_player_properties_get_name(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_properties_get_selected_color()              omm_player_properties_get_color(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_properties_get_selected_anims()              omm_player_properties_get_anims(__OMM_PLAYER_SELECTED_INDEX__)

#define omm_player_physics_get_selected_ground()                omm_player_physics_get_ground(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_physics_get_selected_air()                   omm_player_physics_get_air(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_physics_get_selected_swim()                  omm_player_physics_get_swim(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_physics_get_selected_jump()                  omm_player_physics_get_jump(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_physics_get_selected_gravity()               omm_player_physics_get_gravity(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_physics_get_selected_slide()                 omm_player_physics_get_slide(__OMM_PLAYER_SELECTED_INDEX__)

#define omm_player_graphics_get_selected_model()                omm_player_graphics_get_model(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_graphics_get_selected_normal_cap()           omm_player_graphics_get_normal_cap(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_graphics_get_selected_wing_cap()             omm_player_graphics_get_wing_cap(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_graphics_get_selected_metal_cap()            omm_player_graphics_get_metal_cap(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_graphics_get_selected_winged_metal_cap()     omm_player_graphics_get_winged_metal_cap(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_graphics_get_selected_cap(wing, metal)       omm_player_graphics_get_cap(__OMM_PLAYER_SELECTED_INDEX__, wing, metal)

#define get_additive_y_vel_for_jumps()                          ((initialVelY + (m->forwardVel * multiplier)) * (omm_player_physics_get_selected_jump() - 1.f))

//
// Health
//

s32  omm_health_odyssey(s32 health);
s32  omm_health_from_ticks(s32 ticks);
s32  omm_health_to_ticks(s32 health);
s32  omm_health_get_max(s32 health);
s32  omm_health_get_max_hud(s32 health);
s32  omm_health_get_max_ticks(s32 ticks);
s32  omm_health_get_max_ticks_hud(s32 ticks);
f32  omm_health_get_life_up_gauge_position(s32 ticks);
f32  omm_health_get_life_gauge_rumble_strength(struct MarioState *m);
void omm_health_update(struct MarioState *m);
void omm_health_heal_mario(struct MarioState *m);
void omm_health_fully_heal_mario(struct MarioState *m);
void omm_health_life_up(struct MarioState *m);
void omm_health_set_max(struct MarioState *m);
bool omm_health_is_at_max(struct MarioState *m);

//
// Text
//

u8 *omm_text_convert(const char *str, bool heapAlloc);
u8 *omm_text_copy(const u8 *str64, bool heapAlloc);
u8 *omm_text_capitalize(u8 *str64);
u8 *omm_text_decapitalize(u8 *str64);
u8 *omm_text_replace_char(u8 *str64, u8 from, u8 to);
u8 *omm_text_replace_names(u8 *str64);
s32 omm_text_length(const u8 *str64);
struct DialogEntry **omm_dialog_get_table();
struct DialogEntry *omm_dialog_get_entry(void **dialogTable, s16 dialogId, s32 mode);

//
// Camera
//

void omm_camera_init();
bool omm_camera_update(struct Camera *c, struct MarioState *m);
bool omm_camera_is_available(struct MarioState *m);
s16  omm_camera_get_intended_yaw(struct MarioState *m);
s32  omm_camera_get_relative_dist_mode();
void omm_camera_warp(struct Camera *c, f32 dx, f32 dy, f32 dz);

//
// Audio
//

u8  *omm_audio_mix(u8 *output, const u8 *input, s32 length, s32 volume, s32 distance);
s32  omm_audio_resize(u8 **output, const u8 *input, s32 inputLength, s32 desiredLength);
s32  omm_audio_resample(u8 **output, const u8 *input, s32 inputLength, f32 outputScale);
s32  omm_audio_resample_fast(u8 **output, const u8 *input, s32 inputLength, f32 outputScale);
s32  omm_audio_time_stretch(u8 **output, const u8 *input, s32 inputLength, s32 audioFreq, f32 timeStretch);
s32  omm_audio_pitch_shift(u8 **output, const u8 *input, s32 inputLength, s32 audioFreq, f32 pitchScale);

void omm_audio_init();
void omm_sound_play(s32 id, f32 *pos);
void omm_sound_stop(s32 id);
bool omm_sound_is_playing(s32 id);
bool omm_sound_is_bank_playing(u8 bank);
bool omm_sound_play_character_sound_n64(s32 id, f32 *pos);
bool omm_sound_play_character_sound_omm(s32 id, f32 *pos);
bool omm_sound_play_character_sound_r96(const char *id, f32 *pos);
bool omm_sound_stop_character_sound_n64(s32 id, f32 *pos);
bool omm_sound_stop_character_sound_omm(s32 id, f32 *pos);
bool omm_sound_stop_character_sound_r96(const char *id, f32 *pos);

//
// Level
//

s32  omm_level_get_count();
s32 *omm_level_get_list();
s32  omm_level_get_course(s32 level);
s32  omm_level_get_areas(s32 level);
s32  omm_level_get_num_red_coins(s32 level, s32 area);
u8  *omm_level_get_name(s32 level, bool decaps, bool num);
u8  *omm_level_get_act_name(s32 level, s32 act, bool decaps, bool num);
s16 *omm_level_get_warp(s32 level, s32 area, u8 id);
s16 *omm_level_get_entry_warp(s32 level, s32 area);
s16 *omm_level_get_exit_warp(s32 level, s32 area);
s16 *omm_level_get_death_warp(s32 level, s32 area);
bool omm_level_can_warp(s32 level);
const LevelScript *omm_level_get_script(s32 level);

//
// Warps
//

bool omm_warp_to_level(s32 level, s32 area, s32 act);
bool omm_restart_level();
bool omm_restart_area();
bool omm_exit_level();
bool omm_return_to_castle(bool fadeOut, bool force);
bool omm_is_warping();
void *omm_update_warp(void *cmd, bool inited);

//
// Palette editor
//

extern s32 gOmmPaletteEditorState;
enum {
    OMM_PALETTE_EDITOR_STATE_CLOSED,
    OMM_PALETTE_EDITOR_STATE_OPENING,
    OMM_PALETTE_EDITOR_STATE_OPEN,
    OMM_PALETTE_EDITOR_STATE_CLOSING,
};

extern const LevelScript omm_level_palette_editor[];
void omm_palette_editor_open();
void omm_palette_editor_close();

#endif // OMM_SYSTEM_H

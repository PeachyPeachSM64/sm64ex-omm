#ifndef OMM_SYSTEM_H
#define OMM_SYSTEM_H

#include "types.h"
#include "pc/fs/fs.h"
#include "game/mario_step.h" // declare get_additive_y_vel_for_jumps before macro-ing it

//
// System
//

void omm_add_routine(s32 type, void (*func)(void));
void omm_execute_routines(s32 type);
void omm_select_save_file(s32 fileIndex, s32 modeIndex, s32 courseNum, bool skipIntro);
void omm_return_to_main_menu();
void omm_update();
void omm_pre_render();
void *omm_update_cmd(void *cmd, s32 reg);
bool omm_is_main_menu();
bool omm_is_game_paused();
bool omm_is_transition_active();
bool omm_is_ending_cutscene();
bool omm_is_ending_cake_screen();
bool omm_is_warping_to_last_course();

//
// Profiling
//

#define OMM_PRF_OMM (0) // Count the elapsed time spent by omm_update().
#define OMM_PRF_LVL (1) // Count the elapsed time spent by the level script update.
#define OMM_PRF_PRE (2) // Count the elapsed time spent by omm_pre_render().
#define OMM_PRF_GEO (3) // Count the elapsed time spent by render_game().
#define OMM_PRF_GFX (4) // Count the elapsed time spent by gfx_run_dl() during 1 game update (1/30 second).
#define OMM_PRF_RDR (5) // Count the elapsed time spent by the scene rendering during 1 game update (1/30 second).
#define OMM_PRF_FRM (6) // Count the elapsed time between every call to gfx_start_frame() and gfx_end_frame() during 1 game update (1/30 second).
#define OMM_PRF_MAX (7)

void omm_profiler_start(s32 prf);
void omm_profiler_stop(s32 prf);
void omm_profiler_update_fps(f32 fps, u32 refreshRate);
void omm_profiler_display();

//
// Player
//

bool omm_player_is_unlocked(s32 playerIndex);
void omm_player_select(s32 playerIndex);
s32  omm_player_get_selected_index();
s32  omm_player_get_selected_index_model_and_sounds();

const char *omm_player_properties_get_name(s32 playerIndex);
const char *omm_player_properties_get_name_upper(s32 playerIndex);
const char *omm_player_properties_get_name_lower(s32 playerIndex);
u32 omm_player_properties_get_color(s32 playerIndex);
const void *omm_player_properties_get_anims(s32 playerIndex);

f32 omm_player_physics_get_ground(s32 playerIndex, bool isCapture);
f32 omm_player_physics_get_air(s32 playerIndex, bool isCapture);
f32 omm_player_physics_get_swim(s32 playerIndex, bool isCapture);
f32 omm_player_physics_get_jump(s32 playerIndex, bool isCapture);
f32 omm_player_physics_get_gravity(s32 playerIndex, bool isCapture);
f32 omm_player_physics_get_slide(s32 playerIndex, bool isCapture);

s32 omm_player_graphics_get_model(s32 playerIndex);
s32 omm_player_graphics_get_normal_cap(s32 playerIndex);
s32 omm_player_graphics_get_wing_cap(s32 playerIndex);
s32 omm_player_graphics_get_metal_cap(s32 playerIndex);
s32 omm_player_graphics_get_winged_metal_cap(s32 playerIndex);
s32 omm_player_graphics_get_cap(s32 playerIndex, bool wing, bool metal);

#define __OMM_PLAYER_SELECTED_INDEX__                           (omm_peach_vibe_is_active() ? (gOmmPeach->vibeType + OMM_NUM_PLAYABLE_CHARACTERS - 1) : omm_player_get_selected_index())

#define omm_player_properties_get_selected_name()               omm_player_properties_get_name(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_properties_get_selected_name_upper()         omm_player_properties_get_name_upper(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_properties_get_selected_name_lower()         omm_player_properties_get_name_lower(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_properties_get_selected_color()              omm_player_properties_get_color(__OMM_PLAYER_SELECTED_INDEX__)
#define omm_player_properties_get_selected_anims()              omm_player_properties_get_anims(__OMM_PLAYER_SELECTED_INDEX__)

#define omm_player_physics_get_selected_ground()                omm_player_physics_get_ground(__OMM_PLAYER_SELECTED_INDEX__, false)
#define omm_player_physics_get_selected_air()                   omm_player_physics_get_air(__OMM_PLAYER_SELECTED_INDEX__, false)
#define omm_player_physics_get_selected_swim()                  omm_player_physics_get_swim(__OMM_PLAYER_SELECTED_INDEX__, false)
#define omm_player_physics_get_selected_jump()                  omm_player_physics_get_jump(__OMM_PLAYER_SELECTED_INDEX__, false)
#define omm_player_physics_get_selected_gravity()               omm_player_physics_get_gravity(__OMM_PLAYER_SELECTED_INDEX__, false)
#define omm_player_physics_get_selected_slide()                 omm_player_physics_get_slide(__OMM_PLAYER_SELECTED_INDEX__, false)

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
void omm_health_update(struct MarioState *m);
void omm_health_heal_mario(struct MarioState *m, bool updateHealthState);
void omm_health_fully_heal_mario(struct MarioState *m, bool updateHealthState);
void omm_health_life_up(struct MarioState *m);
void omm_health_set_max(struct MarioState *m);
bool omm_health_is_at_max(struct MarioState *m);
bool omm_health_is_healing(struct MarioState *m);
bool omm_health_is_taking_damage(struct MarioState *m);
void omm_health_state_get_anim_params(s32 state, s32 timer, f32 *shakeX, f32 *shakeY, f32 *relPos);

//
// Text
//

u8 *omm_text_convert(const char *str, bool heapAlloc);
u8 *omm_text_duplicate(const u8 *str64, bool heapAlloc);
u8 *omm_text_copy(u8 *dest, s32 size, const u8 *str64);
u8 *omm_text_capitalize(u8 *str64);
u8 *omm_text_decapitalize(u8 *str64);
u8 *omm_text_replace_char(u8 *str64, u8 from, u8 to);
s32 omm_text_length(const u8 *str64);
s32 omm_text_compare(const u8 *str1, const u8 *str2);
u8 *omm_text_get_string_for_selected_player(u8 *str64);

//
// Dialog
//

struct OmmDialogEntry { s32 id; s32 soundBits; s32 linesPerBox; s32 leftOffset; s32 downOffset; const char *str; };
extern struct OmmDialogEntry gOmmDialogSigns[];
extern struct OmmDialogEntry gOmmDialogNPCs[];
extern struct OmmDialogEntry gOmmDialogBowser[];
extern struct OmmDialogEntry gOmmDialogCheats[];
extern struct OmmDialogEntry gOmmDialogSparklyStars1[];
extern struct OmmDialogEntry gOmmDialogSparklyStars2[];
extern struct OmmDialogEntry gOmmDialogSparklyStars3[];
struct DialogEntry *omm_dialog_get_entry(void **dialogTable, s16 dialogId);

//
// Camera
//

#define OMM_CAMERA_X (configCameraInvertX ? -1 : +1)
#define OMM_CAMERA_Y (configCameraInvertY ? -1 : +1)

void omm_camera_init();
bool omm_camera_update(struct Camera *c, struct MarioState *m);
void omm_camera_update_settings();
bool omm_camera_is_available(struct MarioState *m);
bool omm_camera_is_bowser_fight();
s16  omm_camera_get_intended_yaw(struct MarioState *m);
s32  omm_camera_get_relative_dist_mode();
void omm_camera_warp(struct Camera *c, Vec3f displacement);
bool omm_camera_snapshot_mode_init();
void omm_camera_snapshot_mode_update();

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
void omm_sound_play_with_priority(s32 id, f32 *pos, u8 priority);
void omm_sound_stop(s32 id);
bool omm_sound_is_playing(s32 id);
bool omm_sound_is_bank_playing(u8 bank);

bool omm_sound_play_character_sound_n64(s32 id, f32 *pos);
bool omm_sound_play_character_sound_omm(s32 id, f32 *pos);
bool omm_sound_play_character_sound_omm_with_priority(s32 id, f32 *pos, u8 priority);
bool omm_sound_play_character_sound_r96(const char *id, f32 *pos);
bool omm_sound_stop_character_sound_n64(s32 id, f32 *pos);
bool omm_sound_stop_character_sound_omm(s32 id, f32 *pos);
bool omm_sound_stop_character_sound_r96(const char *id, f32 *pos);

//
// Level
//

typedef struct {
    f32 x, y, z; s16 yaw;
    s32 srcLevelNum, srcAreaIndex, srcId, srcType;
    s32 dstLevelNum, dstAreaIndex, dstId;
} Warp;

s32   omm_level_get_count();
s32  *omm_level_get_list();
s32   omm_level_get_course(s32 levelNum);
s32   omm_level_from_course(s32 courseNum);
s32   omm_level_get_areas(s32 levelNum);
u64   omm_level_get_available_captures(s32 levelNum, s32 areaIndex);
u64   omm_level_get_all_available_captures(s32 modeIndex);
s32   omm_level_get_all_available_captures_count(s32 modeIndex);
u8   *omm_level_get_course_name(ustr_t dst, s32 levelNum, s32 modeIndex, bool decaps, bool num);
u8   *omm_level_get_act_name(ustr_t dst, s32 levelNum, s32 actNum, s32 modeIndex, bool decaps, bool num);
Warp *omm_level_get_warp(s32 levelNum, s32 areaIndex, s32 id);
Warp *omm_level_get_entry_warp(s32 levelNum, s32 areaIndex);
Warp *omm_level_get_exit_warp(s32 levelNum, s32 areaIndex);
Warp *omm_level_get_death_warp(s32 levelNum, s32 areaIndex);
bool  omm_level_can_warp(s32 levelNum);
const LevelScript *omm_level_get_script(s32 levelNum);

//
// Warps
//

bool omm_warp_to_level(s32 levelNum, s32 areaIndex, s32 actNum);
bool omm_restart_level();
bool omm_restart_area();
bool omm_exit_level(s32 levelNum, s32 areaIndex, bool instant);
bool omm_return_to_castle(bool fadeOut, bool force);
bool omm_is_warping();
void omm_process_instant_warp(struct MarioState *m, Vec3f displacement, s16 areaIndex);
void omm_check_instant_warp();
void *omm_update_warp(void *cmd, bool inited);

//
// Palette editor
//

extern const LevelScript omm_level_palette_editor[];
bool omm_palette_editor_is_opening();
bool omm_palette_editor_is_open();
bool omm_palette_editor_is_closing();
bool omm_palette_editor_is_closed();
void omm_palette_editor_open();
void omm_palette_editor_set_open();
void omm_palette_editor_close();
void omm_palette_editor_set_closed();
s32  omm_palette_editor_get_current_palette();

//
// Stats
//

typedef struct OmmStats {

    // Objects
    u64 starsCollected;
    u64 sparklyStarsCollected;
    u64 coinsCollected;
    u64 capsCollected;
    u64 mushrooms1upCollected;
    u64 secretsCollected;
    u64 exclamationBoxesBroken;
    u64 enemiesDefeated;
    u64 bowsersDefeated;

    // Actions
    u64 aPresses;
    u64 jumps;
    u64 attacks;
    u64 cappyThrows;
    u64 cappyBounces;
    u64 captures;
    u64 hitsTaken;
    u64 restarts;
    u64 deaths;

    // Distance (Mario/Capture)
    u64 distanceTotal[2];
    u64 distanceOnGround[2];
    u64 distanceAirborne[2];
    u64 distanceUnderwater[2];
    u64 distanceWingCap[2];
    u64 distanceMetalCap[2];
    u64 distanceVanishCap[2];

    // Time (Mario/Capture)
    u64 timeTotal[2];
    u64 timeOnGround[2];
    u64 timeAirborne[2];
    u64 timeUnderwater[2];
    u64 timeWingCap[2];
    u64 timeMetalCap[2];
    u64 timeVanishCap[2];

} OmmStats;

#define OMM_STATS_PREFIX_LOCAL  "stat_"
#define OMM_STATS_PREFIX_GLOBAL "stats_"

bool omm_stats_read(OmmStats *stats, const char *prefix, const char *name, const char *value1, const char *value2, bool *invalid);
void omm_stats_write(const OmmStats *stats, const char *prefix);
void omm_stats_reset(OmmStats *stats);

#define omm_stats_increase(stat, value) { \
    u64 inc = (u64) (value); \
    gOmmStats->stat += inc; \
    OmmStats *stats = omm_save_file_get_stats(gCurrSaveFileNum - 1, OMM_GAME_MODE); \
    if (stats) { stats->stat += inc; } \
}

//
// Rewards
//

#define OMM_REWARD_INSTANT_CAPS         (0x00)
#define OMM_REWARD_UNLIMITED_CAPS       (0x01)
#define OMM_REWARD_SUMMON_YOSHI         (0x02)
#define OMM_REWARD_SPARKLY_STARS        (0x03)
#define OMM_REWARD_SPARKLY_SPARKLES     (0x04)
#define OMM_REWARD_PLAYABLE_PEACH       (0x05)
#define OMM_REWARD_PERRY_CHARGE         (0x06)
#define OMM_REWARD__REDACTED_           (0x07)

#define OMM_REWARD_IS_INSTANT_CAPS_UNLOCKED         omm_rewards_is_unlocked(OMM_REWARD_INSTANT_CAPS, true)
#define OMM_REWARD_IS_UNLIMITED_CAPS_UNLOCKED       omm_rewards_is_unlocked(OMM_REWARD_UNLIMITED_CAPS, true)
#define OMM_REWARD_IS_SUMMON_YOSHI_UNLOCKED         omm_rewards_is_unlocked(OMM_REWARD_SUMMON_YOSHI, true)
#define OMM_REWARD_IS_SPARKLY_STARS_UNLOCKED        omm_rewards_is_unlocked(OMM_REWARD_SPARKLY_STARS, true)
#define OMM_REWARD_IS_SPARKLY_SPARKLES_UNLOCKED     omm_rewards_is_unlocked(OMM_REWARD_SPARKLY_SPARKLES, true)
#define OMM_REWARD_IS_PLAYABLE_PEACH_UNLOCKED       omm_rewards_is_unlocked(OMM_REWARD_PLAYABLE_PEACH, true)
#define OMM_REWARD_IS_PERRY_CHARGE_UNLOCKED         omm_rewards_is_unlocked(OMM_REWARD_PERRY_CHARGE, true)
#define OMM_REWARD_IS__REDACTED__UNLOCKED           omm_rewards_is_unlocked(OMM_REWARD__REDACTED_, true)

u32  omm_rewards_get_count();
u32  omm_rewards_get_unlocked_count(bool local);
bool omm_rewards_is_unlocked(u32 reward, bool local);
bool omm_rewards_get(u32 reward, const char **name, const char **cond, const char **text);

//
// Secrets
//

#define OMM_SECRET_BOWSER_THROW         (0x00)
#define OMM_SECRET_DARK_BASEMENT        (0x01)
#define OMM_SECRET_PEACHY_ROOM          (0x02)
#define OMM_SECRET_FLYING_DORRIE        (0x03)
#define OMM_SECRET_TOAD_CHEAT_CODE      (0x04)
#define OMM_SECRET_CHEATER_FISH         (0x05)
#define OMM_SECRET_PEACH_SECRET_1       (0x06)
#define OMM_SECRET_PEACH_SECRET_2       (0x07)
#define OMM_SECRET_YOSHI_SECRET         (0x08)
#define OMM_SECRET_SMMS_SECRET          (0x09)
#define OMM_SECRET_SM74_SECRET          (0x0A)
#define OMM_SECRET_SMSR_SECRET          (0x0B)
#define OMM_SECRET_SMGS_SECRET          (0x0C)
#define OMM_SECRET_R96_SECRET           (0x0D)
// TODO: YOSHIMODE
// #define OMM_SECRET_YOSHI_ENDING         (0x0E)
// #define OMM_SECRET_SMSR_YOSHI_ENDING    (0x0F)

u32  omm_secrets_get_count();
u32  omm_secrets_get_unlocked_count();
bool omm_secrets_is_unlocked(u32 secret);
bool omm_secrets_get(u32 secret, const char **name, const char **cond, const char **text);

bool omm_secrets_read(const char *name, const char *value, bool *invalid);
void omm_secrets_write();
void omm_secrets_reset();
bool omm_secrets_unlock(u32 secret);

//
// Speedrun
//

#define OMM_SPLIT_STAR   (0)
#define OMM_SPLIT_EXIT   (1)
#define OMM_SPLIT_BOWSER (2)

void omm_speedrun_split(s32 type);

#endif // OMM_SYSTEM_H

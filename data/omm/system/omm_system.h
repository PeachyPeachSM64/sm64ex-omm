#ifndef OMM_SYSTEM_H
#define OMM_SYSTEM_H

#include "data/omm/omm_includes.h"
#include "omm_render.h"

//
// System
//

const char *omm_exe_path();
const char *omm_user_path();
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
#define sm74_mode__omm_get_dialog_table         gCurrAreaIndex
#endif

//
// Debug
//

#define OMM_COUNTER_FPS     0 // Count the elapsed time between 2 successive game updates. Used to compute the actual frame rate.
#define OMM_COUNTER_FRM     1 // Count the elapsed time between gfx_start_frame() and gfx_end_frame().
#define OMM_COUNTER_OMM     2 // Count the elapsed time spent by omm_update().
#define OMM_COUNTER_LVL     3 // Count the elapsed time spent by the level script update.
#define OMM_COUNTER_PRE     4 // Count the elapsed time spent by omm_pre_render().
#define OMM_COUNTER_GEO     5 // Count the elapsed time spent by render_game().
#define OMM_COUNTER_GFX     6 // Count the elapsed time spent by gfx_run_dl().
#define OMM_COUNTER_RDR     7 // Count the elapsed time spent by the scene rendering.
#define OMM_NUM_COUNTERS    8

void omm_debug_start_counter(s32 counter);
void omm_debug_end_counter(s32 counter);

//
// Player
//

bool omm_player_is_unlocked(s32 index);
bool omm_player_is_selected(s32 index);
void omm_player_select(s32 index);
s32  omm_player_get_selected_index();

const char *omm_player_get_name(s32 index);
u32 omm_player_get_color(s32 index);
s32 omm_player_get_model(s32 index);
s32 omm_player_get_normal_cap(s32 index);
s32 omm_player_get_wing_cap(s32 index);
s32 omm_player_get_metal_cap(s32 index);
s32 omm_player_get_winged_metal_cap(s32 index);
s32 omm_player_get_cap(s32 index, bool wing, bool metal);

f32 omm_player_get_ground_speed_multiplier(s32 index);
f32 omm_player_get_air_speed_multiplier(s32 index);
f32 omm_player_get_jump_multiplier(s32 index);

#define OMM_PLAYER_SEL_GFX_INDEX                            omm_player_get_selected_index()
#define OMM_PLAYER_SEL_PHY_INDEX                            omm_player_get_selected_index() + (omm_peach_vibe_is_active() ? (gOmmData->mario->peach.vibeType + OMM_NUM_PLAYABLE_CHARACTERS - 2) : (0))

#define omm_player_get_selected_name()                      omm_player_get_name(OMM_PLAYER_SEL_GFX_INDEX)
#define omm_player_get_selected_color()                     omm_player_get_color(OMM_PLAYER_SEL_GFX_INDEX)
#define omm_player_get_selected_model()                     omm_player_get_model(OMM_PLAYER_SEL_GFX_INDEX)
#define omm_player_get_selected_normal_cap()                omm_player_get_normal_cap(OMM_PLAYER_SEL_GFX_INDEX)
#define omm_player_get_selected_wing_cap()                  omm_player_get_wing_cap(OMM_PLAYER_SEL_GFX_INDEX)
#define omm_player_get_selected_metal_cap()                 omm_player_get_metal_cap(OMM_PLAYER_SEL_GFX_INDEX)
#define omm_player_get_selected_winged_metal_cap()          omm_player_get_winged_metal_cap(OMM_PLAYER_SEL_GFX_INDEX)
#define omm_player_get_selected_cap(wing, metal)            omm_player_get_cap(OMM_PLAYER_SEL_GFX_INDEX, wing, metal)

#define omm_player_get_selected_ground_speed_multiplier()   omm_player_get_ground_speed_multiplier(OMM_PLAYER_SEL_PHY_INDEX)
#define omm_player_get_selected_air_speed_multiplier()      omm_player_get_air_speed_multiplier(OMM_PLAYER_SEL_PHY_INDEX)
#define omm_player_get_selected_jump_multiplier()           omm_player_get_jump_multiplier(OMM_PLAYER_SEL_PHY_INDEX)
#define get_additive_y_vel_for_jumps()                      ((initialVelY + (m->forwardVel * multiplier)) * (omm_player_get_selected_jump_multiplier() - 1.f))

//
// Health
//

s32  omm_health_get_max(s32 health);
s32  omm_health_get_max_hp(s32 hp);
bool omm_health_is_mario_losing_hp(struct MarioState *m);
void omm_health_update(struct MarioState *m);
void omm_health_heal_mario(struct MarioState *m);
void omm_health_fully_heal_mario(struct MarioState *m);
void omm_health_life_up(struct MarioState *m);
void omm_health_set(struct MarioState *m, s16 health);
bool omm_health_is_at_max(struct MarioState *m);

//
// Text
//

u8 *omm_text_convert(const char *str, bool heapAlloc);
u8 *omm_text_copy(const u8 *str64, bool heapAlloc);
u8 *omm_text_capitalize(u8 *str64);
u8 *omm_text_decapitalize(u8 *str64);
u8 *omm_text_replace_char(u8 *str64, u8 from, u8 to);
u8 *omm_text_replace_names(u8 *str64, bool inplace);
s32 omm_text_length(const u8 *str64);
struct DialogEntry **omm_get_dialog_table();
struct DialogEntry *omm_get_dialog_entry(void **dialogTable, s16 dialogId);

//
// Camera
//

void omm_camera_init();
bool omm_camera_update(struct Camera *c, struct MarioState *m);
bool omm_camera_is_available(struct MarioState *m);
s16  omm_camera_get_intended_yaw(struct MarioState *m);
s32  omm_camera_get_relative_dist_mode();

//
// Audio
//

u8  *omm_audio_mix(u8 *output, const u8 *input, s32 length, s32 volume, s32 distance);
s32  omm_audio_resize(u8 **output, const u8 *input, s32 inputLength, s32 desiredLength);
s32  omm_audio_resample(u8 **output, const u8 *input, s32 inputLength, f32 outputScale);
s32  omm_audio_resample_fast(u8 **output, const u8 *input, s32 inputLength, f32 outputScale);
s32  omm_audio_time_stretch(u8 **output, const u8 *input, s32 inputLength, s32 audioFreq, f32 timeStretch);
s32  omm_audio_pitch_shift(u8 **output, const u8 *input, s32 inputLength, s32 audioFreq, f32 pitchScale);

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
// Options
//

#define DECLARE_KBINDS(option)                  extern u32 option[MAX_BINDS]
#define DECLARE_TOGGLE(option)                  extern bool option
#define DECLARE_TOGGLE_SC(option)               extern bool option; DECLARE_KBINDS(option##Shortcuts)
#define DECLARE_CHOICE(option)                  extern u32 option
#define DECLARE_CHOICE_SC(option)               extern u32 option; DECLARE_KBINDS(option##Shortcuts)

#define DEFINE_KBINDS(option, v0, v1, v2)       u32 option[MAX_BINDS] = { v0, v1, v2 }
#define DEFINE_TOGGLE(option, v)                bool option = v
#define DEFINE_TOGGLE_SC(option, v)             bool option = v; DEFINE_KBINDS(option##Shortcuts, VK_INVALID, VK_INVALID, VK_INVALID)
#define DEFINE_CHOICE(option, v)                u32 option = v
#define DEFINE_CHOICE_SC(option, v)             u32 option = v; DEFINE_KBINDS(option##Shortcuts, VK_INVALID, VK_INVALID, VK_INVALID)

#define READ_KBINDS(option, buf)                if (strcmp(buf[0], #option) == 0) { sscanf(buf[1], "%X", &option[0]); sscanf(buf[2], "%X", &option[1]); sscanf(buf[3], "%X", &option[2]); continue; }
#define READ_TOGGLE(option, buf)                if (strcmp(buf[0], #option) == 0) { option = (buf[1][0] == '1'); continue; }
#define READ_TOGGLE_SC(option, buf)             if (strcmp(buf[0], #option) == 0) { option = (buf[1][0] == '1'); continue; } READ_KBINDS(option##Shortcuts, buf)
#define READ_CHOICE(option, buf)                if (strcmp(buf[0], #option) == 0) { sscanf(buf[1], "%u", &option); continue; }
#define READ_CHOICE_SC(option, buf)             if (strcmp(buf[0], #option) == 0) { sscanf(buf[1], "%u", &option); continue; } READ_KBINDS(option##Shortcuts, buf)

#define WRITE_KBINDS(option)                    write("%s = %04hX %04hX %04hX\n", #option, (u16) option[0], (u16) option[1], (u16) option[2])
#define WRITE_TOGGLE(option)                    write("%s = %u\n", #option, (option ? 1 : 0))
#define WRITE_TOGGLE_SC(option)                 write("%s = %u\n", #option, (option ? 1 : 0)); WRITE_KBINDS(option##Shortcuts)
#define WRITE_CHOICE(option)                    write("%s = %u\n", #option, option)
#define WRITE_CHOICE_SC(option)                 write("%s = %u\n", #option, option); WRITE_KBINDS(option##Shortcuts)

extern u8 gOmmOptMenu[];
extern u8 gOmmOptControls[];
#if !OMM_GAME_IS_R96A
extern u8 gOmmOptCheats[];
#endif
#if !OMM_CODE_DYNOS
extern u8 gOmmOptWarpToLevel[];
extern u8 gOmmOptReturnToMainMenu[];
#endif

DECLARE_KBINDS(gOmmControlsButtonA);
DECLARE_KBINDS(gOmmControlsButtonB);
DECLARE_KBINDS(gOmmControlsButtonX);
DECLARE_KBINDS(gOmmControlsButtonY);
DECLARE_KBINDS(gOmmControlsButtonStart);
DECLARE_KBINDS(gOmmControlsTriggerL);
DECLARE_KBINDS(gOmmControlsTriggerR);
DECLARE_KBINDS(gOmmControlsTriggerZ);
DECLARE_KBINDS(gOmmControlsCUp);
DECLARE_KBINDS(gOmmControlsCDown);
DECLARE_KBINDS(gOmmControlsCLeft);
DECLARE_KBINDS(gOmmControlsCRight);
DECLARE_KBINDS(gOmmControlsDUp);
DECLARE_KBINDS(gOmmControlsDDown);
DECLARE_KBINDS(gOmmControlsDLeft);
DECLARE_KBINDS(gOmmControlsDRight);
DECLARE_KBINDS(gOmmControlsStickUp);
DECLARE_KBINDS(gOmmControlsStickDown);
DECLARE_KBINDS(gOmmControlsStickLeft);
DECLARE_KBINDS(gOmmControlsStickRight);
#if !OMM_GAME_IS_R96A
DECLARE_TOGGLE(gOmmCheatEnable);
DECLARE_TOGGLE(gOmmCheatMoonJump);
DECLARE_TOGGLE(gOmmCheatGodMode);
DECLARE_TOGGLE(gOmmCheatInvincible);
DECLARE_TOGGLE(gOmmCheatSuperSpeed);
DECLARE_TOGGLE(gOmmCheatSuperResponsive);
DECLARE_TOGGLE(gOmmCheatNoFallDamage);
DECLARE_TOGGLE(gOmmCheatWalkOnLava);
DECLARE_TOGGLE(gOmmCheatWalkOnQuicksand);
DECLARE_TOGGLE(gOmmCheatWalkOnWater);
DECLARE_TOGGLE(gOmmCheatWalkOnGas);
DECLARE_TOGGLE(gOmmCheatWalkOnSlope);
DECLARE_TOGGLE(gOmmCheatWalkOnDeathBarrier);
DECLARE_TOGGLE(gOmmCheatBljAnywhere);
#endif
DECLARE_CHOICE(gOmmFPS);
DECLARE_CHOICE_SC(gOmmCharacter);
DECLARE_CHOICE_SC(gOmmMovesetType);
DECLARE_CHOICE_SC(gOmmCapType);
DECLARE_CHOICE_SC(gOmmStarsMode);
DECLARE_CHOICE_SC(gOmmPowerUpsType);
DECLARE_CHOICE_SC(gOmmCameraMode);
DECLARE_CHOICE_SC(gOmmSparklyStarsMode);
DECLARE_TOGGLE_SC(gOmmSparklyStarsHint);
DECLARE_TOGGLE_SC(gOmmSparklyStarsAssist);
DECLARE_TOGGLE(gOmmCheatUnlimitedCappyBounces);
DECLARE_TOGGLE(gOmmCheatCappyStaysForever);
DECLARE_TOGGLE(gOmmCheatHomingAttackGlobalRange);
DECLARE_TOGGLE(gOmmCheatMarioTeleportsToCappy);
DECLARE_TOGGLE(gOmmCheatCappyCanCollectStars);
DECLARE_TOGGLE(gOmmCheatPlayAsCappy);
DECLARE_TOGGLE(gOmmCheatPeachEndlessVibeGauge);
DECLARE_CHOICE(gOmmExtrasMarioColors);
DECLARE_CHOICE(gOmmExtrasPeachColors);
DECLARE_TOGGLE_SC(gOmmExtrasSMOAnimations);
DECLARE_TOGGLE_SC(gOmmExtrasCappyAndTiara);
DECLARE_TOGGLE_SC(gOmmExtrasColoredStars);
DECLARE_TOGGLE_SC(gOmmExtrasVanishingHUD);
DECLARE_TOGGLE_SC(gOmmExtrasRevealSecrets);
DECLARE_TOGGLE_SC(gOmmExtrasRedCoinsRadar);
DECLARE_TOGGLE_SC(gOmmExtrasShowStarNumber);
DECLARE_TOGGLE_SC(gOmmExtrasInvisibleMode);
#if OMM_CODE_DEV
DECLARE_TOGGLE_SC(gOmmExtrasRender96Peach);
#endif
DECLARE_TOGGLE_SC(gOmmExtrasCrystalStarsReward);
DECLARE_TOGGLE_SC(gOmmExtrasNebulaStarsReward);
#if OMM_CODE_DEBUG
DECLARE_TOGGLE_SC(gOmmDebugHitbox);
DECLARE_TOGGLE_SC(gOmmDebugHurtbox);
DECLARE_TOGGLE_SC(gOmmDebugWallbox);
DECLARE_TOGGLE_SC(gOmmDebugSurface);
DECLARE_TOGGLE_SC(gOmmDebugMario);
DECLARE_TOGGLE_SC(gOmmDebugCappy);
DECLARE_TOGGLE_SC(gOmmDebugProfiler);
#if OMM_CODE_DEV_DEBUG
DECLARE_TOGGLE_SC(gOmmDebugGameSpeedEnable);
DECLARE_CHOICE_SC(gOmmDebugGameSpeedFps);
#endif
#endif

void omm_opt_return_to_main_menu(UNUSED void *opt, s32 arg);
void omm_opt_reset_binds(u32 *binds);
bool omm_mario_colors_read(const char **tokens);
void omm_mario_colors_write(char **buffer);
const char **omm_mario_colors_choices();
s32 omm_mario_colors_count();

#define omm_opt_select_available(option, zero, count, cond) \
{ static u32 s##option = (u32) (-1); \
if (s##option == (u32) (-1)) { s##option = option; } \
s32 d##option = sign_0_s(option - s##option) * (abs_s(option - s##option) > 1 ? -1 : +1); \
if (d##option != 0) { while (!(cond)) { option = (option + d##option + (count)) % (count); } } \
else if (!(cond)) { option = (zero); } \
s##option = option; }

#endif // OMM_SYSTEM_H

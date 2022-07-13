#ifndef OMM_MACROS_H
#define OMM_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <xmmintrin.h>

// Must be here, but can't use SM64 typenames
typedef struct { unsigned int ts; short v; } s16_ts;
typedef struct { unsigned int ts; int v; } s32_ts;
typedef struct { unsigned int ts; unsigned short v; } u16_ts;
typedef struct { unsigned int ts; unsigned int v; } u32_ts;
typedef struct { unsigned int ts; float v; } f32_ts;
typedef struct { unsigned int ts; float v[3]; } Vec3f_ts;
typedef struct { unsigned int ts; short v[3]; } Vec3s_ts;
typedef struct { unsigned int ts; float v[4][4]; } Mat4_ts;
typedef struct { unsigned int ts; void *v; } ptr_ts;

//
// Versions (auto-detected by omm.mk)
// SMEX : sm64ex-nightly
// SMMS : sm64ex-nightly + Moonshine
// R96A : Render96ex-alpha
// XALO : sm64ex-alo
// SM74 : Super Mario 74
// SMSR : Super Mario Star Road
//

#if   defined(SMEX)
#define DEF(smex, smms, r96a, xalo, sm74, smsr) smex
#elif defined(SMMS)
#define DEF(smex, smms, r96a, xalo, sm74, smsr) smms
#elif defined(R96A)
#define DEF(smex, smms, r96a, xalo, sm74, smsr) r96a
#elif defined(XALO)
#define DEF(smex, smms, r96a, xalo, sm74, smsr) xalo
#elif defined(SM74)
#define DEF(smex, smms, r96a, xalo, sm74, smsr) sm74
#elif defined(SMSR)
#define DEF(smex, smms, r96a, xalo, sm74, smsr) smsr
#else
#error "No version defined!"
#endif

//
// OMM macros
// Depending on which repo/conversion/hack we are building the game,
// some code must be enabled or disabled for OMM to work properly.
// The macros OMM_BOWSER, OMM_DEBUG and OMM_DEV can be passed to the make
// command with values 0 or 1 to enable or disable the corresponding code.
//

// OMM_GAME_* | Attributes a numeric value to every game version/type/save/mode
#define OMM_GAME_SM64           0
#define OMM_GAME_SMEX           0
#define OMM_GAME_SMMS           1
#define OMM_GAME_R96A           2
#define OMM_GAME_XALO           3
#define OMM_GAME_SM74           4
#define OMM_GAME_SMSR           5
#define OMM_GAME_VERSION        DEF(OMM_GAME_SMEX, OMM_GAME_SMMS, OMM_GAME_R96A, OMM_GAME_XALO, OMM_GAME_SM74, OMM_GAME_SMSR)
#define OMM_GAME_TYPE           DEF(OMM_GAME_SM64, OMM_GAME_SMMS, OMM_GAME_SM64, OMM_GAME_SM64, OMM_GAME_SM74, OMM_GAME_SMSR)
#define OMM_GAME_SAVE           DEF(OMM_GAME_SM64, OMM_GAME_SMMS, OMM_GAME_R96A, OMM_GAME_SM64, OMM_GAME_SM74, OMM_GAME_SMSR)
#define OMM_GAME_MODE           DEF(0, 0, 0, 0, sm74_mode__omm_save, 0)
#define OMM_GAME_IS_SM64        (OMM_GAME_TYPE == OMM_GAME_SM64)
#define OMM_GAME_IS_SMEX        (OMM_GAME_VERSION == OMM_GAME_SMEX)
#define OMM_GAME_IS_SMMS        (OMM_GAME_VERSION == OMM_GAME_SMMS)
#define OMM_GAME_IS_R96A        (OMM_GAME_VERSION == OMM_GAME_R96A)
#define OMM_GAME_IS_XALO        (OMM_GAME_VERSION == OMM_GAME_XALO)
#define OMM_GAME_IS_SM74        (OMM_GAME_VERSION == OMM_GAME_SM74)
#define OMM_GAME_IS_SMSR        (OMM_GAME_VERSION == OMM_GAME_SMSR)

// OMM_BOWSER | Replaces Vanilla Bowser with OMM Bowser
#if defined(OMM_BOWSER)
#define OMM_CODE_BOWSER         DEF(OMM_BOWSER, OMM_BOWSER, OMM_BOWSER, OMM_BOWSER, OMM_BOWSER, OMM_BOWSER)
#else
#define OMM_CODE_BOWSER         DEF(1, 0, 1, 1, 0, 0)
#endif

// OMM_MARIO_COLORS | Enables Mario colors feature in Extras
#if defined(OMM_MARIO_COLORS)
#define OMM_CODE_MARIO_COLORS   DEF(OMM_MARIO_COLORS, 0, 0, OMM_MARIO_COLORS, OMM_MARIO_COLORS, OMM_MARIO_COLORS)
#else
#define OMM_CODE_MARIO_COLORS   DEF(1, 0, 0, 1, 1, 1)
#endif

// OMM_DEBUG | Enables some debug stuff
#if defined(OMM_DEBUG)
#define OMM_CODE_DEBUG          DEF(OMM_DEBUG, OMM_DEBUG, OMM_DEBUG, OMM_DEBUG, OMM_DEBUG, OMM_DEBUG)
#else
#define OMM_CODE_DEBUG          DEF(0, 0, 0, 0, 0, 0)
#endif

// EXT_OPTIONS_MENU | If set, enables options types implementation in file omm_options.c
#if defined(EXT_OPTIONS_MENU)
#define OMM_OPT_TYPES_IMPL      DEF(1, 1, 1, 0, 0, 0)
#else
#define OMM_OPT_TYPES_IMPL      DEF(0, 0, 0, 0, 0, 0)
#endif

// OMM_DEV | If set, enables super secret features (dev branch only, requires OMM_DEBUG flag, DEBUG flag and SDL backend for debug features)
#if defined(OMM_DEV)
#define OMM_CODE_DEV            DEF(OMM_DEV, OMM_DEV, OMM_DEV, OMM_DEV, OMM_DEV, OMM_DEV)
#if OMM_CODE_DEBUG && defined(DEBUG) && defined(WAPI_SDL2)
#define OMM_CODE_DEV_DEBUG      DEF(OMM_DEV, OMM_DEV, OMM_DEV, OMM_DEV, OMM_DEV, OMM_DEV)
#else
#define OMM_CODE_DEV_DEBUG      DEF(0, 0, 0, 0, 0, 0)
#endif
#else
#define OMM_CODE_DEV            DEF(0, 0, 0, 0, 0, 0)
#define OMM_CODE_DEV_DEBUG      DEF(0, 0, 0, 0, 0, 0)
#endif

//
// Macros
//

// Better camera
#define BETTER_CAM_IS_PUPPY_CAM                 DEF(0, 0, 0, 1, 1, 1)
#define BETTER_CAM_IS_ENABLED                   DEF(newcam_active, newcam_active, newcam_active, gPuppyCam.enabled, gPuppyCam.enabled, gPuppyCam.enabled)
#define BETTER_CAM_MODE                         DEF(CAMERA_MODE_NEWCAM, CAMERA_MODE_NEWCAM, CAMERA_MODE_NEWCAM, 0, 0, 0)
#define BETTER_CAM_YAW                          DEF(newcam_yaw, newcam_yaw, newcam_yaw, 0, 0, 0)
#define BETTER_CAM_RAYCAST_ARGS                 DEF(, , , __EXPAND(, UNUSED s32 flags), __EXPAND(, UNUSED s32 flags), __EXPAND(, UNUSED s32 flags))

// Animation
#define AnimInfoStruct                          DEF(GraphNodeObject_sub, GraphNodeObject_sub, GraphNodeObject_sub, AnimInfo, AnimInfo, AnimInfo)
#define mAreaIndex                              DEF(unk18, unk18, unk18, areaIndex, areaIndex, areaIndex)
#define mActiveAreaIndex                        DEF(unk19, unk19, unk19, activeAreaIndex, activeAreaIndex, activeAreaIndex)
#define mModel                                  DEF(unk18, unk18, unk18, model, model, model)
#define mAnimInfo                               DEF(unk38, unk38, curAnim, animInfo, animInfo, animInfo)
#define mAnimYTransDivisor                      DEF(unk02, unk02, unk02, animYTransDivisor, animYTransDivisor, animYTransDivisor)
#define mStartFrame                             DEF(unk04, unk04, unk04, startFrame, startFrame, startFrame)
#define mLoopStart                              DEF(unk06, unk06, unk06, loopStart, loopStart, loopStart)
#define mLoopEnd                                DEF(unk08, unk08, unk08, loopEnd, loopEnd, loopEnd)

// Mario animation
#define MarioAnimationsStruct                   DEF(struct MarioAnimation, struct MarioAnimation, struct MarioAnimation, struct DmaHandlerList, struct DmaHandlerList, struct DmaHandlerList)
#define gMarioAnimations                        gMarioState->DEF(animation, animation, animation, animList, animList, animList)
#define gMarioCurrAnimAddr                      gMarioAnimations->DEF(currentAnimAddr, currentAnimAddr, currentAnimAddr, currentAddr, currentAddr, currentAddr)
#define gMarioTargetAnim                        gMarioAnimations->DEF(targetAnim, targetAnim, targetAnim, bufTarget, bufTarget, bufTarget)
#define gMarioAnimsData                         DEF(D_80339D10, D_80339D10, Data_MarioAnims, gMarioAnimsBuf, gMarioAnimsBuf, gMarioAnimsBuf)

// Audio
#define gGlobalSoundArgs                        DEF(gDefaultSoundArgs, gDefaultSoundArgs, gDefaultSoundArgs, gGlobalSoundSource, gGlobalSoundSource, gGlobalSoundSource)
#define sAcousticReachPerLevel                  DEF(D_80332028, D_80332028, D_80332028, sLevelAcousticReaches, sLevelAcousticReaches, sLevelAcousticReaches)
#define audio_play_wing_cap_music()             DEF(play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)), r96_play_cap_music(R96_EVENT_CAP_WING), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)))
#define audio_play_metal_cap_music()            DEF(play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP)), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP)), r96_play_cap_music(R96_EVENT_CAP_METAL), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP)), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP)), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP)))
#define audio_play_vanish_cap_music()           DEF(play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)), r96_play_cap_music(R96_EVENT_CAP_VANISH), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)), play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)))
#define audio_stop_cap_music()                  DEF(stop_cap_music(), stop_cap_music(), r96_stop_cap_music(), stop_cap_music(), stop_cap_music(), stop_cap_music())
#define audio_play_shell_music()                DEF(play_shell_music(), play_shell_music(), r96_play_shell_music(), play_shell_music(), play_shell_music(), play_shell_music())
#define audio_stop_shell_music()                DEF(stop_shell_music(), stop_shell_music(), r96_stop_shell_music(), stop_shell_music(), stop_shell_music(), stop_shell_music())
#define audio_play_course_clear()               DEF(play_course_clear(), play_course_clear(), dynos_music_pause(); r96_play_jingle(R96_EVENT_STAR_COLLECT), play_course_clear(), play_course_clear(), play_course_clear())
#define audio_stop_course_clear()               DEF(, __EXPAND(stop_background_music(SEQ_EVENT_BOSS); play_secondary_music(0, 0, 0, 0); func_80321080(60)), dynos_music_resume(), , , )
#define audio_play_puzzle_jingle()              DEF(play_puzzle_jingle(), play_puzzle_jingle(), r96_play_jingle(R96_EVENT_SOLVE_PUZZLE), play_puzzle_jingle(), play_puzzle_jingle(), play_puzzle_jingle())
#define audio_play_toads_jingle()               DEF(play_toads_jingle(), play_toads_jingle(), r96_play_jingle(R96_EVENT_TOAD_MESSAGE), play_toads_jingle(), play_toads_jingle(), play_toads_jingle())
#define audio_play_star_jingle()                DEF(play_power_star_jingle(1), play_power_star_jingle(1), r96_play_jingle(R96_EVENT_STAR_APPEAR), play_power_star_jingle(1), play_power_star_jingle(1), play_power_star_jingle(1))
#define audio_play_star_fanfare()               DEF(play_star_fanfare(), play_star_fanfare(), r96_play_jingle(R96_EVENT_STAR_FANFARE), play_star_fanfare(), play_star_fanfare(), play_star_fanfare())

// Music
#define music_pause()                           DEF(gSequencePlayers[0].muted = true, gSequencePlayers[0].muted = true, dynos_music_pause(), gSequencePlayers[0].muted = true, gSequencePlayers[0].muted = true, gSequencePlayers[0].muted = true)
#define music_resume()                          DEF(gSequencePlayers[0].muted = false, gSequencePlayers[0].muted = false, dynos_music_resume(), gSequencePlayers[0].muted = false, gSequencePlayers[0].muted = false, gSequencePlayers[0].muted = false)
#define music_stop()                            DEF(, , dynos_music_stop(); dynos_jingle_stop(), , , )
#define audio_mute(...)                         DEF(set_sound_disabled(__VA_ARGS__), set_sound_disabled(__VA_ARGS__), set_sound_disabled(__VA_ARGS__), set_audio_muted(__VA_ARGS__), set_audio_muted(__VA_ARGS__), set_audio_muted(__VA_ARGS__))
#define music_fade_out(...)                     DEF(sequence_player_fade_out(__VA_ARGS__), sequence_player_fade_out(__VA_ARGS__), sequence_player_fade_out(__VA_ARGS__), seq_player_fade_out(__VA_ARGS__), seq_player_fade_out(__VA_ARGS__), seq_player_fade_out(__VA_ARGS__))
#define music_lower_volume(...)                 DEF(func_8031FFB4(__VA_ARGS__), func_8031FFB4(__VA_ARGS__), func_8031FFB4(__VA_ARGS__), seq_player_lower_volume(__VA_ARGS__), seq_player_lower_volume(__VA_ARGS__), seq_player_lower_volume(__VA_ARGS__))
#define music_unlower_volume(...)               DEF(sequence_player_unlower(__VA_ARGS__), sequence_player_unlower(__VA_ARGS__), sequence_player_unlower(__VA_ARGS__), seq_player_unlower_volume(__VA_ARGS__), seq_player_unlower_volume(__VA_ARGS__), seq_player_unlower_volume(__VA_ARGS__))
#define music_play_sequence_define              DEF(0, 0, 0, 1, 1, 1)
#define MUSIC_QUEUE_MAX_SIZE                    DEF(MAX_BG_MUSIC_QUEUE_SIZE, MAX_BG_MUSIC_QUEUE_SIZE, MAX_BG_MUSIC_QUEUE_SIZE, MAX_BACKGROUND_MUSIC_QUEUE_SIZE, MAX_BACKGROUND_MUSIC_QUEUE_SIZE, MAX_BACKGROUND_MUSIC_QUEUE_SIZE)

// Sound
#define sound_stop(...)                         DEF(func_803205E8(__VA_ARGS__), func_803205E8(__VA_ARGS__), func_803205E8(__VA_ARGS__), stop_sound(__VA_ARGS__), stop_sound(__VA_ARGS__), stop_sound(__VA_ARGS__))
#define sound_stop_from_source(...)             DEF(func_803206F8(__VA_ARGS__), func_803206F8(__VA_ARGS__), func_803206F8(__VA_ARGS__), stop_sounds_from_source(__VA_ARGS__), stop_sounds_from_source(__VA_ARGS__), stop_sounds_from_source(__VA_ARGS__))
#define sound_stop_continuous()                 DEF(func_80320890(), func_80320890(), func_80320890(), stop_sounds_in_continuous_banks(), stop_sounds_in_continuous_banks(), stop_sounds_in_continuous_banks())
#define sound_set_moving_speed(...)             DEF(func_80320A4C(__VA_ARGS__), func_80320A4C(__VA_ARGS__), func_80320A4C(__VA_ARGS__), set_sound_moving_speed(__VA_ARGS__), set_sound_moving_speed(__VA_ARGS__), set_sound_moving_speed(__VA_ARGS__))
#define SOUND_OBJ_WHOMP_SLAM                    DEF(SOUND_OBJ_WHOMP_LOWPRIO, SOUND_OBJ_WHOMP_LOWPRIO, SOUND_OBJ_WHOMP_LOWPRIO, SOUND_OBJ_WHOMP, SOUND_OBJ_WHOMP, SOUND_OBJ_WHOMP)

// Object fields
#define oSnowmansBodyScale                      DEF(oSnowmansBottomUnkF4, oSnowmansBottomUnkF4, oSnowmansBottomUnkF4, oSnowmansBottomScale, oSnowmansBottomScale, oSnowmansBottomScale)
#define oBitsPlatformBowserObject               DEF(oPlatformUnkF8, oPlatformUnkF8, oPlatformUnkF8, oBitsPlatformBowser, oBitsPlatformBowser, oBitsPlatformBowser)
#define oBowserCameraState                      DEF(oBowserUnk88, oBowserUnk88, oBowserUnk88, oBowserCamAct, oBowserCamAct, oBowserCamAct)
#define oBowserFlags                            DEF(oBowserUnkF4, oBowserUnkF4, oBowserUnkF4, oBowserStatus, oBowserStatus, oBowserStatus)
#define oBowserActionTimer                      DEF(oBowserUnkF8, oBowserUnkF8, oBowserUnkF8, oBowserTimer, oBowserTimer, oBowserTimer)
#define oBowserBitsJump                         DEF(oBowserUnk106, oBowserUnk106, oBowserUnk106, oBowserBitsJustJump, oBowserBitsJustJump, oBowserBitsJustJump)
#define oBowserSpitFireNumFlames                DEF(oBowserUnk108, oBowserUnk108, oBowserUnk108, oBowserRandSplitFloor, oBowserRandSplitFloor, oBowserRandSplitFloor)
#define oBowserGrabbedAction                    DEF(oBowserUnk10E, oBowserUnk10E, oBowserUnk10E, oBowserGrabbedStatus, oBowserGrabbedStatus, oBowserGrabbedStatus)
#define oBowserActionSelected                   DEF(oBowserUnk110, oBowserUnk110, oBowserUnk110, oBowserIsReacting, oBowserIsReacting, oBowserIsReacting)
#define oBowserOpacityTarget                    DEF(oBowserUnk1AC, oBowserUnk1AC, oBowserUnk1AC, oBowserTargetOpacity, oBowserTargetOpacity, oBowserTargetOpacity)
#define oBowserBlinkTimer                       DEF(oBowserUnk1AE, oBowserUnk1AE, oBowserUnk1AE, oBowserEyesTimer, oBowserEyesTimer, oBowserEyesTimer)
#define oBowserRainbowLightEffect               DEF(oBowserUnk1B2, oBowserUnk1B2, oBowserUnk1B2, oBowserRainbowLight, oBowserRainbowLight, oBowserRainbowLight)
#define oBowserShockwaveScale                   DEF(oBowserShockWaveUnkF4, oBowserShockWaveUnkF4, oBowserShockWaveUnkF4, oBowserShockWaveScale, oBowserShockWaveScale, oBowserShockWaveScale)

// Dialogs and menus
#define gDialogBoxState                         DEF(gDialogBoxState, gDialogBoxState, gDialogBoxState, gMenuState, gDialogBoxState, gMenuState)
#define gDialogBoxAngle                         DEF(gDialogBoxOpenTimer, gDialogBoxOpenTimer, gDialogBoxOpenTimer, gDialogBoxAngle, gDialogBoxOpenTimer, gDialogBoxAngle)
#define gDialogLineIndex                        DEF(gDialogLineNum, gDialogLineNum, gDialogLineNum, gMenuLineNum, gDialogLineNum, gMenuLineNum)

// Level scripts
#define LEVEL_SCRIPT_PRESSED_START              DEF(1, 1, 1, 0, 0, 0)
#define level_script_entry_point                DEF(level_script_entry, level_script_entry, level_script_entry, level_script_entry, level_script_entry, level_script_entry)
#define level_script_splash_screen              DEF(level_intro_entry_1, level_intro_entry_1, level_intro_entry_1, level_intro_splash_screen, level_intro_splash_screen, level_intro_splash_screen)
#define level_script_goddard_regular            DEF(level_intro_entry_2, level_intro_entry_2, level_intro_entry_2, level_intro_mario_head_regular, level_intro_mario_head_regular, level_intro_mario_head_regular)
#define level_script_goddard_game_over          DEF(level_intro_entry_3, level_intro_entry_3, level_intro_entry_3, level_intro_mario_head_dizzy, level_intro_mario_head_dizzy, level_intro_mario_head_dizzy)
#define level_script_debug_level_select         DEF(level_intro_entry_4, level_intro_entry_4, level_intro_entry_4, level_intro_entry_4, level_intro_entry_4, level_intro_entry_4)
#define level_script_to_file_select             DEF(script_intro_L1, script_intro_L1, script_intro_L1, script_intro_L1, script_intro_L1, script_intro_L1)
#define level_script_to_debug_level_select      DEF(script_intro_L2, script_intro_L2, script_intro_L2, script_intro_L2, script_intro_L2, script_intro_L2)
#define level_script_to_star_select_1           DEF(script_intro_L3, script_intro_L3, script_intro_L3, script_intro_L3, script_intro_L3, script_intro_L3)
#define level_script_to_star_select_2           DEF(script_intro_L4, script_intro_L4, script_intro_L4, script_intro_L4, script_intro_L4, script_intro_L4)
#define level_script_to_splash_screen           DEF(script_intro_L5, script_intro_L5, script_intro_L5, script_intro_L5, script_intro_L5, script_intro_L5)
#define level_script_file_select                DEF(level_main_menu_entry_1, level_main_menu_entry_1, level_main_menu_entry_1, level_main_menu_entry_1, level_main_menu_entry_1, level_main_menu_entry_1)
#define level_script_star_select                DEF(level_main_menu_entry_2, level_main_menu_entry_2, level_main_menu_entry_2, level_main_menu_entry_2, level_main_menu_entry_2, level_main_menu_entry_2)
#define level_script_cake_ending                DEF(level_ending_entry, level_ending_entry, level_ending_entry, level_ending_entry, level_ending_entry, level_ending_entry)

// Misc
#define find_static_floor(...)                  DEF(find_floor(__VA_ARGS__), find_floor(__VA_ARGS__), find_floor(__VA_ARGS__), find_floor(__VA_ARGS__), find_floor(__VA_ARGS__), find_floor(__VA_ARGS__))
#define find_dynamic_floor(...)                 DEF(find_floor(__VA_ARGS__), find_floor(__VA_ARGS__), find_floor(__VA_ARGS__), find_floor(__VA_ARGS__), find_floor(__VA_ARGS__), find_floor(__VA_ARGS__))
#define load_gfx_memory_pool()                  DEF(config_gfx_pool(), config_gfx_pool(), config_gfx_pool(), select_gfx_pool(), select_gfx_pool(), select_gfx_pool())
#define init_scene_rendering()                  DEF(init_render_image(), init_render_image(), init_render_image(), init_rcp(), init_rcp(), init_rcp())
#define run_display_list(...)                   DEF(send_display_list(__VA_ARGS__), send_display_list(__VA_ARGS__), send_display_list(__VA_ARGS__), exec_display_list(__VA_ARGS__), exec_display_list(__VA_ARGS__), exec_display_list(__VA_ARGS__))
#define clear_framebuffer(...)                  DEF(clear_frame_buffer(__VA_ARGS__), clear_frame_buffer(__VA_ARGS__), clear_frame_buffer(__VA_ARGS__), clear_framebuffer(__VA_ARGS__), clear_frame_buffer(__VA_ARGS__), clear_framebuffer(__VA_ARGS__))
#define g1HPMode                                DEF(gOmm1HPMode, gOmm1HPMode, gOmm1HPMode, gOmm1HPMode, gOmm1HPMode, gStarRoadHardMode)
#define INPUT_BOUNCE                            DEF(INPUT_UNKNOWN_10, INPUT_UNKNOWN_10, INPUT_UNKNOWN_10, INPUT_STOMPED, INPUT_STOMPED, INPUT_STOMPED)

// OMM
#define OMM_LEVEL_ENTRY_WARP(level)             DEF(0x0A, 0x0A, 0x0A, 0x0A, 0x0A, (level == LEVEL_JRB ? 0x01 : 0x0A))
#define OMM_LEVEL_EXIT_DISTANCE                 DEF(500.f, 500.f, 500.f, 500.f, 150.f, 500.f)
#define OMM_LEVEL_SLIDE                         DEF(LEVEL_PSS, LEVEL_PSS, LEVEL_PSS, LEVEL_PSS, LEVEL_PSS, LEVEL_SA)
#define OMM_LEVEL_RETURN_TO_CASTLE              DEF(__EXPAND(LEVEL_CASTLE, 1, 0x1F, 0), __EXPAND(LEVEL_CASTLE, 1, 0x1F, 0), __EXPAND(LEVEL_CASTLE, 1, 0x1F, 0), __EXPAND(LEVEL_CASTLE, 1, 0x1F, 0), __EXPAND(LEVEL_COURT, gCurrAreaIndex, 0x40, 0), __EXPAND(LEVEL_GROUNDS, 1, 0x80, 0))
#define OMM_CAMERA_LOOK_UP_WARP_STARS           DEF(10, 10, 10, 10, 10, 120)
#define OMM_CAMERA_IS_BOWSER_FIGHT              DEF(omm_camera_is_bowser_fight(), omm_camera_is_bowser_fight(), omm_camera_is_bowser_fight(), omm_camera_is_bowser_fight(), omm_camera_is_bowser_fight(), gCurrLevelNum == LEVEL_BOWSER_3)
#define OMM_NUM_PLAYABLE_CHARACTERS             DEF(2, 2, 4, 2, 2, 2)
#define OMM_CUTSCENE_MSG_INDEX                  DEF(8, 8, 9, 8, 8, 8)
#define STAR                                    DEF("STAR", "MOON", "STAR", "STAR", "STAR", "STAR")
#define Star                                    DEF("Star", "Moon", "Star", "Star", "Star", "Star")

//
// Surface collision redefines
// OMM has a more robust surface load and collision support,
// but it needs to redefine some macros
//

#include "surface_terrains.h"
#undef COL_INIT
#undef COL_VERTEX_INIT
#undef COL_VERTEX
#undef COL_TRI_INIT
#undef COL_TRI
#undef COL_TRI_SPECIAL
#undef COL_TRI_STOP
#undef COL_END
#undef COL_SPECIAL_INIT
#undef COL_WATER_BOX_INIT
#undef COL_WATER_BOX
#define CMD_COL_INIT                            0x0000
#define CMD_COL_VERTEX_INIT                     0x0001
#define CMD_COL_VERTEX                          0x0002
#define CMD_COL_TRI_INIT                        0x0003
#define CMD_COL_TRI                             0x0004
#define CMD_COL_TRI_SPECIAL                     0x0005
#define CMD_COL_TRI_STOP                        0x0006
#define CMD_COL_END                             0x0007
#define CMD_COL_SPECIAL_INIT                    0x0008
#define CMD_COL_WATER_BOX_INIT                  0x0009
#define CMD_COL_TRANSLATE                       0x000A
#define CMD_COL_JUMP                            0x000B
#define CMD_COL_END_BYTE_SEQUENCE               0x8708, 0x9749, 0x6E13, 0xA3B6, 0x83F8, 0x7245, 0x575D, 0xB1AA
#define COL_INIT()                              CMD_COL_INIT
#define COL_VERTEX_INIT(vtxNum)                 CMD_COL_VERTEX_INIT, vtxNum
#define COL_VERTEX(x, y, z)                     CMD_COL_VERTEX, x, y, z
#define COL_TRI_INIT(surfType, triNum)          CMD_COL_TRI_INIT, surfType, triNum
#define COL_TRI(v1, v2, v3)                     CMD_COL_TRI, v1, v2, v3
#define COL_TRI_SPECIAL(v1, v2, v3, param)      CMD_COL_TRI_SPECIAL, v1, v2, v3, param
#define COL_TRI_STOP()                          CMD_COL_TRI_STOP
#define COL_END()                               CMD_COL_END, CMD_COL_END_BYTE_SEQUENCE
#define COL_SPECIAL_INIT(num)                   CMD_COL_SPECIAL_INIT, num
#define COL_WATER_BOX_INIT(num)                 CMD_COL_WATER_BOX_INIT, num
#define COL_WATER_BOX(id, x1, z1, x2, z2, y)    id, x1, z1, x2, z2, y
#define COL_TRANSLATE(x, y, z)                  CMD_COL_TRANSLATE, x, y, z
#define COL_JUMP(index)                         CMD_COL_JUMP, index

#define COL_JUMP_REGISTER_INDEX(index, col) \
extern void omm_surface_register_collision_jump(s16, s16 *); \
OMM_AT_STARTUP static void omm_surface_register_collision_jump_##index() { \
    omm_surface_register_collision_jump(index, (s16 *) (col)); \
}

//
// QoL redefines
// Some of them can bug OMM
// Some of them are already fixed
// Some of them are just annoying
// Some of them must be disabled
//

#include "config.h"
#if defined(QOL_FEATURES)
#include "qol_defines.h"
#endif
#undef USE_SYSTEM_MALLOC
#undef Z_TRIG_EXTRA_ACT
#undef QOL_FEATURE_Z_BUTTON_EXTRA_OPTION
#undef QOL_FEATURE_PROPER_SHOW_COLLECTABLE
#undef QOL_FEATURE_RESPAWN_BLUE_COIN_SWITCH
#undef QOL_FEATURE_BETTER_WF_BREAKEABLE_WALL
#undef QOL_FEATURE_BOUNCE_BOOST
#undef QOL_FEATURE_TINY_GOOMBA_DROP_COIN
#undef QOL_FEATURE_MARIO_HEAD_EASTER_EGG
#undef QOL_FIX_WATER_RING
#undef QOL_FIX_GROUND_POUND_WALL
#undef QOL_FIX_LAVA_INTERACTION
#define Z_TRIG_EXTRA_ACT 0
#define QOL_FEATURE_Z_BUTTON_EXTRA_OPTION 0
#define QOL_FEATURE_PROPER_SHOW_COLLECTABLE 0
#define QOL_FEATURE_RESPAWN_BLUE_COIN_SWITCH 0
#define QOL_FEATURE_BETTER_WF_BREAKEABLE_WALL 0
#define QOL_FEATURE_BOUNCE_BOOST 0
#define QOL_FEATURE_TINY_GOOMBA_DROP_COIN 0
#define QOL_FEATURE_MARIO_HEAD_EASTER_EGG 0
#define QOL_FIX_WATER_RING 0
#define QOL_FIX_GROUND_POUND_WALL 0
#define QOL_FIX_LAVA_INTERACTION 0

//
// Extended bounds redefines
// OMM has built-in extended bounds
// Most of these "features" are unecessary and can break OMM
//

#if defined(EXT_BOUNDS)
#if defined(__cplusplus) // g++ doesn't really like _Static_assert, and extended_bounds calls it
#define __EXTENDED_BOUNDS_H__
#else
#include "engine/extended_bounds.h"
#endif
#endif
#undef EXTENDED_BOUNDS_MODE
#undef MAX_REFEREMCED_WALLS
#undef CELL_HEIGHT_LIMIT
#undef FLOOR_LOWER_LIMIT
#undef FLOOR_LOWER_LIMIT_MISC
#undef FLOOR_LOWER_LIMIT_SHADOW
#undef SURFACE_POOLS_FULL_MESSAGES
#undef NEW_WATER_SURFACES
#undef BETTER_WALL_COLLISION
#undef CEILING_MARGINS
#undef FIND_FLOOR_EXCLUDE_DYNAMIC_CHECK
#undef FIX_LEDGE_GRABS
#undef CENTERED_COLLISION
#undef UNDERWATER_STEEP_FLOORS_AS_WALLS
#undef MIN_UNDERWATER_FLOOR_NORMAL_Y
#undef MIN_FLOOR_NORMAL_Y
#undef MAX_CEIL_NORMAL_Y
#undef NULL_FLOOR_STEPS
#undef WATER_NUM_STEPS
#undef GROUND_NUM_STEPS
#undef AIR_NUM_STEPS
#undef LEVEL_BOUNDARY_MAX
#undef WORLD_SCALE
#undef CELL_SIZE
#undef NUM_CELLS
#undef NUM_CELLS_INDEX
#define EXTENDED_BOUNDS_H
#define MAX_REFEREMCED_WALLS        4
#define CELL_HEIGHT_LIMIT           20000
#define FLOOR_LOWER_LIMIT           -11000
#define FLOOR_LOWER_LIMIT_MISC      (FLOOR_LOWER_LIMIT + 1000)
#define FLOOR_LOWER_LIMIT_SHADOW    FLOOR_LOWER_LIMIT_MISC
#if defined(LEVEL_BOUNDS)
#define LEVEL_BOUNDARY_MAX          LEVEL_BOUNDS
#else
#define LEVEL_BOUNDARY_MAX          0x8000
#define CELL_SIZE                   0x400
#endif
#define NUM_CELLS                   (2 * (LEVEL_BOUNDARY_MAX / CELL_SIZE))
#define NUM_CELLS_INDEX             (NUM_CELLS - 1)

//
// DynOS defines
// OMM has new models that must be added to DynOS actors list
//

#if defined(DYNOS) && defined(DYNOS_CPP_H)
#include "data/omm/object/omm_object_data.h"
#define OMM_DYNOS_ACTORS \
define_actor(omm_geo_star_0_opaque), \
define_actor(omm_geo_star_1_opaque), \
define_actor(omm_geo_star_2_opaque), \
define_actor(omm_geo_star_3_opaque), \
define_actor(omm_geo_star_4_opaque), \
define_actor(omm_geo_star_5_opaque), \
define_actor(omm_geo_star_6_opaque), \
define_actor(omm_geo_star_7_opaque), \
define_actor(omm_geo_star_8_opaque), \
define_actor(omm_geo_star_9_opaque), \
define_actor(omm_geo_star_10_opaque), \
define_actor(omm_geo_star_11_opaque), \
define_actor(omm_geo_star_12_opaque), \
define_actor(omm_geo_star_13_opaque), \
define_actor(omm_geo_star_14_opaque), \
define_actor(omm_geo_star_15_opaque), \
define_actor(omm_geo_star_16_opaque), \
define_actor(omm_geo_star_17_opaque), \
define_actor(omm_geo_star_18_opaque), \
define_actor(omm_geo_star_19_opaque), \
define_actor(omm_geo_star_0_transparent), \
define_actor(omm_geo_star_1_transparent), \
define_actor(omm_geo_star_2_transparent), \
define_actor(omm_geo_star_3_transparent), \
define_actor(omm_geo_star_4_transparent), \
define_actor(omm_geo_star_5_transparent), \
define_actor(omm_geo_star_6_transparent), \
define_actor(omm_geo_star_7_transparent), \
define_actor(omm_geo_star_8_transparent), \
define_actor(omm_geo_star_9_transparent), \
define_actor(omm_geo_star_10_transparent), \
define_actor(omm_geo_star_11_transparent), \
define_actor(omm_geo_star_12_transparent), \
define_actor(omm_geo_star_13_transparent), \
define_actor(omm_geo_star_14_transparent), \
define_actor(omm_geo_star_15_transparent), \
define_actor(omm_geo_star_16_transparent), \
define_actor(omm_geo_star_17_transparent), \
define_actor(omm_geo_star_18_transparent), \
define_actor(omm_geo_star_19_transparent), \
define_actor(omm_geo_number), \
define_actor(omm_geo_star_number), \
define_actor(omm_geo_fire_smoke_red), \
define_actor(omm_geo_fire_smoke_blue), \
define_actor(omm_geo_metal_sparkle), \
define_actor(omm_geo_vanish_mist), \
define_actor(omm_geo_break_particle), \
define_actor(omm_geo_snowball), \
define_actor(omm_geo_mr_i_beam), \
define_actor(omm_geo_snufit_ball), \
define_actor(omm_geo_rock), \
define_actor(omm_geo_explosion), \
define_actor(omm_geo_blargg_fire_ball), \
define_actor(omm_geo_shockwave_whomp), \
define_actor(omm_geo_shockwave_spindrift), \
define_actor(omm_geo_shockwave_fire), \
define_actor(omm_geo_rising_lava), \
define_actor(omm_geo_bowser_mine), \
define_actor(omm_geo_bowser_flame), \
define_actor(omm_geo_bowser_fireball), \
define_actor(omm_geo_bowser_fireball_flame), \
define_actor(omm_geo_flaming_bobomb), \
define_actor(omm_geo_flaming_bobomb_aura), \
define_actor(omm_geo_flaming_bobomb_explosion), \
define_actor(omm_geo_sparkly_star_1_opaque), \
define_actor(omm_geo_sparkly_star_1_transparent), \
define_actor(omm_geo_sparkly_star_1_sparkle), \
define_actor(omm_geo_sparkly_star_1_block), \
define_actor(omm_geo_sparkly_star_1_hint), \
define_actor(omm_geo_sparkly_star_2_opaque), \
define_actor(omm_geo_sparkly_star_2_transparent), \
define_actor(omm_geo_sparkly_star_2_sparkle), \
define_actor(omm_geo_sparkly_star_2_block), \
define_actor(omm_geo_sparkly_star_2_hint), \
define_actor(omm_geo_sparkly_star_3_opaque), \
define_actor(omm_geo_sparkly_star_3_transparent), \
define_actor(omm_geo_sparkly_star_3_sparkle), \
define_actor(omm_geo_sparkly_star_3_block), \
define_actor(omm_geo_sparkly_star_3_hint), \
define_actor(omm_geo_peach), \
define_actor(omm_geo_peachs_cap), \
define_actor(omm_geo_peachs_metal_cap), \
define_actor(omm_geo_perry), \
define_actor(omm_geo_perry_trail), \
define_actor(omm_geo_perry_shockwave), \
define_actor(omm_geo_peach_vibe_aura), \
define_actor(omm_geo_peach_vibe_sparkle), \
define_actor(omm_geo_peach_vibe_joy_tornado), \
define_actor(omm_geo_peach_vibe_joy_gust), \
define_actor(omm_geo_peach_vibe_rage_aura), \
define_actor(omm_geo_peach_vibe_rage_shockwave), \
define_actor(omm_geo_peach_vibe_gloom_aura), \
define_actor(omm_geo_peach_vibe_gloom_tear), \
define_actor(omm_geo_peach_vibe_gloom_tear_small), \
define_actor(omm_geo_peach_vibe_calm_aura), \
define_actor(omm_geo_peach_vibe_calm_sparkle), \
define_actor(omm_geo_menu_button_mario), \
define_actor(omm_geo_menu_button_peach), \
define_actor(omm_geo_menu_button_luigi), \
define_actor(omm_geo_menu_button_wario), \
define_actor(omm_geo_problem)
#endif

//
// Saturn defines
// For Machinima features with OMM custom actions and code
//

#if defined(SATURN)
#include "saturn/saturn.h"
#else
#define enable_head_rotations   true
#define enable_dust_particles   true
#define is_anim_playing         false
#endif

//
// Some useful macros
// (don't use the macros starting with underscores)
//

// Count the number of args inside a variadic macro (up to 8)
#ifdef _MSC_VER
#define N_ARGS(...) __EXPAND_ARGS(__ARGS_AUG(__VA_ARGS__))
#define __ARGS_AUG(...) unused, __VA_ARGS__
#define __EXPAND(...) __VA_ARGS__
#define __EXPAND_ARGS(...) __EXPAND(__COUNT_ARGS(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0))
#define __COUNT_ARGS(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, count, ...) count
#else
#define N_ARGS(...) __COUNT_ARGS(0, ##__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __EXPAND(...) __VA_ARGS__
#define __COUNT_ARGS(_0_, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, count, ...) count
#endif

// Turn things into a string
#undef __STRINGIFY
#undef STRINGIFY
#define __STRINGIFY(...) #__VA_ARGS__
#define STRINGIFY(...) __STRINGIFY(__VA_ARGS__)

// Concatenate two macros
#define __CAT(A, B) A##B
#define CAT(A, B) __CAT(A, B)

//
// Button masks and binds
// (Note to future self: this code needs to
//  be here, don't move it to omm_patches.h)
//

#define X_BUTTON 0x0040
#define Y_BUTTON 0x0080

#define OMM_ADD_BIND(type, cont, var) extern u32 var[]; type##_add_binds(cont, var)
#define OMM_ADD_BINDS(type)                                         \
        OMM_ADD_BIND(type, A_BUTTON, gOmmControlsButtonA);          \
        OMM_ADD_BIND(type, B_BUTTON, gOmmControlsButtonB);          \
        OMM_ADD_BIND(type, X_BUTTON, gOmmControlsButtonX);          \
        OMM_ADD_BIND(type, Y_BUTTON, gOmmControlsButtonY);          \
        OMM_ADD_BIND(type, START_BUTTON, gOmmControlsButtonStart);  \
        OMM_ADD_BIND(type, L_TRIG, gOmmControlsTriggerL);           \
        OMM_ADD_BIND(type, R_TRIG, gOmmControlsTriggerR);           \
        OMM_ADD_BIND(type, Z_TRIG, gOmmControlsTriggerZ);           \
        OMM_ADD_BIND(type, U_CBUTTONS, gOmmControlsCUp);            \
        OMM_ADD_BIND(type, D_CBUTTONS, gOmmControlsCDown);          \
        OMM_ADD_BIND(type, L_CBUTTONS, gOmmControlsCLeft);          \
        OMM_ADD_BIND(type, R_CBUTTONS, gOmmControlsCRight);         \
        OMM_ADD_BIND(type, U_JPAD, gOmmControlsDUp);                \
        OMM_ADD_BIND(type, D_JPAD, gOmmControlsDDown);              \
        OMM_ADD_BIND(type, L_JPAD, gOmmControlsDLeft);              \
        OMM_ADD_BIND(type, R_JPAD, gOmmControlsDRight);             \
        OMM_ADD_BIND(type, STICK_UP, gOmmControlsStickUp);          \
        OMM_ADD_BIND(type, STICK_DOWN, gOmmControlsStickDown);      \
        OMM_ADD_BIND(type, STICK_LEFT, gOmmControlsStickLeft);      \
        OMM_ADD_BIND(type, STICK_RIGHT, gOmmControlsStickRight);
        
#define keyboard_bindkeys_0 keyboard_bindkeys()
#define keyboard_bindkeys_1 keyboard_bindkeys(void) { \
    bzero(keyboard_mapping, sizeof(keyboard_mapping)); \
    num_keybinds = 0; \
    OMM_ADD_BINDS(keyboard); \
 } void keyboard_bindkeys_(void)
#define keyboard_bindkeys(...) CAT(keyboard_bindkeys_, N_ARGS(__VA_ARGS__))

#define controller_sdl_bind_0 controller_sdl_bind()
#define controller_sdl_bind_1 controller_sdl_bind(void) { \
    bzero(joy_binds, sizeof(joy_binds)); \
    bzero(mouse_binds, sizeof(mouse_binds)); \
    num_joy_binds = 0; \
    num_mouse_binds = 0; \
    OMM_ADD_BINDS(controller); \
 } void controller_sdl_bind_(void)
#define controller_sdl_bind(...) CAT(controller_sdl_bind_, N_ARGS(__VA_ARGS__))

#endif

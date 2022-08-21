#ifndef OMM_MACROS_SMSR_H
#define OMM_MACROS_SMSR_H

// Better camera
#define BETTER_CAM_IS_PUPPY_CAM                 1
#define BETTER_CAM_IS_ENABLED                   gPuppyCam.enabled
#define BETTER_CAM_MODE                         0
#define BETTER_CAM_YAW                          0
#define BETTER_CAM_RAYCAST_ARGS                 __EXPAND(, UNUSED s32 flags)
#define BETTER_CAM_MOUSE_CAM                    (BETTER_CAM_IS_ENABLED && configCameraMouse)
#define gMouseXPos                              gMouseX
#define gMouseYPos                              gMouseY
#define gOldMouseXPos                           gMouseX
#define gOldMouseYPos                           gMouseY
#ifndef GFX_SCREEN_CONFIG_H // exclude configfile.c
#define configMouse                             configCameraMouse
#endif

// Animation
#define AnimInfoStruct                          AnimInfo
#define mAreaIndex                              areaIndex
#define mActiveAreaIndex                        activeAreaIndex
#define mModel                                  model
#define mAnimInfo                               animInfo
#define mAnimYTransDivisor                      animYTransDivisor
#define mStartFrame                             startFrame
#define mLoopStart                              loopStart
#define mLoopEnd                                loopEnd

// Mario animation
#define MarioAnimationsStruct                   struct DmaHandlerList
#define gMarioAnimations                        gMarioState->animList
#define gMarioCurrAnimAddr                      gMarioAnimations->currentAddr
#define gMarioTargetAnim                        gMarioAnimations->bufTarget
#define gMarioAnimsData                         gMarioAnimsBuf

// Audio
#define gGlobalSoundArgs                        gGlobalSoundSource
#define sAcousticReachPerLevel                  sLevelAcousticReaches
#define audio_play_wing_cap_music()             { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)); }
#define audio_play_metal_cap_music()            { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP)); }
#define audio_play_vanish_cap_music()           { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)); }
#define audio_stop_cap_music()                  { stop_cap_music(); }
#define audio_play_shell_music()                { play_shell_music(); }
#define audio_stop_shell_music()                { stop_shell_music(); }
#define audio_play_course_clear()               { play_course_clear(); }
#define audio_stop_course_clear()               
#define audio_play_puzzle_jingle()              { play_puzzle_jingle(); }
#define audio_play_toads_jingle()               { play_toads_jingle(); }
#define audio_play_star_jingle()                { play_power_star_jingle(1); }
#define audio_play_star_fanfare()               { play_star_fanfare(); }

// Music
#define music_pause()                           { gSequencePlayers[0].muted = true; }
#define music_resume()                          { gSequencePlayers[0].muted = false; }
#define music_stop()                            
#define audio_mute(...)                         { set_audio_muted(__VA_ARGS__); }
#define music_fade_out(...)                     { seq_player_fade_out(__VA_ARGS__); }
#define music_lower_volume(...)                 { seq_player_lower_volume(__VA_ARGS__); }
#define music_unlower_volume(...)               { seq_player_unlower_volume(__VA_ARGS__); }
#define music_play_sequence_define              1
#define MUSIC_QUEUE_MAX_SIZE                    MAX_BACKGROUND_MUSIC_QUEUE_SIZE

// Sound
#define sound_stop(...)                         { stop_sound(__VA_ARGS__); }
#define sound_stop_from_source(...)             { stop_sounds_from_source(__VA_ARGS__); }
#define sound_stop_continuous()                 { stop_sounds_in_continuous_banks(); }
#define sound_set_moving_speed(...)             { set_sound_moving_speed(__VA_ARGS__); }
#define SOUND_OBJ_WHOMP_SLAM                    SOUND_OBJ_WHOMP

// Object fields
#define oBhvArgs                                oBehParams
#define oBhvArgs2ndByte                         oBehParams2ndByte
#define oBhvArgsUnused                          oUnk1A8
#define oSnowmansBodyScale                      oSnowmansBottomScale
#define oBitsPlatformBowserObject               oBitsPlatformBowser
#define oBowserCameraState                      oBowserCamAct
#define oBowserFlags                            oBowserStatus
#define oBowserActionTimer                      oBowserTimer
#define oBowserBitsJump                         oBowserBitsJustJump
#define oBowserSpitFireNumFlames                oBowserRandSplitFloor
#define oBowserGrabbedAction                    oBowserGrabbedStatus
#define oBowserActionSelected                   oBowserIsReacting
#define oBowserOpacityTarget                    oBowserTargetOpacity
#define oBowserBlinkTimer                       oBowserEyesTimer
#define oBowserRainbowLightEffect               oBowserRainbowLight
#define oBowserShockwaveScale                   oBowserShockWaveScale

// Dialogs and menus
#define gDialogBoxState                         gMenuState
#define gDialogBoxAngle                         gDialogBoxAngle
#define gDialogLineIndex                        gMenuLineNum
#define gDialogTextPos                          gDialogPageStartStrIndex
#define gLastDialogPageStrPos                   gNextDialogPageStartStrIndex
#define DIALOG_STATE_OPENING                    MENU_STATE_DIALOG_OPENING
#define DIALOG_STATE_VERTICAL                   MENU_STATE_DIALOG_OPEN
#define DIALOG_STATE_HORIZONTAL                 MENU_STATE_DIALOG_SCROLLING
#define DIALOG_STATE_CLOSING                    MENU_STATE_DIALOG_CLOSING
#define GLOBAL_CHAR_TERMINATOR                  GLOBAR_CHAR_TERMINATOR

// Level scripts
#define LEVEL_SCRIPT_PRESSED_START              0
#define level_script_entry_point                level_script_entry
#define level_script_splash_screen              level_intro_splash_screen
#define level_script_goddard_regular            level_intro_mario_head_regular
#define level_script_goddard_game_over          level_intro_mario_head_dizzy
#define level_script_debug_level_select         level_intro_entry_4
#define level_script_to_file_select             script_intro_L1
#define level_script_to_debug_level_select      script_intro_L2
#define level_script_to_star_select_1           script_intro_L3
#define level_script_to_star_select_2           script_intro_L4
#define level_script_to_splash_screen           script_intro_L5
#define level_script_file_select                level_main_menu_entry_1
#define level_script_star_select                level_main_menu_entry_2
#define level_script_cake_ending                level_ending_entry

// Misc
#define check_surface_collisions_for_camera()   (gCheckingSurfaceCollisionsForCamera)
#define enable_surface_collisions_for_camera()  { gCheckingSurfaceCollisionsForCamera = TRUE; }
#define disable_surface_collisions_for_camera() { gCheckingSurfaceCollisionsForCamera = FALSE; }
#define check_surface_intangible_find_floor()   (gFindFloorIncludeSurfaceIntangible)
#define enable_surface_intangible_find_floor()  { gFindFloorIncludeSurfaceIntangible = TRUE; }
#define disable_surface_intangible_find_floor() { gFindFloorIncludeSurfaceIntangible = FALSE; }
#define find_static_floor(...)                  find_floor(__VA_ARGS__)
#define find_dynamic_floor(...)                 find_floor(__VA_ARGS__)
#define load_gfx_memory_pool()                  select_gfx_pool()
#define init_scene_rendering()                  init_rcp()
#define clear_framebuffer(...)                  clear_framebuffer(__VA_ARGS__)
#define INPUT_BOUNCE                            INPUT_STOMPED
#define MAX_REFEREMCED_WALLS                    MAX_REFERENCED_WALLS
#define MODEL_BOWSER2                           MODEL_BOWSER_NO_SHADOW

// OMM
#define OMM_LEVEL_ENTRY_WARP(level)             (level == LEVEL_JRB ? 0x01 : 0x0A)
#define OMM_LEVEL_EXIT_DISTANCE                 500
#define OMM_LEVEL_SLIDE                         LEVEL_SA
#define OMM_LEVEL_RETURN_TO_CASTLE              LEVEL_GROUNDS, 1, 0x80, 0
#define OMM_CAMERA_LOOK_UP_WARP_STARS           120
#define OMM_CAMERA_IS_BOWSER_FIGHT              (gCurrLevelNum == LEVEL_BOWSER_3)
#define OMM_NUM_PLAYABLE_CHARACTERS             2
#define OMM_CUTSCENE_MSG_INDEX                  8
#define OMM_PALETTE_EDITOR_MUSIC                0x33
#define STAR                                    "STAR"
#define Star                                    "Star"

// Files
#define FILE_MACRO_PRESETS_H                    "macro_preset_names.h"
#define FILE_SPECIAL_PRESETS_H                  "special_preset_names.h"
#define FILE_BETTERCAM_H                        "extras/bettercamera.h"
#define FILE_OPTIONS_H                          "extras/options_menu.h"
#define FILE_SOUNDS_H                           "sounds.h"
#define FILE_CHEATS_H                           "extras/cheats.h"
#define FILE_MARIO_CHEATS_H                     "extras/cheats.h"
#define FILE_TITLE_H                            "menu/title_screen.h"
#define FILE_TXT_CONV_H                         "types.h"
#define FILE_TEXT_LOADER_H                      "types.h"
#define FILE_R96_SYSTEM_H                       "types.h"

#endif

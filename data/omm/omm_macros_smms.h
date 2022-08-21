#ifndef OMM_MACROS_SMMS_H
#define OMM_MACROS_SMMS_H

// Better camera
#define BETTER_CAM_IS_PUPPY_CAM                 0
#define BETTER_CAM_IS_ENABLED                   newcam_active
#define BETTER_CAM_MODE                         CAMERA_MODE_NEWCAM
#define BETTER_CAM_YAW                          newcam_yaw
#define BETTER_CAM_RAYCAST_ARGS                 
#define BETTER_CAM_MOUSE_CAM                    (BETTER_CAM_IS_ENABLED && configCameraMouse)
#define mouse_x                                 gMouseX
#define mouse_y                                 gMouseY

// Animation
#define AnimInfoStruct                          GraphNodeObject_sub
#define mAreaIndex                              unk18
#define mActiveAreaIndex                        unk19
#define mModel                                  unk18
#define mAnimInfo                               unk38
#define mAnimYTransDivisor                      unk02
#define mStartFrame                             unk04
#define mLoopStart                              unk06
#define mLoopEnd                                unk08

// Mario animation
#define MarioAnimationsStruct                   struct MarioAnimation
#define gMarioAnimations                        gMarioState->animation
#define gMarioCurrAnimAddr                      gMarioAnimations->currentAnimAddr
#define gMarioTargetAnim                        gMarioAnimations->targetAnim
#define gMarioAnimsData                         D_80339D10

// Audio
#define gGlobalSoundArgs                        gDefaultSoundArgs
#define sAcousticReachPerLevel                  D_80332028
#define audio_play_wing_cap_music()             { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)); }
#define audio_play_metal_cap_music()            { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP)); }
#define audio_play_vanish_cap_music()           { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)); }
#define audio_stop_cap_music()                  { stop_cap_music(); }
#define audio_play_shell_music()                { play_shell_music(); }
#define audio_stop_shell_music()                { stop_shell_music(); }
#define audio_play_course_clear()               { play_course_clear(); }
#define audio_stop_course_clear()               { stop_background_music(SEQ_EVENT_BOSS); play_secondary_music(0, 0, 0, 0); func_80321080(60); }
#define audio_play_puzzle_jingle()              { play_puzzle_jingle(); }
#define audio_play_toads_jingle()               { play_toads_jingle(); }
#define audio_play_star_jingle()                { play_power_star_jingle(1); }
#define audio_play_star_fanfare()               { play_star_fanfare(); }

// Music
#define music_pause()                           { gSequencePlayers[0].muted = true; }
#define music_resume()                          { gSequencePlayers[0].muted = false; }
#define music_stop()                            
#define audio_mute(...)                         { set_sound_disabled(__VA_ARGS__); }
#define music_fade_out(...)                     { sequence_player_fade_out(__VA_ARGS__); }
#define music_lower_volume(...)                 { func_8031FFB4(__VA_ARGS__); }
#define music_unlower_volume(...)               { sequence_player_unlower(__VA_ARGS__); }
#define music_play_sequence_define              0
#define MUSIC_QUEUE_MAX_SIZE                    MAX_BG_MUSIC_QUEUE_SIZE

// Sound
#define sound_stop(...)                         { func_803205E8(__VA_ARGS__); }
#define sound_stop_from_source(...)             { func_803206F8(__VA_ARGS__); }
#define sound_stop_continuous()                 { func_80320890(); }
#define sound_set_moving_speed(...)             { func_80320A4C(__VA_ARGS__); }
#define SOUND_OBJ_WHOMP_SLAM                    SOUND_OBJ_WHOMP_LOWPRIO

// Object fields
#define oBhvArgs                                oBehParams
#define oBhvArgs2ndByte                         oBehParams2ndByte
#define oBhvArgsUnused                          oUnk1A8
#define oSnowmansBodyScale                      oSnowmansBottomUnkF4
#define oBitsPlatformBowserObject               oPlatformUnkF8
#define oBowserCameraState                      oBowserUnk88
#define oBowserFlags                            oBowserUnkF4
#define oBowserActionTimer                      oBowserUnkF8
#define oBowserBitsJump                         oBowserUnk106
#define oBowserSpitFireNumFlames                oBowserUnk108
#define oBowserGrabbedAction                    oBowserUnk10E
#define oBowserActionSelected                   oBowserUnk110
#define oBowserOpacityTarget                    oBowserUnk1AC
#define oBowserBlinkTimer                       oBowserUnk1AE
#define oBowserRainbowLightEffect               oBowserUnk1B2
#define oBowserShockwaveScale                   oBowserShockWaveUnkF4

// Dialogs and menus
#define gDialogBoxState                         gDialogBoxState
#define gDialogBoxAngle                         gDialogBoxOpenTimer
#define gDialogLineIndex                        gDialogLineNum
#define GLOBAL_CHAR_TERMINATOR                  GLOBAR_CHAR_TERMINATOR

// Level scripts
#define LEVEL_SCRIPT_PRESSED_START              1
#define level_script_entry_point                level_script_entry
#define level_script_splash_screen              level_intro_entry_1
#define level_script_goddard_regular            level_intro_entry_2
#define level_script_goddard_game_over          level_intro_entry_3
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
#define load_gfx_memory_pool()                  config_gfx_pool()
#define init_scene_rendering()                  init_render_image()
#define clear_framebuffer(...)                  clear_frame_buffer(__VA_ARGS__)
#define INPUT_BOUNCE                            INPUT_UNKNOWN_10

// OMM
#define OMM_LEVEL_ENTRY_WARP(level)             0x0A
#define OMM_LEVEL_EXIT_DISTANCE                 500
#define OMM_LEVEL_SLIDE                         LEVEL_PSS
#define OMM_LEVEL_RETURN_TO_CASTLE              LEVEL_CASTLE, 1, 0x1F, 0
#define OMM_CAMERA_LOOK_UP_WARP_STARS           10
#define OMM_CAMERA_IS_BOWSER_FIGHT              omm_camera_is_bowser_fight()
#define OMM_NUM_PLAYABLE_CHARACTERS             2
#define OMM_CUTSCENE_MSG_INDEX                  8
#define OMM_PALETTE_EDITOR_MUSIC                SEQ_MENU_TITLE_SCREEN
#define STAR                                    "MOON"
#define Star                                    "Moon"

// Files
#define FILE_MACRO_PRESETS_H                    "macro_preset_names.h"
#define FILE_SPECIAL_PRESETS_H                  "special_preset_names.h"
#define FILE_BETTERCAM_H                        "game/bettercamera.h"
#define FILE_OPTIONS_H                          "game/options_menu.h"
#define FILE_SOUNDS_H                           "audio_defines.h"
#define FILE_CHEATS_H                           "pc/cheats.h"
#define FILE_MARIO_CHEATS_H                     "pc/cheats.h"
#define FILE_TITLE_H                            "menu/level_select_menu.h"
#define FILE_TXT_CONV_H                         "types.h"
#define FILE_TEXT_LOADER_H                      "types.h"
#define FILE_R96_SYSTEM_H                       "types.h"

#endif

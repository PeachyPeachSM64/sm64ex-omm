#ifndef OMM_MACROS_R96X_H
#define OMM_MACROS_R96X_H

// Game macros
#define OMM_GAME_TYPE                                   OMM_GAME_SM64
#define OMM_GAME_SAVE                                   OMM_GAME_R96X
#define OMM_GAME_MODE                                   0
#define OMM_GAME_MENU                                   "sm64"
#define OMM_GAME_RF14                                   0

// Better camera
#define BETTER_CAM_IS_PUPPY_CAM                         0
#define BETTER_CAM_IS_ENABLED                           newcam_active
#define BETTER_CAM_MODE                                 CAMERA_MODE_NEWCAM
#define BETTER_CAM_YAW                                  newcam_yaw
#define BETTER_CAM_RAYCAST_ARGS
#define BETTER_CAM_MOUSE_CAM                            (BETTER_CAM_IS_ENABLED && configCameraMouse)
#define mouse_x                                         gMouseDeltaX
#define mouse_y                                         gMouseDeltaY
#define gMouseXPos                                      gMouseDeltaX
#define gMouseYPos                                      gMouseDeltaY
#define gOldMouseXPos                                   gMouseDeltaX
#define gOldMouseYPos                                   gMouseDeltaY

// Animation
#define struct_AnimInfo                                 struct GraphNodeObject_sub
#define mAreaIndex                                      unk18
#define mActiveAreaIndex                                unk19
#define mModel                                          unk18
#define mAnimInfo                                       curAnim
#define mStartFrame                                     unk04
#define mLoopStart                                      unk06
#define mLoopEnd                                        unk08

// Mario animation
#define struct_MarioAnimations                          struct MarioAnimation
#define struct_MarioAnimDmaTable                        struct MarioAnimDmaRelatedThing
#define gMarioAnimations                                gMarioState->animation
#define gMarioAnimDmaTable                              gMarioAnimations->animDmaTable
#define gMarioCurrAnimAddr                              gMarioAnimations->currentAnimAddr
#define gMarioTargetAnim                                gMarioAnimations->targetAnim
#define gMarioAnimsData                                 Data_MarioAnims

// Audio
#define gGlobalSoundArgs                                gDefaultSoundArgs
#define gAcousticReachPerLevel                          D_80332028
#define audio_play_wing_cap_music()                     { r96_play_cap_music(R96_EVENT_CAP_WING); }
#define audio_play_metal_cap_music()                    { r96_play_cap_music(R96_EVENT_CAP_METAL); }
#define audio_play_vanish_cap_music()                   { r96_play_cap_music(R96_EVENT_CAP_VANISH); }
#define audio_stop_cap_music()                          { r96_stop_cap_music(); }
#define audio_play_shell_music()                        { r96_play_shell_music(); }
#define audio_stop_shell_music()                        { r96_stop_shell_music(); }
#define audio_play_course_clear()                       { dynos_music_pause(); r96_play_jingle(R96_EVENT_STAR_COLLECT, 0.1, 1.0, 1); }
#define audio_stop_course_clear()                       { dynos_music_resume(); }
#define audio_play_puzzle_jingle()                      { r96_play_jingle(R96_EVENT_SOLVE_PUZZLE, 0.1, 1.0, 1); r96_music_fade(0, -1, 0.0, 1, 1); }
#define audio_play_toads_jingle()                       { r96_play_jingle(R96_EVENT_TOAD_MESSAGE, 0.1, 1.0, 1); r96_music_fade(0, -1, 0.0, 1, 1); }
#define audio_play_star_jingle()                        { r96_play_jingle(R96_EVENT_STAR_APPEAR, 0.1, 1.0, 1); r96_music_fade(0, -1, 0.0, 1, 1); }
#define audio_play_star_fanfare()                       { r96_play_jingle(R96_EVENT_STAR_FANFARE, 0.1, 1.0, 1); r96_music_fade(0, -1, 0.0, 1, 1); }

// Music
#define music_pause()                                   { dynos_music_pause(); }
#define music_resume()                                  { dynos_music_resume(); }
#define music_stop()                                    { dynos_music_stop(); dynos_jingle_stop(); }
#define audio_mute(...)                                 { set_sound_disabled(__VA_ARGS__); }
#define music_fade_out(...)                             { sequence_player_fade_out(__VA_ARGS__); }
#define music_lower_volume(...)                         { func_8031FFB4(__VA_ARGS__); }
#define music_unlower_volume(...)                       { sequence_player_unlower(__VA_ARGS__); }

// Sound
#define gSfxMute                                        configSfxMute
#define sound_stop(...)                                 { func_803205E8(__VA_ARGS__); }
#define sound_stop_from_source(...)                     { func_803206F8(__VA_ARGS__); }
#define sound_stop_continuous()                         { func_80320890(); }
#define sound_set_moving_speed(...)                     { func_80320A4C(__VA_ARGS__); }

// Object fields
#define oSnowmansBodyScale                              oSnowmansBottomUnkF4
#define oSkeeterTurnAwayFromWall                        oSkeeterUnkF8
#define oBitsPlatformBowserObject                       oPlatformUnkF8
#define oBowserCameraState                              oBowserUnk88
#define oBowserFlags                                    oBowserUnkF4
#define oBowserActionTimer                              oBowserUnkF8
#define oBowserBitsJump                                 oBowserUnk106
#define oBowserSpitFireNumFlames                        oBowserUnk108
#define oBowserGrabbedAction                            oBowserUnk10E
#define oBowserActionSelected                           oBowserUnk110
#define oBowserOpacityTarget                            oBowserUnk1AC
#define oBowserBlinkTimer                               oBowserUnk1AE
#define oBowserRainbowLightEffect                       oBowserUnk1B2
#define oBowserShockwaveScale                           oBowserShockWaveUnkF4

// Text
#define gCourseNameTable(...)                           ((const u8 **) seg2_course_name_table)
#define gActNameTable(...)                              ((const u8 **) seg2_act_name_table)
#define gDialogTable(...)                               ((struct DialogEntry **) dialogPool)

// Level scripts
#define level_script_entry_point                        level_script_entry
#define level_script_splash_screen                      level_intro_entry_1
#define level_script_goddard_regular                    level_intro_entry_2
#define level_script_goddard_game_over                  level_intro_entry_3
#define level_script_debug_level_select                 level_intro_entry_4
#define level_script_to_file_select                     script_intro_L1
#define level_script_to_debug_level_select              script_intro_L2
#define level_script_to_star_select_1                   script_intro_L3
#define level_script_to_star_select_2                   script_intro_L4
#define level_script_to_splash_screen                   script_intro_L5
#define level_script_file_select                        level_main_menu_entry_1
#define level_script_star_select                        level_main_menu_entry_2
#define level_script_cake_ending                        level_ending_entry

// OMM
#define OMM_MARIO_COLORS                                0
#define OMM_CAP_COLORS                                  0
#define OMM_STAR_COLORS                                 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
#define OMM_STAR_COLOR_OFFSET(...)                      0
#define OMM_STAR_COLOR_COUNT                            20
#define OMM_STAR_COLOR_CLASSIC                          { 0xFF, 0xFF, 0x00 }
#define OMM_LEVEL_ENTRY_WARP(levelNum)                  0x0A
#define OMM_LEVEL_EXIT_DISTANCE                         500
#define OMM_LEVEL_SLIDE                                 LEVEL_PSS
#define OMM_LEVEL_ENTRY_POINT                           LEVEL_CASTLE_GROUNDS
#define OMM_LEVEL_RETURN_TO_CASTLE                      LEVEL_CASTLE, 1, 0x1F
#define OMM_LEVEL_YELLOW_COIN_BOO                       LEVEL_CASTLE_COURTYARD
#define OMM_LEVEL_EXCLUDE_LIST                          LEVEL_NONE
#define OMM_LEVEL_HAS_BOWSER                            1
#define OMM_SEQ_MAIN_MENU                               SEQ_MENU_TITLE_SCREEN
#define OMM_SEQ_FILE_SELECT                             SEQ_MENU_FILE_SELECT
#define OMM_SEQ_STAR_SELECT                             SEQ_MENU_STAR_SELECT
#define OMM_SEQ_PALETTE_EDITOR                          SEQ_MENU_TITLE_SCREEN
#define OMM_STATS_BOARD_LEVEL                           LEVEL_CASTLE_GROUNDS
#define OMM_STATS_BOARD_AREA                            1
#define OMM_STATS_BOARD_X                               -5350
#define OMM_STATS_BOARD_Y                               543
#define OMM_STATS_BOARD_Z                               -4000
#define OMM_STATS_BOARD_ANGLE                           0x0000
#define OMM_CAMERA_LOOK_UP_WARP_STARS                   10
#define OMM_CAMERA_IS_BOWSER_FIGHT                      omm_camera_is_bowser_fight()
#define OMM_NUM_PLAYABLE_CHARACTERS                     4
#define OMM_NUM_SAVE_MODES                              OMM_GAME_R96X__MODES
#define OMM_NUM_STARS_MAX_PER_COURSE                    OMM_GAME_R96X__STARS
#define OMM_NUM_ACTS_MAX_PER_COURSE                     (OMM_NUM_STARS_MAX_PER_COURSE - 1)
#define OMM_NUM_MIPS_STARS                              2
#define OMM_TEXT_FORMAT(id, str)                        "OMM_TEXT_" id
#define STAR                                            "STAR"
#define Star                                            "Star"

// Sparkly stars
#define OMM_SPARKLY_BLOCK_LEVEL                         LEVEL_CASTLE_GROUNDS
#define OMM_SPARKLY_BLOCK_AREA                          1
#define OMM_SPARKLY_BLOCK_COUNT                         3
#define OMM_SPARKLY_BLOCK_AVAILABLE                     0b1111
#define OMM_SPARKLY_BLOCK_X                             0
#define OMM_SPARKLY_BLOCK_Y                             820
#define OMM_SPARKLY_BLOCK_Z                             480
#define OMM_SPARKLY_BLOCK_ANGLE                         0x4000

// Extra text
#define OMM_TEXT_FS_PLAY                                OMM_TEXT_FS_SELECT_FILE, NULL, NULL, NULL
#define OMM_TEXT_FS_COPY                                OMM_TEXT_FS_COPY_FILE, NULL, NULL, NULL
#define OMM_TEXT_FS_ERASE                               OMM_TEXT_FS_ERASE_FILE, OMM_TEXT_FS_SPARKLY_STARS, NULL, NULL
#define OMM_TEXT_FS_SCORE                               OMM_TEXT_FS_SCORES, OMM_TEXT_FS_SPARKLY_STARS, NULL, NULL

// Files
#define FILE_BETTERCAM_H                                "game/bettercamera.h"
#define FILE_OPTIONS_H                                  "game/options_menu.h"
#define FILE_SOUNDS_H                                   "audio_defines.h"
#define FILE_CHEATS_H                                   "pc/cheats.h"
#define FILE_MARIO_CHEATS_H                             "game/mario_cheats.h"
#define FILE_TITLE_H                                    "menu/level_select_menu.h"
#define FILE_TXT_CONV_H                                 "text/txtconv.h"
#define FILE_TEXT_LOADER_H                              "text/text-loader.h"
#define FILE_R96_SYSTEM_H                               "data/r96/system/r96_system.h"

#endif

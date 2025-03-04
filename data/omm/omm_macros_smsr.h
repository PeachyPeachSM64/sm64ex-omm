#ifndef OMM_MACROS_SMSR_H
#define OMM_MACROS_SMSR_H

// Game macros
#define OMM_GAME_TYPE                                   OMM_GAME_SMSR
#define OMM_GAME_SAVE                                   OMM_GAME_SMSR
#define OMM_GAME_MODE                                   0
#define OMM_GAME_MENU                                   "smsr"
#define OMM_GAME_RF14                                   1

// Better camera
#define BETTER_CAM_IS_PUPPY_CAM                         1
#define BETTER_CAM_IS_ENABLED                           gPuppyCam.enabled
#define BETTER_CAM_MODE                                 0
#define BETTER_CAM_YAW                                  0
#define BETTER_CAM_RAYCAST_ARGS                         __EXPAND(, UNUSED s32 flags)
#define BETTER_CAM_MOUSE_CAM                            (BETTER_CAM_IS_ENABLED && configCameraMouse)
#define mouse_x                                         gMouseDeltaX
#define mouse_y                                         gMouseDeltaY
#define gMouseXPos                                      gMouseDeltaX
#define gMouseYPos                                      gMouseDeltaY
#define gOldMouseXPos                                   gMouseDeltaX
#define gOldMouseYPos                                   gMouseDeltaY

// Animation
#define struct_AnimInfo                                 struct AnimInfo
#define mAreaIndex                                      areaIndex
#define mActiveAreaIndex                                activeAreaIndex
#define mModel                                          model
#define mAnimInfo                                       animInfo
#define mStartFrame                                     startFrame
#define mLoopStart                                      loopStart
#define mLoopEnd                                        loopEnd

// Mario animation
#define struct_MarioAnimations                          struct DmaHandlerList
#define struct_MarioAnimDmaTable                        struct DmaTable
#define gMarioAnimations                                gMarioState->animList
#define gMarioAnimDmaTable                              gMarioAnimations->dmaTable
#define gMarioCurrAnimAddr                              gMarioAnimations->currentAddr
#define gMarioTargetAnim                                gMarioAnimations->bufTarget
#define gMarioAnimsData                                 gMarioAnimsBuf

// Audio
#define gGlobalSoundArgs                                gGlobalSoundSource
#define gAcousticReachPerLevel                          sLevelAcousticReaches
#define audio_play_wing_cap_music()                     { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)); }
#define audio_play_metal_cap_music()                    { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP)); }
#define audio_play_vanish_cap_music()                   { play_cap_music(SEQUENCE_ARGS(4, 0x32)); }
#define audio_stop_cap_music()                          { stop_cap_music(); }
#define audio_play_shell_music()                        { play_shell_music(); }
#define audio_stop_shell_music()                        { stop_shell_music(); }
#define audio_play_course_clear()                       { play_course_clear(); }
#define audio_stop_course_clear()                       {}
#define audio_play_puzzle_jingle()                      { play_puzzle_jingle(); }
#define audio_play_toads_jingle()                       { play_toads_jingle(); }
#define audio_play_star_jingle()                        { play_power_star_jingle(1); }
#define audio_play_star_fanfare()                       { play_star_fanfare(); }

// Music
#define music_pause()                                   { gSequencePlayers[0].muted = 1; }
#define music_resume()                                  { gSequencePlayers[0].muted = 0; }
#define music_stop()                                    {}
#define audio_mute(...)                                 { set_audio_muted(__VA_ARGS__); }
#define music_fade_out(...)                             { seq_player_fade_out(__VA_ARGS__); }
#define music_lower_volume(...)                         { seq_player_lower_volume(__VA_ARGS__); }
#define music_unlower_volume(...)                       { seq_player_unlower_volume(__VA_ARGS__); }

// Sound
#define gSfxMute                                        0
#define sound_stop(...)                                 { stop_sound(__VA_ARGS__); }
#define sound_stop_from_source(...)                     { stop_sounds_from_source(__VA_ARGS__); }
#define sound_stop_continuous()                         { stop_sounds_in_continuous_banks(); }
#define sound_set_moving_speed(...)                     { set_sound_moving_speed(__VA_ARGS__); }

// Object fields
#define oSnowmansBodyScale                              oSnowmansBottomScale
#define oSkeeterTurnAwayFromWall                        oSkeeterTurningAwayFromWall
#define oBitsPlatformBowserObject                       oBitsPlatformBowser
#define oBowserCameraState                              oBowserCamAct
#define oBowserFlags                                    oBowserStatus
#define oBowserActionTimer                              oBowserTimer
#define oBowserBitsJump                                 oBowserBitsJustJump
#define oBowserSpitFireNumFlames                        oBowserRandSplitFloor
#define oBowserGrabbedAction                            oBowserGrabbedStatus
#define oBowserActionSelected                           oBowserIsReacting
#define oBowserOpacityTarget                            oBowserTargetOpacity
#define oBowserBlinkTimer                               oBowserEyesTimer
#define oBowserRainbowLightEffect                       oBowserRainbowLight
#define oBowserShockwaveScale                           oBowserShockWaveScale

// Text
#define gCourseNameTable(...)                           ((const u8 **) seg2_course_name_table)
#define gActNameTable(...)                              ((const u8 **) seg2_act_name_table)
#define gDialogTable(...)                               ((struct DialogEntry **) seg2_dialog_table)

// Level scripts
#define level_script_entry_point                        level_script_entry
#define level_script_splash_screen                      level_intro_splash_screen
#define level_script_goddard_regular                    level_intro_mario_head_regular
#define level_script_goddard_game_over                  level_intro_mario_head_dizzy
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
#define OMM_MARIO_COLORS                                1
#define OMM_CAP_COLORS                                  1
#define OMM_STAR_COLORS                                 0, 1, 4, 11, 9, 5, 3, 8, 13, 2, 10, 14, 12, 7, 6, 15, 16, 17, 18, 19,
#define OMM_STAR_COLOR_OFFSET(...)                      0
#define OMM_STAR_COLOR_COUNT                            20
#define OMM_STAR_COLOR_CLASSIC                          { 0xFF, 0xFF, 0x00 }
#define OMM_LEVEL_ENTRY_WARP(levelNum)                  ((levelNum) == LEVEL_JRB ? 0x01 : 0x0A)
#define OMM_LEVEL_EXIT_DISTANCE                         500
#define OMM_LEVEL_SLIDE                                 LEVEL_SA
#define OMM_LEVEL_ENTRY_POINT                           LEVEL_CASTLE_GROUNDS
#define OMM_LEVEL_RETURN_TO_CASTLE                      LEVEL_CASTLE_GROUNDS, 1, 0x80
#define OMM_LEVEL_YELLOW_COIN_BOO                       LEVEL_NONE
#define OMM_LEVEL_EXCLUDE_LIST                          LEVEL_NONE
#define OMM_LEVEL_HAS_BOWSER                            1
#define OMM_SEQ_MAIN_MENU                               0x21
#define OMM_SEQ_FILE_SELECT                             0x31
#define OMM_SEQ_STAR_SELECT                             0x2D
#define OMM_SEQ_PALETTE_EDITOR                          0x33
#define OMM_STATS_BOARD_LEVEL                           LEVEL_CASTLE_GROUNDS
#define OMM_STATS_BOARD_AREA                            1
#define OMM_STATS_BOARD_X                               -4000
#define OMM_STATS_BOARD_Y                               -614
#define OMM_STATS_BOARD_Z                               -3500
#define OMM_STATS_BOARD_ANGLE                           0xC000
#define OMM_CAMERA_LOOK_UP_WARP_STARS                   120
#define OMM_CAMERA_IS_BOWSER_FIGHT                      (gCurrLevelNum == LEVEL_BOWSER_3)
#define OMM_NUM_PLAYABLE_CHARACTERS                     2
#define OMM_NUM_SAVE_MODES                              OMM_GAME_SMSR__MODES
#define OMM_NUM_STARS_MAX_PER_COURSE                    OMM_GAME_SMSR__STARS
#define OMM_NUM_ACTS_MAX_PER_COURSE                     (OMM_NUM_STARS_MAX_PER_COURSE - 1)
#define OMM_NUM_MIPS_STARS                              1
#define OMM_TEXT_FORMAT(id, str)                        str
#define STAR                                            "STAR"
#define Star                                            "Star"

// Sparkly stars
#define OMM_SPARKLY_BLOCK_LEVEL                         LEVEL_CASTLE_COURTYARD
#define OMM_SPARKLY_BLOCK_AREA                          1
#define OMM_SPARKLY_BLOCK_COUNT                         1
#define OMM_SPARKLY_BLOCK_AVAILABLE                     0b1001
#define OMM_SPARKLY_BLOCK_X                             -990
#define OMM_SPARKLY_BLOCK_Y                             720
#define OMM_SPARKLY_BLOCK_Z                             -2340
#define OMM_SPARKLY_BLOCK_ANGLE                         0x0000

// Extra text
#define OMM_TEXT_FS_PLAY                                OMM_TEXT_FS_SELECT_FILE, NULL, NULL, NULL
#define OMM_TEXT_FS_COPY                                OMM_TEXT_FS_COPY_FILE, NULL, NULL, NULL
#define OMM_TEXT_FS_ERASE                               OMM_TEXT_FS_ERASE_FILE, OMM_TEXT_FS_SPARKLY_STARS, NULL, NULL
#define OMM_TEXT_FS_SCORE                               OMM_TEXT_FS_SCORES, OMM_TEXT_FS_SPARKLY_STARS, NULL, NULL

// Files
#define FILE_BETTERCAM_H                                "extras/bettercamera.h"
#define FILE_OPTIONS_H                                  "extras/options_menu.h"
#define FILE_SOUNDS_H                                   "sounds.h"
#define FILE_CHEATS_H                                   "extras/cheats.h"
#define FILE_MARIO_CHEATS_H                             "extras/cheats.h"
#define FILE_TITLE_H                                    "menu/title_screen.h"
#define FILE_TXT_CONV_H                                 "types.h"
#define FILE_TEXT_LOADER_H                              "types.h"
#define FILE_R96_SYSTEM_H                               "types.h"

#endif

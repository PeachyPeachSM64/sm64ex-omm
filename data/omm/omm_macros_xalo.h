#ifndef OMM_MACROS_XALO_H
#define OMM_MACROS_XALO_H

// Game macros
#define OMM_GAME_CODE                                   "XALO"
#define OMM_GAME_NAME                                   "Super Mario 64 (ex-alo)"
#define OMM_GAME_TYPE                                   OMM_GAME_SM64
#define OMM_GAME_SAVE                                   OMM_GAME_XALO
#define OMM_GAME_MODE                                   0

// Better camera
#define BETTER_CAM_IS_PUPPY_CAM                         1
#define BETTER_CAM_IS_ENABLED                           gPuppyCam.enabled
#define BETTER_CAM_MODE                                 0
#define BETTER_CAM_YAW                                  0
#define BETTER_CAM_RAYCAST_ARGS                         __EXPAND(, UNUSED s32 flags)
#define BETTER_CAM_MOUSE_CAM                            (BETTER_CAM_IS_ENABLED && configCameraMouse)
#define gMouseXPos                                      gMouseX
#define gMouseYPos                                      gMouseY
#define gOldMouseXPos                                   gMouseX
#define gOldMouseYPos                                   gMouseY
#ifndef GFX_SCREEN_CONFIG_H // exclude configfile.c
#define configMouse                                     configCameraMouse
#endif

// Animation
#define AnimInfoStruct                                  AnimInfo
#define mAreaIndex                                      areaIndex
#define mActiveAreaIndex                                activeAreaIndex
#define mModel                                          model
#define mAnimInfo                                       animInfo
#define mAnimYTransDivisor                              animYTransDivisor
#define mStartFrame                                     startFrame
#define mLoopStart                                      loopStart
#define mLoopEnd                                        loopEnd

// Mario animation
#define MarioAnimationsStruct                           struct DmaHandlerList
#define gMarioAnimations                                gMarioState->animList
#define gMarioCurrAnimAddr                              gMarioAnimations->currentAddr
#define gMarioTargetAnim                                gMarioAnimations->bufTarget
#define gMarioAnimsData                                 gMarioAnimsBuf

// Audio
#define gGlobalSoundArgs                                gGlobalSoundSource
#define sAcousticReachPerLevel                          sLevelAcousticReaches
#define audio_play_wing_cap_music()                     { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)); }
#define audio_play_metal_cap_music()                    { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP)); }
#define audio_play_vanish_cap_music()                   { play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP)); }
#define audio_stop_cap_music()                          { stop_cap_music(); }
#define audio_play_shell_music()                        { play_shell_music(); }
#define audio_stop_shell_music()                        { stop_shell_music(); }
#define audio_play_course_clear()                       { play_course_clear(); }
#define audio_stop_course_clear()               
#define audio_play_puzzle_jingle()                      { play_puzzle_jingle(); }
#define audio_play_toads_jingle()                       { play_toads_jingle(); }
#define audio_play_star_jingle()                        { play_power_star_jingle(1); }
#define audio_play_star_fanfare()                       { play_star_fanfare(); }

// Music
#define music_pause()                                   { gSequencePlayers[0].muted = 1; }
#define music_resume()                                  { gSequencePlayers[0].muted = 0; }
#define music_stop()                            
#define audio_mute(...)                                 { set_audio_muted(__VA_ARGS__); }
#define music_fade_out(...)                             { seq_player_fade_out(__VA_ARGS__); }
#define music_lower_volume(...)                         { seq_player_lower_volume(__VA_ARGS__); }
#define music_unlower_volume(...)                       { seq_player_unlower_volume(__VA_ARGS__); }
#define music_play_sequence_define                      1
#define MUSIC_QUEUE_MAX_SIZE                            MAX_BACKGROUND_MUSIC_QUEUE_SIZE

// Sound
#define sound_stop(...)                                 { stop_sound(__VA_ARGS__); }
#define sound_stop_from_source(...)                     { stop_sounds_from_source(__VA_ARGS__); }
#define sound_stop_continuous()                         { stop_sounds_in_continuous_banks(); }
#define sound_set_moving_speed(...)                     { set_sound_moving_speed(__VA_ARGS__); }
#define SOUND_OBJ_WHOMP_SLAM                            SOUND_OBJ_WHOMP

// Object fields
#define oFloorType                                      OBJECT_FIELD_S16(0x4C, 0)
#define oFloorRoom                                      OBJECT_FIELD_S16(0x4C, 1)
#define oBhvArgs                                        oBhvParams
#define oBhvArgs2ndByte                                 oBhvParams2ndByte
#define oBhvArgsUnused                                  oUnusedBhvParams
#define oToadMessageDialogId                            oToadMessageDialogID
#define oSnowmansBodyScale                              oSnowmansBottomScale
#define oBitsPlatformBowserObject                       oBitSPlatformBowser
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

// Tables, dialogs and menus
#define gCourseNameTable                                ((const u8 **) seg2_course_name_table)
#define gActNameTable                                   ((const u8 **) seg2_act_name_table)
#define gDialogTable                                    ((struct DialogEntry **) seg2_dialog_table)
#define gDialogBoxState                                 gMenuState
#define gDialogBoxAngle                                 gDialogBoxAngle
#define gDialogLineIndex                                gMenuLineNum
#define gDialogTextPos                                  gDialogPageStartStrIndex
#define gDialogTextAlpha                                gMenuTextAlpha
#define gLastDialogPageStrPos                           gNextDialogPageStartStrIndex
#define DIALOG_STATE_OPENING                            MENU_STATE_DIALOG_OPENING
#define DIALOG_STATE_VERTICAL                           MENU_STATE_DIALOG_OPEN
#define DIALOG_STATE_HORIZONTAL                         MENU_STATE_DIALOG_SCROLLING
#define DIALOG_STATE_CLOSING                            MENU_STATE_DIALOG_CLOSING

// Level scripts
#define LEVEL_SCRIPT_PRESSED_START                      0
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

// Misc
#define check_surface_collisions_for_camera()           (gCollisionFlags & COLLISION_FLAG_CAMERA)
#define enable_surface_collisions_for_camera()          { gCollisionFlags |= COLLISION_FLAG_CAMERA; }
#define disable_surface_collisions_for_camera()         { gCollisionFlags &= ~COLLISION_FLAG_CAMERA; }
#define check_surface_intangible_find_floor()           (gCollisionFlags & COLLISION_FLAG_INCLUDE_INTANGIBLE)
#define enable_surface_intangible_find_floor()          { gCollisionFlags |= COLLISION_FLAG_INCLUDE_INTANGIBLE; }
#define disable_surface_intangible_find_floor()         { gCollisionFlags &= ~COLLISION_FLAG_INCLUDE_INTANGIBLE; }
#define find_static_floor(...)                          find_floor(__VA_ARGS__)
#define find_dynamic_floor(...)                         find_floor(__VA_ARGS__)
#define load_gfx_memory_pool()                          select_gfx_pool()
#define init_scene_rendering()                          init_rcp()
#define clear_framebuffer(...)                          clear_framebuffer(__VA_ARGS__)
#define INPUT_BOUNCE                                    INPUT_STOMPED
#define MODEL_BOWSER2                                   MODEL_BOWSER_NO_SHADOW

// OMM
#define OMM_STAR_COLORS                                 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
#define OMM_LEVEL_ENTRY_WARP(level)                     0x0A
#define OMM_LEVEL_EXIT_DISTANCE                         500
#define OMM_LEVEL_SLIDE                                 LEVEL_PSS
#define OMM_LEVEL_RETURN_TO_CASTLE                      LEVEL_CASTLE, 1, 0x1F, 0
#define OMM_LEVEL_END                                   LEVEL_ENDING
#define OMM_CAMERA_LOOK_UP_WARP_STARS                   10
#define OMM_CAMERA_IS_BOWSER_FIGHT                      omm_camera_is_bowser_fight()
#define OMM_NUM_PLAYABLE_CHARACTERS                     2
#define OMM_NUM_SAVE_FILES                              1
#define OMM_PALETTE_EDITOR_MUSIC                        SEQ_MENU_TITLE_SCREEN
#define OMM_TEXT_FORMAT(id, str)                        str
#define STAR                                            "STAR"
#define Star                                            "Star"

// Sparkly stars
#define OMM_SPARKLY_REQUIREMENT                         120
#define OMM_SPARKLY_BLOCK_LEVEL                         LEVEL_GROUNDS
#define OMM_SPARKLY_BLOCK_AREA                          1
#define OMM_SPARKLY_BLOCK_COUNT                         3
#define OMM_SPARKLY_BLOCK_AVAILABLE                     { 1, 1, 1 }
#define OMM_SPARKLY_BLOCK_X                             0
#define OMM_SPARKLY_BLOCK_Y                             820
#define OMM_SPARKLY_BLOCK_Z                             480
#define OMM_SPARKLY_BLOCK_ANGLE                         0x4000

// Files
#define FILE_MACRO_PRESETS_H                            "macro_presets.h"
#define FILE_SPECIAL_PRESETS_H                          "special_presets.h"
#define FILE_BETTERCAM_H                                "extras/bettercamera.h"
#define FILE_OPTIONS_H                                  "extras/options_menu.h"
#define FILE_SOUNDS_H                                   "sounds.h"
#define FILE_CHEATS_H                                   "extras/cheats.h"
#define FILE_MARIO_CHEATS_H                             "extras/cheats.h"
#define FILE_TITLE_H                                    "menu/title_screen.h"
#define FILE_TXT_CONV_H                                 "types.h"
#define FILE_TEXT_LOADER_H                              "types.h"
#define FILE_R96_SYSTEM_H                               "types.h"

// NOT COOL, ALOXADO
#define RAYCAST_FIND_FLOOR                              RAYCAST_FLAG_FLOORS
#define RAYCAST_FIND_CEIL                               RAYCAST_FLAG_CEILS
#define RAYCAST_FIND_WALL                               RAYCAST_FLAG_WALLS
#define RAYCAST_FIND_WATER                              0
#define RAYCAST_FIND_SOLID                              RAYCAST_FLAGS_SURFACES
#define RAYCAST_FIND_ALL                                RAYCAST_FLAGS_CAMERA
#define GOOMBA_BP_TRIPLET_FLAG_MASK                     GOOMBA_BP_TRIPLET_RESPAWN_FLAG_MASK
#define MR_BLIZZARD_STYPE_NO_CAP                        MR_BLIZZARD_STYPE_GENERIC
#define UKIKI_CAGE                                      UKIKI_BP_CAGE
#define PURPLE_SWITCH_IDLE                              PURPLE_SWITCH_ACT_IDLE
#define PURPLE_SWITCH_PRESSED                           PURPLE_SWITCH_ACT_PRESSED

// Behaviors
#define bhv1upJumpOnApproach                            bhv1UpJumpOnApproach
#define bhv1upRunningAway                               bhv1UpRunningAway
#define bhv1upSliding                                   bhv1UpSliding
#define bhv1upWalking                                   bhv1UpWalking
#define bhvBbhTiltingTrapPlatform                       bhvBBHTiltingTrapPlatform
#define bhvBbhTumblingBridge                            bhvBBHTumblingBridge
#define bhvBitfsSinkingCagePlatform                     bhvBitFSSinkingCagePlatform
#define bhvBitfsSinkingPlatforms                        bhvBitFSSinkingPlatforms
#define bhvBitfsTiltingInvertedPyramid                  bhvBitFSTiltingInvertedPyramid
#define bhvBobBowlingBallSpawner                        bhvBoBBowlingBallSpawner
#define bhvCcmTouchedStarSpawn                          bhvCCMTouchedStarSpawn
#define bhvDddMovingPole                                bhvDDDMovingPole
#define bhvDddWarp                                      bhvDDDWarp
#define bhvHidden1up                                    bhvHidden1Up
#define bhvHidden1upInPole                              bhvHidden1UpInPole
#define bhvHidden1upInPoleSpawner                       bhvHidden1UpInPoleSpawner
#define bhvHidden1upInPoleTrigger                       bhvHidden1UpInPoleTrigger
#define bhvHidden1upTrigger                             bhvHidden1UpTrigger
#define bhvHmcElevatorPlatform                          bhvHMCElevatorPlatform
#define bhvJrbFloatingBox                               bhvJRBFloatingBox
#define bhvJrbFloatingPlatform                          bhvJRBFloatingPlatform
#define bhvJrbSlidingBox                                bhvJRBSlidingBox
#define bhvLargeFishGroup                               bhvFishSpawner
#define bhvLllBowserPuzzle                              bhvLLLBowserPuzzle
#define bhvLllBowserPuzzlePiece                         bhvLLLBowserPuzzlePiece
#define bhvLllDrawbridge                                bhvLLLDrawbridge
#define bhvLllDrawbridgeSpawner                         bhvLLLDrawbridgeSpawner
#define bhvLllFloatingWoodBridge                        bhvLLLFloatingWoodBridge
#define bhvLllHexagonalMesh                             bhvLLLHexagonalMesh
#define bhvLllMovingOctagonalMeshPlatform               bhvLLLMovingOctagonalMeshPlatform
#define bhvLllRollingLog                                bhvLLLRollingLog
#define bhvLllRotatingBlockWithFireBars                 bhvLLLRotatingBlockWithFireBars
#define bhvLllRotatingHexFlame                          bhvLLLRotatingHexFlame
#define bhvLllRotatingHexagonalPlatform                 bhvLLLRotatingHexagonalPlatform
#define bhvLllRotatingHexagonalRing                     bhvLLLRotatingHexagonalRing
#define bhvLllSinkingRectangularPlatform                bhvLLLSinkingRectangularPlatform
#define bhvLllSinkingRockBlock                          bhvLLLSinkingRockBlock
#define bhvLllSinkingSquarePlatforms                    bhvLLLSinkingSquarePlatforms
#define bhvLllTiltingInvertedPyramid                    bhvLLLTiltingInvertedPyramid
#define bhvLllTumblingBridge                            bhvLLLTumblingBridge
#define bhvLllVolcanoFallingTrap                        bhvLLLVolcanoFallingTrap
#define bhvLllWoodPiece                                 bhvLLLWoodPiece
#define bhvMrIBlueCoin                                  bhvSpawnedBlueCoin
#define bhvRrCruiserWing                                bhvRRCruiserWing
#define bhvRrElevatorPlatform                           bhvRRElevatorPlatform
#define bhvRrRotatingBridgePlatform                     bhvRRRotatingBridgePlatform
#define bhvSslMovingPyramidWall                         bhvSSLMovingPyramidWall
#define bhvThiBowlingBallSpawner                        bhvTHIBowlingBallSpawner
#define bhvThiHugeIslandTop                             bhvTHIHugeIslandTop
#define bhvThiTinyIslandTop                             bhvTHITinyIslandTop
#define bhvTreasureChests                               bhvTreasureChestsDDD
#define bhvTreasureChestsJrb                            bhvTreasureChestsJRB
#define bhvTtmBowlingBallSpawner                        bhvTTMBowlingBallSpawner
#define bhvTtmRollingLog                                bhvTTMRollingLog
#define bhvWdwExpressElevator                           bhvWDWExpressElevator
#define bhvWdwExpressElevatorPlatform                   bhvWDWExpressElevatorPlatform
#define bhvWdwRectangularFloatingPlatform               bhvWDWRectangularFloatingPlatform
#define bhvWdwSquareFloatingPlatform                    bhvWDWSquareFloatingPlatform
#define bhvWfBreakableWallLeft                          bhvWFBreakableWallLeft
#define bhvWfBreakableWallRight                         bhvWFBreakableWallRight
#define bhvWfElevatorTowerPlatform                      bhvWFElevatorTowerPlatform
#define bhvWfRotatingWoodenPlatform                     bhvWFRotatingWoodenPlatform
#define bhvWfSlidingPlatform                            bhvWFSlidingPlatform
#define bhvWfSlidingTowerPlatform                       bhvWFSlidingTowerPlatform
#define bhvWfSolidTowerPlatform                         bhvWFSolidTowerPlatform
#define bhvWfTumblingBridge                             bhvTumblingBridge

// Macro objects
#define macro_box_star_1                                macro_box_star_act_1
#define macro_box_star_2                                macro_box_star_act_2
#define macro_box_star_3                                macro_box_star_act_3
#define macro_box_star_4                                macro_box_star_act_4
#define macro_box_star_5                                macro_box_star_act_5
#define macro_box_star_6                                macro_box_star_act_6

#endif

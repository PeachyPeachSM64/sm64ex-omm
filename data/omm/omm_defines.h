#ifndef OMM_DEFINES_H
#define OMM_DEFINES_H

//
// Utilities
//

#define OMM_AT_STARTUP                                  __attribute__((constructor))
#define OMM_AT_EXIT                                     __attribute__((destructor))
#define OMM_LIKELY(expr)                                (__builtin_expect(!!(expr), 1))
#define OMM_UNLIKELY(expr)                              (__builtin_expect(!!(expr), 0))
#define OMM_INLINE                                      static inline
#define OMM_RETURN_IF_TRUE(cond, ret, ...)              { if (cond) { __VA_ARGS__; return ret; } }
#define OMM_DO_ONCE                                     static volatile bool _done_ = false; if (!_done_ && (_done_ = true))
#define OMM_LEVEL_NO_WARP(levelNum)                     (omm_level_get_course(levelNum) == COURSE_NONE)
#define OMM_LEVEL_IS_CASTLE_LEVEL(levelNum)             ((levelNum) == LEVEL_CASTLE_GROUNDS || (levelNum) == LEVEL_CASTLE || (levelNum) == LEVEL_CASTLE_COURTYARD)
#if OMM_LEVEL_HAS_BOWSER
#define OMM_LEVEL_IS_BOWSER_LEVEL(levelNum)             ((levelNum) == LEVEL_BITDW || (levelNum) == LEVEL_BITFS || (levelNum) == LEVEL_BITS)
#define OMM_LEVEL_IS_BOWSER_FIGHT(levelNum)             ((levelNum) == LEVEL_BOWSER_1 || (levelNum) == LEVEL_BOWSER_2 || (levelNum) == LEVEL_BOWSER_3)
#define OMM_BOWSER_IN_THE_LEVEL(levelNum)               ((levelNum) == LEVEL_BOWSER_1 ? LEVEL_BITDW : ((levelNum) == LEVEL_BOWSER_2 ? LEVEL_BITFS : ((levelNum) == LEVEL_BOWSER_3 ? LEVEL_BITS : (levelNum))))
#else
#define OMM_LEVEL_IS_BOWSER_LEVEL(levelNum)             (false)
#define OMM_LEVEL_IS_BOWSER_FIGHT(levelNum)             (false)
#define OMM_BOWSER_IN_THE_LEVEL(levelNum)               (levelNum)
#endif

//
// Physics
//

#define OMM_COLLISION_CHECK_PUS                         (OMM_MOVESET_ODYSSEY)
#define OMM_COLLISION_CHECK_NEIGHBOR_CELLS              (OMM_MOVESET_ODYSSEY)
#define OMM_COLLISION_FIX_WALL_COLLISIONS               0 // TODO: Disable that for now
#define OMM_COLLISION_FIX_UNREFERENCED_WALLS            (OMM_MOVESET_ODYSSEY)
#define OMM_COLLISION_FIX_OUT_OF_BOUNDS_SLANTED_WALLS   (OMM_MOVESET_ODYSSEY)
#define OMM_STEP_NUM_SUB_STEPS                          (OMM_MOVESET_ODYSSEY ? 16 : 4)
#define OMM_STEP_NUM_SUB_STEPS_WATER                    (OMM_MOVESET_ODYSSEY ? 16 : 1)
#define OMM_STEP_NUM_SUB_STEPS_CAPTURE                  (OMM_MOVESET_ODYSSEY ? 8 : 4)
#define OMM_STEP_NUM_SUB_STEPS_OBJECT                   (OMM_MOVESET_ODYSSEY ? 4 : 1)
#define OMM_STEP_FIX_EXPOSED_CEILINGS                   (OMM_MOVESET_ODYSSEY)
#define OMM_STEP_FIX_OUT_OF_BOUNDS_BONK                 (OMM_MOVESET_ODYSSEY)
#define OMM_STEP_FIX_CEILINGS_BONK                      (OMM_MOVESET_ODYSSEY)
#define OMM_STEP_FIX_LEDGE_GRABS                        (OMM_MOVESET_ODYSSEY)
#define OMM_STEP_FIX_MOVING_PLATFORM_JUMPS              (OMM_MOVESET_ODYSSEY)
#define OMM_STEP_FIX_WATER_WARPS                        (OMM_MOVESET_ODYSSEY)
#define OMM_STEP_FIX_WHIRLPOOLS                         (OMM_MOVESET_ODYSSEY)
#define OMM_OBJECT_LOST_COIN                            (0x1057)

#if OMM_GAME_IS_SM74
// --------------------------------------------------------------------------
// Hacky fix to the underwater tunnel bug in Breezing Beach
// --------------------------------------------------------------------------
// Mario/Peach gets stuck in the tunnel if the Odyssey Moveset is enabled.
// The following code reduces the number of sub-steps to the Vanilla amount
// and disable the check to neighbor cells in Breezing Beach.
// --------------------------------------------------------------------------
#undef OMM_COLLISION_CHECK_NEIGHBOR_CELLS
#define OMM_COLLISION_CHECK_NEIGHBOR_CELLS              (OMM_MOVESET_ODYSSEY && !(gCurrLevelNum == LEVEL_SL && OMM_GAME_MODE == OMM_SM74_MODE_EXTREME))
#undef OMM_STEP_NUM_SUB_STEPS_WATER
#define OMM_STEP_NUM_SUB_STEPS_WATER                    ((OMM_MOVESET_ODYSSEY && !(gCurrLevelNum == LEVEL_SL && OMM_GAME_MODE == OMM_SM74_MODE_EXTREME)) ? 16 : 1)
#endif

//
// Options
//

#define OMM_MOVESET_CLASSIC                             (gOmmMovesetType == 0)
#define OMM_MOVESET_ODYSSEY                             (gOmmMovesetType != 0)
#define OMM_MOVESET_ODYSSEY_3H                          (gOmmMovesetType == 1)
#define OMM_MOVESET_ODYSSEY_6H                          (gOmmMovesetType == 2)
#define OMM_MOVESET_ODYSSEY_1H                          (gOmmMovesetType == 3)
#define OMM_CAP_CLASSIC                                 (gOmmCapType == 0)
#define OMM_CAP_CAPPY_NO_CAPTURE                        (gOmmCapType == 1)
#define OMM_CAP_CAPPY_CAPTURE_PRESS                     (gOmmCapType == 2)
#define OMM_CAP_CAPPY_CAPTURE_HOLD                      (gOmmCapType == 3)
#define OMM_CAP_CAPPY_CAPTURE                           (OMM_CAP_CAPPY_CAPTURE_PRESS || OMM_CAP_CAPPY_CAPTURE_HOLD)
#define OMM_STARS_CLASSIC                               (gOmmStarsMode == 0)
#define OMM_STARS_NON_STOP                              (gOmmStarsMode == 1)
#define OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE          (OMM_STARS_NON_STOP && !omm_is_ending_cutscene())
#define OMM_POWER_UPS_CLASSIC                           (gOmmPowerUpsType == 0)
#define OMM_POWER_UPS_IMPROVED                          (gOmmPowerUpsType == 1)
#define OMM_CAMERA_CLASSIC                              (gOmmCameraMode == 0)
#define OMM_CAMERA_8_DIRECTIONS                         (gOmmCameraMode == 1)
#define OMM_CAMERA_16_DIRECTIONS                        (gOmmCameraMode == 2)
#define OMM_CAMERA_INF_DIRECTIONS                       (gOmmCameraMode == 3)
#define OMM_SPARKLY_STARS_MODE                          (gOmmSparklyStarsMode)
#define OMM_SPARKLY_STARS_HINT_ALWAYS                   (gOmmSparklyStarsHintAtLevelEntry == 0)
#define OMM_SPARKLY_STARS_HINT_NOT_COLLECTED            (gOmmSparklyStarsHintAtLevelEntry == 1)
#define OMM_SPARKLY_STARS_HINT_NEVER                    (gOmmSparklyStarsHintAtLevelEntry == 2)
#define OMM_SPARKLY_STARS_COMPLETION_REWARD             (gOmmSparklyStarsCompletionReward)
#define OMM_SPARKLY_STARS_PERRY_CHARGE                  (gOmmSparklyStarsPerryCharge)
#define OMM_EXTRAS_SMO_ANIMATIONS                       (gOmmExtrasSMOAnimations && OMM_MOVESET_ODYSSEY)
#define OMM_EXTRAS_CAPPY_AND_TIARA                      (gOmmExtrasCappyAndTiara && !OMM_CAP_CLASSIC && !OMM_EXTRAS_INVISIBLE_MODE)
#define OMM_EXTRAS_COLORED_STARS                        (gOmmExtrasColoredStars && !OMM_GAME_IS_SMMS)
#define OMM_EXTRAS_REVEAL_SECRETS                       (gOmmExtrasRevealSecrets && !OMM_GAME_IS_SMGS)
#define OMM_EXTRAS_OBJECTS_RADAR_ENABLED                (gOmmExtrasObjectsRadar != 0)
#define OMM_EXTRAS_SHOW_STAR_NUMBER                     (gOmmExtrasShowStarNumber && !omm_is_ending_cutscene())
#define OMM_EXTRAS_INVISIBLE_MODE                       (gOmmExtrasInvisibleMode)
#define OMM_EXTRAS_MARIO_MODE                           (gOmmExtrasMarioMode)
#define OMM_HUD_ALWAYS                                  (gOmmHudMode == OMM_HUD_MODE_ALWAYS)
#define OMM_HUD_VANISHING                               (gOmmHudMode == OMM_HUD_MODE_VANISHING)
#define OMM_HUD_PRO                                     (gOmmHudMode == OMM_HUD_MODE_PRO)
#define OMM_HUD_NONE                                    (gOmmHudMode == OMM_HUD_MODE_NONE)
#define OMM_HUD_SHOW_NON_ESSENTIAL                      (!OMM_HUD_NONE && (!OMM_HUD_PRO || omm_is_game_paused()))
#define OMM_CHEAT_UNLIMITED_CAPPY_BOUNCES               (gOmmCheatUnlimitedCappyBounces == 1)
#define OMM_CHEAT_CAPPY_STAYS_FOREVER                   (gOmmCheatCappyStaysForever == 1)
#define OMM_CHEAT_HOMING_ATTACK_GLOBAL_RANGE            (gOmmCheatHomingAttackGlobalRange == 1)
#define OMM_CHEAT_MARIO_TELEPORTS_TO_CAPPY              (gOmmCheatMarioTeleportsToCappy == 1)
#define OMM_CHEAT_CAPPY_CAN_COLLECT_STARS               (gOmmCheatCappyCanCollectStars == 1)
#define OMM_CHEAT_PLAY_AS_CAPPY                         (gOmmCheatPlayAsCappy == 1)
#define OMM_CHEAT_PEACH_ENDLESS_VIBE_GAUGE              (gOmmCheatPeachEndlessVibeGauge == 1)
#define OMM_CHEAT_SHADOW_MARIO                          (gOmmCheatShadowMario == 1 && OMM_PLAYER_MODEL_IS_MARIO)
#if OMM_GAME_IS_R96X
#define OMM_CHEAT_MOON_JUMP                             (Cheats.EnableCheats && (Cheats.MoonJump && !Cheats.ChaosMode))
#define OMM_CHEAT_GOD_MODE                              (Cheats.EnableCheats && (Cheats.GodMode && !Cheats.ChaosMode))
#define OMM_CHEAT_INVINCIBLE                            (false) // Not implemented in Render96
#define OMM_CHEAT_SUPER_SPEED                           (false) // Handled by Render96
#define OMM_CHEAT_SUPER_RESPONSIVE                      (Cheats.EnableCheats && ((Cheats.Responsive && !Cheats.ChaosMode) || ((Cheats.ChaosControls >> 0) & 1)))
#define OMM_CHEAT_NO_FALL_DAMAGE                        (Cheats.EnableCheats && (Cheats.GodMode && !Cheats.ChaosMode))
#define OMM_CHEAT_CAP_MODIFIER                          (Cheats.EnableCheats && (Cheats.CapModifier && !Cheats.ChaosMode))
#define OMM_CHEAT_WALK_ON_LAVA                          (Cheats.EnableCheats && (Cheats.WalkOnHazards && !Cheats.ChaosMode))
#define OMM_CHEAT_WALK_ON_QUICKSAND                     (Cheats.EnableCheats && (Cheats.WalkOnHazards && !Cheats.ChaosMode))
#define OMM_CHEAT_WALK_ON_WATER                         (false) // Not implemented in Render96
#define OMM_CHEAT_WALK_ON_GAS                           (false) // Not implemented in Render96
#define OMM_CHEAT_WALK_ON_SLOPE                         (false) // Not implemented in Render96
#define OMM_CHEAT_WALK_ON_DEATH_BARRIER                 (Cheats.EnableCheats && (Cheats.NoDeathBarrier && !Cheats.ChaosMode))
#define OMM_CHEAT_BLJ_ANYWHERE                          (Cheats.EnableCheats && (Cheats.BLJAnywhere && !Cheats.ChaosMode))
#else
#define OMM_CHEAT_MOON_JUMP                             (gOmmCheatEnable && gOmmCheatMoonJump)
#define OMM_CHEAT_GOD_MODE                              (gOmmCheatEnable && gOmmCheatGodMode)
#define OMM_CHEAT_INVINCIBLE                            (gOmmCheatEnable && gOmmCheatInvincible)
#define OMM_CHEAT_SUPER_SPEED                           (gOmmCheatEnable && gOmmCheatSuperSpeed)
#define OMM_CHEAT_SUPER_RESPONSIVE                      (gOmmCheatEnable && gOmmCheatSuperResponsive)
#define OMM_CHEAT_NO_FALL_DAMAGE                        (gOmmCheatEnable && gOmmCheatNoFallDamage)
#define OMM_CHEAT_CAP_MODIFIER                          (gOmmCheatEnable && gOmmCheatCapModifier)
#define OMM_CHEAT_WALK_ON_LAVA                          (gOmmCheatEnable && gOmmCheatWalkOnLava)
#define OMM_CHEAT_WALK_ON_QUICKSAND                     (gOmmCheatEnable && gOmmCheatWalkOnQuicksand)
#define OMM_CHEAT_WALK_ON_WATER                         (gOmmCheatEnable && gOmmCheatWalkOnWater)
#define OMM_CHEAT_WALK_ON_GAS                           (gOmmCheatEnable && gOmmCheatWalkOnGas)
#define OMM_CHEAT_WALK_ON_SLOPE                         (gOmmCheatEnable && gOmmCheatWalkOnSlope)
#define OMM_CHEAT_WALK_ON_DEATH_BARRIER                 (gOmmCheatEnable && gOmmCheatWalkOnDeathBarrier)
#define OMM_CHEAT_BLJ_ANYWHERE                          (gOmmCheatEnable && gOmmCheatBljAnywhere)
#endif
#define OMM_CHEAT_BLJ_ANYWHERE_REQ                      (OMM_CHEAT_BLJ_ANYWHERE && (m->forwardVel < -10.f) && (m->controller->buttonDown & Z_TRIG))
#define OMM_CHEATS_ENABLED                              (OMM_CHEAT_MOON_JUMP || \
                                                         OMM_CHEAT_GOD_MODE || \
                                                         OMM_CHEAT_INVINCIBLE || \
                                                         OMM_CHEAT_SUPER_SPEED || \
                                                         OMM_CHEAT_SUPER_RESPONSIVE || \
                                                         OMM_CHEAT_NO_FALL_DAMAGE || \
                                                         OMM_CHEAT_CAP_MODIFIER || \
                                                         OMM_CHEAT_WALK_ON_LAVA || \
                                                         OMM_CHEAT_WALK_ON_QUICKSAND || \
                                                         OMM_CHEAT_WALK_ON_WATER || \
                                                         OMM_CHEAT_WALK_ON_GAS || \
                                                         OMM_CHEAT_WALK_ON_SLOPE || \
                                                         OMM_CHEAT_WALK_ON_DEATH_BARRIER || \
                                                         OMM_CHEAT_BLJ_ANYWHERE || \
                                                         OMM_CHEAT_UNLIMITED_CAPPY_BOUNCES || \
                                                         OMM_CHEAT_CAPPY_STAYS_FOREVER || \
                                                         OMM_CHEAT_HOMING_ATTACK_GLOBAL_RANGE || \
                                                         OMM_CHEAT_MARIO_TELEPORTS_TO_CAPPY || \
                                                         OMM_CHEAT_CAPPY_CAN_COLLECT_STARS || \
                                                         OMM_CHEAT_PLAY_AS_CAPPY || \
                                                         OMM_CHEAT_PEACH_ENDLESS_VIBE_GAUGE || \
                                                         OMM_CHEAT_SHADOW_MARIO)
#if OMM_GAME_IS_R96X
#define OMM_CHEATS_DISABLE                              {gOmmCheatUnlimitedCappyBounces = false; \
                                                         gOmmCheatCappyStaysForever = false; \
                                                         gOmmCheatHomingAttackGlobalRange = false; \
                                                         gOmmCheatMarioTeleportsToCappy = false; \
                                                         gOmmCheatCappyCanCollectStars = false; \
                                                         gOmmCheatPlayAsCappy = false; \
                                                         gOmmCheatPeachEndlessVibeGauge = false; \
                                                         gOmmCheatShadowMario = false;}
#else
#define OMM_CHEATS_DISABLE                              {gOmmCheatEnable = false; \
                                                         gOmmCheatMoonJump = false; \
                                                         gOmmCheatGodMode = false; \
                                                         gOmmCheatInvincible = false; \
                                                         gOmmCheatSuperSpeed = false; \
                                                         gOmmCheatSuperResponsive = false; \
                                                         gOmmCheatNoFallDamage = false; \
                                                         gOmmCheatCapModifier = false; \
                                                         gOmmCheatWalkOnLava = false; \
                                                         gOmmCheatWalkOnQuicksand = false; \
                                                         gOmmCheatWalkOnWater = false; \
                                                         gOmmCheatWalkOnGas = false; \
                                                         gOmmCheatWalkOnSlope = false; \
                                                         gOmmCheatWalkOnDeathBarrier = false; \
                                                         gOmmCheatBljAnywhere = false; \
                                                         gOmmCheatUnlimitedCappyBounces = false; \
                                                         gOmmCheatCappyStaysForever = false; \
                                                         gOmmCheatHomingAttackGlobalRange = false; \
                                                         gOmmCheatMarioTeleportsToCappy = false; \
                                                         gOmmCheatCappyCanCollectStars = false; \
                                                         gOmmCheatPlayAsCappy = false; \
                                                         gOmmCheatPeachEndlessVibeGauge = false; \
                                                         gOmmCheatShadowMario = false;}
#endif

//
// Dialogs
//

#define OMM_DIALOG_START_INDEX                          (0x100)

// Signs
#define OMM_DIALOG_CREDITS                              (OMM_DIALOG_START_INDEX + 0x000)
#define OMM_DIALOG_PEACHY_ROOM                          (OMM_DIALOG_START_INDEX + 0x001)
#define OMM_DIALOG_PEACHY_ROOM_FINAL                    (OMM_DIALOG_START_INDEX + 0x002)
#define OMM_DIALOG_LEVEL_VARIANTS                       (OMM_DIALOG_START_INDEX + 0x003)
#if OMM_GAME_IS_SM74
#define OMM_DIALOG_SM74_ROOM_1                          (OMM_DIALOG_START_INDEX + 0x004)
#define OMM_DIALOG_SM74_ROOM_2                          (OMM_DIALOG_START_INDEX + 0x005)
#endif

// NPCs
#define OMM_DIALOG_YOSHI_CAPTURE                        (OMM_DIALOG_START_INDEX + 0x020)
#define OMM_DIALOG_TOAD_SECRET                          (OMM_DIALOG_START_INDEX + 0x021)
#define OMM_DIALOG_TOAD_YOSHI                           (OMM_DIALOG_START_INDEX + 0x022)
#define OMM_DIALOG_SPARKLY_MIPS_1                       (OMM_DIALOG_START_INDEX + 0x023)
#define OMM_DIALOG_SPARKLY_MIPS_2                       (OMM_DIALOG_START_INDEX + 0x024)
#define OMM_DIALOG_SPARKLY_MIPS_3                       (OMM_DIALOG_START_INDEX + 0x025)
#define OMM_DIALOG_SPARKLY_TOAD_1                       (OMM_DIALOG_START_INDEX + 0x026)
#define OMM_DIALOG_SPARKLY_TOAD_2                       (OMM_DIALOG_START_INDEX + 0x027)
#define OMM_DIALOG_SPARKLY_TOAD_3                       (OMM_DIALOG_START_INDEX + 0x028)

// Bowser
#define OMM_DIALOG_BOWSER_1_INTRO                       (OMM_DIALOG_START_INDEX + 0x080)
#define OMM_DIALOG_BOWSER_1_HINT_1                      (OMM_DIALOG_START_INDEX + 0x081)
#define OMM_DIALOG_BOWSER_1_HINT_2                      (OMM_DIALOG_START_INDEX + 0x082)
#define OMM_DIALOG_BOWSER_1_HINT_3                      (OMM_DIALOG_START_INDEX + 0x083)
#define OMM_DIALOG_BOWSER_2_INTRO                       (OMM_DIALOG_START_INDEX + 0x084)
#define OMM_DIALOG_BOWSER_3_INTRO                       (OMM_DIALOG_START_INDEX + 0x085)
#define OMM_DIALOG_BOWSER_3_DEFEAT                      (OMM_DIALOG_START_INDEX + 0x086)

// Cheats
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_INTRO             (OMM_DIALOG_START_INDEX + 0x0F0)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_0                 (OMM_DIALOG_START_INDEX + 0x0F1)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_1                 (OMM_DIALOG_START_INDEX + 0x0F2)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_2                 (OMM_DIALOG_START_INDEX + 0x0F3)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_3                 (OMM_DIALOG_START_INDEX + 0x0F4)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_4                 (OMM_DIALOG_START_INDEX + 0x0F5)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_5                 (OMM_DIALOG_START_INDEX + 0x0F6)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_6                 (OMM_DIALOG_START_INDEX + 0x0F7)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_7                 (OMM_DIALOG_START_INDEX + 0x0F8)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_FISH              (OMM_DIALOG_START_INDEX + 0x0F9)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_0             (OMM_DIALOG_START_INDEX + 0x0FA)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_1             (OMM_DIALOG_START_INDEX + 0x0FB)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_2             (OMM_DIALOG_START_INDEX + 0x0FC)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_FISH          (OMM_DIALOG_START_INDEX + 0x0FD)

// Sparkly Stars
#define OMM_DIALOG_SPARKLY_STAR(mode, index)            (OMM_DIALOG_START_INDEX + 0x100 + 0x100 * ((mode) - 1) + (index))
#define OMM_DIALOG_SPARKLY_STAR_COMPLETED(mode, index)  (OMM_DIALOG_START_INDEX + 0x180 + 0x100 * ((mode) - 1) + (index))
#define OMM_DIALOG_SPARKLY_STAR_NAMES(mode)             (OMM_DIALOG_START_INDEX + 0x1FC + 0x100 * ((mode) - 1))
#define OMM_DIALOG_SPARKLY_BOWSER_4_UNLOCKED(mode)      (OMM_DIALOG_START_INDEX + 0x1FD + 0x100 * ((mode) - 1))
#define OMM_DIALOG_SPARKLY_BOWSER_4_INTRO(mode)         (OMM_DIALOG_START_INDEX + 0x1FE + 0x100 * ((mode) - 1))
#define OMM_DIALOG_SPARKLY_BOWSER_4_DEFEAT(mode)        (OMM_DIALOG_START_INDEX + 0x1FF + 0x100 * ((mode) - 1))

//
// Texts
//

#define OMM_TEXT_(id, str) extern const char OMM_TEXT_##id[];
#include "data/omm/omm_defines_texts.inl"
#undef OMM_TEXT_

//
// Textures
//

#define OMM_TEXTURE_WHITEN ".__WHITEN__"
#define OMM_TEXTURE_(id, str) extern const char OMM_TEXTURE_##id[];
#include "data/omm/omm_defines_textures.inl"
#undef OMM_TEXTURE_
extern const char *OMM_TEXTURE_STAR_BODY_[20];
extern const char *OMM_TEXTURE_STAR_FULL_[20];
extern int OMM_STAR_COLOR_[OMM_STAR_COLOR_COUNT];

//
// Assets
//

#define OMM_ASSET_(id, str) extern const char OMM_ASSET_##id[];
#include "data/omm/omm_defines_assets.inl"
#undef OMM_ASSET_

//
// Palettes
//

#define OMM_PALETTE_(id, str) extern const char OMM_PALETTE_##id[];
#define OMM_PALETTE_LEVEL_(id, str) extern const char OMM_PALETTE_##id[];
#include "data/omm/omm_defines_palettes.inl"
#undef OMM_PALETTE_
#undef OMM_PALETTE_LEVEL_

//
// Sounds
//

#define OMM_SOUND_PEACH_JUMP_YAH                        (0x00)
#define OMM_SOUND_PEACH_JUMP_WAH                        (0x01)
#define OMM_SOUND_PEACH_JUMP_HOO                        (0x02)
#define OMM_SOUND_PEACH_PUNCH_YAH                       (0x03)
#define OMM_SOUND_PEACH_PUNCH_WAH                       (0x04)
#define OMM_SOUND_PEACH_PUNCH_HOO                       (0x05)
#define OMM_SOUND_PEACH_DIVE_HOOHOO                     (0x06)
#define OMM_SOUND_PEACH_JUMP_YAHOO                      (0x07)
#define OMM_SOUND_PEACH_JUMP_YAHOO_2                    (0x08)
#define OMM_SOUND_PEACH_JUMP_WAHA                       (0x09)
#define OMM_SOUND_PEACH_JUMP_YIPPEE                     (0x0A)
#define OMM_SOUND_PEACH_LAND_HAHA                       (0x0B)
#define OMM_SOUND_PEACH_LAND_HAHA_2                     (0x0C)
#define OMM_SOUND_PEACH_LEDGE_MISS_UH                   (0x0D)
#define OMM_SOUND_PEACH_LEDGE_CLIMB_UH2                 (0x0E)
#define OMM_SOUND_PEACH_LEDGE_CLIMB_UH2_2               (0x0F)
#define OMM_SOUND_PEACH_GRAB_HRMM                       (0x10)
#define OMM_SOUND_PEACH_THROW_WAH2                      (0x11)
#define OMM_SOUND_PEACH_GROUND_POUND_WAH                (0x12)
#define OMM_SOUND_PEACH_LEDGE_GRAB_WHOA                 (0x13)
#define OMM_SOUND_PEACH_LEDGE_CLIMB_EEUH                (0x14)
#define OMM_SOUND_PEACH_ATTACKED                        (0x15)
#define OMM_SOUND_PEACH_BONK_OOOF                       (0x16)
#define OMM_SOUND_PEACH_BONK_OOOF_2                     (0x17)
#define OMM_SOUND_PEACH_BONK_DOH                        (0x18)
#define OMM_SOUND_PEACH_HERE_WE_GO                      (0x19)
#define OMM_SOUND_PEACH_YAWNING                         (0x1A)
#define OMM_SOUND_PEACH_PANTING1                        (0x1B)
#define OMM_SOUND_PEACH_PANTING2                        (0x1C)
#define OMM_SOUND_PEACH_PANTING3                        (0x1D)
#define OMM_SOUND_PEACH_COUGHING1                       (0x1E)
#define OMM_SOUND_PEACH_COUGHING2                       (0x1F)
#define OMM_SOUND_PEACH_COUGHING3                       (0x20)
#define OMM_SOUND_PEACH_FALLING_WAAAOOOW                (0x21)
#define OMM_SOUND_PEACH_ON_FIRE                         (0x22)
#define OMM_SOUND_PEACH_DYING                           (0x23)
#define OMM_SOUND_PEACH_DROWNING                        (0x24)
#define OMM_SOUND_PEACH_MAMA_MIA                        (0x25)
#define OMM_SOUND_PEACH_TWIRL_BOUNCE                    (0x26)
#define OMM_SOUND_PEACH_SO_LONGA_BOWSER                 (0x27)
#define OMM_SOUND_PEACH_LETS_A_GO                       (0x28)
#define OMM_SOUND_PEACH_VIBE_JOY                        (0x29)
#define OMM_SOUND_PEACH_VIBE_RAGE                       (0x2A)
#define OMM_SOUND_PEACH_VIBE_GLOOM                      (0x2B)
#define OMM_SOUND_PEACH_VIBE_CALM                       (0x2C)

#define OMM_SOUND_TOAD_1                                (0x40)
#define OMM_SOUND_TOAD_2                                (0x41)
#define OMM_SOUND_TOAD_3                                (0x42)
#define OMM_SOUND_TOAD_4                                (0x43)
#define OMM_SOUND_TOAD_5                                (0x44)
#define OMM_SOUND_TOAD_6                                (0x45)
#define OMM_SOUND_TOAD_7                                (0x46)
#define OMM_SOUND_TOAD_8                                (0x47)
#define OMM_SOUND_TOAD_9                                (0x48)

#define OMM_SOUND_EVENT_CAPTURE                         (0x50)
#define OMM_SOUND_EVENT_UNCAPTURE                       (0x51)
#define OMM_SOUND_EVENT_LIFE_UP                         (0x52)
#define OMM_SOUND_EVENT_SPARKLY_STAR_GET                (0x53)
#define OMM_SOUND_EVENT_SECRET                          (0x54)
#define OMM_SOUND_EVENT_DEATH                           (0x55)
#define OMM_SOUND_EVENT_DEATH_WATER                     (0x56)
#define OMM_SOUND_EVENT_DEATH_FALL                      (0x57)
#define OMM_SOUND_EVENT_DEATH_MARIO                     (0x58)
#define OMM_SOUND_EVENT_DEATH_MARIO_WATER               (0x59)
#define OMM_SOUND_EVENT_DEATH_MARIO_FALL                (0x5A)
#define OMM_SOUND_EVENT_DEATH_PEACH                     (0x5B)
#define OMM_SOUND_EVENT_DEATH_PEACH_WATER               (0x5C)
#define OMM_SOUND_EVENT_DEATH_PEACH_FALL                (0x5D)
#define OMM_SOUND_EVENT_DEATH_LUIGI                     (0x5E)
#define OMM_SOUND_EVENT_DEATH_LUIGI_WATER               (0x5F)
#define OMM_SOUND_EVENT_DEATH_LUIGI_FALL                (0x60)
#define OMM_SOUND_EVENT_DEATH_WARIO                     (0x61)
#define OMM_SOUND_EVENT_DEATH_WARIO_WATER               (0x62)
#define OMM_SOUND_EVENT_DEATH_WARIO_FALL                (0x63)

#define OMM_SOUND_EFFECT_DAMAGE                         (0x80)
#define OMM_SOUND_EFFECT_HEAL                           (0x81)
#define OMM_SOUND_EFFECT_FREEZE_1                       (0x82)
#define OMM_SOUND_EFFECT_FREEZE_2                       (0x83)
#define OMM_SOUND_EFFECT_PROPELLER_1                    (0x84)
#define OMM_SOUND_EFFECT_PROPELLER_2                    (0x85)
#define OMM_SOUND_EFFECT_PROPELLER_3                    (0x86)
#define OMM_SOUND_EFFECT_MIDAIR_SPIN                    (0x87)
#define OMM_SOUND_EFFECT_PEACH_GLIDE                    (0x88)
#define OMM_SOUND_EFFECT_PEACH_VIBE                     (0x89)
#define OMM_SOUND_EFFECT_PERRY_CHARGE                   (0x8A)
#define OMM_SOUND_EFFECT_PERRY_CHARGED                  (0x8B)
#define OMM_SOUND_EFFECT_PERRY_BEAM                     (0x8C)
#define OMM_SOUND_EFFECT_SONIC_JUMP_1                   (0x8D)
#define OMM_SOUND_EFFECT_SONIC_JUMP_2                   (0x8E)
#define OMM_SOUND_EFFECT_SONIC_CHARGE                   (0x8F)
#define OMM_SOUND_EFFECT_SONIC_RELEASE                  (0x90)
#define OMM_SOUND_EFFECT_SONIC_SPIN                     (0x91)
#define OMM_SOUND_EFFECT_YOSHI_TONGUE                   (0x92)
#define OMM_SOUND_EFFECT_YOSHI_FLUTTER                  (0x93)
#define OMM_SOUND_EFFECT_YOSHI_FLUTTER_END              (0x94)

//
// Playable characters
//

#define OMM_PLAYER_MARIO                                (0)
#define OMM_PLAYER_PEACH                                (1)
#define OMM_PLAYER_LUIGI                                (2)
#define OMM_PLAYER_WARIO                                (3)
#define OMM_PLAYER_YOSHI                                (4) // Used for text replacement and some sounds
#define OMM_PLAYER_IS_MARIO                             (omm_player_get_selected_index() == OMM_PLAYER_MARIO)
#define OMM_PLAYER_IS_PEACH                             (omm_player_get_selected_index() == OMM_PLAYER_PEACH)
#define OMM_PLAYER_IS_LUIGI                             (omm_player_get_selected_index() == OMM_PLAYER_LUIGI)
#define OMM_PLAYER_IS_WARIO                             (omm_player_get_selected_index() == OMM_PLAYER_WARIO)
#define OMM_PLAYER_MODEL_IS_MARIO                       (omm_player_get_selected_index_model_and_sounds() == OMM_PLAYER_MARIO)
#define OMM_PLAYER_MODEL_IS_PEACH                       (omm_player_get_selected_index_model_and_sounds() == OMM_PLAYER_PEACH)
#define OMM_PLAYER_MODEL_IS_LUIGI                       (omm_player_get_selected_index_model_and_sounds() == OMM_PLAYER_LUIGI)
#define OMM_PLAYER_MODEL_IS_WARIO                       (omm_player_get_selected_index_model_and_sounds() == OMM_PLAYER_WARIO)

//
// Routines
//

#define OMM_ROUTINE_TYPE_LEVEL_ENTRY                    (0)
#define OMM_ROUTINE_TYPE_UPDATE                         (1)
#define OMM_ROUTINE_TYPE_PRE_RENDER                     (2)
#define OMM_ROUTINE_TYPES                               (3)
#define OMM_ROUTINE_LEVEL_ENTRY(func)                   static void func(void); OMM_AT_STARTUP static void func##_init() { omm_add_routine(OMM_ROUTINE_TYPE_LEVEL_ENTRY, func); } static void func(void)
#define OMM_ROUTINE_UPDATE(func)                        static void func(void); OMM_AT_STARTUP static void func##_init() { omm_add_routine(OMM_ROUTINE_TYPE_UPDATE, func); } static void func(void)
#define OMM_ROUTINE_PRE_RENDER(func)                    static void func(void); OMM_AT_STARTUP static void func##_init() { omm_add_routine(OMM_ROUTINE_TYPE_PRE_RENDER, func); } static void func(void)

//
// Logs
//

#define omm_printf(fmt, ...)                            { printf(fmt __VA_ARGS__); fflush(stdout); }
#define omm_printf_warning(fmt, ...)                    { printf("\033[33m<WARNING> "); printf(fmt __VA_ARGS__); printf("\033[0m"); fflush(stdout); }
#define omm_printf_error(fmt, ...)                      { printf("\033[31m[ERROR] "); printf(fmt __VA_ARGS__); printf("\033[0m"); fflush(stdout); }
#define omm_log(fmt, ...)                               { printf(__FUNCTION__); printf(": "); printf(fmt __VA_ARGS__); fflush(stdout); }
#define omm_log_warning(fmt, ...)                       { printf("\033[33m"); printf(__FUNCTION__); printf(": <WARNING> "); printf(fmt __VA_ARGS__); printf("\033[0m"); fflush(stdout); }
#define omm_log_error(fmt, ...)                         { printf("\033[31m"); printf(__FUNCTION__); printf(": [ERROR] "); printf(fmt __VA_ARGS__); printf("\033[0m"); fflush(stdout); }
#if OMM_CODE_DEBUG
#define omm_debug_log(fmt, ...)                         { printf(__FUNCTION__); printf(": "); printf(fmt __VA_ARGS__); fflush(stdout); }
#define omm_debug_text(x, y, fmt, ...)                  { char _str_[256]; snprintf(_str_, 256, fmt, __VA_ARGS__); print_text(x, y, _str_); }
#define omm_debug_pos(x, y, z, model)                   { if (gMarioObject) { struct Object *dbg = spawn_object_abs_with_rot(gMarioObject, 0, model, bhvSparkle, x, y, z, 0, 0, 0); if (dbg) { obj_scale(dbg, 0.25f); dbg->oNodeFlags |= GRAPH_RENDER_BILLBOARD; } } }
#else
#define omm_debug_log(fmt, ...)
#define omm_debug_text(x, y, fmt, ...)
#define omm_debug_pos(x, y, z, model)
#endif

#endif // OMM_DEFINES_H

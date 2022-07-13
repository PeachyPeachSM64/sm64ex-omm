#ifndef OMM_DEFINES_H
#define OMM_DEFINES_H

//
// Utilities
//

#define OMM_EXE_FOLDER                                  omm_exe_path()
#define OMM_USER_FOLDER                                 omm_user_path()
#define OMM_RES_FOLDER                                  FS_BASEDIR
#define OMM_GFX_FOLDER                                  OMM_RES_FOLDER "/gfx"
#define OMM_SOUND_FOLDER                                OMM_RES_FOLDER "/sound"
#define OMM_CAPPY_FOLDER                                OMM_GFX_FOLDER "/cappy"
#define OMM_AT_STARTUP                                  __attribute__((constructor))
#define OMM_AT_EXIT                                     __attribute__((destructor))
#define OMM_LIKELY(expr)                                (__builtin_expect(!!(expr), 1))
#define OMM_UNLIKELY(expr)                              (__builtin_expect(!!(expr), 0))
#define OMM_INLINE                                      static inline
#define OMM_UNUSED(x)                                   (void) x
#define OMM_LEVEL_NO_WARP(level)                        (omm_level_get_course(level) == COURSE_NONE)
#define OMM_BOWSER_IN_THE_LEVEL(level)                  (level == LEVEL_BOWSER_1 ? LEVEL_BITDW : (level == LEVEL_BOWSER_2 ? LEVEL_BITFS : (level == LEVEL_BOWSER_3 ? LEVEL_BITS : level)))
#define OMM_STRING(str, size, fmt, ...)                 char str[size]; snprintf(str, size, fmt, __VA_ARGS__);
#define OMM_PRINT_TEXT(x, y, fmt, ...)                  { char _str_[256]; snprintf(_str_, 256, fmt, __VA_ARGS__); print_text(x, y, _str_); }
#define OMM_ARRAY_OF(type)                              (type *) (type[])
#define OMM_ARRAY_SIZE(a)                               (sizeof(a) / sizeof(a[0]))
#define OMM_GET_FLAG(x, f)                              (((x) & (f)) != 0)
#define OMM_SET_FLAG(x, f, v)                           { if (v) { x |= (f); } else { x &= ~(f); } }
#define OMM_RETURN_IF_TRUE(cond, ret, ...)              { if (cond) { __VA_ARGS__; return ret; } }
#define LEVEL_GROUNDS                                   LEVEL_CASTLE_GROUNDS
#define LEVEL_COURT                                     LEVEL_CASTLE_COURTYARD

//
// Memory wrappers
//

#define OMM_MEMNEW(typ, cnt)                            calloc(sizeof(typ), cnt)
#define OMM_MEMDEL(dst)                                 { if (dst) { free((void *) dst); dst = NULL; } }
#define OMM_MEMSET(dst, val, siz)                       memset(dst, val, siz)
#define OMM_MEMCPY(dst, src, siz)                       memcpy(dst, src, siz)
#define OMM_MEMMOV(dst, src, siz)                       memmove(dst, src, siz)
#define OMM_MEMDUP(src, siz)                            memcpy(calloc(1, siz), src, siz)
#define OMM_MEMCMP(pt1, pt2, siz)                       (memcmp(pt1, pt2, siz) == 0)
#define OMM_MEMSWP(pt1, pt2, siz)                       { unsigned char _temp_[siz]; memcpy(&_temp_, &(pt1), siz); memcpy(&(pt1), &(pt2), siz); memcpy(&(pt2), &_temp_, siz); }
#define OMM_MEMREL(dst, src, from, to, typ)             { dst = (typ) (((uintptr_t) (src)) + (((uintptr_t) (to)) - ((uintptr_t) (from)))); }

//
// Physics
//

#define OMM_COLLISION_CHECK_PUS                         OMM_MOVESET_ODYSSEY
#define OMM_COLLISION_CHECK_NEIGHBOR_CELLS              OMM_MOVESET_ODYSSEY
#define OMM_COLLISION_FIX_UNREFERENCED_WALLS            OMM_MOVESET_ODYSSEY
#define OMM_STEP_NUM_SUB_STEPS                          (OMM_MOVESET_ODYSSEY ? 16 : 4)
#define OMM_STEP_NUM_SUB_STEPS_WATER                    (OMM_MOVESET_ODYSSEY ? 16 : 1)
#define OMM_STEP_NUM_SUB_STEPS_CAPTURE                  (OMM_MOVESET_ODYSSEY ? 8 : 4)
#define OMM_STEP_NUM_SUB_STEPS_OBJECT                   (OMM_MOVESET_ODYSSEY ? 4 : 1)
#define OMM_STEP_FIX_EXPOSED_CEILINGS                   OMM_MOVESET_ODYSSEY
#define OMM_STEP_FIX_OUT_OF_BOUNDS_BONK                 OMM_MOVESET_ODYSSEY
#define OMM_STEP_FIX_CEILINGS_BONK                      OMM_MOVESET_ODYSSEY
#define OMM_STEP_FIX_LEDGE_GRABS                        OMM_MOVESET_ODYSSEY
#define OMM_STEP_FIX_MOVING_PLATFORM_JUMPS              OMM_MOVESET_ODYSSEY
#define OMM_STEP_FIX_WATER_WARPS                        OMM_MOVESET_ODYSSEY
#define OMM_STEP_FIX_WHIRLPOOLS                         OMM_MOVESET_ODYSSEY
#define OMM_OBJECT_LOST_COIN                            0x1057

#if OMM_GAME_IS_SM74
// --------------------------------------------------------------------------
// Hacky fix to the underwater tunnel bug in Breezing Beach
// --------------------------------------------------------------------------
// Mario/Peach gets stuck in the tunnel if the Odyssey Moveset is enabled.
// The following code reduces the number of sub-steps to the Vanilla amount
// and disable the check to neighbor cells in Breezing Beach.
// --------------------------------------------------------------------------
#undef OMM_COLLISION_CHECK_NEIGHBOR_CELLS
#define OMM_COLLISION_CHECK_NEIGHBOR_CELLS              (OMM_MOVESET_ODYSSEY && !(gCurrLevelNum == LEVEL_SL && gCurrAreaIndex == 2))
#undef OMM_STEP_NUM_SUB_STEPS_WATER
#define OMM_STEP_NUM_SUB_STEPS_WATER                    ((OMM_MOVESET_ODYSSEY && !(gCurrLevelNum == LEVEL_SL && gCurrAreaIndex == 2)) ? 16 : 1)
#endif

//
// Options
//

#define OMM_MOVESET_CLASSIC                             (gOmmMovesetType == 0)
#define OMM_MOVESET_ODYSSEY                             (gOmmMovesetType != 0)
#define OMM_MOVESET_ODYSSEY_3H                          (gOmmMovesetType == 1)
#define OMM_MOVESET_ODYSSEY_6H                          (gOmmMovesetType == 2)
#define OMM_CAP_CLASSIC                                 (gOmmCapType == 0)
#define OMM_CAP_CAPPY_NO_CAPTURE                        (gOmmCapType == 1)
#define OMM_CAP_CAPPY_CAPTURE                           (gOmmCapType == 2)
#define OMM_STARS_CLASSIC                               (gOmmStarsMode == 0)
#define OMM_STARS_NON_STOP                              (gOmmStarsMode == 1)
#define OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE          (OMM_STARS_NON_STOP && !omm_is_ending_cutscene())
#define OMM_POWER_UPS_CLASSIC                           (gOmmPowerUpsType == 0)
#define OMM_POWER_UPS_IMPROVED                          (gOmmPowerUpsType == 1)
#define OMM_CAMERA_CLASSIC                              (gOmmCameraMode == 0)
#define OMM_CAMERA_8_DIRECTIONS                         (gOmmCameraMode == 1)
#define OMM_CAMERA_16_DIRECTIONS                        (gOmmCameraMode == 2)
#define OMM_SPARKLY_STARS_MODE                          (gOmmSparklyStarsMode)
#define OMM_SPARKLY_STARS_ASSIST                        (gOmmSparklyStarsAssist)
#define OMM_SPARKLY_STARS_HINT                          (gOmmSparklyStarsHint)
#define OMM_EXTRAS_SMO_ANIMATIONS                       (gOmmExtrasSMOAnimations && OMM_MOVESET_ODYSSEY)
#define OMM_EXTRAS_CAPPY_AND_TIARA                      (gOmmExtrasCappyAndTiara && !OMM_CAP_CLASSIC)
#define OMM_EXTRAS_COLORED_STARS                        (gOmmExtrasColoredStars && !OMM_GAME_IS_SMMS)
#define OMM_EXTRAS_VANISHING_HUD                        (gOmmExtrasVanishingHUD)
#define OMM_EXTRAS_REVEAL_SECRETS                       (gOmmExtrasRevealSecrets)
#define OMM_EXTRAS_RED_COINS_RADAR                      (gOmmExtrasRedCoinsRadar)
#define OMM_EXTRAS_SHOW_STAR_NUMBER                     (gOmmExtrasShowStarNumber && !omm_is_ending_cutscene() && (!OMM_GAME_IS_SMSR || (gCurrLevelNum != LEVEL_ENDING)))
#define OMM_EXTRAS_INVISIBLE_MODE                       (gOmmExtrasInvisibleMode)
#define OMM_EXTRAS_CRYSTAL_STARS_REWARD                 (gOmmExtrasCrystalStarsReward && omm_ssd_is_completed(OMM_SSM_HARD))
#define OMM_EXTRAS_NEBULA_STARS_REWARD                  (gOmmExtrasNebulaStarsReward && omm_ssd_is_completed(OMM_SSM_LUNATIC))
#define OMM_CHEAT_UNLIMITED_CAPPY_BOUNCES               (gOmmCheatUnlimitedCappyBounces == 1)
#define OMM_CHEAT_CAPPY_STAYS_FOREVER                   (gOmmCheatCappyStaysForever == 1)
#define OMM_CHEAT_HOMING_ATTACK_GLOBAL_RANGE            (gOmmCheatHomingAttackGlobalRange == 1)
#define OMM_CHEAT_MARIO_TELEPORTS_TO_CAPPY              (gOmmCheatMarioTeleportsToCappy == 1)
#define OMM_CHEAT_CAPPY_CAN_COLLECT_STARS               (gOmmCheatCappyCanCollectStars == 1)
#define OMM_CHEAT_PLAY_AS_CAPPY                         (gOmmCheatPlayAsCappy == 1)
#define OMM_CHEAT_PEACH_ENDLESS_VIBE_GAUGE              (gOmmCheatPeachEndlessVibeGauge == 1)
#if OMM_GAME_IS_R96A
#define OMM_CHEAT_MOON_JUMP                             (Cheats.EnableCheats && (Cheats.MoonJump && !Cheats.ChaosMode))
#define OMM_CHEAT_GOD_MODE                              (Cheats.EnableCheats && (Cheats.GodMode && !Cheats.ChaosMode))
#define OMM_CHEAT_INVINCIBLE                            (false) // Not implemented in R96A
#define OMM_CHEAT_SUPER_SPEED                           (false) // Handled by R96A
#define OMM_CHEAT_SUPER_RESPONSIVE                      (Cheats.EnableCheats && ((Cheats.Responsive && !Cheats.ChaosMode) || ((Cheats.ChaosControls >> 0) & 1)))
#define OMM_CHEAT_NO_FALL_DAMAGE                        (Cheats.EnableCheats && (Cheats.GodMode && !Cheats.ChaosMode))
#define OMM_CHEAT_WALK_ON_LAVA                          (Cheats.EnableCheats && (Cheats.WalkOnHazards && !Cheats.ChaosMode))
#define OMM_CHEAT_WALK_ON_QUICKSAND                     (Cheats.EnableCheats && (Cheats.WalkOnHazards && !Cheats.ChaosMode))
#define OMM_CHEAT_WALK_ON_WATER                         (false) // Not implemented in R96A
#define OMM_CHEAT_WALK_ON_GAS                           (false) // Not implemented in R96A
#define OMM_CHEAT_WALK_ON_SLOPE                         (false) // Not implemented in R96A
#define OMM_CHEAT_WALK_ON_DEATH_BARRIER                 (Cheats.EnableCheats && (Cheats.NoDeathBarrier && !Cheats.ChaosMode))
#define OMM_CHEAT_BLJ_ANYWHERE                          (Cheats.EnableCheats && (Cheats.BLJAnywhere && !Cheats.ChaosMode))
#else
#define OMM_CHEAT_MOON_JUMP                             (gOmmCheatEnable && gOmmCheatMoonJump)
#define OMM_CHEAT_GOD_MODE                              (gOmmCheatEnable && gOmmCheatGodMode)
#define OMM_CHEAT_INVINCIBLE                            (gOmmCheatEnable && gOmmCheatInvincible)
#define OMM_CHEAT_SUPER_SPEED                           (gOmmCheatEnable && gOmmCheatSuperSpeed)
#define OMM_CHEAT_SUPER_RESPONSIVE                      (gOmmCheatEnable && gOmmCheatSuperResponsive)
#define OMM_CHEAT_NO_FALL_DAMAGE                        (gOmmCheatEnable && gOmmCheatNoFallDamage)
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
                                                         OMM_CHEAT_PEACH_ENDLESS_VIBE_GAUGE)
#if OMM_GAME_IS_R96A
#define OMM_CHEATS_DISABLE                              {gOmmCheatUnlimitedCappyBounces = false; \
                                                         gOmmCheatCappyStaysForever = false; \
                                                         gOmmCheatHomingAttackGlobalRange = false; \
                                                         gOmmCheatMarioTeleportsToCappy = false; \
                                                         gOmmCheatCappyCanCollectStars = false; \
                                                         gOmmCheatPlayAsCappy = false; \
                                                         gOmmCheatPeachEndlessVibeGauge = false;}
#else
#define OMM_CHEATS_DISABLE                              {gOmmCheatEnable = false; \
                                                         gOmmCheatMoonJump = false; \
                                                         gOmmCheatGodMode = false; \
                                                         gOmmCheatInvincible = false; \
                                                         gOmmCheatSuperSpeed = false; \
                                                         gOmmCheatSuperResponsive = false; \
                                                         gOmmCheatNoFallDamage = false; \
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
                                                         gOmmCheatPeachEndlessVibeGauge = false;}
#endif

//
// Dialogs
//

#define OMM_DIALOG_START_INDEX                          (DIALOG_COUNT + 10)
#define OMM_DIALOG_BOWSER_1_INTRO                       (OMM_DIALOG_START_INDEX + 0)
#define OMM_DIALOG_BOWSER_1_HINT_1                      (OMM_DIALOG_START_INDEX + 1)
#define OMM_DIALOG_BOWSER_1_HINT_2                      (OMM_DIALOG_START_INDEX + 2)
#define OMM_DIALOG_BOWSER_1_HINT_3                      (OMM_DIALOG_START_INDEX + 3)
#define OMM_DIALOG_BOWSER_2_INTRO                       (OMM_DIALOG_START_INDEX + 4)
#define OMM_DIALOG_BOWSER_3_INTRO                       (OMM_DIALOG_START_INDEX + 5)
#define OMM_DIALOG_BOWSER_3_DEFEAT                      (OMM_DIALOG_START_INDEX + 6)

#define OMM_DIALOG_SPARKLY_MULTI(lpbN, lpbH, lpbE)      0, 0, (((lpbE) * 100) + ((lpbH) * 10) + ((lpbN) * 1))
#define OMM_DIALOG_SPARKLY_MULTI_END                    "+\n"
#define OMM_DIALOG_SPARKLY_STAR_NAMES                   (OMM_DIALOG_START_INDEX + 7)
#define OMM_DIALOG_SPARKLY_STAR                         (OMM_DIALOG_START_INDEX + 8)
#define OMM_DIALOG_SPARKLY_STAR_1                       (OMM_DIALOG_START_INDEX + 8)
#define OMM_DIALOG_SPARKLY_STAR_2                       (OMM_DIALOG_START_INDEX + 9)
#define OMM_DIALOG_SPARKLY_STAR_3                       (OMM_DIALOG_START_INDEX + 10)
#define OMM_DIALOG_SPARKLY_STAR_4                       (OMM_DIALOG_START_INDEX + 11)
#define OMM_DIALOG_SPARKLY_STAR_5                       (OMM_DIALOG_START_INDEX + 12)
#define OMM_DIALOG_SPARKLY_STAR_6                       (OMM_DIALOG_START_INDEX + 13)
#define OMM_DIALOG_SPARKLY_STAR_7                       (OMM_DIALOG_START_INDEX + 14)
#define OMM_DIALOG_SPARKLY_STAR_8                       (OMM_DIALOG_START_INDEX + 15)
#define OMM_DIALOG_SPARKLY_STAR_9                       (OMM_DIALOG_START_INDEX + 16)
#define OMM_DIALOG_SPARKLY_STAR_10                      (OMM_DIALOG_START_INDEX + 17)
#define OMM_DIALOG_SPARKLY_STAR_11                      (OMM_DIALOG_START_INDEX + 18)
#define OMM_DIALOG_SPARKLY_STAR_12                      (OMM_DIALOG_START_INDEX + 19)
#define OMM_DIALOG_SPARKLY_STAR_13                      (OMM_DIALOG_START_INDEX + 20)
#define OMM_DIALOG_SPARKLY_STAR_14                      (OMM_DIALOG_START_INDEX + 21)
#define OMM_DIALOG_SPARKLY_STAR_15                      (OMM_DIALOG_START_INDEX + 22)
#define OMM_DIALOG_SPARKLY_STAR_16                      (OMM_DIALOG_START_INDEX + 23)
#define OMM_DIALOG_SPARKLY_STAR_17                      (OMM_DIALOG_START_INDEX + 24)
#define OMM_DIALOG_SPARKLY_STAR_18                      (OMM_DIALOG_START_INDEX + 25)
#define OMM_DIALOG_SPARKLY_STAR_19                      (OMM_DIALOG_START_INDEX + 26)
#define OMM_DIALOG_SPARKLY_STAR_20                      (OMM_DIALOG_START_INDEX + 27)
#define OMM_DIALOG_SPARKLY_STAR_21                      (OMM_DIALOG_START_INDEX + 28)
#define OMM_DIALOG_SPARKLY_STAR_22                      (OMM_DIALOG_START_INDEX + 29)
#define OMM_DIALOG_SPARKLY_STAR_23                      (OMM_DIALOG_START_INDEX + 30)
#define OMM_DIALOG_SPARKLY_STAR_24                      (OMM_DIALOG_START_INDEX + 31)
#define OMM_DIALOG_SPARKLY_STAR_25                      (OMM_DIALOG_START_INDEX + 32)
#define OMM_DIALOG_SPARKLY_STAR_26                      (OMM_DIALOG_START_INDEX + 33)
#define OMM_DIALOG_SPARKLY_STAR_27                      (OMM_DIALOG_START_INDEX + 34)
#define OMM_DIALOG_SPARKLY_STAR_28                      (OMM_DIALOG_START_INDEX + 35)
#define OMM_DIALOG_SPARKLY_STAR_29                      (OMM_DIALOG_START_INDEX + 36)
#define OMM_DIALOG_SPARKLY_BOWSER_4_UNLOCKED            (OMM_DIALOG_START_INDEX + 37)
#define OMM_DIALOG_SPARKLY_BOWSER_4_INTRO               (OMM_DIALOG_START_INDEX + 38)
#define OMM_DIALOG_SPARKLY_BOWSER_4_DEFEAT              (OMM_DIALOG_START_INDEX + 39)

#define OMM_DIALOG_SPARKLY_MIPS_1                       (OMM_DIALOG_START_INDEX + 40)
#define OMM_DIALOG_SPARKLY_MIPS_2                       (OMM_DIALOG_START_INDEX + 41)
#define OMM_DIALOG_SPARKLY_MIPS_3                       (OMM_DIALOG_START_INDEX + 42)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_INTRO             (OMM_DIALOG_START_INDEX + 43)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_0                 (OMM_DIALOG_START_INDEX + 44)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_1                 (OMM_DIALOG_START_INDEX + 45)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_2                 (OMM_DIALOG_START_INDEX + 46)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_3                 (OMM_DIALOG_START_INDEX + 47)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_4                 (OMM_DIALOG_START_INDEX + 48)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_5                 (OMM_DIALOG_START_INDEX + 49)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_6                 (OMM_DIALOG_START_INDEX + 50)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_7                 (OMM_DIALOG_START_INDEX + 51)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_0             (OMM_DIALOG_START_INDEX + 52)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_1             (OMM_DIALOG_START_INDEX + 53)
#define OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_2             (OMM_DIALOG_START_INDEX + 54)
#define OMM_DIALOG_END_INDEX                            (OMM_DIALOG_START_INDEX + 55)
#define OMM_DIALOG_COUNT                                (OMM_DIALOG_END_INDEX - OMM_DIALOG_START_INDEX)

//
// Texts
//

#define OMM_TEXT_(id, str) extern const char OMM_TEXT_##id[];
#include "data/omm/omm_defines_texts.inl"
#undef OMM_TEXT_

//
// Textures
//

#define OMM_TEXTURE_(id, str) extern const char OMM_TEXTURE_##id[];
#include "data/omm/omm_defines_textures.inl"
#undef OMM_TEXTURE_

//
// Assets
//

#define OMM_ASSET_(id, str) extern const char OMM_ASSET_##id[];
#include "data/omm/omm_defines_assets.inl"
#undef OMM_ASSET_

//
// Sounds
//

#define OMM_SOUND_PEACH_JUMP_YAH                        0x00
#define OMM_SOUND_PEACH_JUMP_WAH                        0x01
#define OMM_SOUND_PEACH_JUMP_HOO                        0x02
#define OMM_SOUND_PEACH_JUMP_YAHOO                      0x03
#define OMM_SOUND_PEACH_JUMP_WAHA                       0x04
#define OMM_SOUND_PEACH_JUMP_YIPPEE                     0x05
#define OMM_SOUND_PEACH_LAND_HAHA                       0x06
#define OMM_SOUND_PEACH_PUNCH_YAH                       0x07
#define OMM_SOUND_PEACH_PUNCH_WAH                       0x08
#define OMM_SOUND_PEACH_PUNCH_HOO                       0x09
#define OMM_SOUND_PEACH_GRAB_HRMM                       0x0A
#define OMM_SOUND_PEACH_THROW_WAH2                      0x0B
#define OMM_SOUND_PEACH_GROUND_POUND_WAH                0x0C
#define OMM_SOUND_PEACH_DIVE_HOOHOO                     0x0D
#define OMM_SOUND_PEACH_TWIRL_BOUNCE                    0x0E
#define OMM_SOUND_PEACH_SO_LONGA_BOWSER                 0x0F
#define OMM_SOUND_PEACH_LEDGE_GRAB_WHOA                 0x10
#define OMM_SOUND_PEACH_LEDGE_CLIMB_EEUH                0x11
#define OMM_SOUND_PEACH_LEDGE_CLIMB_UH2                 0x12
#define OMM_SOUND_PEACH_LEDGE_MISS_UH                   0x13
#define OMM_SOUND_PEACH_BONK_DOH                        0x14
#define OMM_SOUND_PEACH_BONK_OOOF                       0x15
#define OMM_SOUND_PEACH_ATTACKED                        0x16
#define OMM_SOUND_PEACH_ON_FIRE                         0x17
#define OMM_SOUND_PEACH_DYING                           0x18
#define OMM_SOUND_PEACH_DROWNING                        0x19
#define OMM_SOUND_PEACH_FALLING_WAAAOOOW                0x1A
#define OMM_SOUND_PEACH_MAMA_MIA                        0x1B
#define OMM_SOUND_PEACH_LETS_A_GO                       0x1C
#define OMM_SOUND_PEACH_HERE_WE_GO                      0x1D
#define OMM_SOUND_PEACH_PANTING1                        0x1E
#define OMM_SOUND_PEACH_PANTING2                        0x1F
#define OMM_SOUND_PEACH_PANTING3                        0x20
#define OMM_SOUND_PEACH_COUGHING1                       0x21
#define OMM_SOUND_PEACH_COUGHING2                       0x22
#define OMM_SOUND_PEACH_COUGHING3                       0x23
#define OMM_SOUND_PEACH_YAWNING                         0x24
#define OMM_SOUND_PEACH_VIBE_JOY                        0x25
#define OMM_SOUND_PEACH_VIBE_RAGE                       0x26
#define OMM_SOUND_PEACH_VIBE_GLOOM                      0x27
#define OMM_SOUND_PEACH_VIBE_CALM                       0x28

#define OMM_SOUND_TOAD_1                                0x40
#define OMM_SOUND_TOAD_2                                0x41
#define OMM_SOUND_TOAD_3                                0x42
#define OMM_SOUND_TOAD_4                                0x43
#define OMM_SOUND_TOAD_5                                0x44
#define OMM_SOUND_TOAD_6                                0x45
#define OMM_SOUND_TOAD_7                                0x46
#define OMM_SOUND_TOAD_8                                0x47
#define OMM_SOUND_TOAD_9                                0x48

#define OMM_SOUND_EVENT_CAPTURE                         0x50
#define OMM_SOUND_EVENT_UNCAPTURE                       0x51
#define OMM_SOUND_EVENT_LIFE_UP                         0x52
#define OMM_SOUND_EVENT_SPARKLY_STAR_GET                0x53
#define OMM_SOUND_EVENT_DEATH_MARIO                     0x54
#define OMM_SOUND_EVENT_DEATH_MARIO_WATER               0x55
#define OMM_SOUND_EVENT_DEATH_MARIO_FALL                0x56
#define OMM_SOUND_EVENT_DEATH_PEACH                     0x57
#define OMM_SOUND_EVENT_DEATH_PEACH_WATER               0x58
#define OMM_SOUND_EVENT_DEATH_PEACH_FALL                0x59
#define OMM_SOUND_EVENT_DEATH_LUIGI                     0x5A
#define OMM_SOUND_EVENT_DEATH_LUIGI_WATER               0x5B
#define OMM_SOUND_EVENT_DEATH_LUIGI_FALL                0x5C
#define OMM_SOUND_EVENT_DEATH_WARIO                     0x5D
#define OMM_SOUND_EVENT_DEATH_WARIO_WATER               0x5E
#define OMM_SOUND_EVENT_DEATH_WARIO_FALL                0x5F

#define OMM_SOUND_EFFECT_DAMAGE                         0x80
#define OMM_SOUND_EFFECT_HEAL                           0x81
#define OMM_SOUND_EFFECT_FREEZE_1                       0x82
#define OMM_SOUND_EFFECT_FREEZE_2                       0x83
#define OMM_SOUND_EFFECT_PROPELLER_1                    0x84
#define OMM_SOUND_EFFECT_PROPELLER_2                    0x85
#define OMM_SOUND_EFFECT_PROPELLER_3                    0x86
#define OMM_SOUND_EFFECT_MIDAIR_SPIN                    0x87
#define OMM_SOUND_EFFECT_PEACH_GLIDE                    0x88
#define OMM_SOUND_EFFECT_PEACH_VIBE                     0x89
#define OMM_SOUND_EFFECT_PERRY_CHARGE                   0x8A
#define OMM_SOUND_EFFECT_PERRY_CHARGED                  0x8B
#define OMM_SOUND_EFFECT_PERRY_BEAM                     0x8C

//
// Playable characters
//

#define OMM_PLAYER_MARIO                                0
#define OMM_PLAYER_PEACH                                1
#define OMM_PLAYER_LUIGI                                2
#define OMM_PLAYER_WARIO                                3
#define OMM_PLAYER_IS_MARIO                             omm_player_is_selected(OMM_PLAYER_MARIO)
#define OMM_PLAYER_IS_PEACH                             omm_player_is_selected(OMM_PLAYER_PEACH)
#define OMM_PLAYER_IS_LUIGI                             omm_player_is_selected(OMM_PLAYER_LUIGI)
#define OMM_PLAYER_IS_WARIO                             omm_player_is_selected(OMM_PLAYER_WARIO)

//
// Routines
//

#define OMM_ROUTINE_TYPE_LEVEL_ENTRY                    0
#define OMM_ROUTINE_TYPE_UPDATE                         1
#define OMM_ROUTINE_TYPE_PRE_RENDER                     2
#define OMM_ROUTINE_TYPES                               3
#define OMM_ROUTINE_LEVEL_ENTRY(func)                   static void func(void); OMM_AT_STARTUP static void func##_init() { omm_add_routine(OMM_ROUTINE_TYPE_LEVEL_ENTRY, func); } static void func(void)
#define OMM_ROUTINE_UPDATE(func)                        static void func(void); OMM_AT_STARTUP static void func##_init() { omm_add_routine(OMM_ROUTINE_TYPE_UPDATE, func); } static void func(void)
#define OMM_ROUTINE_PRE_RENDER(func)                    static void func(void); OMM_AT_STARTUP static void func##_init() { omm_add_routine(OMM_ROUTINE_TYPE_PRE_RENDER, func); } static void func(void)

//
// Custom object fields
//

#define gCurrGraphNodeObject                            ((struct Object *) gCurGraphNodeObject)
#define oCameraToObject                                 header.gfx.cameraToObject
#define oThrowMatrix                                    header.gfx.throwMatrix
#define oGraphNode                                      header.gfx.sharedChild
#define oNodeFlags                                      header.gfx.node.flags
#define oAreaIndex                                      header.gfx.mAreaIndex
#define oGfxPos                                         header.gfx.pos
#define oGfxAngle                                       header.gfx.angle
#define oGfxScale                                       header.gfx.scale
#define oScaleX                                         header.gfx.scale[0]
#define oScaleY                                         header.gfx.scale[1]
#define oScaleZ                                         header.gfx.scale[2]
#define oAnimInfo                                       header.gfx.mAnimInfo
#define oAnimID                                         header.gfx.mAnimInfo.animID
#define oCurrAnim                                       header.gfx.mAnimInfo.curAnim
#define oWall                                           OBJECT_FIELD_SURFACE(0x03)
#define oCeil                                           OBJECT_FIELD_SURFACE(0x04)
#define oGeoData                                        OBJECT_FIELD_VPTR(0x47)
#define oBehaviorCommand                                OBJECT_FIELD_CVPTR(0x47)
#define oBehaviorStackIndex                             OBJECT_FIELD_U32(0x48)
#define oCappyLifeTimer                                 OBJECT_FIELD_S32(0x1B)
#define oCappyBehavior                                  OBJECT_FIELD_S16(0x1C, 0)
#define oCappyFlags                                     OBJECT_FIELD_S16(0x1C, 1)
#define oCappyYaw                                       OBJECT_FIELD_S16(0x1D, 0)
#define oCappyThrowStrength                             OBJECT_FIELD_S16(0x1D, 1)
#define oGoombaStackParent                              OBJECT_FIELD_OBJ(0x00)
#define oMrIBeamPower                                   OBJECT_FIELD_F32(0x17)
#define oSnufitBallStrong                               OBJECT_FIELD_S32(0x17)
#define oMenuButtonCharacterIndex                       OBJECT_FIELD_S32(0x21)
#define oMipsCurrentWaypoint                            OBJECT_FIELD_S32(0x1D)
#define oMipsTargetWaypoint                             OBJECT_FIELD_S32(0x1E)
#define oMipsGrabbedCounter                             OBJECT_FIELD_S32(0x1F)
#define oMipsEscapePath                                 OBJECT_FIELD_S32(0x20)
#define oMipsEscapeIndex                                OBJECT_FIELD_S32(0x21)
#define oCelebStarRadius                                OBJECT_FIELD_F32(0x1B)
#define oCelebStarHeight                                OBJECT_FIELD_F32(0x1C)
#define oFlamingBobombAura                              OBJECT_FIELD_OBJ(0x00)
#define oFlamingBobombIndex                             OBJECT_FIELD_S32(0x1B)
#define oFlamingBobombCount                             OBJECT_FIELD_S32(0x1C)
#define oFlamingBobombMaxRadius                         OBJECT_FIELD_F32(0x1D)
#define oFlamingBobombMaxHeight                         OBJECT_FIELD_F32(0x1E)
#define oBowserMine                                     OBJECT_FIELD_VPTR(0x1B)
#define oBowserHit                                      OBJECT_FIELD_S32(0x1C)
#define oBowserFireDuration                             OBJECT_FIELD_S32(0x1B)
#define oIsBowserClone                                  OBJECT_FIELD_S32(0x1D)
#define oChainChompFreeState                            OBJECT_FIELD_S32(0x1F)
#define oChainChompFreeTimer                            OBJECT_FIELD_S32(0x20)
#define oChainChompFreeAngle                            OBJECT_FIELD_S32(0x21)
#define oShockwaveFireRadius                            OBJECT_FIELD_F32(0x1B)
#define oShockwaveFireWidth                             OBJECT_FIELD_F32(0x1C)
#define oShockwaveFireHeight                            OBJECT_FIELD_F32(0x1D)
#define oShockwaveFireSpeed                             OBJECT_FIELD_F32(0x1E)
#define oShockwaveFireDuration                          OBJECT_FIELD_S32(0x1F)
#define oRisingLavaMinY                                 OBJECT_FIELD_F32(0x1B)
#define oRisingLavaMaxY                                 OBJECT_FIELD_F32(0x1C)
#define oRisingLavaVelY                                 OBJECT_FIELD_F32(0x1D)
#define oRisingLavaRadius                               OBJECT_FIELD_F32(0x1E)
#define oRisingLavaRotVel                               OBJECT_FIELD_S32(0x1F)
#define oRisingLavaShake                                OBJECT_FIELD_S32(0x20)
#define oSparklyStarMode                                OBJECT_FIELD_S32(0x1C)
#define oSparklyStarPosX                                OBJECT_FIELD_F32(0x1E)
#define oSparklyStarPosY                                OBJECT_FIELD_F32(0x1F)
#define oSparklyStarPosZ                                OBJECT_FIELD_F32(0x20)
#define oPerryFlags                                     OBJECT_FIELD_S32(0x1B)
#define oPerryRightHandRot(x)                           OBJECT_FIELD_S32(0x1C + x)
#define oPerryShockwaveBlast                            OBJECT_FIELD_S16(0x1B, 0)
#define oPerryShockwaveDelay                            OBJECT_FIELD_S16(0x1B, 1)
#define oPerryShockwaveAngleYaw                         OBJECT_FIELD_S32(0x1C)
#define oPerryShockwaveAngleDir                         OBJECT_FIELD_S32(0x1D)
#define oPerryShockwaveBaseScale                        OBJECT_FIELD_F32(0x1E)
#define oPerryChargeSwordTimer                          OBJECT_FIELD_S32(0x1C)
#define oPerryChargeHandTimer                           OBJECT_FIELD_S32(0x1D)
#define oPerryType                                      oAnimState
#define oBehaviorTypes                                  unused1
#define oBehaviorRef                                    unused2
#define oDistToFloor                                    oPosY - o->oFloorHeight

#endif // OMM_DEFINES_H

#ifndef OMM_OPTIONS_H
#define OMM_OPTIONS_H

#include "types.h"
#include "pc/configfile.h" // MAX_BINDS

#define DECLARE_KBINDS(option)                  extern u32 option[MAX_BINDS]
#define DECLARE_TOGGLE(option)                  extern bool option
#define DECLARE_CHOICE(option)                  extern u32 option; extern const u32 option##Count
#define DECLARE_SCROLL(option)                  extern u32 option
#define DECLARE_TOGGLE_SC(option)               DECLARE_TOGGLE(option); DECLARE_KBINDS(option##Shortcuts)
#define DECLARE_CHOICE_SC(option)               DECLARE_CHOICE(option); DECLARE_KBINDS(option##Shortcuts)

#define DEFINE_KBINDS(option, v0, v1, v2)       u32 option[MAX_BINDS] = { v0, v1, v2 }
#define DEFINE_TOGGLE(option, v)                bool option = v
#define DEFINE_CHOICE(option, v, count)         u32 option = v; const u32 option##Count = count
#define DEFINE_SCROLL(option, v)                u32 option = v
#define DEFINE_TOGGLE_SC(option, v)             DEFINE_TOGGLE(option, v); DEFINE_KBINDS(option##Shortcuts, VK_INVALID, VK_INVALID, VK_INVALID)
#define DEFINE_CHOICE_SC(option, v, count)      DEFINE_CHOICE(option, v, count); DEFINE_KBINDS(option##Shortcuts, VK_INVALID, VK_INVALID, VK_INVALID)

#define CONFIG_KBINDS(config, option)           { .name = config, .type = CONFIG_TYPE_BIND, .uintValue = option }
#define CONFIG_TOGGLE(config, option)           { .name = config, .type = CONFIG_TYPE_BOOL, .boolValue = &option }
#define CONFIG_CHOICE(config, option)           { .name = config, .type = CONFIG_TYPE_UINT, .uintValue = &option }
#define CONFIG_SCROLL(config, option)           { .name = config, .type = CONFIG_TYPE_UINT, .uintValue = &option }
#define CONFIG_TOGGLE_SC(config, option)        { .name = config, .type = CONFIG_TYPE_BOOL, .boolValue = &option }, \
                                                { .name = config "_shortcuts", .type = CONFIG_TYPE_BIND, .uintValue = option##Shortcuts }
#define CONFIG_CHOICE_SC(config, option)        { .name = config, .type = CONFIG_TYPE_UINT, .uintValue = &option }, \
                                                { .name = config "_shortcuts", .type = CONFIG_TYPE_BIND, .uintValue = option##Shortcuts }

#define NO_SHORTCUT(option)                     (option##Shortcuts[0] == VK_INVALID && option##Shortcuts[1] == VK_INVALID && option##Shortcuts[2] == VK_INVALID)
#define SHORTCUT_PRESSED(option)                (gKeyPressed != VK_INVALID && (gKeyPressed == option##Shortcuts[0] || gKeyPressed == option##Shortcuts[1] || gKeyPressed == option##Shortcuts[2]))

#define OPT_TYPE(x)                             (((x) & 0x0F) >> 0)
#define OPT_STATE(x)                            (((x) & 0xF0) >> 4)
#define OPT_TYPE_STATE(type, state)             (((type) << 0) | ((state) << 4))

#if OMM_GAME_IS_R96X
#define omm_opt_text(str)                       (const u8 *) str
#define omm_opt_text_length(str)                (s32) strlen((const char *) str)
#else
#define omm_opt_text(str)                       omm_text_convert(str, true)
#define omm_opt_text_length(str)                omm_text_length(str)
#endif

struct Option;
typedef struct { const u8 *label; void *subMenu; } OmmOptMenu;
extern OmmOptMenu gOmmOptMenu;
extern OmmOptMenu gOmmOptModels;
extern OmmOptMenu gOmmOptCs;
extern OmmOptMenu gOmmOptTimeTrials;
extern OmmOptMenu gOmmOptWarpToLevel;
extern OmmOptMenu gOmmOptWarpToCastle;
extern OmmOptMenu gOmmOptControls;
#if !OMM_GAME_IS_R96X
extern OmmOptMenu gOmmOptCheats;
#endif
extern OmmOptMenu gOmmOptDataManagement;
extern OmmOptMenu gOmmOptChangeGame;
extern OmmOptMenu gOmmOptExitGame;
extern u32 gOmmOptYesFunc[1];

enum OmmFps            { OMM_FPS_30, OMM_FPS_60, OMM_FPS_AUTO, OMM_FPS_INF };
enum OmmTextureCaching { OMM_TEXTURE_CACHING_DISABLED, OMM_TEXTURE_CACHING_AT_START_UP, OMM_TEXTURE_CACHING_PERMANENT };
enum OmmHudMode        { OMM_HUD_MODE_ALWAYS, OMM_HUD_MODE_VANISHING, OMM_HUD_MODE_PRO, OMM_HUD_MODE_NONE };
enum OmmCameraInvert   { OMM_CAMERA_INVERT_NONE, OMM_CAMERA_INVERT_X, OMM_CAMERA_INVERT_Y, OMM_CAMERA_INVERT_BOTH };
DECLARE_CHOICE(gOmmFrameRate);
#if OMM_CODE_DEBUG
DECLARE_CHOICE(gOmmShowFPS);
#else
DECLARE_TOGGLE(gOmmShowFPS);
#endif
DECLARE_CHOICE(gOmmTextureCaching);
DECLARE_TOGGLE(gOmmModelPackCaching);
DECLARE_CHOICE(gOmmHudMode);
DECLARE_SCROLL(gOmmHudSize);
DECLARE_CHOICE(gOmmCameraInvert1stPerson);
DECLARE_CHOICE(gOmmCameraInvert3rdPerson);
DECLARE_KBINDS(gOmmControlsButtonA);
DECLARE_KBINDS(gOmmControlsButtonB);
DECLARE_KBINDS(gOmmControlsButtonX);
DECLARE_KBINDS(gOmmControlsButtonY);
DECLARE_KBINDS(gOmmControlsButtonStart);
DECLARE_KBINDS(gOmmControlsButtonSpin);
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
#if !OMM_GAME_IS_R96X
DECLARE_TOGGLE(gOmmCheatEnable);
DECLARE_TOGGLE(gOmmCheatMoonJump);
DECLARE_TOGGLE(gOmmCheatGodMode);
DECLARE_TOGGLE(gOmmCheatInvincible);
DECLARE_TOGGLE(gOmmCheatSuperSpeed);
DECLARE_TOGGLE(gOmmCheatSuperResponsive);
DECLARE_TOGGLE(gOmmCheatNoFallDamage);
DECLARE_TOGGLE(gOmmCheatCapModifier);
DECLARE_TOGGLE(gOmmCheatWalkOnLava);
DECLARE_TOGGLE(gOmmCheatWalkOnQuicksand);
DECLARE_TOGGLE(gOmmCheatWalkOnWater);
DECLARE_TOGGLE(gOmmCheatWalkOnGas);
DECLARE_TOGGLE(gOmmCheatWalkOnSlope);
DECLARE_TOGGLE(gOmmCheatWalkOnDeathBarrier);
DECLARE_TOGGLE(gOmmCheatBljAnywhere);
#endif
DECLARE_TOGGLE(gOmmCsPalettePreset);
DECLARE_TOGGLE(gOmmCsAnimations);
DECLARE_TOGGLE(gOmmCsVoices);
DECLARE_SCROLL(gOmmCsVoiceVolumeFactor);
DECLARE_TOGGLE(gOmmTimeTrialsEnabled);
DECLARE_TOGGLE(gOmmTimeTrialsShowStarGhosts);
DECLARE_TOGGLE(gOmmTimeTrialsShowBowserGhosts);
DECLARE_TOGGLE(gOmmTimeTrialsShowSlideGhosts);
DECLARE_TOGGLE(gOmmTimeTrialsShowCoinsGhosts);
DECLARE_TOGGLE(gOmmTimeTrialsShowAllStarsGhosts);
DECLARE_CHOICE_SC(gOmmCharacter);
DECLARE_CHOICE_SC(gOmmMovesetType);
DECLARE_CHOICE_SC(gOmmCapType);
DECLARE_CHOICE_SC(gOmmStarsMode);
DECLARE_CHOICE_SC(gOmmPowerUpsType);
DECLARE_CHOICE_SC(gOmmCameraMode);
DECLARE_CHOICE_SC(gOmmSparklyStarsMode);
DECLARE_CHOICE_SC(gOmmSparklyStarsHintAtLevelEntry);
DECLARE_CHOICE_SC(gOmmSparklyStarsCompletionReward);
DECLARE_TOGGLE_SC(gOmmSparklyStarsPerryCharge);
DECLARE_TOGGLE(gOmmCheatUnlimitedCappyBounces);
DECLARE_TOGGLE(gOmmCheatCappyStaysForever);
DECLARE_TOGGLE(gOmmCheatHomingAttackGlobalRange);
DECLARE_TOGGLE(gOmmCheatMarioTeleportsToCappy);
DECLARE_TOGGLE(gOmmCheatCappyCanCollectStars);
DECLARE_TOGGLE(gOmmCheatPlayAsCappy);
DECLARE_TOGGLE(gOmmCheatPeachEndlessVibeGauge);
DECLARE_TOGGLE(gOmmCheatShadowMario);
DECLARE_CHOICE_SC(gOmmExtrasMarioColors);
DECLARE_CHOICE_SC(gOmmExtrasPeachColors);
DECLARE_CHOICE_SC(gOmmExtrasObjectsRadar);
DECLARE_TOGGLE_SC(gOmmExtrasSMOAnimations);
DECLARE_TOGGLE_SC(gOmmExtrasCappyAndTiara);
DECLARE_TOGGLE_SC(gOmmExtrasColoredStars);
DECLARE_TOGGLE_SC(gOmmExtrasRevealSecrets);
DECLARE_TOGGLE_SC(gOmmExtrasShowStarNumber);
DECLARE_TOGGLE_SC(gOmmExtrasInvisibleMode);
DECLARE_TOGGLE_SC(gOmmExtrasMarioMode);
#if OMM_CODE_DEBUG
DECLARE_TOGGLE_SC(gOmmDebugHitbox);
DECLARE_TOGGLE_SC(gOmmDebugHurtbox);
DECLARE_TOGGLE_SC(gOmmDebugWallbox);
DECLARE_TOGGLE_SC(gOmmDebugSurface);
DECLARE_TOGGLE_SC(gOmmDebugMario);
DECLARE_TOGGLE_SC(gOmmDebugCappy);
#endif
#if OMM_CODE_DEV
#include "data/omm/dev/omm_dev_opt_declare.inl"
#endif

#if OMM_GAME_IS_SM74
void omm_opt_sm74_change_mode(UNUSED struct Option *opt, s32 arg);
#endif
void omm_opt_return_to_previous_menu(UNUSED struct Option *opt, s32 arg);
void omm_opt_return_to_main_menu(UNUSED struct Option *opt, s32 arg);
void omm_opt_camera_enter_snapshot_mode(UNUSED struct Option *opt, s32 arg);
void omm_opt_update_binds();
u32  omm_opt_get_state();

#define omm_opt_select_available(option, zero, count, cond) \
{ static u32 s##option = (u32) (-1); \
if (s##option == (u32) (-1)) { s##option = option; } \
s32 d##option = sign_0_s(option - s##option) * (abs_s(option - s##option) > 1 ? -1 : +1); \
if (d##option != 0) { while (!(cond)) { option = (option + d##option + (count)) % (count); } } \
else if (!(cond)) { option = (zero); } \
s##option = option; }

#endif // OMM_OPTIONS_H

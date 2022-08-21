#ifndef OMM_OPTIONS_H
#define OMM_OPTIONS_H

#include "types.h"
#include "pc/configfile.h" // MAX_BINDS

#define DECLARE_KBINDS(option)                  extern u32 option[MAX_BINDS]
#define DECLARE_TOGGLE(option)                  extern bool option
#define DECLARE_TOGGLE_SC(option)               extern bool option; DECLARE_KBINDS(option##Shortcuts)
#define DECLARE_CHOICE(option)                  extern u32 option
#define DECLARE_CHOICE_SC(option)               extern u32 option; DECLARE_KBINDS(option##Shortcuts)
#define DECLARE_SCROLL(option)                  extern u32 option

#define DEFINE_KBINDS(option, v0, v1, v2)       u32 option[MAX_BINDS] = { v0, v1, v2 }
#define DEFINE_TOGGLE(option, v)                bool option = v
#define DEFINE_TOGGLE_SC(option, v)             bool option = v; DEFINE_KBINDS(option##Shortcuts, VK_INVALID, VK_INVALID, VK_INVALID)
#define DEFINE_CHOICE(option, v)                u32 option = v
#define DEFINE_CHOICE_SC(option, v)             u32 option = v; DEFINE_KBINDS(option##Shortcuts, VK_INVALID, VK_INVALID, VK_INVALID)
#define DEFINE_SCROLL(option, v)                u32 option = v

#define READ_KBINDS(option)                     if (strcmp(token.args[0], #option) == 0) { sscanf(token.args[1], "%X", &option[0]); sscanf(token.args[2], "%X", &option[1]); sscanf(token.args[3], "%X", &option[2]); continue; }
#define READ_TOGGLE(option)                     if (strcmp(token.args[0], #option) == 0) { option = (token.args[1][0] == '1'); continue; }
#define READ_TOGGLE_SC(option)                  if (strcmp(token.args[0], #option) == 0) { option = (token.args[1][0] == '1'); continue; } READ_KBINDS(option##Shortcuts)
#define READ_CHOICE(option)                     if (strcmp(token.args[0], #option) == 0) { sscanf(token.args[1], "%u", &option); continue; }
#define READ_CHOICE_SC(option)                  if (strcmp(token.args[0], #option) == 0) { sscanf(token.args[1], "%u", &option); continue; } READ_KBINDS(option##Shortcuts)

#define WRITE_KBINDS(option)                    write("%s = %04hX %04hX %04hX\n", #option, (u16) option[0], (u16) option[1], (u16) option[2])
#define WRITE_TOGGLE(option)                    write("%s = %u\n", #option, (option ? 1 : 0))
#define WRITE_TOGGLE_SC(option)                 write("%s = %u\n", #option, (option ? 1 : 0)); WRITE_KBINDS(option##Shortcuts)
#define WRITE_CHOICE(option)                    write("%s = %u\n", #option, option)
#define WRITE_CHOICE_SC(option)                 write("%s = %u\n", #option, option); WRITE_KBINDS(option##Shortcuts)

typedef struct { const u8 *label; void *subMenu; } OmmOptMenu;
extern OmmOptMenu gOmmOptMenu;
extern OmmOptMenu gOmmOptControls;
#if !OMM_GAME_IS_R96X
extern OmmOptMenu gOmmOptCheats;
#endif
#if !OMM_CODE_DYNOS
extern OmmOptMenu gOmmOptWarpToLevel;
extern OmmOptMenu gOmmOptReturnToMainMenu;
#endif

enum OmmFps { OMM_FPS_30, OMM_FPS_60, OMM_FPS_AUTO, OMM_FPS_INF };
DECLARE_TOGGLE(gOmmOneHealthMode);
DECLARE_CHOICE(gOmmFrameRate);
DECLARE_TOGGLE(gOmmShowFPS);
DECLARE_CHOICE(gOmmPreloadTextures);
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
#if !OMM_GAME_IS_R96X
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
DECLARE_CHOICE_SC(gOmmCharacter);
DECLARE_CHOICE_SC(gOmmMovesetType);
DECLARE_CHOICE_SC(gOmmCapType);
DECLARE_CHOICE_SC(gOmmStarsMode);
DECLARE_CHOICE_SC(gOmmPowerUpsType);
DECLARE_CHOICE_SC(gOmmCameraMode);
DECLARE_CHOICE_SC(gOmmSparklyStarsMode);
DECLARE_CHOICE_SC(gOmmSparklyStarsHintAtLevelEntry);
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
DECLARE_CHOICE_SC(gOmmExtrasSparklyStarsReward);
#if OMM_CODE_DEBUG
DECLARE_TOGGLE_SC(gOmmDebugHitbox);
DECLARE_TOGGLE_SC(gOmmDebugHurtbox);
DECLARE_TOGGLE_SC(gOmmDebugWallbox);
DECLARE_TOGGLE_SC(gOmmDebugSurface);
DECLARE_TOGGLE_SC(gOmmDebugMario);
DECLARE_TOGGLE_SC(gOmmDebugCappy);
#endif
#if OMM_CODE_DEV
DECLARE_TOGGLE_SC(gOmmDevGameSpeedEnable);
DECLARE_CHOICE_SC(gOmmDevGameSpeedModifier);
DECLARE_TOGGLE_SC(gOmmDevSaveState);
DECLARE_TOGGLE_SC(gOmmDevLoadState);
DECLARE_SCROLL(gOmmDevMainStarsBob);
DECLARE_SCROLL(gOmmDevMainStarsWf);
DECLARE_SCROLL(gOmmDevMainStarsJrb);
DECLARE_SCROLL(gOmmDevMainStarsCcm);
DECLARE_SCROLL(gOmmDevMainStarsBbh);
DECLARE_SCROLL(gOmmDevMainStarsHmc);
DECLARE_SCROLL(gOmmDevMainStarsLll);
DECLARE_SCROLL(gOmmDevMainStarsSsl);
DECLARE_SCROLL(gOmmDevMainStarsDdd);
DECLARE_SCROLL(gOmmDevMainStarsSl);
DECLARE_SCROLL(gOmmDevMainStarsWdw);
DECLARE_SCROLL(gOmmDevMainStarsTtm);
DECLARE_SCROLL(gOmmDevMainStarsThi);
DECLARE_SCROLL(gOmmDevMainStarsTtc);
DECLARE_SCROLL(gOmmDevMainStarsRr);
DECLARE_SCROLL(gOmmDevMainStarsBitdw);
DECLARE_SCROLL(gOmmDevMainStarsBitfs);
DECLARE_SCROLL(gOmmDevMainStarsBits);
DECLARE_SCROLL(gOmmDevMainStarsTotwc);
DECLARE_SCROLL(gOmmDevMainStarsCotmc);
DECLARE_SCROLL(gOmmDevMainStarsVcutm);
DECLARE_SCROLL(gOmmDevMainStarsPss);
DECLARE_SCROLL(gOmmDevMainStarsWmotr);
DECLARE_SCROLL(gOmmDevMainStarsSa);
DECLARE_SCROLL(gOmmDevMainStarsCastle);
DECLARE_SCROLL(gOmmDevSparklyStarsNormal);
DECLARE_SCROLL(gOmmDevSparklyStarsHard);
DECLARE_SCROLL(gOmmDevSparklyStarsLunatic);
#endif

void omm_opt_return_to_main_menu(UNUSED void *opt, s32 arg);
void omm_opt_reset_binds(u32 *binds);
bool omm_mario_colors_read(const char **tokens);
void omm_mario_colors_write(char **buffer);
const char **omm_mario_colors_choices(bool peach);
s32 omm_mario_colors_count();
s32 omm_mario_colors_lights_count(bool peach);
u32 *omm_mario_colors_get_light(bool peach, s32 palette, s32 index);
const char *omm_mario_colors_get_light_name(bool peach, s32 index);

#define omm_opt_select_available(option, zero, count, cond) \
{ static u32 s##option = (u32) (-1); \
if (s##option == (u32) (-1)) { s##option = option; } \
s32 d##option = sign_0_s(option - s##option) * (abs_s(option - s##option) > 1 ? -1 : +1); \
if (d##option != 0) { while (!(cond)) { option = (option + d##option + (count)) % (count); } } \
else if (!(cond)) { option = (zero); } \
s##option = option; }

#endif // OMM_OPTIONS_H

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#if !OMM_GAME_IS_R96X
u32 gKeyPressed;
#endif

//
// From options_menu.c
//

#if !OMM_GAME_IS_RF14
enum OptType { OPT_INVALID = 0, OPT_TOGGLE, OPT_CHOICE, OPT_SCROLL, OPT_SUBMENU, OPT_BIND, OPT_BUTTON, };
struct SubMenu;
struct Option { enum OptType type; const u8 *label; union { u32 *uval; bool *bval; }; union { struct { const u8 **choices; s32 numChoices; }; struct { u32 scrMin; u32 scrMax; u32 scrStep; }; struct SubMenu *nextMenu; void (*actionFn)(struct Option *, s32); }; };
struct SubMenu { struct SubMenu *prev; const u8 *label; struct Option *opts; s32 numOpts; s32 select; s32 scroll; };
#endif

//
// Options definition
//

OmmOptMenu gOmmOptMenu;
OmmOptMenu gOmmOptModels;
OmmOptMenu gOmmOptCs;
OmmOptMenu gOmmOptTimeTrials;
OmmOptMenu gOmmOptWarpToLevel;
OmmOptMenu gOmmOptWarpToCastle;
OmmOptMenu gOmmOptControls;
#if !OMM_GAME_IS_R96X
OmmOptMenu gOmmOptCheats;
#endif
OmmOptMenu gOmmOptDataManagement;
OmmOptMenu gOmmOptChangeGame;
OmmOptMenu gOmmOptExitGame;
u32 gOmmOptYesFunc[1];

#define K_NONE VK_INVALID

typedef struct {
    u32 *binds;
    const u32 defaults[MAX_BINDS];
    const bool necessary;
} OmmControlsBind;

static OmmControlsBind sOmmControlsBinds[] = {                      //  QW  /  AZ  | XBOne Con | Switch PC |
    { gOmmControlsButtonA,     { 0x0026, 0x1000, K_NONE }, true  }, //     [L]     |    (A)    |    (A)    |
    { gOmmControlsButtonB,     { 0x0033, 0x1001, K_NONE }, false }, //  [,] / [;]  |    (B)    |    (B)    |
    { gOmmControlsButtonX,     { 0x0025, 0x1002, K_NONE }, false }, //     [K]     |    (X)    |    (X)    |
    { gOmmControlsButtonY,     { 0x0032, 0x1003, K_NONE }, false }, //  [M] / [,]  |    (Y)    |    (Y)    |
    { gOmmControlsButtonStart, { 0x0039, 0x1006, K_NONE }, true  }, //   [SPACE]   |  (Start)  |    (+)    |
    { gOmmControlsButtonSpin,  { 0x1102, 0x1007, K_NONE }, false }, //    (MWB)    |   (LSB)   |   (LSB)   |
    { gOmmControlsTriggerL,    { 0x002A, 0x1009, K_NONE }, false }, //   [LSHFT]   |    (LB)   |    (L)    |
    { gOmmControlsTriggerR,    { 0x0036, 0x100A, K_NONE }, true  }, //   [RSHFT]   |    (RB)   |    (R)    |
    { gOmmControlsTriggerZ,    { 0x0018, 0x101B, K_NONE }, false }, //     [O]     |    (RT)   |    (ZR)   |
    { gOmmControlsCUp,         { 0x0148, K_NONE, K_NONE }, false }, //     [^]     |   (R-U)   |   (R-U)   |
    { gOmmControlsCDown,       { 0x0150, K_NONE, K_NONE }, false }, //     [v]     |   (R-D)   |   (R-D)   |
    { gOmmControlsCLeft,       { 0x014B, K_NONE, K_NONE }, false }, //     [<]     |   (R-L)   |   (R-L)   |
    { gOmmControlsCRight,      { 0x014D, K_NONE, K_NONE }, false }, //     [>]     |   (R-R)   |   (R-R)   |
    { gOmmControlsDUp,         { 0x000C, 0x100B, K_NONE }, false }, //  [-] / [)]  |   (D-U)   |   (D-U)   |
    { gOmmControlsDDown,       { 0x001A, 0x100C, K_NONE }, false }, //  [{] / [^]  |   (D-D)   |   (D-D)   |
    { gOmmControlsDLeft,       { 0x0019, 0x100D, K_NONE }, false }, //     [P]     |   (D-L)   |   (D-L)   |
    { gOmmControlsDRight,      { 0x001B, 0x100E, K_NONE }, false }, //  [}] / [$]  |   (D-R)   |   (D-R)   |
    { gOmmControlsStickUp,     { 0x0011, K_NONE, K_NONE }, false }, //  [W] / [Z]  |   (L-U)   |   (L-U)   |
    { gOmmControlsStickDown,   { 0x001F, K_NONE, K_NONE }, false }, //     [S]     |   (L-D)   |   (L-D)   |
    { gOmmControlsStickLeft,   { 0x001E, K_NONE, K_NONE }, false }, //  [A] / [Q]  |   (L-L)   |   (L-L)   |
    { gOmmControlsStickRight,  { 0x0020, K_NONE, K_NONE }, false }, //     [D]     |   (L-R)   |   (L-R)   |
};
const u32 NO_BIND[MAX_BINDS] = { K_NONE, K_NONE, K_NONE };

DEFINE_KBINDS(gOmmControlsButtonA,     0x0026, 0x1000, K_NONE);
DEFINE_KBINDS(gOmmControlsButtonB,     0x0033, 0x1001, K_NONE);
DEFINE_KBINDS(gOmmControlsButtonX,     0x0025, 0x1002, K_NONE);
DEFINE_KBINDS(gOmmControlsButtonY,     0x0032, 0x1003, K_NONE);
DEFINE_KBINDS(gOmmControlsButtonStart, 0x0039, 0x1006, K_NONE);
DEFINE_KBINDS(gOmmControlsButtonSpin,  0x1102, 0x1007, K_NONE);
DEFINE_KBINDS(gOmmControlsTriggerL,    0x002A, 0x1009, K_NONE);
DEFINE_KBINDS(gOmmControlsTriggerR,    0x0036, 0x100A, K_NONE);
DEFINE_KBINDS(gOmmControlsTriggerZ,    0x0018, 0x101B, K_NONE);
DEFINE_KBINDS(gOmmControlsCUp,         0x0148, K_NONE, K_NONE);
DEFINE_KBINDS(gOmmControlsCDown,       0x0150, K_NONE, K_NONE);
DEFINE_KBINDS(gOmmControlsCLeft,       0x014B, K_NONE, K_NONE);
DEFINE_KBINDS(gOmmControlsCRight,      0x014D, K_NONE, K_NONE);
DEFINE_KBINDS(gOmmControlsDUp,         0x000C, 0x100B, K_NONE);
DEFINE_KBINDS(gOmmControlsDDown,       0x001A, 0x100C, K_NONE);
DEFINE_KBINDS(gOmmControlsDLeft,       0x0019, 0x100D, K_NONE);
DEFINE_KBINDS(gOmmControlsDRight,      0x001B, 0x100E, K_NONE);
DEFINE_KBINDS(gOmmControlsStickUp,     0x0011, K_NONE, K_NONE);
DEFINE_KBINDS(gOmmControlsStickDown,   0x001F, K_NONE, K_NONE);
DEFINE_KBINDS(gOmmControlsStickLeft,   0x001E, K_NONE, K_NONE);
DEFINE_KBINDS(gOmmControlsStickRight,  0x0020, K_NONE, K_NONE);
#if !OMM_GAME_IS_R96X
DEFINE_TOGGLE(gOmmCheatEnable, false);                                          // Disabled
DEFINE_TOGGLE(gOmmCheatMoonJump, false);                                        // Disabled
DEFINE_TOGGLE(gOmmCheatGodMode, false);                                         // Disabled
DEFINE_TOGGLE(gOmmCheatInvincible, false);                                      // Disabled
DEFINE_TOGGLE(gOmmCheatSuperSpeed, false);                                      // Disabled
DEFINE_TOGGLE(gOmmCheatSuperResponsive, false);                                 // Disabled
DEFINE_TOGGLE(gOmmCheatNoFallDamage, false);                                    // Disabled
DEFINE_TOGGLE(gOmmCheatCapModifier, false);                                     // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnLava, false);                                      // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnQuicksand, false);                                 // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnWater, false);                                     // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnGas, false);                                       // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnSlope, false);                                     // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnDeathBarrier, false);                              // Disabled
DEFINE_TOGGLE(gOmmCheatBljAnywhere, false);                                     // Disabled
#endif
DEFINE_CHOICE(gOmmFrameRate, OMM_FPS_AUTO, 4);                                  // Auto
#if OMM_CODE_DEBUG
DEFINE_CHOICE(gOmmShowFPS, 0, 4);                                               // Disabled
#else
DEFINE_TOGGLE(gOmmShowFPS, false);                                              // Disabled
#endif
DEFINE_CHOICE(gOmmTextureCaching, OMM_TEXTURE_CACHING_PERMANENT, 3);            // Permanent
DEFINE_TOGGLE(gOmmModelPackCaching, true);                                      // Enabled
DEFINE_CHOICE(gOmmHudMode, OMM_HUD_MODE_VANISHING, 4);                          // Vanishing
DEFINE_SCROLL(gOmmHudSize, OMM_RENDER_GLYPH_SIZE_DEFAULT);                      // 10
DEFINE_CHOICE(gOmmCameraInvert1stPerson, OMM_CAMERA_INVERT_NONE, 4);            // Disabled
DEFINE_CHOICE(gOmmCameraInvert3rdPerson, OMM_CAMERA_INVERT_NONE, 4);            // Disabled
DEFINE_TOGGLE(gOmmCsPalettePreset, true);                                       // Enabled
DEFINE_TOGGLE(gOmmCsAnimations, true);                                          // Enabled
DEFINE_TOGGLE(gOmmCsVoices, true);                                              // Enabled
DEFINE_SCROLL(gOmmCsVoiceVolumeFactor, 100);                                    // 100%
DEFINE_TOGGLE(gOmmTimeTrialsEnabled, false);                                    // Disabled
DEFINE_TOGGLE(gOmmTimeTrialsShowStarGhosts, true);                              // Enabled
DEFINE_TOGGLE(gOmmTimeTrialsShowBowserGhosts, true);                            // Enabled
DEFINE_TOGGLE(gOmmTimeTrialsShowSlideGhosts, true);                             // Enabled
DEFINE_TOGGLE(gOmmTimeTrialsShowCoinsGhosts, true);                             // Enabled
DEFINE_TOGGLE(gOmmTimeTrialsShowAllStarsGhosts, true);                          // Enabled
DEFINE_CHOICE_SC(gOmmCharacter, 0, OMM_NUM_PLAYABLE_CHARACTERS);                // Mario
DEFINE_CHOICE_SC(gOmmMovesetType, 1, 4);                                        // Odyssey (3-Health)
DEFINE_CHOICE_SC(gOmmCapType, 2, 4);                                            // Cappy (Capture - Press)
DEFINE_CHOICE_SC(gOmmStarsMode, 1, 2);                                          // Non-Stop
DEFINE_CHOICE_SC(gOmmPowerUpsType, 1, 2);                                       // Improved
DEFINE_CHOICE_SC(gOmmCameraMode, 1, 4);                                         // 8-Dir
DEFINE_CHOICE_SC(gOmmSparklyStarsMode, 0, OMM_SPARKLY_MODE_COUNT);              // Disabled
DEFINE_CHOICE_SC(gOmmSparklyStarsHintAtLevelEntry, 0, 3);                       // Always
DEFINE_CHOICE_SC(gOmmSparklyStarsCompletionReward, 0, OMM_SPARKLY_MODE_COUNT);  // Disabled
DEFINE_TOGGLE_SC(gOmmSparklyStarsPerryCharge, true);                            // Enabled
DEFINE_TOGGLE(gOmmCheatUnlimitedCappyBounces, false);                           // Disabled
DEFINE_TOGGLE(gOmmCheatCappyStaysForever, false);                               // Disabled
DEFINE_TOGGLE(gOmmCheatHomingAttackGlobalRange, false);                         // Disabled
DEFINE_TOGGLE(gOmmCheatMarioTeleportsToCappy, false);                           // Disabled
DEFINE_TOGGLE(gOmmCheatCappyCanCollectStars, false);                            // Disabled
DEFINE_TOGGLE(gOmmCheatPlayAsCappy, false);                                     // Disabled
DEFINE_TOGGLE(gOmmCheatPeachEndlessVibeGauge, false);                           // Disabled
DEFINE_TOGGLE(gOmmCheatShadowMario, false);                                     // Disabled
DEFINE_CHOICE_SC(gOmmExtrasMarioColors, 0, /*omm_mario_colors_count()*/ 32);    // Default
DEFINE_CHOICE_SC(gOmmExtrasPeachColors, 0, /*omm_mario_colors_count()*/ 32);    // Default
DEFINE_CHOICE_SC(gOmmExtrasObjectsRadar, 1, 4);                                 // Red coins
DEFINE_TOGGLE_SC(gOmmExtrasSMOAnimations, true);                                // Enabled
DEFINE_TOGGLE_SC(gOmmExtrasCappyAndTiara, true);                                // Enabled
DEFINE_TOGGLE_SC(gOmmExtrasColoredStars, true);                                 // Enabled
DEFINE_TOGGLE_SC(gOmmExtrasRevealSecrets, false);                               // Disabled
DEFINE_TOGGLE_SC(gOmmExtrasShowStarNumber, true);                               // Enabled
DEFINE_TOGGLE_SC(gOmmExtrasInvisibleMode, false);                               // Disabled
DEFINE_TOGGLE_SC(gOmmExtrasMarioMode, false);                                   // Disabled
#if OMM_CODE_DEBUG
DEFINE_TOGGLE_SC(gOmmDebugHitbox, false);                                       // Disabled
DEFINE_TOGGLE_SC(gOmmDebugHurtbox, false);                                      // Disabled
DEFINE_TOGGLE_SC(gOmmDebugWallbox, false);                                      // Disabled
DEFINE_TOGGLE_SC(gOmmDebugSurface, false);                                      // Disabled
DEFINE_TOGGLE_SC(gOmmDebugMario, false);                                        // Disabled
DEFINE_TOGGLE_SC(gOmmDebugCappy, false);                                        // Disabled
#endif
#if OMM_CODE_DEV
#include "data/omm/dev/omm_dev_opt_define.inl"
#endif

//
// State
//

#define OMM_OPT_STATE_SPARKLY_STARS_LOCKED      (0)
#define OMM_OPT_STATE_SPARKLY_STARS_UNLOCKED    (1)
#define OMM_OPT_STATE_PEACH_UNLOCKED            (2)
#define OMM_OPT_STATE_PERRY_CHARGE_UNLOCKED     (3)
#define OMM_OPT_STATE__REDACTED__UNLOCKED       (4)
#define OMM_OPT_STATE_COUNT                     (5)

u32 omm_opt_get_state() {
    if (OMM_REWARD_IS__REDACTED__UNLOCKED) return OMM_OPT_STATE__REDACTED__UNLOCKED;
    if (OMM_REWARD_IS_PERRY_CHARGE_UNLOCKED) return OMM_OPT_STATE_PERRY_CHARGE_UNLOCKED;
    if (OMM_REWARD_IS_PLAYABLE_PEACH_UNLOCKED) return OMM_OPT_STATE_PEACH_UNLOCKED;
    if (OMM_REWARD_IS_SPARKLY_STARS_UNLOCKED) return OMM_OPT_STATE_SPARKLY_STARS_UNLOCKED;
    return OMM_OPT_STATE_SPARKLY_STARS_LOCKED;
}

//
// Shortcuts
//

typedef struct {
    enum OptType type;
    u32 *binds;
    const u8 *label;
    union {
        struct {
            bool *value;
        } toggle;
        struct {
            u32 *value;
            s32 numChoices;
            const u8 **choices;
        } choice;
    };
} OmmOptShortcut;

static OmmArray_(OmmOptShortcut *) sOmmOptShortcuts = omm_array_zero;

//
// Option wrappers
//

static struct Option omm_opt_make_toggle(s32 state, const char *label, bool *value, u32 *binds) {
    struct Option opt = {0};
    opt.type = OPT_TYPE_STATE(OPT_TOGGLE, state);
    opt.label = omm_opt_text(label);
    opt.bval = value;
    if (binds) {
        OmmOptShortcut *shortcut = mem_new(OmmOptShortcut, 1);
        shortcut->type = opt.type;
        shortcut->binds = binds;
        shortcut->label = opt.label;
        shortcut->toggle.value = opt.bval;
        omm_array_add(sOmmOptShortcuts, ptr, shortcut);
    }
    return opt;
}

static struct Option omm_opt_make_choice(s32 state, const char *label, u32 *value, const char **choices, s32 numChoices, u32 *binds) {
    struct Option opt = {0};
    opt.type = OPT_TYPE_STATE(OPT_CHOICE, state);
    opt.label = omm_opt_text(label);
    opt.uval = value;
    opt.choices = mem_new(const u8 *, numChoices);
    opt.numChoices = numChoices;
    for (s32 i = 0; i != numChoices; ++i) {
        opt.choices[i] = omm_opt_text(choices[i]);
    }
    if (binds) {
        OmmOptShortcut *shortcut = mem_new(OmmOptShortcut, 1);
        shortcut->type = opt.type;
        shortcut->binds = binds;
        shortcut->label = opt.label;
        shortcut->choice.value = opt.uval;
        shortcut->choice.choices = opt.choices;
        shortcut->choice.numChoices = opt.numChoices;
        omm_array_add(sOmmOptShortcuts, ptr, shortcut);
    }
    return opt;
}

static struct Option omm_opt_make_scroll(s32 state, const char *label, u32 *value, u32 min, u32 max, u32 step) {
    struct Option opt = {0};
    opt.type = OPT_TYPE_STATE(OPT_SCROLL, state);
    opt.label = omm_opt_text(label);
    opt.uval = value;
    opt.scrMin = min;
    opt.scrMax = max;
    opt.scrStep = step;
    return opt;
}

static struct Option omm_opt_make_bind(s32 state, const char *label, u32 *binds) {
    struct Option opt = {0};
    opt.type = OPT_TYPE_STATE(OPT_BIND, state);
    opt.label = omm_opt_text(label);
    opt.uval = binds;
    return opt;
}

static struct Option omm_opt_make_button(s32 state, const char *label, void (*actionFn)(struct Option *, s32)) {
    struct Option opt = {0};
    opt.type = OPT_TYPE_STATE(OPT_BUTTON, state);
    opt.label = omm_opt_text(label);
    opt.actionFn = actionFn;
    return opt;
}

static struct Option omm_opt_make_submenu(s32 state, const char *label, const char *title, struct Option *options, s32 numOptions) {
    struct Option opt = {0};
    opt.type = OPT_TYPE_STATE(OPT_SUBMENU, state);
    opt.label = omm_opt_text(label);
    opt.nextMenu = mem_new(struct SubMenu, 1);
    opt.nextMenu->label = omm_opt_text(title);
    opt.nextMenu->opts = mem_new(struct Option, numOptions);
    opt.nextMenu->numOpts = numOptions;
    for (s32 i = 0; i != numOptions; ++i) {
        opt.nextMenu->opts[i] = options[i];
    }
    return opt;
}

static struct Option omm_opt_make_yes_no(s32 state, const char *label, const char *ask, void (*actionFnYes)(struct Option *, s32)) {
    struct Option opt = omm_opt_make_submenu(
        state, label, ask, array_of(struct Option) {
            omm_opt_make_button(state, OMM_TEXT_OPT_NO, omm_opt_return_to_previous_menu),
            omm_opt_make_button(state, OMM_TEXT_OPT_YES, actionFnYes),
        }, 2
    );
    opt.nextMenu->opts[1].uval = gOmmOptYesFunc;
    return opt;
}

static struct Option omm_opt_make_shortcuts_submenu(const char *label, const char *title, s32 numShortcuts) {
    struct Option opt = {0};
    opt.type = OPT_SUBMENU;
    opt.label = omm_opt_text(label);
    opt.nextMenu = mem_new(struct SubMenu, 1);
    opt.nextMenu->label = omm_opt_text(title);
    opt.nextMenu->opts = mem_new(struct Option, numShortcuts + 1);
    opt.nextMenu->numOpts = numShortcuts + 1;
    omm_array_for_each(sOmmOptShortcuts, p_shortcut) {
        OmmOptShortcut *shortcut = p_shortcut->as_ptr;
        struct Option *shortcutOpt = &opt.nextMenu->opts[i_p_shortcut];
        shortcutOpt->type = OPT_TYPE_STATE(OPT_BIND, OPT_STATE(shortcut->type));
        shortcutOpt->label = shortcut->label;
        shortcutOpt->uval = shortcut->binds;
    }
    return opt;
}

#if OMM_GAME_IS_SM74

//
// Super Mario 74 code
//

#define OMM_OPTIONS_C
#include "data/omm/system/omm_sm74.inl"
#undef OMM_OPTIONS_C

#else

//
// Warp to level (init)
//

typedef struct { u32 levelValue; union { s32 areaIndex; s32 actNum; }; } OmmOptWarpData;

static struct {
    OmmArray_(s32) levels;
    OmmArray_(OmmOptWarpData *) areas;
    OmmArray_(OmmOptWarpData *) acts;
    u32 prevLevel, currLevel;
    u32 prevArea, currArea;
    u32 prevAct, currAct;
} sOmmWarp[1];

static void omm_opt_init_warp_to_level() {
    for (s32 i = 0; i != omm_level_get_count() - 3; ++i) {
        s32 levelNum = omm_level_get_list()[i];
        if (omm_level_can_warp(levelNum)) {

            // Areas
            s32 areas = omm_level_get_areas(levelNum);
            for (s32 areaIndex = 0; areaIndex != 8; ++areaIndex) {
                if (((areas >> areaIndex) & 1) && omm_level_get_entry_warp(levelNum, areaIndex)) {

                    // Register level
                    if (omm_array_find(sOmmWarp->levels, s32, levelNum) == -1) {
                        omm_array_add(sOmmWarp->levels, s32, levelNum);
                    }

                    // Register area
                    OmmOptWarpData *area = mem_new(OmmOptWarpData, 1);
                    area->levelValue = omm_array_count(sOmmWarp->levels) - 1;
                    area->areaIndex = areaIndex;
                    omm_array_add(sOmmWarp->areas, ptr, area);
                }
            }

            // Acts
            s32 levelValue = omm_array_find(sOmmWarp->levels, s32, levelNum);
            if (levelValue != -1) {
                s32 starLevelFlags = omm_stars_get_level_flags(levelNum, OMM_GAME_MODE);

                // Level with stars or Bowser fight
                if (starLevelFlags != 0 || OMM_LEVEL_IS_BOWSER_FIGHT(levelNum)) {
                    s32 courseNum = omm_level_get_course(levelNum);

                    // Main course
                    if (COURSE_IS_MAIN_COURSE(courseNum)) {
                        for (s32 actIndex = 0; actIndex != OMM_NUM_ACTS_MAX_PER_COURSE; ++actIndex) {
                            if ((starLevelFlags >> actIndex) & 1) {
                                OmmOptWarpData *act = mem_new(OmmOptWarpData, 1);
                                act->levelValue = levelValue;
                                act->actNum = actIndex + 1;
                                omm_array_add(sOmmWarp->acts, ptr, act);
                            }
                        }
                    }

                    // Bonus course
                    else {
                        OmmOptWarpData *act = mem_new(OmmOptWarpData, 1);
                        act->levelValue = levelValue;
                        act->actNum = -1;
                        omm_array_add(sOmmWarp->acts, ptr, act);
                    }
                }

                // No star, remove level from the list
                else {
                    omm_array_remove(sOmmWarp->levels, levelValue);
                }
            }
        }
    }
}

static struct Option omm_opt_make_choice_level(const char *label, u32 *value) {
    struct Option opt = {0};
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(label);
    opt.uval = value;
    opt.numChoices = omm_array_count(sOmmWarp->levels);
    const u8 **choices = opt.choices = mem_new(const u8 *, opt.numChoices);
    omm_array_for_each(sOmmWarp->levels, p_levelNum) {
        s32 levelNum = p_levelNum->as_s32;
#if OMM_GAME_IS_R96X
        str_t name;
        str_fmt(name, sizeof(name), "%s_%02d", OMM_TEXT_OPT_WARP_TO_LEVEL_COURSE_NAME, levelNum);
#else
        ustr_t name;
        omm_level_get_course_name(name, levelNum, OMM_GAME_MODE, true, true);
#endif
        *(choices++) = mem_dup(name, omm_opt_text_length(name) + 1);
    }
    return opt;
}

static struct Option omm_opt_make_choice_area(const char *label, u32 *value) {
    struct Option opt = {0};
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(label);
    opt.uval = value;
    opt.numChoices = omm_array_count(sOmmWarp->areas);
    const u8 **choices = opt.choices = mem_new(const u8 *, opt.numChoices);
    omm_array_for_each(sOmmWarp->areas, p_area) {
        OmmOptWarpData *area = (OmmOptWarpData *) p_area->as_ptr;
#if OMM_GAME_IS_R96X
        str_t name;
        str_fmt(name, sizeof(name), "%s_%01d", OMM_TEXT_OPT_WARP_TO_LEVEL_AREA_NAME, area->areaIndex);
        *(choices++) = mem_dup(name, strlen(name) + 1);
#else
        str_t name;
        str_fmt(name, sizeof(name), "%s %d", OMM_TEXT_OPT_WARP_TO_LEVEL_AREA, area->areaIndex);
        *(choices++) = omm_text_convert(name, true);
#endif
    }
    return opt;
}

static struct Option omm_opt_make_choice_act(const char *label, u32 *value) {
    struct Option opt = {0};
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(label);
    opt.uval = value;
    opt.numChoices = omm_array_count(sOmmWarp->acts);
    const u8 **choices = opt.choices = mem_new(const u8 *, opt.numChoices);
    omm_array_for_each(sOmmWarp->acts, p_act) {
        OmmOptWarpData *act = (OmmOptWarpData *) p_act->as_ptr;
        s32 levelNum = omm_array_get(sOmmWarp->levels, s32, act->levelValue);
#if OMM_GAME_IS_R96X
        str_t name;
        str_fmt(name, sizeof(name), "%s_%02d_%02d", OMM_TEXT_OPT_WARP_TO_LEVEL_ACT_NAME, levelNum, act->actNum);
#else
        ustr_t name;
        omm_level_get_act_name(name, levelNum, act->actNum, OMM_GAME_MODE, true, true);
#endif
        *(choices++) = mem_dup(name, omm_opt_text_length(name) + 1);
    }
    return opt;
}

//
// Warp to level (update)
//

static u32 omm_opt_get_first_area_value(u32 levelValue) {
    omm_array_for_each(sOmmWarp->areas, p_area) {
        OmmOptWarpData *area = (OmmOptWarpData *) p_area->as_ptr;
        if (area->levelValue == levelValue) {
            return i_p_area;
        }
    }
    return 0;
}

static u32 omm_opt_get_first_act_value(u32 levelValue) {
    omm_array_for_each(sOmmWarp->acts, p_act) {
        OmmOptWarpData *act = (OmmOptWarpData *) p_act->as_ptr;
        if (act->levelValue == levelValue) {
            return i_p_act;
        }
    }
    return 0;
}

static u32 omm_opt_get_level_value_from_area_value(u32 areaValue) {
    return ((OmmOptWarpData *) omm_array_get(sOmmWarp->areas, ptr, areaValue))->levelValue;
}

static u32 omm_opt_get_level_value_from_act_value(u32 actValue) {
    return ((OmmOptWarpData *) omm_array_get(sOmmWarp->acts, ptr, actValue))->levelValue;
}

static s32 omm_opt_get_level_num(u32 levelValue) {
    return omm_array_get(sOmmWarp->levels, s32, levelValue);
}

static s32 omm_opt_get_area_index(u32 areaValue) {
    return ((OmmOptWarpData *) omm_array_get(sOmmWarp->areas, ptr, areaValue))->areaIndex;
}

static s32 omm_opt_get_act_num(u32 actValue) {
    return max_s(1, ((OmmOptWarpData *) omm_array_get(sOmmWarp->acts, ptr, actValue))->actNum);
}

static void omm_opt_update_warp_to_level() {

    // Level changed
    if (sOmmWarp->prevLevel != sOmmWarp->currLevel) {
        sOmmWarp->currArea = omm_opt_get_first_area_value(sOmmWarp->currLevel);
        sOmmWarp->currAct = omm_opt_get_first_act_value(sOmmWarp->currLevel);
    }

    // Area changed
    else if (sOmmWarp->prevArea != sOmmWarp->currArea) {
        sOmmWarp->currLevel = omm_opt_get_level_value_from_area_value(sOmmWarp->currArea);
        if (sOmmWarp->prevLevel != sOmmWarp->currLevel) {
            sOmmWarp->currAct = omm_opt_get_first_act_value(sOmmWarp->currLevel);
        }
    }

    // Act changed
    else if (sOmmWarp->prevAct != sOmmWarp->currAct) {
        sOmmWarp->currLevel = omm_opt_get_level_value_from_act_value(sOmmWarp->currAct);
        if (sOmmWarp->prevLevel != sOmmWarp->currLevel) {
            sOmmWarp->currArea = omm_opt_get_first_area_value(sOmmWarp->currLevel);
        }
    }

    // Update values
    sOmmWarp->prevLevel = sOmmWarp->currLevel;
    sOmmWarp->prevArea = sOmmWarp->currArea;
    sOmmWarp->prevAct = sOmmWarp->currAct;
}

static void omm_opt_warp_to_level(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        s32 levelNum = omm_opt_get_level_num(sOmmWarp->currLevel);
        s32 areaIndex = omm_opt_get_area_index(sOmmWarp->currArea);
        s32 actNum = omm_opt_get_act_num(sOmmWarp->currAct);
        if (omm_is_main_menu() || !omm_warp_to_level(levelNum, areaIndex, actNum)) {
            play_buzz_sound();
        }
    }
}

static void omm_opt_warp_to_castle(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        s32 levelNum = omm_opt_get_level_num(sOmmWarp->currLevel);
        if (omm_is_main_menu() || !omm_exit_level(levelNum, 1, true)) {
            play_buzz_sound();
        }
    }
}

#endif

//
// Change game
//

typedef struct { const char *code; const char *label; struct Option *opt; } OmmOptGame;
static OmmOptGame sOmmOptGames[] = {
#define OMM_GAME(code, suffix, ...) { code, __EXPAND(CAT(OMM_TEXT_OPT_CHANGE_GAME_, suffix)), NULL },
#include "data/omm/omm_games.inl"
#undef OMM_GAME
};

static const char *get_exe_path(sys_path_t dst, const char *gameCode) {
#if WINDOWS_BUILD
    snprintf(dst, sizeof(sys_path_t), "%s/sm64.%s.exe", sys_exe_path(), gameCode);
#else
    snprintf(dst, sizeof(sys_path_t), "%s/sm64.%s", sys_exe_path(), gameCode);
#endif
    return dst;
}

static void omm_opt_change_game(struct Option *opt, s32 arg) {
    if (!arg) {
        for (u32 i = 0; i != array_length(sOmmOptGames); ++i) {
            const OmmOptGame *game = &sOmmOptGames[i];
            if (opt == game->opt) {
                sys_path_t exePath;
                launch_game(get_exe_path(exePath, game->code));
                gOmmGlobals->configNoSave = true;
                game_exit();
            }
        }
    }
}

static struct Option omm_opt_init_change_game() {
    struct Option optGames[array_length(sOmmOptGames)] = {0};
    struct Option optChangeGame = omm_opt_make_submenu(0, OMM_TEXT_OPT_CHANGE_GAME_LABEL, OMM_TEXT_OPT_CHANGE_GAME_TITLE, optGames, array_length(sOmmOptGames));
    for (u32 i = 0, j = 0; i != array_length(sOmmOptGames); ++i) {
        OmmOptGame *game = &sOmmOptGames[i];
        sys_path_t exePath;
        if (strcmp(game->code, OMM_GAME_CODE) != 0 && fs_sys_file_exists(get_exe_path(exePath, game->code))) {
            optChangeGame.nextMenu->opts[j] = omm_opt_make_yes_no(0, game->label, OMM_TEXT_OPT_CHANGE_GAME_ASK, omm_opt_change_game);
            game->opt = &optChangeGame.nextMenu->opts[j].nextMenu->opts[1];
            j++;
        } else {
            optChangeGame.nextMenu->numOpts--;
        }
    }
    return optChangeGame;
}

//
// Buttons
//

static void omm_opt_clear_shortcuts(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        omm_array_for_each(sOmmOptShortcuts, p_shortcut) {
            OmmOptShortcut *shortcut = p_shortcut->as_ptr;
            mem_cpy(shortcut->binds, NO_BIND, sizeof(NO_BIND));
        }
    }
}

static void omm_opt_disable_all_model_packs(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        omm_models_disable_all();
    }
}

void omm_opt_camera_enter_snapshot_mode(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        if (!omm_camera_snapshot_mode_init()) {
            play_buzz_sound();
        }
    }
}

static void omm_opt_reset_controls(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        array_for_each_(OmmControlsBind, bind, sOmmControlsBinds) {
            mem_cpy(bind->binds, bind->defaults, sizeof(bind->defaults));
        }
    }
}

static void omm_opt_reset_mario_palettes(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        omm_mario_colors_reset(false);
        omm_save_file_do_save();
    }
}

static void omm_opt_reset_peach_palettes(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        omm_mario_colors_reset(true);
        omm_save_file_do_save();
    }
}

static void omm_opt_delete_sparkly_stars_data(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        omm_sparkly_clear_all();
        omm_save_file_do_save();
    }
}

static void omm_opt_delete_global_stats(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        omm_stats_reset(gOmmStats);
        omm_save_file_do_save();
    }
}

static void omm_opt_delete_secrets(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        omm_secrets_reset();
        omm_save_file_do_save();
    }
}

void omm_opt_return_to_main_menu(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        if (!omm_is_main_menu()) {
            omm_return_to_main_menu();
        } else {
            play_buzz_sound();
        }
    }
}

static void omm_opt_exit_game(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        game_exit();
    }
}

//
// Init
//

#define choices(...) (array_of(const char *) { __VA_ARGS__ })
#define options(...) (array_of(struct Option) { __VA_ARGS__ })

static struct Option omm_opt_make_main_menu() {

    // Character
    struct Option optCharacter = omm_opt_make_choice(0, OMM_TEXT_OPT_CHARACTER_LABEL, &gOmmCharacter, choices(
        OMM_TEXT_MARIO,
        OMM_TEXT_PEACH,
        OMM_TEXT_LUIGI,
        OMM_TEXT_WARIO),
    gOmmCharacterCount, gOmmCharacterShortcuts);

    // Moveset
    struct Option optMoveset = omm_opt_make_choice(0, OMM_TEXT_OPT_MOVESET_LABEL, &gOmmMovesetType, choices(
        OMM_TEXT_OPT_MOVESET_CLASSIC,
        OMM_TEXT_OPT_MOVESET_ODYSSEY_3H,
        OMM_TEXT_OPT_MOVESET_ODYSSEY_6H,
        OMM_TEXT_OPT_MOVESET_ODYSSEY_1H),
    gOmmMovesetTypeCount, gOmmMovesetTypeShortcuts);

    // Cap
    struct Option optCap = omm_opt_make_choice(0, OMM_TEXT_OPT_CAP_LABEL, &gOmmCapType, choices(
        OMM_TEXT_OPT_CAP_CLASSIC,
        OMM_TEXT_OPT_CAP_NO_CAPTURE,
        OMM_TEXT_OPT_CAP_CAPTURE_PRESS,
        OMM_TEXT_OPT_CAP_CAPTURE_HOLD),
    gOmmCapTypeCount, gOmmCapTypeShortcuts);

    // Stars
    struct Option optStars = omm_opt_make_choice(0, OMM_TEXT_OPT_STARS_LABEL, &gOmmStarsMode, choices(
        OMM_TEXT_OPT_STARS_CLASSIC,
        OMM_TEXT_OPT_STARS_NON_STOP),
    gOmmStarsModeCount, gOmmStarsModeShortcuts);

    // Power-ups
    struct Option optPowerUps = omm_opt_make_choice(0, OMM_TEXT_OPT_POWER_UPS_LABEL, &gOmmPowerUpsType, choices(
        OMM_TEXT_OPT_POWER_UPS_CLASSIC,
        OMM_TEXT_OPT_POWER_UPS_IMPROVED),
    gOmmPowerUpsTypeCount, gOmmPowerUpsTypeShortcuts);

    // Camera
    struct Option optCamera = omm_opt_make_choice(0, OMM_TEXT_OPT_CAMERA_LABEL, &gOmmCameraMode, choices(
        OMM_TEXT_OPT_CAMERA_CLASSIC,
        OMM_TEXT_OPT_CAMERA_8_DIR,
        OMM_TEXT_OPT_CAMERA_16_DIR,
        OMM_TEXT_OPT_CAMERA_INF_DIR),
    gOmmCameraModeCount, gOmmCameraModeShortcuts);

    // Sparkly Stars
    struct Option optSparklyStars = omm_opt_make_submenu(OMM_OPT_STATE_SPARKLY_STARS_UNLOCKED, OMM_TEXT_OPT_SPARKLY_STARS_LABEL, OMM_TEXT_OPT_SPARKLY_STARS_TITLE, options(
        omm_opt_make_choice(OMM_OPT_STATE_SPARKLY_STARS_UNLOCKED, OMM_TEXT_OPT_SPARKLY_STARS_MODE, &gOmmSparklyStarsMode, choices(
            OMM_TEXT_OPT_SPARKLY_STARS_MODE_DISABLED,
            OMM_TEXT_OPT_SPARKLY_STARS_MODE_NORMAL,
            OMM_TEXT_OPT_SPARKLY_STARS_MODE_HARD,
            OMM_TEXT_OPT_SPARKLY_STARS_MODE_LUNATIC),
        gOmmSparklyStarsModeCount, gOmmSparklyStarsModeShortcuts),
        omm_opt_make_choice(OMM_OPT_STATE_SPARKLY_STARS_UNLOCKED, OMM_TEXT_OPT_SPARKLY_STARS_HINT, &gOmmSparklyStarsHintAtLevelEntry, choices(
            OMM_TEXT_OPT_SPARKLY_STARS_HINT_ALWAYS,
            OMM_TEXT_OPT_SPARKLY_STARS_HINT_NOT_COLLECTED,
            OMM_TEXT_OPT_SPARKLY_STARS_HINT_NEVER),
        gOmmSparklyStarsHintAtLevelEntryCount, gOmmSparklyStarsHintAtLevelEntryShortcuts),
        omm_opt_make_choice(OMM_OPT_STATE_PEACH_UNLOCKED, OMM_TEXT_OPT_SPARKLY_STARS_COMPLETION_REWARD, &gOmmSparklyStarsCompletionReward, choices(
            OMM_TEXT_OPT_SPARKLY_STARS_COMPLETION_REWARD_DISABLED,
            OMM_TEXT_OPT_SPARKLY_STARS_COMPLETION_REWARD_NORMAL,
            OMM_TEXT_OPT_SPARKLY_STARS_COMPLETION_REWARD_HARD,
            OMM_TEXT_OPT_SPARKLY_STARS_COMPLETION_REWARD_LUNATIC),
        gOmmSparklyStarsCompletionRewardCount, gOmmSparklyStarsCompletionRewardShortcuts),
        omm_opt_make_toggle(OMM_OPT_STATE_PERRY_CHARGE_UNLOCKED, OMM_TEXT_OPT_SPARKLY_STARS_PERRY_CHARGE, &gOmmSparklyStarsPerryCharge, gOmmSparklyStarsPerryChargeShortcuts),
    ), 4);

    // Cheats
    struct Option optCheats = omm_opt_make_submenu(0, OMM_TEXT_OPT_CHEATS_LABEL, OMM_TEXT_OPT_CHEATS_TITLE, options(
        omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_UNLIMITED_CAPPY_BOUNCES, &gOmmCheatUnlimitedCappyBounces, NULL),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_CAPPY_STAYS_FOREVER, &gOmmCheatCappyStaysForever, NULL),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_GLOBAL_HOMING_ATTACK_RANGE, &gOmmCheatHomingAttackGlobalRange, NULL),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_MARIO_TELEPORTS_TO_CAPPY, &gOmmCheatMarioTeleportsToCappy, NULL),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_CAPPY_CAN_COLLECT_STARS, &gOmmCheatCappyCanCollectStars, NULL),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_PLAY_AS_CAPPY, &gOmmCheatPlayAsCappy, NULL),
        omm_opt_make_toggle(OMM_OPT_STATE_PEACH_UNLOCKED, OMM_TEXT_OPT_CHEAT_PEACH_ENDLESS_VIBE_GAUGE, &gOmmCheatPeachEndlessVibeGauge, NULL),
        omm_opt_make_toggle(OMM_OPT_STATE_PEACH_UNLOCKED, OMM_TEXT_OPT_CHEAT_SHADOW_MARIO, &gOmmCheatShadowMario, NULL),
    ), 8);

    // Extras
    struct Option optExtras = omm_opt_make_submenu(0, OMM_TEXT_OPT_EXTRAS_LABEL, OMM_TEXT_OPT_EXTRAS_TITLE, options(
        omm_opt_make_choice(0, OMM_TEXT_OPT_MARIO_COLORS, &gOmmExtrasMarioColors, omm_mario_colors_choices(false), gOmmExtrasMarioColorsCount, gOmmExtrasMarioColorsShortcuts),
        omm_opt_make_choice(OMM_OPT_STATE_PEACH_UNLOCKED, OMM_TEXT_OPT_PEACH_COLORS, &gOmmExtrasPeachColors, omm_mario_colors_choices(true), gOmmExtrasPeachColorsCount, gOmmExtrasPeachColorsShortcuts),
        omm_opt_make_choice(0, OMM_TEXT_OPT_OBJECTS_RADAR, &gOmmExtrasObjectsRadar, choices(
            OMM_TEXT_OPT_OBJECTS_RADAR_DISABLED,
            OMM_TEXT_OPT_OBJECTS_RADAR_RED_COINS,
            OMM_TEXT_OPT_OBJECTS_RADAR_SECRETS,
            OMM_TEXT_OPT_OBJECTS_RADAR_STARS),
        gOmmExtrasObjectsRadarCount, gOmmExtrasObjectsRadarShortcuts),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_SMO_ANIMATIONS, &gOmmExtrasSMOAnimations, gOmmExtrasSMOAnimationsShortcuts),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_CAPPY_AND_TIARA, &gOmmExtrasCappyAndTiara, gOmmExtrasCappyAndTiaraShortcuts),
#if !OMM_GAME_IS_SMMS
        omm_opt_make_toggle(0, OMM_TEXT_OPT_COLORED_STARS, &gOmmExtrasColoredStars, gOmmExtrasColoredStarsShortcuts),
#endif
#if !OMM_GAME_IS_SMGS
        omm_opt_make_toggle(0, OMM_TEXT_OPT_REVEAL_SECRETS, &gOmmExtrasRevealSecrets, gOmmExtrasRevealSecretsShortcuts),
#endif
        omm_opt_make_toggle(0, OMM_TEXT_OPT_SHOW_STAR_NUMBER, &gOmmExtrasShowStarNumber, gOmmExtrasShowStarNumberShortcuts),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_INVISIBLE_MODE, &gOmmExtrasInvisibleMode, gOmmExtrasInvisibleModeShortcuts),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_MARIO_MODE, &gOmmExtrasMarioMode, gOmmExtrasMarioModeShortcuts),
    ), 8 + !OMM_GAME_IS_SMMS + !OMM_GAME_IS_SMGS);

#if OMM_CODE_DEBUG
    // Debug
    struct Option optDebug = omm_opt_make_submenu(0, OMM_TEXT_OPT_DEBUG_LABEL, OMM_TEXT_OPT_DEBUG_TITLE, options(
        omm_opt_make_toggle(0, OMM_TEXT_OPT_DEBUG_HITBOX, &gOmmDebugHitbox, gOmmDebugHitboxShortcuts),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_DEBUG_HURTBOX, &gOmmDebugHurtbox, gOmmDebugHurtboxShortcuts),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_DEBUG_WALLBOX, &gOmmDebugWallbox, gOmmDebugWallboxShortcuts),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_DEBUG_SURFACE, &gOmmDebugSurface, gOmmDebugSurfaceShortcuts),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_DEBUG_MARIO, &gOmmDebugMario, gOmmDebugMarioShortcuts),
        omm_opt_make_toggle(0, OMM_TEXT_OPT_DEBUG_CAPPY, &gOmmDebugCappy, gOmmDebugCappyShortcuts),
    ), 6);
#endif

    // Shortcuts
    s32 numShortcuts = omm_array_count(sOmmOptShortcuts);
    struct Option optShortcuts = omm_opt_make_shortcuts_submenu(OMM_TEXT_OPT_SHORTCUTS_LABEL, OMM_TEXT_OPT_SHORTCUTS_TITLE, numShortcuts);
    optShortcuts.nextMenu->opts[numShortcuts] = omm_opt_make_yes_no(0, OMM_TEXT_OPT_SHORTCUTS_CLEAR, OMM_TEXT_OPT_SHORTCUTS_CLEAR_ASK, omm_opt_clear_shortcuts);

#if OMM_CODE_DEV
#include "data/omm/dev/omm_dev_opt_make.inl"
#endif

    // OMM menu
    struct Option optOmmMenu = omm_opt_make_submenu(0, OMM_TEXT_OPT_MENU_LABEL, OMM_TEXT_OPT_MENU_TITLE, options(
        optCharacter,
        optMoveset,
        optCap,
        optStars,
        optPowerUps,
        optCamera,
        optSparklyStars,
        optCheats,
        optExtras,
        optShortcuts,
#if OMM_CODE_DEBUG
        optDebug,
#endif
#if OMM_CODE_DEV
        optDev,
#endif
    ), 10 + OMM_CODE_DEBUG + OMM_CODE_DEV);
    return optOmmMenu;
}

OMM_AT_STARTUP static void omm_opt_init() {
    fs_init(sys_ropaths, NULL, NULL);
    extern void omm_data_init();
    omm_data_init();
    omm_save_file_load_all();
    omm_player_select(gOmmCharacter);

    // OMM sub-menu
    struct Option optOmmMenu = omm_opt_make_main_menu();
    gOmmOptMenu.label = mem_dup(optOmmMenu.label, omm_opt_text_length(optOmmMenu.label) + 1);
    gOmmOptMenu.subMenu = mem_dup(optOmmMenu.nextMenu, sizeof(struct SubMenu));

    // Model packs sub-menu
    const void **packs = omm_models_init();
    if (packs) {
        const char **packNames = (const char **) packs[0];
        bool **packToggles = (bool **) packs[1];
        s32 numPacks = 0; for (const char **p = packNames; *p; ++p, ++numPacks);
        struct Option *optPacks = mem_new(struct Option, numPacks + 1);
        for (s32 i = 0; i != numPacks; ++i) {
            optPacks[i] = omm_opt_make_toggle(0, packNames[i], packToggles[i], NULL);
        }
        optPacks[numPacks] = omm_opt_make_button(0, OMM_TEXT_OPT_MODELS_DISABLE_ALL, omm_opt_disable_all_model_packs);
        struct Option optModels = omm_opt_make_submenu(0, OMM_TEXT_OPT_MODELS_LABEL, OMM_TEXT_OPT_MODELS_TITLE, optPacks, numPacks + 1);
        gOmmOptModels.label = (const u8 *) OMM_TEXT_OPT_MODELS_LABEL;
        gOmmOptModels.subMenu = mem_dup(optModels.nextMenu, sizeof(struct SubMenu));
        mem_del(packNames);
        mem_del(packToggles);
        mem_del(packs);
    } else {
        gOmmOptModels.label = NULL;
        gOmmOptModels.subMenu = NULL;
    }

    // Character Select sub-menu
    if (omm_models_cs_get_size(1)) {
        struct Option optCs =
            omm_opt_make_submenu(0, OMM_TEXT_OPT_CS_LABEL, OMM_TEXT_OPT_CS_TITLE, options(
                omm_opt_make_toggle(0, OMM_TEXT_OPT_CS_PALETTE_PRESET, &gOmmCsPalettePreset, NULL),
                omm_opt_make_toggle(0, OMM_TEXT_OPT_CS_ANIMATIONS, &gOmmCsAnimations, NULL),
                omm_opt_make_toggle(0, OMM_TEXT_OPT_CS_VOICES, &gOmmCsVoices, NULL),
                omm_opt_make_scroll(0, OMM_TEXT_OPT_CS_VOICE_VOLUME_FACTOR, &gOmmCsVoiceVolumeFactor, 0, 200, 5),
            ), 4);
        gOmmOptCs.label = mem_dup(optCs.label, omm_opt_text_length(optCs.label) + 1);
        gOmmOptCs.subMenu = mem_dup(optCs.nextMenu, sizeof(struct SubMenu));
    } else {
        gOmmOptCs.label = NULL;
        gOmmOptCs.subMenu = NULL;
    }

    // Time Trials sub-menu
    struct Option optTimeTrials =
        omm_opt_make_submenu(0, OMM_TEXT_OPT_TIME_TRIALS_LABEL, OMM_TEXT_OPT_TIME_TRIALS_TITLE, options(
            omm_opt_make_toggle(0, OMM_TEXT_OPT_TIME_TRIALS_TOGGLE, &gOmmTimeTrialsEnabled, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_TIME_TRIALS_SHOW_STAR_GHOSTS, &gOmmTimeTrialsShowStarGhosts, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_TIME_TRIALS_SHOW_BOWSER_GHOSTS, &gOmmTimeTrialsShowBowserGhosts, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_TIME_TRIALS_SHOW_SLIDE_GHOSTS, &gOmmTimeTrialsShowSlideGhosts, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_TIME_TRIALS_SHOW_COINS_GHOSTS, &gOmmTimeTrialsShowCoinsGhosts, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_TIME_TRIALS_SHOW_ALL_STARS_GHOSTS, &gOmmTimeTrialsShowAllStarsGhosts, NULL),
        ), 6);
    gOmmOptTimeTrials.label = mem_dup(optTimeTrials.label, omm_opt_text_length(optTimeTrials.label) + 1);
    gOmmOptTimeTrials.subMenu = mem_dup(optTimeTrials.nextMenu, sizeof(struct SubMenu));

    // Warp to level sub-menu
    omm_opt_init_warp_to_level();
    struct Option optWarpToLevel =
        omm_opt_make_submenu(0, OMM_TEXT_OPT_WARP_TO_LEVEL_LABEL, OMM_TEXT_OPT_WARP_TO_LEVEL_TITLE, options(
            omm_opt_make_choice_level(OMM_TEXT_OPT_WARP_TO_LEVEL_LEVEL, &sOmmWarp->currLevel),
            omm_opt_make_choice_area(OMM_TEXT_OPT_WARP_TO_LEVEL_AREA, &sOmmWarp->currArea),
            omm_opt_make_choice_act(OMM_TEXT_OPT_WARP_TO_LEVEL_ACT, &sOmmWarp->currAct),
            omm_opt_make_button(0, OMM_TEXT_OPT_WARP_TO_LEVEL_WARP, omm_opt_warp_to_level),
        ), 4);
    gOmmOptWarpToLevel.label = mem_dup(optWarpToLevel.label, omm_opt_text_length(optWarpToLevel.label) + 1);
    gOmmOptWarpToLevel.subMenu = mem_dup(optWarpToLevel.nextMenu, sizeof(struct SubMenu));
    omm_add_routine(OMM_ROUTINE_TYPE_UPDATE, omm_opt_update_warp_to_level);

    // Warp to castle sub-menu
    struct Option optWarpToCastle =
        omm_opt_make_submenu(0, OMM_TEXT_OPT_WARP_TO_CASTLE_LABEL, OMM_TEXT_OPT_WARP_TO_CASTLE_TITLE, options(
            omm_opt_make_choice_level(OMM_TEXT_OPT_WARP_TO_LEVEL_LEVEL, &sOmmWarp->currLevel),
#if OMM_GAME_IS_SM74
            omm_opt_make_choice_area(OMM_TEXT_OPT_WARP_TO_LEVEL_AREA, &sOmmWarp->currArea),
#endif
            omm_opt_make_button(0, OMM_TEXT_OPT_WARP_TO_LEVEL_WARP, omm_opt_warp_to_castle),
        ), 2 + OMM_GAME_IS_SM74);
    gOmmOptWarpToCastle.label = mem_dup(optWarpToCastle.label, omm_opt_text_length(optWarpToCastle.label) + 1);
    gOmmOptWarpToCastle.subMenu = mem_dup(optWarpToCastle.nextMenu, sizeof(struct SubMenu));

    // Controls sub-menu
    struct Option optControls =
        omm_opt_make_submenu(0, OMM_TEXT_OPT_CONTROLS_LABEL, OMM_TEXT_OPT_CONTROLS_TITLE, options(
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_A_BUTTON, gOmmControlsButtonA),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_B_BUTTON, gOmmControlsButtonB),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_X_BUTTON, gOmmControlsButtonX),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_Y_BUTTON, gOmmControlsButtonY),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_START_BUTTON, gOmmControlsButtonStart),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_SPIN_BUTTON, gOmmControlsButtonSpin),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_L_TRIGGER, gOmmControlsTriggerL),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_R_TRIGGER, gOmmControlsTriggerR),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_Z_TRIGGER, gOmmControlsTriggerZ),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_C_UP, gOmmControlsCUp),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_C_DOWN, gOmmControlsCDown),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_C_LEFT, gOmmControlsCLeft),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_C_RIGHT, gOmmControlsCRight),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_D_UP, gOmmControlsDUp),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_D_DOWN, gOmmControlsDDown),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_D_LEFT, gOmmControlsDLeft),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_D_RIGHT, gOmmControlsDRight),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_STICK_UP, gOmmControlsStickUp),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_STICK_DOWN, gOmmControlsStickDown),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_STICK_LEFT, gOmmControlsStickLeft),
            omm_opt_make_bind(0, OMM_TEXT_OPT_CONTROLS_STICK_RIGHT, gOmmControlsStickRight),
            omm_opt_make_scroll(0, OMM_TEXT_OPT_CONTROLS_STICK_DEADZONE, &configStickDeadzone, 0, 100, 1),
            omm_opt_make_yes_no(0, OMM_TEXT_OPT_CONTROLS_RESET, OMM_TEXT_OPT_CONTROLS_RESET_ASK, omm_opt_reset_controls),
        ), 23);
    gOmmOptControls.label = mem_dup(optControls.label, omm_opt_text_length(optControls.label) + 1);
    gOmmOptControls.subMenu = mem_dup(optControls.nextMenu, sizeof(struct SubMenu));

    // Cheats sub-menu
#if !OMM_GAME_IS_R96X
    struct Option optCheats =
        omm_opt_make_submenu(0, OMM_TEXT_OPT_CHEATS_LABEL, OMM_TEXT_OPT_CHEATS_TITLE, options(
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_ENABLE, &gOmmCheatEnable, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_MOON_JUMP, &gOmmCheatMoonJump, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_GOD_MODE, &gOmmCheatGodMode, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_INVINCIBLE, &gOmmCheatInvincible, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_SUPER_SPEED, &gOmmCheatSuperSpeed, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_SUPER_RESPONSIVE, &gOmmCheatSuperResponsive, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_NO_FALL_DAMAGE, &gOmmCheatNoFallDamage, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_CAP_MODIFIER, &gOmmCheatCapModifier, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_WALK_ON_LAVA, &gOmmCheatWalkOnLava, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_WALK_ON_QUICKSAND, &gOmmCheatWalkOnQuicksand, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_WALK_ON_WATER, &gOmmCheatWalkOnWater, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_WALK_ON_GAS, &gOmmCheatWalkOnGas, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_WALK_ON_SLOPE, &gOmmCheatWalkOnSlope, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_WALK_ON_DEATH_BARRIER, &gOmmCheatWalkOnDeathBarrier, NULL),
            omm_opt_make_toggle(0, OMM_TEXT_OPT_CHEAT_BLJ_ANYWHERE, &gOmmCheatBljAnywhere, NULL),
        ), 15);
    gOmmOptCheats.label = mem_dup(optCheats.label, omm_opt_text_length(optCheats.label) + 1);
    gOmmOptCheats.subMenu = mem_dup(optCheats.nextMenu, sizeof(struct SubMenu));
#endif

    // Data management sub-menu
    struct Option optDataManagement =
        omm_opt_make_submenu(0, OMM_TEXT_OPT_DATA_LABEL, OMM_TEXT_OPT_DATA_TITLE, options(
            omm_opt_make_yes_no(0, OMM_TEXT_OPT_DATA_RESET_MARIO_PALETTES, OMM_TEXT_OPT_DATA_RESET_MARIO_PALETTES_ASK, omm_opt_reset_mario_palettes),
            omm_opt_make_yes_no(OMM_OPT_STATE_SPARKLY_STARS_UNLOCKED, OMM_TEXT_OPT_DATA_RESET_PEACH_PALETTES, OMM_TEXT_OPT_DATA_RESET_PEACH_PALETTES_ASK, omm_opt_reset_peach_palettes),
            omm_opt_make_yes_no(OMM_OPT_STATE_SPARKLY_STARS_UNLOCKED, OMM_TEXT_OPT_DATA_DELETE_SPARKLY_STARS_DATA, OMM_TEXT_OPT_DATA_DELETE_SPARKLY_STARS_DATA_ASK, omm_opt_delete_sparkly_stars_data),
            omm_opt_make_yes_no(0, OMM_TEXT_OPT_DATA_DELETE_GLOBAL_STATS, OMM_TEXT_OPT_DATA_DELETE_GLOBAL_STATS_ASK, omm_opt_delete_global_stats),
            omm_opt_make_yes_no(0, OMM_TEXT_OPT_DATA_DELETE_SECRETS, OMM_TEXT_OPT_DATA_DELETE_SECRETS_ASK, omm_opt_delete_secrets),
        ), 5);
    gOmmOptDataManagement.label = mem_dup(optDataManagement.label, omm_opt_text_length(optDataManagement.label) + 1);
    gOmmOptDataManagement.subMenu = mem_dup(optDataManagement.nextMenu, sizeof(struct SubMenu));

    // Change game sub-menu
    struct Option optChangeGame = omm_opt_init_change_game();
    if (optChangeGame.nextMenu->numOpts > 0) {
        gOmmOptChangeGame.label = mem_dup(optChangeGame.label, omm_opt_text_length(optChangeGame.label) + 1);
        gOmmOptChangeGame.subMenu = mem_dup(optChangeGame.nextMenu, sizeof(struct SubMenu));
    }

    // Exit game sub-menu
    struct Option optExitGame = omm_opt_make_yes_no(0, OMM_TEXT_OPT_EXIT_GAME_LABEL, OMM_TEXT_OPT_EXIT_GAME_ASK, omm_opt_exit_game);
    gOmmOptExitGame.label = mem_dup(optExitGame.label, omm_opt_text_length(optExitGame.label) + 1);
    gOmmOptExitGame.subMenu = mem_dup(optExitGame.nextMenu, sizeof(struct SubMenu));

    // Edit the options menu to include OMM sub-menus
    extern void omm_opt_init_main_menu();
    omm_opt_init_main_menu();
}

//
// Update
//

OMM_ROUTINE_PRE_RENDER(omm_opt_update_shortcuts) {
    static const char *TOGGLE_STRINGS[2] = { OMM_TEXT_OPT_DISABLED, OMM_TEXT_OPT_ENABLED };
    static struct {
        const void *label;
        const void **strings;
        union { bool *bval; u32 *uval; };
        u8 text[4][64];
        u8 color;
        u8 timer;
    } sOmmOptShortcutDisplay[1];

    // Check shortcuts and change the corresponding option if pressed
    if (!omm_is_main_menu() && !optmenu_open && gMarioObject) {
        bool changed = false;
#if !OMM_GAME_IS_R96X
        gKeyPressed = controller_get_raw_key();
#endif
        if (!omm_is_game_paused() && gKeyPressed != VK_INVALID) {
            u32 state = omm_opt_get_state();
            omm_array_for_each(sOmmOptShortcuts, p_shortcut) {
                OmmOptShortcut *shortcut = p_shortcut->as_ptr;
                if (OPT_STATE(shortcut->type) <= state) {
                    for (s32 j = 0; j != MAX_BINDS; ++j) {
                        if (gKeyPressed == shortcut->binds[j]) {
                            switch (OPT_TYPE(shortcut->type)) {
                                case OPT_TOGGLE: {
                                    *(shortcut->toggle.value) = !*(shortcut->toggle.value);
                                    sOmmOptShortcutDisplay->label = shortcut->label;
                                    sOmmOptShortcutDisplay->strings = (const void **) TOGGLE_STRINGS;
                                    sOmmOptShortcutDisplay->bval = shortcut->toggle.value;
                                    changed = true;
                                } break;

                                case OPT_CHOICE: {
                                    *(shortcut->choice.value) = (*(shortcut->choice.value) + 1) % shortcut->choice.numChoices;
                                    sOmmOptShortcutDisplay->label = shortcut->label;
                                    sOmmOptShortcutDisplay->strings = (const void **) shortcut->choice.choices;
                                    sOmmOptShortcutDisplay->uval = shortcut->choice.value;
                                    changed = true;
                                } break;
                            }
                            break;
                        }
                    }
                }
            }
        }

        // Must return now if the values changed, to let them update properly before displaying the message box
        if (changed) {
            return;
        }
    }

    // Create the text to display and reset the timer
    if (sOmmOptShortcutDisplay->label && sOmmOptShortcutDisplay->strings) {
        configfile_save(configfile_name());
        play_sound(SOUND_MENU_MARIO_CASTLE_WARP2, gGlobalSoundArgs);
        omm_text_copy(sOmmOptShortcutDisplay->text[0], 64, omm_text_convert(OMM_TEXT_OPT_SHORTCUTS_OPTION, false));
#if OMM_GAME_IS_R96X
        omm_text_copy(sOmmOptShortcutDisplay->text[1], 64, omm_text_convert(sOmmOptShortcutDisplay->label, false));
#else
        omm_text_copy(sOmmOptShortcutDisplay->text[1], 64, sOmmOptShortcutDisplay->label);
#endif
        omm_text_copy(sOmmOptShortcutDisplay->text[2], 64, omm_text_convert(OMM_TEXT_OPT_SHORTCUTS_SET_TO, false));
        if (sOmmOptShortcutDisplay->strings == (const void **) TOGGLE_STRINGS) {
            omm_text_copy(sOmmOptShortcutDisplay->text[3], 64, omm_text_convert(sOmmOptShortcutDisplay->strings[*sOmmOptShortcutDisplay->bval], false));
            sOmmOptShortcutDisplay->color = 2 + *sOmmOptShortcutDisplay->bval;
        } else {
#if OMM_GAME_IS_R96X
            omm_text_copy(sOmmOptShortcutDisplay->text[3], 64, omm_text_convert(sOmmOptShortcutDisplay->strings[*sOmmOptShortcutDisplay->uval], false));
#else
            omm_text_copy(sOmmOptShortcutDisplay->text[3], 64, sOmmOptShortcutDisplay->strings[*sOmmOptShortcutDisplay->uval]);
#endif
            sOmmOptShortcutDisplay->color = 1;
        }
        sOmmOptShortcutDisplay->timer = 60;
        sOmmOptShortcutDisplay->label = NULL;
        sOmmOptShortcutDisplay->strings = NULL;
    }

    // Display the text
    if (sOmmOptShortcutDisplay->timer) {
        Gfx *start = gDisplayListHead;
        gSPDisplayList(gDisplayListHead++, NULL);
        s32 w = omm_render_get_string_width(sOmmOptShortcutDisplay->text[0]) +
                omm_render_get_string_width(sOmmOptShortcutDisplay->text[1]) +
                omm_render_get_string_width(sOmmOptShortcutDisplay->text[2]) +
                omm_render_get_string_width(sOmmOptShortcutDisplay->text[3]);
        s32 x = (SCREEN_WIDTH - w) / 2;

        // Black box
        Vtx *vtx = omm_alloc_vtx(4);
        vtx[0] = (Vtx) {{{ -(w + 8) / 2, -8, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x60 }}};
        vtx[1] = (Vtx) {{{ -(w + 8) / 2, +8, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x60 }}};
        vtx[2] = (Vtx) {{{ +(w + 8) / 2, -8, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x60 }}};
        vtx[3] = (Vtx) {{{ +(w + 8) / 2, +8, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x60 }}};
        create_dl_translation_matrix(MENU_MTX_PUSH, SCREEN_WIDTH / 2, 27, 0);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gSPVertex(gDisplayListHead++, vtx, 4, 0);
        gSP2Triangles(gDisplayListHead++, 2, 1, 0, 0, 1, 2, 3, 0);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

        // Text
        s32 colors[4] = { 0, 1, 0, sOmmOptShortcutDisplay->color };
        for (s32 i = 0; i != 4; ++i) {
            u8 r, g, b;
            switch (colors[i]) {
                case 0: r = 0x00, g = 0xE0, b = 0xFF; break;
                case 1: r = 0xFF, g = 0xFF, b = 0xFF; break;
                case 2: r = 0xFF, g = 0x20, b = 0x20; break;
                case 3: r = 0x20, g = 0xE0, b = 0x20; break;
            }
            omm_render_string(x + 1, 22, r / 4, g / 4, b / 4, 0xFF, sOmmOptShortcutDisplay->text[i], 0);
            omm_render_string(x, 23, r, g, b, 0xFF, sOmmOptShortcutDisplay->text[i], 0);
            x += omm_render_get_string_width(sOmmOptShortcutDisplay->text[i]);
        }
        gSPEndDisplayList(gDisplayListHead++);
        gSPDisplayList(start, gDisplayListHead);
        sOmmOptShortcutDisplay->timer--;
    }
}

void omm_opt_update_binds() {
    array_for_each_(OmmControlsBind, bind, sOmmControlsBinds) {

        // Prevent the game from leaving necessary buttons without any bind
        if (bind->necessary && mem_eq(bind->binds, NO_BIND, sizeof(NO_BIND))) {
            mem_cpy(bind->binds, bind->defaults, sizeof(bind->defaults));
        }

        for (s32 i = 0; i != MAX_BINDS; ++i) {
            if (bind->binds[i] != VK_INVALID) {

                // Prevent the game from binding keys and buttons to other binds if they're already bound to A
                if (bind->binds != gOmmControlsButtonA) {
                    for (s32 j = 0; j != MAX_BINDS; ++j) {
                        if (bind->binds[i] == gOmmControlsButtonA[j]) {
                            bind->binds[i] = VK_INVALID;
                        }
                    }
                }

                // Prevent the game from binding keys and buttons to Shortcuts if they are already bound to Controls
                omm_array_for_each(sOmmOptShortcuts, p_shortcut) {
                    OmmOptShortcut *shortcut = p_shortcut->as_ptr;
                    for (s32 j = 0; j != MAX_BINDS; ++j) {
                        if (shortcut->binds[j] == bind->binds[i]) {
                            shortcut->binds[j] = VK_INVALID;
                        }
                    }
                }
            }
        }
    }
}

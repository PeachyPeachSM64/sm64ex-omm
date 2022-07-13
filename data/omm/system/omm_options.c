#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

// Required because some weird gcc optimizations make the OMM options menu not accessible in some releases
#pragma GCC push_options
#pragma GCC optimize ("O0")

//
// From options_menu.c
//

#if OMM_OPT_TYPES_IMPL
enum OptType { OPT_INVALID = 0, OPT_TOGGLE, OPT_CHOICE, OPT_SCROLL, OPT_SUBMENU, OPT_BIND, OPT_BUTTON, };
struct SubMenu;
struct Option { enum OptType type; const u8 *label; union { u32 *uval; bool *bval; }; union { struct { const u8 **choices; s32 numChoices; }; struct { u32 scrMin; u32 scrMax; u32 scrStep; }; struct SubMenu *nextMenu; void (*actionFn)(struct Option *, s32); }; };
struct SubMenu { struct SubMenu *prev; const u8 *label; struct Option *opts; s32 numOpts; s32 select; s32 scroll; };
#endif

//
// Options definition
//

u8 gOmmOptMenu[0x100 + sizeof(struct SubMenu)];
u8 gOmmOptControls[0x100 + sizeof(struct SubMenu)];
#if !OMM_GAME_IS_R96A
u8 gOmmOptCheats[0x100 + sizeof(struct SubMenu)];
#endif
#if !OMM_CODE_DYNOS
u8 gOmmOptWarpToLevel[0x100 + sizeof(struct SubMenu)];
u8 gOmmOptReturnToMainMenu[0x100];
#endif

#define K_NONE VK_INVALID
static const struct {
    u32 *binds;                 u32 b0; u32 b1; u32 b2;
} sOmmControlsDefault[] = {                               //  QW  /  AZ  | XBOne Con | Switch PC |
    { gOmmControlsButtonA,      0x0026, 0x1000, K_NONE }, //     [L]     |    (A)    |    (B)    |
    { gOmmControlsButtonB,      0x0033, 0x1001, K_NONE }, //  [,] / [;]  |    (B)    |    (A)    |
    { gOmmControlsButtonX,      0x0025, 0x1002, K_NONE }, //     [K]     |    (X)    |    (X)    |
    { gOmmControlsButtonY,      0x0032, 0x1003, K_NONE }, //  [M] / [,]  |    (Y)    |    (Y)    |
    { gOmmControlsButtonStart,  0x0039, 0x1006, K_NONE }, //   [SPACE]   |  (Start)  |    (+)    |
    { gOmmControlsTriggerL,     0x002A, 0x1009, K_NONE }, //   [LSHFT]   |    (LB)   |    (L)    |
    { gOmmControlsTriggerR,     0x0036, 0x100A, K_NONE }, //   [RSHFT]   |    (RB)   |    (R)    |
    { gOmmControlsTriggerZ,     0x0018, 0x101B, K_NONE }, //     [O]     |    (RT)   |    (ZR)   |
    { gOmmControlsCUp,          0x0148, K_NONE, K_NONE }, //     [^]     |   (R-U)   |   (R-U)   |
    { gOmmControlsCDown,        0x0150, K_NONE, K_NONE }, //     [v]     |   (R-D)   |   (R-D)   |
    { gOmmControlsCLeft,        0x014B, K_NONE, K_NONE }, //     [<]     |   (R-L)   |   (R-L)   |
    { gOmmControlsCRight,       0x014D, K_NONE, K_NONE }, //     [>]     |   (R-R)   |   (R-R)   |
    { gOmmControlsDUp,          0x000C, 0x100B, K_NONE }, //  [-] / [)]  |   (D-U)   |   (D-U)   |
    { gOmmControlsDDown,        0x001A, 0x100C, K_NONE }, //  [{] / [^]  |   (D-D)   |   (D-D)   |
    { gOmmControlsDLeft,        0x0019, 0x100D, K_NONE }, //     [P]     |   (D-L)   |   (D-L)   |
    { gOmmControlsDRight,       0x001B, 0x100E, K_NONE }, //  [}] / [$]  |   (D-R)   |   (D-R)   |
    { gOmmControlsStickUp,      0x0011, K_NONE, K_NONE }, //  [W] / [Z]  |   (L-U)   |   (L-U)   |
    { gOmmControlsStickDown,    0x001F, K_NONE, K_NONE }, //     [S]     |   (L-D)   |   (L-D)   |
    { gOmmControlsStickLeft,    0x001E, K_NONE, K_NONE }, //  [A] / [Q]  |   (L-L)   |   (L-L)   |
    { gOmmControlsStickRight,   0x0020, K_NONE, K_NONE }, //     [D]     |   (L-R)   |   (L-R)   |
};

// Somehow works with GCC, but not MSVC
DEFINE_KBINDS(gOmmControlsButtonA,     sOmmControlsDefault[ 0].b0, sOmmControlsDefault[ 0].b1, sOmmControlsDefault[ 0].b2);
DEFINE_KBINDS(gOmmControlsButtonB,     sOmmControlsDefault[ 1].b0, sOmmControlsDefault[ 1].b1, sOmmControlsDefault[ 1].b2);
DEFINE_KBINDS(gOmmControlsButtonX,     sOmmControlsDefault[ 2].b0, sOmmControlsDefault[ 2].b1, sOmmControlsDefault[ 2].b2);
DEFINE_KBINDS(gOmmControlsButtonY,     sOmmControlsDefault[ 3].b0, sOmmControlsDefault[ 3].b1, sOmmControlsDefault[ 3].b2);
DEFINE_KBINDS(gOmmControlsButtonStart, sOmmControlsDefault[ 4].b0, sOmmControlsDefault[ 4].b1, sOmmControlsDefault[ 4].b2);
DEFINE_KBINDS(gOmmControlsTriggerL,    sOmmControlsDefault[ 5].b0, sOmmControlsDefault[ 5].b1, sOmmControlsDefault[ 5].b2);
DEFINE_KBINDS(gOmmControlsTriggerR,    sOmmControlsDefault[ 6].b0, sOmmControlsDefault[ 6].b1, sOmmControlsDefault[ 6].b2);
DEFINE_KBINDS(gOmmControlsTriggerZ,    sOmmControlsDefault[ 7].b0, sOmmControlsDefault[ 7].b1, sOmmControlsDefault[ 7].b2);
DEFINE_KBINDS(gOmmControlsCUp,         sOmmControlsDefault[ 8].b0, sOmmControlsDefault[ 8].b1, sOmmControlsDefault[ 8].b2);
DEFINE_KBINDS(gOmmControlsCDown,       sOmmControlsDefault[ 9].b0, sOmmControlsDefault[ 9].b1, sOmmControlsDefault[ 9].b2);
DEFINE_KBINDS(gOmmControlsCLeft,       sOmmControlsDefault[10].b0, sOmmControlsDefault[10].b1, sOmmControlsDefault[10].b2);
DEFINE_KBINDS(gOmmControlsCRight,      sOmmControlsDefault[11].b0, sOmmControlsDefault[11].b1, sOmmControlsDefault[11].b2);
DEFINE_KBINDS(gOmmControlsDUp,         sOmmControlsDefault[12].b0, sOmmControlsDefault[12].b1, sOmmControlsDefault[12].b2);
DEFINE_KBINDS(gOmmControlsDDown,       sOmmControlsDefault[13].b0, sOmmControlsDefault[13].b1, sOmmControlsDefault[13].b2);
DEFINE_KBINDS(gOmmControlsDLeft,       sOmmControlsDefault[14].b0, sOmmControlsDefault[14].b1, sOmmControlsDefault[14].b2);
DEFINE_KBINDS(gOmmControlsDRight,      sOmmControlsDefault[15].b0, sOmmControlsDefault[15].b1, sOmmControlsDefault[15].b2);
DEFINE_KBINDS(gOmmControlsStickUp,     sOmmControlsDefault[16].b0, sOmmControlsDefault[16].b1, sOmmControlsDefault[16].b2);
DEFINE_KBINDS(gOmmControlsStickDown,   sOmmControlsDefault[17].b0, sOmmControlsDefault[17].b1, sOmmControlsDefault[17].b2);
DEFINE_KBINDS(gOmmControlsStickLeft,   sOmmControlsDefault[18].b0, sOmmControlsDefault[18].b1, sOmmControlsDefault[18].b2);
DEFINE_KBINDS(gOmmControlsStickRight,  sOmmControlsDefault[19].b0, sOmmControlsDefault[19].b1, sOmmControlsDefault[19].b2);
#if !OMM_GAME_IS_R96A
DEFINE_TOGGLE(gOmmCheatEnable, 0);                   // Disabled
DEFINE_TOGGLE(gOmmCheatMoonJump, 0);                 // Disabled
DEFINE_TOGGLE(gOmmCheatGodMode, 0);                  // Disabled
DEFINE_TOGGLE(gOmmCheatInvincible, 0);               // Disabled
DEFINE_TOGGLE(gOmmCheatSuperSpeed, 0);               // Disabled
DEFINE_TOGGLE(gOmmCheatSuperResponsive, 0);          // Disabled
DEFINE_TOGGLE(gOmmCheatNoFallDamage, 0);             // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnLava, 0);               // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnQuicksand, 0);          // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnWater, 0);              // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnGas, 0);                // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnSlope, 0);              // Disabled
DEFINE_TOGGLE(gOmmCheatWalkOnDeathBarrier, 0);       // Disabled
DEFINE_TOGGLE(gOmmCheatBljAnywhere, 0);              // Disabled
#endif
DEFINE_CHOICE(gOmmFPS, 0);                           // 30 FPS
DEFINE_CHOICE_SC(gOmmCharacter, 0);                  // Mario
DEFINE_CHOICE_SC(gOmmMovesetType, 1);                // Odyssey (3-Health)
DEFINE_CHOICE_SC(gOmmCapType, 2);                    // Cappy (Capture)
DEFINE_CHOICE_SC(gOmmStarsMode, 1);                  // Non-Stop
DEFINE_CHOICE_SC(gOmmPowerUpsType, 1);               // Improved
DEFINE_CHOICE_SC(gOmmCameraMode, 0);                 // Classic
DEFINE_CHOICE_SC(gOmmSparklyStarsMode, 0);           // Disabled
DEFINE_TOGGLE_SC(gOmmSparklyStarsAssist, 0);         // Disabled
DEFINE_TOGGLE_SC(gOmmSparklyStarsHint, 1);           // Enabled
DEFINE_TOGGLE(gOmmCheatUnlimitedCappyBounces, 0);    // Disabled
DEFINE_TOGGLE(gOmmCheatCappyStaysForever, 0);        // Disabled
DEFINE_TOGGLE(gOmmCheatHomingAttackGlobalRange, 0);  // Disabled
DEFINE_TOGGLE(gOmmCheatMarioTeleportsToCappy, 0);    // Disabled
DEFINE_TOGGLE(gOmmCheatCappyCanCollectStars, 0);     // Disabled
DEFINE_TOGGLE(gOmmCheatPlayAsCappy, 0);              // Disabled
DEFINE_TOGGLE(gOmmCheatPeachEndlessVibeGauge, 0);    // Disabled
DEFINE_CHOICE(gOmmExtrasMarioColors, 0);             // Default
DEFINE_CHOICE(gOmmExtrasPeachColors, 0);             // Default
DEFINE_TOGGLE_SC(gOmmExtrasSMOAnimations, 1);        // Enabled
DEFINE_TOGGLE_SC(gOmmExtrasCappyAndTiara, 1);        // Enabled
DEFINE_TOGGLE_SC(gOmmExtrasColoredStars, 1);         // Enabled
DEFINE_TOGGLE_SC(gOmmExtrasVanishingHUD, 1);         // Enabled
DEFINE_TOGGLE_SC(gOmmExtrasRevealSecrets, 0);        // Disabled
DEFINE_TOGGLE_SC(gOmmExtrasRedCoinsRadar, 0);        // Disabled
DEFINE_TOGGLE_SC(gOmmExtrasShowStarNumber, 0);       // Disabled
DEFINE_TOGGLE_SC(gOmmExtrasInvisibleMode, 0);        // Disabled
#if OMM_CODE_DEV
DEFINE_TOGGLE_SC(gOmmExtrasRender96Peach, 0);        // Disabled
#endif
DEFINE_TOGGLE_SC(gOmmExtrasCrystalStarsReward, 0);   // Disabled
DEFINE_TOGGLE_SC(gOmmExtrasNebulaStarsReward, 0);    // Disabled
#if OMM_CODE_DEBUG
DEFINE_TOGGLE_SC(gOmmDebugHitbox, 0);                // Disabled
DEFINE_TOGGLE_SC(gOmmDebugHurtbox, 0);               // Disabled
DEFINE_TOGGLE_SC(gOmmDebugWallbox, 0);               // Disabled
DEFINE_TOGGLE_SC(gOmmDebugSurface, 0);               // Disabled
DEFINE_TOGGLE_SC(gOmmDebugMario, 0);                 // Disabled
DEFINE_TOGGLE_SC(gOmmDebugCappy, 0);                 // Disabled
DEFINE_TOGGLE_SC(gOmmDebugProfiler, 0);              // Disabled
#if OMM_CODE_DEV_DEBUG
DEFINE_TOGGLE_SC(gOmmDebugGameSpeedEnable, 0);       // Disabled
DEFINE_CHOICE_SC(gOmmDebugGameSpeedFps, 0);          // 30 FPS
#endif
#endif

//
// Shortcuts
//

typedef struct {
    s32 type;
    u32 *binds;
    const char *label;
    union {
        struct {
            bool *option;
        } toggle;
        struct {
            u32 *option;
            s32 numChoices;
            const char *choices[8];
        } choice;
    };
} OmmOptShortcut;

#define DEFINE_SHORTCUT_TOGGLE(opt, lbl)            { .type = OPT_TOGGLE, .binds = opt##Shortcuts, .label = lbl, .toggle.option = &opt }
#define DEFINE_SHORTCUT_CHOICE(opt, lbl, nch, ...)  { .type = OPT_CHOICE, .binds = opt##Shortcuts, .label = lbl, .choice.option = &opt, .choice.numChoices = nch, .choice.choices = { __VA_ARGS__ } }

static const OmmOptShortcut sOmmOptShortcuts[] = {
DEFINE_SHORTCUT_CHOICE(gOmmCharacter, OMM_TEXT_OPT_CHARACTER_LABEL, OMM_NUM_PLAYABLE_CHARACTERS, OMM_TEXT_MARIO, OMM_TEXT_PEACH, OMM_TEXT_LUIGI, OMM_TEXT_WARIO),
DEFINE_SHORTCUT_CHOICE(gOmmMovesetType, OMM_TEXT_OPT_MOVESET_LABEL, 3, OMM_TEXT_OPT_MOVESET_CLASSIC, OMM_TEXT_OPT_MOVESET_ODYSSEY_3H, OMM_TEXT_OPT_MOVESET_ODYSSEY_6H),
DEFINE_SHORTCUT_CHOICE(gOmmCapType, OMM_TEXT_OPT_CAP_LABEL, 3, OMM_TEXT_OPT_CAP_CLASSIC, OMM_TEXT_OPT_CAP_NO_CAPTURE, OMM_TEXT_OPT_CAP_CAPTURE),
DEFINE_SHORTCUT_CHOICE(gOmmStarsMode, OMM_TEXT_OPT_STARS_LABEL, 2, OMM_TEXT_OPT_STARS_CLASSIC, OMM_TEXT_OPT_STARS_NON_STOP),
DEFINE_SHORTCUT_CHOICE(gOmmPowerUpsType, OMM_TEXT_OPT_POWER_UPS_LABEL, 2, OMM_TEXT_OPT_POWER_UPS_CLASSIC, OMM_TEXT_OPT_POWER_UPS_IMPROVED),
DEFINE_SHORTCUT_CHOICE(gOmmCameraMode, OMM_TEXT_OPT_CAMERA_LABEL, 3, OMM_TEXT_OPT_CAMERA_CLASSIC, OMM_TEXT_OPT_CAMERA_8_DIR, OMM_TEXT_OPT_CAMERA_16_DIR),
DEFINE_SHORTCUT_CHOICE(gOmmSparklyStarsMode, OMM_TEXT_OPT_SPARKLY_STARS_MODE, 4, OMM_TEXT_OPT_SPARKLY_STARS_MODE_0, OMM_TEXT_OPT_SPARKLY_STARS_MODE_1, OMM_TEXT_OPT_SPARKLY_STARS_MODE_2, OMM_TEXT_OPT_SPARKLY_STARS_MODE_3),
DEFINE_SHORTCUT_TOGGLE(gOmmSparklyStarsAssist, OMM_TEXT_OPT_SPARKLY_STARS_ASSIST),
DEFINE_SHORTCUT_TOGGLE(gOmmSparklyStarsHint, OMM_TEXT_OPT_SPARKLY_STARS_HINT),
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasSMOAnimations, OMM_TEXT_OPT_SMO_ANIMATIONS),
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasCappyAndTiara, OMM_TEXT_OPT_CAPPY_AND_TIARA),
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasColoredStars, OMM_TEXT_OPT_COLORED_STARS),
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasVanishingHUD, OMM_TEXT_OPT_VANISHING_HUD),
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasRevealSecrets, OMM_TEXT_OPT_REVEAL_SECRETS),
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasRedCoinsRadar, OMM_TEXT_OPT_RED_COINS_RADAR),
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasShowStarNumber, OMM_TEXT_OPT_SHOW_STAR_NUMBER),
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasInvisibleMode, OMM_TEXT_OPT_INVISIBLE_MODE),
#if OMM_CODE_DEV
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasRender96Peach, OMM_TEXT_OPT_RENDER96_PEACH),
#endif
#if OMM_CODE_DEBUG
DEFINE_SHORTCUT_TOGGLE(gOmmDebugHitbox, OMM_TEXT_OPT_DEBUG_HITBOX),
DEFINE_SHORTCUT_TOGGLE(gOmmDebugHurtbox, OMM_TEXT_OPT_DEBUG_HURTBOX),
DEFINE_SHORTCUT_TOGGLE(gOmmDebugWallbox, OMM_TEXT_OPT_DEBUG_WALLBOX),
DEFINE_SHORTCUT_TOGGLE(gOmmDebugSurface, OMM_TEXT_OPT_DEBUG_SURFACE),
DEFINE_SHORTCUT_TOGGLE(gOmmDebugMario, OMM_TEXT_OPT_DEBUG_MARIO),
DEFINE_SHORTCUT_TOGGLE(gOmmDebugCappy, OMM_TEXT_OPT_DEBUG_CAPPY),
DEFINE_SHORTCUT_TOGGLE(gOmmDebugProfiler, OMM_TEXT_OPT_DEBUG_PROFILER),
#if OMM_CODE_DEV_DEBUG
DEFINE_SHORTCUT_TOGGLE(gOmmDebugGameSpeedEnable, OMM_TEXT_OPT_DEBUG_GAME_SPEED),
DEFINE_SHORTCUT_CHOICE(gOmmDebugGameSpeedFps, OMM_TEXT_OPT_DEBUG_GAME_SPEED, 5, OMM_TEXT_OPT_DEBUG_GAME_SPEED_30_FPS, OMM_TEXT_OPT_DEBUG_GAME_SPEED_20_FPS, OMM_TEXT_OPT_DEBUG_GAME_SPEED_15_FPS, OMM_TEXT_OPT_DEBUG_GAME_SPEED_10_FPS, OMM_TEXT_OPT_DEBUG_GAME_SPEED_5_FPS),
#endif
#endif
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasCrystalStarsReward, OMM_TEXT_OPT_CRYSTAL_STARS_REWARD), // Must be last
DEFINE_SHORTCUT_TOGGLE(gOmmExtrasNebulaStarsReward, OMM_TEXT_OPT_CRYSTAL_STARS_REWARD), // Must be last
};

//
// Option wrappers
//

#if OMM_GAME_IS_R96A
#define omm_opt_text(str, ...)      (const u8 *) str
#define omm_opt_text_length(str)    (s32) strlen((const char *) str)
#else
#define omm_opt_text(str, ...)      omm_text_convert(str, __VA_ARGS__)
#define omm_opt_text_length(str)    omm_text_length(str)
#endif

static struct Option omm_opt_make_toggle(const char *label, bool *value) {
    struct Option opt = { 0 };
    opt.type = OPT_TOGGLE;
    opt.label = omm_opt_text(label, true);
    opt.bval = value;
    return opt;
}

static struct Option omm_opt_make_choice(const char *label, u32 *value, const char **choices, s32 numChoices) {
    struct Option opt = { 0 };
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(label, true);
    opt.uval = value;
    opt.choices = OMM_MEMNEW(u8 *, numChoices);
    opt.numChoices = numChoices;
    for (s32 i = 0; i != numChoices; ++i) {
        opt.choices[i] = omm_opt_text(choices[i], true);
    }
    return opt;
}

static struct Option omm_opt_make_scroll(const char *label, u32 *value, u32 min, u32 max, u32 step) {
    struct Option opt = { 0 };
    opt.type = OPT_SCROLL;
    opt.label = omm_opt_text(label, true);
    opt.uval = value;
    opt.scrMin = min;
    opt.scrMax = max;
    opt.scrStep = step;
    return opt;
}

static struct Option omm_opt_make_bind(const char *label, u32 *binds) {
    struct Option opt = { 0 };
    opt.type = OPT_BIND;
    opt.label = omm_opt_text(label, true);
    opt.uval = binds;
    return opt;
}

static struct Option omm_opt_make_button(const char *label, void (*actionFn)(struct Option *, s32)) {
    struct Option opt = { 0 };
    opt.type = OPT_BUTTON;
    opt.label = omm_opt_text(label, true);
    opt.actionFn = actionFn;
    return opt;
}

static struct Option omm_opt_make_submenu(const char *label, const char *title, struct Option *options, s32 numOptions) {
    struct Option opt = { 0 };
    opt.type = OPT_SUBMENU;
    opt.label = omm_opt_text(label, true);
    opt.nextMenu = OMM_MEMNEW(struct SubMenu, 1);
    opt.nextMenu->label = omm_opt_text(title, true);
    opt.nextMenu->opts = OMM_MEMNEW(struct Option, numOptions);
    opt.nextMenu->numOpts = numOptions;
    for (s32 i = 0; i != numOptions; ++i) {
        opt.nextMenu->opts[i] = options[i];
    }
    return opt;
}

static struct Option omm_opt_make_shortcuts_submenu(const char *label, const char *title) {
    s32 numShortcuts = OMM_ARRAY_SIZE(sOmmOptShortcuts) - !omm_ssd_is_completed(OMM_SSM_HARD);
    struct Option opt = { 0 };
    opt.type = OPT_SUBMENU;
    opt.label = omm_opt_text(label, true);
    opt.nextMenu = OMM_MEMNEW(struct SubMenu, 1);
    opt.nextMenu->label = omm_opt_text(title, true);
    opt.nextMenu->opts = OMM_MEMNEW(struct Option, numShortcuts);
    opt.nextMenu->numOpts = numShortcuts;
    for (s32 i = 0; i != numShortcuts; ++i) {
        opt.nextMenu->opts[i] = omm_opt_make_bind(sOmmOptShortcuts[i].label, sOmmOptShortcuts[i].binds);
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

static u32 sOmmWarpLevel = 0;

typedef struct { s32 level; s32 area; } AreaValues;
static OmmArray sOmmWarpAreaValues = omm_array_zero;
static u32 sOmmWarpArea = 0;

typedef struct { s32 level; s32 act; } ActValues;
static OmmArray sOmmWarpActValues = omm_array_zero;
static u32 sOmmWarpAct = 0;

static void omm_opt_init_warp_to_level() {

    // Areas
    for (s32 i = 0; i != omm_level_get_count(); ++i) {
        s32 level = omm_level_get_list()[i];
        s32 areas = omm_level_get_areas(level);
        for (s32 j = 0; j != 32; ++j) {
            if ((areas >> j) & 1) {
                AreaValues *value = OMM_MEMNEW(AreaValues, 1);
                value->level = level;
                value->area = j;
                omm_array_add(sOmmWarpAreaValues, ptr, value);
            }
        }
    }

    // Acts
    for (s32 i = 0; i != omm_level_get_count(); ++i) {
        s32 level = omm_level_get_list()[i];
        s32 stars = omm_stars_get_bits_total(level);
        if (stars != 0) {
            for (s32 j = 0; j != 6; ++j) {
                if ((stars >> j) & 1) {
                    ActValues *value = OMM_MEMNEW(ActValues, 1);
                    value->level = level;
                    value->act = j + 1;
                    omm_array_add(sOmmWarpActValues, ptr, value);
                }
            }
        } else {
            ActValues *value = OMM_MEMNEW(ActValues, 1);
            value->level = level;
            value->act = 1;
            omm_array_add(sOmmWarpActValues, ptr, value);
        }
    }
}

static struct Option omm_opt_make_choice_level(const char *label, u32 *value) {
    struct Option opt = { 0 };
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(label, true);
    opt.uval = value;
    opt.choices = OMM_MEMNEW(u8 *, omm_level_get_count());
    opt.numChoices = omm_level_get_count();
    for (s32 i = 0; i != (s32) opt.numChoices; ++i) {
        const u8 *name = omm_level_get_name(omm_level_get_list()[i], true, true);
        opt.choices[i] = OMM_MEMDUP(name, omm_opt_text_length(name) + 1);
    }
    return opt;
}

static struct Option omm_opt_make_choice_area(const char *label, u32 *value) {
    struct Option opt = { 0 };
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(label, true);
    opt.uval = value;
    opt.choices = OMM_MEMNEW(u8 *, omm_array_count(sOmmWarpAreaValues));
    opt.numChoices = omm_array_count(sOmmWarpAreaValues);
    for (s32 i = 0; i != (s32) opt.numChoices; ++i) {
        OMM_STRING(name, 256, "Area %d", ((AreaValues *) omm_array_get(sOmmWarpAreaValues, ptr, i))->area);
        opt.choices[i] = omm_opt_text(name, true);
    }
    return opt;
}

static struct Option omm_opt_make_choice_act(const char *label, u32 *value) {
    struct Option opt = { 0 };
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(label, true);
    opt.uval = value;
    opt.choices = OMM_MEMNEW(u8 *, omm_array_count(sOmmWarpActValues));
    opt.numChoices = omm_array_count(sOmmWarpActValues);
    for (s32 i = 0; i != (s32) opt.numChoices; ++i) {
        s32 level = ((ActValues *) omm_array_get(sOmmWarpActValues, ptr, i))->level;
        s32 act = ((ActValues *) omm_array_get(sOmmWarpActValues, ptr, i))->act;
        const u8 *name = omm_level_get_act_name(level, act, true, true);
        opt.choices[i] = OMM_MEMDUP(name, omm_opt_text_length(name) + 1);
    }
    return opt;
}

//
// Warp to level (update)
//

static u32 omm_opt_get_level_index(s32 level) {
    for (s32 i = 0; i != omm_level_get_count(); ++i) {
        if (omm_level_get_list()[i] == level) {
            return i;
        }
    }
    return 0;
}

static u32 omm_opt_get_first_area_index(s32 level) {
    omm_array_for_each(sOmmWarpAreaValues, p) {
        AreaValues *areaValues = (AreaValues *) p->as_ptr;
        if (areaValues->level == level) {
            return i_p;
        }
    }
    return 0;
}

static u32 omm_opt_get_first_act_index(s32 level) {
    omm_array_for_each(sOmmWarpActValues, p) {
        ActValues *actValues = (ActValues *) p->as_ptr;
        if (actValues->level == level) {
            return i_p;
        }
    }
    return 0;
}

static void omm_opt_update_warp_to_level() {
    static u32 sOmmWarpLevelPrev = 0;
    static u32 sOmmWarpAreaPrev = 0;
    static u32 sOmmWarpActPrev = 0;

    // Level changed
    if (sOmmWarpLevelPrev != sOmmWarpLevel) {
        s32 level = omm_level_get_list()[sOmmWarpLevel];
        sOmmWarpArea = omm_opt_get_first_area_index(level);
        sOmmWarpAct = omm_opt_get_first_act_index(level);
    }

    // Area changed
    else if (sOmmWarpAreaPrev != sOmmWarpArea) {
        s32 count = omm_array_count(sOmmWarpAreaValues);
        u32 left  = (sOmmWarpAreaPrev + count - 1) % count;
        u32 right = (sOmmWarpAreaPrev + count + 1) % count;
        s32 prev  = ((AreaValues *) omm_array_get(sOmmWarpAreaValues, ptr, sOmmWarpAreaPrev))->area;
        s32 curr  = ((AreaValues *) omm_array_get(sOmmWarpAreaValues, ptr, sOmmWarpArea))->area;
        s32 level = ((AreaValues *) omm_array_get(sOmmWarpAreaValues, ptr, sOmmWarpArea))->level;
        if ((sOmmWarpArea == left && curr >= prev) || (sOmmWarpArea == right && curr <= prev))  {
            sOmmWarpAct = omm_opt_get_first_act_index(level);
        }
        sOmmWarpLevel = omm_opt_get_level_index(level);
    }

    // Act changed
    else if (sOmmWarpActPrev != sOmmWarpAct) {
        s32 count = omm_array_count(sOmmWarpActValues);
        u32 left  = (sOmmWarpActPrev + count - 1) % count;
        u32 right = (sOmmWarpActPrev + count + 1) % count;
        s32 prev  = ((ActValues *) omm_array_get(sOmmWarpActValues, ptr, sOmmWarpActPrev))->act;
        s32 curr  = ((ActValues *) omm_array_get(sOmmWarpActValues, ptr, sOmmWarpAct))->act;
        s32 level = ((ActValues *) omm_array_get(sOmmWarpActValues, ptr, sOmmWarpAct))->level;
        if ((sOmmWarpAct == left && curr >= prev) || (sOmmWarpAct == right && curr <= prev))  {
            sOmmWarpArea = omm_opt_get_first_area_index(level);
        }
        sOmmWarpLevel = omm_opt_get_level_index(level);
    }

    // Update values
    sOmmWarpLevelPrev = sOmmWarpLevel;
    sOmmWarpAreaPrev = sOmmWarpArea;
    sOmmWarpActPrev = sOmmWarpAct;
}

static void omm_opt_warp_to_level(UNUSED void *opt, s32 arg) {
    if (!arg) {
        s32 level = omm_level_get_list()[sOmmWarpLevel];
        s32 area = ((AreaValues *) omm_array_get(sOmmWarpAreaValues, ptr, sOmmWarpArea))->area;
        s32 act = ((ActValues *) omm_array_get(sOmmWarpActValues, ptr, sOmmWarpAct))->act;
        if (!omm_warp_to_level(level, area, act)) {
            play_sound(SOUND_MENU_CAMERA_BUZZ | 0xFF00, gGlobalSoundArgs);
        }
    }
}

#endif

//
// Buttons
//

void omm_opt_return_to_main_menu(UNUSED void *opt, s32 arg) {
    if (!arg) {
        omm_return_to_main_menu();
    }
}

void omm_opt_reset_binds(u32 *binds) {
    for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmControlsDefault); ++i) {
        if (binds   == sOmmControlsDefault[i].binds) {
            binds[0] = sOmmControlsDefault[i].b0;
            binds[1] = sOmmControlsDefault[i].b1;
            binds[2] = sOmmControlsDefault[i].b2;
            return;
        }
    }
}

static void omm_opt_reset_controls(UNUSED void *opt, s32 arg) {
    if (!arg) {
        for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmControlsDefault); ++i) {
            sOmmControlsDefault[i].binds[0] = sOmmControlsDefault[i].b0;
            sOmmControlsDefault[i].binds[1] = sOmmControlsDefault[i].b1;
            sOmmControlsDefault[i].binds[2] = sOmmControlsDefault[i].b2;
        }
    }
}

//
// Init
//

#define choices(...) (OMM_ARRAY_OF(const char *) { __VA_ARGS__ })
#define options(...) (OMM_ARRAY_OF(struct Option) { __VA_ARGS__ })

#if !OMM_CODE_DYNOS
OMM_AT_STARTUP static
#endif
void omm_opt_init() {
    save_file_load_all();
    omm_player_select(gOmmCharacter);
#if OMM_CODE_DEV_DEBUG
    gOmmDebugGameSpeedEnable = false;
    gOmmDebugGameSpeedFps = 0;
#endif

    // OMM menu
    struct Option optOmmMenu =
        omm_opt_make_submenu(OMM_TEXT_OPT_MENU_LABEL, OMM_TEXT_OPT_MENU_TITLE, options(
            omm_opt_make_choice(OMM_TEXT_OPT_CHARACTER_LABEL, &gOmmCharacter, choices(OMM_TEXT_MARIO, OMM_TEXT_PEACH, OMM_TEXT_LUIGI, OMM_TEXT_WARIO), OMM_NUM_PLAYABLE_CHARACTERS),
            omm_opt_make_choice(OMM_TEXT_OPT_MOVESET_LABEL, &gOmmMovesetType, choices(OMM_TEXT_OPT_MOVESET_CLASSIC, OMM_TEXT_OPT_MOVESET_ODYSSEY_3H, OMM_TEXT_OPT_MOVESET_ODYSSEY_6H), 3),
            omm_opt_make_choice(OMM_TEXT_OPT_CAP_LABEL, &gOmmCapType, choices(OMM_TEXT_OPT_CAP_CLASSIC, OMM_TEXT_OPT_CAP_NO_CAPTURE, OMM_TEXT_OPT_CAP_CAPTURE), 3),
            omm_opt_make_choice(OMM_TEXT_OPT_STARS_LABEL, &gOmmStarsMode, choices(OMM_TEXT_OPT_STARS_CLASSIC, OMM_TEXT_OPT_STARS_NON_STOP), 2),
            omm_opt_make_choice(OMM_TEXT_OPT_POWER_UPS_LABEL, &gOmmPowerUpsType, choices(OMM_TEXT_OPT_POWER_UPS_CLASSIC, OMM_TEXT_OPT_POWER_UPS_IMPROVED), 2),
            omm_opt_make_choice(OMM_TEXT_OPT_CAMERA_LABEL, &gOmmCameraMode, choices(OMM_TEXT_OPT_CAMERA_CLASSIC, OMM_TEXT_OPT_CAMERA_8_DIR, OMM_TEXT_OPT_CAMERA_16_DIR), 3),
            omm_opt_make_submenu(OMM_TEXT_OPT_SPARKLY_STARS_LABEL, OMM_TEXT_OPT_SPARKLY_STARS_TITLE, options(
                omm_opt_make_choice(OMM_TEXT_OPT_SPARKLY_STARS_MODE, &gOmmSparklyStarsMode, choices(OMM_TEXT_OPT_SPARKLY_STARS_MODE_0, OMM_TEXT_OPT_SPARKLY_STARS_MODE_1, OMM_TEXT_OPT_SPARKLY_STARS_MODE_2, OMM_TEXT_OPT_SPARKLY_STARS_MODE_3), OMM_SSM_COUNT),
                omm_opt_make_toggle(OMM_TEXT_OPT_SPARKLY_STARS_ASSIST, &gOmmSparklyStarsAssist),
                omm_opt_make_toggle(OMM_TEXT_OPT_SPARKLY_STARS_HINT, &gOmmSparklyStarsHint),
            ), 3),
            omm_opt_make_submenu(OMM_TEXT_OPT_CHEATS_LABEL, OMM_TEXT_OPT_CHEATS_TITLE, options(
                omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_UNLIMITED_CAPPY_BOUNCES, &gOmmCheatUnlimitedCappyBounces),
                omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_CAPPY_STAYS_FOREVER, &gOmmCheatCappyStaysForever),
                omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_GLOBAL_HOMING_ATTACK_RANGE, &gOmmCheatHomingAttackGlobalRange),
                omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_MARIO_TELEPORTS_TO_CAPPY, &gOmmCheatMarioTeleportsToCappy),
                omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_CAPPY_CAN_COLLECT_STARS, &gOmmCheatCappyCanCollectStars),
                omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_PLAY_AS_CAPPY, &gOmmCheatPlayAsCappy),
                omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_PEACH_ENDLESS_VIBE_GAUGE, &gOmmCheatPeachEndlessVibeGauge),
            ), 6 + omm_player_is_unlocked(OMM_PLAYER_PEACH)),
            omm_opt_make_submenu(OMM_TEXT_OPT_EXTRAS_LABEL, OMM_TEXT_OPT_EXTRAS_TITLE, options(
#if OMM_CODE_MARIO_COLORS
                omm_opt_make_choice(OMM_TEXT_OPT_MARIO_COLORS, &gOmmExtrasMarioColors, omm_mario_colors_choices(), omm_mario_colors_count()),
#endif
                omm_opt_make_choice(OMM_TEXT_OPT_PEACH_COLORS, &gOmmExtrasPeachColors, omm_mario_colors_choices(), omm_mario_colors_count()),
                omm_opt_make_toggle(OMM_TEXT_OPT_SMO_ANIMATIONS, &gOmmExtrasSMOAnimations),
                omm_opt_make_toggle(OMM_TEXT_OPT_CAPPY_AND_TIARA, &gOmmExtrasCappyAndTiara),
#if !OMM_GAME_IS_SMMS
                omm_opt_make_toggle(OMM_TEXT_OPT_COLORED_STARS, &gOmmExtrasColoredStars),
#endif
                omm_opt_make_toggle(OMM_TEXT_OPT_VANISHING_HUD, &gOmmExtrasVanishingHUD),
                omm_opt_make_toggle(OMM_TEXT_OPT_REVEAL_SECRETS, &gOmmExtrasRevealSecrets),
                omm_opt_make_toggle(OMM_TEXT_OPT_RED_COINS_RADAR, &gOmmExtrasRedCoinsRadar),
                omm_opt_make_toggle(OMM_TEXT_OPT_SHOW_STAR_NUMBER, &gOmmExtrasShowStarNumber),
                omm_opt_make_toggle(OMM_TEXT_OPT_INVISIBLE_MODE, &gOmmExtrasInvisibleMode),
#if OMM_CODE_DEV
                omm_opt_make_toggle(OMM_TEXT_OPT_RENDER96_PEACH, &gOmmExtrasRender96Peach),
#endif
                omm_opt_make_toggle(OMM_TEXT_OPT_CRYSTAL_STARS_REWARD, &gOmmExtrasCrystalStarsReward),
                omm_opt_make_toggle(OMM_TEXT_OPT_NEBULA_STARS_REWARD, &gOmmExtrasNebulaStarsReward),
            ), 8 + OMM_CODE_MARIO_COLORS + !OMM_GAME_IS_SMMS + OMM_CODE_DEV + omm_ssd_is_completed(OMM_SSM_HARD) + omm_ssd_is_completed(OMM_SSM_LUNATIC)),
            omm_opt_make_shortcuts_submenu(OMM_TEXT_OPT_SHORTCUTS_LABEL, OMM_TEXT_OPT_SHORTCUTS_TITLE),
#if OMM_CODE_DEBUG
            omm_opt_make_submenu(OMM_TEXT_OPT_DEBUG_LABEL, OMM_TEXT_OPT_DEBUG_TITLE, options(
                omm_opt_make_toggle(OMM_TEXT_OPT_DEBUG_HITBOX, &gOmmDebugHitbox),
                omm_opt_make_toggle(OMM_TEXT_OPT_DEBUG_HURTBOX, &gOmmDebugHurtbox),
                omm_opt_make_toggle(OMM_TEXT_OPT_DEBUG_WALLBOX, &gOmmDebugWallbox),
                omm_opt_make_toggle(OMM_TEXT_OPT_DEBUG_SURFACE, &gOmmDebugSurface),
                omm_opt_make_toggle(OMM_TEXT_OPT_DEBUG_MARIO, &gOmmDebugMario),
                omm_opt_make_toggle(OMM_TEXT_OPT_DEBUG_CAPPY, &gOmmDebugCappy),
                omm_opt_make_toggle(OMM_TEXT_OPT_DEBUG_PROFILER, &gOmmDebugProfiler),
#if OMM_CODE_DEV_DEBUG
                omm_opt_make_toggle(OMM_TEXT_OPT_DEBUG_GAME_SPEED, &gOmmDebugGameSpeedEnable),
                omm_opt_make_choice(OMM_TEXT_BLANK, &gOmmDebugGameSpeedFps, choices(
                    OMM_TEXT_OPT_DEBUG_GAME_SPEED_30_FPS,
                    OMM_TEXT_OPT_DEBUG_GAME_SPEED_20_FPS,
                    OMM_TEXT_OPT_DEBUG_GAME_SPEED_15_FPS,
                    OMM_TEXT_OPT_DEBUG_GAME_SPEED_10_FPS,
                    OMM_TEXT_OPT_DEBUG_GAME_SPEED_5_FPS),
                5),
#endif
            ), 7 + 2 * OMM_CODE_DEV_DEBUG),
#endif
        ), 10 + OMM_CODE_DEBUG);
    OMM_MEMCPY(gOmmOptMenu, optOmmMenu.label, omm_opt_text_length(optOmmMenu.label) + 1);
    OMM_MEMCPY(gOmmOptMenu + 0x100, optOmmMenu.nextMenu, sizeof(struct SubMenu));

#if !OMM_CODE_DYNOS
    // Warp to level
    omm_opt_init_warp_to_level();
    struct Option optWarpToLevel =
        omm_opt_make_submenu(OMM_TEXT_OPT_WARP_TO_LEVEL_LABEL, OMM_TEXT_OPT_WARP_TO_LEVEL_TITLE, options(
            omm_opt_make_choice_level(OMM_TEXT_OPT_WARP_TO_LEVEL_LEVEL, &sOmmWarpLevel),
            omm_opt_make_choice_area(OMM_TEXT_OPT_WARP_TO_LEVEL_AREA, &sOmmWarpArea),
            omm_opt_make_choice_act(OMM_TEXT_OPT_WARP_TO_LEVEL_ACT, &sOmmWarpAct),
            omm_opt_make_button(OMM_TEXT_OPT_WARP_TO_LEVEL_WARP, (void (*)(struct Option *, s32)) omm_opt_warp_to_level),
        ), 4);
    OMM_MEMCPY(gOmmOptWarpToLevel, optWarpToLevel.label, omm_opt_text_length(optWarpToLevel.label) + 1);
    OMM_MEMCPY(gOmmOptWarpToLevel + 0x100, optWarpToLevel.nextMenu, sizeof(struct SubMenu));
    omm_add_routine(OMM_ROUTINE_TYPE_UPDATE, omm_opt_update_warp_to_level);

    // Return to main menu
    u8 *optReturnToMainMenuLabel = omm_opt_text(OMM_TEXT_OPT_RETURN_TO_MAIN_MENU_LABEL, false);
    OMM_MEMCPY(gOmmOptReturnToMainMenu, optReturnToMainMenuLabel, omm_opt_text_length(optReturnToMainMenuLabel) + 1);
#endif

    // Controls
    struct Option optControls =
        omm_opt_make_submenu(OMM_TEXT_OPT_CONTROLS_TITLE, OMM_TEXT_OPT_CONTROLS_TITLE, options(
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_A_BUTTON, gOmmControlsButtonA),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_B_BUTTON, gOmmControlsButtonB),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_X_BUTTON, gOmmControlsButtonX),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_Y_BUTTON, gOmmControlsButtonY),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_START_BUTTON, gOmmControlsButtonStart),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_L_TRIGGER, gOmmControlsTriggerL),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_R_TRIGGER, gOmmControlsTriggerR),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_Z_TRIGGER, gOmmControlsTriggerZ),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_C_UP, gOmmControlsCUp),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_C_DOWN, gOmmControlsCDown),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_C_LEFT, gOmmControlsCLeft),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_C_RIGHT, gOmmControlsCRight),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_D_UP, gOmmControlsDUp),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_D_DOWN, gOmmControlsDDown),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_D_LEFT, gOmmControlsDLeft),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_D_RIGHT, gOmmControlsDRight),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_STICK_UP, gOmmControlsStickUp),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_STICK_DOWN, gOmmControlsStickDown),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_STICK_LEFT, gOmmControlsStickLeft),
            omm_opt_make_bind(OMM_TEXT_OPT_CONTROLS_STICK_RIGHT, gOmmControlsStickRight),
            omm_opt_make_scroll(OMM_TEXT_OPT_CONTROLS_STICK_DEADZONE, &configStickDeadzone, 0, 100, 1),
            omm_opt_make_button(OMM_TEXT_OPT_CONTROLS_RESET, (void (*)(struct Option *, s32)) omm_opt_reset_controls),
        ), 22);
    OMM_MEMCPY(gOmmOptControls, optControls.label, omm_opt_text_length(optControls.label) + 1);
    OMM_MEMCPY(gOmmOptControls + 0x100, optControls.nextMenu, sizeof(struct SubMenu));

    // Cheats
#if !OMM_GAME_IS_R96A
    struct Option optCheats =
        omm_opt_make_submenu(OMM_TEXT_OPT_CHEATS_TITLE, OMM_TEXT_OPT_CHEATS_TITLE, options(
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_ENABLE, &gOmmCheatEnable),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_MOON_JUMP, &gOmmCheatMoonJump),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_GOD_MODE, &gOmmCheatGodMode),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_INVINCIBLE, &gOmmCheatInvincible),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_SUPER_SPEED, &gOmmCheatSuperSpeed),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_SUPER_RESPONSIVE, &gOmmCheatSuperResponsive),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_NO_FALL_DAMAGE, &gOmmCheatNoFallDamage),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_WALK_ON_LAVA, &gOmmCheatWalkOnLava),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_WALK_ON_QUICKSAND, &gOmmCheatWalkOnQuicksand),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_WALK_ON_WATER, &gOmmCheatWalkOnWater),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_WALK_ON_GAS, &gOmmCheatWalkOnGas),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_WALK_ON_SLOPE, &gOmmCheatWalkOnSlope),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_WALK_ON_DEATH_BARRIER, &gOmmCheatWalkOnDeathBarrier),
            omm_opt_make_toggle(OMM_TEXT_OPT_CHEAT_BLJ_ANYWHERE, &gOmmCheatBljAnywhere),
        ), 14);
    OMM_MEMCPY(gOmmOptCheats, optCheats.label, omm_opt_text_length(optCheats.label) + 1);
    OMM_MEMCPY(gOmmOptCheats + 0x100, optCheats.nextMenu, sizeof(struct SubMenu));
#endif

    // Edit the options menu to include OMM sub-menus
    extern void omm_opt_init_main_menu();
    omm_opt_init_main_menu();
}

//
// Options shortcuts
//

OMM_ROUTINE_PRE_RENDER(omm_opt_update_shortcuts) {
    static const char *sToggleStrings[] = { OMM_TEXT_OPT_DISABLED, OMM_TEXT_OPT_ENABLED };
    static const char *sOptionLabel = NULL;
    static const char **sOptionStrings = NULL;
    static uintptr_t sOption = 0;
    static u8 *sDisplayStrings[4] = { NULL, NULL, NULL, NULL };
    static s32 sDisplayColor = 0;
    static s32 sDisplayTimer = 0;

    // Check shortcuts and change the corresponding option if pressed
    if (!omm_is_main_menu() && !omm_is_game_paused() && gMarioObject) {
        bool changed = false;
#if OMM_GAME_IS_R96A
        u32 keyPressed = gKeyPressed;
#else
        u32 keyPressed = controller_get_raw_key();
#endif
        if (keyPressed != VK_INVALID) {
            for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmOptShortcuts); ++i) {
                const OmmOptShortcut *sc = &sOmmOptShortcuts[i];
                for (s32 j = 0; j != MAX_BINDS; ++j) {
                    if (keyPressed == sc->binds[j]) {
                        switch (sc->type) {
                            case OPT_TOGGLE: {
                                *sc->toggle.option = !(*sc->toggle.option);
                                sOptionLabel = sc->label;
                                sOptionStrings = (const char **) sToggleStrings;
                                sOption = 0 + (((uintptr_t) sc->toggle.option) << 1);
                            } break;

                            case OPT_CHOICE: {
                                *sc->choice.option = (*sc->choice.option + 1) % sc->choice.numChoices;
                                sOptionLabel = sc->label;
                                sOptionStrings = (const char **) sc->choice.choices;
                                sOption = 1 + (((uintptr_t) sc->choice.option) << 1);
                            } break;
                        }
                        changed = true;
                        break;
                    }
                }
            }
        }

        // Must return now if the values changed, to let them update properly before displaying the message box
        if (changed) {
            return;
        }
    }

    // Create the strings to display and reset the timer
    if (sOption && sOptionLabel && sOptionStrings) {
        gSaveFileModified = true;
        play_sound(SOUND_MENU_MARIO_CASTLE_WARP2, gGlobalSoundArgs);
        bool isChoice = (sOption & 1);
        u32 opt = (isChoice ? *((u32 *) (sOption >> 1)) : (u32) *((bool *) (sOption >> 1)));
        OMM_MEMDEL(sDisplayStrings[0]);
        OMM_MEMDEL(sDisplayStrings[1]);
        OMM_MEMDEL(sDisplayStrings[2]);
        OMM_MEMDEL(sDisplayStrings[3]);
        sDisplayStrings[0] = omm_text_convert(OMM_TEXT_OPT_SHORTCUTS_OPTION, true);
        sDisplayStrings[1] = omm_text_convert(sOptionLabel, true);
        sDisplayStrings[2] = omm_text_convert(OMM_TEXT_OPT_SHORTCUTS_SET_TO, true);
        sDisplayStrings[3] = omm_text_convert(sOptionStrings[opt], true);
        sDisplayColor = (isChoice ? 1 : (opt ? 2 : 3));
        sDisplayTimer = 60;
        sOptionLabel = NULL;
        sOptionStrings = NULL;
        sOption = 0;
    }

    // Display the strings
    if (sDisplayTimer-- > 0) {
        Gfx *start = gDisplayListHead;
        gSPDisplayList(gDisplayListHead++, NULL);
        s32 w = omm_render_get_string_width(sDisplayStrings[0]) +
                omm_render_get_string_width(sDisplayStrings[1]) +
                omm_render_get_string_width(sDisplayStrings[2]) +
                omm_render_get_string_width(sDisplayStrings[3]);
        s32 x = (SCREEN_WIDTH - w) / 2;

        // Black box
        static Vtx sOmmHudBlackBoxVertices[4];
        sOmmHudBlackBoxVertices[0] = (Vtx) { { { -(w + 8) / 2, -8, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x60 } } };
        sOmmHudBlackBoxVertices[1] = (Vtx) { { { -(w + 8) / 2, +8, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x60 } } };
        sOmmHudBlackBoxVertices[2] = (Vtx) { { { +(w + 8) / 2, -8, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x60 } } };
        sOmmHudBlackBoxVertices[3] = (Vtx) { { { +(w + 8) / 2, +8, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x60 } } };
        create_dl_translation_matrix(MENU_MTX_PUSH, SCREEN_WIDTH / 2, 27, 0);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gSPVertex(gDisplayListHead++, sOmmHudBlackBoxVertices, 4, 0);
        gSP2Triangles(gDisplayListHead++, 2, 1, 0, 0, 1, 2, 3, 0);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

        // Strings
        s32 colors[4] = { 0, 1, 0, sDisplayColor };
        for (s32 i = 0; i != 4; ++i) {
            u8 r, g, b;
            switch (colors[i]) {
                case 0: r = 0x00, g = 0xE0, b = 0xFF; break;
                case 1: r = 0xFF, g = 0xFF, b = 0xFF; break;
                case 2: r = 0x20, g = 0xE0, b = 0x20; break;
                case 3: r = 0xFF, g = 0x20, b = 0x20; break;
            }
            omm_render_string(x + 1, 22, r / 4, g / 4, b / 4, 0xFF, sDisplayStrings[i], 0);
            omm_render_string(x, 23, r, g, b, 0xFF, sDisplayStrings[i], 0); 
            x += omm_render_get_string_width(sDisplayStrings[i]);
        }
        gSPEndDisplayList(gDisplayListHead++);
        gSPDisplayList(start, gDisplayListHead);
    }
}

#pragma GCC pop_options
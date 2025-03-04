#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

static Scroll sOmmStatsBoardPageScroll = {0};
static Scroll sOmmStatsBoardSubScroll = {0};
InterpData sOmmStatsBoard[1];

#define OMM_STATS_BOARD_FADE            (10)
#define OMM_STATS_BOARD_WIDTH           (256)
#define OMM_STATS_BOARD_HEIGHT          (152)

#define OMM_STATS_BOARD_PAGE_OBJECTS    (0)
#define OMM_STATS_BOARD_PAGE_ACTIONS    (1)
#define OMM_STATS_BOARD_PAGE_DISTANCE   (2)
#define OMM_STATS_BOARD_PAGE_TIME       (3)
#define OMM_STATS_BOARD_PAGE_CAPTURES   (4)
#define OMM_STATS_BOARD_PAGE_REWARDS    (5)
#define OMM_STATS_BOARD_PAGE_SECRETS    (6)
#define OMM_STATS_BOARD_PAGES           (7)

static s32 bhv_omm_stats_board_get_sub_page_count(s32 page) {
    switch (page) {
        case OMM_STATS_BOARD_PAGE_OBJECTS: return 2;
        case OMM_STATS_BOARD_PAGE_ACTIONS: return 2;
        case OMM_STATS_BOARD_PAGE_DISTANCE: return 2;
        case OMM_STATS_BOARD_PAGE_TIME: return 2;
        case OMM_STATS_BOARD_PAGE_CAPTURES: return (omm_level_get_all_available_captures_count(OMM_GAME_MODE) + 15) / 16;
        case OMM_STATS_BOARD_PAGE_REWARDS: return omm_rewards_get_count();
        case OMM_STATS_BOARD_PAGE_SECRETS: return omm_secrets_get_count();
    }
    return 1;
}

static void bhv_omm_stats_board_update_scrolls() {

    // Page
    s32 prevPage = sOmmStatsBoardPageScroll.idx;
    omm_render_update_scroll(&sOmmStatsBoardPageScroll, OMM_STATS_BOARD_PAGES, -gPlayer1Controller->stickY);
    if (sOmmStatsBoardPageScroll.idx != prevPage) {
        sOmmStatsBoardSubScroll.idx = 0;
    }

    // Sub-page
    s32 subPageCount = bhv_omm_stats_board_get_sub_page_count(sOmmStatsBoardPageScroll.idx);
    if (subPageCount > 1) {
        omm_render_update_scroll(&sOmmStatsBoardSubScroll, subPageCount, gPlayer1Controller->stickX);
    }
}

//
// Stats
//

static const u8 OMM_STATS_BOARD_LOCAL_GLOBAL_RGB[2][3] = {
    { 0xFF, 0xFF, 0x80 },
    { 0x80, 0xC0, 0xFF },
};

static const char *bhv_omm_stats_board_get_stat_name_and_value(s32 page, s32 index, const OmmStats *stats, const u64 **value) {
    switch (page) {
        case OMM_STATS_BOARD_PAGE_OBJECTS: switch (index) {
            case 0: *value = &stats->starsCollected; return OMM_TEXT_STATS_STARS;
            case 1: *value = &stats->sparklyStarsCollected; return OMM_TEXT_STATS_SPARKLY_STARS;
            case 2: *value = &stats->coinsCollected; return OMM_TEXT_STATS_COINS;
            case 3: *value = &stats->capsCollected; return OMM_TEXT_STATS_CAPS;
            case 4: *value = &stats->mushrooms1upCollected; return OMM_TEXT_STATS_MUSHROOMS_1UP;
            case 5: *value = &stats->secretsCollected; return OMM_TEXT_STATS_SECRETS;
            case 6: *value = &stats->exclamationBoxesBroken; return OMM_TEXT_STATS_EXCLAMATION_BOXES;
            case 7: *value = &stats->enemiesDefeated; return OMM_TEXT_STATS_ENEMIES;
            case 8: *value = &stats->bowsersDefeated; return OMM_TEXT_STATS_BOWSERS;
        } break;
        case OMM_STATS_BOARD_PAGE_ACTIONS: switch (index) {
            case 0: *value = &stats->aPresses; return OMM_TEXT_STATS_A_PRESSES;
            case 1: *value = &stats->jumps; return OMM_TEXT_STATS_JUMPS;
            case 2: *value = &stats->attacks; return OMM_TEXT_STATS_ATTACKS;
            case 3: *value = &stats->cappyThrows; return OMM_TEXT_STATS_CAPPY_THROWS;
            case 4: *value = &stats->cappyBounces; return OMM_TEXT_STATS_CAPPY_BOUNCES;
            case 5: *value = &stats->captures; return OMM_TEXT_STATS_CAPTURES;
            case 6: *value = &stats->hitsTaken; return OMM_TEXT_STATS_HITS_TAKEN;
            case 7: *value = &stats->restarts; return OMM_TEXT_STATS_RESTARTS;
            case 8: *value = &stats->deaths; return OMM_TEXT_STATS_DEATHS;
        } break;
        case OMM_STATS_BOARD_PAGE_DISTANCE: switch (index) {
            case 0: *value = stats->distanceTotal; return OMM_TEXT_STATS_TOTAL;
            case 1: *value = stats->distanceOnGround; return OMM_TEXT_STATS_ON_GROUND;
            case 2: *value = stats->distanceAirborne; return OMM_TEXT_STATS_AIRBORNE;
            case 3: *value = stats->distanceUnderwater; return OMM_TEXT_STATS_UNDERWATER;
            case 4: *value = stats->distanceWingCap; return OMM_TEXT_STATS_WING_CAP;
            case 5: *value = stats->distanceMetalCap; return OMM_TEXT_STATS_METAL_CAP;
            case 6: *value = stats->distanceVanishCap; return OMM_TEXT_STATS_VANISH_CAP;
        } break;
        case OMM_STATS_BOARD_PAGE_TIME: switch (index) {
            case 0: *value = stats->timeTotal; return OMM_TEXT_STATS_TOTAL;
            case 1: *value = stats->timeOnGround; return OMM_TEXT_STATS_ON_GROUND;
            case 2: *value = stats->timeAirborne; return OMM_TEXT_STATS_AIRBORNE;
            case 3: *value = stats->timeUnderwater; return OMM_TEXT_STATS_UNDERWATER;
            case 4: *value = stats->timeWingCap; return OMM_TEXT_STATS_WING_CAP;
            case 5: *value = stats->timeMetalCap; return OMM_TEXT_STATS_METAL_CAP;
            case 6: *value = stats->timeVanishCap; return OMM_TEXT_STATS_VANISH_CAP;
        } break;
    }
    return NULL;
}

//
// Objects / Actions
//

static void bhv_omm_stats_board_render_page_objects_actions(s16 bgx, s16 bgy, s16 bgw, s16 bgh, f32 t) {
    const OmmStats *stats = (sOmmStatsBoardSubScroll.idx ? gOmmStats : omm_save_file_get_stats(gCurrSaveFileNum - 1, OMM_GAME_MODE));
    if (stats) {
        const u8 *rgb = OMM_STATS_BOARD_LOCAL_GLOBAL_RGB[sOmmStatsBoardSubScroll.idx];
        for (s32 i = 0; i != 9; ++i) {
            const u64 *value;
            const char *nameStr = bhv_omm_stats_board_get_stat_name_and_value(sOmmStatsBoardPageScroll.idx, i, stats, &value);
            if (nameStr) {
                str_t valueStr;
                str_fmt(valueStr, sizeof(valueStr), "%llu", *value);
                const u8 *nameStr64 = omm_text_convert(nameStr, false);
                const u8 *valueStr64 = omm_text_convert(valueStr, false);
                s16 xl = bgx + 8 * t;
                s16 xr = bgx + bgw - (8 * t + omm_render_get_string_width_sized(valueStr64, 8 * t));
                s16 y = bgy + bgh - (i + 1) * 16 * t;
                omm_render_string_sized(xl, y, 8 * t, 8 * t, 0xFF, 0xFF, 0xFF, 0xFF * t, nameStr64, false);
                omm_render_string_sized(xr, y, 8 * t, 8 * t, rgb[0], rgb[1], rgb[2], 0xFF * t, valueStr64, false);
            }
        }
    }
}

//
// Distance / Time
//

static void bhv_omm_stats_board_render_time(f32 t, s16 bgx, s16 bgw, s16 dx, s16 y, const u8 *rgb, u64 value) {
    u64 ms = (1000llu * value) / 30llu;
    u64 s = (ms / 1000llu) % 60;
    u64 m = (ms / 60000llu) % 60;
    u64 h = (ms / 3600000llu);
    str_t valueStr;
    str_fmt(valueStr, sizeof(valueStr), "%llu:%02llu:%02llu", h, m, s);
    u8 *valueStr64 = omm_text_convert(valueStr, false);
    s16 x = bgx + bgw - ((8 + dx) * t + omm_render_get_string_width_sized(valueStr64, 8 * t));
    omm_render_string_sized(x, y, 8 * t, 8 * t, rgb[0], rgb[1], rgb[2], 0xFF * t, valueStr64, false);
    for (u8 *c = valueStr64; *c != 0xFF; ++c) {
        if (*c >= 1 && *c <= 9) {
            *c = 0xFF;
            break;
        }
    }
    omm_render_string_sized(x, y, 8 * t, 8 * t, 0x40, 0x40, 0x40, 0xFF * t, valueStr64, false);
}

static void bhv_omm_stats_board_render_page_distance_time(s16 bgx, s16 bgy, s16 bgw, s16 bgh, f32 t) {
    const OmmStats *stats = (sOmmStatsBoardSubScroll.idx ? gOmmStats : omm_save_file_get_stats(gCurrSaveFileNum - 1, OMM_GAME_MODE));
    if (stats) {
        bool isTime = (sOmmStatsBoardPageScroll.idx == OMM_STATS_BOARD_PAGE_TIME);
        const u8 *rgb = OMM_STATS_BOARD_LOCAL_GLOBAL_RGB[sOmmStatsBoardSubScroll.idx];
        const u8 *titleStr64 = omm_text_convert(isTime ? OMM_TEXT_STATS_TIME : OMM_TEXT_STATS_DISTANCE, false);
        const u8 *column0Str64 = omm_text_convert(OMM_TEXT_STATS_MARIO, false);
        const u8 *column1Str64 = omm_text_convert(OMM_TEXT_STATS_CAPTURE, false);
        s16 xl = bgx + 8 * t;
        s16 xr0 = bgx + bgw - ((8 + OMM_STATS_BOARD_WIDTH / 3) * t + omm_render_get_string_width_sized(column0Str64, 8 * t));
        s16 xr1 = bgx + bgw - (8 * t + omm_render_get_string_width_sized(column1Str64, 8 * t));
        s16 yt0 = bgy + bgh - 16 * t;
        s16 yt1 = bgy + bgh - 32 * t;
        omm_render_string_sized(xl, yt0, 8 * t, 8 * t, 0xFF, 0xFF, 0xFF, 0xFF * t, titleStr64, false);
        omm_render_string_sized(xr0, yt1, 8 * t, 8 * t, 0xFF, 0xFF, 0xFF, 0xFF * t, column0Str64, false);
        omm_render_string_sized(xr1, yt1, 8 * t, 8 * t, 0xFF, 0xFF, 0xFF, 0xFF * t, column1Str64, false);
        for (s32 i = 0; i != 7; ++i) {
            const u64 *value;
            const char *nameStr = bhv_omm_stats_board_get_stat_name_and_value(sOmmStatsBoardPageScroll.idx, i, stats, &value);
            if (nameStr) {
                const u8 *nameStr64 = omm_text_convert(nameStr, false);
                s16 xl = bgx + 8 * t;
                s16 y = bgy + bgh - (i + 3) * 16 * t;
                omm_render_string_sized(xl, y, 8 * t, 8 * t, 0xFF, 0xFF, 0xFF, 0xFF * t, nameStr64, false);
                if (isTime) {
                    bhv_omm_stats_board_render_time(t, bgx, bgw, OMM_STATS_BOARD_WIDTH / 3, y, rgb, value[0]);
                    if (i < 4) { // Don't display cap values for captures
                        bhv_omm_stats_board_render_time(t, bgx, bgw, 0, y, rgb, value[1]);
                    }
                } else {
                    f64 toMeters = 1.55 / 120.0; // Mario's human height (155 cm) / Mario's model height (120 u)
                    str_t value0Str; str_fmt(value0Str, sizeof(value0Str), "%llu", (u64) (value[0] * toMeters));
                    str_t value1Str; str_fmt(value1Str, sizeof(value1Str), "%llu", (u64) (value[1] * toMeters));
                    const u8 *value0Str64 = omm_text_convert(value0Str, false);
                    const u8 *value1Str64 = omm_text_convert(value1Str, false);
                    s16 xr0 = bgx + bgw - ((8 + OMM_STATS_BOARD_WIDTH / 3) * t + omm_render_get_string_width_sized(value0Str64, 8 * t));
                    s16 xr1 = bgx + bgw - (8 * t + omm_render_get_string_width_sized(value1Str64, 8 * t));
                    omm_render_string_sized(xr0, y, 8 * t, 8 * t, rgb[0], rgb[1], rgb[2], 0xFF * t, value0Str64, false);
                    if (i < 4) { // Don't display cap values for captures
                        omm_render_string_sized(xr1, y, 8 * t, 8 * t, rgb[0], rgb[1], rgb[2], 0xFF * t, value1Str64, false);
                    }
                }
            }
        }
    }
}

//
// Captures
//

#define FLAG_NAME(x) { OMM_CAPTURE_##x, OMM_TEXT_CAPTURE_##x }
static struct { u64 flag; const char *name; } OMM_STATS_BOARD_CAPTURES[] = {

    // Page 1
    FLAG_NAME(GOOMBA),
    FLAG_NAME(KOOPA),
    FLAG_NAME(KOOPA_SHELL),
    FLAG_NAME(KOOPA_SHELL_WATER),
    FLAG_NAME(BOBOMB),
    FLAG_NAME(BOBOMB_BUDDY),
    FLAG_NAME(BOBOMB_BUDDY_CANNON),
    FLAG_NAME(CHAIN_CHOMP),
    FLAG_NAME(HOOT),
    FLAG_NAME(WHOMP),
    FLAG_NAME(THWOMP),
    FLAG_NAME(BULLET_BILL),
    FLAG_NAME(SPINDRIFT),
    FLAG_NAME(SMALL_PENGUIN),
    FLAG_NAME(SNOWMAN),
    FLAG_NAME(MR_BLIZZARD),

    // Page 2
    FLAG_NAME(BOO),
    FLAG_NAME(BIG_BOO),
    FLAG_NAME(SCUTTLEBUG),
    FLAG_NAME(MAD_PIANO),
    FLAG_NAME(CRAZY_BOX),
    FLAG_NAME(MR_I),
    FLAG_NAME(SWOOP),
    FLAG_NAME(SNUFIT),
    FLAG_NAME(MONTY_MOLE),
    FLAG_NAME(DORRIE),
    FLAG_NAME(BULLY),
    FLAG_NAME(FLY_GUY),
    FLAG_NAME(POKEY),
    FLAG_NAME(AMP),
    FLAG_NAME(GRINDEL),
    FLAG_NAME(FIRE_SPITTER),

    // Page 3
    FLAG_NAME(CHEEP_CHEEP),
    FLAG_NAME(SUSHI_SHARK),
    FLAG_NAME(MONEY_BAG),
    FLAG_NAME(SKEETER),
    FLAG_NAME(HEAVE_HO),
    FLAG_NAME(CHUCKYA),
    FLAG_NAME(HAT_UKIKI),
    FLAG_NAME(CAGE_UKIKI),
    FLAG_NAME(GOOMBA_HUGE),
    FLAG_NAME(GOOMBA_TINY),
    FLAG_NAME(LAKITU),
    FLAG_NAME(SPINY),
    FLAG_NAME(TOAD),
    FLAG_NAME(MIPS),
    FLAG_NAME(FLAMING_BOBOMB),
    FLAG_NAME(YOSHI),
};

static void bhv_omm_stats_board_render_page_captures(s16 bgx, s16 bgy, s16 bgw, s16 bgh, f32 t) {

    // Capture count
    {
        s32 availableCaptureCount = omm_level_get_all_available_captures_count(OMM_GAME_MODE);
        s32 captureCount = omm_save_file_get_capture_count(gCurrSaveFileNum - 1, OMM_GAME_MODE);
        str_t countStr; str_fmt(countStr, sizeof(countStr), "%d", captureCount);
        str_t totalStr; str_fmt(totalStr, sizeof(totalStr), " / %d", availableCaptureCount);
        const u8 *titleStr64 = omm_text_convert(OMM_TEXT_STATS_CATEGORY_CAPTURES, false);
        const u8 *countStr64 = omm_text_convert(countStr, false);
        const u8 *totalStr64 = omm_text_convert(totalStr, false);
        s16 xl = bgx + 8 * t;
        s16 xr0 = bgx + bgw - (8 * t + omm_render_get_string_width_sized(totalStr64, 8 * t));
        s16 xr1 = xr0 - omm_render_get_string_width_sized(countStr64, 8 * t);
        s16 y = bgy + bgh - 16 * t;
        omm_render_string_sized(xl,  y, 8 * t, 8 * t, 0xFF, 0xFF, 0xFF, 0xFF * t, titleStr64, false);
        omm_render_string_sized(xr0, y, 8 * t, 8 * t, 0xFF, 0xFF, 0xFF, 0xFF * t, totalStr64, false);
        omm_render_string_sized(xr1, y, 8 * t, 8 * t, 0xFF, 0xFF, 0x80, 0xFF * t, countStr64, false);
    }

    // Captures
    u64 availableCaptureFlags = omm_level_get_all_available_captures(OMM_GAME_MODE);
    u64 captureFlags = omm_save_file_get_capture_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE);
    for (s32 i = 0, j = 0, j0 = sOmmStatsBoardSubScroll.idx * 16, j1 = (sOmmStatsBoardSubScroll.idx + 1) * 16; i != OMM_MAX_CAPTURES && j != j1; ++i) {
        u64 flag = OMM_STATS_BOARD_CAPTURES[i].flag;
        if (flag & availableCaptureFlags) {
            if (j >= j0) {
                s16 row = (j - j0) % 8;
                s16 col = (j - j0) / 8;
                const u8 *nameStr64 = omm_text_convert(OMM_STATS_BOARD_CAPTURES[i].name, false);
                s16 x = bgx + (col ? (bgw - (8 * t + omm_render_get_string_width_sized(nameStr64, 8 * t))) : (8 * t));
                s16 y = bgy + bgh - (row + 2) * 16 * t;
                if (flag & captureFlags) {
                    omm_render_string_sized(x, y, 8 * t, 8 * t, 0x00, 0xFF, 0xFF, 0xFF * t, nameStr64, false);
                } else {
                    omm_render_string_sized(x, y, 8 * t, 8 * t, 0x80, 0x80, 0x80, 0xFF * t, nameStr64, false);
                }
            }
            j++;
        }
    }
}

//
// Rewards / Secrets
//

static u32 omm_rewards_get_unlocked_count_global() {
    return omm_rewards_get_unlocked_count(false);
}

static bool omm_rewards_is_unlocked_global(u32 reward) {
    return omm_rewards_is_unlocked(reward, false);
}

static const struct {
    const char *name;
    u32 (*get_count)();
    u32 (*get_unlocked_count)();
    bool (*is_unlocked)(u32);
    bool (*get)(u32, const char **, const char **, const char **);
} OMM_STATS_BOARD_REWARDS_SECRETS[] = {
    {
        OMM_TEXT_STATS_CATEGORY_REWARDS,
        omm_rewards_get_count,
        omm_rewards_get_unlocked_count_global,
        omm_rewards_is_unlocked_global,
        omm_rewards_get,
    },
    {
        OMM_TEXT_STATS_CATEGORY_SECRETS,
        omm_secrets_get_count,
        omm_secrets_get_unlocked_count,
        omm_secrets_is_unlocked,
        omm_secrets_get,
    }
};

static s16 bhv_omm_stats_board_render_multiline_text(s16 x, s16 y0, s16 dy, u8 r, u8 g, u8 b, u8 a, f32 t, u8 *str64) {
    for (;;) {
        u8 *end = memchr(str64, 0xFE, omm_text_length(str64));
        if (end) {
            *end = 0xFF;
            omm_render_string_sized(x, y0 - dy * t, 6 * t, 6 * t, r, g, b, a * t, str64, false);
            str64 = end + 1;
            dy += 9;
        } else {
            omm_render_string_sized(x, y0 - dy * t, 6 * t, 6 * t, r, g, b, a * t, str64, false);
            break;
        }
    }
    return dy;
}

static void bhv_omm_stats_board_render_page_rewards_secrets(s16 bgx, s16 bgy, s16 bgw, s16 bgh, f32 t) {
    bool isSecrets = (sOmmStatsBoardPageScroll.idx == OMM_STATS_BOARD_PAGE_SECRETS);

    // Count
    {
        u32 count = OMM_STATS_BOARD_REWARDS_SECRETS[isSecrets].get_count();
        u32 unlocked = OMM_STATS_BOARD_REWARDS_SECRETS[isSecrets].get_unlocked_count();
        str_t countStr; str_fmt(countStr, sizeof(countStr), " / %d", count);
        str_t unlockedStr; str_fmt(unlockedStr, sizeof(unlockedStr), "%d", unlocked);
        const u8 *nameStr64 = omm_text_convert(OMM_STATS_BOARD_REWARDS_SECRETS[isSecrets].name, false);
        const u8 *countStr64 = omm_text_convert(countStr, false);
        const u8 *unlockedStr64 = omm_text_convert(unlockedStr, false);
        s16 xl = bgx + 8 * t;
        s16 xr0 = bgx + bgw - (8 * t + omm_render_get_string_width_sized(countStr64, 8 * t));
        s16 xr1 = xr0 - omm_render_get_string_width_sized(unlockedStr64, 8 * t);
        s16 y = bgy + bgh - 16 * t;
        omm_render_string_sized(xl,  y, 8 * t, 8 * t, 0xFF, 0xFF, 0xFF, 0xFF * t, nameStr64, false);
        omm_render_string_sized(xr0, y, 8 * t, 8 * t, 0xFF, 0xFF, 0xFF, 0xFF * t, countStr64, false);
        omm_render_string_sized(xr1, y, 8 * t, 8 * t, 0xFF, 0xFF, 0x80, 0xFF * t, unlockedStr64, false);
    }

    // Current
    const char *name, *cond, *text;
    if (OMM_STATS_BOARD_REWARDS_SECRETS[isSecrets].get(sOmmStatsBoardSubScroll.idx, &name, &cond, &text)) {
        bool unlocked = OMM_STATS_BOARD_REWARDS_SECRETS[isSecrets].is_unlocked(sOmmStatsBoardSubScroll.idx);
        s16 xl = bgx + 8 * t;
        s16 xr = bgx + bgw - 8 * t;
        s16 y0 = bgy + bgh;
        s16 dy = 32;

        // Name
        str_t numberStr; str_fmt(numberStr, sizeof(numberStr), "-%02d- ", sOmmStatsBoardSubScroll.idx + 1);
        const u8 *numberStr64 = omm_text_convert(numberStr, false);
        if (unlocked) {
            omm_render_string_sized(xl, y0 - dy * t, 8 * t, 8 * t, 0x80, 0xFF, 0x80, 0xFF * t, numberStr64, false);
            omm_render_string_sized(xl + 32, y0 - dy * t, 8 * t, 8 * t, 0x80, 0xFF, 0x80, 0xFF * t, omm_text_convert(name, false), false);
        } else {
            omm_render_string_sized(xl, y0 - dy * t, 8 * t, 8 * t, 0x80, 0x80, 0x80, 0xFF * t, numberStr64, false);
            omm_render_string_sized(xl + 32, y0 - dy * t, 8 * t, 8 * t, 0x80, 0x80, 0x80, 0xFF * t, omm_text_convert(OMM_TEXT_UNKNOWN, false), false);
        }

        // Condition
        u8 *condStr64 = omm_text_convert(cond, false);
        dy = bhv_omm_stats_board_render_multiline_text(xl, y0, dy + 18, 0xC0, 0xC0, 0xC0, 0xFF, t, condStr64);

        // Text
        if (unlocked && *text) {
            omm_render_texrect(xl, y0 - (dy + 8) * t, (xr - xl), 1, 32, 32, 0x60, 0x60, 0x60, 0xFF * t, OMM_TEXTURE_MISC_WHITE, false);
            u8 *textStr64 = omm_text_convert(text, false);
            dy = bhv_omm_stats_board_render_multiline_text(xl, y0, dy + 21, 0xF0, 0xF0, 0xF0, 0xFF, t, textStr64);
        }
    }
}

//
// Render
//

Gfx *omm_render_stats_board(Gfx *pos, Gfx *gfx, f32 t) {
    gSPDisplayList(pos, pos + 3);
    gSPDisplayList(pos + 1, gfx);
    gSPEndDisplayList(pos + 2);
    pos += 3;

    // Render board
    Gfx *displayListHead = gDisplayListHead;
    gDisplayListHead = gfx;
    if (t > 0) {

        // Black background
        s16 bgw = OMM_STATS_BOARD_WIDTH * t;
        s16 bgh = OMM_STATS_BOARD_HEIGHT * t;
        s16 bgx = (SCREEN_WIDTH - bgw) / 2;
        s16 bgy = (SCREEN_HEIGHT - bgh) / 2;
        omm_render_texrect(bgx, bgy, bgw, bgh, 32, 32, 0x00, 0x00, 0x00, 0xC0 * t, OMM_TEXTURE_MISC_WHITE, false);

        // Vertical arrows
        s16 aw = 8 * t;
        s16 ah = 8 * t;
        s16 vaxl = (SCREEN_WIDTH) / 2 - aw;
        s16 vaxr = (SCREEN_WIDTH) / 2;
        s16 vayu = (SCREEN_HEIGHT + bgh) / 2 + 4 * t;
        s16 vayd = (SCREEN_HEIGHT - bgh) / 2 - 4 * t - ah;
        omm_render_texrect(vaxl, vayu, aw, ah, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF * t, OMM_TEXTURE_MISC_WHITE_DOWN_RIGHT, false);
        omm_render_texrect(vaxr, vayu, aw, ah, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF * t, OMM_TEXTURE_MISC_WHITE_DOWN_LEFT, false);
        omm_render_texrect(vaxl, vayd, aw, ah, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF * t, OMM_TEXTURE_MISC_WHITE_UP_RIGHT, false);
        omm_render_texrect(vaxr, vayd, aw, ah, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF * t, OMM_TEXTURE_MISC_WHITE_UP_LEFT, false);

        // Horizontal arrows
        if (bhv_omm_stats_board_get_sub_page_count(sOmmStatsBoardPageScroll.idx) > 1) {
            s16 haxl = (SCREEN_WIDTH - bgw) / 2 - 4 * t - aw;
            s16 haxr = (SCREEN_WIDTH + bgw) / 2 + 4 * t;
            s16 hayu = (SCREEN_HEIGHT / 2);
            s16 hayd = (SCREEN_HEIGHT / 2) - ah;
            omm_render_texrect(haxl, hayu, aw, ah, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF * t, OMM_TEXTURE_MISC_WHITE_DOWN_RIGHT, false);
            omm_render_texrect(haxl, hayd, aw, ah, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF * t, OMM_TEXTURE_MISC_WHITE_UP_RIGHT, false);
            omm_render_texrect(haxr, hayu, aw, ah, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF * t, OMM_TEXTURE_MISC_WHITE_DOWN_LEFT, false);
            omm_render_texrect(haxr, hayd, aw, ah, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF * t, OMM_TEXTURE_MISC_WHITE_UP_LEFT, false);
        }

        // Current page
        switch (sOmmStatsBoardPageScroll.idx) {
            case OMM_STATS_BOARD_PAGE_OBJECTS:
            case OMM_STATS_BOARD_PAGE_ACTIONS: {
                bhv_omm_stats_board_render_page_objects_actions(bgx, bgy, bgw, bgh, t);
            } break;

            case OMM_STATS_BOARD_PAGE_DISTANCE:
            case OMM_STATS_BOARD_PAGE_TIME: {
                bhv_omm_stats_board_render_page_distance_time(bgx, bgy, bgw, bgh, t);
            } break;

            case OMM_STATS_BOARD_PAGE_CAPTURES: {
                bhv_omm_stats_board_render_page_captures(bgx, bgy, bgw, bgh, t);
            } break;

            case OMM_STATS_BOARD_PAGE_REWARDS:
            case OMM_STATS_BOARD_PAGE_SECRETS: {
                bhv_omm_stats_board_render_page_rewards_secrets(bgx, bgy, bgw, bgh, t);
            } break;
        }
    }
    gSPEndDisplayList(gDisplayListHead++);
    gDisplayListHead = displayListHead;
    return pos;
}

OMM_ROUTINE_PRE_RENDER(bhv_omm_stats_board_render) {
    struct Object *activeBoard = obj_get_first_with_behavior_and_field_s32(bhvOmmStatsBoard, _FIELD(oAction), 1);
    if (activeBoard) {

        // Interpolate and render
        f32 t = sqr_f(relerp_0_1_f(abs_f(activeBoard->oSubAction), 0, OMM_STATS_BOARD_FADE, 0, 1));
        Gfx *gfx = omm_alloc_gfx(0x1000);
        interp_data_update(sOmmStatsBoard, t != 0, gDisplayListHead, gfx, NULL, 0, 0, 0, 0, 0, t);
        gDisplayListHead = omm_render_stats_board(gDisplayListHead, sOmmStatsBoard->gfx, sOmmStatsBoard->t0);

        // Inputs
        if (t == 1) {

            // Change the current page
            bhv_omm_stats_board_update_scrolls();

            // Close the board
            if (gPlayer1Controller->buttonPressed & (A_BUTTON | B_BUTTON)) {
                activeBoard->oSubAction = -OMM_STATS_BOARD_FADE;
                return;
            }
        }
        return;
    }
    interp_data_reset(sOmmStatsBoard);
}

//
// Behavior
//

static void bhv_omm_stats_board_update() {
    struct Object *o = gCurrentObject;
    o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    switch (o->oAction) {

        // Closed
        case 0: {
            o->oIntangibleTimer = 0;
            obj_load_collision_model(o);
        } break;

        // Open
        case 1: {
            if (++(o->oSubAction) == 0) {
                mem_zero(&sOmmStatsBoardPageScroll, sizeof(sOmmStatsBoardPageScroll));
                mem_zero(&sOmmStatsBoardSubScroll, sizeof(sOmmStatsBoardSubScroll));
                o->oAction = 0;
            }
        } break;
    }
}

const BehaviorScript bhvOmmStatsBoard[] = {
    OBJ_TYPE_SURFACE,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_LOAD_COLLISION_DATA(wooden_signpost_seg3_collision_0302DD80),
    BHV_SET_INTERACT_TYPE(INTERACT_TEXT),
    BHV_SET_INT(oInteractionSubtype, INT_SUBTYPE_SIGN),
    BHV_DROP_TO_FLOOR(),
    BHV_SET_HITBOX(150, 80),
    BHV_BEGIN_LOOP(),
        BHV_SET_INT(oIntangibleTimer, -1),
        BHV_CALL_NATIVE(bhv_omm_stats_board_update),
        BHV_SET_INT(oInteractStatus, 0),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_stats_board(struct Object *o, f32 x, f32 y, f32 z, s16 yaw) {
    struct Object *board = obj_spawn_from_geo(o, wooden_signpost_geo, bhvOmmStatsBoard);
    board->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    obj_set_xyz(board, x, y, z);
    obj_set_angle(board, 0, yaw, 0);
    obj_set_scale(board, 1, 1, 1);
    board->oAction = 0;
    board->oSubAction = 0;
    return board;
}

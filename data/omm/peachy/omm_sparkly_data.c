#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

// Helper macros to make Sparkly Stars data easier to read and write
#define HINT_NEVER          OMM_SPARKLY_HINT_NEVER
#define HINT_ALWAYS         OMM_SPARKLY_HINT_ALWAYS
#define HINT_AREA_1         OMM_SPARKLY_HINT_AREA_1
#define HINT_AREA_2         OMM_SPARKLY_HINT_AREA_2
#define HINT_AREA_3         OMM_SPARKLY_HINT_AREA_3
#define HINT_AREA_4         OMM_SPARKLY_HINT_AREA_4
#define COINS(value)        OMM_SPARKLY_DATA_SET_COINS(value)
#define RED_COINS(value)    OMM_SPARKLY_DATA_SET_RED_COINS(value)
#define NO_BUTTON(value)    OMM_SPARKLY_DATA_SET_BUTTONS(value)
#define MUSHROOMS(value)    OMM_SPARKLY_DATA_SET_MUSHROOMS(value)
#define CONDITION           OMM_SPARKLY_DATA_CONDITION
#define COIN_Y              OMM_SPARKLY_DATA_ONLY_COIN_Y
#define COIN_R              OMM_SPARKLY_DATA_ONLY_COIN_R
#define COIN_B              OMM_SPARKLY_DATA_ONLY_COIN_B
#define NO_MUSHROOM         OMM_SPARKLY_DATA_NO_MUSHROOM
#define NO_CAPTURE          OMM_SPARKLY_DATA_NO_CAPTURE
#define NO_VIBE             OMM_SPARKLY_DATA_NO_VIBE
#define NO_VIBE_AUTO_REFILL OMM_SPARKLY_DATA_NO_VIBE_AUTO_REFILL
#define WING_CAP            OMM_SPARKLY_DATA_WING_CAP
#define VANISH_CAP          OMM_SPARKLY_DATA_VANISH_CAP
#define METAL_CAP           OMM_SPARKLY_DATA_METAL_CAP
#define SHARDS              OMM_SPARKLY_DATA_SHARDS
#define DARK_MODE           OMM_SPARKLY_DATA_DARK_MODE
#define FLAMES              OMM_SPARKLY_DATA_FLAMES
#define BOXES               OMM_SPARKLY_DATA_BOXES
#define RINGS               OMM_SPARKLY_DATA_RINGS
#define ENEMIES             OMM_SPARKLY_DATA_ENEMIES
#define HEALTH_3            OMM_SPARKLY_DATA_3_HEALTH

const OmmSparklyData gOmmSparklyData[4][OMM_SPARKLY_STARS_MAX] = {
// Disabled
{
},
// Normal mode: Pink-Gold Stars
{
{  0,  0, OMM_GAME_SM64, LEVEL_BOB,     1,   4360,   4500,   1230, -5200,     0,  6750, 0xC000, HINT_AREA_1,     0, CONDITION },
{  1,  1, OMM_GAME_SM64, LEVEL_WF,      1,      0,   6250,      0,  3075,   256,  5245, 0xC000, HINT_AREA_1,     0, CONDITION | COINS(90) | COIN_Y },
{  2,  2, OMM_GAME_SM64, LEVEL_JRB,     1,   5000,   2500,   2500, -6060,  1126,   940, 0xA000, HINT_AREA_1,     0, CONDITION | COINS(80) },
{  3,  3, OMM_GAME_SM64, LEVEL_CCM,     2,  -4880,   4800,   5000, -5730,  6656, -5665, 0x8000, HINT_AREA_2,     0, 0 },
{  4,  4, OMM_GAME_SM64, LEVEL_BBH,     1,    666,   4000,    260,   916,  -204,  3064, 0x0000, HINT_AREA_1,     0, CONDITION | COINS(100) | COIN_B },
{  5,  5, OMM_GAME_SM64, LEVEL_HMC,     1,   6000,   4500,   6200, -7100,  2048,  5900, 0x2000, HINT_AREA_1,     0, 0 },
{  6,  6, OMM_GAME_SM64, LEVEL_LLL,     2,   2530,   5500,   -900,  -750,   103, -1225, 0xE000, HINT_NEVER,      0, CONDITION | RED_COINS(8) | SHARDS },
{  7,  7, OMM_GAME_SM64, LEVEL_SSL,     1,  -2050,   5200,  -1020,   200,    38,  6530, 0x4000, HINT_AREA_1,     0, 0 },
{  8,  8, OMM_GAME_SM64, LEVEL_DDD,     2,   3900,   2400,  -4000,  4700,   110,  6100, 0x8000, HINT_AREA_1,     0, 0 },
{  9,  9, OMM_GAME_SM64, LEVEL_SL,      1,      0,   6000,      0,  2035,  1023,   475, 0x6000, HINT_AREA_1,     0, CONDITION },
{ 10, 10, OMM_GAME_SM64, LEVEL_WDW,     1,  -1075,   8300,   1280,  1400,     0,  -700, 0x4000, HINT_AREA_1,     0, 0 },
{ 11, 11, OMM_GAME_SM64, LEVEL_TTM,     2,  -8465,   7650, -14670,  6300, 18781,  6700, 0x2000, HINT_AREA_2,     0, 0 },
{ 12, 12, OMM_GAME_SM64, LEVEL_THI,     2,  -1900,   -400,   1800, -1700,  -890,  2250, 0xC000, HINT_AREA_2,     0, CONDITION },
{ 13, 13, OMM_GAME_SM64, LEVEL_TTC,     1,      0,   4800,      0,   800, -4822,   140, 0x6000, HINT_AREA_1,  1800, CONDITION | BOXES },
{ 14, 14, OMM_GAME_SM64, LEVEL_RR,      1,   5280,   4940,    590,  3100, -1833,  2070, 0xC000, HINT_AREA_1,     0, CONDITION },
{ 15, 15, OMM_GAME_SM64, LEVEL_BITDW,   1,  -7440,  -2600,   3900, -7440, -3153,  3530, 0x0000, HINT_NEVER,      0, CONDITION | RED_COINS(8) },
{ 16, 16, OMM_GAME_SM64, LEVEL_BITFS,   1,   6740,   3333,     90, -7577, -2764,  -360, 0x0000, HINT_AREA_1,     0, CONDITION | MUSHROOMS(7) },
{ 17, 17, OMM_GAME_SM64, LEVEL_BITS,    1,    360,   6600,  -3220, -7360, -4812,     0, 0x4000, HINT_AREA_1,  1800, CONDITION | COINS(75) },
{ 18, 18, OMM_GAME_SM64, LEVEL_TOTWC,   1,      0,  19950,      0, -1000, -2047,     0, 0x4000, HINT_AREA_1,  1800, CONDITION | COINS(63) | WING_CAP },
{ 19, 19, OMM_GAME_SM64, LEVEL_VCUTM,   1,  -2300,   6300,  -6400, -6400,  5734, -6140, 0x4000, HINT_AREA_1,  1500, CONDITION | VANISH_CAP },
{ 20, 20, OMM_GAME_SM64, LEVEL_COTMC,   1,      0,   1500,  -7300, -4000,    20,  -333, 0xE000, HINT_ALWAYS,  1200, METAL_CAP | NO_CAPTURE },
{ 21, 21, OMM_GAME_SM64, LEVEL_PSS,     1,  -6360,  -4100,   3150,  3566,  6144, -6100, 0x4000, HINT_AREA_1,  -720, CONDITION | COINS(50) | NO_BUTTON(A_BUTTON | B_BUTTON | START_BUTTON | Z_TRIG | X_BUTTON | Y_BUTTON) | COIN_Y },
{ 22, 22, OMM_GAME_SM64, LEVEL_WMOTR,   1,  -3200,   5000,  -4500,  -410,  1669,   326, 0x6000, HINT_AREA_1,     0, CONDITION | RED_COINS(8) },
{ 23, 23, OMM_GAME_SM64, LEVEL_SA,      1,      0,  -2200,      0, -9000,     0,     0, 0x0000, HINT_AREA_1,     0, CONDITION | RED_COINS(8) },
{ 24, 24, OMM_GAME_SM64, LEVEL_GROUNDS, 1,      0,   9400,  -4020,     0,   800,  1400, 0x0000, HINT_NEVER,      0, 0 },
{ 25, 25, OMM_GAME_SM64, LEVEL_CASTLE,  1,  -1024,    250,    717, -1350,     0,     0, 0x0000, HINT_NEVER,      0, CONDITION | RED_COINS(8) | SHARDS },
{ 26, 26, OMM_GAME_SM64, LEVEL_CASTLE,  3,   7050,  -1000,   -512,  -800, -1279,  2592, 0x8000, HINT_NEVER,      0, CONDITION },
{ 27, 27, OMM_GAME_SM64, LEVEL_CASTLE,  2,   -205,   3900,   6750,  3975,  1408,  1830, 0xC000, HINT_NEVER,      0, CONDITION | RED_COINS(8) | SHARDS },
{ 28, 28, OMM_GAME_SM64, LEVEL_COURT,   1,      0,    300,    960,     0,     0, -3590, 0x0000, HINT_NEVER,      0, CONDITION },
{ 29, 29, OMM_GAME_SM64, LEVEL_GROUNDS, 2,      0,      0,      0,     0,     0,     0, 0x0000, HINT_NEVER,      0, CONDITION },
},
// Hard mode: Crystal Stars
{
{  0,  0, OMM_GAME_SM64, LEVEL_BOB,     1,  -1506,   7000,   1250, -5200,     0,  6750, 0xC000, HINT_AREA_1,     0, CONDITION },
{  1,  1, OMM_GAME_SM64, LEVEL_WF,      1,      0,   1000,      0,  3075,   256,  5245, 0xC000, HINT_AREA_1,  3150, CONDITION | COINS(100) | COIN_Y },
{  2,  2, OMM_GAME_SM64, LEVEL_JRB,     1,  -1080,  19500,   7070, -6060,  1126,   940, 0xA000, HINT_AREA_1,     0, CONDITION | COINS(101) | DARK_MODE },
{  3,  3, OMM_GAME_SM64, LEVEL_CCM,     1,    -55,   9100,   -150,  2560,  2560, -1615, 0x8000, HINT_AREA_1,     0, 0 },
{  4,  4, OMM_GAME_SM64, LEVEL_BBH,     1,    666,   4000,    260,   916,  -204,  3064, 0x0000, HINT_AREA_1,  3900, CONDITION | COINS(100) | COIN_B },
{  5,  5, OMM_GAME_SM64, LEVEL_HMC,     1,  -7500,   2500,   7500, -7100,  2048,  5900, 0x2000, HINT_AREA_1,  4500, CONDITION | FLAMES },
{  6,  6, OMM_GAME_SM64, LEVEL_LLL,     2,      0,   7000,      0,  -750,   103, -1225, 0xE000, HINT_NEVER,      0, CONDITION | RED_COINS(8) | SHARDS },
{  7,  7, OMM_GAME_SM64, LEVEL_SSL,     2,      0,   5650,    256,   600,     0,  4500, 0xC000, HINT_AREA_2,   450, CONDITION | COINS(62) },
{  8,  8, OMM_GAME_SM64, LEVEL_DDD,     2,   3900,   4000,   6000,  4700,   110,  6100, 0x8000, HINT_AREA_1,     0, 0 },
{  9,  9, OMM_GAME_SM64, LEVEL_SL,      1,      0,   5700,      0,  2035,  1023,   475, 0x6000, HINT_AREA_1,     0, CONDITION | COINS(85) | NO_CAPTURE },
{ 10, 10, OMM_GAME_SM64, LEVEL_WDW,     0,      0,      0,      0,  1400,     0,  -700, 0x4000, HINT_AREA_1,     0, CONDITION },
{ 11, 11, OMM_GAME_SM64, LEVEL_TTM,     2, -16240,  13850, -16240,  6300, 18781,  6700, 0x2000, HINT_AREA_2,     0, 0 },
{ 12, 12, OMM_GAME_SM64, LEVEL_THI,     1,      0,   8765,  -1540, -7800, -2969,  7800, 0xE000, HINT_AREA_1,     0, CONDITION },
{ 13, 13, OMM_GAME_SM64, LEVEL_TTC,     1,      0,   8000,      0,   800, -4822,   140, 0x6000, HINT_AREA_1,  1050, 0 },
{ 14, 14, OMM_GAME_SM64, LEVEL_RR,      1,   5120,   6500,   3325,  3100, -1833,  2070, 0xC000, HINT_AREA_1,  2850, CONDITION | MUSHROOMS(9) | NO_CAPTURE },
{ 15, 15, OMM_GAME_SM64, LEVEL_BITDW,   1,  -7440,  -2048,   3900, -7440, -3153,  3530, 0x0000, HINT_NEVER,   3000, CONDITION },
{ 16, 16, OMM_GAME_SM64, LEVEL_BITFS,   1,   1240,   8000,     90, -7577, -2764,  -360, 0x0000, HINT_AREA_1,     0, CONDITION | HEALTH_3 },
{ 17, 17, OMM_GAME_SM64, LEVEL_BITS,    1,  -6969,  -4000,      0, -7360, -4812,     0, 0x4000, HINT_NEVER,   3300, CONDITION | ENEMIES },
{ 18, 18, OMM_GAME_SM64, LEVEL_TOTWC,   1,      0,  19950,      0, -1000, -2047,     0, 0x4000, HINT_AREA_1,  1050, CONDITION | COINS(63) | WING_CAP },
{ 19, 19, OMM_GAME_SM64, LEVEL_VCUTM,   1,  -2300,   6400,  -6400, -6400,  5734, -6140, 0x4000, HINT_AREA_1,  1200, CONDITION | VANISH_CAP },
{ 20, 20, OMM_GAME_SM64, LEVEL_COTMC,   1,      0,   4000,  -7450, -4000,    20,  -333, 0xE000, HINT_ALWAYS,   900, METAL_CAP | NO_CAPTURE },
{ 21, 21, OMM_GAME_SM64, LEVEL_PSS,     1,  -6360,  -4400,   3450,  3566,  6144, -6100, 0x4000, HINT_AREA_1,  -660, CONDITION | COINS(50) | NO_BUTTON(A_BUTTON | B_BUTTON | START_BUTTON | Z_TRIG | X_BUTTON | Y_BUTTON) | COIN_Y },
{ 22, 22, OMM_GAME_SM64, LEVEL_WMOTR,   1,    200,   7100,  -2340,  -410,  1669,   326, 0x6000, HINT_AREA_1,     0, CONDITION | COINS(56) },
{ 23, 23, OMM_GAME_SM64, LEVEL_SA,      1,      0,  -2200,      0, -9000,     0,     0, 0x0000, HINT_AREA_1,     0, CONDITION | COINS(56) | NO_BUTTON(X_BUTTON) },
{ 24, 24, OMM_GAME_SM64, LEVEL_GROUNDS, 1,      0,  10000,  -4020,     0,   800,  1400, 0x0000, HINT_NEVER,      0, 0 },
{ 25, 25, OMM_GAME_SM64, LEVEL_CASTLE,  1,  -6960,   1080,  -5770, -1350,     0,     0, 0x0000, HINT_NEVER,      0, 0 },
{ 26, 26, OMM_GAME_SM64, LEVEL_CASTLE,  3,  -1509,   -900,  -1564,  -800, -1279,  2592, 0x8000, HINT_NEVER,      0, CONDITION },
{ 27, 27, OMM_GAME_SM64, LEVEL_CASTLE,  2,   7350,   1750,   1830,  3975,  1408,  1830, 0xC000, HINT_NEVER,      0, 0 },
{ 28, 28, OMM_GAME_SM64, LEVEL_COURT,   1,      9,   3800,  -1735,     0,     0, -3590, 0x0000, HINT_NEVER,      0, CONDITION },
{ 29, 29, OMM_GAME_SM64, LEVEL_GROUNDS, 2,      0,      0,      0,     0,     0,     0, 0x0000, HINT_NEVER,      0, CONDITION },
},
// Lunatic mode: Nebula Stars
{
{  0,  0, OMM_GAME_SM64, LEVEL_BOB,     1,   3300,   5300,  -4600, -5200,     0,  6750, 0xC000, 1, -3600, CONDITION },
{  1,  1, OMM_GAME_SMMS, LEVEL_BBH,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{  2,  2, OMM_GAME_SM64, LEVEL_JRB,     1,  -6666,  19950,      0, -6060,  1126,   940, 0xA000, 1,     0, CONDITION | COINS(101) | NO_VIBE_AUTO_REFILL },
{  3,  3, OMM_GAME_SM64, LEVEL_CCM,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{  4,  4, OMM_GAME_SMSR, LEVEL_TTM,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{  5,  5, OMM_GAME_SM74, LEVEL_HMC,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{  6,  6, OMM_GAME_SMSR, LEVEL_LLL,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{  7,  7, OMM_GAME_SM74, LEVEL_SSL,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{  8,  8, OMM_GAME_SMSR, LEVEL_DDD,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{  9,  9, OMM_GAME_SM74, LEVEL_WDW,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 10, 10, OMM_GAME_SM74, LEVEL_TTM,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 11, 11, OMM_GAME_SM64, LEVEL_TTM,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 12, 12, OMM_GAME_SMSR, LEVEL_TTC,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 13, 13, OMM_GAME_SMSR, LEVEL_RR,      1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 14, 14, OMM_GAME_SM74, LEVEL_RR,      2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 15, 15, OMM_GAME_SM64, LEVEL_BITS,    1,    350,  11000,   8200, -7360, -4812,     0, 0x4000, 1,  5400, CONDITION | NO_VIBE_AUTO_REFILL | RINGS },
{ 16, 16, OMM_GAME_SMSR, LEVEL_BITS,    1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 17, 17, OMM_GAME_SM74, LEVEL_BITS,    2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 18, 18, OMM_GAME_SMSR, LEVEL_TOTWC,   1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION | WING_CAP },
{ 19, 19, OMM_GAME_SMMS, LEVEL_LLL,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION | VANISH_CAP },
{ 20, 20, OMM_GAME_SM74, LEVEL_COTMC,   2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION | METAL_CAP },
{ 21, 21, OMM_GAME_SM64, LEVEL_PSS,     1,  -6360,  -4350,   3435,  3566,  6144, -6100, 0x4000, 1,  -630, CONDITION | COINS(50) | NO_BUTTON(A_BUTTON | B_BUTTON | START_BUTTON | Z_TRIG | X_BUTTON | Y_BUTTON) | COIN_Y },
{ 22, 22, OMM_GAME_SMSR, LEVEL_PSS,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 23, 23, OMM_GAME_SM74, LEVEL_PSS,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 24, 24, OMM_GAME_SM64, LEVEL_GROUNDS, 1,      0,  16500,  -4020,     0,   800,  1400, 0x0000, 0,     0, 0 },
{ 25, 25, OMM_GAME_SM74, LEVEL_WMOTR,   2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 26, 26, OMM_GAME_SMSR, LEVEL_CASTLE,  1,      0,  19000,      0,     0,     0,     0, 0x0000, 1,     0, 0 },
{ 27, 27, OMM_GAME_SMSR, 39,            1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 28, 28, OMM_GAME_SM74, LEVEL_SA,      2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0, CONDITION },
{ 29, 29, OMM_GAME_SM64, LEVEL_GROUNDS, 2,      0,      0,      0,     0,     0,     0, 0x0000, 0,     0, CONDITION },
},
};

s32 omm_sparkly_get_index(s32 mode, s32 level, s32 area) {
    for (s32 i = 0; i != omm_sparkly_get_bowser_4_index(mode); ++i) {
        if (gOmmSparklyData[mode][i].game == OMM_GAME_TYPE &&
            gOmmSparklyData[mode][i].level == level && (
            gOmmSparklyData[mode][i].area == 0 ||
            gOmmSparklyData[mode][i].area == area)) {
            return i;
        }
    }
    return -1;
}

static const u8 *omm_sparkly_get_star_strings(s32 mode, s32 index, s32 *lp, s32 *sp, s32 *ep) {
    
    // Mode check
    // Only Normal, Hard and Lunatic are allowed
    if (mode < 1 || mode >= OMM_SPARKLY_MODE_COUNT) {
        return NULL;
    }

    // Index check
    // Out of bounds indexes are not allowed
    if (index < 0 || index > omm_sparkly_get_bowser_4_index(mode)) {
        return NULL;
    }

    // Game check
    // Don't return strings from a different game
    if (gOmmSparklyData[mode][index].game != OMM_GAME_TYPE) {
        return NULL;
    }

    // String table
    const u8 *strTable = omm_dialog_get_entry(NULL, OMM_DIALOG_SPARKLY_STAR_NAMES, mode)->str;
    if (!strTable) {
        return NULL;
    }

    // String positions
    // "level$star\n"
    *lp = 0;
    for (s32 i = 0, j = gOmmSparklyData[mode][index].dialog;; ++i) {
        if (strTable[i] == 0xFE) {
            j--;
            if (j == 0) {
                *lp = i + 1;
            } else if (j == -1) {
                *ep = i;
                break;
            }
        } else if (strTable[i] == 0xF9 /* $ char */ && j == 0) {
            *sp = i + 1;
        }
    }
    return strTable;
}

u8 *omm_sparkly_get_level_name(s32 mode, s32 index) {
    s32 lp, sp, ep;
    const u8 *strings = omm_sparkly_get_star_strings(mode, index, &lp, &sp, &ep);
    if (strings) {
        static u8 sLevelName[0x100];
        omm_set(sLevelName, 0xFF, sizeof(sLevelName));
        omm_copy(sLevelName, strings + lp, min_s(sizeof(sLevelName) - 1, sp - lp - 1));
        return sLevelName;
    }
    return NULL;
}

u8 *omm_sparkly_get_star_name(s32 mode, s32 index) {
    s32 lp, sp, ep;
    const u8 *strings = omm_sparkly_get_star_strings(mode, index, &lp, &sp, &ep);
    if (strings) {
        static u8 sStarName[0x100];
        omm_set(sStarName, 0xFF, sizeof(sStarName));
        omm_copy(sStarName, strings + sp, min_s(sizeof(sStarName) - 1, ep - sp));
        return sStarName;
    }
    return NULL;
}

s32 omm_sparkly_get_collected_count(s32 mode) {
    s32 count = 0;
    for (s32 i = 0; i != omm_sparkly_get_bowser_4_index(mode); ++i) {
        count += omm_sparkly_is_star_collected(mode, i);
    }
    return count + omm_sparkly_is_grand_star_collected(mode);
}

s32 omm_sparkly_get_bowser_4_index(s32 mode) {
    switch (mode) {
    case OMM_SPARKLY_MODE_NORMAL:  return 29;
    case OMM_SPARKLY_MODE_HARD:    return 29;
    case OMM_SPARKLY_MODE_LUNATIC: return 89;
    default:                       return 0;
    }
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

// Helper macros to make Sparkly Stars data easier to read and write
#define COINS(value)        OMM_SSD_SET_COINS(value)
#define RED_COINS(value)    OMM_SSD_SET_RED_COINS(value)
#define NO_BUTTON(value)    OMM_SSD_SET_BUTTONS(value)
#define MUSHROOMS(value)    OMM_SSD_SET_MUSHROOMS(value)
#define CONDITION           OMM_SSD_CONDITION
#define COIN_Y              OMM_SSD_ONLY_COIN_Y
#define COIN_R              OMM_SSD_ONLY_COIN_R
#define COIN_B              OMM_SSD_ONLY_COIN_B
#define NO_MUSHROOM         OMM_SSD_NO_MUSHROOM
#define NO_CAPTURE          OMM_SSD_NO_CAPTURE
#define NO_VIBE             OMM_SSD_NO_VIBE
#define NO_VIBE_AUTO_REFILL OMM_SSD_NO_VIBE_AUTO_REFILL
#define WING_CAP            OMM_SSD_WING_CAP
#define VANISH_CAP          OMM_SSD_VANISH_CAP
#define METAL_CAP           OMM_SSD_METAL_CAP
#define SHARDS              OMM_SSD_SHARDS
#define DARK_MODE           OMM_SSD_DARK_MODE
#define FLAMES              OMM_SSD_FLAMES
#define BOXES               OMM_SSD_BOXES
#define RINGS               OMM_SSD_RINGS

const OmmSSD gOmmSSD[4][30] = {
// Disabled
{
},
// Normal mode: Pink Gold Stars
{
{  0, OMM_GAME_SM64, LEVEL_BOB,     1,   4360,   4500,   1230, -5200,     0,  6750, 0xC000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{  1, OMM_GAME_SM64, LEVEL_WF,      1,      0,   6250,      0,  3075,   256,  5245, 0xC000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | COINS(90) | COIN_Y },
{  2, OMM_GAME_SM64, LEVEL_JRB,     1,   5000,   2500,   2500, -6060,  1126,   940, 0xA000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | COINS(80) },
{  3, OMM_GAME_SM64, LEVEL_CCM,     2,  -4880,   4800,   5000,     0,     0,     0, 0x0000, 1, -4890,     0,   6000, 0x0000, 1,     0, 0 },
{  4, OMM_GAME_SM64, LEVEL_BBH,     1,    666,   4000,    260,   916,  -204,  3064, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | COINS(100) | COIN_B },
{  5, OMM_GAME_SM64, LEVEL_HMC,     1,   6000,   4500,   6200,     0,     0,     0, 0x0000, 1,  7000,  2000,   7100, 0x6000, 1,     0, 0 },
{  6, OMM_GAME_SM64, LEVEL_LLL,     2,   2530,   5500,   -900,  -750,   103, -1225, 0xE000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION | RED_COINS(8) | SHARDS },
{  7, OMM_GAME_SM64, LEVEL_SSL,     1,  -2050,   5300,  -1020,     0,     0,     0, 0x0000, 1, -2050,  2000,  -1020, 0x8000, 1,     0, 0 },
{  8, OMM_GAME_SM64, LEVEL_DDD,     2,   3900,   2400,  -4000,     0,     0,     0, 0x0000, 1,  3900,  1700,  -4000, 0x8000, 1,     0, 0 },
{  9, OMM_GAME_SM64, LEVEL_SL,      1,      0,   6000,      0,  2035,  1023,   475, 0x6000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 10, OMM_GAME_SM64, LEVEL_WDW,     1,  -1075,   6000,   1280,     0,     0,     0, 0x0000, 1, -1075,  4500,   1280, 0xA000, 1,     0, 0 },
{ 11, OMM_GAME_SM64, LEVEL_TTM,     2,  -8465,   7650, -14670,     0,     0,     0, 0x0000, 1, -9040,  6250, -15240, 0x5000, 1,     0, 0 },
{ 12, OMM_GAME_SM64, LEVEL_THI,     2,  -1900,   -400,   1800, -1700,  -890,  2250, 0xC000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 13, OMM_GAME_SM64, LEVEL_TTC,     1,      0,   4800,      0,   800, -4822,   140, 0x6000, 1,     0,     0,      0, 0x0000, 0,  1800, CONDITION | BOXES },
{ 14, OMM_GAME_SM64, LEVEL_RR,      1,  -4200,   7200,  -5850,  3100, -1833,  2070, 0xC000, 1,     0,     0,      0, 0x0000, 0,     0, NO_BUTTON(A_BUTTON) },
{ 15, OMM_GAME_SM64, LEVEL_BITDW,   1,  -7440,  -2600,   3900, -7440, -3153,  3530, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION | RED_COINS(8) },
{ 16, OMM_GAME_SM64, LEVEL_BITFS,   1,   6740,   3333,     90, -7577, -2764,  -360, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | MUSHROOMS(7) },
{ 17, OMM_GAME_SM64, LEVEL_BITS,    1,    350,   6800,  -5300, -7360, -4812,     0, 0x4000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | RED_COINS(8) | NO_BUTTON(A_BUTTON) },
{ 18, OMM_GAME_SM64, LEVEL_TOTWC,   1,      0,  19950,      0, -1000, -2047,     0, 0x4000, 1,     0,     0,      0, 0x0000, 0,  1800, CONDITION | COINS(63) | WING_CAP },
{ 19, OMM_GAME_SM64, LEVEL_VCUTM,   1,  -2300,   6300,  -6400, -6400,  5734, -6140, 0x4000, 1,     0,     0,      0, 0x0000, 0,  1500, CONDITION | VANISH_CAP },
{ 20, OMM_GAME_SM64, LEVEL_COTMC,   1,      0,   1500,  -7300, -4000,    20,  -333, 0xE000, 2,     0,   600,  -7300, 0x8000, 1,  1200, METAL_CAP | NO_CAPTURE },
{ 21, OMM_GAME_SM64, LEVEL_PSS,     1,  -6360,  -4100,   3150,  3566,  6144, -6100, 0x4000, 1,     0,     0,      0, 0x0000, 0,  -720, CONDITION | COINS(50) | NO_BUTTON(A_BUTTON | B_BUTTON | START_BUTTON | R_TRIG | L_TRIG | Z_TRIG | X_BUTTON | Y_BUTTON) | COIN_Y },
{ 22, OMM_GAME_SM64, LEVEL_WMOTR,   1,  -3200,   5000,  -4500,  -410,  1669,   326, 0x6000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | RED_COINS(8) },
{ 23, OMM_GAME_SM64, LEVEL_SA,      1,      0,  -2200,      0, -9000,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | RED_COINS(8) },
{ 24, OMM_GAME_SM64, LEVEL_GROUNDS, 1,      0,   9400,  -4020,     0,   800,  1400, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, 0 },
{ 25, OMM_GAME_SM64, LEVEL_CASTLE,  1,  -1024,    250,    717, -1350,     0,     0, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION | RED_COINS(8) | SHARDS },
{ 26, OMM_GAME_SM64, LEVEL_CASTLE,  3,   7050,  -1000,   -512,  -800, -1279,  2592, 0x8000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 27, OMM_GAME_SM64, LEVEL_CASTLE,  2,   -205,   3900,   6750,     0,     0,     0, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION | RED_COINS(8) | SHARDS },
{ 28, OMM_GAME_SM64, LEVEL_COURT,   1,      0,    300,    960,     0,     0, -3590, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 29, OMM_GAME_SM64, LEVEL_GROUNDS, 2,      0,      0,      0,     0,     0,     0, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION },
},
// Hard mode: Crystal Stars
{
{  0, OMM_GAME_SM64, LEVEL_BOB,     1,  -1506,   7000,   1250, -5200,     0,  6750, 0xC000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{  1, OMM_GAME_SM64, LEVEL_WF,      1,      0,   1000,      0,  3075,   256,  5245, 0xC000, 1,     0,     0,      0, 0x0000, 0,  3300, CONDITION | COINS(100) | COIN_Y },
{  2, OMM_GAME_SM64, LEVEL_JRB,     1,  -1080,  19500,   7070, -6060,  1126,   940, 0xA000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | COINS(101) | DARK_MODE },
{  3, OMM_GAME_SM64, LEVEL_CCM,     1,    -55,   9100,   -150,     0,     0,     0, 0x0000, 1,   -55,  5800,   -150, 0x0000, 1,     0, 0 },
{  4, OMM_GAME_SM64, LEVEL_BBH,     1,    666,   4000,    260,   916,  -204,  3064, 0x0000, 1,     0,     0,      0, 0x0000, 0,  4050, CONDITION | COINS(100) | COIN_B },
{  5, OMM_GAME_SM64, LEVEL_HMC,     1,  -7500,   2500,   7500, -7100,  2048,  5900, 0x2000, 1,     0,     0,      0, 0x0000, 0,  5400, CONDITION | FLAMES },
{  6, OMM_GAME_SM64, LEVEL_LLL,     2,      0,   7000,      0,  -750,   103, -1225, 0xE000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION | RED_COINS(8) | SHARDS },
{  7, OMM_GAME_SM64, LEVEL_SSL,     2,      0,   5650,    256,   600,     0,  4500, 0xC000, 1,     0,     0,      0, 0x0000, 0,   450, CONDITION | COINS(62) },
{  8, OMM_GAME_SM64, LEVEL_DDD,     2,   3900,   4000,   6000,     0,     0,     0, 0x0000, 1,  3900,  2000,   6000, 0x0000, 1,     0, 0 },
{  9, OMM_GAME_SM64, LEVEL_SL,      1,      0,   8400,      0,  2035,  1023,   475, 0x6000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 10, OMM_GAME_SM64, LEVEL_WDW,     0,      0,      0,      0,  1400,     0,  -700, 0x4000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 11, OMM_GAME_SM64, LEVEL_TTM,     2, -16240,  13850, -16240,     0,     0,     0, 0x0000, 1, -6040, 13150,  -6240, 0x6000, 1,     0, 0 },
{ 12, OMM_GAME_SM64, LEVEL_THI,     1,      0,   8900,  -1540, -7800, -2969,  7800, 0xE000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 13, OMM_GAME_SM64, LEVEL_TTC,     1,      0,   8000,      0,   800, -4822,   140, 0x6000, 1,     0,     0,      0, 0x0000, 0,     0, 0 },
{ 14, OMM_GAME_SM64, LEVEL_RR,      1,   3554,   5000,  -2327,  3100, -1833,  2070, 0xC000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | NO_BUTTON(A_BUTTON | B_BUTTON | Z_TRIG) | MUSHROOMS(9) },
{ 15, OMM_GAME_SM64, LEVEL_BITDW,   1,  -7440,  -2048,   3900, -7440, -3153,  3530, 0x0000, 0,     0,     0,      0, 0x0000, 0,  3150, CONDITION },
{ 16, OMM_GAME_SM64, LEVEL_BITFS,   1,   1240,   8000,     90, -7577, -2764,  -360, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 17, OMM_GAME_SM64, LEVEL_BITS,    1,    350,   7700,  -6030, -7360, -4812,     0, 0x4000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | RED_COINS(8) | NO_BUTTON(A_BUTTON | B_BUTTON | Z_TRIG) | COIN_R | NO_MUSHROOM | NO_CAPTURE },
{ 18, OMM_GAME_SM64, LEVEL_TOTWC,   1,      0,  19950,      0, -1000, -2047,     0, 0x4000, 1,     0,     0,      0, 0x0000, 0,  1050, CONDITION | COINS(63) | WING_CAP },
{ 19, OMM_GAME_SM64, LEVEL_VCUTM,   1,  -2300,   6400,  -6400, -6400,  5734, -6140, 0x4000, 1,     0,     0,      0, 0x0000, 0,  1200, CONDITION | VANISH_CAP },
{ 20, OMM_GAME_SM64, LEVEL_COTMC,   1,      0,   4000,  -7450, -4000,    20,  -333, 0xE000, 2,     0,   600,  -7300, 0x8000, 1,   900, METAL_CAP | NO_CAPTURE },
{ 21, OMM_GAME_SM64, LEVEL_PSS,     1,  -6360,  -4400,   3450,  3566,  6144, -6100, 0x4000, 1,     0,     0,      0, 0x0000, 0,  -660, CONDITION | COINS(50) | NO_BUTTON(A_BUTTON | B_BUTTON | START_BUTTON | R_TRIG | L_TRIG | Z_TRIG | X_BUTTON | Y_BUTTON) | COIN_Y },
{ 22, OMM_GAME_SM64, LEVEL_WMOTR,   1,    200,   7100,  -2340,  -410,  1669,   326, 0x6000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | COINS(56) },
{ 23, OMM_GAME_SM64, LEVEL_SA,      1,      0,  -2200,      0, -9000,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | COINS(56) | NO_BUTTON(X_BUTTON) },
{ 24, OMM_GAME_SM64, LEVEL_GROUNDS, 1,      0,  10000,  -4020,     0,   800,  1400, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, 0 },
{ 25, OMM_GAME_SM64, LEVEL_CASTLE,  1,  -6960,   1080,  -5770, -1350,     0,     0, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, 0 },
{ 26, OMM_GAME_SM64, LEVEL_CASTLE,  3,  -1509,   -900,  -1564,     0,     0,     0, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 27, OMM_GAME_SM64, LEVEL_CASTLE,  2,   7350,   1750,   1830,  3975,  1408,  1830, 0xC000, 0,     0,     0,      0, 0x0000, 0,     0, 0 },
{ 28, OMM_GAME_SM64, LEVEL_COURT,   1,      9,   3800,  -1735,     0,     0, -3590, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 29, OMM_GAME_SM64, LEVEL_GROUNDS, 2,      0,      0,      0,     0,     0,     0, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION },
},
// Lunatic mode: Nebula Stars
{
{  0, OMM_GAME_SM64, LEVEL_BOB,     1,   3300,   5300,  -4600, -5200,     0,  6750, 0xC000, 1,     0,     0,      0, 0x0000, 0, -3600, CONDITION },
{  1, OMM_GAME_SMMS, LEVEL_BBH,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{  2, OMM_GAME_SM64, LEVEL_JRB,     1,  -6666,  19950,      0, -6060,  1126,   940, 0xA000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | COINS(101) | NO_VIBE_AUTO_REFILL },
{  3, OMM_GAME_SM64, LEVEL_CCM,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{  4, OMM_GAME_SMSR, LEVEL_TTM,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{  5, OMM_GAME_SM74, LEVEL_HMC,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{  6, OMM_GAME_SMSR, LEVEL_LLL,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{  7, OMM_GAME_SM74, LEVEL_SSL,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{  8, OMM_GAME_SMSR, LEVEL_DDD,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{  9, OMM_GAME_SM74, LEVEL_WDW,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 10, OMM_GAME_SM74, LEVEL_TTM,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 11, OMM_GAME_SM64, LEVEL_TTM,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 12, OMM_GAME_SMSR, LEVEL_TTC,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 13, OMM_GAME_SMSR, LEVEL_RR,      1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 14, OMM_GAME_SM74, LEVEL_RR,      2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 15, OMM_GAME_SM64, LEVEL_BITS,    1,    350,  11000,   8200, -7360, -4812,     0, 0x4000, 1,     0,     0,      0, 0x0000, 0,  5400, CONDITION | NO_VIBE_AUTO_REFILL | RINGS },
{ 16, OMM_GAME_SMSR, LEVEL_BITS,    1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 17, OMM_GAME_SM74, LEVEL_BITS,    2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 18, OMM_GAME_SMSR, LEVEL_TOTWC,   1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | WING_CAP },
{ 19, OMM_GAME_SMMS, LEVEL_LLL,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | VANISH_CAP },
{ 20, OMM_GAME_SM74, LEVEL_COTMC,   2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION | METAL_CAP },
{ 21, OMM_GAME_SM64, LEVEL_PSS,     1,  -6360,  -4350,   3435,  3566,  6144, -6100, 0x4000, 1,     0,     0,      0, 0x0000, 0,  -630, CONDITION | COINS(50) | NO_BUTTON(A_BUTTON | B_BUTTON | START_BUTTON | R_TRIG | L_TRIG | Z_TRIG | X_BUTTON | Y_BUTTON) | COIN_Y },
{ 22, OMM_GAME_SMSR, LEVEL_PSS,     1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 23, OMM_GAME_SM74, LEVEL_PSS,     2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 24, OMM_GAME_SM64, LEVEL_GROUNDS, 1,      0,  16500,  -4020,     0,   800,  1400, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, 0 },
{ 25, OMM_GAME_SM74, LEVEL_WMOTR,   2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 26, OMM_GAME_SMSR, LEVEL_CASTLE,  1,      0,  19000,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, 0 },
{ 27, OMM_GAME_SMSR, 39,            1,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 28, OMM_GAME_SM74, LEVEL_SA,      2,      0,      0,      0,     0,     0,     0, 0x0000, 1,     0,     0,      0, 0x0000, 0,     0, CONDITION },
{ 29, OMM_GAME_SM64, LEVEL_GROUNDS, 2,      0,      0,      0,     0,     0,     0, 0x0000, 0,     0,     0,      0, 0x0000, 0,     0, CONDITION },
},
};

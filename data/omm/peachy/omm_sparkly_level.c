#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

typedef struct {
    bool (*update)(struct MarioState *, const s32 *);
    const s32 *params;
} OmmSparklyLevel;

//
// Parameters
//

static const s32 params_0_06[] = {
// Lava yMin, yMax, yVel, radius
    0, 3570, 2, 4096,
// Shards
    -1700,   60,  -440,
     2550,  630,  1120,
      250, 1210,  2640,
    -2080, 1915,  -300,
    -1600, 2440, -2150,
     1300, 3705, -2500,
      420, 2365,   575,
      662, 3300,   708,
};

static const s32 params_0_09[] = {
// maxSnowmenAtTop, topX, topY, topZ
    1, 0, 4860, 0
};

static const s32 params_0_12[] = {
// Goomba size
    GOOMBA_SIZE_TINY
};

static const s32 params_0_13[] = {
// Num boxes
    13
};

static const s32 params_0_14[] = {
// Num secrets
    6,
// Secrets
     620, -3350,  7360,
     615,  -740,  3670,
    2680,  1270,   295,
    3810,  2090,   295,
    3555,  4895, -2330,
    5120,  4900,  3325,
};

static const s32 params_0_15[] = {
// Required stacked goombas
    0
};

static const s32 params_0_16[] = {
// Num mushrooms, capture required
    7, true
};

static const s32 params_0_17[] = {
// Inc frames
    30
};

static const s32 params_0_20[] = {
// Auto start
    true
};

static const s32 params_0_25[] = {
// Shards
    -1024,  400,  -250,
    -1024,  800,  1400,
    -4500, 1200,  -450,
    -2280, -400, -3040,
     -128, -300, -2640,
     2000,  900, -1700,
     1910,  850, -2120,
    -5500,  600, -4300,
};

static const s32 params_0_27[] = {
// Shards
    -1000, 1210,  1480,
    -4780, 3200, -3240,
     3538, 1680,  3880,
     -610, 1900,  2500,
     -200, 2800,  5000,
    -3400, 3600,  5886,
     3000, 2900,  5886,
     -200, 5400, -3700,
};

static const s32 params_0_28[] = {
// Num secrets
    8,
// Secrets
     1500,    0, -1740,
     -820,   20, -3630,
      820,   20, -3630,
     -820,   20,   200,
      820,   20,   200,
    -1500,    0, -1740,
     2040, -200,  -480,
    -2240, -200, -2815,
};

static const s32 params_1_05[] = {
// Num flames
    17,
// Flames
    4936, -357, -4146,
    5018, -460, -5559,
    1997,  666,  -235,
    1762, -460, -2610,
    4178, -255, -3737,
    2233, -460,   256,
    5510, -255, -3429,
    4690, -357,  -767,
    3462, -255, -1125,
    1762,  666,     0,
    1762, -460,   256,
    6482,  461,  3226,
    1075,  461,  6543,
    6912,  461,  6543,
    6912,  461,  3697,
    6482,  461,  7014,
    3817,  717,  1034,
};

static const s32 params_1_06[] = {
// Lava yMin, yMax, yVel, radius
    0, 3570, 3, 4096,
// Shards
    -1700,   60,  -440,
      180, 1050, -1360,
     2550,  630,  1120,
     -480, 1210,  2630,
      705, 2160,   330,
        0, 4690, -2800,
      662, 3900,   708,
     2800, 3000,   250,
};

static const s32 params_1_07[] = {
// Inc frames
    30
};

static const s32 params_1_08[] = {
// Purple switch pos
    3920, 570, -1000
};

static const s32 params_1_12[] = {
// Goomba size
    GOOMBA_SIZE_HUGE
};

static const s32 params_1_13[] = {
// Auto start
    false
};

static const s32 params_1_14[] = {
// Num mushrooms, capture required
    9, false
};

static const s32 params_1_15[] = {
// Required stacked goombas
    6
};

static const s32 params_1_20[] = {
// Auto start
    true
};

static const s32 params_1_25[] = {
// Num toads
    2,
// Toads
    OMM_DIALOG_SPARKLY_TOAD_1, -1075, -101, -5479, 0x4000,
    OMM_DIALOG_SPARKLY_TOAD_2,  -975, -101, -5479, 0xC000,
};

static const s32 params_1_27[] = {
// Num toads
    1,
// Toads
    OMM_DIALOG_SPARKLY_TOAD_3, -205, 2253, 7200, 0x7000,
};

static const s32 params_1_28[] = {
// targetPos
    9, 433, -1735, 310
};

//
// Update per difficulty and level
//

static const OmmSparklyLevel sOmmSparklyLevels[4][OMM_SPARKLY_STARS_MAX] = {
// Disabled
{
},
// Normal mode: Pink-Gold Stars
{
[ 0] = { omm_sparkly_level__all_secrets, NULL },
[ 1] = { NULL, NULL },
[ 2] = { omm_sparkly_level__invalidate_if_not_flooded, NULL },
[ 3] = { NULL, NULL },
[ 4] = { NULL, NULL },
[ 5] = { NULL, NULL },
[ 6] = { omm_sparkly_level__eight_shards_and_lava, params_0_06 },
[ 7] = { NULL, NULL },
[ 8] = { NULL, NULL },
[ 9] = { omm_sparkly_level_sl__snowmen_at_top, params_0_09 },
[10] = { omm_sparkly_level_wdw__secret, NULL },
[11] = { NULL, NULL },
[12] = { omm_sparkly_level__ten_goombas_stack, params_0_12 },
[13] = { omm_sparkly_level__all_boxes, params_0_13 },
[14] = { omm_sparkly_level__secrets_in_order, params_0_14 },
[15] = { omm_sparkly_level_bitdw__goomba_and_coins, params_0_15 },
[16] = { omm_sparkly_level__all_mushrooms, params_0_16 },
[17] = { omm_sparkly_level__inc_timer_with_coins, params_0_17 },
[18] = { NULL, NULL },
[19] = { omm_sparkly_level_vcutm__five_corners, NULL },
[20] = { omm_sparkly_level__dont_touch_floor, params_0_20 },
[21] = { NULL, NULL },
[22] = { NULL, NULL },
[23] = { omm_sparkly_level__invalidate_if_not_frozen, NULL },
[24] = { NULL, NULL },
[25] = { omm_sparkly_level__eight_shards, params_0_25 },
[26] = { omm_sparkly_level__toads_on_pillars, NULL },
[27] = { omm_sparkly_level__eight_shards, params_0_27 },
[28] = { omm_sparkly_level__secrets_in_order, params_0_28 },
[29] = { NULL, NULL },
},
// Hard mode: Crystal Stars
{
[ 0] = { omm_sparkly_level__all_secrets, NULL },
[ 1] = { NULL, NULL },
[ 2] = { omm_sparkly_level__invalidate_if_not_flooded, NULL },
[ 3] = { NULL, NULL },
[ 4] = { NULL, NULL },
[ 5] = { omm_sparkly_level__all_flames, params_1_05 },
[ 6] = { omm_sparkly_level__eight_shards_and_lava, params_1_06 },
[ 7] = { omm_sparkly_level__inc_timer_with_coins, params_1_07 },
[ 8] = { omm_sparkly_level_ddd__switch_and_boxes, params_1_08 },
[ 9] = { omm_sparkly_level__shell_ride, NULL },
[10] = { omm_sparkly_level_wdw__star_box_red_coins, NULL },
[11] = { NULL, NULL },
[12] = { omm_sparkly_level__ten_goombas_stack, params_1_12 },
[13] = { omm_sparkly_level__dont_touch_floor, params_1_13 },
[14] = { omm_sparkly_level__all_mushrooms, params_1_14 },
[15] = { omm_sparkly_level_bitdw__goomba_and_coins, params_1_15 },
[16] = { omm_sparkly_level_bitfs__six_secrets, NULL },
[17] = { omm_sparkly_level_bits__bobomb_and_enemies, NULL },
[18] = { NULL, NULL },
[19] = { omm_sparkly_level_vcutm__five_corners, NULL },
[20] = { omm_sparkly_level__dont_touch_floor, params_1_20 },
[21] = { NULL, NULL },
[22] = { NULL, NULL },
[23] = { omm_sparkly_level__invalidate_if_not_frozen, NULL },
[24] = { NULL, NULL },
[25] = { omm_sparkly_level__toads_hide_and_seek, params_1_25 },
[26] = { omm_sparkly_level_basement__catch_mips, NULL },
[27] = { omm_sparkly_level_upstairs__toads_hide_and_seek, params_1_27 },
[28] = { omm_sparkly_level__launch_cappy_at_target, params_1_28 },
[29] = { NULL, NULL },
},
// Lunatic mode: Nebula Stars
{
},
};

//
// Update
//

void omm_sparkly_context_update_level(struct MarioState *m, const OmmSparklyData *data) {

    // Update
    bool updateTimer = true;
    const OmmSparklyLevel *level = &sOmmSparklyLevels[gOmmSparklyMode][data->starIndex];
    if (level->update) {
        updateTimer = level->update(m, level->params);
    }

    // PSS / Koopa the Quick timer
    if (data->timer < 0) {
        if ((gHudDisplay.flags & HUD_DISPLAY_FLAG_TIMER) && gHudDisplay.timer >= -data->timer) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        }
        return;
    }

    // Countdown
    if (data->timer > 0) {
        if (OMM_SPARKLY_STATE_IS_OK) {

            // Start the timer...
            if (!gOmmSparklyContext->timerStarted) {
                if (updateTimer) {
                    s32 cap = 0;
                    switch (data->flags & OMM_SPARKLY_DATA_CAPS) {
                        case OMM_SPARKLY_DATA_WING_CAP: cap = MARIO_WING_CAP; audio_play_wing_cap_music(); break;
                        case OMM_SPARKLY_DATA_METAL_CAP: cap = MARIO_METAL_CAP; audio_play_metal_cap_music(); break;
                        case OMM_SPARKLY_DATA_VANISH_CAP: cap = MARIO_VANISH_CAP; audio_play_vanish_cap_music(); break;
                    }
                    if (cap) {
                        m->flags &= ~(MARIO_CAP_IN_HAND | MARIO_WING_CAP | MARIO_METAL_CAP | MARIO_VANISH_CAP);
                        m->flags |= (MARIO_CAP_ON_HEAD | cap);
                        m->capTimer = data->timer;
                        m->marioObj->oHomeX = m->pos[0];
                        m->marioObj->oHomeY = m->pos[1];
                        m->marioObj->oHomeZ = m->pos[2];
                    }
                    gHudDisplay.timer = data->timer;
                    gHudDisplay.flags |= HUD_DISPLAY_FLAG_TIMER;
                    gOmmSparklyContext->timerStarted = true;
                }
                return;
            }

            // ...and grab the star before it runs out
            bool starSpawnedNormalMode = (OMM_SPARKLY_MODE_IS_NORMAL && gOmmSparklyContext->starSpawned);
            if (gOmmSparkly->marioUpdated && !gOmmSparkly->gamePaused && !gOmmSparkly->transition && !gOmmSparkly->timeStop &&
                !starSpawnedNormalMode && !omm_mario_is_reading(m)) {
                if (gHudDisplay.timer > 0) {
                    gHudDisplay.timer -= updateTimer;
                    gHudDisplay.flags |= HUD_DISPLAY_FLAG_TIMER;
                } else {
                    omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
                }
            }

            // Sync the cap timer with the countdown timer during Cap races
            if ((data->flags & OMM_SPARKLY_DATA_CAPS) && gHudDisplay.timer > 0 && !starSpawnedNormalMode && !omm_peach_vibe_is_active()) {
                m->capTimer = gHudDisplay.timer;
            }
        } else {
            gHudDisplay.flags &= ~HUD_DISPLAY_FLAG_TIMER;
        }
    }
}

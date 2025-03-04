#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Buffers
//

static s32 sOmmCoinSparkles[OMM_RENDER_HUD_COIN_SPARKLE_COUNT] = {0};
static s32 sOmmCurrentRadar = 0;
static s32 sOmmRadarTimer = OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_TOTAL;

//
// Frame interpolation
//

static InterpData sOmmPowerMeter[1];
static InterpData sOmmBreathMeter[1];
static InterpData sOmmTimer[1];
static InterpData sOmmRadarArrow[1];
static InterpData sOmmMontyMoleArrow[1];
extern InterpData sOmmStatsBoard[1];
extern InterpData gTimeTrialsTimer[1];

static Gfx *omm_render_hud_power_meter_with_offset(Gfx *pos, Gfx *gfx, Vtx *vtx, f32 shakeX, f32 shakeY, f32 relPos, f32 alpha, s32 state, f32 ticks);
static Gfx *omm_render_hud_breath_meter_gauge(Gfx *pos, Gfx *gfx, Vtx *vtx, f32 x, f32 y, f32 alpha);
static Gfx *omm_render_hud_timer_m_s_ms(Gfx *pos, Vtx *vtx, f32 timer);
static Gfx *omm_render_hud_objects_radar_arrow(Gfx *pos, Vtx *vtx, f32 angle);
static Gfx *omm_render_hud_monty_mole_hole_indicator_arrow(Gfx *pos, Vtx *vtx, f32 x, f32 y, f32 radius, f32 angle, f32 scale);
extern Gfx *omm_render_stats_board(Gfx *pos, Gfx *gfx, f32 t);
extern Gfx *time_trials_render_timer_interpolated(Gfx *pos, Vtx *vtx, f32 timer);

void gfx_interpolate_frame_hud(f32 t) {

    // Power meter (health)
    if (sOmmPowerMeter->pos) {
        interp_data_lerp(sOmmPowerMeter, t);
        omm_render_hud_power_meter_with_offset(sOmmPowerMeter->pos, sOmmPowerMeter->gfx, sOmmPowerMeter->vtx, sOmmPowerMeter->x, sOmmPowerMeter->y, sOmmPowerMeter->z, sOmmPowerMeter->a, sOmmPowerMeter->s, sOmmPowerMeter->t);
    }

    // Breath meter
    if (sOmmBreathMeter->pos) {
        interp_data_lerp(sOmmBreathMeter, t);
        omm_render_hud_breath_meter_gauge(sOmmBreathMeter->pos, sOmmBreathMeter->gfx, sOmmBreathMeter->vtx, sOmmBreathMeter->x, sOmmBreathMeter->y, sOmmBreathMeter->a);
    }

    // Timer
    if (sOmmTimer->pos) {
        interp_data_lerp(sOmmTimer, t);
        omm_render_hud_timer_m_s_ms(sOmmTimer->pos, sOmmTimer->vtx, sOmmTimer->t);
    }

    // Objects radar arrow
    if (sOmmRadarArrow->pos) {
        interp_data_lerp(sOmmRadarArrow, t);
        omm_render_hud_objects_radar_arrow(sOmmRadarArrow->pos, sOmmRadarArrow->vtx, sOmmRadarArrow->a);
    }

    // Monty Mole hole arrow
    if (sOmmMontyMoleArrow->pos) {
        interp_data_lerp(sOmmMontyMoleArrow, t);
        omm_render_hud_monty_mole_hole_indicator_arrow(sOmmMontyMoleArrow->pos, sOmmMontyMoleArrow->vtx, sOmmMontyMoleArrow->x, sOmmMontyMoleArrow->y, sOmmMontyMoleArrow->z, sOmmMontyMoleArrow->a, sOmmMontyMoleArrow->s);
    }

    // Stats board
    if (sOmmStatsBoard->pos) {
        interp_data_lerp(sOmmStatsBoard, t);
        omm_render_stats_board(sOmmStatsBoard->pos, sOmmStatsBoard->gfx, sOmmStatsBoard->t);
    }

    // Timer Trials timer
    if (gTimeTrialsTimer->pos) {
        interp_data_lerp(gTimeTrialsTimer, t);
        time_trials_render_timer_interpolated(gTimeTrialsTimer->pos, gTimeTrialsTimer->vtx, gTimeTrialsTimer->t);
    }
}

void gfx_clear_frame_hud() {
    sOmmPowerMeter->pos = NULL;
    sOmmBreathMeter->pos = NULL;
    sOmmTimer->pos = NULL;
    sOmmRadarArrow->pos = NULL;
    sOmmMontyMoleArrow->pos = NULL;
    sOmmStatsBoard->pos = NULL;
    gTimeTrialsTimer->pos = NULL;
}

//
// HUD timers
//

#define OMM_HUD_TIMER_FADE  (10)
#define OMM_HUD_TIMER_MAX   (OMM_HUD_TIMER_FADE + 20)

typedef struct OmmHudTimer {
    u8 (*update)(struct OmmHudTimer *, struct MarioState *);
    s32 timerMin;
    s32 timerMax;
    s32 timer;
    s32 prev;
} OmmHudTimer;

static const struct { u32 action; bool fadeOut; } OMM_HUD_TIMER_FADE_OUT_ACTIONS[] = {
    { ACT_SHOCKWAVE_BOUNCE, true },
    { ACT_OMM_SHOCKED_FROM_HIGH_FALL, true },
    { ACT_WATER_IDLE, false },
    { ACT_HOLD_WATER_IDLE, false },
    { ACT_WATER_ACTION_END, false },
    { ACT_HOLD_WATER_ACTION_END, false },
    { ACT_METAL_WATER_STANDING, false },
    { ACT_HOLD_METAL_WATER_STANDING, false },
    { ACT_INTRO_CUTSCENE, false },
    { ACT_HOLDING_POLE, false },
    { ACT_TOP_OF_POLE, false },
    { ACT_START_HANGING, false },
    { ACT_HANGING, false },
    { ACT_LEDGE_GRAB, false },
    { ACT_IN_CANNON, false },
    { ACT_OMM_METAL_WATER_IDLE, false },
    { ACT_OMM_METAL_WATER_START_CROUCHING, false },
    { ACT_OMM_METAL_WATER_CROUCHING, false },
    { ACT_OMM_METAL_WATER_STOP_CROUCHING, false },
    { ACT_OMM_METAL_WATER_START_CRAWLING, false },
    { ACT_OMM_METAL_WATER_STOP_CRAWLING, false },
    { ACT_OMM_METAL_WATER_GROUND_POUND_LAND_STOP, false },
};

static const struct { u32 group; bool fadeOut; } OMM_HUD_TIMER_FADE_OUT_GROUPS[] = {
    { ACT_GROUP_STATIONARY, false },
    { ACT_GROUP_MOVING, true },
    { ACT_GROUP_AIRBORNE, true },
    { ACT_GROUP_SUBMERGED, true },
    { ACT_GROUP_CUTSCENE, true },
    { ACT_GROUP_AUTOMATIC, true },
    { ACT_GROUP_OBJECT, true },
    { ACT_GROUP_METAL_WATER, true },
};

static bool should_start_fade_out_capture(struct MarioState *m) {
    struct Object *o = gOmmCapture;

    // Capturing
    if (gOmmMario->capture.timer < 20) {
        return true;
    }

    // Locked
    if (POBJ_IS_STAR_DANCING || POBJ_IS_OPENING_DOORS || omm_mario_is_locked(m)) {
        return true;
    }

    // Non-zero inputs
    if (POBJ_NONZERO_ANALOG || POBJ_A_BUTTON_PRESSED || POBJ_A_BUTTON_DOWN || POBJ_B_BUTTON_PRESSED || POBJ_B_BUTTON_DOWN || POBJ_X_BUTTON_PRESSED || POBJ_X_BUTTON_DOWN) {
        return true;
    }

    // Airborne (affected by gravity)
    if (!obj_is_on_ground(o) && pobj_get_gravity(o) != 0.f) {
        return true;
    }

    // Moving
    if (POBJ_IS_WALKING) {
        return true;
    }

    // Warping
    if (gOmmWarp->state > POBJ_WARP_STATE_NOT_WARPING) {
        return true;
    }

    // Idle
    return false;
}

static bool should_start_fade_out(struct MarioState *m) {

    // Star dance
    if (omm_mario_is_star_dancing(m)) {
        return true;
    }

    // Capture
    if (omm_mario_is_capture(m)) {
        return should_start_fade_out_capture(m);
    }

    // Sliding down a pole
    if (m->action == ACT_HOLDING_POLE && m->controller->stickY < -16.f) {
        return true;
    }

    // Actions
    for (u32 i = 0; i != array_length(OMM_HUD_TIMER_FADE_OUT_ACTIONS); ++i) {
        if (m->action == OMM_HUD_TIMER_FADE_OUT_ACTIONS[i].action) {
            return OMM_HUD_TIMER_FADE_OUT_ACTIONS[i].fadeOut;
        }
    }

    // Action groups
    for (u32 i = 0; i != array_length(OMM_HUD_TIMER_FADE_OUT_GROUPS); ++i) {
        if ((m->action & ACT_GROUP_MASK) == OMM_HUD_TIMER_FADE_OUT_GROUPS[i].group) {
            return OMM_HUD_TIMER_FADE_OUT_GROUPS[i].fadeOut;
        }
    }

    // Default
    return true;
}

static void omm_hud_update_timer(OmmHudTimer *timer, struct MarioState *m) {
    if (timer->timer < 0) {
        timer->timer = min_s(timer->timer + 1, 0);
    } else if (timer->timer == 0) {
        timer->timer = OMM_HUD_PRO || should_start_fade_out(m);
    } else if (!OMM_HUD_PRO && !should_start_fade_out(m)) {
        timer->timer = max_s(timer->timer - 1, 0);
    } else if (timer->timer < OMM_HUD_TIMER_FADE) {
        timer->timer = timer->timer + 1;
    } else {
        timer->timer = timer->timerMax;
    }
}

static u8 omm_hud_timer_compute_alpha(const OmmHudTimer *timer, u8 alphaMin, u8 alphaMax) {
    return relerp_0_1_s(timer->timer, 0, OMM_HUD_TIMER_FADE, alphaMax, alphaMin);
}

static u8 omm_hud_update_stars_timer(OmmHudTimer *timer, struct MarioState *m) {

    // Always hide if there is no HUD
    if (OMM_HUD_NONE) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Always show when paused
    if (OMM_HUD_ALWAYS || omm_is_game_paused()) {
        return 0xFF;
    }

    // Always hide in Pro mode
    if (OMM_HUD_PRO) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Reset timer when star dancing
    if (m->action != ACT_OMM_SPARKLY_STAR_DANCE && (omm_mario_is_star_dancing(m) || (m->action == ACT_JUMBO_STAR_CUTSCENE && !omm_sparkly_is_bowser_4_battle()))) {
        timer->timer = timer->timerMin;
    }

    // Update timer
    else {
        omm_hud_update_timer(timer, m);
    }

    // Compute and return alpha
    return omm_hud_timer_compute_alpha(timer, 0x00, 0xFF);
}

static u8 omm_hud_update_sparkly_timer(OmmHudTimer *timer, struct MarioState *m) {

    // Always hide if there is no HUD or if Sparkly mode is not enabled
    if (OMM_HUD_NONE || !OMM_SPARKLY_MODE_IS_ENABLED) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Always show when paused
    if (OMM_HUD_ALWAYS || omm_is_game_paused()) {
        return 0xFF;
    }

    // Always hide in Pro mode
    if (OMM_HUD_PRO) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Reset timer when Sparkly star dancing
    if (m->action == ACT_OMM_SPARKLY_STAR_DANCE || (m->action == ACT_JUMBO_STAR_CUTSCENE && omm_sparkly_is_bowser_4_battle())) {
        timer->timer = timer->timerMin;
    }

    // Update timer
    else {
        omm_hud_update_timer(timer, m);
    }

    // Compute and return alpha
    return omm_hud_timer_compute_alpha(timer, 0x00, 0xFF);
}

static u8 omm_hud_update_coins_timer(OmmHudTimer *timer, struct MarioState *m) {

    // Always hide if there is no HUD
    if (OMM_HUD_NONE) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Always show when paused
    if (OMM_HUD_ALWAYS || omm_is_game_paused()) {
        return 0xFF;
    }

    // Always hide in Pro mode
    if (OMM_HUD_PRO) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Reset timer when coins count changes
    if (timer->prev < gHudDisplay.coins) {
        timer->timer = timer->timerMin;
    } else if (timer->prev > gHudDisplay.coins) {
        timer->timer = timer->timerMin + 1;
    }

    // Update timer
    else {
        omm_hud_update_timer(timer, m);
    }

    // Update previous value
    timer->prev = gHudDisplay.coins;

    // Compute and return alpha
    return omm_hud_timer_compute_alpha(timer, 0x00, 0xFF);
}

static u8 omm_hud_update_vibe_timer(OmmHudTimer *timer, struct MarioState *m) {

    // Always hide if there is no HUD
    if (OMM_HUD_NONE) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Always show when paused
    if (OMM_HUD_ALWAYS || omm_is_game_paused()) {
        return 0xFF;
    }

    // Always hide in Pro mode
    if (OMM_HUD_PRO) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Reset timer if not full
    if (gOmmPeach->vibeGauge != 0) {
        timer->timer = timer->timerMin;
    }

    // Update timer
    else {
        omm_hud_update_timer(timer, m);
    }

    // Compute and return alpha
    return omm_hud_timer_compute_alpha(timer, 0x00, 0xFF);
}

static u8 omm_hud_update_health_timer(OmmHudTimer *timer, struct MarioState *m) {

    // Always hide if there is no HUD
    if (OMM_HUD_NONE) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Always show when paused
    if (OMM_HUD_ALWAYS || omm_is_game_paused()) {
        return 0xFF;
    }

    // Reset timer when healing/taking damage
    if (gOmmMario->state.health.state != OMM_HEALTH_STATE_NONE || omm_health_is_healing(m) || omm_health_is_taking_damage(m) || (OMM_MOVESET_CLASSIC && m->health != timer->prev)) {
        timer->timer = timer->timerMin;
    }

    // Update timer
    else {
        omm_hud_update_timer(timer, m);
    }

    // Update previous value
    timer->prev = m->health;

    // Compute and return alpha
    return omm_hud_timer_compute_alpha(timer, OMM_HUD_PRO ? 0x00 : 0x40, 0xFF);
}

static u8 omm_hud_update_breath_timer(OmmHudTimer *timer, struct MarioState *m) {
    bool notFull = gOmmMario->state.breath != 0;
    bool noBreathLoss = (m->flags & MARIO_METAL_CAP) || omm_mario_is_capture(m);
    bool halfTransparency = !OMM_HUD_PRO && notFull && noBreathLoss;

    // Always hide if there is no HUD
    if (OMM_HUD_NONE) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // When paused, invisible if faded out, fully opaque otherwise
    if (omm_is_game_paused()) {
        return (timer->timer < timer->timerMax) * 0xFF;
    }

    // Update timer
    if (notFull && (!noBreathLoss || timer->prev > gOmmMario->state.breath)) {
        timer->timer = timer->timerMin;
        timer->timerMax = 6;
    } else {
        if (halfTransparency) timer->timerMax = 12;
        timer->timer = min_s(timer->timer + 1, timer->timerMax / (halfTransparency ? 2 : 1));
    }

    // Update previous value
    timer->prev = gOmmMario->state.breath;

    // No fading
    if (OMM_HUD_ALWAYS) {
        if (timer->timer < 0) return 0xFF;
        if (timer->timer >= timer->timerMax) return 0x00;
        if (halfTransparency) return 0x7F;
        return 0xFF;
    }

    // Compute and return alpha
    return relerp_0_1_s(timer->timer, 0, timer->timerMax, 0xFF, 0x00);
}

static u8 omm_hud_update_camera_timer(OmmHudTimer *timer, struct MarioState *m) {

    // Always hide if there is no HUD
    if (OMM_HUD_NONE) {
        timer->timer = timer->timerMax;
        return 0x00;
    }

    // Always show when paused
    if (OMM_HUD_ALWAYS || omm_is_game_paused()) {
        return 0xFF;
    }

    // Reset timer if status changes
    s32 status = (omm_camera_is_available(m) ? ((s32) (gOmmCameraMode << 4) + omm_camera_get_relative_dist_mode()) : update_camera_hud_status(gCamera));
    if (timer->prev != status) {
        timer->timer = timer->timerMin;
    }

    // Update timer
    else {
        omm_hud_update_timer(timer, m);
    }

    // Update previous value
    timer->prev = status;

    // Compute and return alpha
    return omm_hud_timer_compute_alpha(timer, 0x00, 0xFF);
}

static OmmHudTimer sOmmHudStarsTimer[1]   = { { .update = omm_hud_update_stars_timer,   .timerMin = -45, .timerMax = OMM_HUD_TIMER_MAX, .timer = OMM_HUD_TIMER_MAX, .prev = 0 } };
static OmmHudTimer sOmmHudSparklyTimer[1] = { { .update = omm_hud_update_sparkly_timer, .timerMin = -45, .timerMax = OMM_HUD_TIMER_MAX, .timer = OMM_HUD_TIMER_MAX, .prev = 0 } };
static OmmHudTimer sOmmHudCoinsTimer[1]   = { { .update = omm_hud_update_coins_timer,   .timerMin = -60, .timerMax = OMM_HUD_TIMER_MAX, .timer = OMM_HUD_TIMER_MAX, .prev = 0 } };
static OmmHudTimer sOmmHudVibeTimer[1]    = { { .update = omm_hud_update_vibe_timer,    .timerMin = -30, .timerMax = OMM_HUD_TIMER_MAX, .timer = OMM_HUD_TIMER_MAX, .prev = 0 } };
static OmmHudTimer sOmmHudHealthTimer[1]  = { { .update = omm_hud_update_health_timer,  .timerMin = -30, .timerMax = OMM_HUD_TIMER_MAX, .timer = OMM_HUD_TIMER_MAX, .prev = 0 } };
static OmmHudTimer sOmmHudBreathTimer[1]  = { { .update = omm_hud_update_breath_timer,  .timerMin = -30, .timerMax = 12,                .timer = 12,                .prev = 0 } };
static OmmHudTimer sOmmHudCameraTimer[1]  = { { .update = omm_hud_update_camera_timer,  .timerMin = -30, .timerMax = OMM_HUD_TIMER_MAX, .timer = OMM_HUD_TIMER_MAX, .prev = 0 } };

#if OMM_CODE_DEV
u8 *gOmmHudStarsTimerData   = (u8 *) sOmmHudStarsTimer;
u8 *gOmmHudSparklyTimerData = (u8 *) sOmmHudSparklyTimer;
u8 *gOmmHudCoinsTimerData   = (u8 *) sOmmHudCoinsTimer;
u8 *gOmmHudVibeTimerData    = (u8 *) sOmmHudVibeTimer;
u8 *gOmmHudHealthTimerData  = (u8 *) sOmmHudHealthTimer;
u8 *gOmmHudBreathTimerData  = (u8 *) sOmmHudBreathTimer;
u8 *gOmmHudCameraTimerData  = (u8 *) sOmmHudCameraTimer;
const s32 gOmmHudTimerSize  = sizeof(OmmHudTimer);
#endif

OMM_ROUTINE_LEVEL_ENTRY(omm_render_at_level_entry) {
    sOmmHudStarsTimer->timer = sOmmHudStarsTimer->timerMax;
    sOmmHudSparklyTimer->timer = sOmmHudSparklyTimer->timerMax;
    sOmmHudCoinsTimer->timer = sOmmHudCoinsTimer->timerMax;
    sOmmHudCoinsTimer->prev = 0;
    sOmmHudVibeTimer->timer = sOmmHudVibeTimer->timerMax;
    sOmmHudHealthTimer->timer = sOmmHudHealthTimer->timerMax;
    sOmmHudHealthTimer->prev = gMarioState->health;
    sOmmHudBreathTimer->timer = sOmmHudBreathTimer->timerMax;
    sOmmHudCameraTimer->timer = sOmmHudCameraTimer->timerMax;
    sOmmRadarTimer = OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_TOTAL;
}

//
// Stars
//

static const f32 STAR_SHADING[2][2] = { { 0.f, 0.f }, { 0.625f, 1.f } };
static const f32 STAR_OPACITY[2][2] = { { 0.f, 0.5f }, { 1.f, 1.f } };

static s16 omm_render_hud_course_stars(s16 x, s16 y, s16 w, u8 alpha, s32 levelNum) {
    s32 courseNum = omm_level_get_course(levelNum);
    u8 starLevelFlags = omm_stars_get_level_flags(levelNum, OMM_GAME_MODE);
    u8 starSaveFlags = omm_save_file_get_star_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, courseNum - 1);
    u8 starFlags = omm_stars_get_flags();
    for (u8 starIndex = 0; starIndex != OMM_NUM_STARS_MAX_PER_COURSE; ++starIndex) {
        u8 flag = (1 << starIndex);
        if (flag & starLevelFlags) {
#if OMM_GAME_IS_SMSR
            const BehaviorScript *bhv = NULL;
            if (omm_stars_get_star_data(levelNum, 1, starIndex, &bhv, NULL) && bhv == bhvCustomSMSRStarReplica &&
                omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE) < 121) {
                continue;
            }
#endif
            bool collected = (flag & starSaveFlags) != 0;
            bool available = (flag & starFlags) == 0;
            f32 shading = STAR_SHADING[collected][available];
            f32 opacity = STAR_OPACITY[collected][available];
            const void *tex = omm_render_get_star_glyph(clamp_s(courseNum, 0, 16), OMM_GAME_MODE, OMM_EXTRAS_COLORED_STARS, collected);
            const u8 *rgb = omm_render_get_star_rgb(OMM_GAME_MODE, OMM_EXTRAS_COLORED_STARS, true);
            OMM_RENDER_DEFINE_GLYPH_SIZE(w);
            omm_render_glyph_hud(x, y, rgb[0] * shading, rgb[1] * shading, rgb[2] * shading, alpha * opacity, tex, false);
            x += OMM_RENDER_STAR_OFFSET_X;
        }
    }
    return x;
}

static s16 omm_render_hud_sparkly_stars(s16 x, s16 y, s16 w, u8 alpha, s32 levelNum) {
    if (OMM_SPARKLY_MODE_IS_ENABLED) {
        s32 sparklyMode = gOmmSparklyMode;
        s32 starIndex = omm_sparkly_get_star_index_in_level(sparklyMode, levelNum, gCurrAreaIndex);
        if (starIndex != -1) {
            bool collected = omm_sparkly_is_star_collected(sparklyMode, starIndex);
            bool state = OMM_SPARKLY_STATE_IS_OK;
            f32 shading = STAR_SHADING[collected][state];
            f32 opacity = STAR_OPACITY[collected][state];
            OMM_RENDER_DEFINE_GLYPH_SIZE(w);
            omm_render_glyph_hud(x, y, 0xFF * shading, 0xFF * shading, 0xFF * shading, alpha * opacity, OMM_SPARKLY_HUD_GLYPH[sparklyMode], false);
            return x + OMM_RENDER_STAR_OFFSET_X;
        }
    }
    return x;
}

void omm_render_hud_stars(s16 x, s16 y, s16 w, u8 alpha, s32 levelNum) {
    x = omm_render_hud_course_stars(x, y, w, alpha, levelNum);
    x = omm_render_hud_sparkly_stars(x, y, w, alpha, levelNum);
}

//
// Coins
//

void omm_render_hud_coins(s16 x, s16 y, s16 w, u8 alpha, s32 coins) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(w);
    omm_render_glyph_hud(x, y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_COIN, false);
    omm_render_number_hud(x + (OMM_RENDER_VALUE_NUMBER_X - OMM_RENDER_VALUE_GLYPH_X), y, alpha, coins, 3, true, false);
}

//
// Camera
//

static void omm_render_hud_camera(struct MarioState *m) {
    if (HUD_DISPLAY_CAMERA && !gOmmGlobals->hideHudCamera) {
        u8 alpha = sOmmHudCameraTimer->update(sOmmHudCameraTimer, m);
        if (alpha) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);

            // OMM cam
            if (!OMM_CAMERA_CLASSIC) {
                omm_render_create_dl_ortho_matrix();
                omm_render_glyph_hud(OMM_RENDER_CAMERA_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_CAMERA, false);
                if (OMM_CAMERA_8_DIRECTIONS) {
                    omm_render_glyph_hud(OMM_RENDER_CAMERA_X + OMM_RENDER_CAMERA_OFFSET_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_CAMERA_8, false);
                } else if (OMM_CAMERA_16_DIRECTIONS) {
                    omm_render_glyph_hud(OMM_RENDER_CAMERA_X + OMM_RENDER_CAMERA_OFFSET_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_CAMERA_16, false);
                } else if (OMM_CAMERA_INF_DIRECTIONS) {
                    omm_render_glyph_hud(OMM_RENDER_CAMERA_X + OMM_RENDER_CAMERA_OFFSET_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_CAMERA_INF, false);
                }
                s32 mode = omm_camera_get_relative_dist_mode();
                s32 zoom = abs_s(mode);
                s32 sign = (mode > 0 ? -1 : +1);
                for (s32 i = zoom; i > 0; --i) {
                    omm_render_glyph_hud(OMM_RENDER_CAMERA_X, OMM_RENDER_CAMERA_Y + sign * (OMM_RENDER_CAMERA_OFFSET_Y + OMM_RENDER_CAMERA_STEP_Y * i), 0xFF, 0xFF, 0xFF, alpha, (mode > 0 ? OMM_TEXTURE_HUD_CAMERA_FAR : OMM_TEXTURE_HUD_CAMERA_NEAR), false);
                }
            }

            // Puppy/better cam
            else if (BETTER_CAM_IS_ENABLED) {
                omm_render_create_dl_ortho_matrix();
                omm_render_glyph_hud(OMM_RENDER_CAMERA_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, ((const u8 **) main_hud_camera_lut)[GLYPH_CAM_CAMERA], false);
#if BETTER_CAM_IS_PUPPY_CAM
                omm_render_glyph_hud(OMM_RENDER_CAMERA_X + OMM_RENDER_CAMERA_OFFSET_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, texture_hud_char_puppycam, false);
#else
                omm_render_glyph_hud(OMM_RENDER_CAMERA_X + OMM_RENDER_CAMERA_OFFSET_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, ((const u8 **) main_hud_camera_lut)[GLYPH_CAM_LAKITU_HEAD], false);
#endif
            }

            // Lakitu cam
            else {
                s32 status = update_camera_hud_status(gCamera);
                if (status) {
                    omm_render_create_dl_ortho_matrix();
                    omm_render_glyph_hud(OMM_RENDER_CAMERA_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, ((const u8 **) main_hud_camera_lut)[GLYPH_CAM_CAMERA], false);
                    switch (status & CAM_STATUS_MODE_GROUP) {
                        case CAM_STATUS_MARIO:  omm_render_glyph_hud(OMM_RENDER_CAMERA_X + OMM_RENDER_CAMERA_OFFSET_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, ((const u8 **) main_hud_camera_lut)[GLYPH_CAM_MARIO_HEAD], false); break;
                        case CAM_STATUS_LAKITU: omm_render_glyph_hud(OMM_RENDER_CAMERA_X + OMM_RENDER_CAMERA_OFFSET_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, ((const u8 **) main_hud_camera_lut)[GLYPH_CAM_LAKITU_HEAD], false); break;
                        case CAM_STATUS_FIXED:  omm_render_glyph_hud(OMM_RENDER_CAMERA_X + OMM_RENDER_CAMERA_OFFSET_X, OMM_RENDER_CAMERA_Y, 0xFF, 0xFF, 0xFF, alpha, ((const u8 **) main_hud_camera_lut)[GLYPH_CAM_FIXED], false); break;
                    }
                    switch (status & CAM_STATUS_C_MODE_GROUP) {
                        case CAM_STATUS_C_DOWN: omm_render_glyph_hud_small(OMM_RENDER_CAMERA_X + OMM_RENDER_GLYPH_SIZE / 4, OMM_RENDER_CAMERA_Y + OMM_RENDER_GLYPH_SIZE / 4 - OMM_RENDER_CAMERA_OFFSET_Y, 0xFF, 0xFF, 0xFF, alpha, ((const u8 **) main_hud_camera_lut)[GLYPH_CAM_ARROW_DOWN], false); break;
                        case CAM_STATUS_C_UP:   omm_render_glyph_hud_small(OMM_RENDER_CAMERA_X + OMM_RENDER_GLYPH_SIZE / 4, OMM_RENDER_CAMERA_Y + OMM_RENDER_GLYPH_SIZE / 4 + OMM_RENDER_CAMERA_OFFSET_Y, 0xFF, 0xFF, 0xFF, alpha, ((const u8 **) main_hud_camera_lut)[GLYPH_CAM_ARROW_UP], false); break;
                    }
                }
            }
        }
    }
    gOmmGlobals->hideHudCamera = false;
}

//
// Health meter
//

static void omm_render_hud_power_meter_background(Gfx **gfx, Vtx **vtx, f32 x, f32 y, f32 alpha) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    for (s32 i = 0; i != OMM_RENDER_POWER_BACKGROUND_NUM_TRIS; ++i) {
        f32 a0 = ((i + 0) * 65536.f) / OMM_RENDER_POWER_BACKGROUND_NUM_TRIS;
        f32 a1 = ((i + 1) * 65536.f) / OMM_RENDER_POWER_BACKGROUND_NUM_TRIS;
        f32 d  = OMM_RENDER_POWER_SEGMENT_RADIUS_3 + 0.4f;
        gSPVertex((*gfx)++, (*vtx), 3, 0);
        gSP1Triangle((*gfx)++, 0, 2, 1, 0x0);
        *((*vtx)++) = (Vtx) { { { x,                y,                0 }, 0, { 0, 0 }, { OMM_RENDER_POWER_BACKGROUND_R, OMM_RENDER_POWER_BACKGROUND_G, OMM_RENDER_POWER_BACKGROUND_B, (u8) clamp_s(alpha, 0x00, 0xFF) } } };
        *((*vtx)++) = (Vtx) { { { x + d * sins(a0), y + d * coss(a0), 0 }, 0, { 0, 0 }, { OMM_RENDER_POWER_BACKGROUND_R, OMM_RENDER_POWER_BACKGROUND_G, OMM_RENDER_POWER_BACKGROUND_B, (u8) clamp_s(alpha, 0x00, 0xFF) } } };
        *((*vtx)++) = (Vtx) { { { x + d * sins(a1), y + d * coss(a1), 0 }, 0, { 0, 0 }, { OMM_RENDER_POWER_BACKGROUND_R, OMM_RENDER_POWER_BACKGROUND_G, OMM_RENDER_POWER_BACKGROUND_B, (u8) clamp_s(alpha, 0x00, 0xFF) } } };
    }
}

static void omm_render_hud_power_meter_get_segment_color(s32 index0, s32 index1, f32 ticks, Vec3s color) {
    if (OMM_MOVESET_ODYSSEY && index0 != index1) {
        f32 delta = (fmodf(ticks - 0.001f, OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT) + 0.001f) / OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT;
        color[0] = lerp_s(delta, OMM_RENDER_POWER_SEGMENT_COLOR_R(index1), OMM_RENDER_POWER_SEGMENT_COLOR_R(index0));
        color[1] = lerp_s(delta, OMM_RENDER_POWER_SEGMENT_COLOR_G(index1), OMM_RENDER_POWER_SEGMENT_COLOR_G(index0));
        color[2] = lerp_s(delta, OMM_RENDER_POWER_SEGMENT_COLOR_B(index1), OMM_RENDER_POWER_SEGMENT_COLOR_B(index0));
    } else {
        color[0] = OMM_RENDER_POWER_SEGMENT_COLOR_R(index0);
        color[1] = OMM_RENDER_POWER_SEGMENT_COLOR_G(index0);
        color[2] = OMM_RENDER_POWER_SEGMENT_COLOR_B(index0);
    }
}

static void omm_render_hud_power_meter_segments(Gfx **gfx, Vtx **vtx, f32 x0, f32 y0, f32 alpha, f32 ticks, bool isLifeUp) {

    // Colors
    s32 index0 = !isLifeUp * (
        (ticks <= OMM_RENDER_POWER_TICKS_MAX) +
        (ticks <= OMM_RENDER_POWER_TICKS_LOW) +
        (ticks <= OMM_RENDER_POWER_TICKS_CRITICAL)
    );
    s32 index1 = !isLifeUp * min_s(3, 1 + (
        (ticks <= OMM_RENDER_POWER_TICKS_NORMAL) +
        (ticks <= OMM_RENDER_POWER_TICKS_LOW) +
        (ticks <= OMM_RENDER_POWER_TICKS_CRITICAL)
    ));
    Vec3s center, border;
    omm_render_hud_power_meter_get_segment_color(index0, index1, ticks, center);
    vec3s_interpolate(border, gVec3sZero, center, 0.75f);

    // Render
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    f32 ma = (65536.f * ticks) / (OMM_RENDER_POWER_FULL_SEGMENTS * OMM_RENDER_POWER_TICKS_PER_SEGMENT);
    f32 da = (65536.f)         / (OMM_RENDER_POWER_FULL_SEGMENTS * OMM_RENDER_POWER_SEGMENT_NUM_QUADS);
    for (s32 segment = 0; segment != OMM_RENDER_POWER_FULL_SEGMENTS; ++segment) {
        f32 sa = (segment * 65536 + 32768) / OMM_RENDER_POWER_FULL_SEGMENTS;
        f32 x = x0 + OMM_RENDER_POWER_SEGMENT_DELTA * sins(sa);
        f32 y = y0 + OMM_RENDER_POWER_SEGMENT_DELTA * coss(sa);
        for (s32 i = 0; i != OMM_RENDER_POWER_SEGMENT_NUM_QUADS; ++i) {
            f32 a0 = da * (segment * OMM_RENDER_POWER_SEGMENT_NUM_QUADS + i + 0);
            f32 a1 = da * (segment * OMM_RENDER_POWER_SEGMENT_NUM_QUADS + i + 1);
            bool blank = (((a0 + a1) / 2) > ma);
            gSPVertex((*gfx)++, (*vtx), 8, 0);
            gSP2Triangles((*gfx)++, 2, 6, 3, 0x0, 3, 6, 7, 0x0);
            gSP2Triangles((*gfx)++, 1, 5, 2, 0x0, 2, 5, 6, 0x0);
            gSP2Triangles((*gfx)++, 0, 4, 1, 0x0, 1, 4, 5, 0x0);
            *((*vtx)++) = (Vtx) { { { x + sins(a0) * OMM_RENDER_POWER_SEGMENT_RADIUS_0, y + coss(a0) * OMM_RENDER_POWER_SEGMENT_RADIUS_0, 0 }, 0, { 0, 0 }, { (blank ? 0xFF : border[0]), (blank ? 0xFF : border[1]), (blank ? 0xFF : border[2]), (u8) clamp_s(alpha, 0x00, 0xFF) } } };
            *((*vtx)++) = (Vtx) { { { x + sins(a0) * OMM_RENDER_POWER_SEGMENT_RADIUS_1, y + coss(a0) * OMM_RENDER_POWER_SEGMENT_RADIUS_1, 0 }, 0, { 0, 0 }, { (blank ? 0xFF : center[0]), (blank ? 0xFF : center[1]), (blank ? 0xFF : center[2]), (u8) clamp_s(alpha, 0x00, 0xFF) } } };
            *((*vtx)++) = (Vtx) { { { x + sins(a0) * OMM_RENDER_POWER_SEGMENT_RADIUS_2, y + coss(a0) * OMM_RENDER_POWER_SEGMENT_RADIUS_2, 0 }, 0, { 0, 0 }, { (blank ? 0xFF : center[0]), (blank ? 0xFF : center[1]), (blank ? 0xFF : center[2]), (u8) clamp_s(alpha, 0x00, 0xFF) } } };
            *((*vtx)++) = (Vtx) { { { x + sins(a0) * OMM_RENDER_POWER_SEGMENT_RADIUS_3, y + coss(a0) * OMM_RENDER_POWER_SEGMENT_RADIUS_3, 0 }, 0, { 0, 0 }, { (blank ? 0xFF : border[0]), (blank ? 0xFF : border[1]), (blank ? 0xFF : border[2]), (u8) clamp_s(alpha, 0x00, 0xFF) } } };
            *((*vtx)++) = (Vtx) { { { x + sins(a1) * OMM_RENDER_POWER_SEGMENT_RADIUS_0, y + coss(a1) * OMM_RENDER_POWER_SEGMENT_RADIUS_0, 0 }, 0, { 0, 0 }, { (blank ? 0xFF : border[0]), (blank ? 0xFF : border[1]), (blank ? 0xFF : border[2]), (u8) clamp_s(alpha, 0x00, 0xFF) } } };
            *((*vtx)++) = (Vtx) { { { x + sins(a1) * OMM_RENDER_POWER_SEGMENT_RADIUS_1, y + coss(a1) * OMM_RENDER_POWER_SEGMENT_RADIUS_1, 0 }, 0, { 0, 0 }, { (blank ? 0xFF : center[0]), (blank ? 0xFF : center[1]), (blank ? 0xFF : center[2]), (u8) clamp_s(alpha, 0x00, 0xFF) } } };
            *((*vtx)++) = (Vtx) { { { x + sins(a1) * OMM_RENDER_POWER_SEGMENT_RADIUS_2, y + coss(a1) * OMM_RENDER_POWER_SEGMENT_RADIUS_2, 0 }, 0, { 0, 0 }, { (blank ? 0xFF : center[0]), (blank ? 0xFF : center[1]), (blank ? 0xFF : center[2]), (u8) clamp_s(alpha, 0x00, 0xFF) } } };
            *((*vtx)++) = (Vtx) { { { x + sins(a1) * OMM_RENDER_POWER_SEGMENT_RADIUS_3, y + coss(a1) * OMM_RENDER_POWER_SEGMENT_RADIUS_3, 0 }, 0, { 0, 0 }, { (blank ? 0xFF : border[0]), (blank ? 0xFF : border[1]), (blank ? 0xFF : border[2]), (u8) clamp_s(alpha, 0x00, 0xFF) } } };
        }
    }
}

static void omm_render_hud_power_meter_heart(Gfx **gfx, Vtx **vtx, f32 x, f32 y, f32 alpha, f32 ticks) {

    // Colors
    Vec3s center, border;
    vec3s_interpolate(center, OMM_RENDER_POWER_HEART_COLOR_RED, OMM_RENDER_POWER_HEART_COLOR_WHITE, OMM_RENDER_POWER_HEART_PULSE(ticks));
    vec3s_interpolate(border, gVec3sZero, center, 0.8f);

    // Render
    for (s32 i = 0; i != OMM_RENDER_POWER_HEART_NUM_PIECES; ++i) {
        f32 t0 = relerp_f(i + 0, 0, OMM_RENDER_POWER_HEART_NUM_PIECES, -1.f, +1.f);
        f32 t1 = relerp_f(i + 1, 0, OMM_RENDER_POWER_HEART_NUM_PIECES, -1.f, +1.f);

        // Heart shape <3
        f32 a0 = (t0 * 0x10000) / (2.f * M_PI);
        f32 a1 = (t1 * 0x10000) / (2.f * M_PI);
        f32 r0 = (1.f - abs_f(t0)) * (1.f + 3.f * abs_f(t0));
        f32 r1 = (1.f - abs_f(t1)) * (1.f + 3.f * abs_f(t1));
        f32 dx0 = r0 * sins(a0);
        f32 dy0 = r0 * coss(a0) - 0.65f;
        f32 dx1 = r1 * sins(a1);
        f32 dy1 = r1 * coss(a1) - 0.65f;
        gSPVertex((*gfx)++, (*vtx), 5, 0);
        gSP2Triangles((*gfx)++, 2, 1, 4, 0x0, 1, 3, 4, 0x0);
        gSP1Triangle((*gfx)++, 1, 0, 3, 0x0);
        OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
        *((*vtx)++) = (Vtx) { { { x,                                           y,                                           0 }, 0, { 0, 0 }, { center[0], center[1], center[2], (u8) clamp_s(alpha, 0x00, 0xFF) } } };
        *((*vtx)++) = (Vtx) { { { x + dx0 * OMM_RENDER_POWER_HEART_RADIUS_1_X, y + dy0 * OMM_RENDER_POWER_HEART_RADIUS_1_Y, 0 }, 0, { 0, 0 }, { center[0], center[1], center[2], (u8) clamp_s(alpha, 0x00, 0xFF) } } };
        *((*vtx)++) = (Vtx) { { { x + dx0 * OMM_RENDER_POWER_HEART_RADIUS_2_X, y + dy0 * OMM_RENDER_POWER_HEART_RADIUS_2_Y, 0 }, 0, { 0, 0 }, { border[0], border[1], border[2], (u8) clamp_s(alpha, 0x00, 0xFF) } } };
        *((*vtx)++) = (Vtx) { { { x + dx1 * OMM_RENDER_POWER_HEART_RADIUS_1_X, y + dy1 * OMM_RENDER_POWER_HEART_RADIUS_1_Y, 0 }, 0, { 0, 0 }, { center[0], center[1], center[2], (u8) clamp_s(alpha, 0x00, 0xFF) } } };
        *((*vtx)++) = (Vtx) { { { x + dx1 * OMM_RENDER_POWER_HEART_RADIUS_2_X, y + dy1 * OMM_RENDER_POWER_HEART_RADIUS_2_Y, 0 }, 0, { 0, 0 }, { border[0], border[1], border[2], (u8) clamp_s(alpha, 0x00, 0xFF) } } };
    }
}

static void omm_render_hud_power_meter_number(Gfx **gfx, f32 x, f32 y, f32 alpha, f32 segments) {
    static const char *OMM_HUD_POWER_METER_NUMBER_GLYPHS[] = {
        OMM_TEXTURE_HUD_0, OMM_TEXTURE_HUD_1,
        OMM_TEXTURE_HUD_2, OMM_TEXTURE_HUD_3,
        OMM_TEXTURE_HUD_4, OMM_TEXTURE_HUD_5,
        OMM_TEXTURE_HUD_6, OMM_TEXTURE_HUD_7,
        OMM_TEXTURE_HUD_8, OMM_TEXTURE_HUD_9,
    };
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    f32 w = OMM_RENDER_POWER_SIZE;
    f32 h = OMM_RENDER_POWER_SIZE;
    s16 x0 = (s16) (x * 4.f);
    s16 y0 = (s16) ((SCREEN_HEIGHT - h - y) * 4.f);
    s16 x1 = (s16) ((x + w) * 4.f);
    s16 y1 = (s16) ((SCREEN_HEIGHT - y) * 4.f);
    gDPSetTexturePersp((*gfx)++, G_TP_NONE);
    gDPSetRenderMode((*gfx)++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetCombineLERP((*gfx)++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    gDPSetEnvColor((*gfx)++, 0xFF, 0xFF, 0xFF, (u8) clamp_s(alpha, 0x00, 0xFF));
    gDPSetTextureFilter((*gfx)++, G_TF_POINT);
    gSPTexture((*gfx)++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock((*gfx)++, OMM_HUD_POWER_METER_NUMBER_GLYPHS[(s32) segments], G_IM_FMT_RGBA, G_IM_SIZ_32b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gSPTextureRectangle((*gfx)++, x0, y0, x1, y1, G_TX_RENDERTILE, 0, 0, (s16) (0x4000 / w), (s16) (0x4000 / h));
    gDPSetTexturePersp((*gfx)++, G_TP_PERSP);
    gDPSetRenderMode((*gfx)++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetCombineLERP((*gfx)++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetEnvColor((*gfx)++, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetTextureFilter((*gfx)++, G_TF_BILERP);
    gSPTexture((*gfx)++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
}

static void omm_render_hud_power_meter_health_gauge(Gfx **gfx, Vtx **vtx, f32 shakeX, f32 shakeY, f32 relPos, f32 alpha, s32 state, f32 ticks, bool isLifeUp) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    f32 x = lerp_f(relPos, OMM_RENDER_POWER_X, OMM_RENDER_POWER_ANIM_X) + shakeX;
    f32 y = lerp_f(relPos, OMM_RENDER_POWER_Y, OMM_RENDER_POWER_ANIM_Y) + shakeY;
    if (!isLifeUp) {
        f32 ticksNoLifeUp = min_f(ticks, omm_health_get_max_ticks_hud(0));
        omm_render_hud_power_meter_segments(gfx, vtx, x, y, alpha, ticksNoLifeUp, false);
        if (state == OMM_HEALTH_STATE_LIFE_UP || ticks == ticksNoLifeUp) {
            omm_render_hud_power_meter_heart(gfx, vtx, x, y - OMM_RENDER_POWER_HEART_OFFSET_Y, alpha, ticksNoLifeUp);
            omm_render_hud_power_meter_number(gfx, x - OMM_RENDER_POWER_NUMBER_OFFSET_X, y - OMM_RENDER_POWER_NUMBER_OFFSET_Y, alpha, OMM_RENDER_POWER_TICKS_TO_SEGMENTS(ticksNoLifeUp));
        }
    } else if (OMM_MOVESET_ODYSSEY_3H && ticks > omm_health_to_ticks(OMM_HEALTH_ODYSSEY_3_SEGMENTS)) {
        omm_render_hud_power_meter_background(gfx, vtx, x, y, alpha);
        omm_render_hud_power_meter_segments(gfx, vtx, x, y, alpha, ticks - omm_health_get_max_ticks_hud(0), true);
        omm_render_hud_power_meter_heart(gfx, vtx, x, y - OMM_RENDER_POWER_HEART_OFFSET_Y, alpha, ticks);
        omm_render_hud_power_meter_number(gfx, x - OMM_RENDER_POWER_NUMBER_OFFSET_X, y - OMM_RENDER_POWER_NUMBER_OFFSET_Y, alpha, OMM_RENDER_POWER_TICKS_TO_SEGMENTS(ticks));
    }
}

static Gfx *omm_render_hud_power_meter_with_offset(Gfx *pos, Gfx *gfx, Vtx *vtx, f32 shakeX, f32 shakeY, f32 relPos, f32 alpha, s32 state, f32 ticks) {
    gSPDisplayList(pos++, gfx);
    gSPClearGeometryMode(gfx++, G_LIGHTING);
    gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetCombineLERP(gfx++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    if (OMM_MOVESET_ODYSSEY) {
        OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
        omm_render_hud_power_meter_health_gauge(&gfx, &vtx, shakeX, shakeY, relPos * (state != OMM_HEALTH_STATE_LIFE_UP), alpha, state, ticks, false);
        omm_render_hud_power_meter_health_gauge(&gfx, &vtx, shakeX - OMM_RENDER_POWER_LIFE_UP_OFFSET_X, shakeY, relPos, alpha, state, ticks, true);
    } else {
        omm_render_hud_power_meter_health_gauge(&gfx, &vtx, shakeX, shakeY, 0.f, alpha, state, ticks, false);
    }
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    gSPEndDisplayList(gfx);
    return pos;
}

static void omm_render_hud_power_meter(struct MarioState *m) {
    if (HUD_DISPLAY_POWER_METER && !OMM_MOVESET_ODYSSEY_1H
#if OMM_GAME_IS_SMSR
        && !(gOmmGlobals->yoshiMode && gOmmGlobals->booZeroLife)
#endif
    ) {
        u8 alpha = sOmmHudHealthTimer->update(sOmmHudHealthTimer, m);
        if (alpha) {
            omm_render_create_dl_ortho_matrix();
            s32 state = gOmmMario->state.health.state;
            s32 timer = gOmmMario->state.health.timer;
            s32 ticks = (OMM_MOVESET_ODYSSEY ? gOmmMario->state.ticks : (m->health >> 8));
            f32 shakeX, shakeY, relPos;
            Gfx *gfx = omm_alloc_gfx(2 * OMM_RENDER_POWER_BACKGROUND_NUM_TRIS + 32 * OMM_RENDER_POWER_SEGMENT_NUM_QUADS +  6 * OMM_RENDER_POWER_HEART_NUM_PIECES + 45);
            Vtx *vtx = omm_alloc_vtx(3 * OMM_RENDER_POWER_BACKGROUND_NUM_TRIS + 64 * OMM_RENDER_POWER_SEGMENT_NUM_QUADS + 10 * OMM_RENDER_POWER_HEART_NUM_PIECES);
            omm_health_state_get_anim_params(state, timer, &shakeX, &shakeY, &relPos);
            interp_data_update(sOmmPowerMeter, sOmmPowerMeter->s1 == state, gDisplayListHead, gfx, vtx, shakeX, shakeY, relPos, alpha, state, ticks);
            gDisplayListHead = omm_render_hud_power_meter_with_offset(gDisplayListHead,
                sOmmPowerMeter->gfx,
                sOmmPowerMeter->vtx,
                sOmmPowerMeter->x0,
                sOmmPowerMeter->y0,
                sOmmPowerMeter->z0,
                sOmmPowerMeter->a0,
                sOmmPowerMeter->s0,
                sOmmPowerMeter->t0
            );
            return;
        }
    }
    interp_data_reset(sOmmPowerMeter);
}

//
// Breath meter
//

static Gfx *omm_render_hud_breath_meter_gauge(Gfx *pos, Gfx *gfx, Vtx *vtx, f32 x, f32 y, f32 alpha) {
    f32 t = clamp_0_1_f((f32) (gOmmMario->state.breath - (OMM_BREATH_MAX_DURATION / 100.f)) / (f32) (OMM_BREATH_MAX_DURATION * 0.99f));
    f32 j = t * OMM_RENDER_BREATH_NUM_QUADS;
    bool blink = (t == 1.f) && (gGlobalTimer & 1);

    // Display list
    gSPClearGeometryMode(pos++, G_LIGHTING);
    gDPSetRenderMode(pos++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetCombineLERP(pos++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gSPDisplayList(pos++, gfx);
    gSPSetGeometryMode(pos++, G_LIGHTING);

    // Triangles
    for (s32 i = 0; i != OMM_RENDER_BREATH_NUM_QUADS; ++i) {
        gSPVertex(gfx++, vtx + 4 * i, 4, 0);
        gSP2Triangles(gfx++, 0, 1, 2, 0, 3, 2, 1, 0);
    }
    gSPEndDisplayList(gfx);

    // Vertices
    for (s32 i = 0; i != OMM_RENDER_BREATH_NUM_QUADS; ++i) {
        s32 a0 = ((i + 0) * -65536) / OMM_RENDER_BREATH_NUM_QUADS;
        s32 a1 = ((i + 1) * -65536) / OMM_RENDER_BREATH_NUM_QUADS;
        *(vtx++) = (Vtx) {{{ x + OMM_RENDER_BREATH_RADIUS_IN  * sins(a0), y + OMM_RENDER_BREATH_RADIUS_IN  * coss(a0), 0 }, 0, { 0, 0 }, { OMM_RENDER_BREATH_COLOR_R, OMM_RENDER_BREATH_COLOR_G, OMM_RENDER_BREATH_COLOR_B, (u8) clamp_s(alpha, 0x00, 0xFF) }}};
        *(vtx++) = (Vtx) {{{ x + OMM_RENDER_BREATH_RADIUS_OUT * sins(a0), y + OMM_RENDER_BREATH_RADIUS_OUT * coss(a0), 0 }, 0, { 0, 0 }, { OMM_RENDER_BREATH_COLOR_R, OMM_RENDER_BREATH_COLOR_G, OMM_RENDER_BREATH_COLOR_B, (u8) clamp_s(alpha, 0x00, 0xFF) }}};
        *(vtx++) = (Vtx) {{{ x + OMM_RENDER_BREATH_RADIUS_IN  * sins(a1), y + OMM_RENDER_BREATH_RADIUS_IN  * coss(a1), 0 }, 0, { 0, 0 }, { OMM_RENDER_BREATH_COLOR_R, OMM_RENDER_BREATH_COLOR_G, OMM_RENDER_BREATH_COLOR_B, (u8) clamp_s(alpha, 0x00, 0xFF) }}};
        *(vtx++) = (Vtx) {{{ x + OMM_RENDER_BREATH_RADIUS_OUT * sins(a1), y + OMM_RENDER_BREATH_RADIUS_OUT * coss(a1), 0 }, 0, { 0, 0 }, { OMM_RENDER_BREATH_COLOR_R, OMM_RENDER_BREATH_COLOR_G, OMM_RENDER_BREATH_COLOR_B, (u8) clamp_s(alpha, 0x00, 0xFF) }}};
    }

    return pos;
}

static void omm_render_hud_breath_meter(struct MarioState *m) {
    if (HUD_DISPLAY_BREATH_METER && OMM_MOVESET_ODYSSEY) {
        u8 alpha = sOmmHudBreathTimer->update(sOmmHudBreathTimer, m);
        if (alpha) {

            // Camera normal
            Vec3f camN;
            vec3f_dif(camN, gLakituState.focus, gLakituState.pos);
            vec3f_normalize(camN);

            // Camera horizontal axis
            s16 camYaw = atan2s(camN[2], camN[0]) - 0x4000;
            Vec3f camH = { sins(camYaw), 0.f, coss(camYaw) };

            // Camera vertical axis
            Vec3f camV;
            vec3f_cross(camV, camN, camH);
            vec3f_normalize(camV);

            // Mario's position to camera plane
            Vec2f pos2d;
            f32 dist;
            vec3f_to_2d_plane(pos2d, &dist, m->pos, gLakituState.pos, camN, camH, camV);
            if (dist > 0.f) {
                f32 x = (SCREEN_WIDTH  / 2) * (1.f + (pos2d[0] / dist)) + ((SCREEN_WIDTH  * 2.f) / sqrtf(dist));
                f32 y = (SCREEN_HEIGHT / 2) * (1.f - (pos2d[1] / dist)) + ((SCREEN_HEIGHT * 3.f) / sqrtf(dist));
                Gfx *gfx = omm_alloc_gfx(2 * OMM_RENDER_BREATH_NUM_QUADS + 1);
                Vtx *vtx = omm_alloc_vtx(4 * OMM_RENDER_BREATH_NUM_QUADS);
                interp_data_update(sOmmBreathMeter, sOmmBreathMeter->t1 >= (f32) gGlobalTimer - 1, gDisplayListHead, gfx, vtx, x, y, 0, alpha, 0, gGlobalTimer);
                gDisplayListHead = omm_render_hud_breath_meter_gauge(gDisplayListHead, sOmmBreathMeter->gfx, sOmmBreathMeter->vtx, sOmmBreathMeter->x0, sOmmBreathMeter->y0, sOmmBreathMeter->a0);
                return;
            }
        }
    }
    interp_data_reset(sOmmBreathMeter);
}

//
// Timer
//

static Gfx *omm_render_hud_timer_m_s_ms(Gfx *pos, Vtx *vtx, f32 timer) {
    static const char *OMM_HUD_TIMER_GLYPHS[] = {
        OMM_TEXTURE_HUD_0, OMM_TEXTURE_HUD_1,
        OMM_TEXTURE_HUD_2, OMM_TEXTURE_HUD_3,
        OMM_TEXTURE_HUD_4, OMM_TEXTURE_HUD_5,
        OMM_TEXTURE_HUD_6, OMM_TEXTURE_HUD_7,
        OMM_TEXTURE_HUD_8, OMM_TEXTURE_HUD_9,
        OMM_TEXTURE_HUD_M, OMM_TEXTURE_HUD_S,
    };
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);

    // Clock
    s16 clockW = (OMM_RENDER_GLYPH_SIZE * 8) / 5;
    s16 clockX = OMM_RENDER_TIMER_X - (clockW * 3) / 2;
    s16 clockY = OMM_RENDER_TIMER_Y - (clockW - OMM_RENDER_GLYPH_SIZE) / 2;

    // Rotating clock hand
    s16 handW = (clockW * 3) / 4;
    s16 handX = (clockW / 2) + clockX;
    s16 handY = (clockW / 2) + clockY;
    s16 handR = ((timer * 0x10000) / 30.f);
    vtx[0] = (Vtx) {{{ handX + handW * coss(0xA000 - handR), handY + handW * sins(0xA000 - handR), 0 }, 0, { 0x0000, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[1] = (Vtx) {{{ handX + handW * coss(0xE000 - handR), handY + handW * sins(0xE000 - handR), 0 }, 0, { 0x2000, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[2] = (Vtx) {{{ handX + handW * coss(0x2000 - handR), handY + handW * sins(0x2000 - handR), 0 }, 0, { 0x2000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[3] = (Vtx) {{{ handX + handW * coss(0x6000 - handR), handY + handW * sins(0x6000 - handR), 0 }, 0, { 0x0000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    gDPSetTexturePersp(pos++, G_TP_NONE);
    gDPSetRenderMode(pos++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetCombineLERP(pos++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetTextureFilter(pos++, G_TF_POINT);
    gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock(pos++, OMM_TEXTURE_HUD_CLOCK_HAND, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gSPVertex(pos++, vtx, 4, 0);
    gSP2Triangles(pos++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPSetTexturePersp(pos++, G_TP_PERSP);
    gDPSetRenderMode(pos++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineLERP(pos++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetTextureFilter(pos++, G_TF_BILERP);
    gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);

    // Timer digits
    s32 ms = 1000.f * (timer / 30.f);
    s32 glyphs[] = {
        ((ms / 600000) % 10),
        ((ms /  60000) % 10),
        10,
        ((ms /  10000) %  6),
        ((ms /   1000) % 10),
        11,
        ((ms /    100) % 10),
        ((ms /     10) % 10),
    };
    for (s32 i = 0, j = 0; i != 8; ++i) {
        j = max_s(j, glyphs[i] * (glyphs[i] < 10));
        s16 x = OMM_RENDER_TIMER_X + i * OMM_RENDER_TIMER_OFFSET_X;
        s16 y = OMM_RENDER_TIMER_Y;
        s16 w = OMM_RENDER_GLYPH_SIZE;
        s16 h = OMM_RENDER_GLYPH_SIZE;
        gDPSetTexturePersp(pos++, G_TP_NONE);
        gDPSetRenderMode(pos++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
        gDPSetCombineLERP(pos++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
        gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF - (0xAA * (j == 0)));
        gDPSetTextureFilter(pos++, G_TF_POINT);
        gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(pos++, OMM_HUD_TIMER_GLYPHS[glyphs[i]], G_IM_FMT_RGBA, G_IM_SIZ_32b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
        gSPTextureRectangle(pos++, (x) << 2, (SCREEN_HEIGHT - h - y) << 2, (x + w) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, (0x4000 / w), (0x4000 / h));
        gDPSetTexturePersp(pos++, G_TP_PERSP);
        gDPSetRenderMode(pos++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        gDPSetCombineLERP(pos++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
        gDPSetTextureFilter(pos++, G_TF_BILERP);
        gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    }
    return pos;
}

static void omm_render_hud_timer() {
    if (HUD_DISPLAY_TIMER && !OMM_HUD_NONE) {
        OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);

        // Clock
        s16 clockW = (OMM_RENDER_GLYPH_SIZE * 8) / 5;
        s16 clockX = OMM_RENDER_TIMER_X - (clockW * 3) / 2;
        s16 clockY = OMM_RENDER_TIMER_Y - (clockW - OMM_RENDER_GLYPH_SIZE) / 2;
        omm_render_texrect(clockX, clockY, clockW, clockW, 16, 16, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_HUD_CLOCK, false);

        // Clock hand and timer
        omm_render_create_dl_ortho_matrix();
        Vtx *vtx = omm_alloc_vtx(4);
        interp_data_update(sOmmTimer, sOmmTimer->s1 >= (f32) gGlobalTimer - 1, gDisplayListHead, NULL, vtx, 0, 0, 0, 0, gGlobalTimer, gHudDisplay.timer);
        gDisplayListHead = omm_render_hud_timer_m_s_ms(gDisplayListHead, sOmmTimer->vtx, sOmmTimer->t0);
        return;
    }
    interp_data_reset(sOmmTimer);
}

//
// Cannon
//

static void omm_render_hud_cannon_reticle() {
    if (HUD_DISPLAY_EFFECTS && !OMM_HUD_NONE && gCurrentArea && gCurrentArea->camera && gCurrentArea->camera->mode == CAMERA_MODE_INSIDE_CANNON) {
        render_hud_cannon_reticle();
    }
}

//
// Values
//

static s16 omm_render_hud_star_count(struct MarioState *m, s16 y) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    s32 levelNum = gCurrLevelNum;

    // Course stars + course Sparkly star
    if (!omm_is_game_paused() && !OMM_LEVEL_IS_BOWSER_FIGHT(levelNum) && omm_stars_get_level_flags(levelNum, OMM_GAME_MODE) != 0) {
        if (HUD_DISPLAY_STAR_COUNT || HUD_DISPLAY_SPARKLY_COUNT) {
            sOmmHudSparklyTimer->update(sOmmHudSparklyTimer, m);
            u8 alpha = sOmmHudStarsTimer->update(sOmmHudStarsTimer, m);
            if (alpha) {
                omm_render_hud_stars(OMM_RENDER_VALUE_GLYPH_X, y, OMM_RENDER_GLYPH_SIZE, alpha, levelNum);
                y -= OMM_RENDER_OFFSET_Y;
            }
        }
    }

    // Star counter + Sparkly star counter
    else {

        // Star counter
        if (HUD_DISPLAY_STAR_COUNT) {
            u8 alpha = sOmmHudStarsTimer->update(sOmmHudStarsTimer, m);
            if (alpha) {
                const void *tex = omm_render_get_star_glyph(0, OMM_GAME_MODE, OMM_EXTRAS_COLORED_STARS, true);
                const u8 *rgb = omm_render_get_star_rgb(OMM_GAME_MODE, OMM_EXTRAS_COLORED_STARS, true);
                omm_render_glyph_hud(OMM_RENDER_VALUE_GLYPH_X, y, rgb[0], rgb[1], rgb[2], alpha, tex, false);
                omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, alpha, gHudDisplay.stars, 3, true, false);
                y -= OMM_RENDER_OFFSET_Y;
            }
        }

        // Sparkly star counter
        if (HUD_DISPLAY_SPARKLY_COUNT) {
            u8 alpha = sOmmHudSparklyTimer->update(sOmmHudSparklyTimer, m);
            if (alpha) {
                s32 sparklyMode = gOmmSparklyMode;
                omm_render_glyph_hud(OMM_RENDER_VALUE_GLYPH_X, y, 0xFF, 0xFF, 0xFF, alpha, OMM_SPARKLY_HUD_GLYPH[sparklyMode], false);
                omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, alpha, omm_sparkly_get_collected_count(sparklyMode), 3, true, false);
                y -= OMM_RENDER_OFFSET_Y;
            }
        }
    }
    return y;
}

static s16 omm_render_hud_coin_count(struct MarioState *m, s16 y) {
    if (HUD_DISPLAY_COIN_COUNT) {
        u8 alpha = sOmmHudCoinsTimer->update(sOmmHudCoinsTimer, m);
        if (alpha) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);

            // Coins
            omm_render_hud_coins(OMM_RENDER_VALUE_GLYPH_X, y, OMM_RENDER_GLYPH_SIZE, alpha, gHudDisplay.coins);

            // Sparkles
            if (OMM_HUD_VANISHING) {

                // New sparkle
                if (gHudDisplay.coins > 0 && sOmmHudCoinsTimer->timer == sOmmHudCoinsTimer->timerMin) {
                    for (s32 i = 0; i != OMM_RENDER_HUD_COIN_SPARKLE_COUNT; ++i) {
                        if (sOmmCoinSparkles[i] <= 0) {
                            sOmmCoinSparkles[i] = OMM_RENDER_HUD_COIN_SPARKLE_FRAMES * OMM_RENDER_HUD_COIN_SPARKLE_DELAY;
                            break;
                        }
                    }
                }

                // Update
                for (s32 i = 0; i != OMM_RENDER_HUD_COIN_SPARKLE_COUNT; ++i) {
                    if (sOmmCoinSparkles[i]-- > 0) {
                        omm_render_texrect(
                            OMM_RENDER_HUD_COIN_SPARKLE_X, OMM_RENDER_HUD_COIN_SPARKLE_Y,
                            OMM_RENDER_HUD_COIN_SPARKLE_SIZE, OMM_RENDER_HUD_COIN_SPARKLE_SIZE,
                            32, 32, 0xFF, 0xFF, 0xFF, 0xFF,
                            (const void *) (array_of(const char *) {
                                OMM_ASSET_SPARKLE_5,
                                OMM_ASSET_SPARKLE_4,
                                OMM_ASSET_SPARKLE_3,
                                OMM_ASSET_SPARKLE_2,
                                OMM_ASSET_SPARKLE_1,
                                OMM_ASSET_SPARKLE_0,
                            })[sOmmCoinSparkles[i] / OMM_RENDER_HUD_COIN_SPARKLE_DELAY],
                            false
                        );
                    }
                }
            }
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static s16 omm_render_hud_vibe_gauge(struct MarioState *m, s16 y) {
    if (HUD_DISPLAY_VIBE_GAUGE && OMM_PLAYER_IS_PEACH) {
        u8 alpha = sOmmHudVibeTimer->update(sOmmHudVibeTimer, m);
        if (alpha) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            switch (gOmmPeach->vibeType) {
                case OMM_PEACH_VIBE_TYPE_NONE:  omm_render_glyph_hud(OMM_RENDER_VALUE_GLYPH_X, y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_VIBE_NORMAL, false); break;
                case OMM_PEACH_VIBE_TYPE_JOY:   omm_render_glyph_hud(OMM_RENDER_VALUE_GLYPH_X, y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_VIBE_JOY, false); break;
                case OMM_PEACH_VIBE_TYPE_RAGE:  omm_render_glyph_hud(OMM_RENDER_VALUE_GLYPH_X, y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_VIBE_RAGE, false); break;
                case OMM_PEACH_VIBE_TYPE_GLOOM: omm_render_glyph_hud(OMM_RENDER_VALUE_GLYPH_X, y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_VIBE_GLOOM, false); break;
                case OMM_PEACH_VIBE_TYPE_CALM:  omm_render_glyph_hud(OMM_RENDER_VALUE_GLYPH_X, y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_VIBE_CALM, false); break;
            }
            s32 value = relerp_0_1_s(gOmmPeach->vibeGauge, OMM_PEACH_VIBE_GAUGE_MAX, OMM_PEACH_VIBE_GAUGE_HEART_INC, 0, 100);
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, alpha, value, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static s16 omm_render_hud_power_up(struct MarioState *m, s16 y) {
    if (HUD_DISPLAY_CAP_TIMER && OMM_HUD_SHOW_NON_ESSENTIAL) {
        static const char *OMM_HUD_POWER_UP_GLYPHS[4][2][2] = {
            { { OMM_TEXTURE_HUD_CAP_MARIO,   OMM_TEXTURE_HUD_CAP_MARIO_METAL   },
              { OMM_TEXTURE_HUD_CAPPY_MARIO, OMM_TEXTURE_HUD_CAPPY_MARIO_METAL }, },
            { { OMM_TEXTURE_HUD_CAP_PEACH,   OMM_TEXTURE_HUD_CAP_PEACH_METAL   },
              { OMM_TEXTURE_HUD_CAPPY_PEACH, OMM_TEXTURE_HUD_CAPPY_PEACH_METAL }, },
            { { OMM_TEXTURE_HUD_CAP_LUIGI,   OMM_TEXTURE_HUD_CAP_LUIGI_METAL   },
              { OMM_TEXTURE_HUD_CAPPY_LUIGI, OMM_TEXTURE_HUD_CAPPY_LUIGI_METAL }, },
            { { OMM_TEXTURE_HUD_CAP_WARIO,   OMM_TEXTURE_HUD_CAP_WARIO_METAL   },
              { OMM_TEXTURE_HUD_CAPPY_WARIO, OMM_TEXTURE_HUD_CAPPY_WARIO_METAL }, },
        };

        s32 idx = omm_player_get_selected_index_model_and_sounds();
        bool cc = !OMM_CAP_CLASSIC;
        bool wc = (m->flags & MARIO_WING_CAP) != 0;
        bool mc = (m->flags & MARIO_METAL_CAP) != 0;
        bool vc = (m->flags & MARIO_VANISH_CAP) != 0;

        // Render the cap power-up remaining time
        if (m->capTimer > 0 && (wc || mc || vc)) {
            u8 alpha = (vc ? 0x80 : 0xFF);
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            if (wc) omm_render_glyph_hud(OMM_RENDER_VALUE_GLYPH_X, y, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_CAP_WINGS, false);
            omm_render_glyph_hud(OMM_RENDER_VALUE_GLYPH_X, y, 0xFF, 0xFF, 0xFF, alpha, OMM_HUD_POWER_UP_GLYPHS[idx][cc][mc], false);
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, 0xFF, (m->capTimer + 29) / 30, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static void omm_render_sparkly_stars_background(s16 y) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    s16 x0 = GFX_DIMENSIONS_FROM_LEFT_EDGE(-1);
    s16 x1 = OMM_RENDER_VALUE_NUMBER_X + 4 * OMM_RENDER_GLYPH_SIZE;
    s16 y0 = y - (OMM_RENDER_OFFSET_Y - OMM_RENDER_GLYPH_SIZE) / 2;
    s16 y1 = y0 + OMM_RENDER_OFFSET_Y;
    u8 r = OMM_SPARKLY_HUD_COLOR_R[gOmmSparklyMode];
    u8 g = OMM_SPARKLY_HUD_COLOR_G[gOmmSparklyMode];
    u8 b = OMM_SPARKLY_HUD_COLOR_B[gOmmSparklyMode];

    Vtx *vtx = omm_alloc_vtx(4);
    vtx[0] = (Vtx) {{{ x0, y0, 0 }, 0, { 0, 0 }, { r, g, b, 0xA0 }}};
    vtx[1] = (Vtx) {{{ x1, y0, 0 }, 0, { 0, 0 }, { r, g, b, 0x00 }}};
    vtx[2] = (Vtx) {{{ x0, y1, 0 }, 0, { 0, 0 }, { r, g, b, 0xA0 }}};
    vtx[3] = (Vtx) {{{ x1, y1, 0 }, 0, { 0, 0 }, { r, g, b, 0x00 }}};

    omm_render_create_dl_ortho_matrix();
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0x0, 2, 1, 3, 0x0);
    gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
}

static s16 omm_render_hud_sparkly_coins(s16 y) {
    s32 numCoins = omm_sparkly_context_get_data(OMM_SPARKLY_DATA_COINS);
    if (numCoins > 0 && OMM_SPARKLY_STATE_IS_OK) {
        s32 collectedCoins = (
            gOmmSparklyContext->coinsYellow * 1 +
            gOmmSparklyContext->coinsRed * 2 +
            gOmmSparklyContext->coinsBlue * 5
        );
        s32 coins = numCoins - collectedCoins;
        if (coins > 0) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            s32 coinW = OMM_RENDER_GLYPH_SIZE;
            s32 coinX = OMM_RENDER_VALUE_GLYPH_X - (coinW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 coinY = y - (coinW - OMM_RENDER_GLYPH_SIZE) / 2;

            // Coin type
            static const struct { u8 r, g, b; s32 value; } OMM_HUD_SPARKLY_COINS[] = {
                { 0xFF, 0xFF, 0x00, 1 }, // Yellow
                { 0xFF, 0x00, 0x00, 2 }, // Red
                { 0x78, 0x78, 0xFF, 5 }, // Blue
                { 0xFF, 0xFF, 0x00, 1 },
            };
            s32 coinType = clamp_s(-2 +
                2 * omm_sparkly_context_get_data(OMM_SPARKLY_DATA_ONLY_COIN_Y) +
                3 * omm_sparkly_context_get_data(OMM_SPARKLY_DATA_ONLY_COIN_R) +
                4 * omm_sparkly_context_get_data(OMM_SPARKLY_DATA_ONLY_COIN_B),
                0, 3
            );

            // Render the background
            omm_render_sparkly_stars_background(y);

            // Render the coin icon
            omm_render_texrect(
                coinX, coinY, coinW, coinW, 32, 32,
                OMM_HUD_SPARKLY_COINS[coinType].r, OMM_HUD_SPARKLY_COINS[coinType].g, OMM_HUD_SPARKLY_COINS[coinType].b, 0xFF,
                (const void *) (array_of(const char *) {
                    OMM_ASSET_COIN_0,
                    OMM_ASSET_COIN_1,
                    OMM_ASSET_COIN_2,
                    OMM_ASSET_COIN_3,
                })[(gGlobalTimer >> 1) & 3],
                false
            );

            // Render the number
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, 0xFF, coins / OMM_HUD_SPARKLY_COINS[coinType].value, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static s16 omm_render_hud_sparkly_red_coins(s16 y) {
    s32 numRedCoins = omm_sparkly_context_get_data(OMM_SPARKLY_DATA_RED_COINS);
    if (numRedCoins > 0 && OMM_SPARKLY_STATE_IS_OK) {
        s32 redCoins = numRedCoins - gOmmSparklyContext->coinsRed;
        if (redCoins > 0) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            s32 redCoinW = OMM_RENDER_GLYPH_SIZE;
            s32 redCoinX = OMM_RENDER_VALUE_GLYPH_X - (redCoinW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 redCoinY = y - (redCoinW - OMM_RENDER_GLYPH_SIZE) / 2;

            // Render the background
            omm_render_sparkly_stars_background(y);

            // Render the red coin or star shard icon
            if (omm_sparkly_context_get_data(OMM_SPARKLY_DATA_OBJECT_TYPE) == OMM_SPARKLY_DATA_OBJECT_TYPE_SHARDS) {
                omm_render_texrect(
                    redCoinX, redCoinY, redCoinW, redCoinW,
                    32, 32, 0xFF, 0xFF, 0xFF, 0xFF,
                    (const void *) (array_of(const char *) {
                        OMM_ASSET_SPARKLE_5,
                        OMM_ASSET_SPARKLE_4,
                        OMM_ASSET_SPARKLE_3,
                        OMM_ASSET_SPARKLE_2,
                        OMM_ASSET_SPARKLE_1,
                        OMM_ASSET_SPARKLE_0,
                    })[(gGlobalTimer >> 1) % 6],
                    false
                );
            } else {
                omm_render_texrect(
                    redCoinX, redCoinY, redCoinW, redCoinW,
                    32, 32, 0xFF, 0x00, 0x00, 0xFF,
                    (const void *) (array_of(const char *) {
                        OMM_ASSET_COIN_0,
                        OMM_ASSET_COIN_1,
                        OMM_ASSET_COIN_2,
                        OMM_ASSET_COIN_3,
                    })[(gGlobalTimer >> 1) & 3],
                    false
                );
            }

            // Render the number
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, 0xFF, redCoins, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static s16 omm_render_hud_sparkly_secrets(s16 y) {
    if (omm_sparkly_context_get_data(OMM_SPARKLY_DATA_OBJECT_TYPE) == OMM_SPARKLY_DATA_OBJECT_TYPE_SECRETS && OMM_SPARKLY_STATE_IS_OK) {
        s32 secrets = gOmmSparklyContext->secrets;
        if (secrets > 0) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            s32 secretsW = (OMM_RENDER_GLYPH_SIZE * 3) / 5;
            s32 secretsX = OMM_RENDER_VALUE_GLYPH_X - (secretsW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 secretsY = y - (secretsW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 sparklesW = OMM_RENDER_GLYPH_SIZE;
            s32 sparklesX = OMM_RENDER_VALUE_GLYPH_X - (sparklesW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 sparklesY = y - (sparklesW - OMM_RENDER_GLYPH_SIZE) / 2;

            // Render the background
            omm_render_sparkly_stars_background(y);

            // Render secret icon
            omm_render_texrect(
                secretsX, secretsY, secretsW, secretsW,
                32, 32, 0xFF, 0xFF, 0xFF, 0xFF,
                (const void *) OMM_ASSET_SECRET,
                false
            );

            // Render sparkles
            omm_render_texrect(
                sparklesX, sparklesY, sparklesW, sparklesW,
                32, 32, 0xFF, 0xFF, 0xFF, 0xC0,
                (const void *) (array_of(const char *) {
                    OMM_ASSET_WHITE_SPARKLE_0,
                    OMM_ASSET_WHITE_SPARKLE_1,
                    OMM_ASSET_WHITE_SPARKLE_2,
                    OMM_ASSET_WHITE_SPARKLE_3,
                    OMM_ASSET_WHITE_SPARKLE_4,
                    OMM_ASSET_WHITE_SPARKLE_5,
                })[(gGlobalTimer >> 1) % 6],
                false
            );

            // Render the number
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, 0xFF, secrets, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static s16 omm_render_hud_sparkly_flames(s16 y) {
    if (omm_sparkly_context_get_data(OMM_SPARKLY_DATA_OBJECT_TYPE) == OMM_SPARKLY_DATA_OBJECT_TYPE_FLAMES && OMM_SPARKLY_STATE_IS_OK) {
        s32 flames = omm_sparkly_context_get_remaining_flames();
        if (flames > 0) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            s32 flameW = 2 * OMM_RENDER_GLYPH_SIZE;
            s32 flameX = OMM_RENDER_VALUE_GLYPH_X - (flameW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 flameY = y - (flameW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 flameT = (gGlobalTimer / 2) % 8;

            // Render the background
            omm_render_sparkly_stars_background(y);

            // Render the animated flame
            omm_render_texrect(
                flameX, flameY, flameW, flameW,
                32, 32, 0xFF, 0x00, 0x00, 0xFF,
                (const void *) (array_of(const char *) {
                    OMM_ASSET_FLAME_0,
                    OMM_ASSET_FLAME_1,
                    OMM_ASSET_FLAME_2,
                    OMM_ASSET_FLAME_3,
                    OMM_ASSET_FLAME_4,
                    OMM_ASSET_FLAME_5,
                    OMM_ASSET_FLAME_6,
                    OMM_ASSET_FLAME_7,
                })[flameT],
                false
            );

            // Render the number
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, 0xFF, flames, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static s16 omm_render_hud_sparkly_boxes(s16 y) {
    if (omm_sparkly_context_get_data(OMM_SPARKLY_DATA_OBJECT_TYPE) == OMM_SPARKLY_DATA_OBJECT_TYPE_BOXES && OMM_SPARKLY_STATE_IS_OK) {
        s32 boxes = omm_sparkly_context_get_remaining_boxes();
        if (boxes > 0) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            s32 boxW = OMM_RENDER_GLYPH_SIZE;
            s32 boxX = OMM_RENDER_VALUE_GLYPH_X - (boxW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 boxY = y - (boxW - OMM_RENDER_GLYPH_SIZE) / 2;

            // Render the background
            omm_render_sparkly_stars_background(y);

            // Render the box icon
            omm_render_texrect(
                boxX, boxY, boxW, boxW,
                32, 32, 0xFF, 0xFF, 0xFF, 0xFF,
                (const void *) OMM_ASSET_EXCLAMATION_BOX,
                false
            );

            // Render the number
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, 0xFF, boxes, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static s16 omm_render_hud_sparkly_mushrooms(s16 y) {
    s32 numMushrooms = omm_sparkly_context_get_data(OMM_SPARKLY_DATA_MUSHROOMS);
    if (numMushrooms > 0 && OMM_SPARKLY_STATE_IS_OK) {
        s32 mushrooms = numMushrooms - gOmmSparklyContext->mushrooms;
        if (mushrooms > 0) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            s32 mushroomW = OMM_RENDER_GLYPH_SIZE;
            s32 mushroomX = OMM_RENDER_VALUE_GLYPH_X - (mushroomW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 mushroomY = y - (mushroomW - OMM_RENDER_GLYPH_SIZE) / 2;

            // Render the background
            omm_render_sparkly_stars_background(y);

            // Render the mushroom icon
            omm_render_texrect(
                mushroomX, mushroomY, mushroomW, mushroomW,
                32, 64, 0xFF, 0xFF, 0xFF, 0xFF,
                (const void *) OMM_ASSET_MUSHROOM_1UP,
                false
            );

            // Render the number
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, 0xFF, mushrooms, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static s16 omm_render_hud_sparkly_star_rings(s16 y) {
    if (omm_sparkly_context_get_data(OMM_SPARKLY_DATA_OBJECT_TYPE) == OMM_SPARKLY_DATA_OBJECT_TYPE_RINGS && OMM_SPARKLY_STATE_IS_OK) {
        s32 rings = omm_sparkly_context_get_remaining_star_rings();
        if (rings > 0) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            s32 ringW = OMM_RENDER_GLYPH_SIZE;
            s32 ringX = OMM_RENDER_VALUE_GLYPH_X + ringW / 2;
            s32 ringY = y + ringW / 2;
            f32 ringS = ringW / 512.f;

            // Render the background
            omm_render_sparkly_stars_background(y);

            // Render the star ring
            create_dl_translation_matrix(G_MTX_PUSH, ringX, ringY, 0);
            create_dl_scale_matrix(G_MTX_PUSH, ringS, ringS, ringS);
            gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
            gSPDisplayList(gDisplayListHead++, omm_geo_star_ring[11]);
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

            // Render the number
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, 0xFF, rings, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

static s16 omm_render_hud_sparkly_enemies(s16 y) {
    if (omm_sparkly_context_get_data(OMM_SPARKLY_DATA_OBJECT_TYPE) == OMM_SPARKLY_DATA_OBJECT_TYPE_ENEMIES && OMM_SPARKLY_STATE_IS_OK) {
        s32 enemies = omm_sparkly_context_get_remaining_enemies();
        if (enemies > 0) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            s32 enemyW = (OMM_RENDER_GLYPH_SIZE * 7) / 5;
            s32 enemyX = OMM_RENDER_VALUE_GLYPH_X - (enemyW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 enemyY = y - (enemyW - OMM_RENDER_GLYPH_SIZE) / 2;
            s32 enemyT = gGlobalTimer % 30;
            s32 enemyRow = (enemyT / 8);
            s32 enemyCol = (enemyT % 8);

            Vtx *vtx = omm_alloc_vtx(4);
            vtx[0] = (Vtx) {{{ enemyX,          enemyY,          0 }, 0, { (512 * 32 * (enemyCol + 0)) / 8, (512 * 32 * (enemyRow + 1)) / 4 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
            vtx[1] = (Vtx) {{{ enemyX + enemyW, enemyY,          0 }, 0, { (512 * 32 * (enemyCol + 1)) / 8, (512 * 32 * (enemyRow + 1)) / 4 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
            vtx[2] = (Vtx) {{{ enemyX + enemyW, enemyY + enemyW, 0 }, 0, { (512 * 32 * (enemyCol + 1)) / 8, (512 * 32 * (enemyRow + 0)) / 4 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
            vtx[3] = (Vtx) {{{ enemyX,          enemyY + enemyW, 0 }, 0, { (512 * 32 * (enemyCol + 0)) / 8, (512 * 32 * (enemyRow + 0)) / 4 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};

            Gfx *tri = omm_alloc_gfx(3);
            gSPVertex(tri + 0, vtx, 4, 0);
            gSP2Triangles(tri + 1, 0, 1, 2, 0, 0, 2, 3, 0);
            gSPEndDisplayList(tri + 2);

            // Render the background
            omm_render_sparkly_stars_background(y);

            // Render the enemy
            omm_render_create_dl_ortho_matrix();
            OMM_RENDER_ENABLE_ALPHA(gDisplayListHead++);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0);
            gDPLoadTextureBlock(gDisplayListHead++, OMM_TEXTURE_HUD_ENEMY, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPDisplayList(gDisplayListHead++, tri);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
            gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
            gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);

            // Render the number
            omm_render_number_hud(OMM_RENDER_VALUE_NUMBER_X, y, 0xFF, enemies, 3, true, false);
            y -= OMM_RENDER_OFFSET_Y;
        }
    }
    return y;
}

typedef struct { u64 flag; s32 value; const char *texture; bool checkBypassRestrictions; } SparklyRestriction;
static const SparklyRestriction OMM_HUD_SPARKLY_RESTRICTIONS[] = {
    { OMM_SPARKLY_DATA_1_HEALTH, TRUE, OMM_TEXTURE_HUD_1_HEALTH, false },
    { OMM_SPARKLY_DATA_3_HEALTH, TRUE, OMM_TEXTURE_HUD_3_HEALTH, false },
    { OMM_SPARKLY_DATA_NO_GROUND, TRUE, OMM_TEXTURE_HUD_NO_GROUND, false },
    { OMM_SPARKLY_DATA_BUTTONS, A_BUTTON | B_BUTTON | START_BUTTON | Z_TRIG | X_BUTTON | Y_BUTTON, OMM_TEXTURE_HUD_NO_BUTTON, true },
    { OMM_SPARKLY_DATA_BUTTONS, X_BUTTON, OMM_TEXTURE_HUD_NO_X_BUTTON, true },
    { OMM_SPARKLY_DATA_NO_MUSHROOM, TRUE, OMM_TEXTURE_HUD_NO_MUSHROOM, true },
    { OMM_SPARKLY_DATA_NO_CAPTURE, TRUE, OMM_TEXTURE_HUD_NO_CAPTURE, true },
    { OMM_SPARKLY_DATA_NO_VIBE, TRUE, OMM_TEXTURE_HUD_NO_VIBE, true },
};

static void omm_render_hud_sparkly_restrictions(s16 y) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    s16 x = OMM_RENDER_VALUE_NUMBER_X;
    array_for_each_(const SparklyRestriction, restriction, OMM_HUD_SPARKLY_RESTRICTIONS) {
        if (omm_sparkly_context_get_data(restriction->flag) == restriction->value &&
            (!restriction->checkBypassRestrictions || !OMM_SPARKLY_BYPASS_RESTRICTIONS)
        ) {
            omm_render_texrect(
                x, y, OMM_RENDER_GLYPH_SIZE, OMM_RENDER_GLYPH_SIZE,
                32, 32, 0xFF, 0xFF, 0xFF, 0xFF,
                (const void *) restriction->texture,
                false
            );
            x += (OMM_RENDER_GLYPH_SIZE + 1);
        }
    }
}

static bool omm_render_hud_sparkly_elements(struct MarioState *m, s16 *y, bool *renderPowerUp) {
    if (HUD_DISPLAY_SPARKLY_OBJECTIVE && OMM_HUD_SHOW_NON_ESSENTIAL && !OMM_LEVEL_IS_BOWSER_FIGHT(gCurrLevelNum) && OMM_SPARKLY_MODE_IS_ENABLED && OMM_SPARKLY_STATE_IS_OK) {
        s16 displayFlags = gHudDisplay.flags;

        // Star and restrictions
        omm_render_sparkly_stars_background(*y);
        if (omm_is_game_paused()) {
            gHudDisplay.flags = (gHudDisplay.flags & ~HUD_DISPLAY_FLAG_STAR_COUNT) | HUD_DISPLAY_FLAG_SPARKLY_COUNT;
            *y = omm_render_hud_star_count(m, *y);
        } else {
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
            omm_render_hud_sparkly_stars(OMM_RENDER_VALUE_GLYPH_X, *y, OMM_RENDER_GLYPH_SIZE, 0xFF, gCurrLevelNum);
            omm_render_hud_sparkly_restrictions(*y);
            *y -= OMM_RENDER_OFFSET_Y;
        }

        // Conditions
        *y = omm_render_hud_sparkly_coins(*y);
        *y = omm_render_hud_sparkly_red_coins(*y);
        *y = omm_render_hud_sparkly_secrets(*y);
        *y = omm_render_hud_sparkly_flames(*y);
        *y = omm_render_hud_sparkly_boxes(*y);
        *y = omm_render_hud_sparkly_mushrooms(*y);
        *y = omm_render_hud_sparkly_star_rings(*y);
        *y = omm_render_hud_sparkly_enemies(*y);
        if (omm_sparkly_context_get_data(OMM_SPARKLY_DATA_CAPS)) {
            gHudDisplay.flags |= HUD_DISPLAY_FLAG_CAP_TIMER;
            OMM_RENDER_BACKUP_DL_HEAD(gDisplayListHead);
            s16 y0 = *y;
            if (omm_render_hud_power_up(m, *y) != y0) {
                OMM_RENDER_RESTORE_DL_HEAD(gDisplayListHead);
                omm_render_sparkly_stars_background(*y);
                *y = omm_render_hud_power_up(m, *y);
                *renderPowerUp = false;
            }
        }

        // Make sure to hide other counters
        sOmmHudStarsTimer->timer = sOmmHudStarsTimer->timerMax;
        sOmmHudSparklyTimer->timer = sOmmHudSparklyTimer->timerMax;
        sOmmHudCoinsTimer->timer = sOmmHudCoinsTimer->timerMax;
        sOmmHudCoinsTimer->prev = gHudDisplay.coins;
        gHudDisplay.flags = displayFlags;
        return true;
    }
    return false;
}

static void omm_render_hud_values(struct MarioState *m) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    s16 y = OMM_RENDER_VALUE_Y;
    bool renderPowerUp = true;
    if (!omm_render_hud_sparkly_elements(m, &y, &renderPowerUp)) {
        y = omm_render_hud_star_count(m, y);
        y = omm_render_hud_coin_count(m, y);
    }
    y = omm_render_hud_vibe_gauge(m, y);
    if (renderPowerUp) {
        y = omm_render_hud_power_up(m, y);
    }
}

//
// Objects radar
//

static Gfx *omm_render_hud_objects_radar_arrow(Gfx *pos, Vtx *vtx, f32 angle) {
    static const char *OMM_HUD_OBJECTS_RADAR_ARROWS[] = {
        OMM_TEXTURE_HUD_ARROW,
        OMM_TEXTURE_HUD_ARROW_RED_COIN,
        OMM_TEXTURE_HUD_ARROW_SECRET,
        OMM_TEXTURE_HUD_ARROW_STAR,
    };
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);

    // Vertices
    vtx[0] = (Vtx) {{{ 0, 0, 0 }, 0, { 0x0000, 0x1000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[1] = (Vtx) {{{ 0, 0, 0 }, 0, { 0x1000, 0x1000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[2] = (Vtx) {{{ 0, 0, 0 }, 0, { 0x1000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[3] = (Vtx) {{{ 0, 0, 0 }, 0, { 0x0000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    for (s32 i = 0; i != 4; ++i) {
        s16 a = angle + ((i * 0x4000) - 0x6000);
        vtx[i].v.ob[0] = OMM_RENDER_OBJECT_RADAR_X + OMM_RENDER_OBJECT_RADAR_RADIUS * coss(angle + 0x4000) + OMM_RENDER_OBJECT_RADAR_ARROW_SIZE * OMM_INVSQRT2 * coss(a);
        vtx[i].v.ob[1] = OMM_RENDER_OBJECT_RADAR_Y + OMM_RENDER_OBJECT_RADAR_RADIUS * sins(angle + 0x4000) + OMM_RENDER_OBJECT_RADAR_ARROW_SIZE * OMM_INVSQRT2 * sins(a);
    }

    // Display list
    gDPSetRenderMode(pos++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetCombineLERP(pos++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock(pos++, OMM_HUD_OBJECTS_RADAR_ARROWS[sOmmCurrentRadar], G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gSPVertex(pos++, vtx, 4, 0);
    gSP2Triangles(pos++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPSetRenderMode(pos++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineLERP(pos++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    return pos;
}

#if OMM_GAME_IS_SM74

static s32 obj_get_map_side(struct Object *o) {
    return gCurrLevelNum == LEVEL_BITS && OMM_GAME_MODE == OMM_SM74_MODE_EXTREME && o->oPosX > 8192;
}

static struct Object *obj_get_nearest_with_behavior_sm74(struct Object *o, const BehaviorScript *bhv) {
    s32 mapSide = obj_get_map_side(o);
    f32 distMin = 1e10f;
    struct Object *nearest = NULL;
    for_each_object_with_behavior(obj, bhv) {
        if (obj != o) {
            s32 objMapSide = obj_get_map_side(obj);
            if (objMapSide == mapSide) {
                f32 dist = obj_get_distance(o, obj);
                if (dist < distMin) {
                    nearest = obj;
                    distMin = dist;
                }
            }
        }
    }
    return nearest;
}

static struct Object *obj_get_nearest_red_coin(struct MarioState *m) {
    return obj_get_nearest_with_behavior_sm74(m->marioObj, bhvRedCoin);
}

static struct Object *obj_get_nearest_secret(struct MarioState *m) {
    return obj_get_nearest_with_behavior_sm74(m->marioObj, bhvHiddenStarTrigger);
}

static bool obj_is_valid(struct MarioState *m, struct Object *o) {
    return !obj_is_dormant(o) && (o->behavior != bhvMips || o->oMipsStarStatus != MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR) && obj_get_map_side(o) == obj_get_map_side(m->marioObj);
}

#else

static struct Object *obj_get_nearest_red_coin(struct MarioState *m) {
    return obj_get_nearest_with_behavior(m->marioObj, bhvRedCoin);
}

static struct Object *obj_get_nearest_secret(struct MarioState *m) {
    return obj_get_nearest_with_behavior(m->marioObj, bhvHiddenStarTrigger);
}

static bool obj_is_valid(UNUSED struct MarioState *m, struct Object *o) {
    return !obj_is_dormant(o) && (o->behavior != bhvMips || o->oMipsStarStatus != MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR);
}

#endif

static const char *OMM_HUD_OBJECT_RADAR_COIN_TEXTURES[] = {
    OMM_ASSET_COIN_0,
    OMM_ASSET_COIN_1,
    OMM_ASSET_COIN_2,
    OMM_ASSET_COIN_3,
};

static const char *OMM_HUD_OBJECT_RADAR_SECRET_TEXTURES[] = {
    OMM_ASSET_WHITE_SPARKLE_0,
    OMM_ASSET_WHITE_SPARKLE_1,
    OMM_ASSET_WHITE_SPARKLE_2,
    OMM_ASSET_WHITE_SPARKLE_3,
    OMM_ASSET_WHITE_SPARKLE_4,
    OMM_ASSET_WHITE_SPARKLE_5,
};

static const char *OMM_HUD_OBJECT_RADAR_STAR_TEXTURES[] = {
    OMM_ASSET_WHITE_STAR_0,
    OMM_ASSET_WHITE_STAR_1,
    OMM_ASSET_WHITE_STAR_2,
    OMM_ASSET_WHITE_STAR_3,
    OMM_ASSET_WHITE_STAR_4,
    OMM_ASSET_WHITE_STAR_5,
    OMM_ASSET_WHITE_STAR_6,
    OMM_ASSET_WHITE_STAR_7
};

static s32 omm_render_hud_objects_radar_get_size(s32 index) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    switch (index) {
        case 0: return OMM_RENDER_OBJECT_RADAR_COIN_SIZE;
        case 1: return OMM_RENDER_OBJECT_RADAR_SECRET_SIZE;
        case 2: return OMM_RENDER_OBJECT_RADAR_STAR_SIZE;
    }
    return 0;
}

static const char *omm_render_hud_objects_radar_get_texture(s32 index, s32 t) {
    switch (index) {
        case 0: return OMM_HUD_OBJECT_RADAR_COIN_TEXTURES[t % array_length(OMM_HUD_OBJECT_RADAR_COIN_TEXTURES)];
        case 1: return OMM_HUD_OBJECT_RADAR_SECRET_TEXTURES[t % array_length(OMM_HUD_OBJECT_RADAR_SECRET_TEXTURES)];
        case 2: return OMM_HUD_OBJECT_RADAR_STAR_TEXTURES[t % array_length(OMM_HUD_OBJECT_RADAR_STAR_TEXTURES)];
    }
    return NULL;
}

static bool omm_render_hud_objects_radar_get_target(struct MarioState *m, s32 radar, s32 *type, s32 *index, struct Object **obj) {
    switch (radar) {

        // Red coin
        case 1: {
            struct Object *redCoinStar = obj_get_red_coin_star();
            if (redCoinStar) {
                struct Object *redCoin = obj_get_nearest_red_coin(m);
                if (redCoin) {
                    *type = 1;
                    *index = 0;
                    *obj = redCoin;
                    return true;
                }
                if (obj_is_valid(m, redCoinStar) && gRedCoinsCollected >= gOmmArea->redCoins) {
                    *type = 1;
                    *index = 2;
                    *obj = redCoinStar;
                    return true;
                }
            }
        } break;

        // Secret
        case 2: {
            struct Object *secretStar = obj_get_secret_star();
            if (secretStar) {
                struct Object *secret = obj_get_nearest_secret(m);
                if (secret) {
                    *type = 2;
                    *index = 1;
                    *obj = secret;
                    return true;
                }
                if (obj_is_valid(m, secretStar) && obj_get_count_with_behavior(bhvHiddenStarTrigger) == 0) {
                    *type = 2;
                    *index = 2;
                    *obj = secretStar;
                    return true;
                }
            }
        } break;

        // Star
        case 3: {
            struct Object *nearest = NULL;
            f32 distMin = 1e10f;
            u32 stars = 0;

            // Spawned stars
            for_each_in_(const BehaviorScript *, bhv, {
                bhvStarSpawnCoordinates,
                bhvSpawnedStar,
                bhvSpawnedStarNoLevelExit,
            }) {
                for_each_object_with_behavior(obj, *bhv) {
                    if (obj_is_valid(m, obj)) {
                        s32 starIndex = (obj->oBehParams >> 24) & 0xFF;
                        if (starIndex < OMM_NUM_STARS_MAX_PER_COURSE && !(stars & (1 << starIndex)) && !omm_stars_is_collected(starIndex)) {
                            f32 dist = obj_get_distance(m->marioObj, obj);
                            if (dist < distMin) {
                                nearest = obj;
                                distMin = dist;
                            }
                            stars |= (1 << starIndex);
                        }
                    }
                }
            }

            // Level script stars
            for (s32 starIndex = 0; starIndex != OMM_NUM_STARS_MAX_PER_COURSE; ++starIndex) {
                if (!(stars & (1 << starIndex)) && !omm_stars_is_collected(starIndex)) {
                    const BehaviorScript *bhv; s32 behParams;
                    if (omm_stars_get_star_data(gCurrLevelNum, gCurrAreaIndex, starIndex, &bhv, &behParams)) {
                        if (bhv != bhvHiddenRedCoinStar && bhv != bhvBowserCourseRedCoinStar && bhv != bhvHiddenStar) {
                            for_each_object_with_behavior(obj, bhv) {
                                if (obj_is_valid(m, obj) && obj->oBehParams == behParams) {
                                    f32 dist = obj_get_distance(m->marioObj, obj);
                                    if (dist < distMin) {
                                        nearest = obj;
                                        distMin = dist;
                                    }
                                    stars |= (1 << starIndex);
                                }
                            }
                        }
                    }
                }
            }

            // Found star
            if (nearest) {
                *type = 3;
                *index = 2;
                *obj = nearest;
                return true;
            }
        } break;
    }
    return false;
}

static bool omm_render_hud_objects_radar_get_next_available(struct MarioState *m, s32 radar, s32 *type, s32 *index, struct Object **obj) {
    for (s32 i = 0; i != 3; ++i) {
        radar = 1 + (radar % 3);
        if (omm_render_hud_objects_radar_get_target(m, radar, type, index, obj)) {
            return true;
        }
    }
    return false;
}

static void omm_render_hud_objects_radar(struct MarioState *m) {
    if (HUD_DISPLAY_OBJECTS_RADAR && !gOmmGlobals->hideHudRadar && !OMM_LEVEL_IS_BOWSER_FIGHT(gCurrLevelNum) && OMM_HUD_SHOW_NON_ESSENTIAL && OMM_EXTRAS_OBJECTS_RADAR_ENABLED) {
        static const u32 OMM_HUD_OBJECTS_RADAR_COLORS[] = { 0, 0xFF0000FF, 0x00FF00FF, 0xFFFF00FF };
        s32 type; s32 index; struct Object *obj;
        if (omm_render_hud_objects_radar_get_next_available(m, gOmmExtrasObjectsRadar - 1, &type, &index, &obj)) {
            if (sOmmCurrentRadar != type) {
                sOmmCurrentRadar = type;
                sOmmRadarTimer = OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_TOTAL;
            }
            u8 targetR = (OMM_HUD_OBJECTS_RADAR_COLORS[sOmmCurrentRadar] >> 24) & 0xFF;
            u8 targetG = (OMM_HUD_OBJECTS_RADAR_COLORS[sOmmCurrentRadar] >> 16) & 0xFF;
            u8 targetB = (OMM_HUD_OBJECTS_RADAR_COLORS[sOmmCurrentRadar] >>  8) & 0xFF;
            s32 size = omm_render_hud_objects_radar_get_size(index);
            OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);

            // Display target object
            omm_render_create_dl_ortho_matrix();
            omm_render_texrect(
                OMM_RENDER_OBJECT_RADAR_X - size / 2,
                OMM_RENDER_OBJECT_RADAR_Y - size / 2,
                size, size, 32, 32,
                targetR, targetG, targetB, 0xFF,
                omm_render_hud_objects_radar_get_texture(index, gGlobalTimer >> 1),
                false
            );

            // Display radar height indicator
            f32 hdist = obj_get_horizontal_distance(m->marioObj, obj);
            f32 ydist = obj->oPosY - m->pos[1];
            if (sOmmRadarTimer > OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_TOTAL && abs_f(ydist) > max_f(1000.f, hdist * 2.f)) {
                sOmmRadarTimer = 0;
            }
            for (s32 i = 0; i != 3; ++i) {
                u8 alpha = 0xFF * (1.f - abs_f(1.f - relerp_0_1_f(sOmmRadarTimer, i * OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_NEXT, i * OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_NEXT + OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_DURATION, 0.f, 2.f)));
                s16 s = 6 + 2 * i;
                s16 x = OMM_RENDER_OBJECT_RADAR_X - (s / 2);
                s16 y = OMM_RENDER_OBJECT_RADAR_Y - (s / 2) + sign_s(ydist) * (8 + i * 3 + (i == 2));
                omm_render_glyph(x, y, s, s, targetR, targetG, targetB, alpha, (ydist < 0 ? OMM_TEXTURE_HUD_ARROW_DOWN : OMM_TEXTURE_HUD_ARROW_UP), false);
            }
            sOmmRadarTimer++;

            // Display radar arrow
            s16 angle = atan2s(
                obj->oPosZ - m->pos[2],
                obj->oPosX - m->pos[0]
            ) - atan2s(
                m->pos[2] - gCamera->pos[2],
                m->pos[0] - gCamera->pos[0]
            );
            omm_render_create_dl_ortho_matrix();
            Vtx *vtx = omm_alloc_vtx(4);
            interp_data_update(sOmmRadarArrow, true, gDisplayListHead, NULL, vtx, 0, 0, 0, angle, 0, 0);
            if (sOmmRadarArrow->a0 * sOmmRadarArrow->a1 < 0) { // Check the -32768/+32767 over/underflow
                f32 dl = 0x8000 - abs_f(sOmmRadarArrow->a0);
                f32 dr = 0x8000 - abs_f(sOmmRadarArrow->a1);
                sOmmRadarArrow->a0 -= (dl + dr < 0x8000) * sign_f(sOmmRadarArrow->a0) * 0x10000;
            }
            gDisplayListHead = omm_render_hud_objects_radar_arrow(gDisplayListHead, sOmmRadarArrow->vtx, sOmmRadarArrow->a0);
            return;
        }
    }
    interp_data_reset(sOmmRadarArrow);
    gOmmGlobals->hideHudRadar = false;
}

//
// Monty mole hole indicator
//

static Gfx *omm_render_hud_monty_mole_hole_indicator_arrow(Gfx *pos, Vtx *vtx, f32 x, f32 y, f32 radius, f32 angle, f32 scale) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);

    // Vertices
    vtx[0] = (Vtx) {{{ 0, 0, 0 }, 0, { 0x0000, 0x1000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[1] = (Vtx) {{{ 0, 0, 0 }, 0, { 0x1000, 0x1000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[2] = (Vtx) {{{ 0, 0, 0 }, 0, { 0x1000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[3] = (Vtx) {{{ 0, 0, 0 }, 0, { 0x0000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    for (s32 i = 0; i != 4; ++i) {
        s16 a = angle + ((i * 0x4000) - 0x6000);
        vtx[i].v.ob[0] = x +         (radius * coss(angle + 0x4000) + OMM_RENDER_OBJECT_RADAR_ARROW_SIZE * 1.2f * OMM_INVSQRT2 * coss(a));
        vtx[i].v.ob[1] = y + scale * (radius * sins(angle + 0x4000) + OMM_RENDER_OBJECT_RADAR_ARROW_SIZE * 1.2f * OMM_INVSQRT2 * sins(a));
    }

    // Display list
    gSPClearGeometryMode(pos++, G_CULL_BOTH);
    gDPSetRenderMode(pos++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetCombineLERP(pos++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock(pos++, OMM_TEXTURE_HUD_ARROW, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gSPVertex(pos++, vtx, 4, 0);
    gSP2Triangles(pos++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPSetRenderMode(pos++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineLERP(pos++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    return pos;
}

void omm_render_hud_monty_mole_hole_indicator() {
    struct Object *currHole = gOmmCapture->oMontyMoleCurrentHole;
    struct Object *nextHole = gOmmCapture->oMontyMoleTargetHole;
    Vec3f src3d = { currHole->oPosX, currHole->oPosY, currHole->oPosZ };

    // Camera normal
    Vec3f camN;
    vec3f_dif(camN, gLakituState.focus, gLakituState.pos);
    vec3f_normalize(camN);

    // Camera horizontal axis
    s16 camYaw = atan2s(camN[2], camN[0]) - 0x4000;
    Vec3f camH = { sins(camYaw), 0.f, coss(camYaw) };

    // Camera vertical axis
    Vec3f camV;
    vec3f_cross(camV, camN, camH);
    vec3f_normalize(camV);

    // Hole position to camera plane
    Vec2f pos2d;
    f32 dist;
    vec3f_to_2d_plane(pos2d, &dist, src3d, gLakituState.pos, camN, camH, camV);
    if (dist > 0.f) {
        f32 x = (SCREEN_WIDTH  / 2) * (1.f + (1.75f * (pos2d[0] / dist)));
        f32 y = (SCREEN_HEIGHT / 2) * (1.f - (2.25f * (pos2d[1] / dist)));

        // Angle, radius and scale
        s16 angle = atan2s(
            nextHole->oPosZ - currHole->oPosZ,
            nextHole->oPosX - currHole->oPosX
        ) - atan2s(
            currHole->oPosZ - gCamera->pos[2],
            currHole->oPosX - gCamera->pos[0]
        );
        f32 radius = (120 * SCREEN_HEIGHT) / dist;
        f32 scale = relerp_0_1_f(abs_f(coss(angle)), 0, 1, 1, (1 + abs_f(camN[1])) / 2);

        // Render
        omm_render_create_dl_ortho_matrix();
        Vtx *vtx = omm_alloc_vtx(4);
        interp_data_update(sOmmMontyMoleArrow, true, gDisplayListHead, NULL, vtx, x, y, radius, angle, scale, 0);
        if (sOmmMontyMoleArrow->a0 * sOmmMontyMoleArrow->a1 < 0) { // Check the -32768/+32767 over/underflow
            f32 dl = 0x8000 - abs_f(sOmmMontyMoleArrow->a0);
            f32 dr = 0x8000 - abs_f(sOmmMontyMoleArrow->a1);
            sOmmMontyMoleArrow->a0 -= (dl + dr < 0x8000) * sign_f(sOmmMontyMoleArrow->a0) * 0x10000;
        }
        gDisplayListHead = omm_render_hud_monty_mole_hole_indicator_arrow(gDisplayListHead, sOmmMontyMoleArrow->vtx, sOmmMontyMoleArrow->x0, sOmmMontyMoleArrow->y0, sOmmMontyMoleArrow->z0, sOmmMontyMoleArrow->a0, sOmmMontyMoleArrow->s0);
        return;
    }
    interp_data_reset(sOmmMontyMoleArrow);
}

//
// HUD
//

void omm_render_hud() {
    struct MarioState *m = gMarioState;

    // Cake end screen
    // Render the custom cake end screen on top of
    // the regular cake end screen, if necessary
    if (omm_is_ending_cake_screen()) {
        omm_render_cake_ending_screen();
        return;
    }

    // Version number
    // Only on title screen/main menu
    if (omm_is_main_menu() && !omm_palette_editor_is_open() && !omm_palette_editor_is_closing()) {
        const u8 *textVersion = omm_text_convert("OMM V" OMM_VERSION, false);
        s32 textVersionWidth = omm_render_get_string_width(textVersion);
        omm_render_string_left_align(GFX_DIMENSIONS_FROM_RIGHT_EDGE(textVersionWidth + 2), 3, 0xFF, 0xFF, 0xFF, 0xFF, textVersion, true);
        return;
    }

    // Monty mole capture hole indicator
    if (omm_mario_is_capture(m) && omm_capture_get_type(gOmmCapture) == OMM_CAPTURE_MONTY_MOLE && gOmmCapture->oMontyMoleCurrentHole && gOmmCapture->oMontyMoleTargetHole) {
        omm_render_hud_monty_mole_hole_indicator();
    } else {
        interp_data_reset(sOmmMontyMoleArrow);
    }

    // HUD
    if (HUD_DISPLAY_HUD && !gOmmGlobals->cameraSnapshotMode) {
        gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
        omm_render_create_dl_ortho_matrix();
        omm_render_hud_cannon_reticle();
        omm_render_effect_dark_mode();
        omm_render_effect_freeze();
        omm_render_effect_vibe();
        omm_render_effect_you_got_a_star();
        omm_render_hud_breath_meter(m);
        omm_render_hud_power_meter(m);
        omm_render_hud_objects_radar(m);
        omm_render_hud_values(m);
        omm_render_hud_camera(m);
        omm_render_hud_timer();
        time_trials_render_hud_timer();
    }
}

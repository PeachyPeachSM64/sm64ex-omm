#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Data
//

// struct
static Scroll sCastleScrollV = {0};
static Scroll sCastleScrollH = {0};
static Scroll sCourseScrollV = {0};
static s32 sPauseType = 0;
static s32 sPauseAlpha = 0;

static s32 OMM_PAUSE_LEVEL_LIST[] = {
    LEVEL_BOB, LEVEL_WF, LEVEL_JRB, LEVEL_CCM, LEVEL_BBH,
    LEVEL_HMC, LEVEL_LLL, LEVEL_SSL, LEVEL_DDD, LEVEL_SL,
    LEVEL_WDW, LEVEL_TTM, LEVEL_THI, LEVEL_TTC, LEVEL_RR,
    LEVEL_BITDW, LEVEL_BITFS, LEVEL_BITS,
    LEVEL_TOTWC, LEVEL_COTMC, LEVEL_VCUTM,
    LEVEL_PSS, LEVEL_SA, LEVEL_WMOTR,
    LEVEL_ENDING, LEVEL_CASTLE
};

#define OMM_PAUSE_NUM_LEVELS ((s32) array_length(OMM_PAUSE_LEVEL_LIST))

//
// Sparkly Stars timer
//

static void omm_render_pause_sparkly_timer(s16 x, s16 y, s16 w, u8 alpha, s32 sparklyMode, s32 timer, bool blink) {
    static const char *OMM_PAUSE_SPARKLY_TIMER_GLYPHS[] = {
        OMM_TEXTURE_HUD_0,
        OMM_TEXTURE_HUD_1,
        OMM_TEXTURE_HUD_2,
        OMM_TEXTURE_HUD_3,
        OMM_TEXTURE_HUD_4,
        OMM_TEXTURE_HUD_5,
        OMM_TEXTURE_HUD_6,
        OMM_TEXTURE_HUD_7,
        OMM_TEXTURE_HUD_8,
        OMM_TEXTURE_HUD_9,
        OMM_TEXTURE_HUD_DOTS,
        OMM_TEXTURE_MISC_WHITE,
        OMM_TEXTURE_STAR_FULL_17,
        OMM_TEXTURE_STAR_FULL_18,
        OMM_TEXTURE_STAR_FULL_19,
    };

    s32 glyphs[] = {
        11 + sparklyMode,
        -1,
        ((timer / 1080000) % 10),
        ((timer /  108000) % 10),
        10,
        ((timer /   18000) %  6),
        ((timer /    1800) % 10),
        10,
        ((timer /     300) %  6),
        ((timer /      30) % 10),
    };
    for (s32 i = 0; i != 10; ++i) {
        if (glyphs[i] != -1) {
            u8 a = alpha / ((glyphs[i] == 10 && blink && (timer % 30) >= 15) ? 3 : 1);
            omm_render_glyph(
                x + (i - 2) * OMM_RENDER_SPARKLY_TIMER_OFFSET_X(w), y, w, w,
                0xFF, 0xFF, 0xFF, a, OMM_PAUSE_SPARKLY_TIMER_GLYPHS[glyphs[i]], false
            );
        }
    }
}

//
// Collectibles
//

OMM_INLINE void omm_render_pause_collectible(s16 x, s16 y, const void *texture, bool collected) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(OMM_RENDER_GLYPH_SIZE_DEFAULT);
    omm_render_glyph_hud(x, y, 0xFF * collected, 0xFF * collected, 0xFF * collected, sPauseAlpha / (2 - collected), texture, false);
}

static void omm_render_pause_collectibles() {
#if OMM_GAME_IS_R96X
    OMM_RENDER_DEFINE_GLYPH_SIZE(OMM_RENDER_GLYPH_SIZE_DEFAULT);

    // Luigi keys
    if ((gHudDisplay.flags & HUD_DISPLAY_FLAG_KEYS) && gHudDisplay.keys > 0 && gHudDisplay.keys < NUM_KEYS) {
        for (s32 i = 0; i != NUM_KEYS; ++i) {
            omm_render_pause_collectible(
                OMM_RENDER_COLLECTIBLE_X + OMM_RENDER_NUMBER_OFFSET_X * i,
                OMM_RENDER_COLLECTIBLE_Y,
                "textures/segment2/boo_key.rgba16",
                omm_save_file_get_taken_luigi_key(gCurrSaveFileNum - 1, OMM_GAME_MODE, i)
            );
        }
        gOmmGlobals->hideHudRadar = true;
    }

    // Wario coins
    if ((gHudDisplay.flags & HUD_DISPLAY_FLAG_KEYS) && gHudDisplay.wario_coins > 0 && gHudDisplay.wario_coins < NUM_WARIO_COINS) {
        for (s32 i = 0; i != NUM_WARIO_COINS; ++i) {
            omm_render_pause_collectible(
                OMM_RENDER_COLLECTIBLE_X + OMM_RENDER_STAR_OFFSET_X * i,
                OMM_RENDER_COLLECTIBLE_Y + OMM_RENDER_OFFSET_Y,
                "textures/segment2/wario_coin.rgba16",
                omm_save_file_get_taken_wario_coin(gCurrSaveFileNum - 1, OMM_GAME_MODE, i)
            );
        }
        gOmmGlobals->hideHudRadar = true;
    }
#endif
}

//
// Init
//

void omm_render_pause_init() {
    sPauseAlpha = 0;
    level_set_transition(-1, NULL);
    play_sound(SOUND_MENU_PAUSE | 0xFF00, gGlobalSoundArgs);
    if (gCurrCourseNum != COURSE_NONE) {
        sPauseType = 1;
        sCourseScrollV.idx = 0;
    } else {
        sPauseType = 2;
        if (omm_level_get_course(gLastCompletedLevelNum) != COURSE_NONE) {
            sCastleScrollV.idx = -1;
        }
    }
}

static void omm_render_pause_score_board(bool isCoinScore) {
    omm_render_texrect(GFX_DIMENSIONS_FROM_LEFT_EDGE(-1), 0, GFX_DIMENSIONS_FROM_RIGHT_EDGE(-1) - GFX_DIMENSIONS_FROM_LEFT_EDGE(-1), SCREEN_HEIGHT, 32, 32, 0x00, 0x00, 0x00, sPauseAlpha * 0.8f, OMM_TEXTURE_MISC_WHITE, false);
    extern void omm_render_score_board(s32 fileIndex, s32 modeIndex, f32 scale, u8 a, bool isCoinScore, bool highlightCurrentCourse);
    omm_render_score_board(gCurrSaveFileNum - 1, OMM_GAME_MODE, 1.f, sPauseAlpha, isCoinScore, true);
}

//
// Pause (Course)
//

static s16 omm_render_pause_course_name(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum);
    s32 courseNum = omm_level_get_course(levelNum);
    u8 *textCourse = omm_text_convert(OMM_TEXT_COURSE___, false);
    s32 textCourseLength = omm_text_length(textCourse);
    if (COURSE_IS_MAIN_COURSE(courseNum)) {
        textCourse[textCourseLength - 3] = DIALOG_CHAR_SPACE;
        if (courseNum >= 10) {
            textCourse[textCourseLength - 2] = courseNum / 10;
            textCourse[textCourseLength - 1] = courseNum % 10;
            textCourse[textCourseLength - 0] = 0xFF;
        } else {
            textCourse[textCourseLength - 2] = courseNum;
            textCourse[textCourseLength - 1] = 0xFF;
        }
    } else {
        textCourse[textCourseLength - 3] = 0xFF;
    }
    ustr_t courseName;
    omm_render_string_right_align(OMM_RENDER_PAUSE_COURSE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textCourse, true);
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_level_get_course_name(courseName, levelNum, OMM_GAME_MODE, false, false), false);
    y -= OMM_RENDER_PAUSE_COURSE_OFFSET_Y;
    return y;
}

static s16 omm_render_pause_course_act(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum);
    if (OMM_STARS_CLASSIC && COURSE_IS_MAIN_COURSE(omm_level_get_course(levelNum))) {
        u8 *textAct = omm_text_convert(OMM_TEXT_ACT__, false);
        s32 length = omm_text_length(textAct);
        textAct[length - 2] = DIALOG_CHAR_SPACE;
        textAct[length - 1] = gCurrActNum;
        textAct[length - 0] = 0xFF;
        ustr_t actName;
        omm_render_string_right_align(OMM_RENDER_PAUSE_COURSE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textAct, true);
        omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_level_get_act_name(actName, levelNum, gCurrActNum, OMM_GAME_MODE, false, false), false);
        y -= OMM_RENDER_PAUSE_COURSE_OFFSET_Y;
    }
    return y;
}

static s16 omm_render_pause_course_stars(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum);
    if (omm_stars_get_level_flags(levelNum, OMM_GAME_MODE) != 0) {
        OMM_RENDER_DEFINE_GLYPH_SIZE(OMM_RENDER_GLYPH_SIZE_DEFAULT);
        u8 *textStars = omm_text_convert(OMM_TEXT_MY_STARS, false);
        omm_render_string_right_align(OMM_RENDER_PAUSE_COURSE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textStars, true);
        omm_render_hud_stars(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, y - ((OMM_RENDER_GLYPH_SIZE - 8) / 2), OMM_RENDER_GLYPH_SIZE, sPauseAlpha, levelNum);
        y -= OMM_RENDER_PAUSE_COURSE_OFFSET_Y;
    }
    return y;
}

static s16 omm_render_pause_course_coins(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum);
    if (omm_stars_get_level_flags(levelNum, OMM_GAME_MODE) != 0) {
        OMM_RENDER_DEFINE_GLYPH_SIZE(OMM_RENDER_GLYPH_SIZE_DEFAULT);
        u8 *textScore = omm_text_convert(OMM_TEXT_MY_SCORE, false);
        omm_render_string_right_align(OMM_RENDER_PAUSE_COURSE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textScore, true);
        omm_render_hud_coins(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, y - ((OMM_RENDER_GLYPH_SIZE - 8) / 2), OMM_RENDER_GLYPH_SIZE, sPauseAlpha, omm_save_file_get_course_coin_score(gCurrSaveFileNum - 1, OMM_GAME_MODE, omm_level_get_course(levelNum) - 1));
        y -= OMM_RENDER_PAUSE_COURSE_OFFSET_Y;
    }
    return y;
}

static void omm_render_pause_course_red_coins() {
    if (gOmmArea->redCoins > 0 && obj_get_red_coin_star()) {
        OMM_RENDER_DEFINE_GLYPH_SIZE(OMM_RENDER_GLYPH_SIZE_DEFAULT);
        s32 x = OMM_RENDER_RED_COINS_X;
        omm_render_texrect(
            x, OMM_RENDER_RED_COINS_Y, OMM_RENDER_GLYPH_SIZE, OMM_RENDER_GLYPH_SIZE,
            32, 32, 0xFF, 0x00, 0x00, 0xFF,
            (const void *) (array_of(const char *) {
                OMM_ASSET_COIN_0,
                OMM_ASSET_COIN_1,
                OMM_ASSET_COIN_2,
                OMM_ASSET_COIN_3,
            })[(gGlobalTimer >> 1) & 3],
            false
        );
        x += 2 * OMM_RENDER_NUMBER_OFFSET_X;
        omm_render_number_hud(x, OMM_RENDER_RED_COINS_Y, sPauseAlpha, min_s(gRedCoinsCollected, gOmmArea->redCoins), 2, true, false);
        x += OMM_RENDER_NUMBER_OFFSET_X + OMM_RENDER_GLYPH_SIZE;
        omm_render_glyph_hud(x, OMM_RENDER_RED_COINS_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, OMM_TEXTURE_HUD_SLASH, false);
        x += OMM_RENDER_GLYPH_SIZE;
        omm_render_number_hud(x, OMM_RENDER_RED_COINS_Y, sPauseAlpha, gOmmArea->redCoins, 2, true, false);
        gOmmGlobals->hideHudCamera = true;
    }
}

static void omm_render_pause_course_collectibles() {
    omm_render_pause_collectibles();
}

static void omm_render_pause_course_sparkly_timer() {
    if (OMM_SPARKLY_MODE_IS_ENABLED && !time_trials_is_enabled()) {
        s32 sparklyMode = gOmmSparklyMode;
        omm_render_pause_sparkly_timer(
            OMM_RENDER_SPARKLY_TIMER_X(OMM_RENDER_SPARKLY_TIMER_GLYPH_SIZE),
            OMM_RENDER_SPARKLY_TIMER_Y,
            OMM_RENDER_SPARKLY_TIMER_GLYPH_SIZE,
            sPauseAlpha,
            sparklyMode,
            omm_sparkly_get_timer(sparklyMode),
           !omm_sparkly_is_grand_star_collected(sparklyMode)
        );
    }
}

static void omm_render_pause_course_options() {
    omm_render_update_scroll(&sCourseScrollV, 4, -gPlayer1Controller->stickY);

    // Options
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, OMM_RENDER_PAUSE_COURSE_OPTIONS_Y - 0 * OMM_RENDER_PAUSE_COURSE_OFFSET_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_text_convert(OMM_TEXT_CONTINUE, false), false);
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, OMM_RENDER_PAUSE_COURSE_OPTIONS_Y - 1 * OMM_RENDER_PAUSE_COURSE_OFFSET_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_text_convert(OMM_TEXT_RESTART_LEVEL, false), false);
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, OMM_RENDER_PAUSE_COURSE_OPTIONS_Y - 2 * OMM_RENDER_PAUSE_COURSE_OFFSET_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_text_convert(OMM_TEXT_EXIT_LEVEL, false), false);
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, OMM_RENDER_PAUSE_COURSE_OPTIONS_Y - 3 * OMM_RENDER_PAUSE_COURSE_OFFSET_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_text_convert(OMM_TEXT_RETURN_TO_CASTLE, false), false);

    // Cursor
    static const Vtx sWhiteTriVtx[] = {
        { { {  0,  0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { { -6, -6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { { -6, +6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    };
    create_dl_translation_matrix(MENU_MTX_PUSH, OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X - 6, OMM_RENDER_PAUSE_COURSE_OPTIONS_Y - sCourseScrollV.idx * OMM_RENDER_PAUSE_COURSE_OFFSET_Y + 4, 0);
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, sPauseAlpha);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, sWhiteTriVtx, 3, 0);
    gSP1Triangle(gDisplayListHead++, 2, 1, 0, 0);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

static void omm_render_pause_course_select_option() {
    if (gPlayer1Controller->buttonPressed & (A_BUTTON | START_BUTTON)) {
        level_set_transition(0, NULL);
        play_sound(SOUND_MENU_PAUSE_2, gGlobalSoundArgs);
        sPauseType = 0;
        gMenuMode = -1;
        switch (sCourseScrollV.idx) {
            case 0: { // Continue
                raise_background_noise(1);
                gCameraMovementFlags &= ~CAM_MOVE_PAUSE_SCREEN;
                set_play_mode(0);
            } break;

            case 1: { // Restart Level
                omm_restart_level();
            } break;

            case 2: { // Exit Level
                omm_exit_level(gCurrLevelNum, gCurrAreaIndex, false);
            } break;

            case 3: { // Return to Castle (same as Exit Course)
                omm_health_set_max(gMarioState);
                omm_return_to_castle(true, true);
            } break;
        }
    }
}

void omm_render_pause_course() {
    if (gPlayer1Controller->buttonDown & L_TRIG) {
        omm_render_pause_score_board(gPlayer1Controller->buttonDown & A_BUTTON);
    } else {
        s16 y = OMM_RENDER_PAUSE_COURSE_INFO_Y - OMM_RENDER_PAUSE_COURSE_OFFSET_Y * !(OMM_STARS_CLASSIC && COURSE_IS_MAIN_COURSE(omm_level_get_course(OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum))));
        y = omm_render_pause_course_name(y);
        y = omm_render_pause_course_act(y);
        y = omm_render_pause_course_stars(y);
        y = omm_render_pause_course_coins(y);
        omm_render_pause_course_options();
        omm_render_pause_course_red_coins();
        omm_render_pause_course_collectibles();
        omm_render_pause_course_sparkly_timer();
        omm_render_pause_course_select_option();
    }
}

//
// Pause (Castle)
//

static void omm_render_pause_castle_update() {

    // Try to place the cursor on the last visited course
    if (sCastleScrollV.idx == -1) {
        for (s32 i = 0; i != OMM_PAUSE_NUM_LEVELS; ++i) {
            if (gLastCompletedLevelNum == OMM_PAUSE_LEVEL_LIST[i]) {
                sCastleScrollV.idx = i;
                gLastCompletedLevelNum = LEVEL_CASTLE;
                break;
            }
        }
    }

    // Update the cursor, skip unknown courses (no star collected)
    s32 count = OMM_PAUSE_NUM_LEVELS + 1 + OMM_SPARKLY_MODE_IS_ENABLED;
    omm_render_update_scroll(&sCastleScrollV, count, -gPlayer1Controller->stickY);
    for (s32 dir = sign_s(-gPlayer1Controller->stickY);; sCastleScrollV.idx = (sCastleScrollV.idx + count + dir) % count) {
        if (sCastleScrollV.idx >= OMM_PAUSE_NUM_LEVELS - 1 || omm_save_file_get_star_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, omm_level_get_course(OMM_PAUSE_LEVEL_LIST[sCastleScrollV.idx]) - 1) != 0) {
            break;
        }
    }
}

static void omm_render_pause_castle_pause() {

    // Pause
    u8 *textPause = omm_text_convert(OMM_TEXT_PAUSE, false);
    omm_render_string_hud_centered(SCREEN_HEIGHT / 2, 0xFF, 0xFF, 0xFF, sPauseAlpha, textPause, false);

    // Black box
    static const Vtx sBlackBoxVtx[] = {
        { { { -OMM_RENDER_PAUSE_CASTLE_BOX_W / 2, -OMM_RENDER_PAUSE_CASTLE_BOX_H / 2, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { { -OMM_RENDER_PAUSE_CASTLE_BOX_W / 2, +OMM_RENDER_PAUSE_CASTLE_BOX_H / 2, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { { +OMM_RENDER_PAUSE_CASTLE_BOX_W / 2, -OMM_RENDER_PAUSE_CASTLE_BOX_H / 2, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { { +OMM_RENDER_PAUSE_CASTLE_BOX_W / 2, +OMM_RENDER_PAUSE_CASTLE_BOX_H / 2, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    };
    create_dl_translation_matrix(MENU_MTX_PUSH, SCREEN_WIDTH / 2, OMM_RENDER_PAUSE_CASTLE_BOX_Y, 0);
    gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, sPauseAlpha / 2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, sBlackBoxVtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 2, 1, 0, 0, 1, 2, 3, 0);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // Vertical arrows
    static const Vtx sWhiteTriVtx[] = {
        { { {  0, -8, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { { -8,  0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { { +8,  0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { {  0, +8, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    };
    create_dl_translation_matrix(MENU_MTX_PUSH, SCREEN_WIDTH / 2, OMM_RENDER_PAUSE_CASTLE_BOX_TRIANGLE_DOWN_Y, 0);
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, sPauseAlpha);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, sWhiteTriVtx + 0, 3, 0);
    gSP1Triangle(gDisplayListHead++, 2, 1, 0, 0);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    create_dl_translation_matrix(MENU_MTX_PUSH, SCREEN_WIDTH / 2, OMM_RENDER_PAUSE_CASTLE_BOX_TRIANGLE_UP_Y, 0);
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, sPauseAlpha);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, sWhiteTriVtx + 1, 3, 0);
    gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

static void omm_render_pause_castle_course() {
    s32 levelNum = OMM_PAUSE_LEVEL_LIST[sCastleScrollV.idx];
    s32 courseIndex = omm_level_get_course(levelNum) - 1;

    // Course name
    ustr_t textCourseName;
    if (courseIndex == -1) {
        omm_text_copy(textCourseName, sizeof(ustr_t), omm_text_convert(OMM_TEXT_LEVEL_SECRET_STARS, false));
    } else if (omm_save_file_get_star_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, courseIndex)) {
        omm_level_get_course_name(textCourseName, levelNum, OMM_GAME_MODE, false, true);
    } else {
        omm_text_copy(textCourseName, sizeof(ustr_t), omm_text_convert(OMM_TEXT_UNKNOWN, false));
    }
    omm_render_string_centered(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_1_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textCourseName, false);

    // Course stars
    if (courseIndex == -1) {
        omm_render_counter(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, 0xFA, omm_save_file_get_course_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE, -1));
    } else {
        s16 x = (SCREEN_WIDTH - 92) / 2;
        u8 starLevelFlags = omm_stars_get_level_flags(levelNum, OMM_GAME_MODE);
        u8 starSaveFlags = omm_save_file_get_star_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, courseIndex);
        u8 starGlyph[] = { DIALOG_CHAR_STAR_FILLED, 0xFF };
        for (s32 starIndex = 0; starIndex != OMM_NUM_STARS_MAX_PER_COURSE; ++starIndex) {
            if (starLevelFlags & (1 << starIndex)) {
#if OMM_GAME_IS_SMSR
                const BehaviorScript *bhv = NULL;
                if (omm_stars_get_star_data(levelNum, 1, starIndex, &bhv, NULL) && bhv == bhvCustomSMSRStarReplica &&
                    omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE) < 121) {
                    continue;
                }
#endif
                if (starSaveFlags & (1 << starIndex)) {
                    omm_render_string_left_align(x, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, starGlyph, false);
                } else {
                    omm_render_string_left_align(x, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y, 0x80, 0x80, 0x80, sPauseAlpha, starGlyph, false);
                }
                x += 14;
            }
        }
    }

    // Course score
    if (courseIndex != -1) {
        omm_render_counter(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, 0xF9, omm_save_file_get_course_coin_score(gCurrSaveFileNum - 1, OMM_GAME_MODE, courseIndex));
    }
}

static void omm_render_pause_castle_caps_keys_captures() {
    const u32 flagsAndColors[][4] = {
        { SAVE_FLAG_HAVE_WING_CAP, 0xFF, 0x00, 0x00 },
        { SAVE_FLAG_HAVE_METAL_CAP, 0x00, 0xC0, 0x00 },
        { SAVE_FLAG_HAVE_VANISH_CAP, 0x00, 0x80, 0xFF },
        { SAVE_FLAG_UNLOCKED_BASEMENT_DOOR | SAVE_FLAG_HAVE_KEY_1, 0xFF, 0xFF, 0x00 },
        { SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR | SAVE_FLAG_HAVE_KEY_2, 0xFF, 0xFF, 0x00 },
    };
    u32 flags = omm_save_file_get_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE);
    u8 starGlyph[] = { DIALOG_CHAR_STAR_FILLED, 0xFF };

    // Caps
    const u8 *textCaps = omm_text_convert(OMM_TEXT_PAUSE_CAPS, false);
    omm_render_string_right_align(SCREEN_WIDTH / 2 - 5, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_1_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textCaps, false);
    for (s32 i = 0; i != 3; ++i) {
        if (flags & flagsAndColors[i][0]) {
            omm_render_string_left_align(SCREEN_WIDTH / 2 + 5 + i * 12, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_1_Y, flagsAndColors[i][1], flagsAndColors[i][2], flagsAndColors[i][3], sPauseAlpha, starGlyph, false);
        } else {
            omm_render_string_left_align(SCREEN_WIDTH / 2 + 5 + i * 12, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_1_Y, 0x80, 0x80, 0x80, sPauseAlpha, starGlyph, false);
        }
    }

    // Bowser keys
    const u8 *textKeys = omm_text_convert(OMM_TEXT_PAUSE_KEYS, false);
    omm_render_string_right_align(SCREEN_WIDTH / 2 - 5, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textKeys, false);
    for (s32 i = 3; i != 5; ++i) {
        if (flags & flagsAndColors[i][0]) {
            omm_render_string_left_align(SCREEN_WIDTH / 2 + 5 + (i - 3) * 12, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y, flagsAndColors[i][1], flagsAndColors[i][2], flagsAndColors[i][3], sPauseAlpha, starGlyph, false);
        } else {
            omm_render_string_left_align(SCREEN_WIDTH / 2 + 5 + (i - 3) * 12, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y, 0x80, 0x80, 0x80, sPauseAlpha, starGlyph, false);
        }
    }

    // Captures
    str_t strCaptureCount;
    str_fmt(strCaptureCount, sizeof(strCaptureCount), "%d / %d", omm_save_file_get_capture_count(gCurrSaveFileNum - 1, OMM_GAME_MODE), omm_level_get_all_available_captures_count(OMM_GAME_MODE));
    const u8 *textCaptures = omm_text_convert(OMM_TEXT_PAUSE_CAPTURES, false);
    const u8 *textCaptureCount = omm_text_convert(strCaptureCount, false);
    omm_render_string_right_align(SCREEN_WIDTH / 2 - 5, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textCaptures, false);
    omm_render_string_left_align(SCREEN_WIDTH / 2 + 5, OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textCaptureCount, false);
}

static void omm_render_pause_castle_sparkly_stars_and_timer() {
    s32 sparklyMode = gOmmSparklyMode;
    s32 count = omm_sparkly_get_num_stars(sparklyMode);
    u8 textR = OMM_SPARKLY_HUD_COLOR_R[sparklyMode];
    u8 textG = OMM_SPARKLY_HUD_COLOR_G[sparklyMode];
    u8 textB = OMM_SPARKLY_HUD_COLOR_B[sparklyMode];
    omm_render_update_scroll(&sCastleScrollH, count, gPlayer1Controller->stickX);

    // Horizontal arrows
    static const Vtx sWhiteTriVtx[] = {
        { { { -8,  0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { {  0, -8, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { {  0, +8, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
        { { { +8,  0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    };
    create_dl_translation_matrix(MENU_MTX_PUSH, OMM_RENDER_PAUSE_CASTLE_BOX_TRIANGLE_LEFT_X, OMM_RENDER_PAUSE_CASTLE_BOX_Y, 0);
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, sPauseAlpha);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, sWhiteTriVtx + 0, 3, 0);
    gSP1Triangle(gDisplayListHead++, 2, 1, 0, 0);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    create_dl_translation_matrix(MENU_MTX_PUSH, OMM_RENDER_PAUSE_CASTLE_BOX_TRIANGLE_RIGHT_X, OMM_RENDER_PAUSE_CASTLE_BOX_Y, 0);
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, sPauseAlpha);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, sWhiteTriVtx + 1, 3, 0);
    gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // Title
    u8 *textSparklyStars = omm_text_convert(OMM_SPARKLY_TEXT_STARS[sparklyMode], false);
    omm_render_string_centered(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_1_Y, textR, textG, textB, sPauseAlpha, textSparklyStars, false);

    // Star count
    omm_render_counter(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y, textR, textG, textB, sPauseAlpha, 0xFA, omm_sparkly_get_collected_count(sparklyMode));

    // Level display
    for (;; sCastleScrollH.idx = (sCastleScrollH.idx + count + sign_s(sCastleScrollH.inc)) % count) {
        ustr_t levelName;
        if (omm_sparkly_get_level_name(levelName, sparklyMode, sCastleScrollH.idx)) {
            if ((sCastleScrollH.idx == count - 1 && omm_sparkly_is_grand_star_collected(sparklyMode)) || omm_sparkly_is_star_collected(sparklyMode, sCastleScrollH.idx)) {
                omm_render_string_centered(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y, textR, textG, textB, sPauseAlpha, levelName, false);
            } else if (sCastleScrollH.idx == count - 1 && !omm_sparkly_is_completed(sparklyMode)) {
                omm_render_string_centered(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y, textR / 2, textG / 2, textB / 2, sPauseAlpha, omm_text_convert(OMM_TEXT_UNKNOWN, false), false);
            } else {
                omm_render_string_centered(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y, textR / 2, textG / 2, textB / 2, sPauseAlpha, levelName, false);
            }
            break;
        }
    }

    // Sparkly Stars timer
    omm_render_pause_sparkly_timer(
        OMM_RENDER_SPARKLY_TIMER_X(OMM_RENDER_SPARKLY_TIMER_GLYPH_SIZE),
        OMM_RENDER_SPARKLY_TIMER_Y,
        OMM_RENDER_SPARKLY_TIMER_GLYPH_SIZE,
        sPauseAlpha,
        sparklyMode,
        omm_sparkly_get_timer(sparklyMode),
        !omm_sparkly_is_grand_star_collected(sparklyMode)
    );
}

static void omm_render_pause_castle_sm74_edition() {
#if OMM_GAME_IS_SM74
    const u8 *textSm74 = omm_text_convert(OMM_TEXT_SM74_SUPER_MARIO_74, false);
    omm_render_string_centered(SCREEN_HEIGHT - 40, 0xFF, 0xFF, 0xFF, sPauseAlpha, textSm74, true);
    switch (OMM_GAME_MODE) {
        case OMM_SM74_MODE_NORMAL: {
            const u8 *textMode = omm_text_convert(OMM_TEXT_SM74_NORMAL_EDITION, false);
            omm_render_string_centered(SCREEN_HEIGHT - 54, 0x40, 0xFF, 0x40, sPauseAlpha, textMode, true);
        } break;
        case OMM_SM74_MODE_EXTREME: {
            const u8 *textMode = omm_text_convert(OMM_TEXT_SM74_EXTREME_EDITION, false);
            omm_render_string_centered(SCREEN_HEIGHT - 54, 0xFF, 0x40, 0x20, sPauseAlpha, textMode, true);
        } break;
    }
#endif
}

static void omm_render_pause_castle_collectibles() {
    omm_render_pause_collectibles();
}

static void omm_render_pause_castle_close() {
    if (gPlayer1Controller->buttonPressed & (A_BUTTON | START_BUTTON)) {
        level_set_transition(0, NULL);
        play_sound(SOUND_MENU_PAUSE_2, gGlobalSoundArgs);
        sPauseType = 0;
        gMenuMode = -1;
        raise_background_noise(1);
        gCameraMovementFlags &= ~CAM_MOVE_PAUSE_SCREEN;
        set_play_mode(0);
    }
}

void omm_render_pause_castle() {
    omm_render_create_dl_ortho_matrix();
    if (gPlayer1Controller->buttonDown & L_TRIG) {
        omm_render_pause_score_board(gPlayer1Controller->buttonDown & A_BUTTON);
    } else if (time_trials_render_pause_castle(sPauseAlpha)) {
        omm_render_pause_castle_collectibles();
        omm_render_pause_castle_close();
    } else {
        omm_render_pause_castle_update();
        omm_render_pause_castle_pause();
        omm_render_pause_castle_sm74_edition();
        switch (sCastleScrollV.idx) {
            case OMM_PAUSE_NUM_LEVELS + 0: omm_render_pause_castle_caps_keys_captures(); break;
            case OMM_PAUSE_NUM_LEVELS + 1: omm_render_pause_castle_sparkly_stars_and_timer(); break;
            default:                       omm_render_pause_castle_course(); break;
        }
        omm_render_pause_castle_collectibles();
        omm_render_pause_castle_close();
    }
}

//
// Pause (render)
//

s32 omm_render_pause() {
    if (gOmmGlobals->cameraSnapshotMode) {
        omm_camera_snapshot_mode_update();
        return 0;
    }
    omm_render_create_dl_ortho_matrix();
    omm_render_shade_screen(0x7F);
    if (optmenu_open) {
        optmenu_draw();
    } else switch (sPauseType) {
        case 0: omm_render_pause_init(); break;
        case 1: omm_render_pause_course(); break;
        case 2: omm_render_pause_castle(); break;
    }
    optmenu_check_buttons();
    optmenu_draw_prompt();
    sPauseAlpha = min_s(sPauseAlpha + 0x20, 0xFF);
    return 0;
}

OMM_ROUTINE_PRE_RENDER(omm_render_pause_update) {
    if (!omm_is_game_paused() || omm_is_main_menu() || omm_is_ending_cutscene() || omm_is_ending_cake_screen()) {
        sPauseType = 0;
    }
}

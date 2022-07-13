#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Data
//

static Scroll sCastleScrollV = { 0 };
static Scroll sCastleScrollH = { 0 };
static Scroll sCourseScrollV = { 0 };
static s32 sPauseType = 0;
static s32 sPauseAlpha = 0;
static s32 sLevelList[] = { LEVEL_BOB, LEVEL_WF, LEVEL_JRB, LEVEL_CCM, LEVEL_BBH, LEVEL_HMC, LEVEL_LLL, LEVEL_SSL, LEVEL_DDD, LEVEL_SL, LEVEL_WDW, LEVEL_TTM, LEVEL_THI, LEVEL_TTC, LEVEL_RR, LEVEL_BITDW, LEVEL_BITFS, LEVEL_BITS, LEVEL_TOTWC, LEVEL_COTMC, LEVEL_VCUTM, LEVEL_PSS, LEVEL_SA, LEVEL_WMOTR, LEVEL_CASTLE };

//
// Sparkly Stars timer
//

static void omm_render_pause_ssd_timer(s16 x, s16 y, s16 w, u8 alpha, s32 mode, s32 timer, bool blink) {
    static const char *sTimerGlyphs[] = {
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
        OMM_TEXTURE_STAR_FULL_16,
        OMM_TEXTURE_STAR_FULL_17,
        OMM_TEXTURE_STAR_FULL_18,
        OMM_TEXTURE_STAR_FULL_19,
    };

    s32 glyphs[] = {
        11 + mode,
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
                0xFF, 0xFF, 0xFF, a, sTimerGlyphs[glyphs[i]], false
            );
        }
    }
}

//
// Collectibles
//

OMM_INLINE void omm_render_pause_collectible(s16 x, s16 y, const void *texture, bool collected) {
    omm_render_glyph_hud(x, y, 0xFF * collected, 0xFF * collected, 0xFF * collected, sPauseAlpha / (2 - collected), texture, false);
}

static void omm_render_pause_collectibles() {
#if OMM_GAME_IS_R96A

    // Luigi keys
    if ((gHudDisplay.flags & HUD_DISPLAY_FLAG_KEYS) && gHudDisplay.keys > 0 && gHudDisplay.keys < NUM_KEYS) {
        for (s32 i = 0; i != NUM_KEYS; ++i) {
            omm_render_pause_collectible(
                OMM_RENDER_COLLECTIBLE_X + OMM_RENDER_NUMBER_OFFSET_X * i,
                OMM_RENDER_COLLECTIBLE_Y,
                "textures/segment2/boo_key.rgba16",
                save_file_taken_key(gCurrSaveFileNum - 1, i)
            );
        }
    }

    // Wario coins
    if ((gHudDisplay.flags & HUD_DISPLAY_FLAG_KEYS) && gHudDisplay.wario_coins > 0 && gHudDisplay.wario_coins < NUM_WARIO_COINS) {
        for (s32 i = 0; i != NUM_WARIO_COINS; ++i) {
            omm_render_pause_collectible(
                OMM_RENDER_COLLECTIBLE_X + OMM_RENDER_STAR_OFFSET_X * i,
                OMM_RENDER_COLLECTIBLE_Y + OMM_RENDER_OFFSET_Y,
                "textures/segment2/wario_coin.rgba16",
                save_file_taken_wario_coin(gCurrSaveFileNum - 1, i)
            );
        }
    }
#endif
}

//
// Init
//

void omm_render_pause_init() {
    sPauseAlpha = 0;
    level_set_transition(-1, 0);
    play_sound(SOUND_MENU_PAUSE | 0xFF00, gGlobalSoundArgs);
    if (gCurrCourseNum != COURSE_NONE) {
        sPauseType = 1;
        sCourseScrollV.idx = 0;
    } else {
        sPauseType = 2;
        if (omm_level_get_course(gLastCompletedCourseNum) != COURSE_NONE) {
            sCastleScrollV.idx = -1;
        }
    }
}

//
// Pause (Course)
//

static s16 omm_render_pause_course_name(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum);
    s32 courseNum = omm_level_get_course(levelNum);
    u8 *textCourse = omm_text_convert(OMM_TEXT_COURSE___, false);
    if (COURSE_IS_MAIN_COURSE(courseNum)) {
        s32 length = omm_text_length(textCourse);
        textCourse[length - 3] = DIALOG_CHAR_SPACE;
        if (courseNum >= 10) {
            textCourse[length - 2] = courseNum / 10;
            textCourse[length - 1] = courseNum % 10;
            textCourse[length - 0] = 0xFF;
        } else {
            textCourse[length - 2] = courseNum;
            textCourse[length - 1] = 0xFF;
        }
    }
    omm_render_string_right_align(OMM_RENDER_PAUSE_COURSE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textCourse, true);
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_level_get_name(levelNum, false, false), false);
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
        omm_render_string_right_align(OMM_RENDER_PAUSE_COURSE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textAct, true);
        omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_level_get_act_name(levelNum, gCurrActNum, false, false), false);
        y -= OMM_RENDER_PAUSE_COURSE_OFFSET_Y;
    }
    return y;
}

static s16 omm_render_pause_course_stars(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum);
    if (omm_stars_get_bits_total(levelNum) != 0) {
        u8 *textStars = omm_text_convert(OMM_TEXT_MY_STARS, false);
        omm_render_string_right_align(OMM_RENDER_PAUSE_COURSE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textStars, true);
        omm_render_hud_stars(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, y - ((OMM_RENDER_GLYPH_SIZE - 8) / 2), sPauseAlpha, levelNum, true, false);
        y -= OMM_RENDER_PAUSE_COURSE_OFFSET_Y;
    }
    return y;
}

static s16 omm_render_pause_course_coins(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum);
    if (omm_stars_get_bits_total(levelNum) != 0) {
        u8 *textScore = omm_text_convert(OMM_TEXT_MY_SCORE, false);
        omm_render_string_right_align(OMM_RENDER_PAUSE_COURSE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textScore, true);
        omm_render_hud_coins(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, y - ((OMM_RENDER_GLYPH_SIZE - 8) / 2), sPauseAlpha, save_file_get_course_coin_score(gCurrSaveFileNum - 1, omm_level_get_course(levelNum) - 1), false);
        y -= OMM_RENDER_PAUSE_COURSE_OFFSET_Y;
    }
    return y;
}

static void omm_render_pause_course_red_coins() {
    s32 numRedCoins = omm_level_get_num_red_coins(gCurrLevelNum, gCurrAreaIndex);
    if (numRedCoins > 0 && obj_get_red_coin_star()) {
        s32 redCoinsCollected = numRedCoins - obj_get_count_with_behavior(bhvRedCoin);
        s32 x = OMM_RENDER_RED_COINS_X;
        omm_render_texrect(
            x, OMM_RENDER_RED_COINS_Y, OMM_RENDER_GLYPH_SIZE, OMM_RENDER_GLYPH_SIZE,
            G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 
            0xFF, 0x00, 0x00, 0xFF,
            (const void *) (OMM_ARRAY_OF(const char *) {
                OMM_ASSET_COIN_0,
                OMM_ASSET_COIN_1,
                OMM_ASSET_COIN_2,
                OMM_ASSET_COIN_3,
            })[(gGlobalTimer >> 1) & 3],
            false
        );
        x += 2 * OMM_RENDER_NUMBER_OFFSET_X;
        omm_render_number_hud(x, OMM_RENDER_RED_COINS_Y, sPauseAlpha, redCoinsCollected, 2, true, false);
        x += OMM_RENDER_NUMBER_OFFSET_X + OMM_RENDER_GLYPH_SIZE;
        omm_render_glyph_hud(x, OMM_RENDER_RED_COINS_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, OMM_TEXTURE_HUD_SLASH, false);
        x += OMM_RENDER_GLYPH_SIZE;
        omm_render_number_hud(x, OMM_RENDER_RED_COINS_Y, sPauseAlpha, numRedCoins, 2, true, false);
    }
}

static void omm_render_pause_course_collectibles() {
    omm_render_pause_collectibles();
}

static void omm_render_pause_course_ssd_timer() {
    if (OMM_SSM_IS_ENABLED) {
        s32 mode = gOmmSSM;
        omm_render_pause_ssd_timer(
            OMM_RENDER_SPARKLY_TIMER_X(OMM_RENDER_SPARKLY_TIMER_GLYPH_SIZE),
            OMM_RENDER_SPARKLY_TIMER_Y,
            OMM_RENDER_SPARKLY_TIMER_GLYPH_SIZE,
            sPauseAlpha,
            mode,
            omm_ssd_get_timer(mode),
           !omm_ssd_is_grand_star_collected(mode)
        );
    }
}

static void omm_render_pause_course_options() {
    omm_render_update_scroll(&sCourseScrollV, 4, -gPlayer1Controller->stickY);
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, OMM_RENDER_PAUSE_COURSE_OPTIONS_Y - 0 * OMM_RENDER_PAUSE_COURSE_OFFSET_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_text_convert(OMM_TEXT_CONTINUE, false), false);
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, OMM_RENDER_PAUSE_COURSE_OPTIONS_Y - 1 * OMM_RENDER_PAUSE_COURSE_OFFSET_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_text_convert(OMM_TEXT_RESTART_LEVEL, false), false);
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, OMM_RENDER_PAUSE_COURSE_OPTIONS_Y - 2 * OMM_RENDER_PAUSE_COURSE_OFFSET_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_text_convert(OMM_TEXT_EXIT_LEVEL, false), false);
    omm_render_string_left_align(OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X, OMM_RENDER_PAUSE_COURSE_OPTIONS_Y - 3 * OMM_RENDER_PAUSE_COURSE_OFFSET_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, omm_text_convert(OMM_TEXT_RETURN_TO_CASTLE, false), false);
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
        level_set_transition(0, 0);
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
                omm_exit_level();
            } break;

            case 3: { // Return to Castle (same as Exit Course)
                omm_return_to_castle(true, true);
            } break;
        }
    }
}

void omm_render_pause_course() {
    s16 y = OMM_RENDER_PAUSE_COURSE_INFO_Y - OMM_RENDER_PAUSE_COURSE_OFFSET_Y * !(OMM_STARS_CLASSIC && COURSE_IS_MAIN_COURSE(omm_level_get_course(OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum))));
    y = omm_render_pause_course_name(y);
    y = omm_render_pause_course_act(y);
    y = omm_render_pause_course_stars(y);
    y = omm_render_pause_course_coins(y);
    omm_render_pause_course_red_coins();
    omm_render_pause_course_collectibles();
    omm_render_pause_course_ssd_timer();
    omm_render_pause_course_options();
    omm_render_pause_course_select_option();
}

//
// Pause (Castle)
//

static void omm_render_pause_castle_update() {
    if (sCastleScrollV.idx == -1) {
        for (s32 i = 0; i != OMM_ARRAY_SIZE(sLevelList); ++i) {
            if (gLastCompletedCourseNum == sLevelList[i]) {
                sCastleScrollV.idx = i;
                gLastCompletedCourseNum = LEVEL_CASTLE;
                break;
            }
        }
    }
    omm_render_update_scroll(&sCastleScrollV, OMM_ARRAY_SIZE(sLevelList) + OMM_SSM_IS_ENABLED, -gPlayer1Controller->stickY);
}

static void omm_render_pause_castle_pause() {
    u8 *textPause = omm_text_convert(OMM_TEXT_PAUSE, false);
    omm_render_string_hud_centered(SCREEN_HEIGHT / 2, 0xFF, 0xFF, 0xFF, sPauseAlpha, textPause, false);
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
    s32 levelNum = sLevelList[sCastleScrollV.idx];
    s32 courseIndex = omm_level_get_course(levelNum) - 1;
    
    // Course name
    const u8 *textCourseName = ((courseIndex != -1) ? omm_level_get_name(levelNum, false, true) : omm_text_convert(OMM_TEXT_LEVEL_SECRET_STARS, false));
    omm_render_string_centered(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_1_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, textCourseName, false);

    // Course stars
    if (courseIndex == -1) {
        omm_render_counter(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, 0xFA, save_file_get_course_star_count(gCurrSaveFileNum - 1, -1));
    } else {
        s16 x = (SCREEN_WIDTH - 92) / 2;
        u8 starBits = omm_stars_get_bits_total(levelNum);
        u8 starFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, courseIndex);
        u8 starGlyph[] = { DIALOG_CHAR_STAR_FILLED, 0xFF };
        for (s32 i = 0; i != 7; ++i) {
            if (starBits & (1 << i)) {
                if (starFlags & (1 << i)) {
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
        omm_render_counter(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y, 0xFF, 0xFF, 0xFF, sPauseAlpha, 0xF9, save_file_get_course_coin_score(gCurrSaveFileNum - 1, courseIndex));
    }
}

static void omm_render_pause_castle_ssd_stars_and_timer() {
    omm_render_update_scroll(&sCastleScrollH, 30, gPlayer1Controller->stickX);
    s32 mode = gOmmSSM;
    u8 textR = OMM_SSX_HUD_COLOR[mode][0];
    u8 textG = OMM_SSX_HUD_COLOR[mode][1];
    u8 textB = OMM_SSX_HUD_COLOR[mode][2];

    // Title
    u8 *textSparklyStars = omm_text_convert(OMM_SSX_TEXT_STARS[mode], false);
    omm_render_string_centered(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_1_Y, textR, textG, textB, sPauseAlpha, textSparklyStars, false);

    // Star count
    omm_render_counter(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y, textR, textG, textB, sPauseAlpha, 0xFA, omm_ssd_get_star_count(mode));

    // Level display
    u8 *textLevelName = omm_ssd_get_star_level_name(mode, sCastleScrollH.idx);
    if ((sCastleScrollH.idx == 29 && omm_ssd_is_grand_star_collected(mode)) || omm_ssd_is_star_collected(mode, sCastleScrollH.idx)) {
        omm_render_string_centered(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y, textR, textG, textB, sPauseAlpha, textLevelName, false);
    } else {
        omm_render_string_centered(OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y, textR / 2, textG / 2, textB / 2, sPauseAlpha, textLevelName, false);
    }

    // Sparkly Stars timer
    omm_render_pause_ssd_timer(
        OMM_RENDER_SPARKLY_TIMER_X(OMM_RENDER_SPARKLY_TIMER_GLYPH_SIZE),
        OMM_RENDER_SPARKLY_TIMER_Y,
        OMM_RENDER_SPARKLY_TIMER_GLYPH_SIZE,
        sPauseAlpha,
        mode,
        omm_ssd_get_timer(mode),
        !omm_ssd_is_grand_star_collected(mode)
    );
}

static void omm_render_pause_castle_sm74_edition() {
#if OMM_GAME_IS_SM74
    const u8 *textSm74 = omm_text_convert(OMM_TEXT_SM74_SUPER_MARIO_74, false);
    omm_render_string_centered(SCREEN_HEIGHT - 40, 0xFF, 0xFF, 0xFF, sPauseAlpha, textSm74, true);
    switch (sm74_mode__omm_render_pause_castle) {
        case 1: {
            const u8 *textMode = omm_text_convert(OMM_TEXT_SM74_NORMAL_EDITION, false);
            omm_render_string_centered(SCREEN_HEIGHT - 54, 0x40, 0xFF, 0x40, sPauseAlpha, textMode, true);
        } break;
        case 2: {
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
        level_set_transition(0, 0);
        play_sound(SOUND_MENU_PAUSE_2, gGlobalSoundArgs);
        sPauseType = 0;
        gMenuMode = -1;
        raise_background_noise(1);
        gCameraMovementFlags &= ~CAM_MOVE_PAUSE_SCREEN;
        set_play_mode(0);
    }
}

void omm_render_pause_castle() {
#if OMM_CODE_TIME_TRIALS
    gDialogTextAlpha = sPauseAlpha;
    omm_render_create_dl_ortho_matrix();
    if (!time_trials_render_time_table(&gDialogLineIndex)) {
#endif
    omm_render_pause_castle_update();
    omm_render_pause_castle_pause();
    omm_render_pause_castle_sm74_edition();
    if (sCastleScrollV.idx != OMM_ARRAY_SIZE(sLevelList)) {
        omm_render_pause_castle_course();
    } else {
        omm_render_pause_castle_ssd_stars_and_timer();
    }
#if OMM_CODE_TIME_TRIALS
    }
#endif
    omm_render_pause_castle_collectibles();
    omm_render_pause_castle_close();
}

//
// Pause (render)
//

s32 omm_render_pause() {
    static u8 sOptMenuOpen = 0;
    omm_render_create_dl_ortho_matrix();
    omm_render_shade_screen(110);
    if (optmenu_open) {
        optmenu_draw();
        sOptMenuOpen = true;
    } else {
        if (sOptMenuOpen) {
            gSaveFileModified = true;
            sOptMenuOpen = false;
        }
        switch (sPauseType) {
            case 0: omm_render_pause_init(); break;
            case 1: omm_render_pause_course(); break;
            case 2: omm_render_pause_castle(); break;
        }
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

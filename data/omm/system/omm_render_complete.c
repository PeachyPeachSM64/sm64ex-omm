#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Data
//

static s32 sCompleteState = 0;
static s32 sCompleteCoins = 0;
static s32 sCompleteTimer = 0;
static s32 sCompleteAlpha = 0;

//
// Course complete
//

static void omm_render_course_complete_init() {
    if (sCompleteState == 0) {
        sCompleteCoins = -1;
        sCompleteTimer = -1;
        sCompleteAlpha = 0;
        sCompleteState = 1;
    }
}

static void omm_render_course_complete_update() {
    if (sCompleteCoins < gHudDisplay.coins) {
        sCompleteCoins = min_s(sCompleteCoins + 1, gHudDisplay.coins);
        play_sound(SOUND_MENU_YOSHI_GAIN_LIVES, gGlobalSoundArgs);
        if (sCompleteCoins == gHudDisplay.coins) {
            sCompleteTimer = 0;
            audio_play_star_fanfare();
            if (gGotFileCoinHiScore) {
                play_sound(SOUND_MENU_MARIO_CASTLE_WARP2, gGlobalSoundArgs);
            }
        }
    }
}

static s16 omm_render_course_complete_coins(s16 y) {
    u8 *textScore = omm_text_convert(OMM_TEXT_MY_SCORE, false);
    omm_render_string_right_align(OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sCompleteAlpha, textScore, true);
    omm_render_hud_coins(OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X, y - ((OMM_RENDER_GLYPH_SIZE - 8) / 2), sCompleteAlpha, sCompleteCoins, false);
    if (sCompleteCoins == gHudDisplay.coins && gGotFileCoinHiScore) {
        u8 intensity = (0xFF * ((1.f + sins(gGlobalTimer * 0x1000)) / 2.f));
        u8 *textHiScore = omm_text_convert(OMM_TEXT_COURSE_COMPLETE_HI_SCORE, false);
        omm_render_string_left_align(OMM_RENDER_COURSE_COMPLETE_HI_SCORE_X, y, intensity, intensity, intensity, sCompleteAlpha, textHiScore, true);
    }
    y += OMM_RENDER_COURSE_COMPLETE_OFFSET_Y;
    return y;
}

static s16 omm_render_course_complete_stars(s16 y) {
    u8 *textStars = omm_text_convert(OMM_TEXT_MY_STARS, false);
    omm_render_string_right_align(OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sCompleteAlpha, textStars, true);
    omm_render_hud_stars(OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X, y - ((OMM_RENDER_GLYPH_SIZE - 8) / 2), sCompleteAlpha, OMM_BOWSER_IN_THE_LEVEL(gLastCompletedCourseNum), true, false);
    y += OMM_RENDER_COURSE_COMPLETE_OFFSET_Y;
    return y;
}

static s16 omm_render_course_complete_act_name(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gLastCompletedCourseNum);
    if (COURSE_IS_MAIN_COURSE(omm_level_get_course(levelNum))) {
        u8 *textAct = omm_text_convert(OMM_TEXT_ACT__, false);
        s32 length = omm_text_length(textAct);
        textAct[length - 2] = DIALOG_CHAR_SPACE;
        textAct[length - 1] = gLastCompletedStarNum;
        textAct[length - 0] = 0xFF;
        omm_render_string_right_align(OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sCompleteAlpha, textAct, true);
        omm_render_string_left_align(OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sCompleteAlpha, omm_level_get_act_name(levelNum, gLastCompletedStarNum, false, false), false);
        y += OMM_RENDER_COURSE_COMPLETE_OFFSET_Y;
    }
    return y;
}

static s16 omm_render_course_complete_course_name(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gLastCompletedCourseNum);
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
    omm_render_string_right_align(OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sCompleteAlpha, textCourse, true);
    omm_render_string_left_align(OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sCompleteAlpha, omm_level_get_name(levelNum, false, false), false);
    y += OMM_RENDER_COURSE_COMPLETE_OFFSET_Y;
    return y;
}

static s16 omm_render_course_complete_message(s16 y) {
    y = (((SCREEN_HEIGHT + (y - 8)) / 2) - 8);
    u8 intensity = 0x80 + (0x7F * ((1.f + sins(gGlobalTimer * 0x1000)) / 2.f));
    u8 *textMessage = omm_text_convert(((gLastCompletedCourseNum == LEVEL_BITDW || gLastCompletedCourseNum == LEVEL_BITFS) ? OMM_TEXT_COURSE_COMPLETE_CONGRATULATIONS : OMM_TEXT_COURSE_COMPLETE_GOT_A_STAR), false);
    omm_render_string_hud_centered(y, intensity, intensity, intensity, sCompleteAlpha, textMessage, false);
    return y;
}

static void omm_render_course_complete_end() {
    if (sCompleteTimer != -1 && sCompleteTimer++ >= 90 && obj_anim_is_at_end(gMarioObject)) {
        sCompleteAlpha = max_s(sCompleteAlpha - 0x10, 0x00);
        if (sCompleteAlpha == 0) {
            level_set_transition(0, 0);
            gSaveOptSelectIndex = 1; // Save and continue
            sCompleteState = 0;
            gMenuMode = -1;
        }
    } else {
        sCompleteAlpha = min_s(sCompleteAlpha + 0x40, 0xFF);
    }
}

s32 omm_render_course_complete() {
    s16 y = ((SCREEN_HEIGHT - ((16 * (3 + (COURSE_IS_MAIN_COURSE(omm_level_get_course(OMM_BOWSER_IN_THE_LEVEL(gLastCompletedCourseNum))) ? 1 : 0))) - 8)) / 2);
    omm_render_course_complete_init();
    omm_render_course_complete_update();
    omm_render_shade_screen(sCompleteAlpha / 2);
    y = omm_render_course_complete_coins(y);
    y = omm_render_course_complete_stars(y);
    y = omm_render_course_complete_act_name(y);
    y = omm_render_course_complete_course_name(y);
    y = omm_render_course_complete_message(y);
    omm_render_course_complete_end();
    return 0;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static struct {
    s32 score;
    s32 coins;
    s32 timer;
    s32 alpha;
} sOmmComplete[1];

//
// Course complete
//

void omm_render_course_complete_init() {
    sOmmComplete->score = gMarioState->numCoins;
    sOmmComplete->coins = -1;
    sOmmComplete->timer = -1;
    sOmmComplete->alpha = 0;
}

static void omm_render_course_complete_update() {
    if (sOmmComplete->coins < sOmmComplete->score) {
        sOmmComplete->coins = min_s(sOmmComplete->coins + 1, sOmmComplete->score);
        play_sound(SOUND_MENU_YOSHI_GAIN_LIVES, gGlobalSoundArgs);
        if (sOmmComplete->coins == sOmmComplete->score) {
            sOmmComplete->timer = 0;
            audio_play_star_fanfare();
            if (gGotFileCoinHiScore) {
                play_sound(SOUND_MENU_MARIO_CASTLE_WARP2, gGlobalSoundArgs);
            }
        }
    }
}

static s16 omm_render_course_complete_coins(s16 y) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(OMM_RENDER_GLYPH_SIZE_DEFAULT);
    u8 *textScore = omm_text_convert(OMM_TEXT_MY_SCORE, false);
    omm_render_string_right_align(OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sOmmComplete->alpha, textScore, true);
    omm_render_hud_coins(OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X, y - ((OMM_RENDER_GLYPH_SIZE - 8) / 2), OMM_RENDER_GLYPH_SIZE, sOmmComplete->alpha, sOmmComplete->coins);
    if (sOmmComplete->coins == sOmmComplete->score && gGotFileCoinHiScore) {
        u8 intensity = (0xFF * ((1.f + sins(gGlobalTimer * 0x1000)) / 2.f));
        u8 *textHiScore = omm_text_convert(OMM_TEXT_COURSE_COMPLETE_HI_SCORE, false);
        omm_render_string_left_align(OMM_RENDER_COURSE_COMPLETE_HI_SCORE_X, y, intensity, intensity, intensity, sOmmComplete->alpha, textHiScore, true);
    }
    y += OMM_RENDER_COURSE_COMPLETE_OFFSET_Y;
    return y;
}

static s16 omm_render_course_complete_stars(s16 y) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(OMM_RENDER_GLYPH_SIZE_DEFAULT);
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gLastCompletedLevelNum);
    u8 *textStars = omm_text_convert(OMM_TEXT_MY_STARS, false);
    omm_render_string_right_align(OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sOmmComplete->alpha, textStars, true);
    omm_render_hud_stars(OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X, y - ((OMM_RENDER_GLYPH_SIZE - 8) / 2), OMM_RENDER_GLYPH_SIZE, sOmmComplete->alpha, levelNum);
    y += OMM_RENDER_COURSE_COMPLETE_OFFSET_Y;
    return y;
}

static s16 omm_render_course_complete_act_name(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gLastCompletedLevelNum);
    if (COURSE_IS_MAIN_COURSE(omm_level_get_course(levelNum))) {
        u8 *textAct = omm_text_convert(OMM_TEXT_ACT__, false);
        s32 length = omm_text_length(textAct);
        textAct[length - 2] = DIALOG_CHAR_SPACE;
        textAct[length - 1] = gLastCompletedStarNum;
        textAct[length - 0] = 0xFF;
        ustr_t actName;
        omm_render_string_right_align(OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sOmmComplete->alpha, textAct, true);
        omm_render_string_left_align(OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sOmmComplete->alpha, omm_level_get_act_name(actName, levelNum, gLastCompletedStarNum, OMM_GAME_MODE, false, false), false);
        y += OMM_RENDER_COURSE_COMPLETE_OFFSET_Y;
    }
    return y;
}

static s16 omm_render_course_complete_course_name(s16 y) {
    s32 levelNum = OMM_BOWSER_IN_THE_LEVEL(gLastCompletedLevelNum);
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
    omm_render_string_right_align(OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sOmmComplete->alpha, textCourse, true);
    omm_render_string_left_align(OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X, y, 0xFF, 0xFF, 0xFF, sOmmComplete->alpha, omm_level_get_course_name(courseName, levelNum, OMM_GAME_MODE, false, false), false);
    y += OMM_RENDER_COURSE_COMPLETE_OFFSET_Y;
    return y;
}

static s16 omm_render_course_complete_message(s16 y) {
    y = (((SCREEN_HEIGHT + (y - 8)) / 2) - 8);
    u8 intensity = 0x80 + (0x7F * ((1.f + sins(gGlobalTimer * 0x1000)) / 2.f));
    u8 *textMessage = omm_text_convert((OMM_LEVEL_IS_BOWSER_LEVEL(gLastCompletedLevelNum) ? OMM_TEXT_COURSE_COMPLETE_CONGRATULATIONS : OMM_TEXT_COURSE_COMPLETE_GOT_A_STAR), false);
    omm_render_string_hud_centered(y, intensity, intensity, intensity, sOmmComplete->alpha, textMessage, false);
    return y;
}

static void omm_render_course_complete_end() {
    if (sOmmComplete->timer >= 0 && sOmmComplete->timer++ >= 90 && obj_anim_is_at_end(gMarioObject)) {
        sOmmComplete->alpha = max_s(sOmmComplete->alpha - 0x10, 0x00);
        if (sOmmComplete->alpha == 0) {
            level_set_transition(0, NULL);
            gSaveOptSelectIndex = 1; // Save and continue
            gMenuMode = -1;
        }
    } else {
        sOmmComplete->alpha = min_s(sOmmComplete->alpha + 0x40, 0xFF);
    }
}

s32 omm_render_course_complete() {
    s16 y = ((SCREEN_HEIGHT - ((16 * (3 + (COURSE_IS_MAIN_COURSE(omm_level_get_course(OMM_BOWSER_IN_THE_LEVEL(gLastCompletedLevelNum))) ? 1 : 0))) - 8)) / 2);
    omm_render_course_complete_update();
    omm_render_shade_screen(sOmmComplete->alpha / 2);
    y = omm_render_course_complete_coins(y);
    y = omm_render_course_complete_stars(y);
    y = omm_render_course_complete_act_name(y);
    y = omm_render_course_complete_course_name(y);
    y = omm_render_course_complete_message(y);
    omm_render_course_complete_end();
    return 0;
}

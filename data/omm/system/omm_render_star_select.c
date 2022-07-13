#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Data
//

static struct Object *sStarSelectStars[6];
static Scroll sStarSelectScrollV = { 0 };
static Scroll sStarSelectScrollH = { 0 };
static f32 sStarSelectCurrent;
static s32 sStarSelectTarget;
static s32 sStarSelectCourse;
static s32 sStarSelectTimer;
static s32 sStarSelectAlpha = 0;

//
// Star select
//

s32 omm_render_star_select_get_string_width(const u8 *str64, s32 glyphSize, s32 spacing) {
    s32 width = -spacing;
    for (; *str64 != 0xFF; ++str64) {
        width += spacing;
        switch (*str64) {
            case OMM_RENDER_STAR_SELECT_SPACE:      width +=  8; break;
            case OMM_RENDER_STAR_SELECT_COMMA:      width +=  8; break;
            case OMM_RENDER_STAR_SELECT_HYPHEN:     width += 14; break;
            case OMM_RENDER_STAR_SELECT_APOSTROPHE: width +=  8; break;
            default:                                width += 12; break;
        }
    }
    return ((width * glyphSize) / 16);
}

void omm_render_star_select_string(const u8 *str64, s32 x, s32 y, s32 glyphSize, s32 spacing, bool outline, bool shadow) {
    for (; *str64 != 0xFF; ++str64) {
        s32 dw = 0;
        s32 dx = 0;
        s32 dy = 0;
        const void *texture = NULL;
        switch (*str64) {
            case OMM_RENDER_STAR_SELECT_SPACE:      { dw =  8; dx =  0; dy =  0; texture = NULL; } break;
            case OMM_RENDER_STAR_SELECT_COMMA:      { dw =  8; dx = -2; dy = -6; texture = ((const u8 **) main_hud_lut)[GLYPH_APOSTROPHE]; } break;
            case OMM_RENDER_STAR_SELECT_HYPHEN:     { dw = 14; dx =  0; dy =  0; texture = OMM_TEXTURE_HUD_HYPHEN; } break;
            case OMM_RENDER_STAR_SELECT_APOSTROPHE: { dw =  8; dx = -2; dy = +6; texture = ((const u8 **) main_hud_lut)[GLYPH_APOSTROPHE]; } break;
            case OMM_RENDER_STAR_SELECT_J:          { dw = 12; dx =  0; dy =  0; texture = OMM_TEXTURE_HUD_J; } break;
            case OMM_RENDER_STAR_SELECT_Q:          { dw = 12; dx =  0; dy =  0; texture = OMM_TEXTURE_HUD_Q; } break;
            case OMM_RENDER_STAR_SELECT_V:          { dw = 12; dx =  0; dy =  0; texture = OMM_TEXTURE_HUD_V; } break;
            case OMM_RENDER_STAR_SELECT_X:          { dw = 12; dx =  0; dy =  0; texture = OMM_TEXTURE_HUD_X; } break;
            case OMM_RENDER_STAR_SELECT_Z:          { dw = 12; dx =  0; dy =  0; texture = OMM_TEXTURE_HUD_Z; } break;
            default:                                { dw = 12; dx =  0; dy =  0; texture = ((const u8 **) main_hud_lut)[*str64]; } break;
        }
        dw += spacing;
        dw = ((dw * glyphSize) / 16);
        dx = ((dx * glyphSize) / 16);
        dy = ((dy * glyphSize) / 16);
        if (texture) {
            if (shadow) {
            omm_render_glyph(x + dx + 3, y + dy - 3, glyphSize, glyphSize, 0x20, 0x20, 0x20, sStarSelectAlpha / 2, texture, false);
            }
            if (outline) {
            omm_render_glyph(x + dx + 1, y + dy,     glyphSize, glyphSize, 0x00, 0x00, 0x00, sStarSelectAlpha, texture, false);
            omm_render_glyph(x + dx - 1, y + dy,     glyphSize, glyphSize, 0x00, 0x00, 0x00, sStarSelectAlpha, texture, false);
            omm_render_glyph(x + dx,     y + dy + 1, glyphSize, glyphSize, 0x00, 0x00, 0x00, sStarSelectAlpha, texture, false);
            omm_render_glyph(x + dx,     y + dy - 1, glyphSize, glyphSize, 0x00, 0x00, 0x00, sStarSelectAlpha, texture, false);
            }
            omm_render_glyph(x + dx,     y + dy,     glyphSize, glyphSize, 0xFF, 0xFF, 0xFF, sStarSelectAlpha, texture, false);
        }
        x += dw;
    }
}

void omm_render_star_select_background_rect(s32 x0, s32 y0, u8 r0, u8 g0, u8 b0, u8 a0, s32 x1, s32 y1, u8 r1, u8 g1, u8 b1, u8 a1, bool vrt) {
    Vtx *vtx = alloc_display_list(sizeof(Vtx) * 4);
    vtx[0] = (Vtx) { { { x0, y0, 0 }, 0, { 0, 0 }, { r0, g0, b0, (a0              * sStarSelectAlpha) / 0xFF } } };
    vtx[1] = (Vtx) { { { x1, y0, 0 }, 0, { 0, 0 }, { r0, g0, b0, ((vrt ? a0 : a1) * sStarSelectAlpha) / 0xFF } } };
    vtx[2] = (Vtx) { { { x1, y1, 0 }, 0, { 0, 0 }, { r1, g1, b1, (a1              * sStarSelectAlpha) / 0xFF } } };
    vtx[3] = (Vtx) { { { x0, y1, 0 }, 0, { 0, 0 }, { r1, g1, b1, ((vrt ? a1 : a0) * sStarSelectAlpha) / 0xFF } } };
    omm_render_create_dl_ortho_matrix();
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);;
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0x0, 0, 2, 3, 0x0);
    gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
}

//
// Update
//

static s32 omm_render_star_select_init_values() {
    u8 starFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
    sStarSelectTimer = 0;
    sStarSelectCourse = gCurrCourseNum - COURSE_MIN;
    sStarSelectCurrent = 0;
    while ((sStarSelectCurrent < 6) && (starFlags & (1 << ((s32) (sStarSelectCurrent))))) sStarSelectCurrent += 1;
    sStarSelectCurrent = (((s32) sStarSelectCurrent) % 6);
    sStarSelectTarget = sStarSelectCurrent;
    return 0;
}

static s32 omm_render_star_select_init_objects() {
    sStarSelectStars[0] = spawn_object(gCurrentObject, MODEL_STAR, omm_bhv_act_select_star);
    sStarSelectStars[1] = spawn_object(gCurrentObject, MODEL_STAR, omm_bhv_act_select_star);
    sStarSelectStars[2] = spawn_object(gCurrentObject, MODEL_STAR, omm_bhv_act_select_star);
    sStarSelectStars[3] = spawn_object(gCurrentObject, MODEL_STAR, omm_bhv_act_select_star);
    sStarSelectStars[4] = spawn_object(gCurrentObject, MODEL_STAR, omm_bhv_act_select_star);
    sStarSelectStars[5] = spawn_object(gCurrentObject, MODEL_STAR, omm_bhv_act_select_star);
    return 0;
}

static s32 omm_render_star_select_update_objects() {
    u8 starFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
    s32 selectedIndex = ((((s32) (sStarSelectCurrent < 0.f ? (sStarSelectCurrent - 0.5f) : (sStarSelectCurrent + 0.5f))) + 6) % 6);
    for (s32 i = 0; i != 6; ++i) {
        f32 t = (f32) i - sStarSelectCurrent;
        sStarSelectStars[i]->oPosX = OMM_RENDER_STAR_SELECT_ACT_STAR_X + OMM_RENDER_STAR_SELECT_ACT_STAR_DX * sins((t * 0x10000) / 6);
        sStarSelectStars[i]->oPosY = OMM_RENDER_STAR_SELECT_ACT_STAR_Y;
        sStarSelectStars[i]->oPosZ = OMM_RENDER_STAR_SELECT_ACT_STAR_Z + OMM_RENDER_STAR_SELECT_ACT_STAR_DZ * coss((t * 0x10000) / 6);
        sStarSelectStars[i]->oGraphNode = gLoadedGraphNodes[((starFlags & (1 << i)) ? MODEL_STAR : MODEL_TRANSPARENT_STAR)];
        sStarSelectStars[i]->oFaceAngleYaw = (sStarSelectStars[i]->oFaceAngleYaw + 0x800) * (i == selectedIndex);
    }
    return 0;
}

static s32 omm_render_star_select_update_inputs() {
    u8 starFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);

    // Up-down scroll to change the course
    // A course is available only if at least 1 star has been collected
    // The course that showed up first is always available
    sStarSelectScrollV.idx = gCurrCourseNum - COURSE_MIN;
    omm_render_update_scroll(&sStarSelectScrollV, COURSE_STAGES_COUNT, -gPlayer1Controller->stickY);
    while (sStarSelectScrollV.inc != 0) {
        s32 nextCourseIndex = (((gCurrCourseNum - COURSE_MIN) + sStarSelectScrollV.inc + COURSE_STAGES_COUNT) % COURSE_STAGES_COUNT);
        if (nextCourseIndex == sStarSelectCourse || (save_file_get_course_star_count(gCurrSaveFileNum - 1, nextCourseIndex) != 0)) {
            gCurrLevelNum = omm_level_get_list()[nextCourseIndex];
            gCurrCourseNum = omm_level_get_course(gCurrLevelNum);
            gSavedCourseNum = gCurrCourseNum;
            sWarpDest.levelNum = gCurrLevelNum;
            starFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
            break;
        }
        gCurrCourseNum = nextCourseIndex + COURSE_MIN;
    }

    // Left-right scroll to change the act
    // An act is available only if its star or the star of the previous act has been collected
    // The first act is always available
    s32 targetIndex = ((sStarSelectTarget + 6) % 6);
    if (targetIndex != 0 && !(starFlags & (1 << targetIndex)) && !(starFlags & (1 << (targetIndex - 1)))) {
        sStarSelectScrollH.inc = -1;
    } else {
        sStarSelectScrollH.idx = ((sStarSelectTarget + 6) % 6);
        omm_render_update_scroll(&sStarSelectScrollH, 6, +gPlayer1Controller->stickX);
    }
    while (sStarSelectScrollH.inc != 0) {
        sStarSelectTarget += sStarSelectScrollH.inc;
        s32 nextTargetIndex = ((sStarSelectTarget + 6) % 6);
        if (nextTargetIndex == 0 || (starFlags & (1 << nextTargetIndex)) || (starFlags & (1 << (nextTargetIndex - 1)))) {
            break;
        }
    }

    // Move smoothly the star selector
    f32 delta = clamp_f(sStarSelectTarget - sStarSelectCurrent, -1.f, +1.f) / 4.f;
    sStarSelectCurrent += delta;
    if (abs_f(sStarSelectTarget - sStarSelectCurrent) < 0.01f) { sStarSelectCurrent = sStarSelectTarget; }
    while (sStarSelectTarget <= -6) { sStarSelectTarget += 6; sStarSelectCurrent += 6; }
    while (sStarSelectTarget >= +6) { sStarSelectTarget -= 6; sStarSelectCurrent -= 6; }
    while (abs_f(sStarSelectTarget - sStarSelectCurrent) >= 6) { sStarSelectCurrent += 6 * sign_f(sStarSelectTarget - sStarSelectCurrent); }

    // Press A, B or Start to select the act
    if ((sStarSelectTimer++ >= 15) && (gPlayer1Controller->buttonPressed & (A_BUTTON | B_BUTTON | START_BUTTON))) {
        play_sound(SOUND_MENU_STAR_SOUND_LETS_A_GO, gGlobalSoundArgs);
        s32 selectedIndex = ((sStarSelectTarget + 6) % 6);
        gDialogCourseActNum = selectedIndex + 1;
        return selectedIndex + 1;
    }

    // Update objects
    area_update_objects();
    return 0;
}

static s32 omm_render_star_select() {
    u8 starFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
    s32 selectedIndex = ((sStarSelectTarget + 6) % 6);
    u32 color = (OMM_GAME_IS_SMMS ? 0x00F000FF : (OMM_EXTRAS_COLORED_STARS ? omm_stars_get_color(gCurrLevelNum) : 0xFFFFFFFF));
    u8 r = (color >> 24) & 0xFF;
    u8 g = (color >> 16) & 0xFF;
    u8 b = (color >>  8) & 0xFF;

    // Background
    omm_render_star_select_background_rect(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), 0, r, g, b, 0xFF, SCREEN_WIDTH / 2, SCREEN_HEIGHT, r, g, b, 0, false);
    omm_render_star_select_background_rect(SCREEN_WIDTH / 2, 0, r, g, b, 0, GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0), SCREEN_HEIGHT, r, g, b, 0xFF, false);
    omm_render_star_select_background_rect(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), OMM_RENDER_STAR_SELECT_BACKGROUND_BOTTOM_Y0, r, g, b, 0xFF, GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0), OMM_RENDER_STAR_SELECT_BACKGROUND_BOTTOM_Y1, r / 2, g / 2, b / 2, 0xFF, true);
    omm_render_star_select_background_rect(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), OMM_RENDER_STAR_SELECT_BACKGROUND_TOP_Y0, r / 2, g / 2, b / 2, 0xFF, GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0), OMM_RENDER_STAR_SELECT_BACKGROUND_TOP_Y1, r, g, b, 0xFF, true);

    // Course name
    const u8 *courseName = omm_level_get_name(gCurrLevelNum, false, false);
    s32 length = omm_text_length(courseName);
    s32 courseNum = omm_level_get_course(gCurrLevelNum);
    u8 buffer[64];
    if (courseNum < 10) {
        buffer[0] = courseNum;
        buffer[1] = DIALOG_CHAR_SPACE;
        buffer[2] = OMM_RENDER_STAR_SELECT_HYPHEN;
        buffer[3] = DIALOG_CHAR_SPACE;
        OMM_MEMCPY(&buffer[4], courseName, length + 1);
    } else {
        buffer[0] = courseNum / 10;
        buffer[1] = courseNum % 10;
        buffer[2] = DIALOG_CHAR_SPACE;
        buffer[3] = OMM_RENDER_STAR_SELECT_HYPHEN;
        buffer[4] = DIALOG_CHAR_SPACE;
        OMM_MEMCPY(&buffer[5], courseName, length + 1);
    }
    omm_text_capitalize(buffer);
    omm_render_star_select_string(buffer, (SCREEN_WIDTH - omm_render_star_select_get_string_width(buffer, 16, 1)) / 2, OMM_RENDER_STAR_SELECT_COURSE_NAME_Y, 16, 1, true, true);
    
    // Act text
    u8 *actText = omm_text_convert(OMM_TEXT_ACT, false);
    s32 actTextW = omm_render_star_select_get_string_width(actText, 12, 2);
    s32 actTextX = (SCREEN_WIDTH - (actTextW + 18)) / 2;
    s32 actTextY = OMM_RENDER_STAR_SELECT_BACKGROUND_BOTTOM_Y1 - 6;
    omm_render_star_select_string(actText, actTextX, actTextY, 12, 2, true, true);

    // Act number
    u8 actNumber[] = { selectedIndex + 1, 0xFF };
    s32 actNumberX = actTextX + actTextW + 6;
    s32 actNumberY = OMM_RENDER_STAR_SELECT_BACKGROUND_BOTTOM_Y1 - 8;
    omm_render_star_select_string(actNumber, actNumberX, actNumberY, 16, 0, true, true);

    // Act name
    u8 *actName = omm_level_get_act_name(gCurrLevelNum, selectedIndex + 1, false, false);
    s32 actNameX = (SCREEN_WIDTH - omm_render_get_string_width(actName)) / 2;
    omm_render_string(actNameX - 1, OMM_RENDER_STAR_SELECT_ACT_NAME_Y,     0x00, 0x00, 0x00, sStarSelectAlpha, actName, false);
    omm_render_string(actNameX + 1, OMM_RENDER_STAR_SELECT_ACT_NAME_Y,     0x00, 0x00, 0x00, sStarSelectAlpha, actName, false);
    omm_render_string(actNameX,     OMM_RENDER_STAR_SELECT_ACT_NAME_Y - 1, 0x00, 0x00, 0x00, sStarSelectAlpha, actName, false);
    omm_render_string(actNameX,     OMM_RENDER_STAR_SELECT_ACT_NAME_Y + 1, 0x00, 0x00, 0x00, sStarSelectAlpha, actName, false);
    omm_render_string(actNameX,     OMM_RENDER_STAR_SELECT_ACT_NAME_Y,     0xFF, 0xFF, 0xFF, sStarSelectAlpha, actName, false);

    // Score text
    u8 *scoreText = omm_text_convert(OMM_TEXT_MY_SCORE, false);
    s32 scoreTextX = (SCREEN_WIDTH / 2) - (omm_render_get_string_width(scoreText) + 11);
    omm_render_string(scoreTextX - 1, OMM_RENDER_STAR_SELECT_SCORE_Y,     0x40, 0x40, 0x00, sStarSelectAlpha, scoreText, false);
    omm_render_string(scoreTextX + 1, OMM_RENDER_STAR_SELECT_SCORE_Y,     0x40, 0x40, 0x00, sStarSelectAlpha, scoreText, false);
    omm_render_string(scoreTextX,     OMM_RENDER_STAR_SELECT_SCORE_Y - 1, 0x40, 0x40, 0x00, sStarSelectAlpha, scoreText, false);
    omm_render_string(scoreTextX,     OMM_RENDER_STAR_SELECT_SCORE_Y + 1, 0x40, 0x40, 0x00, sStarSelectAlpha, scoreText, false);
    omm_render_string(scoreTextX,     OMM_RENDER_STAR_SELECT_SCORE_Y,     0xFF, 0xFF, 0x00, sStarSelectAlpha, scoreText, false);

    // Coin score
    s32 scoreCoins = save_file_get_course_coin_score(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
    s32 scoreCoinsX = (SCREEN_WIDTH / 2) - 7;
    omm_render_hud_coins(scoreCoinsX, OMM_RENDER_STAR_SELECT_SCORE_Y - ((OMM_RENDER_GLYPH_SIZE - 8) / 2), sStarSelectAlpha, scoreCoins, false);

    // 100 coins star
    if (starFlags & 0x40) {
        s32 coinsStarX = OMM_RENDER_STAR_SELECT_100_COINS_STAR_X;
        s32 coinsStarY = OMM_RENDER_STAR_SELECT_SCORE_Y - ((OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE - 8) / 2) + 1;
        const void *coinsStarTex = omm_render_get_star_glyph(clamp_s(gCurrCourseNum, 0, 16), OMM_EXTRAS_COLORED_STARS);
        omm_render_glyph(coinsStarX + 1, coinsStarY,     OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, 0x00, 0x00, 0x00, sStarSelectAlpha, coinsStarTex, false);
        omm_render_glyph(coinsStarX - 1, coinsStarY,     OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, 0x00, 0x00, 0x00, sStarSelectAlpha, coinsStarTex, false);
        omm_render_glyph(coinsStarX,     coinsStarY + 1, OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, 0x00, 0x00, 0x00, sStarSelectAlpha, coinsStarTex, false);
        omm_render_glyph(coinsStarX,     coinsStarY - 1, OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, 0x00, 0x00, 0x00, sStarSelectAlpha, coinsStarTex, false);
        omm_render_glyph(coinsStarX,     coinsStarY,     OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE, 0xFF, 0xFF, 0xFF, sStarSelectAlpha, coinsStarTex, false);
    }

#if OMM_CODE_TIME_TRIALS
    time_trials_render_star_select_time(selectedIndex);
#endif
    return 0;
}

s32 omm_render_star_select_update(s32 action) {
    sStarSelectAlpha = (clamp_s(sStarSelectTimer, 0, 15) * 17);
    switch (action) {
        case 0: return omm_render_star_select_init_values();
        case 1: return omm_render_star_select_init_objects();
        case 2: return omm_render_star_select_update_objects();
        case 3: return omm_render_star_select_update_inputs();
        case 4: return omm_render_star_select();
    }
    return 0;
}

//
// Behavior functions
//

#if OMM_GAME_IS_R96A
s32 lvl_star_select() {
    r96_play_menu_jingle(R96_EVENT_STAR_SELECT);
}
#endif

void bhv_act_selector_star_type_loop(void) {
}

void bhv_act_selector_init(void) {
    omm_render_star_select_update(1);
}

void bhv_act_selector_loop(void) {
    omm_render_star_select_update(2);
}

s32 lvl_init_act_selector_values_and_stars(UNUSED s32 arg, UNUSED s32 unused) {
    return omm_render_star_select_update(0);
}

s32 lvl_update_obj_and_load_act_button_actions(UNUSED s32 arg, UNUSED s32 unused) {
    return omm_render_star_select_update(3);
}

#ifdef AVOID_UB
Gfx *geo_act_selector_strings(s16 callContext, UNUSED struct GraphNode *node, UNUSED void *context) {
#else
Gfx *geo_act_selector_strings(s16 callContext, UNUSED struct GraphNode *node) {
#endif
    if (callContext == GEO_CONTEXT_RENDER) {
        omm_render_star_select_update(4);
    }
    return NULL;
}

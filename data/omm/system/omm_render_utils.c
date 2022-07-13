#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
//       COORDS
//
//   |         
//   |   _    _
//   |  | \  / |
//   |  ||\\//||
// y--- || \/ ||
//   |
//   +--|---------
//      x
///////////////////

void omm_render_update_scroll(Scroll *scroll, s32 count, s16 stickValue) {
    s32 val = 2 * (stickValue > +60) + 1 * (stickValue < -60);
    s32 inc = (((val ^ scroll->val) & val) == 2) - (((val ^ scroll->val) & val) == 1);
    if (inc != 0) {
        scroll->idx += inc;
        play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundArgs);
    }
    if (scroll->tmr == 10) {
        scroll->tmr = 8;
        scroll->val = 0;
    } else {
        scroll->tmr = (scroll->tmr + 1) * (val != 0);
        scroll->val = val;
    }
    scroll->idx = ((scroll->idx + count) % count);
    scroll->inc = inc;
}

void omm_render_create_dl_ortho_matrix() {
    static const Mtx sIdentMatrix = { {
        { 1.f, 0.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f, 0.f },
        { 0.f, 0.f, 0.f, 1.f },
    } };
    static const Mtx sOrthoMatrix = { {
        { 2.f / SCREEN_WIDTH, 0.f, 0.f, 0.f },
        { 0.f, 2.f / SCREEN_HEIGHT, 0.f, 0.f },
        { 0.f, 0.f, -0.1f, 0.f },
        { -1.f, -1.f, 0.f, 1.f },
    } };
    gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
    gSPMatrix(gDisplayListHead++, &sIdentMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gDisplayListHead++, &sIdentMatrix, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gDisplayListHead++, &sOrthoMatrix, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
}

void omm_render_shade_screen(u8 alpha) {
    static Vtx sBlackScreenVtx[4];
    sBlackScreenVtx[0] = (Vtx) {{{ GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 0, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF }}};
    sBlackScreenVtx[1] = (Vtx) {{{ GFX_DIMENSIONS_FROM_RIGHT_EDGE(0), 0, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF }}};
    sBlackScreenVtx[2] = (Vtx) {{{ GFX_DIMENSIONS_FROM_RIGHT_EDGE(0), SCREEN_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF }}};
    sBlackScreenVtx[3] = (Vtx) {{{ GFX_DIMENSIONS_FROM_LEFT_EDGE(0), SCREEN_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF }}};
    omm_render_create_dl_ortho_matrix();
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, alpha);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, sBlackScreenVtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
}

void omm_render_texrect(s16 x, s16 y, s16 w, s16 h, s32 fmt, s32 siz, s16 texw, s16 texh, u8 red, u8 green, u8 blue, u8 alpha, const void *texture, bool shadow) {
    if (shadow) {
        omm_render_texrect(x + 1, y - 1, w, h, fmt, siz, texw, texh, 0, 0, 0, alpha, texture, false);
    }
    omm_render_create_dl_ortho_matrix();
    gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    switch (siz) {
        case G_IM_SIZ_4b:  gDPLoadTextureBlock(gDisplayListHead++, texture, fmt, G_IM_SIZ_4b,  texw, texh, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0); break;
        case G_IM_SIZ_8b:  gDPLoadTextureBlock(gDisplayListHead++, texture, fmt, G_IM_SIZ_8b,  texw, texh, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0); break;
        case G_IM_SIZ_16b: gDPLoadTextureBlock(gDisplayListHead++, texture, fmt, G_IM_SIZ_16b, texw, texh, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0); break;
        case G_IM_SIZ_32b: gDPLoadTextureBlock(gDisplayListHead++, texture, fmt, G_IM_SIZ_32b, texw, texh, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0); break;
    }
    gSPTextureRectangle(gDisplayListHead++, (x) << 2, (SCREEN_HEIGHT - h - y) << 2, (x + w) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, ((0x400 * texw) / w), ((0x400 * texh) / h));
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
}

void omm_render_number(s16 x, s16 y, s16 w, s16 h, s16 xStep, u8 alpha, s32 number, s32 digits, bool makeZerosTransparent, bool shadow) {
    static const char *sOmmNumberGlyphs[] = {
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
    };

    s32 glyphs[16];
    number = clamp_s(number, 0, 0x7FFFFFFF);
    digits = clamp_s(digits, 1, 15);
    for (s32 i = 0; i != digits; ++i) {
        glyphs[i] = (number % 10);
        number /= 10;
    }
    for (s32 i = digits, j = 0, n = 0, a = 0; i != 0; --i, x += xStep) {
        n = glyphs[i - 1];
        j = max_s(j, n);
        a = (alpha * (0xFF - 0xAA * (makeZerosTransparent && ((digits == 1) || (i > 1)) && (j == 0)))) / 0xFF;
        omm_render_texrect(x, y, w, h, G_IM_FMT_RGBA, G_IM_SIZ_32b, 16, 16, 0xFF, 0xFF, 0xFF, a, sOmmNumberGlyphs[n], shadow);
    }
}

void omm_render_string(s16 x, s16 y, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow) {
    if (shadow) {
        omm_render_string(x + 1, y - 1, 0, 0, 0, alpha, str64, false);
    }
    omm_render_create_dl_ortho_matrix();
    u8 *temp = omm_text_capitalize(omm_text_copy(str64, false));
    gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
    for (; *temp != DIALOG_CHAR_TERMINATOR; temp++) {
        if (*temp != DIALOG_CHAR_SPACE) {
            gDPLoadTextureBlock(gDisplayListHead++, menu_font_lut[*temp], G_IM_FMT_IA, G_IM_SIZ_8b, 8, 8, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
            gSPTextureRectangle(gDisplayListHead++, x << 2, (SCREEN_HEIGHT - 8 - y) << 2, (x + 8) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        }
        x += omm_render_get_char_width(*temp);
    }
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

void omm_render_string_hud(s16 x, s16 y, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow) {
    if (shadow) {
        omm_render_string_hud(x + 1, y - 1, 0, 0, 0, alpha, str64, false);
    }
    omm_render_create_dl_ortho_matrix();
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
    for (; *str64 != GLOBAR_CHAR_TERMINATOR; str64++) {
        if (*str64 != GLOBAL_CHAR_SPACE) {
            gDPLoadTextureBlock(gDisplayListHead++, main_hud_lut[*str64], G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
            gSPTextureRectangle(gDisplayListHead++, x << 2, (SCREEN_HEIGHT - 16 - y) << 2, (x + 16) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        }
        x += omm_render_get_char_width_hud(*str64);
    }
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

s32 omm_render_get_char_width(u8 c) {
    return ((c == DIALOG_CHAR_SPACE) ? 4 : gDialogCharWidths[c - (26 * (c >= 0x24 && c <= 0x3D))]);
}

s32 omm_render_get_char_width_hud(u8 c) {
    return ((c == DIALOG_CHAR_SPACE) ? 8 : 12);
}

s32 omm_render_get_string_width(const u8 *str64) {
    s32 width = 0;
    for (; *str64 != 0xFF; str64++) {
        width += omm_render_get_char_width(*str64);
    }
    return width;
}

s32 omm_render_get_string_width_hud(const u8 *str64) {
    s32 width = 0;
    for (; *str64 != 0xFF; str64++) {
        width += omm_render_get_char_width_hud(*str64);
    }
    return width;
}

u8 *omm_render_get_star_glyph(s32 index, bool colored) {
    const void *sOmmStarGlyphs[][2] = {
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_0 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_1 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_2 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_3 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_4 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_5 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_6 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_7 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_8 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_9 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_10 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_11 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_12 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_13 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_14 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_15 },
        { (const void *) ((const u8 **) main_hud_lut)[GLYPH_STAR], (const void *) OMM_TEXTURE_STAR_FULL_16 },
    };
    return (u8 *) sOmmStarGlyphs[index][colored];
}

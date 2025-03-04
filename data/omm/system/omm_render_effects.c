#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

s32 sOmmVibeType = OMM_PEACH_VIBE_TYPE_NONE;
f32 sOmmVibeAlpha = 0.f;
f32 sOmmVibeBgAlpha = 0.f;
u8  sOmmYouGotAStarText[3][64];
s32 sOmmYouGotAStarTimer = 0;
s32 sOmmYouGotAStarRender = 0;

//
// Frame interpolation
//

static InterpData sOmmVibeBg[1];
static InterpData sOmmVibeHeart[1];
static InterpData sOmmYouGotAStar[1];

static Gfx *omm_render_effect_vibe_background(Gfx *pos, Gfx *gfx, Vtx *vtx, f32 alpha, f32 timer);
static Gfx *omm_render_effect_vibe_heart(Gfx *pos, f32 alpha, f32 scale);
static Gfx *omm_render_effect_you_got_a_star_pop_up(Gfx *pos, f32 timer, f32 offset);

void gfx_interpolate_frame_effects(f32 t) {

    // Vibe background
    if (sOmmVibeBg->pos) {
        interp_data_lerp(sOmmVibeBg, t);
        omm_render_effect_vibe_background(sOmmVibeBg->pos, sOmmVibeBg->gfx, sOmmVibeBg->vtx, sOmmVibeBg->a, sOmmVibeBg->t);
    }

    // Vibe heart
    if (sOmmVibeHeart->pos) {
        interp_data_lerp(sOmmVibeHeart, t);
        omm_render_effect_vibe_heart(sOmmVibeHeart->pos, sOmmVibeHeart->a, sOmmVibeHeart->s);
    }

    // You got a star
    if (sOmmYouGotAStar->pos) {
        interp_data_lerp(sOmmYouGotAStar, t);
        omm_render_effect_you_got_a_star_pop_up(sOmmYouGotAStar->pos, sOmmYouGotAStar->t, sOmmYouGotAStar->y);
    }
}

void gfx_clear_frame_effects() {
    sOmmVibeBg->pos = NULL;
    sOmmVibeHeart->pos = NULL;
    sOmmYouGotAStar->pos = NULL;
}

//
// Freezing water
//

void omm_render_effect_freeze() {
    u8 alpha = (u8) (128.9f * clamp_f((f32) gOmmMario->state.freeze.gfx / (f32) OMM_FREEZING_WATER_TIMER_CRITICAL, 0.f, 1.f));
    if (HUD_DISPLAY_EFFECTS && !OMM_HUD_NONE && alpha > 0) {
        if (alpha >= 128) {
            alpha += 32 * (gGlobalTimer & 1);
        }

        f32 x0 = GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0);
        f32 x1 = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0);
        f32 y0 = 0.f;
        f32 y1 = SCREEN_HEIGHT;
        Vtx *vtx = omm_alloc_vtx(4);
        vtx[0] = (Vtx) {{{ x0, y0, 0 }, 0, { 0x0000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
        vtx[1] = (Vtx) {{{ x1, y0, 0 }, 0, { 0x4000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
        vtx[2] = (Vtx) {{{ x1, y1, 0 }, 0, { 0x4000, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
        vtx[3] = (Vtx) {{{ x0, y1, 0 }, 0, { 0x0000, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};

        omm_render_create_dl_ortho_matrix();
        OMM_RENDER_ENABLE_ALPHA(gDisplayListHead++);
        gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, alpha / (OMM_HUD_PRO ? 1.5f : 1.f));
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(gDisplayListHead++, OMM_TEXTURE_EFFECT_FREEZE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 128, 0, G_TX_MIRROR, G_TX_MIRROR, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, vtx, 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    }
}

//
// Dark mode
//

void omm_render_effect_dark_mode() {
    if (HUD_DISPLAY_EFFECTS && omm_sparkly_context_get_data(OMM_SPARKLY_DATA_DARK_MODE) && OMM_SPARKLY_STATE_IS_OK) {
        Vtx *vtx = omm_alloc_vtx(OMM_RENDER_DARK_MODE_NUM_POINTS * 6);
        Gfx *gfx = omm_alloc_gfx(OMM_RENDER_DARK_MODE_NUM_POINTS * 3 + 16);
        omm_render_create_dl_ortho_matrix();
        OMM_RENDER_ENABLE_ALPHA(gDisplayListHead++);
        gSPDisplayList(gDisplayListHead++, gfx);
        gSPClearGeometryMode(gfx++, G_LIGHTING | G_CULL_BOTH);
        gDPSetCombineLERP(gfx++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        for (s32 i = 0; i != OMM_RENDER_DARK_MODE_NUM_POINTS; ++i, vtx += 6) {
            s32 a0 = ((i + 0) * 0x10000) / OMM_RENDER_DARK_MODE_NUM_POINTS;
            s32 a1 = ((i + 1) * 0x10000) / OMM_RENDER_DARK_MODE_NUM_POINTS;
            f32 ca0 = coss(a0);
            f32 ca1 = coss(a1);
            f32 sa0 = sins(a0);
            f32 sa1 = sins(a1);
            vtx[0] = (Vtx) { { { OMM_RENDER_DARK_MODE_CENTER_X + sa0 * OMM_RENDER_DARK_MODE_RADIUS_MIN * OMM_RENDER_DARK_MODE_HALF_WIDTH, OMM_RENDER_DARK_MODE_CENTER_Y + ca0 * OMM_RENDER_DARK_MODE_RADIUS_MIN * OMM_RENDER_DARK_MODE_HALF_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x00 } } };
            vtx[1] = (Vtx) { { { OMM_RENDER_DARK_MODE_CENTER_X + sa1 * OMM_RENDER_DARK_MODE_RADIUS_MIN * OMM_RENDER_DARK_MODE_HALF_WIDTH, OMM_RENDER_DARK_MODE_CENTER_Y + ca1 * OMM_RENDER_DARK_MODE_RADIUS_MIN * OMM_RENDER_DARK_MODE_HALF_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x00 } } };
            vtx[2] = (Vtx) { { { OMM_RENDER_DARK_MODE_CENTER_X + sa0 * OMM_RENDER_DARK_MODE_RADIUS_MAX * OMM_RENDER_DARK_MODE_HALF_WIDTH, OMM_RENDER_DARK_MODE_CENTER_Y + ca0 * OMM_RENDER_DARK_MODE_RADIUS_MAX * OMM_RENDER_DARK_MODE_HALF_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF } } };
            vtx[3] = (Vtx) { { { OMM_RENDER_DARK_MODE_CENTER_X + sa1 * OMM_RENDER_DARK_MODE_RADIUS_MAX * OMM_RENDER_DARK_MODE_HALF_WIDTH, OMM_RENDER_DARK_MODE_CENTER_Y + ca1 * OMM_RENDER_DARK_MODE_RADIUS_MAX * OMM_RENDER_DARK_MODE_HALF_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF } } };
            vtx[4] = (Vtx) { { { OMM_RENDER_DARK_MODE_CENTER_X + sa0 * OMM_RENDER_DARK_MODE_RADIUS_OUT * OMM_RENDER_DARK_MODE_HALF_WIDTH, OMM_RENDER_DARK_MODE_CENTER_Y + ca0 * OMM_RENDER_DARK_MODE_RADIUS_OUT * OMM_RENDER_DARK_MODE_HALF_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF } } };
            vtx[5] = (Vtx) { { { OMM_RENDER_DARK_MODE_CENTER_X + sa1 * OMM_RENDER_DARK_MODE_RADIUS_OUT * OMM_RENDER_DARK_MODE_HALF_WIDTH, OMM_RENDER_DARK_MODE_CENTER_Y + ca1 * OMM_RENDER_DARK_MODE_RADIUS_OUT * OMM_RENDER_DARK_MODE_HALF_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF } } };
            gSPVertex(gfx++, vtx, 6, 0);
            gSP2Triangles(gfx++, 0, 1, 2, 0, 1, 2, 3, 0);
            gSP2Triangles(gfx++, 2, 3, 4, 0, 3, 4, 5, 0);
        }
        gDPSetCombineLERP(gfx++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        gSPSetGeometryMode(gfx++, G_LIGHTING | G_CULL_BACK);
        gSPEndDisplayList(gfx);
    }
}

//
// Vibes (Peach only)
//

static const char *OMM_VIBE_TEXTURES[5][2] = {
    { OMM_TEXTURE_HUD_VIBE_NORMAL, NULL },
    { OMM_TEXTURE_HUD_VIBE_JOY, OMM_TEXTURE_EFFECT_VIBE_JOY },
    { OMM_TEXTURE_HUD_VIBE_RAGE, OMM_TEXTURE_EFFECT_VIBE_RAGE },
    { OMM_TEXTURE_HUD_VIBE_GLOOM, OMM_TEXTURE_EFFECT_VIBE_GLOOM },
    { OMM_TEXTURE_HUD_VIBE_CALM, OMM_TEXTURE_EFFECT_VIBE_CALM },
};

static Vtx omm_render_effect_vibe_get_vertex(f32 x, f32 y, u8 alpha, f32 timer) {
    Vtx v;
    v.v.ob[0] = x;
    v.v.ob[1] = y;
    v.v.ob[2] = 0.f;
    v.v.tc[0] = (invlerp_f(x, GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0)) * 256 - (fmod(timer, 512) / 2.f)) * 32;
    v.v.tc[1] = (invlerp_f(y, 0.f, SCREEN_HEIGHT) * 256) * 32;
    v.v.cn[0] = 0xFF;
    v.v.cn[1] = 0xFF;
    v.v.cn[2] = 0xFF;
    v.v.cn[3] = alpha;
    v.v.flag  = 0;
    return v;
}

static Gfx *omm_render_effect_vibe_background(Gfx *pos, Gfx *gfx, Vtx *vtx, f32 alpha, f32 timer) {
    if (alpha > 0.f) {
        s32 sw = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0);
        s32 sh = SCREEN_HEIGHT;
        s32 dw = (s32) relerp_f(sins(timer * 0x300), -1.f, 1.f, sw / 8.f, sw / 6.f);
        s32 dh = (s32) relerp_f(sins(timer * 0x300), -1.f, 1.f, sh / 6.f, sh / 4.f);
        s32 x0 = GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0);
        s32 x1 = GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(dw);
        s32 x2 = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(dw);
        s32 x3 = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0);
        s32 y0 = 0;
        s32 y1 = dh;
        s32 y2 = SCREEN_HEIGHT - dh;
        s32 y3 = SCREEN_HEIGHT;

        // Vertices
        vtx[0]  = omm_render_effect_vibe_get_vertex(x0, y0, 0xA0 * alpha, timer);
        vtx[1]  = omm_render_effect_vibe_get_vertex(x1, y0, 0xA0 * alpha, timer);
        vtx[2]  = omm_render_effect_vibe_get_vertex(x2, y0, 0xA0 * alpha, timer);
        vtx[3]  = omm_render_effect_vibe_get_vertex(x3, y0, 0xA0 * alpha, timer);
        vtx[4]  = omm_render_effect_vibe_get_vertex(x0, y1, 0xA0 * alpha, timer);
        vtx[5]  = omm_render_effect_vibe_get_vertex(x1, y1, 0x00 * alpha, timer);
        vtx[6]  = omm_render_effect_vibe_get_vertex(x2, y1, 0x00 * alpha, timer);
        vtx[7]  = omm_render_effect_vibe_get_vertex(x3, y1, 0xA0 * alpha, timer);
        vtx[8]  = omm_render_effect_vibe_get_vertex(x0, y2, 0xA0 * alpha, timer);
        vtx[9]  = omm_render_effect_vibe_get_vertex(x1, y2, 0x00 * alpha, timer);
        vtx[10] = omm_render_effect_vibe_get_vertex(x2, y2, 0x00 * alpha, timer);
        vtx[11] = omm_render_effect_vibe_get_vertex(x3, y2, 0xA0 * alpha, timer);
        vtx[12] = omm_render_effect_vibe_get_vertex(x0, y3, 0xA0 * alpha, timer);
        vtx[13] = omm_render_effect_vibe_get_vertex(x1, y3, 0xA0 * alpha, timer);
        vtx[14] = omm_render_effect_vibe_get_vertex(x2, y3, 0xA0 * alpha, timer);
        vtx[15] = omm_render_effect_vibe_get_vertex(x3, y3, 0xA0 * alpha, timer);

        // Display list
        OMM_RENDER_ENABLE_ALPHA(pos++);
        gSPDisplayList(pos++, gfx);
        gSPClearGeometryMode(gfx++, G_LIGHTING | G_CULL_BOTH);
        gDPSetCombineLERP(gfx++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0);
        gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(gfx++, OMM_VIBE_TEXTURES[sOmmVibeType][1], G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gfx++, vtx, 16, 0);
        gSP2Triangles(gfx++, 0, 1, 5, 0, 0, 4, 5, 0);
        gSP2Triangles(gfx++, 2, 3, 6, 0, 3, 6, 7, 0);
        gSP2Triangles(gfx++, 8, 9, 12, 0, 9, 12, 13, 0);
        gSP2Triangles(gfx++, 10, 11, 15, 0, 10, 14, 15, 0);
        gSP2Triangles(gfx++, 4, 5, 9, 0, 4, 8, 9, 0);
        gSP2Triangles(gfx++, 6, 7, 11, 0, 6, 10, 11, 0);
        gSP2Triangles(gfx++, 1, 2, 5, 0, 2, 5, 6, 0);
        gSP2Triangles(gfx++, 9, 10, 13, 0, 10, 13, 14, 0);
        gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineLERP(gfx++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        gSPSetGeometryMode(gfx++, G_LIGHTING | G_CULL_BACK);
        gSPEndDisplayList(gfx);
    }
    return pos;
}

static Gfx *omm_render_effect_vibe_heart(Gfx *pos, f32 alpha, f32 size) {
    if (alpha > 0.f && size > 0.f) {
        f32 x = (SCREEN_WIDTH - size) / 2.f;
        f32 y = (SCREEN_HEIGHT - size) / 2.f;
        f32 w = max_f(size, 1);
        f32 h = max_f(size, 1);
        s16 x0 = (s16) (x * 4.f);
        s16 y0 = (s16) ((SCREEN_HEIGHT - h - y) * 4.f);
        s16 x1 = (s16) ((x + w) * 4.f);
        s16 y1 = (s16) ((SCREEN_HEIGHT - y) * 4.f);
        gDPSetRenderMode(pos++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
        gDPSetCombineLERP(pos++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
        gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, (255.9f * alpha));
        gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(pos++, OMM_VIBE_TEXTURES[gOmmPeach->vibeType][0], G_IM_FMT_RGBA, G_IM_SIZ_32b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
        gSPTextureRectangle(pos++, x0, y0, x1, y1, G_TX_RENDERTILE, 0, 0, (s16) (0x4000 / w), (s16) (0x4000 / h));
        gDPSetRenderMode(pos++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        gDPSetCombineLERP(pos++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
        gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    }
    return pos;
}

void omm_render_effect_vibe() {
    if (HUD_DISPLAY_EFFECTS && !OMM_HUD_NONE && OMM_PLAYER_IS_PEACH) {
        u32 timer = gGlobalTimer - gOmmPeach->vibeGfxTimer;

        // Update vibe values
        sOmmVibeAlpha = 1.f - clamp_0_1_f(timer / 15.f);
        if (gOmmPeach->vibeType != OMM_PEACH_VIBE_TYPE_NONE) {
            sOmmVibeType = gOmmPeach->vibeType;
            sOmmVibeBgAlpha = 1.f - sOmmVibeAlpha;
        } else {
            sOmmVibeBgAlpha = sOmmVibeAlpha;
        }

        // Vibe background
        Gfx *vibeBgGfx = omm_alloc_gfx(32);
        Vtx *vibeBgVtx = omm_alloc_vtx(16);
        omm_render_create_dl_ortho_matrix();
        interp_data_update(sOmmVibeBg, timer > 0, gDisplayListHead, vibeBgGfx, vibeBgVtx, 0, 0, 0, sOmmVibeBgAlpha, 0, gGlobalTimer);
        gDisplayListHead = omm_render_effect_vibe_background(gDisplayListHead, sOmmVibeBg->gfx, sOmmVibeBg->vtx, sOmmVibeBg->a0, sOmmVibeBg->t0);

        // Vibe heart
        f32 heartSize = lerp_f(1.f - sOmmVibeAlpha, 1, SCREEN_HEIGHT);
        omm_render_create_dl_ortho_matrix();
        interp_data_update(sOmmVibeHeart, timer > 0, gDisplayListHead, NULL, NULL, 0, 0, 0, sOmmVibeAlpha / (OMM_HUD_PRO ? 2.f : 1.f), heartSize, 0);
        gDisplayListHead = omm_render_effect_vibe_heart(gDisplayListHead, sOmmVibeHeart->a0, sOmmVibeHeart->s0);
        return;
    }
    interp_data_reset(sOmmVibeBg);
    interp_data_reset(sOmmVibeHeart);
}

//
// You got a star!
//

void omm_render_effect_you_got_a_star_begin(const char *title, const u8 *courseName, const u8 *starName) {
    const u8 *converted = omm_text_convert(title, false);
    mem_cpy(sOmmYouGotAStarText[0], converted, omm_text_length(converted) + 1);
    mem_cpy(sOmmYouGotAStarText[1], courseName, omm_text_length(courseName) + 1);
    mem_cpy(sOmmYouGotAStarText[2], starName, omm_text_length(starName) + 1);
    sOmmYouGotAStarTimer = 5;
    sOmmYouGotAStarRender = 1;
}

void omm_render_effect_you_got_a_star_end() {
    sOmmYouGotAStarTimer = 61;
    sOmmYouGotAStarRender = 0;
}

void omm_render_effect_you_got_a_star_stop() {
    sOmmYouGotAStarTimer = 0;
    sOmmYouGotAStarRender = 0;
}

static Gfx *omm_render_effect_you_got_a_star_pop_up(Gfx *pos, f32 timer, f32 offset) {
    OMM_RENDER_BACKUP_DL_HEAD(pos);
    u8 alpha = (u8) clamp_f(255.9f * timer, 0, 255);

    // Background
    static const Vtx sOmmYouGotAStarBackgroundVtx[4] = {
        { { { (SCREEN_WIDTH - OMM_RENDER_YOU_GOT_A_STAR_W) / 2, OMM_RENDER_YOU_GOT_A_STAR_Y + OMM_RENDER_YOU_GOT_A_STAR_H, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x00 } } },
        { { { (SCREEN_WIDTH + OMM_RENDER_YOU_GOT_A_STAR_W) / 2, OMM_RENDER_YOU_GOT_A_STAR_Y + OMM_RENDER_YOU_GOT_A_STAR_H, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x00 } } },
        { { { (SCREEN_WIDTH - OMM_RENDER_YOU_GOT_A_STAR_W) / 2, OMM_RENDER_YOU_GOT_A_STAR_Y,                               0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x80 } } },
        { { { (SCREEN_WIDTH + OMM_RENDER_YOU_GOT_A_STAR_W) / 2, OMM_RENDER_YOU_GOT_A_STAR_Y,                               0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0x80 } } },
    };
    omm_render_create_dl_ortho_matrix();
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, alpha);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, sOmmYouGotAStarBackgroundVtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 2, 1, 0x0, 2, 3, 1, 0x0);

    // You got a star
    omm_render_string_hud_centered(OMM_RENDER_YOU_GOT_A_STAR_LINE_1_Y + offset, 0xFF, 0xFF, 0xFF, alpha, sOmmYouGotAStarText[0], false);
    omm_render_string_centered    (OMM_RENDER_YOU_GOT_A_STAR_LINE_2_Y + offset, 0xFF, 0xFF, 0xFF, alpha, sOmmYouGotAStarText[1], true);
    omm_render_string_centered    (OMM_RENDER_YOU_GOT_A_STAR_LINE_3_Y + offset, 0xFF, 0xFF, 0xFF, alpha, sOmmYouGotAStarText[2], true);
    OMM_RENDER_RESTORE_DL_HEAD(pos);
    return pos;
}

void omm_render_effect_you_got_a_star() {
    sOmmYouGotAStarTimer = max_s(sOmmYouGotAStarTimer - 1, 0);
    if (HUD_DISPLAY_EFFECTS && !OMM_HUD_PRO && !OMM_HUD_NONE && (sOmmYouGotAStarTimer != 0 || sOmmYouGotAStarRender)) {
        f32 timer, offset;
        if (sOmmYouGotAStarRender) { // Fade-in
            timer = (f32) (4 - sOmmYouGotAStarTimer) / 4.f;
            offset = 2.f * (2 - abs_s(sOmmYouGotAStarTimer - 2));
        } else { // Fade-out
            timer = min_f(1.f, sOmmYouGotAStarTimer / 15.f);
            offset = 0;
        }
        interp_data_update(sOmmYouGotAStar, true, gDisplayListHead, NULL, NULL, 0, offset, 0, 0, 0, timer);
        gDisplayListHead = omm_render_effect_you_got_a_star_pop_up(gDisplayListHead, sOmmYouGotAStar->t0, sOmmYouGotAStar->y0);
        return;
    }
    interp_data_reset(sOmmYouGotAStar);
}

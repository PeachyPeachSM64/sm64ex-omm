#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Buffers
//

static Vtx sFreezeVtx[4];
static Vtx sDarkModeVtx[OMM_RENDER_DARK_MODE_NUM_POINTS * 6];
static Gfx sDarkModeGfx[OMM_RENDER_DARK_MODE_NUM_POINTS * 3 + 16];
static s32 sVibeType = OMM_PEACH_VIBE_TYPE_NONE;
static f32 sVibeAlpha = 0.f;
static f32 sVibeBgAlpha = 0.f;
static Vtx sVibeBgVtx[16];
static Gfx sVibeBgGfx[32];
static u8  sYouGotAStarText[3][64];
static s32 sYouGotAStarTimer = 0;
static s32 sYouGotAStarRender = 0;

//
// Frame interpolation
//

static struct {
    Gfx *pos;
    f32 alpha, _alpha;
    u32 timer, _timer;
} sVibeBg[MAX_INTERPOLATED_FRAMES];

static struct {
    Gfx *pos;
    f32 alpha, _alpha;
    f32 size, _size;
} sVibeHeart[MAX_INTERPOLATED_FRAMES];

static struct {
    Gfx *pos;
    f32 t, _t;
    f32 dy, _dy;
} sYouGotAStar[MAX_INTERPOLATED_FRAMES];

static void omm_render_effect_vibe_background(Gfx **pos, f32 alpha, u32 timer);
static void omm_render_effect_vibe_heart(Gfx **pos, f32 alpha, f32 size);
static void omm_render_effect_you_got_a_star_at(Gfx *pos, f32 t, f32 dy);

void gfx_patch_interpolated_frame_effects(s32 k) {

    // Vibe background
    if (sVibeBg[k].pos) {
        omm_render_effect_vibe_background(&sVibeBg[k].pos, sVibeBg[k].alpha, sVibeBg[k].timer);
        sVibeBg[k].pos = NULL;
    }

    // Vibe heart
    if (sVibeHeart[k].pos) {
        omm_render_effect_vibe_heart(&sVibeHeart[k].pos, sVibeHeart[k].alpha, sVibeHeart[k].size);
        sVibeHeart[k].pos = NULL;
    }

    // You got a star
    if (sYouGotAStar[k].pos) {
        omm_render_effect_you_got_a_star_at(sYouGotAStar[k].pos, sYouGotAStar[k].t, sYouGotAStar[k].dy);
        sYouGotAStar[k].pos = NULL;
    }
}

//
// Freezing water
//

void omm_render_effect_freeze() {
    u8 alpha = (u8) (128.9f * clamp_f((f32) gOmmData->mario->state.freeze.gfx / (f32) OMM_FREEZING_WATER_TIMER_CRITICAL, 0.f, 1.f));
    if (alpha > 0) {
        if (alpha >= 128) {
            alpha += 32 * (gGlobalTimer & 1);
        }

        f32 x0 = GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0);
        f32 x1 = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0);
        f32 y0 = 0.f;
        f32 y1 = SCREEN_HEIGHT;
        sFreezeVtx[0] = (Vtx) { { { x0, y0, 0 }, 0, { 0x0000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
        sFreezeVtx[1] = (Vtx) { { { x1, y0, 0 }, 0, { 0x4000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
        sFreezeVtx[2] = (Vtx) { { { x1, y1, 0 }, 0, { 0x4000, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
        sFreezeVtx[3] = (Vtx) { { { x0, y1, 0 }, 0, { 0x0000, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };

        omm_render_create_dl_ortho_matrix();
        OMM_RENDER_ENABLE_ALPHA(gDisplayListHead++);
        gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, alpha);
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(gDisplayListHead++, OMM_TEXTURE_EFFECT_FREEZE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 128, 0, G_TX_MIRROR, G_TX_MIRROR, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, sFreezeVtx, 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    }
}

//
// Dark mode
//

void omm_render_effect_dark_mode() {
    if (omm_ssc_data_flags(OMM_SSD_DARK_MODE) && OMM_SSC_IS_OK) {
        Vtx *vtx = sDarkModeVtx;
        Gfx *gfx = sDarkModeGfx;
        omm_render_create_dl_ortho_matrix();
        OMM_RENDER_ENABLE_ALPHA(gDisplayListHead++);
        gSPDisplayList(gDisplayListHead++, sDarkModeGfx);
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

static const char *sVibeTextures[5][2] = {
    { OMM_TEXTURE_HUD_VIBE_NORMAL, NULL },
    { OMM_TEXTURE_HUD_VIBE_JOY, OMM_TEXTURE_EFFECT_VIBE_JOY },
    { OMM_TEXTURE_HUD_VIBE_RAGE, OMM_TEXTURE_EFFECT_VIBE_RAGE },
    { OMM_TEXTURE_HUD_VIBE_GLOOM, OMM_TEXTURE_EFFECT_VIBE_GLOOM },
    { OMM_TEXTURE_HUD_VIBE_CALM, OMM_TEXTURE_EFFECT_VIBE_CALM },
};

static Vtx omm_render_effect_vibe_get_vertex(f32 x, f32 y, u8 alpha, u32 timer) {
    Vtx v;
    v.v.ob[0] = x;
    v.v.ob[1] = y;
    v.v.ob[2] = 0.f;
    v.v.tc[0] = (invlerp_f(x, GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0)) * 256 - ((timer & 0x1FF) / 2.f)) * 32;
    v.v.tc[1] = (invlerp_f(y, 0.f, SCREEN_HEIGHT) * 256) * 32;
    v.v.cn[0] = 0xFF;
    v.v.cn[1] = 0xFF;
    v.v.cn[2] = 0xFF;
    v.v.cn[3] = alpha;
    v.v.flag  = 0;
    return v;
}

static void omm_render_effect_vibe_background(Gfx **pos, f32 alpha, u32 timer) {
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
        sVibeBgVtx[0]  = omm_render_effect_vibe_get_vertex(x0, y0, 0xA0 * alpha, timer);
        sVibeBgVtx[1]  = omm_render_effect_vibe_get_vertex(x1, y0, 0xA0 * alpha, timer);
        sVibeBgVtx[2]  = omm_render_effect_vibe_get_vertex(x2, y0, 0xA0 * alpha, timer);
        sVibeBgVtx[3]  = omm_render_effect_vibe_get_vertex(x3, y0, 0xA0 * alpha, timer);
        sVibeBgVtx[4]  = omm_render_effect_vibe_get_vertex(x0, y1, 0xA0 * alpha, timer);
        sVibeBgVtx[5]  = omm_render_effect_vibe_get_vertex(x1, y1, 0x00 * alpha, timer);
        sVibeBgVtx[6]  = omm_render_effect_vibe_get_vertex(x2, y1, 0x00 * alpha, timer);
        sVibeBgVtx[7]  = omm_render_effect_vibe_get_vertex(x3, y1, 0xA0 * alpha, timer);
        sVibeBgVtx[8]  = omm_render_effect_vibe_get_vertex(x0, y2, 0xA0 * alpha, timer);
        sVibeBgVtx[9]  = omm_render_effect_vibe_get_vertex(x1, y2, 0x00 * alpha, timer);
        sVibeBgVtx[10] = omm_render_effect_vibe_get_vertex(x2, y2, 0x00 * alpha, timer);
        sVibeBgVtx[11] = omm_render_effect_vibe_get_vertex(x3, y2, 0xA0 * alpha, timer);
        sVibeBgVtx[12] = omm_render_effect_vibe_get_vertex(x0, y3, 0xA0 * alpha, timer);
        sVibeBgVtx[13] = omm_render_effect_vibe_get_vertex(x1, y3, 0xA0 * alpha, timer);
        sVibeBgVtx[14] = omm_render_effect_vibe_get_vertex(x2, y3, 0xA0 * alpha, timer);
        sVibeBgVtx[15] = omm_render_effect_vibe_get_vertex(x3, y3, 0xA0 * alpha, timer);

        // Display list
        Gfx *gfx = sVibeBgGfx;
        OMM_RENDER_ENABLE_ALPHA((*pos)++);
        gSPDisplayList((*pos)++, sVibeBgGfx);
        gSPClearGeometryMode(gfx++, G_LIGHTING | G_CULL_BOTH);
        gDPSetCombineLERP(gfx++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0);
        gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(gfx++, sVibeTextures[sVibeType][1], G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gfx++, sVibeBgVtx, 16, 0);
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
}

static void omm_render_effect_vibe_heart(Gfx **pos, f32 alpha, f32 size) {
    s16 x = (SCREEN_WIDTH - size) / 2.f;
    s16 y = (SCREEN_HEIGHT - size) / 2.f;
    s16 w = (s16) max_f(size, 1);
    s16 h = (s16) max_f(size, 1);
    gDPSetTexturePersp((*pos)++, G_TP_NONE);
    gDPSetRenderMode((*pos)++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetCombineLERP((*pos)++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    gDPSetEnvColor((*pos)++, 0xFF, 0xFF, 0xFF, (255.9f * alpha));
    gDPSetTextureFilter((*pos)++, G_TF_POINT);
    gSPTexture((*pos)++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock((*pos)++, sVibeTextures[gOmmData->mario->peach.vibeType][0], G_IM_FMT_RGBA, G_IM_SIZ_32b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gSPTextureRectangle((*pos)++, (x) << 2, (SCREEN_HEIGHT - h - y) << 2, (x + w) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, (0x4000 / w), (0x4000 / h));
    gDPSetTexturePersp((*pos)++, G_TP_PERSP);
    gDPSetRenderMode((*pos)++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineLERP((*pos)++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetEnvColor((*pos)++, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetTextureFilter((*pos)++, G_TF_BILERP);
    gSPTexture((*pos)++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
}

void omm_render_effect_vibe() {
    if (OMM_PLAYER_IS_PEACH) {
        bool shouldInterpolate = is_frame_interpolation_enabled() && (gOmmData->mario->peach.vibeTimer > 0);
        
        // Update vibe values
        sVibeAlpha = 1.f - clamp_0_1_f(gOmmData->mario->peach.vibeTimer / 15.f);
        if (gOmmData->mario->peach.vibeType != OMM_PEACH_VIBE_TYPE_NONE) {
            sVibeType = gOmmData->mario->peach.vibeType;
            sVibeBgAlpha = 1.f - sVibeAlpha;
        } else {
            sVibeBgAlpha = sVibeAlpha;
        }

        // Vibe background
        omm_render_create_dl_ortho_matrix();
        interpolate {
            sVibeBg[k].pos = gDisplayListHead;
            interpolate_f32(sVibeBg[k].alpha, sVibeBg[k]._alpha, sVibeBgAlpha);
            interpolate_u32(sVibeBg[k].timer, sVibeBg[k]._timer, gGlobalTimer);
            sVibeBg[k]._alpha = sVibeBgAlpha;
            sVibeBg[k]._timer = gGlobalTimer;
        }
        omm_render_effect_vibe_background(&gDisplayListHead, sVibeBg[0].alpha, sVibeBg[0].timer);

        // Vibe heart
        if (sVibeAlpha > 0.f) {
            f32 size = lerp_f(1.f - sVibeAlpha, 1, SCREEN_HEIGHT);
            omm_render_create_dl_ortho_matrix();
            interpolate {
                sVibeHeart[k].pos = gDisplayListHead;
                interpolate_f32(sVibeHeart[k].alpha, sVibeHeart[k]._alpha, sVibeAlpha);
                interpolate_f32(sVibeHeart[k].size, sVibeHeart[k]._size, size);
                sVibeHeart[k]._alpha = sVibeAlpha;
                sVibeHeart[k]._size = size;
            }
            omm_render_effect_vibe_heart(&gDisplayListHead, sVibeHeart[0].alpha, sVibeHeart[0].size);
        }
    }
}

//
// You got a star!
//

void omm_render_effect_you_got_a_star_begin(const char *title, const u8 *courseName, const u8 *starName) {
    const u8 *converted = omm_text_convert(title, false);
    OMM_MEMCPY(sYouGotAStarText[0], converted, omm_text_length(converted) + 1);
    OMM_MEMCPY(sYouGotAStarText[1], courseName, omm_text_length(courseName) + 1);
    OMM_MEMCPY(sYouGotAStarText[2], starName, omm_text_length(starName) + 1);
    sYouGotAStarTimer = 4;
    sYouGotAStarRender = 1;
}

void omm_render_effect_you_got_a_star_end() {
    sYouGotAStarTimer = 60;
    sYouGotAStarRender = 0;
}

static void omm_render_effect_you_got_a_star_at(Gfx *pos, f32 t, f32 dy) {
    gDisplayListHead = pos;
    u8 alpha = (u8) (255.9f * t);

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
    omm_render_string_hud_centered(OMM_RENDER_YOU_GOT_A_STAR_LINE_1_Y + dy, 0xFF, 0xFF, 0xFF, alpha, sYouGotAStarText[0], false);
    omm_render_string_centered    (OMM_RENDER_YOU_GOT_A_STAR_LINE_2_Y + dy, 0xFF, 0xFF, 0xFF, alpha, sYouGotAStarText[1], true);
    omm_render_string_centered    (OMM_RENDER_YOU_GOT_A_STAR_LINE_3_Y + dy, 0xFF, 0xFF, 0xFF, alpha, sYouGotAStarText[2], true);
}

void omm_render_effect_you_got_a_star() {
    if (sYouGotAStarTimer != 0 || sYouGotAStarRender) {
        f32 t, dy;
        if (sYouGotAStarRender) { // Fade-in
            t = (f32) (4 - sYouGotAStarTimer) / 4.f;
            dy = 2.f * (2 - abs_s(sYouGotAStarTimer - 2));
        } else { // Fade-out
            t = min_f(1.f, sYouGotAStarTimer / 15.f);
            dy = 0;
        }
        bool shouldInterpolate = is_frame_interpolation_enabled();
        interpolate {
            sYouGotAStar[k].pos = gDisplayListHead;
            interpolate_f32(sYouGotAStar[k].t, sYouGotAStar[k]._t, t);
            interpolate_f32(sYouGotAStar[k].dy, sYouGotAStar[k]._dy, dy);
            sYouGotAStar[k]._t = t;
            sYouGotAStar[k]._dy = dy;
        }
        omm_render_effect_you_got_a_star_at(gDisplayListHead, sYouGotAStar[0].t, sYouGotAStar[0].dy);
    }
    if (sYouGotAStarTimer != 0) {
        sYouGotAStarTimer--;
    }
}

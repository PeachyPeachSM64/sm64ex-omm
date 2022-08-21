#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define REFRESH_RATE 15
#define GFX_PROFILER 0 // Set to 1 if needed

static struct {
    s64 curr;
    s64 sum;
    f64 disp;
} sPrf[OMM_PRF_MAX];
static s64 sFrameTimer = 0;
static s64 sFrameCounter = 0;
static s64 sFrameCounterDisp = 0;

#if OMM_CODE_DEBUG && GFX_PROFILER
static struct {
    s64 curr;
    s64 sum;
    s64 count;
    f64 disp;
} sGfx[0x40];
#endif

void omm_profiler_start(s32 prf) {
#if OMM_CODE_DEBUG
    sPrf[prf].curr = SDL_GetPerformanceCounter();
#endif
}

void omm_profiler_stop(s32 prf) {
#if OMM_CODE_DEBUG
    sPrf[prf].sum += SDL_GetPerformanceCounter() - sPrf[prf].curr;
#endif
}

void omm_profiler_start_gfx_op(u8 op) {
#if OMM_CODE_DEBUG && GFX_PROFILER
    sGfx[op].curr = SDL_GetPerformanceCounter();
#endif
}

void omm_profiler_stop_gfx_op(u8 op) {
#if OMM_CODE_DEBUG && GFX_PROFILER
    sGfx[op].sum += SDL_GetPerformanceCounter() - sGfx[op].curr;
    sGfx[op].count++;
#endif
}

void omm_profiler_frame_drawn() {
    sFrameCounter++;
}

OMM_ROUTINE_UPDATE(omm_profiler_update) {
    s64 curr = SDL_GetPerformanceCounter();
    sPrf[OMM_PRF_FPS].sum += curr - sPrf[OMM_PRF_FPS].curr;
    sPrf[OMM_PRF_FPS].curr = curr;
    if (sFrameTimer++ % REFRESH_RATE == 0) {
        f64 freq = SDL_GetPerformanceFrequency();
        for (s32 i = 0; i != OMM_PRF_MAX; ++i) {
            sPrf[i].disp = (f64) sPrf[i].sum / (freq * REFRESH_RATE);
            sPrf[i].sum = 0;
        }
        sFrameCounterDisp = sFrameCounter;
        sFrameCounter = 0;
#if OMM_CODE_DEBUG && GFX_PROFILER
        if (gOmmShowFPS) {
            static struct { u8 op; const char *name; } sGfxOps[] = {
                { 0x01, "gfx_sp_vertex" },
                { 0x05, "gfx_sp_tri1" },
                { 0x06, "gfx_sp_tri2" },
                { 0x17, "gfx_sp_texture" },
                { 0x18, "gfx_sp_pop_matrix" },
                { 0x19, "gfx_sp_geometry_mode" },
                { 0x1A, "gfx_sp_matrix" },
                { 0x1B, "gfx_sp_moveword" },
                { 0x1C, "gfx_sp_movemem" },
                { 0x22, "gfx_sp_set_other_mode_l" },
                { 0x23, "gfx_sp_set_other_mode_h" },
                { 0x24, "gfx_dp_texture_rectangle" },
                { 0x25, "gfx_dp_texture_rectangle" },
                { 0x2D, "gfx_dp_set_scissor" },
                { 0x31, "gfx_xp_swap_cmd" },
                { 0x32, "gfx_dp_set_tile_size" },
                { 0x33, "gfx_dp_load_block" },
                { 0x34, "gfx_dp_load_tile" },
                { 0x35, "gfx_dp_set_tile" },
                { 0x36, "gfx_dp_fill_rectangle" },
                { 0x37, "gfx_dp_set_fill_color" },
                { 0x38, "gfx_dp_set_fog_color" },
                { 0x3A, "gfx_dp_set_prim_color" },
                { 0x3B, "gfx_dp_set_env_color" },
                { 0x3C, "gfx_dp_set_combine_mode" },
                { 0x3D, "gfx_dp_set_texture_image" },
                { 0x3E, "gfx_dp_set_z_image" },
                { 0x3F, "gfx_dp_set_color_image" },
            };
            omm_printf("Command ----------------------- Elapsed ------- Calls ----- Avg --\n");
            for (s32 i = 0; i != omm_static_array_length(sGfxOps); ++i) {
                u8 op = sGfxOps[i].op;
                f64 elapsed_us = sGfx[op].sum * 1000000.0 / (REFRESH_RATE * freq);
                f64 elapsed_us_per_call = elapsed_us * 1000000.0 / max(1, sGfx[op].count);
                omm_printf("%-32s %6d us   %8d    %6d ps\n",, sGfxOps[i].name, (s32) elapsed_us, (s32) sGfx[op].count, (s32) elapsed_us_per_call);
                sGfx[op].sum = 0;
                sGfx[op].count = 0;
            }
        }
#endif
    }
}

void omm_profiler_display() {
    if (gOmmShowFPS) {
        static Vtx sPrfVtx[0x10000];
        static char sPrfText[OMM_PRF_MAX][32];
        omm_zero(sPrfText, sizeof(sPrfText));
#if OMM_CODE_DEBUG
        snprintf(sPrfText[OMM_PRF_OMM], 32, "$OMM %06d$", (s32) (1000000.0 * sPrf[OMM_PRF_OMM].disp));
        snprintf(sPrfText[OMM_PRF_LVL], 32, "$LVL %06d$", (s32) (1000000.0 * sPrf[OMM_PRF_LVL].disp));
        snprintf(sPrfText[OMM_PRF_PRE], 32, "$PRE %06d$", (s32) (1000000.0 * sPrf[OMM_PRF_PRE].disp));
        snprintf(sPrfText[OMM_PRF_GEO], 32, "$GEO %06d$", (s32) (1000000.0 * sPrf[OMM_PRF_GEO].disp));
        snprintf(sPrfText[OMM_PRF_GFX], 32, "$GFX %06d$", (s32) (1000000.0 * sPrf[OMM_PRF_GFX].disp));
        snprintf(sPrfText[OMM_PRF_RDR], 32, "$RDR %06d$", (s32) (1000000.0 * sPrf[OMM_PRF_RDR].disp));
        snprintf(sPrfText[OMM_PRF_FRM], 32, "$FRM %06d$", (s32) (1000000.0 * sPrf[OMM_PRF_FRM].disp));
#endif
        snprintf(sPrfText[OMM_PRF_FPS], 32, "$FPS %06.2f$", (f32) sFrameCounterDisp / (sPrf[OMM_PRF_FPS].disp * REFRESH_RATE));
        omm_render_create_dl_ortho_matrix();
        OMM_RENDER_ENABLE_ALPHA(gDisplayListHead++);
        gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xE0);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(gDisplayListHead++, OMM_TEXTURE_MENU_FONT, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1024, 1024, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        for (s32 j = 0, k = 0, x0 = GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(2), x = x0, y = 2 + 8 * (OMM_PRF_MAX - 1); k != OMM_PRF_MAX; ++k, x = x0, y -= 8) {
            for (char *c = sPrfText[k], d = 0, e = 0; *c; ++c, d += (*c >= '1' && *c <= '9'), e += (*c == ' '), j += 4) {
                if (*c >= ' ' && *c <= '~') {
                    s32 u = ((((*c - 0x20) % 20) * 48 + 0) * 32);
                    s32 v = ((((*c - 0x20) / 20) * 80 - 8) * 32);
                    s32 w = (*c == '$' ? 2 : 5);
                    s32 h = 8;
                    s32 i = (*c == '0' && !d ? 0x10 : (e ? 0xFF : 0xAA));
                    sPrfVtx[j + 0] = (Vtx) { { { x ,    y,     0 }, 0, { u,        v + 2560 }, { i, i, i, 0xFF } } };
                    sPrfVtx[j + 1] = (Vtx) { { { x + w, y,     0 }, 0, { u + 1536, v + 2560 }, { i, i, i, 0xFF } } };
                    sPrfVtx[j + 2] = (Vtx) { { { x + w, y + h, 0 }, 0, { u + 1536, v        }, { i, i, i, 0xFF } } };
                    sPrfVtx[j + 3] = (Vtx) { { { x,     y + h, 0 }, 0, { u,        v        }, { i, i, i, 0xFF } } };
                    gSPVertex(gDisplayListHead++, sPrfVtx + j, 4, 0);
                    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
                    x += w;
                }
            }
        }
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
    }
}

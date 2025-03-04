#ifdef GFX_PC_C
#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

extern Lights1 omm_tiara_eye_top_light;
extern Lights1 omm_tiara_eye_bottom_light;
static OmmHMap_(Lights1 *) sOmmTiaraEyeTexLightTop = omm_hmap_zero;
static OmmHMap_(Lights1 *) sOmmTiaraEyeTexLightBottom = omm_hmap_zero;
static OmmHMap_(PxMtx *) sOmmTiaraEyeTexPixelMatrix = omm_hmap_zero;
typedef struct { bool *px; f32 xc, yc, dx, dy; } PxMtx;

static void omm_peach_tiara_gfx_set_tiara_tex_palette(GfxTexture *tex) {
    s32 i = omm_hmap_find(sOmmTiaraEyeTexLightTop, tex->hash);
    Lights1 *l1 = NULL;
    Lights1 *l2 = NULL;
    PxMtx   *px = NULL;
    if (i != -1) {
        l1 = omm_hmap_get(sOmmTiaraEyeTexLightTop, Lights1 *, i);
        l2 = omm_hmap_get(sOmmTiaraEyeTexLightBottom, Lights1 *, i);
        px = omm_hmap_get(sOmmTiaraEyeTexPixelMatrix, PxMtx *, i);
    } else {
        l1 = mem_new(Lights1, 1);
        l2 = mem_new(Lights1, 1);
        px = mem_new(PxMtx, 1);
        px->px = mem_new(bool, tex->w * tex->h);
        u32 xmin = tex->w;
        u32 xmax = 0;
        u32 ymin = tex->h;
        u32 ymax = 0;
        for (u32 i = 0, j = 0; i != tex->w * tex->h * 4; i += 4, ++j) {
            u32 x = j % tex->w;
            u32 y = j / tex->w;
            if (x < tex->w / 2) {
                u8 r = tex->data[i + 0];
                u8 g = tex->data[i + 1];
                u8 b = tex->data[i + 2];
                f32 d = sqrtf((sqr_s(r - 0xFF) + sqr_s(g - 0x20) + sqr_s(b - 0xC0)) / 3.f) / 255.f;
                if (d < 0.25f) {
                    px->px[j] = true;
                    xmin = min_s(xmin, x);
                    xmax = max_s(xmax, x);
                    ymin = min_s(ymin, y);
                    ymax = max_s(ymax, y);
                }
            }
        }
        px->xc = (xmax + xmin) / 2.f;
        px->yc = (ymax + ymin) / 2.f;
        px->dx = (xmax - xmin) / 2.f;
        px->dy = (ymax - ymin) / 2.f;
        omm_hmap_insert(sOmmTiaraEyeTexLightTop, tex->hash, l1);
        omm_hmap_insert(sOmmTiaraEyeTexLightBottom, tex->hash, l2);
        omm_hmap_insert(sOmmTiaraEyeTexPixelMatrix, tex->hash, px);
    }
    if (!mem_eq(l1, &omm_tiara_eye_top_light, sizeof(Lights1)) || !mem_eq(l2, &omm_tiara_eye_bottom_light, sizeof(Lights1))) {
        u32 size = tex->w * tex->h * 4;
        u8 *data = mem_dup(tex->data, size);
        for (u32 i = 0, j = 0; i != size; i += 4, ++j) {
            if (px->px[j]) {
                u32 x = j % tex->w;
                u32 y = j / tex->w;
                f32 dx = (x - px->xc) / px->dx;
                f32 dy = (y - px->yc) / px->dy;
                f32 dc = relerp_0_1_f(sqrtf(dx * dx + dy * dy), 0.933f, 0.966f, 1.f, 0.8f);
                data[i + 0] = (u8) (relerp_0_1_f(dy, -1, 1, omm_tiara_eye_top_light.l->l.col[0], omm_tiara_eye_bottom_light.l->l.col[0]) * dc);
                data[i + 1] = (u8) (relerp_0_1_f(dy, -1, 1, omm_tiara_eye_top_light.l->l.col[1], omm_tiara_eye_bottom_light.l->l.col[1]) * dc);
                data[i + 2] = (u8) (relerp_0_1_f(dy, -1, 1, omm_tiara_eye_top_light.l->l.col[2], omm_tiara_eye_bottom_light.l->l.col[2]) * dc);
            }
        }
        sGfxRapi->upload_texture(data, tex->w, tex->h);
        mem_del(data);
        mem_cpy(l1, &omm_tiara_eye_top_light, sizeof(Lights1));
        mem_cpy(l2, &omm_tiara_eye_bottom_light, sizeof(Lights1));
    }
}

//
// Custom commands
//

static void omm_peach_tiara_gfx_sp_tri1() {
    gfx_sp_tri1();
    if (!sGfxProc->texChanged[0]) { // Make sure the texture is loaded
        omm_peach_tiara_gfx_set_tiara_tex_palette(sGfxProc->textures[0]);
    }
}

static void omm_peach_tiara_gfx_sp_tri2() {
    gfx_sp_tri2();
    if (!sGfxProc->texChanged[0]) { // Make sure the texture is loaded
        omm_peach_tiara_gfx_set_tiara_tex_palette(sGfxProc->textures[0]);
    }
}

static void omm_peach_tiara_gfx_sp_tri_ext() {
    gfx_sp_tri_ext();
    if (!sGfxProc->texChanged[0]) { // Make sure the texture is loaded
        omm_peach_tiara_gfx_set_tiara_tex_palette(sGfxProc->textures[0]);
    }
}

//
// Display lists
//

const Gfx omm_peach_tiara_gfx_enable[] = {
    gsXPSwapCmd(G_TRI1, omm_peach_tiara_gfx_sp_tri1),
    gsXPSwapCmd(G_TRI2, omm_peach_tiara_gfx_sp_tri2),
    gsXPSwapCmd(G_TRIEXT, omm_peach_tiara_gfx_sp_tri_ext),
    gsSPEndDisplayList(),
};

const Gfx omm_peach_tiara_gfx_disable[] = {
    gsXPSwapCmd(G_TRI1, NULL),
    gsXPSwapCmd(G_TRI2, NULL),
    gsXPSwapCmd(G_TRIEXT, NULL),
    gsSPEndDisplayList(),
};

#endif
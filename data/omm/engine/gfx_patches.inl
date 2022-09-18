#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

//
// Patch the dialog font height and rendering
//

static const Vtx vtx_ia8_char[] = {
    { { { 0,  0, 0 }, 0, { 0x000, 0x100 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 8,  0, 0 }, 0, { 0x000, 0x000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 8, 16, 0 }, 0, { 0x200, 0x000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 16, 0 }, 0, { 0x200, 0x100 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

static const Gfx gfx_ia8_char[] = {
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 3, G_TX_NOLOD, G_TX_CLAMP, 4, G_TX_NOLOD),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, ((16 * 8 + G_IM_SIZ_4b_INCR) >> G_IM_SIZ_4b_SHIFT) - 1, CALC_DXT(16, G_IM_SIZ_4b_BYTES)),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_4b, 1, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 3, G_TX_NOLOD, G_TX_CLAMP, 4, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (16 - 1) << G_TEXTURE_IMAGE_FRAC, (8 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPVertex(vtx_ia8_char, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

//
// Patches
//

static OmmHMap sGfxPatches = omm_hmap_zero;

OMM_AT_STARTUP static void gfx_init_patch_display_lists() {
    omm_hmap_insert(sGfxPatches, (uintptr_t) dl_ia_text_tex_settings, gfx_ia8_char);
}

OMM_INLINE void *gfx_patch_display_list(void *gfx) {
    s32 i = omm_hmap_find(sGfxPatches, (uintptr_t) gfx);
    return (i != -1) ? omm_hmap_get(sGfxPatches, void *, i) : gfx;
}

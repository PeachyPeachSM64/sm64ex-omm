#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Vtx omm_bowser_fireball_flame_vertices[] = {
    { { { -25, 0, 0 }, 0, { 0, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 1600, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 1600, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 1600, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 1600, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 3200, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 3200, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 3200, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 3200, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 4800, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 4800, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 4800, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 4800, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 6400, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 6400, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 6400, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 6400, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 8000, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 8000, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 8000, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 8000, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 9600, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 9600, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 9600, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 9600, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 11200, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 11200, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 11200, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 11200, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 12800, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 12800, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 12800, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 12800, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 14400, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 14400, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 14400, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 14400, 0 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 16000, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 16000, 0 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 0, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 0, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 1600, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 1600, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 1600, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 1600, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 3200, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 3200, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 3200, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 3200, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 4800, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 4800, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 4800, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 4800, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 6400, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 6400, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 6400, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 6400, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 8000, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 8000, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 8000, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 8000, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 9600, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 9600, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 9600, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 9600, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 11200, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 11200, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 11200, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 11200, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 12800, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 12800, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 12800, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 12800, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 14400, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 14400, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 14400, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 14400, 4096 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 16000, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 16000, 4096 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 0, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 0, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 1600, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 1600, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 1600, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 1600, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 3200, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 3200, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 3200, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 3200, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 4800, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 4800, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 4800, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 4800, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 6400, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 6400, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 6400, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 6400, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 8000, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 8000, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 8000, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 8000, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 9600, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 9600, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 9600, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 9600, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 11200, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 11200, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 11200, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 11200, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 12800, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 12800, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 12800, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 12800, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 14400, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 14400, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 14400, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 14400, 8192 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 16000, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 16000, 8192 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 0, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 0, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 1600, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 1600, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 1600, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 1600, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 3200, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 3200, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 3200, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 3200, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 4800, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 4800, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 4800, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 4800, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 6400, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 6400, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 6400, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 6400, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 8000, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 8000, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 8000, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 8000, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 9600, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 9600, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 9600, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 9600, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 11200, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 11200, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 11200, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 11200, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 12800, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 12800, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 12800, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 12800, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 14400, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 14400, 12288 }, { 255, 255, 255, 255 } } },
    { { { -25, 0, 0 }, 0, { 14400, 16384 }, { 255, 255, 255, 255 } } },
    { { { -25, 128, 0 }, 0, { 14400, 12288 }, { 255, 255, 255, 255 } } },
    { { { 25, 0, 0 }, 0, { 16000, 16384 }, { 255, 255, 255, 255 } } },
    { { { 25, 128, 0 }, 0, { 16000, 12288 }, { 255, 255, 255, 255 } } },
};

static const Gfx omm_bowser_fireball_flame_0_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 0, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_1_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 4, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_2_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 8, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_3_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 12, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_4_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 16, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_5_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 20, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_6_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 24, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_7_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 28, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_8_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 32, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_9_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 36, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_10_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 40, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_11_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 44, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_12_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 48, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_13_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 52, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_14_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 56, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_15_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 60, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_16_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 64, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_17_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 68, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_18_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 72, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_19_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 76, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_20_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 80, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_21_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 84, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_22_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 88, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_23_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 92, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_24_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 96, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_25_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 100, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_26_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 104, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_27_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 108, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_28_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 112, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_29_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 116, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_30_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 120, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_31_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 124, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_32_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 128, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_33_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 132, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_34_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 136, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_35_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 140, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_36_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 144, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_37_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 148, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_38_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 152, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_bowser_fireball_flame_39_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_bowser_fireball_flame_vertices + 156, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

//
// Geo layout
//

const GeoLayout omm_geo_bowser_fireball_flame[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(40, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_0_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_2_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_3_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_4_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_5_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_6_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_7_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_8_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_9_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_10_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_11_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_12_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_13_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_14_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_15_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_16_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_17_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_18_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_19_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_20_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_21_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_22_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_23_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_24_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_25_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_26_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_27_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_28_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_29_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_30_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_31_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_32_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_33_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_34_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_35_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_36_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_37_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_38_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_39_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_bowser_fireball_flame_update() {
    struct Object *o = gCurrentObject;
    switch (o->oAction) {

        // Gets bigger
        case 0: {
            f32 t = clamp_0_1_f(o->oTimer / 10.f);
            o->oScaleX = 6.f * t;
            o->oScaleY = 4.f * t;
            o->oScaleZ = 6.f * t;
            if (o->oTimer >= 10) {
                o->oAction = 1;
                o->oTimer = 0;
            }
        } break;

        // Stays big
        case 1: {
            o->oScaleX = 6.f;
            o->oScaleY = 4.f;
            o->oScaleZ = 6.f;
            if (o->oTimer > o->oBowserFireDuration) {
                o->oAction = 2;
                o->oTimer = 0;
            }
        } break;

        // Gets smaller and vanishes
        case 2: {
            f32 t = 1.f - clamp_0_1_f(o->oTimer / 15.f);
            o->oScaleX = 6.f * t;
            o->oScaleY = 4.f * t;
            o->oScaleZ = 6.f * t;
            if (o->oTimer >= 15) {
                obj_mark_for_deletion(o);
                return;
            }
        } break;
    }
    o->oGraphYOffset = -8 * o->oScaleY;

    // Update Y position and delete if there is no valid floor
    struct Surface *floor = NULL;
    o->oPosY = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
    if (!floor || floor->type == SURFACE_BURNING || floor->type == SURFACE_DEATH_PLANE) {
        obj_mark_for_deletion(o);
        return;
    }

    // Small flames don't damage Mario
    if (o->oScaleY >= 1.f) {
        obj_set_params(o, INTERACT_FLAME, 1, 99, 0, true);
        obj_reset_hitbox(o, 10, 40, 0, 0, 0, 0);
    } else {
        obj_set_params(o, INTERACT_FLAME, 1, 99, 0, false);
        obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    }
}

const BehaviorScript omm_bhv_bowser_fireball_flame[] = {
    OBJ_TYPE_GENACTOR,
    0x11010001,
    0x08000000,
    0x0F1A0001,
    0x0C000000, (uintptr_t) omm_bhv_bowser_fireball_flame_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_bowser_fireball_flame(struct Object *o, s32 duration) {
    struct Object *flame = obj_spawn_from_geo(o, omm_geo_bowser_fireball_flame, omm_bhv_bowser_fireball_flame);
    flame->oBowserFireDuration = duration;
    flame->oAnimState = random_u16() % 40;
    flame->oOpacity = 255;
    flame->oAction = 0;
    obj_scale(flame, 0);
    return flame;
}

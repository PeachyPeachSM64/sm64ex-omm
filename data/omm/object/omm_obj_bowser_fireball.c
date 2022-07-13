#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Vtx omm_bowser_fireball_vertices[] = {
    { { { -128, -128, 0 }, 0, { 0, 4096 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 4096, 4096 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 4096, 0 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 4096, 4096 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 4096, 0 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 8192, 4096 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 8192, 0 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 8192, 4096 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 8192, 0 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 12288, 4096 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 12288, 0 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 12288, 4096 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 12288, 0 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 16384, 4096 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 16384, 0 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 0, 8192 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 0, 4096 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 4096, 8192 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 4096, 4096 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 4096, 8192 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 4096, 4096 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 8192, 8192 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 8192, 4096 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 8192, 8192 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 8192, 4096 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 12288, 8192 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 12288, 4096 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 12288, 8192 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 12288, 4096 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 16384, 8192 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 16384, 4096 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 0, 12288 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 0, 8192 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 4096, 12288 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 4096, 8192 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 4096, 12288 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 4096, 8192 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 8192, 12288 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 8192, 8192 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 8192, 12288 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 8192, 8192 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 12288, 12288 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 12288, 8192 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 12288, 12288 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 12288, 8192 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 16384, 12288 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 16384, 8192 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 0, 16384 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 0, 12288 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 4096, 16384 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 4096, 12288 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 4096, 16384 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 4096, 12288 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 8192, 16384 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 8192, 12288 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 8192, 16384 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 8192, 12288 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 12288, 16384 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 12288, 12288 }, { 255, 255, 255, 255 } } },
    { { { -128, -128, 0 }, 0, { 12288, 16384 }, { 255, 255, 255, 255 } } },
    { { { -128, 128, 0 }, 0, { 12288, 12288 }, { 255, 255, 255, 255 } } },
    { { { 128, -128, 0 }, 0, { 16384, 16384 }, { 255, 255, 255, 255 } } },
    { { { 128, 128, 0 }, 0, { 16384, 12288 }, { 255, 255, 255, 255 } } },
};

static const Gfx omm_bowser_fireball_0_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 0, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_1_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 4, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_2_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 8, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_3_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 12, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_4_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 16, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_5_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 20, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_6_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 24, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_7_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 28, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_8_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 32, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_9_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 36, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_10_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 40, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_11_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 44, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_12_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 48, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_13_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 52, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_14_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 56, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_fireball_15_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_bowser_fireball_vertices + 60, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 2, 3, 1, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_bowser_fireball[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(16, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_0_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_2_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_3_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_4_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_5_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_6_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_7_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_8_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_9_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_10_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_11_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_12_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_13_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_14_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_15_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_bowser_fireball_loop(void) {
    struct Object *o = gCurrentObject;
    if (o->oTimer < o->oBowserFireDuration) {
        o->oPosX += o->oVelX;
        o->oPosZ += o->oVelZ;
        f32 t = clamp_0_1_f(o->oTimer / 10.f);
        obj_scale(o, 2.f * t);
        obj_set_params(o, INTERACT_FLAME, 1, 99, 0, true);
        obj_reset_hitbox(o, 50, 100, 0, 0, 0, 50);
        if (o->oTimer & 1) {
            omm_spawn_bowser_fireball_flame(o, 30);
        }
    } else {
        obj_spawn_white_puff(o, -1);
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript omm_bhv_bowser_fireball[] = {
    OBJ_TYPE_GENACTOR,
    0x11010001,
    0x08000000,
    0x0F1A0001,
    0x0C000000, (uintptr_t) omm_bhv_bowser_fireball_loop,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_bowser_fireball(struct Object *o, f32 x, f32 y, f32 z, f32 forwardVel, f32 maxDistance, s16 angle) {
    struct Object *fireball = obj_spawn_from_geo(o, omm_geo_bowser_fireball, omm_bhv_bowser_fireball);
    obj_set_pos(fireball, x, y, z);
    obj_set_angle(fireball, 0, angle, 0);
    obj_set_forward_vel(fireball, angle, 1.f, forwardVel);
    obj_scale(fireball, 0);
    fireball->oOpacity = 255;
    fireball->oBowserFireDuration = (s32) (maxDistance / forwardVel);
    play_sound(SOUND_AIR_BOWSER_SPIT_FIRE, fireball->oCameraToObject);
    return fireball;
}

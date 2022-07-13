#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define OMM_STAR_RAY_CYCLE 36

//
// Gfx data
//

static const Lights1 omm_star_light = gdSPDefLights1(
    0x7F, 0x7F, 0x7F,
    0xFF, 0xFF, 0xFF,
    0x28, 0x28, 0x28
);

//
// Body
//

static const Vtx omm_star_body_vertices[] = {
    { { { 0.000000, 0.000000, 64.000000 }, 0, { 0, 0 }, { 0, 0, 127, 0xFF } } },
    { { { 0.000000, 0.000000, -64.000000 }, 0, { 0, 0 }, { 0, 0, -127, 0xFF } } },
    { { { 0.000000, 256.000000, 0.000000 }, 0, { 0, 0 }, { 0, 127, 0, 0xFF } } },
    { { { 243.446182, 79.183044, 0.000000 }, 0, { 0, 0 }, { 120, 39, 0, 0xFF } } },
    { { { 150.600082, -207.015976, 0.000000 }, 0, { 0, 0 }, { 74, -102, 0, 0xFF } } },
    { { { -150.282333, -207.246750, 0.000000 }, 0, { 0, 0 }, { -74, -102, 0, 0xFF } } },
    { { { -243.567368, 78.809509, 0.000000 }, 0, { 0, 0 }, { -120, 39, 0, 0xFF } } },
    { { { 75.141167, 103.623375, 0.000000 }, 0, { 0, 0 }, { 74, 102, 0, 0xFF } } },
    { { { 121.783684, -39.404755, 0.000000 }, 0, { 0, 0 }, { 120, -39, 0, 0xFF } } },
    { { { 0.000000, -128.000000, 0.000000 }, 0, { 0, 0 }, { 0, -127, 0, 0xFF } } },
    { { { -121.723091, -39.591522, 0.000000 }, 0, { 0, 0 }, { -120, -39, 0, 0xFF } } },
    { { { -75.300041, 103.507988, 0.000000 }, 0, { 0, 0 }, { -74, 102, 0, 0xFF } } },
};

static const Gfx omm_star_body_triangles[] = {
    gsSPVertex(omm_star_body_vertices, 12, 0),
    gsSP1Triangle(0, 7, 2, 0),
    gsSP1Triangle(0, 3, 7, 0),
    gsSP1Triangle(0, 8, 3, 0),
    gsSP1Triangle(0, 4, 8, 0),
    gsSP1Triangle(0, 9, 4, 0),
    gsSP1Triangle(0, 5, 9, 0),
    gsSP1Triangle(0, 10, 5, 0),
    gsSP1Triangle(0, 6, 10, 0),
    gsSP1Triangle(0, 11, 6, 0),
    gsSP1Triangle(0, 2, 11, 0),
    gsSP1Triangle(1, 2, 7, 0),
    gsSP1Triangle(1, 7, 3, 0),
    gsSP1Triangle(1, 3, 8, 0),
    gsSP1Triangle(1, 8, 4, 0),
    gsSP1Triangle(1, 4, 9, 0),
    gsSP1Triangle(1, 9, 5, 0),
    gsSP1Triangle(1, 5, 10, 0),
    gsSP1Triangle(1, 10, 6, 0),
    gsSP1Triangle(1, 6, 11, 0),
    gsSP1Triangle(1, 11, 2, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_0_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_0, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_1_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_2_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_3_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_3, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_4_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_4, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_5_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_5, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_6_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_6, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_7_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_7, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_8_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_8, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_9_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_9, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_10_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_10, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_11_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_11, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_12_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_12, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_13_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_13, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_14_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_14, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_15_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_15, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_16_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_16, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_17_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_17, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_18_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_18, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_19_texture_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_19, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_opaque_gfx[] = {
    gsSPSetGeometryMode(G_TEXTURE_GEN),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0),
    gsSPTexture(0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_star_light.l, 1),
    gsSPLight(&omm_star_light.a, 2),
    gsSPDisplayList(omm_star_body_triangles),
    gsSPTexture(0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_transparent_gfx[] = {
    gsSPSetGeometryMode(G_TEXTURE_GEN),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0x80),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0),
    gsSPTexture(0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_star_light.l, 1),
    gsSPLight(&omm_star_light.a, 2),
    gsSPDisplayList(omm_star_body_triangles),
    gsSPTexture(0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Eyes
//

static const Vtx omm_star_eyes_vertices[] = {
    { { { 0, 0, 64 }, 0, { 4300, 2867 }, { 0, 0, 127, 0xFF } } },
    { { { 0, 128, 32 }, 0, { 4198, -1536 }, { 0, 127, 0, 0xFF } } },
    { { { 75.1411665, 103.623375, 32 }, 0, { 204.5, -614.5 }, { 74, 102, 0, 0xFF } } },
    { { { 121.723091, 39.591522, 32 }, 0, { -2560, 1535.5 }, { 120, 39, 0, 0xFF } } },
    { { { 121.783684, -39.4047545, 32 }, 0, { -2560, 4198 }, { 120, -39, 0, 0xFF } } },
    { { { 75.300041, -103.507988, 32 }, 0, { 204.5, 6348.5 }, { 74, -102, 0, 0xFF } } },
    { { { 0, -128, 32 }, 0, { 4300, 7270 }, { 0, -127, 0, 0xFF } } },
    { { { 0, 128, 32 }, 0, { 4198, -1536 }, { 0, 127, 0, 0xFF } } },
    { { { -75.300041, 103.507988, 32 }, 0, { 204.5, -614.5 }, { -74, 102, 0, 0xFF } } },
    { { { -121.783684, 39.4047545, 32 }, 0, { -2560, 1535.5 }, { -120, 39, 0, 0xFF } } },
    { { { -121.723091, -39.591522, 32 }, 0, { -2560, 4198 }, { -120, -39, 0, 0xFF } } },
    { { { -75.1411665, -103.623375, 32 }, 0, { 204.5, 6348.5 }, { -74, -102, 0, 0xFF } } },
    { { { 0, -128, 32 }, 0, { 4300, 7270 }, { 0, -127, 0, 0xFF } } },

};

static const Gfx omm_star_eyes_triangles[] = {
    gsSPVertex(omm_star_eyes_vertices, 13, 0),
    gsSP1Triangle(0, 2, 1, 0),
    gsSP1Triangle(0, 3, 2, 0),
    gsSP1Triangle(0, 4, 3, 0),
    gsSP1Triangle(0, 5, 4, 0),
    gsSP1Triangle(0, 6, 5, 0),
    gsSP1Triangle(0, 7, 8, 0),
    gsSP1Triangle(0, 8, 9, 0),
    gsSP1Triangle(0, 9, 10, 0),
    gsSP1Triangle(0, 10, 11, 0),
    gsSP1Triangle(0, 11, 12, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_eyes_opaque_gfx[] = {
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_EYE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 2, 2, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_star_light.l, 1),
    gsSPLight(&omm_star_light.a, 2),
    gsSPDisplayList(omm_star_eyes_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_eyes_transparent_gfx[] = {
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0x80),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_EYE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 2, 2, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_star_light.l, 1),
    gsSPLight(&omm_star_light.a, 2),
    gsSPDisplayList(omm_star_eyes_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Glow
//

static const Vtx omm_star_glow_vertices[] = {
    { { { 0.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 128.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 118.256577, 48.983479, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 85.959549, 94.841743, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 48.983479, 118.256577, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 0.000000, 128.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { -48.983479, 118.256577, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { -90.509666, 90.509666, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { -118.256577, 48.983479, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { -128.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { -118.256577, -48.983479, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { -90.509666, -90.509666, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { -48.983479, -118.256577, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 0.000000, -128.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 48.983479, -118.256577, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 90.509666, -90.509666, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 118.256577, -48.983479, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xDC } } },
    { { { 512.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 473.026306, 195.933914, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 362.038666, 362.038666, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 195.933914, 473.026306, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0.000000, 512.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -195.933914, 473.026306, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -362.038666, 362.038666, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -473.026306, 195.933914, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -512.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -473.026306, -195.933914, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -362.038666, -362.038666, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -195.933914, -473.026306, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0.000000, -512.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 195.933914, -473.026306, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 362.038666, -362.038666, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 473.026306, -195.933914, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
};

static const Gfx omm_star_glow_triangles[] = {
    gsSPVertex(omm_star_glow_vertices, 33, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsSP1Triangle(0, 3, 4, 0),
    gsSP1Triangle(0, 4, 5, 0),
    gsSP1Triangle(0, 5, 6, 0),
    gsSP1Triangle(0, 6, 7, 0),
    gsSP1Triangle(0, 7, 8, 0),
    gsSP1Triangle(0, 8, 9, 0),
    gsSP1Triangle(0, 9, 10, 0),
    gsSP1Triangle(0, 10, 11, 0),
    gsSP1Triangle(0, 11, 12, 0),
    gsSP1Triangle(0, 12, 13, 0),
    gsSP1Triangle(0, 13, 14, 0),
    gsSP1Triangle(0, 14, 15, 0),
    gsSP1Triangle(0, 15, 16, 0),
    gsSP1Triangle(0, 16, 1, 0),
    gsSP2Triangles(1, 17, 2, 0, 2, 17, 18, 0),
    gsSP2Triangles(2, 18, 3, 0, 3, 18, 19, 0),
    gsSP2Triangles(3, 19, 4, 0, 4, 19, 20, 0),
    gsSP2Triangles(4, 20, 5, 0, 5, 20, 21, 0),
    gsSP2Triangles(5, 21, 6, 0, 6, 21, 22, 0),
    gsSP2Triangles(6, 22, 7, 0, 7, 22, 23, 0),
    gsSP2Triangles(7, 23, 8, 0, 8, 23, 24, 0),
    gsSP2Triangles(8, 24, 9, 0, 9, 24, 25, 0),
    gsSP2Triangles(9, 25, 10, 0, 10, 25, 26, 0),
    gsSP2Triangles(10, 26, 11, 0, 11, 26, 27, 0),
    gsSP2Triangles(11, 27, 12, 0, 12, 27, 28, 0),
    gsSP2Triangles(12, 28, 13, 0, 13, 28, 29, 0),
    gsSP2Triangles(13, 29, 14, 0, 14, 29, 30, 0),
    gsSP2Triangles(14, 30, 15, 0, 15, 30, 31, 0),
    gsSP2Triangles(15, 31, 16, 0, 16, 31, 32, 0),
    gsSP2Triangles(16, 32, 1, 0, 1, 32, 17, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_glow_gfx[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPDisplayList(omm_star_glow_triangles),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

//
// Rays
//

static const Vtx omm_star_rays_vertices[] = {
    { { { 0.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 1198.554565, 58.881210, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 1198.554565, -58.881210, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 1196.748535, 88.277481, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 1196.748535, -88.277481, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 1194.221680, 117.620567, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 1194.221680, -117.620567, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 1190.975464, 146.892807, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 1190.975464, -146.892807, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 1187.011841, 176.076569, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 1187.011841, -176.076569, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0.000000, 0.000000, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 1182.333130, 205.154266, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 1182.333130, -205.154266, 0.000000 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
};

static const Gfx omm_star_ray_0_triangles[] = {
    gsSPVertex(omm_star_rays_vertices + 0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 1, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_1_triangles[] = {
    gsSPVertex(omm_star_rays_vertices + 3, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 1, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_2_triangles[] = {
    gsSPVertex(omm_star_rays_vertices + 6, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 1, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_3_triangles[] = {
    gsSPVertex(omm_star_rays_vertices + 9, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 1, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_4_triangles[] = {
    gsSPVertex(omm_star_rays_vertices + 12, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 1, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_5_triangles[] = {
    gsSPVertex(omm_star_rays_vertices + 15, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 1, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_0_gfx[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPDisplayList(omm_star_ray_0_triangles),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_1_gfx[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPDisplayList(omm_star_ray_1_triangles),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_2_gfx[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPDisplayList(omm_star_ray_2_triangles),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_3_gfx[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPDisplayList(omm_star_ray_3_triangles),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_4_gfx[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPDisplayList(omm_star_ray_4_triangles),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_ray_5_gfx[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPDisplayList(omm_star_ray_5_triangles),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    s32 prevTimer;
    s32 counter;
    const char *texture;
    struct {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } color;
    struct {
        Gfx gfx[OMM_ARRAY_SIZE(omm_star_glow_gfx)];
    } glow;
    struct {
        Gfx gfx[OMM_ARRAY_SIZE(omm_star_ray_0_gfx)];
        f32 scale;
        s16 angle;
        u8 alpha;
    } rays[6];
} OmmStarGeoData;

static OmmStarGeoData *omm_geo_star_get_data(struct Object *o) {
    if (o->oGeoData == NULL) {
        OmmStarGeoData *data = omm_memory_new(gOmmMemoryPoolGeoData, sizeof(OmmStarGeoData), o);

        // Init display lists
        OMM_MEMCPY(data->glow.gfx, omm_star_glow_gfx, sizeof(omm_star_glow_gfx));
        OMM_MEMCPY(data->rays[0].gfx, omm_star_ray_0_gfx, sizeof(omm_star_ray_0_gfx));
        OMM_MEMCPY(data->rays[1].gfx, omm_star_ray_1_gfx, sizeof(omm_star_ray_1_gfx));
        OMM_MEMCPY(data->rays[2].gfx, omm_star_ray_2_gfx, sizeof(omm_star_ray_2_gfx));
        OMM_MEMCPY(data->rays[3].gfx, omm_star_ray_3_gfx, sizeof(omm_star_ray_3_gfx));
        OMM_MEMCPY(data->rays[4].gfx, omm_star_ray_4_gfx, sizeof(omm_star_ray_4_gfx));
        OMM_MEMCPY(data->rays[5].gfx, omm_star_ray_5_gfx, sizeof(omm_star_ray_5_gfx));

        // Init rays
        for (s32 i = 0; i != 6; ++i) {
            data->rays[i].scale = 0.5f + (0.3f * random_float());
            data->rays[i].angle = random_u16();
            data->rays[i].alpha = 0x80 + (random_u16() & 0x7F);
        }

        // Init data
        data->prevTimer = 0;
        data->counter = 0;
        data->texture = NULL;
        data->color.r = 0;
        data->color.g = 0;
        data->color.b = 0;
        data->color.a = 0;
        o->oGeoData = (void *) data;
    }
    return (OmmStarGeoData *) o->oGeoData;
}

static void omm_geo_star_set_env_color(Gfx *displayList, u32 r, u32 g, u32 b, u32 a) {
    displayList->words.w1 = (uintptr_t) (((r & 0xFF) << 24llu) | ((g & 0xFF) << 16llu) | ((b & 0xFF) << 8llu) | ((a & 0xFF) << 0llu));
}

static struct GraphNode *omm_geo_star_get_child(struct GraphNode *node, s16 type) {
    node = node->children;
    while (node->type != type) { node = node->next; }
    return node;
}

//
// Geo functions
//

static void omm_geo_star_compute_color(OmmStarGeoData *data, const char *texture) {
    static OmmMap sOmmStarTexColors = omm_map_zero;
    data->color.a = 0xFF;
    data->texture = texture;

    // Retrieve the color from the texture
    omm_map_for_each(sOmmStarTexColors, tex, color) {
        if (strcmp(tex->as_ptr, texture) == 0) {
            data->color.r = (color->as_u32 >> 16) & 0xFF;
            data->color.g = (color->as_u32 >>  8) & 0xFF;
            data->color.b = (color->as_u32 >>  0) & 0xFF;
            return;
        }
    }
    
    // Not found, compute new color and register it
    OMM_STRING(filename, 256, "%s/%s/%s.png", OMM_EXE_FOLDER, OMM_GFX_FOLDER, texture);
    s32 w, h;
    u8 *p = stbi_load(filename, &w, &h, NULL, 4);
    if (p) {
        u32 r = 0;
        u32 g = 0;
        u32 b = 0;
        for (s32 i = 0; i != w * h; ++i) {
            r += (u32) p[4 * i + 0];
            g += (u32) p[4 * i + 1];
            b += (u32) p[4 * i + 2];
        }
        data->color.r = (u8) (r / (w * h));
        data->color.g = (u8) (g / (w * h));
        data->color.b = (u8) (b / (w * h));
        stbi_image_free(p);
        omm_map_add(sOmmStarTexColors, ptr, OMM_MEMDUP(texture, strlen(texture) + 1), u32, (data->color.r << 16) | (data->color.g << 8) | (data->color.b << 0));
    }
}

static Gfx *omm_geo_star_update_color(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        OmmStarGeoData *data = omm_geo_star_get_data(gCurrGraphNodeObject);
        const char *texture = (const char *) ((const Gfx *) ((struct GraphNodeDisplayList *) node->next)->displayList)->words.w1;
        if (texture != data->texture) {
            omm_geo_star_compute_color(data, texture);
        }
    }
    return NULL;
}

static Gfx *omm_geo_star_enable_effects(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeScale *scaleNode = (struct GraphNodeScale *) node->next;
        scaleNode->scale = 1.f * !obj_get_first_with_behavior(bhvActSelector);
    }
    return NULL;
}

static Gfx *omm_geo_star_update_glow(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        OmmStarGeoData *data = omm_geo_star_get_data(gCurrGraphNodeObject);

        // Translate
        struct GraphNodeTranslation *translationNode = (struct GraphNodeTranslation *) node->next;
        geo_move_from_camera(gCurGraphNodeObject, translationNode, 240.f * gCurrGraphNodeObject->oScaleY);

        // Update env color
        omm_geo_star_set_env_color(data->glow.gfx, data->color.r, data->color.g, data->color.b, 0xFF);
        struct GraphNodeDisplayList *displayListNode = (struct GraphNodeDisplayList *) omm_geo_star_get_child(&translationNode->node, GRAPH_NODE_TYPE_DISPLAY_LIST);
        displayListNode->displayList = data->glow.gfx;
    }
    return NULL;
}

static Gfx *omm_geo_star_update_ray(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        OmmStarGeoData *data = omm_geo_star_get_data(gCurrGraphNodeObject);
        data->counter += (data->prevTimer != gCurrGraphNodeObject->oTimer);
        data->prevTimer = gCurrGraphNodeObject->oTimer;

        // If current tick == 0 (<=> alpha == 0), pick new random values
        struct GraphNodeGenerated *asGenerated = (struct GraphNodeGenerated *) node;
        s32 i = asGenerated->parameter;
        s32 t = (data->counter + ((OMM_STAR_RAY_CYCLE / 6) * i)) % OMM_STAR_RAY_CYCLE;
        if (t == 0) {
            data->rays[i].scale = 0.5f + (0.3f * random_float());
            data->rays[i].angle = random_u16();
            data->rays[i].alpha = 0x80 + (random_u16() & 0x7F);
        }

        // Update scale
        struct GraphNodeScale *scaleNode = (struct GraphNodeScale *) node->next;
        scaleNode->scale = data->rays[i].scale;

        // Update angle
        struct GraphNodeRotation *rotationNode = (struct GraphNodeRotation *) omm_geo_star_get_child(&scaleNode->node, GRAPH_NODE_TYPE_ROTATION);
        rotationNode->rotation[2] = data->rays[i].angle;

        // Update env color (rays color is slightly brighter)
        u32 r = ((data->color.r * 4) + 0xFF) / 5;
        u32 g = ((data->color.g * 4) + 0xFF) / 5;
        u32 b = ((data->color.b * 4) + 0xFF) / 5;
        s32 a = data->rays[i].alpha * ((coss(0x8000 + ((0x10000 * t) / OMM_STAR_RAY_CYCLE)) + 1.f) / 2.f);
        omm_geo_star_set_env_color(data->rays[i].gfx, r, g, b, a);
        struct GraphNodeDisplayList *displayListNode = (struct GraphNodeDisplayList *) omm_geo_star_get_child(&rotationNode->node, GRAPH_NODE_TYPE_DISPLAY_LIST);
        displayListNode->displayList = data->rays[i].gfx;
    }
    return NULL;
}

//
// Geo layouts
//

static const GeoLayout omm_geo_star_effects[] = {
    GEO_ASM(0, omm_geo_star_enable_effects),
    GEO_SCALE(0, 0x10000),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_OPEN_NODE(),
                GEO_ASM(0, omm_geo_star_update_glow),
                GEO_TRANSLATE_NODE(0, 0, 0, 0),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_glow_gfx),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
            GEO_OPEN_NODE(),
                GEO_ASM(0, omm_geo_star_update_ray),
                GEO_SCALE(0, 0x10000),
                GEO_OPEN_NODE(),
                    GEO_ROTATION_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_ray_0_gfx),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
            GEO_OPEN_NODE(),
                GEO_ASM(1, omm_geo_star_update_ray),
                GEO_SCALE(0, 0x10000),
                GEO_OPEN_NODE(),
                    GEO_ROTATION_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_ray_1_gfx),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
            GEO_OPEN_NODE(),
                GEO_ASM(2, omm_geo_star_update_ray),
                GEO_SCALE(0, 0x10000),
                GEO_OPEN_NODE(),
                    GEO_ROTATION_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_ray_2_gfx),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
            GEO_OPEN_NODE(),
                GEO_ASM(3, omm_geo_star_update_ray),
                GEO_SCALE(0, 0x10000),
                GEO_OPEN_NODE(),
                    GEO_ROTATION_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_ray_3_gfx),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
            GEO_OPEN_NODE(),
                GEO_ASM(4, omm_geo_star_update_ray),
                GEO_SCALE(0, 0x10000),
                GEO_OPEN_NODE(),
                    GEO_ROTATION_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_ray_4_gfx),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
            GEO_OPEN_NODE(),
                GEO_ASM(5, omm_geo_star_update_ray),
                GEO_SCALE(0, 0x10000),
                GEO_OPEN_NODE(),
                    GEO_ROTATION_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_ray_5_gfx),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_RETURN(),
};

const GeoLayout omm_geo_star_0_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_0_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_1_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_1_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_2_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_2_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_3_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_3_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_4_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_4_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_5_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_5_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_6_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_6_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_7_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_7_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_8_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_8_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_9_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_9_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_10_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_10_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_11_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_11_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_12_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_12_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_13_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_13_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_14_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_14_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_15_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_15_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_16_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_16_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_17_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_17_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_18_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_18_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_19_opaque[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_star_update_color),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_19_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx),
        GEO_CLOSE_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_BRANCH(1, omm_geo_star_effects),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_0_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_0_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_1_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_1_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_2_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_2_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_3_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_3_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_4_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_4_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_5_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_5_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_6_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_6_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_7_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_7_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_8_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_8_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_9_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_9_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_10_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_10_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_11_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_11_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_12_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_12_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_13_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_13_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_14_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_14_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_15_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_15_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_16_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_16_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_17_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_17_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_18_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_18_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_star_19_transparent[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_19_texture_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

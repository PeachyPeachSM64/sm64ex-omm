#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Lights1 omm_menu_button_light = gdSPDefLights1(
    0x3f, 0x3f, 0x3f,
    0xff, 0xff, 0xff,
    0x28, 0x28, 0x28
);

static const Vtx omm_menu_button_vertices[] = {
    { { { -163, -122, 0 }, 0, { 0, 990 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { 163, -122, 0 }, 0, { 990, 990 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { -122, -81, 30 }, 0, { 96, 820 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { 122, -81, 30 }, 0, { 862, 820 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { -163, -122, 0 }, 0, { 0, 990 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -122, -81, 30 }, 0, { 96, 820 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -163, 122, 0 }, 0, { 0, 0 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -122, 81, 30 }, 0, { 96, 138 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -122, 81, 30 }, 0, { 96, 138 }, { 0x00, 0x4a, 0x66, 0xff } } },
    { { { 122, 81, 30 }, 0, { 862, 138 }, { 0x00, 0x4a, 0x66, 0xff } } },
    { { { 163, 122, 0 }, 0, { 990, 0 }, { 0x00, 0x4a, 0x66, 0xff } } },
    { { { -163, 122, 0 }, 0, { 0, 0 }, { 0x00, 0x4a, 0x66, 0xff } } },
    { { { 122, 81, 30 }, 0, { 862, 138 }, { 0x4a, 0x00, 0x66, 0xff } } },
    { { { 122, -81, 30 }, 0, { 862, 820 }, { 0x4a, 0x00, 0x66, 0xff } } },
    { { { 163, -122, 0 }, 0, { 990, 990 }, { 0x4a, 0x00, 0x66, 0xff } } },
    { { { 163, 122, 0 }, 0, { 990, 0 }, { 0x4a, 0x00, 0x66, 0xff } } },
    { { { 122, 81, 30 }, 0, { 990, 0 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { -122, 81, 30 }, 0, { 0, 0 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { 122, -81, 30 }, 0, { 990, 990 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { -122, -81, 30 }, 0, { 0, 990 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { 163, -122, 0 }, 0, { 0, 990 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -163, -122, 0 }, 0, { 990, 990 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 163, 122, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -163, 122, 0 }, 0, { 990, 0 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -163, -122, 0 }, 0, { 990, 0 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -122, -81, 30 }, 0, { 862, 138 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -163, 122, 0 }, 0, { 990, 990 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -143, 102, 0 }, 0, { 926, 904 }, { 0x59, 0x00, 0xa7, 0xff } } },
    { { { -133, 92, 10 }, 0, { 894, 862 }, { 0x59, 0x00, 0xa7, 0xff } } },
    { { { -133, -92, 10 }, 0, { 894, 96 }, { 0x59, 0x00, 0xa7, 0xff } } },
    { { { -133, 92, 10 }, 0, { 894, 862 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 133, -92, 10 }, 0, { 64, 96 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -133, -92, 10 }, 0, { 894, 96 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 133, 92, 10 }, 0, { 64, 862 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 133, 92, 10 }, 0, { 64, 862 }, { 0x00, 0xa7, 0xa7, 0xff } } },
    { { { -133, 92, 10 }, 0, { 894, 862 }, { 0x00, 0xa7, 0xa7, 0xff } } },
    { { { -143, 102, 0 }, 0, { 926, 904 }, { 0x00, 0xa7, 0xa7, 0xff } } },
    { { { 143, -102, 0 }, 0, { 32, 54 }, { 0xa7, 0x00, 0xa7, 0xff } } },
    { { { 133, 92, 10 }, 0, { 64, 862 }, { 0xa7, 0x00, 0xa7, 0xff } } },
    { { { 143, 102, 0 }, 0, { 32, 904 }, { 0xa7, 0x00, 0xa7, 0xff } } },
    { { { 143, -102, 0 }, 0, { 32, 54 }, { 0xa7, 0x00, 0xa7, 0xff } } },
    { { { 133, -92, 10 }, 0, { 64, 96 }, { 0xa7, 0x00, 0xa7, 0xff } } },
    { { { 133, 92, 10 }, 0, { 64, 862 }, { 0xa7, 0x00, 0xa7, 0xff } } },
    { { { 133, 92, 10 }, 0, { 64, 862 }, { 0x00, 0xa7, 0xa7, 0xff } } },
    { { { -143, 102, 0 }, 0, { 926, 904 }, { 0x00, 0xa7, 0xa7, 0xff } } },
    { { { 143, 102, 0 }, 0, { 32, 904 }, { 0x00, 0xa7, 0xa7, 0xff } } },
    { { { -143, -102, 0 }, 0, { 926, 54 }, { 0x00, 0x59, 0xa7, 0xff } } },
    { { { 133, -92, 10 }, 0, { 64, 96 }, { 0x00, 0x59, 0xa7, 0xff } } },
    { { { 143, -102, 0 }, 0, { 32, 54 }, { 0x00, 0x59, 0xa7, 0xff } } },
    { { { -133, -92, 10 }, 0, { 894, 96 }, { 0x00, 0x59, 0xa7, 0xff } } },
    { { { -143, 102, 0 }, 0, { 926, 904 }, { 0x59, 0x00, 0xa7, 0xff } } },
    { { { -133, -92, 10 }, 0, { 894, 96 }, { 0x59, 0x00, 0xa7, 0xff } } },
    { { { -143, -102, 0 }, 0, { 926, 54 }, { 0x59, 0x00, 0xa7, 0xff } } },
    { { { 163, 122, 0 }, 0, { 0, 990 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -143, 102, 0 }, 0, { 926, 904 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -163, 122, 0 }, 0, { 990, 990 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 163, 122, 0 }, 0, { 0, 990 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 143, 102, 0 }, 0, { 32, 904 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -143, 102, 0 }, 0, { 926, 904 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 143, -102, 0 }, 0, { 32, 54 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 163, -122, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -163, 122, 0 }, 0, { 990, 990 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -143, -102, 0 }, 0, { 926, 54 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -163, -122, 0 }, 0, { 990, 0 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 163, -122, 0 }, 0, { 0, 0 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { 122, -81, 30 }, 0, { 96, 138 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { -122, -81, 30 }, 0, { 862, 138 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { -122, -81, 30 }, 0, { 862, 138 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -122, 81, 30 }, 0, { 862, 820 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -163, 122, 0 }, 0, { 990, 990 }, { 0xb6, 0x00, 0x66, 0xff } } },
    { { { -122, 81, 30 }, 0, { 862, 820 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { -122, -81, 30 }, 0, { 862, 138 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { 122, -81, 30 }, 0, { 96, 138 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { -163, -122, 0 }, 0, { 990, 0 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { 163, -122, 0 }, 0, { 0, 0 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { -122, -81, 30 }, 0, { 862, 138 }, { 0x00, 0xb6, 0x66, 0xff } } },
    { { { -122, 81, 30 }, 0, { 862, 820 }, { 0x00, 0x4a, 0x66, 0xff } } },
    { { { 122, 81, 30 }, 0, { 96, 820 }, { 0x00, 0x4a, 0x66, 0xff } } },
    { { { 163, 122, 0 }, 0, { 0, 990 }, { 0x00, 0x4a, 0x66, 0xff } } },
    { { { -163, 122, 0 }, 0, { 990, 990 }, { 0x00, 0x4a, 0x66, 0xff } } },
    { { { 122, 81, 30 }, 0, { 96, 820 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { 163, 122, 0 }, 0, { 0, 990 }, { 0x4a, 0x00, 0x66, 0xff } } },
    { { { 122, 81, 30 }, 0, { 96, 820 }, { 0x4a, 0x00, 0x66, 0xff } } },
    { { { 163, -122, 0 }, 0, { 0, 0 }, { 0x4a, 0x00, 0x66, 0xff } } },
    { { { 122, -81, 30 }, 0, { 96, 138 }, { 0x4a, 0x00, 0x66, 0xff } } },
};

static const Gfx omm_menu_button_triangles[] = {
    gsSPVertex(omm_menu_button_vertices, 16, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 5, 7, 6, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 8, 10, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 12, 14, 0),
    gsSPEndDisplayList(),

    gsSPVertex(omm_menu_button_vertices + 16, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSPEndDisplayList(),

    gsSPVertex(omm_menu_button_vertices + 20, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSPEndDisplayList(),

    gsSPVertex(omm_menu_button_vertices + 24, 16, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 6, 9, 7, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 15, 0),
    gsSPVertex(omm_menu_button_vertices + 40, 16, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 6, 9, 7, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 15, 0),
    gsSPVertex(omm_menu_button_vertices + 56, 14, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    gsSP2Triangles(0, 4, 3, 0, 5, 2, 6, 0),
    gsSP2Triangles(5, 6, 7, 0, 6, 3, 4, 0),
    gsSP2Triangles(6, 4, 7, 0, 8, 9, 10, 0),
    gsSP1Triangle(11, 12, 13, 0),
    gsSPVertex(omm_menu_button_vertices + 70, 15, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 6, 8, 0),
    gsSP2Triangles(10, 0, 2, 0, 11, 12, 13, 0),
    gsSP1Triangle(12, 14, 13, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_menu_button_mario_front_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_menu_button_light.l, 1),
    gsSPLight(&omm_menu_button_light.a, 2),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_MARIO_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_MARIO_HEAD, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 6),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_MARIO_DARK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 9),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_menu_button_mario_back_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_MARIO_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPLight(&omm_menu_button_light.l, 1),
    gsSPLight(&omm_menu_button_light.a, 2),
    gsSPDisplayList(omm_menu_button_triangles + 12),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_menu_button_peach_front_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_menu_button_light.l, 1),
    gsSPLight(&omm_menu_button_light.a, 2),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_PEACH_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_PEACH_HEAD, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 6),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_PEACH_DARK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 9),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_menu_button_peach_back_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_PEACH_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPLight(&omm_menu_button_light.l, 1),
    gsSPLight(&omm_menu_button_light.a, 2),
    gsSPDisplayList(omm_menu_button_triangles + 12),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_menu_button_luigi_front_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_menu_button_light.l, 1),
    gsSPLight(&omm_menu_button_light.a, 2),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_LUIGI_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_LUIGI_HEAD, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 6),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_LUIGI_DARK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 9),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_menu_button_luigi_back_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_LUIGI_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPLight(&omm_menu_button_light.l, 1),
    gsSPLight(&omm_menu_button_light.a, 2),
    gsSPDisplayList(omm_menu_button_triangles + 12),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_menu_button_wario_front_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_menu_button_light.l, 1),
    gsSPLight(&omm_menu_button_light.a, 2),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_WARIO_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_WARIO_HEAD, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 6),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_WARIO_DARK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_menu_button_triangles + 9),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_menu_button_wario_back_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_MENU_WARIO_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPLight(&omm_menu_button_light.l, 1),
    gsSPLight(&omm_menu_button_light.a, 2),
    gsSPDisplayList(omm_menu_button_triangles + 12),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

//
// Geo layouts
//

const GeoLayout omm_geo_menu_button_mario[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x80000),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_menu_button_mario_front_gfx),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_menu_button_mario_back_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_menu_button_peach[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x80000),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_menu_button_peach_front_gfx),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_menu_button_peach_back_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_menu_button_luigi[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x80000),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_menu_button_luigi_front_gfx),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_menu_button_luigi_back_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_menu_button_wario[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x80000),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_menu_button_wario_front_gfx),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_menu_button_wario_back_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

const BehaviorScript omm_bhv_menu_character_select_button[] = {
    OBJ_TYPE_LEVEL,
    0x04000000, (uintptr_t) bhvMenuButton
};

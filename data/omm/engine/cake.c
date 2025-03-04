#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static const char cake_end_texture[] = "levels/ending/cake";

static const Vtx cake_end_vertex[] = {
    {{{            0,             0, -1 }, 0, {    0, 608 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{ SCREEN_WIDTH,             0, -1 }, 0, { 2528, 608 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{ SCREEN_WIDTH, SCREEN_HEIGHT, -1 }, 0, { 2528,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{            0, SCREEN_HEIGHT, -1 }, 0, {    0,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
};

const Gfx dl_cake_end_screen[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPSetRenderMode(G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(cake_end_texture, G_IM_FMT_RGBA, G_IM_SIZ_16b, 80, 20, 0, G_TX_CLAMP, G_TX_CLAMP, 7, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(cake_end_vertex, 4, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 2, 3, 0x0),
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};

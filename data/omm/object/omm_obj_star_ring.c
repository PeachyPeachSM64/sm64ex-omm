#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define OMM_RING_SCALE 1.00f
#define oRingDot0 OBJECT_FIELD_F32(0x1B)
#define oRingDot1 OBJECT_FIELD_F32(0x1C)

//
// Gfx
//

static const Lights1 omm_star_ring_light = gdSPDefLights1(
    0x7f, 0x7f, 0x7f,
    0xff, 0xff, 0xff,
    0x28, 0x28, 0x28
);

static const Vtx omm_star_ring_vtx[] = {
    { { {    0,  264,   0 }, 0, { 0, 0 }, { 0x00, 0x2a, 0x77, 0x00 } } },
    { { { -131,  228,   0 }, 0, { 0, 0 }, { 0xeb, 0x25, 0x77, 0x00 } } },
    { { {  -98,  171,  23 }, 0, { 0, 0 }, { 0xeb, 0x25, 0x77, 0x00 } } },
    { { {    0,  198,  23 }, 0, { 0, 0 }, { 0x00, 0x2a, 0x77, 0x00 } } },
    { { {    0,  198,  23 }, 0, { 0, 0 }, { 0x00, 0x81, 0x00, 0x00 } } },
    { { {  -98,  171,  23 }, 0, { 0, 0 }, { 0x3f, 0x93, 0x00, 0x00 } } },
    { { {  -98,  171, -23 }, 0, { 0, 0 }, { 0x3f, 0x93, 0x00, 0x00 } } },
    { { {    0,  198, -23 }, 0, { 0, 0 }, { 0x00, 0x81, 0x00, 0x00 } } },
    { { {    0,  198, -23 }, 0, { 0, 0 }, { 0x00, 0x2a, 0x89, 0x00 } } },
    { { {  -98,  171, -23 }, 0, { 0, 0 }, { 0xeb, 0x25, 0x89, 0x00 } } },
    { { { -131,  228,   0 }, 0, { 0, 0 }, { 0xeb, 0x25, 0x89, 0x00 } } },
    { { {    0,  264,   0 }, 0, { 0, 0 }, { 0x00, 0x2a, 0x89, 0x00 } } },
    { { { -227,  132,   0 }, 0, { 0, 0 }, { 0xdb, 0x15, 0x77, 0x00 } } },
    { { { -170,   99,  23 }, 0, { 0, 0 }, { 0xdb, 0x15, 0x77, 0x00 } } },
    { { { -170,   99,  23 }, 0, { 0, 0 }, { 0x6d, 0xc1, 0x00, 0x00 } } },
    { { { -170,   99, -23 }, 0, { 0, 0 }, { 0x6d, 0xc1, 0x00, 0x00 } } },
    { { {  -98,  171, -23 }, 0, { 0, 0 }, { 0xeb, 0x25, 0x89, 0x00 } } },
    { { { -170,   99, -23 }, 0, { 0, 0 }, { 0xdb, 0x15, 0x89, 0x00 } } },
    { { { -227,  132,   0 }, 0, { 0, 0 }, { 0xdb, 0x15, 0x89, 0x00 } } },
    { { { -131,  228,   0 }, 0, { 0, 0 }, { 0xeb, 0x25, 0x89, 0x00 } } },
    { { { -227,  132,   0 }, 0, { 0, 0 }, { 0xdb, 0x15, 0x77, 0x00 } } },
    { { { -263,    0,   0 }, 0, { 0, 0 }, { 0xd6, 0x00, 0x77, 0x00 } } },
    { { { -197,    0,  23 }, 0, { 0, 0 }, { 0xd6, 0x00, 0x77, 0x00 } } },
    { { { -170,   99,  23 }, 0, { 0, 0 }, { 0xdb, 0x15, 0x77, 0x00 } } },
    { { { -170,   99,  23 }, 0, { 0, 0 }, { 0x6d, 0xc1, 0x00, 0x00 } } },
    { { { -197,    0,  23 }, 0, { 0, 0 }, { 0x7f, 0x00, 0x00, 0x00 } } },
    { { { -197,    0, -23 }, 0, { 0, 0 }, { 0x7f, 0x00, 0x00, 0x00 } } },
    { { { -170,   99, -23 }, 0, { 0, 0 }, { 0x6d, 0xc1, 0x00, 0x00 } } },
    { { { -197,    0, -23 }, 0, { 0, 0 }, { 0xd6, 0x00, 0x89, 0x00 } } },
    { { { -263,    0,   0 }, 0, { 0, 0 }, { 0xd6, 0x00, 0x89, 0x00 } } },
    { { { -227, -132,   0 }, 0, { 0, 0 }, { 0xdb, 0xeb, 0x77, 0x00 } } },
    { { { -170,  -99,  23 }, 0, { 0, 0 }, { 0xdb, 0xeb, 0x77, 0x00 } } },
    { { { -197,    0,  23 }, 0, { 0, 0 }, { 0x7f, 0x00, 0x00, 0x00 } } },
    { { { -170,  -99,  23 }, 0, { 0, 0 }, { 0x6d, 0x3f, 0x00, 0x00 } } },
    { { { -170,  -99, -23 }, 0, { 0, 0 }, { 0x6d, 0x3f, 0x00, 0x00 } } },
    { { { -197,    0, -23 }, 0, { 0, 0 }, { 0x7f, 0x00, 0x00, 0x00 } } },
    { { { -197,    0, -23 }, 0, { 0, 0 }, { 0xd6, 0x00, 0x89, 0x00 } } },
    { { { -170,  -99, -23 }, 0, { 0, 0 }, { 0xdb, 0xeb, 0x89, 0x00 } } },
    { { { -227, -132,   0 }, 0, { 0, 0 }, { 0xdb, 0xeb, 0x89, 0x00 } } },
    { { { -263,    0,   0 }, 0, { 0, 0 }, { 0xd6, 0x00, 0x89, 0x00 } } },
    { { { -227, -132,   0 }, 0, { 0, 0 }, { 0xdb, 0xeb, 0x77, 0x00 } } },
    { { { -131, -228,   0 }, 0, { 0, 0 }, { 0xeb, 0xdb, 0x77, 0x00 } } },
    { { {  -98, -171,  23 }, 0, { 0, 0 }, { 0xeb, 0xdb, 0x77, 0x00 } } },
    { { { -170,  -99,  23 }, 0, { 0, 0 }, { 0xdb, 0xeb, 0x77, 0x00 } } },
    { { {  -98, -171,  23 }, 0, { 0, 0 }, { 0x3f, 0x6d, 0x00, 0x00 } } },
    { { {  -98, -171, -23 }, 0, { 0, 0 }, { 0x3f, 0x6d, 0x00, 0x00 } } },
    { { {  -98, -171, -23 }, 0, { 0, 0 }, { 0xeb, 0xdb, 0x89, 0x00 } } },
    { { { -131, -228,   0 }, 0, { 0, 0 }, { 0xeb, 0xdb, 0x89, 0x00 } } },
    { { { -131, -228,   0 }, 0, { 0, 0 }, { 0xeb, 0xdb, 0x77, 0x00 } } },
    { { {    0, -264,   0 }, 0, { 0, 0 }, { 0x00, 0xd6, 0x77, 0x00 } } },
    { { {    0, -198,  23 }, 0, { 0, 0 }, { 0x00, 0xd6, 0x77, 0x00 } } },
    { { {  -98, -171,  23 }, 0, { 0, 0 }, { 0xeb, 0xdb, 0x77, 0x00 } } },
    { { {  -98, -171,  23 }, 0, { 0, 0 }, { 0x3f, 0x6d, 0x00, 0x00 } } },
    { { {    0, -198,  23 }, 0, { 0, 0 }, { 0x00, 0x7f, 0x00, 0x00 } } },
    { { {    0, -198, -23 }, 0, { 0, 0 }, { 0x00, 0x7f, 0x00, 0x00 } } },
    { { {  -98, -171, -23 }, 0, { 0, 0 }, { 0x3f, 0x6d, 0x00, 0x00 } } },
    { { {  -98, -171, -23 }, 0, { 0, 0 }, { 0xeb, 0xdb, 0x89, 0x00 } } },
    { { {    0, -198, -23 }, 0, { 0, 0 }, { 0x00, 0xd6, 0x89, 0x00 } } },
    { { {    0, -264,   0 }, 0, { 0, 0 }, { 0x00, 0xd6, 0x89, 0x00 } } },
    { { { -131, -228,   0 }, 0, { 0, 0 }, { 0xeb, 0xdb, 0x89, 0x00 } } },
    { { {  132, -228,   0 }, 0, { 0, 0 }, { 0x15, 0xdb, 0x77, 0x00 } } },
    { { {   99, -171,  23 }, 0, { 0, 0 }, { 0x15, 0xdb, 0x77, 0x00 } } },
    { { {   99, -171,  23 }, 0, { 0, 0 }, { 0xc1, 0x6d, 0x00, 0x00 } } },
    { { {   99, -171, -23 }, 0, { 0, 0 }, { 0xc1, 0x6d, 0x00, 0x00 } } },
    { { {    0, -198, -23 }, 0, { 0, 0 }, { 0x00, 0xd6, 0x89, 0x00 } } },
    { { {   99, -171, -23 }, 0, { 0, 0 }, { 0x15, 0xdb, 0x89, 0x00 } } },
    { { {  132, -228,   0 }, 0, { 0, 0 }, { 0x15, 0xdb, 0x89, 0x00 } } },
    { { {    0, -264,   0 }, 0, { 0, 0 }, { 0x00, 0xd6, 0x89, 0x00 } } },
    { { {  132, -228,   0 }, 0, { 0, 0 }, { 0x15, 0xdb, 0x77, 0x00 } } },
    { { {  229, -132,   0 }, 0, { 0, 0 }, { 0x25, 0xeb, 0x77, 0x00 } } },
    { { {  172,  -99,  23 }, 0, { 0, 0 }, { 0x25, 0xeb, 0x77, 0x00 } } },
    { { {   99, -171,  23 }, 0, { 0, 0 }, { 0x15, 0xdb, 0x77, 0x00 } } },
    { { {   99, -171,  23 }, 0, { 0, 0 }, { 0xc1, 0x6d, 0x00, 0x00 } } },
    { { {  172,  -99,  23 }, 0, { 0, 0 }, { 0x93, 0x3f, 0x00, 0x00 } } },
    { { {  172,  -99, -23 }, 0, { 0, 0 }, { 0x93, 0x3f, 0x00, 0x00 } } },
    { { {   99, -171, -23 }, 0, { 0, 0 }, { 0xc1, 0x6d, 0x00, 0x00 } } },
    { { {  172,  -99, -23 }, 0, { 0, 0 }, { 0x25, 0xeb, 0x89, 0x00 } } },
    { { {  229, -132,   0 }, 0, { 0, 0 }, { 0x25, 0xeb, 0x89, 0x00 } } },
    { { {  264,    0,   0 }, 0, { 0, 0 }, { 0x2a, 0x00, 0x77, 0x00 } } },
    { { {  198,    0,  23 }, 0, { 0, 0 }, { 0x2a, 0x00, 0x77, 0x00 } } },
    { { {  172,  -99,  23 }, 0, { 0, 0 }, { 0x93, 0x3f, 0x00, 0x00 } } },
    { { {  198,    0,  23 }, 0, { 0, 0 }, { 0x81, 0x00, 0x00, 0x00 } } },
    { { {  198,    0, -23 }, 0, { 0, 0 }, { 0x81, 0x00, 0x00, 0x00 } } },
    { { {  172,  -99, -23 }, 0, { 0, 0 }, { 0x93, 0x3f, 0x00, 0x00 } } },
    { { {  172,  -99, -23 }, 0, { 0, 0 }, { 0x25, 0xeb, 0x89, 0x00 } } },
    { { {  198,    0, -23 }, 0, { 0, 0 }, { 0x2a, 0x00, 0x89, 0x00 } } },
    { { {  264,    0,   0 }, 0, { 0, 0 }, { 0x2a, 0x00, 0x89, 0x00 } } },
    { { {  229, -132,   0 }, 0, { 0, 0 }, { 0x25, 0xeb, 0x89, 0x00 } } },
    { { {  264,    0,   0 }, 0, { 0, 0 }, { 0x2a, 0x00, 0x77, 0x00 } } },
    { { {  229,  132,   0 }, 0, { 0, 0 }, { 0x25, 0x15, 0x77, 0x00 } } },
    { { {  172,   99,  23 }, 0, { 0, 0 }, { 0x25, 0x15, 0x77, 0x00 } } },
    { { {  198,    0,  23 }, 0, { 0, 0 }, { 0x2a, 0x00, 0x77, 0x00 } } },
    { { {  172,   99,  23 }, 0, { 0, 0 }, { 0x93, 0xc1, 0x00, 0x00 } } },
    { { {  172,   99, -23 }, 0, { 0, 0 }, { 0x93, 0xc1, 0x00, 0x00 } } },
    { { {  172,   99, -23 }, 0, { 0, 0 }, { 0x25, 0x15, 0x89, 0x00 } } },
    { { {  229,  132,   0 }, 0, { 0, 0 }, { 0x25, 0x15, 0x89, 0x00 } } },
    { { {  229,  132,   0 }, 0, { 0, 0 }, { 0x25, 0x15, 0x77, 0x00 } } },
    { { {  132,  228,   0 }, 0, { 0, 0 }, { 0x15, 0x25, 0x77, 0x00 } } },
    { { {   99,  171,  23 }, 0, { 0, 0 }, { 0x15, 0x25, 0x77, 0x00 } } },
    { { {  172,   99,  23 }, 0, { 0, 0 }, { 0x25, 0x15, 0x77, 0x00 } } },
    { { {  172,   99,  23 }, 0, { 0, 0 }, { 0x93, 0xc1, 0x00, 0x00 } } },
    { { {   99,  171,  23 }, 0, { 0, 0 }, { 0xc1, 0x93, 0x00, 0x00 } } },
    { { {   99,  171, -23 }, 0, { 0, 0 }, { 0xc1, 0x93, 0x00, 0x00 } } },
    { { {  172,   99, -23 }, 0, { 0, 0 }, { 0x93, 0xc1, 0x00, 0x00 } } },
    { { {  172,   99, -23 }, 0, { 0, 0 }, { 0x25, 0x15, 0x89, 0x00 } } },
    { { {   99,  171, -23 }, 0, { 0, 0 }, { 0x15, 0x25, 0x89, 0x00 } } },
    { { {  132,  228,   0 }, 0, { 0, 0 }, { 0x15, 0x25, 0x89, 0x00 } } },
    { { {  229,  132,   0 }, 0, { 0, 0 }, { 0x25, 0x15, 0x89, 0x00 } } },
    { { {    0,  264,   0 }, 0, { 0, 0 }, { 0x00, 0x2a, 0x77, 0x00 } } },
    { { {    0,  198,  23 }, 0, { 0, 0 }, { 0x00, 0x2a, 0x77, 0x00 } } },
    { { {    0,  198,  23 }, 0, { 0, 0 }, { 0x00, 0x81, 0x00, 0x00 } } },
    { { {    0,  198, -23 }, 0, { 0, 0 }, { 0x00, 0x81, 0x00, 0x00 } } },
    { { {   99,  171, -23 }, 0, { 0, 0 }, { 0x15, 0x25, 0x89, 0x00 } } },
    { { {    0,  198, -23 }, 0, { 0, 0 }, { 0x00, 0x2a, 0x89, 0x00 } } },
    { { {    0,  264,   0 }, 0, { 0, 0 }, { 0x00, 0x2a, 0x89, 0x00 } } },
    { { {  132,  228,   0 }, 0, { 0, 0 }, { 0x15, 0x25, 0x89, 0x00 } } },
    { { {    0,    0,  32 }, 0, { 0, 0 }, {    0,    0,  127, 0x00 } } },
    { { {    0,    0, -32 }, 0, { 0, 0 }, {    0,    0, -127, 0x00 } } },
    { { {    0,  128,   0 }, 0, { 0, 0 }, {    0,  127,    0, 0x00 } } },
    { { {  121,   39,   0 }, 0, { 0, 0 }, {  120,   39,    0, 0x00 } } },
    { { {   75, -103,   0 }, 0, { 0, 0 }, {   74, -102,    0, 0x00 } } },
    { { {  -75, -103,   0 }, 0, { 0, 0 }, {  -74, -102,    0, 0x00 } } },
    { { { -121,   39,   0 }, 0, { 0, 0 }, { -120,   39,    0, 0x00 } } },
    { { {   37,   51,   0 }, 0, { 0, 0 }, {   74,  102,    0, 0x00 } } },
    { { {   60,  -19,   0 }, 0, { 0, 0 }, {  120,  -39,    0, 0x00 } } },
    { { {    0,  -64,   0 }, 0, { 0, 0 }, {    0, -127,    0, 0x00 } } },
    { { {  -60,  -19,   0 }, 0, { 0, 0 }, { -120,  -39,    0, 0x00 } } },
    { { {  -37,   51,   0 }, 0, { 0, 0 }, {  -74,  102,    0, 0x00 } } },
};

static const Gfx omm_star_ring_gfx[] = {
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN),
    gsDPSetCombineMode(G_CC_DECALFADE, G_CC_DECALFADE),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_STAR_RING, G_IM_FMT_RGBA, G_IM_SIZ_32b, 64, 32, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, 6, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPTexture(0x1800, 0x07C0, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_star_ring_light.l, 1),
    gsSPLight(&omm_star_ring_light.a, 2),
    gsSPVertex(omm_star_ring_vtx, 16, 0),
    gsSP2Triangles(0,  1,  2, 0, 0,  2,  3, 0),
    gsSP2Triangles(4,  5,  6, 0, 4,  6,  7, 0),
    gsSP2Triangles(8,  9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(1, 12, 13, 0, 1, 13,  2, 0),
    gsSP2Triangles(5, 14, 15, 0, 5, 15,  6, 0),
    gsSPVertex(omm_star_ring_vtx + 16, 16, 0),
    gsSP2Triangles(0,  1,  2, 0, 0,  2,  3, 0),
    gsSP2Triangles(4,  5,  6, 0, 4,  6,  7, 0),
    gsSP2Triangles(8,  9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(1, 12, 13, 0, 1, 13,  2, 0),
    gsSP2Triangles(5, 14, 15, 0, 5, 15,  6, 0),
    gsSPVertex(omm_star_ring_vtx + 32, 16, 0),
    gsSP2Triangles(0,  1,  2, 0, 0,  2,  3, 0),
    gsSP2Triangles(4,  5,  6, 0, 4,  6,  7, 0),
    gsSP2Triangles(8,  9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(1, 12, 13, 0, 1, 13,  2, 0),
    gsSP2Triangles(5, 14, 15, 0, 5, 15,  6, 0),
    gsSPVertex(omm_star_ring_vtx + 48, 16, 0),
    gsSP2Triangles(0,  1,  2, 0, 0,  2,  3, 0),
    gsSP2Triangles(4,  5,  6, 0, 4,  6,  7, 0),
    gsSP2Triangles(8,  9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(1, 12, 13, 0, 1, 13,  2, 0),
    gsSP2Triangles(5, 14, 15, 0, 5, 15,  6, 0),
    gsSPVertex(omm_star_ring_vtx + 64, 16, 0),
    gsSP2Triangles(0,  1,  2, 0, 0,  2,  3, 0),
    gsSP2Triangles(4,  5,  6, 0, 4,  6,  7, 0),
    gsSP2Triangles(8,  9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(1, 12, 13, 0, 1, 13,  2, 0),
    gsSP2Triangles(5, 14, 15, 0, 5, 15,  6, 0),
    gsSPVertex(omm_star_ring_vtx + 80, 16, 0),
    gsSP2Triangles(0,  1,  2, 0, 0,  2,  3, 0),
    gsSP2Triangles(4,  5,  6, 0, 4,  6,  7, 0),
    gsSP2Triangles(8,  9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(1, 12, 13, 0, 1, 13,  2, 0),
    gsSP2Triangles(5, 14, 15, 0, 5, 15,  6, 0),
    gsSPVertex(omm_star_ring_vtx + 96, 16, 0),
    gsSP2Triangles(0,  1,  2, 0, 0,  2,  3, 0),
    gsSP2Triangles(4,  5,  6, 0, 4,  6,  7, 0),
    gsSP2Triangles(8,  9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(1, 12, 13, 0, 1, 13,  2, 0),
    gsSP2Triangles(5, 14, 15, 0, 5, 15,  6, 0),
    gsSPVertex(omm_star_ring_vtx + 112, 4, 0),
    gsSP2Triangles(0,  1,  2, 0, 0,  2,  3, 0),
    gsSPVertex(omm_star_ring_vtx + 116, 12, 0),
    gsSP2Triangles(0,  7,  2, 0, 0,  3,  7, 0),
    gsSP2Triangles(0,  8,  3, 0, 0,  4,  8, 0),
    gsSP2Triangles(0,  9,  4, 0, 0,  5,  9, 0),
    gsSP2Triangles(0, 10,  5, 0, 0,  6, 10, 0),
    gsSP2Triangles(0, 11,  6, 0, 0,  2, 11, 0),
    gsSP2Triangles(1,  2,  7, 0, 1,  7,  3, 0),
    gsSP2Triangles(1,  3,  8, 0, 1,  8,  4, 0),
    gsSP2Triangles(1,  4,  9, 0, 1,  9,  5, 0),
    gsSP2Triangles(1,  5, 10, 0, 1, 10,  6, 0),
    gsSP2Triangles(1,  6, 11, 0, 1, 11,  2, 0),
    gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPTexture(0x0F80, 0x07C0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

//
// Geo
//

const GeoLayout omm_geo_star_ring[] = {
    GEO_SCALE(0x00, 0x8000),
    GEO_OPEN_NODE(),
        GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
        GEO_OPEN_NODE(),
            GEO_ASM(0, geo_update_layer_transparency),
            GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, omm_star_ring_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_star_ring_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    switch (o->oAction) {

        // Init
        case 0: {
            o->oWaterRingMarioDistInFront = 0.f;
            o->oBehParams2ndByte = obj_get_count_with_behavior(omm_bhv_star_ring);
            o->oRingDot0 = 0.f;
            o->oRingDot1 = 0.f;
            o->oAction = 1;
        } break;

        // Not collected
        case 1: {
            obj_scale(o, relerp_f(coss(o->oTimer * 0x800), -1.f, 1.f, OMM_RING_SCALE - 0.02f, OMM_RING_SCALE + 0.02f));
            o->oOpacity = 0xC0;

            // Collision check
            bool collected = false;
            f32 dx = m->pos[0] - o->oPosX;
            f32 dz = m->pos[2] - o->oPosZ;
            f32 dy0 = m->pos[1] - o->oPosY + 40.f;
            f32 dy1 = m->pos[1] - o->oPosY + 120.f;

            // Horizontal ring, infinite vertical cylinder
            if (o->oFaceAnglePitch != 0) {
                if (vec3f_hdist(m->pos, &o->oPosX) < 120.f * o->oScaleX) {
                    collected |= (o->oRingDot0 * dy0) < 0.f;
                    collected |= (o->oRingDot1 * dy1) < 0.f;
                    o->oRingDot0 = dy0;
                    o->oRingDot1 = dy1;
                } else {
                    o->oRingDot0 = 0.f;
                    o->oRingDot1 = 0.f;
                }
            }
            
            // Vertical ring, infinite horizontal cylinder
            else {
                Vec3f n = { sins(o->oFaceAngleYaw), 0, coss(o->oFaceAngleYaw) };
                Vec3f d0 = { dx, dy0, dz };
                Vec3f d1 = { dx, dy1, dz };
                Vec3f p0; vec3f_project_point(p0, d0, gVec3fZero, n);
                Vec3f p1; vec3f_project_point(p1, d1, gVec3fZero, n);
                if (vec3f_length(p0) < 120.f * o->oScaleX ||
                    vec3f_length(p1) < 120.f * o->oScaleX) {
                    f32 dot0 = vec3f_dot(n, d0);
                    f32 dot1 = vec3f_dot(n, d1);
                    collected |= (o->oRingDot0 * dot0) < 0.f;
                    collected |= (o->oRingDot1 * dot1) < 0.f;
                    o->oRingDot0 = dot0;
                    o->oRingDot1 = dot1;
                } else {
                    o->oRingDot0 = 0.f;
                    o->oRingDot1 = 0.f;
                }
            }

            // Collected?
            if (collected) {
                s32 number = 1 + o->oBehParams2ndByte - obj_get_count_with_behavior_and_field_s32(omm_bhv_star_ring, 0x31, 1);
                omm_spawn_number(o, number);
                play_sound(SOUND_MENU_COLLECT_SECRET + (max_s(0, 4 + number - o->oBehParams2ndByte) << 16), gGlobalSoundArgs);
                o->oAction = 2;
            }
        } break;

        // Collected
        case 2: {
            f32 t = invlerp_0_1_f(o->oTimer, 0, 30);
            obj_scale(o, lerp_f(t, OMM_RING_SCALE, 2.f * OMM_RING_SCALE));
            o->oOpacity = lerp_s(t, 0xC0, 0x00);
            if (t == 1.f) {
                obj_mark_for_deletion(o);
            }
        } break;
    }
}

const BehaviorScript omm_bhv_star_ring[] = {
    OBJ_TYPE_LEVEL,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_star_ring_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_star_ring(struct Object *o, f32 x, f32 y, f32 z, bool vertical, s32 yaw) {
    struct Object *ring = obj_spawn_from_geo(o, omm_geo_star_ring, omm_bhv_star_ring);
    obj_set_pos(ring, x, y, z);
    obj_set_angle(ring, vertical ? 0 : 0x4000, vertical ? yaw : 0, 0);
    obj_scale(ring, 0);
    ring->oOpacity = 0;
    ring->oAction = 0;
    return ring;
}

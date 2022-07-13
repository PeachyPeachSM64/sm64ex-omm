#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Vtx omm_metal_sparkle_vertices[] = {
    { { { -32, 0, 0 }, 0, { 0, 992 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 32, 0, 0 }, 0, { 992, 992 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 32, 64, 0 }, 0, { 992, 0 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { -32, 64, 0 }, 0, { 0, 0 }, { 0xff, 0xff, 0xff, 0xff } } },
};

static const Gfx omm_metal_sparkle_triangles[] = {
    gsSPVertex(omm_metal_sparkle_vertices, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_metal_sparkle_shared_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(omm_metal_sparkle_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_metal_sparkle_1_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_2_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_3_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_3, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_4_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_4, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_5_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_5, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_6_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_6, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_7_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_7, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_8_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_8, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_9_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_9, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_10_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_10, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_11_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_11, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_12_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_12, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_13_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_13, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_14_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_14, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_15_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_15, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

static const Gfx omm_metal_sparkle_16_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_METAL_SPARKLE_16, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_metal_sparkle_shared_gfx),
};

//
// Geo layout
//

const GeoLayout omm_geo_metal_sparkle[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(16, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_2_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_3_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_4_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_5_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_6_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_7_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_8_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_9_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_10_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_11_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_12_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_13_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_14_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_15_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_16_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

const BehaviorScript omm_bhv_metal_sparkle[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x111AFFFF,
    0x05000010,
    0x0F1A0001,
    0x06000000,
    0x1D000000
};

//
// Spawner
//

struct Object *omm_spawn_metal_sparkle(struct Object *o) {
    struct Object *sparkle = obj_spawn_from_geo(o, omm_geo_metal_sparkle, omm_bhv_metal_sparkle);
    sparkle->activeFlags  |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    sparkle->oPosX         = o->oPosX;
    sparkle->oPosY         = o->oPosY + 50;
    sparkle->oPosZ         = o->oPosZ;
    sparkle->oOpacity      = 255;
    obj_translate_xyz_random(sparkle, 150.f);
    obj_scale_random(sparkle, 1.5f, 0.5f);
    return sparkle;
}
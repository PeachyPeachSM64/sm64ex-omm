#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx
//

static const Vtx omm_sparkle_vtx[] = {
    { { { -32,  0, 0 }, 0, {   0, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { {  32,  0, 0 }, 0, { 992, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { {  32, 64, 0 }, 0, { 992,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -32, 64, 0 }, 0, {   0,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

static const Gfx omm_sparkle_gfx[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPVertex(omm_sparkle_vtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkle_gfx_0[] = {
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b, 1, OMM_ASSET_SPARKLE_ANIMATION_0),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkle_gfx_1[] = {
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b, 1, OMM_ASSET_SPARKLE_ANIMATION_1),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkle_gfx_2[] = {
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b, 1, OMM_ASSET_SPARKLE_ANIMATION_2),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkle_gfx_3[] = {
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b, 1, OMM_ASSET_SPARKLE_ANIMATION_3),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkle_gfx_4[] = {
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b, 1, OMM_ASSET_SPARKLE_ANIMATION_4),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    Gfx gfx[OMM_ARRAY_SIZE(omm_sparkle_gfx)];
} OmmSparkleGeoData;

//
// Geo layout
//

const GeoLayout omm_geo_sparkle[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SWITCH_CASE(9, geo_switch_anim_state),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkle_gfx_0),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkle_gfx_1),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkle_gfx_2),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkle_gfx_3),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkle_gfx_4),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkle_gfx_3),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkle_gfx_2),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkle_gfx_1),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkle_gfx_0),
        GEO_CLOSE_NODE(),
        GEO_TRANSLATE_NODE(LAYER_TRANSPARENT, 0, 0, 0),
        GEO_OPEN_NODE(),
            GEO_ASM(0, geo_link_geo_data),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Spawner
//

struct Object *omm_spawn_sparkle(struct Object *o, u8 r, u8 g, u8 b, f32 translationRange, f32 minScale, f32 maxScale) {
    struct Object *sparkle = obj_spawn_from_geo(o, omm_geo_sparkle, bhvSparkle);
    obj_translate_xyz_random(sparkle, translationRange * 2.f);
    obj_scale_random(sparkle, maxScale - minScale, minScale);
    OmmSparkleGeoData *data = geo_get_geo_data(sparkle, sizeof(OmmSparkleGeoData), omm_sparkle_gfx, sizeof(omm_sparkle_gfx));
    data->gfx[0].words.w1 = (uintptr_t) (((r & 0xFF) << 24llu) | ((g & 0xFF) << 16llu) | ((b & 0xFF) << 8llu) | 0xFF);
    return sparkle;
}

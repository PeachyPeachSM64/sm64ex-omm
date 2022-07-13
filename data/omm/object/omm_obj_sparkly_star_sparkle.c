#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Vtx omm_sparkly_star_sparkle_vertices[] = {
    { { { -32, -32, 0 }, 0, { 0, 992 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 32, -32, 0 }, 0, { 992, 992 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 32, 32, 0 }, 0, { 992, 0 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { -32, 32, 0 }, 0, { 0, 0 }, { 0xff, 0xff, 0xff, 0xff } } },
};

static const Gfx omm_sparkly_star_sparkle_triangles[] = {
    gsSPVertex(omm_sparkly_star_sparkle_vertices, 4, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 2, 3, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_1_sparkle_1_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_1_SPARKLE_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_sparkle_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_1_sparkle_2_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_1_SPARKLE_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_sparkle_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_2_sparkle_1_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_2_SPARKLE_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_sparkle_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_2_sparkle_2_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_2_SPARKLE_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_sparkle_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_3_sparkle_1_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_3_SPARKLE_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_sparkle_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_3_sparkle_2_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_3_SPARKLE_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_sparkle_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

//
// Geo layouts
//

const GeoLayout omm_geo_sparkly_star_1_sparkle[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkly_star_1_sparkle_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkly_star_1_sparkle_2_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_sparkly_star_2_sparkle[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkly_star_2_sparkle_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkly_star_2_sparkle_2_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_sparkly_star_3_sparkle[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkly_star_3_sparkle_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_sparkly_star_3_sparkle_2_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_sparkly_star_sparkle_update() {
    struct Object *o = gCurrentObject;
    o->oPosX += o->oVelX;
    o->oPosY += o->oVelY;
    o->oPosZ += o->oVelZ;
    o->oAnimState++;
}

const BehaviorScript omm_bhv_sparkly_star_sparkle[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x05000020,
    0x0C000000, (uintptr_t) omm_bhv_sparkly_star_sparkle_update,
    0x06000000,
    0x1D000000
};

const BehaviorScript omm_bhv_sparkly_star_sparkle_mario[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x05000020,
    0x0C000000, (uintptr_t) omm_bhv_sparkly_star_sparkle_update,
    0x06000000,
    0x1D000000
};

//
// Spawner
//

struct Object *omm_spawn_sparkly_star_sparkle(struct Object *o, s32 mode, f32 yOffset, f32 vel, f32 scale, f32 offset) {
    struct Object *sparkle = obj_spawn_from_geo(o, OMM_SSX_SPARKLE_GEO[mode], omm_bhv_sparkly_star_sparkle);
    f32 vx = vel * (random_float() - 0.5f);
    f32 vy = vel * (random_float() - 0.5f);
    f32 vz = vel * (random_float() - 0.5f);
    sparkle->oPosY += yOffset;
    sparkle->oVelX = vx;
    sparkle->oVelY = vy;
    sparkle->oVelZ = vz;
    f32 dv = sqrtf(sqr_f(vx) + sqr_f(vy) + sqr_f(vz));
    if (dv != 0.f) {
        sparkle->oPosX += offset * (vx / dv);
        sparkle->oPosY += offset * (vy / dv);
        sparkle->oPosZ += offset * (vz / dv);
    }
    obj_scale_random(sparkle, scale, scale);
    sparkle->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    sparkle->oAnimState = (random_u16() & 1);
    sparkle->oSparklyStarMode = mode;
    return sparkle;
}

struct Object* omm_spawn_sparkly_star_sparkle_mario(struct Object* o, s32 mode, f32 yOffset, f32 vel, f32 scale, f32 offset) {
    struct Object *sparkle = omm_spawn_sparkly_star_sparkle(o, mode, yOffset, vel, scale, offset);
    sparkle->behavior = omm_bhv_sparkly_star_sparkle_mario;
    return sparkle;
}

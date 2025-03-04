#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Lights1 omm_sparkly_star_hint_light = gdSPDefLights1(
    0x7f, 0x7f, 0x7f,
    0xff, 0xff, 0xff,
    0x28, 0x28, 0x28
);

static const Vtx omm_sparkly_star_hint_vertices[] = {
    { { { 0, 504, 19 }, 0, { 962, 478 }, { 0x00, 0x7e, 0xfc, 0xff } } },
    { { { 24, 490, 34 }, 0, { 938, 780 }, { 0x62, 0x3b, 0x36, 0xff } } },
    { { { 0, 490, -6 }, 0, { 938, 478 }, { 0xff, 0x39, 0x90, 0xff } } },
    { { { -23, 490, 34 }, 0, { 938, 178 }, { 0x9e, 0x3a, 0x36, 0xff } } },
    { { { 0, -39, 34 }, 0, { 36, 478 }, { 0x00, 0xfc, 0x7e, 0xff } } },
    { { { -23, -39, -6 }, 0, { 36, 178 }, { 0x93, 0x00, 0xc1, 0xff } } },
    { { { 24, -39, -6 }, 0, { 36, 780 }, { 0x6d, 0x00, 0xc0, 0xff } } },
    { { { 180, 260, 20 }, 0, { 990, 990 }, { 0x00, 0x81, 0x00, 0xff } } },
    { { { -179, 260, 60 }, 0, { 0, 990 }, { 0x00, 0x81, 0x00, 0xff } } },
    { { { -179, 260, 20 }, 0, { 0, 990 }, { 0x00, 0x81, 0x00, 0xff } } },
    { { { 180, 260, 20 }, 0, { 990, 990 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -179, 260, 20 }, 0, { 0, 990 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -179, 460, 20 }, 0, { 0, 0 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -179, 260, 20 }, 0, { 0, 990 }, { 0x81, 0x00, 0x00, 0xff } } },
    { { { -179, 260, 60 }, 0, { 0, 990 }, { 0x81, 0x00, 0x00, 0xff } } },
    { { { -179, 460, 60 }, 0, { 0, 0 }, { 0x81, 0x00, 0x00, 0xff } } },
    { { { -179, 460, 20 }, 0, { 0, 0 }, { 0x81, 0x00, 0x00, 0xff } } },
    { { { 180, 260, 60 }, 0, { 990, 990 }, { 0x00, 0x81, 0x00, 0xff } } },
    { { { -179, 460, 20 }, 0, { 0, 0 }, { 0x00, 0x7f, 0x00, 0xff } } },
    { { { -179, 460, 60 }, 0, { 0, 0 }, { 0x00, 0x7f, 0x00, 0xff } } },
    { { { 180, 460, 60 }, 0, { 990, 0 }, { 0x00, 0x7f, 0x00, 0xff } } },
    { { { 180, 460, 20 }, 0, { 990, 0 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 180, 460, 20 }, 0, { 990, 0 }, { 0x00, 0x7f, 0x00, 0xff } } },
    { { { 180, 260, 60 }, 0, { 990, 990 }, { 0x7f, 0x00, 0x00, 0xff } } },
    { { { 180, 460, 20 }, 0, { 990, 0 }, { 0x7f, 0x00, 0x00, 0xff } } },
    { { { 180, 460, 60 }, 0, { 990, 0 }, { 0x7f, 0x00, 0x00, 0xff } } },
    { { { 180, 260, 20 }, 0, { 990, 990 }, { 0x7f, 0x00, 0x00, 0xff } } },
    { { { -179, 260, 60 }, 0, { 0, 990 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { 180, 460, 60 }, 0, { 990, 0 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { -179, 460, 60 }, 0, { 0, 0 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { 180, 260, 60 }, 0, { 990, 990 }, { 0x00, 0x00, 0x7f, 0xff } } },
};

static const Gfx omm_sparkly_star_hint_triangles[] = {
    gsSPVertex(omm_sparkly_star_hint_vertices, 7, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 2, 3, 0, 0x0),
    gsSP2Triangles(0, 3, 1, 0x0, 4, 1, 3, 0x0),
    gsSP2Triangles(3, 5, 4, 0x0, 5, 3, 2, 0x0),
    gsSP2Triangles(2, 6, 5, 0x0, 1, 4, 6, 0x0),
    gsSP1Triangle(6, 2, 1, 0x0),
    gsSPVertex(omm_sparkly_star_hint_vertices + 7, 16, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 3, 4, 5, 0x0),
    gsSP2Triangles(6, 7, 8, 0x0, 6, 8, 9, 0x0),
    gsSP2Triangles(0, 10, 1, 0x0, 11, 12, 13, 0x0),
    gsSP2Triangles(3, 5, 14, 0x0, 11, 13, 15, 0x0),
    gsSPVertex(omm_sparkly_star_hint_vertices + 23, 4, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 3, 1, 0x0),
    gsSPEndDisplayList(),

    gsSPVertex(omm_sparkly_star_hint_vertices + 27, 4, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 3, 1, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_1_hint_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPLight(&omm_sparkly_star_hint_light.l, 1),
    gsSPLight(&omm_sparkly_star_hint_light.a, 2),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_1_HINT_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0),
    gsSPDisplayList(omm_sparkly_star_hint_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_1_HINT_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0),
    gsSPDisplayList(omm_sparkly_star_hint_triangles + 14),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_2_hint_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPLight(&omm_sparkly_star_hint_light.l, 1),
    gsSPLight(&omm_sparkly_star_hint_light.a, 2),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_2_HINT_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0),
    gsSPDisplayList(omm_sparkly_star_hint_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_2_HINT_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0),
    gsSPDisplayList(omm_sparkly_star_hint_triangles + 14),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_3_hint_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPLight(&omm_sparkly_star_hint_light.l, 1),
    gsSPLight(&omm_sparkly_star_hint_light.a, 2),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_3_HINT_BACK, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0),
    gsSPDisplayList(omm_sparkly_star_hint_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_3_HINT_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0),
    gsSPDisplayList(omm_sparkly_star_hint_triangles + 14),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

//
// Geo layouts
//

const GeoLayout omm_geo_sparkly_star_1_hint[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0xC8, 60),
    GEO_OPEN_NODE(),
        GEO_SCALE(0x00, 16384),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_sparkly_star_1_hint_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_sparkly_star_2_hint[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0xC8, 60),
    GEO_OPEN_NODE(),
        GEO_SCALE(0x00, 16384),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_sparkly_star_2_hint_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_sparkly_star_3_hint[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0xC8, 60),
    GEO_OPEN_NODE(),
        GEO_SCALE(0x00, 16384),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_sparkly_star_3_hint_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

const BehaviorScript bhvOmmSparklyStarHint[] = {
    OBJ_TYPE_SURFACE,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_LOAD_COLLISION_DATA(wooden_signpost_seg3_collision_0302DD80),
    BHV_SET_INTERACT_TYPE(INTERACT_TEXT),
    BHV_SET_INT(oInteractionSubtype, INT_SUBTYPE_SIGN),
    BHV_SET_HITBOX(150, 80),
    BHV_SET_INT(oWoodenPostTotalMarioAngle, 0),
    BHV_BEGIN_LOOP(),
        BHV_SET_INT(oIntangibleTimer, 0),
        BHV_CALL_NATIVE(load_object_collision_model),
        BHV_SET_INT(oInteractStatus, 0),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_sparkly_star_hint(struct Object *o, s32 sparklyMode, f32 x, f32 y, f32 z, s16 yaw, s32 dialogId) {
    struct Object *sign = obj_spawn_from_geo(o, OMM_SPARKLY_HINT_GEO[sparklyMode], bhvOmmSparklyStarHint);
    obj_set_xyz(sign, x, y, z);
    obj_set_angle(sign, 0, yaw, 0);
    obj_drop_to_floor(sign);
    sign->oBehParams = (dialogId << 16);
    sign->oBehParams2ndByte = dialogId;
    sign->oSparklyStarMode = sparklyMode;
    return sign;
}




#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

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

static void bhv_omm_sparkly_star_sparkle_update() {
    struct Object *o = gCurrentObject;
    if (o->parentObj != o) {
        o->oPosX = o->parentObj->oPosX + o->oHomeX + o->oVelX * o->oTimer;
        o->oPosY = o->parentObj->oPosY + o->oHomeY + o->oVelY * o->oTimer;
        o->oPosZ = o->parentObj->oPosZ + o->oHomeZ + o->oVelZ * o->oTimer;
    } else {
        o->oPosX += o->oVelX;
        o->oPosY += o->oVelY;
        o->oPosZ += o->oVelZ;
    }
    o->oAnimState++;
    if (o->oTimer > o->oAction) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmSparklyStarSparkle[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_REPEAT(30),
        BHV_CALL_NATIVE(bhv_omm_sparkly_star_sparkle_update),
    BHV_END_REPEAT(),
    BHV_DEACTIVATE(),
};

const BehaviorScript bhvOmmSparklyStarSparkleMario[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_REPEAT(30),
        BHV_CALL_NATIVE(bhv_omm_sparkly_star_sparkle_update),
    BHV_END_REPEAT(),
    BHV_DEACTIVATE(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_sparkly_star_sparkle(struct Object *o, s32 sparklyMode, f32 yOffset, f32 vel, f32 scale, f32 offset) {
    struct Object *sparkle = obj_spawn_from_geo(o, OMM_SPARKLY_SPARKLE_GEO[sparklyMode], bhvOmmSparklyStarSparkle);
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
    sparkle->oSparklyStarMode = sparklyMode;
    sparkle->oAction = 30;
    sparkle->parentObj = sparkle;
    return sparkle;
}

struct Object *omm_obj_spawn_sparkly_star_sparkle_star_spawn(struct Object *o, s32 sparklyMode, s32 duration, f32 scale, f32 offset) {
    struct Object *sparkle = obj_spawn_from_geo(o, OMM_SPARKLY_SPARKLE_GEO[sparklyMode], bhvOmmSparklyStarSparkle);
    duration = min_s(duration, 30);
    f32 vx = (offset / (f32) duration) * (random_float() - 0.5f);
    f32 vy = (offset / (f32) duration) * (random_float() - 0.5f);
    f32 vz = (offset / (f32) duration) * (random_float() - 0.5f);
    sparkle->oHomeX = duration * -vx;
    sparkle->oHomeY = duration * -vy;
    sparkle->oHomeZ = duration * -vz;
    sparkle->oPosX += sparkle->oHomeX;
    sparkle->oPosY += sparkle->oHomeY;
    sparkle->oPosZ += sparkle->oHomeZ;
    sparkle->oVelX = vx;
    sparkle->oVelY = vy;
    sparkle->oVelZ = vz;
    obj_scale_random(sparkle, scale, scale);
    sparkle->activeFlags &= ~ACTIVE_FLAG_UNIMPORTANT;
    sparkle->oAnimState = (random_u16() & 1);
    sparkle->oSparklyStarMode = sparklyMode;
    sparkle->oAction = duration;
    return sparkle;
}

struct Object *omm_obj_spawn_sparkly_star_sparkle_mario(struct Object* o, s32 sparklyMode, f32 yOffset, f32 vel, f32 scale, f32 offset) {
    struct Object *sparkle = omm_obj_spawn_sparkly_star_sparkle(o, sparklyMode, yOffset, vel, scale, offset);
    sparkle->behavior = bhvOmmSparklyStarSparkleMario;
    return sparkle;
}

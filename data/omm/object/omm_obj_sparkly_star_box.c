#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Lights1 omm_sparkly_star_box_lights = gdSPDefLights1(
    0x7F, 0x7F, 0x7F,
    0xFF, 0xFF, 0xFF,
    0x28, 0x28, 0x28
);

static const Vtx omm_sparkly_star_box_vtx[] = {
    { { {  150, 300, -150 }, 0, { 990,    0 }, { 0x00, 0x7F, 0x00, 0xFF } } },
    { { { -150, 300, -150 }, 0, {   0,    0 }, { 0x00, 0x7F, 0x00, 0xFF } } },
    { { { -150, 300,  150 }, 0, {   0, 2012 }, { 0x00, 0x7F, 0x00, 0xFF } } },
    { { {  150, 300,  150 }, 0, { 990, 2012 }, { 0x00, 0x7F, 0x00, 0xFF } } },
    { { { -150,   0, -150 }, 0, { 990, 2012 }, { 0x00, 0x00, 0x81, 0xFF } } },
    { { { -150, 300, -150 }, 0, { 990,    0 }, { 0x00, 0x00, 0x81, 0xFF } } },
    { { {  150, 300, -150 }, 0, {   0,    0 }, { 0x00, 0x00, 0x81, 0xFF } } },
    { { {  150,   0, -150 }, 0, {   0, 2012 }, { 0x00, 0x00, 0x81, 0xFF } } },
    { { { -150,   0,  150 }, 0, { 990, 2012 }, { 0x81, 0x00, 0x00, 0xFF } } },
    { { { -150, 300,  150 }, 0, { 990,    0 }, { 0x81, 0x00, 0x00, 0xFF } } },
    { { { -150, 300, -150 }, 0, {   0,    0 }, { 0x81, 0x00, 0x00, 0xFF } } },
    { { { -150,   0, -150 }, 0, {   0, 2012 }, { 0x81, 0x00, 0x00, 0xFF } } },
    { { {  150,   0,  150 }, 0, { 990,    0 }, { 0x00, 0x81, 0x00, 0xFF } } },
    { { { -150,   0,  150 }, 0, {   0,    0 }, { 0x00, 0x81, 0x00, 0xFF } } },
    { { { -150,   0, -150 }, 0, {   0, 2012 }, { 0x00, 0x81, 0x00, 0xFF } } },
    { { {  150,   0, -150 }, 0, { 990, 2012 }, { 0x00, 0x81, 0x00, 0xFF } } },
    { { {  150,   0,  150 }, 0, { 990, 2012 }, { 0x00, 0x00, 0x7F, 0xFF } } },
    { { { -150, 300,  150 }, 0, {   0,    0 }, { 0x00, 0x00, 0x7F, 0xFF } } },
    { { { -150,   0,  150 }, 0, {   0, 2012 }, { 0x00, 0x00, 0x7F, 0xFF } } },
    { { {  150, 300,  150 }, 0, { 990,    0 }, { 0x00, 0x00, 0x7F, 0xFF } } },
    { { {  150,   0, -150 }, 0, { 990, 2012 }, { 0x7F, 0x00, 0x00, 0xFF } } },
    { { {  150, 300, -150 }, 0, { 990,    0 }, { 0x7F, 0x00, 0x00, 0xFF } } },
    { { {  150, 300,  150 }, 0, {   0,    0 }, { 0x7F, 0x00, 0x00, 0xFF } } },
    { { {  150,   0,  150 }, 0, {   0, 2012 }, { 0x7F, 0x00, 0x00, 0xFF } } },
};

static const Gfx omm_sparkly_star_box_tri[] = {
    gsSPLight(&omm_sparkly_star_box_lights.l, 1),
    gsSPLight(&omm_sparkly_star_box_lights.a, 2),
    gsSPVertex(omm_sparkly_star_box_vtx, 24, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  2,  3, 0x0),
    gsSP2Triangles( 4,  5,  6, 0x0,  4,  6,  7, 0x0),
    gsSP2Triangles( 8,  9, 10, 0x0,  8, 10, 11, 0x0),
    gsSP2Triangles(12, 13, 14, 0x0, 12, 14, 15, 0x0),
    gsSP2Triangles(16, 17, 18, 0x0, 16, 19, 17, 0x0),
    gsSP2Triangles(20, 21, 22, 0x0, 20, 22, 23, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_1_box_gfx[] = {
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_1_BOX_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 64, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(omm_sparkly_star_box_tri),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_2_box_gfx[] = {
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_2_BOX_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 64, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(omm_sparkly_star_box_tri),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_3_box_gfx[] = {
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_3_BOX_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 64, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(omm_sparkly_star_box_tri),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_sparkly_star_1_box[] = {
    GEO_SHADOW(SHADOW_SQUARE_PERMANENT, 0xB4, 70),
    GEO_OPEN_NODE(),
        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_sparkly_star_1_box_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_sparkly_star_2_box[] = {
    GEO_SHADOW(SHADOW_SQUARE_PERMANENT, 0xB4, 70),
    GEO_OPEN_NODE(),
        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_sparkly_star_2_box_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_sparkly_star_3_box[] = {
    GEO_SHADOW(SHADOW_SQUARE_PERMANENT, 0xB4, 70),
    GEO_OPEN_NODE(),
        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_sparkly_star_3_box_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

const BehaviorScript bhvOmmSparklyStarBox[] = {
    OBJ_TYPE_SURFACE,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_LOAD_COLLISION_DATA(metal_box_seg8_collision_08024C28),
    BHV_SET_HOME(),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_pushable_loop),
        BHV_CALL_NATIVE(load_object_collision_model),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_sparkly_star_box(struct Object *o, s32 sparklyMode, f32 x, f32 y, f32 z, f32 maxHeight) {
    struct Object *box = obj_spawn_from_geo(o, OMM_SPARKLY_BOX_GEO[sparklyMode], bhvOmmSparklyStarBox);
    obj_set_always_rendered(box, true);
    obj_set_xyz(box, x, y, z);
    obj_set_angle(box, 0, 0, 0);
    obj_scale(box, 1.f);
    box->oSparklyStarMode = sparklyMode;
    box->oSparklyStarBoxMaxHeight = maxHeight;
    return box;
}

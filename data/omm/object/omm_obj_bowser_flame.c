#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Vtx omm_bowser_flame_vertices[] = {
    { { { -50, 0, 0 }, 0, { 512, 2944 }, { 255, 255, 255, 255 } } },
    { { { -50, 150, 0 }, 0, { 512, 0 }, { 255, 255, 255, 255 } } },
    { { { +50, 0, 0 }, 0, { 2560, 2944 }, { 255, 255, 255, 255 } } },
    { { { +50, 150, 0 }, 0, { 2560, 0 }, { 255, 255, 255, 255 } } },
    { { { -50, 0, 0 }, 0, { 3584, 2944 }, { 255, 255, 255, 255 } } },
    { { { -50, 150, 0 }, 0, { 3584, 0 }, { 255, 255, 255, 255 } } },
    { { { +50, 0, 0 }, 0, { 5632, 2944 }, { 255, 255, 255, 255 } } },
    { { { +50, 150, 0 }, 0, { 5632, 0 }, { 255, 255, 255, 255 } } },
    { { { -50, 0, 0 }, 0, { 512, 6016 }, { 255, 255, 255, 255 } } },
    { { { -50, 150, 0 }, 0, { 512, 3072 }, { 255, 255, 255, 255 } } },
    { { { +50, 0, 0 }, 0, { 2560, 6016 }, { 255, 255, 255, 255 } } },
    { { { +50, 150, 0 }, 0, { 2560, 3072 }, { 255, 255, 255, 255 } } },
    { { { -50, 0, 0 }, 0, { 3584, 6016 }, { 255, 255, 255, 255 } } },
    { { { -50, 150, 0 }, 0, { 3584, 3072 }, { 255, 255, 255, 255 } } },
    { { { +50, 0, 0 }, 0, { 5632, 6016 }, { 255, 255, 255, 255 } } },
    { { { +50, 150, 0 }, 0, { 5632, 3072 }, { 255, 255, 255, 255 } } },
};

static const Gfx omm_bowser_flame_0_triangles[] = {
    gsSPVertex(omm_bowser_flame_vertices + 0, 4, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 2, 3, 1, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_flame_1_triangles[] = {
    gsSPVertex(omm_bowser_flame_vertices + 4, 4, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 2, 3, 1, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_flame_2_triangles[] = {
    gsSPVertex(omm_bowser_flame_vertices + 8, 4, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 2, 3, 1, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_flame_3_triangles[] = {
    gsSPVertex(omm_bowser_flame_vertices + 12, 4, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 2, 3, 1, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_flame_0_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_BREATH, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_bowser_flame_0_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_flame_1_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_BREATH, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_bowser_flame_1_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_flame_2_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_BREATH, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_bowser_flame_2_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_flame_3_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_BREATH, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_bowser_flame_3_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_bowser_flame[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(4, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_flame_0_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_flame_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_flame_2_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_flame_3_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_bowser_flame_loop() {
    struct Object *o = gCurrentObject;
    o->oGraphYOffset = 0;
    o->oOpacity = 255;

    switch (o->oAction) {

        // Gets bigger
        case 0: {
            f32 t = clamp_0_1_f(o->oTimer / 30.f);
            o->oScaleX = 3.f * t;
            o->oScaleY = 4.f * t;
            o->oScaleZ = 3.f * t;
            if (o->oTimer > 30) {
                o->oAction = 1;
                o->oTimer = 0;
            }
        } break;

        // Stays big for 'duration' frames
        case 1: {
            o->oScaleX = 3.f;
            o->oScaleY = 4.f;
            o->oScaleZ = 3.f;
            if (o->oTimer > o->oBowserFireDuration) {
                o->oAction = 2;
                o->oTimer = 0;
            }
        } break;

        // Gets smaller and vanishes
        case 2: {
            f32 t = 1.f - clamp_0_1_f(o->oTimer / 20.f);
            o->oScaleX = 3.f * t;
            o->oScaleY = 4.f * t;
            o->oScaleZ = 3.f * t;
            if (o->oTimer > 20) {
                obj_mark_for_deletion(o);
                return;
            }
        } break;
    }

    // Doesn't burn Mario if too small
    if (o->oScaleY >= 1.f) {
        obj_set_params(o, INTERACT_FLAME, 1, 99, 0, true);
        obj_reset_hitbox(o, 30, 50, 0, 0, 0, 0);
    } else {
        obj_set_params(o, 0, 0, 99, 0, false);
        obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    }
}

const BehaviorScript omm_bhv_bowser_flame[] = {
    OBJ_TYPE_GENACTOR,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) bhv_omm_bowser_flame_loop,
    0x0F1A0001,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_bowser_flame(struct Object *o, f32 x, f32 y, f32 z, s32 duration) {
    struct Object *flame = obj_spawn_from_geo(o, omm_geo_bowser_flame, omm_bhv_bowser_flame);
    obj_set_pos(flame, x, y, z);
    flame->oAction = 0;
    flame->oAnimState = random_u16() % 4;
    flame->oBowserFireDuration = duration;
    play_sound(SOUND_AIR_BOWSER_SPIT_FIRE, o->oCameraToObject);
    return flame;
}

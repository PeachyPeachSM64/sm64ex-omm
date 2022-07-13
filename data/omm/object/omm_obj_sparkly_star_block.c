#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Lights1 omm_sparkly_star_block_light = gdSPDefLights1(
    0x3f, 0x3f, 0x3f,
    0xff, 0xff, 0xff,
    0x28, 0x28, 0x28
);

static const Vtx omm_sparkly_star_block_vertices[] = {
    { { { -25, 1, 26 }, 0, { 992, 992 }, { 0x81, 0x00, 0x00, 0xff } } },
    { { { -25, 52, 26 }, 0, { 992, 0 }, { 0x81, 0x00, 0x00, 0xff } } },
    { { { -25, 52, -25 }, 0, { 0, 0 }, { 0x81, 0x00, 0x00, 0xff } } },
    { { { 26, 1, 26 }, 0, { 992, 992 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { 26, 52, 26 }, 0, { 992, 0 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { -25, 52, 26 }, 0, { 0, 0 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { -25, 1, 26 }, 0, { 0, 992 }, { 0x00, 0x00, 0x7f, 0xff } } },
    { { { 26, 1, -25 }, 0, { 992, 992 }, { 0x7f, 0x00, 0x00, 0xff } } },
    { { { 26, 52, -25 }, 0, { 992, 0 }, { 0x7f, 0x00, 0x00, 0xff } } },
    { { { 26, 52, 26 }, 0, { 0, 0 }, { 0x7f, 0x00, 0x00, 0xff } } },
    { { { 26, 1, 26 }, 0, { 0, 992 }, { 0x7f, 0x00, 0x00, 0xff } } },
    { { { -25, 1, -25 }, 0, { 992, 992 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 26, 52, -25 }, 0, { 0, 0 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { 26, 1, -25 }, 0, { 0, 992 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -25, 52, -25 }, 0, { 992, 0 }, { 0x00, 0x00, 0x81, 0xff } } },
    { { { -25, 1, -25 }, 0, { 0, 992 }, { 0x81, 0x00, 0x00, 0xff } } },
    { { { -25, 52, 26 }, 0, { 0, 992 }, { 0x00, 0x7f, 0x00, 0xff } } },
    { { { 26, 52, 26 }, 0, { 992, 992 }, { 0x00, 0x7f, 0x00, 0xff } } },
    { { { 26, 52, -25 }, 0, { 992, 0 }, { 0x00, 0x7f, 0x00, 0xff } } },
    { { { -25, 52, -25 }, 0, { 0, 0 }, { 0x00, 0x7f, 0x00, 0xff } } },
    { { { -25, 1, 26 }, 0, { 0, 0 }, { 0x00, 0x81, 0x00, 0xff } } },
    { { { -25, 1, -25 }, 0, { 0, 992 }, { 0x00, 0x81, 0x00, 0xff } } },
    { { { 26, 1, -25 }, 0, { 992, 992 }, { 0x00, 0x81, 0x00, 0xff } } },
    { { { 26, 1, 26 }, 0, { 992, 0 }, { 0x00, 0x81, 0x00, 0xff } } },
};

static const Gfx omm_sparkly_star_block_triangles[] = {
    gsSPVertex(omm_sparkly_star_block_vertices, 16, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 3, 4, 5, 0x0),
    gsSP2Triangles(3, 5, 6, 0x0, 7, 8, 9, 0x0),
    gsSP2Triangles(7, 9, 10, 0x0, 11, 12, 13, 0x0),
    gsSP2Triangles(11, 14, 12, 0x0, 0, 2, 15, 0x0),
    gsSPEndDisplayList(),
    gsSPVertex(omm_sparkly_star_block_vertices + 16, 8, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 2, 3, 0x0),
    gsSP2Triangles(4, 5, 6, 0x0, 4, 6, 7, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_block_1_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPLight(&omm_sparkly_star_block_light.l, 1),
    gsSPLight(&omm_sparkly_star_block_light.a, 2),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_1_BLOCK_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_block_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_1_BLOCK_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_block_triangles + 6),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_block_2_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPLight(&omm_sparkly_star_block_light.l, 1),
    gsSPLight(&omm_sparkly_star_block_light.a, 2),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_2_BLOCK_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_block_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_2_BLOCK_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_block_triangles + 6),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_star_block_3_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPLight(&omm_sparkly_star_block_light.l, 1),
    gsSPLight(&omm_sparkly_star_block_light.a, 2),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_3_BLOCK_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_block_triangles + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_SPARKLY_STAR_3_BLOCK_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_sparkly_star_block_triangles + 6),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

//
// Geo layouts
//

const GeoLayout omm_geo_sparkly_star_1_block[] = {
    GEO_SHADOW(SHADOW_SQUARE_PERMANENT, 0xB4, 70),
    GEO_OPEN_NODE(),
        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_sparkly_star_block_1_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_sparkly_star_2_block[] = {
    GEO_SHADOW(SHADOW_SQUARE_PERMANENT, 0xB4, 70),
    GEO_OPEN_NODE(),
        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_sparkly_star_block_2_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_sparkly_star_3_block[] = {
    GEO_SHADOW(SHADOW_SQUARE_PERMANENT, 0xB4, 70),
    GEO_OPEN_NODE(),
        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_sparkly_star_block_3_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_sparkly_star_block_update() {
    struct Object *o = gCurrentObject;
    switch (o->oAction) {
        case 0: {
            load_object_collision_model();
            if (cur_obj_was_attacked_or_ground_pounded()) {
                o->oIntangibleTimer = -1;
                o->oHomeY = o->oPosY;
                o->oAction = 1;
                o->oTimer = 0;
            }
        } break;

        case 1: {
            o->oScaleX *= 1.1f;
            o->oScaleY *= 1.1f;
            o->oScaleZ *= 1.1f;
            o->oPosY = o->oHomeY - (o->oScaleY - 2.5f) * 25.f;
            if (o->oTimer == 6) {
                for (s32 i = 0; i != 60; ++i) {
                    omm_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, o->oScaleY * 25.f, 60.f, 0.8f, 80.f);
                }
                o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                play_sound(SOUND_GENERAL_BREAK_BOX, o->oCameraToObject);
                set_environmental_camera_shake(SHAKE_ENV_FALLING_BITS_PLAT);
                o->oAction = 2;
                o->oTimer = 0;
            }
        } break;

        case 2: {
        } break;

        case 3: {
            obj_scale(o, 0.f);
            if (o->oTimer == 20) {
                audio_play_puzzle_jingle();
                obj_mark_for_deletion(o);
            }
        } break;
    }

    // Unloads the Block if the corresponding Sparkly Stars mode is enabled
    if (o->oAction == 0 && omm_ssd_is_selected(o->oSparklyStarMode)) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript omm_bhv_sparkly_star_1_block[] = {
    OBJ_TYPE_SURFACE,
    0x11010001,
    0x2A000000, (uintptr_t) exclamation_box_outline_seg8_collision_08025F78,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_sparkly_star_block_update,
    0x09000000,
};

const BehaviorScript omm_bhv_sparkly_star_2_block[] = {
    OBJ_TYPE_SURFACE,
    0x11010001,
    0x2A000000, (uintptr_t) exclamation_box_outline_seg8_collision_08025F78,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_sparkly_star_block_update,
    0x09000000,
};

const BehaviorScript omm_bhv_sparkly_star_3_block[] = {
    OBJ_TYPE_SURFACE,
    0x11010001,
    0x2A000000, (uintptr_t) exclamation_box_outline_seg8_collision_08025F78,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_sparkly_star_block_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_sparkly_star_block(struct Object *o, s32 mode, f32 x, f32 y, f32 z) {
    struct Object *block = obj_spawn_from_geo(o, OMM_SSX_BLOCK_GEO[mode], OMM_SSX_BLOCK_BHV[mode]);
    obj_set_always_rendered(block, true);
    obj_set_pos(block, x, y, z);
    obj_set_angle(block, 0, 0, 0);
    obj_scale(block, 2.5f);
    obj_set_params(block, INTERACT_BREAKABLE, 0, 1, 0, true);
    obj_reset_hitbox(block, 30, 50, 30, 50, 0, 0);
    obj_update_gfx(block);
    block->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    block->oSparklyStarMode = mode;
    return block;
}

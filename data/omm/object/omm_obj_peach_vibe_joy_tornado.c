#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Lights1 omm_peach_vibe_joy_tornado_light = gdSPDefLights1(
    0xFF, 0xFF, 0x60,
    0xFF, 0xFF, 0x60,
    0x28, 0x28, 0x28
);

static const Vtx omm_peach_vibe_joy_tornado_vertices[] = {
    { { { 93, 129, 161 }, 0, { 650, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 129, 185 }, 0, { 480, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 564, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 161, 129, 93 }, 0, { 820, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 734, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 189, 802, 328 }, 0, { 650, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 802, 379 }, 0, { 480, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 328, 802, 189 }, 0, { 820, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -92, 129, 161 }, 0, { 308, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -188, 802, 328 }, 0, { 308, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 394, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 185, 129, 0 }, 0, { 990, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 904, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 379, 802, 0 }, 0, { 990, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -92, 129, -160 }, 0, { 1672, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -188, 802, -327 }, 0, { 1672, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 802, -378 }, 0, { 1502, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 161, 129, -92 }, 0, { 1160, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 379, 802, 0 }, 0, { 990, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 185, 129, 0 }, 0, { 990, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 328, 802, -188 }, 0, { 1160, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 1076, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 93, 129, -160 }, 0, { 1330, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 1246, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 189, 802, -327 }, 0, { 1330, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 129, -184 }, 0, { 1502, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 1416, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -92, 129, 161 }, 0, { 308, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -160, 129, 93 }, 0, { 138, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 224, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -92, 129, -160 }, 0, { 1672, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 129, -184 }, 0, { 1502, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 1586, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -160, 129, -92 }, 0, { 1842, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 1756, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -188, 802, -327 }, 0, { 1672, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -327, 802, -188 }, 0, { 1842, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -184, 129, 0 }, 0, { 2012, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 1926, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -378, 802, 0 }, 0, { 2012, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -160, 129, 93 }, 0, { 138, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -184, 129, 0 }, 0, { 0, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { 54, 2012 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -378, 802, 0 }, 0, { 0, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -92, 129, 161 }, 0, { 308, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -188, 802, 328 }, 0, { 308, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -327, 802, 189 }, 0, { 138, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -160, 129, 93 }, 0, { 138, 1708 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -378, 802, 0 }, 0, { 0, 128 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

static const Gfx omm_peach_vibe_joy_tornado_triangles[] = {
    gsSPVertex(omm_peach_vibe_joy_tornado_vertices + 0, 14, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 3, 0, 4, 0x0),
    gsSP2Triangles(0, 5, 6, 0x0, 0, 6, 1, 0x0),
    gsSP2Triangles(3, 5, 0, 0x0, 3, 7, 5, 0x0),
    gsSP2Triangles(1, 6, 8, 0x0, 6, 9, 8, 0x0),
    gsSP2Triangles(1, 8, 10, 0x0, 11, 3, 12, 0x0),
    gsSP2Triangles(11, 13, 3, 0x0, 13, 7, 3, 0x0),
    gsSPVertex(omm_peach_vibe_joy_tornado_vertices + 14, 16, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 3, 4, 5, 0x0),
    gsSP2Triangles(3, 6, 4, 0x0, 3, 5, 7, 0x0),
    gsSP2Triangles(8, 3, 9, 0x0, 8, 6, 3, 0x0),
    gsSP2Triangles(8, 10, 6, 0x0, 11, 8, 12, 0x0),
    gsSP2Triangles(2, 10, 8, 0x0, 11, 2, 8, 0x0),
    gsSP2Triangles(0, 2, 11, 0x0, 13, 14, 15, 0x0),
    gsSPVertex(omm_peach_vibe_joy_tornado_vertices + 30, 14, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 3, 0, 4, 0x0),
    gsSP2Triangles(3, 5, 0, 0x0, 3, 6, 5, 0x0),
    gsSP2Triangles(7, 3, 8, 0x0, 9, 6, 3, 0x0),
    gsSP2Triangles(7, 9, 3, 0x0, 10, 11, 12, 0x0),
    gsSP1Triangle(10, 13, 11, 0x0),
    gsSPVertex(omm_peach_vibe_joy_tornado_vertices + 44, 5, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 2, 3, 0x0),
    gsSP1Triangle(3, 2, 4, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_peach_vibe_joy_tornado_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_VIBE_JOY_TORNADO, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 64, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_peach_vibe_joy_tornado_light.l, 1),
    gsSPLight(&omm_peach_vibe_joy_tornado_light.a, 2),
    gsSPDisplayList(omm_peach_vibe_joy_tornado_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_peach_vibe_joy_tornado[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(10, geo_update_layer_transparency),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_joy_tornado_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_peach_vibe_joy_tornado_update() {
    struct Object *o = gCurrentObject;
    struct MarioState *m = gMarioState;
    if (!omm_peach_vibe_is_joy()) {
        o->oTimer = clamp_s(o->oTimer - 2, 0, 15);
        if (o->oTimer == 0) {
            obj_mark_for_deletion(o);
        }
    }

    // Update gfx
    f32 t = invlerp_0_1_s(o->oTimer, 0, 15);
    obj_set_pos(o, m->pos[0], m->pos[1], m->pos[2]);
    obj_set_angle(o, 0, o->oFaceAngleYaw + 0x1800, 0);
    obj_set_scale(o, 0.4f * m->marioObj->oScaleX, 0.3f * m->marioObj->oScaleY, 0.4f * m->marioObj->oScaleZ);
    obj_update_gfx(o);
    o->oForwardVel = m->forwardVel;
    o->oGfxPos[1] -= 50.f * m->marioObj->oScaleY;
    o->oOpacity = 0x60 * t;

    // Produce a sparkle every 6 frames
    if (o->oTimer % 6 == 0) {
        f32 height = lerp_f(random_float(), 0.f, 160.f);
        f32 radius = relerp_0_1_f(height, 0.f, 160.f, 150.f, 200.f);
        omm_spawn_peach_vibe_joy_sparkle(
            m->marioObj,
            radius * m->marioObj->oScaleX,
            height * m->marioObj->oScaleY,
            (s16) random_u16(),
            lerp_s(random_float(), 0x400, 0xC00),
            min_s(0xFF, o->oOpacity * 1.5f)
        );
    }
    
    // Repel enemies, attract and collect coins
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 300, 600, 0, 0, 0, 0);
    omm_obj_process_interactions(o, OBJ_INT_PRESET_PEACH_VIBE_JOY_TORNADO);
}

const BehaviorScript omm_bhv_peach_vibe_joy_tornado[] = {
    OBJ_TYPE_SPECIAL,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_peach_vibe_joy_tornado_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_peach_vibe_joy_tornado(struct Object *o) {
    struct Object *tornado = obj_get_first_with_behavior(omm_bhv_peach_vibe_joy_tornado);
    if (tornado == NULL) {
        tornado = obj_spawn_from_geo(o, omm_geo_peach_vibe_joy_tornado, omm_bhv_peach_vibe_joy_tornado);
        obj_set_always_rendered(tornado, true);
        obj_set_angle(tornado, 0, 0, 0);
    }
    return tornado;
}

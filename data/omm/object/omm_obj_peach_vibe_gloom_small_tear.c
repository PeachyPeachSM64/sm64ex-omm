#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Lights1 omm_peach_vibe_gloom_tear_small_light_1 = gdSPDefLights1(
    0x80, 0xC0, 0xFF,
    0x80, 0xC0, 0xFF,
    0x28, 0x28, 0x28
);

static const Lights1 omm_peach_vibe_gloom_tear_small_light_2 = gdSPDefLights1(
    0x00, 0x80, 0xFF,
    0x00, 0x80, 0xFF,
    0x28, 0x28, 0x28
);

static const Vtx omm_peach_vibe_gloom_tear_small_vertices[] = {
    { { { 0, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 0, 8, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 3, 7, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 6, 6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 7, 3, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 8, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 7, -3, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 6, -6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 3, -7, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 0, -8, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -3, -7, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -6, -6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -7, -3, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -8, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -7, 3, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -6, 6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -3, 7, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 0, 32, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 12, 30, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 23, 23, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 30, 12, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 32, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 30, -12, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 23, -23, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 12, -30, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { 0, -32, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -12, -30, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -23, -23, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -30, -12, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -32, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -30, 12, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -23, 23, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
    { { { -12, 30, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x80 } } },
};

static const Gfx omm_peach_vibe_gloom_tear_small_triangles[] = {
    gsSP1Triangle(1, 0, 2, 0),
    gsSP1Triangle(2, 0, 3, 0),
    gsSP1Triangle(3, 0, 4, 0),
    gsSP1Triangle(4, 0, 5, 0),
    gsSP1Triangle(5, 0, 6, 0),
    gsSP1Triangle(6, 0, 7, 0),
    gsSP1Triangle(7, 0, 8, 0),
    gsSP1Triangle(8, 0, 9, 0),
    gsSP1Triangle(9, 0, 10, 0),
    gsSP1Triangle(10, 0, 11, 0),
    gsSP1Triangle(11, 0, 12, 0),
    gsSP1Triangle(12, 0, 13, 0),
    gsSP1Triangle(13, 0, 14, 0),
    gsSP1Triangle(14, 0, 15, 0),
    gsSP1Triangle(15, 0, 16, 0),
    gsSP1Triangle(16, 0, 1, 0),
    gsSP2Triangles(1, 2, 17, 0, 17, 2, 18, 0),
    gsSP2Triangles(2, 3, 18, 0, 18, 3, 19, 0),
    gsSP2Triangles(3, 4, 19, 0, 19, 4, 20, 0),
    gsSP2Triangles(4, 5, 20, 0, 20, 5, 21, 0),
    gsSP2Triangles(5, 6, 21, 0, 21, 6, 22, 0),
    gsSP2Triangles(6, 7, 22, 0, 22, 7, 23, 0),
    gsSP2Triangles(7, 8, 23, 0, 23, 8, 24, 0),
    gsSP2Triangles(8, 9, 24, 0, 24, 9, 25, 0),
    gsSP2Triangles(9, 10, 25, 0, 25, 10, 26, 0),
    gsSP2Triangles(10, 11, 26, 0, 26, 11, 27, 0),
    gsSP2Triangles(11, 12, 27, 0, 27, 12, 28, 0),
    gsSP2Triangles(12, 13, 28, 0, 28, 13, 29, 0),
    gsSP2Triangles(13, 14, 29, 0, 29, 14, 30, 0),
    gsSP2Triangles(14, 15, 30, 0, 30, 15, 31, 0),
    gsSP2Triangles(15, 16, 31, 0, 31, 16, 32, 0),
    gsSP2Triangles(16, 1, 32, 0, 32, 1, 17, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_peach_vibe_gloom_tear_small_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPLight(&omm_peach_vibe_gloom_tear_small_light_2.l, 1),
    gsSPLight(&omm_peach_vibe_gloom_tear_small_light_2.a, 2),
    gsSPVertex(omm_peach_vibe_gloom_tear_small_vertices, 33, 0),
    gsSPLight(&omm_peach_vibe_gloom_tear_small_light_1.l, 1),
    gsSPLight(&omm_peach_vibe_gloom_tear_small_light_1.a, 2),
    gsSPVertex(omm_peach_vibe_gloom_tear_small_vertices, 17, 0),
    gsSPDisplayList(omm_peach_vibe_gloom_tear_small_triangles),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_peach_vibe_gloom_tear_small[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_gloom_tear_small_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_peach_vibe_gloom_tear_small_update() {
    struct Object *o = gCurrentObject;
    o->oPosX += o->oVelX;
    o->oPosY += o->oVelY;
    o->oPosZ += o->oVelZ;
    o->oVelY -= 2.f;
    if (o->oTimer > 10) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript omm_bhv_peach_vibe_gloom_tear_small[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_peach_vibe_gloom_tear_small_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_peach_vibe_gloom_tear_small(struct Object *o, f32 x, f32 y, f32 z) {
    struct Object *small = obj_spawn_from_geo(o, omm_geo_peach_vibe_gloom_tear_small, omm_bhv_peach_vibe_gloom_tear_small);
    obj_scale(small, 0.1f + random_float() * 0.1f);
    obj_set_pos(small, x, y, z);
    obj_set_vel(small, random_f32_around_zero(16.f), 10.f + random_float() * 10.f, random_f32_around_zero(16.f));
    obj_set_home(small, x, y, z);
    obj_set_angle(small, 0, 0, 0);
    return small;
}

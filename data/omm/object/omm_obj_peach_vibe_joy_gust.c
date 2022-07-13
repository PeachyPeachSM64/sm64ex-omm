#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Lights1 omm_peach_vibe_joy_gust_light = gdSPDefLights1(
    0xFF, 0xFF, 0x60,
    0xFF, 0xFF, 0x60,
    0x28, 0x28, 0x28
);

static const Vtx omm_peach_vibe_joy_gust_vertices[] = {
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

static const Gfx omm_peach_vibe_joy_gust_triangles[] = {
    gsSPVertex(omm_peach_vibe_joy_gust_vertices + 0, 14, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 3, 0, 4, 0x0),
    gsSP2Triangles(0, 5, 6, 0x0, 0, 6, 1, 0x0),
    gsSP2Triangles(3, 5, 0, 0x0, 3, 7, 5, 0x0),
    gsSP2Triangles(1, 6, 8, 0x0, 6, 9, 8, 0x0),
    gsSP2Triangles(1, 8, 10, 0x0, 11, 3, 12, 0x0),
    gsSP2Triangles(11, 13, 3, 0x0, 13, 7, 3, 0x0),
    gsSPVertex(omm_peach_vibe_joy_gust_vertices + 14, 16, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 3, 4, 5, 0x0),
    gsSP2Triangles(3, 6, 4, 0x0, 3, 5, 7, 0x0),
    gsSP2Triangles(8, 3, 9, 0x0, 8, 6, 3, 0x0),
    gsSP2Triangles(8, 10, 6, 0x0, 11, 8, 12, 0x0),
    gsSP2Triangles(2, 10, 8, 0x0, 11, 2, 8, 0x0),
    gsSP2Triangles(0, 2, 11, 0x0, 13, 14, 15, 0x0),
    gsSPVertex(omm_peach_vibe_joy_gust_vertices + 30, 14, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 3, 0, 4, 0x0),
    gsSP2Triangles(3, 5, 0, 0x0, 3, 6, 5, 0x0),
    gsSP2Triangles(7, 3, 8, 0x0, 9, 6, 3, 0x0),
    gsSP2Triangles(7, 9, 3, 0x0, 10, 11, 12, 0x0),
    gsSP1Triangle(10, 13, 11, 0x0),
    gsSPVertex(omm_peach_vibe_joy_gust_vertices + 44, 5, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 2, 3, 0x0),
    gsSP1Triangle(3, 2, 4, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_peach_vibe_joy_gust_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_VIBE_JOY_TORNADO, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 64, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_peach_vibe_joy_gust_light.l, 1),
    gsSPLight(&omm_peach_vibe_joy_gust_light.a, 2),
    gsSPDisplayList(omm_peach_vibe_joy_gust_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_peach_vibe_joy_gust[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(10, geo_update_layer_transparency),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_joy_gust_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_peach_vibe_joy_gust_update() {
    struct Object *o = gCurrentObject;
    if (o->oTimer >= 60) {
        obj_mark_for_deletion(o);
    } else if (o->oAction == 1) {
        perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STICKY_FEET);
    } else {
        obj_set_pos(o, o->parentObj->oPosX, o->parentObj->oPosY, o->parentObj->oPosZ);
        obj_set_home(o, o->parentObj->oPosX, o->parentObj->oPosY, o->parentObj->oPosZ);
        o->oAction = (o->oTimer >= 8);
    }

    // Gfx
    f32 t = clamp_0_1_f(o->oAction + invlerp_0_1_s(o->oTimer, 0, 8)) * invlerp_0_1_s(o->oTimer, 60, 50);
    obj_set_scale(o, 0.3f * t, 0.25f * t, 0.3f * t);
    obj_set_angle(o, 0, o->oFaceAngleYaw + 0x2000, 0);
    obj_update_gfx(o);
    o->oOpacity = 0x80;

    // Attack weak enemies, attract and collect coins
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 300, 800, 0, 0, 200, 0);
    omm_obj_process_interactions(o, OBJ_INT_PRESET_PEACH_VIBE_JOY_GUST);
}

const BehaviorScript omm_bhv_peach_vibe_joy_gust[] = {
    OBJ_TYPE_SPECIAL,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_peach_vibe_joy_gust_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_peach_vibe_joy_gust(struct Object *o, f32 fvel, s16 angle) {
    struct Object *gust = obj_spawn_from_geo(o, omm_geo_peach_vibe_joy_gust, omm_bhv_peach_vibe_joy_gust);
    obj_set_vel(gust, fvel * sins(angle), 0.f, fvel * coss(angle));
    obj_set_angle(gust, 0, 0, 0);
    return gust;
}

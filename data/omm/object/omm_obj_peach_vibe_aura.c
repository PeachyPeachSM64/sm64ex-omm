#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Lights1 omm_peach_vibe_aura_light = gdSPDefLights1(
    0xFF, 0x80, 0xFF,
    0xFF, 0x80, 0xFF,
    0x28, 0x28, 0x28
);

static const Lights1 omm_peach_vibe_aura_full_light = gdSPDefLights1(
    0xFF, 0xFF, 0x40,
    0xFF, 0xFF, 0x40,
    0x28, 0x28, 0x28
);

static const Vtx omm_peach_vibe_aura_vertices[] = {
    { { { 0, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0, 8, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 3, 7, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 6, 6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 7, 3, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 8, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 7, -3, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 6, -6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 3, -7, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0, -8, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -3, -7, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -6, -6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -7, -3, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -8, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -7, 3, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -6, 6, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { -3, 7, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x00 } } },
    { { { 0, 32, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 12, 30, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 23, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 30, 12, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 32, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 30, -12, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -23, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 12, -30, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, -32, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -12, -30, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -23, -23, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -30, -12, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -32, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -30, 12, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -23, 23, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -12, 30, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

static const Gfx omm_peach_vibe_aura_triangles[] = {
    gsSPVertex(omm_peach_vibe_aura_vertices, 33, 0),
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

static const Gfx omm_peach_vibe_aura_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPLight(&omm_peach_vibe_aura_light.l, 1),
    gsSPLight(&omm_peach_vibe_aura_light.a, 2),
    gsSPDisplayList(omm_peach_vibe_aura_triangles),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_peach_vibe_aura_full_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsDPSetCombineLERP(SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0),
    gsSPLight(&omm_peach_vibe_aura_full_light.l, 1),
    gsSPLight(&omm_peach_vibe_aura_full_light.a, 2),
    gsSPDisplayList(omm_peach_vibe_aura_triangles),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_peach_vibe_aura[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_ASM(-80, geo_move_from_camera_mario_scale),
            GEO_TRANSLATE_NODE(0, 0, 0, 0),
            GEO_OPEN_NODE(),
                GEO_ASM(10, geo_update_layer_transparency),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_aura_gfx),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_peach_vibe_aura_full[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_ASM(-80, geo_move_from_camera_mario_scale),
            GEO_TRANSLATE_NODE(0, 0, 0, 0),
            GEO_OPEN_NODE(),
                GEO_ASM(10, geo_update_layer_transparency),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_aura_full_gfx),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_peach_vibe_aura_update() {
    struct Object *o = gCurrentObject;
    Vec3f marioRootPos; geo_get_marios_anim_part_pos(NULL, marioRootPos, MARIO_ANIM_PART_ROOT);
    f32 t = invlerp_0_1_s(o->oTimer, 0, 30);
    obj_set_xyz(o, marioRootPos[0], marioRootPos[1], marioRootPos[2]);
    obj_scale(o, gMarioObject->oScaleY * sqrtf(t) * 5.f);
    o->oOpacity = 0xC0 * sqr(1.f - t);
    if (t == 1.f) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmPeachVibeAura[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_peach_vibe_aura_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_peach_vibe_aura(struct Object *o, bool full) {
    struct Object *aura = obj_spawn_from_geo(o, full ? omm_geo_peach_vibe_aura_full : omm_geo_peach_vibe_aura, bhvOmmPeachVibeAura);
    obj_set_always_rendered(aura, true);
    obj_set_angle(aura, 0, 0, 0);
    return aura;
}

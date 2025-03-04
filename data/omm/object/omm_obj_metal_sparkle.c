#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Vtx omm_metal_sparkle_vertices[] = {
    OMM_VTX_RECT(32, 0, 0, 4, 4),
    OMM_VTX_RECT(32, 0, 1, 4, 4),
    OMM_VTX_RECT(32, 0, 2, 4, 4),
    OMM_VTX_RECT(32, 0, 3, 4, 4),
    OMM_VTX_RECT(32, 1, 0, 4, 4),
    OMM_VTX_RECT(32, 1, 1, 4, 4),
    OMM_VTX_RECT(32, 1, 2, 4, 4),
    OMM_VTX_RECT(32, 1, 3, 4, 4),
    OMM_VTX_RECT(32, 2, 0, 4, 4),
    OMM_VTX_RECT(32, 2, 1, 4, 4),
    OMM_VTX_RECT(32, 2, 2, 4, 4),
    OMM_VTX_RECT(32, 2, 3, 4, 4),
    OMM_VTX_RECT(32, 3, 0, 4, 4),
    OMM_VTX_RECT(32, 3, 1, 4, 4),
    OMM_VTX_RECT(32, 3, 2, 4, 4),
    OMM_VTX_RECT(32, 3, 3, 4, 4),
};

static const Gfx omm_metal_sparkle_triangles[] = {
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 0),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 1),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 2),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 3),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 4),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 5),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 6),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 7),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 8),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 9),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 10),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 11),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 12),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 13),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 14),
    OMM_GFX_QUAD(omm_metal_sparkle_vertices, 15),
};

OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 0);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 1);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 2);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 3);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 4);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 5);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 6);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 7);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 8);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 9);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 10);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 11);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 12);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 13);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 14);
OMM_GFX_DL(omm_metal_sparkle, OMM_TEXTURE_EFFECT_METAL_SPARKLE, omm_metal_sparkle_triangles, 15);

//
// Geo layout
//

const GeoLayout omm_geo_metal_sparkle[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(16, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_0_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_2_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_3_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_4_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_5_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_6_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_7_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_8_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_9_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_10_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_11_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_12_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_13_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_14_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_metal_sparkle_15_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_metal_sparkle_init() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    if (omm_mario_is_invisible(m) && (!gOmmGlobals->yoshiMode || !obj_get_first_with_behavior(bhvOmmYoshiModeYoshi))) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmMetalSparkle[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_SET_INT(oAnimState, -1),
    BHV_CALL_NATIVE(bhv_omm_metal_sparkle_init),
    BHV_BEGIN_REPEAT(16),
        BHV_ADD_INT(oAnimState, 1),
    BHV_END_REPEAT(),
    BHV_DEACTIVATE(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_metal_sparkle(struct Object *o) {
    struct Object *sparkle = obj_spawn_from_geo(o, omm_geo_metal_sparkle, bhvOmmMetalSparkle);
    sparkle->activeFlags  |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    sparkle->oPosX         = o->oPosX;
    sparkle->oPosY         = o->oPosY + 80;
    sparkle->oPosZ         = o->oPosZ;
    sparkle->oOpacity      = 255;
    obj_translate_xyz_random(sparkle, 150.f);
    obj_scale_random(sparkle, 1.5f, 0.5f);
    return sparkle;
}
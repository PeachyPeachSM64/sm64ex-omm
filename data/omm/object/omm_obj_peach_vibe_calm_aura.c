#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Vtx omm_peach_vibe_calm_aura_vertices[] = {
    OMM_VTX_RECT(32, 0, 0, 5, 5),
    OMM_VTX_RECT(32, 0, 1, 5, 5),
    OMM_VTX_RECT(32, 0, 2, 5, 5),
    OMM_VTX_RECT(32, 0, 3, 5, 5),
    OMM_VTX_RECT(32, 0, 4, 5, 5),
    OMM_VTX_RECT(32, 1, 0, 5, 5),
    OMM_VTX_RECT(32, 1, 1, 5, 5),
    OMM_VTX_RECT(32, 1, 2, 5, 5),
    OMM_VTX_RECT(32, 1, 3, 5, 5),
    OMM_VTX_RECT(32, 1, 4, 5, 5),
    OMM_VTX_RECT(32, 2, 0, 5, 5),
    OMM_VTX_RECT(32, 2, 1, 5, 5),
    OMM_VTX_RECT(32, 2, 2, 5, 5),
    OMM_VTX_RECT(32, 2, 3, 5, 5),
    OMM_VTX_RECT(32, 2, 4, 5, 5),
    OMM_VTX_RECT(32, 3, 0, 5, 5),
    OMM_VTX_RECT(32, 3, 1, 5, 5),
    OMM_VTX_RECT(32, 3, 2, 5, 5),
};

static const Gfx omm_peach_vibe_calm_aura_triangles[] = {
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 0),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 1),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 2),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 3),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 4),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 5),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 6),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 7),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 8),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 9),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 10),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 11),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 12),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 13),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 14),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 15),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 16),
    OMM_GFX_QUAD(omm_peach_vibe_calm_aura_vertices, 17),
};

OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 0);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 1);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 2);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 3);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 4);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 5);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 6);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 7);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 8);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 9);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 10);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 11);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 12);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 13);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 14);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 15);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 16);
OMM_GFX_DL(omm_peach_vibe_calm_aura, OMM_TEXTURE_EFFECT_VIBE_CALM_AURA, omm_peach_vibe_calm_aura_triangles, 17);

//
// Geo layout
//

const GeoLayout omm_geo_peach_vibe_calm_aura[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_ASM(-80, geo_move_from_camera_mario_scale),
            GEO_TRANSLATE_NODE(0, 0, 0, 0),
            GEO_OPEN_NODE(),
                GEO_SWITCH_CASE(18, geo_switch_anim_state),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_0_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_1_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_2_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_3_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_4_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_5_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_6_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_7_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_8_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_9_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_10_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_11_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_12_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_13_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_14_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_15_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_16_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_aura_17_gfx),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_peach_vibe_calm_aura_update() {
    struct Object *o = gCurrentObject;
    if (omm_peach_vibe_is_calm()) {
        Vec3f marioRootPos; geo_get_marios_anim_part_pos(NULL, marioRootPos, MARIO_ANIM_PART_ROOT);
        o->oPosX = marioRootPos[0];
        o->oPosY = marioRootPos[1];
        o->oPosZ = marioRootPos[2];
        if (gGlobalTimer % 2 == 0) {
            o->oAnimState++;
        }
        if (gGlobalTimer % 3 == 0) {
            omm_obj_spawn_peach_vibe_calm_sparkle(o);
        }
    } else {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmPeachVibeCalmAura[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_peach_vibe_calm_aura_update),
    BHV_END_LOOP()
};

//
// Spawner
//

struct Object *omm_obj_spawn_peach_vibe_calm_aura(struct Object *o) {
    struct Object *aura = obj_get_first_with_behavior(bhvOmmPeachVibeCalmAura);
    if (!aura) {
        aura = obj_spawn_from_geo(o, omm_geo_peach_vibe_calm_aura, bhvOmmPeachVibeCalmAura);
        obj_set_always_rendered(aura, true);
        obj_set_angle(aura, 0, 0, 0);
        obj_scale(aura, 5.f * gMarioObject->oScaleY);
    }
    return aura;
}
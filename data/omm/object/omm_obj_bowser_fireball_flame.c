#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Vtx omm_bowser_fireball_flame_vertices[] = {
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 0, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 1, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 2, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 3, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 4, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 5, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 6, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 7, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 8, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 0, 9, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 0, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 1, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 2, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 3, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 4, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 5, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 6, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 7, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 8, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 1, 9, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 0, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 1, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 2, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 3, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 4, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 5, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 6, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 7, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 8, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 2, 9, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 0, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 1, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 2, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 3, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 4, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 5, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 6, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 7, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 8, 4, 10.24f),
    OMM_VTX_RECT_XY(-25, 25, 0, 128, 3, 9, 4, 10.24f),
};

static const Gfx omm_bowser_fireball_flame_triangles[] = {
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 0),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 1),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 2),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 3),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 4),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 5),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 6),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 7),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 8),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 9),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 10),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 11),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 12),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 13),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 14),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 15),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 16),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 17),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 18),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 19),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 20),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 21),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 22),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 23),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 24),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 25),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 26),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 27),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 28),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 29),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 30),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 31),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 32),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 33),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 34),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 35),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 36),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 37),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 38),
    OMM_GFX_QUAD(omm_bowser_fireball_flame_vertices, 39),
};

OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 0);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 1);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 2);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 3);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 4);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 5);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 6);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 7);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 8);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 9);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 10);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 11);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 12);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 13);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 14);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 15);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 16);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 17);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 18);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 19);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 20);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 21);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 22);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 23);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 24);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 25);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 26);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 27);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 28);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 29);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 30);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 31);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 32);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 33);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 34);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 35);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 36);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 37);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 38);
OMM_GFX_DL(omm_bowser_fireball_flame, OMM_TEXTURE_BOWSER_FIRE_MAGIC_2, omm_bowser_fireball_flame_triangles, 39);

//
// Geo layout
//

const GeoLayout omm_geo_bowser_fireball_flame[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(40, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_0_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_2_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_3_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_4_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_5_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_6_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_7_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_8_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_9_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_10_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_11_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_12_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_13_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_14_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_15_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_16_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_17_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_18_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_19_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_20_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_21_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_22_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_23_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_24_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_25_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_26_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_27_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_28_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_29_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_30_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_31_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_32_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_33_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_34_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_35_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_36_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_37_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_38_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_flame_39_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_bowser_fireball_flame_update() {
    struct Object *o = gCurrentObject;
    switch (o->oAction) {

        // Gets bigger
        case 0: {
            f32 t = clamp_0_1_f(o->oTimer / 10.f);
            o->oScaleX = 6.f * t;
            o->oScaleY = 4.f * t;
            o->oScaleZ = 6.f * t;
            if (o->oTimer >= 10) {
                o->oAction = 1;
                o->oTimer = 0;
            }
        } break;

        // Stays big
        case 1: {
            o->oScaleX = 6.f;
            o->oScaleY = 4.f;
            o->oScaleZ = 6.f;
            if (o->oTimer > o->oBowserFireDuration) {
                o->oAction = 2;
                o->oTimer = 0;
            }
        } break;

        // Gets smaller and vanishes
        case 2: {
            f32 t = 1.f - clamp_0_1_f(o->oTimer / 15.f);
            o->oScaleX = 6.f * t;
            o->oScaleY = 4.f * t;
            o->oScaleZ = 6.f * t;
            if (o->oTimer >= 15) {
                obj_mark_for_deletion(o);
                return;
            }
        } break;
    }
    o->oGraphYOffset = -8 * o->oScaleY;

    // Update Y position and delete if there is no valid floor
    struct Surface *floor = NULL;
    o->oPosY = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
    if (!floor || SURFACE_IS_LETHAL(floor->type)) {
        obj_mark_for_deletion(o);
        return;
    }

    // Small flames don't damage Mario
    if (o->oScaleY >= 1.f) {
        obj_set_params(o, INTERACT_FLAME, 1, 99, 0, true);
        obj_reset_hitbox(o, 10, 40, 0, 0, 0, 0);
    } else {
        obj_set_params(o, INTERACT_FLAME, 1, 99, 0, false);
        obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    }
}

const BehaviorScript bhvOmmBowserFireballFlame[] = {
    OBJ_TYPE_GENACTOR,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_ADD_INT(oAnimState, 1),
        BHV_CALL_NATIVE(bhv_omm_bowser_fireball_flame_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_bowser_fireball_flame(struct Object *o, s32 duration) {
    struct Object *flame = obj_spawn_from_geo(o, omm_geo_bowser_fireball_flame, bhvOmmBowserFireballFlame);
    flame->oBowserFireDuration = duration;
    flame->oAnimState = random_u16() % 40;
    flame->oOpacity = 255;
    flame->oAction = 0;
    obj_scale(flame, 0);
    return flame;
}

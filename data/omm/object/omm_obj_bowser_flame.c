#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Vtx omm_bowser_flame_vertices[] = {
    OMM_VTX_RECT_XY(-75, 75, 0, 150, 0, 0, (256.f / 96.f), (256.f / 96.f)),
    OMM_VTX_RECT_XY(-75, 75, 0, 150, 0, 1, (256.f / 96.f), (256.f / 96.f)),
    OMM_VTX_RECT_XY(-75, 75, 0, 150, 1, 0, (256.f / 96.f), (256.f / 96.f)),
    OMM_VTX_RECT_XY(-75, 75, 0, 150, 1, 1, (256.f / 96.f), (256.f / 96.f)),
};

static const Gfx omm_bowser_flame_triangles[] = {
    OMM_GFX_QUAD(omm_bowser_flame_vertices, 0),
    OMM_GFX_QUAD(omm_bowser_flame_vertices, 1),
    OMM_GFX_QUAD(omm_bowser_flame_vertices, 2),
    OMM_GFX_QUAD(omm_bowser_flame_vertices, 3),
};

OMM_GFX_DL(omm_bowser_flame, OMM_TEXTURE_BOWSER_FIRE_BREATH, omm_bowser_flame_triangles, 0);
OMM_GFX_DL(omm_bowser_flame, OMM_TEXTURE_BOWSER_FIRE_BREATH, omm_bowser_flame_triangles, 1);
OMM_GFX_DL(omm_bowser_flame, OMM_TEXTURE_BOWSER_FIRE_BREATH, omm_bowser_flame_triangles, 2);
OMM_GFX_DL(omm_bowser_flame, OMM_TEXTURE_BOWSER_FIRE_BREATH, omm_bowser_flame_triangles, 3);

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

const BehaviorScript bhvOmmBowserFlame[] = {
    OBJ_TYPE_GENACTOR,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_bowser_flame_loop),
        BHV_ADD_INT(oAnimState, 1),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_bowser_flame(struct Object *o, f32 x, f32 y, f32 z, s32 duration) {
    struct Object *flame = obj_spawn_from_geo(o, omm_geo_bowser_flame, bhvOmmBowserFlame);
    obj_set_xyz(flame, x, y, z);
    flame->oAction = 0;
    flame->oAnimState = random_u16() % 4;
    flame->oBowserFireDuration = duration;
    obj_play_sound(o, SOUND_AIR_BOWSER_SPIT_FIRE);
    return flame;
}

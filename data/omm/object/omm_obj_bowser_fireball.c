#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Vtx omm_bowser_fireball_vertices[] = {
    OMM_VTX_RECT(128, 0, 0, 4, 4),
    OMM_VTX_RECT(128, 0, 1, 4, 4),
    OMM_VTX_RECT(128, 0, 2, 4, 4),
    OMM_VTX_RECT(128, 0, 3, 4, 4),
    OMM_VTX_RECT(128, 1, 0, 4, 4),
    OMM_VTX_RECT(128, 1, 1, 4, 4),
    OMM_VTX_RECT(128, 1, 2, 4, 4),
    OMM_VTX_RECT(128, 1, 3, 4, 4),
    OMM_VTX_RECT(128, 2, 0, 4, 4),
    OMM_VTX_RECT(128, 2, 1, 4, 4),
    OMM_VTX_RECT(128, 2, 2, 4, 4),
    OMM_VTX_RECT(128, 2, 3, 4, 4),
    OMM_VTX_RECT(128, 3, 0, 4, 4),
    OMM_VTX_RECT(128, 3, 1, 4, 4),
    OMM_VTX_RECT(128, 3, 2, 4, 4),
    OMM_VTX_RECT(128, 3, 3, 4, 4),
};

static const Gfx omm_bowser_fireball_triangles[] = {
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 0),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 1),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 2),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 3),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 4),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 5),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 6),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 7),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 8),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 9),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 10),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 11),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 12),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 13),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 14),
    OMM_GFX_QUAD(omm_bowser_fireball_vertices, 15),
};

OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 0);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 1);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 2);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 3);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 4);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 5);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 6);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 7);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 8);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 9);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 10);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 11);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 12);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 13);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 14);
OMM_GFX_DL(omm_bowser_fireball, OMM_TEXTURE_BOWSER_FIRE_MAGIC_1, omm_bowser_fireball_triangles, 15);

//
// Geo layout
//

const GeoLayout omm_geo_bowser_fireball[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(16, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_0_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_2_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_3_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_4_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_5_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_6_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_7_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_8_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_9_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_10_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_11_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_12_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_13_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_14_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_fireball_15_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_bowser_fireball_loop(void) {
    struct Object *o = gCurrentObject;
    if (o->oTimer < o->oBowserFireDuration) {
        o->oPosX += o->oVelX;
        o->oPosZ += o->oVelZ;
        f32 t = clamp_0_1_f(o->oTimer / 10.f);
        obj_scale(o, 2.f * t);
        obj_set_params(o, INTERACT_FLAME, 1, 99, 0, true);
        obj_reset_hitbox(o, 50, 100, 0, 0, 0, 50);
        if (o->oTimer & 1) {
            omm_obj_spawn_bowser_fireball_flame(o, 30);
        }
    } else {
        obj_spawn_white_puff(o, NO_SOUND);
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmBowserFireball[] = {
    OBJ_TYPE_GENACTOR,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_ADD_INT(oAnimState, 1),
        BHV_CALL_NATIVE(bhv_omm_bowser_fireball_loop),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_bowser_fireball(struct Object *o, f32 x, f32 y, f32 z, f32 forwardVel, f32 maxDistance, s16 angle) {
    struct Object *fireball = obj_spawn_from_geo(o, omm_geo_bowser_fireball, bhvOmmBowserFireball);
    obj_set_xyz(fireball, x, y, z);
    obj_set_angle(fireball, 0, angle, 0);
    obj_set_forward_vel(fireball, angle, 1.f, forwardVel);
    obj_scale(fireball, 0);
    fireball->oOpacity = 255;
    fireball->oBowserFireDuration = (s32) (maxDistance / forwardVel);
    obj_play_sound(fireball, SOUND_AIR_BOWSER_SPIT_FIRE);
    return fireball;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#define OMM_AMP_DISCHARGE_RADIUS (100)

//
// Gfx data
//

static const Vtx omm_amp_discharge_vertices[] = {
    OMM_VTX_RECT(OMM_AMP_DISCHARGE_RADIUS, 0, 0, 2, 2),
    OMM_VTX_RECT(OMM_AMP_DISCHARGE_RADIUS, 0, 1, 2, 2),
    OMM_VTX_RECT(OMM_AMP_DISCHARGE_RADIUS, 1, 0, 2, 2),
    OMM_VTX_RECT(OMM_AMP_DISCHARGE_RADIUS, 1, 1, 2, 2),
};

static const Gfx omm_amp_discharge_triangles[] = {
    OMM_GFX_QUAD(omm_amp_discharge_vertices, 0),
    OMM_GFX_QUAD(omm_amp_discharge_vertices, 1),
    OMM_GFX_QUAD(omm_amp_discharge_vertices, 2),
    OMM_GFX_QUAD(omm_amp_discharge_vertices, 3),
};

OMM_GFX_DL(omm_amp_discharge_1, OMM_TEXTURE_EFFECT_DISCHARGE_1, omm_amp_discharge_triangles, 0);
OMM_GFX_DL(omm_amp_discharge_1, OMM_TEXTURE_EFFECT_DISCHARGE_1, omm_amp_discharge_triangles, 1);
OMM_GFX_DL(omm_amp_discharge_1, OMM_TEXTURE_EFFECT_DISCHARGE_1, omm_amp_discharge_triangles, 2);

OMM_GFX_DL(omm_amp_discharge_2, OMM_TEXTURE_EFFECT_DISCHARGE_2, omm_amp_discharge_triangles, 0);
OMM_GFX_DL(omm_amp_discharge_2, OMM_TEXTURE_EFFECT_DISCHARGE_2, omm_amp_discharge_triangles, 1);
OMM_GFX_DL(omm_amp_discharge_2, OMM_TEXTURE_EFFECT_DISCHARGE_2, omm_amp_discharge_triangles, 2);
OMM_GFX_DL(omm_amp_discharge_2, OMM_TEXTURE_EFFECT_DISCHARGE_2, omm_amp_discharge_triangles, 3);

//
// Geo layout
//

const GeoLayout omm_geo_amp_discharge[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_ASM(10, geo_move_from_camera),
            GEO_TRANSLATE_NODE(0, 0, 0, 0),
            GEO_OPEN_NODE(),
                GEO_SWITCH_CASE(12, geo_switch_anim_state),
                GEO_OPEN_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_0_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_0_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_0_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_1_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_0_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_2_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_0_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_3_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_1_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_0_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_1_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_1_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_1_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_2_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_1_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_3_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_2_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_0_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_2_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_1_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_2_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_2_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(0, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_1_2_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_amp_discharge_2_3_gfx),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_amp_discharge_update() {
    struct Object *o = gCurrentObject;
    if (o->parentObj && o->parentObj->activeFlags != ACTIVE_FLAG_DEACTIVATED && o->oTimer < 20) {
        o->oAnimState = random_u16() % 12;
        f32 scaleFactor = lerp_f(o->oObjectPower, 1.f, 2.5f);
        obj_scale(o, clamp_0_1_f((f32) o->oTimer / 5.f) * lerp_f(random_float(), 0.9f, 1.1f) * o->parentObj->oScaleY * scaleFactor);
        obj_set_xyz(o, o->parentObj->oPosX, o->parentObj->oPosY + 60 * o->parentObj->oScaleY, o->parentObj->oPosZ);
        obj_play_sound(o, SOUND_AIR_AMP_BUZZ | 0xFF00);
        obj_set_params(o, 0, 0, 0, 0, true);
        if (o->parentObj == gOmmCapture) {
            obj_reset_hitbox(o, OMM_AMP_DISCHARGE_RADIUS, OMM_AMP_DISCHARGE_RADIUS * 2, OMM_AMP_DISCHARGE_RADIUS, OMM_AMP_DISCHARGE_RADIUS * 2, 0, OMM_AMP_DISCHARGE_RADIUS);
            omm_obj_process_interactions(o, obj_get_interaction_flags_from_power(o->oObjectPower, OBJ_INT_PRESET_AMP_DISCHARGE) | OBJ_INT_ATTRACT_COINS_RANGE(lerp_s(o->oObjectPower, 600, 1200)));
        }
    } else {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmAmpDischarge[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_amp_discharge_update),
    BHV_END_LOOP()
};

//
// Spawner
//

struct Object *omm_obj_spawn_amp_discharge(struct Object *o, f32 power) {
    struct Object *discharge = obj_spawn_from_geo(o, omm_geo_amp_discharge, bhvOmmAmpDischarge);
    obj_set_always_rendered(discharge, true);
    obj_set_angle(discharge, 0, 0, 0);
    discharge->oObjectPower = power;
    obj_set_interact_id(o, discharge);
    return discharge;
}
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_explosion[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(explosion_geo),
};

const GeoLayout omm_geo_explosion_2[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SCALE(0, 0x1E0000),
            GEO_OPEN_NODE(),
                GEO_ROTATION_NODE(0, 90, 0, 0),
                GEO_OPEN_NODE(),
                    GEO_BRANCH(1, invisible_bowser_accessory_geo),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_explosion_update() {
    struct Object *o = gCurrentObject;
    if (o->oTimer >= o->oShockwaveDuration) {
        spawn_object(o, MODEL_SMOKE, bhvBobombBullyDeathSmoke);
        obj_mark_for_deletion(o);
        return;
    }

    // Hitbox
    obj_scale(o, 1.f);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 90 * o->oShockwaveScaleMax, 120 * o->oShockwaveScaleMax, 0, 0, 0, 40 * o->oShockwaveScaleMax);

    // Gfx
    f32 t = (f32) o->oTimer / (f32) o->oShockwaveDuration;
    obj_scale(o, lerp_f(t, o->oShockwaveScaleMin, o->oShockwaveScaleMax));
    o->oOpacity = lerp_s(t, 0xFF, 0x00);
    o->oAnimState = (s32) (t * 10);

    // Damage King Bob-omb
    for_each_object_with_behavior(kbo, bhvKingBobomb) { if (kbo->oAction == 6) { kbo->oIntangibleTimer = 0; } }
    struct Object *interacted = omm_obj_process_interactions(o, OBJ_INT_PRESET_EXPLOSION);
    if (interacted &&
        interacted->behavior == bhvKingBobomb &&
        interacted->oHeldState == HELD_FREE && (
        interacted->oAction == 1 ||
        interacted->oAction == 2 ||
        interacted->oAction == 6)) {
        obj_set_angle(interacted, 0, obj_get_object1_angle_yaw_to_object2(o, interacted), 0);
        obj_set_forward_and_y_vel(interacted, 20, 50);
        obj_anim_play_with_sound(interacted, 6, 1, 0, true);
        interacted->oAction = 4;
    }
}

const BehaviorScript bhvOmmExplosion[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_explosion_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_explosion(struct Object *o, f32 offsetY, f32 scaleMin, f32 scaleMax, s32 duration, const GeoLayout *geoLayout) {
    struct Object *explosion  = obj_spawn_from_geo(o, geoLayout, bhvOmmExplosion);
    explosion->oPosY += offsetY;
    explosion->oGraphYOffset = 0;
    explosion->oAnimState = 0;
    explosion->oOpacity = 0xFF;
    explosion->oShockwaveScaleMin = scaleMin;
    explosion->oShockwaveScaleMax = scaleMax;
    explosion->oShockwaveDuration = duration;
    obj_play_sound(o, SOUND_GENERAL2_BOBOMB_EXPLOSION);
    set_environmental_camera_shake(SHAKE_ENV_EXPLOSION);
    obj_set_interact_id(o, explosion);
    return explosion;
}

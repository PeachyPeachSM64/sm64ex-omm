#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_shockwave[] = {
    OMM_GEO_BRANCH(invisible_bowser_accessory_geo),
};

//
// Behavior
//

static void bhv_omm_shockwave_update() {
    struct Object *o = gCurrentObject;
    f32 t = (f32) o->oTimer / (f32) o->oShockwaveDuration;
    if (t >= 1.f) {
        obj_mark_for_deletion(o);
        return;
    }
    obj_scale(o, lerp_f(t, o->oShockwaveScaleMin, o->oShockwaveScaleMax));
    o->oOpacity = clamp_s(lerp_f(t, o->oShockwaveAlphaMax, o->oShockwaveAlphaMin), 0x00, 0xFF);
}

const BehaviorScript bhvOmmShockwave[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_shockwave_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_shockwave(struct Object *o, f32 scaleMin, f32 scaleMax, u8 alphaMin, u8 alphaMax, s32 duration) {
    struct Object *shockwave = obj_spawn_from_geo(o, omm_geo_shockwave, bhvOmmShockwave);
    obj_scale(shockwave, scaleMin);
    shockwave->oOpacity = alphaMax;
    shockwave->oShockwaveScaleMin = scaleMin;
    shockwave->oShockwaveScaleMax = scaleMax;
    shockwave->oShockwaveAlphaMin = alphaMin;
    shockwave->oShockwaveAlphaMax = alphaMax;
    shockwave->oShockwaveDuration = duration;
    return shockwave;
}

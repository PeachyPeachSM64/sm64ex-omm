#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_spindrift_shockwave[] = {
    OMM_GEO_BRANCH(invisible_bowser_accessory_geo),
};

//
// Behavior
//

static void bhv_omm_spindrift_shockwave_update() {
    struct Object *o = gCurrentObject;
    f32 t = (f32) o->oTimer / 12.f;
    if (t >= 1.f) {
        obj_mark_for_deletion(o);
        return;
    }

    o->oPosX = o->parentObj->oPosX;
    o->oPosY = o->parentObj->oPosY + o->oParentRelativePosY;
    o->oPosZ = o->parentObj->oPosZ;
    f32 scale = 30.f + 15.f * t;
    obj_scale(o, 1.f);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 5 * scale, 160, 0, 0, 0, 80);
    obj_scale(o, scale);
    o->oOpacity = min_s(255, 510 * (1.f - t));
    omm_obj_process_interactions(o, OBJ_INT_PRESET_SPINDRIFT_SHOCKWAVE);
}

const BehaviorScript bhvOmmSpindriftShockwave[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_spindrift_shockwave_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_spindrift_shockwave(struct Object *o) {
    struct Object *shockwave = obj_spawn_from_geo(o, omm_geo_spindrift_shockwave, bhvOmmSpindriftShockwave);
    shockwave->oParentRelativePosY = o->hitboxHeight / 2.f;
    obj_set_interact_id(o, shockwave);
    return shockwave;
}

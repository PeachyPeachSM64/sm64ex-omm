#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_shockwave_spindrift[] = {
    OMM_GEO_BRANCH(invisible_bowser_accessory_geo),
};

//
// Behavior
//

static void omm_bhv_shockwave_spindrift_update() {
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
    omm_obj_process_interactions(o, OBJ_INT_PRESET_ATTACK_AND_COLLECT);
}

const BehaviorScript omm_bhv_shockwave_spindrift[] = {
    OBJ_TYPE_SPECIAL,
    0x110100C1,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_shockwave_spindrift_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_shockwave_spindrift(struct Object *o) {
    struct Object *shockwave = obj_spawn_from_geo(o, omm_geo_shockwave_spindrift, omm_bhv_shockwave_spindrift);
    shockwave->oParentRelativePosY = o->hitboxHeight / 2.f;
    return shockwave;
}

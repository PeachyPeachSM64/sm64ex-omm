#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_shockwave_whomp[] = {
    OMM_GEO_BRANCH(invisible_bowser_accessory_geo),
};

//
// Behavior
//

static void omm_bhv_shockwave_whomp_update() {
    struct Object *o = gCurrentObject;
    f32 t = (f32) o->oTimer / 30.f;
    if (t >= 1.f) {
        obj_mark_for_deletion(o);
        return;
    }

    f32 scale = 30.f + 120.f * t;
    obj_scale(o, 1.f);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 5 * scale, 120, 0, 0, 0, 60);
    obj_scale(o, scale);
    o->oOpacity = min_s(255, 510 * (1.f - t));
    omm_obj_process_interactions(o, OBJ_INT_PRESET_SHOCKWAVE);
}

const BehaviorScript omm_bhv_shockwave_whomp[] = {
    OBJ_TYPE_SPECIAL,
    0x110100C1,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_shockwave_whomp_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_shockwave_whomp(struct Object *o, f32 x, f32 y, f32 z) {
    struct Object *shockwave   = obj_spawn_from_geo(o, omm_geo_shockwave_whomp, omm_bhv_shockwave_whomp);
    shockwave->oPosX           = x;
    shockwave->oPosY           = y + 20.f;
    shockwave->oPosZ           = z;
    shockwave->oFaceAnglePitch = 0;
    shockwave->oFaceAngleYaw   = 0;
    shockwave->oFaceAngleRoll  = 0;
    shockwave->oMoveAnglePitch = 0;
    shockwave->oMoveAngleYaw   = 0;
    shockwave->oMoveAngleRoll  = 0;
    return shockwave;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_whomp_shockwave[] = {
    OMM_GEO_BRANCH(invisible_bowser_accessory_geo),
};

//
// Behavior
//

static void bhv_omm_whomp_shockwave_update() {
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
    omm_obj_process_interactions(o, OBJ_INT_PRESET_WHOMP_SHOCKWAVE);
}

const BehaviorScript bhvOmmWhompShockwave[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_whomp_shockwave_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_whomp_shockwave(struct Object *o, f32 x, f32 y, f32 z) {
    struct Object *shockwave   = obj_spawn_from_geo(o, omm_geo_whomp_shockwave, bhvOmmWhompShockwave);
    shockwave->oPosX           = x;
    shockwave->oPosY           = y + 20.f;
    shockwave->oPosZ           = z;
    shockwave->oFaceAnglePitch = 0;
    shockwave->oFaceAngleYaw   = 0;
    shockwave->oFaceAngleRoll  = 0;
    shockwave->oMoveAnglePitch = 0;
    shockwave->oMoveAngleYaw   = 0;
    shockwave->oMoveAngleRoll  = 0;
    obj_set_interact_id(o, shockwave);
    return shockwave;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_vanish_mist[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(mist_geo),
};

//
// Behavior
//

static void bhv_omm_vanish_mist_init() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    if (omm_mario_is_invisible(m)) {
        obj_mark_for_deletion(o);
    }
}

static void bhv_omm_vanish_mist_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    if (!p || !p->activeFlags) {
        obj_mark_for_deletion(o);
        return;
    }

    f32 dt = 1.f - clamp_0_1_f(o->oTimer / 30.f);
    f32 dx = o->oPosX - p->oPosX;
    f32 dy = o->oPosY - p->oPosY - p->hitboxHeight / 2;
    f32 dz = o->oPosZ - p->oPosZ;
    f32 dv = sqrtf(sqr_f(dx) + sqr_f(dy) + sqr_f(dz));
    o->oScaleX = (60.f / 25.f) * dt;
    o->oScaleY = (90.f / 25.f) * dt;
    o->oScaleZ = (60.f / 25.f) * dt;
    o->oOpacity = 0x80 * dt * (dv > 80.f);
    if (dt == 0) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmVanishMist[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_CALL_NATIVE(bhv_omm_vanish_mist_init),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_vanish_mist_update),
    BHV_END_LOOP()
};

//
// Spawner
//

struct Object *omm_obj_spawn_vanish_mist(struct Object *o) {
    struct Object *mist = obj_spawn_from_geo(o, omm_geo_vanish_mist, bhvOmmVanishMist);
    mist->activeFlags  |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    mist->oPosX         = o->oPosX;
    mist->oPosY         = o->oPosY + o->hitboxHeight / 2;
    mist->oPosZ         = o->oPosZ;
    mist->oOpacity      = 0;
    return mist;
}

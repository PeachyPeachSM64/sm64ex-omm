#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_vanish_mist[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(mist_geo),
};

//
// Behavior
//

static void omm_bhv_vanish_mist_update() {
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

const BehaviorScript omm_bhv_vanish_mist[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_vanish_mist_update,
    0x09000000
};

//
// Spawner
//

struct Object *omm_spawn_vanish_mist(struct Object *o) {
    struct Object *mist = obj_spawn_from_geo(o, omm_geo_vanish_mist, omm_bhv_vanish_mist);
    mist->activeFlags  |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    mist->oPosX         = o->oPosX;
    mist->oPosY         = o->oPosY + o->hitboxHeight / 2;
    mist->oPosZ         = o->oPosZ;
    mist->oOpacity      = 0;
    return mist;
}

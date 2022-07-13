#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layouts
//

const GeoLayout omm_geo_fire_smoke_red[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(red_flame_geo),
};

const GeoLayout omm_geo_fire_smoke_blue[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(blue_flame_geo),
};

//
// Behavior
//

static void omm_bhv_fire_smoke_init() {
    cur_obj_scale(4.f + 2.f * random_float());
}

const BehaviorScript omm_bhv_fire_smoke[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x0C000000, (uintptr_t) omm_bhv_fire_smoke_init,
    0x101AFFFF,
    0x0500000A,
    0x0F1A0001,
    0x06000000,
    0x1D000000,
};

//
// Spawner
//

struct Object *omm_spawn_fire_smoke(struct Object *o, s32 type) {
    struct Object *smoke = NULL;
    switch (type) {
        default:
        case 0: smoke = obj_spawn_from_geo(o, omm_geo_fire_smoke_red, omm_bhv_fire_smoke); break;
        case 1: smoke = obj_spawn_from_geo(o, omm_geo_fire_smoke_blue, omm_bhv_fire_smoke); break;
    }
    return smoke;
}

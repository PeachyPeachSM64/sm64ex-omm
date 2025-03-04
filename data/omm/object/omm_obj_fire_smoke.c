#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

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

static void bhv_omm_fire_smoke_init() {
    cur_obj_scale(4.f + 2.f * random_float());
}

const BehaviorScript bhvOmmFireSmoke[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_CALL_NATIVE(bhv_omm_fire_smoke_init),
    BHV_SET_INT(oAnimState, -1),
    BHV_BEGIN_REPEAT(10),
        BHV_ADD_INT(oAnimState, 1),
    BHV_END_REPEAT(),
    BHV_DEACTIVATE(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_fire_smoke(struct Object *o, s32 type) {
    struct Object *smoke = NULL;
    switch (type) {
        default:
        case 0: smoke = obj_spawn_from_geo(o, omm_geo_fire_smoke_red, bhvOmmFireSmoke); break;
        case 1: smoke = obj_spawn_from_geo(o, omm_geo_fire_smoke_blue, bhvOmmFireSmoke); break;
    }
    return smoke;
}

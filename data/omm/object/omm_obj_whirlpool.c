#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_whirlpool[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ROTATE_WITH_DL(LAYER_TRANSPARENT, 90, 0, 0, whirlpool_seg5_dl_05013CB8),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_whirlpool_update() {
    struct Object *o = gCurrentObject;
    o->oFaceAngleRoll += o->oAngleVelRoll;
}

const BehaviorScript bhvOmmWhirlpool[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_whirlpool_update),
    BHV_END_LOOP()
};

//
// Spawner
//

struct Object *omm_obj_spawn_whirlpool(struct Object *o) {
    struct Object *whirlpool = obj_spawn_from_geo(o, omm_geo_whirlpool, bhvOmmWhirlpool);
    return whirlpool;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_bowser_mine[] = {
    OMM_GEO_BRANCH(bowser_bomb_geo),
};

//
// Behavior
//

const BehaviorScript omm_bhv_bowser_mine[] = {
    OBJ_TYPE_GENACTOR,
    0x110100C1,
    0x08000000,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_bowser_mine(struct Object *o, f32 x, f32 y, f32 z, s16 yaw) {
    struct Object *mine = obj_spawn_from_geo(o, omm_geo_bowser_mine, omm_bhv_bowser_mine);
    mine->oPosX         = x;
    mine->oPosY         = y;
    mine->oPosZ         = z;
    mine->oFaceAngleYaw = yaw;
    mine->oMoveAngleYaw = yaw;
    return mine;
}

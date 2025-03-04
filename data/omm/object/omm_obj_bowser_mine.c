#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

const BehaviorScript bhvOmmBowserMine[] = {
    OBJ_TYPE_GENACTOR,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR),
    BHV_BEGIN_LOOP(),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_bowser_mine(struct Object *o, f32 x, f32 y, f32 z, s16 yaw) {
    struct Object *mine = obj_spawn_from_geo(o, bowser_bomb_geo, bhvOmmBowserMine);
    mine->oPosX         = x;
    mine->oPosY         = y;
    mine->oPosZ         = z;
    mine->oFaceAngleYaw = yaw;
    mine->oMoveAngleYaw = yaw;
    return mine;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_flaming_bobomb_aura[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(blue_flame_geo),
};

//
// Behavior
//

const BehaviorScript bhvOmmFlamingBobombAura[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_ADD_INT(oAnimState, 1),
    BHV_END_LOOP()
};

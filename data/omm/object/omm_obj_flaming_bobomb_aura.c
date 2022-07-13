#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_flaming_bobomb_aura[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(blue_flame_geo),
};

//
// Behavior
//

const BehaviorScript omm_bhv_flaming_bobomb_aura[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0F1A0001,
    0x09000000
};

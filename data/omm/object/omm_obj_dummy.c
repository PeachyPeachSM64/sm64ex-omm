#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx
//

const Gfx null[] = {
    gsSPEndDisplayList()
};

//
// Behaviors
//

const BehaviorScript omm_bhv_dummy[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x08000000,
    0x09000000
};

const BehaviorScript omm_bhv_cappy[] = {
    OBJ_TYPE_SPECIAL,
    0x08000000,
    0x09000000
};

const BehaviorScript omm_bhv_possessed_object[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x08000000,
    0x09000000
};

const BehaviorScript omm_bhv_possessed_object_cap[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x08000000,
    0x09000000
};

const BehaviorScript omm_bhv_possessed_koopa_shell[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x08000000,
    0x09000000
};

const BehaviorScript omm_bhv_act_select_star[] = {
    OBJ_TYPE_LEVEL,
    0x11010001,
    0x08000000,
    0x09000000,
};

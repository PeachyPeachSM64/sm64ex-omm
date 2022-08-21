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

static void bhv_dummy_update() {
}

const BehaviorScript bhvOmmDummy[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x08000000,
    0x09000000
};

const BehaviorScript bhvOmmCappy[] = {
    OBJ_TYPE_SPECIAL,
    0x08000000,
    0x09000000
};

const BehaviorScript bhvOmmPossessedObject[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x08000000,
    0x0C000000, (uintptr_t) bhv_dummy_update,
    0x09000000
};

const BehaviorScript bhvOmmPossessedObjectCap[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x08000000,
    0x09000000
};

const BehaviorScript bhvOmmPossessedKoopaShell[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x08000000,
    0x09000000
};

const BehaviorScript bhvOmmActSelectStar[] = {
    OBJ_TYPE_LEVEL,
    0x11010001,
    0x08000000,
    0x09000000,
};

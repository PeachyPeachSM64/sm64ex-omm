#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behaviors
//

static void bhv_dummy_update() {
}

const BehaviorScript bhvOmmDummy[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_BEGIN_LOOP(),
    BHV_END_LOOP()
};

const BehaviorScript bhvOmmCappy[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
    BHV_END_LOOP()
};

const BehaviorScript bhvOmmPossessedObject[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_dummy_update),
    BHV_END_LOOP()
};

const BehaviorScript bhvOmmPossessedObjectCap[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
    BHV_END_LOOP()
};

const BehaviorScript bhvOmmPossessedKoopaShell[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_BEGIN_LOOP(),
    BHV_END_LOOP()
};

const BehaviorScript bhvOmmActSelectStar[] = {
    OBJ_TYPE_LEVEL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_SCALE(0, 0),
    BHV_BEGIN_LOOP(),
    BHV_END_LOOP(),
};

const BehaviorScript bhvOmmMainMenuMario[] = {
    OBJ_TYPE_DEFAULT,
    BHV_BEGIN_LOOP(),
    BHV_END_LOOP()
};

const BehaviorScript bhvOmmMainMenuCappy[] = {
    OBJ_TYPE_DEFAULT,
    BHV_BEGIN_LOOP(),
    BHV_END_LOOP()
};

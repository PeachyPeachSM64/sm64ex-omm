#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

const BehaviorScript bhvOmmDamageMario[] = {
    OBJ_TYPE_GENACTOR,
    BHV_DELAY(1),
    BHV_DEACTIVATE(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_damage_mario(struct Object *o, s32 interactType, s32 damage) {
    struct Object *dmg = spawn_object(o, MODEL_NONE, bhvOmmDamageMario);
    obj_set_params(dmg, interactType, damage, 0, 0, true);
    obj_reset_hitbox(dmg, 300, 600, 300, 600, 0, 300);
    return dmg;
}

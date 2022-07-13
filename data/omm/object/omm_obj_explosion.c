#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_explosion[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(explosion_geo),
};

//
// Behavior
//

static void omm_bhv_explosion_update() {
    struct Object *o = gCurrentObject;
    if (o->oTimer >= 10) {
        spawn_object(o, MODEL_SMOKE, bhvBobombBullyDeathSmoke);
        obj_mark_for_deletion(o);
        return;
    }

    obj_scale(o, 1.f);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 360, 480, 0, 0, 0, 120);
    obj_scale(o, 1.f + (o->oTimer / 3.f));
    o->oOpacity = max_s(0, 0xFF - (0x1C * o->oTimer));
    o->oAnimState++;
    omm_obj_process_interactions(o, OBJ_INT_PRESET_EXPLOSION);
}

const BehaviorScript omm_bhv_explosion[] = {
    OBJ_TYPE_SPECIAL,
    0x110100C1,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_explosion_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_explosion(struct Object *o) {
    struct Object *explosion  = obj_spawn_from_geo(o, omm_geo_explosion, omm_bhv_explosion);
    explosion->oAnimState     = -1;
    explosion->oGraphYOffset += 100.0f;
    explosion->oOpacity       = 255;
    create_sound_spawner(SOUND_GENERAL2_BOBOMB_EXPLOSION);
    set_environmental_camera_shake(SHAKE_ENV_EXPLOSION);
    return explosion;
}

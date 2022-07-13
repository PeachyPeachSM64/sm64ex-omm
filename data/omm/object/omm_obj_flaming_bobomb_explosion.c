#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_flaming_bobomb_explosion[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(explosion_geo),
};

//
// Behavior
//

static void omm_bhv_flaming_bobomb_explosion_update(void) {
    struct Object *o = gCurrentObject;
    if (o->oTimer >= 9) {
        spawn_object(o, MODEL_SMOKE, bhvBobombBullyDeathSmoke);
        obj_mark_for_deletion(o);
        return;
    }

    f32 scale = (f32) o->oTimer / 4.f + 1.0f;
    obj_scale(o, scale);
    obj_set_params(o, 0, 0, 0, 0, false);
    obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    o->oOpacity = max_s(0, 0xFF - (0x18 * o->oTimer));
    o->oAnimState++;
}

const BehaviorScript omm_bhv_flaming_bobomb_explosion[] = {
    OBJ_TYPE_SPECIAL,
    0x110100C1,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_flaming_bobomb_explosion_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_flaming_bobomb_explosion(struct Object *o) {
    struct Object *explosion  = obj_spawn_from_geo(o, omm_geo_flaming_bobomb_explosion, omm_bhv_flaming_bobomb_explosion);
    explosion->oAnimState     = -1;
    explosion->oGraphYOffset += 100.0f;
    explosion->oOpacity       = 255;
    create_sound_spawner(SOUND_GENERAL2_BOBOMB_EXPLOSION);
    set_environmental_camera_shake(SHAKE_ENV_EXPLOSION);
    return explosion;
}

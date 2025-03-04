#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_flaming_bobomb_explosion[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(explosion_geo),
};

//
// Behavior
//

static void bhv_omm_flaming_bobomb_explosion_update(void) {
    struct Object *o = gCurrentObject;
    if (o->oTimer >= 9) {
        spawn_object(o, MODEL_SMOKE, bhvBobombBullyDeathSmoke);
        obj_mark_for_deletion(o);
        return;
    }

    f32 scale = (f32) o->oTimer / 4.f + 1.f;
    obj_scale(o, scale);
    obj_set_params(o, 0, 0, 0, 0, false);
    obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    o->oOpacity = max_s(0, 0xFF - (0x18 * o->oTimer));
    o->oAnimState++;
}

const BehaviorScript bhvOmmFlamingBobombExplosion[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_flaming_bobomb_explosion_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_flaming_bobomb_explosion(struct Object *o) {
    struct Object *explosion  = obj_spawn_from_geo(o, omm_geo_flaming_bobomb_explosion, bhvOmmFlamingBobombExplosion);
    explosion->oAnimState     = -1;
    explosion->oGraphYOffset += 100;
    explosion->oOpacity       = 255;
    obj_create_sound_spawner(o, SOUND_GENERAL2_BOBOMB_EXPLOSION);
    set_environmental_camera_shake(SHAKE_ENV_EXPLOSION);
    return explosion;
}

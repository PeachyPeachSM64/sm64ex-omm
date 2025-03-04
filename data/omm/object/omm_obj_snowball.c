#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_snowball[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(white_particle_geo),
};

//
// Behavior
//

static void bhv_omm_snowball_delete(struct Object *o) {
    f32 scale = o->oScaleX;
    obj_create_sound_spawner(o, SOUND_OBJ_SNOW_SAND1);
    obj_spawn_particles(o, 8, MODEL_WHITE_PARTICLE, -o->hitboxDownOffset, 4 * scale, 2 * scale, 8 * scale, 4 * scale, -2 * sqrtf(scale), 0.3f * scale, 0.2f * scale);
    obj_mark_for_deletion(o);
}

static void bhv_omm_snowball_update() {
    struct Object *o = gCurrentObject;

    // In Mr. Blizzard's hand
    if (o->parentObj) {

        // Release if no longer in Mr. Blizzard's hand
        if (!o->parentObj->oHeldProjectile) {
            obj_set_throw_vel(o, o->parentObj, 35.f, 20.f);
            o->parentObj = NULL;
        }

        // Follow parent's hand
        else {
            s16 da = o->parentObj->oFaceAngleYaw - 0x4000;
            f32 dh =  88.f * o->parentObj->oScaleX;
            f32 dv = 140.f * o->parentObj->oScaleY;
            o->oPosX = o->parentObj->oPosX + dh * sins(da);
            o->oPosY = o->parentObj->oPosY + dv;
            o->oPosZ = o->parentObj->oPosZ + dh * coss(da);
            o->oFaceAngleYaw = o->parentObj->oFaceAngleYaw;
        }
    }

    // Released
    else {
        s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);
        o->oVelY -= 2.f;

        // Out of bounds, or collided with a wall/ceiling/floor
        if (check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS | OBJECT_STEP_CHECK_HIT_WALL | OBJECT_STEP_CHECK_HIT_FLOOR_WITH_NEGATIVE_VEL | OBJECT_STEP_CHECK_HIT_SLANTED_FLOOR | OBJECT_STEP_CHECK_HIT_SLANTED_CEILING)) {
            bhv_omm_snowball_delete(o);
            return;
        }
    }

    // Update
    vec3f_copy(o->oGfxPos, &o->oPosX);
    obj_set_params(o, 0, 0, 0, 0, !o->parentObj);
    obj_reset_hitbox(o, 30, 50, 0, 0, 15, 25);
    if (!o->parentObj) {
        struct Object *interacted = omm_obj_process_interactions(o, obj_get_interaction_flags_from_power(o->oObjectPower, OBJ_INT_PRESET_SNOWBALL));
        if (interacted && !omm_obj_is_collectible(interacted)) {
            bhv_omm_snowball_delete(o);
        }
    }
}

const BehaviorScript bhvOmmSnowball[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_snowball_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_snowball(struct Object *o) {
    struct Object *snowball = obj_spawn_from_geo(o, omm_geo_snowball, bhvOmmSnowball);
    snowball->parentObj = o;
    obj_set_interact_id(o, snowball);
    return snowball;
}

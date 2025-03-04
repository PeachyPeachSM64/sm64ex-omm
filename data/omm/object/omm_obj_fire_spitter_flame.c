#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_fire_spitter_flame_red[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(red_flame_geo)
};

const GeoLayout omm_geo_fire_spitter_flame_blue[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(blue_flame_geo)
};

//
// Behavior
//

static void bhv_omm_fire_spitter_flame_delete(struct Object *o) {
    obj_spawn_white_puff(o, NO_SOUND);
    obj_mark_for_deletion(o);
}

static void bhv_omm_fire_spitter_flame_update() {
    struct Object *o = gCurrentObject;
    if (o->oAction == 0) {
        s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);

        // Out of bounds/time, collided with a surface
        if (o->oTimer > 90 || check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS | OBJECT_STEP_CHECK_HIT_WALL | OBJECT_STEP_CHECK_HIT_SLANTED_FLOOR | OBJECT_STEP_CHECK_HIT_SLANTED_CEILING)) {
            bhv_omm_fire_spitter_flame_delete(o);
            return;
        }
    } else {
        o->oPosX += o->oVelX;
        o->oPosY += o->oVelY;
        o->oPosZ += o->oVelZ;
        obj_scale(o, relerp_0_1_f(o->oTimer, 0, 10, o->oFireSpitterScaleVel / 4.f, o->oFireSpitterScaleVel));

        // Out of time
        if (o->oTimer > 10) {
            obj_mark_for_deletion(o);
            return;
        }
    }

    // Update
    o->oAnimState++;
    obj_update_gfx(o);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 16, 24, 0, 0, 8, 12);
    struct Object *interacted = omm_obj_process_interactions(o, OBJ_INT_PRESET_FIREBALL);
    if (interacted && o->oAction == 0 && !omm_obj_is_collectible(interacted)) {
        bhv_omm_fire_spitter_flame_delete(o);
        return;
    }

    // Spawn fire trail
    if (o->oAction == 0) {
        struct Object *trail = obj_spawn_from_geo(o, o->oGraphNode->georef, bhvOmmBlarggFireTrail);
        obj_scale(trail, o->oScaleY);
        trail->oAction = (s32) (200.f * trail->oScaleY);
        trail->oPrevAction = trail->oAction;
        trail->oAnimState = (random_u16() & 7);
        trail->oTimer = 7;
    }
}

const BehaviorScript bhvOmmFireSpitterFlame[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_fire_spitter_flame_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_fire_spitter_flame(struct Object *o, bool flamethrower) {
    f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);
    const GeoLayout *flameGeo = obj_is_underwater(o, waterLevel) ? omm_geo_fire_spitter_flame_blue : omm_geo_fire_spitter_flame_red;
    struct Object *flame = obj_spawn_from_geo(o, flameGeo, bhvOmmFireSpitterFlame);
    s16 da = o->oFaceAngleYaw;
    flame->oPosX = o->oPosX;
    flame->oPosY = o->oPosY + 160.f * o->oScaleY;
    flame->oPosZ = o->oPosZ;
    flame->oFaceAngleYaw = da;
    flame->oAnimState = (random_u16() & 7);
    if (flamethrower) {
        flame->oFireSpitterScaleVel = 20.f * o->oScaleY;
        obj_set_throw_vel(flame, o, 240.f * o->oScaleX, 0.f);
        obj_scale(flame, 0.f);
        flame->oVelY = 16.f * o->oScaleY * sins(o->oTimer * 0x800);
        flame->oAction = 1;
        flame->oFlags |= OBJ_FLAG_DONT_RENDER_ON_INIT;
        obj_play_sound(o, SOUND_AIR_BLOW_FIRE);
    } else {
        obj_set_throw_vel(flame, o, 200.f * o->oScaleX, 0.f);
        obj_scale(flame, 16.f * o->oScaleY);
        flame->oAction = 0;
        obj_play_sound(o, SOUND_OBJ_FLAME_BLOWN);
    }
    obj_set_interact_id(o, flame);
    return flame;
}

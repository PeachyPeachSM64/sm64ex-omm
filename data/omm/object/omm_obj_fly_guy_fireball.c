#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_fly_guy_fireball[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(red_flame_geo)
};

//
// Behavior
//

static void bhv_omm_fly_guy_fireball_delete(struct Object *o) {
    obj_spawn_white_puff(o, NO_SOUND);
    obj_mark_for_deletion(o);
}

static void bhv_omm_fly_guy_fireball_update() {
    struct Object *o = gCurrentObject;
    s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_CHECK_ON_GROUND);
    o->oVelY -= 4.f;

    // Out of time/bounds
    if (o->oTimer > 90 || check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS)) {
        bhv_omm_fly_guy_fireball_delete(o);
        return;
    }

    // Touched water
    if (o->oPosY < find_water_level(o->oPosX, o->oPosZ)) {
        obj_create_sound_spawner(o, SOUND_GENERAL_FLAME_OUT);
        bhv_omm_fly_guy_fireball_delete(o);
        return;
    }

    // Bounce on floor
    if (o->oDistToFloor < 1.f) {
        o->oVelY = 10.f * o->oScaleY;
    }

    // Collided with a wall (or a slanted floor)
    if (check_object_step(o, step, OBJECT_STEP_CHECK_HIT_WALL)) {
        obj_bounce_on_wall(o, o->oWall ? o->oWall : o->oFloor, false, false);
    }

    // Collided with a ceiling
    if (check_object_step(o, step, OBJECT_STEP_CHECK_HIT_CEILING) && o->oCeil && o->oCeil->normal.y > -0.9f) {
        obj_bounce_on_wall(o, o->oCeil, false, false);
    }

    // Update
    o->oAnimState++;
    obj_update_gfx(o);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 16, 24, 0, 0, 8, 12);
    struct Object *interacted = omm_obj_process_interactions(o, OBJ_INT_PRESET_FIREBALL);
    if (interacted && !omm_obj_is_collectible(interacted)) {
        bhv_omm_fly_guy_fireball_delete(o);
        return;
    }

    // Spawn fire trail
    struct Object *trail = obj_spawn_from_geo(o, omm_geo_fire_smoke_red, bhvOmmBlarggFireTrail);
    obj_scale(trail, o->oScaleX);
    trail->oAction = (s32) (200.f * trail->oScaleX);
    trail->oPrevAction = trail->oAction;
    trail->oAnimState = (random_u16() & 7);
    trail->oTimer = 7;
}

const BehaviorScript bhvOmmFlyGuyFireball[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_fly_guy_fireball_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_fly_guy_fireball(struct Object *o) {
    struct Object *fireball = obj_spawn_from_geo(o, omm_geo_fly_guy_fireball, bhvOmmFlyGuyFireball);
    s16 da = o->oFaceAngleYaw;
    fireball->oPosX = o->oPosX + 40.f * o->oScaleX * sins(da);
    fireball->oPosY = o->oPosY + 60.f * o->oScaleY;
    fireball->oPosZ = o->oPosZ + 40.f * o->oScaleX * coss(da);
    obj_set_angle(fireball, 0, da, 0);
    obj_set_throw_vel(fireball, o, 24.f * o->oScaleX, 0.f);
    obj_scale(fireball, 2.f * o->oScaleY);
    fireball->oAnimState = (random_u16() & 7);
    obj_play_sound(o, SOUND_OBJ_FLAME_BLOWN);
    obj_set_interact_id(o, fireball);
    return fireball;
}

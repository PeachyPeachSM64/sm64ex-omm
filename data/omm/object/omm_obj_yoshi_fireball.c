#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_yoshi_fireball_red[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(red_flame_geo)
};

const GeoLayout omm_geo_yoshi_fireball_blue[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(blue_flame_geo)
};

//
// Behavior
//

static void bhv_omm_yoshi_fireball_delete(struct Object *o) {
    obj_spawn_white_puff(o, NO_SOUND);
    obj_mark_for_deletion(o);
}

static void bhv_omm_yoshi_fireball_update() {
    struct Object *o = gCurrentObject;
    obj_reset_hitbox(o, 12, 16, 0, 0, 6, 8);
    s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);

    // Out of time/bounds
    if (o->oTimer > 150 || check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS)) {
        bhv_omm_yoshi_fireball_delete(o);
        return;
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
    struct Object *interacted = omm_obj_process_interactions(o, OBJ_INT_PRESET_YOSHI_FIREBALL);
    if (interacted && !omm_obj_is_collectible(interacted) && !omm_obj_is_weak(interacted) && !omm_obj_is_bully(interacted)) {
        bhv_omm_yoshi_fireball_delete(o);
        return;
    }

    // Spawn fire trail
    struct Object *trail = obj_spawn_from_geo(o, o->oGraphNode->georef, bhvOmmBlarggFireTrail);
    obj_scale(trail, o->oScaleY);
    trail->oAction = (s32) (200.f * trail->oScaleY);
    trail->oPrevAction = trail->oAction;
    trail->oAnimState = (random_u16() & 7);
    trail->oTimer = 7;
}

const BehaviorScript bhvOmmYoshiFireball[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_yoshi_fireball_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_yoshi_fireball(struct Object *o) {
    f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);
    const GeoLayout *flameGeo = obj_is_underwater(o, waterLevel) ? omm_geo_yoshi_fireball_blue : omm_geo_yoshi_fireball_red;
    struct Object *flame = obj_spawn_from_geo(o, flameGeo, bhvOmmYoshiFireball);
    flame->oFaceAngleYaw = o->oFaceAngleYaw;
    flame->oAnimState = (random_u16() & 7);
    obj_set_throw_vel(flame, o, 80.f * o->oScaleX, 0.f);
    obj_scale(flame, 8.f * o->oScaleY);
    obj_play_sound(o, SOUND_OBJ_FLAME_BLOWN);
    obj_set_interact_id(o, flame);
    return flame;
}

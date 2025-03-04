#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_monty_mole_rock[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_ALPHA, pebble_seg3_dl_0301CB00),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END()
};

//
// Behavior
//

static void bhv_omm_monty_mole_rock_delete(struct Object *o) {
    obj_spawn_white_puff(o, NO_SOUND);
    obj_mark_for_deletion(o);
}

static void bhv_omm_monty_mole_rock_update() {
    struct Object *o = gCurrentObject;
    s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);
    o->oVelY -= 2.f;

    // Out of bounds, or collided with a wall/ceiling/floor
    if (check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS | OBJECT_STEP_CHECK_HIT_WALL | OBJECT_STEP_CHECK_HIT_FLOOR_WITH_NEGATIVE_VEL | OBJECT_STEP_CHECK_HIT_SLANTED_FLOOR | OBJECT_STEP_CHECK_HIT_SLANTED_CEILING)) {
        bhv_omm_monty_mole_rock_delete(o);
        return;
    }

    // Update
    obj_update_gfx(o);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 30, 50, 0, 0, 15, 25);
    struct Object *interacted = omm_obj_process_interactions(o, obj_get_interaction_flags_from_power(o->oObjectPower, OBJ_INT_PRESET_MONTY_MOLE_ROCK));
    if (interacted && !omm_obj_is_collectible(interacted)) {
        bhv_omm_monty_mole_rock_delete(o);
    }
}

const BehaviorScript bhvOmmMontyMoleRock[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_monty_mole_rock_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_monty_mole_rock(struct Object *o, f32 power) {
    struct Object *rock = obj_spawn_from_geo(o, omm_geo_monty_mole_rock, bhvOmmMontyMoleRock);
    f32 scaleFactor = lerp_f(power, 1.f, 2.5f);
    s16 da = o->oFaceAngleYaw + 0x4000;
    f32 dx = -40.f * o->oScaleX * sins(da);
    f32 dy = +25.f * o->oScaleY;
    f32 dz = -40.f * o->oScaleX * coss(da);
    obj_set_xyz(rock, o->oPosX + dx, o->oPosY + dy, o->oPosZ + dz);
    obj_set_angle(rock, 0, o->oFaceAngleYaw, 0);
    obj_set_throw_vel(rock, o, 30.f, 20.f);
    obj_scale(rock, 0.5f + scaleFactor);
    rock->oObjectPower = power;
    obj_play_sound(o, SOUND_OBJ_MONTY_MOLE_ATTACK);
    obj_set_interact_id(o, rock);
    return rock;
}

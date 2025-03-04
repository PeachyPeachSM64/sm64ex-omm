#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

static void bhv_omm_pokey_ball_delete(struct Object *o) {
    obj_spawn_white_puff(o, SOUND_OBJ_DEFAULT_DEATH);
    obj_mark_for_deletion(o);
}

static void bhv_omm_pokey_ball_loop() {
    struct Object *o = gCurrentObject;
    s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_CHECK_ON_GROUND | OBJ_STEP_STICKY_FEET | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);
    o->oVelY = max_f(o->oVelY - 4.f, -75.f);

    // Delete after some time or OoB
    if (o->oTimer > 150 || o->oForwardVel <= 0.f || check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS)) {
        bhv_omm_pokey_ball_delete(o);
        return;
    }

    // Change direction after touching a wall
    if (o->oWall || step >= OBJECT_STEP_HIT_CEILING) {
        s16 wAngle = (o->oWall ? atan2s(o->oWall->normal.z, o->oWall->normal.x) : o->oFaceAngleYaw);
        s16 dAngle = o->oFaceAngleYaw - wAngle;
        o->oFaceAngleYaw = (0x8000 + wAngle - dAngle);
        obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, o->oForwardVel);
    }

    // Interactions
    obj_reset_hitbox(o, 25, 40, 0, 0, 20, 0);
    obj_set_params(o, 0, 0, 0, 0, true);
    struct Object *interacted = omm_obj_process_interactions(o, OBJ_INT_PRESET_POKEY_BALL);
    if (interacted && !omm_obj_is_collectible(interacted)) {
        bhv_omm_pokey_ball_delete(o);
    }

    // Roll
    o->oGraphYOffset = o->oScaleY * 22.f;
    obj_rotate_billboard(o, 0x1000);
}

const BehaviorScript bhvOmmPokeyBall[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_pokey_ball_loop),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_pokey_ball(struct Object *o) {
    struct Object *ball = obj_spawn_from_geo(o, pokey_body_part_geo, bhvOmmPokeyBall);
    obj_set_angle(ball, 0, o->oFaceAngleYaw, 0);
    obj_set_throw_vel(ball, o, 50.f * o->oScaleX, 20.f * !obj_is_on_ground(o));
    obj_scale(ball, 4.f * o->oScaleY);
    obj_set_interact_id(o, ball);
    return ball;
}

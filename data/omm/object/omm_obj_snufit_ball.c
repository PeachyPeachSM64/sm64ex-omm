#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_snufit_ball[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(bowling_ball_geo),
};

//
// Behavior
//

static void bhv_omm_snufit_ball_delete(struct Object *o) {
    obj_spawn_white_puff(o, NO_SOUND);
    obj_mark_for_deletion(o);
}

static void bhv_omm_snufit_ball_loop() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    if (!p || !p->activeFlags) {
        obj_mark_for_deletion(o);
        return;
    }

    // Wait for x frames before shooting
    if (o->oAction > 0) {
        obj_scale(o, 0.f);
        obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
        obj_set_params(o, 0, 0, 0, 0, false);
        o->oVelX = 48.f * sins(o->oFaceAngleYaw);
        o->oVelY = 0.f;
        o->oVelZ = 48.f * coss(o->oFaceAngleYaw);
        o->oPosX = p->oGfxPos[0] + o->oVelX;
        o->oPosY = p->oGfxPos[1] + o->oVelY;
        o->oPosZ = p->oGfxPos[2] + o->oVelZ;
        o->oAction--;
        return;
    }

    // Update pos and collisions
    obj_play_sound(o, SOUND_OBJ_SNUFIT_SHOOT * (o->oTimer == 0));
    s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);
    if (o->oTimer > 60 || check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS | OBJECT_STEP_CHECK_HIT_WALL | OBJECT_STEP_CHECK_HIT_SLANTED_FLOOR | OBJECT_STEP_CHECK_HIT_SLANTED_CEILING)) {
        bhv_omm_snufit_ball_delete(o);
        return;
    }

    // Process interactions
    obj_scale(o, 0.1f * (o->oSnufitBallStrong ? 2.f : 1.f));
    obj_reset_hitbox(o, 250, 400, 0, 0, 100, 200);
    obj_set_params(o, 0, 0, 0, 0, true);
    struct Object *interacted = omm_obj_process_interactions(o, obj_get_interaction_flags_from_power(o->oSnufitBallStrong, OBJ_INT_PRESET_SNUFIT_BALL));
    if (interacted && !omm_obj_is_collectible(interacted)) {
        bhv_omm_snufit_ball_delete(o);
    }
}

const BehaviorScript bhvOmmSnufitBall[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_snufit_ball_loop),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_snufit_ball(struct Object *o, s32 delay, bool strong) {
    struct Object *ball = obj_spawn_from_geo(o, omm_geo_snufit_ball, bhvOmmSnufitBall);
    obj_scale(ball, 0.f);
    obj_reset_hitbox(ball, 0, 0, 0, 0, 0, 0);
    obj_set_params(ball, 0, 0, 0, 0, false);
    obj_set_throw_vel(ball, o, 45.f, 0.f);
    ball->oPosX = o->oGfxPos[0] + ball->oVelX;
    ball->oPosY = o->oGfxPos[1] + ball->oVelY;
    ball->oPosZ = o->oGfxPos[2] + ball->oVelZ;
    ball->oAction = delay;
    ball->oSnufitBallStrong = strong;
    obj_set_interact_id(o, ball);
    return ball;
}

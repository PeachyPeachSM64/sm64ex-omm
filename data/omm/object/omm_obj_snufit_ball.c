#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_snufit_ball[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(bowling_ball_geo),
};

//
// Behavior
//

static void omm_bhv_snufit_ball_delete(struct Object *o) {
    obj_spawn_white_puff(o, -1);
    obj_mark_for_deletion(o);
}

static void omm_bhv_snufit_ball_loop() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    if (!p || p->activeFlags == ACTIVE_FLAG_DEACTIVATED) {
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
    perform_object_step(o, OBJ_STEP_UPDATE_HOME);
    if (o->oTimer > 60 || o->oWall || o->oCeil || o->oDistToFloor <= 0.f) {
        omm_bhv_snufit_ball_delete(o);
        return;
    }

    // Process interactions
    obj_scale(o, 0.1f * (o->oSnufitBallStrong ? 2.f : 1.f));
    obj_reset_hitbox(o, 250, 400, 0, 0, 100, 200);
    obj_set_params(o, 0, 0, 0, 0, true);
    struct Object *interacted = omm_obj_process_interactions(o, (o->oSnufitBallStrong ? OBJ_INT_PRESET_SNUFIT_BALL_LARGE : OBJ_INT_PRESET_SNUFIT_BALL_SMALL));
    if (interacted && !omm_obj_is_collectible(interacted)) {
        omm_bhv_snufit_ball_delete(o);
    }
}

const BehaviorScript omm_bhv_snufit_ball[] = {
    OBJ_TYPE_GENACTOR,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_snufit_ball_loop,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_snufit_ball(struct Object *o, s32 delay, bool strong) {
    struct Object *ball = obj_spawn_from_geo(o, omm_geo_snufit_ball, omm_bhv_snufit_ball);
    obj_scale(ball, 0.f);
    obj_reset_hitbox(ball, 0, 0, 0, 0, 0, 0);
    obj_set_params(ball, 0, 0, 0, 0, false);
    ball->oVelX = 48.f * sins(o->oFaceAngleYaw);
    ball->oVelY = 0.f;
    ball->oVelZ = 48.f * coss(o->oFaceAngleYaw);
    ball->oPosX = o->oGfxPos[0] + ball->oVelX;
    ball->oPosY = o->oGfxPos[1] + ball->oVelY;
    ball->oPosZ = o->oGfxPos[2] + ball->oVelZ;
    ball->oAction = delay;
    ball->oSnufitBallStrong = strong;
    return ball;
}

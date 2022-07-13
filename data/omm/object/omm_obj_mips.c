#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
//                       |   |
//                +------+   +------+
//                | 2             1 |
//                |   +---------+   |
// ------+        |   |         |   |
//       |        |   |  +---+  | 0 |
// --+   +--------+   |  | 9 |  +---+
//   | 5        4   3 |  |   |
//   |   +----+   +---+  |   |
// --+   |    |   |      |   |
//       |    |   |      |   |
// --+   |    |   |      |   |
//   |   +----+   +------+   |
//   | 6        7          8 |
//   +-----------------------+
//
static Vec3f sOmmMipsWaypointPositions[] = {
    { -3850, 0, -1200 },
    { -3850, 0,   650 },
    { -1850, 0,   650 },
    { -1850, 0, -1500 },
    { -1430, 0, -1500 },
    {  -200, 0, -1500 },
    {  -200, 0, -3100 },
    { -1430, 0, -3100 },
    { -2920, 0, -3100 },
    { -2920, 0, -1435 },
};
static const s32 sOmmMipsWaypointConnections[][4] = {
    { 1, -1, -1, -1 },
    { 0,  2, -1, -1 },
    { 1,  3, -1, -1 },
    { 2,  4, -1, -1 },
    { 3,  5,  7, -1 },
    { 4,  6, -1, -1 },
    { 5,  7, -1, -1 },
    { 4,  6,  8, -1 },
    { 7,  9, -1, -1 },
    { 8, -1, -1, -1 },
};
static const s32 sOmmMipsEscapePaths[][8] = {
    { 1, 2, 3, 4, 5, 6,    -1 },
    { 2, 3, 4, 7, 8, 9,    -1 },
    { 3, 4, 7, 6, 5,       -1 },
    { 4, 5, 6, 7, 8, 9,    -1 },
    { 7, 6, 5, 4, 3, 2, 1, -1 },
    { 6, 7, 4, 3, 2, 1, 0, -1 },
    { 7, 4, 3, 2, 1, 0,    -1 },
    { 6, 5, 4, 3, 2, 1, 0, -1 },
    { 7, 4, 3, 2, 1, 0,    -1 },
    { 8, 7, 6, 5, 4, 3,    -1 }
};

//
// Behavior
//

#define OMM_MIPS_ACT_WAIT 0
#define OMM_MIPS_ACT_MOVE 1
#define OMM_MIPS_ACT_IDLE 2

static s32 omm_bhv_mips_get_nearest_waypoint(struct Object *o) {
    s32 nearestWaypoint = -1;
    f32 nearestDist = 1e10f;
    for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmMipsWaypointPositions); ++i) {
        f32 dist = vec3f_hdist(sOmmMipsWaypointPositions[i], &o->oPosX);
        if (dist < nearestDist) {
            nearestWaypoint = i;
            nearestDist = dist;
        }
    }
    return nearestWaypoint;
}

static s32 omm_bhv_mips_get_target_waypoint(struct Object *o) {
    s32 nearestWaypoint = omm_bhv_mips_get_nearest_waypoint(o);
    f32 marioDeltaDists[4] = { -1e10f, -1e10f, -1e10f, -1e10f };
    f32 cappyDeltaDists[4] = { 0, 0, 0, 0 };

    // Predetermined path
    if (o->oMipsEscapePath != -1) {
        s32 targetWaypoint = sOmmMipsEscapePaths[o->oMipsEscapePath][o->oMipsEscapeIndex];
        o->oMipsEscapeIndex++;
        if (sOmmMipsEscapePaths[o->oMipsEscapePath][o->oMipsEscapeIndex] == -1) { // Next point
            o->oMipsEscapePath = -1;
        }
        return targetWaypoint;
    }

    // Compute delta dists between Mips positions, Mario and Cappy
    for (s32 i = 0; i != 4; ++i) {
        s32 index = sOmmMipsWaypointConnections[nearestWaypoint][i];
        if (index != -1) {
            Vec3f dirMipsToPoint = { sOmmMipsWaypointPositions[index][0] - o->oPosX, 0, sOmmMipsWaypointPositions[index][2] - o->oPosZ };
            vec3f_normalize(dirMipsToPoint);
            Vec3f currMipsPos = {
                o->oPosX,
                o->oPosY,
                o->oPosZ,
            };
            Vec3f nextMipsPos = {
                o->oPosX + dirMipsToPoint[0] * o->oMipsForwardVelocity,
                o->oPosY + dirMipsToPoint[1] * o->oMipsForwardVelocity,
                o->oPosZ + dirMipsToPoint[2] * o->oMipsForwardVelocity,
            };

            // Delta distance between Mips and Mario
            f32 currDistToMario = vec3f_hdist(currMipsPos, gMarioState->pos);
            f32 nextDistToMario = vec3f_hdist(nextMipsPos, gMarioState->pos);
            marioDeltaDists[i] = nextDistToMario - currDistToMario;

            // Delta distance between Mips and Cappy
            struct Object *cappy = omm_cappy_get_object();
            if (cappy) {
                f32 currDistToCappy = vec3f_hdist(currMipsPos, &cappy->oPosX);
                f32 nextDistToCappy = vec3f_hdist(nextMipsPos, &cappy->oPosX);
                cappyDeltaDists[i] = nextDistToCappy - currDistToCappy;
            }
        }
    }

    // First, select points that have a delta dist >= 0
    s32 waypoints[4];
    s32 numWaypoints = 0;
    for (s32 i = 0; i != 4; ++i) {
        if (marioDeltaDists[i] >= 0 && cappyDeltaDists[i] >= 0) {
            waypoints[numWaypoints++] = sOmmMipsWaypointConnections[nearestWaypoint][i];
        }
    }
    if (numWaypoints > 0) {
        return waypoints[random_u16() % numWaypoints];
    }

    // If no such point was found, select the point that have the maximal delta dist
    f32 maxDeltaDist = -1e10f;
    s32 waypoint = -1;
    for (s32 i = 0; i != 4; ++i) {
        f32 deltaDist = min_f(marioDeltaDists[i], cappyDeltaDists[i]);
        if (deltaDist > maxDeltaDist) {
            waypoint = sOmmMipsWaypointConnections[nearestWaypoint][i];
            maxDeltaDist = deltaDist;
        }
    }
    return waypoint;
}

static bool omm_bhv_mips_should_move(struct Object *o) {
    struct Object *mario = gMarioObject;
    struct Object *cappy = omm_cappy_get_object();
    return (mario && obj_get_horizontal_distance(o, mario) < 600.f) ||
           (cappy && obj_get_horizontal_distance(o, cappy) < 600.f);
}

static bool omm_bhv_mips_should_jump(struct Object *o) {
    struct Object *mario = gMarioObject;
    struct Object *cappy = omm_cappy_get_object();
    return (mario && obj_get_horizontal_distance(o, mario) < 300.f && !(gMarioState->action & ACT_FLAG_AIR)) ||
           (cappy && obj_get_horizontal_distance(o, cappy) < 300.f);
}

static void omm_bhv_mips_play_step_sound(struct Object *o) {
    if (o->oPosY < find_water_level(o->oPosX, o->oPosZ)) {
        obj_play_sound(o, SOUND_OBJ_MIPS_RABBIT_WATER);
        spawn_object(o, MODEL_NONE, bhvShallowWaterSplash);
    } else {
        obj_play_sound(o, SOUND_OBJ_MIPS_RABBIT);
    }
}

static void omm_bhv_mips_step(struct Object *o, f32 fVel, bool dropToFloor) {
    obj_set_forward_vel(o, o->oMoveAngleYaw, 1.f, fVel);
    perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_CHECK_ON_GROUND);
    if (dropToFloor) {
        obj_drop_to_floor(o);
        o->oVelY = 0.f;
    } else {
        o->oVelY = !obj_is_on_ground(o) * max_f(o->oVelY - o->oGravity, -75.f);
    }
}

//
// Init
//

static void omm_bhv_mips_init() {
    struct Object *o = gCurrentObject;
    obj_anim_play_with_sound(o, 0, 1.f, 0, true);
}

//
// Free state
//

static void omm_bhv_mips_act_wait(struct Object *o) {
    f32 dx = gMarioState->pos[0] - o->oPosX;
    f32 dz = gMarioState->pos[2] - o->oPosZ;
    if (dx != 0.f || dz != 0.f) {
        s16 angleToMario = atan2s(dz, dx);
        o->oFaceAngleYaw = angleToMario - approach_s32((s16) (angleToMario - o->oFaceAngleYaw), 0, 0x1000, 0x1000);
        o->oMoveAngleYaw = o->oFaceAngleYaw;
    }
    omm_bhv_mips_step(o, 0.f, true);
    obj_anim_play_with_sound(o, 0, 1.f, 0, false);
    if (o->oMipsEscapePath != -1 || (o->oTimer >= (4 - 2 * o->oMipsGrabbedCounter) && omm_bhv_mips_should_move(o))) {
        s32 nextWaypoint = omm_bhv_mips_get_target_waypoint(o);
        if (nextWaypoint != -1) {
            o->oAction = OMM_MIPS_ACT_MOVE;
            o->oMipsTargetWaypoint = nextWaypoint;
            obj_anim_play_with_sound(o, 1, 1.f, 0, true);
        }
    }
}

static void omm_bhv_mips_act_move(struct Object *o) {
    f32 dx = sOmmMipsWaypointPositions[o->oMipsTargetWaypoint][0] - o->oPosX;
    f32 dz = sOmmMipsWaypointPositions[o->oMipsTargetWaypoint][2] - o->oPosZ;
    s16 angle = atan2s(dz, dx);
    o->oFaceAngleYaw = angle;
    o->oMoveAngleYaw = angle;
    switch (o->oSubAction) {

        // Running
        case 0: {
            omm_bhv_mips_step(o, o->oMipsForwardVelocity, true);
            if (omm_bhv_mips_should_jump(o)) {
                o->oVelY = 70.f;
                o->oFloor = NULL;
                o->oSubAction = 1;
                o->oForwardVel = o->oMipsForwardVelocity * (0.5f + 0.3f * random_float());
                obj_anim_play_with_sound(o, 1, 1.f, 0, true);
                omm_bhv_mips_play_step_sound(o);
            } else if (obj_anim_is_near_end(o)) {
                omm_bhv_mips_play_step_sound(o);
            }
        } break;

        // Jumping
        case 1: {
            omm_bhv_mips_step(o, o->oForwardVel, false);
            if (obj_is_on_ground(o)) {
                o->oSubAction = 0;
            }
        } break;

        // Waypoint reached
        case 2: {
            if (obj_is_on_ground(o)) {
                obj_anim_play_with_sound(o, 0, 1.f, 0, true);
                o->oMipsCurrentWaypoint = o->oMipsTargetWaypoint;
                o->oAction = OMM_MIPS_ACT_WAIT;
            } else {
                omm_bhv_mips_step(o, 0.f, false);
            }
        } break;
    }

    // If waypoint reached, update sub-action
    f32 distToTarget = vec3f_hdist(sOmmMipsWaypointPositions[o->oMipsTargetWaypoint], &o->oPosX);
    if (distToTarget < 50.f) {
        o->oSubAction = 2;
    }
}

static void omm_bhv_mips_act_idle(struct Object *o) {
    switch (o->oSubAction) {

        // Falling
        case 0: {
            omm_bhv_mips_step(o, 0.f, false);
            obj_anim_set_frame(o, 0);
            if (obj_is_on_ground(o)) {
                o->oSubAction = 1;
                o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
                o->oMoveAngleYaw = o->oFaceAngleYaw;
                obj_anim_play_with_sound(o, 0, 1.f, 0, true);
                if (o->oPosY < find_water_level(o->oPosX, o->oPosZ)) {
                    spawn_object(o, MODEL_NONE, bhvShallowWaterSplash);
                }
            }
        } break;
        
        // Idle
        case 1: {
            omm_bhv_mips_step(o, 0.f, true);
        } break;
    }
}

static void omm_bhv_mips_free(struct Object *o) {
    switch (o->oAction) {
        case OMM_MIPS_ACT_WAIT: omm_bhv_mips_act_wait(o); break;
        case OMM_MIPS_ACT_MOVE: omm_bhv_mips_act_move(o); break;
        case OMM_MIPS_ACT_IDLE: omm_bhv_mips_act_idle(o); break;
    }
}

//
// Held state
//

static void omm_bhv_mips_held(struct Object *o) {
    if (!(o->oInteractionSubtype & INT_SUBTYPE_DROP_IMMEDIATELY)) {
        o->oIntangibleTimer = -1;
        o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        obj_anim_play_with_sound(o, 4, 1.f, 0, false);
        cur_obj_set_pos_relative(gMarioObject, 0, 60.f, 100.f);
        if (o->oMipsGrabbedCounter < 3) {
            if (set_mario_npc_dialog(1) == 2) {
                o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
                if (cutscene_object_with_dialog(CUTSCENE_DIALOG, o, OMM_DIALOG_SPARKLY_MIPS_1 + o->oMipsGrabbedCounter)) {
                    o->oMipsEscapePath = omm_bhv_mips_get_nearest_waypoint(o);
                    o->oMipsEscapeIndex = 0;
                    o->oMipsGrabbedCounter++;
                    o->oMipsForwardVelocity += 15.f;
                    o->oInteractionSubtype |= INT_SUBTYPE_DROP_IMMEDIATELY;
                    o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
                    set_mario_npc_dialog(0);
                }
            }
        }
    }
}

//
// Dropped state
//

static void omm_bhv_mips_dropped(struct Object *o) {
    if (o->oMipsGrabbedCounter < 3) {
        o->oAction = OMM_MIPS_ACT_WAIT;
    } else {
        o->oAction = OMM_MIPS_ACT_IDLE;
    }
    o->oSubAction = 0;
    o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    o->oInteractionSubtype &= ~INT_SUBTYPE_DROP_IMMEDIATELY;
    o->oIntangibleTimer = 0;
    o->oHeldState = HELD_FREE;
    o->oVelY = 0.f;
}

//
// Update
//

static void omm_bhv_mips_update() {
    struct Object *o = gCurrentObject;
    switch (o->oHeldState) {
        case HELD_FREE: omm_bhv_mips_free(o); break;
        case HELD_HELD: omm_bhv_mips_held(o); break;
        case HELD_THROWN: omm_bhv_mips_dropped(o); break;
        case HELD_DROPPED: omm_bhv_mips_dropped(o); break;
    }
}

const BehaviorScript omm_bhv_mips[] = {
    OBJ_TYPE_GENACTOR,
    0x11010409,
    0x27260000, (uintptr_t) mips_seg6_anims_06015634,
    0x0C000000, (uintptr_t) omm_bhv_mips_init,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_mips_update,
    0x09000000
};

//
// Spawner
//

struct Object *omm_spawn_mips(struct Object *o, f32 x, f32 y, f32 z, f32 fVel) {
    struct Object *mips = spawn_object(o, MODEL_MIPS, omm_bhv_mips);
    obj_set_pos(mips, x, y, z);
    obj_set_home(mips, x, y, z);
    obj_set_angle(mips, 0, 0, 0);
    obj_scale(mips, 1.f);
    obj_drop_to_floor(mips);
    obj_set_params(mips, INTERACT_GRABBABLE, 0, 0, 0, true);
    obj_reset_hitbox(mips, 30, 40, 0, 0, 30, 0);
    mips->oGravity = 10.f;
    mips->oFriction = 0.89f;
    mips->oBuoyancy = 1.2f;
    mips->oInteractionSubtype = INT_SUBTYPE_HOLDABLE_NPC;
    mips->oMipsForwardVelocity = fVel;
    mips->oMipsCurrentWaypoint = omm_bhv_mips_get_nearest_waypoint(mips);
    mips->oMipsTargetWaypoint = 0;
    mips->oMipsGrabbedCounter = 0;
    mips->oMipsEscapePath = -1;
    mips->oMipsEscapeIndex = 0;
    return mips;
}
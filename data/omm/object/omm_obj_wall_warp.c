#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

static bool bhv_omm_wall_warp_check_availability(struct Object *p, s32 kind) {
    switch (kind) {

        // Bowser 4
        case 1: return omm_sparkly_is_bowser_4_unlocked(gOmmSparklyMode) && gCurrLevelNum == LEVEL_CASTLE_GROUNDS && gCurrAreaIndex == 1;

        // Peachy room
        case 2: return p && p->oBehParams2ndByte == 0b111;

        // Default, always available
        default: return true;
    }
}

static void bhv_omm_wall_warp_update() {
    struct Object *o = gCurrentObject;
    struct MarioState *m = gMarioState;

    // Availability
    if (!bhv_omm_wall_warp_check_availability(o->parentObj, o->oWallWarpKind)) {
        obj_mark_for_deletion(o);
        return;
    }

    // Gfx
    obj_update_gfx(o);

    // Spawn sparkles
    if (o->oWallWarpSparkleGeo) {
        struct Object *sparkle = obj_spawn_from_geo(o, o->oWallWarpSparkleGeo, bhvOmmSparklyStarSparkle);
        Vec3f transPos = { random_float_n1_p1(), random_float_n1_p1(), 0 };
        Vec3s transRot = { 0, o->oFaceAngleYaw, 0 };
        Vec3f transSca = { o->oWallWarpHalfWidth, o->oWallWarpHalfHeight, o->oWallWarpHalfWidth };
        vec3f_transform(&sparkle->oPosX, transPos, &o->oPosX, transRot, transSca);
        obj_set_forward_and_y_vel(sparkle, 20.f * random_float(), 0.f);
        obj_scale_random(sparkle, 1.f, 0.5f);
        sparkle->oAnimState = (random_u16() & 1);
        sparkle->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        sparkle->oAction = 30;
        sparkle->parentObj = sparkle;
    }

    // Check collision
    if (m->action != ACT_OMM_WARPING) {
        f32 *pos, width, height, depth;
        if (omm_mario_is_capture(m)) {
            pos = &gOmmCapture->oPosX;
            width = omm_capture_get_hitbox_radius(gOmmCapture) * 0.75f;
            height = omm_capture_get_hitbox_height(gOmmCapture);
            depth = omm_capture_get_wall_hitbox_radius(gOmmCapture) + 5.f;
        } else {
            pos = m->pos;
            width = 30;
            height = 120 - 40 * ((m->action & (ACT_FLAG_DIVING | ACT_FLAG_SHORT_HITBOX)) != 0);
            depth = 60 + abs_f(m->forwardVel);
        }

        // Project Mario's pos on painting plane
        Vec3f n  = { sins(o->oFaceAngleYaw), 0, coss(o->oFaceAngleYaw) };
        Vec3f e0 = { sins(o->oFaceAngleYaw + 0x4000), 0, coss(o->oFaceAngleYaw + 0x4000) };
        Vec3f e1 = { 0, 1, 0 };
        Vec3f pp; vec3f_project_point(pp, pos, &o->oPosX, n);

        // Check depth
        Vec3f vn; vec3f_dif(vn, pos, pp);
        f32 tn = vec3f_dot(vn, n);
        if (0 < tn && tn < depth) {

            // Check horizontal axis
            Vec3f p0; f32 t0;
            Vec3f oe0; vec3f_sum(oe0, &o->oPosX, e0);
            vec3f_get_projected_point_on_line(p0, &t0, pp, &o->oPosX, oe0);
            if (-o->oWallWarpHalfWidth + width < t0 && t0 < o->oWallWarpHalfWidth - width) {

                // Check vertical axis
                Vec3f p1; f32 t1;
                Vec3f oe1; vec3f_sum(oe1, &o->oPosX, e1);
                vec3f_get_projected_point_on_line(p1, &t1, pp, &o->oPosX, oe1);
                if (-o->oWallWarpHalfHeight < t1 && t1 < o->oWallWarpHalfHeight - height) {

                    // Prepare to warp
                    omm_cappy_unload();
                    omm_mario_unpossess_object_before_warp(m);
                    omm_mario_set_action(m, ACT_OMM_WARPING, o->oWallWarpDuration, 0xFFFF);
                    play_sound(SOUND_MENU_ENTER_HOLE, gGlobalSoundArgs);
                    play_transition(
                        o->oWallWarpTransitionType,
                        o->oWallWarpTransitionDuration,
                        (o->oWallWarpTransitionColor >> 16) & 0xFF,
                        (o->oWallWarpTransitionColor >> 8) & 0xFF,
                        (o->oWallWarpTransitionColor >> 0) & 0xFF
                    );
                    m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                    m->interactObj = o;
                    m->usedObj = o;
                    omm_mario_unset_cap(m);
                }
            }
        }
    }
}

const BehaviorScript bhvOmmWallWarp[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_wall_warp_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_wall_warp(
    struct Object *o, s32 kind, f32 centerX, f32 centerY, f32 centerZ, f32 halfWidth, f32 halfHeight, s16 yaw,
    s16 destLevel, s16 destArea, s16 destNode, s16 warpDuration, s16 transitionType, s16 transitionDuration, u32 transitionColor,
    f32 scaleX, f32 scaleY, f32 scaleZ, const GeoLayout *wallGeo, const GeoLayout *sparkleGeo
) {
    if (bhv_omm_wall_warp_check_availability(o, kind)) {
        struct Object *warp = obj_spawn_from_geo(o, wallGeo, bhvOmmWallWarp);
        warp->parentObj = o;
        warp->oWallWarpKind = kind;
        warp->oWallWarpDestLevel = destLevel;
        warp->oWallWarpDestArea = destArea;
        warp->oWallWarpDestNode = destNode;
        warp->oWallWarpDuration = warpDuration;
        warp->oWallWarpHalfWidth = halfWidth;
        warp->oWallWarpHalfHeight = halfHeight;
        warp->oWallWarpTransitionType = transitionType;
        warp->oWallWarpTransitionDuration = transitionDuration;
        warp->oWallWarpTransitionColor = transitionColor;
        warp->oWallWarpSparkleGeo = sparkleGeo;
        obj_set_xyz(warp, centerX, centerY, centerZ);
        obj_set_angle(warp, 0, yaw, 0);
        obj_set_scale(warp, scaleX, scaleY, scaleZ);
        return warp;
    }
    return NULL;
}

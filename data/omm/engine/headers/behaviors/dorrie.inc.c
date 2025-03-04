#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

static const f32 sDorrieCircles[] = {
    -3550, 3550, 4000, // Bounds
    -3550, 3550, 1100, // Central island
      500, 2900, 1100, // Star 6
    -1250, 5885,  500, // Metal cap
      -60, 7000, 1600,
};

void bhv_dorrie_update_pos(struct Object *obj, f32 radius) {
    for (u8 done = FALSE; !done;) {
        Vec3f pos;
        obj_pos_as_vec3f(obj, pos);

        // Keep Dorrie in bounds
        const f32 *bounds = sDorrieCircles;
        f32 dx = obj->oPosX - bounds[0];
        f32 dz = obj->oPosZ - bounds[1];
        f32 dist = sqrtf(sqr_f(dx) + sqr_f(dz));
        if (dist + radius > bounds[2]) {
            obj->oPosX = bounds[0] + (bounds[2] - radius) * (dx / dist);
            obj->oPosZ = bounds[1] + (bounds[2] - radius) * (dz / dist);
        }

        // Move Dorrie out of circles
        for (s32 i = 3; i != array_length(sDorrieCircles); i += 3) {
            const f32 *circle = sDorrieCircles + i;
            f32 dx = obj->oPosX - circle[0];
            f32 dz = obj->oPosZ - circle[1];
            f32 dist = sqrtf(sqr_f(dx) + sqr_f(dz));
            if (dist < circle[2] + radius) {
                obj->oPosX = circle[0] + (circle[2] + radius) * (dx / dist);
                obj->oPosZ = circle[1] + (circle[2] + radius) * (dz / dist);
            }
        }

        // Has Dorrie moved?
        done = mem_eq(pos, &obj->oPosX, sizeof(Vec3f));
    }
    o->oPosY = o->oHomeY + o->oDorrieOffsetY;
}

static void bhv_dorrie_act_move(void) {
    s16 startYaw = o->oMoveAngleYaw;
    o->oDorrieNeckAngle = -0x26F4;
    cur_obj_init_animation_with_sound(1);

    if (o->oDorrieForwardDistToMario < 320.f && o->oDorrieGroundPounded) {
        cur_obj_play_sound_2(SOUND_OBJ_DORRIE);
        o->collisionData = (void *) dorrie_seg6_collision_0600FBB8;
        o->oAction = DORRIE_ACT_LOWER_HEAD;
        o->oForwardVel = 0.f;
        o->oDorrieYawVel = 0;
    } else {
        s16 targetYaw;
        s16 targetSpeed;
        if (gMarioObject->platform == o) {
            targetYaw = gMarioObject->oFaceAngleYaw;
            targetSpeed = 10;
        } else {
            s16 circularTurn = 0x4000 - atan2s(2000.f, o->oDorrieDistToHome - 2000.f);
            if ((s16) (o->oMoveAngleYaw - o->oDorrieAngleToHome) < 0) {
                circularTurn = -circularTurn;
            }
            targetYaw = o->oDorrieAngleToHome + circularTurn;
            targetSpeed = 5;
        }
        obj_forward_vel_approach(targetSpeed, 0.5f);
        o->oDorrieYawVel = approach_s16_symmetric(o->oDorrieYawVel, (s16)(targetYaw - o->oMoveAngleYaw) / 50, 5);
        o->oMoveAngleYaw += o->oDorrieYawVel;
    }

    o->oAngleVelYaw = o->oMoveAngleYaw - startYaw;
}

static void bhv_dorrie_act_lower_head(void) {
    if (cur_obj_init_anim_check_frame(2, 35)) {
        cur_obj_reverse_animation();
        if (gMarioObject->platform == o) {
            if (o->oDorrieOffsetY == -17.f && o->oDorrieForwardDistToMario > 780.f && set_mario_npc_dialog(2) == 1) {
                o->oDorrieLiftingMario = TRUE;
                o->oAction = DORRIE_ACT_RAISE_HEAD;
                o->oDorrieHeadRaiseSpeed = 0;
            } else if (o->oDorrieForwardDistToMario > 320.f) {
                o->oTimer = 0;
            }
        } else if (o->oTimer > 150) {
            o->oDorrieLiftingMario = FALSE;
            o->oAction = DORRIE_ACT_RAISE_HEAD;
            o->oDorrieHeadRaiseSpeed = 0;
        }
    } else {
        o->oDorrieNeckAngle += 0x115;
    }
}

static void bhv_dorrie_act_raise_head(void) {
    o->collisionData = (void *) dorrie_seg6_collision_0600F644;

    if (cur_obj_check_if_near_animation_end()) {
        o->oAction = DORRIE_ACT_MOVE;
    } else if (o->oDorrieLiftingMario && o->oAnimFrame < 74) {
        if (set_mario_npc_dialog(2) == 2) {
            o->oDorrieHeadRaiseSpeed += 0x1CC;
            if (cur_obj_check_anim_frame(73)) {
                set_mario_npc_dialog(0);
            }
            s16 startAngle = o->oDorrieNeckAngle;
            o->oDorrieNeckAngle -= (s16) absf(370.f * sins(o->oDorrieHeadRaiseSpeed));
            f32 xzDisp = 440.f * (coss(o->oDorrieNeckAngle) - coss(startAngle));
            f32 yDisp = 440.f * (sins(o->oDorrieNeckAngle) - sins(startAngle));
            set_mario_pos(gMarioObject->oPosX + xzDisp * sins(o->oMoveAngleYaw), gMarioObject->oPosY - yDisp, gMarioObject->oPosZ + xzDisp * coss(o->oMoveAngleYaw));
        } else {
            cur_obj_reverse_animation();
        }
    }
}

void bhv_dorrie_update(void) {
    if (!(o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)) {
        o->oDorrieForwardDistToMario = o->oDistanceToMario * coss(o->oAngleToMario - o->oMoveAngleYaw);

        obj_perform_position_op(0);
        cur_obj_move_using_fvel_and_gravity();
        o->oDorrieAngleToHome = cur_obj_angle_to_home();
        o->oDorrieDistToHome = cur_obj_lateral_dist_to_home();
        o->oDorrieGroundPounded = cur_obj_is_mario_ground_pounding_platform();

        // Shift dorrie's radius to account for her neck
        f32 radius = 600.f + max_f(0.f, 440.f * sins(o->oDorrieNeckAngle + 0x26F4));
        bhv_dorrie_update_pos(o, radius);

        f32 maxOffsetY = 0.f;
        if (gMarioObject->platform == o) {
            maxOffsetY = -17.f;
            if (o->oDorrieOffsetY >= 0.f) {
                if (o->oDorrieGroundPounded) {
                    o->oDorrieVelY = -15.f;
                } else {
                    o->oDorrieVelY = -6.f;
                }
            }
        }

        o->oDorrieOffsetY += o->oDorrieVelY;
        approach_f32_ptr(&o->oDorrieVelY, 3.f, 1.f);
        if (o->oDorrieVelY > 0.f && o->oDorrieOffsetY > maxOffsetY) {
            o->oDorrieOffsetY = maxOffsetY;
        }

        switch (o->oAction) {
            case DORRIE_ACT_MOVE: bhv_dorrie_act_move(); break;
            case DORRIE_ACT_LOWER_HEAD: bhv_dorrie_act_lower_head(); break;
            case DORRIE_ACT_RAISE_HEAD: bhv_dorrie_act_raise_head(); break;
        }

        obj_perform_position_op(1);
    }
}

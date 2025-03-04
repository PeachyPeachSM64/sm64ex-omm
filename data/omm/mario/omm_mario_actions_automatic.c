#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_act_hanging_update(struct MarioState *m) {
    m->vel[1] = 0.f;
    m->pos[1] = m->ceilHeight - 160.f;
    gOmmMario->state.peakHeight = m->pos[1];
    mario_set_forward_vel(m, 0.f);
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
}

//////////
// SM64 //
//////////

// Fix fake pole grabs
static s32 omm_act_grab_pole(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        struct Object *pole = m->usedObj;
        f32 poleMin = pole->oPosY - m->usedObj->hitboxDownOffset + 5.f;
        f32 poleMax = pole->oPosY + m->usedObj->hitboxHeight - 100.f;
        m->pos[0] = m->usedObj->oPosX;
        m->pos[2] = m->usedObj->oPosZ;
        m->pos[1] = clamp_f(m->pos[1], poleMin, poleMax);
        m->marioObj->oMarioPolePos = m->pos[1] - pole->oPosY;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_holding_pole(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY && (m->controller->buttonPressed & Z_TRIG)) {
        mario_set_forward_vel(m, -10.f);
        omm_mario_set_action(m, ACT_FREEFALL, 0, Z_TRIG);
        gOmmMario->state.poleObject = m->usedObj;
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_climbing_pole(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY && (m->controller->buttonDown & B_BUTTON)) {
        m->controller->stickY *= 2.f;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_top_of_pole_transition(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        obj_anim_set_speed(m->marioObj, 2.f);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_top_of_pole(struct MarioState *m) {
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_TOP_OF_POLE_JUMP, 0, RETURN_CANCEL, m->vel[1] = 48.f * omm_player_physics_get_selected_jump(););
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_start_hanging(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        action_init(0.f, 0.f, 0, SOUND_MARIO_WHOA);
        action_z_pressed(1, ACT_GROUND_POUND, 0, RETURN_CANCEL);
        action_a_pressed(1, ACT_FREEFALL, 0, RETURN_CANCEL);
        action_b_pressed(1, ACT_FREEFALL, 0, RETURN_CANCEL);
        action_condition(!m->ceil || m->ceil->type != SURFACE_HANGABLE, ACT_FREEFALL, 0, RETURN_CANCEL);
        action_condition((m->input & INPUT_NONZERO_ANALOG) && m->actionTimer >= 15, ACT_HANG_MOVING, 0, RETURN_CANCEL);

        omm_act_hanging_update(m);
        ANM(MARIO_ANIM_HANG_ON_CEILING, 2.f);
        play_sound_if_no_flag(m, SOUND_ACTION_HANGING_STEP, MARIO_ACTION_SOUND_PLAYED);
        action_condition(obj_anim_is_at_end(m->marioObj), ACT_HANGING, 0, RETURN_BREAK);
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_hanging(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        action_z_pressed(1, ACT_GROUND_POUND, 0, RETURN_CANCEL);
        action_a_pressed(1, ACT_FREEFALL, 0, RETURN_CANCEL);
        action_b_pressed(1, ACT_FREEFALL, 0, RETURN_CANCEL);
        action_condition(!m->ceil || m->ceil->type != SURFACE_HANGABLE, ACT_FREEFALL, 0, RETURN_CANCEL);
        action_condition(m->input & INPUT_NONZERO_ANALOG, ACT_HANG_MOVING, m->actionArg, RETURN_CANCEL);

        omm_act_hanging_update(m);
        ANM((m->actionArg ? MARIO_ANIM_HANDSTAND_LEFT : MARIO_ANIM_HANDSTAND_RIGHT), 1.f);
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_hang_moving(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        action_z_pressed(1, ACT_GROUND_POUND, 0, RETURN_CANCEL);
        action_a_pressed(1, ACT_FREEFALL, 0, RETURN_CANCEL);
        action_b_pressed(1, ACT_FREEFALL, 0, RETURN_CANCEL);
        action_condition(!m->ceil || m->ceil->type != SURFACE_HANGABLE, ACT_FREEFALL, 0, RETURN_CANCEL);

        omm_mario_update_hanging_speed(m);
        mario_set_forward_vel(m, m->forwardVel);
        action_condition(perform_hang_step(m) == HANG_STEP_LEFT_CEIL, ACT_FREEFALL, 0, RETURN_BREAK);

        f32 animSpeed = relerp_0_1_f(m->forwardVel, 0.f, OMM_MARIO_HANG_MAX_SPEED, 1.f, 2.f);
        ANM((m->actionArg ? MARIO_ANIM_MOVE_ON_WIRE_NET_RIGHT : MARIO_ANIM_MOVE_ON_WIRE_NET_LEFT), animSpeed);
        if (obj_anim_is_past_frame(m->marioObj, 12)) {
            play_sound_if_no_flag(m, SOUND_ACTION_HANGING_STEP, MARIO_ACTION_SOUND_PLAYED);
        }
        if (obj_anim_is_near_end(m->marioObj)) {
            m->actionArg = !m->actionArg;
            m->flags &= ~MARIO_ACTION_SOUND_PLAYED;
            action_condition(!(m->input & INPUT_NONZERO_ANALOG), ACT_HANGING, m->actionArg, RETURN_BREAK);
        }
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_ledge_climb_slow(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY, ACT_LEDGE_CLIMB_FAST, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_in_cannon(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        switch (m->actionState) {

            // Entering cannon
            case 0: {
                m->marioObj->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
                m->usedObj->oInteractStatus = INT_STATUS_INTERACTED;
                m->statusForCamera->cameraEvent = CAM_EVENT_CANNON;
                m->statusForCamera->usedObj = m->usedObj;
                vec3f_zero(m->vel);
                m->pos[0] = m->usedObj->oPosX;
                m->pos[1] = m->usedObj->oPosY + 350;
                m->pos[2] = m->usedObj->oPosZ;
                m->forwardVel = 0;
                m->actionState = 1;
            } break;

            // Cannon cutscene
            case 1: {
                if (m->usedObj->oAction == 1) {
                    m->faceAngle[0] = m->usedObj->oMoveAnglePitch;
                    m->faceAngle[1] = m->usedObj->oMoveAngleYaw;
                    m->actionState = 2;
                }
            } break;

            // Aiming and fire
            case 2: {
                m->faceAngle[0] = clamp_s(m->faceAngle[0] + OMM_CAMERA_Y * ((m->controller->stickY * 10) - (BETTER_CAM_MOUSE_CAM * gOmmGlobals->mouseDeltaY * 16)), 0x0000, 0x3C00);
                m->faceAngle[1] =         m->faceAngle[1] - OMM_CAMERA_X * ((m->controller->stickX * 10) + (BETTER_CAM_MOUSE_CAM * gOmmGlobals->mouseDeltaX * 16));
                if (m->controller->buttonPressed & (A_BUTTON | B_BUTTON)) {
                    mario_set_forward_vel(m, 100 * coss(m->faceAngle[0]));
                    m->vel[1]  = 100 * sins(m->faceAngle[0]);
                    m->pos[0] += 120 * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
                    m->pos[1] += 120 * sins(m->faceAngle[0]);
                    m->pos[2] += 120 * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
                    m->marioObj->oNodeFlags |= GRAPH_RENDER_ACTIVE;
                    SFX(SOUND_ACTION_FLYING_FAST);
                    SFX(SOUND_OBJ_POUNDING_CANNON);
                    omm_mario_set_action(m, ACT_SHOT_FROM_CANNON, 0, A_BUTTON | B_BUTTON);
                    set_camera_mode(m->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
                    m->usedObj->oAction = 2;
                    return OMM_MARIO_ACTION_RESULT_BREAK;
                }
                if (m->controller->stickX || m->controller->stickY) {
                    SFX(SOUND_MOVING_AIM_CANNON);
                }
            } break;
        }
        vec3f_copy(m->marioObj->oGfxPos, m->pos);
        vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
        ANM(MARIO_ANIM_DIVE, 1);
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

/////////////
// Odyssey //
/////////////

static struct {
    struct Object *obj;
    s16 yawAngle;
    s16 yawVel;
} sOmmBowserTargetMine[1];

static s32 omm_act_grab_bowser(struct MarioState *m) {
    action_init(0, 0, 0, 0, obj_anim_play_with_sound(m->marioObj, MARIO_ANIM_GRAB_BOWSER, 1.f, SOUND_MARIO_HRMM, true););
    if (obj_anim_is_at_end(m->marioObj)) {
        omm_mario_set_action(m, ACT_OMM_HOLD_BOWSER, 0, 0);
        m->angleVel[1] = 0;
    }
    stationary_ground_step(m);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_hold_bowser(struct MarioState *m) {
    ANM(m->angleVel[1] == 0 ? MARIO_ANIM_HOLDING_BOWSER : MARIO_ANIM_SWINGING_BOWSER, 1.f);

    // Prepare release
    if (!m->actionState && (m->controller->buttonPressed & B_BUTTON) && (abs_s(m->angleVel[1]) >= 0x0E00)) {

        // Target the nearest mine to throw Bowser into
        // Takes the current rotation into account
        struct Object *targetMine = NULL;
        s16 targetAngle = 0;
        u16 minAngle = 0xFFFF;
        for_each_object_with_behavior(mine, bhvOmmBowserMine) {
            s16 mineAngle = atan2s(mine->oPosZ, mine->oPosX);
            for (s32 deltaAngle = 0; abs_s(deltaAngle) < 0x10000; deltaAngle += m->angleVel[1]) {
                s16 nextAngle = m->faceAngle[1] + (s16) deltaAngle;
                u16 angleDiff = (u16) abs_s(nextAngle - mineAngle);
                if (angleDiff < (u16) abs_s(m->angleVel[1])) {
                    if ((u16) abs_s(deltaAngle) < minAngle) {
                        minAngle = (u16) abs_s(deltaAngle);
                        targetAngle = mineAngle;
                        targetMine = mine;
                    }
                    break;
                }
            }
        }

        sOmmBowserTargetMine->obj = targetMine;
        sOmmBowserTargetMine->yawAngle = targetAngle;
        sOmmBowserTargetMine->yawVel = m->angleVel[1];
        m->actionState = 1;
    }

    // About to release
    if (m->actionState) {
        u16 angleDiff = (u16) abs_s(m->faceAngle[1] - sOmmBowserTargetMine->yawAngle);
        if (angleDiff < (u16) abs_s((s16) sOmmBowserTargetMine->yawVel)) {

            // Set Bowser's starting point and direction
            m->faceAngle[1] = sOmmBowserTargetMine->yawAngle;
            struct Object *bowser = obj_get_first_with_behavior(bhvOmmBowser);
            f32 distBowserFromCenter = sqrtf(sqr_f(bowser->oPosX) + sqr_f(bowser->oPosZ));
            bowser->oPosX = distBowserFromCenter * sins(m->faceAngle[1]);
            bowser->oPosZ = distBowserFromCenter * coss(m->faceAngle[1]);
            bowser->oFaceAngleYaw = m->faceAngle[1];
            bowser->oBowserMine = sOmmBowserTargetMine->obj;

            // Release
            obj_anim_play_with_sound(m->marioObj, MARIO_ANIM_RELEASE_BOWSER, 1.f, SOUND_MARIO_SO_LONGA_BOWSER | 0xFF00, true);
            omm_mario_set_action(m, ACT_OMM_RELEASE_BOWSER, 0, B_BUTTON);
            return OMM_MARIO_ACTION_RESULT_CANCEL;
        }
        m->angleVel[1] = sOmmBowserTargetMine->yawVel;

    } else {

        // Spin accel
        if (m->intendedMag > 20.f) {
            if (m->actionArg == 0) {
                m->actionArg = 1;
                m->twirlYaw = m->intendedYaw;
            } else {
                s16 angleAccel = clamp_s((s16) (m->intendedYaw - m->twirlYaw) / 100, -200, +200);
                m->twirlYaw = m->intendedYaw;
                m->angleVel[1] = clamp_s(m->angleVel[1] + angleAccel, -0x1000, +0x1000);
            }
        } else {
            m->actionArg = 0;
            m->angleVel[1] = approach_s32(m->angleVel[1], 0, 0x40, 0x40);
        }
    }

    // Update face angle
    s16 prevAngle = m->faceAngle[1];
    m->faceAngle[1] += m->angleVel[1];
    if ((m->angleVel[1] < 0 && prevAngle < m->faceAngle[1]) ||
        (m->angleVel[1] > 0 && prevAngle > m->faceAngle[1])) {
        SFX(SOUND_OBJ_BOWSER_SPINNING);
    }
    stationary_ground_step(m);

    // Update Bowser's pos and angle
    struct Object *bowser = obj_get_first_with_behavior(bhvOmmBowser);
    f32 distBowserFromCenter = sqrtf(sqr_f(bowser->oPosX) + sqr_f(bowser->oPosZ));
    bowser->oPosX = distBowserFromCenter * sins(m->faceAngle[1]);
    bowser->oPosZ = distBowserFromCenter * coss(m->faceAngle[1]);
    bowser->oFaceAngleYaw = m->faceAngle[1];
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_release_bowser(struct MarioState *m) {
    if (obj_anim_is_at_end(m->marioObj)) {
        omm_mario_set_action(m, ACT_IDLE, 0, 0);
    }
    stationary_ground_step(m);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

///////////////
// Automatic //
///////////////

static bool omm_check_common_automatic_cancels(struct MarioState *m) {

    // Plunge
    if (m->pos[1] < m->waterLevel - 100) {
        action_condition(m->action == ACT_GRABBED, ACT_OMM_GRABBED_WATER, 0, return true);
        set_water_plunge_action(m);
        return true;
    }

    return false;
}

s32 omm_mario_execute_automatic_action(struct MarioState *m) {
    gOmmMario->wallSlide.jumped = false;
    gOmmMario->peach.floated = false;
    gOmmMario->cappy.bounced = false;
    gOmmMario->state.airCombo = 0;
    gOmmMario->midairSpin.counter = 0;
    m->quicksandDepth = 0;

    // Cancels
    if (omm_check_common_automatic_cancels(m)) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Actions
    switch (m->action) {

        // SM64
        case ACT_GRAB_POLE_SLOW:            return omm_act_grab_pole(m);
        case ACT_GRAB_POLE_FAST:            return omm_act_grab_pole(m);
        case ACT_HOLDING_POLE:              return omm_act_holding_pole(m);
        case ACT_CLIMBING_POLE:             return omm_act_climbing_pole(m);
        case ACT_TOP_OF_POLE_TRANSITION:    return omm_act_top_of_pole_transition(m);
        case ACT_TOP_OF_POLE:               return omm_act_top_of_pole(m);
        case ACT_START_HANGING:             return omm_act_start_hanging(m);
        case ACT_HANGING:                   return omm_act_hanging(m);
        case ACT_HANG_MOVING:               return omm_act_hang_moving(m);
        case ACT_LEDGE_GRAB:                return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_LEDGE_CLIMB_SLOW_1:        return omm_act_ledge_climb_slow(m);
        case ACT_LEDGE_CLIMB_SLOW_2:        return omm_act_ledge_climb_slow(m);
        case ACT_LEDGE_CLIMB_DOWN:          return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_LEDGE_CLIMB_FAST:          return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_GRABBED:                   return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_IN_CANNON:                 return omm_act_in_cannon(m);
        case ACT_TORNADO_TWIRLING:          return OMM_MARIO_ACTION_RESULT_CONTINUE;

        // Odyssey
        case ACT_OMM_GRAB_BOWSER:           return omm_act_grab_bowser(m);
        case ACT_OMM_HOLD_BOWSER:           return omm_act_hold_bowser(m);
        case ACT_OMM_RELEASE_BOWSER:        return omm_act_release_bowser(m);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

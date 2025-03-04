#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static s32 omm_mario_get_water_handling(struct MarioState *m, f32 velMin, f32 velMax, f32 handlingMin, f32 handlingMax) {
    if (OMM_CHEAT_SUPER_RESPONSIVE) return 0x4000;
    return (s32) (handlingMin + sqr_f(1.f - invlerp_0_1_f(abs_f(m->forwardVel), velMin, velMax)) * (handlingMax - handlingMin));
}

static void omm_act_submerged_update_swimming_pitch(struct MarioState *m, bool hold) {
    s32 handling = omm_mario_get_water_handling(m, 8.f, OMM_MARIO_SWIMMING_MAX_SPEED(hold), 0x200, 0x400);
    m->faceAngle[0] -= (abs_f(m->controller->stickY) > 8 ? handling * m->controller->stickY / 64.f : (handling / 4) * sign_0_s(m->faceAngle[0]));
    m->faceAngle[0] = (abs_s(m->faceAngle[0]) > handling / 8) * clamp_s(m->faceAngle[0], -0x3F00, 0x3F00);
}

static void omm_act_submerged_update_swimming_yaw_and_roll(struct MarioState *m, bool hold, f32 maxRollMult) {
    s32 handling = omm_mario_get_water_handling(m, 8.f, OMM_MARIO_SWIMMING_MAX_SPEED(hold), 0x200, 0x400);
    m->faceAngle[1] -= (abs_f(m->controller->stickX) > 8 ? handling * m->controller->stickX / 64.f : 0);
    m->faceAngle[2] = approach_s32(m->faceAngle[2], 0x1000 * maxRollMult * m->controller->stickX / 64.f, 0x200, 0x200);
}

static void omm_act_submerged_update_swimming_speed(struct MarioState *m, bool hold) {
    f32 buoyancy = max_f(0, get_buoyancy(m));
    m->forwardVel = clamp_f(m->forwardVel - 4 * ((m->action & ACT_FLAG_STATIONARY) != 0), 0.f, OMM_MARIO_SWIMMING_MAX_SPEED(hold));
    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]) + buoyancy;
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
    vec3s_zero(m->angleVel);
}

static void omm_act_submerged_update_water_pitch(struct MarioState *m) {
    struct Object *marioObj = m->marioObj;
    if (marioObj->oGfxAngle[0] > 0) {
        marioObj->oGfxPos[1] += 60.f * sins(marioObj->oGfxAngle[0]) * sins(marioObj->oGfxAngle[0]);
        marioObj->oGfxAngle[0] *= 1.25f;
    } else if (marioObj->oGfxAngle[0] < 0) {
        marioObj->oGfxAngle[0] *= 0.6f;
    }
}

static void omm_act_submerged_set_swimming_at_surface_particles(struct MarioState *m, u32 particleFlag) {
    static bool sWasAtSurface = false;
    bool isAtSurface = (m->pos[1] >= m->waterLevel - OMM_MARIO_WATER_PARTICLES_SURFACE_OFFSET);
    if (isAtSurface) {
        PFX(particleFlag);
        if (isAtSurface != sWasAtSurface) {
            SFX(SOUND_ACTION_UNKNOWN431);
        }
    }
    sWasAtSurface = isAtSurface;
}

static void omm_act_submerged_idle_step(struct MarioState *m, bool hold, s32 animID, f32 animAccel) {
    omm_act_submerged_update_swimming_pitch(m, hold);
    omm_act_submerged_update_swimming_yaw_and_roll(m, hold, 1.f);
    omm_act_submerged_update_swimming_speed(m, hold);
    perform_water_step(m);
    omm_act_submerged_update_water_pitch(m);
    m->marioBodyState->headAngle[0] = (
        m->faceAngle[0] > 0 ?
        approach_s32(m->marioBodyState->headAngle[0], m->faceAngle[0] / 2, 0x80, 0x200) :
        approach_s32(m->marioBodyState->headAngle[0], 0, 0x200, 0x200)
    );
    ANM(animID, animAccel);
    omm_act_submerged_set_swimming_at_surface_particles(m, PARTICLE_IDLE_WATER_WAVE);
}

static u32 omm_act_submerged_perform_water_step_and_correct_pitch_and_yaw(struct MarioState *m) {
    f32 speedFactor     = vec3f_length(m->vel) / OMM_MARIO_WATER_SHELL_MAX_SPEED;
    s32 pitchCorrection = (s32) (0x200 * speedFactor * clamp_0_1_f(1.f - (abs_f(m->controller->stickY) / 64.f)));
    s32 yawCorrection   = (s32) (0x200 * speedFactor * clamp_0_1_f(1.f - (abs_f(m->controller->stickX) / 64.f)));
    u32 step = perform_water_step(m);
    switch (step) {
        case WATER_STEP_HIT_FLOOR:   m->faceAngle[0] += pitchCorrection; break;
        case WATER_STEP_HIT_CEILING: m->faceAngle[0] -= pitchCorrection; break;
        case WATER_STEP_HIT_WALL:    if (m->wall && abs_s(m->faceAngle[0]) < 0x1000) {
            s16 angleDiff = atan2s(m->wall->normal.z, m->wall->normal.x) - m->faceAngle[1];
            m->faceAngle[1] += (
                (angleDiff > +0x4000 && angleDiff < +0x7000 ? min_s(yawCorrection, abs_s(angleDiff - 0x4000)) : 0) -
                (angleDiff < -0x4000 && angleDiff > -0x7000 ? min_s(yawCorrection, abs_s(angleDiff + 0x4000)) : 0)
            );
        } break;
    }
    return step;
}

static void omm_act_submerged_common_swimming_step(struct MarioState *m, bool hold) {
    omm_act_submerged_update_swimming_pitch(m, hold);
    omm_act_submerged_update_swimming_yaw_and_roll(m, hold, 1.25f);
    omm_act_submerged_update_swimming_speed(m, hold);
    omm_act_submerged_perform_water_step_and_correct_pitch_and_yaw(m);
    omm_act_submerged_update_water_pitch(m);
    m->marioBodyState->headAngle[0] = approach_s32(m->marioBodyState->headAngle[0], 0, 0x200, 0x200);
    omm_act_submerged_set_swimming_at_surface_particles(m, PARTICLE_WAVE_TRAIL);
}

static void omm_act_submerged_stationary_slow_down(struct MarioState *m, f32 slowModifier) {
    m->angleVel[0] = 0;
    m->angleVel[1] = 0;
    m->faceAngle[0] = approach_s32(m->faceAngle[0], 0, 0x200, 0x200);
    m->faceAngle[2] = approach_s32(m->faceAngle[2], 0, 0x100, 0x100);
    m->forwardVel = approach_f32(m->forwardVel, 0.f, slowModifier, slowModifier);
    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
    m->vel[1] = approach_f32(m->vel[1], 0.f, 2.f * slowModifier, slowModifier);
    omm_act_submerged_set_swimming_at_surface_particles(m, PARTICLE_IDLE_WATER_WAVE);
}

static bool omm_act_submerged_check_water_jump(struct MarioState *m, u32 action, bool checkInputs) {
    static s32 sFramesAtSurface = 0;
    if (m->pos[1] >= m->waterLevel - OMM_MARIO_WATER_JUMP_SURFACE_OFFSET) {
        if (!checkInputs || ((m->controller->buttonDown & A_BUTTON) && (m->controller->stickY < -60))) {
            bool superJump = (
                checkInputs &&
                OMM_MOVESET_ODYSSEY &&
                (action == ACT_WATER_JUMP) &&
                (m->forwardVel >= OMM_MARIO_WATER_SUPER_JUMP_SPEED_THRESHOLD) &&
                (sFramesAtSurface < OMM_MARIO_WATER_SUPER_JUMP_FRAMES) &&
                (m->framesSinceA < OMM_MARIO_WATER_SUPER_JUMP_FRAMES)
            );

            // Compute now the water jump velocity
            // The more horizontal Mario swims, the further
            // The more vertical Mario swims, the higher
            f32 pitchFactor = invlerp_0_1_s(m->faceAngle[0], 0x2000, 0x3F00);
            f32 forwardVel = m->forwardVel * (1.f - pitchFactor) * 0.9f;
            f32 upwardsVel = (superJump ? 64.f : 48.f) * lerp_f(sqr_f(pitchFactor), 1.f, 1.f + m->forwardVel / 300.f);
            omm_mario_set_action(m, action, superJump ? 2 : 0, 0);
            mario_set_forward_vel(m, forwardVel);
            m->vel[1] = upwardsVel;
            PFX(PARTICLE_WATER_SPLASH);
            vec3s_zero(m->angleVel);
            sFramesAtSurface = 0;
            return true;
        }
        sFramesAtSurface++;
    } else {
        sFramesAtSurface = 0;
    }
    return false;
}

//////////
// SM64 //
//////////

static s32 omm_act_submerged_cancels__cappy_dash_pound(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_water_idle(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Cancels
    action_condition(omm_act_submerged_check_water_jump(m, ACT_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
    action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_WATER_PUNCH, 0, RETURN_CANCEL);
    action_a_pressed(1, ACT_BREASTSTROKE, 0, RETURN_CANCEL);

    // Step
    if (m->actionArg) {
        omm_act_submerged_idle_step(m, false, MARIO_ANIM_WATER_ACTION_END, 1.f);
        m->actionArg *= !obj_anim_is_at_end(m->marioObj);
    } else {
        omm_act_submerged_idle_step(m, false, MARIO_ANIM_WATER_IDLE, m->faceAngle[0] < -0x1000 ? 3.f : 1.f);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_water_action_end(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY, ACT_WATER_IDLE, 1, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_swimming_end(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Cancels
    action_condition(omm_act_submerged_check_water_jump(m, ACT_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
    action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_WATER_PUNCH, 0, RETURN_CANCEL);
    action_condition(m->actionTimer++ >= 15, ACT_WATER_ACTION_END, 0, RETURN_CANCEL);
    action_condition((m->controller->buttonDown & A_BUTTON) && m->actionTimer > 7, ACT_BREASTSTROKE, 0, RETURN_CANCEL);

    // Step
    m->forwardVel -= 0.25f;
    ANM(MARIO_ANIM_SWIM_PART2, 1.f);
    omm_act_submerged_common_swimming_step(m, false);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_breaststroke(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Cancels
    action_condition(omm_act_submerged_check_water_jump(m, ACT_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
    action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_WATER_PUNCH, 0, RETURN_CANCEL);
    action_condition(++m->actionTimer >= 14, ACT_FLUTTER_KICK, 0, RETURN_CANCEL);

    // Step
    m->forwardVel += 1.5f * (m->actionTimer < 6) + 2.5f * (m->actionTimer >= 9);
    m->actionState += (m->actionTimer >= 2 && m->actionTimer < 6 && (m->controller->buttonPressed & A_BUTTON));
    if (m->actionState && m->actionTimer == 9) {
        obj_anim_set_frame(m->marioObj, 0);
        m->actionState = 0;
        m->actionTimer = 1;
    }
    ANM(MARIO_ANIM_SWIM_PART1, 1.f);
    omm_act_submerged_common_swimming_step(m, false);
    if (m->actionTimer == 1) {
        SFX(SOUND_ACTION_SWIM_FAST);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_flutter_kick(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Cancels
    action_condition(omm_act_submerged_check_water_jump(m, ACT_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
    action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_WATER_PUNCH, 0, RETURN_CANCEL);
    action_condition(!(m->controller->buttonDown & A_BUTTON), ACT_SWIMMING_END, 0, RETURN_CANCEL);

    // Step
    m->forwardVel = approach_f32(m->forwardVel, OMM_MARIO_FLUTTER_KICK_MAX_SPEED(false), 0.4f, 0.2f);
    omm_act_submerged_common_swimming_step(m, false);
    if (m->marioObj->oAnimID == MARIO_ANIM_FLUTTERKICK) {
        s32 frame = m->marioObj->oAnimFrame;
        if (frame == 0 || frame == 12) SFX(SOUND_ACTION_UNKNOWN434);
    } else if (obj_anim_is_near_end(m->marioObj)) {
        ANM(MARIO_ANIM_FLUTTERKICK, 1.f);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_hold_water_idle(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Cancels
    action_condition(omm_act_submerged_check_water_jump(m, ACT_HOLD_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_HOLD_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_HOLD_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_WATER_IDLE, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_b_pressed(1, ACT_WATER_THROW, 0, RETURN_CANCEL);
    action_a_pressed(1, ACT_HOLD_BREASTSTROKE, 0, RETURN_CANCEL);

    // Step
    if (m->actionArg) {
        omm_act_submerged_idle_step(m, true, m->actionArg == 2 ? MARIO_ANIM_STOP_GRAB_OBJ_WATER : MARIO_ANIM_WATER_ACTION_END_WITH_OBJ, 1.f);
        m->actionArg *= !obj_anim_is_at_end(m->marioObj);
    } else {
        omm_act_submerged_idle_step(m, true, MARIO_ANIM_WATER_IDLE_WITH_OBJ, m->faceAngle[0] < -0x1000 ? 3.f : 1.f);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_hold_water_action_end(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY, ACT_HOLD_WATER_IDLE, 1, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_hold_swimming_end(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Cancels
    action_condition(omm_act_submerged_check_water_jump(m, ACT_HOLD_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_HOLD_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_HOLD_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_WATER_IDLE, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_b_pressed(1, ACT_WATER_THROW, 0, RETURN_CANCEL);
    action_condition(m->actionTimer++ >= 15, ACT_HOLD_WATER_ACTION_END, 0, RETURN_CANCEL);
    action_condition((m->controller->buttonDown & A_BUTTON) && m->actionTimer > 7, ACT_HOLD_BREASTSTROKE, 0, RETURN_CANCEL);

    // Step
    m->forwardVel -= 0.25f;
    ANM(MARIO_ANIM_SWIM_WITH_OBJ_PART2, 1.f);
    omm_act_submerged_common_swimming_step(m, true);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_hold_breaststroke(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Cancels
    action_condition(omm_act_submerged_check_water_jump(m, ACT_HOLD_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_HOLD_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_HOLD_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_WATER_IDLE, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_b_pressed(1, ACT_WATER_THROW, 0, RETURN_CANCEL);
    action_condition(++m->actionTimer >= 14, ACT_HOLD_FLUTTER_KICK, 0, RETURN_CANCEL);

    // Step
    m->forwardVel += 1.f * (m->actionTimer < 6) + 2.f * (m->actionTimer >= 9);
    m->actionState += (m->actionTimer >= 2 && m->actionTimer < 6 && (m->controller->buttonPressed & A_BUTTON));
    if (m->actionState && m->actionTimer == 9) {
        obj_anim_set_frame(m->marioObj, 0);
        m->actionState = 0;
        m->actionTimer = 1;
    }
    ANM(MARIO_ANIM_SWIM_WITH_OBJ_PART1, 1.f);
    omm_act_submerged_common_swimming_step(m, true);
    if (m->actionTimer == 1) {
        SFX(SOUND_ACTION_SWIM);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_hold_flutter_kick(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Cancels
    action_condition(omm_act_submerged_check_water_jump(m, ACT_HOLD_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_HOLD_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_HOLD_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_WATER_IDLE, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_b_pressed(1, ACT_WATER_THROW, 0, RETURN_CANCEL);
    action_condition(!(m->controller->buttonDown & A_BUTTON), ACT_HOLD_SWIMMING_END, 0, RETURN_CANCEL);

    // Step
    m->forwardVel = approach_f32(m->forwardVel, OMM_MARIO_FLUTTER_KICK_MAX_SPEED(true), 0.4f, 0.2f);
    omm_act_submerged_common_swimming_step(m, true);
    if (m->marioObj->oAnimID == MARIO_ANIM_FLUTTERKICK_WITH_OBJ) {
        s32 frame = m->marioObj->oAnimFrame;
        if (frame == 0 || frame == 12) SFX(SOUND_ACTION_UNKNOWN434);
    } else if (obj_anim_is_near_end(m->marioObj)) {
        ANM(MARIO_ANIM_FLUTTERKICK_WITH_OBJ, 1.f);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_water_shell_swimming(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Cancels
    action_init(0, 0, 0, 0, audio_play_shell_music(););
    action_condition(omm_act_submerged_check_water_jump(m, ACT_HOLD_WATER_JUMP, true), 0, 0, RETURN_CANCEL, audio_stop_shell_music(););
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_WATER_IDLE, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_b_pressed(1, ACT_WATER_THROW, 0, RETURN_CANCEL);

    // Pitch
    s16 targetPitch = (s16) (-250.f * m->controller->stickY);
    m->faceAngle[0] = targetPitch - approach_s32((s16) (targetPitch - m->faceAngle[0]), 0, 0x200, 0x200);

    // Yaw
    s16 yawVel = (s16) (0x300 * abs_f(m->controller->stickX / 64.f));
    m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, yawVel, yawVel);

    // Roll
    s16 targetRoll = (s16) (0x1800 * m->controller->stickX / 64.f);
    m->faceAngle[2] = approach_s32(m->faceAngle[2], targetRoll, 0x400, 0x400);

    // Step
    m->forwardVel = approach_f32(m->forwardVel, OMM_MARIO_WATER_SHELL_MAX_SPEED, 4, 1);
    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
    omm_act_submerged_perform_water_step_and_correct_pitch_and_yaw(m);

    // Animation and sound
    PFX((m->pos[1] >= m->waterLevel - OMM_MARIO_WATER_PARTICLES_SURFACE_OFFSET) ? 0 : (PARTICLE_BUBBLE | (PARTICLE_PLUNGE_BUBBLE * ((gGlobalTimer % 3) == 0))));
    m->marioBodyState->headAngle[0] = approach_s32(m->marioBodyState->headAngle[0], 0, 0x200, 0x200);
    ANM(MARIO_ANIM_FLUTTERKICK_WITH_OBJ, 2);
    omm_act_submerged_set_swimming_at_surface_particles(m, PARTICLE_WAVE_TRAIL);
    s32 frame = m->marioObj->oAnimFrame;
    if (frame == 0 || frame == 12) {
        SFX(SOUND_ACTION_UNKNOWN434);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_water_throw(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    action_init(m->forwardVel / 2.f, 0, 0, SOUND_ACTION_SWIM);

    // Cancels
    if (!m->heldObj) {
        action_condition(omm_act_submerged_check_water_jump(m, ACT_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
        action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
        action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
        action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
        action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
        action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
        action_b_pressed(1, ACT_WATER_PUNCH, 0, RETURN_CANCEL);
        action_a_pressed(1, ACT_BREASTSTROKE, 0, RETURN_CANCEL);
    }

    // Step
    m->forwardVel = approach_f32(m->forwardVel, 0, 0, 0.5f);
    omm_act_submerged_idle_step(m, m->heldObj != NULL, MARIO_ANIM_WATER_THROW_OBJ, 2.f);
    if (m->heldObj && m->actionTimer++ >= 3) {
        mario_throw_held_object(m);
    }
    action_condition(!m->heldObj && obj_anim_is_at_end(m->marioObj), ACT_WATER_IDLE, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_water_punch(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    if (m->actionTimer++ == 0) {
        SFX(SOUND_ACTION_SWIM);
        mario_set_forward_vel(m, 24.f * (m->actionArg == 0));
    }

    // Cancels
    if (!m->heldObj) {
        action_condition(omm_act_submerged_check_water_jump(m, ACT_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
        action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
        action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
        action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
        action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
        action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
        action_a_pressed(1, ACT_BREASTSTROKE, 0, RETURN_CANCEL);
    }

    // Step
    m->forwardVel = approach_f32(m->forwardVel, 0, 0, 1);
    switch (m->actionState) {
        case 0: {
            omm_act_submerged_idle_step(m, false, MARIO_ANIM_WATER_GRAB_OBJ_PART1, 2.f);
            if (obj_anim_is_at_end(m->marioObj)) {
                m->actionState = 1;
                if (m->marioObj->collidedObjInteractTypes & INTERACT_GRABBABLE) {
                    struct Object *obj = mario_get_collided_object(m, INTERACT_GRABBABLE);
                    f32 dx = obj->oPosX - m->pos[0];
                    f32 dz = obj->oPosZ - m->pos[2];
                    s16 dAngleToObject = atan2s(dz, dx) - m->faceAngle[1];
                    if (abs_s(dAngleToObject) < 0x2AAA) {
                        m->usedObj = obj;
                        mario_grab_used_object(m);
                        m->marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ;
                        m->actionState = 2;
                    }
                }
            }
        } break;

        case 1: {
            omm_act_submerged_idle_step(m, false, MARIO_ANIM_WATER_GRAB_OBJ_PART2, 2.f);
            action_condition(obj_anim_is_at_end(m->marioObj), ACT_WATER_ACTION_END, 0, RETURN_BREAK);
        } break;

        case 2: {
            omm_act_submerged_idle_step(m, true, MARIO_ANIM_WATER_PICK_UP_OBJ, 2.f);
            if (obj_anim_is_at_end(m->marioObj)) {
                action_condition(m->heldObj->behavior == bhvKoopaShellUnderwater, ACT_WATER_SHELL_SWIMMING, 0, RETURN_BREAK);
                action_condition(1, ACT_HOLD_WATER_IDLE, 2, RETURN_BREAK);
            }
        } break;
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_water_plunge(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY && (m->controller->buttonDown & Z_TRIG) && (m->prevAction == ACT_GROUND_POUND || m->prevAction == ACT_OMM_SPIN_POUND)) {
        if (!omm_mario_has_metal_cap(m)) {
            omm_mario_set_action(m, ACT_OMM_WATER_GROUND_POUND, 0, 0);
        } else if (m->prevAction == ACT_OMM_SPIN_POUND) {
            omm_mario_set_action(m, ACT_OMM_METAL_WATER_SPIN_POUND, 0, 0);
        } else /*if (m->prevAction == ACT_GROUND_POUND)*/ {
            omm_mario_set_action(m, ACT_OMM_METAL_WATER_GROUND_POUND, 0, 0);
        }
        omm_mario_init_next_action(m);
        m->actionState = 1;
        SFX(SOUND_ACTION_UNKNOWN430);
        PFX(PARTICLE_WATER_SPLASH);
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }
    if (omm_act_submerged_cancels__cappy_dash_pound(m) == OMM_MARIO_ACTION_RESULT_CANCEL) {
        SFX(SOUND_ACTION_UNKNOWN430);
        PFX(PARTICLE_WATER_SPLASH);
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_knockback_water(struct MarioState *m, s32 animID) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // Step
    omm_act_submerged_stationary_slow_down(m, 1.5f);
    perform_water_step(m);

    // Animation
    ANM(animID, 1.5f);
    m->marioBodyState->headAngle[0] = 0;
    if (obj_anim_is_at_end(m->marioObj)) {
        m->invincTimer = 30 * (m->actionArg > 0);
        omm_mario_set_action(m, omm_mario_is_dead(m) ? ACT_WATER_DEATH : ACT_WATER_IDLE, 0, 0);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

/////////////
// Odyssey //
/////////////

static s32 omm_act_water_first_person(struct MarioState *m) {
    action_init_no_y_vel(m->forwardVel, 0, NO_SOUND, music_lower_volume(SEQ_PLAYER_LEVEL, 60, 40));

    m->faceAngle[2] = 0;
    s32 animID = obj_anim_is_at_end(m->marioObj) ? MARIO_ANIM_FIRST_PERSON : m->marioObj->oAnimID;
    omm_act_submerged_update_swimming_speed(m, false);
    perform_water_step(m);
    ANM(animID, 1.f);
    omm_act_submerged_set_swimming_at_surface_particles(m, PARTICLE_IDLE_WATER_WAVE);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_water_dash(struct MarioState *m) {

    // Cancels
    action_condition(OMM_MOVESET_CLASSIC, ACT_WATER_IDLE, 0, RETURN_CANCEL);
    action_condition(omm_act_submerged_check_water_jump(m, ACT_WATER_JUMP, true), 0, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_condition(!(m->controller->buttonDown & B_BUTTON), ACT_WATER_ACTION_END, 0, RETURN_CANCEL);

    // Speed
    m->forwardVel = OMM_MARIO_WATER_DASH_MAX_SPEED;
    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);

    // Pitch
    s16 targetPitch = (s16) (-250.f * m->controller->stickY);
    m->faceAngle[0] = targetPitch - approach_s32((s16) (targetPitch - m->faceAngle[0]), 0, 0x200, 0x200);

    // Yaw
    s16 yawVel = (s16) (0x200 * abs_f(m->controller->stickX / 64.f));
    m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, yawVel, yawVel);

    // Roll
    s16 targetRoll = (s16) (0x1800 * m->controller->stickX / 64.f);
    m->faceAngle[2] = approach_s32(m->faceAngle[2], targetRoll, 0x400, 0x400);

    // Step
    u32 step = omm_act_submerged_perform_water_step_and_correct_pitch_and_yaw(m);
    if (step == WATER_STEP_HIT_WALL && m->wall) {
        Vec3f v; vec3f_copy(v, m->vel); vec3f_normalize(v);
        f32 dot = vec3f_dot(v, (f32 *) &m->wall->normal);
        if (dot < -0.85f) {
            m->vel[1] /= 1.5f;
            mario_set_forward_vel(m, m->forwardVel / 1.5f);
            SFX(SOUND_MARIO_OOOF2);
            PFX(PARTICLE_VERTICAL_STAR);
            action_condition(1, ACT_BACKWARD_WATER_KB, 0, RETURN_BREAK);
        }
    }

    // Animation
    PFX(PARTICLE_PLUNGE_BUBBLE | PARTICLE_BUBBLE);
    m->marioBodyState->headAngle[0] = approach_s32(m->marioBodyState->headAngle[0], 0, 0x200, 0x200);
    ANM(MARIO_ANIM_FLUTTERKICK, 3.f);
    omm_act_submerged_set_swimming_at_surface_particles(m, PARTICLE_WAVE_TRAIL);
    if ((m->actionTimer++ % 4) == 0) {
        SFX(SOUND_ACTION_UNKNOWN434);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_water_ground_pound(struct MarioState *m) {

    // Cancels
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);

    // States
    if (m->actionState == 0) {
        vec3f_set(m->vel, 0, 0, 0);
        ANM(MARIO_ANIM_START_GROUND_POUND, 1.25f);
        if (m->actionTimer == 0) {
            SFX(SOUND_ACTION_SPIN);
        }

        if (m->actionTimer < 10) {
            m->pos[1] += (20 - 2 * m->actionTimer);
            vec3f_copy(m->marioObj->oGfxPos, m->pos);
        } else if (m->actionTimer >= m->marioObj->oCurrAnim->mLoopEnd + 1) {
            SFX(SOUND_MARIO_GROUND_POUND_WAH);
            SFX(SOUND_ACTION_UNKNOWN430);
            vec3s_set(m->faceAngle, 0, m->faceAngle[1], 0);
            vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
            m->vel[1] = -105.f;
            m->actionState = 1;
        }
        m->actionTimer++;
    } else {
        if (m->actionState == 1 || (m->controller->buttonDown & Z_TRIG)) {
            m->vel[1] = min_f(m->vel[1], -80.f);
            m->actionState = 2;
        }
        m->vel[1] += 5.f;
        ANM(MARIO_ANIM_GROUND_POUND, 1.f);
        PFX(PARTICLE_PLUNGE_BUBBLE | PARTICLE_BUBBLE);

        s32 step = perform_water_step(m);
        action_condition(step == WATER_STEP_HIT_FLOOR, ACT_OMM_WATER_GROUND_POUND_LAND, 0, RETURN_BREAK, PFX(PARTICLE_MIST_CIRCLE | PARTICLE_HORIZONTAL_STAR););
        action_condition(m->vel[1] >= 0, ACT_WATER_IDLE, 0, RETURN_BREAK, m->vel[1] = 0;);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_water_ground_pound_land(struct MarioState *m) {

    // Cancels
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL_LAND, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALL_LAND, 0, RETURN_CANCEL);

    // Step
    stationary_ground_step(m);
    if (m->actionState == 0) {
        action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_GROUND_POUND_JUMP, 0, RETURN_CANCEL);
        ANM(MARIO_ANIM_GROUND_POUND_LANDING, 0.75f);
        if (obj_anim_is_at_end(m->marioObj)) m->actionState = 1;
    } else {
        ANM(MARIO_ANIM_STOP_SLIDE, 1.f);
        action_condition(obj_anim_is_at_end(m->marioObj), ACT_WATER_IDLE, 0, RETURN_BREAK);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_water_ground_pound_jump(struct MarioState *m) {
    action_init(0, 56.f, 0, SOUND_ACTION_UNKNOWN430, gOmmMario->spin.yaw = 1;);

    // Cancels
    action_condition(OMM_MOVESET_CLASSIC, ACT_WATER_IDLE, 0, RETURN_CANCEL);
    action_condition(omm_act_submerged_check_water_jump(m, ACT_WATER_JUMP, false), 0, 0, RETURN_CANCEL);
    action_condition(m->vel[1] <= 0.f, ACT_WATER_IDLE, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
    action_zb_pressed(1, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_WATER_PUNCH, 0, RETURN_CANCEL);
    action_a_pressed(1, ACT_BREASTSTROKE, 0, RETURN_CANCEL);

    // Step
    omm_act_submerged_stationary_slow_down(m, 2.f);
    perform_water_step(m);

    // Animation
    ANM(MARIO_ANIM_DOUBLE_JUMP_RISE, 1.f);
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += (0x80 * m->vel[1]) * (prevSpinYaw != 0) * (omm_mario_has_vanish_cap(m) ? 0.8f : 1.f) / sqr_f(omm_player_physics_get_selected_jump());
    gOmmMario->spin.yaw *= ((u16) prevSpinYaw < (u16) gOmmMario->spin.yaw) * (m->vel[1] > 0.f);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] + gOmmMario->spin.yaw;
    PFX(PARTICLE_PLUNGE_BUBBLE | PARTICLE_BUBBLE);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_leave_object_water(struct MarioState *m) {

    // Cancels
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_WATER_PUNCH, 0, RETURN_CANCEL);
    action_a_pressed(1, ACT_BREASTSTROKE, 0, RETURN_CANCEL);

    // Step
    omm_act_submerged_stationary_slow_down(m, 1.f);
    perform_water_step(m);
    action_condition(omm_act_submerged_check_water_jump(m, ACT_WATER_JUMP, false), 0, 0, RETURN_CANCEL);
    action_condition(abs_f(m->vel[1]) < 1.f, ACT_WATER_IDLE, 0, RETURN_BREAK);

    // Animation
    ANM(MARIO_ANIM_WATER_IDLE, 1.f);
    m->marioBodyState->headAngle[0] = 0;
    PFX(PARTICLE_SPARKLES);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_cappy_throw_water(struct MarioState *m) {
    action_init_no_y_vel(m->forwardVel, 0, SOUND_ACTION_SWIM);

    // Cancels
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_BREASTSTROKE, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_WATER_PUNCH, 0, RETURN_CANCEL);

    // Step
    m->vel[1] *= 0.9f;
    mario_set_forward_vel(m, m->forwardVel * 0.9f);
    perform_water_step(m);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_grabbed_water(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_UNK2) {
        m->vel[1] *= 0.5f;
        m->faceAngle[1] = m->usedObj->oMoveAngleYaw;
        vec3f_copy(m->pos, m->marioObj->oGfxPos);
        SFX(SOUND_MARIO_WAAAOOOW);
        return omm_mario_set_action(m, m->forwardVel >= 0.f ? ACT_FORWARD_WATER_KB : ACT_BACKWARD_WATER_KB, 0, 0xFFFF);
    }
    ANM(MARIO_ANIM_BEING_GRABBED, 1.f);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

///////////////
// Submerged //
///////////////

static bool omm_check_common_submerged_cancels(struct MarioState *m) {

    // Water surface
    if (m->pos[1] >= m->waterLevel - OMM_MARIO_WATER_SURFACE_OFFSET) {
        if (m->floorHeight >= m->waterLevel - OMM_MARIO_WATER_SURFACE_OFFSET) {
            if (OMM_MOVESET_ODYSSEY && m->action == ACT_WATER_SHELL_SWIMMING) {
                audio_stop_shell_music();
            }
            transition_submerged_to_walking(m);
            return true;
        }
        m->pos[1] = m->waterLevel - OMM_MARIO_WATER_SURFACE_OFFSET;
        m->faceAngle[0] = approach_s32(m->faceAngle[0], 0, 0, 0x200 * (m->controller->stickY > -8));
    }

    // Dead
    if (omm_mario_is_dead(m) && !(m->action & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE))) {
        drop_and_set_mario_action(m, ACT_DROWNING, 0);
        return true;
    }

    // Entering water with an underwater koopa shell
    if (OMM_MOVESET_ODYSSEY && m->heldObj && m->heldObj->behavior == bhvKoopaShellUnderwater && m->prevAction == ACT_WATER_PLUNGE) {
        omm_mario_set_action(m, ACT_WATER_SHELL_SWIMMING, 0, 0);
        return true;
    }

    return false;
}

s32 omm_mario_execute_submerged_action(struct MarioState *m) {
    gOmmMario->wallSlide.jumped = false;
    gOmmMario->peach.floated = false;
    gOmmMario->cappy.bounced = false;
    gOmmMario->state.airCombo = 0;
    gOmmMario->midairSpin.counter = 0;
    m->quicksandDepth = 0.f;
    m->marioBodyState->headAngle[1] = 0;
    m->marioBodyState->headAngle[2] = 0;

    // Cancels
    if (omm_check_common_submerged_cancels(m)) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Actions
    switch (m->action) {

        // SM64
        case ACT_WATER_IDLE:                        return omm_act_water_idle(m);
        case ACT_WATER_ACTION_END:                  return omm_act_water_action_end(m);
        case ACT_SWIMMING_END:                      return omm_act_swimming_end(m);
        case ACT_BREASTSTROKE:                      return omm_act_breaststroke(m);
        case ACT_FLUTTER_KICK:                      return omm_act_flutter_kick(m);
        case ACT_HOLD_WATER_IDLE:                   return omm_act_hold_water_idle(m);
        case ACT_HOLD_WATER_ACTION_END:             return omm_act_hold_water_action_end(m);
        case ACT_HOLD_SWIMMING_END:                 return omm_act_hold_swimming_end(m);
        case ACT_HOLD_BREASTSTROKE:                 return omm_act_hold_breaststroke(m);
        case ACT_HOLD_FLUTTER_KICK:                 return omm_act_hold_flutter_kick(m);
        case ACT_DROWNING:                          return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_BACKWARD_WATER_KB:                 return omm_act_knockback_water(m, MARIO_ANIM_BACKWARDS_WATER_KB);
        case ACT_FORWARD_WATER_KB:                  return omm_act_knockback_water(m, MARIO_ANIM_WATER_FORWARD_KB);
        case ACT_WATER_DEATH:                       return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_WATER_SHOCKED:                     return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_WATER_SHELL_SWIMMING:              return omm_act_water_shell_swimming(m);
        case ACT_WATER_THROW:                       return omm_act_water_throw(m);
        case ACT_WATER_PUNCH:                       return omm_act_water_punch(m);
        case ACT_WATER_PLUNGE:                      return omm_act_water_plunge(m);
        case ACT_CAUGHT_IN_WHIRLPOOL:               return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_METAL_WATER_STANDING:              return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_IDLE,               0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_HOLD_METAL_WATER_STANDING:         return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_HOLD_IDLE,          0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_METAL_WATER_WALKING:               return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_WALKING,            0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_HOLD_METAL_WATER_WALKING:          return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_HOLD_WALKING,       0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_METAL_WATER_FALLING:               return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_FREEFALL,           0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_HOLD_METAL_WATER_FALLING:          return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_HOLD_FREEFALL,      0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_METAL_WATER_FALL_LAND:             return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_FREEFALL_LAND,      0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_HOLD_METAL_WATER_FALL_LAND:        return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_HOLD_FREEFALL_LAND, 0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_METAL_WATER_JUMP:                  return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_JUMP,               0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_HOLD_METAL_WATER_JUMP:             return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_HOLD_JUMP,          0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_METAL_WATER_JUMP_LAND:             return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_JUMP_LAND,          0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);
        case ACT_HOLD_METAL_WATER_JUMP_LAND:        return (OMM_POWER_UPS_IMPROVED ? omm_mario_set_action(m, ACT_OMM_METAL_WATER_HOLD_JUMP_LAND,     0, 0) : OMM_MARIO_ACTION_RESULT_CONTINUE);

        // Odyssey
        case ACT_OMM_WATER_FIRST_PERSON:            return omm_act_water_first_person(m);
        case ACT_OMM_WATER_GROUND_POUND:            return omm_act_water_ground_pound(m);
        case ACT_OMM_WATER_GROUND_POUND_LAND:       return omm_act_water_ground_pound_land(m);
        case ACT_OMM_WATER_GROUND_POUND_JUMP:       return omm_act_water_ground_pound_jump(m);
        case ACT_OMM_WATER_DASH:                    return omm_act_water_dash(m);
        case ACT_OMM_LEAVE_OBJECT_WATER:            return omm_act_leave_object_water(m);
        case ACT_OMM_CAPPY_THROW_WATER:             return omm_act_cappy_throw_water(m);
        case ACT_OMM_GRABBED_WATER:                 return omm_act_grabbed_water(m);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

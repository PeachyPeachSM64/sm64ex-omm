#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_act_submerged_stationary_slow_down(struct MarioState *m, f32 slowModifier) {
    m->angleVel[0] = 0;
    m->angleVel[1] = 0;
    m->faceAngle[0] = approach_s32(m->faceAngle[0], 0, 0x200, 0x200);
    m->faceAngle[2] = approach_s32(m->faceAngle[2], 0, 0x100, 0x100);
    m->forwardVel = approach_f32(m->forwardVel, 0.f, slowModifier, slowModifier);
    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
    m->vel[1] = approach_f32(m->vel[1], 0.f, 2.f * slowModifier, slowModifier);
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
    m->faceAngle[1] += m->angleVel[1];
    return omm_act_submerged_cancels__cappy_dash_pound(m);
}

static s32 omm_act_hold_water_idle(struct MarioState *m) {
    m->faceAngle[1] += m->angleVel[1];
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_knockback_water(struct MarioState *m, s32 animID) {
    if (OMM_MOVESET_CLASSIC) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    omm_act_submerged_stationary_slow_down(m, 1.5f);
    perform_water_step(m);

    obj_anim_play(m->marioObj, animID, 1.5f);
    m->marioBodyState->headAngle[0] = 0;
    if (obj_anim_is_at_end(m->marioObj)) {
        m->invincTimer = 30 * (m->actionArg > 0);
        omm_mario_set_action(m, omm_mario_is_dead(m) ? ACT_WATER_DEATH : ACT_WATER_IDLE, 0, 0);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_water_throw(struct MarioState *m) {
    if (m->heldObj) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    return omm_act_submerged_cancels__cappy_dash_pound(m);
}

/////////////
// Odyssey //
/////////////

static s32 omm_act_water_dash(struct MarioState *m) {
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_condition(!(m->controller->buttonDown & B_BUTTON), ACT_SWIMMING_END, 0, RETURN_CANCEL);

    // Speed
    m->forwardVel = 56.f;
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
    m->faceAngle[2] = (s16) (m->controller->stickX * 64.f);

    // Perform step
    u32 step = perform_water_step(m);
    if (step == WATER_STEP_HIT_WALL && m->wall) {
        Vec3f v; vec3f_copy(v, m->vel); vec3f_normalize(v);
        f32 dot = vec3f_dot(v, (f32 *) &m->wall->normal);
        if (dot < -0.5f) {
            m->vel[1] /= 1.5f;
            mario_set_forward_vel(m, m->forwardVel / 1.5f);
            SFX(SOUND_MARIO_OOOF2);
            PFX(PARTICLE_VERTICAL_STAR);
            action_condition(1, ACT_BACKWARD_WATER_KB, 0, RETURN_BREAK);
        }
    }

    // Animation
    obj_anim_play(m->marioObj, MARIO_ANIM_FLUTTERKICK, 3.f);
    m->marioBodyState->headAngle[0] = approach_s32(m->marioBodyState->headAngle[0], 0, 0x200, 0x200);
    m->particleFlags |= (PARTICLE_PLUNGE_BUBBLE | PARTICLE_BUBBLE);
    if ((m->actionTimer++ % 4) == 0) {
        play_sound(SOUND_ACTION_UNKNOWN434, m->marioObj->oCameraToObject);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_water_ground_pound(struct MarioState *m) {
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_DASH, 0, RETURN_CANCEL);

    if (m->actionState == 0) {
        vec3f_set(m->vel, 0, 0, 0);
        obj_anim_play(m->marioObj, MARIO_ANIM_START_GROUND_POUND, 1.25f);
        if (m->actionTimer == 0) {
            play_sound(SOUND_ACTION_SPIN, m->marioObj->oCameraToObject);
        }

        if (m->actionTimer < 10) {
            m->pos[1] += (20 - 2 * m->actionTimer);
            vec3f_copy(m->marioObj->oGfxPos, m->pos);
        } else if (m->actionTimer >= m->marioObj->oCurrAnim->mLoopEnd + 1) {
            play_sound(SOUND_MARIO_GROUND_POUND_WAH, m->marioObj->oCameraToObject);
            play_sound(SOUND_ACTION_UNKNOWN430, m->marioObj->oCameraToObject);
            vec3s_set(m->faceAngle, 0, m->faceAngle[1], 0);
            vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
            m->vel[1] = -105.f;
            m->actionState = 1;
        }
        m->actionTimer++;
    } else {
        if (m->actionState == 1) {
            if (m->controller->buttonDown & Z_TRIG) {
                m->vel[1] = min_f(m->vel[1], -80.f);
            } else {
                m->actionState = 2;
            }
        }
        m->vel[1] += 5.f;
        obj_anim_play(m->marioObj, MARIO_ANIM_GROUND_POUND, 1.f);
        m->particleFlags |= (PARTICLE_PLUNGE_BUBBLE | PARTICLE_BUBBLE);

        s32 step = perform_water_step(m);
        action_condition(step == WATER_STEP_HIT_FLOOR, ACT_OMM_WATER_GROUND_POUND_LAND, 0, RETURN_BREAK, PFX(PARTICLE_MIST_CIRCLE | PARTICLE_HORIZONTAL_STAR););
        action_condition(m->vel[1] >= 0, ACT_WATER_IDLE, 0, RETURN_BREAK, m->vel[1] = 0;);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_water_ground_pound_land(struct MarioState *m) {
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL_LAND, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALL_LAND, 0, RETURN_CANCEL);

    stationary_ground_step(m);
    if (m->actionState == 0) {
        action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_GROUND_POUND_JUMP, 0, RETURN_CANCEL);
        obj_anim_play(m->marioObj, MARIO_ANIM_GROUND_POUND_LANDING, 0.5f);
        if (obj_anim_is_at_end(m->marioObj)) m->actionState = 1;
    } else {
        obj_anim_play(m->marioObj, MARIO_ANIM_STOP_SLIDE, 1.f);
        action_condition(obj_anim_is_at_end(m->marioObj), ACT_WATER_IDLE, 0, RETURN_BREAK);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_water_ground_pound_jump(struct MarioState *m) {
    action_init(0, 48.f, 0, SOUND_ACTION_UNKNOWN430, gOmmMario->spin.yaw = 1;);
    action_condition(m->pos[1] >= m->waterLevel - 80, ACT_WATER_JUMP, 0, RETURN_CANCEL);
    action_condition(m->vel[1] <= 0.f, ACT_WATER_IDLE, 0, RETURN_CANCEL);
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);

    omm_act_submerged_stationary_slow_down(m, 2.f);
    perform_water_step(m);

    obj_anim_play(m->marioObj, MARIO_ANIM_DOUBLE_JUMP_RISE, 1.f);
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += (0x90 * m->vel[1]) * (prevSpinYaw != 0) * (omm_mario_has_vanish_cap(m) ? 0.8f : 1.f) / sqr_f(omm_player_physics_get_selected_jump());
    gOmmMario->spin.yaw *= ((u16) prevSpinYaw < (u16) gOmmMario->spin.yaw) * (m->vel[1] > 0.f);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] + gOmmMario->spin.yaw;
    m->particleFlags |= (PARTICLE_PLUNGE_BUBBLE | PARTICLE_BUBBLE);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_leave_object_water(struct MarioState *m) {
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);

    omm_act_submerged_stationary_slow_down(m, 1.f);
    perform_water_step(m);
    obj_anim_play(m->marioObj, MARIO_ANIM_WATER_IDLE, 1.f);
    m->marioBodyState->headAngle[0] = 0;
    m->particleFlags |= PARTICLE_SPARKLES;

    action_condition(m->pos[1] >= m->waterLevel - 80, ACT_WATER_JUMP, 0, RETURN_BREAK);
    action_condition(abs_f(m->vel[1]) < 1.f, ACT_WATER_IDLE, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_cappy_throw_water(struct MarioState *m) {
    action_condition(omm_mario_has_metal_cap(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->flags & MARIO_METAL_CAP, ACT_METAL_WATER_FALLING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_WATER, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_BREASTSTROKE, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_WATER_PUNCH, 0, RETURN_CANCEL);
    action_init(m->forwardVel, m->vel[1], 0, SOUND_ACTION_SWIM);

    m->vel[1] *= 0.9f;
    mario_set_forward_vel(m, m->forwardVel * 0.9f);
    perform_water_step(m);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

///////////////
// Submerged //
///////////////

static bool omm_check_common_submerged_cancels(struct MarioState *m) {

    // Water surface
    if (m->pos[1] > m->waterLevel - 80) {
        if (m->floorHeight >= m->waterLevel - 80) {
            transition_submerged_to_walking(m);
            return true;
        }
        m->pos[1] = m->waterLevel - 80;
    }

    // Dead
    if (omm_mario_is_dead(m) && !(m->action & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE))) {
        drop_and_set_mario_action(m, ACT_DROWNING, 0);
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
        case ACT_HOLD_WATER_IDLE:                   return omm_act_hold_water_idle(m);
        case ACT_WATER_ACTION_END:                  return omm_act_water_idle(m);
        case ACT_HOLD_WATER_ACTION_END:             return omm_act_hold_water_idle(m);
        case ACT_DROWNING:                          return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_BACKWARD_WATER_KB:                 return omm_act_knockback_water(m, MARIO_ANIM_BACKWARDS_WATER_KB);
        case ACT_FORWARD_WATER_KB:                  return omm_act_knockback_water(m, MARIO_ANIM_WATER_FORWARD_KB);
        case ACT_WATER_DEATH:                       return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_WATER_SHOCKED:                     return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_BREASTSTROKE:                      return omm_act_submerged_cancels__cappy_dash_pound(m);
        case ACT_SWIMMING_END:                      return omm_act_submerged_cancels__cappy_dash_pound(m);
        case ACT_FLUTTER_KICK:                      return omm_act_submerged_cancels__cappy_dash_pound(m);
        case ACT_HOLD_BREASTSTROKE:                 return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLD_SWIMMING_END:                 return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLD_FLUTTER_KICK:                 return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_WATER_SHELL_SWIMMING:              return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_WATER_THROW:                       return omm_act_water_throw(m);
        case ACT_WATER_PUNCH:                       return omm_act_submerged_cancels__cappy_dash_pound(m);
        case ACT_WATER_PLUNGE:                      return OMM_MARIO_ACTION_RESULT_CONTINUE;
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
        case ACT_OMM_WATER_GROUND_POUND:            return omm_act_water_ground_pound(m);
        case ACT_OMM_WATER_GROUND_POUND_LAND:       return omm_act_water_ground_pound_land(m);
        case ACT_OMM_WATER_GROUND_POUND_JUMP:       return omm_act_water_ground_pound_jump(m);
        case ACT_OMM_WATER_DASH:                    return omm_act_water_dash(m);
        case ACT_OMM_LEAVE_OBJECT_WATER:            return omm_act_leave_object_water(m);
        case ACT_OMM_CAPPY_THROW_WATER:             return omm_act_cappy_throw_water(m);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

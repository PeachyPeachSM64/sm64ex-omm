#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_common_ground_knockback_action(struct MarioState *m, s32 animID, bool playHeavyLandingSound) {

    // Animation
    obj_anim_play(m->marioObj, animID, 1.25f);
    m->actionTimer++;

    // Sound effects
    if (playHeavyLandingSound) play_mario_heavy_landing_sound_once(m, SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
    play_sound_if_no_flag(m, m->actionArg > 0 ? SOUND_MARIO_ATTACKED : SOUND_MARIO_OOOF2, MARIO_MARIO_SOUND_PLAYED);

    // Step
    mario_set_forward_vel(m, clamp_f(m->forwardVel * 0.9f, -32.f, 32.f));
    if (perform_ground_step(m) == GROUND_STEP_LEFT_GROUND) {
        omm_mario_set_action(m, m->forwardVel >= 0.f ? ACT_FORWARD_AIR_KB : ACT_BACKWARD_AIR_KB, m->actionArg, 0);
    } else if (obj_anim_is_at_end(m->marioObj)) {
        if (m->health <= OMM_HEALTH_DEAD) {
            omm_mario_set_action(m, ACT_STANDING_DEATH, 0, 0);
        } else {
            m->invincTimer = 30 * (m->actionArg > 0);
            omm_mario_set_action(m, ACT_IDLE, 0, 0);
        }
    }
}

static void anim_and_audio_for_walk(struct MarioState *m) {
    f32 walkingSpeed = max_3_f(4.f, m->forwardVel, m->intendedMag);
    struct {
        bool condToTipToe;
        bool condToWalk;
        bool condToRun;
        bool tiltBody;
        s32 animID;
        f32 animAccel;
        s32 soundStep1;
        s32 soundStep2;
    } walkAnimAndAudio[5] = {

        // Start tip-toe
        {
            .condToTipToe = m->marioObj->oCurrAnim && obj_anim_get_frame(m->marioObj) >= 23,
            .condToWalk = walkingSpeed > 8.f,
            .condToRun = false,
            .tiltBody = false,
            .animID = MARIO_ANIM_START_TIPTOE,
            .animAccel = 0.25f,
            .soundStep1 = 7,
            .soundStep2 = 22,
        },

        // Tip-toe
        {
            .condToTipToe = false,
            .condToWalk = walkingSpeed > 8.f,
            .condToRun = false,
            .tiltBody = false,
            .animID = MARIO_ANIM_TIPTOE,
            .animAccel = 1.f,
            .soundStep1 = 14,
            .soundStep2 = 72,
        },

        // Walk
        {
            .condToTipToe = walkingSpeed < 5.f,
            .condToWalk = false,
            .condToRun = omm_mario_should_run(m),
            .tiltBody = true,
            .animID = MARIO_ANIM_WALKING,
            .animAccel = 0.25f,
            .soundStep1 = 10,
            .soundStep2 = 49,
        },

        // Run
        {
            .condToTipToe = false,
            .condToWalk = omm_mario_should_walk(m),
            .condToRun = false,
            .tiltBody = true,
            .animID = MARIO_ANIM_RUNNING,
            .animAccel = (OMM_PLAYER_IS_WARIO ? 0.20f : 0.25f),
            .soundStep1 = 9,
            .soundStep2 = 45,
        },

        // Quicksand
        {
            .condToTipToe = false,
            .condToWalk = false,
            .condToRun = false,
            .tiltBody = false,
            .animID = MARIO_ANIM_MOVE_IN_QUICKSAND,
            .animAccel = 0.25f,
            .soundStep1 = 19,
            .soundStep2 = 93,
        }
    };

    for (;;) {
        bool inQuicksand = (m->quicksandDepth > 50.f);
        m->actionTimer *= !inQuicksand;
        s32 index = m->actionTimer + 4 * inQuicksand;
        if (walkAnimAndAudio[index].condToRun) {
            m->actionTimer = 3;
        } else if (walkAnimAndAudio[index].condToWalk) {
            m->actionTimer = 2;
        } else if (walkAnimAndAudio[index].condToTipToe) {
            m->actionTimer = 1;
        } else {
            s16 targetPitch = walkAnimAndAudio[index].tiltBody * -((find_floor_slope(m, 0) * m->forwardVel) / 40.f);
            m->marioObj->oMarioWalkingPitch = (s16) approach_s32(m->marioObj->oMarioWalkingPitch, targetPitch, 0x800, 0x800);
            m->marioObj->oGfxAngle[0] = m->marioObj->oMarioWalkingPitch;
            obj_anim_play(m->marioObj, walkAnimAndAudio[index].animID, max_f(walkingSpeed * walkAnimAndAudio[index].animAccel, 0.1f));
            play_step_sound(m, walkAnimAndAudio[index].soundStep1, walkAnimAndAudio[index].soundStep2);
            break;
        }
    }
}

//
// Actions
//

static s32 omm_act_walking(struct MarioState *m) {
    mario_drop_held_object(m);

    // Cancels
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_za_pressed(OMM_MOVESET_ODYSSEY && m->forwardVel > 8.f, ACT_LONG_JUMP, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY && analog_stick_held_back(m) && m->forwardVel >= 8.f, ACT_SIDE_FLIP, 0, RETURN_CANCEL);
    action_a_pressed(1, 0, 0, RETURN_CANCEL, set_jump_from_landing(m););
    action_b_pressed(OMM_MOVESET_ODYSSEY && !omm_player_is_selected(OMM_PLAYER_WARIO), ACT_MOVE_PUNCHING, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_CROUCH_SLIDE, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    action_condition(!omm_peach_vibe_is_gloom() && should_begin_sliding(m), ACT_BEGIN_SLIDING, 0, RETURN_CANCEL);
    action_condition(m->input & INPUT_FIRST_PERSON, 0, 0, RETURN_CANCEL, begin_braking_action(m););
    action_condition(m->input & INPUT_UNKNOWN_5, 0, 0, RETURN_CANCEL, begin_braking_action(m););
    action_condition(check_ground_dive_or_punch(m), 0, 0, RETURN_CANCEL);
    action_condition(!omm_peach_vibe_is_gloom() && analog_stick_held_back(m) && m->forwardVel >= (OMM_MOVESET_ODYSSEY ? 8.f : 16.f), ACT_TURNING_AROUND, 0, RETURN_CANCEL);

    // Step
    m->actionState = 0;
    update_walking_speed(m);
    switch (perform_ground_step(m)) {
        case GROUND_STEP_NONE: {
            if (omm_peach_vibe_is_gloom()) {
                obj_anim_play(m->marioObj, MARIO_ANIM_RUNNING, max_f(1.f, abs_f(m->forwardVel / 2.f)));
                play_step_sound(m, 9, 45);
            } else {
                anim_and_audio_for_walk(m);
            }
            if (m->intendedMag - m->forwardVel > 16.f) {
                m->particleFlags |= PARTICLE_DUST;
            }
        } break;

        case GROUND_STEP_LEFT_GROUND: {
            omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
            obj_anim_play(m->marioObj, MARIO_ANIM_GENERAL_FALL, 1.f);
        } break;

        case GROUND_STEP_HIT_WALL: {
            push_or_sidle_wall(m, gOmmData->mario->state.previous.pos);
            m->actionTimer = 0;
        } break;
    }
    check_ledge_climb_down(m);
    tilt_body_walking(m, gOmmData->mario->state.previous.angle[1]);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_turning_around(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_MOVE_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_finish_turning_around(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_braking(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    if (omm_peach_vibe_is_gloom() && !(m->input & INPUT_NONZERO_ANALOG)) {
        mario_set_forward_vel(m, m->forwardVel * 0.9f);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_decelerating(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_MOVE_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    if (omm_peach_vibe_is_gloom() && !(m->input & INPUT_NONZERO_ANALOG)) {
        mario_set_forward_vel(m, m->forwardVel * 0.9f);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_crawling(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_crouch_slide(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_jump_land(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_freefall_land(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_double_jump_land(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_side_flip_land(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_triple_jump_land(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_backflip_land(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_quicksand_jump_land(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_long_jump_land(struct MarioState *m) {
#if !OMM_GAME_IS_R96A
    if (OMM_CHEAT_BLJ_ANYWHERE_REQ) {
        action_a_pressed(1, ACT_LONG_JUMP, 0, RETURN_CANCEL, m->vel[1] = -50.f;);
    }
#endif
    action_condition(!OMM_CHEAT_BLJ_ANYWHERE_REQ && omm_mario_is_roll_landing(m), ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_dive_slide(struct MarioState *m) {
    action_condition(omm_mario_is_roll_landing(m), ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_burning_ground(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        m->hurtCounter += (m->marioObj->oMarioBurnTimer == 0);
        action_condition(m->marioObj->oMarioBurnTimer > 160, ACT_WALKING, 0, RETURN_CANCEL);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_hard_backward_ground_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    omm_common_ground_knockback_action(m, MARIO_ANIM_FALL_OVER_BACKWARDS, true);
    if (m->actionTimer == 43 && m->prevAction == ACT_SPECIAL_DEATH_EXIT) {
        play_sound(SOUND_MARIO_MAMA_MIA, m->marioObj->oCameraToObject);
    }
    if (m->actionTimer == 55) {
        play_mario_landing_sound_once(m, SOUND_ACTION_TERRAIN_LANDING);
    }

    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_hard_forward_ground_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    omm_common_ground_knockback_action(m, MARIO_ANIM_LAND_ON_STOMACH, true);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_backward_ground_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    omm_common_ground_knockback_action(m, MARIO_ANIM_BACKWARD_KB, true);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_forward_ground_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    omm_common_ground_knockback_action(m, MARIO_ANIM_FORWARD_KB, true);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_soft_backward_ground_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    omm_common_ground_knockback_action(m, MARIO_ANIM_SOFT_BACK_KB, false);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_soft_forward_ground_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    omm_common_ground_knockback_action(m, MARIO_ANIM_SOFT_FRONT_KB, false);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_ground_bonk(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    omm_common_ground_knockback_action(m, MARIO_ANIM_GROUND_BONK, true);
    if (m->actionTimer == 25) {
        play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
    }

    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_move_punching(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY && OMM_PLAYER_IS_PEACH, ACT_OMM_PEACH_ATTACK_GROUND, ((m->prevAction == ACT_OMM_SPIN_GROUND) ? 4 : 0), RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

#if OMM_GAME_IS_R96A
static s32 omm_act_wario_charge(struct MarioState *m) {
    extern s32 charge_count;
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL, charge_count = 0;);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL, charge_count = 0;);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}
#endif

static s32 omm_act_roll(struct MarioState *m) {
    bool superRoll = (m->prevAction == ACT_GROUND_POUND_LAND) || (m->prevAction == ACT_OMM_SPIN_POUND_LAND) || ((m->prevAction == ACT_OMM_ROLL) && (m->forwardVel >= 45.f));
    action_init(max_f(m->forwardVel, superRoll ? 75.f : 60.f), 0, superRoll ? PARTICLE_HORIZONTAL_STAR : PARTICLE_MIST_CIRCLE, 0, m->faceAngle[0] *= (m->prevAction == ACT_OMM_ROLL););
    action_cappy(m->actionTimer >= 6, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_LONG_JUMP, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY && m->actionTimer >= 6, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    m->actionTimer++;

    // Speed
    s32 floorClass = mario_get_floor_class(m);
    bool isFlatGround = (floorClass == SURFACE_CLASS_NOT_SLIPPERY) || ((floorClass != SURFACE_CLASS_SLIPPERY) && (floorClass != SURFACE_CLASS_VERY_SLIPPERY) && (m->floor->normal.y > 0.94f)); // < 20 deg
    if (isFlatGround) {

        // Mario slowly decelerates on flat ground
        // Decelerates even slower if Z is held
        f32 decelMult = ((m->controller->buttonDown & Z_TRIG) ? 0.25f : 1.00f);
        mario_set_forward_vel(m, max_f(0, m->forwardVel - OMM_MARIO_ROLL_DECEL * decelMult));
        s32 handling = (OMM_CHEAT_SUPER_RESPONSIVE ? 0x8000 : OMM_MARIO_ROLL_YAW_VEL);
        m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, handling, handling);

    } else {

        // Mario accelerates instead of decelerate if he's facing away the slope
        // Decelerates (only) even slower if Z is held
        f32 sign = ((sins(m->faceAngle[1]) * m->floor->normal.x + coss(m->faceAngle[1]) * m->floor->normal.z > 0.f) ? +1.f : -1.f);
        f32 t = clamp_0_1_f(1.f - m->floor->normal.y);
        f32 decelMult = ((m->controller->buttonDown & Z_TRIG) && (sign < 0) ? 0.25f : 1.00f);
        mario_set_forward_vel(m, clamp_f(m->forwardVel + sign * OMM_MARIO_ROLL_DECEL * decelMult * (1.f + 2.f * t), 0.f, OMM_MARIO_ROLL_MAX_SPEED));

        // Progressively turn Mario in the direction of the slope...
        if (m->floor->normal.z != 0 || m->floor->normal.x != 0) {
            s16 floorAngle = atan2s(m->floor->normal.z, m->floor->normal.x);
            s16 yawVel = OMM_MARIO_ROLL_YAW_VEL * (0.75f + (sign == +1 ? 1.f : 5.f) * t);
            m->faceAngle[1] = floorAngle - approach_s32((s16) (floorAngle - m->faceAngle[1]), 0, yawVel, yawVel);
        }

        // ...but this rotation can be partially canceled with the control stick
        if (m->input & INPUT_NONZERO_ANALOG) {
            f32 yawVelMult = 1.f + 0.25f * (m->forwardVel / OMM_MARIO_ROLL_MAX_SPEED);
            s32 handling = (OMM_CHEAT_SUPER_RESPONSIVE ? 0x8000 : (yawVelMult * OMM_MARIO_ROLL_YAW_VEL));
            m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, handling, handling);
        }
    }

    // Stop if Mario isn't fast enough
    action_condition(m->forwardVel <= OMM_MARIO_ROLL_MIN_SPEED && isFlatGround, ACT_WALKING, 0, RETURN_CANCEL);
    action_condition(m->forwardVel <= OMM_MARIO_ROLL_MIN_SPEED && !isFlatGround, ACT_STOMACH_SLIDE, 0, RETURN_CANCEL);

    // Perform step
    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_ROLL_AIR, 0, RETURN_BREAK);
    action_condition(step == GROUND_STEP_HIT_WALL, ACT_BACKWARD_GROUND_KB, 0, RETURN_BREAK, PFX(PARTICLE_VERTICAL_STAR););

    // Animation
    f32 speed = m->forwardVel / 60.f;
    s16 prevAngle = m->faceAngle[0];
    m->faceAngle[0] += (s16) (0x1C00 * speed);
    obj_anim_play(m->marioObj, MARIO_ANIM_FORWARD_SPINNING, 0.01f);
    obj_anim_set_frame(m->marioObj, 0);
    Vec3f v = { 0, -60, -10 };
    vec3f_rotate_zxy(v, v, m->faceAngle[0], m->faceAngle[1], 0);
    m->marioObj->oGfxPos[0] += v[0];
    m->marioObj->oGfxPos[1] += v[1] + 50;
    m->marioObj->oGfxPos[2] += v[2];
    m->marioObj->oGfxAngle[0] = m->faceAngle[0];
    obj_set_shadow_pos_to_object_pos(gMarioObject);
    m->particleFlags |= PARTICLE_DUST;
    if (prevAngle > m->faceAngle[0]) {
        play_sound(SOUND_ACTION_TWIRL, m->marioObj->oCameraToObject);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_cappy_throw_ground(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_za_pressed(OMM_MOVESET_ODYSSEY && m->forwardVel > 8.f, ACT_LONG_JUMP, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    f32 f = coss(abs_s(m->faceAngle[1] - m->intendedYaw)) * m->controller->stickMag / 64.f;
    mario_set_forward_vel(m, m->forwardVel * clamp_f(f, 0.80f, 0.98f));
    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_FREEFALL, 0, RETURN_BREAK);
    action_condition(step == GROUND_STEP_HIT_WALL, ACT_IDLE, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

//
// Moving
//

static bool omm_check_common_moving_cancels(struct MarioState *m) {

    // Plunge
    if (m->pos[1] < m->waterLevel - 100) {
        set_water_plunge_action(m);
        return true;
    }

    // Squished
    if (m->input & INPUT_SQUISHED) {
        drop_and_set_mario_action(m, ACT_SQUISHED, 0);
        return true;
    }

    // Dead
    if ((m->health <= OMM_HEALTH_DEAD) && !(m->action & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE))) {
        drop_and_set_mario_action(m, ACT_STANDING_DEATH, 0);
        return true;
    }

    return false;
}

s32 omm_mario_execute_moving_action(struct MarioState *m) {
    gOmmData->mario->wallSlide.jumped = false;
    gOmmData->mario->peach.floated = false;
    gOmmData->mario->cappy.bounced = false;
    gOmmData->mario->state.airCombo = 0;
    gOmmData->mario->midairSpin.counter = 0;

    // Cancels
    if (omm_check_common_moving_cancels(m)) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Quicksand
    // Needs to be 0 to not apply the sinking twice per frame
    if (mario_update_quicksand(m, (m->action == ACT_WALKING ? 0.25f : 0.f))) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // In water
    if (m->input & INPUT_IN_WATER) {
        m->particleFlags |= PARTICLE_WAVE_TRAIL;
        m->particleFlags &= ~PARTICLE_DUST;
    }

    // Actions
    switch (m->action) {
        case ACT_WALKING:                   return omm_act_walking(m);
        case ACT_TURNING_AROUND:            return omm_act_turning_around(m);
        case ACT_FINISH_TURNING_AROUND:     return omm_act_finish_turning_around(m);
        case ACT_BRAKING:                   return omm_act_braking(m);
        case ACT_CRAWLING:                  return omm_act_crawling(m);
        case ACT_DECELERATING:              return omm_act_decelerating(m);
        case ACT_CROUCH_SLIDE:              return omm_act_crouch_slide(m);
        case ACT_JUMP_LAND:                 return omm_act_jump_land(m);
        case ACT_FREEFALL_LAND:             return omm_act_freefall_land(m);
        case ACT_DOUBLE_JUMP_LAND:          return omm_act_double_jump_land(m);
        case ACT_SIDE_FLIP_LAND:            return omm_act_side_flip_land(m);
        case ACT_TRIPLE_JUMP_LAND:          return omm_act_triple_jump_land(m);
        case ACT_BACKFLIP_LAND:             return omm_act_backflip_land(m);
        case ACT_QUICKSAND_JUMP_LAND:       return omm_act_quicksand_jump_land(m);
        case ACT_LONG_JUMP_LAND:            return omm_act_long_jump_land(m);
        case ACT_DIVE_SLIDE:                return omm_act_dive_slide(m);
        case ACT_BURNING_GROUND:            return omm_act_burning_ground(m);
        case ACT_HARD_BACKWARD_GROUND_KB:   return omm_act_hard_backward_ground_kb(m);
        case ACT_HARD_FORWARD_GROUND_KB:    return omm_act_hard_forward_ground_kb(m);
        case ACT_BACKWARD_GROUND_KB:        return omm_act_backward_ground_kb(m);
        case ACT_FORWARD_GROUND_KB:         return omm_act_forward_ground_kb(m);
        case ACT_SOFT_BACKWARD_GROUND_KB:   return omm_act_soft_backward_ground_kb(m);
        case ACT_SOFT_FORWARD_GROUND_KB:    return omm_act_soft_forward_ground_kb(m);
        case ACT_GROUND_BONK:               return omm_act_ground_bonk(m);
        case ACT_MOVE_PUNCHING:             return omm_act_move_punching(m);
#if OMM_GAME_IS_R96A
        case ACT_WARIO_CHARGE:              return omm_act_wario_charge(m);
#endif

        // Odyssey
        case ACT_OMM_ROLL:                  return omm_act_roll(m);
        case ACT_OMM_CAPPY_THROW_GROUND:    return omm_act_cappy_throw_ground(m);

        // Peach
        case ACT_OMM_PEACH_ATTACK_FAST:     return omm_act_peach_attack_fast(m);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

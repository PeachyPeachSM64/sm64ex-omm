#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_act_walking_anim_and_audio(struct MarioState *m) {
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
            .condToTipToe = m->marioObj->oCurrAnim && m->marioObj->oAnimFrame >= 23,
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
            ANM(walkAnimAndAudio[index].animID, max_f(walkingSpeed * walkAnimAndAudio[index].animAccel, 0.1f));
            play_step_sound(m, walkAnimAndAudio[index].soundStep1, walkAnimAndAudio[index].soundStep2);
            break;
        }
    }
}

void omm_act_roll_update_gfx(struct MarioState *m) {
    f32 headHeight = geo_get_marios_anim_part_height(m->marioObj->oGraphNode, MARIO_ANIM_PART_HEAD);
    f32 rollOffsetMult = (1.f + 0.5f * ((headHeight - 85.f) / 85.f)) * m->marioObj->oScaleY;
    Vec3f v = { 0.f, -60.f * rollOffsetMult, -20.f };
    vec3f_rotate_zxy(v, v, m->faceAngle[0], m->faceAngle[1], 0);
    m->marioObj->oGfxPos[0] += v[0];
    m->marioObj->oGfxPos[1] += v[1] + 50.f * rollOffsetMult + 10.f * rollOffsetMult * sins(m->faceAngle[0]);
    m->marioObj->oGfxPos[2] += v[2];
    m->marioObj->oGfxAngle[0] = m->faceAngle[0];
    m->marioObj->oFlags |= OBJ_FLAG_SHADOW_COPY_OBJ_POS;
}

static void omm_act_roll_audio_and_particles(struct MarioState *m, s16 prevAngle) {

    // Terrain
    m->actionState += (u16) m->forwardVel;
    if (m->actionState > OMM_MARIO_ROLL_MAX_SPEED) {
        m->actionState -= OMM_MARIO_ROLL_MAX_SPEED;
        if (m->quicksandDepth > 50.f) {
            obj_play_sound(m->marioObj, SOUND_ACTION_QUICKSAND_STEP);
        } else {
            switch (m->terrainSoundAddend >> 16) {
                case SOUND_TERRAIN_WATER: PFX(PARTICLE_SHALLOW_WATER_WAVE); break;
                case SOUND_TERRAIN_SAND:  PFX(PARTICLE_DIRT); break;
                case SOUND_TERRAIN_SNOW:  PFX(PARTICLE_SNOW); break;
            }
            if (m->flags & MARIO_METAL_CAP) {
                obj_play_sound(m->marioObj, SOUND_ACTION_METAL_STEP);
            } else {
                obj_play_sound(m->marioObj, SOUND_ACTION_TERRAIN_STEP + m->terrainSoundAddend);
            }
        }
    }

    // Spin
    PFX(PARTICLE_DUST);
    if (prevAngle > m->faceAngle[0]) {
        SFX(SOUND_ACTION_TWIRL + 0x1100);
    }
}

//////////
// SM64 //
//////////

static s32 omm_act_moving_cancels__cappy_roll_punch_spin(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_MOVE_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_walking(struct MarioState *m) {
    mario_drop_held_object(m);

    // Cancels
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_za_pressed(OMM_MOVESET_ODYSSEY && m->forwardVel > 8.f, ACT_LONG_JUMP, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY && analog_stick_held_back(m) && m->forwardVel >= 8.f, ACT_SIDE_FLIP, 0, RETURN_CANCEL);
    action_a_pressed(1, 0, 0, RETURN_CANCEL, set_jump_from_landing(m););
    action_b_pressed(OMM_MOVESET_ODYSSEY && !OMM_PLAYER_IS_WARIO, ACT_MOVE_PUNCHING, 0, RETURN_CANCEL);
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
                ANM(MARIO_ANIM_RUNNING, max_f(1.f, abs_f(m->forwardVel / 2.f)));
                play_step_sound(m, 9, 45);
            } else {
                omm_act_walking_anim_and_audio(m);
            }
            if (m->intendedMag - m->forwardVel > 16.f) {
                PFX(PARTICLE_DUST);
            }
        } break;

        case GROUND_STEP_LEFT_GROUND: {
            omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
            ANM(MARIO_ANIM_GENERAL_FALL, 1.f);
        } break;

        case GROUND_STEP_HIT_WALL: {
            push_or_sidle_wall(m, gOmmMario->state.previous.pos);
            m->actionTimer = 0;
        } break;
    }
    check_ledge_climb_down(m);
    tilt_body_walking(m, gOmmMario->state.previous.angle[1]);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_riding_shell_ground(struct MarioState *m) {
    if (!OMM_MOVESET_ODYSSEY) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    action_a_pressed(1, ACT_RIDING_SHELL_JUMP, 1, RETURN_CANCEL, play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, 0););
    action_z_pressed(1, ACT_WALKING, 0, RETURN_CANCEL, mario_stop_riding_object(m););
    action_cappy(1, ACT_RIDING_SHELL_GROUND, 0xFF, NO_RETURN);

    // Make sure the shell stays above water
    if (m->floorHeight < m->waterLevel) {
        m->floorHeight = m->waterLevel;
        m->floor = &gWaterSurfacePseudoFloor;
        m->floor->originOffset = -m->waterLevel;
    }

    // Update velocity
    f32 targetVel = clamp_f(m->intendedMag * 2.5f, OMM_MARIO_SHELL_RIDE_MIN_SPEED, OMM_MARIO_SHELL_RIDE_MAX_SPEED);
    m->forwardVel += (m->forwardVel <= targetVel ? 1.5f * (1.f - clamp_f(m->forwardVel, 0.f, targetVel) / OMM_MARIO_SHELL_RIDE_MAX_SPEED) : -1.f * (m->floor->normal.y >= 0.95f));
    m->forwardVel = min_f(m->forwardVel, OMM_MARIO_SHELL_RIDE_MAX_SPEED); // Keep the shell backwards hyperspeed

    // Update yaw
    s16 prevYaw = m->faceAngle[1];
    s32 handling = lerp_f(sqrtf(invlerp_0_1_f(abs_f(m->forwardVel), 0.f, OMM_MARIO_SHELL_RIDE_MAX_SPEED)), 0x2000, 0x800);
    m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, handling, handling);
    apply_slope_accel(m);

    // Update tilt
    s16 deltaYaw = clamp_s((s16) (m->faceAngle[1] - prevYaw), -0x800, +0x800);
    s16 currTilt = (s16) (m->actionTimer & 0xFFFC);
    s16 cTiltDir = (s16) (m->actionTimer & 0x0003);
    if (abs_s(deltaYaw) > 0x400) {
        currTilt = approach_s32(currTilt, deltaYaw, 0x100, 0x100);
        if (currTilt > 0) cTiltDir = 1;
        else if (currTilt < 0) cTiltDir = 2;
        else cTiltDir = 0;
    } else {
        switch (cTiltDir) {
            case 0: currTilt = approach_s32(currTilt,  0x000, 0x100, 0x100); break;
            case 1: currTilt = approach_s32(currTilt, +0x400, 0x100, 0x100); break;
            case 2: currTilt = approach_s32(currTilt, -0x400, 0x100, 0x100); break;
        }
        if (abs_s(currTilt) == 0x400) cTiltDir = 0;
    }
    m->actionTimer = (u16) ((((u16) currTilt) & 0xFFFC) | (((u16) cTiltDir) & 0x0003));

    // Perform step
    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_RIDING_SHELL_FALL, 0, RETURN_BREAK, play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, 0););
    action_condition(step == GROUND_STEP_HIT_WALL && m->forwardVel > OMM_MARIO_SHELL_RIDE_BONK_SPEED, ACT_BACKWARD_GROUND_KB, 0, RETURN_BREAK,
        mario_stop_riding_object(m);
        SFX((m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_BONK : SOUND_ACTION_BONK);
        PFX(PARTICLE_VERTICAL_STAR);
    );

    // Animation and sound
    if (m->actionArg != 0xFF) {
#if OMM_GAME_IS_R96X
        ANM(OMM_PLAYER_IS_WARIO ? MARIO_ANIM_RIDING_SHELL : MARIO_ANIM_BEND_KNESS_RIDING_SHELL, 1.f);
#else
        ANM(MARIO_ANIM_BEND_KNESS_RIDING_SHELL, 1.f);
#endif
        obj_anim_loop(m->marioObj);
        tilt_body_ground_shell(m, m->faceAngle[1] - currTilt);
        m->marioObj->oGfxPos[1] -= 45.f;
    } else {
        vec3s_zero(m->marioBodyState->torsoAngle);
        vec3s_zero(m->marioBodyState->headAngle);
    }
    if (m->floor->type == SURFACE_BURNING) {
        SFX(SOUND_MOVING_RIDING_SHELL_LAVA);
    } else {
        SFX(SOUND_MOVING_TERRAIN_RIDING_SHELL + m->terrainSoundAddend);
    }
    adjust_sound_for_speed(m);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_crouching_crawling(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_burning_ground(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        action_condition(m->marioObj->oMarioBurnTimer > 160, ACT_WALKING, 0, RETURN_CANCEL);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_decelerating(struct MarioState *m) {
    if (omm_peach_vibe_is_gloom() && !(m->input & INPUT_NONZERO_ANALOG)) {
        mario_set_forward_vel(m, m->forwardVel * 0.9f);
    }
    return omm_act_moving_cancels__cappy_roll_punch_spin(m);
}

static s32 omm_act_slide(struct MarioState *m) {
    action_condition(omm_mario_is_roll_landing(m), ACT_OMM_ROLL, 0, RETURN_CANCEL);
    if (!m->heldObj && !(m->input & INPUT_ABOVE_SLIDE) && (m->action & ACT_FLAG_DIVING)) {
        action_a_pressed(OMM_MOVESET_ODYSSEY, m->forwardVel >= 0 ? ACT_FORWARD_ROLLOUT : ACT_BACKWARD_ROLLOUT, 0, RETURN_CANCEL);
        action_b_pressed(OMM_MOVESET_ODYSSEY, m->forwardVel >= 0 ? ACT_FORWARD_ROLLOUT : ACT_BACKWARD_ROLLOUT, 0, RETURN_CANCEL);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_move_punching(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY && OMM_PERRY_IS_AVAILABLE, ACT_OMM_PEACH_ATTACK_GROUND, ((m->prevAction == ACT_OMM_SPIN_GROUND) ? 4 : 0), RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_knockback_ground(struct MarioState *m, s32 animID, u16 landingSoundTimer, bool playHeavyLandingSound) {
    if (OMM_MOVESET_CLASSIC || omm_mario_is_dead(m)) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    // Animation
    ANM(animID, 1.25f);
    m->actionTimer++;

    // Heavy landing sound
    if (playHeavyLandingSound) {
        play_mario_heavy_landing_sound_once(m, SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
    }

    // Landing sound
    if (m->actionTimer == landingSoundTimer) {
        play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
    }

    // Death exit sound
    if (m->actionTimer == 43 && m->prevAction == ACT_SPECIAL_DEATH_EXIT) {
        SFX(SOUND_MARIO_MAMA_MIA);
    }

    // Attacked sound
    play_sound_if_no_flag(m, m->actionArg > 0 ? SOUND_MARIO_ATTACKED : SOUND_MARIO_OOOF2, MARIO_MARIO_SOUND_PLAYED);

    // Step
    mario_set_forward_vel(m, clamp_f(m->forwardVel * 0.9f, -32.f, 32.f));
    s32 step = perform_ground_step(m);
    if (step == GROUND_STEP_LEFT_GROUND) {
        omm_mario_set_action(m, m->forwardVel >= 0.f ? ACT_FORWARD_AIR_KB : ACT_BACKWARD_AIR_KB, m->actionArg, 0);
    } else if (obj_anim_is_at_end(m->marioObj)) {
        if (omm_mario_is_dead(m)) {
            omm_mario_set_action(m, ACT_STANDING_DEATH, 0, 0);
        } else {
            m->invincTimer = 30 * (m->actionArg > 0);
            omm_mario_set_action(m, ACT_IDLE, 0, 0);
        }
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_long_jump_land(struct MarioState *m) {
#if !OMM_GAME_IS_R96X
    if (OMM_CHEAT_BLJ_ANYWHERE_REQ) {
        action_a_pressed(1, ACT_LONG_JUMP, 0, RETURN_CANCEL, m->vel[1] = -50.f;);
    }
#endif
    action_condition(!OMM_CHEAT_BLJ_ANYWHERE_REQ && omm_mario_is_roll_landing(m), ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

#if OMM_GAME_IS_R96X
static s32 omm_act_wario_charge(struct MarioState *m) {
    extern s32 charge_count;
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL, charge_count = 0;);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL, charge_count = 0;);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}
#endif

/////////////
// Odyssey //
/////////////

static s32 omm_act_roll(struct MarioState *m) {
    bool superRoll = (m->prevAction == ACT_GROUND_POUND_LAND) || (m->prevAction == ACT_OMM_SPIN_POUND_LAND) || ((m->prevAction == ACT_OMM_ROLL) && (m->forwardVel >= 45.f));
    action_init(max_f(m->forwardVel, superRoll ? 75.f : 60.f), 0, superRoll ? PARTICLE_HORIZONTAL_STAR : PARTICLE_MIST_CIRCLE, 0, m->faceAngle[0] *= (m->prevAction == ACT_OMM_ROLL););
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
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
    ANM(MARIO_ANIM_FORWARD_SPINNING, 0.01f);
    obj_anim_set_frame(m->marioObj, 0);
    omm_act_roll_update_gfx(m);
    omm_act_roll_audio_and_particles(m, prevAngle);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_spin_ground(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, 0, 0);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_JUMP, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_MOVE_PUNCHING, 0, RETURN_CANCEL);
    action_off_floor(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_FREEFALL, 0, RETURN_CANCEL);
    action_condition(gOmmMario->spin.timer == 0, ACT_IDLE, 0, RETURN_CANCEL);

    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_SPIN_AIR, 0, RETURN_BREAK);

    mario_set_forward_vel(m, max_f(0, m->forwardVel - 1.f));
    ANM(MARIO_ANIM_TWIRL, 1.f);
    SFX(SOUND_MOVING_TERRAIN_SLIDE + m->terrainSoundAddend);
    gOmmMario->spin.yaw += min_s(0x31E9, 0x280 * gOmmMario->spin.timer);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmMario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    PFX(PARTICLE_DUST);
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

////////////
// Moving //
////////////

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
    if (omm_mario_is_dead(m) && !(m->action & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE))) {
        drop_and_set_mario_action(m, ACT_STANDING_DEATH, 0);
        return true;
    }

    return false;
}

s32 omm_mario_execute_moving_action(struct MarioState *m) {
    gOmmMario->wallSlide.jumped = false;
    gOmmMario->peach.floated = false;
    gOmmMario->cappy.bounced = false;
    gOmmMario->state.airCombo = 0;
    gOmmMario->midairSpin.counter = 0;

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
        PFX(PARTICLE_WAVE_TRAIL);
        m->particleFlags &= ~PARTICLE_DUST;
    }

    // Actions
    switch (m->action) {

        // SM64
        case ACT_WALKING:                   return omm_act_walking(m);
        case ACT_HOLD_WALKING:              return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_TURNING_AROUND:            return omm_act_moving_cancels__cappy_roll_punch_spin(m);
        case ACT_FINISH_TURNING_AROUND:     return omm_act_moving_cancels__cappy_roll_punch_spin(m);
        case ACT_BRAKING:                   return omm_act_decelerating(m);
        case ACT_RIDING_SHELL_GROUND:       return omm_act_riding_shell_ground(m);
        case ACT_HOLD_HEAVY_WALKING:        return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_CRAWLING:                  return omm_act_crouching_crawling(m);
        case ACT_BURNING_GROUND:            return omm_act_burning_ground(m);
        case ACT_DECELERATING:              return omm_act_decelerating(m);
        case ACT_HOLD_DECELERATING:         return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_BEGIN_SLIDING:             return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLD_BEGIN_SLIDING:        return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_BUTT_SLIDE:                return omm_act_slide(m);
        case ACT_STOMACH_SLIDE:             return omm_act_slide(m);
        case ACT_HOLD_BUTT_SLIDE:           return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLD_STOMACH_SLIDE:        return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_DIVE_SLIDE:                return omm_act_slide(m);
        case ACT_MOVE_PUNCHING:             return omm_act_move_punching(m);
        case ACT_CROUCH_SLIDE:              return omm_act_crouching_crawling(m);
        case ACT_SLIDE_KICK_SLIDE:          return omm_act_slide(m);
        case ACT_HARD_BACKWARD_GROUND_KB:   return omm_act_knockback_ground(m, MARIO_ANIM_FALL_OVER_BACKWARDS, 55, true);
        case ACT_HARD_FORWARD_GROUND_KB:    return omm_act_knockback_ground(m, MARIO_ANIM_LAND_ON_STOMACH, 0, true);
        case ACT_BACKWARD_GROUND_KB:        return omm_act_knockback_ground(m, MARIO_ANIM_BACKWARD_KB, 0, true);
        case ACT_FORWARD_GROUND_KB:         return omm_act_knockback_ground(m, MARIO_ANIM_FORWARD_KB, 0, true);
        case ACT_SOFT_BACKWARD_GROUND_KB:   return omm_act_knockback_ground(m, MARIO_ANIM_SOFT_BACK_KB, 0, false);
        case ACT_SOFT_FORWARD_GROUND_KB:    return omm_act_knockback_ground(m, MARIO_ANIM_SOFT_FRONT_KB, 0, false);
        case ACT_GROUND_BONK:               return omm_act_knockback_ground(m, MARIO_ANIM_GROUND_BONK, 25, true);
        case ACT_DEATH_EXIT_LAND:           return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_JUMP_LAND:                 return omm_act_moving_cancels__cappy_roll_punch_spin(m);
        case ACT_FREEFALL_LAND:             return omm_act_moving_cancels__cappy_roll_punch_spin(m);
        case ACT_DOUBLE_JUMP_LAND:          return omm_act_moving_cancels__cappy_roll_punch_spin(m);
        case ACT_SIDE_FLIP_LAND:            return omm_act_moving_cancels__cappy_roll_punch_spin(m);
        case ACT_HOLD_JUMP_LAND:            return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLD_FREEFALL_LAND:        return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_QUICKSAND_JUMP_LAND:       return omm_act_moving_cancels__cappy_roll_punch_spin(m);
        case ACT_HOLD_QUICKSAND_JUMP_LAND:  return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_TRIPLE_JUMP_LAND:          return omm_act_moving_cancels__cappy_roll_punch_spin(m);
        case ACT_LONG_JUMP_LAND:            return omm_act_long_jump_land(m);
        case ACT_BACKFLIP_LAND:             return omm_act_moving_cancels__cappy_roll_punch_spin(m);
#if OMM_GAME_IS_R96X
        case ACT_WARIO_CHARGE:              return omm_act_wario_charge(m);
#endif

        // Odyssey
        case ACT_OMM_ROLL:                  return omm_act_roll(m);
        case ACT_OMM_SPIN_GROUND:           return omm_act_spin_ground(m);
        case ACT_OMM_CAPPY_THROW_GROUND:    return omm_act_cappy_throw_ground(m);

        // Peach
        case ACT_OMM_PEACH_ATTACK_GROUND:   return omm_act_peach_attack_ground(m);
        case ACT_OMM_PEACH_ATTACK_FAST:     return omm_act_peach_attack_fast(m);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

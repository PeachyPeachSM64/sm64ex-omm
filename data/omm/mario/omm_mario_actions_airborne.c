#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_common_air_knockback_step(struct MarioState *m, u32 landAction, s32 animID, bool isForward) {
    mario_set_forward_vel(m, m->forwardVel * 0.95f);
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE:
            obj_anim_play(m->marioObj, animID, 1.f);
            if (m->actionTimer++ >= 30 + (15 * isForward)) {
                obj_anim_play(m->marioObj, MARIO_ANIM_GENERAL_FALL, 1.f);
                omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
            }
            break;

        case AIR_STEP_LANDED:
            omm_mario_set_action(m, landAction, m->actionArg, 0);
            break;

        case AIR_STEP_HIT_WALL:
            obj_anim_play(m->marioObj, MARIO_ANIM_BACKWARD_AIR_KB, 1.f);
            mario_bonk_reflection(m, FALSE);
            mario_set_forward_vel(m, -m->forwardVel);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }
}

static void omm_play_knockback_sound(struct MarioState *m) {
    if (random_u16() & 1) {
        play_sound_if_no_flag(m, SOUND_MARIO_DOH, MARIO_MARIO_SOUND_PLAYED);
    } else {
        play_sound_if_no_flag(m, SOUND_MARIO_UH, MARIO_MARIO_SOUND_PLAYED);
    }
}

static f32 omm_get_initial_upwards_velocity(struct MarioState *m, f32 max) {
    static const s32 sAirActions[][3] = {
        { ACT_DIVE,                     -1, 0 },
        { ACT_JUMP_KICK,                +1, 1 },
        { ACT_GROUND_POUND,              0, 0 },
        { ACT_OMM_SPIN_AIR,             +1, 1 },
        { ACT_OMM_SPIN_POUND,            0, 0 },
        { ACT_OMM_MIDAIR_SPIN,           0, 1 },
        { ACT_OMM_CAPPY_BOUNCE,         -1, 0 },
        { ACT_OMM_CAPPY_THROW_AIRBORNE, +1, 1 },
    };
    for (s32 i = 0; i != OMM_ARRAY_SIZE(sAirActions); ++i) {
        if (m->action == (u32) sAirActions[i][0]) {
            gOmmData->mario->state.airCombo += sAirActions[i][1];
            if (sAirActions[i][2]) {
                return max * clamp_0_1_f(1.f - (((gOmmData->mario->state.airCombo) - 5) / 5.f));
            } else {
                return max;
            }
        }
    }
    gOmmData->mario->state.airCombo = 0;
    return max;
}

static s16 omm_wall_slide_get_jump_angle(struct MarioState *m) {
    s16 wAngle = atan2s(m->wall->normal.z, m->wall->normal.x);
    s16 dAngle = m->faceAngle[1] - wAngle;
    if (m->controller->stickMag > 32.f) {
        dAngle = m->intendedYaw - wAngle;
        if (-0x4000 < dAngle && dAngle < +0x4000) {
            dAngle = 0x8000 - dAngle;
        }
        if (dAngle < 0) {
            dAngle = min_s(dAngle, -0x8000 + OMM_MARIO_WALL_SLIDE_JUMP_ANGLE_MAX);
        } else {
            dAngle = max_s(dAngle, +0x8000 - OMM_MARIO_WALL_SLIDE_JUMP_ANGLE_MAX);
        }
    }
    return (0x8000 + wAngle - dAngle);
}

static void omm_wall_slide_cancel(struct MarioState *m, f32 forwardVel, f32 upwardsVel, bool setJumpAngle) {
    if (setJumpAngle) {
        m->faceAngle[1] = omm_wall_slide_get_jump_angle(m);
    }
    mario_set_forward_vel(m, forwardVel);
    m->vel[1] = omm_get_initial_upwards_velocity(m, upwardsVel * omm_player_get_selected_jump_multiplier());
    gOmmData->mario->wallSlide.height = m->pos[1] - 200.f;
    gOmmData->mario->wallSlide.jumped = true;
    gOmmData->mario->midairSpin.counter = 0;
}

//
// Actions
//

static s32 omm_act_jump(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_double_jump(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_triple_jump(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_special_triple_jump(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY, ACT_TRIPLE_JUMP, 0, RETURN_CANCEL, gSpecialTripleJump = 0;);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_backflip(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_long_jump(struct MarioState *m) {
#if !OMM_GAME_IS_R96A
    if (OMM_CHEAT_BLJ_ANYWHERE_REQ && (m->controller->buttonPressed & A_BUTTON)) {
        m->vel[1] = -50.f;
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }
#endif
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY && !OMM_CHEAT_BLJ_ANYWHERE_REQ, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY && !OMM_CHEAT_BLJ_ANYWHERE_REQ, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_freefall(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_side_flip(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_wall_kick_air(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_water_jump(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_steep_jump(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY, ACT_JUMP, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_ground_pound(struct MarioState *m) {
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_DIVE, 0, RETURN_CANCEL);

    // Allows Mario to move during the ascending part of a ground pound
    // if he's above a ceiling if Odyssey Moveset is enabled.
    // Normally, Mario shouldn't move if he's inside (above) a ceiling,
    // but since OMM removes infinite height ceilings, this restriction
    // no longer has sense.
    if (OMM_MOVESET_ODYSSEY && m->actionState == 0 && m->actionTimer < 10) {
        f32 yOffset = 20 - 2 * m->actionTimer;
        if (m->pos[1] + yOffset + 160.f >= m->ceilHeight &&
            m->pos[1] + yOffset -  80.f >  m->ceilHeight) {
            m->ceilHeight = m->pos[1] + 300.f;
        }
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_dive(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) action_init(32.f, omm_get_initial_upwards_velocity(m, 40.f), PARTICLE_MIST_CIRCLE, 0);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_burning_jump(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        m->hurtCounter += (m->marioObj->oMarioBurnTimer == 0);
        action_condition(m->marioObj->oMarioBurnTimer > 160, ACT_FREEFALL, 0, RETURN_CANCEL);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_burning_fall(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        m->hurtCounter += (m->marioObj->oMarioBurnTimer == 0);
        action_condition(m->marioObj->oMarioBurnTimer > 160, ACT_FREEFALL, 0, RETURN_CANCEL);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_jump_kick(struct MarioState *m) {
    if (m->actionState == 0) {
        if (OMM_MOVESET_ODYSSEY && OMM_PLAYER_IS_PEACH) {
            mario_set_forward_vel(m, m->forwardVel / 1.5f);
            m->vel[1] = omm_get_initial_upwards_velocity(m, 24.f * omm_player_get_selected_jump_multiplier());
        } else {
            s32 rainbowSpin = OMM_MOVESET_ODYSSEY && omm_cappy_get_object();
            mario_set_forward_vel(m, m->forwardVel / (1.f + rainbowSpin));
            m->vel[1] = omm_get_initial_upwards_velocity(m, (20.f + 10.f * rainbowSpin) * omm_player_get_selected_jump_multiplier());
            m->actionArg = rainbowSpin;
        }
        m->actionState = 1;
        m->actionTimer = 0;
        m->marioObj->oAnimID = -1;
        play_sound(SOUND_MARIO_PUNCH_HOO, m->marioObj->oCameraToObject);
    }

    // Cancels
    action_condition(OMM_MOVESET_ODYSSEY && OMM_PLAYER_IS_PEACH, ACT_OMM_PEACH_ATTACK_AIR, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(omm_mario_has_wing_cap(m), ACT_FLYING, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY && (m->actionTimer >= 16), ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    m->controller->buttonPressed &= ~B_BUTTON;
    m->actionTimer++;

    // Vanilla
    if (OMM_MOVESET_CLASSIC && !m->actionArg) {
        obj_anim_play(m->marioObj, MARIO_ANIM_AIR_KICK, 1.f);
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    // Step
    omm_mario_update_air_without_turn(m);
    s32 step = perform_air_step(m, OMM_MOVESET_ODYSSEY * AIR_STEP_CHECK_LEDGE_GRAB);
    action_condition(step == AIR_STEP_LANDED, ACT_FREEFALL_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_GRABBED_LEDGE, ACT_LEDGE_GRAB, 0, RETURN_BREAK, obj_anim_play(m->marioObj, MARIO_ANIM_IDLE_ON_LEDGE, 1.f););
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_check_and_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);

    // Cappy twirl
    if (m->actionArg) {
        if (OMM_EXTRAS_SMO_ANIMATIONS) {
            obj_anim_play(m->marioObj, MARIO_ANIM_OMM_CAPPY_RAINBOW_SPIN, 1.f);
            m->flags |= MARIO_KICKING * (obj_anim_get_frame(m->marioObj) < 20);
        } else {
            obj_anim_play(m->marioObj, MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN, 1.5f);
            obj_anim_clamp_frame(m->marioObj, 74, 94);
            m->flags |= MARIO_KICKING * (obj_anim_get_frame(m->marioObj) < 94);
        }
        m->marioBodyState->punchState = 0;
    }

    // Kick
    else {
        obj_anim_play(m->marioObj, MARIO_ANIM_AIR_KICK, 1.f);
        if (obj_anim_is_past_frame(m->marioObj, 0)) m->marioBodyState->punchState = (2 << 6) | 6;
        m->flags |= MARIO_KICKING * (obj_anim_get_frame(m->marioObj) < 8);
    }

    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_flying_triple_jump(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        m->input &= ~INPUT_B_PRESSED;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_top_of_pole_jump(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_twirling(struct MarioState *m) {
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_POUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_backward_air_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    action_init(-20.f, 20.f, 0, 0);
    omm_play_knockback_sound(m);
    omm_common_air_knockback_step(m, ACT_BACKWARD_GROUND_KB, MARIO_ANIM_BACKWARD_AIR_KB, false);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_forward_air_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    action_init(+20.f, 20.f, 0, 0);
    omm_play_knockback_sound(m);
    omm_common_air_knockback_step(m, ACT_FORWARD_GROUND_KB, MARIO_ANIM_AIR_FORWARD_KB, true);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_hard_backward_air_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    action_init(-20.f, 20.f, 0, 0);
    omm_play_knockback_sound(m);
    omm_common_air_knockback_step(m, ACT_HARD_BACKWARD_GROUND_KB, MARIO_ANIM_BACKWARD_AIR_KB, false);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_hard_forward_air_kb(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    action_init(+20.f, 20.f, 0, 0);
    omm_play_knockback_sound(m);
    omm_common_air_knockback_step(m, ACT_HARD_FORWARD_GROUND_KB, MARIO_ANIM_AIR_FORWARD_KB, true);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_soft_bonk(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    // Soft bonks from ledge grabs are shorter
    if (m->prevAction == ACT_LEDGE_GRAB ||
        m->prevAction == ACT_LEDGE_CLIMB_SLOW_1 ||
        m->prevAction == ACT_LEDGE_CLIMB_SLOW_2 ||
        m->prevAction == ACT_LEDGE_CLIMB_DOWN ||
        m->prevAction == ACT_LEDGE_CLIMB_FAST) {
        m->actionTimer = max_s(m->actionTimer, 15);
    }
    action_init(m->forwardVel, 0.f, 0, 0);
    omm_play_knockback_sound(m);
    omm_common_air_knockback_step(m, ACT_FREEFALL_LAND, MARIO_ANIM_GENERAL_FALL, false);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_air_hit_wall(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || m->health <= OMM_HEALTH_ODYSSEY_DEAD) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    mario_drop_held_object(m);
    if (m->forwardVel >= 32.f) {
        m->particleFlags |= PARTICLE_VERTICAL_STAR;
        omm_mario_set_action(m, ACT_BACKWARD_AIR_KB, 0, 0);
    } else {
        mario_set_forward_vel(m, -m->forwardVel);
        omm_mario_set_action(m, ACT_SOFT_BONK, 0, 0);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_forward_rollout(struct MarioState *m) {
    action_init(m->forwardVel, 30.f, 0, SOUND_ACTION_TERRAIN_JUMP, m->marioObj->oAnimID = -1;);

    // Spin
    if (m->actionState == 1) {
        obj_anim_play(m->marioObj, MARIO_ANIM_FORWARD_SPINNING, 1.f);
        if (m->actionTimer++ == 4) play_sound(SOUND_ACTION_SPIN, m->marioObj->oCameraToObject);
        if (obj_anim_is_at_end(m->marioObj)) m->actionState = 2;
    }

    // Fall
    if (m->actionState == 2) {
        obj_anim_play(m->marioObj, MARIO_ANIM_GENERAL_FALL, 1.f);
    }

    // Step
    omm_mario_update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_NONE && OMM_MOVESET_ODYSSEY && m->actionState == 2, ACT_FREEFALL, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_LANDED, ACT_FREEFALL_LAND_STOP, 0, RETURN_BREAK, SFX(SOUND_ACTION_TERRAIN_LANDING););
    action_condition(step == AIR_STEP_HIT_WALL, 0, 0, RETURN_BREAK, mario_set_forward_vel(m, 0.f););
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_backward_rollout(struct MarioState *m) {
    action_init(m->forwardVel, 30.f, 0, SOUND_ACTION_TERRAIN_JUMP, m->marioObj->oAnimID = -1;);

    // Spin
    if (m->actionState == 1) {
        obj_anim_play(m->marioObj, MARIO_ANIM_BACKWARD_SPINNING, 1.f);
        if (m->actionTimer++ == 4) play_sound(SOUND_ACTION_SPIN, m->marioObj->oCameraToObject);
        if (obj_anim_is_past_frame(m->marioObj, 2)) m->actionState = 2;
    }

    // Fall
    if (m->actionState == 2) {
        obj_anim_play(m->marioObj, MARIO_ANIM_GENERAL_FALL, 1.f);
    }

    // Step
    omm_mario_update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_NONE && OMM_MOVESET_ODYSSEY && m->actionState == 2, ACT_FREEFALL, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_LANDED, ACT_FREEFALL_LAND_STOP, 0, RETURN_BREAK, SFX(SOUND_ACTION_TERRAIN_LANDING););
    action_condition(step == AIR_STEP_HIT_WALL, 0, 0, RETURN_BREAK, mario_set_forward_vel(m, 0.f););
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_vertical_wind(struct MarioState *m) {
    action_a_pressed(OMM_PLAYER_IS_PEACH, ACT_OMM_PEACH_GLIDE, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_lava_boost(UNUSED struct MarioState *m) {
    gOmmData->mario->cappy.bounced = false;
    gOmmData->mario->state.airCombo = 0;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_getting_blown(struct MarioState *m) {
    m->actionTimer = 0; // Mario's cap sticks to his head
    action_condition(OMM_MOVESET_ODYSSEY && ++m->actionArg > 60, ACT_FREEFALL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_thrown_forward(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY && ++m->actionTimer > 45, ACT_FREEFALL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_thrown_backward(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY && ++m->actionTimer > 45, ACT_FREEFALL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

#if OMM_GAME_IS_R96A
static s32 omm_act_wario_triple_jump(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}
#endif

static s32 omm_act_wall_slide(struct MarioState *m) {
    action_condition(!m->wall, ACT_FREEFALL, 0, RETURN_CANCEL, SFX(SOUND_MARIO_UH););
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_WALL_KICK_AIR, 0, RETURN_CANCEL, omm_wall_slide_cancel(m, 24.f, 52.f, 1););
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_FREEFALL, 0, RETURN_CANCEL, omm_wall_slide_cancel(m, -8.f, 0.f, 0); SFX(SOUND_MARIO_UH););
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL, omm_wall_slide_cancel(m, 8.f, 0.f, 1););

    mario_set_forward_vel(m, 0);
    obj_anim_play(m->marioObj, MARIO_ANIM_START_WALLKICK, 1.f);
    play_sound(SOUND_MOVING_TERRAIN_SLIDE + m->terrainSoundAddend, m->marioObj->oCameraToObject);
    m->particleFlags |= PARTICLE_DUST;

    // Cling Mario to the wall before performing the air step,
    // to avoid missing slightly slanted walls (normal.y near 0, but not 0)
    m->pos[0] -= m->wall->normal.x * 4.f;
    m->pos[2] -= m->wall->normal.z * 4.f;
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_IDLE, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_NONE, ACT_FREEFALL, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL && !omm_mario_check_wall_slide(m), ACT_FREEFALL, 0, RETURN_BREAK);

    // Turn Mario away from the wall
    m->marioObj->oGfxAngle[1] = atan2s(m->wall->normal.z, m->wall->normal.x);
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_cappy_bounce(struct MarioState *m) {
    action_init(m->forwardVel * 0.8f, omm_get_initial_upwards_velocity(m, 56.f), 0, SOUND_MARIO_HOOHOO);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    action_condition(common_air_action_step(m, ACT_DOUBLE_JUMP_LAND, (m->vel[1] >= 0.f ? MARIO_ANIM_DOUBLE_JUMP_RISE : MARIO_ANIM_DOUBLE_JUMP_FALL), AIR_STEP_CHECK_LEDGE_GRAB) != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_ground_cappy_bounce(struct MarioState *m) {
    action_init(m->forwardVel * 0.8f, 68.f, 0, SOUND_MARIO_YAHOO_WAHA_YIPPEE + ((random_u16() % 5) << 16));
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    if (OMM_EXTRAS_SMO_ANIMATIONS) {
        action_condition(common_air_action_step(m, ACT_DOUBLE_JUMP_LAND, MARIO_ANIM_OMM_CAPPY_VAULT, AIR_STEP_CHECK_LEDGE_GRAB) != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
        if (obj_anim_is_past_frame(m->marioObj, 6)) play_sound(SOUND_ACTION_SIDE_FLIP_UNK, m->marioObj->oCameraToObject);
    } else {
        action_condition(common_air_action_step(m, ACT_DOUBLE_JUMP_LAND, MARIO_ANIM_TRIPLE_JUMP, AIR_STEP_CHECK_LEDGE_GRAB) != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
        play_flip_sounds(m, 2, 8, 20);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_ground_pound_jump(struct MarioState *m) {
    action_init(0, 66.f, 0, SOUND_MARIO_YAHOO, gOmmData->mario->spin.yaw = 1;);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);

    if (m->vel[1] >= 42.f) {
        action_condition(perform_air_step(m, 0) == AIR_STEP_LANDED, ACT_JUMP_LAND, 0, RETURN_BREAK);
    } else {
        action_condition(common_air_action_step(m, ACT_JUMP_LAND, MARIO_ANIM_SINGLE_JUMP, AIR_STEP_CHECK_LEDGE_GRAB) != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
    }

    obj_anim_play(m->marioObj, MARIO_ANIM_SINGLE_JUMP, 1.f);
    obj_anim_clamp_frame(m->marioObj, 0, 9); // Luigi's freaking flutter jump (again)
    s16 prevSpinYaw = gOmmData->mario->spin.yaw;
    gOmmData->mario->spin.yaw += (0x80 * m->vel[1]) * (prevSpinYaw != 0) * (omm_mario_has_vanish_cap(m) ? 0.8f : 1.f) / sqr_f(omm_player_get_selected_jump_multiplier());
    gOmmData->mario->spin.yaw *= ((u16) prevSpinYaw < (u16) gOmmData->mario->spin.yaw) * (m->vel[1] > 0.f);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] + gOmmData->mario->spin.yaw;
    m->particleFlags |= (m->vel[1] > 0.f ? PARTICLE_DUST : 0);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_leave_object_jump(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    action_condition(common_air_action_step(m, ACT_JUMP_LAND, MARIO_ANIM_SINGLE_JUMP, AIR_STEP_CHECK_LEDGE_GRAB) != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
    m->particleFlags |= PARTICLE_SPARKLES;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_cappy_throw_airborne(struct MarioState *m) {
    action_init(min_f(m->forwardVel, 8.f), omm_get_initial_upwards_velocity(m, 16.f), 0, 0);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    action_condition(common_air_action_step(m, ACT_FREEFALL_LAND, m->marioObj->oAnimID, 0) != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_roll_air(struct MarioState *m) {
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_condition((m->forwardVel < 45.f) && !(m->controller->buttonDown & Z_TRIG), ACT_FREEFALL, 0, RETURN_CANCEL, obj_anim_play(m->marioObj, MARIO_ANIM_GENERAL_FALL, 1.f););

    omm_mario_update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_OMM_ROLL, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL, ACT_BACKWARD_AIR_KB, 0, RETURN_BREAK, PFX(PARTICLE_VERTICAL_STAR););
    
    f32 speed = m->forwardVel / 60.f;
    s16 prevAngle = m->faceAngle[0];
    m->faceAngle[0] += (s16) (0x1C00 * speed);
    obj_anim_play(m->marioObj, MARIO_ANIM_FORWARD_SPINNING, 1.f);
    obj_anim_set_frame(m->marioObj, 0);
    Vec3f v = { 0, -60, -10 };
    vec3f_rotate_zxy(v, v, m->faceAngle[0], m->faceAngle[1], 0);
    m->marioObj->oGfxPos[0] += v[0];
    m->marioObj->oGfxPos[1] += v[1] + 50;
    m->marioObj->oGfxPos[2] += v[2];
    m->marioObj->oGfxAngle[0] = m->faceAngle[0];
    obj_set_shadow_pos_to_object_pos(gMarioObject);
    if (prevAngle > m->faceAngle[0]) play_sound(SOUND_ACTION_TWIRL, m->marioObj->oCameraToObject);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_spin_air(struct MarioState *m) {
    action_init(m->forwardVel, omm_get_initial_upwards_velocity(m, 12.f), 0, 0);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_condition(gOmmData->mario->spin.timer == 0, ACT_FREEFALL, 0, RETURN_CANCEL);

    omm_mario_update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_JUMP_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_check_wall_slide(m) && omm_mario_can_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK);

    obj_anim_play(m->marioObj, MARIO_ANIM_TWIRL, 1.f);
    s16 prevSpinYaw = gOmmData->mario->spin.yaw;
    gOmmData->mario->spin.yaw += min_s(0x4714, 0x400 * gOmmData->mario->spin.timer);
    if (gOmmData->mario->spin.yaw < prevSpinYaw) play_sound(SOUND_ACTION_SPIN, m->marioObj->oCameraToObject);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmData->mario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_spin_jump(struct MarioState *m) {
    action_init(m->forwardVel, 45.f, 0, SOUND_MARIO_YAHOO_WAHA_YIPPEE + ((2 + (random_u16() % 3)) << 16));
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);

    omm_mario_update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_JUMP_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(m->vel[1] <= 0.f, ACT_FREEFALL, 0, RETURN_BREAK);

    obj_anim_play(m->marioObj, MARIO_ANIM_TWIRL, 1.f);
    s16 prevSpinYaw = gOmmData->mario->spin.yaw;
    gOmmData->mario->spin.yaw += min_s(0x5EDC, 0x400 * m->vel[1]);
    if (gOmmData->mario->spin.yaw < prevSpinYaw) play_sound(SOUND_ACTION_SPIN, m->marioObj->oCameraToObject);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmData->mario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    m->particleFlags |= PARTICLE_SPARKLES;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_spin_pound(struct MarioState *m) {
    action_init(0.f, -60.f, 0, 0);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_DIVE, 0, RETURN_CANCEL);

    mario_set_forward_vel(m, 0);
    s32 step = perform_air_step(m, 0);
    s32 soundBits = (((m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_HEAVY_LANDING : SOUND_ACTION_TERRAIN_HEAVY_LANDING) | 0x0000FF00);
    u32 particles = (PARTICLE_MIST_CIRCLE | PARTICLE_HORIZONTAL_STAR);
    action_condition(step == AIR_STEP_LANDED, ACT_OMM_SPIN_POUND_LAND, 0, RETURN_BREAK, SFX(soundBits); PFX(particles); set_camera_shake_from_hit(SHAKE_GROUND_POUND););
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);

    obj_anim_play(m->marioObj, MARIO_ANIM_TWIRL, 1.f);
    s16 prevSpinYaw = gOmmData->mario->spin.yaw;
    gOmmData->mario->spin.yaw += 0x4000;
    if (gOmmData->mario->spin.yaw < prevSpinYaw) play_sound(SOUND_ACTION_SIDE_FLIP_UNK, m->marioObj->oCameraToObject);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmData->mario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    m->particleFlags |= PARTICLE_DUST;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_midair_spin(struct MarioState *m) {
    action_init(m->forwardVel, omm_get_initial_upwards_velocity(m, 0.f), 0, 0,
        m->actionTimer = 0;
        gOmmData->mario->midairSpin.timer = 24;
        gOmmData->mario->midairSpin.counter++;
        omm_sound_play(OMM_SOUND_EFFECT_MIDAIR_SPIN, m->marioObj->oCameraToObject);
    );
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);

    omm_mario_update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_JUMP_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_check_wall_slide(m) && omm_mario_can_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK);

    if (m->actionTimer == 8) {
        if (m->prevAction == ACT_DOUBLE_JUMP) {
            obj_anim_play(m->marioObj, MARIO_ANIM_DOUBLE_JUMP_FALL, 1.f);
            m->prevAction = m->action;
            m->action = ACT_DOUBLE_JUMP;
            m->actionArg = 0;
            m->actionState = 1;
            m->actionTimer = 1;
            m->flags |= (MARIO_ACTION_SOUND_PLAYED | MARIO_MARIO_SOUND_PLAYED);
        } else {
            obj_anim_play(m->marioObj, MARIO_ANIM_GENERAL_FALL, 1.f);
            omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
        }
        return OMM_MARIO_ACTION_RESULT_BREAK;
    } else {
        obj_anim_play(m->marioObj, MARIO_ANIM_TWIRL, 1.f);
        m->marioObj->oGfxAngle[1] = m->faceAngle[1] - (s16) (++m->actionTimer * 0x2000);
        m->marioBodyState->handState = MARIO_HAND_FISTS;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static void omm_set_flying_camera(struct MarioState *m) {
    if (m->area->camera->mode != CAMERA_MODE_BEHIND_MARIO) {
        if (BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM) {
            m->area->camera->mode = BETTER_CAM_MODE;
            gLakituState.mode = BETTER_CAM_MODE;
        } else {
            set_camera_mode(m->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
        }
    }
}

static void omm_set_default_camera(struct MarioState *m) {
    if (m->area->camera->mode == CAMERA_MODE_BEHIND_MARIO) {
        if (BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM) {
            m->area->camera->mode = BETTER_CAM_MODE;
            gLakituState.mode = BETTER_CAM_MODE;
        } else {
            set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
        }
    }
}

#define OMM_WING_FLYING_VEL_MAX 56.f
static void omm_update_flying(struct MarioState *m) {

    // Angles
    s32 targetPitch = (s32) ((m->controller->stickY / 64.f) * -0x3000);
    s32 targetYaw   = (s32) ((m->controller->stickX / 64.f) * -0x0200) + m->faceAngle[1];
    s32 targetRoll  = (s32) ((m->controller->stickX / 64.f) * +0x3000);
    m->faceAngle[0] = targetPitch - approach_s32((s16) (targetPitch - m->faceAngle[0]), 0, 400, 400);
    m->faceAngle[1] = targetYaw;
    m->faceAngle[2] = targetRoll  - approach_s32((s16) (targetRoll  - m->faceAngle[2]), 0, 800, 800);

    // Braking and targeting
    if (m->controller->buttonDown & B_BUTTON) {
        m->forwardVel = max_f(abs_f(m->forwardVel) - 2.f, 0.f);
        if (m->forwardVel != 0.f) {
            m->marioObj->oAnimID = -1;
            m->particleFlags |= PARTICLE_DUST;
        }
        m->faceAngle[2] *= (m->forwardVel / OMM_WING_FLYING_VEL_MAX);
        obj_anim_play(m->marioObj, MARIO_ANIM_FALL_FROM_SLIDE_KICK, 1.f);
    }
    
    // Flying
    else {
        m->forwardVel = min_f(abs_f(m->forwardVel) + 1.f, OMM_WING_FLYING_VEL_MAX * omm_player_get_selected_air_speed_multiplier());
        obj_anim_play(m->marioObj, MARIO_ANIM_FORWARD_SPINNING_FLIP, 1.f);
        if (obj_anim_is_past_frame(m->marioObj, 1)) {
            play_sound(SOUND_ACTION_SPIN, m->marioObj->oCameraToObject);
        }
    }

    // Velocity
    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
    m->slideVelX = m->vel[0];
    m->slideVelZ = m->vel[2];
}

static s32 omm_act_flying(struct MarioState *m) {
    action_cappy(OMM_POWER_UPS_CLASSIC || !(m->controller->buttonDown & B_BUTTON), 0, 0, NO_RETURN, SFX(SOUND_MARIO_YAHOO_WAHA_YIPPEE + ((2 + (random_u16() % 3)) << 16)););
    action_z_pressed(1, ACT_GROUND_POUND, 1, RETURN_CANCEL, omm_set_default_camera(m););
    action_condition(!(m->flags & MARIO_WING_CAP), ACT_FREEFALL, 0, RETURN_CANCEL, omm_set_default_camera(m););
    if (!omm_mario_has_wing_cap(m)) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    // Improved Wing cap
    omm_set_flying_camera(m);
    omm_update_flying(m);
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            m->faceAngle[0] = 0;
            omm_mario_set_action(m, ACT_DIVE_SLIDE, 0, 0);
            obj_anim_play(m->marioObj, MARIO_ANIM_DIVE, 1.f);
            obj_anim_set_frame(m->marioObj, 7);
            omm_set_default_camera(m);
            return OMM_MARIO_ACTION_RESULT_BREAK;

        case AIR_STEP_HIT_WALL:
            if (m->wall != NULL) {
                m->faceAngle[0] = 0;
                m->vel[1] = min_f(0.f, m->vel[1]);
                mario_set_forward_vel(m, -16.0f);
                play_sound((m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_BONK : SOUND_ACTION_BONK, m->marioObj->oCameraToObject);
                m->particleFlags |= PARTICLE_VERTICAL_STAR;
                omm_mario_set_action(m, ACT_BACKWARD_AIR_KB, 0, 0);
                omm_set_default_camera(m);
                return OMM_MARIO_ACTION_RESULT_BREAK;
            }
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            return OMM_MARIO_ACTION_RESULT_BREAK;
    }

    // Gfx
    m->marioObj->oGfxAngle[0] = -m->faceAngle[0];
    m->marioObj->oGfxAngle[2] = +m->faceAngle[2];

    // Sound
    play_sound(SOUND_MOVING_FLYING, m->marioObj->oCameraToObject);
    sound_set_moving_speed(1, 0x40 * (abs_f(m->forwardVel) / OMM_WING_FLYING_VEL_MAX));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

//
// Airborne
//

static bool omm_check_common_airborne_cancels(struct MarioState *m) {

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

    // Vertical wind
    if (m->floor->type == SURFACE_VERTICAL_WIND && (m->action & ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)) {
        if (m->action != ACT_OMM_PEACH_GLIDE) {
            drop_and_set_mario_action(m, ACT_VERTICAL_WIND, 0);
            return true;
        }
    }

    return false;
}

s32 omm_mario_execute_airborne_action(struct MarioState *m) {
    m->quicksandDepth = 0.0f;

    // Cancels
    if (omm_check_common_airborne_cancels(m)) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Actions
    switch (m->action) {
        case ACT_JUMP:                          return omm_act_jump(m);
        case ACT_DOUBLE_JUMP:                   return omm_act_double_jump(m);
        case ACT_TRIPLE_JUMP:                   return omm_act_triple_jump(m);
        case ACT_SPECIAL_TRIPLE_JUMP:           return omm_act_special_triple_jump(m);
        case ACT_BACKFLIP:                      return omm_act_backflip(m);
        case ACT_LONG_JUMP:                     return omm_act_long_jump(m);
        case ACT_FREEFALL:                      return omm_act_freefall(m);
        case ACT_SIDE_FLIP:                     return omm_act_side_flip(m);
        case ACT_WALL_KICK_AIR:                 return omm_act_wall_kick_air(m);
        case ACT_WATER_JUMP:                    return omm_act_water_jump(m);
        case ACT_STEEP_JUMP:                    return omm_act_steep_jump(m);
        case ACT_GROUND_POUND:                  return omm_act_ground_pound(m);
        case ACT_DIVE:                          return omm_act_dive(m);
        case ACT_BURNING_JUMP:                  return omm_act_burning_jump(m);
        case ACT_BURNING_FALL:                  return omm_act_burning_fall(m);
        case ACT_JUMP_KICK:                     return omm_act_jump_kick(m);
        case ACT_FLYING:                        return omm_act_flying(m);
        case ACT_FLYING_TRIPLE_JUMP:            return omm_act_flying_triple_jump(m);
        case ACT_TOP_OF_POLE_JUMP:              return omm_act_top_of_pole_jump(m);
        case ACT_TWIRLING:                      return omm_act_twirling(m);
        case ACT_BACKWARD_AIR_KB:               return omm_act_backward_air_kb(m);
        case ACT_FORWARD_AIR_KB:                return omm_act_forward_air_kb(m);
        case ACT_HARD_FORWARD_AIR_KB:           return omm_act_hard_forward_air_kb(m);
        case ACT_HARD_BACKWARD_AIR_KB:          return omm_act_hard_backward_air_kb(m);
        case ACT_SOFT_BONK:                     return omm_act_soft_bonk(m);
        case ACT_AIR_HIT_WALL:                  return omm_act_air_hit_wall(m);
        case ACT_FORWARD_ROLLOUT:               return omm_act_forward_rollout(m);
        case ACT_BACKWARD_ROLLOUT:              return omm_act_backward_rollout(m);
        case ACT_VERTICAL_WIND:                 return omm_act_vertical_wind(m);
        case ACT_LAVA_BOOST:                    return omm_act_lava_boost(m);
        case ACT_GETTING_BLOWN:                 return omm_act_getting_blown(m);
        case ACT_THROWN_FORWARD:                return omm_act_thrown_forward(m);
        case ACT_THROWN_BACKWARD:               return omm_act_thrown_backward(m);
#if OMM_GAME_IS_R96A
        case ACT_WARIO_TRIPLE_JUMP:             return omm_act_wario_triple_jump(m);
#endif

        // Odyssey
        case ACT_OMM_WALL_SLIDE:                return omm_act_wall_slide(m);
        case ACT_OMM_CAPPY_BOUNCE:              return omm_act_cappy_bounce(m);
        case ACT_OMM_GROUND_CAPPY_BOUNCE:       return omm_act_ground_cappy_bounce(m);
        case ACT_OMM_GROUND_POUND_JUMP:         return omm_act_ground_pound_jump(m);
        case ACT_OMM_LEAVE_OBJECT_JUMP:         return omm_act_leave_object_jump(m);
        case ACT_OMM_CAPPY_THROW_AIRBORNE:      return omm_act_cappy_throw_airborne(m);
        case ACT_OMM_ROLL_AIR:                  return omm_act_roll_air(m);
        case ACT_OMM_SPIN_AIR:                  return omm_act_spin_air(m);
        case ACT_OMM_SPIN_JUMP:                 return omm_act_spin_jump(m);
        case ACT_OMM_SPIN_POUND:                return omm_act_spin_pound(m);
        case ACT_OMM_MIDAIR_SPIN:               return omm_act_midair_spin(m);

        // Peach
        case ACT_OMM_PEACH_FLOAT:               return omm_act_peach_float(m);
        case ACT_OMM_PEACH_GLIDE:               return omm_act_peach_glide(m);
        case ACT_OMM_PEACH_ATTACK_AIR:          return omm_act_peach_attack_air(m);
        case ACT_OMM_PEACH_PERRY_CHARGE_AIR:    return omm_act_peach_perry_charge_air(m);
        case ACT_OMM_PEACH_VIBE_JOY_MOVE:       return omm_act_peach_vibe_joy_move(m);
        case ACT_OMM_PEACH_VIBE_JOY_FLY:        return omm_act_peach_vibe_joy_fly(m);
        case ACT_OMM_PEACH_VIBE_JOY_ATTACK:     return omm_act_peach_vibe_joy_attack(m);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

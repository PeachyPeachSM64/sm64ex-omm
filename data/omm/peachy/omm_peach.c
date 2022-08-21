#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_peach_set_animation_and_play_sound(struct MarioState *m, s32 animID, f32 animAccel, s32 sound) {
    if (m->marioObj->oAnimID != animID) {
        obj_play_sound(m->marioObj, sound);
    }
    obj_anim_play(m->marioObj, animID, animAccel);
}

static void omm_peach_play_random_attack_sound(struct MarioState *m) {
    static s32 prevSound = 0;
    s32 nextSound;
    do {
        switch (random_u16() % 3) {
            case 0: nextSound = SOUND_MARIO_PUNCH_WAH; break;
            case 1: nextSound = SOUND_MARIO_PUNCH_YAH; break;
            case 2: nextSound = SOUND_MARIO_PUNCH_HOO; break;
        }
    } while (nextSound == prevSound);
    play_sound(nextSound, m->marioObj->oCameraToObject); 
    prevSound = nextSound;
}

static void omm_peach_perry_charge_update_animation_and_sound(struct MarioState *m, s32 animID, f32 animAccel, s32 frameMin, s32 frameMax) {
    obj_anim_play(m->marioObj, animID, animAccel);
    obj_anim_set_frame(m->marioObj, frameMin + animAccel * (gOmmPerryCharge - OMM_PERRY_CHARGE_ACTION));
    obj_anim_clamp_frame(m->marioObj, frameMin, frameMax);
    m->actionTimer = max_s(0, gOmmPerryCharge - OMM_PERRY_CHARGE_ACTION);
    
    // Play the charge sound effect when Peach starts charging
    if (m->actionState == 1 && gOmmPerryCharge >= OMM_PERRY_CHARGE_START) {
        omm_sound_play(OMM_SOUND_EFFECT_PERRY_CHARGE, m->marioObj->oCameraToObject);
        m->actionState = 2;
    }
    
    // Play the charged sound when fully charged
    if (m->actionState == 2 && gOmmPerryCharge >= OMM_PERRY_CHARGE_FULL) {
        omm_sound_play(OMM_SOUND_EFFECT_PERRY_CHARGED, m->marioObj->oCameraToObject);
        m->actionState = 3;
    }
}

//
// Common actions
//

s32 omm_act_peach_float(struct MarioState *m) {
    if (m->actionState == 0) {
        action_condition(gOmmPeach->floated, ACT_OMM_PEACH_GLIDE, 0, RETURN_CANCEL);
        gOmmPeach->floatTimer = 0;
        gOmmPeach->floated = true;
        m->actionState = 1;
    }

    // Cancels
    action_condition(!OMM_PLAYER_IS_PEACH, ACT_FREEFALL, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    if (!(m->controller->buttonDown & A_BUTTON) || (gOmmPeach->floatTimer >= OMM_PEACH_FLOAT_DURATION)) {
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
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Step
    m->vel[1] = 0.f;
    omm_mario_update_air_with_turn(m);
    s32 step = perform_air_step(m, OMM_MOVESET_ODYSSEY * AIR_STEP_CHECK_LEDGE_GRAB);
    action_condition(step == AIR_STEP_LANDED, ACT_FREEFALL_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_GRABBED_LEDGE, ACT_LEDGE_GRAB, 0, RETURN_BREAK, obj_anim_play(m->marioObj, MARIO_ANIM_IDLE_ON_LEDGE, 1.f););
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_check_and_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);

    // Gfx
    if (obj_anim_is_at_end(m->marioObj)) m->marioObj->oAnimID = -1;
    obj_anim_play(m->marioObj, MARIO_ANIM_BEND_KNESS_RIDING_SHELL, 1.f);
    m->particleFlags |= PARTICLE_SPARKLES;
    gOmmPeach->floatTimer++;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

s32 omm_act_peach_glide(struct MarioState *m) {
    if (m->actionState == 0) {
        omm_sound_play(OMM_SOUND_EFFECT_PEACH_GLIDE, m->marioObj->oCameraToObject);
        obj_anim_play(m->marioObj, MARIO_ANIM_GRAB_POLE_SHORT, 2.f);
        m->actionState = 1;
    }

    // Cancels
    action_condition(!OMM_PLAYER_IS_PEACH, ACT_FREEFALL, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_condition(!(m->controller->buttonDown & A_BUTTON), ACT_FREEFALL, 0, RETURN_CANCEL);

    // Step
    omm_mario_update_air_with_turn(m);
    s32 step = perform_air_step(m, OMM_MOVESET_ODYSSEY * AIR_STEP_CHECK_LEDGE_GRAB);
    action_condition(step == AIR_STEP_LANDED, ACT_FREEFALL_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_GRABBED_LEDGE, ACT_LEDGE_GRAB, 0, RETURN_BREAK, obj_anim_play(m->marioObj, MARIO_ANIM_IDLE_ON_LEDGE, 1.f););
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);

    // Gfx
    if (++m->actionTimer > 6) obj_anim_play(m->marioObj, MARIO_ANIM_IDLE_ON_POLE, 1.f);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

s32 omm_act_peach_attack_ground(struct MarioState *m) {
    action_condition(!OMM_PLAYER_IS_PEACH, ACT_FREEFALL, 0, RETURN_CANCEL);
    action_condition((m->actionState == 0) && (m->controller->buttonDown & A_BUTTON), ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_init(((abs_f(m->forwardVel) < 12.f) ? 12.f : m->forwardVel), 0.f, 0, 0);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, ACT_JUMP, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_FREEFALL, 0, RETURN_CANCEL);
    action_condition((m->forwardVel > 28.f) && (m->controller->stickMag > 56.f), ACT_OMM_PEACH_ATTACK_FAST, 0, RETURN_CANCEL);

    // Spawn shockwave
    if (m->actionTimer == 0 && OMM_PEACH_SPAWN_PERRY_SHOCKWAVE) {
        omm_spawn_perry_shockwave(m->marioObj, 6 + 2 * (m->actionArg == 4), omm_perry_get_type(m), m->actionArg == 4);
    }

    // Update vel
    if (m->forwardVel > 0.f) {
        apply_slope_decel(m, 0.5f);
    } else {
        m->forwardVel = min_f(m->forwardVel + 8.f, 0.f);
        apply_slope_accel(m);
    }
    mario_set_forward_vel(m, m->forwardVel);

    // Step
    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_FREEFALL, 0, RETURN_BREAK);

    // Update attack sequence
    switch (m->actionArg) {

        // First attack (part 1)
        case 0: {
            omm_peach_set_animation_and_play_sound(m, MARIO_ANIM_FIRST_PUNCH, 1.f, SOUND_MARIO_PUNCH_WAH);
            action_condition(obj_anim_is_near_end(m->marioObj), 0, 0, NO_RETURN, m->actionArg = 1;);
            action_condition(mario_check_object_grab(m), 0, 0, RETURN_BREAK);
            m->actionState = 2;
        } break;

        // First attack (part 2)
        case 1: {
            omm_peach_set_animation_and_play_sound(m, MARIO_ANIM_FIRST_PUNCH_FAST, 1.f, 0);
            action_b_pressed(1, 0, 0, NO_RETURN, m->actionArg = 2;);
            action_condition(obj_anim_is_at_end(m->marioObj), ACT_IDLE, 0, RETURN_BREAK);
            m->actionState = 1 + (obj_anim_get_frame(m->marioObj) < 2);
        } break;

        // Second attack (part 1)
        case 2: {
            omm_peach_set_animation_and_play_sound(m, MARIO_ANIM_SECOND_PUNCH, 1.f, SOUND_MARIO_PUNCH_YAH);
            action_condition(obj_anim_is_near_end(m->marioObj), 0, 0, NO_RETURN, m->actionArg = 3;);
            m->actionState = 2;
        } break;

        // Second attack (part 2)
        case 3: {
            omm_peach_set_animation_and_play_sound(m, MARIO_ANIM_SECOND_PUNCH_FAST, 1.f, 0);
            action_b_pressed(1, 0, 0, NO_RETURN, m->actionArg = 4;);
            action_condition(obj_anim_is_at_end(m->marioObj), ACT_IDLE, 0, RETURN_BREAK);
            m->actionState = 1 + (obj_anim_get_frame(m->marioObj) < 2);
        } break;

        // Spin attack
        case 4: {
            if (OMM_EXTRAS_SMO_ANIMATIONS) {
                omm_peach_set_animation_and_play_sound(m, MARIO_ANIM_OMM_CAPPY_THROW, 1.5f, SOUND_MARIO_PUNCH_HOO);
                obj_anim_clamp_frame(m->marioObj, 0, 28);
                action_condition(obj_anim_is_past_frame(m->marioObj, 28), ACT_IDLE, 0, RETURN_BREAK);
                m->actionState = 1 + (obj_anim_get_frame(m->marioObj) <= 20);
            } else {
                omm_peach_set_animation_and_play_sound(m, MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN, 2.f, SOUND_MARIO_PUNCH_HOO);
                obj_anim_clamp_frame(m->marioObj, 70, 94);
                action_condition(obj_anim_is_past_frame(m->marioObj, 94), ACT_IDLE, 0, RETURN_BREAK);
                m->actionState = 2;
            }
        } break;
    }

    // Gfx
    m->actionTimer++;
    m->flags |= MARIO_PUNCHING;
    m->particleFlags |= PARTICLE_DUST * (abs_f(m->forwardVel) > 16.f);
    m->marioBodyState->punchState = 0;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

s32 omm_act_peach_attack_fast(struct MarioState *m) {
    static s32 sAttackTimer = 0;
    action_condition(!OMM_PLAYER_IS_PEACH, ACT_FREEFALL, 0, RETURN_CANCEL);
    action_init(m->forwardVel * 1.2f, 0.f, PARTICLE_HORIZONTAL_STAR, 0, sAttackTimer = 0;);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, ACT_JUMP, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_FREEFALL, 0, RETURN_CANCEL);

    // Spawn shockwave
    if (m->actionTimer == 0 && OMM_PEACH_SPAWN_PERRY_SHOCKWAVE) {
        omm_spawn_perry_shockwave(m->marioObj, 4, omm_perry_get_type(m), false);
    }

    // Step
    omm_mario_update_walking_speed(m);
    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_FREEFALL, 0, RETURN_BREAK);

    // Update attack sequence
    switch (m->actionArg) {

        // First attack (part 1)
        case 0: {
            if (sAttackTimer++ == 0) omm_peach_play_random_attack_sound(m);
            obj_anim_play(m->marioObj, MARIO_ANIM_FIRST_PUNCH, 1.f);
            action_condition(obj_anim_is_near_end(m->marioObj), 0, 0, NO_RETURN, m->actionArg = 1; sAttackTimer = 0;);
        } break;

        // First attack (part 2)
        case 1: {
            obj_anim_play(m->marioObj, MARIO_ANIM_FIRST_PUNCH_FAST, 1.f);
            action_condition(++sAttackTimer > 3, 0, 0, NO_RETURN, m->actionArg = 2; sAttackTimer = 0;);
        } break;

        // Second attack (part 1)
        case 2: {
            if (sAttackTimer++ == 0) omm_peach_play_random_attack_sound(m);
            obj_anim_play(m->marioObj, MARIO_ANIM_SECOND_PUNCH, 1.f);
            action_condition(obj_anim_is_near_end(m->marioObj), 0, 0, NO_RETURN, m->actionArg = 3; sAttackTimer = 0;);
        } break;

        // Second attack (part 2)
        case 3: {
            obj_anim_play(m->marioObj, MARIO_ANIM_SECOND_PUNCH_FAST, 1.f);
            action_condition(++sAttackTimer > 3, ACT_WALKING, 0, RETURN_BREAK);
        } break;
    }

    // Gfx
    m->actionTimer++;
    m->particleFlags |= PARTICLE_DUST;
    m->marioBodyState->punchState = 0;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

s32 omm_act_peach_attack_air(struct MarioState *m) {

    // Prevent the Perry charge cancel to trigger the flying cancel
    if (m->actionState != 2 && (m->controller->buttonPressed & B_BUTTON)) {
        m->controller->buttonPressed &= ~(B_BUTTON * !(m->controller->buttonDown & B_BUTTON));
        m->actionState = 2;
    }
    
    // Cancels
    action_condition(!OMM_PLAYER_IS_PEACH, ACT_FREEFALL, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(omm_mario_has_wing_cap(m), ACT_FLYING, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY && (m->actionTimer >= 16), ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    m->controller->buttonPressed &= ~B_BUTTON;

    // Spawn shockwave
    if (m->actionTimer == 0 && OMM_PEACH_SPAWN_PERRY_SHOCKWAVE) {
        omm_spawn_perry_shockwave(m->marioObj, 8, omm_perry_get_type(m), true);
    }

    // Step
    omm_mario_update_air_without_turn(m);
    s32 step = perform_air_step(m, OMM_MOVESET_ODYSSEY * AIR_STEP_CHECK_LEDGE_GRAB);
    action_condition(step == AIR_STEP_LANDED, ACT_FREEFALL_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_GRABBED_LEDGE, ACT_LEDGE_GRAB, 0, RETURN_BREAK, obj_anim_play(m->marioObj, MARIO_ANIM_IDLE_ON_LEDGE, 1.f););
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_check_and_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);

    // Gfx
    m->actionTimer++;
    if (OMM_EXTRAS_SMO_ANIMATIONS) {
        obj_anim_play(m->marioObj, MARIO_ANIM_OMM_CAPPY_THROW, 1.5f);
        obj_anim_clamp_frame(m->marioObj, 31, 127);
        m->flags |= MARIO_KICKING * (obj_anim_get_frame(m->marioObj) < 52);
    } else {
        obj_anim_play(m->marioObj, MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN, 2.f);
        obj_anim_clamp_frame(m->marioObj, 70, 94);
        m->flags |= MARIO_KICKING * (obj_anim_get_frame(m->marioObj) < 94);
    }
    m->marioBodyState->punchState = 0;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

s32 omm_act_peach_perry_charge_ground(struct MarioState *m) {
    if (m->actionArg) { m->actionState = m->actionArg; m->actionArg = 0; }
    action_init(m->forwardVel, 0.f, 0, SOUND_MARIO_UH2);
    action_cappy(OMM_PERRY_CHARGED, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_PERRY_CHARGED, ACT_JUMP, 0, RETURN_CANCEL);
    action_spin(OMM_PERRY_CHARGED, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    action_moving(OMM_PERRY_CHARGED, ACT_WALKING, 0, RETURN_CANCEL);
    action_condition(m->floor->normal.y < 0.3f, 0, 0, RETURN_CANCEL, mario_push_off_steep_floor(m, ACT_FREEFALL, 0););
    action_condition(m->input & INPUT_BOUNCE, ACT_SHOCKWAVE_BOUNCE, 0, RETURN_CANCEL);
    action_condition(m->input & INPUT_ABOVE_SLIDE, ACT_BEGIN_SLIDING, 0, RETURN_CANCEL);
    action_condition(m->input & INPUT_FIRST_PERSON, ACT_FIRST_PERSON, 0, RETURN_CANCEL);
    action_condition(!gOmmPerryCharge, ACT_IDLE, 0, RETURN_CANCEL);
    
    // Animation and sound
    omm_peach_perry_charge_update_animation_and_sound(m, MARIO_ANIM_SUMMON_STAR, 4.f, 18, 58);

    // Update vel
    if (m->forwardVel > 1.f) {
        apply_slope_decel(m, 1.f);
        m->particleFlags |= PARTICLE_DUST;
    } else if (m->forwardVel < 0.f) {
        m->forwardVel = min_f(m->forwardVel + 8.f, 0.f);
        apply_slope_accel(m);
        m->particleFlags |= PARTICLE_DUST;
    } else {
        m->forwardVel = 0.f;
    }
    mario_set_forward_vel(m, m->forwardVel);

    // Step
    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND && OMM_PERRY_CHARGED, ACT_FREEFALL, 0, RETURN_BREAK);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_PEACH_PERRY_CHARGE_AIR, m->actionState, RETURN_BREAK, omm_peach_perry_charge_update_animation_and_sound(m, MARIO_ANIM_SINGLE_JUMP, 1.f, 0, 10););
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

s32 omm_act_peach_perry_charge_air(struct MarioState *m) {
    if (m->actionArg) { m->actionState = m->actionArg; m->actionArg = 0; }
    action_init(m->forwardVel, m->vel[1], 0, SOUND_MARIO_UH2);
    action_cappy(OMM_PERRY_CHARGED, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_midair_spin(OMM_PERRY_CHARGED, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_PERRY_CHARGED, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    action_condition(!gOmmPerryCharge, ACT_FREEFALL, 0, RETURN_CANCEL);

    // Animation and sound
    omm_peach_perry_charge_update_animation_and_sound(m, MARIO_ANIM_SINGLE_JUMP, 1.f, 0, 10);

    // Step
    omm_mario_update_air_with_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED && OMM_PERRY_CHARGED, ACT_JUMP_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_LANDED, ACT_OMM_PEACH_PERRY_CHARGE_GROUND, m->actionState, RETURN_BREAK, omm_peach_perry_charge_update_animation_and_sound(m, MARIO_ANIM_SUMMON_STAR, 4.f, 18, 58););
    action_condition(step == AIR_STEP_HIT_WALL && OMM_PERRY_CHARGED && omm_mario_check_and_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

//
// Joy actions
//

static s32 omm_peach_vibe_joy_update(struct MarioState *m, s32 yawVel, bool fly) {

    // Update forward vel
    f32 fVelTarget = relerp_0_1_f(m->intendedMag, 0.f, 32.f, 0.f, 30.f);
    mario_set_forward_vel(m, approach_f32(m->forwardVel, fVelTarget, 1.5f, 3.f));

    // Update vertical vel
    f32 yVelInc = fly * clamp_f((31.f - m->vel[1]) / 2.f, 1.f, 4.f);
    m->vel[1] = approach_f32(m->vel[1], 30.f, yVelInc, sign_0_f(yVelInc));

    // Update angle
    s32 handling = (OMM_CHEAT_SUPER_RESPONSIVE ? 0x8000 : relerp_0_1_f(m->forwardVel, 8.f, 30.f, 0x2000, 0x800));
    m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, handling, handling);

    // Perform step
    s32 step = (!fly && m->pos[1] < find_floor_height(m->pos[0], m->pos[1], m->pos[2]) + 4.f) ? perform_ground_step(m) : perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED && fly, ACT_OMM_PEACH_VIBE_JOY_MOVE, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);

    // Update Gfx
    obj_anim_play(m->marioObj, MARIO_ANIM_TWIRL, 1.f);
    gOmmPeach->joySpinYaw += yawVel;
    gOmmMario->state.peakHeight = m->pos[1];
    m->peakHeight = m->pos[1];
    m->quicksandDepth = 0.f;
    m->particleFlags |= fly * PARTICLE_DUST;
    m->marioObj->oGfxAngle[1] = gOmmPeach->joySpinYaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

s32 omm_act_peach_vibe_joy_move(struct MarioState *m) {
    action_condition(!omm_peach_vibe_is_joy(), ACT_FREEFALL, 0, RETURN_CANCEL);
    action_a_pressed(1, ACT_OMM_PEACH_VIBE_JOY_FLY, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_PEACH_VIBE_JOY_ATTACK, 0, RETURN_CANCEL);
    return omm_peach_vibe_joy_update(m, 0x1000, false);
}

s32 omm_act_peach_vibe_joy_fly(struct MarioState *m) {
    action_init(m->forwardVel, m->vel[1], 0, SOUND_ACTION_INTRO_UNK45E);
    action_condition(!omm_peach_vibe_is_joy(), ACT_FREEFALL, 0, RETURN_CANCEL);
    action_condition(!(m->controller->buttonDown & A_BUTTON), ACT_OMM_PEACH_VIBE_JOY_MOVE, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_PEACH_VIBE_JOY_ATTACK, 0, RETURN_CANCEL);
    return omm_peach_vibe_joy_update(m, 0x2000, true);
}

s32 omm_act_peach_vibe_joy_attack(struct MarioState *m) {
    action_init(m->forwardVel, max_f(m->vel[1], 10.f), 0, SOUND_ACTION_FLYING_FAST, omm_spawn_peach_vibe_joy_gust(m->marioObj, 60.f, m->faceAngle[1]););
    action_condition(!omm_peach_vibe_is_joy(), ACT_FREEFALL, 0, RETURN_CANCEL);
    action_condition(m->actionTimer++ >= 12, ACT_OMM_PEACH_VIBE_JOY_MOVE, 0, RETURN_CANCEL);
    return omm_peach_vibe_joy_update(m, relerp_0_1_f(m->actionTimer, 8, 12, 0x3000, 0x1000), false);
}

//
// Charge
//

typedef struct {
    u32 action;
    bool charge;    // trigger a charge action if B is held
    bool charged;   // keep Perry charged as long as B is held
    bool release;   // attack as soon as B is released
    bool attack;    // is an attack action
} PerryChargeAction;

static PerryChargeAction sOmmPerryChargeActions[] = {
    { 0,                                 0, 0, 0, 0 },

    // Stationary
    { ACT_IDLE,                          1, 1, 1, 0 },
    { ACT_START_SLEEPING,                1, 1, 1, 0 },
    { ACT_SLEEPING,                      1, 1, 1, 0 },
    { ACT_WAKING_UP,                     1, 1, 1, 0 },
    { ACT_PANTING,                       1, 1, 1, 0 },
    { ACT_STANDING_AGAINST_WALL,         1, 1, 1, 0 },
    { ACT_COUGHING,                      1, 1, 1, 0 },
    { ACT_SHIVERING,                     1, 1, 1, 0 },
    { ACT_IN_QUICKSAND,                  1, 1, 1, 0 },
    { ACT_CROUCHING,                     1, 1, 1, 0 },
    { ACT_START_CROUCHING,               1, 1, 1, 0 },
    { ACT_STOP_CROUCHING,                1, 1, 1, 0 },
    { ACT_START_CRAWLING,                1, 1, 1, 0 },
    { ACT_STOP_CRAWLING,                 1, 1, 1, 0 },
    { ACT_SLIDE_KICK_SLIDE_STOP,         1, 1, 1, 0 },
    { ACT_BACKFLIP_LAND_STOP,            1, 1, 1, 0 },
    { ACT_JUMP_LAND_STOP,                1, 1, 1, 0 },
    { ACT_DOUBLE_JUMP_LAND_STOP,         1, 1, 1, 0 },
    { ACT_FREEFALL_LAND_STOP,            1, 1, 1, 0 },
    { ACT_SIDE_FLIP_LAND_STOP,           1, 1, 1, 0 },
    { ACT_AIR_THROW_LAND,                1, 1, 1, 0 },
    { ACT_TWIRL_LAND,                    1, 1, 1, 0 },
    { ACT_LAVA_BOOST_LAND,               1, 1, 1, 0 },
    { ACT_TRIPLE_JUMP_LAND_STOP,         1, 1, 1, 0 },
    { ACT_LONG_JUMP_LAND_STOP,           1, 1, 1, 0 },
    { ACT_GROUND_POUND_LAND,             1, 1, 1, 0 },
    { ACT_BRAKING_STOP,                  1, 1, 1, 0 },
    { ACT_BUTT_SLIDE_STOP,               1, 1, 1, 0 },

    // Moving
    { ACT_WALKING,                       1, 1, 1, 0 },
    { ACT_TURNING_AROUND,                1, 1, 1, 0 },
    { ACT_FINISH_TURNING_AROUND,         1, 1, 1, 0 },
    { ACT_BRAKING,                       1, 1, 1, 0 },
    { ACT_CRAWLING,                      1, 1, 1, 0 },
    { ACT_DECELERATING,                  1, 1, 1, 0 },
    { ACT_BUTT_SLIDE,                    0, 1, 1, 0 },
    { ACT_STOMACH_SLIDE,                 0, 1, 0, 0 },
    { ACT_DIVE_SLIDE,                    0, 1, 0, 0 },
    { ACT_MOVE_PUNCHING,                 1, 1, 1, 0 },
    { ACT_CROUCH_SLIDE,                  1, 1, 1, 0 },
    { ACT_SLIDE_KICK_SLIDE,              0, 1, 0, 0 },
    { ACT_JUMP_LAND,                     1, 1, 1, 0 },
    { ACT_FREEFALL_LAND,                 1, 1, 1, 0 },
    { ACT_DOUBLE_JUMP_LAND,              1, 1, 1, 0 },
    { ACT_SIDE_FLIP_LAND,                1, 1, 1, 0 },
    { ACT_QUICKSAND_JUMP_LAND,           1, 1, 1, 0 },
    { ACT_TRIPLE_JUMP_LAND,              1, 1, 1, 0 },
    { ACT_LONG_JUMP_LAND,                1, 1, 1, 0 },
    { ACT_BACKFLIP_LAND,                 1, 1, 1, 0 },

    // Airborne
    { ACT_JUMP,                          0, 1, 1, 0 },
    { ACT_DOUBLE_JUMP,                   0, 1, 1, 0 },
    { ACT_TRIPLE_JUMP,                   0, 1, 1, 0 },
    { ACT_BACKFLIP,                      0, 1, 1, 0 },
    { ACT_STEEP_JUMP,                    0, 1, 1, 0 },
    { ACT_WALL_KICK_AIR,                 0, 1, 1, 0 },
    { ACT_SIDE_FLIP,                     0, 1, 1, 0 },
    { ACT_LONG_JUMP,                     0, 1, 1, 0 },
    { ACT_WATER_JUMP,                    0, 1, 1, 0 },
    { ACT_DIVE,                          0, 1, 0, 0 },
    { ACT_FREEFALL,                      0, 1, 1, 0 },
    { ACT_TOP_OF_POLE_JUMP,              0, 1, 1, 0 },
    { ACT_BUTT_SLIDE_AIR,                0, 1, 1, 0 },
    { ACT_TWIRLING,                      0, 1, 0, 0 },
    { ACT_FORWARD_ROLLOUT,               0, 1, 1, 0 },
    { ACT_BACKWARD_ROLLOUT,              0, 1, 1, 0 },
    { ACT_RIDING_HOOT,                   0, 1, 0, 0 },
    { ACT_GROUND_POUND,                  0, 1, 0, 0 },
    { ACT_SLIDE_KICK,                    0, 1, 0, 0 },
    { ACT_JUMP_KICK,                     1, 1, 1, 1 },

    // Automatic
    { ACT_HOLDING_POLE,                  0, 1, 0, 0 },
    { ACT_GRAB_POLE_SLOW,                0, 1, 0, 0 },
    { ACT_GRAB_POLE_FAST,                0, 1, 0, 0 },
    { ACT_CLIMBING_POLE,                 0, 1, 0, 0 },
    { ACT_TOP_OF_POLE_TRANSITION,        0, 1, 0, 0 },
    { ACT_TOP_OF_POLE,                   0, 1, 0, 0 },
    { ACT_START_HANGING,                 0, 1, 0, 0 },
    { ACT_HANGING,                       0, 1, 0, 0 },
    { ACT_HANG_MOVING,                   0, 1, 0, 0 },
    { ACT_LEDGE_GRAB,                    0, 1, 0, 0 },
    { ACT_LEDGE_CLIMB_SLOW_1,            0, 1, 0, 0 },
    { ACT_LEDGE_CLIMB_SLOW_2,            0, 1, 0, 0 },
    { ACT_LEDGE_CLIMB_DOWN,              0, 1, 0, 0 },
    { ACT_LEDGE_CLIMB_FAST,              0, 1, 0, 0 },
    { ACT_TORNADO_TWIRLING,              0, 1, 0, 0 },

    // Object
    { ACT_PUNCHING,                      1, 1, 1, 0 },
    { ACT_STOMACH_SLIDE_STOP,            1, 1, 1, 0 },

    // OMM
    { ACT_OMM_SPIN_GROUND,               1, 1, 1, 0 },
    { ACT_OMM_SPIN_POUND_LAND,           1, 1, 1, 0 },
    { ACT_OMM_ROLL,                      0, 1, 1, 0 },
    { ACT_OMM_CAPPY_THROW_GROUND,        1, 1, 1, 0 },
    { ACT_OMM_CAPPY_BOUNCE,              0, 1, 1, 0 },
    { ACT_OMM_CAPPY_VAULT,               0, 1, 1, 0 },
    { ACT_OMM_GROUND_POUND_JUMP,         0, 1, 1, 0 },
    { ACT_OMM_LEAVE_OBJECT_JUMP,         0, 1, 1, 0 },
    { ACT_OMM_WALL_SLIDE,                1, 1, 1, 0 },
    { ACT_OMM_CAPPY_THROW_AIRBORNE,      0, 1, 1, 0 },
    { ACT_OMM_ROLL_AIR,                  0, 1, 1, 0 },
    { ACT_OMM_SPIN_AIR,                  0, 1, 1, 0 },
    { ACT_OMM_SPIN_JUMP,                 0, 1, 1, 0 },
    { ACT_OMM_SPIN_POUND,                0, 1, 0, 0 },

    // Peach
    { ACT_OMM_PEACH_ATTACK_GROUND,       1, 1, 1, 1 },
    { ACT_OMM_PEACH_ATTACK_FAST,         1, 1, 1, 1 },
    { ACT_OMM_PEACH_FLOAT,               0, 1, 1, 0 },
    { ACT_OMM_PEACH_GLIDE,               0, 1, 1, 0 },
    { ACT_OMM_PEACH_ATTACK_AIR,          1, 1, 1, 1 },
    { ACT_OMM_PEACH_PERRY_CHARGE_GROUND, 1, 1, 1, 0 },
    { ACT_OMM_PEACH_PERRY_CHARGE_AIR,    1, 1, 1, 0 },
};

static PerryChargeAction *get_action(struct MarioState *m) {
    for (s32 i = 0; i != (s32) omm_static_array_length(sOmmPerryChargeActions); ++i) {
        if (sOmmPerryChargeActions[i].action == m->action) {
            return &sOmmPerryChargeActions[i];
        }
    }
    return &sOmmPerryChargeActions[0];
}

OMM_ROUTINE_PRE_RENDER(omm_act_peach_perry_charge_update) {
    static bool sPerryChargeCanceled = false;
    if (gMarioObject && !omm_is_game_paused() && !(gTimeStopState & TIME_STOP_ENABLED)) {
        struct MarioState *m = gMarioState;
        PerryChargeAction *a = get_action(m);

        // Clear Perry blast flag if not attacking
        if (!a->attack) {
            gOmmPerryBlast = 0;
        }

        // Clear cancel flag if B is released or during the jump kick/air attack action
        if (!(gPlayer1Controller->buttonDown & B_BUTTON) || m->action == ACT_JUMP_KICK || m->action == ACT_OMM_PEACH_ATTACK_AIR) {
            sPerryChargeCanceled = false;
        }

        // Stop the charge sound effect if not charging
        if (!OMM_PERRY_CHARGING) {
            omm_sound_stop(OMM_SOUND_EFFECT_PERRY_CHARGE);
        }

        // Cancel charge if...
        if (!OMM_SPARKLY_IS_PERRY_CHARGE_UNLOCKED || // Not unlocked
            !OMM_PLAYER_IS_PEACH ||                  // Not Peach
            !OMM_MOVESET_ODYSSEY ||                  // Not Odyssey Moveset
            (!OMM_PERRY_CHARGED && !a->charge) ||    // Not a valid action during the charge
            (OMM_PERRY_CHARGED && !a->charged))      // Not a valid action when fully charged
        {
            sPerryChargeCanceled = true;
            gOmmPerryCharge = 0;
            gOmmPerryBlast = 0;
            return;
        }

        // Don't charge until the cancel flag is cleared
        if (!sPerryChargeCanceled) {

            // B button held
            if (gPlayer1Controller->buttonDown & B_BUTTON) {
                gOmmPerryCharge = min_s(gOmmPerryCharge + 1, OMM_PERRY_CHARGE_END);
                gPlayer1Controller->buttonPressed &= ~B_BUTTON;

                // After some time, change the current action to a charge action
                if (gOmmPerryCharge > OMM_PERRY_CHARGE_ACTION &&
                    gOmmPerryCharge < OMM_PERRY_CHARGE_FULL &&
                    m->action != ACT_OMM_PEACH_PERRY_CHARGE_GROUND &&
                    m->action != ACT_OMM_PEACH_PERRY_CHARGE_AIR) {
                    if (m->action & ACT_FLAG_AIR) {
                        action_set(omm_mario_set_action, ACT_OMM_PEACH_PERRY_CHARGE_AIR, 0, 0, NO_RETURN);
                    } else {
                        action_set(omm_mario_set_action, ACT_OMM_PEACH_PERRY_CHARGE_GROUND, 0, 0, NO_RETURN);
                    }
                }
            }
            
            // B button released
            else {

                // If the charge is complete, release a powerful shockwave
                if (gOmmPerryCharge >= OMM_PERRY_CHARGE_FULL && a->release) {
                    gOmmPerryBlast = 1;
                    if (m->action & ACT_FLAG_AIR) {
                        action_set(omm_mario_set_action, ACT_JUMP_KICK, 0, 0, NO_RETURN);
                    } else {
                        action_set(omm_mario_set_action, ACT_OMM_PEACH_ATTACK_GROUND, 4, 0, NO_RETURN);
                    }
                }

                // Cancel the charge and restore the B press if the charge hasn't started yet
                gPlayer1Controller->buttonPressed |= B_BUTTON * (gOmmPerryCharge > 0 && gOmmPerryCharge < OMM_PERRY_CHARGE_ACTION);
                gOmmPerryCharge = 0;
            }
        }
    }
}
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static f32 omm_metal_water_get_mario_y_vel_based_on_fspeed(struct MarioState *m, f32 velY, f32 multiplier) {
    return velY + multiplier * m->forwardVel;
}

static void omm_metal_water_update_walking_speed(struct MarioState *m) {
    mario_set_forward_vel(m, approach_f32(m->forwardVel, OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED * m->intendedMag / 32.f, 1.f, 3.f));
    if (m->forwardVel < 0.f && !(m->input & INPUT_NONZERO_ANALOG)) {
        mario_set_forward_vel(m, 0.f);
    }
    if (OMM_MOVESET_ODYSSEY || (gOmmMario->metalWater.jumpNext != ACT_OMM_METAL_WATER_LONG_JUMP)) {
        s32 handling = (s32) relerp_0_1_f(abs_f(m->forwardVel), OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 4.f, OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED, OMM_MOVESET_ODYSSEY ? 0x2000 : 0x800, 0x800);
        m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, handling, handling);
    }
}

static void omm_metal_water_play_step_sound(struct MarioState *m, s16 frame1, s16 frame2) {
    if (obj_anim_is_past_frame(m->marioObj, frame1) || obj_anim_is_past_frame(m->marioObj, frame2)) {
        SFX(SOUND_ACTION_METAL_STEP_WATER);
        PFX(PARTICLE_DUST);
    }
}

static void omm_metal_water_set_next_jump(u32 jumpAction) {
    gOmmMario->metalWater.jumpNext = jumpAction;
    gOmmMario->metalWater.jumpTimer = 8;
}

static bool omm_metal_water_common_stationary_step(struct MarioState* m, u32 endAction, s32 animID) {
    ANM(animID, 1.f);
    stationary_ground_step(m);
    if (obj_anim_is_at_end(m->marioObj)) {
        omm_mario_set_action(m, endAction, 0, 0);
        return true;
    }
    return false;
}

static bool omm_metal_water_common_air_action_step(struct MarioState *m, u32 landAction, s32 animID) {
    update_air_without_turn(m);
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE: {
            ANM(animID, 1.f);
        } break;

        case AIR_STEP_LANDED: {
            omm_mario_set_action(m, landAction, 0, 0);
        } break;

        case AIR_STEP_HIT_WALL: {
            ANM(animID, 1.f);
            if (omm_mario_check_wall_slide(m)) {
                if (omm_mario_can_perform_wall_slide(m)) {
                    omm_mario_set_action(m, ACT_OMM_METAL_WATER_WALL_SLIDE, 0, 0);
                    return true;
                }
                break;
            }
            if (!m->heldObj && m->forwardVel > OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 2.f) {
                mario_bonk_reflection(m, false);
                m->faceAngle[1] += 0x8000;
                PFX(PARTICLE_VERTICAL_STAR);
                drop_and_set_mario_action(m, ACT_OMM_METAL_WATER_BACKWARD_AIR_KB, 0);
                return true;
            } else if (m->wall) {
                mario_set_forward_vel(m, 0.f);
            }
        } break;
    }
    return false;
}

static u32 omm_metal_water_update_punch_sequence(struct MarioState *m) {
    switch (gOmmMario->metalWater.punchType) {

        // First punch, first half
        case 0: {
            m->flags |= MARIO_PUNCHING;
            play_mario_action_sound(m, SOUND_ACTION_METAL_STEP_WATER, 0);
            ANM(MARIO_ANIM_FIRST_PUNCH, 1.f);
            if (obj_anim_is_near_end(m->marioObj)) {
                gOmmMario->metalWater.punchType = 1;
                m->marioBodyState->punchState = (0 << 6) | 4;
            }
        } break;

        // First punch, second half
        case 1: {
            m->flags |= MARIO_PUNCHING;
            ANM(MARIO_ANIM_FIRST_PUNCH_FAST, 1.f);
            if (m->controller->buttonPressed & B_BUTTON) {
                gOmmMario->metalWater.punchType = 2;
                m->flags &= ~MARIO_ACTION_SOUND_PLAYED;
            } else if (obj_anim_is_at_end(m->marioObj)) {
                return ACT_OMM_METAL_WATER_IDLE;
            }
        } break;

        // Second punch, first half
        case 2: {
            m->flags |= MARIO_PUNCHING;
            play_mario_action_sound(m, SOUND_ACTION_METAL_STEP_WATER, 0);
            ANM(MARIO_ANIM_SECOND_PUNCH, 1.f);
            if (obj_anim_is_near_end(m->marioObj)) {
                gOmmMario->metalWater.punchType = 3;
                m->marioBodyState->punchState = (1 << 6) | 4;
            }
        } break;

        // Second punch, second half
        case 3: {
            m->flags |= MARIO_PUNCHING;
            ANM(MARIO_ANIM_SECOND_PUNCH_FAST, 1.f);
            if (m->controller->buttonPressed & B_BUTTON) {
                gOmmMario->metalWater.punchType = 4;
                m->flags &= ~MARIO_ACTION_SOUND_PLAYED;
            } else if (obj_anim_is_at_end(m->marioObj)) {
                return ACT_OMM_METAL_WATER_IDLE;
            }
        } break;

        // Ground kick
        case 4: {
            if (!(m->flags & MARIO_ACTION_SOUND_PLAYED)) {
                play_mario_action_sound(m, SOUND_ACTION_METAL_JUMP_WATER, 0);
                m->marioBodyState->punchState = (2 << 6) | 6;
            }
            ANM(MARIO_ANIM_GROUND_KICK, 1.f);
            if (m->marioObj->oAnimFrame < 8) {
                m->flags |= MARIO_KICKING;
            } else if (obj_anim_is_at_end(m->marioObj)) {
                return ACT_OMM_METAL_WATER_IDLE;
            }
        } break;
    }
    return 0;
}

///////////
// Moves //
///////////

static s32 omm_act_metal_water_idle(struct MarioState *m) {
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_START_CROUCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_WALKING, 0, RETURN_CANCEL);

    bool isAnimAtEnd = omm_metal_water_common_stationary_step(m, 0, MARIO_ANIM_IDLE_HEAD_LEFT + m->actionState);
    if (isAnimAtEnd) m->actionState = (m->actionState + 1) % 3;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_first_person(struct MarioState *m) {
    action_init(m->forwardVel, 0, 0, NO_SOUND, music_lower_volume(SEQ_PLAYER_LEVEL, 60, 40));

    s32 animID = obj_anim_is_at_end(m->marioObj) ? MARIO_ANIM_FIRST_PERSON : m->marioObj->oAnimID;
    omm_metal_water_common_stationary_step(m, 0, animID);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_walking(struct MarioState *m) {
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_za_pressed(OMM_MOVESET_ODYSSEY && m->forwardVel >= OMM_MARIO_METAL_WATER_LONG_JUMP_MIN_VEL, ACT_OMM_METAL_WATER_LONG_JUMP, 0, RETURN_CANCEL);
    action_a_pressed(analog_stick_held_back(m) && m->forwardVel >= OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 4.f, ACT_OMM_METAL_WATER_SIDE_FLIP, 0, RETURN_CANCEL, m->faceAngle[1] = 0x8000 + m->intendedYaw;);
    action_a_pressed((m->forwardVel >= OMM_MARIO_METAL_WATER_LONG_JUMP_MIN_VEL || gOmmMario->metalWater.jumpNext == ACT_OMM_METAL_WATER_LONG_JUMP) && (m->controller->buttonDown & Z_TRIG), ACT_OMM_METAL_WATER_LONG_JUMP, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
#if OMM_GAME_IS_R96X
    action_b_pressed(OMM_PLAYER_IS_WARIO && m->forwardVel >= OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED * 0.65f, ACT_OMM_METAL_WATER_WARIO_CHARGE, 0, RETURN_CANCEL);
#endif
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_START_CROUCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    omm_metal_water_update_walking_speed(m);
    Vec3f pos = { m->pos[0], m->pos[1], m->pos[2] };
    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_BREAK);
    action_condition(m->forwardVel == 0, ACT_OMM_METAL_WATER_IDLE, 0, RETURN_BREAK);

    // Push / Side-step
    if (step == GROUND_STEP_HIT_WALL) {
        mario_set_forward_vel(m, min_f(m->forwardVel, OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 4.f));
        if (!m->wall) {
            ANM(MARIO_ANIM_PUSHING, 1.f);
            omm_metal_water_play_step_sound(m, 6, 18);
        } else {
            s16 wallAngle = atan2s(m->wall->normal.z, m->wall->normal.x);
            s16 dWallAngle = wallAngle - m->faceAngle[1];
            if (abs_s(dWallAngle) >= 0x7200) {
                ANM(MARIO_ANIM_PUSHING, 1.f);
                omm_metal_water_play_step_sound(m, 6, 18);
            } else {
                f32 animAccel = 2.f * sqrtf(sqr_f(m->pos[0] - pos[0]) + sqr_f(m->pos[2] - pos[2]));
                ANM(MARIO_ANIM_SIDESTEP_LEFT + (dWallAngle < 0), animAccel);
                omm_metal_water_play_step_sound(m, 20, 20);
                m->marioObj->oGfxAngle[1] = wallAngle + 0x8000;
            }
        }
    } else {
        ANM(MARIO_ANIM_WALKING, max_f(OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 16.f, m->forwardVel / 4.f));
        omm_metal_water_play_step_sound(m, 10, 49);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_start_crouching(struct MarioState *m) {
    action_a_pressed(m->forwardVel >= OMM_MARIO_METAL_WATER_LONG_JUMP_MIN_VEL && m->controller->buttonDown & Z_TRIG, ACT_OMM_METAL_WATER_LONG_JUMP, 0, RETURN_CANCEL);
    action_a_pressed(1, ACT_OMM_METAL_WATER_BACKFLIP, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    mario_set_forward_vel(m, max_f(0.f, m->forwardVel - 2.f));
    perform_ground_step(m);
    m->pos[1] = m->floorHeight;
    m->vel[1] = 0.f;
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);

    ANM(MARIO_ANIM_START_CROUCHING, 1.f);
    if (m->forwardVel > 0.f) PFX(PARTICLE_DUST);
    action_condition(m->forwardVel == 0.f && obj_anim_is_at_end(m->marioObj), ACT_OMM_METAL_WATER_CROUCHING, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_crouching(struct MarioState *m) {
    action_condition(!(m->controller->buttonDown & Z_TRIG), ACT_OMM_METAL_WATER_STOP_CROUCHING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, ACT_OMM_METAL_WATER_BACKFLIP, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_START_CRAWLING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, 0, MARIO_ANIM_CROUCHING);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_stop_crouching(struct MarioState *m) {
    action_a_pressed(1, ACT_OMM_METAL_WATER_BACKFLIP, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_IDLE, MARIO_ANIM_STOP_CROUCHING);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_start_crawling(struct MarioState *m) {
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_CRAWLING, MARIO_ANIM_START_CRAWLING);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_crawling(struct MarioState *m) {
    action_condition(!(m->controller->buttonDown & Z_TRIG), ACT_OMM_METAL_WATER_STOP_CRAWLING, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    m->intendedMag *= 0.125f;
    omm_metal_water_update_walking_speed(m);
    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_BREAK);
    action_condition(m->forwardVel == 0, ACT_OMM_METAL_WATER_STOP_CRAWLING, 0, RETURN_BREAK);

    ANM(MARIO_ANIM_CRAWLING, max_f(OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 16.f, m->forwardVel * 2.f));
    omm_metal_water_play_step_sound(m, 26, 79);

    // Align Mario with the ground below
    static Mat4 sFloorAlignMatrix;
    m->pos[1] = m->floorHeight;
    mtxf_align_terrain_triangle(sFloorAlignMatrix, m->pos, m->faceAngle[1], 40.f);
    m->marioObj->oThrowMatrix = &sFloorAlignMatrix;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_stop_crawling(struct MarioState *m) {
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_CROUCHING, MARIO_ANIM_STOP_CRAWLING);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

///////////
// Jumps //
///////////

static s32 omm_act_metal_water_jump(struct MarioState *m) {
    action_init(m->forwardVel * 0.9f, omm_metal_water_get_mario_y_vel_based_on_fspeed(m, 34.f, 0.12f), PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_JUMP_LAND, MARIO_ANIM_SINGLE_JUMP);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_jump_land(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_LAND_WATER, omm_metal_water_set_next_jump(ACT_OMM_METAL_WATER_DOUBLE_JUMP););
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_WALKING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_IDLE, MARIO_ANIM_LAND_FROM_SINGLE_JUMP);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_double_jump(struct MarioState *m) {
    action_init(m->forwardVel * 0.9f, omm_metal_water_get_mario_y_vel_based_on_fspeed(m, 42.f, 0.12f), PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_DOUBLE_JUMP_LAND, m->vel[1] > 0.f ? MARIO_ANIM_DOUBLE_JUMP_RISE : MARIO_ANIM_DOUBLE_JUMP_FALL);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_double_jump_land(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_LAND_WATER, omm_metal_water_set_next_jump(ACT_OMM_METAL_WATER_TRIPLE_JUMP););
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_WALKING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_IDLE, MARIO_ANIM_LAND_FROM_DOUBLE_JUMP);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_triple_jump(struct MarioState *m) {
#if OMM_GAME_IS_R96X
    action_condition(OMM_PLAYER_IS_WARIO, ACT_OMM_METAL_WATER_WARIO_TRIPLE_JUMP, 0, RETURN_CANCEL);
#endif
    action_init(m->forwardVel * 0.9f, omm_metal_water_get_mario_y_vel_based_on_fspeed(m, 52.f, 0.1f), PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    if (OMM_EXTRAS_SMO_ANIMATIONS && m->actionArg == 1) {
        omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_TRIPLE_JUMP_LAND, MARIO_ANIM_OMM_CAPPY_VAULT);
        if (obj_anim_is_past_frame(m->marioObj, 6)) SFX(SOUND_ACTION_SIDE_FLIP_UNK);
    } else {
        omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_TRIPLE_JUMP_LAND, MARIO_ANIM_TRIPLE_JUMP);
        play_flip_sounds(m, 2, 8, 20);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_triple_jump_land(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_LAND_WATER, omm_metal_water_set_next_jump(ACT_OMM_METAL_WATER_JUMP););
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_WALKING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_IDLE, MARIO_ANIM_TRIPLE_JUMP_LAND);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_backflip(struct MarioState *m) {
    action_init(-OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 2.f, 48.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_JUMP_LAND, MARIO_ANIM_BACKFLIP);
    play_flip_sounds(m, 2, 3, 17);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_backflip_land(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_LAND_WATER, omm_metal_water_set_next_jump(ACT_OMM_METAL_WATER_DOUBLE_JUMP););
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_WALKING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_IDLE, MARIO_ANIM_TRIPLE_JUMP_LAND);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_side_flip(struct MarioState *m) {
    action_init(OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 4.f, 48.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER, m->faceAngle[1] += 0x8000;);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_JUMP_LAND, MARIO_ANIM_SLIDEFLIP);
    if (obj_anim_is_past_frame(m->marioObj, 6)) SFX(SOUND_ACTION_SIDE_FLIP_UNK);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] + 0x8000;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_side_flip_land(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_LAND_WATER, omm_metal_water_set_next_jump(ACT_OMM_METAL_WATER_DOUBLE_JUMP););
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_WALKING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_IDLE, MARIO_ANIM_SLIDEFLIP_LAND);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_long_jump(struct MarioState *m) {
#if !OMM_GAME_IS_R96X
    if (OMM_CHEAT_BLJ_ANYWHERE_REQ && (m->controller->buttonPressed & A_BUTTON)) {
        m->vel[1] = -50.f;
    }
#endif
    action_init((m->forwardVel *= 1.5f), (OMM_CHEAT_BLJ_ANYWHERE_REQ ? -50.f : 30.f), PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY && !OMM_CHEAT_BLJ_ANYWHERE_REQ, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_LONG_JUMP_LAND, MARIO_ANIM_SLOW_LONGJUMP);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_long_jump_land(struct MarioState *m) {
#if !OMM_GAME_IS_R96X
    if (OMM_CHEAT_BLJ_ANYWHERE_REQ) {
        action_a_pressed(1, ACT_OMM_METAL_WATER_LONG_JUMP, 0, RETURN_CANCEL, m->vel[1] = -50.f;);
    }
#endif
    action_init(m->forwardVel, 0.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_LAND_WATER, omm_metal_water_set_next_jump(ACT_OMM_METAL_WATER_LONG_JUMP););
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(m->controller->buttonDown & Z_TRIG, gOmmMario->metalWater.jumpNext, 0, NO_RETURN);
    action_a_pressed(1, ACT_OMM_METAL_WATER_JUMP, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_moving(!OMM_CHEAT_BLJ_ANYWHERE_REQ, ACT_OMM_METAL_WATER_WALKING, 0, RETURN_CANCEL);

    if (!(m->input & INPUT_NONZERO_ANALOG)) {
        mario_set_forward_vel(m, max_f(0.f, m->forwardVel - 2.f));
    } else {
        mario_set_forward_vel(m, approach_f32(m->forwardVel, 0.f, 1.5f, 1.5f));
    }
    perform_ground_step(m);
    m->pos[1] = m->floorHeight;
    m->vel[1] = 0.f;
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);

    ANM(MARIO_ANIM_CROUCHING, 1.f);
    if (m->forwardVel > 0.f) PFX(PARTICLE_DUST);
    action_condition(m->forwardVel == 0.f, ACT_OMM_METAL_WATER_STOP_CROUCHING, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_freefall(struct MarioState *m) {
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_FREEFALL_LAND, MARIO_ANIM_GENERAL_FALL);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_freefall_land(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_LAND_WATER, omm_metal_water_set_next_jump(ACT_OMM_METAL_WATER_DOUBLE_JUMP););
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_WALKING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_IDLE, MARIO_ANIM_GENERAL_LAND);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_wall_slide(struct MarioState *m) {
    action_condition(!m->wall, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL, SFX(SOUND_ACTION_METAL_STEP_WATER););
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL, SFX(SOUND_ACTION_METAL_STEP_WATER););
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_WALL_KICK_AIR, 0, RETURN_CANCEL,
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
        m->faceAngle[1] = (0x8000 + wAngle - dAngle);
    );

    mario_set_forward_vel(m, 0);
    ANM(MARIO_ANIM_START_WALLKICK, 1.f);
    SFX(SOUND_MOVING_TERRAIN_SLIDE + SOUND_TERRAIN_STONE);
    PFX(PARTICLE_DUST);

    // Cling Mario to the wall before performing the air step,
    // to avoid missing slightly slanted walls (normal.y near 0, but not 0)
    m->pos[0] -= m->wall->normal.x * 4.f;
    m->pos[2] -= m->wall->normal.z * 4.f;
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_OMM_METAL_WATER_IDLE, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_NONE, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_should_stop_wall_slide(m), ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_BREAK);

    // Turn Mario away from the wall
    m->marioObj->oGfxAngle[1] = atan2s(m->wall->normal.z, m->wall->normal.x);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_wall_kick_air(struct MarioState *m) {
    action_init(18.f, 40.f, 0, SOUND_ACTION_METAL_JUMP_WATER);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_JUMP_LAND, MARIO_ANIM_SLIDEJUMP);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

/////////////
// Attacks //
/////////////

static s32 omm_act_metal_water_punching(struct MarioState *m) {
    action_init(max_f(12.f, m->forwardVel), 0.f, 0, 0, omm_metal_water_set_next_jump(ACT_OMM_METAL_WATER_JUMP););
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    mario_set_forward_vel(m, approach_f32(clamp_f(m->forwardVel, -OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED, OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED), 0, 1.5f, 1.f));
    perform_ground_step(m);
    u32 action = omm_metal_water_update_punch_sequence(m);
    action_condition(action != 0, action, 0, RETURN_BREAK);
    if (m->forwardVel != 0.f) PFX(PARTICLE_DUST);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_jump_kick(struct MarioState *m) {
    bool cappyTwirl = (OMM_MOVESET_ODYSSEY && omm_cappy_get_object());
    action_init(min_f(m->forwardVel, 8.f), (cappyTwirl ? 30.f : 25.f), PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER, m->actionArg = cappyTwirl;);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);

    // Twirl
    if (m->actionArg == 1) {
        if (OMM_EXTRAS_SMO_ANIMATIONS) {
            ANM(MARIO_ANIM_OMM_CAPPY_RAINBOW_SPIN, 1.f);
            m->flags |= MARIO_KICKING * (m->marioObj->oAnimFrame < 20);
        } else {
            ANM(MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN, 1.5f);
            obj_anim_clamp_frame(m->marioObj, 74, 94);
            m->flags |= MARIO_KICKING * (m->marioObj->oAnimFrame < 94);
        }
        m->marioBodyState->punchState = 0;

        update_air_without_turn(m);
        switch (perform_air_step(m, 0)) {
            case AIR_STEP_LANDED: omm_mario_set_action(m, ACT_OMM_METAL_WATER_FREEFALL_LAND, 0, 0); break;
            case AIR_STEP_HIT_WALL: mario_set_forward_vel(m, 0.f); break;
        }
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }

    // Kick
    ANM(MARIO_ANIM_AIR_KICK, 1.f);
    if (obj_anim_is_past_frame(m->marioObj, 0)) m->marioBodyState->punchState = (2 << 6) | 6;
    if (!obj_anim_is_near_end(m->marioObj)) m->flags |= MARIO_KICKING;
    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_FREEFALL_LAND, MARIO_ANIM_AIR_KICK);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_dive(struct MarioState *m) {
    action_init(14.f, 28.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER);

    ANM(MARIO_ANIM_DIVE, 1.f);
    update_air_without_turn(m);
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE: {
            m->faceAngle[0] = max_s(m->faceAngle[0] - 0x200 * (m->vel[1] < 0.f), -0x2AAA);
            m->marioObj->oGfxAngle[0] = -m->faceAngle[0];
        } break;

        case AIR_STEP_LANDED: {
            m->faceAngle[0] = 0;
            SFX(SOUND_ACTION_METAL_LAND_WATER);
            omm_mario_set_action(m, ACT_OMM_METAL_WATER_FREEFALL_LAND, 0, 0);
        } break;

        case AIR_STEP_HIT_WALL: {
            mario_bonk_reflection(m, true);
            m->faceAngle[0] = 0;
            m->vel[1] = min_f(0, m->vel[1]);
            PFX(PARTICLE_VERTICAL_STAR);
            omm_mario_set_action(m, ACT_OMM_METAL_WATER_BACKWARD_AIR_KB, 0, 0);
        } break;
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_ground_pound(struct MarioState *m) {
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_DIVE, 0, RETURN_CANCEL);

    if (m->actionState == 0) {
        m->pos[1] = min_f(m->pos[1] + max_s(0, 20 - 2 * m->actionTimer), m->ceilHeight - 160.f);
        vec3f_copy(m->marioObj->oGfxPos, m->pos);
        mario_set_forward_vel(m, 0.f);
        ANM(m->actionArg == 0 ? MARIO_ANIM_START_GROUND_POUND : MARIO_ANIM_TRIPLE_JUMP_GROUND_POUND, 1.f);
        if (m->actionTimer == 0) SFX(SOUND_ACTION_SPIN);
        if (++m->actionTimer >= m->marioObj->oCurrAnim->mLoopEnd + 6) {
            m->vel[1] = -48.f;
            m->actionState = 1;
        }
    } else {
        m->vel[1] = -48.f;
        mario_set_forward_vel(m, 0);
        ANM(MARIO_ANIM_GROUND_POUND, 1.f);
        PFX(PARTICLE_PLUNGE_BUBBLE);
        s32 step = perform_air_step(m, 0);
        if (step == AIR_STEP_LANDED) {
            SFX(SOUND_ACTION_METAL_LAND_WATER);
            PFX(PARTICLE_MIST_CIRCLE | PARTICLE_HORIZONTAL_STAR);
            omm_mario_set_action(m, ACT_OMM_METAL_WATER_GROUND_POUND_LAND, 0, 0);
            set_camera_shake_from_hit(SHAKE_GROUND_POUND);
        }
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_ground_pound_land(struct MarioState *m) {
    action_init(0, 0, 0, 0, omm_metal_water_set_next_jump(ACT_OMM_METAL_WATER_JUMP););
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_GROUND_POUND_JUMP, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_GROUND_POUND_LAND_STOP, MARIO_ANIM_GROUND_POUND_LANDING);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_ground_pound_land_stop(struct MarioState *m) {
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(1, gOmmMario->metalWater.jumpNext, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_IDLE, MARIO_ANIM_STOP_SLIDE);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_ground_pound_jump(struct MarioState *m) {
    action_init(0, 51.f, 0, SOUND_ACTION_METAL_JUMP_WATER, gOmmMario->spin.yaw = 1;);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    if (m->vel[1] >= 30.f) {
        s32 step = perform_air_step(m, 0);
        action_condition(step == AIR_STEP_LANDED, ACT_OMM_METAL_WATER_JUMP_LAND, 0, RETURN_BREAK);
    } else {
        bool shouldBreak = omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_JUMP_LAND, MARIO_ANIM_SINGLE_JUMP);
        if (shouldBreak) RETURN_BREAK;
    }

    ANM(MARIO_ANIM_SINGLE_JUMP, 1.f);
    obj_anim_clamp_frame(m->marioObj, 0, 9); // Luigi's freaking flutter jump (again)
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += (0x80 * m->vel[1]) * (prevSpinYaw != 0) * (omm_mario_has_vanish_cap(m) ? 0.8f : 1.f) / sqr_f(omm_player_physics_get_selected_jump());
    gOmmMario->spin.yaw *= ((u16) prevSpinYaw < (u16) gOmmMario->spin.yaw) * (m->vel[1] > 0.f);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] + gOmmMario->spin.yaw;
    PFX(m->vel[1] > 0.f ? PARTICLE_DUST : 0);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_backward_ground_kb(struct MarioState *m) {
    mario_set_forward_vel(m, m->forwardVel * 0.94f);
    ANM(MARIO_ANIM_BACKWARD_KB, 1.f);

    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_METAL_WATER_BACKWARD_AIR_KB, 0, RETURN_BREAK);
    action_condition(obj_anim_is_at_end(m->marioObj), ACT_OMM_METAL_WATER_IDLE, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_backward_air_kb(struct MarioState *m) {
    action_init((m->prevAction == ACT_OMM_METAL_WATER_BACKWARD_GROUND_KB ? m->forwardVel : -16.f), +18.f, 0, 0);
    action_condition(m->actionTimer++ >= 60, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_BREAK);
    mario_set_forward_vel(m, m->forwardVel * 0.97f);
    ANM(MARIO_ANIM_BACKWARD_AIR_KB, 1.f);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED: {
            SFX(SOUND_ACTION_METAL_LAND_WATER);
            omm_mario_set_action(m, ACT_OMM_METAL_WATER_BACKWARD_GROUND_KB, 0, 0);
        } break;

        case AIR_STEP_HIT_WALL: {
            mario_bonk_reflection(m, false);
            m->vel[1] = min_f(0, m->vel[1]);
            mario_set_forward_vel(m, m->forwardVel);
        } break;
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

///////////
// Cappy //
///////////

static s32 omm_act_metal_water_cappy_throw_ground(struct MarioState *m) {
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_za_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_LONG_JUMP, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_JUMP, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);

    f32 f = coss(abs_s(m->faceAngle[1] - m->intendedYaw)) * m->controller->stickMag / 64.f;
    mario_set_forward_vel(m, m->forwardVel * clamp_f(f, 0.80f, 0.98f));

    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_BREAK);
    action_condition(step == GROUND_STEP_HIT_WALL, ACT_OMM_METAL_WATER_IDLE, 0, RETURN_BREAK, m->forwardVel = 0;);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_cappy_throw_airborne(struct MarioState *m) {
    action_init(min_f(m->forwardVel, 4.f), 10.f, 0, 0);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_FREEFALL_LAND, m->marioObj->oAnimID);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_cappy_bounce(struct MarioState *m) {
    action_init(m->forwardVel * 0.9f, 45.f, 0, SOUND_ACTION_METAL_JUMP_WATER);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_DOUBLE_JUMP_LAND, m->vel[1] >= 0.f ? MARIO_ANIM_DOUBLE_JUMP_RISE : MARIO_ANIM_DOUBLE_JUMP_FALL);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

//////////
// Spin //
//////////

static s32 omm_act_metal_water_spin_ground(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, 0, 0);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_JUMP, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_PUNCHING, 0, RETURN_CANCEL);
    action_off_floor(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);
    action_condition(gOmmMario->spin.timer == 0, ACT_OMM_METAL_WATER_IDLE, 0, RETURN_CANCEL);

    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_METAL_WATER_SPIN_AIR, 0, RETURN_BREAK);

    mario_set_forward_vel(m, max_f(0, m->forwardVel - 0.8f));
    ANM(MARIO_ANIM_TWIRL, 1.f);
    SFX(SOUND_MOVING_TERRAIN_SLIDE + SOUND_TERRAIN_STONE);
    gOmmMario->spin.yaw += min_s(0x238A, 0x200 * gOmmMario->spin.timer);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmMario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    PFX(PARTICLE_DUST | PARTICLE_PLUNGE_BUBBLE);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_spin_air(struct MarioState *m) {
    action_init(m->forwardVel, 12.f, 0, 0);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_condition(gOmmMario->spin.timer == 0, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    bool shouldBreak = omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_JUMP_LAND, MARIO_ANIM_TWIRL);
    if (shouldBreak) RETURN_BREAK;

    ANM(MARIO_ANIM_TWIRL, 1.f);
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += min_s(0x4139, 0x300 * gOmmMario->spin.timer);
    if (gOmmMario->spin.yaw < prevSpinYaw) SFX(SOUND_ACTION_SPIN);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmMario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    PFX(PARTICLE_PLUNGE_BUBBLE);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_spin_jump(struct MarioState *m) {
    action_init(m->forwardVel, 54.f, 0, SOUND_ACTION_METAL_JUMP_WATER);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_POUND, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_JUMP_KICK, 0, RETURN_CANCEL);
    action_condition(m->vel[1] <= 0.f, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    bool shouldBreak = omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_JUMP_LAND, MARIO_ANIM_TWIRL);
    if (shouldBreak) RETURN_BREAK;

    ANM(MARIO_ANIM_TWIRL, 1.f);
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += min_s(0x44CD, 0x300 * m->vel[1]);
    if (gOmmMario->spin.yaw < prevSpinYaw) SFX(SOUND_ACTION_SPIN);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmMario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    PFX(PARTICLE_SPARKLES | PARTICLE_PLUNGE_BUBBLE);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_spin_pound(struct MarioState *m) {
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_DIVE, 0, RETURN_CANCEL);

    m->vel[1] = -56.f;
    mario_set_forward_vel(m, 0);
    s32 step = perform_air_step(m, 0);
    s32 soundBits = (SOUND_ACTION_METAL_LAND_WATER | 0x0000FF00);
    u32 particles = (PARTICLE_MIST_CIRCLE | PARTICLE_HORIZONTAL_STAR);
    action_condition(step == AIR_STEP_LANDED, ACT_OMM_METAL_WATER_SPIN_POUND_LAND, 0, RETURN_BREAK, SFX(soundBits); PFX(particles); set_camera_shake_from_hit(SHAKE_GROUND_POUND););

    ANM(MARIO_ANIM_TWIRL, 1.f);
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += 0x3000;
    if (gOmmMario->spin.yaw < prevSpinYaw) SFX(SOUND_ACTION_SIDE_FLIP_UNK);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmMario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    PFX(PARTICLE_DUST | PARTICLE_PLUNGE_BUBBLE);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_spin_pound_land(struct MarioState *m) {
    action_init(0, 0, 0, 0);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_GROUND_POUND_JUMP, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_METAL_WATER_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_IDLE, MARIO_ANIM_GENERAL_LAND);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

//////////
// Hold //
//////////

static s32 omm_act_metal_water_hold_idle(struct MarioState *m) {
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_OMM_METAL_WATER_IDLE, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_z_pressed(1, ACT_OMM_METAL_WATER_START_CROUCHING, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_a_pressed(1, ACT_OMM_METAL_WATER_HOLD_JUMP, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_HOLD_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_HOLD_WALKING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, 0, MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_hold_walking(struct MarioState *m) {
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_OMM_METAL_WATER_WALKING, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_z_pressed(1, ACT_OMM_METAL_WATER_START_CROUCHING, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_a_pressed(1, ACT_OMM_METAL_WATER_HOLD_JUMP, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_HOLD_FREEFALL, 0, RETURN_CANCEL);

    m->intendedMag *= 0.4f;
    omm_metal_water_update_walking_speed(m);
    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_METAL_WATER_HOLD_FREEFALL, 0, RETURN_BREAK);
    action_condition(m->forwardVel == 0, ACT_OMM_METAL_WATER_HOLD_IDLE, 0, RETURN_BREAK);

    if (step == GROUND_STEP_HIT_WALL) mario_set_forward_vel(m, 0);
    ANM(MARIO_ANIM_RUN_WITH_LIGHT_OBJ, max_f(OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 16.f, (m->forwardVel / 2.f)));
    omm_metal_water_play_step_sound(m, 10, 49);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_hold_jump(struct MarioState *m) {
    action_init(m->forwardVel * 0.9f, omm_metal_water_get_mario_y_vel_based_on_fspeed(m, 34.f, 0.12f), PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER);
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL, mario_drop_held_object(m););

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_HOLD_JUMP_LAND, MARIO_ANIM_JUMP_WITH_LIGHT_OBJ);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_hold_jump_land(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_LAND_WATER);
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_OMM_METAL_WATER_IDLE, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_z_pressed(1, ACT_OMM_METAL_WATER_START_CROUCHING, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_a_pressed(1, ACT_OMM_METAL_WATER_HOLD_JUMP, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_HOLD_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_HOLD_WALKING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_HOLD_IDLE, MARIO_ANIM_JUMP_LAND_WITH_LIGHT_OBJ);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_hold_freefall(struct MarioState *m) {
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_OMM_METAL_WATER_FREEFALL, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL, mario_drop_held_object(m););

    omm_metal_water_common_air_action_step(m, ACT_OMM_METAL_WATER_HOLD_FREEFALL_LAND, MARIO_ANIM_FALL_WITH_LIGHT_OBJ);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_hold_freefall_land(struct MarioState *m) {
    action_init(m->forwardVel, 0.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_LAND_WATER);
    action_condition(m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT, ACT_OMM_METAL_WATER_IDLE, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_z_pressed(1, ACT_OMM_METAL_WATER_START_CROUCHING, 0, RETURN_CANCEL, mario_drop_held_object(m););
    action_a_pressed(1, ACT_OMM_METAL_WATER_HOLD_JUMP, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_OMM_METAL_WATER_HOLD_FREEFALL, 0, RETURN_CANCEL);
    action_moving(1, ACT_OMM_METAL_WATER_HOLD_WALKING, 0, RETURN_CANCEL);

    omm_metal_water_common_stationary_step(m, ACT_OMM_METAL_WATER_HOLD_IDLE, MARIO_ANIM_FALL_LAND_WITH_LIGHT_OBJ);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

#if OMM_GAME_IS_R96X

///////////
// Wario //
///////////

static void omm_update_wario_charge_speed(struct MarioState *m) {
    f32 maxSpeed = OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED * 2.f;
    f32 tgtSpeed = maxSpeed * MIN(MAX(m->intendedMag / 32.f, 0.5f), 1.f);

    if (m->forwardVel <= 0.f) {
        m->forwardVel += 1.5f;
    } else if (m->forwardVel <= tgtSpeed) {
        m->forwardVel += 1.5f - m->forwardVel / maxSpeed;
    } else if (m->floor->normal.y >= 0.9f) {
        m->forwardVel -= 1.f;
    }

    m->forwardVel = MIN(MAX(m->forwardVel, 0.f), maxSpeed);
    m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, 0x800, 0x800);
    apply_slope_accel(m);
}

static s32 omm_act_metal_water_wario_charge(struct MarioState *m) {
    action_init(m->forwardVel, 0, 0, 0);
    action_a_pressed(1, ACT_OMM_METAL_WATER_WARIO_TRIPLE_JUMP, 0, RETURN_CANCEL);

    if (m->actionTimer++ < 60) {
        omm_update_wario_charge_speed(m);
        ANM(MARIO_ANIM_RUNNING_UNUSED, 10.f);
        omm_metal_water_play_step_sound(m, 9, 45);
        adjust_sound_for_speed(m);
    } else {
        omm_mario_set_action(m, ACT_OMM_METAL_WATER_WALKING, 0, 0);
    }

    switch (perform_ground_step(m)) {
        case GROUND_STEP_LEFT_GROUND: {
            ANM(MARIO_ANIM_GENERAL_FALL, 1.f);
            omm_mario_set_action(m, ACT_OMM_METAL_WATER_FREEFALL, 0, 0);
            return OMM_MARIO_ACTION_RESULT_BREAK;
        } break;

        case GROUND_STEP_HIT_WALL: {
            SFX(SOUND_ACTION_METAL_LAND_WATER);
            PFX(PARTICLE_VERTICAL_STAR);
            omm_mario_set_action(m, ACT_OMM_METAL_WATER_BACKWARD_GROUND_KB, 0, 0);
            return OMM_MARIO_ACTION_RESULT_BREAK;
        } break;

        case GROUND_STEP_NONE: {
            m->flags |= MARIO_KICKING;
            PFX(PARTICLE_DUST | PARTICLE_PLUNGE_BUBBLE);
        } break;
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_metal_water_wario_triple_jump(struct MarioState *m) {
    action_init(m->forwardVel * 0.9f, 48.f, PARTICLE_MIST_CIRCLE, SOUND_ACTION_METAL_JUMP_WATER);
    action_cappy(1, ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_b_pressed(1, ACT_OMM_METAL_WATER_DIVE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_OMM_METAL_WATER_GROUND_POUND, 0, RETURN_CANCEL);

    update_air_without_turn(m);
    mario_set_forward_vel(m, MIN(m->forwardVel, 20.f));
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED: {
            if (m->actionState++ == 1) {
                m->vel[1] = 32.f;
                PFX(PARTICLE_MIST_CIRCLE);
            } else {
                omm_mario_set_action(m, ACT_OMM_METAL_WATER_FREEFALL_LAND, 0, 0);
            }
            SFX(SOUND_ACTION_METAL_LAND_WATER);
        } break;

        case AIR_STEP_HIT_WALL: {
            mario_bonk_reflection(m, true);
        } break;
    }

    if (m->actionState < 2 || m->vel[1] > 0.f) {
        ANM(MARIO_ANIM_FORWARD_SPINNING, 1.f);
        if (obj_anim_is_past_frame(m->marioObj, 0)) {
            SFX(SOUND_ACTION_SPIN);
        }
    } else {
        ANM(MARIO_ANIM_GENERAL_FALL, 1.f);
    }

    return OMM_MARIO_ACTION_RESULT_BREAK;
}

#endif

/////////////////
// Metal Water //
/////////////////

s32 omm_mario_execute_metal_water_action(struct MarioState *m) {

    // Drown Metal Mario if dead
    if (omm_mario_is_dead(m)) {
        drop_and_set_mario_action(m, ACT_DROWNING, 0);
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Return to Submerged action if not Metal
    if (!omm_mario_has_metal_cap(m)) {
        omm_mario_set_action(m, m->heldObj ? ACT_HOLD_WATER_IDLE : ACT_WATER_IDLE, 0, 0);
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Return to vanilla Metal if not Improved Metal cap
    if (!OMM_POWER_UPS_IMPROVED) {
        omm_mario_set_action(m, m->heldObj ? ACT_HOLD_METAL_WATER_FALLING : ACT_METAL_WATER_FALLING, 0, 0);
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Water jump
    if (m->vel[1] > 0.f && m->pos[1] > m->waterLevel - OMM_MARIO_WATER_JUMP_SURFACE_OFFSET) {
        omm_mario_set_action(m, m->heldObj ? ACT_HOLD_WATER_JUMP : ACT_WATER_JUMP, 0, 0);
        SFX(SOUND_ACTION_UNKNOWN430);
        PFX(PARTICLE_WATER_SPLASH);
        vec3s_zero(m->angleVel);
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Water surface
    if (m->pos[1] > m->waterLevel - OMM_MARIO_WATER_SURFACE_OFFSET) {
        if (m->floorHeight >= m->waterLevel - OMM_MARIO_WATER_SURFACE_OFFSET) {
            transition_submerged_to_walking(m);
            return OMM_MARIO_ACTION_RESULT_CANCEL;
        }
        m->pos[1] = m->waterLevel - OMM_MARIO_WATER_SURFACE_OFFSET;
    }

    // Jump timer
    if (gOmmMario->metalWater.jumpTimer == 0) {
        gOmmMario->metalWater.jumpNext = ACT_OMM_METAL_WATER_JUMP;
    } else {
        gOmmMario->metalWater.jumpTimer--;
    }

    // Update
    gOmmMario->metalWater.punchType *= (m->action == ACT_OMM_METAL_WATER_PUNCHING);
    gOmmMario->wallSlide.jumped = false;
    gOmmMario->peach.floated = false;
    gOmmMario->cappy.bounced = false;
    gOmmMario->state.airCombo = 0;
    gOmmMario->midairSpin.counter = 0;
    m->quicksandDepth = 0.f;
    m->marioBodyState->headAngle[1] = 0;
    m->marioBodyState->headAngle[2] = 0;
    PFX(PARTICLE_BUBBLE);
    m->flags &= ~(MARIO_PUNCHING | MARIO_KICKING | MARIO_TRIPPING);

    // Actions
    switch (m->action) {

        // Moves
        case ACT_OMM_METAL_WATER_IDLE:                      return omm_act_metal_water_idle(m);
        case ACT_OMM_METAL_WATER_FIRST_PERSON:              return omm_act_metal_water_first_person(m);
        case ACT_OMM_METAL_WATER_WALKING:                   return omm_act_metal_water_walking(m);
        case ACT_OMM_METAL_WATER_START_CROUCHING:           return omm_act_metal_water_start_crouching(m);
        case ACT_OMM_METAL_WATER_CROUCHING:                 return omm_act_metal_water_crouching(m);
        case ACT_OMM_METAL_WATER_STOP_CROUCHING:            return omm_act_metal_water_stop_crouching(m);
        case ACT_OMM_METAL_WATER_START_CRAWLING:            return omm_act_metal_water_start_crawling(m);
        case ACT_OMM_METAL_WATER_CRAWLING:                  return omm_act_metal_water_crawling(m);
        case ACT_OMM_METAL_WATER_STOP_CRAWLING:             return omm_act_metal_water_stop_crawling(m);

        // Jumps
        case ACT_OMM_METAL_WATER_JUMP:                      return omm_act_metal_water_jump(m);
        case ACT_OMM_METAL_WATER_JUMP_LAND:                 return omm_act_metal_water_jump_land(m);
        case ACT_OMM_METAL_WATER_DOUBLE_JUMP:               return omm_act_metal_water_double_jump(m);
        case ACT_OMM_METAL_WATER_DOUBLE_JUMP_LAND:          return omm_act_metal_water_double_jump_land(m);
        case ACT_OMM_METAL_WATER_TRIPLE_JUMP:               return omm_act_metal_water_triple_jump(m);
        case ACT_OMM_METAL_WATER_TRIPLE_JUMP_LAND:          return omm_act_metal_water_triple_jump_land(m);
        case ACT_OMM_METAL_WATER_BACKFLIP:                  return omm_act_metal_water_backflip(m);
        case ACT_OMM_METAL_WATER_BACKFLIP_LAND:             return omm_act_metal_water_backflip_land(m);
        case ACT_OMM_METAL_WATER_SIDE_FLIP:                 return omm_act_metal_water_side_flip(m);
        case ACT_OMM_METAL_WATER_SIDE_FLIP_LAND:            return omm_act_metal_water_side_flip_land(m);
        case ACT_OMM_METAL_WATER_LONG_JUMP:                 return omm_act_metal_water_long_jump(m);
        case ACT_OMM_METAL_WATER_LONG_JUMP_LAND:            return omm_act_metal_water_long_jump_land(m);
        case ACT_OMM_METAL_WATER_FREEFALL:                  return omm_act_metal_water_freefall(m);
        case ACT_OMM_METAL_WATER_FREEFALL_LAND:             return omm_act_metal_water_freefall_land(m);
        case ACT_OMM_METAL_WATER_WALL_SLIDE:                return omm_act_metal_water_wall_slide(m);
        case ACT_OMM_METAL_WATER_WALL_KICK_AIR:             return omm_act_metal_water_wall_kick_air(m);

        // Attacks
        case ACT_OMM_METAL_WATER_PUNCHING:                  return omm_act_metal_water_punching(m);
        case ACT_OMM_METAL_WATER_JUMP_KICK:                 return omm_act_metal_water_jump_kick(m);
        case ACT_OMM_METAL_WATER_DIVE:                      return omm_act_metal_water_dive(m);
        case ACT_OMM_METAL_WATER_GROUND_POUND:              return omm_act_metal_water_ground_pound(m);
        case ACT_OMM_METAL_WATER_GROUND_POUND_LAND:         return omm_act_metal_water_ground_pound_land(m);
        case ACT_OMM_METAL_WATER_GROUND_POUND_LAND_STOP:    return omm_act_metal_water_ground_pound_land_stop(m);
        case ACT_OMM_METAL_WATER_GROUND_POUND_JUMP:         return omm_act_metal_water_ground_pound_jump(m);
        case ACT_OMM_METAL_WATER_BACKWARD_GROUND_KB:        return omm_act_metal_water_backward_ground_kb(m);
        case ACT_OMM_METAL_WATER_BACKWARD_AIR_KB:           return omm_act_metal_water_backward_air_kb(m);

        // Cappy
        case ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND:        return omm_act_metal_water_cappy_throw_ground(m);
        case ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE:      return omm_act_metal_water_cappy_throw_airborne(m);
        case ACT_OMM_METAL_WATER_CAPPY_BOUNCE:              return omm_act_metal_water_cappy_bounce(m);

        // Spin
        case ACT_OMM_METAL_WATER_SPIN_GROUND:               return omm_act_metal_water_spin_ground(m);
        case ACT_OMM_METAL_WATER_SPIN_AIR:                  return omm_act_metal_water_spin_air(m);
        case ACT_OMM_METAL_WATER_SPIN_JUMP:                 return omm_act_metal_water_spin_jump(m);
        case ACT_OMM_METAL_WATER_SPIN_POUND:                return omm_act_metal_water_spin_pound(m);
        case ACT_OMM_METAL_WATER_SPIN_POUND_LAND:           return omm_act_metal_water_spin_pound_land(m);

        // Hold
        case ACT_OMM_METAL_WATER_HOLD_IDLE:                 return omm_act_metal_water_hold_idle(m);
        case ACT_OMM_METAL_WATER_HOLD_WALKING:              return omm_act_metal_water_hold_walking(m);
        case ACT_OMM_METAL_WATER_HOLD_JUMP:                 return omm_act_metal_water_hold_jump(m);
        case ACT_OMM_METAL_WATER_HOLD_JUMP_LAND:            return omm_act_metal_water_hold_jump_land(m);
        case ACT_OMM_METAL_WATER_HOLD_FREEFALL:             return omm_act_metal_water_hold_freefall(m);
        case ACT_OMM_METAL_WATER_HOLD_FREEFALL_LAND:        return omm_act_metal_water_hold_freefall_land(m);

#if OMM_GAME_IS_R96X
        // Wario
        case ACT_OMM_METAL_WATER_WARIO_CHARGE:              return omm_act_metal_water_wario_charge(m);
        case ACT_OMM_METAL_WATER_WARIO_TRIPLE_JUMP:         return omm_act_metal_water_wario_triple_jump(m);
#endif
    }

    return OMM_MARIO_ACTION_RESULT_BREAK;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Actions
//

// m->actionState is the position of Mario's head: 0 = left, 1 = right, 2 = center, 3 = fall asleep (only possible if m->actionTimer >= 10)
// m->actionTimer is the number of completed head turning cycles
static s32 omm_act_idle(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    action_condition(m->quicksandDepth > 30.f, ACT_IN_QUICKSAND, 0, RETURN_CANCEL);
    action_condition(!OMM_PEACH_VIBE_NO_COUGHING && (m->input & INPUT_IN_POISON_GAS), ACT_COUGHING, 0, RETURN_CANCEL);
    action_condition(!g1HPMode && !OMM_PEACH_VIBE_NO_PANTING && !(m->actionArg & 1) && (m->health < 0x300), ACT_PANTING, 0, RETURN_CANCEL);
    action_condition(check_common_idle_cancels(m), 0, 0, RETURN_CANCEL);
    action_condition(m->actionState == 3 && !OMM_PEACH_VIBE_NO_SHIVERING && (m->area->terrainType & TERRAIN_MASK) == TERRAIN_SNOW, ACT_SHIVERING, 0, RETURN_CANCEL);
    action_condition(m->actionState == 3 && !OMM_PEACH_VIBE_NO_SLEEPING, ACT_START_SLEEPING, 0, RETURN_CANCEL);

    if (stationary_ground_step(m) == GROUND_STEP_NONE) {
        if (m->actionArg & 1) {
            if (!is_anim_playing) {
                obj_anim_play(m->marioObj, MARIO_ANIM_STAND_AGAINST_WALL, 1.f);
            }
        } else {
            if (OMM_PEACH_VIBE_NO_HEAD_TURN) {
                if (!is_anim_playing) {
                    obj_anim_play(m->marioObj, MARIO_ANIM_FIRST_PERSON, 1.f);
                }
            } else {
                if (!is_anim_playing) {
                    if (enable_head_rotations) {
                        switch (m->actionState) {
                            case 0: obj_anim_play(m->marioObj, MARIO_ANIM_IDLE_HEAD_LEFT, 1.f); break;
                            case 1: obj_anim_play(m->marioObj, MARIO_ANIM_IDLE_HEAD_RIGHT, 1.f); break;
                            case 2: obj_anim_play(m->marioObj, MARIO_ANIM_IDLE_HEAD_CENTER, 1.f); break;
                        }
                        if (obj_anim_is_at_end(m->marioObj) && ++m->actionState == 3) {
                            f32 deltaY = abs_f(m->pos[1] - find_floor_height_relative_polar(m, -0x8000, 60.f));
                            if (deltaY > 24.f || (m->floor->flags & SURFACE_FLAG_DYNAMIC) || ++m->actionTimer < 10) {
                                m->actionState = 0;
                            }
                        }
                    } else {
                        obj_anim_play(m->marioObj, MARIO_ANIM_FIRST_PERSON, 1.f);
                    }
                }
            }
        }
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_panting(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_PANTING, ACT_IDLE, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_coughing(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_COUGHING, ACT_IDLE, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_shivering(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_SHIVERING, ACT_IDLE, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_start_sleeping(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_SLEEPING, ACT_WAKING_UP, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_sleeping(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_SLEEPING, ACT_WAKING_UP, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_standing_against_wall(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_in_quicksand(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_PUNCHING, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_crouching(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_crawling(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_braking_stop(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_jump_land_stop(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_double_jump_land_stop(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_side_flip_land_stop(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_freefall_land_stop(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_triple_jump_land_stop(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_backflip_land_stop(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_long_jump_land_stop(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_ground_pound_land(struct MarioState *m) {
    action_init(0, 0, 0, 0);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_GROUND_POUND_JUMP, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_shocked_from_high_fall(struct MarioState *m) {
    action_init(0, 0, PARTICLE_MIST_CIRCLE, SOUND_MARIO_ATTACKED, play_mario_heavy_landing_sound(m, SOUND_ACTION_TERRAIN_BODY_HIT_GROUND););
    obj_anim_play(m->marioObj, MARIO_ANIM_SHOCKED, 1.f);
    stop_and_set_height_to_floor(m);
    if (m->actionTimer++ < 30) set_camera_shake_from_hit(SHAKE_SHOCK);
    action_condition(m->actionTimer >= 45, ACT_IDLE, 0, RETURN_BREAK);
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
    action_condition(gOmmData->mario->spin.timer == 0, ACT_IDLE, 0, RETURN_CANCEL);

    s32 step = perform_ground_step(m);
    action_condition(step == GROUND_STEP_LEFT_GROUND, ACT_OMM_SPIN_AIR, 0, RETURN_BREAK);

    mario_set_forward_vel(m, max_f(0, m->forwardVel - 1.f));
    obj_anim_play(m->marioObj, MARIO_ANIM_TWIRL, 1.f);
    play_sound(SOUND_MOVING_TERRAIN_SLIDE + m->terrainSoundAddend, m->marioObj->oCameraToObject);
    gOmmData->mario->spin.yaw += min_s(0x31E9, 0x280 * gOmmData->mario->spin.timer);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmData->mario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    m->particleFlags |= PARTICLE_DUST;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_spin_pound_land(struct MarioState *m) {
    action_init(0, 0, 0, 0);
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_GROUND_POUND_JUMP, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    action_off_floor(1, ACT_FREEFALL, 0, RETURN_CANCEL);

    stationary_ground_step(m);
    obj_anim_play(m->marioObj, MARIO_ANIM_GENERAL_LAND, 1.f);
    action_condition(obj_anim_is_at_end(m->marioObj), ACT_IDLE, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

//
// Stationary
//

static bool omm_check_common_stationary_cancels(struct MarioState *m) {

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

s32 omm_mario_execute_stationary_action(struct MarioState *m) {
    gOmmData->mario->wallSlide.jumped = false;
    gOmmData->mario->peach.floated = false;
    gOmmData->mario->cappy.bounced = false;
    gOmmData->mario->state.airCombo = 0;
    gOmmData->mario->midairSpin.counter = 0;

    // Cancels
    if (omm_check_common_stationary_cancels(m)) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Quicksand
    // Needs to be 0 to not apply the sinking twice per frame
    if (mario_update_quicksand(m, (m->action == ACT_IDLE ? 0.25f : 0.f))) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // In water
    if (m->input & INPUT_IN_WATER) {
        m->particleFlags |= PARTICLE_WAVE_TRAIL;
        m->particleFlags &= ~PARTICLE_DUST;
    }

    // Actions
    switch (m->action) {
        case ACT_IDLE:                          return omm_act_idle(m);
        case ACT_PANTING:                       return omm_act_panting(m);
        case ACT_COUGHING:                      return omm_act_coughing(m);
        case ACT_SHIVERING:                     return omm_act_shivering(m);
        case ACT_START_SLEEPING:                return omm_act_start_sleeping(m);
        case ACT_SLEEPING:                      return omm_act_sleeping(m);
        case ACT_STANDING_AGAINST_WALL:         return omm_act_standing_against_wall(m);
        case ACT_IN_QUICKSAND:                  return omm_act_in_quicksand(m);
        case ACT_START_CROUCHING:               return omm_act_crouching(m);
        case ACT_CROUCHING:                     return omm_act_crouching(m);
        case ACT_STOP_CROUCHING:                return omm_act_crouching(m);
        case ACT_START_CRAWLING:                return omm_act_crawling(m);
        case ACT_STOP_CRAWLING:                 return omm_act_crawling(m);
        case ACT_BRAKING_STOP:                  return omm_act_braking_stop(m);
        case ACT_JUMP_LAND_STOP:                return omm_act_jump_land_stop(m);
        case ACT_DOUBLE_JUMP_LAND_STOP:         return omm_act_double_jump_land_stop(m);
        case ACT_SIDE_FLIP_LAND_STOP:           return omm_act_side_flip_land_stop(m);
        case ACT_FREEFALL_LAND_STOP:            return omm_act_freefall_land_stop(m);
        case ACT_TRIPLE_JUMP_LAND_STOP:         return omm_act_triple_jump_land_stop(m);
        case ACT_BACKFLIP_LAND_STOP:            return omm_act_backflip_land_stop(m);
        case ACT_LONG_JUMP_LAND_STOP:           return omm_act_long_jump_land_stop(m);
        case ACT_GROUND_POUND_LAND:             return omm_act_ground_pound_land(m);

        // Odyssey
        case ACT_OMM_SHOCKED_FROM_HIGH_FALL:    return omm_act_shocked_from_high_fall(m);
        case ACT_OMM_SPIN_GROUND:               return omm_act_spin_ground(m);
        case ACT_OMM_SPIN_POUND_LAND:           return omm_act_spin_pound_land(m);

        // SPP
        case ACT_OMM_PEACH_ATTACK_GROUND:       return omm_act_peach_attack_ground(m);
        case ACT_OMM_PEACH_PERRY_CHARGE_GROUND: return omm_act_peach_perry_charge_ground(m);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

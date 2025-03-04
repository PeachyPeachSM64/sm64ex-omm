#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//////////
// SM64 //
//////////

static s32 omm_act_stationary_cancels__cappy_roll_punch_spin(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

// m->actionState is the position of Mario's head: 0 = left, 1 = right, 2 = center, 3 = fall asleep (only possible if m->actionTimer >= 10)
// m->actionTimer is the number of completed head turning cycles
static s32 omm_act_idle(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_PUNCHING, 0, RETURN_CANCEL);
    action_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_GROUND, 0, RETURN_CANCEL);
    action_condition(m->quicksandDepth > 30.f, ACT_IN_QUICKSAND, 0, RETURN_CANCEL);
    action_condition(!OMM_PEACH_VIBE_NO_COUGHING && (m->input & INPUT_IN_POISON_GAS), ACT_COUGHING, 0, RETURN_CANCEL);
    action_condition(m->health < 0x300 && !omm_health_is_at_max(m) && !OMM_PEACH_VIBE_NO_PANTING && !(m->actionArg & 1), ACT_PANTING, 0, RETURN_CANCEL);
    action_condition(check_common_idle_cancels(m), 0, 0, RETURN_CANCEL);
    action_condition(m->actionState == 3 && !OMM_PEACH_VIBE_NO_SHIVERING && omm_world_is_cold(), ACT_SHIVERING, 0, RETURN_CANCEL);
    action_condition(m->actionState == 3 && !OMM_PEACH_VIBE_NO_SLEEPING, ACT_START_SLEEPING, 0, RETURN_CANCEL);

    s32 step = stationary_ground_step(m);
    if (step == GROUND_STEP_NONE) {
        if (m->actionArg & 1) {
            ANM(MARIO_ANIM_STAND_AGAINST_WALL, 1.f);
        } else {
            if (OMM_PEACH_VIBE_NO_HEAD_TURN) {
                ANM(MARIO_ANIM_FIRST_PERSON, 1.f);
            } else {
                switch (m->actionState) {
                    case 0: ANM(MARIO_ANIM_IDLE_HEAD_LEFT, 1.f); break;
                    case 1: ANM(MARIO_ANIM_IDLE_HEAD_RIGHT, 1.f); break;
                    case 2: ANM(MARIO_ANIM_IDLE_HEAD_CENTER, 1.f); break;
                }
                if (obj_anim_is_at_end(m->marioObj) && ++m->actionState == 3) {
                    f32 deltaY = abs_f(m->pos[1] - find_floor_height_relative_polar(m, -0x8000, 60.f));
                    if (deltaY > 24.f || (m->floor->flags & SURFACE_FLAG_DYNAMIC) || ++m->actionTimer < (omm_peach_vibe_is_calm() ? 3 : 10)) {
                        m->actionState = 0;
                    }
                }
            }
        }
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_start_sleeping(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_SLEEPING, ACT_WAKING_UP, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_sleeping(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_SLEEPING, ACT_WAKING_UP, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_panting(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_PANTING, ACT_IDLE, 0, RETURN_CANCEL);
    return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
}

static s32 omm_act_coughing(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_COUGHING, ACT_IDLE, 0, RETURN_CANCEL);
    return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
}

static s32 omm_act_shivering(struct MarioState *m) {
    action_condition(OMM_PEACH_VIBE_NO_SHIVERING, ACT_IDLE, 0, RETURN_CANCEL);
    return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
}

static s32 omm_act_crouching_crawling(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_ground_pound_land(struct MarioState *m) {
    action_init(0, 0, 0, 0);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_GROUND_POUND_JUMP, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
}

/////////////
// Odyssey //
/////////////

static s32 omm_act_shocked_from_high_fall(struct MarioState *m) {
    if (m->actionTimer < 3) {
        action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_FORWARD_ROLLOUT, 0, RETURN_CANCEL, PFX(PARTICLE_SPARKLES););
    } else if (m->actionTimer == 3) {
        PFX(PARTICLE_MIST_CIRCLE);
        SFX(SOUND_MARIO_ATTACKED);
        play_mario_heavy_landing_sound(m, SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
        mario_set_forward_vel(m, 0.f);
    }
    m->vel[1] = 0.f;
    m->pos[1] = m->floorHeight = find_floor_height(m->pos[0], m->pos[1], m->pos[2]);
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
    ANM(MARIO_ANIM_SHOCKED, 1.f);
    if (m->actionTimer++ < 30) set_camera_shake_from_hit(SHAKE_SHOCK);
    action_condition(m->actionTimer >= 45, ACT_IDLE, 0, RETURN_BREAK);
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
    ANM(MARIO_ANIM_GENERAL_LAND, 1.f);
    action_condition(obj_anim_is_at_end(m->marioObj), ACT_IDLE, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

////////////////
// Stationary //
////////////////

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
    if (omm_mario_is_dead(m) && !(m->action & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE))) {
        drop_and_set_mario_action(m, ACT_STANDING_DEATH, 0);
        return true;
    }

    return false;
}

s32 omm_mario_execute_stationary_action(struct MarioState *m) {
    gOmmMario->wallSlide.jumped = false;
    gOmmMario->peach.floated = false;
    gOmmMario->cappy.bounced = false;
    gOmmMario->state.airCombo = 0;
    gOmmMario->midairSpin.counter = 0;

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
        PFX(PARTICLE_WAVE_TRAIL);
        m->particleFlags &= ~PARTICLE_DUST;
    }

    // Actions
    switch (m->action) {

        // SM64
        case ACT_IDLE:                          return omm_act_idle(m);
        case ACT_START_SLEEPING:                return omm_act_start_sleeping(m);
        case ACT_SLEEPING:                      return omm_act_sleeping(m);
        case ACT_WAKING_UP:                     return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_PANTING:                       return omm_act_panting(m);
        case ACT_HOLD_IDLE:                     return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLD_HEAVY_IDLE:               return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_STANDING_AGAINST_WALL:         return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_COUGHING:                      return omm_act_coughing(m);
        case ACT_SHIVERING:                     return omm_act_shivering(m);
        case ACT_IN_QUICKSAND:                  return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_CROUCHING:                     return omm_act_crouching_crawling(m);
        case ACT_START_CROUCHING:               return omm_act_crouching_crawling(m);
        case ACT_STOP_CROUCHING:                return omm_act_crouching_crawling(m);
        case ACT_START_CRAWLING:                return omm_act_crouching_crawling(m);
        case ACT_STOP_CRAWLING:                 return omm_act_crouching_crawling(m);
        case ACT_SLIDE_KICK_SLIDE_STOP:         return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_SHOCKWAVE_BOUNCE:              return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_FIRST_PERSON:                  return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_BACKFLIP_LAND_STOP:            return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_JUMP_LAND_STOP:                return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_DOUBLE_JUMP_LAND_STOP:         return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_FREEFALL_LAND_STOP:            return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_SIDE_FLIP_LAND_STOP:           return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_HOLD_JUMP_LAND_STOP:           return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLD_FREEFALL_LAND_STOP:       return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_AIR_THROW_LAND:                return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_TWIRL_LAND:                    return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_LAVA_BOOST_LAND:               return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_TRIPLE_JUMP_LAND_STOP:         return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_LONG_JUMP_LAND_STOP:           return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_GROUND_POUND_LAND:             return omm_act_ground_pound_land(m);
        case ACT_BRAKING_STOP:                  return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_BUTT_SLIDE_STOP:               return omm_act_stationary_cancels__cappy_roll_punch_spin(m);
        case ACT_HOLD_BUTT_SLIDE_STOP:          return OMM_MARIO_ACTION_RESULT_CONTINUE;

        // Odyssey
        case ACT_OMM_SHOCKED_FROM_HIGH_FALL:    return omm_act_shocked_from_high_fall(m);
        case ACT_OMM_SPIN_POUND_LAND:           return omm_act_spin_pound_land(m);

        // Peach
        case ACT_OMM_PEACH_PERRY_CHARGE_GROUND: return omm_act_peach_perry_charge_ground(m);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

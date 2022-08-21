#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//////////
// SM64 //
//////////

static s32 omm_act_punching(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY && OMM_PLAYER_IS_PEACH, ACT_OMM_PEACH_ATTACK_GROUND, ((m->prevAction == ACT_OMM_SPIN_GROUND) ? 4 : 0), RETURN_CANCEL);
    action_zb_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_ROLL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_stomach_slide_stop(struct MarioState *m) {
    if (!m->heldObj && !(m->input & INPUT_ABOVE_SLIDE)) {
        action_a_pressed(OMM_MOVESET_ODYSSEY, m->forwardVel > 0 ? ACT_FORWARD_ROLLOUT : ACT_BACKWARD_ROLLOUT, 0, RETURN_CANCEL);
        action_b_pressed(OMM_MOVESET_ODYSSEY, m->forwardVel > 0 ? ACT_FORWARD_ROLLOUT : ACT_BACKWARD_ROLLOUT, 0, RETURN_CANCEL);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_holding_bowser(struct MarioState *m) {
    static struct Object *sTargetBomb = NULL;
    bool bPressed = (gPlayer1Controller->buttonPressed & B_BUTTON) != 0;
    bool aDown = (gPlayer1Controller->buttonDown & A_BUTTON) != 0;

    // If A is down, B is pressed and Mario spins fast enough, try to locate the nearest bomb
    if (!m->actionState && aDown && bPressed && abs_s(m->angleVel[1]) >= 0xE00) {
        f32 distTarget = LEVEL_BOUNDARY_MAX;
        sTargetBomb = NULL;
        for_each_object_with_behavior(obj, bhvBowserBomb) {
            f32 distToObj = obj_get_horizontal_distance(m->marioObj, obj);
            if (distToObj < distTarget) {
                distTarget = distToObj;
                sTargetBomb = obj;
                m->actionState = 1;
            }
        }
    }

    // If A is still down, hold Bowser until he's aligned with the bomb, then throw him
    if (m->actionState && sTargetBomb) {
        if (aDown) {
            s16 angleToTarget = obj_get_object1_angle_yaw_to_object2(m->marioObj, sTargetBomb);
            if (abs_s((s16) (angleToTarget - m->faceAngle[1])) <= abs_s(m->angleVel[1]) / 2) {
                m->faceAngle[1] = angleToTarget;
                m->input |= INPUT_B_PRESSED;
            } else {
                m->intendedMag = max_f(21.f, m->intendedMag);
                m->twirlYaw = m->intendedYaw;
                m->input &= ~INPUT_B_PRESSED;
            }
        } else {
            m->input |= INPUT_B_PRESSED;
        }
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

#if OMM_GAME_IS_R96X
static s32 omm_act_wario_pile_driver_land(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_GROUND, 0, RETURN_CANCEL);
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_GROUND_POUND_JUMP, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}
#endif

////////////
// Object //
////////////

static bool omm_check_common_object_cancels(struct MarioState *m) {

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
    if (omm_mario_is_dead(m)) {
        drop_and_set_mario_action(m, ACT_STANDING_DEATH, 0);
        return true;
    }

    return false;
}

s32 omm_mario_execute_object_action(struct MarioState *m) {
    gOmmMario->wallSlide.jumped = false;
    gOmmMario->peach.floated = false;
    gOmmMario->cappy.bounced = false;
    gOmmMario->state.airCombo = 0;
    gOmmMario->midairSpin.counter = 0;

    // Cancels
    if (omm_check_common_object_cancels(m)) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Quicksand
    // Needs to be 0 to not apply the sinking twice per frame
    if (mario_update_quicksand(m, 0)) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Actions
    switch (m->action) {

        // SM64
        case ACT_PUNCHING:                  return omm_act_punching(m);
        case ACT_PICKING_UP:                return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_DIVE_PICKING_UP:           return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_STOMACH_SLIDE_STOP:        return omm_act_stomach_slide_stop(m);
        case ACT_PLACING_DOWN:              return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_THROWING:                  return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HEAVY_THROW:               return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_PICKING_UP_BOWSER:         return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLDING_BOWSER:            return omm_act_holding_bowser(m);
        case ACT_RELEASING_BOWSER:          return OMM_MARIO_ACTION_RESULT_CONTINUE;
#if OMM_GAME_IS_R96X
        case ACT_WARIO_PILE_DRIVER_LAND:    return omm_act_wario_pile_driver_land(m);
#endif
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

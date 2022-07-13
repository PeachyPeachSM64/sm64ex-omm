#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Data
//

#define OMM_SPIN_CHECKPOINT(angle) (((u16) (angle)) / 0x4000)
#define OMM_SPIN_INTENDED_NEXT(cp, dir) (((cp) + (dir) + 4) % 4)

static struct Object *sOmmDialogObject = NULL;
static s32 sOmmDialogState;
static s16 sOmmDialogId;
static s16 sOmmDialogTurn;
static bool sOmmDialogChoice;

//
// Actions
//

bool omm_mario_set_action(struct MarioState *m, u32 action, u32 actionArg, u16 buttons) {
    if (action != 0) set_mario_action(m, action, actionArg);
    m->controller->buttonPressed     &= ~buttons;
    if (buttons & A_BUTTON) m->input &= ~INPUT_A_PRESSED;
    if (buttons & B_BUTTON) m->input &= ~INPUT_B_PRESSED;
    if (buttons & Z_TRIG)   m->input &= ~INPUT_Z_PRESSED;
    return true;
}

bool omm_mario_throw_cappy(struct MarioState *m, u32 action, u32 actionArg, u16 buttons) {
    if (omm_cappy_spawn(m) == NULL) return false;
    return omm_mario_set_action(m, action, actionArg, buttons);
}

void omm_mario_init_next_action(struct MarioState *m) {

    // Set Mario's facing direction
    if ((m->controller->stickMag > 32.f) && (
        (m->prevAction == ACT_OMM_WATER_GROUND_POUND_JUMP) ||
        (m->action == ACT_OMM_PEACH_VIBE_JOY_ATTACK) ||
        (m->action == ACT_OMM_CAPPY_THROW_GROUND) ||
        (m->action == ACT_OMM_CAPPY_THROW_AIRBORNE) ||
        (m->action == ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND) ||
        (m->action == ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE) ||
        (m->action == ACT_OMM_METAL_WATER_SPIN_GROUND) ||
        (m->action == ACT_OMM_METAL_WATER_SPIN_JUMP) ||
        (m->action == ACT_OMM_METAL_WATER_GROUND_POUND_JUMP) ||
        (m->action == ACT_OMM_GROUND_POUND_JUMP) ||
        (m->action == ACT_OMM_SPIN_JUMP) ||
        (m->action == ACT_OMM_ROLL && (m->prevAction != ACT_OMM_ROLL && m->prevAction != ACT_OMM_ROLL_AIR)))) {
        m->faceAngle[1] = m->intendedYaw;
    }

    // Update Mario's graphics
    if ((m->action != ACT_SWIMMING_END) &&
        (m->action != ACT_OMM_ROLL) &&
        (m->action != ACT_OMM_ROLL_AIR) &&
        (m->action != ACT_OMM_WATER_GROUND_POUND) &&
        (m->action != ACT_OMM_WATER_DASH) &&
        (m->action != ACT_OMM_CAPPY_THROW_WATER) &&
        (m->action != ACT_FLYING)) {
        vec3s_set(m->faceAngle, 0, m->faceAngle[1], 0);
        vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
    }

    // Init spin
    if ((m->action == ACT_OMM_SPIN_GROUND) ||
        (m->action == ACT_OMM_SPIN_AIR) ||
        (m->action == ACT_OMM_METAL_WATER_SPIN_GROUND) ||
        (m->action == ACT_OMM_METAL_WATER_SPIN_AIR)) {
        gOmmData->mario->spin.timer = OMM_MARIO_SPIN_DURATION;
        gOmmData->mario->spin.yaw = 0;
    } else {
        gOmmData->mario->spin.timer = 0;
        gOmmData->mario->spin.yaw = 0;
    }
}

bool omm_mario_check_wall_slide(struct MarioState *m) {
    return OMM_MOVESET_ODYSSEY &&
           (m->action != ACT_DIVE) &&
           (m->action != ACT_LONG_JUMP) &&
           (m->action != ACT_OMM_METAL_WATER_DIVE) &&
           (m->action != ACT_OMM_METAL_WATER_LONG_JUMP) &&
           (m->heldObj == NULL) &&
           (m->wall != NULL) &&
           (m->wall->type < SURFACE_PAINTING_WOBBLE_A6);
}

bool omm_mario_can_perform_wall_slide(struct MarioState *m) {
    if (m->vel[1] < 0.f && m->forwardVel > 8.f) {
        s16 wallAngle = atan2s(m->wall->normal.z, m->wall->normal.x);
        f32 wallHeight = m->pos[1];
        if (gOmmData->mario->wallSlide.jumped) {
            u16 diffAngle = (u16) abs_s((s32) wallAngle - (s32) gOmmData->mario->wallSlide.angle);
            f32 diffHeight = wallHeight - gOmmData->mario->wallSlide.height;
            if (diffAngle < 0x2000 && diffHeight > 0) {
                return false;
            }
        }
        gOmmData->mario->wallSlide.angle = wallAngle;
        gOmmData->mario->wallSlide.height = wallHeight;
        return true;
    }
    return false;
}

bool omm_mario_try_to_perform_wall_slide(struct MarioState *m) {
    if (omm_mario_check_wall_slide(m)) {
        if (omm_mario_can_perform_wall_slide(m)) {
            omm_mario_set_action(m, ACT_OMM_WALL_SLIDE, 0, 0);
        }
        return true;
    }
    return false; 
}

bool omm_mario_check_and_perform_wall_slide(struct MarioState *m) {
    return omm_mario_check_wall_slide(m) &&
           omm_mario_can_perform_wall_slide(m) &&
           omm_mario_set_action(m, ACT_OMM_WALL_SLIDE, 0, 0);
}

bool omm_mario_is_idling(struct MarioState *m) {
    return (m->action == ACT_IDLE) ||
           (m->action == ACT_START_SLEEPING) ||
           (m->action == ACT_SLEEPING) ||
           (m->action == ACT_WAKING_UP) ||
           (m->action == ACT_PANTING) ||
           (m->action == ACT_HOLD_PANTING_UNUSED) ||
           (m->action == ACT_HOLD_IDLE) ||
           (m->action == ACT_HOLD_HEAVY_IDLE) ||
           (m->action == ACT_STANDING_AGAINST_WALL) ||
           (m->action == ACT_SHIVERING) ||
           (m->action == ACT_FIRST_PERSON) ||
           (m->action == ACT_WATER_IDLE) ||
           (m->action == ACT_HOLD_WATER_IDLE) ||
           (m->action == ACT_METAL_WATER_STANDING) ||
           (m->action == ACT_HOLD_METAL_WATER_STANDING) ||
           (m->action == ACT_READING_AUTOMATIC_DIALOG) ||
           (m->action == ACT_READING_NPC_DIALOG) ||
           (m->action == ACT_READING_SIGN) ||
           (m->action == ACT_TELEPORT_FADE_OUT) ||
           (m->action == ACT_TELEPORT_FADE_IN) ||
           (m->action == ACT_HOLDING_POLE) ||
           (m->action == ACT_TOP_OF_POLE) ||
           (m->action == ACT_LEDGE_GRAB) ||
           (m->action == ACT_OMM_POSSESSION) ||
           (m->action == ACT_OMM_METAL_WATER_IDLE);
}

bool omm_mario_is_punching(struct MarioState *m) {
    return (m->action == ACT_PUNCHING) ||
           (m->action == ACT_MOVE_PUNCHING) ||
           (m->action == ACT_OMM_METAL_WATER_PUNCHING) ||
           (m->action == ACT_OMM_PEACH_ATTACK_GROUND);
}

bool omm_mario_is_kicking(struct MarioState *m) {
    return (m->action == ACT_JUMP_KICK) ||
           (m->action == ACT_OMM_METAL_WATER_JUMP_KICK) ||
           (m->action == ACT_OMM_PEACH_ATTACK_AIR);
}

bool omm_mario_is_ground_pounding(struct MarioState *m) {
    return (m->action == ACT_GROUND_POUND) ||
#if OMM_GAME_IS_R96A
           (m->action == ACT_WARIO_PILE_DRIVER) ||
#endif
           (m->action == ACT_OMM_SPIN_POUND) ||
           (m->action == ACT_OMM_WATER_GROUND_POUND) ||
           (m->action == ACT_OMM_METAL_WATER_GROUND_POUND) ||
           (m->action == ACT_OMM_METAL_WATER_SPIN_POUND);
}

bool omm_mario_is_ground_pound_landing(struct MarioState *m) {
    return (m->action == ACT_GROUND_POUND_LAND) ||
#if OMM_GAME_IS_R96A
           (m->action == ACT_WARIO_PILE_DRIVER_LAND) ||
#endif
           (m->action == ACT_OMM_SPIN_POUND_LAND) ||
           (m->action == ACT_OMM_WATER_GROUND_POUND_LAND) ||
           (m->action == ACT_OMM_METAL_WATER_GROUND_POUND_LAND) ||
           (m->action == ACT_OMM_METAL_WATER_GROUND_POUND_LAND_STOP) ||
           (m->action == ACT_OMM_METAL_WATER_SPIN_POUND_LAND);
}

bool omm_mario_is_roll_landing(struct MarioState *m) {
    return OMM_MOVESET_ODYSSEY &&
           (m->controller->buttonDown & Z_TRIG) &&
           (m->forwardVel >= OMM_MARIO_ROLL_MIN_SPEED) && (
           (m->action == ACT_DIVE_SLIDE) ||
           (m->action == ACT_LONG_JUMP_LAND));
}

bool omm_mario_is_reading(struct MarioState *m) {
    return (m->action == ACT_READING_SIGN) ||
           (m->action == ACT_READING_NPC_DIALOG) ||
           (m->action == ACT_READING_AUTOMATIC_DIALOG);
}

bool omm_mario_is_picking_up(struct MarioState *m) {
    return (m->action == ACT_PICKING_UP) ||
           (m->action == ACT_DIVE_PICKING_UP) ||
           (m->action == ACT_PICKING_UP_BOWSER) ||
           (m->action == ACT_OMM_GRAB_BOWSER);
}

bool omm_mario_is_holding(struct MarioState *m) {
    return (m->action == ACT_HOLD_PANTING_UNUSED) ||
           (m->action == ACT_HOLD_IDLE) ||
           (m->action == ACT_HOLD_HEAVY_IDLE) ||
           (m->action == ACT_HOLD_JUMP_LAND_STOP) ||
           (m->action == ACT_HOLD_FREEFALL_LAND_STOP) ||
           (m->action == ACT_HOLD_BUTT_SLIDE_STOP) ||
           (m->action == ACT_HOLD_WALKING) ||
           (m->action == ACT_HOLD_HEAVY_WALKING) ||
           (m->action == ACT_HOLD_DECELERATING) ||
           (m->action == ACT_HOLD_BEGIN_SLIDING) ||
           (m->action == ACT_HOLD_BUTT_SLIDE) ||
           (m->action == ACT_HOLD_STOMACH_SLIDE) ||
           (m->action == ACT_HOLD_JUMP_LAND) ||
           (m->action == ACT_HOLD_FREEFALL_LAND) ||
           (m->action == ACT_HOLD_QUICKSAND_JUMP_LAND) ||
           (m->action == ACT_HOLD_JUMP) ||
           (m->action == ACT_HOLD_FREEFALL) ||
           (m->action == ACT_HOLD_BUTT_SLIDE_AIR) ||
           (m->action == ACT_HOLD_WATER_JUMP) ||
           (m->action == ACT_HOLD_WATER_IDLE) ||
           (m->action == ACT_HOLD_WATER_ACTION_END) ||
           (m->action == ACT_HOLD_BREASTSTROKE) ||
           (m->action == ACT_HOLD_SWIMMING_END) ||
           (m->action == ACT_HOLD_FLUTTER_KICK) ||
           (m->action == ACT_HOLD_METAL_WATER_STANDING) ||
           (m->action == ACT_HOLD_METAL_WATER_WALKING) ||
           (m->action == ACT_HOLD_METAL_WATER_FALLING) ||
           (m->action == ACT_HOLD_METAL_WATER_FALL_LAND) ||
           (m->action == ACT_HOLD_METAL_WATER_JUMP) ||
           (m->action == ACT_HOLD_METAL_WATER_JUMP_LAND) ||
           (m->action == ACT_HOLDING_POLE) ||
           (m->action == ACT_HOLDING_BOWSER) ||
           (m->action == ACT_OMM_HOLD_BOWSER);
}

bool omm_mario_is_hanging(struct MarioState *m) {
    return (m->action == ACT_START_HANGING) ||
           (m->action == ACT_HANGING) ||
           (m->action == ACT_HANG_MOVING);
}

bool omm_mario_is_stuck_in_ground_after_fall(struct MarioState *m) {
    u16 terrainType = (m->area->terrainType & TERRAIN_MASK);
    return (terrainType == TERRAIN_SNOW || terrainType == TERRAIN_SAND) &&
           (m->floor != NULL) &&
           (m->floor->type != SURFACE_BURNING) &&
           (m->floor->type != SURFACE_HARD) &&
           (m->floor->type != SURFACE_HARD_SLIPPERY) &&
           (m->floor->type != SURFACE_HARD_NOT_SLIPPERY) &&
           (m->floor->type != SURFACE_HARD_VERY_SLIPPERY) &&
          !(m->floor->flags & SURFACE_FLAG_DYNAMIC) &&
           (m->floor->normal.y >= 0.866f); // ~30 deg
}

bool omm_mario_is_star_dancing(struct MarioState *m) {
    return (m->action == ACT_STAR_DANCE_EXIT) ||
           (m->action == ACT_STAR_DANCE_NO_EXIT) ||
           (m->action == ACT_STAR_DANCE_WATER) ||
           (m->action == ACT_FALL_AFTER_STAR_GRAB) ||
           (m->action == ACT_JUMBO_STAR_CUTSCENE) ||
           (m->action == ACT_OMM_STAR_DANCE) ||
           (m->action == ACT_OMM_POSSESSION && gOmmData->mario->capture.timer == 0xFF);
}

bool omm_mario_should_walk(struct MarioState *m) {
    return OMM_PLAYER_IS_PEACH || (max_3_f(4.f, m->intendedMag, m->forwardVel) < 18.f);
}

bool omm_mario_should_run(struct MarioState *m) {
    return !OMM_PLAYER_IS_PEACH && (max_3_f(4.f, m->intendedMag, m->forwardVel) > 22.f);
}

bool omm_mario_has_wing_cap(struct MarioState *m) {
    return OMM_POWER_UPS_IMPROVED &&
           (m->flags & MARIO_WING_CAP) &&
           (m->action != ACT_OMM_POSSESSION);
}

bool omm_mario_has_vanish_cap(struct MarioState *m) {
    return OMM_POWER_UPS_IMPROVED &&
           (m->flags & MARIO_VANISH_CAP) &&
           (m->action != ACT_OMM_POSSESSION);
}

bool omm_mario_has_metal_cap(struct MarioState *m) {
    return OMM_POWER_UPS_IMPROVED &&
           (m->flags & MARIO_METAL_CAP) &&
           (m->action != ACT_OMM_POSSESSION);
}

bool omm_mario_check_dead(struct MarioState *m, s16 hp) {
    if (OMM_LEVEL_NO_WARP(gCurrLevelNum)) {
        return false;
    }

    // Anti-softlock
    if (m->action == ACT_OMM_DEATH ||
        m->action == ACT_OMM_DEATH_WATER ||
        m->action == ACT_OMM_DEATH_FALL ||
        m->action == ACT_OMM_DEATH_FROZEN ||
        m->action == ACT_OMM_DEATH_EATEN_BY_BUBBA ||
        m->action == ACT_OMM_DEATH_QUICKSAND ||
        m->action == ACT_OMM_DEATH_SQUISHED) {
        return false;
    }

    // Non-Stop mode only
    // Getting caught cheating also disable the SMO-style death cutscenes
    if (OMM_STARS_CLASSIC || OMM_SSF_IS_CHEAT_DETECTED) {
        return false;
    }

    // SM64 Death actions
    u32 act_omm_death_water = ((m->area->terrainType & TERRAIN_MASK) == TERRAIN_SNOW ? ACT_OMM_DEATH_FROZEN : ACT_OMM_DEATH_WATER);
    switch (m->action) {
        case ACT_STANDING_DEATH:    omm_mario_set_action(m, ACT_OMM_DEATH, 0, 0xFFFF);                  return true;
        case ACT_ELECTROCUTION:     omm_mario_set_action(m, ACT_OMM_DEATH, 0, 0xFFFF);                  return true;
        case ACT_SUFFOCATION:       omm_mario_set_action(m, ACT_OMM_DEATH, 0, 0xFFFF);                  return true;
        case ACT_DEATH_ON_BACK:     omm_mario_set_action(m, ACT_OMM_DEATH, 0, 0xFFFF);                  return true;
        case ACT_DEATH_ON_STOMACH:  omm_mario_set_action(m, ACT_OMM_DEATH, 0, 0xFFFF);                  return true;
        case ACT_QUICKSAND_DEATH:   omm_mario_set_action(m, ACT_OMM_DEATH_QUICKSAND, 0, 0xFFFF);        return true;
        case ACT_EATEN_BY_BUBBA:    omm_mario_set_action(m, ACT_OMM_DEATH_EATEN_BY_BUBBA, 0, 0xFFFF);   return true;
        case ACT_DROWNING:          omm_mario_set_action(m, act_omm_death_water, 0, 0xFFFF);            return true;
        case ACT_WATER_DEATH:       omm_mario_set_action(m, act_omm_death_water, 0, 0xFFFF);            return true;
    }

    // Default: Mario's health <= OMM_HEALTH_DEAD
    if (hp <= OMM_HEALTH_DEAD) {
        omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, false, 0);
        if (m->squishTimer == 0xFF) {
            omm_mario_set_action(m, ACT_OMM_DEATH_SQUISHED, 0, 0xFFFF);
        } else if ((m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) {
            omm_mario_set_action(m, act_omm_death_water, 0, 0xFFFF);
        } else {
            omm_mario_set_action(m, ACT_OMM_DEATH, 0, 0xFFFF);
        }
        return true;
    }
    return false;
}

bool omm_mario_check_death_warp(struct MarioState *m, s32 warpOp) {
    if (OMM_STARS_NON_STOP && !OMM_LEVEL_NO_WARP(gCurrLevelNum) && !OMM_SSF_IS_CHEAT_DETECTED) {
        switch (warpOp) {
            case WARP_OP_DEATH: {
                return omm_mario_check_dead(m, OMM_HEALTH_DEAD);
            } break;

            case WARP_OP_WARP_FLOOR: {
                if (!OMM_CHEAT_WALK_ON_DEATH_BARRIER &&
                    (m->pos[1] < m->floorHeight + 2048.f) &&
                    (m->floor != NULL) && (
                    (m->floor->type == SURFACE_DEATH_PLANE) ||
                    (m->floor->type == SURFACE_VERTICAL_WIND))) {
                    omm_mario_set_action(m, ACT_OMM_DEATH_FALL, 0, 0);
                    return true;
                }
            } break;
        }
    }
    return false;
}

const bool gIsBowserInteractible[] = {
    1, // bowser_act_default
    1, // bowser_act_thrown_dropped
    0, // bowser_act_jump_onto_stage
    1, // bowser_act_dance
    0, // bowser_act_dead
    0, // bowser_act_text_wait
    0, // bowser_act_intro_walk
    1, // bowser_act_charge_mario
    1, // bowser_act_spit_fire_into_sky
    1, // bowser_act_spit_fire_onto_floor
    1, // bowser_act_hit_edge
    1, // bowser_act_turn_from_edge
    1, // bowser_act_hit_mine
    1, // bowser_act_jump
    1, // bowser_act_walk_to_mario
    1, // bowser_act_breath_fire
    0, // bowser_act_teleport
    1, // bowser_act_jump_towards_mario
    1, // bowser_act_unused_slow_walk
    0, // bowser_act_ride_tilting_platform
};
bool omm_mario_check_grab(struct MarioState *m, struct Object *o, bool ignoreAngles) {
    if (!(o->oInteractionSubtype & INT_SUBTYPE_NOT_GRABBABLE)) {

        // Bowser's tail?
        if (o->behavior == bhvBowserTailAnchor) {
            struct Object *bowser = o->parentObj;

            // Vanilla Bowser?
            if (bowser && !obj_is_dormant(bowser)) {
#if OMM_GAME_IS_R96A
                // Spamba Bowser
                if (bowser->oInteractType == INTERACT_DAMAGE) {
                    return false;
                }
#endif
                // Is Bowser grabbable and is Mario facing Bowser's tail, in a 120 deg arc?
                if (gIsBowserInteractible[bowser->oAction] && (ignoreAngles || abs_s((s16) (m->faceAngle[1] - bowser->oFaceAngleYaw)) < 0x5555)) {
                    gOmmData->mario->grab.obj = bowser;
                    return true;
                }
            }
        }

        // Is Mario on ground?
        else if (!(m->action & ACT_FLAG_AIR)) {

            // Grabbable object?
            if (omm_obj_is_grabbable(o)) {
                s16 deltaYaw = m->faceAngle[1] - ((o->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO) ? o->oFaceAngleYaw : mario_obj_angle_to_object(m, o));

                // Is Mario facing the object, in a 60 deg arc?
                if (ignoreAngles || (-0x2AAA < deltaYaw && deltaYaw < +0x2AAA)) {
                    gOmmData->mario->grab.obj = o;
                    return true;
                }
            }
        }
    }

    // Not a valid grab
    return false;
}

//
// Update
//

void omm_mario_lock_camera(struct MarioState *m, bool isStarCutscene) {
    if (isStarCutscene) {
        gLakituState.posHSpeed = 0.1f;
        gLakituState.posVSpeed = 0.1f;
        gLakituState.focHSpeed = 0.2f;
        gLakituState.focVSpeed = 0.2f;
        gLakituState.goalPos[0] = m->pos[0] + 1024.f * sins(m->faceAngle[1]);
        gLakituState.goalPos[1] = m->pos[1] + 256.f;
        gLakituState.goalPos[2] = m->pos[2] + 1024.f * coss(m->faceAngle[1]);
        gLakituState.goalFocus[0] = m->pos[0];
        gLakituState.goalFocus[1] = m->pos[1];
        gLakituState.goalFocus[2] = m->pos[2];
    } else {
        gLakituState.posHSpeed = 0;
        gLakituState.posVSpeed = 0;
        gLakituState.goalFocus[0] = m->pos[0];
        gLakituState.goalFocus[1] = m->pos[1];
        gLakituState.goalFocus[2] = m->pos[2];
    }
}

bool omm_mario_start_dialog(struct MarioState *m, struct Object *o, s16 dialogId, bool dialogWithChoice, bool freezeObject, s16 angleVelTurnObjectTowardsMario) {
    if ((m->action != ACT_FIRST_PERSON) &&
       !(m->action & ACT_FLAG_RIDING_SHELL) &&
       !(m->action & ACT_FLAG_INVULNERABLE) && (
        (m->action == ACT_WAITING_FOR_DIALOG) ||
        (m->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY ||
        (m->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING)) {
        enable_time_stop();
        if (freezeObject) {
            o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
        } else {
            o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        }
        o->oDialogResponse = 0;
        sOmmDialogObject = o;
        sOmmDialogState = 0;
        sOmmDialogId = dialogId;
        sOmmDialogTurn = angleVelTurnObjectTowardsMario;
        sOmmDialogChoice = dialogWithChoice;
        return true;
    }
    return false;
}

void omm_mario_update_dialog(struct MarioState *m) {
    if (sOmmDialogObject) {
        enable_time_stop();
        switch (sOmmDialogState) {
            case 0: {
                
                // Rotate Mario to the object
                s16 startYaw = m->faceAngle[1];
                m->faceAngle[1] = approach_s16_symmetric(m->faceAngle[1], obj_angle_to_object(m->marioObj, sOmmDialogObject), 0x800);
                bool doneTurning = (m->faceAngle[1] - startYaw) == 0;

                // Rotate the object to Mario
                if (sOmmDialogTurn != 0) {
                    s16 startYaw = (s16) sOmmDialogObject->oFaceAngleYaw;
                    sOmmDialogObject->oFaceAngleYaw = approach_s16_symmetric(sOmmDialogObject->oFaceAngleYaw, obj_angle_to_object(sOmmDialogObject, m->marioObj), sOmmDialogTurn);
                    sOmmDialogObject->oAngleVelYaw = (s16) ((s16) sOmmDialogObject->oFaceAngleYaw - startYaw);
                    doneTurning = (sOmmDialogObject->oAngleVelYaw == 0);
                }

                if (set_mario_npc_dialog(2) == 2 && doneTurning) {
                    sOmmDialogState = 1;
                }
            } break;

            case 1: {
                sOmmDialogObject->oDialogResponse = cutscene_object_with_dialog(sOmmDialogChoice ? CUTSCENE_RACE_DIALOG : CUTSCENE_DIALOG, sOmmDialogObject, sOmmDialogId);
                if (sOmmDialogObject->oDialogResponse != 0) {
                    sOmmDialogState = 2;
                }
            } break;

            case 2: {
                if (m->action != ACT_READING_NPC_DIALOG) {
                    disable_time_stop();
                    sOmmDialogObject->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
                    sOmmDialogObject = NULL;
                } else {
                    set_mario_npc_dialog(0);
                }
            } break;
        }
    }
}

void omm_mario_update_grab(struct MarioState *m) {
    struct Object *o = gOmmData->mario->grab.obj;
    if (o) {
        gOmmData->mario->grab.obj = NULL;
        s16 angleMarioToObject = mario_obj_angle_to_object(m, o);
        m->faceAngle[1] = angleMarioToObject;
        m->interactObj = o;
        m->usedObj = o;
        m->input &= ~INPUT_INTERACT_OBJ_GRABBABLE;
        m->collidedObjInteractTypes &= ~INTERACT_GRABBABLE;

        // Bowser
        if (o->behavior == bhvBowser) {
            o->oFaceAngleYaw = angleMarioToObject;
            o->oIntangibleTimer = 0;
            obj_get_first_with_behavior(bhvBowserTailAnchor)->oAction = 2;
            omm_mario_set_action(m, ACT_PICKING_UP_BOWSER, 0, 0);
        }
        
        // Dive
        else if ((m->action & ACT_FLAG_DIVING) && !(o->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO)) {
            mario_grab_used_object(m);
            omm_mario_set_action(m, ACT_DIVE_PICKING_UP, 0, 0);
        }
        
        // Punch
        else {
            obj_anim_play(m->marioObj, MARIO_ANIM_FIRST_PUNCH, 1.f);
            omm_mario_set_action(m, ACT_PICKING_UP, 0, 0);
        }
    }
}

void omm_mario_update_spin(struct MarioState *m) {
    static u8 sSpinBufferTimer = 0;
    static u8 sSpinNumHitCheckpoints = 0;
    static s8 sSpinCheckpoint = -1;
    static s8 sSpinDirection = 0;
    if (OMM_MOVESET_CLASSIC) {
        return;
    }

    // Update midair spin timer
    if (!OMM_PLAYER_IS_PEACH) {
        if (gOmmData->mario->midairSpin.timer > 0) {
            gOmmData->mario->midairSpin.timer--;
        }
    }

    // Update spin timer
    if (gOmmData->mario->spin.timer > 0) {
        gOmmData->mario->spin.timer--;
    }

    // Update spin buffer
    if (m->controller->stickMag >= 40.f) {
        s8 cp = OMM_SPIN_CHECKPOINT(atan2s(m->controller->stickY, m->controller->stickX));

        // Ignore the cp -1
        if (cp != -1) {

            // We first set the first cp
            if (sSpinCheckpoint == -1) {
                sSpinCheckpoint = cp;
                sSpinNumHitCheckpoints++;
                sSpinBufferTimer = OMM_MARIO_SPIN_BUFFER_DURATION;
            }

            // Then we set the direction
            else if (sSpinDirection == 0) {
                if (cp != sSpinCheckpoint) {
                    if (cp == OMM_SPIN_INTENDED_NEXT(sSpinCheckpoint, -1)) {
                        sSpinCheckpoint = cp;
                        sSpinDirection = -1;
                        sSpinNumHitCheckpoints++;
                        sSpinBufferTimer = OMM_MARIO_SPIN_BUFFER_DURATION;
                    } else if (cp == OMM_SPIN_INTENDED_NEXT(sSpinCheckpoint, +1)) {
                        sSpinCheckpoint = cp;
                        sSpinDirection = +1;
                        sSpinNumHitCheckpoints++;
                        sSpinBufferTimer = OMM_MARIO_SPIN_BUFFER_DURATION;
                    } else {
                        sSpinBufferTimer = 0;
                    }
                }
            }

            // And we check if the hit cp is the intended next
            else if (cp != sSpinCheckpoint) {
                if (cp == OMM_SPIN_INTENDED_NEXT(sSpinCheckpoint, sSpinDirection)) {
                    sSpinCheckpoint = cp;
                    sSpinNumHitCheckpoints++;
                    sSpinBufferTimer = OMM_MARIO_SPIN_BUFFER_DURATION;
                } else {
                    sSpinBufferTimer = 0;
                }
            }
        }
    } else {
        sSpinBufferTimer = 0;
    }

    // Hidden spin shortcut (joystick button or mouse scroll click)
    if (SDL_GameControllerGetButton(SDL_GameControllerOpen(0), SDL_CONTROLLER_BUTTON_LEFTSTICK) || (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE))) {
        sSpinNumHitCheckpoints = OMM_MARIO_SPIN_MIN_HIT_CHECKPOINTS;
    }

    // If Mario has the Improved Vanish cap, press (A) after a Midair Spin or during an Air Spin to perform or extend an Air Spin
    if (omm_mario_has_vanish_cap(m) && (m->prevAction == ACT_OMM_MIDAIR_SPIN || m->action == ACT_OMM_SPIN_AIR) && (m->controller->buttonPressed & A_BUTTON)) {
        sSpinNumHitCheckpoints = OMM_MARIO_SPIN_MIN_HIT_CHECKPOINTS;
    }

    // If we successfully hit OMM_MARIO_SPIN_MIN_HIT_CHECKPOINTS checkpoints in a row, Mario can start spinning
    if (sSpinNumHitCheckpoints == OMM_MARIO_SPIN_MIN_HIT_CHECKPOINTS) {
        gOmmData->mario->spin.timer = OMM_MARIO_SPIN_DURATION;
        sSpinBufferTimer = 0;
    }

    // Update spin buffer timer
    if (sSpinBufferTimer == 0) {
        sSpinNumHitCheckpoints = 0;
        sSpinCheckpoint = -1;
        sSpinDirection = 0;
    } else {
        sSpinBufferTimer--;
    }
}

void omm_mario_update_fall(struct MarioState *m) {
    if (OMM_CHEAT_NO_FALL_DAMAGE) {
        m->peakHeight = m->pos[1];
        gOmmData->mario->state.peakHeight = m->pos[1];
        return;
    }
    if (OMM_MOVESET_ODYSSEY) {
        
        // Airborne
        if ((m->action & ACT_GROUP_MASK) == ACT_GROUP_AIRBORNE) {

            // Set peak height
            if (omm_mario_has_metal_cap(m) ||
                omm_mario_is_ground_pounding(m) ||
                (m->vel[1] >= 0.f) ||
                (m->action == ACT_FLYING) ||
                (m->action == ACT_TWIRLING) ||
                (m->action == ACT_SHOT_FROM_CANNON) ||
                (m->action == ACT_RIDING_SHELL_FALL) ||
                (m->action == ACT_RIDING_SHELL_JUMP) ||
                (m->action == ACT_OMM_SPIN_AIR) ||
                (m->action == ACT_OMM_MIDAIR_SPIN) ||
                (m->action == ACT_OMM_ROLL_AIR) ||
                (m->action == ACT_OMM_WALL_SLIDE) ||
                (m->action == ACT_OMM_PEACH_GLIDE) ||
                (m->action == ACT_OMM_PEACH_PERRY_CHARGE_AIR)) {
                gOmmData->mario->state.peakHeight = m->pos[1];
            }

            // Play the WAAAOOOW sound after falling for a while
            if (!(m->action & ACT_FLAG_INVULNERABLE) && !(m->flags & MARIO_UNKNOWN_18)) {
                if ((gOmmData->mario->state.peakHeight - m->pos[1]) > (OMM_MARIO_FALL_DAMAGE_HEIGHT - 400.f)) {
                    omm_sound_play_character_sound_n64(SOUND_MARIO_WAAAOOOW, m->marioObj->oCameraToObject);
                    m->flags |= MARIO_UNKNOWN_18;
                }
            }

        } else {

            // On ground
            if (((m->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING) ||
                ((m->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY)) {

                // Interrupts Mario's action if he lands on a non-slippery surface
                if (((gOmmData->mario->state.peakHeight - m->pos[1]) > OMM_MARIO_FALL_DAMAGE_HEIGHT) &&
                    (m->health > OMM_HEALTH_ODYSSEY_DEAD) &&
                    (m->vel[1] < -50.0f) &&
                    !mario_floor_is_slippery(m) &&
                    !omm_mario_has_metal_cap(m) &&
                    !omm_mario_is_roll_landing(m) &&
                    !omm_mario_is_ground_pound_landing(m) &&
                    (m->floor->type != SURFACE_BURNING) &&
                    (m->action != ACT_RIDING_SHELL_GROUND) &&
                    (m->action != ACT_OMM_ROLL)) {

                    // Stuck in ground
                    if (omm_mario_is_stuck_in_ground_after_fall(m)) {
                        play_sound(SOUND_MARIO_OOOF2, m->marioObj->oCameraToObject);
                        m->particleFlags |= PARTICLE_MIST_CIRCLE;

                        // Head stuck
                        if ((m->action == ACT_DIVE_SLIDE || m->action == ACT_DIVE_PICKING_UP)) {
                            drop_and_set_mario_action(m, ACT_HEAD_STUCK_IN_GROUND, 0);
                        }

                        // Feet stuck
                        else {
                            drop_and_set_mario_action(m, ACT_FEET_STUCK_IN_GROUND, 0);
                        }
                    }
                    
                    // Shocked
                    else {
                        drop_and_set_mario_action(m, ACT_OMM_SHOCKED_FROM_HIGH_FALL, 0);
                    }
                }
            }

            // Reset peak height (and stop the WAAAOOOW sound)
            gOmmData->mario->state.peakHeight = m->pos[1];
            omm_sound_stop_character_sound_n64(SOUND_MARIO_WAAAOOOW, m->marioObj->oCameraToObject);
        }

        // Update classic peak height
        m->peakHeight = m->pos[1];
    }
}

void omm_mario_update_caps(struct MarioState *m) {

    // Improved Wing cap
    if (omm_mario_has_wing_cap(m)) {
        omm_spawn_wing_glow_and_trail(m->marioObj);
    }

    // Improved Vanish cap
    if (omm_mario_has_vanish_cap(m)) {
        omm_spawn_vanish_mist(m->marioObj);
    }

    // Improved Metal cap
    if (omm_mario_has_metal_cap(m)) {
        if (gGlobalTimer & 1) {
            omm_spawn_metal_sparkle(m->marioObj);
        }
    
        // Cannot die, unless if he's already dead
        if (m->health > OMM_HEALTH_DEAD) {
            m->health = max_s(m->health, OMM_HEALTH_1_HP);
            m->hurtCounter = 0;
        }

        // Destroy objects that try to squish Mario
        if ((m->action == ACT_SQUISHED) &&
            (m->marioObj->oScaleY <= 0.05f) &&
            (m->ceil != NULL) &&
            (m->ceil->object != NULL) &&
            (m->ceil->object->activeFlags != 0)) {
            obj_destroy(m->ceil->object);
            obj_scale(m->marioObj, 1.f);
            m->squishTimer = 0;
            m->input &= ~INPUT_SQUISHED;
            omm_mario_set_action(m, ACT_IDLE, 0, 0);
        }
    }
}

void omm_mario_update_burn(struct MarioState *m) {
    if ((m->health > OMM_HEALTH_DEAD) && (
        (m->action == ACT_BURNING_GROUND) ||
        (m->action == ACT_BURNING_JUMP) ||
        (m->action == ACT_BURNING_FALL))) {
        m->invincTimer = 30;
        m->marioObj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    } else if (m->health <= OMM_HEALTH_DEAD) {
        m->invincTimer = 0;
    }
}

void omm_mario_update_action(struct MarioState *m) {

    // Instant squish: Mario takes damage from some attacks even if not fully squished
    if (m->action == ACT_SQUISHED && m->ceil && m->ceil->object) {
        f32 heightDiff = m->ceilHeight - m->floorHeight;
        f32 hitboxHeight = m->marioObj->hitboxHeight;
        if (m->ceil->object->behavior == bhvEyerokHand && heightDiff < hitboxHeight / 2.f) {
            m->ceilHeight = m->floorHeight;
        }
    }

    // Flooded fail-safe
    if (omm_world_is_flooded() && (
        (m->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY ||
        (m->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING ||
        (m->action & ACT_GROUP_MASK) == ACT_GROUP_AIRBORNE ||
        (m->action == ACT_WARP_DOOR_SPAWN) ||
        (m->action == ACT_EMERGE_FROM_PIPE) ||
        (m->action == ACT_SPAWN_SPIN_AIRBORNE) ||
        (m->action == ACT_SPAWN_SPIN_LANDING) ||
        (m->action == ACT_SPAWN_NO_SPIN_AIRBORNE) ||
        (m->action == ACT_SPAWN_NO_SPIN_LANDING))) {
        omm_mario_set_action(m, ACT_WATER_IDLE, 0, 0);
    }
}

void omm_mario_update_camera_mode(struct MarioState *m) {
    if (!BETTER_CAM_IS_ENABLED && m->area && m->area->camera) {
        
        // Return to default camera when leaving water
        if(((m->area->camera->mode) == CAMERA_MODE_WATER_SURFACE ||
            (m->area->camera->mode) == CAMERA_MODE_BEHIND_MARIO) && (
            (m->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY ||
            (m->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING     ||
            (m->action & ACT_GROUP_MASK) == ACT_GROUP_AIRBORNE   ||
            (m->action & ACT_GROUP_MASK) == ACT_GROUP_CUTSCENE   ||
            (m->action & ACT_GROUP_MASK) == ACT_GROUP_OBJECT)) {
            set_camera_mode(m->area->camera, m->area->camera->defMode, 0);
        }

        // Set close camera if underwater with Metal Mario
        if(((m->area->camera->mode) == CAMERA_MODE_WATER_SURFACE ||
            (m->area->camera->mode) == CAMERA_MODE_BEHIND_MARIO) && (
            (m->action & ACT_GROUP_MASK) == ACT_GROUP_METAL_WATER)) {
            set_camera_mode(m->area->camera, CAMERA_MODE_CLOSE, 0);
        }

        // Return to default camera when leaving water with Metal Mario
        if ((m->area->camera->mode) == CAMERA_MODE_CLOSE &&
            omm_mario_has_metal_cap(m) && !(m->action & ACT_FLAG_METAL_WATER)) {
            set_camera_mode(m->area->camera, m->area->camera->defMode, 0);
        }
    }
}

void omm_mario_update_castle_collisions(struct MarioState *m) {
#if OMM_GAME_IS_SM64
    if (m->floor) {
        s16 room = m->floor->room;

        // Ceiling on Castle 2nd floor
        if ((gCurrLevelNum == LEVEL_CASTLE) && (gCurrAreaIndex == 2) && (room != 6)) {
            if (m->pos[1] > 4000.f) {
                m->pos[1] = 4000.f;
                m->vel[1] = min_f(m->vel[1], 0.f);
            }
        }

        // Truly infinite stairs (only for Odyssey Moveset)
        if ((gCurrLevelNum == LEVEL_CASTLE) && (gCurrAreaIndex == 2) && (room == 6) && (m->numStars < 70) && (m->pos[2] < 800.f) && OMM_MOVESET_ODYSSEY) {

            // Displacement
            struct Surface *floor;
            f32 displacementZ = 410.f;
            f32 floorHeightBefore = find_floor(m->pos[0], m->pos[1] + 80.f, m->pos[2], &floor);
            f32 floorHeightAfter = find_floor(m->pos[0], m->pos[1] + 80.f, m->pos[2] + displacementZ, &floor);
            f32 displacementY = (floorHeightAfter - floorHeightBefore);

            // Warp Mario
            m->pos[1] += displacementY;
            m->pos[2] += displacementZ;
            m->marioObj->oPosY += displacementY;
            m->marioObj->oPosZ += displacementZ;
            m->marioObj->oGfxPos[1] += displacementY;
            m->marioObj->oGfxPos[2] += displacementZ;

            // Warp the camera
            s16 cameraAngle = m->area->camera->yaw;
            change_area(gCurrAreaIndex);
            warp_camera(0, displacementY, displacementZ);
            m->area = gCurrentArea;
            m->area->camera->yaw = cameraAngle;
        }
    }
#else
    OMM_UNUSED(m);
#endif
}

static void omm_mario_update_inputs(struct MarioState *m) {
    update_mario_inputs(m);
    if (OMM_CHEAT_MOON_JUMP && (m->controller->buttonDown & L_TRIG)) {
        m->vel[1] = +30.f;
    }
    if (OMM_CHEAT_INVINCIBLE) {
        m->invincTimer = max_s(m->invincTimer, 1);
        if (m->action == ACT_OMM_POSSESSION) {
            gOmmData->object->state.invincTimer = max_s(gOmmData->object->state.invincTimer, 1);
        }
    }
}

//
// Mario update
//

extern s32 omm_mario_execute_stationary_action(struct MarioState *m);
extern s32 omm_mario_execute_moving_action(struct MarioState *m);
extern s32 omm_mario_execute_airborne_action(struct MarioState *m);
extern s32 omm_mario_execute_submerged_action(struct MarioState *m);
extern s32 omm_mario_execute_cutscene_action(struct MarioState *m);
extern s32 omm_mario_execute_automatic_action(struct MarioState *m);
extern s32 omm_mario_execute_object_action(struct MarioState *m);
extern s32 omm_mario_execute_metal_water_action(struct MarioState *m);

static s32 (*sExecuteOmmActionFunction[(ACT_GROUP_MASK >> 6) + 1])(struct MarioState *) = {
    omm_mario_execute_stationary_action,    // 0x000 Stationary
    omm_mario_execute_moving_action,        // 0x040 Moving
    omm_mario_execute_airborne_action,      // 0x080 Airborne
    omm_mario_execute_submerged_action,     // 0x0C0 Submerged
    omm_mario_execute_cutscene_action,      // 0x100 Cutscene
    omm_mario_execute_automatic_action,     // 0x140 Automatic
    omm_mario_execute_object_action,        // 0x180 Object
    omm_mario_execute_metal_water_action,   // 0x1C0 Metal Water
};

static s32 (*sExecuteSm64ActionFunction[(ACT_GROUP_MASK >> 6) + 1])(struct MarioState *) = {
    mario_execute_stationary_action,        // 0x000 Stationary
    mario_execute_moving_action,            // 0x040 Moving
    mario_execute_airborne_action,          // 0x080 Airborne
    mario_execute_submerged_action,         // 0x0C0 Submerged
    mario_execute_cutscene_action,          // 0x100 Cutscene
    mario_execute_automatic_action,         // 0x140 Automatic
    mario_execute_object_action,            // 0x180 Object
    NULL,                                   // 0x1C0 Metal Water
};

void bhv_mario_update() {
    gOmmData->update();
    struct MarioState *m = gMarioState;
    if (!m->action) {
        return;
    }

    // Update old Cheats flags
#if OMM_GAME_IS_XALO || OMM_GAME_IS_SM74 || OMM_GAME_IS_SMSR
    OMM_MEMSET(&Cheats, 0, sizeof(Cheats));
    Cheats.EnableCheats = gOmmCheatEnable;
    Cheats.Responsive = OMM_CHEAT_SUPER_RESPONSIVE;
    Cheats.WalkOn.Lava = OMM_CHEAT_WALK_ON_LAVA;
    Cheats.WalkOn.Quicksand = OMM_CHEAT_WALK_ON_QUICKSAND;
    Cheats.WalkOn.Slope = OMM_CHEAT_WALK_ON_SLOPE;
    Cheats.WalkOn.DeathBarrier = OMM_CHEAT_WALK_ON_DEATH_BARRIER;
#elif !OMM_GAME_IS_R96A
    Cheats.EnableCheats = gOmmCheatEnable;
    Cheats.Responsive = OMM_CHEAT_SUPER_RESPONSIVE;
#endif

    // Infinite health cheat
    if (OMM_CHEAT_GOD_MODE) {
        if (g1HPMode) {
            m->health = OMM_HEALTH_1_HP;
        } else {
            m->health = OMM_HEALTH_MAX;
        }
    }

    // Reset state
    m->marioObj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    mario_reset_bodystate(m);

    // Inputs
    if (omm_peach_vibe_is_gloom() && m->controller->stickMag != 0.f) {
        m->controller->stickX  *= 64.f / m->controller->stickMag;
        m->controller->stickY  *= 64.f / m->controller->stickMag;
        m->controller->stickMag = 64.f;
    }
    omm_mario_update_inputs(m);
    if (omm_peach_vibe_is_gloom() && ((m->input & INPUT_NONZERO_ANALOG) || (abs_f(m->forwardVel) > 16.f))) {
        m->input &= ~INPUT_ABOVE_SLIDE;
    }
    if (m->input & INPUT_NONZERO_ANALOG) {
        m->intendedYaw = omm_camera_get_intended_yaw(m);
    }

    // Actions
    omm_mario_update_grab(m);
    omm_mario_update_action(m);
    omm_mario_update_spin(m);

    // Interactions
    mario_handle_special_floors(m);
    if (m->action != ACT_OMM_POSSESSION) {
        omm_mario_preprocess_interactions(m);
        mario_process_interactions(m);

        // Mario bounced off an object, apply his jump modifier.
        // Nice side effect: the flag MARIO_UNKNOWN_08 allows the player to
        // control Mario's jump height for his regular jumps by holding down A.
        // Resetting this flag after a bounce allows the player to control
        // the height of the bounce, like in Super Mario Bros. games.
        if (!(m->action & ACT_FLAG_INTANGIBLE) && !(m->flags & MARIO_UNKNOWN_08)) {
            m->vel[1] *= (omm_player_get_selected_jump_multiplier() * ((m->action & ACT_FLAG_CONTROL_JUMP_HEIGHT) ? 1.5f : 1.f));
            m->flags |= MARIO_UNKNOWN_08;
        }
    }

    // Out of Bounds
    if (m->floor == NULL) {
        return;
    }

    // Adjust camera depending on Mario's action
    omm_mario_update_camera_mode(m);

    // Execute actions
    for (;;) {

        // Check if Mario is dead, and change his action if needed
        omm_mario_check_dead(m, m->health);

        // Determine execute functions depending on Mario's action
        s32 group = ((m->action & ACT_GROUP_MASK) >> 6);
        s32 actResult = OMM_MARIO_ACTION_RESULT_CONTINUE;
        s32 (*execOmm)(struct MarioState *) = sExecuteOmmActionFunction[group];
        s32 (*execSm64)(struct MarioState *) = sExecuteSm64ActionFunction[group];

        // Execute OMM actions
        if (execOmm != NULL) {
            actResult = execOmm(m);
        }

        // Execute SM64 actions
        if (actResult == OMM_MARIO_ACTION_RESULT_CONTINUE) {
            if (!(m->action & ACT_FLAG_OMM_ACTION) && (execSm64 != NULL)) {
                actResult = execSm64(m);
            } else {
                actResult = OMM_MARIO_ACTION_RESULT_BREAK;
            }
        }

        // Cancel? -> loop again
        // Break? -> end loop
        if (actResult == OMM_MARIO_ACTION_RESULT_CANCEL) {
            continue;
        } else if (actResult == OMM_MARIO_ACTION_RESULT_BREAK) {
            break;
        }
    }

    // Action post-update
    omm_mario_update_action(m);
    omm_mario_update_fall(m);
    omm_mario_update_caps(m);
    omm_mario_update_dialog(m);
    omm_mario_update_castle_collisions(m);

    // World objects
    omm_world_update(m);

    // Gfx & camera stuff
#if OMM_GAME_IS_R96A
    if (!m->milk) squish_mario_model(m);
#else
    squish_mario_model(m);
#endif
    sink_mario_in_quicksand(m);
    set_submerged_cam_preset_and_spawn_bubbles(m);
    update_mario_info_for_cam(m);
    mario_update_hitbox_and_cap_model(m);
    omm_mario_update_burn(m);

    // Vibes (Peach only)
    if (OMM_PLAYER_IS_PEACH) {
        omm_peach_vibe_update(m);
    }

    // Health
    omm_health_update(m);

    // Cappy
    omm_cappy_update(m);

    // Wind gfx/sfx
    if (m->floor->type == SURFACE_HORIZONTAL_WIND) {
        spawn_wind_particles(0, (m->floor->force << 8));
        play_sound(SOUND_ENV_WIND2, m->marioObj->oCameraToObject);
    } else if (m->floor->type == SURFACE_VERTICAL_WIND) {
        spawn_wind_particles(1, 0);
        play_sound(SOUND_ENV_WIND2, m->marioObj->oCameraToObject);
    }

    // Misc
#if !OMM_GAME_IS_R96A
    play_infinite_stairs_music();
#endif
    m->numLives = max_s(m->numLives, 1);
    m->numStars = save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1);

    // Update Mario object
    obj_set_pos(gMarioObject, m->pos[0], m->pos[1], m->pos[2]);
    obj_set_vel(gMarioObject, m->vel[0], m->vel[1], m->vel[2]);
    obj_set_angle(gMarioObject, gMarioObject->oGfxAngle[0], gMarioObject->oGfxAngle[1], gMarioObject->oGfxAngle[2]);
    obj_set_angle_vel(gMarioObject, m->angleVel[0], m->angleVel[1], m->angleVel[2]);
    gMarioObject->oInteractStatus = 0;
    gMarioObject->oMarioParticleFlags = m->particleFlags;

    // Update Peach's body state
    if (OMM_PLAYER_IS_PEACH) {

        // Torso
        // Between Mario and Beytah
        m->marioBodyState->torsoAngle[0] *= 0.0f;
        m->marioBodyState->torsoAngle[1] *= 1.0f;
        m->marioBodyState->torsoAngle[2] *= 0.5f;

        // Head
        // Move the head up and down if she's crying
        if (m->action == ACT_IDLE && omm_peach_vibe_is_gloom()) {
            f32 tAnim = obj_anim_get_frame(m->marioObj) / m->marioObj->oCurrAnim->mLoopEnd;
            m->marioBodyState->action |= ACT_FLAG_WATER_OR_TEXT;
            m->marioBodyState->headAngle[0] = relerp_f(sins(tAnim * 0x30000), -1.f, +1.f, -0x1000, -0x1800);
            m->marioBodyState->headAngle[1] = 0;
            m->marioBodyState->headAngle[2] = 0;
        }

        // Eyes
        // Each Vibe has a unique expression
        if (omm_peach_vibe_is_joy()) {
            m->marioBodyState->eyeState = 4;
        } else if (omm_peach_vibe_is_rage()) {
            m->marioBodyState->eyeState = 5;
        } else if (omm_peach_vibe_is_gloom()) {
            m->marioBodyState->eyeState = 6;
        } else if (m->marioBodyState->handState == MARIO_HAND_PEACE_SIGN) {
            m->marioBodyState->eyeState = 7;
        }

        // Hands
        // Show Peach's crown or Tiara in her right hand
        if (OMM_EXTRAS_CAPPY_AND_TIARA && m->marioBodyState->handState == MARIO_HAND_HOLDING_CAP) {
            m->marioBodyState->handState = MARIO_HAND_HOLDING_WING_CAP;
        } else if (!OMM_EXTRAS_CAPPY_AND_TIARA && m->marioBodyState->handState == MARIO_HAND_HOLDING_WING_CAP) {
            m->marioBodyState->handState = MARIO_HAND_HOLDING_CAP;
        }

        // Cap
        // Peach's wings are displayed or hidden depending on the Wing cap state
        // Hide the wings if invisible mode is enabled
        if (OMM_EXTRAS_INVISIBLE_MODE) {
            m->marioBodyState->capState = MARIO_HAS_DEFAULT_CAP_OFF;
        } else if (m->flags & MARIO_WING_CAP) {
            m->marioBodyState->capState |= 2;
        } else {
            m->marioBodyState->capState &= ~2;
        }
    }
    
    // Graph node preprocessing
    // Compute Mario's hands, arms, head and root positions
    geo_preprocess_object_graph_node(gMarioObject);

    // Spawn particles
    m->particleFlags &= ~(!enable_dust_particles * (PARTICLE_DUST | PARTICLE_SNOW | PARTICLE_DIRT));
    if (m->particleFlags & PARTICLE_DUST                ) spawn_particle(ACTIVE_PARTICLE_DUST,                 MODEL_MIST,                 bhvMistParticleSpawner);
    if (m->particleFlags & PARTICLE_VERTICAL_STAR       ) spawn_particle(ACTIVE_PARTICLE_V_STAR,               MODEL_NONE,                 bhvVertStarParticleSpawner);
    if (m->particleFlags & PARTICLE_HORIZONTAL_STAR     ) spawn_particle(ACTIVE_PARTICLE_H_STAR,               MODEL_NONE,                 bhvHorStarParticleSpawner);
    if (m->particleFlags & PARTICLE_SPARKLES            ) spawn_particle(ACTIVE_PARTICLE_SPARKLES,             MODEL_SPARKLES,             bhvSparkleParticleSpawner);
    if (m->particleFlags & PARTICLE_BUBBLE              ) spawn_particle(ACTIVE_PARTICLE_BUBBLE,               MODEL_BUBBLE,               bhvBubbleParticleSpawner);
    if (m->particleFlags & PARTICLE_WATER_SPLASH        ) spawn_particle(ACTIVE_PARTICLE_WATER_SPLASH,         MODEL_WATER_SPLASH,         bhvWaterSplash);
    if (m->particleFlags & PARTICLE_IDLE_WATER_WAVE     ) spawn_particle(ACTIVE_PARTICLE_IDLE_WATER_WAVE,      MODEL_IDLE_WATER_WAVE,      bhvIdleWaterWave);
    if (m->particleFlags & PARTICLE_PLUNGE_BUBBLE       ) spawn_particle(ACTIVE_PARTICLE_PLUNGE_BUBBLE,        MODEL_WHITE_PARTICLE_SMALL, bhvPlungeBubble);
    if (m->particleFlags & PARTICLE_WAVE_TRAIL          ) spawn_particle(ACTIVE_PARTICLE_WAVE_TRAIL,           MODEL_WAVE_TRAIL,           bhvWaveTrail);
    if (m->particleFlags & PARTICLE_FIRE                ) spawn_particle(ACTIVE_PARTICLE_FIRE,                 MODEL_RED_FLAME,            bhvFireParticleSpawner);
    if (m->particleFlags & PARTICLE_SHALLOW_WATER_WAVE  ) spawn_particle(ACTIVE_PARTICLE_SHALLOW_WATER_WAVE,   MODEL_NONE,                 bhvShallowWaterWave);
    if (m->particleFlags & PARTICLE_SHALLOW_WATER_SPLASH) spawn_particle(ACTIVE_PARTICLE_SHALLOW_WATER_SPLASH, MODEL_NONE,                 bhvShallowWaterSplash);
    if (m->particleFlags & PARTICLE_LEAF                ) spawn_particle(ACTIVE_PARTICLE_LEAF,                 MODEL_NONE,                 bhvLeafParticleSpawner);
    if (m->particleFlags & PARTICLE_SNOW                ) spawn_particle(ACTIVE_PARTICLE_SNOW,                 MODEL_NONE,                 bhvSnowParticleSpawner);
    if (m->particleFlags & PARTICLE_BREATH              ) spawn_particle(ACTIVE_PARTICLE_BREATH,               MODEL_NONE,                 bhvBreathParticleSpawner);
    if (m->particleFlags & PARTICLE_DIRT                ) spawn_particle(ACTIVE_PARTICLE_DIRT,                 MODEL_NONE,                 bhvDirtParticleSpawner);
    if (m->particleFlags & PARTICLE_MIST_CIRCLE         ) spawn_particle(ACTIVE_PARTICLE_MIST_CIRCLE,          MODEL_NONE,                 bhvMistCircParticleSpawner);
    if (m->particleFlags & PARTICLE_TRIANGLE            ) spawn_particle(ACTIVE_PARTICLE_TRIANGLE,             MODEL_NONE,                 bhvTriangleParticleSpawner);
}

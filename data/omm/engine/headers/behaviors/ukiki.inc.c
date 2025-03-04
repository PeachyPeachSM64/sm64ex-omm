#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

static struct SoundState sUkikiSoundStates[] = {
    { 1, 1, 10, SOUND_OBJ_UKIKI_STEP_DEFAULT  },
    { 0, 0,  0, NO_SOUND                      },
    { 0, 0,  0, NO_SOUND                      },
    { 0, 0,  0, NO_SOUND                      },
    { 1, 0, -1, SOUND_OBJ_UKIKI_CHATTER_SHORT },
    { 1, 0, -1, SOUND_OBJ_UKIKI_CHATTER_LONG  },
    { 0, 0,  0, NO_SOUND                      },
    { 0, 0,  0, NO_SOUND                      },
    { 1, 0, -1, SOUND_OBJ_UKIKI_CHATTER_LONG  },
    { 1, 0, -1, SOUND_OBJ_UKIKI_STEP_LEAVES   },
    { 1, 0, -1, SOUND_OBJ_UKIKI_CHATTER_IDLE  },
    { 0, 0,  0, NO_SOUND                      },
    { 0, 0,  0, NO_SOUND                      },
};

static Trajectory sCageUkikiPath[] = {
    TRAJECTORY_POS(0, 1011, 2306,  -285),
    TRAJECTORY_POS(0, 1151, 2304,  -510),
    TRAJECTORY_POS(0, 1723, 1861,  -964),
    TRAJECTORY_POS(0, 2082, 1775, -1128),
    TRAJECTORY_POS(0, 2489, 1717, -1141),
    TRAJECTORY_POS(0, 2662, 1694, -1140),
    TRAJECTORY_POS(0, 2902, 1536,  -947),
    TRAJECTORY_POS(0, 2946, 1536,  -467),
    TRAJECTORY_POS(0, 2924, 1536,    72),
    TRAJECTORY_POS(0, 2908, 1536,   536),
    TRAJECTORY_POS(0, 2886, 1536,   783),
    TRAJECTORY_END(),
};

static bool is_hat_ukiki_and_mario_has_hat(void) {
    return o->oBehParams2ndByte == UKIKI_HAT && does_mario_have_hat(gMarioState);
}

static void idle_ukiki_taunt(void) {
    o->oForwardVel = 0.f;

    // Select taunt
    if (o->oSubAction == UKIKI_SUB_ACT_TAUNT_NONE) {
        o->oSubAction = 1 + (random_u16() % 4);
        o->oUkikiTauntCounter = 0;
        o->oUkikiTauntsToBeDone = 2 + (random_u16() % 4);
    }

    // Process taunt
    switch(o->oSubAction) {
        case UKIKI_SUB_ACT_TAUNT_ITCH: {
            cur_obj_init_animation_with_sound(UKIKI_ANIM_ITCH);
            if (cur_obj_check_if_near_animation_end()) {
                o->oSubAction = UKIKI_SUB_ACT_TAUNT_NONE;
            }
        } break;

        case UKIKI_SUB_ACT_TAUNT_SCREECH: {
            cur_obj_init_animation_with_sound(UKIKI_ANIM_SCREECH);
            if (cur_obj_check_if_near_animation_end()) {
                o->oUkikiTauntCounter++;
            }
            if (o->oUkikiTauntCounter >= o->oUkikiTauntsToBeDone * 2) {
                o->oSubAction = UKIKI_SUB_ACT_TAUNT_NONE;
            }
        } break;

        case UKIKI_SUB_ACT_TAUNT_JUMP_CLAP: {
            cur_obj_init_animation_with_sound(UKIKI_ANIM_JUMP_CLAP);
            if (cur_obj_check_if_near_animation_end()) {
                o->oUkikiTauntCounter++;
            }
            if (o->oUkikiTauntCounter >= o->oUkikiTauntsToBeDone) {
                o->oSubAction = UKIKI_SUB_ACT_TAUNT_NONE;
            }
        } break;

        case UKIKI_SUB_ACT_TAUNT_HANDSTAND: {
            cur_obj_init_animation_with_sound(UKIKI_ANIM_HANDSTAND);
            if (cur_obj_check_if_near_animation_end()) {
                o->oSubAction = UKIKI_SUB_ACT_TAUNT_NONE;
            }
        } break;
    }
}

//
// Actions
//

void ukiki_act_idle(void) {
    idle_ukiki_taunt();

    // Get closer to Mario
    if (is_hat_ukiki_and_mario_has_hat()) {
        if (o->oDistanceToMario > 700.f && o->oDistanceToMario < 1000.f) {
            o->oAction = UKIKI_ACT_RUN;
        } else if (o->oDistanceToMario <= 700.f && o->oDistanceToMario > 200.f) {
            if (abs_angle_diff(o->oAngleToMario, o->oMoveAngleYaw) > 0x1000) {
                o->oAction = UKIKI_ACT_TURN_TO_MARIO;
            }
        }
    }

    // Flee Mario
    else if (o->oDistanceToMario < 300.f) {
        o->oAction = UKIKI_ACT_RUN;
    }

    // Go to cage
    if (o->oUkikiTextState == UKIKI_TEXT_GO_TO_CAGE) {
        o->oAction = UKIKI_ACT_GO_TO_CAGE;
    }

    // Jump away from Mario
    if (o->oUkikiTextState == UKIKI_TEXT_STOLE_HAT) {
        o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw + 0x8000;
        if (check_if_moving_over_floor(50.f, 150.f)) {
            o->oAction = UKIKI_ACT_JUMP;
        } else {
            o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw + 0x4000;
            if (check_if_moving_over_floor(50.f, 150.f)) {
                o->oAction = UKIKI_ACT_JUMP;
            } else {
                o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw - 0x4000;
                if (check_if_moving_over_floor(50.f, 150.f)) {
                    o->oAction = UKIKI_ACT_JUMP;
                }
            }
        }
        o->oUkikiTextState = UKIKI_TEXT_HAS_HAT;
    }
}

void ukiki_act_run(void) {
    s32 fleeMario = TRUE;
    s16 goalYaw = o->oAngleToMario + 0x8000;
    if (o->oTimer == 0) {
        o->oUkikiChaseFleeRange = random_float() * 100.f + 350.f;
    }

    // Get closer to Mario
    if (is_hat_ukiki_and_mario_has_hat()) {
        fleeMario = FALSE;
        goalYaw = o->oAngleToMario;
    }

    cur_obj_init_animation_with_sound(UKIKI_ANIM_RUN);
    cur_obj_rotate_yaw_toward(goalYaw, 0x800);
    cur_obj_set_vel_from_mario_vel(20.f, 0.9f);

    // Turn to Mario if far or close enough
    if (( fleeMario && o->oDistanceToMario > o->oUkikiChaseFleeRange) ||
        (!fleeMario && o->oDistanceToMario < o->oUkikiChaseFleeRange)) {
        o->oAction = UKIKI_ACT_TURN_TO_MARIO;
    }

    // Jump away from Mario if near a wall/edge
    if (fleeMario && o->oDistanceToMario < 200.f && is_mario_moving_fast_or_in_air(10)) {
        if (o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
            o->oAction = UKIKI_ACT_JUMP;
            o->oMoveAngleYaw = o->oWallAngle;
        } else if (o->oMoveFlags & OBJ_MOVE_HIT_EDGE) {
            o->oAction = UKIKI_ACT_JUMP;
            o->oMoveAngleYaw += 0x8000;
        }
    }
}

void ukiki_act_turn_to_mario(void) {
    if (o->oTimer == 0) {
        o->oForwardVel = random_float() * 3.f + 2.f;
    }

    cur_obj_init_animation_with_sound(UKIKI_ANIM_TURN);

    // Return to idle if facing Mario
    if (cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x800)) {
        o->oAction = UKIKI_ACT_IDLE;
    }

    // Run towards or from Mario if far or close enough
    if (is_hat_ukiki_and_mario_has_hat()) {
        if (o->oDistanceToMario > 500.f) {
            o->oAction = UKIKI_ACT_RUN;
        }
    } else if (o->oDistanceToMario < 300.f) {
        o->oAction = UKIKI_ACT_RUN;
    }
}

void ukiki_act_jump(void) {
    o->oForwardVel = 10.f;

    // Jumping
    if (o->oSubAction == 0) {
        if (o->oTimer == 0) {
            cur_obj_set_y_vel_and_animation(random_float() * 10.f + 45.f, UKIKI_ANIM_JUMP);
        } else if (o->oMoveFlags & (OBJ_MOVE_MASK_ON_GROUND | OBJ_MOVE_AT_WATER_SURFACE | OBJ_MOVE_UNDERWATER_ON_GROUND)) {
            o->oSubAction++;
            o->oVelY = 0.f;
        }
    }

    // Landing
    else {
        o->oForwardVel = 0.f;
        cur_obj_init_animation_with_sound(UKIKI_ANIM_LAND);
        if (cur_obj_check_if_near_animation_end()) {
            o->oAction = UKIKI_ACT_RUN;
        }
    }
}

void ukiki_act_go_to_cage(void) {
    struct Object *cage = cur_obj_nearest_object_with_behavior(bhvUkikiCage);

    // Unload cage Ukiki if there is no cage
    if (!cage) {
        obj_mark_for_deletion(o);
        return;
    }

    // Process sub-action
    switch(o->oSubAction) {
        case UKIKI_SUB_ACT_CAGE_RUN_TO_CAGE: {
            cur_obj_init_animation_with_sound(UKIKI_ANIM_RUN);
            o->oFlags |= OBJ_FLAG_ACTIVE_FROM_AFAR;

            // Select nearest waypoint from current position
            if (!o->oPathedStartWaypoint) {
                o->oPathedStartWaypoint = (struct Waypoint *) sCageUkikiPath;
                struct Waypoint *nearest = obj_path_get_nearest_waypoint(o, o->oPathedStartWaypoint, 250.f);
                if (nearest) {
                    o->oPathedPrevWaypoint = (struct Waypoint *) max((uintptr_t) o->oPathedStartWaypoint, (uintptr_t) (nearest - 1));
                    o->oPathedPrevWaypointFlags = WAYPOINT_FLAGS_INITIALIZED;
                }
            }

            // Follow path
            if (cur_obj_follow_path(0) != PATH_REACHED_END) {
                o->oForwardVel = 10.f;
                cur_obj_rotate_yaw_toward(o->oPathedTargetYaw, 0x400);
                o->oPosY = o->oFloorHeight;
            } else {
                o->oForwardVel = 0.f;
                o->oSubAction++;
            }
        } break;

        case UKIKI_SUB_ACT_CAGE_WAIT_FOR_MARIO: {
            cur_obj_init_animation_with_sound(UKIKI_ANIM_JUMP_CLAP);
            cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x400);
            if (cur_obj_can_mario_activate_textbox(200.f, 30.f, 0x7FFF)) {
                o->oSubAction++; // fallthrough
            } else {
                break;
            }
        };

        case UKIKI_SUB_ACT_CAGE_TALK_TO_MARIO: {
            cur_obj_become_intangible();
            cur_obj_init_animation_with_sound(UKIKI_ANIM_HANDSTAND);
            if (cur_obj_update_dialog_with_cutscene(3, 1, CUTSCENE_DIALOG, DIALOG_080)) {
                o->oSubAction++;
            }
        } break;

        case UKIKI_SUB_ACT_CAGE_TURN_TO_CAGE: {
            cur_obj_init_animation_with_sound(UKIKI_ANIM_RUN);
            if (cur_obj_rotate_yaw_toward(obj_angle_to_object(o, cage), 0x400)) {
                o->oForwardVel = 10.f;
                o->oSubAction++;
            }
        } break;

        case UKIKI_SUB_ACT_CAGE_JUMP_TO_CAGE: {
            cur_obj_init_animation_with_sound(UKIKI_ANIM_JUMP);
            obj_set_forward_and_y_vel(o, 0.f, 0.f);
            vec3f_copy(&o->oPrevPosX, &o->oPosX);
            o->oSubAction++;
            o->oTimer = 0;
        } break;

        case UKIKI_SUB_ACT_CAGE_LAND_ON_CAGE: {
            obj_set_forward_and_y_vel(o, 0.f, 0.f);
            Vec3f p0 = {
                o->oPrevPosX,
                o->oPrevPosY,
                o->oPrevPosZ,
            };
            Vec3f p2 = {
                cage->oPosX,
                cage->oPosY + cage->oScaleY * 100.f,
                cage->oPosZ,
            };
            Vec3f p1 = {
                (p0[0] + p2[0]) / 2.f,
                max_f(p0[1], p2[1]) + 300.f,
                (p0[2] + p2[2]) / 2.f,
            };
            f32 t = clamp_0_1_f(o->oTimer / 25.f);
            vec3f_interpolate3(&o->oPosX, t, p0, 0.f, p1, 0.5f, p2, 1.f);
            obj_set_angle(o, 0, atan2s(p2[2] - p0[2], p2[0] - p0[0]), 0);
            if (o->oTimer >= 25) {
                audio_play_puzzle_jingle();
                cur_obj_init_animation_with_sound(UKIKI_ANIM_JUMP_CLAP);
                o->oSubAction++;
                o->oUkikiCageSpinTimer = 32;
                cage->oUkikiCageNextAction = UKIKI_CAGE_ACT_SPIN;
            }
        } break;

        case UKIKI_SUB_ACT_CAGE_SPIN_ON_CAGE: {
            o->oMoveAngleYaw += 0x800;
            o->oUkikiCageSpinTimer--;
            if (o->oUkikiCageSpinTimer < 0) {
                o->oSubAction++;
                cage->oUkikiCageNextAction = UKIKI_CAGE_ACT_FALL;
            }
        } break;

        case UKIKI_SUB_ACT_CAGE_DESPAWN: {
            if (cage->oAction == UKIKI_CAGE_ACT_HIDE) {
                obj_mark_for_deletion(o);
            }
        } break;
    }
}

void ukiki_act_wait_to_respawn(void) {
    idle_ukiki_taunt();
    if (cur_obj_mario_far_away()) {
        cur_obj_set_pos_to_home_and_stop();
        o->oAction = UKIKI_ACT_IDLE;
    }
}

void ukiki_act_unused_turn(void) {
    idle_ukiki_taunt();
    if (o->oSubAction == UKIKI_SUB_ACT_TAUNT_JUMP_CLAP) {
        cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x400);
    }
}

//
// Free
//

void (*sUkikiActions[])(void) = {
    ukiki_act_idle,
    ukiki_act_run,
    ukiki_act_turn_to_mario,
    ukiki_act_jump,
    ukiki_act_go_to_cage,
    ukiki_act_wait_to_respawn,
    ukiki_act_unused_turn,
};

void ukiki_free_loop(void) {
    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sUkikiActions);
    cur_obj_move_standard(-88);
    if (o->oBehParams2ndByte == UKIKI_HAT && cur_obj_mario_far_away()) {
        cur_obj_set_pos_to_home_and_stop();
        o->oAction = UKIKI_ACT_IDLE;
    }
    if(!(o->oMoveFlags & OBJ_MOVE_MASK_IN_WATER)) {
        exec_anim_sound_state(sUkikiSoundStates);
    }
}

//
// Held
//

void cage_ukiki_held_loop(void) {
    switch(o->oUkikiTextState) {
        case UKIKI_TEXT_DEFAULT: {
            if (!(gMarioState->action & ACT_FLAG_AIR) && set_mario_npc_dialog(2) == 2) {
                create_dialog_box_with_response(DIALOG_079);
                o->oUkikiTextState = UKIKI_TEXT_CAGE_TEXTBOX;
            }
        } break;

        case UKIKI_TEXT_CAGE_TEXTBOX: {
            if (gDialogResponse != 0) {
                set_mario_npc_dialog(0);
                if (gDialogResponse == 1) {
                    o->oInteractionSubtype |= INT_SUBTYPE_DROP_IMMEDIATELY;
                    o->oUkikiTextState = UKIKI_TEXT_GO_TO_CAGE;
                } else {
                    o->oUkikiTextState = UKIKI_TEXT_DO_NOT_LET_GO;
                    o->oUkikiTextboxTimer = 60;
                }
            }
        } break;

        case UKIKI_TEXT_GO_TO_CAGE: {
        } break;

        case UKIKI_TEXT_DO_NOT_LET_GO: {
            if (o->oUkikiTextboxTimer-- < 0) {
                o->oUkikiTextState = UKIKI_TEXT_DEFAULT;
            }
        } break;
    }
}

void hat_ukiki_held_loop(void) {
    switch(o->oUkikiTextState) {
        case UKIKI_TEXT_DEFAULT: {
            if (mario_lose_cap_to_enemy(2)) {
                o->oUkikiTextState = UKIKI_TEXT_STEAL_HAT;
                o->oUkikiHasHat |= UKIKI_HAT_ON;
            }
        } break;

        case UKIKI_TEXT_STEAL_HAT: {
            if (!(gMarioState->action & ACT_FLAG_AIR) && cur_obj_update_dialog(2, 2, DIALOG_100, 0)) {
                o->oInteractionSubtype |= INT_SUBTYPE_DROP_IMMEDIATELY;
                o->oUkikiTextState = UKIKI_TEXT_STOLE_HAT;
            }
        } break;

        case UKIKI_TEXT_STOLE_HAT: {
        } break;

        case UKIKI_TEXT_HAS_HAT: {
            if (!(gMarioState->action & ACT_FLAG_AIR) && cur_obj_update_dialog(2, 18, DIALOG_101, 0)) {
                mario_retrieve_cap();
                set_mario_npc_dialog(0);
                o->oUkikiHasHat &= ~UKIKI_HAT_ON;
                o->oUkikiTextState = UKIKI_TEXT_GAVE_HAT_BACK;
            }
        } break;

        case UKIKI_TEXT_GAVE_HAT_BACK: {
            o->oUkikiTextState = UKIKI_TEXT_DEFAULT;
            o->oAction = UKIKI_ACT_IDLE;
        } break;
    }
}

//
// Update
//

void bhv_ukiki_init(void) {
}

void bhv_ukiki_loop(void) {
    switch(o->oHeldState) {
        case HELD_FREE: {
            o->oUkikiTextboxTimer = 0;
            ukiki_free_loop();
        } break;

        case HELD_HELD: {
            cur_obj_unrender_and_reset_state(UKIKI_ANIM_HELD, 0);
            obj_copy_pos(o, gMarioObject);
            if (o->oBehParams2ndByte == UKIKI_HAT) {
                hat_ukiki_held_loop();
            } else {
                cage_ukiki_held_loop();
            }
        } break;

        case HELD_THROWN:
        case HELD_DROPPED: {
            cur_obj_get_dropped();
        } break;
    }
    if (o->oUkikiHasHat & UKIKI_HAT_ON) {
        o->oAnimState = UKIKI_ANIM_STATE_HAT_ON;
    } else {
        o->oAnimState = UKIKI_ANIM_STATE_DEFAULT;
    }
    o->oInteractStatus = 0;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

extern void omm_act_roll_update_gfx(struct MarioState *m);

static f32 omm_get_initial_upwards_velocity(struct MarioState *m, f32 max) {
    static const struct { u32 action; s32 inc; f32 apply; } OMM_MARIO_AIR_COMBO_ACTIONS[] = {
        { ACT_DIVE,                     -1, 0 },
        { ACT_JUMP_KICK,                +1, 1 },
        { ACT_RIDING_SHELL_JUMP,        +1, 1 },
        { ACT_RIDING_SHELL_FALL,        +1, 1 },
        { ACT_OMM_SPIN_AIR,             +1, 1 },
        { ACT_OMM_MIDAIR_SPIN,           0, 1 },
        { ACT_OMM_CAPPY_BOUNCE,         -1, 0 },
        { ACT_OMM_CAPPY_VAULT,          -1, 0 },
        { ACT_OMM_CAPPY_THROW_AIRBORNE, +1, 1 },
    };
    for (s32 i = 0; i != array_length(OMM_MARIO_AIR_COMBO_ACTIONS); ++i) {
        if (m->action == OMM_MARIO_AIR_COMBO_ACTIONS[i].action) {
            gOmmMario->state.airCombo = max_s(0, gOmmMario->state.airCombo + OMM_MARIO_AIR_COMBO_ACTIONS[i].inc);
            return max * (1.f - (OMM_MARIO_AIR_COMBO_ACTIONS[i].apply * clamp_0_1_f((gOmmMario->state.airCombo - 6) / 6.f)));
        }
    }
    gOmmMario->state.airCombo = 0;
    return max;
}

static void omm_play_jump_sound_and_particles(struct MarioState *m) {
    switch (m->terrainSoundAddend >> 16) {
        case SOUND_TERRAIN_WATER: PFX(PARTICLE_SHALLOW_WATER_SPLASH); break;
        case SOUND_TERRAIN_SAND:  PFX(PARTICLE_DIRT); break;
        case SOUND_TERRAIN_SNOW:  PFX(PARTICLE_SNOW); break;
    }
    if (m->flags & MARIO_METAL_CAP) {
        obj_play_sound(m->marioObj, SOUND_ACTION_METAL_JUMP);
    } else {
        obj_play_sound(m->marioObj, SOUND_ACTION_TERRAIN_JUMP + m->terrainSoundAddend);
    }
}

static void omm_act_flying_set_flying_camera(struct MarioState *m) {
    if (m->area->camera->mode != CAMERA_MODE_BEHIND_MARIO) {
        if (BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM) {
            m->area->camera->mode = BETTER_CAM_MODE;
            gLakituState.mode = BETTER_CAM_MODE;
        } else {
            set_camera_mode(m->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
        }
    }
}

static void omm_act_flying_set_default_camera(struct MarioState *m) {
    if (m->area->camera->mode == CAMERA_MODE_BEHIND_MARIO) {
        if (BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM) {
            m->area->camera->mode = BETTER_CAM_MODE;
            gLakituState.mode = BETTER_CAM_MODE;
        } else {
            set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
        }
    }
}

static void omm_act_wall_slide_cancel(struct MarioState *m, f32 forwardVel, f32 upwardsVel, bool setJumpAngle) {
    if (setJumpAngle) {
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
    }
    mario_set_forward_vel(m, forwardVel);
    m->vel[1] = upwardsVel * omm_player_physics_get_selected_jump();
    gOmmMario->wallSlide.height = m->pos[1] - 200.f;
    gOmmMario->wallSlide.jumped = true;
    gOmmMario->midairSpin.counter = 0;
    gOmmMario->state.airCombo = 0;
}

static void omm_act_thrown_step(struct MarioState *m, u32 landAction, u32 bonkAction, s32 animation, f32 speed) {
    mario_set_forward_vel(m, speed);
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE: {
            ANM(animation, 1.f);
            if (animation == MARIO_ANIM_AIR_FORWARD_KB) {
                s16 pitch = min_s(0x1800, atan2s(m->forwardVel, -m->vel[1]));
                m->marioObj->header.gfx.angle[0] = pitch + 0x1800;
            }
        } break;

        case AIR_STEP_LANDED: {
            omm_mario_set_action(m, landAction, m->hurtCounter, 0);
        } break;

        case AIR_STEP_HIT_WALL: {
            mario_bonk_reflection(m, TRUE);
            omm_mario_set_action(m, bonkAction, 0, 0);
            PFX(PARTICLE_VERTICAL_STAR);
        } break;

        case AIR_STEP_HIT_LAVA_WALL: {
            lava_boost_on_wall(m);
        } break;
    }
}

//////////
// SM64 //
//////////

static s32 omm_act_airborne_cancels__cappy_pound_kick_spins(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_triple_jump(struct MarioState *m) {
    if (!m->actionState) {
        if (OMM_MOVESET_ODYSSEY) {
            m->vel[1] = (
                (64.f + 0.25f * (m->forwardVel / 0.8f)) *
                ((m->squishTimer != 0 || m->quicksandDepth > 1.f) ? 0.5f : 1.f) *
                omm_player_physics_get_selected_jump()
            );
        }
        obj_anim_play_with_sound(m->marioObj, MARIO_ANIM_TRIPLE_JUMP, 1.f, 0, true);
        m->actionState = 1;
    }
    return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
}

static s32 omm_act_steep_jump(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY, ACT_JUMP, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_wall_kick_air(struct MarioState *m) {
    action_init_no_y_vel(m->forwardVel, 0, 0, obj_anim_play_with_sound(m->marioObj, MARIO_ANIM_SLIDEJUMP, 1.f, 0, true););
    return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
}

static s32 omm_act_long_jump(struct MarioState *m) {
#if !OMM_GAME_IS_R96X
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

static s32 omm_act_water_jump(struct MarioState *m, bool hold) {
    if (OMM_MOVESET_CLASSIC) return OMM_MARIO_ACTION_RESULT_CONTINUE;

    // States
    static const struct { u32 endAction; s32 anim; s32 sound; } sOmmWaterJumpStates[] = {
        { ACT_JUMP_LAND,        MARIO_ANIM_SINGLE_JUMP,         SOUND_MARIO_YAH_WAH_HOO                   }, // Regular
        { ACT_DOUBLE_JUMP_LAND, MARIO_ANIM_TRIPLE_JUMP,         SOUND_MARIO_YAHOO_WAHA_YIPPEE + (2 << 16) }, // Super
        { ACT_HOLD_JUMP_LAND,   MARIO_ANIM_JUMP_WITH_LIGHT_OBJ, SOUND_MARIO_YAH_WAH_HOO                   }, // Hold
    };
    s32 state = (hold ? 2 : (m->actionArg == 2 ? 1 : 0));
    action_init_no_y_vel(
        m->forwardVel, 0,
        sOmmWaterJumpStates[state].sound + ((random_u16() % 3) << 16),
        obj_anim_play_with_sound(m->marioObj, sOmmWaterJumpStates[state].anim, 1.f, SOUND_ACTION_UNKNOWN432, true);
    );

    // Cancels
    action_condition(hold && (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT), ACT_FREEFALL, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_cappy(!hold, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(1, ACT_GROUND_POUND, 0, RETURN_CANCEL, mario_stop_riding_and_holding(m););
    action_b_pressed(!hold, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_b_pressed(hold, ACT_AIR_THROW, 0, RETURN_CANCEL);
    action_midair_spin(!hold, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(!hold, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
    action_condition(m->actionTimer > 30, hold ? ACT_HOLD_FREEFALL : ACT_FREEFALL, 0, RETURN_CANCEL);

    // Step
    if (m->vel[1] <= 0.f) omm_mario_update_air_without_turn(m);
    s32 step = perform_air_step(m, AIR_STEP_CHECK_LEDGE_GRAB);
    action_condition(step == AIR_STEP_LANDED, sOmmWaterJumpStates[state].endAction, 0, RETURN_BREAK, set_camera_mode(m->area->camera, m->area->camera->defMode, 1););
    action_condition(step == AIR_STEP_HIT_WALL && !hold && omm_mario_check_and_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK, set_camera_mode(m->area->camera, m->area->camera->defMode, 1););
    action_condition(step == AIR_STEP_GRABBED_LEDGE && !hold, ACT_LEDGE_GRAB, 0, RETURN_BREAK, ANM(MARIO_ANIM_IDLE_ON_LEDGE, 1.f); set_camera_mode(m->area->camera, m->area->camera->defMode, 1););
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    m->actionTimer = (m->vel[1] <= 0.f) * (m->actionTimer + 1);

    // Animation
    if (state == 1) {
        play_flip_sounds(m, 2, 8, 20);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_dive(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) action_init(32.f, omm_get_initial_upwards_velocity(m, 40.f), PARTICLE_MIST_CIRCLE, 0);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_flying_triple_jump(struct MarioState *m) {
    m->input &= ~(OMM_MOVESET_ODYSSEY * INPUT_B_PRESSED);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_flying(struct MarioState *m) {
    action_cappy(OMM_POWER_UPS_CLASSIC || !(m->controller->buttonDown & B_BUTTON), 0, 0, NO_RETURN, SFX(SOUND_MARIO_YAHOO_WAHA_YIPPEE + ((2 + (random_u16() % 3)) << 16)););
    action_z_pressed(1, ACT_GROUND_POUND, 1, RETURN_CANCEL, omm_act_flying_set_default_camera(m););
    action_condition(!(m->flags & MARIO_WING_CAP), ACT_FREEFALL, 0, RETURN_CANCEL, omm_act_flying_set_default_camera(m););
    if (!omm_mario_has_wing_cap(m)) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    // Improved Wing cap
    omm_act_flying_set_flying_camera(m);

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
            PFX(PARTICLE_DUST);
        }
        m->faceAngle[2] *= (m->forwardVel / OMM_MARIO_WING_FLYING_MAX_SPEED);
        ANM(MARIO_ANIM_FALL_FROM_SLIDE_KICK, 1.f);
    }

    // Flying
    else {
        m->forwardVel = min_f(abs_f(m->forwardVel) + 1.f, OMM_MARIO_WING_FLYING_MAX_SPEED * omm_player_physics_get_selected_air());
        ANM(MARIO_ANIM_FORWARD_SPINNING_FLIP, 1.f);
        if (obj_anim_is_past_frame(m->marioObj, 1)) {
            SFX(SOUND_ACTION_SPIN);
        }
    }

    // Velocity
    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
    m->slideVelX = m->vel[0];
    m->slideVelZ = m->vel[2];

    // Perform step
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED: {
            m->faceAngle[0] = 0;
            omm_mario_set_action(m, ACT_DIVE_SLIDE, 0, 0);
            ANM(MARIO_ANIM_DIVE, 1.f);
            obj_anim_set_frame(m->marioObj, 7);
            omm_act_flying_set_default_camera(m);
        } return OMM_MARIO_ACTION_RESULT_BREAK;

        case AIR_STEP_HIT_WALL: {
            if (m->wall) {
                m->faceAngle[0] = 0;
                m->vel[1] = min_f(0.f, m->vel[1]);
                mario_set_forward_vel(m, -16.f);
                SFX((m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_BONK : SOUND_ACTION_BONK);
                PFX(PARTICLE_VERTICAL_STAR);
                omm_mario_set_action(m, ACT_BACKWARD_AIR_KB, 0, 0);
                omm_act_flying_set_default_camera(m);
                return OMM_MARIO_ACTION_RESULT_BREAK;
            }
        } break;

        case AIR_STEP_HIT_LAVA_WALL: {
            lava_boost_on_wall(m);
        } return OMM_MARIO_ACTION_RESULT_BREAK;
    }

    // Gfx
    m->marioObj->oGfxAngle[0] = -m->faceAngle[0];
    m->marioObj->oGfxAngle[2] = +m->faceAngle[2];

    // Sound
    SFX(SOUND_MOVING_FLYING);
    sound_set_moving_speed(1, 0x40 * (abs_f(m->forwardVel) / OMM_MARIO_WING_FLYING_MAX_SPEED));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_riding_shell_air(struct MarioState *m) {
    if (!OMM_MOVESET_ODYSSEY) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    if (m->actionArg == 0xFF) { action_init(m->forwardVel * 0.85f, omm_get_initial_upwards_velocity(m, 24.f), 0, 0, m->action = ACT_RIDING_SHELL_FALL; ); }
    else { action_init_no_y_vel(m->forwardVel * 0.9f, 0, 0, m->vel[1] *= 1.15f;); }
    action_z_pressed(1, ACT_FREEFALL, 0, RETURN_CANCEL, mario_stop_riding_object(m););
    action_cappy(1, m->action, 0xFF, NO_RETURN);

    // Perform step
    omm_mario_update_air_with_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_RIDING_SHELL_GROUND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL, 0, 0, NO_RETURN, mario_set_forward_vel(m, 0););

    // Animation
    if (m->actionArg != 0xFF) {
        ANM(MARIO_ANIM_JUMP_RIDING_SHELL, 1.f);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_vertical_wind(struct MarioState *m) {
    action_a_pressed(OMM_PLAYER_IS_PEACH && !omm_mario_is_milk(m), ACT_OMM_PEACH_GLIDE, 0, RETURN_CANCEL);
    action_condition(OMM_MOVESET_ODYSSEY && m->actionTimer > 15, ACT_FREEFALL, 0, RETURN_CANCEL);
    m->actionTimer = (m->actionTimer + 1) * (m->vel[1] < 0 && m->floor->type != SURFACE_VERTICAL_WIND);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_hold_jump(struct MarioState *m) {
    bool heavyHold = (OMM_MOVESET_ODYSSEY && !OMM_PLAYER_IS_WARIO && m->heldObj && (m->heldObj->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO));
    action_init_no_y_vel(m->forwardVel, 0, 0, m->vel[1] *= (heavyHold ? 0.9f : 1.f););
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_twirling(struct MarioState *m) {
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_POUND, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_rollout(struct MarioState *m, bool backwards) {
    action_init(m->forwardVel, 30.f, 0, 0, m->marioObj->oAnimID = -1; omm_play_jump_sound_and_particles(m););

    // Spin
    if (m->actionState == 1) {
        ANM(backwards ? MARIO_ANIM_BACKWARD_SPINNING : MARIO_ANIM_FORWARD_SPINNING, 1.f);
        if (m->actionTimer++ == 4) SFX(SOUND_ACTION_SPIN);
        if (backwards ? obj_anim_is_past_frame(m->marioObj, 2) : obj_anim_is_at_end(m->marioObj)) m->actionState = 2;
    }

    // Fall
    if (m->actionState == 2) {
        ANM(MARIO_ANIM_GENERAL_FALL, 1.f);
    }

    // Step
    update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_NONE && OMM_MOVESET_ODYSSEY && m->actionState == 2, ACT_FREEFALL, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_LANDED, ACT_FREEFALL_LAND_STOP, 0, RETURN_BREAK, SFX(SOUND_ACTION_TERRAIN_LANDING););
    action_condition(step == AIR_STEP_HIT_WALL, 0, 0, RETURN_BREAK, mario_set_forward_vel(m, 0.f););
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_air_hit_wall(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC || omm_mario_is_dead(m)) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    mario_drop_held_object(m);
    if (m->forwardVel >= 32.f) {
        PFX(PARTICLE_VERTICAL_STAR);
        omm_mario_set_action(m, ACT_BACKWARD_AIR_KB, 0, 0);
    } else {
        mario_set_forward_vel(m, -m->forwardVel);
        omm_mario_set_action(m, ACT_SOFT_BONK, 0, 0);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
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

    // Rage Vibe makes the ground pound faster
    if (omm_peach_vibe_is_rage()) {
        m->actionTimer += 1;
        m->marioObj->oAnimFrame += 1;
        m->marioObj->oAnimInfo.animFrameAccelAssist += ANIM_ACCEL_ONE;
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_air_throw(struct MarioState *m) {
    if (m->heldObj) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
}

static s32 omm_act_jump_kick(struct MarioState *m) {
    if (m->actionState == 0) {
        if (OMM_MOVESET_ODYSSEY && OMM_PERRY_IS_AVAILABLE) {
            m->vel[1] = omm_get_initial_upwards_velocity(m, 24.f * omm_player_physics_get_selected_jump());
        } else {
            s32 rainbowSpin = OMM_MOVESET_ODYSSEY && omm_cappy_get_object();
            m->vel[1] = omm_get_initial_upwards_velocity(m, (20.f + 10.f * rainbowSpin) * omm_player_physics_get_selected_jump());
            m->actionArg = rainbowSpin;
        }
        m->actionState = 1;
        m->actionTimer = 0;
        m->marioObj->oAnimID = -1;
        SFX(SOUND_MARIO_PUNCH_HOO);
    }

    // Cancels
    action_condition(OMM_MOVESET_ODYSSEY && OMM_PERRY_IS_AVAILABLE, ACT_OMM_PEACH_ATTACK_AIR, 0, RETURN_CANCEL);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(omm_mario_has_wing_cap(m), ACT_FLYING, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY && (m->actionTimer >= 16), ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    m->controller->buttonPressed &= ~B_BUTTON;
    m->actionTimer++;

    // Vanilla
    if (OMM_MOVESET_CLASSIC && !m->actionArg) {
        ANM(MARIO_ANIM_AIR_KICK, 1.f);
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    // Step
    update_air_without_turn(m);
    s32 step = perform_air_step(m, OMM_MOVESET_ODYSSEY * AIR_STEP_CHECK_LEDGE_GRAB);
    action_condition(step == AIR_STEP_LANDED, ACT_FREEFALL_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_GRABBED_LEDGE, ACT_LEDGE_GRAB, 0, RETURN_BREAK, ANM(MARIO_ANIM_IDLE_ON_LEDGE, 1.f););
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_check_and_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);

    // Cappy twirl
    if (m->actionArg) {
        if (OMM_EXTRAS_SMO_ANIMATIONS) {
            ANM(MARIO_ANIM_OMM_CAPPY_RAINBOW_SPIN, 1.f);
            m->flags |= MARIO_KICKING * (m->marioObj->oAnimFrame < 20);
        } else {
            ANM(MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN, 1.5f);
            obj_anim_clamp_frame(m->marioObj, 74, 94);
            m->flags |= MARIO_KICKING * (m->marioObj->oAnimFrame < 94);
        }
        m->marioBodyState->punchState = 0;
    }

    // Kick
    else {
        ANM(MARIO_ANIM_AIR_KICK, 1.f);
        if (obj_anim_is_past_frame(m->marioObj, 0)) m->marioBodyState->punchState = (2 << 6) | 6;
        m->flags |= MARIO_KICKING * (m->marioObj->oAnimFrame < 8);
    }

    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_crazy_box_bounce(struct MarioState *m) {
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_AIR_THROW, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_special_triple_jump(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY, ACT_TRIPLE_JUMP, 0, RETURN_CANCEL, gSpecialTripleJump = 0;);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_knockback_air(struct MarioState *m, f32 forwardVel, f32 upwardsVel, u32 landAction, s32 animID, bool isForward) {
    if (OMM_MOVESET_CLASSIC || omm_mario_is_dead(m)) {
        return OMM_MARIO_ACTION_RESULT_CONTINUE;
    }

    // Init
    action_init(forwardVel, upwardsVel, 0, 0);

    // Knockback sound
    if (random_u16() & 1) {
        play_sound_if_no_flag(m, SOUND_MARIO_DOH, MARIO_MARIO_SOUND_PLAYED);
    } else {
        play_sound_if_no_flag(m, SOUND_MARIO_UH, MARIO_MARIO_SOUND_PLAYED);
    }

    // Bonks from ledge grabs are shorter
    if (m->prevAction == ACT_LEDGE_GRAB ||
        m->prevAction == ACT_LEDGE_CLIMB_SLOW_1 ||
        m->prevAction == ACT_LEDGE_CLIMB_SLOW_2 ||
        m->prevAction == ACT_LEDGE_CLIMB_DOWN ||
        m->prevAction == ACT_LEDGE_CLIMB_FAST) {
        m->actionTimer = max_s(m->actionTimer, 15);
    }

    // Perform step
    mario_set_forward_vel(m, m->forwardVel * 0.95f);
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE: {
            ANM(animID, 1.f);
            if (m->actionTimer++ >= 30 + (15 * isForward)) {
                ANM(MARIO_ANIM_GENERAL_FALL, 1.f);
                omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
            }
        } break;

        case AIR_STEP_LANDED: {
            omm_mario_set_action(m, landAction, m->actionArg, 0);
        } break;

        case AIR_STEP_HIT_WALL: {
            ANM(MARIO_ANIM_BACKWARD_AIR_KB, 1.f);
            mario_bonk_reflection(m, FALSE);
            mario_set_forward_vel(m, -m->forwardVel);
        } break;

        case AIR_STEP_HIT_LAVA_WALL: {
            lava_boost_on_wall(m);
        } break;
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_burning_air(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        action_condition(m->marioObj->oMarioBurnTimer > 160, ACT_FREEFALL, 0, RETURN_CANCEL);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_lava_boost(UNUSED struct MarioState *m) {
    gOmmMario->cappy.bounced = false;
    gOmmMario->state.airCombo = 0;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_getting_blown(struct MarioState *m) {
    m->actionTimer = 0; // Mario's cap sticks to his head
    action_condition(OMM_MOVESET_ODYSSEY && m->vel[1] < 0.f && ++m->actionArg > 45, ACT_FREEFALL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_thrown(struct MarioState *m, u32 landAction, u32 bonkAction, s32 animation) {
    if (OMM_MOVESET_ODYSSEY) {
        action_condition(OMM_MOVESET_ODYSSEY && m->vel[1] < 0.f && ++m->actionTimer > 30, ACT_FREEFALL, 0, RETURN_CANCEL);
        play_sound_if_no_flag(m, SOUND_MARIO_WAAAOOOW, MARIO_MARIO_SOUND_PLAYED);
        omm_act_thrown_step(m, landAction, bonkAction, animation, m->forwardVel);
        m->forwardVel *= 0.98f;
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

#if OMM_GAME_IS_R96X
static s32 omm_act_wario_triple_jump(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}
#endif

/////////////
// Odyssey //
/////////////

static s32 omm_act_wall_slide(struct MarioState *m) {
    action_condition(!m->wall, ACT_FREEFALL, 0, RETURN_CANCEL, SFX(SOUND_MARIO_UH););
    action_a_pressed(OMM_MOVESET_ODYSSEY, ACT_WALL_KICK_AIR, 0, RETURN_CANCEL, omm_act_wall_slide_cancel(m, 24.f, 52.f, 1););
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_FREEFALL, 0, RETURN_CANCEL, omm_act_wall_slide_cancel(m, -8.f, 0.f, 0); SFX(SOUND_MARIO_UH););
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL, omm_act_wall_slide_cancel(m, 8.f, 0.f, 1););

    mario_set_forward_vel(m, 0);
    ANM(MARIO_ANIM_START_WALLKICK, 1.f);
    SFX(SOUND_MOVING_TERRAIN_SLIDE + m->terrainSoundAddend);
    PFX(PARTICLE_DUST);

    // Cling Mario to the wall before performing the air step,
    // to avoid missing slightly slanted walls (normal.y near 0, but not 0)
    m->pos[0] -= m->wall->normal.x * 4.f;
    m->pos[2] -= m->wall->normal.z * 4.f;
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_IDLE, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_NONE, ACT_FREEFALL, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_should_stop_wall_slide(m), ACT_FREEFALL, 0, RETURN_BREAK);

    // Turn Mario away from the wall
    m->marioObj->oGfxAngle[1] = atan2s(m->wall->normal.z, m->wall->normal.x);
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_cappy_bounce(struct MarioState *m) {
    action_init(m->forwardVel * 0.85f, omm_get_initial_upwards_velocity(m, 58.f), 0, SOUND_MARIO_HOOHOO);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);

    s32 step = common_air_action_step(m, ACT_DOUBLE_JUMP_LAND, (m->vel[1] >= 0.f ? MARIO_ANIM_DOUBLE_JUMP_RISE : MARIO_ANIM_DOUBLE_JUMP_FALL), AIR_STEP_CHECK_LEDGE_GRAB);
    action_condition(step != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_cappy_vault(struct MarioState *m) {
    action_init(m->forwardVel * 0.85f, omm_get_initial_upwards_velocity(m, 70.f), 0, SOUND_MARIO_YAHOO_WAHA_YIPPEE + ((random_u16() % 5) << 16), obj_anim_play_with_sound(m->marioObj, MARIO_ANIM_TRIPLE_JUMP, 1.f, 0, true););
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);

    if (OMM_EXTRAS_SMO_ANIMATIONS) {
        s32 step = common_air_action_step(m, ACT_DOUBLE_JUMP_LAND, MARIO_ANIM_OMM_CAPPY_VAULT, AIR_STEP_CHECK_LEDGE_GRAB);
        action_condition(step != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
        if (obj_anim_is_past_frame(m->marioObj, 6)) SFX(SOUND_ACTION_SIDE_FLIP_UNK);
    } else {
        s32 step = common_air_action_step(m, ACT_DOUBLE_JUMP_LAND, MARIO_ANIM_TRIPLE_JUMP, AIR_STEP_CHECK_LEDGE_GRAB);
        action_condition(step != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
        play_flip_sounds(m, 2, 8, 20);
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_ground_pound_jump(struct MarioState *m) {
    action_init(0, 67.f, 0, SOUND_MARIO_YAHOO, gOmmMario->spin.yaw = 1; omm_play_jump_sound_and_particles(m););
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);

    if (m->vel[1] >= 42.f) {
        s32 step = perform_air_step(m, 0);
        action_condition(step == AIR_STEP_LANDED, ACT_JUMP_LAND, 0, RETURN_BREAK);
    } else {
        s32 step = common_air_action_step(m, ACT_JUMP_LAND, MARIO_ANIM_SINGLE_JUMP, AIR_STEP_CHECK_LEDGE_GRAB);
        action_condition(step != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
    }

    ANM(MARIO_ANIM_SINGLE_JUMP, 1.f);
    obj_anim_clamp_frame(m->marioObj, 0, 9); // Luigi's freaking flutter jump (again)
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += (0x80 * m->vel[1]) * (prevSpinYaw != 0) * (omm_mario_has_vanish_cap(m) ? 0.8f : 1.f) / sqr_f(omm_player_physics_get_selected_jump());
    gOmmMario->spin.yaw *= ((u16) prevSpinYaw < (u16) gOmmMario->spin.yaw) * (m->vel[1] > 0.f);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] + gOmmMario->spin.yaw;
    PFX(m->vel[1] > 0.f ? PARTICLE_DUST : 0);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_leave_object_jump(struct MarioState *m) {
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);

    s32 step = common_air_action_step(m, ACT_JUMP_LAND, MARIO_ANIM_SINGLE_JUMP, AIR_STEP_CHECK_LEDGE_GRAB);
    action_condition(step != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
    PFX(PARTICLE_SPARKLES);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_cappy_throw_airborne(struct MarioState *m) {
    action_init(min_f(m->forwardVel, 8.f), omm_get_initial_upwards_velocity(m, 16.f), 0, 0);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
    action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);

    s32 step = common_air_action_step(m, ACT_FREEFALL_LAND, m->marioObj->oAnimID, 0);
    action_condition(step != AIR_STEP_NONE, 0, 0, RETURN_BREAK);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_roll_air(struct MarioState *m) {
    if (!(m->controller->buttonDown & Z_TRIG)) {
        action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
        action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
        action_midair_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_MIDAIR_SPIN, 0, RETURN_CANCEL);
        action_air_spin(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_AIR, 0, RETURN_CANCEL);
        action_condition(m->forwardVel < 45.f, ACT_FREEFALL, 0, RETURN_CANCEL);
    }

    update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_OMM_ROLL, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL, ACT_BACKWARD_AIR_KB, 0, RETURN_BREAK, PFX(PARTICLE_VERTICAL_STAR););

    f32 speed = m->forwardVel / 60.f;
    s16 prevAngle = m->faceAngle[0];
    m->faceAngle[0] += (s16) (0x1C00 * speed);
    ANM(MARIO_ANIM_FORWARD_SPINNING, 0.01f);
    obj_anim_set_frame(m->marioObj, 0);
    omm_act_roll_update_gfx(m);
    if (prevAngle > m->faceAngle[0]) SFX(SOUND_ACTION_TWIRL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_spin_air(struct MarioState *m) {
    action_init(m->forwardVel, omm_get_initial_upwards_velocity(m, 12.f), 0, 0);
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);
    action_condition(gOmmMario->spin.timer == 0, ACT_FREEFALL, 0, RETURN_CANCEL);

    update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_JUMP_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_check_wall_slide(m) && omm_mario_can_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK);

    ANM(MARIO_ANIM_TWIRL, 1.f);
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += min_s(0x4714, 0x400 * gOmmMario->spin.timer);
    if (gOmmMario->spin.yaw < prevSpinYaw) SFX(SOUND_ACTION_SPIN);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmMario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_spin_jump(struct MarioState *m) {
    action_init(m->forwardVel, 45.f, 0, SOUND_MARIO_YAHOO_WAHA_YIPPEE + ((2 + (random_u16() % 3)) << 16), omm_play_jump_sound_and_particles(m););
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_OMM_SPIN_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);

    update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_JUMP_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(m->vel[1] <= 0.f, ACT_FREEFALL, 0, RETURN_BREAK);

    ANM(MARIO_ANIM_TWIRL, 1.f);
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += min_s(0x5EDC, 0x400 * m->vel[1]);
    if (gOmmMario->spin.yaw < prevSpinYaw) SFX(SOUND_ACTION_SPIN);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmMario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    PFX(PARTICLE_SPARKLES);
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

    ANM(MARIO_ANIM_TWIRL, 1.f);
    s16 prevSpinYaw = gOmmMario->spin.yaw;
    gOmmMario->spin.yaw += 0x4000;
    if (gOmmMario->spin.yaw < prevSpinYaw) SFX(SOUND_ACTION_SIDE_FLIP_UNK);
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - gOmmMario->spin.yaw;
    m->marioBodyState->handState = MARIO_HAND_OPEN;
    PFX(PARTICLE_DUST);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_midair_spin(struct MarioState *m) {
    action_init(m->forwardVel, omm_get_initial_upwards_velocity(m, 0.f), 0, 0,
        m->actionTimer = 0;
        gOmmMario->midairSpin.timer = 24;
        gOmmMario->midairSpin.counter++;
        omm_sound_play(OMM_SOUND_EFFECT_MIDAIR_SPIN, m->marioObj->oCameraToObject);
    );
    action_cappy(1, ACT_OMM_CAPPY_THROW_AIRBORNE, 0, RETURN_CANCEL);
    action_z_pressed(OMM_MOVESET_ODYSSEY, ACT_GROUND_POUND, 0, RETURN_CANCEL);
    action_b_pressed(OMM_MOVESET_ODYSSEY, ACT_JUMP_KICK, 0, RETURN_CANCEL);

    update_air_without_turn(m);
    s32 step = perform_air_step(m, 0);
    action_condition(step == AIR_STEP_LANDED, ACT_JUMP_LAND, 0, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_LAVA_WALL && lava_boost_on_wall(m), ACT_LAVA_BOOST, 1, RETURN_BREAK);
    action_condition(step == AIR_STEP_HIT_WALL && omm_mario_check_wall_slide(m) && omm_mario_can_perform_wall_slide(m), ACT_OMM_WALL_SLIDE, 0, RETURN_BREAK);

    if (m->actionTimer == 8) {
        if (m->prevAction == ACT_DOUBLE_JUMP) {
            ANM(MARIO_ANIM_DOUBLE_JUMP_FALL, 1.f);
            m->prevAction = m->action;
            m->action = ACT_DOUBLE_JUMP;
            m->actionArg = 0;
            m->actionState = 1;
            m->actionTimer = 1;
            m->flags |= (MARIO_ACTION_SOUND_PLAYED | MARIO_MARIO_SOUND_PLAYED);
        } else {
            ANM(MARIO_ANIM_GENERAL_FALL, 1.f);
            omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
        }
        return OMM_MARIO_ACTION_RESULT_BREAK;
    } else {
        ANM(MARIO_ANIM_TWIRL, 1.f);
        m->marioObj->oGfxAngle[1] = m->faceAngle[1] - (s16) (++m->actionTimer * 0x2000);
        m->marioBodyState->handState = MARIO_HAND_FISTS;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

//////////////
// Airborne //
//////////////

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
    m->quicksandDepth = 0.f;

    // Cancels
    if (omm_check_common_airborne_cancels(m)) {
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Actions
    switch (m->action) {

        // SM64
        case ACT_JUMP:                          return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
        case ACT_DOUBLE_JUMP:                   return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
        case ACT_TRIPLE_JUMP:                   return omm_act_triple_jump(m);
        case ACT_BACKFLIP:                      return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
        case ACT_STEEP_JUMP:                    return omm_act_steep_jump(m);
        case ACT_WALL_KICK_AIR:                 return omm_act_wall_kick_air(m);
        case ACT_SIDE_FLIP:                     return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
        case ACT_LONG_JUMP:                     return omm_act_long_jump(m);
        case ACT_WATER_JUMP:                    return omm_act_water_jump(m, false);
        case ACT_DIVE:                          return omm_act_dive(m);
        case ACT_FREEFALL:                      return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
        case ACT_TOP_OF_POLE_JUMP:              return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
        case ACT_BUTT_SLIDE_AIR:                return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
        case ACT_FLYING_TRIPLE_JUMP:            return omm_act_flying_triple_jump(m);
        case ACT_SHOT_FROM_CANNON:              return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_FLYING:                        return omm_act_flying(m);
        case ACT_RIDING_SHELL_JUMP:             return omm_act_riding_shell_air(m);
        case ACT_RIDING_SHELL_FALL:             return omm_act_riding_shell_air(m);
        case ACT_VERTICAL_WIND:                 return omm_act_vertical_wind(m);
        case ACT_HOLD_JUMP:                     return omm_act_hold_jump(m);
        case ACT_HOLD_FREEFALL:                 return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLD_BUTT_SLIDE_AIR:           return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_HOLD_WATER_JUMP:               return omm_act_water_jump(m, true);
        case ACT_TWIRLING:                      return omm_act_twirling(m);
        case ACT_FORWARD_ROLLOUT:               return omm_act_rollout(m, false);
        case ACT_AIR_HIT_WALL:                  return omm_act_air_hit_wall(m);
        case ACT_RIDING_HOOT:                   return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_GROUND_POUND:                  return omm_act_ground_pound(m);
        case ACT_SLIDE_KICK:                    return omm_act_airborne_cancels__cappy_pound_kick_spins(m);
        case ACT_AIR_THROW:                     return omm_act_air_throw(m);
        case ACT_JUMP_KICK:                     return omm_act_jump_kick(m);
        case ACT_BACKWARD_ROLLOUT:              return omm_act_rollout(m, true);
        case ACT_CRAZY_BOX_BOUNCE:              return omm_act_crazy_box_bounce(m);
        case ACT_SPECIAL_TRIPLE_JUMP:           return omm_act_special_triple_jump(m);
        case ACT_BACKWARD_AIR_KB:               return omm_act_knockback_air(m, -20, 20, ACT_BACKWARD_GROUND_KB, MARIO_ANIM_BACKWARD_AIR_KB, false);
        case ACT_FORWARD_AIR_KB:                return omm_act_knockback_air(m, +20, 20, ACT_FORWARD_GROUND_KB, MARIO_ANIM_AIR_FORWARD_KB, true);
        case ACT_HARD_BACKWARD_AIR_KB:          return omm_act_knockback_air(m, -20, 20, ACT_HARD_BACKWARD_GROUND_KB, MARIO_ANIM_BACKWARD_AIR_KB, false);
        case ACT_HARD_FORWARD_AIR_KB:           return omm_act_knockback_air(m, +20, 20, ACT_HARD_FORWARD_GROUND_KB, MARIO_ANIM_AIR_FORWARD_KB, true);
        case ACT_BURNING_JUMP:                  return omm_act_burning_air(m);
        case ACT_BURNING_FALL:                  return omm_act_burning_air(m);
        case ACT_SOFT_BONK:                     return omm_act_knockback_air(m, m->forwardVel, 0, ACT_FREEFALL_LAND, MARIO_ANIM_GENERAL_FALL, false);
        case ACT_LAVA_BOOST:                    return omm_act_lava_boost(m);
        case ACT_GETTING_BLOWN:                 return omm_act_getting_blown(m);
        case ACT_THROWN_FORWARD:                return omm_act_thrown(m, ACT_FORWARD_GROUND_KB, ACT_BACKWARD_AIR_KB, MARIO_ANIM_AIR_FORWARD_KB);
        case ACT_THROWN_BACKWARD:               return omm_act_thrown(m, ACT_BACKWARD_GROUND_KB, ACT_FORWARD_AIR_KB, MARIO_ANIM_BACKWARD_AIR_KB);
#if OMM_GAME_IS_R96X
        case ACT_WARIO_TRIPLE_JUMP:             return omm_act_wario_triple_jump(m);
#endif

        // Odyssey
        case ACT_OMM_WALL_SLIDE:                return omm_act_wall_slide(m);
        case ACT_OMM_CAPPY_BOUNCE:              return omm_act_cappy_bounce(m);
        case ACT_OMM_CAPPY_VAULT:               return omm_act_cappy_vault(m);
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

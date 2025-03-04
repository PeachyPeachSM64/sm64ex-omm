#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Capture Data
//

bool omm_capture_init(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return false;
    return data->init(o);
}

void omm_capture_end(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return;
    return data->end(o);
}

s32 omm_capture_update(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0;
    omm_capture_reset_squish(o);
    return data->update(o);
}

void omm_capture_update_gfx(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return;
    o->oTransparency = 0;
    data->update_gfx(o);
    o->oGfxPos[1] += omm_capture_get_hitbox_down_offset(o);
    omm_capture_apply_squish(o);
}

u64 omm_capture_get_type(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0;
    return data->get_type(o);
}

f32 omm_capture_get_top(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->get_top(o);
}

f32 omm_capture_get_walk_speed(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->walkSpeed * data->scale_modifier(o->oScaleX);
}

f32 omm_capture_get_run_speed(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->runSpeed * data->scale_modifier(o->oScaleX);
}

f32 omm_capture_get_dash_speed(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->dashSpeed * data->scale_modifier(o->oScaleX);
}

f32 omm_capture_get_jump_velocity(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->jumpVelocity * data->scale_modifier(o->oScaleY);
}

f32 omm_capture_get_terminal_velocity(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->terminalVelocity * data->scale_modifier(o->oScaleY);
}

f32 omm_capture_get_gravity(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->gravity * data->scale_modifier(o->oScaleY);
}

f32 omm_capture_get_hitbox_radius(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->hitboxRadius * o->oScaleX;
}

f32 omm_capture_get_hitbox_height(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->hitboxHeight * o->oScaleY;
}

f32 omm_capture_get_hitbox_down_offset(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->hitboxDownOffset * o->oScaleY;
}

f32 omm_capture_get_wall_hitbox_radius(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return 0.f;
    return data->wallHitboxRadius * o->oScaleX;
}

bool omm_capture_should_reference_object(struct Object *o) {
    const OmmBhvDataCapture *data = (const OmmBhvDataCapture *) o->oCaptureData;
    if (!data) return false;
    return data->referenceObject;
}

void omm_capture_apply_squish(struct Object *o) {
    if (gOmmObject->state._squishTimer > 0 && vec3f_eq(gOmmObject->state._squishScale, gVec3fZero)) {
        obj_scale_as_vec3f(o, gOmmObject->state._squishScale);
        f32 ceilHeight = find_ceil(o->oPosX, o->oPosY, o->oPosZ, NULL);
        f32 distToCeil = ceilHeight - o->oPosY;
        f32 squishMult = clamp_f(distToCeil / omm_capture_get_top(o), 0.05f, 1.f);
        obj_set_scale(o,
            o->oScaleX * (2.f - squishMult),
            o->oScaleY * squishMult,
            o->oScaleZ * (2.f - squishMult)
        );
    }
}

void omm_capture_reset_squish(struct Object *o) {
    if (!vec3f_eq(gOmmObject->state._squishScale, gVec3fZero)) {
        obj_set_scale_vec3f(o, gOmmObject->state._squishScale);
        vec3f_zero(gOmmObject->state._squishScale);
    }
}

void omm_capture_set_camera_behind_mario() {
    gOmmObject->state._camBehindMario = true;
    if (gMarioState->area->camera->mode != CAMERA_MODE_BEHIND_MARIO) {
        if (BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM) {
            gMarioState->area->camera->mode = BETTER_CAM_MODE;
            gLakituState.mode = BETTER_CAM_MODE;
        } else {
            set_camera_mode(gMarioState->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
        }
    }
}

void omm_capture_reset_camera() {
    gOmmObject->state._camBehindMario = false;
    if (gMarioState->area->camera->mode == CAMERA_MODE_BEHIND_MARIO) {
        if (BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM) {
            gMarioState->area->camera->mode = BETTER_CAM_MODE;
            gLakituState.mode = BETTER_CAM_MODE;
        } else {
            set_camera_mode(gMarioState->area->camera, gMarioState->area->camera->defMode, 1);
        }
    }
}

//
// Attributes
//

OMM_INLINE f32 pobj_get_underwater_mult(struct Object *o, f32 mult) {
    return (POBJ_IS_AFFECTED_BY_WATER && obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ))) ? mult : 1.f;
}

f32 pobj_get_walk_speed(struct Object *o) {
    return omm_capture_get_walk_speed(o) * pobj_get_underwater_mult(o, 0.8f);
}

f32 pobj_get_run_speed(struct Object *o) {
    return omm_capture_get_run_speed(o) * pobj_get_underwater_mult(o, 0.8f);
}

f32 pobj_get_dash_speed(struct Object *o) {
    return omm_capture_get_dash_speed(o) * pobj_get_underwater_mult(o, 0.8f);
}

f32 pobj_get_max_speed(struct Object *o, bool run, bool dash, bool stop) {
    return (stop ? 0.f : (dash ? pobj_get_dash_speed(o) : (run ? pobj_get_run_speed(o) : pobj_get_walk_speed(o))));
}

f32 pobj_get_jump_velocity(struct Object *o) {
    return omm_capture_get_jump_velocity(o) * POBJ_PHYSICS_JUMP * pobj_get_underwater_mult(o, 0.85f);
}

f32 pobj_get_terminal_velocity(struct Object *o) {
    return omm_capture_get_terminal_velocity(o) * pobj_get_underwater_mult(o, 0.5f) * POBJ_PHYSICS_GRAVITY;
}

f32 pobj_get_gravity(struct Object *o) {
    return omm_capture_get_gravity(o) * pobj_get_underwater_mult(o, 0.5f) * (o->oVelY <= 0.f ? POBJ_PHYSICS_GRAVITY : 1.f);
}

//
// Movement
//

static f32 pobj_update_vel(struct Object *o, f32 vel, f32 target) {
    f32 friction;
    if (POBJ_IS_EMERGING_FROM_PIPE) {
        friction = 1.f;
    } else if (POBJ_IS_FLOATING || !obj_is_on_ground(o)) { // Air
        friction = gOmmObject->state.friction[1];
    } else if (o->oFloor->type == SURFACE_ICE) { // Ice
        friction = gOmmObject->state.friction[2];
    } else { // Ground
        friction = gOmmObject->state.friction[0];
    }
    return vel + (target - vel) * (1.f - friction);
}

static s32 pobj_get_handling(struct Object *o, f32 vel) {
    if (OMM_CHEAT_SUPER_RESPONSIVE) {
        return 0x8000;
    }
    return 0x4000 * (obj_is_on_ground(o) ? 1.f : 0.75f) / (vel < 30.f ? (1.f + sqr_f(vel / 30.f)) : (2.f * (vel / 30.f)));
}

bool pobj_process_inputs(struct Object *o) {
    return !pobj_update_door(o) && !omm_mario_is_locked(gMarioState);
}

void pobj_move(struct Object *o, bool run, bool dash, bool stop) {
    pobj_move_max_vel(o, pobj_get_max_speed(o, run, dash, stop));
}

void pobj_move_max_vel(struct Object *o, f32 maxVel) {
    if (POBJ_NONZERO_ANALOG) {
        f32 magVel = gOmmMario->capture.stickMag;
        o->oForwardVel = pobj_update_vel(o, o->oForwardVel, maxVel * magVel);
        s32 handling = pobj_get_handling(o, POBJ_ABS_FORWARD_VEL);
        s32 faceYaw = gOmmMario->capture.stickYaw - approach_s32((s16) (gOmmMario->capture.stickYaw - o->oFaceAngleYaw), 0, handling, handling);
        obj_set_forward_vel(o, faceYaw, 1.f, o->oForwardVel);
        o->oFaceAngleYaw = faceYaw;
        o->oMoveAngleYaw = faceYaw;
    }
}

void pobj_move_3d(struct Object *o, bool run, bool dash, bool stop, s16 pitchMax, s16 pitchVel, s16 yawVel) {
    s32 facePitch = (s32) (gOmmMario->capture.stickY * pitchMax) - approach_s32((s16) ((s32) (gOmmMario->capture.stickY * pitchMax) - o->oFaceAnglePitch), 0, pitchVel, pitchVel);
    s32 faceYaw = o->oFaceAngleYaw - (s32) (gOmmMario->capture.stickX * yawVel);
    f32 maxVel = max_f(0.01f, pobj_get_max_speed(o, run, dash, stop));
    f32 xyzVel = sqrtf(sqr_f(o->oVelX) + sqr_f(o->oVelY) + sqr_f(o->oVelZ));
    xyzVel += ((maxVel / 8.f) * (1.f - (max_f(0.f, xyzVel) / maxVel)));
    o->oVelX = xyzVel * +coss(facePitch) * sins(faceYaw);
    o->oVelY = xyzVel * -sins(facePitch);
    o->oVelZ = xyzVel * +coss(facePitch) * coss(faceYaw);
    o->oFaceAnglePitch = facePitch;
    o->oFaceAngleYaw = faceYaw;
    o->oMoveAnglePitch = facePitch;
    o->oMoveAngleYaw = faceYaw;
}

// If numMaxJumps == 0, the object keeps jumping when A is held
// If numMaxJumps > 1, the object can jump again while being airborne
s32 pobj_jump(struct Object *o, s32 numMaxJumps) {
    bool canJump = gOmmObject->state._squishTimer == 0;
    if (numMaxJumps > 0) {
        static s32 sJumps = 0;
        static s32 sFrames = 0;
        sJumps = (obj_is_on_ground(o) ? 0 : max_s(sJumps, 1));
        if (POBJ_A_BUTTON_PRESSED && canJump && sJumps < numMaxJumps) {
            sJumps++;
            sFrames = 0;
            o->oVelY = max_f(o->oVelY, pobj_get_jump_velocity(o));
            o->oFloor = NULL;
            return POBJ_RESULT_JUMP_START;
        }
        if (POBJ_A_BUTTON_DOWN && canJump && ++sFrames < 6) {
            o->oVelY = max_f(o->oVelY, pobj_get_jump_velocity(o));
            o->oFloor = NULL;
            return POBJ_RESULT_JUMP_HOLD;
        }
        sFrames = 6;
    } else if (obj_is_on_ground(o) && POBJ_A_BUTTON_DOWN && canJump) {
        o->oVelY = max_f(o->oVelY, pobj_get_jump_velocity(o));
        o->oFloor = NULL;
        return POBJ_RESULT_JUMP_START;
    }
    return POBJ_RESULT_NONE;
}

bool pobj_hop(struct Object *o, f32 hopDiv) {
    if (obj_is_on_ground(o) && POBJ_IS_WALKING && gOmmObject->state._squishTimer == 0) {
        o->oVelY = max_f(o->oVelY, pobj_get_jump_velocity(o) / ((POBJ_IS_OPENING_DOORS ? 2.f : 1.f) * hopDiv));
        o->oFloor = NULL;
        return true;
    }
    return false;
}

void pobj_decelerate(struct Object *o) {
    if (!POBJ_NONZERO_ANALOG) {
        o->oVelX = pobj_update_vel(o, o->oVelX, 0.f);
        o->oVelZ = pobj_update_vel(o, o->oVelZ, 0.f);
        o->oForwardVel = pobj_update_vel(o, o->oForwardVel, 0.f);
    }
}

void pobj_apply_gravity(struct Object *o, f32 mult) {
    if (OMM_CHEAT_MOON_JUMP && (gMarioState->controller->buttonDown & L_TRIG)) {
        o->oVelY = 30.f;
    } else if (POBJ_IS_EMERGING_FROM_PIPE) {
        o->oVelY = max_f(o->oVelY - 4.f, min_f(-40.f, pobj_get_terminal_velocity(o)));
    } else if (mult >= 0.f) {
        o->oVelY = max_f(o->oVelY + pobj_get_gravity(o) * mult, pobj_get_terminal_velocity(o) * mult);
    } else {
        o->oVelY = min_f(o->oVelY + pobj_get_gravity(o) * mult, pobj_get_terminal_velocity(o) * mult);
    }
}

static bool pobj_yoshi_mode_check_mario_dead(struct MarioState *m, struct Object *o, s32 damage, u32 unpossessAct, s16 *yaw, u32 deathAction) {
    m->hurtCounter = damage * 4;
    if ((OMM_MOVESET_ODYSSEY && (m->health - OMM_HEALTH_ODYSSEY_PER_SEGMENT) <= OMM_HEALTH_ODYSSEY_DEAD) ||
        (OMM_MOVESET_CLASSIC && m->health - (m->hurtCounter - m->healCounter) * 0x40 <= OMM_HEALTH_CLASSIC_DEAD)
#if OMM_GAME_IS_SMSR
        || gOmmGlobals->booZeroLife
#endif
    ) {
        if (yaw) {
            omm_mario_unpossess_object_with_yaw(m, unpossessAct, 0, *yaw);
        } else {
            omm_mario_unpossess_object(m, unpossessAct, 0);
        }
        obj_destroy(o);
        if (OMM_MOVESET_ODYSSEY && deathAction != ACT_QUICKSAND_DEATH) {
            m->hurtCounter = 1;
        } else {
            m->health = OMM_HEALTH_DEAD;
            m->hurtCounter = 0;
        }
        m->healCounter = 0;
        m->invincTimer = 0;
        if (deathAction) {
            update_mario_sound_and_camera(m);
            omm_mario_set_action(m, deathAction, 0, 0xFFFF);
        }
        if (unpossessAct == OMM_MARIO_UNPOSSESS_ACT_BURNT) {
            m->marioObj->oMarioBurnTimer = 0;
            obj_play_sound(m->marioObj, SOUND_MARIO_ON_FIRE);
        }
        if (unpossessAct == OMM_MARIO_UNPOSSESS_ACT_BLOWN) {
            obj_play_sound(m->marioObj, SOUND_MARIO_WAAAOOOW);
            level_trigger_warp(m, WARP_OP_DEATH);
        }
        return true;
    }
    return false;
}

static bool pobj_yoshi_mode_do_damage(struct MarioState *m, struct Object *o, s32 damage, u32 unpossessAct, s16 *yaw) {
    if (damage == 0) {
        return true;
    }

    if (damage > -2 && gOmmObject->state._invincTimer > 0) {
        return false;
    }

    // Damage type
    switch (damage) {

        // Quicksand
        case -1: {
            if (pobj_yoshi_mode_check_mario_dead(m, o, OMM_MOVESET_CLASSIC ? 8 : 3, OMM_MARIO_UNPOSSESS_ACT_NONE, NULL, ACT_QUICKSAND_DEATH)) {
                return false;
            }
            gOmmObject->state._invincTimer = 30;
            set_camera_shake_from_hit(SHAKE_FALL_DAMAGE);
        } break;

        // Lava boost
        case -2: {
            if (pobj_yoshi_mode_check_mario_dead(m, o, 3, OMM_MARIO_UNPOSSESS_ACT_BURNT, NULL, ACT_LAVA_BOOST)) {
                return false;
            }
            gOmmObject->state._invincTimer = 0;
            gOmmObject->yoshi.flutterTimer = -1;
            gOmmObject->yoshi.lavaBoost = true;
            set_camera_shake_from_hit(SHAKE_FALL_DAMAGE);
        } break;

        // Lava wall
        case -3: {
            m->wall = o->oWall;
            *yaw = atan2s(o->oWall->normal.z, o->oWall->normal.x);
            if (pobj_yoshi_mode_check_mario_dead(m, o, 3, OMM_MARIO_UNPOSSESS_ACT_BURNT, yaw, ACT_LAVA_BOOST)) {
                lava_boost_on_wall(m);
                return false;
            }
            gOmmObject->state._invincTimer = 0;
            gOmmObject->yoshi.flutterTimer = -1;
            gOmmObject->yoshi.lavaBoost = true;
            set_camera_shake_from_hit(SHAKE_FALL_DAMAGE);
        } break;

        // Regular
        default: {
            set_camera_shake_from_hit(SHAKE_SMALL_DAMAGE + min_s(damage / 2, 2));
            if (pobj_yoshi_mode_check_mario_dead(m, o, damage, unpossessAct, yaw, 0)) {
                return false;
            }
            gOmmObject->state._invincTimer = 30;
        } break;
    }

    omm_sound_play_with_priority(OMM_SOUND_EFFECT_YOSHI_FLUTTER_END, o->oCameraToObject, 0xFF);
    return true;
}

static void pobj_yoshi_mode_do_knockback(struct MarioState *m, struct Object *o, s16 yaw, f32 kbFVel, f32 kbYVel, s32 kbDuration) {
    if (kbDuration > 0) {
        if (kbFVel != 0) {
            o->oFaceAngleYaw = yaw;
            obj_set_forward_vel(o, yaw, 1.f, kbFVel);
        }
        if (kbYVel != 0) {
            o->oVelY = kbYVel;
        }
        omm_mario_lock_once(m, kbDuration);
        omm_sound_play_with_priority(OMM_SOUND_EFFECT_YOSHI_FLUTTER_END, o->oCameraToObject, 0xFF);
    }
}

static void pobj_yoshi_mode_damage_and_knockback(struct Object *o, s32 damage, u32 unpossessAct, s16 yaw, f32 kbFVel, f32 kbYVel, s32 kbDuration) {
    struct MarioState *m = gMarioState;
    if (pobj_yoshi_mode_do_damage(m, o, damage, unpossessAct, &yaw)) {
        pobj_yoshi_mode_do_knockback(m, o, yaw, kbFVel, kbYVel, kbDuration);
    }
}

void pobj_handle_special_floors(struct Object *o) {
    o->oFloorType = OBJ_FLOOR_TYPE_NONE;
    f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);
    bool isOnGround = obj_is_on_ground(o);
    bool isUnderwater = obj_is_underwater(o, waterLevel);
    bool wasUnderwater = obj_is_underwater(o, waterLevel + (o->oPosY - o->oPrevPosY));
    bool canUnpossess = !POBJ_IS_TALKING && gOmmWarp->state <= POBJ_WARP_STATE_NOT_WARPING;

    // Out of bounds
    if (!o->oFloor) {
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
        if (gOmmGlobals->yoshiMode) {
            level_trigger_warp(gMarioState, WARP_OP_DEATH);
        }
        obj_destroy(o);
        return;
    }

    // Squished
    if (gOmmObject->state._squishTimer >= 6 && !OMM_CHEAT_GOD_MODE) {
        struct Object *ceil = (o->oCeil ? o->oCeil->object : NULL);
        if (omm_mario_has_metal_cap(gMarioState) && ceil && ceil->activeFlags) {
            obj_destroy(ceil);
            gOmmObject->state._squishTimer = 0;
        } else if (canUnpossess) {
            set_camera_shake_from_hit(SHAKE_SMALL_DAMAGE);
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BACKWARD_KB, 15);
            if (gOmmGlobals->yoshiMode) {
                omm_mario_set_action(gMarioState, ACT_SQUISHED, 0, 0);
                gMarioState->actionState = 2;
                gMarioState->actionTimer = 15;
                gMarioState->squishTimer = 0xFF;
                gMarioState->health = OMM_HEALTH_DEAD;
                level_trigger_warp(gMarioState, WARP_OP_DEATH);
                obj_destroy(o);
            }
            return;
        }
    }

    // On ground
    if (isOnGround) {
        o->oFloorType = OBJ_FLOOR_TYPE_GROUND;
    }

    // Above water
    if (!isUnderwater && !POBJ_IS_ABOVE_WATER && canUnpossess) {
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6);
        if (gOmmGlobals->yoshiMode) {
            gMarioState->health = OMM_HEALTH_DEAD;
            level_trigger_warp(gMarioState, WARP_OP_DEATH);
        }
        obj_destroy(o);
        return;
    }

    // Walk on water
    if (o->oPosY <= waterLevel + 5.f && (OMM_CHEAT_WALK_ON_WATER || POBJ_IS_ABLE_TO_MOVE_ON_WATER)) {
        o->oPosY = waterLevel;
        o->oVelY = max_f(o->oVelY, 0.f);
        o->oFloorHeight = waterLevel;
        o->oFloorType = OBJ_FLOOR_TYPE_WATER;
        isUnderwater = wasUnderwater = false;

        // Landing on water
        if (o->oPrevPosY > waterLevel + 5.f) {
            obj_spawn_particle_preset(o, PARTICLE_SHALLOW_WATER_SPLASH, false);
            obj_play_sound(o, POBJ_SOUND_SHALLOW_WATER_SPLASH);
        }

        // Walking on water
        else if (omm_capture_get_type(o) != OMM_CAPTURE_SKEETER) {
            obj_make_step_sound_and_particle(o, NULL, 0, 0, POBJ_SOUND_RIDING_SHELL_WATER, OBJ_PARTICLE_WATER_TRAIL | OBJ_PARTICLE_WATER_DROPLET);
        }
    }

    // Underwater
    if (isUnderwater && !POBJ_IS_UNDER_WATER && canUnpossess) {
        o->oPosY = waterLevel + max_f(o->hitboxHeight / 2, 60.f);
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6);
        if (gOmmGlobals->yoshiMode) {
            gMarioState->health = OMM_HEALTH_DEAD;
            level_trigger_warp(gMarioState, WARP_OP_DEATH);
        }
        obj_destroy(o);
        return;
    }

    // Water effects
    obj_produce_water_effects(o, isOnGround, isUnderwater, wasUnderwater, waterLevel);

    // Walk on gas
    if (OMM_CHEAT_WALK_ON_GAS) {
        f32 gasLevel = find_poison_gas_level(o->oPosX, o->oPosZ);
        if (o->oPosY <= gasLevel + 5.f) {
            o->oPosY = gasLevel;
            o->oVelY = max_f(o->oVelY, 0.f);
            o->oFloorHeight = gasLevel;
            o->oFloorType = OBJ_FLOOR_TYPE_GROUND;
        }
    }

    // Lava wall
    // TODO: YOSHIMODE
    // if (o->oWall && o->oWall->type == SURFACE_BURNING && !OMM_CHEAT_WALK_ON_LAVA && !POBJ_IS_IMMUNE_TO_LAVA && canUnpossess) {
    //     if (gOmmGlobals->yoshiMode) {
    //         pobj_yoshi_mode_damage_and_knockback(o, -3, OMM_MARIO_UNPOSSESS_ACT_BURNT, o->oFaceAngleYaw, 80, 84, 15);
    //     } else {
    //         gMarioState->wall = o->oWall;
    //         omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BURNT, 0);
    //         lava_boost_on_wall(gMarioState);
    //         obj_destroy(o);
    //     }
    //     return;
    // }

    // Special floors
    switch (o->oFloor->type) {
        case SURFACE_DEATH_PLANE:
        case SURFACE_VERTICAL_WIND: {
            if (o->oDistToFloor < 2048.f && !OMM_CHEAT_WALK_ON_DEATH_BARRIER && canUnpossess) {
                f32 objVelY = o->oVelY;
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
                if (gOmmGlobals->yoshiMode) {
                    check_death_barrier(gMarioState);
                }
                gMarioState->vel[1] = objVelY;
                obj_destroy(o);
            } else if (o->oFloor->type == SURFACE_VERTICAL_WIND && POBJ_IS_AFFECTED_BY_VERTICAL_WIND) {
                f32 offsetY = o->oPosY - -1500.f;
                if (-3000.f < offsetY && offsetY < 2000.f) {
                    f32 maxVelY = 12000.f / (max_f(0.f, offsetY) + 200.f);
                    o->oVelY = min_f(o->oVelY + (maxVelY / 8.f), maxVelY);
                }
            }
        } break;

        case SURFACE_INSTANT_QUICKSAND:
        case SURFACE_INSTANT_MOVING_QUICKSAND: {
            if (o->oDistToFloor < 5.f && !OMM_CHEAT_WALK_ON_QUICKSAND && !POBJ_IS_IMMUNE_TO_QUICKSAND && canUnpossess) {
                if (gOmmGlobals->yoshiMode) {
                    pobj_yoshi_mode_damage_and_knockback(o, -1, OMM_MARIO_UNPOSSESS_ACT_NONE, o->oFaceAngleYaw, 0, 0, 0);
                } else {
                    omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6);
                    obj_destroy(o);
                }
            }
        } break;

        case SURFACE_BURNING: {
            if (o->oDistToFloor < 5.f && !OMM_CHEAT_WALK_ON_LAVA && !POBJ_IS_IMMUNE_TO_LAVA && canUnpossess) {
                if (gOmmGlobals->yoshiMode) {
                    pobj_yoshi_mode_damage_and_knockback(o, -2, OMM_MARIO_UNPOSSESS_ACT_BURNT, o->oFaceAngleYaw, 0.01f, 84, 1);
                } else {
                    omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BURNT, 0);
                    omm_mario_set_action(gMarioState, ACT_LAVA_BOOST, 0, 0);
                    obj_destroy(o);
                }
            } else if (isOnGround) {
                o->oFloorType = OBJ_FLOOR_TYPE_LAVA;
                obj_make_step_sound_and_particle(o, NULL, 0, 0, POBJ_SOUND_RIDING_SHELL_LAVA, OBJ_PARTICLE_FLAME);
            }
        } break;

        case SURFACE_WARP: {
            if (POBJ_CAN_WARP(NULL)) {
                gOmmWarp->state = POBJ_WARP_STATE_WARPING;
                gOmmWarp->timer = level_trigger_warp(gMarioState, WARP_OP_WARP_FLOOR) - 1;
                omm_mario_lock(gMarioState, -1);
            }
        } break;

        case SURFACE_TIMER_START: {
            pss_begin_slide(gMarioState);
        } break;

        case SURFACE_TIMER_END: {
            pss_end_slide(gMarioState);
        } break;
    }
}

//
// Attacks
//

OMM_INLINE bool pobj_is_charging() {
    return (omm_capture_get_type(gOmmCapture) == OMM_CAPTURE_YOSHI ?
        (gOmmObject->state._powerTimer == 0 ? POBJ_X_BUTTON_PRESSED : POBJ_X_BUTTON_DOWN) :
        (gOmmObject->state._powerTimer == 0 ? POBJ_B_BUTTON_PRESSED : POBJ_B_BUTTON_DOWN)
    );
}

// Returns _power_, ranging from 0 to 1.
f32 pobj_check_charge_attack(s32 tmax, struct Object *origin, f32 radius, f32 offset) {
    if (pobj_is_charging()) {
        gOmmObject->state._powerTimer = clamp_s(gOmmObject->state._powerTimer + 1, 1, tmax);
        f32 _power_ = (f32) gOmmObject->state._powerTimer / (f32) tmax;
        if (origin && _power_ == 1.f) {
            struct Object *sparkle = spawn_object(origin, MODEL_SPARKLES_ANIMATION, bhvSparkle);
            if (sparkle) {
                obj_translate_xyz_random(sparkle, radius);
                obj_scale_random(sparkle, 1.f, 0.f);
                sparkle->oPosY += offset;
            }
        }
        return _power_;
    }
    return 0;
}

// Returns _power_, ranging from 0 to 1.
f32 pobj_check_release_attack(s32 tmax) {
    if (gOmmObject->state._powerTimer > 0 && !pobj_is_charging()) {
        f32 _power_ = (f32) gOmmObject->state._powerTimer / (f32) tmax;
        gOmmObject->state._powerTimer = 0;
        return _power_;
    }
    return 0;
}

//
// Doors
//

static void pobj_open_door(struct Object *o, struct Object *door, bool isStarDoor) {
    struct Object *door2 = isStarDoor ? obj_get_nearest_with_behavior_and_radius(door, door->behavior, 250.f) : door;
    if (!door2) door2 = door;
    f32 cX = (door->oPosX + door2->oPosX) / 2.f;
    f32 cZ = (door->oPosZ + door2->oPosZ) / 2.f;
    s16 cY = (door->oMoveAngleYaw);
    f32 cR = (door->hitboxRadius + door2->hitboxRadius + o->hitboxRadius);
    f32 vX = sins(cY);
    f32 vZ = coss(cY);
    f32 dX = o->oPosX - cX;
    f32 dZ = o->oPosZ - cZ;
    s16 dY = atan2s(dZ, dX);
    s16 dA = min_s((u16) (cY - dY), (u16) (dY - cY));
    f32 dS = (dA < 0x4000 ? -1.f : +1.f);
    Vec2f pA = { cX, cZ };
    Vec2f pB = { cX + vX, cZ + vZ };
    Vec2f pP = { o->oPosX, o->oPosZ };
    Vec2f pQ; vec2f_get_projected_point_on_line(pQ, NULL, pA, pB, pP);

    // Init door data
    gOmmObject->door.vel[0][0] = (pQ[0] - pP[0]) / 20.f;
    gOmmObject->door.vel[0][1] = (pQ[1] - pP[1]) / 20.f;
    gOmmObject->door.vel[1][0] = (dS * vX * cR) / 25.f;
    gOmmObject->door.vel[1][1] = (dS * vZ * cR) / 25.f;
    gOmmObject->door.timer = 0;

    // Open door
    door->oInteractStatus = isStarDoor ? INT_STATUS_ENTERING_STAR_DOOR : INT_STATUS_ENTERING_DOOR;

    // Lock Mario
    gOmmMario->capture.openDoors = true;
}

bool pobj_update_door(struct Object *o) {
    if (!POBJ_IS_OPENING_DOORS) {
        return false;
    }

    // Move to door
    if (gOmmObject->door.timer < 20) {
        o->oVelX = gOmmObject->door.vel[0][0];
        o->oVelZ = gOmmObject->door.vel[0][1];
    }

    // Move from door
    else if (gOmmObject->door.timer < 45) {
        o->oVelX = gOmmObject->door.vel[1][0];
        o->oVelZ = gOmmObject->door.vel[1][1];
    }

    // Move freely again
    else {
        gOmmMario->capture.openDoors = false;
        return false;
    }

    // Update
    o->hitboxRadius = 1.f;
    o->hitboxHeight = 1.f;
    o->oWallHitboxRadius = 1.f;
    f32 forwardVel = sqrtf(sqr_f(o->oVelX) + sqr_f(o->oVelZ));
    s16 faceAngleYaw = (forwardVel == 0.f ? o->oFaceAngleYaw : atan2s(o->oVelZ, o->oVelX));
    obj_set_angle(o, 0, faceAngleYaw, 0);
    obj_set_forward_vel(o, faceAngleYaw, 1.f, forwardVel);
    gOmmObject->door.timer++;
    return true;
}

//
// Interactions
//

#pragma GCC push_options
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define INT_STATUS_ATTACKED         (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED)
#define INT_STATUS_HIT_FROM_ABOVE   (ATTACK_FROM_ABOVE | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED)
#define INT_STATUS_HIT_FROM_BELOW   (ATTACK_FROM_BELOW | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED)
#define INT_STATUS_HIT_GROUND_POUND (ATTACK_GROUND_POUND_OR_TWIRL | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED)

static bool pobj_push_out_of_object_hitbox(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {
    if (overlapHitbox) {

        // If the object has collisions, don't push
        if (obj->oSurfaces) {
            return false;
        }

        // Push
        f32 dx = o->oPosX - obj->oPosX;
        f32 dz = o->oPosZ - obj->oPosZ;
        f32 d = sqrtf(sqr_f(dx) + sqr_f(dz));
        if (d == 0.f) {
            // This is unlikely, but we must
            // prevent a div/0 error
            return false;
        }
        f32 diff = (o->hitboxRadius + obj->hitboxRadius) - d;
        f32 pushX = (dx / d) * diff;
        f32 pushZ = (dz / d) * diff;

        // Make all SURFACE and POLELIKE objects unpushable, as well as invulnerable ones
        if (obj_is_surface(obj) || omm_obj_is_treasure_chest(obj) || !POBJ_IS_INVULNERABLE) {
            o->oPosX += pushX;
            o->oPosZ += pushZ;
        } else {
            obj->oPosX -= pushX;
            obj->oPosZ -= pushZ;
        }
    }
    return false;
}

static bool pobj_push_out_of_object_hurtbox(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {
    if (overlapHurtbox) {

        // Destroy if metal
        if (omm_mario_has_metal_cap(gMarioState) && omm_obj_is_metal_destructible(obj)) {
            obj_destroy(obj);
            return false;
        }

        // Push
        f32 dx = o->oPosX - obj->oPosX;
        f32 dz = o->oPosZ - obj->oPosZ;
        f32 d = sqrtf(sqr_f(dx) + sqr_f(dz));
        if (d == 0.f) {
            // This is unlikely, but we must
            // prevent a div/0 error
            return false;
        }
        f32 diff = (o->hitboxRadius + (obj->hurtboxRadius > 0 ? obj->hurtboxRadius : obj->hitboxRadius)) - d;
        f32 pushX = (dx / d) * diff;
        f32 pushZ = (dz / d) * diff;

        // Make all SURFACE and POLELIKE objects unpushable, as well as invulnerable ones
        if (obj_is_surface(obj) || omm_obj_is_treasure_chest(obj) || !POBJ_IS_INVULNERABLE) {
            o->oPosX += pushX;
            o->oPosZ += pushZ;
        } else {
            obj->oPosX -= pushX;
            obj->oPosZ -= pushZ;
        }
    }
    return false;
}

static bool pobj_mario_take_damage_and_unpossess(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Ignore if not tangible
    if (overlapHurtbox) {
        if (POBJ_IS_INTANGIBLE) {
            return false;
        }
    }

    // No damage, push out of object
    if (overlapHurtbox) {
        if (obj->oDamageOrCoinValue == 0 || POBJ_IS_INVULNERABLE) {
            return pobj_push_out_of_object_hurtbox(o, obj, overlapHitbox, overlapHurtbox);
        }
    }

    // Damage Mario
    if (overlapHurtbox) {
        obj->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_ATTACKED_MARIO;
        gMarioState->interactObj = obj;
        s16 angleYaw = obj_get_object1_angle_yaw_to_object2(o, obj);
        u16 angleDiff = abs_s((s16) (angleYaw - o->oFaceAngleYaw));
        if (gOmmGlobals->yoshiMode) {
            if (angleDiff < 0x4000) {
                pobj_yoshi_mode_damage_and_knockback(o, obj->oDamageOrCoinValue, OMM_MARIO_UNPOSSESS_ACT_BACKWARD_KB, angleYaw, -60, 0, 15);
            } else {
                pobj_yoshi_mode_damage_and_knockback(o, obj->oDamageOrCoinValue, OMM_MARIO_UNPOSSESS_ACT_FORWARD_KB, angleYaw + 0x8000, 60, 0, 15);
            }
        } else {
            gMarioState->hurtCounter += 4 * obj->oDamageOrCoinValue;
            if (angleDiff < 0x4000) {
                omm_mario_unpossess_object_with_yaw(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BACKWARD_KB, 15, angleYaw);
            } else {
                omm_mario_unpossess_object_with_yaw(gMarioState, OMM_MARIO_UNPOSSESS_ACT_FORWARD_KB, 15, angleYaw + 0x8000);
            }
            set_camera_shake_from_hit(SHAKE_SMALL_DAMAGE + min_s(obj->oDamageOrCoinValue / 2, 2));
        }
        return true;
    }

    return false;
}

static bool pobj_interact_bounce_top(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Ground pound
    if (overlapHitbox) {
        if (POBJ_IS_GROUND_POUNDING) {
            obj->oInteractStatus = INT_STATUS_HIT_GROUND_POUND;
            return false;
        }
    }

    // Hit and bounce
    if (overlapHitbox) {
        if (obj_is_object2_hit_from_above(o, obj)) {
            obj->oInteractStatus = INT_STATUS_HIT_FROM_ABOVE;
            o->oPosY = obj->oPosY + obj->hitboxHeight - obj->hitboxDownOffset + 20.f;
            o->oVelY = pobj_get_jump_velocity(o) * 1.2f;
            obj_play_sound(o, SOUND_ACTION_BOUNCE_OFF_OBJECT);
            POBJ_SET_BOUNCING;
            return false;
        }
    }

    // Attack
    if (overlapHitbox) {
        if (POBJ_IS_ATTACKING || POBJ_IS_ATTACKING_BREAKABLE) {
            obj->oInteractStatus = (POBJ_IS_GROUND_POUNDING ? INT_STATUS_HIT_GROUND_POUND : INT_STATUS_ATTACKED);
            obj_play_sound(o, SOUND_ACTION_HIT_2);
            return false;
        }
    }

    // Damage Mario
    if (overlapHurtbox) {
        return pobj_mario_take_damage_and_unpossess(o, obj, overlapHitbox, overlapHurtbox);
    }

    return false;
}

static bool pobj_interact_hit_from_below(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Ground pound
    if (overlapHitbox) {
        if (POBJ_IS_GROUND_POUNDING) {
            obj->oInteractStatus = INT_STATUS_HIT_GROUND_POUND;
            return false;
        }
    }

    // Hit from below
    if (overlapHitbox) {
        if (obj_is_object2_hit_from_below(o, obj)) {
            obj->oInteractStatus = INT_STATUS_HIT_FROM_BELOW;
            o->oVelY = 0;
            obj_play_sound(o, SOUND_ACTION_BOUNCE_OFF_OBJECT);
            set_camera_shake_from_hit(SHAKE_HIT_FROM_BELOW);
            return false;
        }
    }

    // Hit from above
    if (overlapHitbox) {
        return pobj_interact_bounce_top(o, obj, overlapHitbox, overlapHurtbox);
    }

    return false;
}

static bool pobj_mario_burn_and_unpossess(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Burn Mario
    if (overlapHurtbox) {

        // No damage if intangible, immune or underwater
        if (POBJ_IS_INTANGIBLE || POBJ_IS_IMMUNE_TO_FIRE || obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ))) {
            return false;
        }

        obj->oInteractStatus = INT_STATUS_INTERACTED;
        gMarioState->interactObj = obj;
        if (gOmmGlobals->yoshiMode) {
            pobj_yoshi_mode_damage_and_knockback(o, 3, OMM_MARIO_UNPOSSESS_ACT_BURNT, o->oFaceAngleYaw, 0, 0, 0);
        } else {
            gMarioState->marioObj->oMarioBurnTimer = 0;
            obj_play_sound(gMarioState->marioObj, SOUND_MARIO_ON_FIRE);
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BURNT, 15);
        }
        return true;
    }

    return false;
}

static bool pobj_interact_bully(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Bully
    if (overlapHitbox) {
        f32 pobjKnockback = 0.65f * obj->hitboxRadius;
        f32 bullyKnockback = 3600.f / obj->hitboxRadius;
        s16 angle = obj_get_object1_angle_yaw_to_object2(o, obj);

        // Bully the bully if capture attacks/jumps or is invulnerable
        if (POBJ_IS_INVULNERABLE || POBJ_IS_ATTACKING || POBJ_IS_ATTACKING_BREAKABLE || POBJ_IS_GROUND_POUNDING || obj_is_object2_hit_from_above(o, obj)) {

            // Capture
            o->oFaceAngleYaw = angle;
            o->oMoveAngleYaw = angle;
            obj_set_forward_vel(o, o->oFaceAngleYaw, 0.25f, -pobjKnockback);
            pobj_push_out_of_object_hitbox(o, obj, overlapHitbox, overlapHurtbox);
            omm_mario_lock_once(gMarioState, 6);

            // Bully
            obj->oFaceAngleYaw = angle + 0x8000;
            obj->oMoveAngleYaw = angle;
            obj->oForwardVel = bullyKnockback;
            obj->oInteractStatus = INT_STATUS_ATTACKED;
            obj_play_sound(obj, SOUND_OBJ_BULLY_METAL);

        } else if (!POBJ_IS_INTANGIBLE) {

            // Bully
            obj->oFaceAngleYaw = angle + 0x8000;
            obj->oMoveAngleYaw = angle + 0x8000;
            obj->oForwardVel = -0.25f * bullyKnockback;
            obj->oInteractStatus = INT_STATUS_INTERACTED;
            obj_play_sound(obj, SOUND_OBJ_BULLY_METAL);

            // If capture was bullied during the last 60 frames, eject Mario
            if (gOmmObject->state._bullyTimer > 0) {
                s16 angleYaw = obj_get_object1_angle_yaw_to_object2(o, obj);
                u16 angleDiff = abs_s((s16) (angleYaw - o->oFaceAngleYaw));
                if (gOmmGlobals->yoshiMode) {
                    if (angleDiff < 0x4000) {
                        pobj_yoshi_mode_damage_and_knockback(o, max_s(1, pobjKnockback / 20), OMM_MARIO_UNPOSSESS_ACT_BACKWARD_KB, angleYaw, -60, 0, 15);
                    } else {
                        pobj_yoshi_mode_damage_and_knockback(o, max_s(1, pobjKnockback / 20), OMM_MARIO_UNPOSSESS_ACT_FORWARD_KB, angleYaw + 0x8000, 60, 0, 15);
                    }
                } else {
                    if (angleDiff < 0x4000) {
                        omm_mario_unpossess_object_with_yaw(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BACKWARD_KB, 15, angleYaw);
                    } else {
                        omm_mario_unpossess_object_with_yaw(gMarioState, OMM_MARIO_UNPOSSESS_ACT_FORWARD_KB, 15, angleYaw + 0x8000);
                    }
                }
                return true;
            }

            // Capture
            gOmmObject->state._bullyTimer = 60;
            o->oFaceAngleYaw = angle;
            o->oMoveAngleYaw = angle;
            obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, -pobjKnockback);
            pobj_push_out_of_object_hitbox(o, obj, overlapHitbox, overlapHurtbox);
            obj_play_sound(o, o->oDeathSound);
            omm_mario_lock_once(gMarioState, 20);
        }
        return false;
    }

    return false;
}

static bool pobj_interact_breakable(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Hit from below
    if (overlapHitbox) {
        if (obj_is_object2_hit_from_below(o, obj)) {
            obj->oInteractStatus = INT_STATUS_HIT_FROM_BELOW;
            set_camera_shake_from_hit(SHAKE_HIT_FROM_BELOW);
            o->oVelY = min_f(o->oVelY, 0.f);
            return false;
        }
    }

    // Attack
    if (overlapHitbox) {
        if (POBJ_IS_ATTACKING_BREAKABLE) {
            obj->oInteractStatus = INT_STATUS_ATTACKED;
            set_camera_shake_from_hit(SHAKE_ATTACK);
            obj_play_sound(o, SOUND_ACTION_HIT_2);
            return false;
        }
    }

    // Ignore if not tangible
    if (overlapHitbox) {
        if (POBJ_IS_INTANGIBLE) {
            return false;
        }
    }

    // Push
    if (overlapHitbox) {
        return pobj_push_out_of_object_hitbox(o, obj, overlapHitbox, overlapHurtbox);
    }

    return false;
}

static bool pobj_interact_grabbable(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Bob-ombs can explode on contact
    if (overlapHitbox) {
        if (obj->behavior == bhvBobomb && obj->oBobombFuseLit) {
            obj->oInteractStatus = INT_STATUS_ATTACKED | INT_STATUS_TOUCHED_BOB_OMB;
            return false;
        }
    }

    // Is attackable
    if (overlapHitbox) {
        if ((obj->oInteractionSubtype & INT_SUBTYPE_KICKABLE) && (POBJ_IS_ATTACKING || POBJ_IS_ATTACKING_BREAKABLE)) {
            obj->oInteractStatus = INT_STATUS_ATTACKED | INT_STATUS_TOUCHED_BOB_OMB;
            obj_play_sound(o, SOUND_ACTION_HIT_2);
            return false;
        }
    }

    // Is breakable (small boxes)
    if (overlapHitbox) {
        if (POBJ_IS_ATTACKING_BREAKABLE) {
            if (omm_obj_is_breakable(obj)) {
                if (omm_obj_is_destructible(obj)) {
                    obj_destroy(obj);
                } else {
                    obj->oInteractStatus = (POBJ_IS_GROUND_POUNDING ? INT_STATUS_HIT_GROUND_POUND : INT_STATUS_ATTACKED);
                    set_camera_shake_from_hit(SHAKE_ATTACK);
                    obj_play_sound(o, SOUND_ACTION_HIT_2);
                }
                return false;
            }
        }
    }

    // Ignore if not tangible
    if (overlapHitbox) {
        if (POBJ_IS_INTANGIBLE) {
            return false;
        }
    }

    // Grabs Mario out of his possession
    if (overlapHitbox) {
        if ((obj->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO) && obj_is_object1_facing_object2(obj, o, 0x2800) && !omm_mario_has_metal_cap(gMarioState)) {
            if (gOmmGlobals->yoshiMode) {
                obj->oInteractStatus = INT_STATUS_INTERACTED;
                s16 angleYaw = obj_get_object1_angle_yaw_to_object2(o, obj);
                u16 angleDiff = abs_s((s16) (angleYaw - o->oFaceAngleYaw));
                if (angleDiff < 0x4000) {
                    pobj_yoshi_mode_damage_and_knockback(o, 3, OMM_MARIO_UNPOSSESS_ACT_BACKWARD_KB, angleYaw, -60, 0, 15);
                } else {
                    pobj_yoshi_mode_damage_and_knockback(o, 3, OMM_MARIO_UNPOSSESS_ACT_FORWARD_KB, angleYaw + 0x8000, 60, 0, 15);
                }
            } else {
                obj->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_GRABBED_MARIO;
                o->oFaceAngleYaw = obj->oMoveAngleYaw;
                gMarioState->interactObj = obj;
                gMarioState->usedObj = obj;
                obj_play_sound(gMarioState->marioObj, SOUND_MARIO_OOOF);
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_GRABBED, 15);
            }
            return true;
        }
    }

    // Damage Mario
    if (overlapHurtbox) {
        return pobj_mario_take_damage_and_unpossess(o, obj, overlapHitbox, overlapHurtbox);
    }

    return false;
}

static bool pobj_interact_clam_or_bubba(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Ignore if not tangible
    if (overlapHurtbox) {
        if (POBJ_IS_INTANGIBLE) {
            return false;
        }
    }

    // Eats Mario and his possession
    if (overlapHurtbox) {
        if (obj->oInteractionSubtype & INT_SUBTYPE_EATS_MARIO) {
            obj->oInteractStatus = INT_STATUS_INTERACTED;
            gMarioState->interactObj = obj;
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_EATEN, 15);
            obj_mark_for_deletion(o);
            return true;
        }
    }

    // Damage Mario
    if (overlapHurtbox) {
        return pobj_mario_take_damage_and_unpossess(o, obj, overlapHitbox, overlapHurtbox);
    }

    return false;
}

static bool pobj_interact_strong_wind(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Blow Mario away, out of his possession
    if (overlapHitbox) {

        // Delete the projectile if intangible or immune to strong winds
        if (POBJ_IS_INTANGIBLE || POBJ_IS_IMMUNE_TO_STRONG_WINDS) {
            obj->oIntangibleTimer = -1;
            obj_mark_for_deletion(obj);
            return false;
        }

        obj->oInteractStatus = INT_STATUS_INTERACTED;
        gMarioState->interactObj = obj;
        gMarioState->usedObj = obj;
        gMarioState->unkC4 = 0.4f;
        if (gOmmGlobals->yoshiMode) {
            pobj_yoshi_mode_damage_and_knockback(o, 1, OMM_MARIO_UNPOSSESS_ACT_BLOWN, obj->oMoveAngleYaw + 0x8000, -60, 0, 15);
        } else {
            obj_play_sound(gMarioState->marioObj, SOUND_MARIO_WAAAOOOW);
            omm_mario_unpossess_object_with_yaw(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BLOWN, 15, obj->oMoveAngleYaw + 0x8000);
        }
        return true;
    }

    return false;
}

static bool pobj_interact_tornado(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Suck Mario out of his possession
    if (overlapHitbox) {

        // Same flags used to resist tornados
        if (POBJ_IS_INTANGIBLE || POBJ_IS_IMMUNE_TO_STRONG_WINDS) {
            return false;
        }

        obj->oInteractStatus = INT_STATUS_INTERACTED;
        gMarioState->interactObj = obj;
        gMarioState->usedObj = obj;
        if (gOmmGlobals->yoshiMode) {
            f32 gravity = -pobj_get_gravity(o);
            pobj_yoshi_mode_damage_and_knockback(o, 0, 0, o->oFaceAngleYaw, 0, 60.f * sqrtf(gravity), 1);
            gOmmObject->state._cannonTimer = (s32) ceilf(o->oVelY / gravity);
            obj_play_sound(o, SOUND_ACTION_FLYING_FAST);
        } else {
            gMarioState->marioObj->oMarioTornadoYawVel = 0x400;
            gMarioState->marioObj->oMarioTornadoPosY = o->oPosY - obj->oPosY;
            obj_play_sound(gMarioState->marioObj, SOUND_MARIO_WAAAOOOW);
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_TORNADO, 15);
        }
        return true;
    }

    return false;
}

static bool pobj_interact_door(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Open the door
    if (overlapHitbox) {
        if (obj->oAction == 0 && POBJ_IS_ABLE_TO_OPEN_DOORS && !POBJ_IS_OPENING_DOORS && !omm_mario_is_locked(gMarioState) && obj_is_on_ground(o)) {
            if (obj->oInteractionSubtype & INT_SUBTYPE_STAR_DOOR) {
                pobj_open_door(o, obj, true);
            } else {
                pobj_open_door(o, obj, false);
            }
            return true;
        }
    }

    return false;
}

bool pobj_interact_warp(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Check warps
    if (overlapHitbox) {
        if (POBJ_CAN_WARP(obj)) {

            // Fading warp
            if (obj->oInteractionSubtype & INT_SUBTYPE_FADING_WARP) {
                if (!POBJ_IS_WALKING && obj_is_on_ground(o)) {
                    gOmmWarp->object = obj;
                    gOmmWarp->state = POBJ_WARP_STATE_FADING_OUT;
                    gOmmWarp->timer = -20;
                    gMarioState->fadeWarpOpacity = 0xF8;
                    omm_mario_lock(gMarioState, -1);
                    obj_play_sound(o, SOUND_ACTION_TELEPORT);
                    return true;
                }
            }

            // Instant warp
            else {
                omm_mario_unpossess_object_before_warp(gMarioState);
                omm_mario_interact_warp(gMarioState, obj);
                return true;
            }
        }
    }

    return false;
}

bool pobj_interact_warp_door(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Check warps
    if (overlapHitbox) {
        u32 saveFlags = omm_save_file_get_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE);
        s32 warpDoorId = obj->oBehParams >> 24;

        // Upstairs door
        if (warpDoorId == 1) {
            if (!(saveFlags & (SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR | SAVE_FLAG_HAVE_KEY_2))) {
                return false;
            }
        }

        // Basement door
        if (warpDoorId == 2) {
            if (!(saveFlags & (SAVE_FLAG_UNLOCKED_BASEMENT_DOOR | SAVE_FLAG_HAVE_KEY_1))) {
                return false;
            }
        }

        // Trigger warp door
        if (POBJ_CAN_WARP(obj) && pobj_interact_door(o, obj, true, overlapHurtbox)) {
            gOmmWarp->object = obj;
            gOmmWarp->state = POBJ_WARP_STATE_OPENING_DOOR;
            gOmmWarp->timer = -16;
            omm_mario_lock(gMarioState, -1);
            return true;
        }
    }

    return false;
}

bool pobj_interact_bbh_entrance(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Unpossess and start BBH entrance cutscene
    if (overlapHitbox) {
        if (gOmmGlobals->yoshiMode) {
            omm_mario_unpossess_object_before_warp(gMarioState);
            omm_mario_interact_warp(gMarioState, obj);
        } else {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
            omm_mario_interact_bbh_entrance(gMarioState, obj);
        }
        return true;
    }

    return false;
}

bool pobj_interact_cap(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Only Yoshi can interact with caps, but the check is in omm_mario_interact_cap
    if (overlapHitbox) {
        if (omm_mario_interact_cap(gMarioState, obj)) {
            return true;
        }
    }

    return false;
}

bool pobj_interact_water_ring(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Regain health
    if (overlapHitbox) {
        omm_mario_interact_water_ring(gMarioState, obj);
        return false;
    }

    return false;
}

bool pobj_interact_cannon_base(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Cannon shot that gives a vertical boost
    if (overlapHitbox) {
        if (POBJ_IS_AFFECTED_BY_CANNON) {
            f32 gravity = -pobj_get_gravity(o);
            o->oPosY = obj->oPosY + obj->hitboxHeight - obj->hitboxDownOffset + 10.f;
            o->oVelY = 72.f * sqrtf(gravity);
            gOmmObject->state._cannonTimer = (s32) ceilf(o->oVelY / gravity);
            obj_spawn_white_puff_at(obj->oPosX, o->oPosY + 50.f, obj->oPosZ, SOUND_OBJ_CANNON4);
            return false;
        }
    }

    return false;
}

bool pobj_interact_whirlpool(struct Object *o, struct Object *obj, bool overlapHitbox, bool overlapHurtbox) {

    // Unpossess and get caught in whirlpool
    if (overlapHitbox) {
        if (!POBJ_IS_IMMUNE_TO_STRONG_WINDS) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
            if (gOmmGlobals->yoshiMode) {
                obj_destroy(o);
            }
            omm_mario_interact_whirlpool(gMarioState, obj);
            return true;
        }
    }

    return false;
}

bool pobj_process_interaction(struct Object *o, struct Object *obj, u32 interactType) {
    if (obj->oInteractStatus & INT_STATUS_INTERACTED) {
        return false;
    }

    bool overlapHitbox = obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX);
    bool overlapHurtbox = obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HURTBOX_HITBOX_IF_ZERO);

    // Interaction
    switch (interactType) {
        // Handled by omm_obj_process_interactions
        // case INTERACT_COIN:
        // case INTERACT_STAR_OR_KEY:

        case INTERACT_SHOCK:
            if (obj->behavior == bhvTreasureChestBottom) return omm_obj_interact_treasure_chest(o, obj);
            return pobj_mario_take_damage_and_unpossess(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_SNUFIT_BULLET:
        case INTERACT_MR_BLIZZARD:
        case INTERACT_UNKNOWN_08:
        case INTERACT_DAMAGE:
            return pobj_mario_take_damage_and_unpossess(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_KOOPA:
        case INTERACT_BOUNCE_TOP:
        case INTERACT_BOUNCE_TOP2:
            return pobj_interact_bounce_top(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_HIT_FROM_BELOW:
            return pobj_interact_hit_from_below(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_FLAME:
            return pobj_mario_burn_and_unpossess(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_BULLY:
            return pobj_interact_bully(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_BREAKABLE:
            return pobj_interact_breakable(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_GRABBABLE:
            return pobj_interact_grabbable(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_CLAM_OR_BUBBA:
            return pobj_interact_clam_or_bubba(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_STRONG_WIND:
            return pobj_interact_strong_wind(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_TORNADO:
            return pobj_interact_tornado(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_DOOR:
            return pobj_interact_door(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_WARP:
            return pobj_interact_warp(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_WARP_DOOR:
            return pobj_interact_warp_door(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_BBH_ENTRANCE:
            return pobj_interact_bbh_entrance(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_CAP:
            return pobj_interact_cap(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_WATER_RING:
            return pobj_interact_water_ring(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_IGLOO_BARRIER:
        case INTERACT_KOOPA_SHELL:
        case INTERACT_TEXT:
            if (obj_get_list_index(obj) == OBJ_LIST_SURFACE) break; // Do nothing
            return pobj_push_out_of_object_hitbox(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_CANNON_BASE:
            return pobj_interact_cannon_base(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_WHIRLPOOL:
            return pobj_interact_whirlpool(o, obj, overlapHitbox, overlapHurtbox);

        case INTERACT_POLE:
        default:
            break; // Do nothing
    }
    return false;
}

#pragma GCC pop_options

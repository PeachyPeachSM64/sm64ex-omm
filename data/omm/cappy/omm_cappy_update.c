#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

void omm_cappy_init_behavior(struct Object *cappy, struct MarioState *m) {
    bool isWater = ((m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED);
    f32 throwStrength = (f32) cappy->oCappyThrowStrength / 4.f;
    switch (cappy->oCappyBehavior) {

        case OMM_CAPPY_BHV_DEFAULT_GROUND:
        case OMM_CAPPY_BHV_DEFAULT_AIR:
            cappy->oPosX = m->pos[0];
            cappy->oPosY = m->pos[1] + CAPPY_BHV_DEFAULT_OFFSET * !isWater;
            cappy->oPosZ = m->pos[2];
            cappy->oVelX = CAPPY_BHV_DEFAULT_VEL * throwStrength * coss(m->faceAngle[0] * isWater) * sins(m->faceAngle[1]);
            cappy->oVelY = CAPPY_BHV_DEFAULT_VEL * throwStrength * sins(m->faceAngle[0] * isWater);
            cappy->oVelZ = CAPPY_BHV_DEFAULT_VEL * throwStrength * coss(m->faceAngle[0] * isWater) * coss(m->faceAngle[1]);
            break;

        case OMM_CAPPY_BHV_UPWARDS_GROUND:
        case OMM_CAPPY_BHV_UPWARDS_AIR:
            cappy->oPosX = m->pos[0] + CAPPY_BHV_UPWARDS_OFFSET * sins(m->faceAngle[1]);
            cappy->oPosY = m->pos[1] + CAPPY_BHV_UPWARDS_OFFSET;
            cappy->oPosZ = m->pos[2] + CAPPY_BHV_UPWARDS_OFFSET * coss(m->faceAngle[1]);
            cappy->oVelX = 0.f;
            cappy->oVelY = CAPPY_BHV_UPWARDS_VEL;
            cappy->oVelZ = 0.f;
            break;

        case OMM_CAPPY_BHV_DOWNWARDS_GROUND:
        case OMM_CAPPY_BHV_DOWNWARDS_AIR:
            cappy->oPosX = m->pos[0] + CAPPY_BHV_DOWNWARDS_OFFSET * sins(m->faceAngle[1]);
            cappy->oPosY = m->pos[1] + CAPPY_BHV_DOWNWARDS_OFFSET;
            cappy->oPosZ = m->pos[2] + CAPPY_BHV_DOWNWARDS_OFFSET * coss(m->faceAngle[1]);
            cappy->oVelX = 0.f;
            cappy->oVelY = -CAPPY_BHV_DOWNWARDS_VEL;
            cappy->oVelZ = 0.f;
            break;

        case OMM_CAPPY_BHV_SPIN_GROUND:
        case OMM_CAPPY_BHV_SPIN_AIR:
            cappy->oPosX = m->pos[0];
            cappy->oPosY = m->pos[1] + CAPPY_BHV_SPIN_OFFSET;
            cappy->oPosZ = m->pos[2];
            cappy->oFaceAngleYaw = m->faceAngle[1];
            break;

        case OMM_CAPPY_BHV_FLYING:
            cappy->oPosX = m->pos[0];
            cappy->oPosY = m->pos[1] + CAPPY_BHV_FLYING_OFFSET;
            cappy->oPosZ = m->pos[2];
            cappy->oFaceAngleYaw = m->faceAngle[1];
            break;
    }
}

//
// Step
//

static bool omm_cappy_collision_handler_floor_ceil_slowdown(struct Object *cappy, f32 *velX, f32 *velY, f32 *velZ, struct Surface *srf) {
    f32 nny = 1.f - abs_f(srf->normal.y);
    f32 slowdown = nny * nny * nny;
    cappy->oVelX /= (1.f + slowdown);
    cappy->oVelZ /= (1.f + slowdown);
    cappy->oVelY = 0;
    *velX = cappy->oVelX;
    *velY = cappy->oVelY;
    *velZ = cappy->oVelZ;
    return false;
}

static bool omm_cappy_collision_handler_floor_change_behavior(struct Object *cappy, f32 *velX, f32 *velY, f32 *velZ, UNUSED struct Surface *srf) {
    if (cappy->oVelY != 0.f) {
        cappy->oVelX = (CAPPY_BHV_DEFAULT_VEL / CAPPY_BHV_DOWNWARDS_VEL) * sins(cappy->oCappyYaw) * abs_f(cappy->oVelY);
        cappy->oVelZ = (CAPPY_BHV_DEFAULT_VEL / CAPPY_BHV_DOWNWARDS_VEL) * coss(cappy->oCappyYaw) * abs_f(cappy->oVelY);
        cappy->oVelY = 0;
        *velX = cappy->oVelX;
        *velY = cappy->oVelY;
        *velZ = cappy->oVelZ;
    }
    return false;
}

static bool omm_cappy_collision_handler_wall(struct Object *cappy, f32 *velX, f32 *velY, f32 *velZ, struct WallCollisionData *wcd) {
    static const s16 sCappyWallAngleStop = 0x2000;
    s16 wallDYaw = atan2s(wcd->walls[0]->normal.z, wcd->walls[0]->normal.x) - atan2s(cappy->oVelZ, cappy->oVelX);
    if ((wallDYaw < +sCappyWallAngleStop || wallDYaw > +(0x8000 - sCappyWallAngleStop)) &&
        (wallDYaw > -sCappyWallAngleStop || wallDYaw < -(0x8000 - sCappyWallAngleStop))) {
        cappy->oVelX = 0;
        cappy->oVelY = 0;
        cappy->oVelZ = 0;
        *velX = cappy->oVelX;
        *velY = cappy->oVelY;
        *velZ = cappy->oVelZ;
        return true;
    }
    return false;
}

static bool omm_cappy_collision_handler_wall_full_stop(UNUSED struct Object *cappy, UNUSED f32 *velX, UNUSED f32 *velY, UNUSED f32 *velZ, UNUSED struct WallCollisionData *wcd) {
    return true;
}

static void omm_cappy_slowdown(struct Object *cappy) {
    cappy->oVelX /= CAPPY_SLOWDOWN_FACTOR;
    cappy->oVelY /= CAPPY_SLOWDOWN_FACTOR;
    cappy->oVelZ /= CAPPY_SLOWDOWN_FACTOR;
}

#define STEPS 16
static void omm_cappy_perform_step(struct Object *cappy, struct MarioState *m, f32 velX, f32 velY, f32 velZ,
    bool (*wallCollisionHandle)  (struct Object *, f32 *, f32 *, f32 *, struct WallCollisionData *),
    bool (*floorCollisionHandle) (struct Object *, f32 *, f32 *, f32 *, struct Surface *),
    bool (*ceilCollisionHandle)  (struct Object *, f32 *, f32 *, f32 *, struct Surface *)
) {
    for (s32 i = 0; i != STEPS; ++i) {
        f32 x = cappy->oPosX;
        f32 y = cappy->oPosY;
        f32 z = cappy->oPosZ;
        cappy->oPosX += (velX / STEPS);
        cappy->oPosY += (velY / STEPS);
        cappy->oPosZ += (velZ / STEPS);

        // Cappy's homing attack ignores collisions if HAGR is enabled
        if (OMM_CHEAT_HOMING_ATTACK_GLOBAL_RANGE && (cappy->oCappyFlags & CAPPY_FLAG_HOMING_ATTACK)) {
            continue;
        }

        // Walls
        struct WallCollisionData hitbox;
        hitbox.x = cappy->oPosX;
        hitbox.y = cappy->oPosY;
        hitbox.z = cappy->oPosZ;
        hitbox.offsetY = cappy->hitboxHeight / 2;
        hitbox.radius = cappy->oWallHitboxRadius;
        if (find_wall_collisions(&hitbox) != 0) {

            // Improved Vanish cap makes Cappy ignore wall collisions
            if (!omm_mario_has_vanish_cap(m)) {
                cappy->oPosX = hitbox.x;
                cappy->oPosY = hitbox.y;
                cappy->oPosZ = hitbox.z;
                if (wallCollisionHandle && wallCollisionHandle(cappy, &velX, &velY, &velZ, &hitbox)) {
                    return;
                }
            }
        }

        // Floor
        struct Surface *floor = NULL;
        f32 floorY = find_floor(cappy->oPosX, cappy->oPosY, cappy->oPosZ, &floor);
        if (floor != NULL) {
            f32 diffY = cappy->oPosY - floorY;
            if (diffY < 0) {
                cappy->oPosY = floorY;
                if (floorCollisionHandle && floorCollisionHandle(cappy, &velX, &velY, &velZ, floor)) {
                    return;
                }
            }
        } else {
            cappy->oPosX = x;
            cappy->oPosY = y;
            cappy->oPosZ = z;
            return;
        }

        // Ceiling
        struct Surface *ceil = NULL;
        f32 ceilY = find_ceil(cappy->oPosX, cappy->oPosY, cappy->oPosZ, &ceil);
        if (ceil != NULL) {
            f32 h = (cappy->hitboxHeight - cappy->hitboxDownOffset);
            if (cappy->oPosY <= ceilY && cappy->oPosY + h > ceilY) {

                // Improved Vanish cap makes Cappy ignore ceiling collisions
                if (!omm_mario_has_vanish_cap(m)) {
                    cappy->oPosY = max_f(ceilY - h, floorY);
                    if (ceilCollisionHandle && ceilCollisionHandle(cappy, &velX, &velY, &velZ, ceil)) {
                        return;
                    }
                }
            }
        }
    }
}
#undef STEPS

void omm_cappy_return_to_mario(struct Object *cappy) {
    if (OMM_LIKELY(cappy)) {
        cappy->oCappyLifeTimer = CAPPY_LIFETIME;
    }
}

bool omm_cappy_perform_step_return_to_mario(struct Object *cappy, struct MarioState *m) {

    // Cheat: Teleports to Cappy
    if (m->controller->buttonPressed & X_BUTTON) {
        if (OMM_CHEAT_MARIO_TELEPORTS_TO_CAPPY && omm_cappy_is_mario_available(m, false)) {
            struct Surface *floor;
            find_floor(cappy->oPosX, cappy->oPosY, cappy->oPosZ, &floor);
            if (floor) {
                m->pos[0] = cappy->oPosX;
                m->pos[1] = cappy->oPosY;
                m->pos[2] = cappy->oPosZ;
                m->vel[1] = 0.f;
                omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
                play_sound(SOUND_ACTION_TELEPORT, m->marioObj->oCameraToObject);
                return true;
            }
        }
    }

    // Move Cappy closer to Mario
    f32 dx = (m->pos[0] - cappy->oPosX);
    f32 dy = (m->pos[1] - cappy->oPosY) + (0.4f * m->marioObj->hitboxHeight * m->marioObj->oScaleY);
    f32 dz = (m->pos[2] - cappy->oPosZ);
    f32 dv = sqrtf(sqr_f(dx) + sqr_f(dy) + sqr_f(dz));
    if (dv > CAPPY_RETURN_VEL) {
        cappy->oPosX += (dx / dv) * CAPPY_RETURN_VEL;
        cappy->oPosY += (dy / dv) * CAPPY_RETURN_VEL;
        cappy->oPosZ += (dz / dv) * CAPPY_RETURN_VEL;
    } else {
        cappy->oPosX += dx;
        cappy->oPosY += dy;
        cappy->oPosZ += dz;
    }

    // Unloads Cappy if he's close enough to Mario
    f32 rMario = (50.f * m->marioObj->oScaleX);
    f32 rCappy = (40.f * cappy->oScaleX);
    return (dv - CAPPY_RETURN_VEL) <= (rMario + rCappy);
}

//
// Callback
//

// Press one of the D-Pad buttons or press X again to call Cappy back
// Cappy does a quick homing attack before returning if the D-Pad is pressed
static void omm_cappy_call_back(struct Object *cappy, struct MarioState *m, s32 frameStart) {
    if (cappy->oCappyLifeTimer >= frameStart) {
        u32 udlrx = (m->controller->buttonPressed & (U_JPAD | D_JPAD | L_JPAD | R_JPAD | X_BUTTON));
        if (udlrx != 0) {

            // D-Pad
            if (!(udlrx & X_BUTTON)) {
                s32 cappyHomingAttackDuration = CAPPY_HOMING_ATTACK_DURATION;
                cappy->oCappyLifeTimer = CAPPY_LIFETIME - cappyHomingAttackDuration - 1;
                cappy->oCappyFlags |= CAPPY_FLAG_HOMING_ATTACK;

                // Targets the nearest interactable object
                struct Object *target = omm_cappy_find_target(CAPPY_HOMING_ATTACK_ORIGIN, m, CAPPY_HOMING_ATTACK_VEL * cappyHomingAttackDuration);
                if (target) {
                    f32 dx = target->oPosX - cappy->oPosX;
                    f32 dy = target->oPosY - cappy->oPosY;
                    f32 dz = target->oPosZ - cappy->oPosZ;
                    f32 dv = sqrtf(sqr_f(dx) + sqr_f(dy) + sqr_f(dz));
                    if (dv != 0) {
                        cappy->oVelX = CAPPY_HOMING_ATTACK_VEL * (dx / dv);
                        cappy->oVelY = CAPPY_HOMING_ATTACK_VEL * (dy / dv);
                        cappy->oVelZ = CAPPY_HOMING_ATTACK_VEL * (dz / dv);
                    } else {
                        omm_cappy_return_to_mario(cappy);
                    }
                }
                
                // If no target, just move Cappy in a fixed direction
                else {
                    switch (cappy->oCappyBehavior) {

                        case OMM_CAPPY_BHV_DEFAULT_GROUND:
                        case OMM_CAPPY_BHV_DEFAULT_AIR:
                        case OMM_CAPPY_BHV_UPWARDS_GROUND:
                        case OMM_CAPPY_BHV_UPWARDS_AIR:
                        case OMM_CAPPY_BHV_DOWNWARDS_GROUND:
                        case OMM_CAPPY_BHV_DOWNWARDS_AIR: {
                            switch (udlrx) {
                                case U_JPAD: {
                                    cappy->oVelX = 0.f;
                                    cappy->oVelY = CAPPY_HOMING_ATTACK_VEL;
                                    cappy->oVelZ = 0.f;
                                } break;

                                case D_JPAD: {
                                    cappy->oVelX = 0.f;
                                    cappy->oVelY = -CAPPY_HOMING_ATTACK_VEL;
                                    cappy->oVelZ = 0.f;
                                } break;

                                case L_JPAD: {
                                    cappy->oVelX = CAPPY_HOMING_ATTACK_VEL * sins(cappy->oCappyYaw + 0x4000);
                                    cappy->oVelY = 0.f;
                                    cappy->oVelZ = CAPPY_HOMING_ATTACK_VEL * coss(cappy->oCappyYaw + 0x4000);
                                } break;

                                case R_JPAD: {
                                    cappy->oVelX = CAPPY_HOMING_ATTACK_VEL * sins(cappy->oCappyYaw - 0x4000);
                                    cappy->oVelY = 0.f;
                                    cappy->oVelZ = CAPPY_HOMING_ATTACK_VEL * coss(cappy->oCappyYaw - 0x4000);
                                } break;

                                default: { // if two or more D-pads buttons are pressed simultaneously and no target was found, cancel the homing attack
                                    omm_cappy_return_to_mario(cappy);
                                } break;
                            }
                        } break;

                        case OMM_CAPPY_BHV_SPIN_GROUND:
                        case OMM_CAPPY_BHV_SPIN_AIR:
                        case OMM_CAPPY_BHV_FLYING:
                        default: {
                            omm_cappy_return_to_mario(cappy);
                        } break;
                    }
                }
            }
            
            // X Button
            else {
                omm_cappy_return_to_mario(cappy);
            }
        }
    }
}

//
// Update
//

void omm_cappy_update_behavior(struct Object *cappy, struct MarioState *m) {

    // Homing attack
    if (cappy->oCappyFlags & CAPPY_FLAG_HOMING_ATTACK) {
        cappy->oCappyFlags &= ~CAPPY_FLAG_INTERACT_MARIO;
        if (m->controller->buttonPressed & X_BUTTON) {
            omm_cappy_return_to_mario(cappy);
        } else {
            omm_cappy_perform_step(cappy, m, cappy->oVelX, cappy->oVelY, cappy->oVelZ, NULL, NULL, NULL);
        }
        return;
    }

    // Standard behavior
    switch (cappy->oCappyBehavior) {

        case OMM_CAPPY_BHV_DEFAULT_GROUND:
        case OMM_CAPPY_BHV_DEFAULT_AIR: {
            if (cappy->oCappyLifeTimer < CAPPY_BHV_DEFAULT_CALL_BACK_START) {
                omm_cappy_perform_step(cappy, m, cappy->oVelX, cappy->oVelY, cappy->oVelZ, omm_cappy_collision_handler_wall, omm_cappy_collision_handler_floor_ceil_slowdown, omm_cappy_collision_handler_floor_ceil_slowdown);
                omm_cappy_slowdown(cappy);
                cappy->oCappyFlags &= ~CAPPY_FLAG_INTERACT_MARIO;
            } else {
                omm_cappy_call_back(cappy, m, CAPPY_BHV_DEFAULT_CALL_BACK_START);
                cappy->oCappyFlags |= CAPPY_FLAG_INTERACT_MARIO;
            }
        } break;

        case OMM_CAPPY_BHV_UPWARDS_GROUND:
        case OMM_CAPPY_BHV_UPWARDS_AIR: {
            if (cappy->oCappyLifeTimer < CAPPY_BHV_UPWARDS_CALL_BACK_START) {
                omm_cappy_perform_step(cappy, m, cappy->oVelX, cappy->oVelY, cappy->oVelZ, NULL, NULL, omm_cappy_collision_handler_floor_ceil_slowdown);
                omm_cappy_slowdown(cappy);
                cappy->oCappyFlags &= ~CAPPY_FLAG_INTERACT_MARIO;
            } else {
                omm_cappy_call_back(cappy, m, CAPPY_BHV_UPWARDS_CALL_BACK_START);
                cappy->oCappyFlags |= CAPPY_FLAG_INTERACT_MARIO;
            }
        } break;

        case OMM_CAPPY_BHV_DOWNWARDS_GROUND:
        case OMM_CAPPY_BHV_DOWNWARDS_AIR: {
            if (cappy->oCappyLifeTimer < CAPPY_BHV_DOWNWARDS_CALL_BACK_START) {
                omm_cappy_perform_step(cappy, m, cappy->oVelX, cappy->oVelY, cappy->oVelZ, NULL, omm_cappy_collision_handler_floor_change_behavior, NULL);
                omm_cappy_slowdown(cappy);
                if (cappy->oVelY == 0.f) {
                    cappy->oVelY = -sqrtf(sqr_f(cappy->oVelX) + sqr_f(cappy->oVelZ)) / (CAPPY_BHV_DEFAULT_VEL / CAPPY_BHV_DOWNWARDS_VEL);
                    cappy->oVelX = 0;
                    cappy->oVelZ = 0;
                }
                cappy->oCappyFlags &= ~CAPPY_FLAG_INTERACT_MARIO;
            } else {
                omm_cappy_call_back(cappy, m, CAPPY_BHV_DOWNWARDS_CALL_BACK_START);
                cappy->oCappyFlags |= CAPPY_FLAG_INTERACT_MARIO;
            }
        } break;

        case OMM_CAPPY_BHV_SPIN_GROUND:
        case OMM_CAPPY_BHV_SPIN_AIR: {
            f32 r = min_f(cappy->oCappyLifeTimer * CAPPY_BHV_SPIN_RADIUS_GROWTH, CAPPY_BHV_SPIN_RADIUS_MAX);
            s16 a = (s16) (cappy->oFaceAngleYaw + (s32) (cappy->oCappyLifeTimer) * CAPPY_BHV_SPIN_ANGLE_VEL);
            cappy->oPosX = m->pos[0];
            cappy->oPosY = m->pos[1] + CAPPY_BHV_SPIN_OFFSET;
            cappy->oPosZ = m->pos[2];
            omm_cappy_perform_step(cappy, m, r * coss(a), 0, r * sins(a), omm_cappy_collision_handler_wall_full_stop, NULL, NULL);
            omm_cappy_call_back(cappy, m, CAPPY_BHV_SPIN_CALL_BACK_START);
            cappy->oCappyFlags &= ~CAPPY_FLAG_INTERACT_MARIO;
            cappy->oCappyYaw = a;
        } break;

        case OMM_CAPPY_BHV_FLYING: {
            if (m->action != ACT_FLYING) {
                omm_cappy_return_to_mario(cappy);
            } else {
                if (cappy->oCappyLifeTimer < CAPPY_BHV_FLYING_CALL_BACK_START) {
                    m->marioObj->oGfxAngle[2] += ((cappy->oCappyLifeTimer * 0x10000) / CAPPY_BHV_FLYING_CALL_BACK_START);
                }
                f32 r = min_f(cappy->oCappyLifeTimer * CAPPY_BHV_FLYING_RADIUS_GROWTH, CAPPY_BHV_FLYING_RADIUS_MAX);
                s16 a = (s16) (cappy->oFaceAngleYaw + (s32) (cappy->oCappyLifeTimer) * CAPPY_BHV_FLYING_ANGLE_VEL);
                Vec3f v = { r * coss(a), r * sins(a), 0 };
                vec3f_rotate_zxy(v, v, -m->faceAngle[0], m->faceAngle[1], 0);
                cappy->oPosX = m->pos[0];
                cappy->oPosY = m->pos[1] + CAPPY_BHV_FLYING_OFFSET;
                cappy->oPosZ = m->pos[2];
                omm_cappy_perform_step(cappy, m, v[0], v[1], v[2], omm_cappy_collision_handler_wall_full_stop, NULL, NULL);
            }
            omm_cappy_call_back(cappy, m, CAPPY_BHV_FLYING_CALL_BACK_START);
            cappy->oCappyFlags &= ~CAPPY_FLAG_INTERACT_MARIO;
            cappy->oCappyYaw = m->faceAngle[1];
        } break;
    }
}

//
// Play as Cappy
//

OMM_ROUTINE_UPDATE(omm_cappy_update_play_as) {
    struct Object *cappy = omm_cappy_get_object();
    if (cappy) {
        if (OMM_CHEAT_PLAY_AS_CAPPY && !omm_is_game_paused() &&
            cappy->oCappyLifeTimer >= CAPPY_BHV_DEFAULT_CALL_BACK_START &&
            cappy->oCappyLifeTimer < CAPPY_LIFETIME &&
            cappy->oCappyBehavior < OMM_CAPPY_BHV_SPIN_GROUND &&
            !(cappy->oCappyFlags & CAPPY_FLAG_HOMING_ATTACK)) {
            
            // Move Cappy unless first person mode or time stop
            cappy->oCappyFlags |= (CAPPY_FLAG_INTERACT_MARIO | CAPPY_FLAG_PLAY_AS);
            struct MarioState *m = gMarioState;
            if (m->action != ACT_FIRST_PERSON && !(gTimeStopState & TIME_STOP_MARIO_AND_DOORS)) {
                s16 yaw = omm_camera_get_intended_yaw(m);
                cappy->oVelX = CAPPY_PLAY_AS_H_VEL * (m->controller->stickMag / 64.f) * sins(yaw);
                cappy->oVelY = CAPPY_PLAY_AS_V_VEL * (((m->controller->buttonDown & A_BUTTON) / A_BUTTON) - ((m->controller->buttonDown & B_BUTTON) / B_BUTTON));
                cappy->oVelZ = CAPPY_PLAY_AS_H_VEL * (m->controller->stickMag / 64.f) * coss(yaw);
                omm_cappy_perform_step(cappy, m, cappy->oVelX, cappy->oVelY, cappy->oVelZ, NULL, NULL, NULL);
                omm_cappy_call_back(cappy, m, CAPPY_BHV_DEFAULT_CALL_BACK_START);
            }
            
            // Reset inputs so Mario doesn't move
            m->controller->rawStickX = 0;
            m->controller->rawStickY = 0;
            m->controller->stickX = 0.f;
            m->controller->stickY = 0.f;
            m->controller->stickMag = 0.f;
            m->controller->buttonPressed &= ~(A_BUTTON | B_BUTTON | X_BUTTON | Y_BUTTON | Z_TRIG);
            m->controller->buttonDown &= ~(A_BUTTON | B_BUTTON | X_BUTTON | Y_BUTTON | Z_TRIG);
            m->actionTimer *= (m->action != ACT_IDLE);
        } else {
            cappy->oCappyFlags &= ~(CAPPY_FLAG_PLAY_AS | CAPPY_FLAG_CAMERA);
        }
    }
}

//
// Mario throw anim
//

typedef struct MarioThrowAnimParams { u32 anim; f32 speed; s16 start; s16 end; s32 sound; s32 cappy; } MarioThrowAnimParams;
static const MarioThrowAnimParams *omm_cappy_get_mario_anim_params(s32 cappyBehavior) {
    static const MarioThrowAnimParams sMarioThrowAnimParams[2][9] = {
    { { MARIO_ANIM_GROUND_THROW,                 1.50f,  0, -1, 0, -4 },
      { MARIO_ANIM_GROUND_THROW,                 1.50f,  0, -1, 0, -4 },
      { MARIO_ANIM_GROUND_KICK,                  1.50f,  0, -1, 0, -4 },
      { MARIO_ANIM_AIR_KICK,                     1.50f,  0, -1, 0, -4 },
      { MARIO_ANIM_GROUND_THROW,                 1.50f,  0, -1, 0, -4 },
      { MARIO_ANIM_GROUND_THROW,                 1.50f,  0, -1, 0, -4 },
      { MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN, 1.50f, 62, 94, 3,  0 },
      { MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN, 1.50f, 62, 94, 3,  0 },
      { MARIO_ANIM_WING_CAP_FLY,                 1.00f,  0, -1, 0,  0 }, },
    { { MARIO_ANIM_OMM_CAPPY_THROW,              1.50f,  0, 28, 0, -4 },
      { MARIO_ANIM_OMM_CAPPY_THROW,              1.50f, 31, -1, 0, -4 },
      { MARIO_ANIM_OMM_CAPPY_UP_THROW,           1.25f,  0, 27, 0, -4 },
      { MARIO_ANIM_OMM_CAPPY_UP_THROW,           1.25f, 29, -1, 0, -4 },
      { MARIO_ANIM_OMM_CAPPY_DOWN_THROW,         1.25f,  0, 27, 0, -4 },
      { MARIO_ANIM_OMM_CAPPY_DOWN_THROW,         1.25f, 29, -1, 0, -4 },
      { MARIO_ANIM_OMM_CAPPY_SPIN_THROW,         1.25f,  0, 37, 3,  0 },
      { MARIO_ANIM_OMM_CAPPY_SPIN_THROW,         1.25f, 39, -1, 3,  0 },
      { MARIO_ANIM_WING_CAP_FLY,                 1.00f,  0, -1, 0,  0 }, }
    };
    return &sMarioThrowAnimParams[OMM_EXTRAS_SMO_ANIMATIONS][cappyBehavior];
}

static const s32 sThrowSounds[] = {
    SOUND_MARIO_PUNCH_YAH,
    SOUND_MARIO_PUNCH_WAH,
    SOUND_MARIO_PUNCH_HOO,
    SOUND_MARIO_YAHOO_WAHA_YIPPEE + (0 << 16), // Yahoo
    SOUND_MARIO_YAHOO_WAHA_YIPPEE + (3 << 16), // Waha
    SOUND_MARIO_YAHOO_WAHA_YIPPEE + (4 << 16), // Yippee
    SOUND_ACTION_METAL_STEP_WATER,
    SOUND_ACTION_METAL_STEP_WATER,
    SOUND_ACTION_METAL_STEP_WATER,
    SOUND_ACTION_METAL_JUMP_WATER,
    SOUND_ACTION_METAL_JUMP_WATER,
    SOUND_ACTION_METAL_JUMP_WATER,
};

static const u32 sMarioThrowEndAction[] = {
    ACT_IDLE,
    ACT_FREEFALL,
    ACT_OMM_METAL_WATER_IDLE,
    ACT_OMM_METAL_WATER_FREEFALL,
    ACT_WATER_IDLE,
    ACT_WATER_IDLE,
};

void omm_cappy_update_mario_anim(struct Object *cappy, struct MarioState *m) {
    static struct Object sCappy = { 0 };
    if (cappy == NULL) {
        cappy = &sCappy;
    }

    // Params
    const MarioThrowAnimParams *p = omm_cappy_get_mario_anim_params(cappy->oCappyBehavior);
    cappy->oCappyLifeTimer = max_s(cappy->oCappyLifeTimer, p->cappy);

    // Skip if Mario is not in a throw action
    if (m->action != ACT_OMM_CAPPY_THROW_GROUND &&
        m->action != ACT_OMM_CAPPY_THROW_AIRBORNE &&
        m->action != ACT_OMM_CAPPY_THROW_WATER &&
        m->action != ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND &&
        m->action != ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE) {
        cappy->oCappyFlags &= ~CAPPY_FLAG_START_ANIM;
    } else {
        bool water = ((m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED);
        bool metal = ((m->action & ACT_GROUP_MASK) == ACT_GROUP_METAL_WATER);
    
        // Start
        if (cappy->oCappyFlags & CAPPY_FLAG_START_ANIM) {
            obj_anim_play(m->marioObj, p->anim, p->speed);
            obj_anim_set_frame(m->marioObj, p->start);
            play_sound(sThrowSounds[p->sound + (random_u16() % 3) + (6 * metal)], m->marioObj->oCameraToObject);
            cappy->oCappyFlags &= ~CAPPY_FLAG_START_ANIM;
        }

        // End
        u32 endAction = sMarioThrowEndAction[(cappy->oCappyBehavior & 1) + (2 * metal) + (4 * water)];
        if (p->end == -1) {
            if (obj_anim_is_at_end(m->marioObj)) {
                omm_mario_set_action(m, endAction, 0, 0);
                omm_mario_init_next_action(m);
            }
        } else {
            if (obj_anim_get_frame(m->marioObj) >= p->end) {
                omm_mario_set_action(m, endAction, 0, 0);
                omm_mario_init_next_action(m);
            }
        }
    }

    // Make a copy of Cappy values
    if (cappy != &sCappy) {
        OMM_MEMCPY(&sCappy.rawData, &cappy->rawData, sizeof(cappy->rawData));
#if IS_64_BIT
        OMM_MEMCPY(&sCappy.ptrData, &cappy->ptrData, sizeof(cappy->ptrData));
#endif
    }
}

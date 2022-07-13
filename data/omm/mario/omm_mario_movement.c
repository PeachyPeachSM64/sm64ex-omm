#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

/*
Handling works like this:
The faster Mario is, the harder he's to control
Mario's controls are very responsive when he's still,
allowing him to quickly change direction, but become
less and less responsive as he's gaining speed.
OMM_MARIO_HANDLING_MODIFIER controls the shape
of the graph between vMin and vMax.
OMM_MARIO_HANDLING_DISABLE sets the velocity to
a high value, disabling completely the handling.

hMax _|______
      |      \
      |       \
hMin _|        \________
      |_________________
       vMin |   | vMax
*/

#define OMM_MARIO_HANDLING_DISABLE      100.f
#define OMM_MARIO_HANDLING_MODIFIER(x)  (sqr_f(x))

static f32 omm_mario_get_handling_velocity(struct MarioState *m) {
    switch (m->action) {
    case ACT_DIVE:                          return OMM_MARIO_HANDLING_DISABLE;
    case ACT_BACKFLIP:                      return OMM_MARIO_HANDLING_DISABLE;
    case ACT_SIDE_FLIP:                     return OMM_MARIO_HANDLING_DISABLE;
    case ACT_LONG_JUMP:                     return OMM_MARIO_HANDLING_DISABLE;
    case ACT_WALL_KICK_AIR:                 return OMM_MARIO_HANDLING_DISABLE;
    case ACT_OMM_METAL_WATER_DIVE:          return OMM_MARIO_HANDLING_DISABLE;
    case ACT_OMM_METAL_WATER_BACKFLIP:      return OMM_MARIO_HANDLING_DISABLE;
    case ACT_OMM_METAL_WATER_SIDE_FLIP:     return OMM_MARIO_HANDLING_DISABLE;
    case ACT_OMM_METAL_WATER_LONG_JUMP:     return OMM_MARIO_HANDLING_DISABLE;
    case ACT_OMM_METAL_WATER_WALL_KICK_AIR: return OMM_MARIO_HANDLING_DISABLE;
    default:                                return abs_f(m->forwardVel);
    }
}

static f32 omm_mario_get_handling_magnitude(struct MarioState *m) {
    if (m->action == ACT_OMM_ROLL_AIR)    return m->intendedMag / 64.f;
    if (m->action & ACT_FLAG_METAL_WATER) return m->intendedMag / 48.f;
    return m->intendedMag / 32.f;
}

static s32 omm_mario_get_handling(struct MarioState *m, f32 velMin, f32 velMax, f32 handlingMin, f32 handlingMax) {
    if (OMM_CHEAT_SUPER_RESPONSIVE) return 0x8000;
    f32 velocity  = omm_mario_get_handling_velocity(m);
    f32 magnitude = omm_mario_get_handling_magnitude(m);
    return (s32) ((handlingMin + OMM_MARIO_HANDLING_MODIFIER(1.f - invlerp_0_1_f(velocity, velMin, velMax)) * (handlingMax - handlingMin)) * magnitude);
}

//
// Mario's ground movement
//

bool omm_mario_update_walking_speed(struct MarioState *m) {
    if (OMM_LIKELY(OMM_MOVESET_ODYSSEY)) {
        f32 speedTgt = max_f(1.f, min_f(m->intendedMag, 32.f) * (1.f / max_f(1.f, m->quicksandDepth / 6.25f)));
        m->forwardVel = min_f(m->forwardVel + 1.2f * (1.f - (max_f(0.f, m->forwardVel) / speedTgt)), 48.f);
        s32 handling = omm_mario_get_handling(m, 8.f, 32.f, 0x800, 0x4000);
        m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, handling, handling);
        apply_slope_accel(m);
        return true;
    }
    return false;
}

//
// Mario's air movement
//

// Personally, I prefer the air
static bool update_air(struct MarioState *m, bool turn) {
    if (OMM_LIKELY(OMM_MOVESET_ODYSSEY)) {
        if (omm_mario_has_metal_cap(m) || !check_horizontal_wind(m)) {
            f32 dragThreshold = (m->action == ACT_LONG_JUMP ? 48.f : 32.f);
            f32 sidewaysSpeed = 0.f;
            m->forwardVel = approach_f32(m->forwardVel, 0.0f, 0.35f, 0.35f);

            if (m->input & INPUT_NONZERO_ANALOG) {
                if (m->action != ACT_BACKFLIP && m->action != ACT_OMM_METAL_WATER_BACKFLIP && ((
                    m->action != ACT_LONG_JUMP && m->action != ACT_OMM_METAL_WATER_LONG_JUMP) || !OMM_CHEAT_BLJ_ANYWHERE)) {
                    s32 handling     = omm_mario_get_handling(m, 8.f, 24.f, 0x200, 0x2000);
                    m->forwardVel   += 1.5f * coss(m->intendedYaw - m->faceAngle[1]) * omm_mario_get_handling_magnitude(m);
                    m->faceAngle[1]  = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, handling, handling);
                } else {
                    s16 intendedDYaw = m->intendedYaw - m->faceAngle[1];
                    f32 intendedMag  = m->intendedMag / 32.0f;
                    m->forwardVel   +=   1.5f * coss(intendedDYaw) * intendedMag;
                    if (turn) {
                    m->faceAngle[1] += 512.0f * sins(intendedDYaw) * intendedMag;
                    } else {
                    sidewaysSpeed    =  10.0f * sins(intendedDYaw) * intendedMag;
                    }
                }
            }

            if (m->forwardVel > dragThreshold) m->forwardVel -= 1.0f;
            if (m->forwardVel < -16.0f) m->forwardVel += 2.0f;

            m->slideVelX  = m->forwardVel * sins(m->faceAngle[1]);
            m->slideVelZ  = m->forwardVel * coss(m->faceAngle[1]);
            m->slideVelX += sidewaysSpeed * sins(m->faceAngle[1] + 0x4000);
            m->slideVelZ += sidewaysSpeed * coss(m->faceAngle[1] + 0x4000);
            m->vel[0]     = m->slideVelX;
            m->vel[2]     = m->slideVelZ;
        }
        return true;
    }
    return false;
}

bool omm_mario_update_air_without_turn(struct MarioState *m) {
    return update_air(m, false);
}

bool omm_mario_update_air_with_turn(struct MarioState *m) {
    return update_air(m, true);
}

//
// Mario's hang movement
//

bool omm_mario_update_hanging_speed(struct MarioState *m) {
    if (OMM_LIKELY(OMM_MOVESET_ODYSSEY)) {
        m->forwardVel = approach_f32(m->forwardVel, OMM_MARIO_HANG_MAX_SPEED * min_f(m->intendedMag, 32.f) / 32.f, 2.f, 4.f);
        s32 handling = omm_mario_get_handling(m, 4.f, OMM_MARIO_HANG_MAX_SPEED, 0x800, 0x2000);
        m->faceAngle[1] = m->intendedYaw - approach_s32((s16) (m->intendedYaw - m->faceAngle[1]), 0, handling, handling);
        return true;
    }
    return false;
}

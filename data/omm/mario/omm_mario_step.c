#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#if OMM_GAME_IS_R96A
#define speed_modifier(...)     cheats_speed_modifier(__VA_ARGS__)
#define jump_modifier(...)      cheats_jump_modifier(__VA_ARGS__)
#define swim_modifier(...)      cheats_swim_modifier(__VA_ARGS__)
#define walk_on_quicksand(...)  cheats_walk_on_hazards(__VA_ARGS__)
#else
#define speed_modifier(...)     (OMM_CHEAT_SUPER_SPEED ? 3 : 1)
#define jump_modifier(...)      (OMM_CHEAT_SUPER_SPEED ? 3 : 1)
#define swim_modifier(...)      (OMM_CHEAT_SUPER_SPEED ? 3 : 1)
#define walk_on_quicksand(...)  (OMM_CHEAT_WALK_ON_QUICKSAND)
#endif

// Enhanced Mario step system
// Enabled by setting Moveset to Odyssey
// Adds several fixes and increases accuracy:
// - Increases number of sub-steps from 4/4/1 to 16/16/16, for more precise surface detection
// - Fixes exposed ceilings, they no longer extends infinitely upwards, but up to half Mario's height
// - Out of bounds does not make Mario bonk, but prevents him from moving horizontally
// - Ceilings does not make Mario bonk, but prevents him from moving horizontally
// - Fixes glitchy ledge grabs (RIP cannonless)
// - Fixes feet stuck on moving platforms
// - Fixes underwater up/downwarps
// - Fixes whirlpool attractive strength

//
// Generalized steps
// Each generalized step is mapped to a specific step,
// depending on the performed step (ground, air, hang, water)
//

#define STEP_NONE               0
#define STEP_HIT_WALL           1
#define STEP_HIT_LAVA_WALL      2
#define STEP_HIT_FLOOR          3
#define STEP_LEFT_FLOOR         4
#define STEP_HIT_CEIL           5
#define STEP_LEFT_CEIL          6
#define STEP_SQUISH             7
#define STEP_OUT_OF_BOUNDS      8
#define STEP_GRABBED_LEDGE      9
#define STEP_GRABBED_CEIL       10

#define GROUND_STEP             0
#define AIR_STEP                1
#define HANG_STEP               2
#define WATER_STEP              3

static const s32 sStepResult[][4] = {
/* STEP_NONE          */ { GROUND_STEP_NONE,        AIR_STEP_NONE,            HANG_STEP_NONE,      WATER_STEP_NONE        },
/* STEP_HIT_WALL      */ { GROUND_STEP_HIT_WALL,    AIR_STEP_HIT_WALL,        HANG_STEP_HIT_WALL,  WATER_STEP_HIT_WALL    },
/* STEP_HIT_LAVA_WALL */ { GROUND_STEP_HIT_WALL,    AIR_STEP_HIT_LAVA_WALL,   HANG_STEP_HIT_WALL,  WATER_STEP_HIT_WALL    },
/* STEP_HIT_FLOOR     */ { GROUND_STEP_NONE,        AIR_STEP_LANDED,          HANG_STEP_NONE,      WATER_STEP_HIT_FLOOR   },
/* STEP_LEFT_FLOOR    */ { GROUND_STEP_LEFT_GROUND, AIR_STEP_NONE,            HANG_STEP_NONE,      WATER_STEP_NONE        },
/* STEP_HIT_CEIL      */ { GROUND_STEP_HIT_WALL,    AIR_STEP_HIT_WALL,        HANG_STEP_HIT_WALL,  WATER_STEP_HIT_CEILING },
/* STEP_LEFT_CEIL     */ { GROUND_STEP_NONE,        AIR_STEP_NONE,            HANG_STEP_LEFT_CEIL, WATER_STEP_NONE        },
/* STEP_SQUISH        */ { GROUND_STEP_HIT_WALL,    AIR_STEP_HIT_WALL,        HANG_STEP_HIT_WALL,  WATER_STEP_HIT_WALL    },
/* STEP_OUT_OF_BOUNDS */ { GROUND_STEP_HIT_WALL,    AIR_STEP_HIT_WALL,        HANG_STEP_HIT_WALL,  WATER_STEP_HIT_WALL    },
/* STEP_GRABBED_LEDGE */ { GROUND_STEP_NONE,        AIR_STEP_GRABBED_LEDGE,   HANG_STEP_NONE,      WATER_STEP_NONE        },
/* STEP_GRABBED_CEIL  */ { GROUND_STEP_NONE,        AIR_STEP_GRABBED_CEILING, HANG_STEP_NONE,      WATER_STEP_NONE        },
};

OMM_INLINE void clamp_mario_pos(struct MarioState *m) {
    m->pos[0] = clamp_f(m->pos[0], -LEVEL_BOUNDS, +LEVEL_BOUNDS);
    m->pos[1] =   min_f(m->pos[1], +20000.f);
    m->pos[2] = clamp_f(m->pos[2], -LEVEL_BOUNDS, +LEVEL_BOUNDS);
}

//
// Improved Vanish cap
// Mario can teleport through walls
// Max distance is proportional to Mario's forward vel
//

static bool omm_mario_check_vanish_cap_step(struct MarioState *m, Vec3f target, s32 stepResult, f32 yOffset, f32 radius) {
    if (stepResult == STEP_HIT_WALL && omm_mario_has_vanish_cap(m) && m->forwardVel != 0.f) {

        // Max distance
        f32 maxDist = 5.f * max_f(m->controller->stickMag / 2.f, abs_f(m->forwardVel));

        // Target direction
        Vec3f dir;
        vec3f_copy(dir, m->vel);
        vec3f_mul(dir, maxDist / m->forwardVel);

        // Target pos
        vec3f_sum(target, m->pos, dir);
        vec3f_mul(dir, -1.f);
        target[1] += yOffset;
        
        // Do a raycast from the target to Mario's current pos (after the wall collision)
        RayCollisionData hits;
        if (find_collisions_on_ray(target, dir, &hits, 1.f, RAYCAST_FLAG_WALLS)) {
            vec3f_copy(dir, (f32 *) &hits.hits[0].surf->normal);
            vec3f_mul(dir, radius * 0.8f);
            vec3f_copy(target, hits.hits[0].pos);
            vec3f_add(target, dir);
            target[1] -= yOffset;
            play_sound(SOUND_ACTION_TELEPORT, m->marioObj->oCameraToObject);
            m->wall = NULL;
            return true;
        }
    }
    return false;
}

static bool omm_object_check_move_through_walls(struct Object *o, Vec3f target) {
    if (o->oWall && o->oForwardVel != 0.f) {
        f32 radius = o->oWallHitboxRadius;
        f32 height = o->hitboxHeight;
        f32 offset = o->hitboxDownOffset;

        // Max distance
        f32 maxDist = 5.f * max_f(gPlayer1Controller->stickMag / 2.f, abs_f(o->oForwardVel));

        // Target direction
        Vec3f dir;
        vec3f_copy(dir, &o->oVelX);
        vec3f_mul(dir, maxDist / o->oForwardVel);

        // Target pos
        vec3f_sum(target, &o->oPosX, dir);
        vec3f_mul(dir, -1.f);
        target[1] += (height - offset) / 2.f;
        
        // Do a raycast from the target to the object's current pos (after the wall collision)
        RayCollisionData hits;
        if (find_collisions_on_ray(target, dir, &hits, 1.f, RAYCAST_FLAG_WALLS)) {
            vec3f_copy(dir, (f32 *) &hits.hits[0].surf->normal);
            vec3f_mul(dir, radius * 0.8f);
            vec3f_copy(target, hits.hits[0].pos);
            vec3f_add(target, dir);
            target[1] -= (height - offset) / 2.f;
            play_sound(SOUND_ACTION_TELEPORT, o->oCameraToObject);
            o->oWall = NULL;
            return true;
        }
    }
    return false;
}

//
// Stationary Ground Step
//

s32 stationary_ground_step(struct MarioState *m) {
    mario_set_forward_vel(m, 0.f);
    if (!(m->action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER))) {
        if (mario_update_moving_sand(m) || mario_update_windy_ground(m)) {
            return perform_ground_step(m);
        }
    }

    // Update gfx and return step result
    m->pos[1] = m->floorHeight;
    m->vel[1] = 0.f;
    clamp_mario_pos(m);
    gOmmData->mario->state.peakHeight = m->pos[1];
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
    return sStepResult[STEP_NONE][GROUND_STEP];
}

void stop_and_set_height_to_floor(struct MarioState *m) {
    mario_set_forward_vel(m, 0.f);
    m->vel[1] = 0.f;
    m->pos[1] = m->floorHeight = find_floor_height(m->pos[0], m->pos[1], m->pos[2]);
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
}

//
// Ground Step
//

struct Surface gWaterSurfacePseudoFloor = {
    SURFACE_VERY_SLIPPERY,
    0, 0, 0, 0, 0,
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 1, 0 },
    0, NULL
};

static s32 omm_mario_perform_ground_sub_step(struct MarioState *m, Vec3f nextPos) {

    // Walls
    resolve_and_return_wall_collisions(nextPos, 30.f, 24.f);
    struct Surface *wall = resolve_and_return_wall_collisions(nextPos, 60.f, 50.f);
    m->wall = wall;

    // Floor
    struct Surface *floor = NULL;
    f32 floorHeight = find_floor(nextPos[0], nextPos[1], nextPos[2], &floor);

    // Ceiling
    struct Surface *ceil = NULL;
    f32 ceilLower = find_ceil(nextPos[0], floorHeight + 80.f, nextPos[2], &ceil) - 160.f;
    f32 ceilUpper = (OMM_STEP_FIX_EXPOSED_CEILINGS ? (ceilLower + 240.f) : +20000.f);

    // Out of bounds
    if (!floor) {
        return (OMM_STEP_FIX_OUT_OF_BOUNDS_BONK ? STEP_NONE : STEP_OUT_OF_BOUNDS);
    }

    // Shell ride
    f32 waterLevel = find_water_level(nextPos[0], nextPos[2]);
    if ((m->action & ACT_FLAG_RIDING_SHELL) && floorHeight < waterLevel) {
        floorHeight = waterLevel;
        floor = &gWaterSurfacePseudoFloor;
        floor->originOffset = -waterLevel;
    }

    // Gloom Vibe allows Peach to walk on water
    if (omm_peach_vibe_is_gloom() && m->action == ACT_WALKING) {
        if (floorHeight <= waterLevel && nextPos[1] <= waterLevel + 80.f) {
            floorHeight = waterLevel - 12.f;
            nextPos[1] = floorHeight;
            floor = get_pseudo_floor_at_pos(nextPos[0], floorHeight, nextPos[2]);
            wall = NULL;
        }
    }

    // Left ground
    if (nextPos[1] > floorHeight + 100.f) {

        // Hit ceil
        if (nextPos[1] >= ceilLower && nextPos[1] < ceilUpper) {
            return (OMM_STEP_FIX_CEILINGS_BONK ? STEP_NONE : STEP_HIT_CEIL);
        }

        vec3f_copy(m->pos, nextPos);
        m->floor = floor;
        m->floorHeight = floorHeight;
        return STEP_LEFT_FLOOR;
    }

    // Squish
    if (floorHeight >= ceilLower && floorHeight < ceilUpper) {
        return STEP_SQUISH;
    }

    // On ground
    vec3f_set(m->pos, nextPos[0], floorHeight, nextPos[2]);
    m->floor = floor;
    m->floorHeight = floorHeight;
    if (wall) {
        s16 wallDYaw = atan2s(wall->normal.z, wall->normal.x) - m->faceAngle[1];
        if ((wallDYaw >= +0x2AAA && wallDYaw <= +0x5555) ||
            (wallDYaw <= -0x2AAA && wallDYaw >= -0x5555)) {
            return STEP_NONE;
        }

        // Lava wall
        if (wall->type == SURFACE_BURNING) {
            return STEP_HIT_LAVA_WALL;
        }

        // Push/Side-step
        return STEP_HIT_WALL;
    }
    return STEP_NONE;
}

s32 perform_ground_step(struct MarioState *m) {

    // Init steps
    s32 stepResult = STEP_NONE;
    f32 speedMul = omm_player_get_selected_ground_speed_multiplier();
    s32 numSteps = OMM_STEP_NUM_SUB_STEPS;
    s32 subSteps = numSteps * speed_modifier(m);

    // Perform steps
    for (s32 i = 0; i != subSteps; ++i) {
        Vec3f intendedPos;
        intendedPos[0] = m->pos[0] + (m->vel[0] * m->floor->normal.y * speedMul) / numSteps;
        intendedPos[1] = m->pos[1];
        intendedPos[2] = m->pos[2] + (m->vel[2] * m->floor->normal.y * speedMul) / numSteps;
        stepResult     = omm_mario_perform_ground_sub_step(m, intendedPos);
        if (stepResult == STEP_OUT_OF_BOUNDS ||
            stepResult == STEP_HIT_LAVA_WALL ||
            stepResult == STEP_LEFT_FLOOR ||
            stepResult == STEP_HIT_CEIL ||
            stepResult == STEP_SQUISH) {
            break;
        }
    }

    // Turn lava walls into regular walls with "Walk on Lava" cheat
    if (OMM_CHEAT_WALK_ON_LAVA && stepResult == STEP_HIT_LAVA_WALL) {
        stepResult = STEP_HIT_WALL;
    }

    // Vanish cap extra step
    Vec3f target;
    if (omm_mario_check_vanish_cap_step(m, target, stepResult, 60.f, 50.f)) {
        stepResult = omm_mario_perform_ground_sub_step(m, target);
        if (stepResult == STEP_HIT_WALL) {
            stepResult = STEP_NONE;
        }
    }

    // Update gfx and return step result
    m->vel[1] = 0.f;
    clamp_mario_pos(m);
    gOmmData->mario->state.peakHeight = m->pos[1];
    m->terrainSoundAddend = mario_get_terrain_sound_addend(m);
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
    return sStepResult[stepResult][GROUND_STEP];
}

u32 mario_update_moving_sand(struct MarioState *m) {
    if (!omm_mario_has_metal_cap(m) && m->floor) {
        s32 floorType = m->floor->type;
        if (floorType == SURFACE_MOVING_QUICKSAND ||
            floorType == SURFACE_DEEP_MOVING_QUICKSAND ||
            floorType == SURFACE_SHALLOW_MOVING_QUICKSAND ||
            floorType == SURFACE_INSTANT_MOVING_QUICKSAND) {
            s16 pushAngle = (m->floor->force << 8) & 0xFFFF;
            f32 pushSpeed = clamp_f(12.f - 4.f * (m->floor->force >> 8), 0.f, 12.f);
            m->vel[0] += pushSpeed * sins(pushAngle);
            m->vel[2] += pushSpeed * coss(pushAngle);
            return 1;
        }
    }
    return 0;
}

u32 mario_update_quicksand(struct MarioState *m, f32 sinkingSpeed) {
    if (!omm_mario_has_metal_cap(m) && !walk_on_quicksand(m) && !(m->action & ACT_FLAG_RIDING_SHELL)) {
        switch (m->floor->type) {
            case SURFACE_SHALLOW_QUICKSAND: {
                m->quicksandDepth = clamp_f(m->quicksandDepth + sinkingSpeed, 1.1f, 10.f);
            } return 0;

            case SURFACE_SHALLOW_MOVING_QUICKSAND: {
                m->quicksandDepth = clamp_f(m->quicksandDepth + sinkingSpeed, 1.1f, 25.f);
            } return 0;

            case SURFACE_QUICKSAND:
            case SURFACE_MOVING_QUICKSAND: {
                m->quicksandDepth = clamp_f(m->quicksandDepth + sinkingSpeed, 1.1f, 60.f);
            } return 0;

            case SURFACE_DEEP_QUICKSAND:
            case SURFACE_DEEP_MOVING_QUICKSAND: {
                m->quicksandDepth = clamp_f(m->quicksandDepth + sinkingSpeed, 1.1f, 160.f);
                if (m->quicksandDepth == 160.f) {
                    update_mario_sound_and_camera(m);
                    return drop_and_set_mario_action(m, ACT_QUICKSAND_DEATH, 0);
                }
            } return 0;

            case SURFACE_INSTANT_QUICKSAND:
            case SURFACE_INSTANT_MOVING_QUICKSAND: {
                if (OMM_LIKELY(OMM_MOVESET_ODYSSEY)) {
                    m->quicksandDepth = clamp_f(m->quicksandDepth + 2.f * sinkingSpeed, 1.1f, 160.f);
                    if (m->quicksandDepth == 160.f) {
                        update_mario_sound_and_camera(m);
                        return drop_and_set_mario_action(m, ACT_QUICKSAND_DEATH, 0);
                    }
                    if (!OMM_CHEAT_GOD_MODE && !m->invincTimer) {
                        if ((m->health - OMM_HEALTH_ODYSSEY_PER_SEGMENT) <= OMM_HEALTH_ODYSSEY_DEAD) {
                            update_mario_sound_and_camera(m);
                            return drop_and_set_mario_action(m, ACT_QUICKSAND_DEATH, 0);
                        } else {
                            set_camera_shake_from_hit(SHAKE_FALL_DAMAGE);
                            m->hurtCounter = 1;
                            m->invincTimer = 60;
                        }
                    }
                } else {
                    update_mario_sound_and_camera(m);
                    return drop_and_set_mario_action(m, ACT_QUICKSAND_DEATH, 0);
                }
            } return 0;
        }
    }
    m->quicksandDepth = 0.f;
    return 0;
}

u32 mario_update_windy_ground(struct MarioState *m) {
    if (!omm_mario_has_metal_cap(m) && m->floor) {
        if (m->floor->type == SURFACE_HORIZONTAL_WIND) {
            f32 pushSpeed = 3.2f + (gGlobalTimer % 4);
            s16 pushAngle = (m->floor->force << 8) & 0xFFFF;
            if (m->action & ACT_FLAG_MOVING) {
                s16 pushDYaw = m->faceAngle[1] - pushAngle;
                pushSpeed = -abs_f(m->forwardVel) * coss(pushDYaw) / 2.f;
                if (pushDYaw > -0x4000 && pushDYaw < 0x4000) {
                    pushSpeed *= -1.f;
                }
            }
            m->vel[0] += pushSpeed * sins(pushAngle);
            m->vel[2] += pushSpeed * coss(pushAngle);
            return 1;
        }
    }
    return 0;
}

u32 mario_push_off_steep_floor(struct MarioState *m, u32 action, u32 actionArg) {
    s16 floorDYaw = m->floorAngle - m->faceAngle[1];
    if (floorDYaw > -0x4000 && floorDYaw < 0x4000) {
        m->forwardVel = 16.f;
        m->faceAngle[1] = m->floorAngle;
    } else {
        m->forwardVel = -16.f;
        m->faceAngle[1] = m->floorAngle + 0x8000;
    }
    return omm_mario_set_action(m, action, actionArg, 0);
}

//
// Air Step
//

static bool omm_mario_check_ledge_grab(struct MarioState *m, struct Surface *wall, Vec3f intendedPos, Vec3f nextPos) {

    // Cannot ledge grab if moving upwards
    if (m->vel[1] > 0) {
        return false;
    }

    // Only ledge grab if the wall displaced Mario
    // in the opposite direction of his velocity.
    if ((nextPos[0] - intendedPos[0]) * m->vel[0] + (nextPos[2] - intendedPos[2]) * m->vel[2] > 0.f) {
        return false;
    }

    // Starts the search for floors above Mario's pos
    struct Surface *ledgeFloor = NULL;
    Vec3f ledgePos;
    ledgePos[0] = nextPos[0] - wall->normal.x * 60.f;
    ledgePos[2] = nextPos[2] - wall->normal.z * 60.f;
    ledgePos[1] = find_floor(ledgePos[0], nextPos[1] + (OMM_STEP_FIX_LEDGE_GRABS ? 80 : 160), ledgePos[2], &ledgeFloor);
    if (!ledgeFloor || ledgePos[1] - nextPos[1] <= 80.f) {
        return false;
    }

    // Make sure to not climb the ledge if it results in a soft bonk the next frame
    if (OMM_STEP_FIX_LEDGE_GRABS) {

        // Floor is too steep (> 25 deg)
        if (ledgeFloor->normal.y < coss(0x11C7)) {
            return false;
        }

        // Z pressed or bounced
        if (m->input & (INPUT_Z_PRESSED | INPUT_BOUNCE)) {
            return false;
        }

        // Stick held in the opposite direction
        if ((m->input & INPUT_NONZERO_ANALOG) && abs_s((s16) (m->intendedYaw - m->faceAngle[1])) > 0x4000) {
            return false;
        }

        // Wall above the ledge
        struct WallCollisionData col = { ledgePos[0], ledgePos[1], ledgePos[2], 60.f, 60.f };
        if (find_wall_collisions(&col)) {
            return false;
        }
    }

    // Ledge-grab
    vec3f_copy(m->pos, ledgePos);
    m->floor = ledgeFloor;
    m->floorHeight = ledgePos[1];
    m->floorAngle = atan2s(ledgeFloor->normal.z, ledgeFloor->normal.x);
    m->faceAngle[0] = 0;
    m->faceAngle[1] = atan2s(wall->normal.z, wall->normal.x) + 0x8000;
    return true;
}

static s32 omm_mario_perform_air_sub_step(struct MarioState *m, Vec3f intendedPos, f32 expectedY, u32 stepArg, struct Surface **wall) {
    Vec3f nextPos;
    vec3f_copy(nextPos, intendedPos);
    *wall = NULL;

    // Walls
    struct Surface *upperWall = resolve_and_return_wall_collisions(nextPos, 150.f, 50.f);
    struct Surface *lowerWall = resolve_and_return_wall_collisions(nextPos, 30.f, 50.f);

    // Floor
    struct Surface *floor;
    f32 floorHeight = find_floor(nextPos[0], nextPos[1], nextPos[2], &floor);

    // Ceiling
    struct Surface *ceil;
    f32 ceilLower = find_ceil(nextPos[0], floorHeight + 80.f, nextPos[2], &ceil) - 160.f;
    f32 ceilUpper = (OMM_STEP_FIX_EXPOSED_CEILINGS ? (ceilLower + 240.f) : +20000.f);

    // Out of bounds
    if (!floor) {
        if (nextPos[1] <= m->floorHeight) {
            m->pos[1] = m->floorHeight;
            return STEP_HIT_FLOOR;
        } else {
            m->pos[1] = nextPos[1];
            return (OMM_STEP_FIX_OUT_OF_BOUNDS_BONK ? STEP_NONE : STEP_OUT_OF_BOUNDS);
        }
    }

    // Shell ride
    f32 waterLevel = find_water_level(nextPos[0], nextPos[2]);
    if ((m->action & ACT_FLAG_RIDING_SHELL) && floorHeight < waterLevel) {
        floorHeight = waterLevel;
        floor = &gWaterSurfacePseudoFloor;
        floor->originOffset = -waterLevel;
    }

    // Landed
    // The second condition is here to prevent Mario from
    // being stuck on a rising platform while trying to jump
    if (nextPos[1] <= floorHeight && !(OMM_STEP_FIX_MOVING_PLATFORM_JUMPS && m->vel[1] > 0.f && expectedY > floorHeight)) {
        if (floorHeight < ceilLower || floorHeight >= ceilUpper) {
            m->pos[0] = nextPos[0];
            m->pos[2] = nextPos[2];
            m->floor = floor;
            m->floorHeight = floorHeight;
        }
        m->pos[1] = floorHeight;
        return STEP_HIT_FLOOR;
    }

    // Hit ceiling
    if (nextPos[1] >= ceilLower && nextPos[1] < ceilUpper) {

        // Hit ceiling from below
        if (m->vel[1] >= 0.f) {
            m->vel[1] = 0.f;

            // Ceiling hang
            if ((stepArg & AIR_STEP_CHECK_HANG) && ceil && ceil->type == SURFACE_HANGABLE) {
                return STEP_GRABBED_CEIL;
            }
            return STEP_NONE;
        }

        // Landed?
        if (nextPos[1] <= m->floorHeight) {
            m->pos[1] = m->floorHeight;
            return STEP_HIT_FLOOR;
        }

        // Hit ceiling from a side
        m->pos[1] = nextPos[1];
        return (OMM_STEP_FIX_CEILINGS_BONK ? STEP_NONE : STEP_HIT_CEIL);
    }

    // Update Mario's pos
    vec3f_copy(m->pos, nextPos);
    m->floor = floor;
    m->floorHeight = floorHeight;

    // Hit lower wall, try to ledge grab
    if ((stepArg & AIR_STEP_CHECK_LEDGE_GRAB) && !upperWall && lowerWall) {

        // Ledge grab
        if (omm_mario_check_ledge_grab(m, lowerWall, intendedPos, nextPos)) {
            return STEP_GRABBED_LEDGE;
        }
        return STEP_NONE;
    }

    // Hit any wall
    struct Surface *refWall = (upperWall ? upperWall : lowerWall);
    if (refWall) {

        // Lava wall
        if (refWall->type == SURFACE_BURNING) {
            *wall = refWall;
            return STEP_HIT_LAVA_WALL;
        }

        // Solid wall
        s16 wallDYaw = atan2s(refWall->normal.z, refWall->normal.x) - m->faceAngle[1];
        if (wallDYaw < -0x6000 || wallDYaw > 0x6000) {
            *wall = refWall;
            m->flags |= MARIO_UNKNOWN_30;
            return STEP_HIT_WALL;
        }
    }
    return STEP_NONE;
}

static void omm_mario_apply_gravity(struct MarioState *m) {
    f32 decV = 4.f;
    f32 maxV = 75.f;

    // Flying
    if (m->action == ACT_FLYING) {
        return;
    }

    // Peach Joy move
    else if (m->action == ACT_OMM_PEACH_VIBE_JOY_MOVE) {
        decV = 2.f;
        maxV = 30.f;
    }

    // Peach Joy fly
    else if (m->action == ACT_OMM_PEACH_VIBE_JOY_FLY) {
        decV = 0.f;
        maxV = 30.f;
    }

    // Peach Joy attack
    else if (m->action == ACT_OMM_PEACH_VIBE_JOY_ATTACK) {
        decV = 2.f;
        maxV = 30.f;
    }

    // Peach glide
    else if (m->action == ACT_OMM_PEACH_GLIDE) {
        decV = 1.f;
        maxV = 15.f;
    }

    // Peach air attack
    else if (m->action == ACT_OMM_PEACH_ATTACK_AIR) {
        static const f32 sActOmmPeachAttackAirDecV[] = { 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4 };
        decV = sActOmmPeachAttackAirDecV[min_s(m->actionTimer, OMM_ARRAY_SIZE(sActOmmPeachAttackAirDecV) - 1)];
    }

    // Wall-slide
    else if (m->action == ACT_OMM_WALL_SLIDE) {
        decV = 2.f;
        maxV = 32.f;
    }

    // Metal-water Wall-slide
    else if (m->action == ACT_OMM_METAL_WATER_WALL_SLIDE) {
        decV = 1.f;
        maxV = 12.f;
    }

    // Spin jump, Spin air
    else if (m->action == ACT_OMM_SPIN_JUMP || m->action == ACT_OMM_SPIN_AIR) {
        decV = 1.5f;
        maxV = 20.f;
    }

    // Midair spin
    else if (m->action == ACT_OMM_MIDAIR_SPIN) {
        decV = clamp_f((gOmmData->mario->midairSpin.counter - 1) * 0.8f, 0.01f, 4.f);
        maxV = 60.f;
    }

    // Cappy throw airborne
    else if (m->action == ACT_OMM_CAPPY_THROW_AIRBORNE) {
        decV = 2.f;
    }

    // Cappy throw airborne (Metal water)
    else if (m->action == ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE) {
        decV = 0.6f;
        maxV = 16.f;
    }

    // Twirl
    else if (m->action == ACT_TWIRLING && m->vel[1] < 0.f) {
        f32 heaviness = 1024.f / max_f(1024.f, m->angleVel[1]);
        decV = 4.f * heaviness;
        maxV = 75.f * heaviness;
    }

    // Cannon
    else if (m->action == ACT_SHOT_FROM_CANNON) {
        decV = 1.f;
    }

    // Slow jump
    else if (m->action == ACT_LONG_JUMP || m->action == ACT_SLIDE_KICK || m->action == ACT_BBH_ENTER_SPIN) {
        decV = 2.f;
    }

    // Lava boost
    else if (m->action == ACT_LAVA_BOOST) {
        decV = 3.2f;
        maxV = 65.f;
    }

    // Getting blown
    else if (m->action == ACT_GETTING_BLOWN) {
        decV = m->unkC4;
    }

    // Control jump height
    else if ((m->flags & MARIO_UNKNOWN_08) && !(m->input & INPUT_A_DOWN) && (m->vel[1] > 20.f) && (m->action & ACT_FLAG_CONTROL_JUMP_HEIGHT)) {
        decV = m->vel[1] * 0.75f;
    }

    // Underwater Metal Mario
    else if (m->action & ACT_FLAG_METAL_WATER) {
        decV = 1.6f;
        maxV = 16.f;
        if (m->action == ACT_OMM_METAL_WATER_GROUND_POUND) maxV = 32.f;
        if (m->action == ACT_OMM_METAL_WATER_SPIN_POUND)   maxV = 40.f;
    }

    // Flutter jump
    else if ((m->flags & MARIO_WING_CAP) && (m->vel[1] < 0.f) && (m->input & INPUT_A_DOWN)) {
        m->marioBodyState->wingFlutter = TRUE;
        decV = 2.f;
        maxV = 40.f;
    }

    // Improved Vanish cap
    if (omm_mario_has_vanish_cap(m)) {
        decV *= 0.7f;
        maxV *= 0.7f;
    }

    // Apply gravity
    m->vel[1] = max_f(-maxV, m->vel[1] - decV);
}

static void omm_mario_apply_vertical_wind(struct MarioState *m) {
    if (m->floor->type == SURFACE_VERTICAL_WIND && !omm_mario_is_ground_pounding(m)) {
        f32 offsetY = m->pos[1] - -1500.f;
        if (-3000.f < offsetY && offsetY < 2000.f) {
            f32 maxVelY = 10000.f / (max_f(0.f, offsetY) + 200.f);
            m->vel[1] = min_f(m->vel[1] + (maxVelY / 8.f), maxVelY);
        }
    }
}

s32 perform_air_step(struct MarioState *m, u32 stepArg) {
    if (OMM_MOVESET_ODYSSEY && m->action == ACT_WALL_KICK_AIR) {
        stepArg |= AIR_STEP_CHECK_HANG;
    }

    // Init steps
    s32 stepResult = STEP_NONE;
    f32 speedMul = omm_player_get_selected_air_speed_multiplier();
    s32 numSteps = OMM_STEP_NUM_SUB_STEPS;
    s32 hStepMul = speed_modifier(m);
    s32 yStepMul = (m->vel[1] > 0.f ? jump_modifier(m) : 1);
    s32 subSteps = numSteps * hStepMul * yStepMul;

    // Perform steps
    m->wall = NULL;
    struct Surface *wall = NULL;
    f32 expectedY = (m->pos[1] + m->vel[1]);
    for (s32 i = 0; i != subSteps; ++i) {
        Vec3f intendedPos;
        intendedPos[0] = m->pos[0] + (m->vel[0] * speedMul) / (numSteps * yStepMul);
        intendedPos[1] = m->pos[1] + (m->vel[1]           ) / (numSteps * hStepMul);
        intendedPos[2] = m->pos[2] + (m->vel[2] * speedMul) / (numSteps * yStepMul);
        s32 subStepResult = omm_mario_perform_air_sub_step(m, intendedPos, expectedY, stepArg, &wall);
        if (wall) {
            m->wall = wall;
        }
        if (subStepResult != STEP_NONE) {
            stepResult = subStepResult;
        }
        if (subStepResult == STEP_HIT_FLOOR ||
            subStepResult == STEP_HIT_LAVA_WALL ||
            subStepResult == STEP_GRABBED_LEDGE ||
            subStepResult == STEP_GRABBED_CEIL) {
            break;
        }
    }

    // Don't bonk into walls if ground pounding
    if (stepResult == STEP_HIT_WALL && OMM_MOVESET_ODYSSEY && omm_mario_is_ground_pounding(m)) {
        stepResult = STEP_NONE;
    }

    // Turn lava walls into regular walls with "Walk on Lava" cheat
    if (OMM_CHEAT_WALK_ON_LAVA && stepResult == STEP_HIT_LAVA_WALL) {
        stepResult = STEP_HIT_WALL;
    }

    // Vanish cap extra step
    Vec3f target;
    if (omm_mario_check_vanish_cap_step(m, target, stepResult, 30.f, 50.f)) {
        stepResult = omm_mario_perform_air_sub_step(m, target, expectedY, stepArg, &m->wall);
        if (stepResult == STEP_HIT_WALL) {
            stepResult = STEP_NONE;
        }
    }

    // Update gfx and return step result
    clamp_mario_pos(m);
    if (m->vel[1] >= 0.f) gOmmData->mario->state.peakHeight = m->pos[1];
    m->terrainSoundAddend = mario_get_terrain_sound_addend(m);
    omm_mario_apply_gravity(m);
    omm_mario_apply_vertical_wind(m);
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
    return sStepResult[stepResult][AIR_STEP];
}

void mario_bonk_reflection(struct MarioState *m, u32 negateSpeed) {
    if (m->wall) {
        s16 wallAngle = atan2s(m->wall->normal.z, m->wall->normal.x);
        m->faceAngle[1] = wallAngle - (s16) (m->faceAngle[1] - wallAngle);
        play_sound((m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_BONK : SOUND_ACTION_BONK, m->marioObj->oCameraToObject);
    } else {
        play_sound(SOUND_ACTION_HIT, m->marioObj->oCameraToObject);
    }

    if (negateSpeed) {
        mario_set_forward_vel(m, -m->forwardVel);
    } else {
        m->faceAngle[1] += 0x8000;
    }
}

//
// Hang step
//

static s32 omm_mario_perform_hang_sub_step(struct MarioState *m, Vec3f nextPos) {
    
    // Walls
    struct Surface *wall = resolve_and_return_wall_collisions(nextPos, 50.f, 50.f);
    m->wall = wall;

    // Floor
    struct Surface *floor = NULL;
    f32 floorHeight = find_floor(nextPos[0], nextPos[1], nextPos[2], &floor);

    // Ceiling
    struct Surface *ceil = NULL;
    f32 ceilLower = find_ceil(nextPos[0], floorHeight + 80.f, nextPos[2], &ceil) - 160.f;
    f32 ceilUpper = (OMM_STEP_FIX_EXPOSED_CEILINGS ? (ceilLower + 240.f) : +20000.f);
    
    // Out of bounds
    if (!floor) {
        return (OMM_STEP_FIX_OUT_OF_BOUNDS_BONK ? STEP_NONE : STEP_OUT_OF_BOUNDS);
    }

    // Left hangable ceil
    if (!ceil || ceil->type != SURFACE_HANGABLE) {
        return STEP_LEFT_CEIL;
    }
    
    // Squish
    if (floorHeight >= ceilLower && floorHeight < ceilUpper) {
        return STEP_SQUISH;
    }

    // Not the same ceiling
    f32 ceilOffset = ceilLower - nextPos[1];
    if (ceilOffset < -30.f) {
        return STEP_HIT_WALL;
    }
    if (ceilOffset > 30.f) {
        return STEP_LEFT_CEIL;
    }

    // Still on ceiling
    nextPos[1] = ceilLower;
    vec3f_copy(m->pos, nextPos);
    m->floor = floor;
    m->ceil = ceil;
    m->floorHeight = floorHeight;
    m->ceilHeight = ceilLower + 160.f;
    return STEP_NONE;
}

s32 perform_hang_step(struct MarioState *m) {

    // Init steps
    s32 stepResult = STEP_NONE;
    f32 speedMul = omm_player_get_selected_air_speed_multiplier();
    s32 numSteps = OMM_STEP_NUM_SUB_STEPS;
    s32 subSteps = numSteps * speed_modifier(m);

    // Perform steps
    for (s32 i = 0; i != subSteps; ++i) {
        Vec3f intendedPos;
        intendedPos[0] = m->pos[0] - (m->vel[0] * m->ceil->normal.y * speedMul) / numSteps;
        intendedPos[1] = m->pos[1];
        intendedPos[2] = m->pos[2] - (m->vel[2] * m->ceil->normal.y * speedMul) / numSteps;
        stepResult     = omm_mario_perform_hang_sub_step(m, intendedPos);
        if (stepResult == STEP_HIT_WALL ||
            stepResult == STEP_LEFT_CEIL) {
            break;
        }
    }

    // Vanish cap extra step
    Vec3f target;
    if (omm_mario_check_vanish_cap_step(m, target, stepResult, 50.f, 50.f)) {
        stepResult = omm_mario_perform_hang_sub_step(m, target);
        if (stepResult == STEP_HIT_WALL) {
            stepResult = STEP_NONE;
        }
    }
    
    // Update gfx and return step result
    m->vel[1] = 0.f;
    clamp_mario_pos(m);
    m->slideYaw = m->faceAngle[1];
    gOmmData->mario->state.peakHeight = m->pos[1];
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
    return sStepResult[stepResult][HANG_STEP];
}

//
// Water Step
//

f32 get_buoyancy(struct MarioState *m) {

    // Metal Mario
    if (m->flags & MARIO_METAL_CAP) {
        return ((m->action & ACT_FLAG_INVULNERABLE) ? -2.f : -18.f);
    }
    
    // Near surface
    if (m->waterLevel - m->pos[1] < 480.f) {
        return 1.25f;
    }

    // Not moving
    if (!(m->action & ACT_FLAG_MOVING)) {
        return -2.f;
    }

    // No buoyancy
    return 0.f;
}

static s32 omm_mario_perform_water_sub_step(struct MarioState *m, Vec3f nextPos) {

    // Walls
    struct Surface *wall = resolve_and_return_wall_collisions(nextPos, 10.f, 110.f);

    // Floor
    struct Surface *floor = NULL;
    f32 floorHeight = find_floor(nextPos[0], nextPos[1], nextPos[2], &floor);

    // Ceiling
    struct Surface *ceil = NULL;
    f32 ceilLower = find_ceil(nextPos[0], floorHeight + 80.f, nextPos[2], &ceil) - 160.f;
    f32 ceilUpper = (OMM_STEP_FIX_EXPOSED_CEILINGS ? (ceilLower + 240.f) : +20000.f);

    // Out of bounds
    if (!floor) {
        return STEP_OUT_OF_BOUNDS;
    }
    m->wall = wall;

    // Above floor
    if (nextPos[1] >= floorHeight) {

        // Below ceil
        if (nextPos[1] < ceilLower || nextPos[1] >= ceilUpper) {
            vec3f_copy(m->pos, nextPos);
            m->floor = floor;
            m->floorHeight = floorHeight;
            if (wall) {
                return STEP_HIT_WALL;
            }
            return STEP_NONE;
        }

        // Hit ceiling
        m->floor = floor;
        m->floorHeight = floorHeight;
        if (OMM_STEP_FIX_WATER_WARPS) {
            f32 intendedDist = sqrt(sqr_f(nextPos[0] - m->pos[0]) + sqr_f(nextPos[1] - m->pos[1]) + sqr_f(nextPos[2] - m->pos[2]));
            f32 displaceDist = sqrt(sqr_f(nextPos[0] - m->pos[0]) + sqr_f(ceilLower  - m->pos[1]) + sqr_f(nextPos[2] - m->pos[2]));
            if (displaceDist > 0) {
                m->pos[0] += (nextPos[0] - m->pos[0]) * min_f(1.f, intendedDist / displaceDist);
                m->pos[1] += (ceilLower  - m->pos[1]) * min_f(1.f, intendedDist / displaceDist);
                m->pos[2] += (nextPos[2] - m->pos[2]) * min_f(1.f, intendedDist / displaceDist);
            }
        } else {
            vec3f_set(m->pos, nextPos[0], ceilLower, nextPos[2]);
        }
        return (OMM_STEP_FIX_CEILINGS_BONK ? STEP_NONE : STEP_HIT_CEIL);
    }

    // Can't fit
    if (floorHeight >= ceilLower && floorHeight < ceilUpper) {
        return STEP_SQUISH;
    }

    // Hit floor
    m->floor = floor;
    m->floorHeight = floorHeight;
    if (OMM_STEP_FIX_WATER_WARPS) {
        f32 intendedDist = sqrt(sqr_f(nextPos[0] - m->pos[0]) + sqr_f(nextPos[1]  - m->pos[1]) + sqr_f(nextPos[2] - m->pos[2]));
        f32 displaceDist = sqrt(sqr_f(nextPos[0] - m->pos[0]) + sqr_f(floorHeight - m->pos[1]) + sqr_f(nextPos[2] - m->pos[2]));
        if (displaceDist > 0) {
            m->pos[0] += (nextPos[0]  - m->pos[0]) * min_f(1.f, intendedDist / displaceDist);
            m->pos[1] += (floorHeight - m->pos[1]) * min_f(1.f, intendedDist / displaceDist);
            m->pos[2] += (nextPos[2]  - m->pos[2]) * min_f(1.f, intendedDist / displaceDist);
        }
    } else {
        vec3f_set(m->pos, nextPos[0], floorHeight, nextPos[2]);
    }
    return STEP_HIT_FLOOR;
}

static void omm_mario_apply_water_current(struct MarioState *m, Vec3f waterVel) {

    // Flowing water
    static const s16 sWaterCurrentSpeeds[] = { 28, 12, 8, 4 };
    if (m->floor->type == SURFACE_FLOWING_WATER) {
        s16 currentAngle = m->floor->force << 8;
        f32 currentSpeed = sWaterCurrentSpeeds[min_s(3, (u8) (m->floor->force >> 8))];
        waterVel[0] += currentSpeed * sins(currentAngle);
        waterVel[2] += currentSpeed * coss(currentAngle);
    }

    // Whirlpools
    for (s32 i = 0; i < 2; i++) {
        struct Whirlpool *whirlpool = gCurrentArea->whirlpools[i];
        if (whirlpool) {
            f32 dx = whirlpool->pos[0] - m->pos[0];
            f32 dy = whirlpool->pos[1] - m->pos[1];
            f32 dz = whirlpool->pos[2] - m->pos[2];
            f32 lateralDist = sqrtf(dx * dx + dz * dz);
            f32 distance = sqrtf(lateralDist * lateralDist + dy * dy);
            s16 pitchToWhirlpool = atan2s(lateralDist, dy);
            s16 yawToWhirlpool = atan2s(dz, dx) - (s16) (0x2000 * 1000.f / (distance + 1000.f));
            f32 whirlpoolStrength = whirlpool->strength * (OMM_STEP_FIX_WHIRLPOOLS * whirlpool->strength > 0.f ? 0.5f : 1.f) * (1.f - (min_f(distance, 2000.f) / 2000.f));
            waterVel[0] += whirlpoolStrength * coss(pitchToWhirlpool) * sins(yawToWhirlpool);
            waterVel[1] += whirlpoolStrength * sins(pitchToWhirlpool);
            waterVel[2] += whirlpoolStrength * coss(pitchToWhirlpool) * coss(yawToWhirlpool);
        }
    }
}

s32 perform_water_step(struct MarioState *m) {
    Vec3f waterVel;
    vec3f_copy(waterVel, m->vel);
    if (m->action & ACT_FLAG_SWIMMING) {
        omm_mario_apply_water_current(m, waterVel);
    }

    // Init steps
    s32 stepResult = STEP_NONE;
    s32 numSteps = OMM_STEP_NUM_SUB_STEPS_WATER;
    s32 subSteps = numSteps * swim_modifier(m);

    // Perform steps
    for (s32 i = 0; i != subSteps; ++i) {
        Vec3f intendedPos;
        intendedPos[0] = m->pos[0] + (waterVel[0] / numSteps);
        intendedPos[1] = m->pos[1] + (waterVel[1] / numSteps);
        intendedPos[2] = m->pos[2] + (waterVel[2] / numSteps);
        if (intendedPos[1] >= m->waterLevel - 80) {
            intendedPos[1] = m->waterLevel - 80;
            m->vel[1] = 0.f;
        }
        stepResult = omm_mario_perform_water_sub_step(m, intendedPos);
    }

    // Vanish cap extra step
    Vec3f target;
    if (omm_mario_check_vanish_cap_step(m, target, stepResult, 10.f, 110.f)) {
        stepResult = omm_mario_perform_hang_sub_step(m, target);
        if (stepResult == STEP_HIT_WALL) {
            stepResult = STEP_NONE;
        }
    }

    // Update gfx and return step result
    clamp_mario_pos(m);
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, -m->faceAngle[0], m->faceAngle[1], m->faceAngle[2]);
    return sStepResult[stepResult][WATER_STEP];
}

//
// Object step
//

static s32 perform_object_sub_step(struct Object *o, bool onGround, bool stickyFeet) {
    f32 radius = o->oWallHitboxRadius;
    f32 height = o->hitboxHeight;
    f32 offset = o->hitboxDownOffset;

    // Wall collisions
    s32 nSteps = max_s(1, height / 50.f);
    f32 yDelta = height / nSteps;
    for (s32 i = 0; i != nSteps; ++i) {
        struct Surface *wall = resolve_and_return_wall_collisions(&o->oPosX, 30.f + i * yDelta - offset, radius);
        if (wall) {
            o->oWall = wall;
        }
    }
    
    // Floor collision
    struct Surface *floor = NULL;
    f32 floorHeight = find_floor(o->oPosX, o->oPosY - offset, o->oPosZ, &floor) + offset;
    if (floor) {
        o->oFloor = floor;
        o->oFloorHeight = floorHeight;
    } else {
        o->oVelX = 0;
        o->oVelZ = 0;
        return STEP_OUT_OF_BOUNDS;
    }

    // Drop object to floor if low enough
    // If the floor is too steep (> ~50 deg), it acts as a wall and push the object out of it
    if (o->oPosY < floorHeight + (onGround ? 40.f : 1.f)) {
        o->oPosY = floorHeight;
        if (!stickyFeet &&
            floor->normal.y < 0.6f &&
            floor->type != SURFACE_SWITCH &&
            floor->type != SURFACE_NOT_SLIPPERY &&
            floor->type != SURFACE_HARD_NOT_SLIPPERY)
        {
            o->oPosX += floor->normal.x * o->oWallHitboxRadius;
            o->oPosZ += floor->normal.z * o->oWallHitboxRadius;
            o->oFloorHeight = (floorHeight -= (1.f - floor->normal.y) * o->oWallHitboxRadius);
        } else {
            o->oVelY = max_f(o->oVelY, 0.f);
        }
    }
    
    // Ceiling collision
    // Treat floor + ceiling collision as OOB to cancel out step
    struct Surface *ceil = NULL;
    f32 ceilHeight = find_ceil(o->oPosX, o->oPosY, o->oPosZ, &ceil);
    f32 diffHeight = height - offset;
    if (ceil && o->oPosY < ceilHeight && ceilHeight < o->oPosY + diffHeight) {
        if (ceilHeight - floorHeight > diffHeight) {
            o->oPosY = ceilHeight - diffHeight;
            o->oVelY = min_f(o->oVelY, 0.f);
            o->oCeil = ceil;
        } else {
            return STEP_OUT_OF_BOUNDS;
        }
    }
    return STEP_NONE;
}

s32 perform_object_step(struct Object *o, u32 flags) {
    bool isCapture = (o == gOmmCapture);
    bool onGround = (flags & OBJ_STEP_CHECK_ON_GROUND) && obj_is_on_ground(o);
    o->oFloor = NULL;
    o->oCeil = NULL;
    o->oWall = NULL;

    // Init steps
    s32 stepResult = STEP_NONE;
    f32 speedMul = (isCapture ? (onGround ? POBJ_GROUND_SPEED_MULTIPLIER : POBJ_AIR_SPEED_MULTIPLIER) : 1.f);
    s32 numSteps = (isCapture ? OMM_STEP_NUM_SUB_STEPS_CAPTURE : OMM_STEP_NUM_SUB_STEPS_OBJECT);
    s32 hStepMul = (isCapture ? speed_modifier(gMarioState) : 1);
    s32 yStepMul = (isCapture && o->oVelY > 0.f ? jump_modifier(gMarioState) : 1);
    s32 subSteps = numSteps * hStepMul * yStepMul;
    
    // Perform steps
    for (s32 i = 0; i != subSteps; ++i) {
        Vec3f pos; vec3f_copy(pos, &o->oPosX);
        o->oPosX += (o->oVelX * speedMul) / (numSteps * yStepMul);
        o->oPosY += (o->oVelY           ) / (numSteps * hStepMul);
        o->oPosZ += (o->oVelZ * speedMul) / (numSteps * yStepMul);
        stepResult = perform_object_sub_step(o, onGround, flags & OBJ_STEP_STICKY_FEET);
        if (stepResult == STEP_OUT_OF_BOUNDS) {
            vec3f_copy(&o->oPosX, pos);
        } else if (flags & OBJ_STEP_CHECK_ON_GROUND) {
            onGround = (obj_is_on_ground(o) && o->oVelY <= 0.f);
        }
    }
    o->oForwardVel = sqrtf(sqr_f(o->oVelX) + sqr_f(o->oVelZ));

    // Squish test
    if (isCapture) {
        f32 floorHeight = find_floor(o->oPosX, o->oPosY - o->hitboxDownOffset, o->oPosZ, NULL) + o->hitboxDownOffset;
        f32 ceilHeight = find_ceil(o->oPosX, o->oPosY, o->oPosZ, NULL);
        f32 diffHeight = o->hitboxHeight - o->hitboxDownOffset;
        if (floorHeight < ceilHeight && ceilHeight < floorHeight + diffHeight &&
            o->oPosY    < ceilHeight && ceilHeight < o->oPosY    + diffHeight) {
            gOmmData->object->state.squishTimer++;
        } else {
            gOmmData->object->state.squishTimer = 0;
        }
    }

    // Vanish cap extra step
    Vec3f pos, target;
    if ((flags & OBJ_STEP_MOVE_THROUGH_WALLS) && omm_object_check_move_through_walls(o, target)) {
        vec3f_copy(pos, &o->oPosX);
        vec3f_copy(&o->oPosX, target);
        stepResult = perform_object_sub_step(o, onGround, flags & OBJ_STEP_STICKY_FEET);
        if (stepResult == STEP_OUT_OF_BOUNDS) {
            vec3f_copy(&o->oPosX, pos);
        } else if (flags & OBJ_STEP_CHECK_ON_GROUND) {
            onGround = (obj_is_on_ground(o) && o->oVelY <= 0.f);
        }
    }

    // Update home and forward vel
    if (flags & OBJ_STEP_UPDATE_HOME) {
        vec3f_copy(&o->oHomeX, &o->oPosX);
    }
    return 0;
}

//
// Bully collision
//

void transfer_bully_speed(struct BullyCollisionData *obj1, struct BullyCollisionData *obj2) {
    f32 rx = obj2->posX - obj1->posX;
    f32 rz = obj2->posZ - obj1->posZ;
    f32 v1 = (+rx * obj1->velX + rz * obj1->velZ) / (rx * rx + rz * rz);
    f32 v2 = (-rx * obj2->velX - rz * obj2->velZ) / (rx * rx + rz * rz);
    obj2->velX += (obj2->conversionRatio * v1 * +rx) - (v2 * -rx);
    obj2->velZ += (obj2->conversionRatio * v1 * +rz) - (v2 * -rz);
    obj1->velX += (obj1->conversionRatio * v2 * -rx) - (v1 * +rx);
    obj1->velZ += (obj1->conversionRatio * v2 * -rz) - (v1 * +rz);
}

BAD_RETURN(s32) init_bully_collision_data(struct BullyCollisionData *data, f32 posX, f32 posZ, f32 forwardVel, s16 yaw, f32 conversionRatio, f32 radius) {
    yaw += 0x8000 * (forwardVel < 0.f);
    forwardVel = abs_f(forwardVel);
    data->radius = radius;
    data->conversionRatio = conversionRatio;
    data->posX = posX;
    data->posZ = posZ;
    data->velX = forwardVel * sins(yaw);
    data->velZ = forwardVel * coss(yaw);
}

//
// Unused functions
//

void stub_mario_step_1(UNUSED struct MarioState *x) {
}

void stub_mario_step_2(void) {
}

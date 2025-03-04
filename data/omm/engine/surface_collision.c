#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

u8 gInterpolatingSurfaces;

// Enhanced surface collision system
// Enabled by setting Moveset to Odyssey
// Adds several fixes and increases accuracy:
// - Treats PUs as out of bounds
// - Fixes unreferenced wall glitch, and returns the closest wall in the direction of Mario
// - Fixes collision from stacked or already referenced walls
// - Checks floor on a square instead of a point, to prevent objects to fall inside small gaps
// - Increases number of cells checked for more precise wall collisions

f32 get_surface_height_at_pos(f32 x, f32 z, struct Surface *surf) {
    return -(x * surf->normal.x + z * surf->normal.z + surf->originOffset) / surf->normal.y;
}

bool recompute_surface_parameters(struct Surface *surf) {
    f32 x1 = surf->vertex1[0];
    f32 y1 = surf->vertex1[1];
    f32 z1 = surf->vertex1[2];
    f32 x2 = surf->vertex2[0];
    f32 y2 = surf->vertex2[1];
    f32 z2 = surf->vertex2[2];
    f32 x3 = surf->vertex3[0];
    f32 y3 = surf->vertex3[1];
    f32 z3 = surf->vertex3[2];
    f32 nx = (y2 - y1) * (z3 - z2) - (z2 - z1) * (y3 - y2);
    f32 ny = (z2 - z1) * (x3 - x2) - (x2 - x1) * (z3 - z2);
    f32 nz = (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);
    f32 mag = sqrtf(sqr_f(nx) + sqr_f(ny) + sqr_f(nz));
    if (mag > 0) {
        surf->lowerY = min_3_s(y1, y2, y3) - 5;
        surf->upperY = max_3_s(y1, y2, y3) + 5;
        surf->normal.x = nx / mag;
        surf->normal.y = ny / mag;
        surf->normal.z = nz / mag;
        surf->originOffset = -(nx * x1 + ny * y1 + nz * z1) / mag;
        return true;
    }
    return false;
}

//
// Walls
//

static bool should_ignore_vanish_cap_walls() {
    if (gCurrentObject && gCurrentObject != gMarioObject) return (gCurrentObject->activeFlags & ACTIVE_FLAG_MOVE_THROUGH_GRATE) != 0; // Object
    if (!omm_mario_is_capture(gMarioState)) return (gMarioState->flags & MARIO_VANISH_CAP) != 0; // Mario
    return POBJ_IS_ABLE_TO_MOVE_THROUGH_WALLS; // Capture
}

static bool is_wall_already_referenced(const struct WallCollisionData *data, struct Surface *surf) {
    for (s32 i = 0; i != data->numWalls; ++i) {
        struct Surface *wall = data->walls[i];

        // The wall is already referenced
        if (wall == surf) {
            return true;
        }

        // A wall with the same normal is already referenced
        if (vec3f_dot((f32 *) &surf->normal, (f32 *) &wall->normal) > 0.94f /* ~20 degrees */) {
            return true;
        }
    }
    return false;
}

static void find_wall_collisions_from_list(OmmArray surfaces, s32 count, struct WallCollisionData *data) {
    for (s32 i = 0; i < count && data->numWalls < MAX_REFERENCED_WALLS; ++i) {
        struct Surface *surf = omm_array_get(surfaces, ptr, i);
        f32 x = data->x;
        f32 y = data->y + data->offsetY;
        f32 z = data->z;

        // Height check optimization
        if (y < surf->lowerY || y > surf->upperY) {
            continue;
        }

        // Offset check optimization
        f32 offset = surf->normal.x * x + surf->normal.y * y + surf->normal.z * z + surf->originOffset;
        if (offset < -data->radius || offset > data->radius) {
            continue;
        }

        // Camera collision
        if (gCheckingSurfaceCollisionsForCamera) {
            if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) {
                continue;
            }
        }

        // Object collision
        else if (
            (surf->type == SURFACE_CAMERA_BOUNDARY) ||
            (surf->type == SURFACE_VANISH_CAP_WALLS && should_ignore_vanish_cap_walls())) {
            continue;
        }

        // Project on X axis
        if (surf->flags & SURFACE_FLAG_X_PROJECTION) {
            f32 w1 = -surf->vertex1[2];
            f32 w2 = -surf->vertex2[2];
            f32 w3 = -surf->vertex3[2];
            f32 y1 = +surf->vertex1[1];
            f32 y2 = +surf->vertex2[1];
            f32 y3 = +surf->vertex3[1];
            if ((surf->normal.x > 0 && (
                (y1 - y) * (w2 - w1) - (w1 - -z) * (y2 - y1) > 0 ||
                (y2 - y) * (w3 - w2) - (w2 - -z) * (y3 - y2) > 0 ||
                (y3 - y) * (w1 - w3) - (w3 - -z) * (y1 - y3) > 0)) ||
                (surf->normal.x < 0 && (
                (y1 - y) * (w2 - w1) - (w1 - -z) * (y2 - y1) < 0 ||
                (y2 - y) * (w3 - w2) - (w2 - -z) * (y3 - y2) < 0 ||
                (y3 - y) * (w1 - w3) - (w3 - -z) * (y1 - y3) < 0))) {
                continue;
            }
        }

        // Project on Z axis
        else {
            f32 w1 = surf->vertex1[0];
            f32 w2 = surf->vertex2[0];
            f32 w3 = surf->vertex3[0];
            f32 y1 = surf->vertex1[1];
            f32 y2 = surf->vertex2[1];
            f32 y3 = surf->vertex3[1];
            if ((surf->normal.z > 0 && (
                (y1 - y) * (w2 - w1) - (w1 - x) * (y2 - y1) > 0 ||
                (y2 - y) * (w3 - w2) - (w2 - x) * (y3 - y2) > 0 ||
                (y3 - y) * (w1 - w3) - (w3 - x) * (y1 - y3) > 0)) ||
                (surf->normal.z < 0 && (
                (y1 - y) * (w2 - w1) - (w1 - x) * (y2 - y1) < 0 ||
                (y2 - y) * (w3 - w2) - (w2 - x) * (y3 - y2) < 0 ||
                (y3 - y) * (w1 - w3) - (w3 - x) * (y1 - y3) < 0))) {
                continue;
            }
        }

        // Apply collision
        if (!OMM_COLLISION_FIX_WALL_COLLISIONS || !is_wall_already_referenced(data, surf)) {
            data->x += surf->normal.x * (data->radius - offset);
            data->z += surf->normal.z * (data->radius - offset);
            data->walls[data->numWalls++] = surf;
        }
    }
}

s32 find_wall_collisions(struct WallCollisionData *data) {
    data->numWalls = 0;

#if OMM_GAME_IS_R96X
    if (cheats_no_bounds(gMarioState)) {
        return 0;
    }
#endif

    // CCM Racing penguin
    // His radius is bigger than 200, but not capping it at 200 breaks its path
#if !OMM_GAME_IS_SMSR
    if (gCurrentObject && gCurrentObject->behavior == bhvRacingPenguin) {
        data->radius = min_f(data->radius, MAX_COLLISION_RADIUS);
    }
#endif

    // Check PUs
    if (OMM_COLLISION_CHECK_PUS) {
        if (data->x < -LEVEL_BOUNDARY_MAX || data->x >= +LEVEL_BOUNDARY_MAX ||
            data->z < -LEVEL_BOUNDARY_MAX || data->z >= +LEVEL_BOUNDARY_MAX) {
            return 0;
        }
    }

    // Check cells
    s16 cx0 = ((s16) ((data->x + LEVEL_BOUNDARY_MAX) / CELL_SIZE)) & NUM_CELLS_INDEX;
    s16 cz0 = ((s16) ((data->z + LEVEL_BOUNDARY_MAX) / CELL_SIZE)) & NUM_CELLS_INDEX;
    for (s16 dz = -(s16) OMM_COLLISION_CHECK_NEIGHBOR_CELLS; dz <= +(s16) OMM_COLLISION_CHECK_NEIGHBOR_CELLS; ++dz)
    for (s16 dx = -(s16) OMM_COLLISION_CHECK_NEIGHBOR_CELLS; dx <= +(s16) OMM_COLLISION_CHECK_NEIGHBOR_CELLS; ++dx) {
        s16 cx = cx0 + dx;
        s16 cz = cz0 + dz;
        if (cx < 0 || cx >= NUM_CELLS ||
            cz < 0 || cz >= NUM_CELLS) {
            continue;
        }
        find_wall_collisions_from_list(gOmmWalls(1, cx, cz)->data, gOmmWalls(1, cx, cz)->count, data);
        find_wall_collisions_from_list(gOmmWalls(0, cx, cz)->data, gOmmWalls(0, cx, cz)->count, data);
    }
    return data->numWalls;
}

struct Surface *resolve_and_return_wall_collisions(Vec3f pos, f32 offsetY, f32 radius) {
    struct Surface *wRef = NULL;
    struct WallCollisionData data;
    data.x = pos[0];
    data.y = pos[1];
    data.z = pos[2];
    data.radius = radius;
    data.offsetY = offsetY;

    // Find wall collisions and retrieve the referenced wall
    if (find_wall_collisions(&data)) {
        if (OMM_COLLISION_FIX_UNREFERENCED_WALLS) {
            s16 yaw = gMarioState->faceAngle[1];
            for (u16 i = 0, mAngle = 0xFFFF; i < (u16) data.numWalls; i++) {
                struct Surface *w = data.walls[i];
                s16 wAngle = 0x8000 + atan2s(w->normal.z, w->normal.x);
                u16 dAngle = (u16) abs_s((s16) (yaw - wAngle));
                if ((i == 0) || (dAngle < mAngle)) {
                    wRef = w;
                    mAngle = dAngle;
                }
            }
        } else {
            wRef = data.walls[0];
        }
    }

    // Update pos
    pos[0] = data.x;
    pos[1] = data.y;
    pos[2] = data.z;
    return wRef;
}

s32 f32_find_wall_collision(f32 *x, f32 *y, f32 *z, f32 offsetY, f32 radius) {

    // Disable Mario wall collisions during capture
    struct MarioState *m = gMarioState;
    if (x == &m->pos[0] && y == &m->pos[1] && z == &m->pos[2] && omm_mario_is_capture(m)) {
        return 0;
    }

    struct WallCollisionData data;
    data.x = *x;
    data.y = *y;
    data.z = *z;
    data.radius = radius;
    data.offsetY = offsetY;
    s32 numCollisions = find_wall_collisions(&data);
    *x = data.x;
    *y = data.y;
    *z = data.z;
    return numCollisions;
}

//
// Ceilings
//

static struct Surface *find_ceil_from_list(OmmArray surfaces, s32 count, f32 x, f32 y, f32 z, f32 *pHeight) {
    struct Surface *ceil = NULL;
    for (s32 i = 0; i < count; ++i) {
        struct Surface *surf = omm_array_get(surfaces, ptr, i);

        // Ignore no-cam if checking for the camera, or camera surfaces if checking for an object
        if ((gCheckingSurfaceCollisionsForCamera && (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION)) ||
           (!gCheckingSurfaceCollisionsForCamera && (surf->type == SURFACE_CAMERA_BOUNDARY))) {
            continue;
        }

        // Skip if too low or above the previous ceiling
        f32 height = get_surface_height_at_pos(x, z, surf);
        if (height < y - 78.f || height >= *pHeight) {
            continue;
        }

        // Checking if point is in bounds of the triangle
        f32 x1 = surf->vertex1[0];
        f32 x2 = surf->vertex2[0];
        f32 x3 = surf->vertex3[0];
        f32 z1 = surf->vertex1[2];
        f32 z2 = surf->vertex2[2];
        f32 z3 = surf->vertex3[2];
        if ((z1 - z) * (x2 - x1) - (x1 - x) * (z2 - z1) > 0.f ||
            (z2 - z) * (x3 - x2) - (x2 - x) * (z3 - z2) > 0.f ||
            (z3 - z) * (x1 - x3) - (x3 - x) * (z1 - z3) > 0.f) {
            continue;
        }

        // Valid ceiling
        *pHeight = height;
        ceil = surf;
    }
    return ceil;
}

f32 find_ceil(f32 x, f32 y, f32 z, struct Surface **pCeil) {
    static struct Surface *sDummyCeil = NULL;
    if (!pCeil) { pCeil = &sDummyCeil; }
    f32 sHeight = CELL_HEIGHT_LIMIT;
    f32 dHeight = CELL_HEIGHT_LIMIT;
    *pCeil = NULL;

#if OMM_GAME_IS_R96X
    if (cheats_no_bounds(gMarioState)) {
        return sHeight;
    }
#endif

    // Check PUs
    if (OMM_COLLISION_CHECK_PUS) {
        if (x < -LEVEL_BOUNDARY_MAX || x >= +LEVEL_BOUNDARY_MAX ||
            z < -LEVEL_BOUNDARY_MAX || z >= +LEVEL_BOUNDARY_MAX) {
            return sHeight;
        }
    }

    // Check the center cell only
    s16 cx = ((s16) ((x + LEVEL_BOUNDARY_MAX) / CELL_SIZE)) & NUM_CELLS_INDEX;
    s16 cz = ((s16) ((z + LEVEL_BOUNDARY_MAX) / CELL_SIZE)) & NUM_CELLS_INDEX;
    struct Surface *dCeil = find_ceil_from_list(gOmmCeils(1, cx, cz)->data, gOmmCeils(1, cx, cz)->count, x, y, z, &dHeight);
    struct Surface *sCeil = find_ceil_from_list(gOmmCeils(0, cx, cz)->data, gOmmCeils(0, cx, cz)->count, x, y, z, &sHeight);

    // Return the lowest of the two ceilings
    if (dCeil && dHeight < sHeight) {
        *pCeil = dCeil;
        return dHeight;
    }
    *pCeil = sCeil;
    return sHeight;
}

f32 vec3f_find_ceil(Vec3f pos, f32 height, struct Surface **ceil) {
    return find_ceil(pos[0], height + 80.f, pos[2], ceil);
}

//
// Floors
//

OMM_INLINE bool check_is_point_inside_floor_triangle(struct Surface *surf, f32 x, f32 z) {
    f32 x1 = surf->vertex1[0];
    f32 x2 = surf->vertex2[0];
    f32 x3 = surf->vertex3[0];
    f32 z1 = surf->vertex1[2];
    f32 z2 = surf->vertex2[2];
    f32 z3 = surf->vertex3[2];
    if ((z1 - z) * (x2 - x1) - (x1 - x) * (z2 - z1) < 0.f ||
        (z2 - z) * (x3 - x2) - (x2 - x) * (z3 - z2) < 0.f ||
        (z3 - z) * (x1 - x3) - (x3 - x) * (z1 - z3) < 0.f) {
        return false;
    }
    return true;
}

static struct Surface *find_floor_from_list(OmmArray surfaces, s32 count, f32 x, f32 y, f32 z, f32 *pHeight) {
    struct Surface *floor = NULL;
    for (s32 i = 0; i < count; ++i) {
        struct Surface *surf = omm_array_get(surfaces, ptr, i);

        // Ignore no-cam if checking for the camera, or camera surfaces if checking for an object
        if ((gCheckingSurfaceCollisionsForCamera && (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION)) ||
           (!gCheckingSurfaceCollisionsForCamera && (surf->type == SURFACE_CAMERA_BOUNDARY))) {
            continue;
        }

        // Reject perfecty vertical surfaces
        if (surf->normal.y == 0) {
            continue;
        }

        // Skip if too high or below the previous floor
        f32 height = get_surface_height_at_pos(x, z, surf);
        if (height > y + 78.f || height <= *pHeight) {
            continue;
        }

        // Checking if point is in bounds of the triangle
        if (!check_is_point_inside_floor_triangle(surf, x, z)) {
            continue;
        }

        // Valid floor
        *pHeight = height;
        floor = surf;
    }
    return floor;
}

struct Surface *get_pseudo_floor_at_pos(f32 x, f32 y, f32 z) {
    static struct Surface sPseudoFloors[2];
    struct Surface *pseudoFloor = &sPseudoFloors[gOmmGlobals->cameraUpdate];
    pseudoFloor->type = SURFACE_DEFAULT;
    pseudoFloor->force = 0;
    pseudoFloor->flags = 0;
    pseudoFloor->room = -1;
    pseudoFloor->lowerY = y;
    pseudoFloor->upperY = y;
    pseudoFloor->vertex1[0] = x;
    pseudoFloor->vertex1[1] = y;
    pseudoFloor->vertex1[2] = z;
    pseudoFloor->vertex2[0] = x;
    pseudoFloor->vertex2[1] = y;
    pseudoFloor->vertex2[2] = z;
    pseudoFloor->vertex3[0] = x;
    pseudoFloor->vertex3[1] = y;
    pseudoFloor->vertex3[2] = z;
    pseudoFloor->normal.x = 0.f;
    pseudoFloor->normal.y = 1.f;
    pseudoFloor->normal.z = 0.f;
    pseudoFloor->originOffset = -y;
    pseudoFloor->object = NULL;
    return pseudoFloor;
}

#if !OMM_GAME_IS_SM64
static struct Surface *get_eyerock_boss_floor(f32 x, f32 z, f32 x0, f32 y0, f32 z0, f32 radius) {
    static struct Surface sEyerockBossFloor[2][1];

    sEyerockBossFloor[0]->type = SURFACE_DEFAULT;
    sEyerockBossFloor[0]->force = 0;
    sEyerockBossFloor[0]->flags = 0;
    sEyerockBossFloor[0]->room = -1;
    sEyerockBossFloor[0]->lowerY = y0;
    sEyerockBossFloor[0]->upperY = y0;
    sEyerockBossFloor[0]->vertex1[0] = x0 - radius;
    sEyerockBossFloor[0]->vertex1[1] = y0;
    sEyerockBossFloor[0]->vertex1[2] = z0 - radius;
    sEyerockBossFloor[0]->vertex2[0] = x0 + radius;
    sEyerockBossFloor[0]->vertex2[1] = y0;
    sEyerockBossFloor[0]->vertex2[2] = z0 + radius;
    sEyerockBossFloor[0]->vertex3[0] = x0 + radius;
    sEyerockBossFloor[0]->vertex3[1] = y0;
    sEyerockBossFloor[0]->vertex3[2] = z0 - radius;
    sEyerockBossFloor[0]->normal.x = 0.f;
    sEyerockBossFloor[0]->normal.y = 1.f;
    sEyerockBossFloor[0]->normal.z = 0.f;
    sEyerockBossFloor[0]->originOffset = -y0;
    sEyerockBossFloor[0]->object = NULL;
    if (check_is_point_inside_floor_triangle(sEyerockBossFloor[0], x, z)) {
        return sEyerockBossFloor[0];
    }

    sEyerockBossFloor[1]->type = SURFACE_DEFAULT;
    sEyerockBossFloor[1]->force = 0;
    sEyerockBossFloor[1]->flags = 0;
    sEyerockBossFloor[1]->room = -1;
    sEyerockBossFloor[1]->lowerY = y0;
    sEyerockBossFloor[1]->upperY = y0;
    sEyerockBossFloor[1]->vertex1[0] = x0 - radius;
    sEyerockBossFloor[1]->vertex1[1] = y0;
    sEyerockBossFloor[1]->vertex1[2] = z0 - radius;
    sEyerockBossFloor[1]->vertex2[0] = x0 - radius;
    sEyerockBossFloor[1]->vertex2[1] = y0;
    sEyerockBossFloor[1]->vertex2[2] = z0 + radius;
    sEyerockBossFloor[1]->vertex3[0] = x0 + radius;
    sEyerockBossFloor[1]->vertex3[1] = y0;
    sEyerockBossFloor[1]->vertex3[2] = z0 + radius;
    sEyerockBossFloor[1]->normal.x = 0.f;
    sEyerockBossFloor[1]->normal.y = 1.f;
    sEyerockBossFloor[1]->normal.z = 0.f;
    sEyerockBossFloor[1]->originOffset = -y0;
    sEyerockBossFloor[1]->object = NULL;
    if (check_is_point_inside_floor_triangle(sEyerockBossFloor[1], x, z)) {
        return sEyerockBossFloor[1];
    }

    return NULL;
}
#endif

static struct Surface *get_environment_floor(f32 x, f32 height, f32 z) {
    if (gCurrentObject == gMarioObject || gOmmGlobals->cameraUpdate) {
        f32 envHeight = height;
        if (OMM_CHEAT_WALK_ON_WATER) {
            envHeight = max_f(envHeight, find_water_level(x, z));
        }
        if (OMM_CHEAT_WALK_ON_GAS) {
            envHeight = max_f(envHeight, find_poison_gas_level(x, z));
        }
        if (envHeight > height) {
            return get_pseudo_floor_at_pos(x, envHeight, z);
        }
    }
    return NULL;
}

#if !OMM_GAME_IS_SMSR
// floorGeo is always filled, even when there is no floor, to avoid NULL pointer deref crash
// (most of the code calling it assumes that (*floorGeo) is never NULL...)
f32 find_floor_height_and_data(f32 x, f32 y, f32 z, struct FloorGeometry **floorGeo) {
    static struct FloorGeometry sFloorGeo;
    struct Surface *floor;
    f32 floorHeight = find_floor(x, y, z, &floor);
    if (floor) {
        sFloorGeo.normalX = floor->normal.x;
        sFloorGeo.normalY = floor->normal.y;
        sFloorGeo.normalZ = floor->normal.z;
        sFloorGeo.originOffset = floor->originOffset;
    } else {
        sFloorGeo.normalX = 0.f;
        sFloorGeo.normalY = 1.f;
        sFloorGeo.normalZ = 0.f;
        sFloorGeo.originOffset = +11000.f;
    }
    *floorGeo = &sFloorGeo;
    return floorHeight;
}
#endif

f32 find_floor_height(f32 x, f32 y, f32 z) {
    return find_floor(x, y, z, NULL);
}

f32 find_room_floor(f32 x, f32 y, f32 z, struct Surface **pFloor) {
    gFindFloorIncludeSurfaceIntangible = TRUE;
    return find_floor(x, y, z, pFloor);
}

f32 find_floor(f32 x, f32 y, f32 z, struct Surface **pFloor) {
    static struct Surface *sDummyFloor = NULL;
    if (!pFloor) { pFloor = &sDummyFloor; }
    f32 sHeight = FLOOR_LOWER_LIMIT;
    f32 dHeight = FLOOR_LOWER_LIMIT;
    *pFloor = NULL;

    // Call from geo_switch_area
    // Tells the game to find a floor for the star cutscene object
    if (gOmmGlobals->findFloorForCutsceneStar) {
        gOmmGlobals->findFloorForCutsceneStar = false;
        if (gCamera && gCutsceneFocus && (
            gCamera->cutscene == CUTSCENE_STAR_SPAWN ||
            gCamera->cutscene == CUTSCENE_RED_COIN_STAR_SPAWN)) {
            f32 floorY = find_floor(
                gCutsceneFocus->oPosX,
                gCutsceneFocus->oPosY + 100.f,
                gCutsceneFocus->oPosZ,
                pFloor
            );
            if (*pFloor) {
                return floorY;
            }
        }
    }

    // Check PUs
    if (OMM_COLLISION_CHECK_PUS) {
        if (x < -LEVEL_BOUNDARY_MAX || x >= +LEVEL_BOUNDARY_MAX ||
            z < -LEVEL_BOUNDARY_MAX || z >= +LEVEL_BOUNDARY_MAX) {
            return sHeight;
        }
    }

    // Check the center cell only
    s16 cx = ((s16) ((x + LEVEL_BOUNDARY_MAX) / CELL_SIZE)) & NUM_CELLS_INDEX;
    s16 cz = ((s16) ((z + LEVEL_BOUNDARY_MAX) / CELL_SIZE)) & NUM_CELLS_INDEX;
    struct Surface *dFloor = find_floor_from_list(gOmmFloors(1, cx, cz)->data, gOmmFloors(1, cx, cz)->count, x, y, z, &dHeight);
    struct Surface *sFloor = find_floor_from_list(gOmmFloors(0, cx, cz)->data, gOmmFloors(0, cx, cz)->count, x, y, z, &sHeight);

    // BBH Merry-Go-Round
    // Handle SURFACE_INTANGIBLE, used to prevent the wrong room from loading
    // when passing above the Merry-Go-Round, but allows Mario to pass through.
    if (!gFindFloorIncludeSurfaceIntangible && sFloor && sFloor->type == SURFACE_INTANGIBLE) {
        f32 y0 = sHeight - 200.f; sHeight = FLOOR_LOWER_LIMIT;
        sFloor = find_floor_from_list(gOmmFloors(0, cx, cz)->data, gOmmFloors(0, cx, cz)->count, x, y0, z, &sHeight);
    } else {
        gFindFloorIncludeSurfaceIntangible = FALSE;
    }

    // Possession fake floor
    // Because the Possession action doesn't check for floors during the animation (first 20 frames),
    // it can easily result in an out of bounds step, triggering the instant death sequence
    if (!sFloor && !dFloor && gCurrentObject == gMarioObject && !gOmmGlobals->cameraUpdate && omm_mario_is_capture(gMarioState) && gOmmMario->capture.timer < 20) {
        sFloor = get_pseudo_floor_at_pos(x, y, z);
        sHeight = y;
    }

#if !OMM_GAME_IS_SM64
    // Eyerock boss fake floor
    // Fix Eyerock boss fight in various rom-hacks
    // The vanilla behavior heavily depends on hard-coded coordinates, so it kinda breaks when used elsewhere...
    // ...but it works pretty good if we define a plane square floor under the hands
    if (gCurrentObject && gCurrentObject->behavior == bhvEyerokHand) {
        struct Object *eyerokBoss = gCurrentObject->parentObj;
        if (eyerokBoss && eyerokBoss->behavior == bhvEyerokBoss) {
            struct Surface *floor = get_eyerock_boss_floor(x, z, eyerokBoss->oPosX, eyerokBoss->oPosY, eyerokBoss->oPosZ, 2000.f);
            if (floor && floor->upperY > sHeight && floor->upperY > dHeight) {
                *pFloor = floor;
                return floor->upperY;
            }
        }
    }
#endif

#if OMM_GAME_IS_R96X
    // If out-of-bounds but NoBounds is enabled, place a fake death plane below Mario
    if (!sFloor && !dFloor && (gCurrentObject == gMarioObject || gOmmGlobals->cameraUpdate) && cheats_no_bounds(gMarioState)) {
        sFloor = get_pseudo_floor_at_pos(x, FLOOR_LOWER_LIMIT, z);
        sFloor->type = SURFACE_DEATH_PLANE;
        sHeight = FLOOR_LOWER_LIMIT;
    }
#endif

    // Walk on water/gas cheats
    struct Surface *envFloor = get_environment_floor(x, max_f(dHeight, sHeight), z);
    if (envFloor) {
        *pFloor = envFloor;
        return envFloor->lowerY;
    }

    // Return the highest of the two floors
    if (dFloor && dHeight > sHeight) {
        *pFloor = dFloor;
        return dHeight;
    }

    // Is it truly out of bounds?
    // If a slightly slanted wall is between two floors, the previous find_floor_from_list can fail to find a floor
    // To avoid an unintended out of bounds step, treat slanted walls as floors
    //         o
    //         |  ______
    //         | /
    //         |/
    //         /
    //        /|
    // ______/ |
    //         v
    if (!sFloor && OMM_COLLISION_FIX_OUT_OF_BOUNDS_SLANTED_WALLS) {
        sFloor = find_floor_from_list(gOmmWalls(0, cx, cz)->data, gOmmWalls(0, cx, cz)->count, x, y, z, &sHeight);
    }
    *pFloor = sFloor;
    return sHeight;
}

s32 floor_type_exists_in_current_cell(UNUSED f32 x, UNUSED f32 z, UNUSED s16 type, UNUSED u32 dynamic) {
    return TRUE;
}

//
// Environment
//

typedef struct {
    s16 type;
    s16 x0;
    s16 z0;
    s16 x1;
    s16 z1;
    s16 value;
} EnvironmentRegion;

f32 find_water_level_and_floor(f32 x, f32 z, struct Surface **pFloor) {
    if (pFloor) *pFloor = NULL;
    return find_water_level(x, z);
}

f32 find_water_level(f32 x, f32 z) {
    if (!omm_world_is_flooded()) {
        if (gEnvironmentRegions) {
            s16 numRegions = gEnvironmentRegions[0];
            const EnvironmentRegion *regions = (const EnvironmentRegion *) &gEnvironmentRegions[1];
            for (s16 i = 0; i < numRegions; ++i) {
                if (regions[i].type < 50 &&
                    regions[i].x0 <= x && x <= regions[i].x1 &&
                    regions[i].z0 <= z && z <= regions[i].z1) {
                    return (f32) regions[i].value;
                }
            }
        }
        return FLOOR_LOWER_LIMIT;
    }
    return CELL_HEIGHT_LIMIT;
}

f32 find_poison_gas_level(f32 x, f32 z) {
    if (gEnvironmentRegions) {
        s16 numRegions = gEnvironmentRegions[0];
        const EnvironmentRegion *regions = (const EnvironmentRegion *) &gEnvironmentRegions[1];
        for (s16 i = 0; i < numRegions; ++i) {
            if (regions[i].type >= 50 && (regions[i].type % 10) == 0 &&
                regions[i].x0 <= x && x <= regions[i].x1 &&
                regions[i].z0 <= z && z <= regions[i].z1) {
                return regions[i].value;
            }
        }
    }
    return FLOOR_LOWER_LIMIT;
}

void debug_surface_list_info(UNUSED f32 x, UNUSED f32 z) {
}

//
// Ray-casting
//

static OmmArray sCheckedSurfaces = omm_array_zero;

static void find_ray_hits_from_surface_list(OmmArray surfaces, s32 count, Vec3f orig, Vec3f ndir, f32 maxDist, f32 surfaceScale, RayCollisionData *hits, bool rejectNoCamCol) {
    f32 upperY = max_f(orig[1], orig[1] + ndir[1] * maxDist);
    f32 lowerY = min_f(orig[1], orig[1] + ndir[1] * maxDist);
    for (s32 i = 0; i < count && hits->count < MAX_RAYCAST_COL_HITS; ++i) {
        struct Surface *surf = omm_array_get(surfaces, ptr, i);

        // Reject surface out of vertical bounds
        if (surf->lowerY > upperY ||
            surf->upperY < lowerY) {
            continue;
        }

        // Reject no-cam collision and vanish cap walls surfaces
        if (rejectNoCamCol && (
            (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) ||
            (surf->type == SURFACE_NO_CAM_COLLISION) ||
            (surf->type == SURFACE_NO_CAM_COLLISION_77) ||
            (surf->type == SURFACE_NO_CAM_COL_VERY_SLIPPERY) ||
            (surf->type == SURFACE_NO_CAM_COL_SLIPPERY) ||
            (surf->type == SURFACE_SWITCH) ||
            (surf->type == SURFACE_VANISH_CAP_WALLS))) {
            continue;
        }

        // Reject camera boundary surfaces
        if (!rejectNoCamCol && surf->type == SURFACE_CAMERA_BOUNDARY) {
            continue;
        }

        // Exclude already checked surfaces
        if (omm_array_find(sCheckedSurfaces, ptr, surf) != -1) {
            continue;
        }
        omm_array_add(sCheckedSurfaces, ptr, surf);

        // Check intersection
        Vec3f v0; vec3s_to_vec3f(v0, surf->vertex1);
        Vec3f v1; vec3s_to_vec3f(v1, surf->vertex2);
        Vec3f v2; vec3s_to_vec3f(v2, surf->vertex3);
        if (surfaceScale != 1.f) {
            Vec3f vc = {
                (v0[0] + v1[0] + v2[0]) / 3.f,
                (v0[1] + v1[1] + v2[1]) / 3.f,
                (v0[2] + v1[2] + v2[2]) / 3.f,
            };
            Vec3f v0c; vec3f_mul(vec3f_dif(v0c, v0, vc), surfaceScale);
            Vec3f v1c; vec3f_mul(vec3f_dif(v1c, v1, vc), surfaceScale);
            Vec3f v2c; vec3f_mul(vec3f_dif(v2c, v2, vc), surfaceScale);
            vec3f_sum(v0, vc, v0c);
            vec3f_sum(v1, vc, v1c);
            vec3f_sum(v2, vc, v2c);
        }
        Vec3f e1; vec3f_dif(e1, v1, v0);
        Vec3f e2; vec3f_dif(e2, v2, v0);
        Vec3f vh; vec3f_cross(vh, ndir, e2);

        // Perpendicular?
        f32 dot = vec3f_dot(e1, vh);
        if (dot > -0.01f && dot < 0.01f) {
            continue;
        }
        f32 invdot = 1.f / dot;

        // Contact?
        Vec3f vo; vec3f_dif(vo, orig, v0);
        f32 u = invdot * vec3f_dot(vo, vh);
        if (u < 0.f || u > 1.f) {
            continue;
        }
        Vec3f voe1; vec3f_cross(voe1, vo, e1);
        f32 v = invdot * vec3f_dot(ndir, voe1);
        if (v < 0.f || (u + v) > 1.f) {
            continue;
        }

        // Compute dist
        f32 dist = invdot * vec3f_dot(e2, voe1);
        if (dist < 0.01f || dist > maxDist) {
            continue;
        }

        // Successful hit
        RayHit hit;
        vec3f_copy(hit.pos, ndir);
        vec3f_mul(hit.pos, dist);
        vec3f_add(hit.pos, orig);
        hit.dist = dist;
        hit.surf = surf;
        hit.ratio = dist / maxDist;

        // Reject hit if not opposed to the ray direction
        if (vec3f_dot(ndir, (f32 *) &hit.surf->normal) > 0.f) {
            continue;
        }

        // Add hit to the hits sorted list
        for (s32 i = 0; i <= hits->count; ++i) {
            if (i == hits->count || hits->hits[i].dist > hit.dist) {
                mem_mov(hits->hits + i + 1, hits->hits + i, sizeof(RayHit) * (hits->count - i));
                hits->hits[i] = hit;
                hits->count++;
                break;
            }
        }
    }
}

static void find_ray_hits_on_cell(s16 cx, s16 cz, Vec3f orig, Vec3f ndir, f32 maxDist, f32 surfaceScale, RayCollisionData *hits, u32 flags) {
    if (cx >= 0 && cx < NUM_CELLS && cz >= 0 && cz < NUM_CELLS) {

        // Walls
        if (flags & RAYCAST_FLAG_WALLS) {
            find_ray_hits_from_surface_list(gOmmWalls(1, cx, cz)->data, gOmmWalls(1, cx, cz)->count, orig, ndir, maxDist, surfaceScale, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
            find_ray_hits_from_surface_list(gOmmWalls(0, cx, cz)->data, gOmmWalls(0, cx, cz)->count, orig, ndir, maxDist, surfaceScale, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
        }

        // Floors
        if ((flags & RAYCAST_FLAG_FLOORS) && ndir[1] < +0.99f) {
            find_ray_hits_from_surface_list(gOmmFloors(1, cx, cz)->data, gOmmFloors(1, cx, cz)->count, orig, ndir, maxDist, surfaceScale, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
            find_ray_hits_from_surface_list(gOmmFloors(0, cx, cz)->data, gOmmFloors(0, cx, cz)->count, orig, ndir, maxDist, surfaceScale, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
        }

        // Ceilings
        if ((flags & RAYCAST_FLAG_CEILS) && ndir[1] > -0.99f) {
            find_ray_hits_from_surface_list(gOmmCeils(1, cx, cz)->data, gOmmCeils(1, cx, cz)->count, orig, ndir, maxDist, surfaceScale, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
            find_ray_hits_from_surface_list(gOmmCeils(0, cx, cz)->data, gOmmCeils(0, cx, cz)->count, orig, ndir, maxDist, surfaceScale, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
        }
    }
}

s32 find_collisions_on_ray(Vec3f orig, Vec3f dir, RayCollisionData *hits, f32 surfaceScale, u32 flags) {
    hits->count = 0;
    f32 maxDist = vec3f_length(dir);
    if (maxDist < 0.0001f) {
        return 0;
    }

    // Normalized dir
    Vec3f ndir;
    vec3f_copy(ndir, dir);
    vec3f_mul(ndir, 1.f / maxDist);

    // Steps and cells
    f32 steps = 4.f * max_f(abs_f(dir[0]), abs_f(dir[2])) / CELL_SIZE;
    f32 dx = dir[0] / (steps * CELL_SIZE);
    f32 dz = dir[2] / (steps * CELL_SIZE);
    f32 cx = (orig[0] + LEVEL_BOUNDARY_MAX) / CELL_SIZE;
    f32 cz = (orig[2] + LEVEL_BOUNDARY_MAX) / CELL_SIZE;

    // DDA
    for (s32 i = 0; i <= (s32) steps; ++i, cx += dx, cz += dz) {
        find_ray_hits_on_cell(cx, cz, orig, ndir, maxDist, surfaceScale, hits, flags);
    }
    omm_array_clear(sCheckedSurfaces);
    return hits->count;
}

bool find_first_hit_on_ray(Vec3f orig, Vec3f dir, Vec3f hitPos, f32 offset, f32 surfaceScale, u32 flags) {
    RayCollisionData hits;
    if (find_collisions_on_ray(orig, dir, &hits, surfaceScale, flags)) {
        const RayHit *hit = hits.hits;
        hitPos[0] = hit->pos[0] + offset * hit->surf->normal.x;
        hitPos[1] = hit->pos[1] + offset * hit->surf->normal.y;
        hitPos[2] = hit->pos[2] + offset * hit->surf->normal.z;
        return true;
    }
    vec3f_sum(hitPos, orig, dir);
    return false;
}

void find_surface_on_ray(Vec3f orig, Vec3f dir, struct Surface **hitSurface, Vec3f hitPos BETTER_CAM_RAYCAST_ARGS) {
    RayCollisionData hits;
    if (find_collisions_on_ray(orig, dir, &hits, 1.f, gCheckingSurfaceCollisionsForCamera ? RAYCAST_FLAGS_CAMERA : RAYCAST_FLAGS_SURFACES)) {
        *hitSurface = hits.hits->surf;
        vec3f_copy(hitPos, hits.hits->pos);
    } else {
        *hitSurface = NULL;
        vec3f_sum(hitPos, orig, dir);
    }
}

//
// Advanced math stuff
//

static bool disk_overlaps_point(Vec2f point, f32 radius2) {
    return vec2f_length2(point) <= radius2;
}

static bool disk_overlaps_segment(Vec2f point0, Vec2f point1, f32 radius2) {
    Vec2f direction; vec2f_dif(direction, point0, point1);
    f32 dot = vec2f_dot(point0, direction);
    if (dot <= 0) {
        return vec2f_length2(point0) <= radius2;
    }
    f32 length2 = vec2f_length2(direction);
    if (dot >= length2) {
        return vec2f_length2(point1) <= radius2;
    }
    f32 dotperp = vec2f_dotperp(direction, point0);
    return dotperp * dotperp <= length2 * radius2;
}

static bool disk_overlaps_polygon(Vec2f *points, u32 count, f32 radius2) {
    bool positive = false, negative = false;
    for (u32 i0 = count - 1, i1 = 0; i1 != count && !(positive && negative); i0 = i1++) {
        Vec2f direction; vec2f_dif(direction, points[i0], points[i1]);
        f32 dotperp = vec2f_dotperp(points[i0], direction);
        positive |= dotperp > 0;
        negative |= dotperp < 0;
    }
    if (!positive || !negative) {
        return true;
    }
    for (u32 i0 = count - 1, i1 = 0; i1 != count; i0 = i1++) {
        if (disk_overlaps_segment(points[i0], points[i1], radius2)) {
            return true;
        }
    }
    return false;
}

static u32 get_index_of_lowest_vertex(Vec3s *vertices) {
    s16 ymin = min_3_s(vertices[0][1], vertices[1][1], vertices[2][1]);
    if (ymin == vertices[0][1]) return 0;
    if (ymin == vertices[1][1]) return 1;
    return 2;
}

static u32 get_index_of_highest_vertex(Vec3s *vertices) {
    s16 ymin = max_3_s(vertices[0][1], vertices[1][1], vertices[2][1]);
    if (ymin == vertices[2][1]) return 2;
    if (ymin == vertices[1][1]) return 1;
    return 0;
}

static void compute_dir(Vec2f dest, Vec2f point0, Vec2f point1, f32 denom) {
    dest[0] = (point0[0] - point1[0]) / denom;
    dest[1] = (point0[1] - point1[1]) / denom;
}

static void compute_polygon_point(Vec2f dest, Vec2f point, f32 mult, Vec2f dir) {
    dest[0] = point[0] + mult * dir[0];
    dest[1] = point[1] + mult * dir[1];
}

// Credits to https://www.geometrictools.com/GTE/Mathematics/IntrTriangle3Cylinder3.h
bool surface_intersects_cylinder(struct Surface *surf, Vec3f pos, f32 radius, f32 height, f32 downOffset) {

    // Sort the triangle vertices so that y[0] <= y[1] <= y[2]
    Vec3s vertices[3];
    vec3s_copy(vertices[0], surf->vertex1);
    vec3s_copy(vertices[1], surf->vertex2);
    vec3s_copy(vertices[2], surf->vertex3);
    u32 li = get_index_of_lowest_vertex(vertices);
    u32 hi = get_index_of_highest_vertex(vertices);
    u32 mi = 3 - (li + hi);

    // Compute the coordinates of the surface vertices with the center of the cylinder as the origin
    f32 heightHalf = height / 2;
    Vec3f xyz[3] = {
        { vertices[li][0] - pos[0], vertices[li][1] - (pos[1] + heightHalf - downOffset), vertices[li][2] - pos[2] },
        { vertices[mi][0] - pos[0], vertices[mi][1] - (pos[1] + heightHalf - downOffset), vertices[mi][2] - pos[2] },
        { vertices[hi][0] - pos[0], vertices[hi][1] - (pos[1] + heightHalf - downOffset), vertices[hi][2] - pos[2] },
    };

    // y components
    f32 y[3] = { xyz[0][1], xyz[1][1], xyz[2][1] };

    // Early exit 1: triangle is strictly below/above cylinder
    if (y[2] < -heightHalf || y[0] > +heightHalf) {
        return false;
    }

    // xz components
    Vec2f xz[3] = {
        { xyz[0][0], xyz[0][2] },
        { xyz[1][0], xyz[1][2] },
        { xyz[2][0], xyz[2][2] },
    };

    // Early exit 2: triangle is not in the cylinder horizontal range
    f32 radius2 = radius * radius;
    if (!disk_overlaps_polygon(xz, 3, radius2)) {
        return false;
    }

    // Early exit 3: if the triangle is vertically inside the cylinder,
    // there is intersection, thanks to the previous check
    if (-heightHalf <= y[0] && y[2] <= heightHalf) {
        return true;
    }

    // Lowest is below the cylinder
    if (y[0] < -heightHalf) {

        // Highest is above the cylinder
        if (y[2] > heightHalf) {

            // Middle is at the top or above the cylinder
            if (y[1] >= heightHalf) {
                f32 numerNeg0 = -heightHalf - y[0];
                f32 numerPos0 = +heightHalf - y[0];
                f32 denom10 = y[1] - y[0];
                f32 denom20 = y[2] - y[0];
                Vec2f dir20; compute_dir(dir20, xz[2], xz[0], denom20);
                Vec2f dir10; compute_dir(dir10, xz[1], xz[0], denom10);
                Vec2f polygon[4];
                compute_polygon_point(polygon[0], xz[0], numerNeg0, dir20);
                compute_polygon_point(polygon[1], xz[0], numerNeg0, dir10);
                compute_polygon_point(polygon[2], xz[0], numerPos0, dir10);
                compute_polygon_point(polygon[3], xz[0], numerPos0, dir20);
                return disk_overlaps_polygon(polygon, 4, radius2);
            }

            // Middle is at the bottom or below the cylinder
            if (y[1] <= -heightHalf) {
                f32 numerNeg2 = -heightHalf - y[2];
                f32 numerPos2 = +heightHalf - y[2];
                f32 denom02 = y[0] - y[2];
                f32 denom12 = y[1] - y[2];
                Vec2f dir02; compute_dir(dir02, xz[0], xz[2], denom02);
                Vec2f dir12; compute_dir(dir12, xz[1], xz[2], denom12);
                Vec2f polygon[4];
                compute_polygon_point(polygon[0], xz[2], numerNeg2, dir02);
                compute_polygon_point(polygon[1], xz[2], numerNeg2, dir12);
                compute_polygon_point(polygon[2], xz[2], numerPos2, dir12);
                compute_polygon_point(polygon[3], xz[2], numerPos2, dir02);
                return disk_overlaps_polygon(polygon, 4, radius2);
            }

            // Middle is inside the cylinder
            f32 numerNeg0 = -heightHalf - y[0];
            f32 numerPos0 = +heightHalf - y[0];
            f32 numerNeg1 = -heightHalf - y[1];
            f32 numerPos1 = +heightHalf - y[1];
            f32 denom20 = y[2] - y[0];
            f32 denom01 = y[0] - y[1];
            f32 denom21 = y[2] - y[1];
            Vec2f dir20; compute_dir(dir20, xz[2], xz[0], denom20);
            Vec2f dir01; compute_dir(dir01, xz[0], xz[1], denom01);
            Vec2f dir21; compute_dir(dir21, xz[2], xz[1], denom21);
            Vec2f polygon[5];
            compute_polygon_point(polygon[0], xz[0], numerNeg0, dir20);
            compute_polygon_point(polygon[1], xz[1], numerNeg1, dir01);
            compute_polygon_point(polygon[2], xz[1], 0, gVec2fZero);
            compute_polygon_point(polygon[3], xz[1], numerPos1, dir21);
            compute_polygon_point(polygon[4], xz[0], numerPos0, dir20);
            return disk_overlaps_polygon(polygon, 5, radius2);
        }

        // Highest is above the bottom of the cylinder
        if (y[2] > -heightHalf) {

            // Middle is at the bottom or below the cylinder
            if (y[1] <= -heightHalf) {
                f32 numerNeg2 = -heightHalf - y[2];
                f32 denom02 = y[0] - y[2];
                f32 denom12 = y[1] - y[2];
                Vec2f dir02; compute_dir(dir02, xz[0], xz[2], denom02);
                Vec2f dir12; compute_dir(dir12, xz[1], xz[2], denom12);
                Vec2f polygon[3];
                compute_polygon_point(polygon[0], xz[2], 0, gVec2fZero);
                compute_polygon_point(polygon[1], xz[2], numerNeg2, dir02);
                compute_polygon_point(polygon[2], xz[2], numerNeg2, dir12);
                return disk_overlaps_polygon(polygon, 3, radius2);
            }

            // Middle is above the bottom of the cylinder
            f32 numerNeg0 = -heightHalf - y[0];
            f32 denom10 = y[1] - y[0];
            f32 denom20 = y[2] - y[0];
            Vec2f dir20; compute_dir(dir20, xz[2], xz[0], denom20);
            Vec2f dir10; compute_dir(dir10, xz[1], xz[0], denom10);
            Vec2f polygon[4];
            compute_polygon_point(polygon[0], xz[0], numerNeg0, dir20);
            compute_polygon_point(polygon[1], xz[0], numerNeg0, dir10);
            compute_polygon_point(polygon[2], xz[1], 0, gVec2fZero);
            compute_polygon_point(polygon[3], xz[2], 0, gVec2fZero);
            return disk_overlaps_polygon(polygon, 4, radius2);
        }

        // Middle is below the cylinder
        if (y[1] < -heightHalf) {
            return disk_overlaps_point(xz[2], radius2);
        }

        // Middle and highest are at the bottom of the cylinder
        return disk_overlaps_segment(xz[1], xz[2], radius2);
    }

    // Lowest is below the top of the cylinder
    if (y[0] < heightHalf) {

        // Middle is at the top or above the cylinder
        if (y[1] >= heightHalf) {
            f32 numerPos0 = +heightHalf - y[0];
            f32 denom10 = y[1] - y[0];
            f32 denom20 = y[2] - y[0];
            Vec2f dir10; compute_dir(dir10, xz[1], xz[0], denom10);
            Vec2f dir20; compute_dir(dir20, xz[2], xz[0], denom20);
            Vec2f polygon[3];
            compute_polygon_point(polygon[0], xz[0], 0, gVec2fZero);
            compute_polygon_point(polygon[1], xz[0], numerPos0, dir10);
            compute_polygon_point(polygon[2], xz[0], numerPos0, dir20);
            return disk_overlaps_polygon(polygon, 3, radius2);
        }

        // Highest and middle are at the top or above the cylinder
        f32 numerPos2 = +heightHalf - y[2];
        f32 denom02 = y[0] - y[2];
        f32 denom12 = y[1] - y[2];
        Vec2f dir02; compute_dir(dir02, xz[0], xz[2], denom02);
        Vec2f dir12; compute_dir(dir12, xz[1], xz[2], denom12);
        Vec2f polygon[4];
        compute_polygon_point(polygon[0], xz[0], 0, gVec2fZero);
        compute_polygon_point(polygon[1], xz[1], 0, gVec2fZero);
        compute_polygon_point(polygon[2], xz[2], numerPos2, dir12);
        compute_polygon_point(polygon[3], xz[2], numerPos2, dir02);
        return disk_overlaps_polygon(polygon, 4, radius2);
    }

    // Middle is above the cylinder
    if (y[1] > heightHalf) {
        return disk_overlaps_point(xz[0], radius2);
    }

    // Lowest and middle are at the top of the cylinder
    return disk_overlaps_segment(xz[0], xz[1], radius2);
}

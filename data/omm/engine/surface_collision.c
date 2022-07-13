#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

s32 gFindFloorForCutsceneStar = 0;
u8 gInterpolatingSurfaces;

// Enhanced surface collision system
// Enabled by setting Moveset to Odyssey
// Adds several fixes and increases accuracy:
// - Treats PUs as out of bounds
// - Fixes unreferenced wall glitch, and returns the closest wall in the direction of Mario
// - Checks floor on a square instead of a point, to prevent objects to fall inside small gaps
// - Increases number of cells checked for more precise wall collisions

static void recompute_surface_parameters(struct Surface *surf) {
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
    f32 mag = max_f(0.0001f, sqrtf(sqr_f(nx) + sqr_f(ny) + sqr_f(nz)));
    surf->lowerY = min_3_s(y1, y2, y3) - 5;
    surf->upperY = max_3_s(y1, y2, y3) + 5;
    surf->normal.x = nx / mag;
    surf->normal.y = ny / mag;
    surf->normal.z = nz / mag;
    surf->originOffset = -(nx * x1 + ny * y1 + nz * z1) / mag;
}

//
// Walls
//

static bool check_wall_extension(struct Surface *surf, f32 y) {
#if OMM_GAME_IS_SM64
    if (OMM_SSM_IS_ENABLED) {

    // Extended volcano walls
    // The purpose is to extend the volcano walls vertically to prevent
    // Mario from wall-jumping behind them when the rising lava object is spawned
    if (gCurrLevelNum == LEVEL_LLL && gCurrAreaIndex == 2 && surf->upperY > 5200) {
        if (surf->upperY < 8000) {
            if (surf->vertex1[1] > 5200) surf->vertex1[1] = 8000;
            if (surf->vertex2[1] > 5200) surf->vertex2[1] = 8000;
            if (surf->vertex3[1] > 5200) surf->vertex3[1] = 8000;
            recompute_surface_parameters(surf);
        }
        return (y > 5200.f);
    }

    // Extended docks walls
    // The purpose is to extend the docks walls vertically
    // to prevent Mario from falling behind them
    if (gCurrLevelNum == LEVEL_DDD && gCurrAreaIndex == 2 && surf->upperY > 2500) {
        if (surf->upperY < 5000) {
            if (surf->vertex1[1] > 2500) surf->vertex1[1] = 5000;
            if (surf->vertex2[1] > 2500) surf->vertex2[1] = 5000;
            if (surf->vertex3[1] > 2500) surf->vertex3[1] = 5000;
            recompute_surface_parameters(surf);
        }
        return (y > 2500.f);
    }
    
    }
#else
    OMM_UNUSED(surf);
    OMM_UNUSED(y);
#endif
    return false;
}

static void find_wall_collisions_from_list(OmmArray surfaces, s32 count, struct WallCollisionData *data) {
    for (s32 i = 0, isWallExtended = 0; i < count && data->numWalls < 16; ++i, isWallExtended = 0) {
        struct Surface *surf = omm_array_get(surfaces, ptr, i);
        f32 x = data->x;
        f32 y = data->y + data->offsetY;
        f32 z = data->z;

        // Check wall extensions
        isWallExtended = check_wall_extension(surf, data->y);

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
            (surf->type == SURFACE_VANISH_CAP_WALLS && gCurrentObject == gMarioObject && (gMarioState->flags & MARIO_VANISH_CAP)) ||
            (surf->type == SURFACE_VANISH_CAP_WALLS && gCurrentObject && (gCurrentObject->activeFlags & ACTIVE_FLAG_MOVE_THROUGH_GRATE))) {
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
        // Register the wall only if it's not an extended wall
        data->x += surf->normal.x * (data->radius - offset);
        data->z += surf->normal.z * (data->radius - offset);
        if (!isWallExtended) {
            data->walls[data->numWalls++] = surf;
        }
    }
}

s32 find_wall_collisions(struct WallCollisionData *data) {
    data->numWalls = 0;

#if OMM_GAME_IS_R96A
    if (cheats_no_bounds(gMarioState)) {
        return 0;
    }
#endif

    // CCM Racing penguin
    // His radius is bigger than 200, but not capping it at 200 breaks its path
#if !OMM_GAME_IS_SMSR
    if (gCurrentObject && gCurrentObject->behavior == bhvRacingPenguin) {
        data->radius = min_f(data->radius, 200.f);
    }
#endif

    // Check PUs
    if (OMM_COLLISION_CHECK_PUS) {
        if (data->x < -LEVEL_BOUNDS || data->x >= +LEVEL_BOUNDS ||
            data->z < -LEVEL_BOUNDS || data->z >= +LEVEL_BOUNDS) {
            return 0;
        }
    }

    // Check cells
    s16 cx0 = ((s16) ((data->x + LEVEL_BOUNDS) / CELL_SIZE)) & CELL_BITS;
    s16 cz0 = ((s16) ((data->z + LEVEL_BOUNDS) / CELL_SIZE)) & CELL_BITS;
    for (s16 dz = -(s16) OMM_COLLISION_CHECK_NEIGHBOR_CELLS; dz <= +(s16) OMM_COLLISION_CHECK_NEIGHBOR_CELLS; ++dz)
    for (s16 dx = -(s16) OMM_COLLISION_CHECK_NEIGHBOR_CELLS; dx <= +(s16) OMM_COLLISION_CHECK_NEIGHBOR_CELLS; ++dx) {
        s16 cx = cx0 + dx;
        s16 cz = cz0 + dz;
        if (cx < 0 || cx >= CELL_COUNT ||
            cz < 0 || cz >= CELL_COUNT) {
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

static void check_ceil_extension(struct Surface *surf) {
#if OMM_GAME_IS_SM64
    if (OMM_SSM_IS_ENABLED) {

    // Extended castle upstairs ceilings
    // The purpose is to extend the castle upstairs ceilings horizontally
    // to prevent Mario from going behind the clock
    if (gCurrLevelNum == LEVEL_CASTLE && gCurrAreaIndex == 2 && surf->lowerY > 3100 && surf->upperY < 3200) {
        s16 xmin = min_3_s(surf->vertex1[0], surf->vertex2[0], surf->vertex3[0]);
        s16 xmax = max_3_s(surf->vertex1[0], surf->vertex2[0], surf->vertex3[0]);
        s16 zmax = max_3_s(surf->vertex1[2], surf->vertex2[2], surf->vertex3[2]);
        if (xmin > -450 && xmax < 50 && zmax < 8000) {
            if (surf->vertex1[2] > 7220) surf->vertex1[2] = 8000;
            if (surf->vertex2[2] > 7220) surf->vertex2[2] = 8000;
            if (surf->vertex3[2] > 7220) surf->vertex3[2] = 8000;
            recompute_surface_parameters(surf);
        }
        return;
    }
    
    }
#else
    OMM_UNUSED(surf);
#endif
}

static struct Surface *find_ceil_from_list(OmmArray surfaces, s32 count, f32 x, f32 y, f32 z, f32 *pHeight) {
    struct Surface *ceil = NULL;
    for (s32 i = 0; i < count; ++i) {
        struct Surface *surf = omm_array_get(surfaces, ptr, i);

        // Ignore no-cam if checking for the camera, or camera surfaces if checking for an object
        if ((gCheckingSurfaceCollisionsForCamera && (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION)) ||
           (!gCheckingSurfaceCollisionsForCamera && (surf->type == SURFACE_CAMERA_BOUNDARY))) {
            continue;
        }

        // Check ceil extensions
        check_ceil_extension(surf);

        // Skip if too low or above the previous ceiling
        f32 height = -(x * surf->normal.x + z * surf->normal.z + surf->originOffset) / surf->normal.y;
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
    f32 sHeight = +20000.f;
    f32 dHeight = +20000.f;
    *pCeil = NULL;

#if OMM_GAME_IS_R96A
    if (cheats_no_bounds(gMarioState)) {
        return sHeight;
    }
#endif

    // Check PUs
    if (OMM_COLLISION_CHECK_PUS) {
        if (x < -LEVEL_BOUNDS || x >= +LEVEL_BOUNDS ||
            z < -LEVEL_BOUNDS || z >= +LEVEL_BOUNDS) {
            return 0;
        }
    }

    // Check the center cell only
    s16 cx = ((s16) ((x + LEVEL_BOUNDS) / CELL_SIZE)) & CELL_BITS;
    s16 cz = ((s16) ((z + LEVEL_BOUNDS) / CELL_SIZE)) & CELL_BITS;
    struct Surface *dCeil = find_ceil_from_list(gOmmCeils(1, cx, cz)->data, gOmmCeils(1, cx, cz)->count, x, y, z, &dHeight);
    struct Surface *sCeil = find_ceil_from_list(gOmmCeils(0, cx, cz)->data, gOmmCeils(0, cx, cz)->count, x, y, z, &sHeight);

    // Return the lowest of the two ceilings
    if (dCeil != NULL && dHeight < sHeight) {
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

        // Skip if too high or below the previous floor
        f32 height = -(x * surf->normal.x + z * surf->normal.z + surf->originOffset) / surf->normal.y;
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
    static struct Surface sPseudoFloors[64];
    static s32 sPseudoFloorIndex = 0;
    struct Surface *pseudoFloor = &sPseudoFloors[(sPseudoFloorIndex = (sPseudoFloorIndex + 1) & 0x3F)];
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
    if (gCurrentObject == gMarioObject) {
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
    if (floor != NULL) {
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
    f32 sHeight = -11000.f;
    f32 dHeight = -11000.f;
    *pFloor = NULL;

    // Call from geo_switch_area
    // Tells the game to find a floor for the star cutscene object
    if (gFindFloorForCutsceneStar) {
        gFindFloorForCutsceneStar = FALSE;
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
        if (x < -LEVEL_BOUNDS || x >= +LEVEL_BOUNDS ||
            z < -LEVEL_BOUNDS || z >= +LEVEL_BOUNDS) {
            return 0;
        }
    }

    // Check the center cell only
    s16 cx = ((s16) ((x + LEVEL_BOUNDS) / CELL_SIZE)) & CELL_BITS;
    s16 cz = ((s16) ((z + LEVEL_BOUNDS) / CELL_SIZE)) & CELL_BITS;
    struct Surface *dFloor = find_floor_from_list(gOmmFloors(1, cx, cz)->data, gOmmFloors(1, cx, cz)->count, x, y, z, &dHeight);
    struct Surface *sFloor = find_floor_from_list(gOmmFloors(0, cx, cz)->data, gOmmFloors(0, cx, cz)->count, x, y, z, &sHeight);

    // BBH Merry-Go-Round
    // Handle SURFACE_INTANGIBLE, used to prevent the wrong room from loading
    // when passing above the Merry-Go-Round, but allows Mario to pass through.
    if (!gFindFloorIncludeSurfaceIntangible && sFloor && sFloor->type == SURFACE_INTANGIBLE) {
        f32 y0 = sHeight - 200.f; sHeight = -11000.f;
        sFloor = find_floor_from_list(gOmmFloors(0, cx, cz)->data, gOmmFloors(0, cx, cz)->count, x, y0, z, &sHeight);
    } else {
        gFindFloorIncludeSurfaceIntangible = FALSE;
    }

    // Possession fake floor
    // Because the Possession action doesn't check for floors during the animation (first 20 frames),
    // it can easily result in an out of bounds step, triggering the instant death sequence
    if (!sFloor && !dFloor) {
        if (gMarioState->action == ACT_OMM_POSSESSION && gOmmData->mario->capture.timer < 20) {
            sFloor = get_pseudo_floor_at_pos(x, y, z);
            sHeight = y;
        }
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

#if OMM_GAME_IS_R96A
    // If out-of-bounds but NoBounds is enabled, place a fake death plane below Mario
    if (!sFloor && !dFloor && cheats_no_bounds(gMarioState)) {
        sFloor = get_pseudo_floor_at_pos(x, -11000.f, z);
        sFloor->type = SURFACE_DEATH_PLANE;
        sHeight = -11000.f;
    }
#endif

    // Walk on water/gas cheats
    struct Surface *envFloor = get_environment_floor(x, max_f(dHeight, sHeight), z);
    if (envFloor) {
        *pFloor = envFloor;
        return envFloor->lowerY;
    }

    // Return the highest of the two floors
    if (dFloor != NULL && dHeight > sHeight) {
        *pFloor = dFloor;
        return dHeight;
    }
    *pFloor = sFloor;
    return sHeight;
}

// What's the purpose of the original function?
// If it's about optimization, it's badly done.
// The game retrieves the floor at (x, z) ONLY if there is an intangible floor in that cell.
// Otherwise, it uses Mario's floor... even if the roomFocusObj is not Mario.
// Remember kids: premature optimization is the root of all evil.
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
        if (gEnvironmentRegions != NULL) {
            s16 numRegions = gEnvironmentRegions[0];
            const EnvironmentRegion *regions = (const EnvironmentRegion *) &gEnvironmentRegions[1];
            for (s16 i = 0; i < numRegions; ++i) {
                if (regions[i].type < 50 &&
                    regions[i].x0 <= x && x <= regions[i].x1 &&
                    regions[i].z0 <= z && z <= regions[i].z1) {
                    return regions[i].value;
                }
            }
        }
        return -11000.f;
    }
    return +20000.f;
}

f32 find_poison_gas_level(f32 x, f32 z) {
    if (gEnvironmentRegions != NULL) {
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
    return -11000.f;
}

void debug_surface_list_info(UNUSED f32 x, UNUSED f32 z) {
}

//
// Ray-casting
//

static OmmArray sCheckedSurfaces = omm_array_zero;

static void find_ray_hits_from_surface_list(OmmArray surfaces, s32 count, Vec3f orig, Vec3f ndir, f32 maxDist, f32 delta, RayCollisionData *hits, bool rejectNoCamCol) {
    f32 upperY = max_f(orig[1], orig[1] + ndir[1] * maxDist);
    f32 lowerY = min_f(orig[1], orig[1] + ndir[1] * maxDist);
    for (s32 i = 0; i < count && hits->count < 16; ++i) {
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
        if (delta != 1.f) {
            Vec3f vc = {
                (v0[0] + v1[0] + v2[0]) / 3.f,
                (v0[1] + v1[1] + v2[1]) / 3.f,
                (v0[2] + v1[2] + v2[2]) / 3.f,
            };
            Vec3f v0c; vec3f_mul(vec3f_dif(v0c, v0, vc), delta);
            Vec3f v1c; vec3f_mul(vec3f_dif(v1c, v1, vc), delta);
            Vec3f v2c; vec3f_mul(vec3f_dif(v2c, v2, vc), delta);
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
                OMM_MEMMOV(hits->hits + i + 1, hits->hits + i, sizeof(RayHit) * (hits->count - i));
                hits->hits[i] = hit;
                hits->count++;
                break;
            }
        }
    }
}

static void find_ray_hits_on_cell(s16 cx, s16 cz, Vec3f orig, Vec3f ndir, f32 maxDist, f32 delta, RayCollisionData *hits, u32 flags) {
    if (cx >= 0 && cx < CELL_COUNT && cz >= 0 && cz < CELL_COUNT) {

        // Walls
        if (flags & RAYCAST_FLAG_WALLS) {
            find_ray_hits_from_surface_list(gOmmWalls(1, cx, cz)->data, gOmmWalls(1, cx, cz)->count, orig, ndir, maxDist, delta, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
            find_ray_hits_from_surface_list(gOmmWalls(0, cx, cz)->data, gOmmWalls(0, cx, cz)->count, orig, ndir, maxDist, delta, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
        }
        
        // Floors
        if ((flags & RAYCAST_FLAG_FLOORS) && ndir[1] < +0.99f) {
            find_ray_hits_from_surface_list(gOmmFloors(1, cx, cz)->data, gOmmFloors(1, cx, cz)->count, orig, ndir, maxDist, delta, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
            find_ray_hits_from_surface_list(gOmmFloors(0, cx, cz)->data, gOmmFloors(0, cx, cz)->count, orig, ndir, maxDist, delta, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
        }

        // Ceilings
        if ((flags & RAYCAST_FLAG_CEILS) && ndir[1] > -0.99f) {
            find_ray_hits_from_surface_list(gOmmCeils(1, cx, cz)->data, gOmmCeils(1, cx, cz)->count, orig, ndir, maxDist, delta, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
            find_ray_hits_from_surface_list(gOmmCeils(0, cx, cz)->data, gOmmCeils(0, cx, cz)->count, orig, ndir, maxDist, delta, hits, flags & RAYCAST_FLAG_NO_CAM_COL);
        }
    }
}

s32 find_collisions_on_ray(Vec3f orig, Vec3f dir, RayCollisionData* hits, f32 delta, u32 flags) {
    hits->count = 0;
    f32 maxDist = vec3f_length(dir);
    if (!maxDist) {
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
    f32 cx = (orig[0] + LEVEL_BOUNDS) / CELL_SIZE;
    f32 cz = (orig[2] + LEVEL_BOUNDS) / CELL_SIZE;

    // DDA
    for (s32 i = 0; i <= (s32) steps; ++i, cx += dx, cz += dz) {
        find_ray_hits_on_cell(cx, cz, orig, ndir, maxDist, delta, hits, flags);
    }
    omm_array_clear(sCheckedSurfaces);
    return hits->count;
}

void find_surface_on_ray(Vec3f orig, Vec3f dir, struct Surface **hitSurface, Vec3f hitPos BETTER_CAM_RAYCAST_ARGS) {
    RayCollisionData hits;
    if (find_collisions_on_ray(orig, dir, &hits, 1.f, gCheckingSurfaceCollisionsForCamera ? RAYCAST_FLAGS_CAMERA : RAYCAST_FLAGS_SURFACES)) {
        *hitSurface = hits.hits[0].surf;
        vec3f_copy(hitPos, hits.hits[0].pos);
    } else {
        *hitSurface = NULL;
        vec3f_sum(hitPos, orig, dir);
    }
}

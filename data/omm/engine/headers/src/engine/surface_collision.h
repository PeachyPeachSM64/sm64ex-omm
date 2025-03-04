#ifndef SURFACE_COLLISION_H
#define SURFACE_COLLISION_H

#include "types.h"

#define MAX_REFERENCED_WALLS (4) // DO NOT INCREASE THIS VALUE, THIS COULD CAUSE A BUFFER OVERFLOW IN collide_with_walls()
#define MAX_RAYCAST_COL_HITS (16)
#define MAX_COLLISION_RADIUS (200)

struct WallCollisionData {
    f32 x, y, z;
    f32 offsetY;
    f32 radius;
    s16 unk14;
    s16 numWalls;
    struct Surface *walls[MAX_REFERENCED_WALLS];
};

struct FloorGeometry {
    f32 unused[4];
    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 originOffset;
};

typedef struct {
    Vec3f pos;
    f32 dist; // from focus to hit pos
    f32 ratio; // dist / maxDist
    struct Surface *surf;
} RayHit;

typedef struct {
    RayHit hits[MAX_RAYCAST_COL_HITS];
    s32 count;
} RayCollisionData;

#define RAYCAST_FLAG_NO_CAM_COL (1 << 0)
#define RAYCAST_FLAG_FLOORS     (1 << 1)
#define RAYCAST_FLAG_CEILS      (1 << 2)
#define RAYCAST_FLAG_WALLS      (1 << 3)
#define RAYCAST_FLAGS_SURFACES  (RAYCAST_FLAG_FLOORS | RAYCAST_FLAG_CEILS | RAYCAST_FLAG_WALLS)
#define RAYCAST_FLAGS_CAMERA    (RAYCAST_FLAG_NO_CAM_COL | RAYCAST_FLAG_FLOORS | RAYCAST_FLAG_CEILS | RAYCAST_FLAG_WALLS)

f32 get_surface_height_at_pos(f32 x, f32 z, struct Surface *surf);
bool recompute_surface_parameters(struct Surface *surf);

s32 find_wall_collisions(struct WallCollisionData *data);
s32 f32_find_wall_collision(f32 *x, f32 *y, f32 *z, f32 offsetY, f32 radius);

f32 find_ceil(f32 x, f32 y, f32 z, struct Surface **pCeil);
f32 vec3f_find_ceil(Vec3f pos, f32 height, struct Surface **ceil);

f32 find_floor_height_and_data(f32 x, f32 y, f32 z, struct FloorGeometry **floorGeo);
f32 find_floor_height(f32 x, f32 y, f32 z);
f32 find_room_floor(f32 x, f32 y, f32 z, struct Surface **pFloor);
f32 find_floor(f32 x, f32 y, f32 z, struct Surface **pFloor);
s32 floor_type_exists_in_current_cell(f32 x, f32 z, s16 type, u32 dynamic);
struct Surface *get_pseudo_floor_at_pos(f32 x, f32 y, f32 z);

f32 find_water_level_and_floor(f32 x, f32 z, struct Surface **pFloor);
f32 find_water_level(f32 x, f32 z);
f32 find_poison_gas_level(f32 x, f32 z);

s32 find_collisions_on_ray(Vec3f orig, Vec3f dir, RayCollisionData *hits, f32 surfaceScale, u32 flags);
bool find_first_hit_on_ray(Vec3f orig, Vec3f dir, Vec3f hitPos, f32 offset, f32 surfaceScale, u32 flags);
void find_surface_on_ray(Vec3f orig, Vec3f dir, struct Surface **hitSurface, Vec3f hitPos BETTER_CAM_RAYCAST_ARGS);

bool surface_intersects_cylinder(struct Surface *surf, Vec3f pos, f32 radius, f32 height, f32 downOffset);

#endif

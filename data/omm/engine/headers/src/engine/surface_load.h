#ifndef SURFACE_LOAD_H
#define SURFACE_LOAD_H

#include "data/omm/system/omm_memory.h"

#define LEVEL_BOUNDS 0x8000  // [-32768, +32767], Vanilla is [-8192, +8191]
#define CELL_SIZE    0x400   // 1024, Vanilla is 1024
#define CELL_COUNT   (2 * (LEVEL_BOUNDS / CELL_SIZE))
#define CELL_BITS    (CELL_COUNT - 1)

#define SURF_FLOOR   0
#define SURF_CEIL    1
#define SURF_WALL    2
#define SURF_TYPE(y) (((y) > +0.01f) ? SURF_FLOOR : (((y) < -0.01f) ? SURF_CEIL : SURF_WALL))

typedef struct { OmmArray data; s32 count; } OmmSurfaceArray;
typedef OmmSurfaceArray OmmSurfaceHashMap[CELL_COUNT][CELL_COUNT][3];
extern OmmSurfaceHashMap gOmmSurfaces[2];

#define gOmmFloors(dyn, cx, cz) (&gOmmSurfaces[dyn][cz][cx][SURF_FLOOR])
#define gOmmCeils(dyn, cx, cz)  (&gOmmSurfaces[dyn][cz][cx][SURF_CEIL])
#define gOmmWalls(dyn, cx, cz)  (&gOmmSurfaces[dyn][cz][cx][SURF_WALL])

u32 get_area_terrain_size(s16 *data);
void load_area_terrain(s16 index, s16 *data, s8 *surfaceRooms, s16 *macroObjects);
void clear_dynamic_surfaces(void);
void load_object_collision_model(void);

#endif

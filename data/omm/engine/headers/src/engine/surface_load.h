#ifndef SURFACE_LOAD_H
#define SURFACE_LOAD_H

#include "types.h"
#include "engine/extended_bounds.h"

u32 get_area_terrain_size(s16 *data);
void load_area_terrain(s16 index, s16 *data, s8 *surfaceRooms, s16 *macroObjects);
void clear_dynamic_surfaces(void);
void load_object_collision_model(void);
s32 get_index_from_surface(struct Surface *s);
struct Surface *get_surface_from_index(s32 index);

#endif

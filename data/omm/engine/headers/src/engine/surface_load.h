#ifndef SURFACE_LOAD_H
#define SURFACE_LOAD_H

#include "types.h"
#include "engine/extended_bounds.h"

struct SurfaceData {
    bool dynamic;
    union {
        struct Surface *surface;
        struct {
            struct Object *object;
            const BehaviorScript *behavior;
            const void *collision;
            s32 index;
            Vec3f pos;
            Vec3s angle;
            Vec3f scale;
        };
    };
};

u32 get_area_terrain_size(s16 *data);
void load_area_terrain(s16 areaIndex, s16 *data, s8 *surfaceRooms, s16 *macroObjects);
void clear_dynamic_surfaces(void);
void load_object_collision_model(void);
void clear_surface_data(struct SurfaceData *data);
void get_surface_data(struct SurfaceData *data, struct Surface *surf);
struct Surface *get_surface_from_data(const struct SurfaceData *data);
struct Surface *get_next_surface(struct Surface *surf);

#endif

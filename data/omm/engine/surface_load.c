#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"
#include "game/macro_special_objects.h"
#include "surface_static_patches.inl"
#include "surface_object_patches.inl"

struct _Surface {
    struct Surface surf[1];
    struct _Surface *next;
};

// Surfaces hash map and pool
OmmSurfaceHashMap gOmmSurfaces[2];
#if !OMM_GAME_IS_R96X
static
#endif
OmmArray_(struct _Surface *) sOmmLoadedSurfaces = omm_array_zero;

// Collision cache
typedef struct { s16 *start; s16 *verts; s16 count; } Col;
static OmmHMap_(Col *) sOmmCollisionCache = omm_hmap_zero;

//
// Add
//

static void omm_surface_add_to_cell(struct _Surface *surface, bool dynamic, s16 cx, s16 cz) {
    s32 type = SURFACE_CATEGORY(surface->surf->normal.y);
    OmmArray *surfaces = &gOmmSurfaces[dynamic][cz][cx][type].data;
    s32 *count = &gOmmSurfaces[dynamic][cz][cx][type].count;
    surface->surf->flags |= SURFACE_FLAG_X_PROJECTION * (abs_f(surface->surf->normal.x) > 0.707f);
    omm_array_grow(*surfaces, ptr, NULL, *count + 1);
    omm_array_set(*surfaces, ptr, surface, *count);
    (*count)++;
}

static void omm_surface_add(struct _Surface *surface, bool dynamic) {
    s16 x0  = min_3_s(surface->surf->vertex1[0], surface->surf->vertex2[0], surface->surf->vertex3[0]);
    s16 x1  = max_3_s(surface->surf->vertex1[0], surface->surf->vertex2[0], surface->surf->vertex3[0]);
    s16 z0  = min_3_s(surface->surf->vertex1[2], surface->surf->vertex2[2], surface->surf->vertex3[2]);
    s16 z1  = max_3_s(surface->surf->vertex1[2], surface->surf->vertex2[2], surface->surf->vertex3[2]);
    s16 cx0 = clamp_s(((x0 + LEVEL_BOUNDARY_MAX - (CELL_SIZE / 2)) / CELL_SIZE), 0, NUM_CELLS - 1);
    s16 cx1 = clamp_s(((x1 + LEVEL_BOUNDARY_MAX + (CELL_SIZE / 2)) / CELL_SIZE), 0, NUM_CELLS - 1);
    s16 cz0 = clamp_s(((z0 + LEVEL_BOUNDARY_MAX - (CELL_SIZE / 2)) / CELL_SIZE), 0, NUM_CELLS - 1);
    s16 cz1 = clamp_s(((z1 + LEVEL_BOUNDARY_MAX + (CELL_SIZE / 2)) / CELL_SIZE), 0, NUM_CELLS - 1);
    for (s16 cz = cz0; cz <= cz1; ++cz) {
        for (s16 cx = cx0; cx <= cx1; ++cx) {
            omm_surface_add_to_cell(surface, dynamic, cx, cz);
        }
    }
}

#if OMM_GAME_IS_R96X

void omm_surface_add_all_surfaces_again() {

    // Clear surface arrays
    OmmSurfaceArray *surfaceArray = (OmmSurfaceArray *) gOmmSurfaces;
    for (s32 i = 0; i != OMM_NUM_SURFACE_ARRAYS; ++i, surfaceArray++) {
        omm_array_delete(surfaceArray->data);
        surfaceArray->count = 0;
    }

    // Add all surfaces again
    for (s32 i = 0; i != gSurfacesAllocated; ++i) {
        struct _Surface *surface = omm_array_get(sOmmLoadedSurfaces, ptr, i);
        omm_surface_add(surface, surface->surf->object != NULL);
    }
}

#endif

//
// Read
//

static OmmArray sOmmCollisionJumps = omm_array_zero;
void omm_surface_register_collision_jump(s16 index, s16 *col) {
    omm_array_grow(sOmmCollisionJumps, ptr, NULL, index + 1);
    omm_array_set(sOmmCollisionJumps, ptr, col, index);
}

static bool check_vertex_in_range(struct _Surface *s, s16 y) {
    return s->surf->lowerY < y && y < s->surf->upperY;
}

static bool fix_vertex(Vec3s u, Vec3s v, s64 r2) {
    s64 dx = (s64) u[0] - (s64) v[0];
    s64 dy = (s64) u[1] - (s64) v[1];
    s64 dz = (s64) u[2] - (s64) v[2];
    s64 l2 = sqr(dx) + sqr(dy) + sqr(dz); // Cast to s64 is needed to avoid integer overflow with the sqr
    if (l2 > 0 && l2 < r2) {
        vec3s_copy(v, u);
        return true;
    }
    return false;
}

static struct _Surface *omm_surface_create(s16 *vBuffer, s16 vCount, s16 *vTranslation, s16 v1, s16 v2, s16 v3) {

    // Check vertex indices
    if (v1 < 0 || v1 >= vCount) return NULL;
    if (v2 < 0 || v2 >= vCount) return NULL;
    if (v3 < 0 || v3 >= vCount) return NULL;

    // Compute vertices and normal
    f32 x1 = vBuffer[4 * v1 + 1] + vTranslation[0];
    f32 y1 = vBuffer[4 * v1 + 2] + vTranslation[1];
    f32 z1 = vBuffer[4 * v1 + 3] + vTranslation[2];
    f32 x2 = vBuffer[4 * v2 + 1] + vTranslation[0];
    f32 y2 = vBuffer[4 * v2 + 2] + vTranslation[1];
    f32 z2 = vBuffer[4 * v2 + 3] + vTranslation[2];
    f32 x3 = vBuffer[4 * v3 + 1] + vTranslation[0];
    f32 y3 = vBuffer[4 * v3 + 2] + vTranslation[1];
    f32 z3 = vBuffer[4 * v3 + 3] + vTranslation[2];
    f32 nx = (y2 - y1) * (z3 - z2) - (z2 - z1) * (y3 - y2);
    f32 ny = (z2 - z1) * (x3 - x2) - (x2 - x1) * (z3 - z2);
    f32 nz = (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);
    f32 mag = sqrtf(sqr_f(nx) + sqr_f(ny) + sqr_f(nz));
    if (mag < 0.1f) return NULL;

    // Create surface
    omm_array_grow(sOmmLoadedSurfaces, ptr, mem_new(struct _Surface, 1), gSurfacesAllocated + 1);
    struct _Surface *surface    = omm_array_get(sOmmLoadedSurfaces, ptr, gSurfacesAllocated++);
    surface->surf->type         = 0;
    surface->surf->force        = 0;
    surface->surf->flags        = 0;
    surface->surf->room         = 0;
    surface->surf->lowerY       = min_3_s(y1, y2, y3) - 5;
    surface->surf->upperY       = max_3_s(y1, y2, y3) + 5;
    surface->surf->vertex1[0]   = x1;
    surface->surf->vertex1[1]   = y1;
    surface->surf->vertex1[2]   = z1;
    surface->surf->vertex2[0]   = x2;
    surface->surf->vertex2[1]   = y2;
    surface->surf->vertex2[2]   = z2;
    surface->surf->vertex3[0]   = x3;
    surface->surf->vertex3[1]   = y3;
    surface->surf->vertex3[2]   = z3;
    surface->surf->normal.x     = nx / mag;
    surface->surf->normal.y     = ny / mag;
    surface->surf->normal.z     = nz / mag;
    surface->surf->originOffset = -(nx * x1 + ny * y1 + nz * z1) / mag;
    surface->surf->object       = NULL;
    surface->next               = NULL;

    // Try to glue nearest vertices together to fix holes due to float to int conversion and vertex misalignments
    if (OMM_MOVESET_ODYSSEY && !gNumStaticSurfaces) {
        u8 fixedVertices = 0;
        for (s32 i = 0; i != gSurfacesAllocated - 1; ++i) {
            struct _Surface *s = omm_array_get(sOmmLoadedSurfaces, ptr, i);
            if (!(fixedVertices & (1 << 0)) && check_vertex_in_range(s, surface->surf->vertex1[1])) {
                fixedVertices |= (fix_vertex(s->surf->vertex1, surface->surf->vertex1, 10) << 0);
                fixedVertices |= (fix_vertex(s->surf->vertex2, surface->surf->vertex1, 10) << 0);
                fixedVertices |= (fix_vertex(s->surf->vertex3, surface->surf->vertex1, 10) << 0);
            }
            if (!(fixedVertices & (1 << 1)) && check_vertex_in_range(s, surface->surf->vertex2[1])) {
                fixedVertices |= (fix_vertex(s->surf->vertex1, surface->surf->vertex2, 10) << 1);
                fixedVertices |= (fix_vertex(s->surf->vertex2, surface->surf->vertex2, 10) << 1);
                fixedVertices |= (fix_vertex(s->surf->vertex3, surface->surf->vertex2, 10) << 1);
            }
            if (!(fixedVertices & (1 << 2)) && check_vertex_in_range(s, surface->surf->vertex3[1])) {
                fixedVertices |= (fix_vertex(s->surf->vertex1, surface->surf->vertex3, 10) << 2);
                fixedVertices |= (fix_vertex(s->surf->vertex2, surface->surf->vertex3, 10) << 2);
                fixedVertices |= (fix_vertex(s->surf->vertex3, surface->surf->vertex3, 10) << 2);
            }
        }

        // Remove invalid surfaces
        if (fixedVertices && !recompute_surface_parameters(surface->surf)) {
            gSurfacesAllocated--;
            return NULL;
        }
    }
    return surface;
}

static bool omm_surface_has_force(s16 surfaceType) {
    return surfaceType == SURFACE_0004 ||
           surfaceType == SURFACE_FLOWING_WATER ||
           surfaceType == SURFACE_HORIZONTAL_WIND ||
           surfaceType == SURFACE_MOVING_QUICKSAND ||
           surfaceType == SURFACE_DEEP_MOVING_QUICKSAND ||
           surfaceType == SURFACE_SHALLOW_MOVING_QUICKSAND ||
           surfaceType == SURFACE_INSTANT_MOVING_QUICKSAND;
}

static bool omm_surface_has_no_cam_collision(s16 surfaceType) {
    return surfaceType == SURFACE_SWITCH ||
           surfaceType == SURFACE_NO_CAM_COLLISION ||
           surfaceType == SURFACE_NO_CAM_COLLISION_77 ||
           surfaceType == SURFACE_NO_CAM_COL_VERY_SLIPPERY;
}

static void omm_surface_process_data(s16 areaIndex, s16 *data, struct Object *o, void *params) {
    s16 vPool[0x20000]; // 0x8000 * 4
    s16 *vBuffer = NULL;
    s16 vCount = 0;
    Vec3s vTranslation = { 0, 0, 0 };
    struct Surface properties;
    bool transformed = false;

    // Optimization: for objects (dynamic surfaces), don't process the vertex data again, start directly with triangles
    if (o && params) {
        Col *col = (Col *) params;
        vBuffer = col->verts;
        vCount = col->count;
        data = col->start;
    }

    // Process every command, until a COL_CMD_END is found
    for (;;) {
        s16 cmd = *data;
        switch (cmd) {

            // Initialize collision data
            case COL_CMD_INIT: {
                vBuffer = NULL;
                vCount = 0;
                properties.type = SURFACE_DEFAULT;
                properties.flags = 0;
                properties.force = false;
                transformed = false;
            } break;

            // Initialize vertex buffer
            case COL_CMD_VERTEX_INIT: {
                vBuffer = &data[2];
                vCount = 0;
                transformed = false;
            } break;

            // Increment vertex counter
            case COL_CMD_VERTEX: {
                vCount++;
            } break;

            // Initialize surface data
            case COL_CMD_TRI_INIT: {
                properties.type = data[1];
                properties.flags = (SURFACE_FLAG_DYNAMIC * (o != NULL)) | (SURFACE_FLAG_NO_CAM_COLLISION * omm_surface_has_no_cam_collision(properties.type));
                properties.force = omm_surface_has_force(properties.type);
                if (o && !transformed) {

                    // Store the vertex buffer in the look-up table
                    if (!params) {
                        Col *col = mem_new(Col, 1);
                        col->start = data;
                        col->count = vCount;
                        col->verts = mem_dup(vBuffer, vCount * 4 * sizeof(s16));
                        omm_hmap_insert(sOmmCollisionCache, (uintptr_t) o->collisionData, col);
                    }

                    // Compute the transform matrix
                    if (!o->oThrowMatrix) {
                        o->oThrowMatrix = &o->transform;
                        obj_build_transform_from_pos_and_angle(o, O_POS_INDEX, O_FACE_ANGLE_INDEX);
                    }
                    Mat4 m; obj_apply_scale_to_matrix(o, m, o->transform);

                    // Transform and store in another buffer
                    for (s16 i = 0, *vSrc = vBuffer + 1, *vDst = vPool + 1; i != vCount; ++i, vSrc += 4, vDst += 4) {
                        Vec3f vtx; vec3s_to_vec3f(vtx, vSrc);
                        Mat4 mat; mtxf_translate(mat, vtx);
                        mtxf_mul(mat, mat, m);
                        vec3f_to_vec3s(vDst, mat[3]);
                    }
                    vBuffer = vPool;
                    transformed = true;
                }
            } break;

            // Create surface
            case COL_CMD_TRI:
            case COL_CMD_TRI_SPECIAL: {

                // Room
                s8 room = 0;
                if (o) {
                    room = 5 * (o->behavior == bhvDddWarp);
                } else if (params) {
                    s8 **surfaceRooms = (s8 **) params;
                    room = *(*surfaceRooms);
                    *surfaceRooms += 1;
                }

                // Surface
                s16 v1 = data[1];
                s16 v2 = data[2];
                s16 v3 = data[3];
                struct _Surface *surface = omm_surface_create(vBuffer, vCount, vTranslation, v1, v2, v3);
                if (surface) {
                    surface->surf->object = o;
                    surface->surf->room = room;
                    surface->surf->type = properties.type;
                    surface->surf->flags = properties.flags;
                    surface->surf->force = (properties.force ? data[4] : 0);
                    omm_surface_add(surface, o != NULL);
                    if (o) {
                        surface->next = o->oSurfaces;
                        o->oSurfaces = surface;
                    }
                }
            } break;

            // Stop creating surfaces
            case COL_CMD_TRI_STOP: {
                properties.type = SURFACE_DEFAULT;
                properties.flags = 0;
                properties.force = false;
            } break;

            // End collision data
            case COL_CMD_END: {
                vec3s_set(vTranslation, 0, 0, 0);
                return;
            } break;

            // Spawn special objects
            case COL_CMD_SPECIAL_INIT: {
                data += 1;
                spawn_special_objects(areaIndex, &data);
            } break;

            // Initialize environment regions
            case COL_CMD_WATER_BOX_INIT: {
                gEnvironmentRegions = &data[1];
                s32 numRegions = data[1];
                for (s32 i = 0; i < numRegions; ++i, data += 6) {
                    if (OMM_LIKELY(i < 20)) {
                        gEnvironmentLevels[i] = data[7];
                    }
                }
            } break;

            // Translate all vertices by a fixed amount (only works with static surfaces)
            case COL_CMD_TRANSLATE: {
                if (!o) {
                    vTranslation[0] = data[1];
                    vTranslation[1] = data[2];
                    vTranslation[2] = data[3];
                }
            } break;

            // Process another collision script (only works with static surfaces)
            case COL_CMD_JUMP: {
                if (!o) {
                    s16 index = data[1];
                    omm_surface_process_data(areaIndex, (s16 *) omm_array_get(sOmmCollisionJumps, ptr, index), NULL, params);
                }
            } break;
        }

        // Advance data pointer
        static const s32 COL_CMD_SIZE[] = { 1, 2, 4, 3, 4, 5, 1, 1, 0, 2, 4, 2 };
        data += COL_CMD_SIZE[cmd];
    }
}

//
// Terrain
//

u32 get_area_terrain_size(s16 *data) {
    static const s16 COL_CMD_END_BYTES[] = { COL_CMD_END_BYTE_SEQUENCE };
    for (s16 *head = data;; head++) {
        if (*head == COL_CMD_END) {
            if (mem_eq(head + 1, COL_CMD_END_BYTES, array_length(COL_CMD_END_BYTES))) {
                return (u32) ((head - data) + 1 + array_length(COL_CMD_END_BYTES));
            }
        }
    }
    return 0;
}

void load_area_terrain(s16 areaIndex, s16 *data, s8 *surfaceRooms, s16 *macroObjects) {
    gEnvironmentRegions = NULL;
    gSurfacesAllocated = 0;
    gNumStaticSurfaces = 0;
    gMarioState->floor = NULL;
    gMarioState->ceil = NULL;
    gMarioState->wall = NULL;

#if OMM_GAME_IS_SM74

    // Due to the nature of SM74 modes, areas within a same level are treated as different levels
    omm_execute_routines(OMM_ROUTINE_TYPE_LEVEL_ENTRY);

#else

    // Treat any Castle area change as a level entry
    if (gCurrCourseNum == COURSE_NONE) {
        gOmmGlobals->cameraNoInit = true;
        omm_execute_routines(OMM_ROUTINE_TYPE_LEVEL_ENTRY);
        gOmmGlobals->cameraNoInit = false;
    }

#endif

    // Clear loaded surfaces
    omm_array_for_each(sOmmLoadedSurfaces, p_surface) {
        struct _Surface *surface = p_surface->as_ptr;
        mem_del(surface);
    }
    omm_array_delete(sOmmLoadedSurfaces);

    // Clear surface arrays
    OmmSurfaceArray *surfaceArray = (OmmSurfaceArray *) gOmmSurfaces;
    for (s32 i = 0; i != OMM_NUM_SURFACE_ARRAYS; ++i, surfaceArray++) {
        omm_array_delete(surfaceArray->data);
        surfaceArray->count = 0;
    }

    // Clear collision cache
    omm_hmap_for_each(sOmmCollisionCache, _, v) {
        Col *col = (Col *) *v;
        mem_del(col->verts);
        mem_del(col);
    }
    omm_hmap_delete(sOmmCollisionCache);

    // Load terrain
    omm_surface_process_data(areaIndex, data, NULL, surfaceRooms ? &surfaceRooms : NULL);
    gNumStaticSurfaces = gSurfacesAllocated;

    // Load static patches
    for (u32 i = 0; i != array_length(sOmmSurfaceStaticPatches); ++i) {
        if (sOmmSurfaceStaticPatches[i].levelAreaIndex == LEVEL_AREA_INDEX(gCurrLevelNum, areaIndex)) {
            omm_surface_process_data(areaIndex, (s16 *) sOmmSurfaceStaticPatches[i].collision, NULL, NULL);
            gNumStaticSurfaces = gSurfacesAllocated;
        }
    }

    // Spawn macro objects
    if (macroObjects && *macroObjects != -1) {
        if (0 <= *macroObjects && *macroObjects < 30) {
            spawn_macro_objects_hardcoded(areaIndex, macroObjects);
        } else {
            spawn_macro_objects(areaIndex, macroObjects);
        }
    }
}

void clear_dynamic_surfaces() {
    if (!(gTimeStopState & TIME_STOP_ACTIVE)) {
        for (s32 cz = 0; cz != NUM_CELLS; ++cz)
        for (s32 cx = 0; cx != NUM_CELLS; ++cx) {
            gOmmFloors(1, cx, cz)->count = 0;
            gOmmCeils(1, cx, cz)->count = 0;
            gOmmWalls(1, cx, cz)->count = 0;
        }
        gSurfacesAllocated = gNumStaticSurfaces;
        for_each_(struct Object, obj, gObjectPool, OBJECT_POOL_CAPACITY) {
            obj->oSurfaces = NULL;
        }
    }
}

//
// Object collision model
//

// Fix vanilla's weird collisions
static void omm_fix_object_collision_data(struct Object *o) {
    for (u32 i = 0; i != array_length(sOmmSurfaceObjectPatches); ++i) {
        if (o->collisionData == sOmmSurfaceObjectPatches[i][OMM_MOVESET_CLASSIC]) {
            o->collisionData = (void *) sOmmSurfaceObjectPatches[i][OMM_MOVESET_ODYSSEY];
            return;
        }
    }
}

void obj_load_collision_model(struct Object *o) {
    if (o && o->collisionData) {
        omm_fix_object_collision_data(o);

        // Load and transform object collision
        if (!(gTimeStopState & TIME_STOP_ACTIVE) && !(o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)) {
            s32 i = omm_hmap_find(sOmmCollisionCache, (uintptr_t) o->collisionData);
            if (i != -1) {
                omm_surface_process_data(gCurrAreaIndex, (s16 *) o->collisionData, o, omm_hmap_get(sOmmCollisionCache, void *, i));
            } else {
                omm_surface_process_data(gCurrAreaIndex, (s16 *) o->collisionData, o, NULL);
            }
        }

        // Update distances
        o->oCollisionDistance = LEVEL_BOUNDARY_MAX;
        o->oDistanceToMario = dist_between_objects(o, gMarioObject);
        o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    }
}

void load_object_collision_model() {
    obj_load_collision_model(gCurrentObject);
}

static s32 get_surface_index(struct Object *obj, struct Surface *surf) {
    s32 index = 0;
    for (struct _Surface *surface = obj->oSurfaces; surface != NULL; surface = surface->next, ++index) {
        if (surf == (struct Surface *) surface) {
            return index;
        }
    }
    return -1;
}

static struct Surface *get_surface_from_index(struct Object *obj, s32 index) {
    if (index < 0) {
        return NULL;
    }
    for (struct _Surface *surface = obj->oSurfaces; surface != NULL; surface = surface->next, --index) {
        if (index == 0) {
            return (struct Surface *) surface;
        }
    }
    return NULL;
}

void clear_surface_data(struct SurfaceData *data) {
    mem_zero(data, sizeof(struct SurfaceData));
}

void get_surface_data(struct SurfaceData *data, struct Surface *surf) {
    if (surf->object) {
        data->dynamic = true;
        data->object = surf->object;
        data->behavior = surf->object->behavior;
        data->collision = surf->object->collisionData;
        data->index = get_surface_index(surf->object, surf);
        obj_pos_as_vec3f(surf->object, data->pos);
        obj_angle_as_vec3s(surf->object, data->angle);
        obj_scale_as_vec3f(surf->object, data->scale);
    } else {
        data->dynamic = false;
        data->surface = surf;
    }
}

struct Surface *get_surface_from_data(const struct SurfaceData *data) {
    if (data->dynamic) {
        if (data->object->activeFlags == ACTIVE_FLAG_DEACTIVATED) {
            return NULL;
        }
        if (data->object->behavior != data->behavior) {
            return NULL;
        }
        if (data->object->collisionData != data->collision) {
            return NULL;
        }
        return get_surface_from_index(data->object, data->index);
    }
    return data->surface;
}

struct Surface *get_next_surface(struct Surface *surf) {
    if (surf) {
        return (struct Surface *) ((struct _Surface *) surf)->next;
    }
    return NULL;
}

#if OMM_CODE_DEBUG

//
// Surface display
//

static Gfx *omm_debug_surfaces_get_display_list() {
    static Lights1 sOmmDebugRenderSurfacesLightFloor1 = gdSPDefLights1(0x00, 0x80, 0xFF, 0x00, 0x40, 0x80, 0x28, 0x28, 0x28);
    static Lights1 sOmmDebugRenderSurfacesLightFloor2 = gdSPDefLights1(0x00, 0x40, 0x80, 0x00, 0x20, 0x40, 0x28, 0x28, 0x28);
    static Lights1 sOmmDebugRenderSurfacesLightCeil1  = gdSPDefLights1(0xFF, 0x00, 0x00, 0x80, 0x00, 0x00, 0x28, 0x28, 0x28);
    static Lights1 sOmmDebugRenderSurfacesLightCeil2  = gdSPDefLights1(0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x28, 0x28, 0x28);
    static Lights1 sOmmDebugRenderSurfacesLightWallX1 = gdSPDefLights1(0x00, 0xA0, 0x00, 0x00, 0x50, 0x00, 0x28, 0x28, 0x28);
    static Lights1 sOmmDebugRenderSurfacesLightWallX2 = gdSPDefLights1(0x00, 0x50, 0x00, 0x00, 0x28, 0x00, 0x28, 0x28, 0x28);
    static Lights1 sOmmDebugRenderSurfacesLightWallZ1 = gdSPDefLights1(0xC0, 0xC0, 0x00, 0x60, 0x60, 0x00, 0x28, 0x28, 0x28);
    static Lights1 sOmmDebugRenderSurfacesLightWallZ2 = gdSPDefLights1(0x60, 0x60, 0x00, 0x30, 0x30, 0x00, 0x28, 0x28, 0x28);
    static Gfx    *sOmmDebugRenderSurfacesDisplayList = NULL;
    static Vtx    *sOmmDebugRenderSurfacesVertices    = NULL;

    // Init
    mem_del(sOmmDebugRenderSurfacesDisplayList);
    mem_del(sOmmDebugRenderSurfacesVertices);
    sOmmDebugRenderSurfacesDisplayList = mem_new(Gfx, 4 + gSurfacesAllocated * 10);
    sOmmDebugRenderSurfacesVertices = mem_new(Vtx, gSurfacesAllocated * 6);
    Gfx *gfx = sOmmDebugRenderSurfacesDisplayList;
    Vtx *vtx = sOmmDebugRenderSurfacesVertices;

    // Surfaces
    gSPClearGeometryMode(gfx++, G_CULL_BOTH);
    gSPSetGeometryMode(gfx++, G_CULL_BACK | G_LIGHTING);
    gDPSetCombineLERP(gfx++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    for (s32 i = 0; i != gSurfacesAllocated; ++i, vtx += 6) {
        struct Surface *surf = omm_array_get(sOmmLoadedSurfaces, ptr, i);

        // Vertices
        vtx[0] = vtx[3] = (Vtx) { { { surf->vertex1[0], surf->vertex1[1], surf->vertex1[2] }, 0, { 0, 0 }, { surf->normal.x * 127.f, surf->normal.y * 127.f, surf->normal.z * 127.f, 0xFF } } };
        vtx[1] = vtx[4] = (Vtx) { { { surf->vertex2[0], surf->vertex2[1], surf->vertex2[2] }, 0, { 0, 0 }, { surf->normal.x * 127.f, surf->normal.y * 127.f, surf->normal.z * 127.f, 0xFF } } };
        vtx[2] = vtx[5] = (Vtx) { { { surf->vertex3[0], surf->vertex3[1], surf->vertex3[2] }, 0, { 0, 0 }, { surf->normal.x * 127.f, surf->normal.y * 127.f, surf->normal.z * 127.f, 0xFF } } };
        Vec3f center = {
            (vtx[0].v.ob[0] + vtx[1].v.ob[0] + vtx[2].v.ob[0]) / 3,
            (vtx[0].v.ob[1] + vtx[1].v.ob[1] + vtx[2].v.ob[1]) / 3,
            (vtx[0].v.ob[2] + vtx[1].v.ob[2] + vtx[2].v.ob[2]) / 3,
        };
        for (s32 k = 0; k != 3; ++k) {
            Vec3f dv = {
                vtx[k].v.ob[0] - center[0],
                vtx[k].v.ob[1] - center[1],
                vtx[k].v.ob[2] - center[2],
            };
            f32 border = 5.f;
            f32 length = vec3f_length(dv);
            f32 mult = max_f(0.5f, 1.f - (border / length));
            vtx[k].v.ob[0] = center[0] + mult * dv[0];
            vtx[k].v.ob[1] = center[1] + mult * dv[1];
            vtx[k].v.ob[2] = center[2] + mult * dv[2];
        }

        // Floor
        if (surf->normal.y > +0.01f) {
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightFloor1.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightFloor1.a, 2);
            gSPVertex(gfx++, vtx, 3, 0);
            gSP1Triangle(gfx++, 0, 1, 2, 0);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightFloor2.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightFloor2.a, 2);
            gSPVertex(gfx++, vtx, 6, 0);
            gSP2Triangles(gfx++, 3, 4, 0, 0, 0, 4, 1, 0);
            gSP2Triangles(gfx++, 4, 5, 1, 0, 1, 5, 2, 0);
            gSP2Triangles(gfx++, 5, 3, 2, 0, 2, 3, 0, 0);
        }

        // Ceiling
        else if (surf->normal.y < -0.01f) {
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightCeil1.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightCeil1.a, 2);
            gSPVertex(gfx++, vtx, 3, 0);
            gSP1Triangle(gfx++, 0, 1, 2, 0);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightCeil2.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightCeil2.a, 2);
            gSPVertex(gfx++, vtx, 6, 0);
            gSP2Triangles(gfx++, 3, 4, 0, 0, 0, 4, 1, 0);
            gSP2Triangles(gfx++, 4, 5, 1, 0, 1, 5, 2, 0);
            gSP2Triangles(gfx++, 5, 3, 2, 0, 2, 3, 0, 0);
        }

        // Wall (X-proj)
        else if (surf->flags & SURFACE_FLAG_X_PROJECTION) {
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWallX1.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWallX1.a, 2);
            gSPVertex(gfx++, vtx, 3, 0);
            gSP1Triangle(gfx++, 0, 1, 2, 0);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWallX2.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWallX2.a, 2);
            gSPVertex(gfx++, vtx, 6, 0);
            gSP2Triangles(gfx++, 3, 4, 0, 0, 0, 4, 1, 0);
            gSP2Triangles(gfx++, 4, 5, 1, 0, 1, 5, 2, 0);
            gSP2Triangles(gfx++, 5, 3, 2, 0, 2, 3, 0, 0);
        }

        // Wall (Z-proj)
        else {
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWallZ1.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWallZ1.a, 2);
            gSPVertex(gfx++, vtx, 3, 0);
            gSP1Triangle(gfx++, 0, 1, 2, 0);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWallZ2.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWallZ2.a, 2);
            gSPVertex(gfx++, vtx, 6, 0);
            gSP2Triangles(gfx++, 3, 4, 0, 0, 0, 4, 1, 0);
            gSP2Triangles(gfx++, 4, 5, 1, 0, 1, 5, 2, 0);
            gSP2Triangles(gfx++, 5, 3, 2, 0, 2, 3, 0, 0);
        }
    }
    gSPEndDisplayList(gfx);
    return sOmmDebugRenderSurfacesDisplayList;
}

OMM_ROUTINE_PRE_RENDER(omm_debug_surfaces_update) {
    if (OMM_LIKELY(gObjectLists && gMarioObject && !omm_is_main_menu() && !omm_is_ending_cutscene() && !omm_is_ending_cake_screen())) {
        static Gfx sOmmDebugRenderSurfacesGfx[] = { gsSPDisplayList(NULL), gsSPEndDisplayList() };
        static const GeoLayout sOmmDebugRenderSurfacesGeo[] = { GEO_NODE_START(), GEO_OPEN_NODE(), GEO_DISPLAY_LIST(LAYER_OPAQUE, sOmmDebugRenderSurfacesGfx), GEO_CLOSE_NODE(), GEO_END() };
        static const BehaviorScript sOmmDebugRenderSurfacesBhv[] = { OBJ_TYPE_UNIMPORTANT, BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE), BHV_BEGIN_LOOP(), BHV_END_LOOP() };
        struct Object *o = obj_get_first_with_behavior(sOmmDebugRenderSurfacesBhv);
        if (gOmmDebugSurface) {
            if (!o) {
                o = obj_spawn_from_geo(gMarioObject, sOmmDebugRenderSurfacesGeo, sOmmDebugRenderSurfacesBhv);
                obj_set_xyz(o, 0, 0, 0);
                obj_set_angle(o, 0, 0, 0);
                obj_scale(o, 1.f);
                obj_set_always_rendered(o, true);
            }
            sOmmDebugRenderSurfacesGfx->words.w1 = (uintptr_t) omm_debug_surfaces_get_display_list();
        } else if (o) {
            obj_mark_for_deletion(o);
        }
    }
}

void omm_debug_surfaces_print_info(s32 x, s32 y) {

    // Loaded surfaces
    s32 surfacesAllocated = omm_array_count(sOmmLoadedSurfaces);
    s32 surfacesUsed = gSurfacesAllocated;
    omm_debug_text(x, y + 36, "SURF %d %d", surfacesUsed, surfacesAllocated);

    // Surface nodes
    s32 nodesAllocated = 0;
    s32 nodesUsed = 0;
    OmmSurfaceArray *surfaceArray = (OmmSurfaceArray *) gOmmSurfaces;
    for (s32 i = 0; i != OMM_NUM_SURFACE_ARRAYS; ++i, surfaceArray++) {
        nodesAllocated += omm_array_count(surfaceArray->data);
        nodesUsed += surfaceArray->count;
    }
    omm_debug_text(x, y + 18, "NODE %d %d", nodesUsed, nodesAllocated);

    // Collision cache
    s32 collisionsCached = omm_hmap_count(sOmmCollisionCache);
    omm_debug_text(x, y, "COLL %d", collisionsCached);
}

#endif

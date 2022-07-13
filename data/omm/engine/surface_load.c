#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "game/macro_special_objects.h"

// Surfaces hash map and pool
OmmSurfaceHashMap gOmmSurfaces[2];
static OmmArray sOmmLoadedSurfaces = omm_array_zero;

// Collision cache
typedef struct { s16 *start; s16 *verts; s16 count; } Col;
static OmmHMap sOmmCollisionCache = omm_hmap_zero;

//
// Add
//

static void omm_surface_add_to_cell(struct Surface *surface, bool dynamic, s16 cx, s16 cz) {
    s32 type = SURF_TYPE(surface->normal.y);
    OmmArray *surfaces = &gOmmSurfaces[dynamic][cz][cx][type].data;
    s32 *count = &gOmmSurfaces[dynamic][cz][cx][type].count;
    surface->flags |= SURFACE_FLAG_X_PROJECTION * (abs_f(surface->normal.x) > 0.707f);
    omm_array_grow(*surfaces, ptr, NULL, *count + 1);
    omm_array_set(*surfaces, ptr, surface, *count);
    (*count)++;
}

static void omm_surface_add(struct Surface *surface, bool dynamic) {
    s16 x0  = min_3_s(surface->vertex1[0], surface->vertex2[0], surface->vertex3[0]);
    s16 x1  = max_3_s(surface->vertex1[0], surface->vertex2[0], surface->vertex3[0]);
    s16 z0  = min_3_s(surface->vertex1[2], surface->vertex2[2], surface->vertex3[2]);
    s16 z1  = max_3_s(surface->vertex1[2], surface->vertex2[2], surface->vertex3[2]);
    s16 cx0 = clamp_s(((x0 + LEVEL_BOUNDS - (CELL_SIZE / 2)) / CELL_SIZE), 0, CELL_COUNT - 1);
    s16 cx1 = clamp_s(((x1 + LEVEL_BOUNDS + (CELL_SIZE / 2)) / CELL_SIZE), 0, CELL_COUNT - 1);
    s16 cz0 = clamp_s(((z0 + LEVEL_BOUNDS - (CELL_SIZE / 2)) / CELL_SIZE), 0, CELL_COUNT - 1);
    s16 cz1 = clamp_s(((z1 + LEVEL_BOUNDS + (CELL_SIZE / 2)) / CELL_SIZE), 0, CELL_COUNT - 1);
    for (s16 cz = cz0; cz <= cz1; ++cz) {
        for (s16 cx = cx0; cx <= cx1; ++cx) {
            omm_surface_add_to_cell(surface, dynamic, cx, cz);
        }
    }
}

//
// Read
//

static OmmArray sOmmCollisionJumps = omm_array_zero;
void omm_surface_register_collision_jump(s16 index, s16 *col) {
    omm_array_grow(sOmmCollisionJumps, ptr, NULL, index + 1);
    omm_array_set(sOmmCollisionJumps, ptr, col, index);
}

static struct Surface *omm_surface_create(s16 *vBuffer, s16 vCount, s16 *vTranslation, s16 v1, s16 v2, s16 v3) {

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
    omm_array_grow(sOmmLoadedSurfaces, ptr, OMM_MEMNEW(struct Surface, 1), gSurfacesAllocated + 1);
    struct Surface *surface = omm_array_get(sOmmLoadedSurfaces, ptr, gSurfacesAllocated++);
    surface->type           = 0;
    surface->force          = 0;
    surface->flags          = 0;
    surface->room           = 0;
    surface->lowerY         = min_3_s(y1, y2, y3) - 5;
    surface->upperY         = max_3_s(y1, y2, y3) + 5;
    surface->vertex1[0]     = x1;
    surface->vertex1[1]     = y1;
    surface->vertex1[2]     = z1;
    surface->vertex2[0]     = x2;
    surface->vertex2[1]     = y2;
    surface->vertex2[2]     = z2;
    surface->vertex3[0]     = x3;
    surface->vertex3[1]     = y3;
    surface->vertex3[2]     = z3;
    surface->normal.x       = nx / mag;
    surface->normal.y       = ny / mag;
    surface->normal.z       = nz / mag;
    surface->originOffset   = -(nx * x1 + ny * y1 + nz * z1) / mag;
    surface->object         = NULL;
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

    // Process every command, until a CMD_COL_END is found
    for (;;) {
        s16 cmd = *data;
        switch (cmd) {

            // Initialize collision data
            case CMD_COL_INIT: {
                vBuffer = NULL;
                vCount = 0;
                properties.type = SURFACE_DEFAULT;
                properties.flags = 0;
                properties.force = false;
                transformed = false;
            } break;

            // Initialize vertex buffer
            case CMD_COL_VERTEX_INIT: {
                vBuffer = &data[2];
                vCount = 0;
                transformed = false;
            } break;

            // Increment vertex counter
            case CMD_COL_VERTEX: {
                vCount++;
            } break;

            // Initialize surface data
            case CMD_COL_TRI_INIT: {
                properties.type = data[1];
                properties.flags = (SURFACE_FLAG_DYNAMIC * (o != NULL)) | (SURFACE_FLAG_NO_CAM_COLLISION * omm_surface_has_no_cam_collision(properties.type));
                properties.force = omm_surface_has_force(properties.type);
                if (o && !transformed) {

                    // Store the vertex buffer in the look-up table
                    if (!params) {
                        Col *col = OMM_MEMNEW(Col, 1);
                        col->start = data;
                        col->count = vCount;
                        col->verts = OMM_MEMDUP(vBuffer, vCount * 4 * sizeof(s16));
                        omm_hmap_insert(sOmmCollisionCache, (uintptr_t) o->collisionData, col);
                    }

                    // Compute the transform matrix
                    if (o->oThrowMatrix == NULL) {
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
            case CMD_COL_TRI:
            case CMD_COL_TRI_SPECIAL: {

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
                struct Surface *surface = omm_surface_create(vBuffer, vCount, vTranslation, v1, v2, v3);
                if (surface != NULL) {
                    surface->object = o;
                    surface->room = room;
                    surface->type = properties.type;
                    surface->flags = properties.flags;
                    surface->force = (properties.force ? data[4] : 0);
                    omm_surface_add(surface, o != NULL);
                }
            } break;

            // Stop creating surfaces
            case CMD_COL_TRI_STOP: {
                properties.type = SURFACE_DEFAULT;
                properties.flags = 0;
                properties.force = false;
            } break;

            // End collision data
            case CMD_COL_END: {
                vec3s_set(vTranslation, 0, 0, 0);
                return;
            } break;

            // Spawn special objects
            case CMD_COL_SPECIAL_INIT: {
                data += 1;
                spawn_special_objects(areaIndex, &data);
            } break;

            // Initialize environment regions
            case CMD_COL_WATER_BOX_INIT: {
                gEnvironmentRegions = &data[1];
                s32 numRegions = data[1];
                for (s32 i = 0; i < numRegions; ++i, data += 6) {
                    if (OMM_LIKELY(i < 20)) {
                        gEnvironmentLevels[i] = data[7];
                    }
                }
            } break;

            // Translate all vertices by a fixed amount (only works with static surfaces)
            case CMD_COL_TRANSLATE: {
                if (!o) {
                    vTranslation[0] = data[1];
                    vTranslation[1] = data[2];
                    vTranslation[2] = data[3];
                }
            } break;

            // Process another collision script (only works with static surfaces)
            case CMD_COL_JUMP: {
                if (!o) {
                    s16 index = data[1];
                    omm_surface_process_data(areaIndex, (s16 *) omm_array_get(sOmmCollisionJumps, ptr, index), NULL, params);
                }
            } break;
        }

        // Advance data pointer
        static const s32 sCmdColSize[] = { 1, 2, 4, 3, 4, 5, 1, 1, 0, 2, 4, 2 };
        data += sCmdColSize[cmd]; 
    }
}

//
// Terrain
//

u32 get_area_terrain_size(s16 *data) {
    static const s16 sCmdColEndByteSequence[] = { CMD_COL_END_BYTE_SEQUENCE };
    for (s16 *head = data;; head++) {
        if (*head == CMD_COL_END) {
            if (OMM_MEMCMP(head + 1, sCmdColEndByteSequence, OMM_ARRAY_SIZE(sCmdColEndByteSequence))) {
                return (u32) ((head - data) + 1 + OMM_ARRAY_SIZE(sCmdColEndByteSequence));
            }
        }
    }
    return 0;
}

void load_area_terrain(s16 index, s16 *data, s8 *surfaceRooms, s16 *macroObjects) {
    for (s32 cz = 0; cz != CELL_COUNT; ++cz)
    for (s32 cx = 0; cx != CELL_COUNT; ++cx) {
        gOmmFloors(0, cx, cz)->count = 0;
        gOmmCeils(0, cx, cz)->count = 0;
        gOmmWalls(0, cx, cz)->count = 0;
    }
    gEnvironmentRegions = NULL;
    gSurfacesAllocated = 0;

    // Load terrain
    omm_surface_process_data(index, data, NULL, surfaceRooms ? &surfaceRooms : NULL);
    gNumStaticSurfaces = gSurfacesAllocated;

    // Spawn macro objects
    if (macroObjects != NULL && *macroObjects != -1) {
        if (0 <= *macroObjects && *macroObjects < 30) {
            spawn_macro_objects_hardcoded(index, macroObjects);
        } else {
            spawn_macro_objects(index, macroObjects);
        }
    }
}

void clear_dynamic_surfaces() {
    if (!(gTimeStopState & TIME_STOP_ACTIVE)) {
        for (s32 cz = 0; cz != CELL_COUNT; ++cz)
        for (s32 cx = 0; cx != CELL_COUNT; ++cx) {
            gOmmFloors(1, cx, cz)->count = 0;
            gOmmCeils(1, cx, cz)->count = 0;
            gOmmWalls(1, cx, cz)->count = 0;
        }
        gSurfacesAllocated = gNumStaticSurfaces;
    }
}

//
// Object collision model
//

// Fix vanilla's weird collisions
extern const Collision omm_thwomp_collision[];
static void omm_fix_object_collision_data(struct Object *o) {
    static const void *sFixedCollisions[][2] = {
        { thwomp_seg5_collision_0500B7D0, omm_thwomp_collision },
        { thwomp_seg5_collision_0500B92C, omm_thwomp_collision },
    };
    for (s32 i = 0; i != OMM_ARRAY_SIZE(sFixedCollisions); ++i) {
        if (o->collisionData == sFixedCollisions[i][OMM_MOVESET_CLASSIC]) {
            o->collisionData = (void *) sFixedCollisions[i][OMM_MOVESET_ODYSSEY];
            return;
        }
    }
}

// Small optimization to not overload the collision cache (and the memory)
OMM_ROUTINE_LEVEL_ENTRY(omm_clear_collision_buffers) {
    omm_hmap_for_each(sOmmCollisionCache, _, v) {
        Col *col = (Col *) *v;
        if (col) {
            OMM_MEMDEL(col->verts);
            OMM_MEMDEL(col);
        }
    }
    omm_hmap_delete(sOmmCollisionCache);
}

void load_object_collision_model() {
    struct Object *o = gCurrentObject;
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
    o->oCollisionDistance = LEVEL_BOUNDS;
    o->oDistanceToMario = dist_between_objects(o, gMarioObject);
    o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
}

s32 get_object_surface_index(struct Object *o, struct Surface *s) {
    if (o) {
        for (s32 i = 0, j = -1; i != gSurfacesAllocated; ++i) {
            struct Surface *surface = omm_array_get(sOmmLoadedSurfaces, ptr, i);
            if (surface->object == o && j == -1) {
                j = i;
            }
            if (surface == s) {
                return i - j;
            }
        }
    }
    return -1;
}

struct Surface *get_object_surface_from_index(struct Object *o, s32 index) {
    if (o) {
        for (s32 i = 0; i != gSurfacesAllocated; ++i) {
            struct Surface *surface = omm_array_get(sOmmLoadedSurfaces, ptr, i);
            if (surface->object == o) {
                index += i;
                break;
            }
        }
        return omm_array_get(sOmmLoadedSurfaces, ptr, index);
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
    static Lights1 sOmmDebugRenderSurfacesLightWall1  = gdSPDefLights1(0x00, 0xA0, 0x00, 0x00, 0x50, 0x00, 0x28, 0x28, 0x28);
    static Lights1 sOmmDebugRenderSurfacesLightWall2  = gdSPDefLights1(0x00, 0x50, 0x00, 0x00, 0x28, 0x00, 0x28, 0x28, 0x28);
    static Gfx    *sOmmDebugRenderSurfacesDisplayList = NULL;
    static Vtx    *sOmmDebugRenderSurfacesVertices    = NULL;

    // Init
    OMM_MEMDEL(sOmmDebugRenderSurfacesDisplayList);
    OMM_MEMDEL(sOmmDebugRenderSurfacesVertices);
    sOmmDebugRenderSurfacesDisplayList = OMM_MEMNEW(Gfx, 4 + gSurfacesAllocated * 10);
    sOmmDebugRenderSurfacesVertices = OMM_MEMNEW(Vtx, gSurfacesAllocated * 6);
    Gfx *gfx = sOmmDebugRenderSurfacesDisplayList;
    Vtx *vtx = sOmmDebugRenderSurfacesVertices;

    // Surfaces
    gSPClearGeometryMode(gfx++, G_CULL_BOTH);
    gSPSetGeometryMode(gfx++, G_CULL_BACK | G_LIGHTING);
    gDPSetCombineLERP(gfx++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    for (s32 i = 0; i != gSurfacesAllocated; ++i, vtx += 6) {
        struct Surface *surface = omm_array_get(sOmmLoadedSurfaces, ptr, i);

        // Vertices
        vtx[0] = vtx[3] = (Vtx) { { { surface->vertex1[0], surface->vertex1[1], surface->vertex1[2] }, 0, { 0, 0 }, { surface->normal.x * 127.f, surface->normal.y * 127.f, surface->normal.z * 127.f, 0xFF } } };
        vtx[1] = vtx[4] = (Vtx) { { { surface->vertex2[0], surface->vertex2[1], surface->vertex2[2] }, 0, { 0, 0 }, { surface->normal.x * 127.f, surface->normal.y * 127.f, surface->normal.z * 127.f, 0xFF } } };
        vtx[2] = vtx[5] = (Vtx) { { { surface->vertex3[0], surface->vertex3[1], surface->vertex3[2] }, 0, { 0, 0 }, { surface->normal.x * 127.f, surface->normal.y * 127.f, surface->normal.z * 127.f, 0xFF } } };
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
        if (surface->normal.y > +0.01f) {
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
        else if (surface->normal.y < -0.01f) {
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
        
        // Wall
        else {
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWall1.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWall1.a, 2);
            gSPVertex(gfx++, vtx, 3, 0);
            gSP1Triangle(gfx++, 0, 1, 2, 0);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWall2.l, 1);
            gSPLight(gfx++, &sOmmDebugRenderSurfacesLightWall2.a, 2);
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
        static const BehaviorScript sOmmDebugRenderSurfacesBhv[] = { OBJ_TYPE_UNIMPORTANT, 0x11010001, 0x08000000, 0x09000000 };
        struct Object *o = obj_get_first_with_behavior(sOmmDebugRenderSurfacesBhv);
        if (gOmmDebugSurface) {
            if (!o) {
                o = obj_spawn_from_geo(gMarioObject, sOmmDebugRenderSurfacesGeo, sOmmDebugRenderSurfacesBhv);
                obj_set_pos(o, 0, 0, 0);
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

#endif

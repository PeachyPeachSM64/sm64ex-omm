#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Constants
//

#define SHADOW_FLOOR_LIMIT -10000.f
#define SHADOW_SHAPE_CIRCLE 10
#define SHADOW_SHAPE_SQUARE 20

static const f32 sShadowRectangles[2][2] = {
    { 360, 230 }, // Spindel
    { 200, 180 }, // Whomp
};

static const f32 sVtxCoords[4][2] = {
    { -1, -1 },
    { +1, -1 },
    { -1, +1 },
    { +1, +1 },
};

static const s32 sTexCoords[4][2] = {
    { -15, -15 },
    { +15, -15 },
    { -15, +15 },
    { +15, +15 },
};

//
// Shadow struct
//

static struct {
    Vec3f parentPos;
    f32 scale;
    u8 solidity;
    struct {
        Vec3f normal;
        s16 type;
        f32 height;
        f32 originOffset;
        s16 downwardAngle;
        s16 tilt;
        f32 dist;
    } floor;
    bool isAboveWater;
    bool isOnIceOrCarpet;
    bool isOnFlyingCarpet;
} sShadow[1];

//
// Utils
//

OMM_INLINE f32 scale_shadow_with_distance(f32 scale, f32 distFromFloor) {
    return scale * relerp_0_1_f(distFromFloor, 0.f, 600.f, 1.f, 0.5f);
}

OMM_INLINE f32 extrapolate_vertex_y_position(f32 x, f32 z) {
    return -(sShadow->floor.normal[0] * x + sShadow->floor.normal[2] * z + sShadow->floor.originOffset) / sShadow->floor.normal[1];
}

static void calculate_vertex_xyz(s32 index, Vec3f vtxPos) {
    f32 halfS = (sVtxCoords[index][0] * sShadow->scale) / 2.f;
    f32 halfT = (sVtxCoords[index][1] * sShadow->scale * coss(sShadow->floor.tilt)) / 2.f;
    f32 sinda = sins(sShadow->floor.downwardAngle);
    f32 cosda = coss(sShadow->floor.downwardAngle);
    vtxPos[0] = (halfT * sinda) + (halfS * cosda) + sShadow->parentPos[0];
    vtxPos[2] = (halfT * cosda) - (halfS * sinda) + sShadow->parentPos[2];
    vtxPos[1] = (sShadow->isAboveWater ? sShadow->floor.height : extrapolate_vertex_y_position(vtxPos[0], vtxPos[2]));
}

static void make_shadow_vertex_at_xyz(Vtx *vtx, s32 index, Vec3f relPos) {
    s16 x = relPos[0];
    s16 y = relPos[1] + 5 * (sShadow->isOnFlyingCarpet);
    s16 z = relPos[2];
    u8 alpha = (sShadow->isAboveWater ? 160 : sShadow->solidity);
    make_vertex(vtx, index, x, y, z, sTexCoords[index][0] << 5, sTexCoords[index][1] << 5, 0xFF, 0xFF, 0xFF, alpha);
}

static void make_shadow_vertices(Vtx *vertices) {
    for (s32 i = 0; i != 4; ++i) {
        Vec3f vtxPos, relPos;
        calculate_vertex_xyz(i, vtxPos);
        vec3f_dif(relPos, vtxPos, sShadow->parentPos);
        make_shadow_vertex_at_xyz(vertices, i, relPos);
    }
}

static void add_shadow_to_display_list(Gfx *gfx, Vtx *vtx, s32 shape) {
    switch (shape) {
        case SHADOW_SHAPE_CIRCLE: gSPDisplayList(gfx++, dl_shadow_circle); break;
        case SHADOW_SHAPE_SQUARE: gSPDisplayList(gfx++, dl_shadow_square); break;
    }
    gSPVertex(gfx++, vtx, 4, 0);
    gSPDisplayList(gfx++, dl_shadow_4_verts);
    gSPDisplayList(gfx++, dl_shadow_end);
    gSPEndDisplayList(gfx);
}

static void rotate_rectangle(Vec3f dest, f32 x, f32 y, f32 z) {
    struct Object *obj = gCurrGraphNodeObject;
    f32 siny = sins(obj->oFaceAngleYaw);
    f32 cosy = coss(obj->oFaceAngleYaw);
    dest[0] = z * siny + x * cosy;
    dest[2] = z * cosy - x * siny;
    dest[1] = y;
}

//
// Create shadow
//

static bool init_shadow(Vec3f pos, f32 scale, u8 solidity) {
    sShadow->isAboveWater = false;
    sShadow->isOnIceOrCarpet = false;
    sShadow->isOnFlyingCarpet = false;

    // Find floor
    struct Surface *floor = NULL;
    f32 floorHeight = find_floor(pos[0], pos[1], pos[2], &floor) + 1.f;
    if (!floor) {
        return false;
    }

    // Water
    f32 waterLevel = find_water_level(pos[0], pos[2]);
    if (floorHeight <= waterLevel && waterLevel <= pos[1]) {
        vec3f_set(sShadow->floor.normal, 0, 1, 0);
        sShadow->floor.type = SURFACE_WATER;
        sShadow->floor.height = waterLevel;
        sShadow->floor.originOffset = -waterLevel;
        sShadow->isAboveWater = true;
    } else if (floor->normal.y > 0.f && floorHeight > SHADOW_FLOOR_LIMIT) {
        vec3f_copy(sShadow->floor.normal, (f32 *) &floor->normal);
        sShadow->floor.type = floor->type;
        sShadow->floor.height = floorHeight;
        sShadow->floor.originOffset = floor->originOffset;
        sShadow->isAboveWater = false;
    } else {
        return false;
    }

    // Ice
    if (sShadow->floor.type == SURFACE_ICE) {
        sShadow->isOnIceOrCarpet = true;
    }

    // Fill the shadow struct
    f32 distFromFloor = pos[1] - sShadow->floor.height;
    f32 floorSteepness = sqrtf(sqr_f(sShadow->floor.normal[0]) + sqr_f(sShadow->floor.normal[2]));
    vec3f_copy(sShadow->parentPos, pos);
    sShadow->floor.downwardAngle = atan2s(sShadow->floor.normal[2], sShadow->floor.normal[0]);
    sShadow->floor.tilt = (floorSteepness == 0.f ? 0 : 0x4000 - atan2s(floorSteepness, sShadow->floor.normal[1]));
    sShadow->floor.dist = distFromFloor;
    sShadow->scale = scale_shadow_with_distance(scale, distFromFloor);
    sShadow->solidity = min_s(solidity, relerp_0_1_f(distFromFloor, 0.f, 600.f, solidity, 120));
    return true;
}

static Gfx *create_shadow_player() {
    Vtx *vtx = alloc_display_list(4 * sizeof(Vtx));
    Gfx *gfx = alloc_display_list(5 * sizeof(Gfx));
    if (vtx && gfx) {
        
#if OMM_GAME_IS_SM64
        // Flying carpet in RR
        if (gCurrLevelNum == LEVEL_RR && sShadow->floor.type != SURFACE_DEATH_PLANE) {
            switch (gFlyingCarpetState) {
                case FLYING_CARPET_MOVING_WITHOUT_MARIO:
                    sShadow->isOnIceOrCarpet = true;
                    sShadow->isOnFlyingCarpet = true;
                    break;

                case FLYING_CARPET_MOVING_WITH_MARIO:
                    sShadow->isOnIceOrCarpet = true;
                    break;
            }
        }
#endif
        s16 animFrame = gMarioObject->oAnimInfo.animFrame;
        switch (gMarioObject->oAnimInfo.animID) {
            case MARIO_ANIM_IDLE_ON_LEDGE:    return NULL;
            case MARIO_ANIM_FAST_LEDGE_GRAB:  sShadow->solidity *= invlerp_0_1_f(animFrame,  5, 14); break;
            case MARIO_ANIM_SLOW_LEDGE_GRAB:  sShadow->solidity *= invlerp_0_1_f(animFrame, 21, 33); break;
            case MARIO_ANIM_CLIMB_DOWN_LEDGE: sShadow->solidity *= invlerp_0_1_f(animFrame,  5,  0); break;
        }
        make_shadow_vertices(vtx);
        add_shadow_to_display_list(gfx, vtx, SHADOW_SHAPE_CIRCLE);
        return gfx;
    }
    return NULL;
}

static Gfx *create_shadow_circle() {
    Vtx *vtx = alloc_display_list(4 * sizeof(Vtx));
    Gfx *gfx = alloc_display_list(5 * sizeof(Gfx));
    if (vtx && gfx) {
        make_shadow_vertices(vtx);
        add_shadow_to_display_list(gfx, vtx, SHADOW_SHAPE_CIRCLE);
        return gfx;
    }
    return NULL;
}

static Gfx *create_shadow_rectangle(f32 halfWidth, f32 halfLength) {
    Vtx *vtx = alloc_display_list(4 * sizeof(Vtx));
    Gfx *gfx = alloc_display_list(5 * sizeof(Gfx));
    if (vtx && gfx) {
        Vec3f fl, fr, bl, br;
        rotate_rectangle(fl, -halfWidth, -sShadow->floor.dist, -halfLength);
        rotate_rectangle(fr, +halfWidth, -sShadow->floor.dist, -halfLength);
        rotate_rectangle(bl, -halfWidth, -sShadow->floor.dist, +halfLength);
        rotate_rectangle(br, +halfWidth, -sShadow->floor.dist, +halfLength);
        make_shadow_vertex_at_xyz(vtx, 0, fl);
        make_shadow_vertex_at_xyz(vtx, 1, fr);
        make_shadow_vertex_at_xyz(vtx, 2, bl);
        make_shadow_vertex_at_xyz(vtx, 3, br);
        add_shadow_to_display_list(gfx, vtx, SHADOW_SHAPE_SQUARE);
        return gfx;
    }
    return NULL;
}

static Gfx *create_shadow_square(f32 scale, s32 type) {
    f32 radius = 0.f;
    switch (type) {
        case SHADOW_SQUARE_PERMANENT: radius = (         scale / 2.f); break;
        case SHADOW_SQUARE_SCALABLE:  radius = (sShadow->scale / 2.f); break;
        case SHADOW_SQUARE_TOGGLABLE: radius = (sShadow->scale / 2.f) * (sShadow->floor.dist < 600.f); break;
        default: return NULL;
    }
    return create_shadow_rectangle(radius, radius);
}

static Gfx *create_shadow_hardcoded_rectangle(s32 type) {
    f32 halfWidth  = scale_shadow_with_distance(sShadowRectangles[type - SHADOW_RECTANGLE_HARDCODED_OFFSET][0], sShadow->floor.dist);
    f32 halfLength = scale_shadow_with_distance(sShadowRectangles[type - SHADOW_RECTANGLE_HARDCODED_OFFSET][1], sShadow->floor.dist);
    return create_shadow_rectangle(halfWidth, halfLength);
}

Gfx *create_shadow_below_xyz(f32 xPos, f32 yPos, f32 zPos, s16 scale, u8 solidity, s8 type) {
    Vec3f pos = { xPos, yPos, zPos };
    if (init_shadow(pos, scale, solidity)) {
        switch (type) {
            case SHADOW_CIRCLE_PLAYER:
                return create_shadow_player();

            case SHADOW_CIRCLE_9_VERTS:
            case SHADOW_CIRCLE_4_VERTS:
            case SHADOW_CIRCLE_4_VERTS_FLAT_UNUSED:
                return create_shadow_circle();

            case SHADOW_SQUARE_PERMANENT:
            case SHADOW_SQUARE_SCALABLE:
            case SHADOW_SQUARE_TOGGLABLE:
                return create_shadow_square(scale, type);

            default:
                return create_shadow_hardcoded_rectangle(type);
        }
    }
    return NULL;
}

s32 shadow_get_layer() {
    if (sShadow->isAboveWater) return LAYER_ALPHA;
    if (sShadow->isOnIceOrCarpet) return LAYER_TRANSPARENT;
    return LAYER_TRANSPARENT_DECAL;
}
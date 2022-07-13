#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Behavior
//

static Vtx omm_wing_get_vertex(Vec3f point, Vec3f dir, f32 radius, u8 alpha) {
    Vtx v;
    v.v.ob[0] = point[0] + dir[0] * radius;
    v.v.ob[1] = point[1] + dir[1] * radius;
    v.v.ob[2] = point[2] + dir[2] * radius;
    v.v.cn[3] = alpha;
    return v;
}

static void omm_wing_set_vertex_color(Vtx *vertex, f32 ratio, u8 *alpha) {
    u32 c = omm_player_get_selected_color();
    vertex->v.cn[0] = (u8) ((((c >> 24) & 0xFF) * ratio) + (0xFF * (1.f - ratio)));
    vertex->v.cn[1] = (u8) ((((c >> 16) & 0xFF) * ratio) + (0xFF * (1.f - ratio)));
    vertex->v.cn[2] = (u8) ((((c >>  8) & 0xFF) * ratio) + (0xFF * (1.f - ratio)));
    if (alpha != NULL) {
        vertex->v.cn[3] = *alpha;
    }
}

//
// Wing glow
//

#define OMM_WING_GLOW_NUM_POINTS 32
#define OMM_WING_GLOW_RADIUS_0 12.f
#define OMM_WING_GLOW_RADIUS_1 20.f
#define OMM_WING_GLOW_RADIUS_2 40.f

static Vtx sOmmWingGlowVertices[(OMM_WING_GLOW_NUM_POINTS + 1) * 4];
static Gfx sOmmWingGlowTriangles[OMM_WING_GLOW_NUM_POINTS * 4 + 1];
static Gfx sOmmWingGlowDisplayList[8];
static GeoLayout sOmmWingGlowGeoLayout[16];

OMM_AT_STARTUP static void omm_bhv_wing_glow_init() {

    // Glow vertices and triangles
    Vtx *vtx = sOmmWingGlowVertices;
    Gfx *tri = sOmmWingGlowTriangles;
    for (s32 i = 0; i <= OMM_WING_GLOW_NUM_POINTS; ++i) {
        s16 a = (s16) ((65536.f / OMM_WING_GLOW_NUM_POINTS) * i);
        Vec3f dir = { sins(a), coss(a), 0 };
        if (i != OMM_WING_GLOW_NUM_POINTS) {
            gSPVertex(tri++, vtx, 8, 0);
            gSP1Triangle(tri++, 0, 1, 5, 0);
            gSP2Triangles(tri++, 1, 2, 5, 0, 5, 2, 6, 0);
            gSP2Triangles(tri++, 2, 3, 6, 0, 6, 3, 7, 0);
        }
        *(vtx++) = omm_wing_get_vertex(gVec3fZero, dir,                    0.f, 0xFF);
        *(vtx++) = omm_wing_get_vertex(gVec3fZero, dir, OMM_WING_GLOW_RADIUS_0, 0xF0);
        *(vtx++) = omm_wing_get_vertex(gVec3fZero, dir, OMM_WING_GLOW_RADIUS_1, 0xC0);
        *(vtx++) = omm_wing_get_vertex(gVec3fZero, dir, OMM_WING_GLOW_RADIUS_2, 0x00);
    }
    gSPEndDisplayList(tri);

    // Gfx
    Gfx *gfx = sOmmWingGlowDisplayList;
    gSPClearGeometryMode(gfx++, G_LIGHTING | G_CULL_BOTH);
    gDPSetCombineLERP(gfx++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gSPDisplayList(gfx++, sOmmWingGlowTriangles);
    gSPSetGeometryMode(gfx++, G_LIGHTING | G_CULL_BACK);
    gSPEndDisplayList(gfx);

    // Geo layout
    GeoLayout geo[] = {
        GEO_NODE_START(),
        GEO_OPEN_NODE(),
            GEO_BILLBOARD(),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, sOmmWingGlowDisplayList),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
        GEO_END(),
    };
    OMM_MEMCPY(sOmmWingGlowGeoLayout, geo, sizeof(geo));
}

static void omm_bhv_wing_glow_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    if (!omm_mario_has_wing_cap(m)) {
        obj_mark_for_deletion(o);
        return;
    }

    // Compute Mario's hand pos
    f32  *fPos = geo_get_marios_forearm_pos(o->oAction);
    f32  *wPos = geo_get_marios_hand_pos(o->oAction);
    Vec3f dPos = { wPos[0] - fPos[0], wPos[1] - fPos[1], wPos[2] - fPos[2] };
    Vec3f hPos = { wPos[0], wPos[1], wPos[2] };
    f32 delta  = sqrtf(sqr_f(dPos[0]) + sqr_f(dPos[1]) + sqr_f(dPos[2]));
    if (delta != 0.f) {
        f32 offset = (180.f - geo_get_marios_height()) / (4.f * delta);
        hPos[0] += dPos[0] * m->marioObj->oScaleX * offset;
        hPos[1] += dPos[1] * m->marioObj->oScaleY * offset;
        hPos[2] += dPos[2] * m->marioObj->oScaleZ * offset;
    }

    // Update vertex color
    Vtx *vtx = sOmmWingGlowVertices;
    for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmWingGlowVertices); i += 4) {
        omm_wing_set_vertex_color(vtx++, 0.0f, NULL);
        omm_wing_set_vertex_color(vtx++, 0.2f, NULL);
        omm_wing_set_vertex_color(vtx++, 1.0f, NULL);
        omm_wing_set_vertex_color(vtx++, 1.0f, NULL);
    }

    // Update object's graphics
    obj_set_pos(o, hPos[0], hPos[1], hPos[2]);
    obj_set_angle(o, 0, 0, 0);
    obj_set_scale(o, 1.f, 1.f, 1.f);
    obj_set_always_rendered(o, true);
    o->oGraphNode = geo_layout_to_graph_node(NULL, sOmmWingGlowGeoLayout);
    o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    if ((m->marioObj->oNodeFlags & GRAPH_RENDER_INVISIBLE) ||
       !(m->marioObj->oNodeFlags & GRAPH_RENDER_ACTIVE)) {
        o->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
    } else {
        o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    }

}

//
// Wing trail
//

#define OMM_WING_TRAIL_NUM_POINTS 24
#define OMM_WING_TRAIL_RADIUS_IN 10.f
#define OMM_WING_TRAIL_RADIUS_OUT 40.f

static Vtx sOmmWingTrailVertices[2][OMM_WING_TRAIL_NUM_POINTS * 5];
static Gfx sOmmWingTrailTriangles[2][(OMM_WING_TRAIL_NUM_POINTS - 1) * 5 + 1];
static Gfx sOmmWingTrailDisplayList[8];
static GeoLayout sOmmWingTrailGeoLayout[8];

OMM_AT_STARTUP static void omm_bhv_wing_trail_init() {

    // Gfx
    Gfx *gfx = sOmmWingTrailDisplayList;
    gSPClearGeometryMode(gfx++, G_LIGHTING | G_CULL_BOTH);
    gDPSetCombineLERP(gfx++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gSPDisplayList(gfx++, sOmmWingTrailTriangles[0]);
    gSPDisplayList(gfx++, sOmmWingTrailTriangles[1]);
    gSPSetGeometryMode(gfx++, G_LIGHTING | G_CULL_BACK);
    gSPEndDisplayList(gfx);

    // Geo layout
    GeoLayout geo[] = {
        GEO_NODE_START(),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, sOmmWingTrailDisplayList),
        GEO_CLOSE_NODE(),
        GEO_END(),
    };
    OMM_MEMCPY(sOmmWingTrailGeoLayout, geo, sizeof(geo));
}

static void omm_bhv_wing_trail_reset() {
    OMM_MEMSET(sOmmWingTrailVertices, 0, sizeof(sOmmWingTrailVertices));
}

static void omm_bhv_wing_trail_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    bool swapTriangleBuffers = (m->faceAngle[2] < 0);
    bool trail = omm_mario_has_wing_cap(m) && (m->action == ACT_FLYING);

    // Unload after some time
    o->oTimer *= !trail;
    if (o->oTimer > OMM_WING_TRAIL_NUM_POINTS) {
        obj_mark_for_deletion(o);
        return;
    }

    // Camera plane
    f32 distCamToCamPlane = 10.f; // Used to correct perspective; DO NOT SET 0
    Vec3f camPlaneNormal = {
        gLakituState.focus[0] - gLakituState.pos[0],
        gLakituState.focus[1] - gLakituState.pos[1],
        gLakituState.focus[2] - gLakituState.pos[2]
    };
    Vec3f camPlaneAxis1;
    Vec3f camPlaneAxis2;
    vec3f_get_nullspace(camPlaneNormal, camPlaneAxis1, camPlaneAxis2, camPlaneNormal);
    Vec3f camPlaneOrigin = {
        gLakituState.pos[0] + camPlaneNormal[0] * distCamToCamPlane,
        gLakituState.pos[1] + camPlaneNormal[1] * distCamToCamPlane,
        gLakituState.pos[2] + camPlaneNormal[2] * distCamToCamPlane,
    };

    // Right hand, Left hand
    for (s32 k = 0; k != 2; ++k) {

        // Compute Mario's hand pos
        f32  *fPos = geo_get_marios_forearm_pos(k);
        f32  *wPos = geo_get_marios_hand_pos(k);
        Vec3f dPos = { wPos[0] - fPos[0], wPos[1] - fPos[1], wPos[2] - fPos[2] };
        Vec3f hPos = { wPos[0], wPos[1], wPos[2] };
        f32 delta  = sqrtf(sqr_f(dPos[0]) + sqr_f(dPos[1]) + sqr_f(dPos[2]));
        if (delta != 0.f) {
            f32 offset = (180.f - geo_get_marios_height()) / (4.f * delta);
            hPos[0] += dPos[0] * m->marioObj->oScaleX * offset;
            hPos[1] += dPos[1] * m->marioObj->oScaleY * offset;
            hPos[2] += dPos[2] * m->marioObj->oScaleZ * offset;
        }

        // Update current vertex
        Vtx *vtxHead = &sOmmWingTrailVertices[k][0];
        Vtx *vtxPrev = vtxHead + 5;
        OMM_MEMMOV(vtxPrev, vtxHead, sizeof(Vtx) * (OMM_WING_TRAIL_NUM_POINTS - 1) * 5);
        if (trail) {
            *vtxHead = omm_wing_get_vertex(hPos, gVec3fZero, 0.f, 0xFF);
            omm_wing_set_vertex_color(vtxHead, 0.f, NULL);
            if (vtxPrev->v.cn[3] == 0) {
                vec3f_copy(vtxPrev->v.ob, vtxHead->v.ob);
            }
        } else {
            vtxHead->v.cn[3] = 0;
        }

        // Update vertices alpha
        u8 alphaDec = (u8) ((0xFF + OMM_WING_TRAIL_NUM_POINTS) / OMM_WING_TRAIL_NUM_POINTS);
        for (s32 i = 0; i != (OMM_WING_TRAIL_NUM_POINTS - 1) * 5; ++i, vtxPrev++) {
            if (vtxPrev->v.cn[3] > alphaDec) {
                vtxPrev->v.cn[3] -= alphaDec;
            } else {
                vtxPrev->v.cn[3] = 0;
            }
        }

        // Compute vertices
        Vec3f dirPrev2d = { 0, 0, 0 };
        Gfx *tri = sOmmWingTrailTriangles[swapTriangleBuffers ? (1 - k) : (k)];
        for (s32 i = 0; i != OMM_WING_TRAIL_NUM_POINTS; ++i, vtxHead += 5) {
            s32 iPrev = min_s(i + 1, OMM_WING_TRAIL_NUM_POINTS - 1);
            s32 iNext = max_s(i - 1, 0);
            Vec3f posCurr; vec3f_copy(posCurr, vtxHead->v.ob);
            Vec3f posPrev; vec3f_copy(posPrev, (vtxHead + (5 * (iPrev - i)))->v.ob);
            Vec3f posNext; vec3f_copy(posNext, (vtxHead + (5 * (iNext - i)))->v.ob);

            // Project onto the camera plane
            Vec3f posCurr2d; vec3f_to_2d_plane(posCurr2d, &posCurr2d[2], posCurr, camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);
            Vec3f posPrev2d; vec3f_to_2d_plane(posPrev2d, &posPrev2d[2], posPrev, camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);
            Vec3f posNext2d; vec3f_to_2d_plane(posNext2d, &posNext2d[2], posNext, camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);

            // Correct perspective
            posPrev2d[0] *= (distCamToCamPlane / (distCamToCamPlane + posPrev2d[2]));
            posPrev2d[1] *= (distCamToCamPlane / (distCamToCamPlane + posPrev2d[2]));
            posNext2d[0] *= (distCamToCamPlane / (distCamToCamPlane + posNext2d[2]));
            posNext2d[1] *= (distCamToCamPlane / (distCamToCamPlane + posNext2d[2]));

            // Correct direction to avoid twists
            Vec3f dirCurr2d = { posNext2d[0] - posPrev2d[0], posNext2d[1] - posPrev2d[1], 0 };
            vec3f_normalize(dirCurr2d);
            if (vec3f_dot(dirPrev2d, dirCurr2d) < 0.f) {
                dirCurr2d[0] *= -1.f;
                dirCurr2d[1] *= -1.f;
            }
            vec3f_copy(dirPrev2d, dirCurr2d);
            Vec2f dirRot90  = { +dirCurr2d[1], -dirCurr2d[0] };
            Vec2f dirRot270 = { -dirCurr2d[1], +dirCurr2d[0] };

            // Compute vertices on the 2D plane
            Vec2f v1Pos2d = { posCurr2d[0] + OMM_WING_TRAIL_RADIUS_IN  * dirRot90 [0], posCurr2d[1] + OMM_WING_TRAIL_RADIUS_IN  * dirRot90 [1] };
            Vec2f v2Pos2d = { posCurr2d[0] + OMM_WING_TRAIL_RADIUS_OUT * dirRot90 [0], posCurr2d[1] + OMM_WING_TRAIL_RADIUS_OUT * dirRot90 [1] };
            Vec2f v3Pos2d = { posCurr2d[0] + OMM_WING_TRAIL_RADIUS_IN  * dirRot270[0], posCurr2d[1] + OMM_WING_TRAIL_RADIUS_IN  * dirRot270[1] };
            Vec2f v4Pos2d = { posCurr2d[0] + OMM_WING_TRAIL_RADIUS_OUT * dirRot270[0], posCurr2d[1] + OMM_WING_TRAIL_RADIUS_OUT * dirRot270[1] };

            // Convert 2D positions to 3D positions
            vec2f_and_dist_to_3d(vtxHead[1].v.ob, v1Pos2d, posCurr2d[2], camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);
            vec2f_and_dist_to_3d(vtxHead[2].v.ob, v2Pos2d, posCurr2d[2], camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);
            vec2f_and_dist_to_3d(vtxHead[3].v.ob, v3Pos2d, posCurr2d[2], camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);
            vec2f_and_dist_to_3d(vtxHead[4].v.ob, v4Pos2d, posCurr2d[2], camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);

            // Update vertex color
            u8 alpha0[] = { 0 };
            u8 *alpha1 = &vtxHead->v.cn[3];
            omm_wing_set_vertex_color(vtxHead + 1, 0.8f, alpha1);
            omm_wing_set_vertex_color(vtxHead + 2, 1.0f, alpha0);
            omm_wing_set_vertex_color(vtxHead + 3, 0.8f, alpha1);
            omm_wing_set_vertex_color(vtxHead + 4, 1.0f, alpha0);

            // Build triangles
            if (i < OMM_WING_TRAIL_NUM_POINTS - 1) {
                gSPVertex(tri++, vtxHead, 10, 0);
                gSP2Triangles(tri++, 0, 1, 5, 0, 1, 5, 6, 0);
                gSP2Triangles(tri++, 1, 2, 6, 0, 2, 6, 7, 0);
                gSP2Triangles(tri++, 0, 3, 5, 0, 3, 5, 8, 0);
                gSP2Triangles(tri++, 3, 4, 8, 0, 4, 8, 9, 0);
            }
        }
        gSPEndDisplayList(tri);
    }

    // Update object's graphics
    obj_set_pos(o, 0, 0, 0);
    obj_set_angle(o, 0, 0, 0);
    obj_set_scale(o, 1.f, 1.f, 1.f);
    obj_set_always_rendered(o, true);
    o->oGraphNode = geo_layout_to_graph_node(NULL, sOmmWingTrailGeoLayout);
    o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
}

//
// Behaviors
//

const BehaviorScript omm_bhv_wing_glow_left_hand[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_wing_glow_update,
    0x09000000,
};

const BehaviorScript omm_bhv_wing_glow_right_hand[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_wing_glow_update,
    0x09000000,
};

const BehaviorScript omm_bhv_wing_trail[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x0C000000, (uintptr_t) omm_bhv_wing_trail_reset,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_wing_trail_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_wing_glow_and_trail(struct Object *o) {

    // Wing glows
    if (!obj_get_first_with_behavior(omm_bhv_wing_glow_right_hand)) {
        spawn_object(o, MODEL_NONE, omm_bhv_wing_glow_right_hand)->oAction = 0;
        spawn_object(o, MODEL_NONE, omm_bhv_wing_glow_left_hand)->oAction = 1;
    }

    // Wing trail
    if (gMarioState->action == ACT_FLYING) {
        if (!obj_get_first_with_behavior(omm_bhv_wing_trail)) {
            spawn_object(o, MODEL_NONE, omm_bhv_wing_trail);
        }
    }

    return NULL;
}

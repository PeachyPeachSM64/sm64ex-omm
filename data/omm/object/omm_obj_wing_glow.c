#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

static Vtx omm_wing_get_vertex(Vec3f point, Vec3f dir, f32 radius, u8 alpha) {
    Vtx v;
    v.v.ob[0] = point[0] + dir[0] * radius;
    v.v.ob[1] = point[1] + dir[1] * radius;
    v.v.ob[2] = point[2] + dir[2] * radius;
    v.v.cn[3] = alpha;
    return v;
}

static void omm_wing_set_vertex_color(Vtx *vertex, f32 ratio, u8 *alpha) {
    u32 c = omm_player_properties_get_selected_color();
    vertex->v.cn[0] = (u8) ((((c >> 24) & 0xFF) * ratio) + (0xFF * (1.f - ratio)));
    vertex->v.cn[1] = (u8) ((((c >> 16) & 0xFF) * ratio) + (0xFF * (1.f - ratio)));
    vertex->v.cn[2] = (u8) ((((c >>  8) & 0xFF) * ratio) + (0xFF * (1.f - ratio)));
    if (alpha) vertex->v.cn[3] = *alpha;
}

static void omm_wing_compute_mario_hand_pos(struct MarioState *m, Vec3f dest, bool isLeft) {
    Vec3f fpos; geo_get_marios_anim_part_pos(NULL, fpos, isLeft ? MARIO_ANIM_PART_LEFT_FOREARM : MARIO_ANIM_PART_RIGHT_FOREARM);
    Vec3f hpos; geo_get_marios_anim_part_pos(NULL, hpos, isLeft ? MARIO_ANIM_PART_LEFT_HAND : MARIO_ANIM_PART_RIGHT_HAND);
    Vec3f dpos; vec3f_dif(dpos, hpos, fpos);
    vec3f_normalize(dpos);
    vec3f_mult(dpos, dpos, m->marioObj->oGfxScale);
    vec3f_mul(dpos, OMM_PLAYER_MODEL_IS_PEACH ? 10.f : 15.f);
    vec3f_sum(dest, hpos, dpos);
}

//
// Wing glow
//

#define OMM_WING_GLOW_NUM_POINTS (32)
#define OMM_WING_GLOW_RADIUS_0   (12.f)
#define OMM_WING_GLOW_RADIUS_1   (20.f)
#define OMM_WING_GLOW_RADIUS_2   (40.f)

static Vtx omm_wing_glow_vtx[(OMM_WING_GLOW_NUM_POINTS + 1) * 4];

static Gfx omm_wing_glow_tri[OMM_WING_GLOW_NUM_POINTS * 4 + 1] = { gsSPEndDisplayList() };

static const Gfx omm_wing_glow_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPDisplayList(omm_wing_glow_tri),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const GeoLayout omm_geo_wing_glow[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_wing_glow_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

OMM_AT_STARTUP static void bhv_omm_wing_glow_init() {
    Vtx *vtx = omm_wing_glow_vtx;
    Gfx *tri = omm_wing_glow_tri;
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
}

static void bhv_omm_wing_glow_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    if (!omm_mario_has_wing_cap(m) || omm_mario_is_capture(m)) {
        obj_mark_for_deletion(o);
        return;
    }

    // Update vertex color
    Vtx *vtx = omm_wing_glow_vtx;
    for (s32 i = 0; i != array_length(omm_wing_glow_vtx); i += 4) {
        omm_wing_set_vertex_color(vtx++, 0.0f, NULL);
        omm_wing_set_vertex_color(vtx++, 0.2f, NULL);
        omm_wing_set_vertex_color(vtx++, 1.0f, NULL);
        omm_wing_set_vertex_color(vtx++, 1.0f, NULL);
    }

    // Update object's graphics
    Vec3f hpos; omm_wing_compute_mario_hand_pos(m, hpos, o->behavior == bhvOmmWingGlowLeft);
    obj_set_xyz(o, hpos[0], hpos[1], hpos[2]);
    obj_set_angle(o, 0, 0, 0);
    obj_set_scale(o, 1.f, 1.f, 1.f);
    obj_set_always_rendered(o, true);
    o->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_wing_glow);
    o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    if (omm_mario_is_invisible(m)) {
        o->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
    } else {
        o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    }
}

//
// Wing trail
//

#define OMM_WING_TRAIL_NUM_POINTS (24)
#define OMM_WING_TRAIL_RADIUS_IN  (10.f)
#define OMM_WING_TRAIL_RADIUS_OUT (40.f)

static Vtx omm_wing_trail_left_vtx[(OMM_WING_TRAIL_NUM_POINTS + 2) * 5];
static Vtx omm_wing_trail_right_vtx[(OMM_WING_TRAIL_NUM_POINTS + 2) * 5];

static Vtx *omm_wing_trail_vtx[] = {
    omm_wing_trail_left_vtx,
    omm_wing_trail_right_vtx,
};

static Gfx omm_wing_trail_left_tri[(OMM_WING_TRAIL_NUM_POINTS - 1) * 5 + 1] = { gsSPEndDisplayList() };
static Gfx omm_wing_trail_right_tri[(OMM_WING_TRAIL_NUM_POINTS - 1) * 5 + 1] = { gsSPEndDisplayList() };

static Gfx *omm_wing_trail_tri[] = {
    omm_wing_trail_left_tri,
    omm_wing_trail_right_tri,
};

static const Gfx omm_wing_trail_left_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPDisplayList(omm_wing_trail_left_tri),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static const Gfx omm_wing_trail_right_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPDisplayList(omm_wing_trail_right_tri),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList()
};

static Gfx *omm_geo_wing_trail_update_gfx(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER && gCurrGraphNodeObject) {
        struct Object *o = gCurrGraphNodeObject;
        struct MarioState *m = gMarioState;
        s32 index = ((struct GraphNodeGenerated *) node)->parameter;
        bool swapTriangleBuffers = (m->faceAngle[2] < 0);

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

        // Compute vertices
        Vec3f dirPrev2d = { 0, 0, 0 };
        Vtx *vtxHead = &omm_wing_trail_vtx[index][10];
        Vtx *vtxCurr = vtxHead;
        Gfx *tri = omm_wing_trail_tri[swapTriangleBuffers ? (1 - index) : (index)];
        for (s32 i = 0; i != OMM_WING_TRAIL_NUM_POINTS; ++i, vtxCurr += 5) {
            s32 iPrev = min_s(i + 1, OMM_WING_TRAIL_NUM_POINTS - 1);
            s32 iNext = max_s(i - 1, 0);
            Vec3f posCurr; vec3f_copy(posCurr, vtxCurr->v.ob);
            Vec3f posPrev; vec3f_copy(posPrev, (vtxCurr + (5 * (iPrev - i)))->v.ob);
            Vec3f posNext; vec3f_copy(posNext, (vtxCurr + (5 * (iNext - i)))->v.ob);

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
            vec2f_and_dist_to_3d(vtxCurr[1].v.ob, v1Pos2d, posCurr2d[2], camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);
            vec2f_and_dist_to_3d(vtxCurr[2].v.ob, v2Pos2d, posCurr2d[2], camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);
            vec2f_and_dist_to_3d(vtxCurr[3].v.ob, v3Pos2d, posCurr2d[2], camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);
            vec2f_and_dist_to_3d(vtxCurr[4].v.ob, v4Pos2d, posCurr2d[2], camPlaneOrigin, camPlaneNormal, camPlaneAxis1, camPlaneAxis2);

            // Update vertex color
            u8 alpha0[] = {0};
            u8 *alpha1 = &vtxCurr->v.cn[3];
            omm_wing_set_vertex_color(vtxCurr + 1, 0.8f, alpha1);
            omm_wing_set_vertex_color(vtxCurr + 2, 1.0f, alpha0);
            omm_wing_set_vertex_color(vtxCurr + 3, 0.8f, alpha1);
            omm_wing_set_vertex_color(vtxCurr + 4, 1.0f, alpha0);

            // Build triangles
            if (i < OMM_WING_TRAIL_NUM_POINTS - 1) {
                gSPVertex(tri++, vtxCurr, 10, 0);
                gSP2Triangles(tri++, 0, 1, 5, 0, 1, 5, 6, 0);
                gSP2Triangles(tri++, 1, 2, 6, 0, 2, 6, 7, 0);
                gSP2Triangles(tri++, 0, 3, 5, 0, 3, 5, 8, 0);
                gSP2Triangles(tri++, 3, 4, 8, 0, 4, 8, 9, 0);
            }
        }
        gSPEndDisplayList(tri);

        // Enable or disable interpolation of first point
        mem_cpy(&omm_wing_trail_vtx[index][5], vtxHead, sizeof(Vtx) * 5);
        if (!o->oSubAction) {
            mem_cpy(&omm_wing_trail_vtx[index][0], vtxHead, sizeof(Vtx) * 5);
        }
    }
    return NULL;
}

static const GeoLayout omm_geo_wing_trail_left[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(0, omm_geo_wing_trail_update_gfx),
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_wing_trail_left_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

static const GeoLayout omm_geo_wing_trail_right[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(1, omm_geo_wing_trail_update_gfx),
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_wing_trail_right_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

static const GeoLayout *omm_geo_wing_trail[] = {
    omm_geo_wing_trail_left,
    omm_geo_wing_trail_right,
};

static bool bhv_omm_wing_trail_is_mario_flying(struct MarioState *m) {
    return m->action == ACT_FLYING || (m->action == ACT_JUMBO_STAR_CUTSCENE && m->actionArg == 2);
}

static void bhv_omm_wing_trail_reset() {
    struct Object *o = gCurrentObject;
    s32 index = (o->behavior == bhvOmmWingTrailRight);
    mem_zero(omm_wing_trail_vtx[index], sizeof(omm_wing_trail_left_vtx));
}

static void bhv_omm_wing_trail_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    s32 index = (o->behavior == bhvOmmWingTrailRight);
    bool trail = omm_mario_has_wing_cap(m) && bhv_omm_wing_trail_is_mario_flying(m) && !omm_mario_is_invisible(m);

    // Unload after some time
    o->oTimer *= !trail;
    if (o->oTimer > OMM_WING_TRAIL_NUM_POINTS) {
        obj_mark_for_deletion(o);
        return;
    }

    // Instant warp displacement
    if (gOmmGlobals->instantWarp.warped) {
        for_each_(Vtx, vtx, omm_wing_trail_vtx[index], array_length(omm_wing_trail_left_vtx)) {
            vec3f_add(vtx->v.ob, gOmmGlobals->instantWarp.displacement);
        }
    }

    // Update current vertex
    Vtx *vtx0 = &omm_wing_trail_vtx[index][0];
    Vtx *vtx1 = &omm_wing_trail_vtx[index][5];
    Vtx *vtxHead = &omm_wing_trail_vtx[index][10];
    Vtx *vtxPrev = vtxHead + 5;
    mem_cpy(vtxHead, vtx1, sizeof(Vtx) * 5);
    mem_cpy(vtx0, vtxHead, sizeof(Vtx) * 5);
    mem_mov(vtxPrev, vtxHead, sizeof(Vtx) * (OMM_WING_TRAIL_NUM_POINTS - 1) * 5);
    o->oSubAction = TRUE; // interpolation of first point
    if (trail) {
        Vec3f hpos; omm_wing_compute_mario_hand_pos(m, hpos, !index);
        *vtxHead = omm_wing_get_vertex(hpos, gVec3fZero, 0.f, 0xFF);
        omm_wing_set_vertex_color(vtxHead, 0.f, NULL);
        if (vtxPrev->v.cn[3] == 0) {
            vec3f_copy(vtxPrev->v.ob, vtxHead->v.ob);
            o->oSubAction = FALSE;
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

    // Update object's graphics
    obj_set_pos_vec3f(o, gVec3fZero);
    obj_set_angle_vec3s(o, gVec3sZero);
    obj_set_scale_vec3f(o, gVec3fOne);
    obj_set_always_rendered(o, true);
    o->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_wing_trail[index]);
    o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
}

void gfx_interpolate_frame_bhv_omm_wing_trail(f32 t) {
    for (u32 index = 0; index != 2; ++index) {
        Vtx *vtx = omm_wing_trail_vtx[index];
        for (u32 i = 0; i != 5; ++i) {
            vtxv_interpolate(
                &(vtx + 10 + i)->v,
                &(vtx +  0 + i)->v,
                &(vtx +  5 + i)->v,
                (gOmmGlobals->cameraSnapshotMode ? 1.f : t)
            );
        }
    }
}

//
// Behaviors
//

const BehaviorScript bhvOmmWingGlowLeft[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_wing_glow_update),
    BHV_END_LOOP(),
};

const BehaviorScript bhvOmmWingGlowRight[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_wing_glow_update),
    BHV_END_LOOP(),
};

const BehaviorScript bhvOmmWingTrailLeft[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_CALL_NATIVE(bhv_omm_wing_trail_reset),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_wing_trail_update),
    BHV_END_LOOP(),
};

const BehaviorScript bhvOmmWingTrailRight[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_CALL_NATIVE(bhv_omm_wing_trail_reset),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_wing_trail_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_wing_glow_and_trail(struct Object *o) {

    // Wing glows
    if (!omm_mario_is_capture(gMarioState)) {
        if (!obj_get_first_with_behavior(bhvOmmWingGlowRight)) {
            spawn_object(o, MODEL_NONE, bhvOmmWingGlowRight);
        }
        if (!obj_get_first_with_behavior(bhvOmmWingGlowLeft)) {
            spawn_object(o, MODEL_NONE, bhvOmmWingGlowLeft);
        }
    }

    // Wing trail
    if (bhv_omm_wing_trail_is_mario_flying(gMarioState)) {
        if (!obj_get_first_with_behavior(bhvOmmWingTrailRight)) {
            spawn_object(o, MODEL_NONE, bhvOmmWingTrailRight);
        }
        if (!obj_get_first_with_behavior(bhvOmmWingTrailLeft)) {
            spawn_object(o, MODEL_NONE, bhvOmmWingTrailLeft);
        }
    }

    return NULL;
}

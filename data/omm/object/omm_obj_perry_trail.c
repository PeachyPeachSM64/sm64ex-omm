#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#define OMM_PERRY_TRAIL_NUM_SEGMENTS_MAX        (16)
#define OMM_PERRY_TRAIL_NUM_POINTS_PER_FRAME    (4)
#define OMM_PERRY_TRAIL_NUM_POINTS_MAX          (OMM_PERRY_TRAIL_NUM_SEGMENTS_MAX * OMM_PERRY_TRAIL_NUM_POINTS_PER_FRAME)

//
// Gfx data
//

static const Gfx omm_perry_trail_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH | G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPDisplayList(NULL),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    Vec4f br, bl, tr, tl;
} Point;

typedef struct {
    Gfx *displayLists[1];
    Gfx gfx[array_length(omm_perry_trail_gfx)];
    Gfx tri[OMM_PERRY_TRAIL_NUM_POINTS_MAX * 2 + 1];
    Vtx vtx[OMM_PERRY_TRAIL_NUM_POINTS_MAX][4];
    Point pts[OMM_PERRY_TRAIL_NUM_POINTS_MAX];
} OmmPerryTrailGeoData;

static const u32 sOmmPerryTrailGeoDataDisplayListsOffsets[] = {
    offsetof(OmmPerryTrailGeoData, gfx),
};

//
// Geo layout
//

const GeoLayout omm_geo_perry_trail[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(0, geo_link_geo_data),
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_perry_trail_update() {
    struct Object *o = gCurrentObject;
    struct MarioState *m = gMarioState;
    OmmPerryTrailGeoData *data = geo_get_geo_data(o,
        sizeof(OmmPerryTrailGeoData),
        sOmmPerryTrailGeoDataDisplayListsOffsets,
        array_length(sOmmPerryTrailGeoDataDisplayListsOffsets)
    );
    o->oPerryType = omm_perry_get_type(m);

    // Instant warp displacement
    if (gOmmGlobals->instantWarp.warped) {
        for_each_(Point, pt, data->pts, array_length(data->pts)) {
            vec3f_add(pt->br, gOmmGlobals->instantWarp.displacement);
            vec3f_add(pt->bl, gOmmGlobals->instantWarp.displacement);
            vec3f_add(pt->tr, gOmmGlobals->instantWarp.displacement);
            vec3f_add(pt->tl, gOmmGlobals->instantWarp.displacement);
        }
    }

    // Add new point...
    mem_mov(data->pts + OMM_PERRY_TRAIL_NUM_POINTS_PER_FRAME, data->pts, sizeof(data->pts[0]) * (OMM_PERRY_TRAIL_NUM_POINTS_MAX - OMM_PERRY_TRAIL_NUM_POINTS_PER_FRAME));
    struct Object *perry = omm_perry_get_object();
    if (o->oAction == 0 && perry && (perry->oPerryFlags & OBJ_INT_PERRY_TRAIL)) {
        Vec3f perryDir = { 0.f, 120.f, 0.f };
        vec3f_rotate_zxy(perryDir, perryDir, perry->oFaceAnglePitch, perry->oFaceAngleYaw, perry->oFaceAngleRoll);

        // Real points, not interpolated
        Point *pts[3] = {
            &data->pts[0],
            &data->pts[OMM_PERRY_TRAIL_NUM_POINTS_PER_FRAME],
            &data->pts[OMM_PERRY_TRAIL_NUM_POINTS_PER_FRAME * 2]
        };

        // Current point, left side
        pts[0]->bl[0] = perry->oPosX;
        pts[0]->bl[1] = perry->oPosY;
        pts[0]->bl[2] = perry->oPosZ;
        pts[0]->bl[3] = 0xF0 * (perry->oOpacity / 255.f) * (o->oTimer >= 2);
        pts[0]->tl[0] = perry->oPosX + perryDir[0] * perry->oScaleX;
        pts[0]->tl[1] = perry->oPosY + perryDir[1] * perry->oScaleY;
        pts[0]->tl[2] = perry->oPosZ + perryDir[2] * perry->oScaleZ;
        pts[0]->tl[3] = 0xF0 * (perry->oOpacity / 255.f) * (o->oTimer >= 2);
        if (o->oTimer == 0) {
            vec4f_copy(pts[0]->br, pts[0]->bl);
            vec4f_copy(pts[0]->tr, pts[0]->tl);
            mem_cpy(pts[1], pts[0], sizeof(Point));
            mem_cpy(pts[2], pts[0], sizeof(Point));
        }

        // Interpolated points
        for (s32 i = 0; i != OMM_PERRY_TRAIL_NUM_POINTS_PER_FRAME; ++i) {
            f32 t = (f32) (i + 1) / (f32) OMM_PERRY_TRAIL_NUM_POINTS_PER_FRAME;
            Vec3f br, tr;
            vec3f_interpolate3(br, t, pts[0]->bl, 0.f, pts[1]->bl, 1.f, pts[2]->bl, 2.f);
            vec3f_interpolate3(tr, t, pts[0]->tl, 0.f, pts[1]->tl, 1.f, pts[2]->tl, 2.f);
            vec3f_copy(data->pts[i].br, br); data->pts[i].br[3] = pts[0]->bl[3];
            vec3f_copy(data->pts[i].tr, tr); data->pts[i].tr[3] = pts[0]->tl[3];
            vec4f_copy(data->pts[i + 1].bl, data->pts[i].br);
            vec4f_copy(data->pts[i + 1].tl, data->pts[i].tr);
        }
    }

    // ...or progressively unload the trail
    else {
        mem_zero(data->pts, sizeof(data->pts[0]) * OMM_PERRY_TRAIL_NUM_POINTS_PER_FRAME);
        o->oAction = 1;
        if (o->oSubAction++ > OMM_PERRY_TRAIL_NUM_SEGMENTS_MAX) {
            obj_mark_for_deletion(o);
        }
    }

    // Colors
    u8 r0 = lerp_s(0.5f, 0xFF, OMM_PERRY_COLOR_FRONT[0]);
    u8 g0 = lerp_s(0.5f, 0xFF, OMM_PERRY_COLOR_FRONT[1]);
    u8 b0 = lerp_s(0.5f, 0xFF, OMM_PERRY_COLOR_FRONT[2]);
    u8 r1 = OMM_PERRY_COLOR_FRONT[0];
    u8 g1 = OMM_PERRY_COLOR_FRONT[1];
    u8 b1 = OMM_PERRY_COLOR_FRONT[2];
    u8 r2 = lerp_s(0.5f, OMM_PERRY_COLOR_FRONT[0], OMM_PERRY_COLOR_BACK[0]);
    u8 g2 = lerp_s(0.5f, OMM_PERRY_COLOR_FRONT[1], OMM_PERRY_COLOR_BACK[1]);
    u8 b2 = lerp_s(0.5f, OMM_PERRY_COLOR_FRONT[2], OMM_PERRY_COLOR_BACK[2]);

    // Update points
    for (s32 i = 0; i != OMM_PERRY_TRAIL_NUM_POINTS_MAX; ++i) {
        const f32 *pts[4] = {
            data->pts[i].br,
            data->pts[i].bl,
            data->pts[i].tr,
            data->pts[i].tl
        };
        for (s32 j = 0; j != 4; ++j) {
            f32 t = 1.f - ((f32) (i + ((j & 1) == 0)) / (f32) OMM_PERRY_TRAIL_NUM_POINTS_MAX);
            Vtx *vtx = &data->vtx[i][j];
            Vec3f center = {
                (pts[j][0] + pts[(j + 2) % 4][0]) / 2.f,
                (pts[j][1] + pts[(j + 2) % 4][1]) / 2.f,
                (pts[j][2] + pts[(j + 2) % 4][2]) / 2.f,
            };
            Vec3f dir = {
                pts[j][0] - center[0],
                pts[j][1] - center[1],
                pts[j][2] - center[2],
            };
            f32 f = relerp_f(t * t * t, 0.f, 1.f, j < 2 ? 1.f : 2.f, 1.f);
            vtx->v.ob[0] = center[0] + dir[0] * f;
            vtx->v.ob[1] = center[1] + dir[1] * f;
            vtx->v.ob[2] = center[2] + dir[2] * f;
            vtx->v.cn[0] = (u8) (t > 0.8f ? relerp_0_1_f(t, 1.f, 0.8f, r0, r1) : relerp_0_1_f(t, 0.8f, 0.5f, r1, r2));
            vtx->v.cn[1] = (u8) (t > 0.8f ? relerp_0_1_f(t, 1.f, 0.8f, g0, g1) : relerp_0_1_f(t, 0.8f, 0.5f, g1, g2));
            vtx->v.cn[2] = (u8) (t > 0.8f ? relerp_0_1_f(t, 1.f, 0.8f, b0, b1) : relerp_0_1_f(t, 0.8f, 0.5f, b1, b2));
            vtx->v.cn[3] = clamp_s(pts[j][3] * clamp_0_1_f(sqr_f(t * 1.2f)), 0x00, 0xFF);
        }
    }

    // Triangles
    Gfx *tri = data->tri;
    for (s32 i = 0; i != OMM_PERRY_TRAIL_NUM_POINTS_MAX; ++i) {
        gSPVertex(tri++, data->vtx[i], 4, 0);
        gSP2Triangles(tri++, 0, 2, 1, 0, 1, 2, 3, 0);
    }
    gSPEndDisplayList(tri++);
    gfx_copy_and_fill_null(data->gfx, omm_perry_trail_gfx, sizeof(omm_perry_trail_gfx), data->tri);

    // Update properties
    obj_scale(o, 1.f);
    obj_set_xyz(o, 0, 0, 0);
    obj_set_angle(o, 0, 0, 0);
    obj_set_always_rendered(o, true);
}

const BehaviorScript bhvOmmPerryTrail[] = {
    OBJ_TYPE_DEFAULT, // This object must be updated after bhvOmmPerry
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_perry_trail_update),
    BHV_END_LOOP(),
};

//
// Spawner (auto)
//

OMM_ROUTINE_UPDATE(omm_obj_spawn_perry_trail) {
    if (gMarioObject && OMM_PERRY_IS_AVAILABLE) {
        struct Object *activeTrail = obj_get_first_with_behavior_and_field_s32(bhvOmmPerryTrail, _FIELD(oAction), 0);
        if (!activeTrail) {
            struct Object *perry = omm_perry_get_object();
            if (perry && (perry->oPerryFlags & OBJ_INT_PERRY_TRAIL)) {
                obj_spawn_from_geo(gMarioObject, omm_geo_perry_trail, bhvOmmPerryTrail);
            }
        }
    }
}

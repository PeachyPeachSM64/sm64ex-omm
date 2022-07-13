#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS                24
#define OMM_PEACH_VIBE_GLOOM_TEAR_OPACITY_MAX_POINT         8
#define OMM_PEACH_VIBE_GLOOM_TEAR_OPACITY_ZERO_POINT        16
#define OMM_PEACH_VIBE_GLOOM_TEAR_NUM_INTERPOLATED_POINTS   4
#define OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT    8
#define OMM_PEACH_VIBE_GLOOM_TEAR_NUM_FRAMES_SIZE_GROW      8
#define OMM_PEACH_VIBE_GLOOM_TEAR_RADIUS                    30.f
#define OMM_PEACH_VIBE_GLOOM_TEAR_OPACITY                   0x60

//
// Gfx data
//

static const Lights1 omm_peach_vibe_gloom_tear_light = gdSPDefLights1(
    0x00, 0x80, 0xFF,
    0x00, 0x80, 0xFF,
    0x28, 0x28, 0x28
);

static const Gfx omm_peach_vibe_gloom_tear_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPLight(&omm_peach_vibe_gloom_tear_light.l, 1),
    gsSPLight(&omm_peach_vibe_gloom_tear_light.a, 2),
    gsSPDisplayList(null),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct OmmPeachVibeGloomTearPoint {
    Vec3f pos;
    Vec3f vel;
    s32 timer;
    bool active;
} Tpt;

typedef struct {
    Gfx gfx[OMM_ARRAY_SIZE(omm_peach_vibe_gloom_tear_gfx)];
    Gfx tri[2 * OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS * OMM_PEACH_VIBE_GLOOM_TEAR_NUM_INTERPOLATED_POINTS * (OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT + 1) + 1];
    Vtx vtx[2 * OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS * OMM_PEACH_VIBE_GLOOM_TEAR_NUM_INTERPOLATED_POINTS * OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT];
    Tpt pts[2][OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS];
} OmmPeachVibeGloomTearGeoData;

//
// Geo layout
//

const GeoLayout omm_geo_peach_vibe_gloom_tear[] = {
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

static void omm_bhv_peach_vibe_gloom_tear_update() {
    struct Object *o = gCurrentObject;
    OmmPeachVibeGloomTearGeoData *data = geo_get_geo_data(o, sizeof(OmmPeachVibeGloomTearGeoData), omm_peach_vibe_gloom_tear_gfx, sizeof(omm_peach_vibe_gloom_tear_gfx));
    Vec3f mp; vec3f_copy(mp, gMarioState->pos);
    Vec3f dp; vec3f_dif(dp, mp, gOmmData->mario->state.previous.pos);

    // Update current points
    for (s32 side = 0; side != 2; ++side) {
        for (s32 i = 0; i != OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS - 1; ++i) {
            Tpt *pt = &data->pts[side][i];
            if (pt->active) {
                pt->timer++;

                // Update pos and vel
                pt->pos[0] += pt->vel[0] - dp[0] / 1.5f;
                pt->pos[1] += pt->vel[1] - dp[1];
                pt->pos[2] += pt->vel[2] - dp[2] / 1.5f;
                pt->vel[1] = max_f(pt->vel[1] - 3.f, -75.f);

                // Process floor and ceiling
                struct Surface *f = NULL;
                f32 yf = find_floor(
                    mp[0] + pt->pos[0],
                    mp[1] + pt->pos[1],
                    mp[2] + pt->pos[2], 
                    &f
                );

                // Out of bounds, cancel XZ displacement
                if (!f) {
                    pt->pos[0] -= pt->vel[0];
                    pt->pos[2] -= pt->vel[2];
                    pt->vel[0] = 0.f;
                    pt->vel[2] = 0.f;
                } 
            
                // Floor collision
                else if (yf > mp[1] + pt->pos[1]) {
                    pt->pos[1] = yf - mp[1];
                    data->pts[side][i + 1].active = false;
                }
            
                // Ceiling
                else {
                    f32 yc = find_ceil(
                        mp[0] + pt->pos[0],
                        mp[1] + pt->pos[1],
                        mp[2] + pt->pos[2],
                        NULL
                    );
                    if (yf < yc && yc < mp[1] + pt->pos[1]) {
                        pt->pos[1] = yc - mp[1];
                    }
                }

                // Process wall collision
                struct WallCollisionData coldata;
                coldata.x = mp[0] + pt->pos[0];
                coldata.y = mp[1] + pt->pos[1];
                coldata.z = mp[2] + pt->pos[2];
                coldata.radius = OMM_PEACH_VIBE_GLOOM_TEAR_RADIUS * invlerp_0_1_f(pt->timer, 0, OMM_PEACH_VIBE_GLOOM_TEAR_NUM_FRAMES_SIZE_GROW);
                coldata.offsetY = 0.f;
                if (find_wall_collisions(&coldata)) {
                    pt->pos[0] = coldata.x - mp[0];
                    pt->pos[1] = coldata.y - mp[1];
                    pt->pos[2] = coldata.z - mp[2];
                }
            }
        }
    }

    // If Gloom is active, add new point, and spawn a small tear
    if (omm_peach_vibe_is_gloom()) {
        f32 *p = geo_get_marios_head_pos();
        f32 *u = geo_get_marios_head_up();
        f32 *f = geo_get_marios_head_forward();
        f32 *r = geo_get_marios_head_right();
        f32 du = 11.f * gMarioObject->oScaleY;
        f32 df = 14.f * gMarioObject->oScaleX;
        f32 dr = 10.f * gMarioObject->oScaleX;
        vec3f_sub(p, mp);

        // Handle separately the running animation
        if (gMarioObject->oAnimID == MARIO_ANIM_RUNNING) {
            p[0] = 0.f;
            p[1] = 120.f * gMarioObject->oScaleY;
            p[2] = 0.f;
            u[0] = 0.f;
            u[1] = 1.f;
            u[2] = 0.f;
            f[0] = sins(gMarioState->faceAngle[1]);
            f[1] = 0.f;
            f[2] = coss(gMarioState->faceAngle[1]);
            df = 8.f * gMarioObject->oScaleX;
            dr = 12.f * gMarioObject->oScaleX;
        }

        // Add a new point for both sides
        for (s32 side = 0; side != 2; ++side) {
            OMM_MEMMOV(data->pts[side] + 1, data->pts[side], (OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS - 1) * sizeof(Tpt));
            f32 sr = (side == 0 ? +1.f : -1.f);
            data->pts[side][0].pos[0] = p[0] + du * u[0] + df * f[0] + dr * sr * r[0];
            data->pts[side][0].pos[1] = p[1] + du * u[1] + df * f[1] + dr * sr * r[1];
            data->pts[side][0].pos[2] = p[2] + du * u[2] + df * f[2] + dr * sr * r[2];
            data->pts[side][0].vel[0] = 14.f * u[0] + 1.f * f[0] + 8.f * sr * r[0];
            data->pts[side][0].vel[1] = 14.f * u[1] + 1.f * f[1] + 8.f * sr * r[1];
            data->pts[side][0].vel[2] = 14.f * u[2] + 1.f * f[2] + 8.f * sr * r[2];
            data->pts[side][0].timer = 0;
            data->pts[side][0].active = true;
        }
        omm_spawn_peach_vibe_gloom_tear_small(o,
            mp[0] + p[0] + 30.f * u[0],
            mp[1] + p[1] + 30.f * u[1],
            mp[2] + p[2] + 30.f * u[2]
        );
    }
    
    // otherwise, cut the flow
    else {
        for (s32 side = 0; side != 2; ++side) {
            if (data->pts[side][0].active) {
                OMM_MEMMOV(data->pts[side] + 1, data->pts[side], (OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS - 1) * sizeof(Tpt));
                data->pts[side][0].active = false;
            }
        }
    }

    // Make the buffer tail a non-active point
    for (s32 side = 0; side != 2; ++side) {
        data->pts[side][OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS - 1].active = false;
    }

    // Delete 1-point clusters
    for (s32 side = 0; side != 2; ++side) {
        for (s32 i = 1; i < OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS - 1; ++i) {
            if (!data->pts[side][i - 1].active && !data->pts[side][i + 1].active) {
                data->pts[side][i].active = false;
            }
        }
    }

    // Compute vertices and triangles
    Vtx *vtx = data->vtx;
    Gfx *tri = data->tri;
    for (s32 side = 0; side != 2; ++side) {
        for (s32 i = 0; i != OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS; ++i) {
            if (data->pts[side][i].active) {

                // Pre-compute some data
                struct { bool active; Vec3f pos; Vec3f dir; f32 rad; } tpts[3], *prev, *curr, *next;
                for (s32 di = -1; di <= +1; ++di) {
                    Tpt *ptPrev = &data->pts[side][clamp_s(i + di - 1, 0, OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS - 1)];
                    Tpt *ptCurr = &data->pts[side][clamp_s(i + di + 0, 0, OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS - 1)];
                    Tpt *ptNext = &data->pts[side][clamp_s(i + di + 1, 0, OMM_PEACH_VIBE_GLOOM_TEAR_MAX_POINTS - 1)];

                    // Active
                    tpts[1 + di].active = ptCurr->active;

                    // Position
                    vec3f_copy(tpts[1 + di].pos, ptCurr->pos);

                    // Direction
                    if (!ptPrev->active || !ptNext->active) {
                        vec3f_copy(tpts[1 + di].dir, ptCurr->vel);
                    } else {
                        vec3f_dif(tpts[1 + di].dir, ptNext->pos, ptPrev->pos);
                    }

                    // Radius
                    tpts[1 + di].rad = ptPrev->active *
                        invlerp_0_1_f(ptCurr->timer, 0, OMM_PEACH_VIBE_GLOOM_TEAR_NUM_FRAMES_SIZE_GROW) *
                        relerp_f(sins((gGlobalTimer - ptCurr->timer) * 0x2000), -1.f, +1.f, 0.8f, 1.2f);
                }
                prev = &tpts[0];
                curr = &tpts[1];
                next = &tpts[2];

                // Interpolate points
                s32 nInterpolated = ((prev->active && next->active) ? OMM_PEACH_VIBE_GLOOM_TEAR_NUM_INTERPOLATED_POINTS : 1);
                for (s32 j = 0; j != nInterpolated; ++j) {
                    f32 t0 = (f32) (j - 1) / (f32) nInterpolated;
                    f32 t1 = (f32) (j + 0) / (f32) nInterpolated;
                    f32 t2 = (f32) (j + 1) / (f32) nInterpolated;

                    // Position
                    Vec3f pos0; vec3f_interpolate3(pos0, t0, prev->pos, -1.f, curr->pos, 0.f, next->pos, 1.f);
                    Vec3f pos1; vec3f_interpolate3(pos1, t1, prev->pos, -1.f, curr->pos, 0.f, next->pos, 1.f);
                    Vec3f pos2; vec3f_interpolate3(pos2, t2, prev->pos, -1.f, curr->pos, 0.f, next->pos, 1.f);

                    // Direction
                    Vec3f dir = {
                        lerp_f(t1, curr->dir[0], next->dir[0]),
                        lerp_f(t1, curr->dir[1], next->dir[1]),
                        lerp_f(t1, curr->dir[2], next->dir[2])
                    };
                    s16 pitch, yaw;
                    vec3f_to_polar_coords(dir, NULL, &pitch, &yaw);

                    // Radius
                    f32 radius = lerp_f(t1, curr->rad, next->rad) * OMM_PEACH_VIBE_GLOOM_TEAR_RADIUS;

                    // Triangles
                    if (next->active) {
                        gSPVertex(tri++, vtx, 2 * OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT, 0);
                        for (s32 k = 0; k != OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT; ++k) {
                            s32 k0 = ((k + 0) % OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT);
                            s32 k1 = ((k + 1) % OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT);
                            s32 k2 = ((k + 0) % OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT) + OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT;
                            s32 k3 = ((k + 1) % OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT) + OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT;
                            gSP2Triangles(tri++, k2, k1, k0, 0, k1, k2, k3, 0);
                        }
                    }

                    // Vertices
                    for (s32 k = 0; k != OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT; ++k) {
                        s16 angle = (s16) ((k * 0x10000) / OMM_PEACH_VIBE_GLOOM_TEAR_NUM_VERTICES_PER_POINT);
                        Vec3f dv = { radius * coss(angle), -radius * sins(angle), 0.f };
                        vec3f_rotate_zxy(dv, dv, -pitch, yaw, 0);
                        vtx->n.ob[0] = pos1[0] + dv[0];
                        vtx->n.ob[1] = pos1[1] + dv[1];
                        vtx->n.ob[2] = pos1[2] + dv[2];
                        vtx->n.flag  = 0;
                        vtx->n.tc[0] = 0;
                        vtx->n.tc[1] = 0;
                        vtx->n.n[0]  = (s8) (dv[0] * 127.f / radius);
                        vtx->n.n[1]  = (s8) (dv[1] * 127.f / radius);
                        vtx->n.n[2]  = (s8) (dv[2] * 127.f / radius);
                        vtx->n.a     = OMM_PEACH_VIBE_GLOOM_TEAR_OPACITY * relerp_0_1_f(i, OMM_PEACH_VIBE_GLOOM_TEAR_OPACITY_MAX_POINT, OMM_PEACH_VIBE_GLOOM_TEAR_OPACITY_ZERO_POINT, 1.f, 0.f);
                        vtx++;
                    }
                }
            }
        }
    }
    gSPEndDisplayList(tri++);

    // Unload object if nothing is drawn
    if (vtx == data->vtx) {
        obj_mark_for_deletion(o);
        return;
    }

    // Update object
    obj_scale(o, 1.f);
    obj_set_pos(o, mp[0], mp[1], mp[2]);
    obj_set_vel(o, 0.f, 0.f, 0.f);
    obj_set_home(o, 0.f, 0.f, 0.f);
    obj_set_angle(o, 0, 0, 0);
}

const BehaviorScript omm_bhv_peach_vibe_gloom_tear[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_peach_vibe_gloom_tear_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_peach_vibe_gloom_tear(struct Object *o) {
    struct Object *tear = obj_get_first_with_behavior(omm_bhv_peach_vibe_gloom_tear);
    if (tear == NULL) {
        tear = obj_spawn_from_geo(o, omm_geo_peach_vibe_gloom_tear, omm_bhv_peach_vibe_gloom_tear);
        obj_set_always_rendered(tear, true);
        obj_scale(tear, 1.f);
    }
    return tear;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES               16
#define OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME     4
#define OMM_PEACH_VIBE_SPARKLE_NUM_POINTS               (OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES * OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME)
#define OMM_PEACH_VIBE_SPARKLE_OPACITY                  0xC0
#define OMM_PEACH_VIBE_SPARKLE_HALF_SIZE                64
#define OMM_PEACH_VIBE_SPARKLE_PHASE_1_DURATION         15
#define OMM_PEACH_VIBE_SPARKLE_PHASE_1_VEL_MULT         4.f
#define OMM_PEACH_VIBE_SPARKLE_PHASE_2_DURATION         60
#define OMM_PEACH_VIBE_SPARKLE_PHASE_2_VEL_MIN          0.f
#define OMM_PEACH_VIBE_SPARKLE_PHASE_2_VEL_MAX          150.f
#define OMM_PEACH_VIBE_SPARKLE_PHASE_2_TARGET_DIST      80.f

//
// Gfx data
//

static const Vtx omm_peach_vibe_sparkle_vertices[] = {
    { { { -OMM_PEACH_VIBE_SPARKLE_HALF_SIZE, -OMM_PEACH_VIBE_SPARKLE_HALF_SIZE, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { +OMM_PEACH_VIBE_SPARKLE_HALF_SIZE, -OMM_PEACH_VIBE_SPARKLE_HALF_SIZE, 0 }, 0, { 4096, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { +OMM_PEACH_VIBE_SPARKLE_HALF_SIZE, +OMM_PEACH_VIBE_SPARKLE_HALF_SIZE, 0 }, 0, { 4096, 4096 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -OMM_PEACH_VIBE_SPARKLE_HALF_SIZE, +OMM_PEACH_VIBE_SPARKLE_HALF_SIZE, 0 }, 0, { 0, 4096 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

static const Gfx omm_peach_vibe_sparkle_triangles[] = {
    gsSPVertex(omm_peach_vibe_sparkle_vertices, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_peach_vibe_sparkle_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_VIBE_SPARKLE, G_IM_FMT_RGBA,G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(null),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_peach_vibe_joy_sparkle_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_VIBE_JOY_SPARKLE, G_IM_FMT_RGBA,G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(null),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    Gfx gfx[OMM_ARRAY_SIZE(omm_peach_vibe_sparkle_gfx)];
    Gfx tri[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS * OMM_ARRAY_SIZE(omm_peach_vibe_sparkle_triangles)];
    Vtx vtx[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS * OMM_ARRAY_SIZE(omm_peach_vibe_sparkle_vertices)];
    Vec3f pos[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS];
    bool inited;
} OmmPeachVibeSparkleGeoData;

//
// Geo layouts
//

const GeoLayout omm_geo_peach_vibe_sparkle[] = {
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

static void omm_bhv_peach_vibe_sparkle_init_buffers(OmmPeachVibeSparkleGeoData *data, f32 *pos0) {
    if (!data->inited) {
        Vtx *vtx = data->vtx;
        Gfx *tri = data->tri;
        for (s32 i = 0; i != OMM_PEACH_VIBE_SPARKLE_NUM_POINTS; ++i) {

            // Init positions
            vec3f_copy(data->pos[i], pos0);

            // Init triangle buffer
            OMM_MEMCPY(tri, omm_peach_vibe_sparkle_triangles, sizeof(omm_peach_vibe_sparkle_triangles));
            gSPVertex(tri, vtx, OMM_ARRAY_SIZE(omm_peach_vibe_sparkle_vertices), 0);
            tri += (OMM_ARRAY_SIZE(omm_peach_vibe_sparkle_triangles) - 1);

            // Init vertex buffer
            OMM_MEMCPY(vtx, omm_peach_vibe_sparkle_vertices, sizeof(omm_peach_vibe_sparkle_vertices));
            vtx += OMM_ARRAY_SIZE(omm_peach_vibe_sparkle_vertices);
        }
        data->inited = true;
    }
}

static void omm_bhv_peach_vibe_sparkle_update() {
    struct Object *o = gCurrentObject;
    struct MarioState *m = gMarioState;
    OmmPeachVibeSparkleGeoData *data = NULL;

    // Unloading
    if (o->oSubAction > 0) {
        o->oSubAction++;
        if (o->oSubAction > OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES) {
            obj_mark_for_deletion(o);
            return;
        }
    }

    // Update position
    switch (o->oAction) {

        // Vibe sparkle
        case 0: {
            data = geo_get_geo_data(o, sizeof(OmmPeachVibeSparkleGeoData), omm_peach_vibe_sparkle_gfx, sizeof(omm_peach_vibe_sparkle_gfx));
            omm_bhv_peach_vibe_sparkle_init_buffers(data, &o->oParentRelativePosX);
            OMM_MEMMOV(data->pos + OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME, data->pos, sizeof(Vec3f) * OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME * (OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES - 1));
            f32 *pos = data->pos[0];

            // Phase 1: scatter
            if (o->oTimer < OMM_PEACH_VIBE_SPARKLE_PHASE_1_DURATION) {
                pos[0] += o->oVelX * (OMM_PEACH_VIBE_SPARKLE_PHASE_1_DURATION - o->oTimer) * OMM_PEACH_VIBE_SPARKLE_PHASE_1_VEL_MULT;
                pos[1] += o->oVelY * (OMM_PEACH_VIBE_SPARKLE_PHASE_1_DURATION - o->oTimer) * OMM_PEACH_VIBE_SPARKLE_PHASE_1_VEL_MULT;
                pos[2] += o->oVelZ * (OMM_PEACH_VIBE_SPARKLE_PHASE_1_DURATION - o->oTimer) * OMM_PEACH_VIBE_SPARKLE_PHASE_1_VEL_MULT;
            }
            
            // Phase 2: target and follow Peach
            else {
                Vec3f target = {
                    m->pos[0],
                    m->pos[1] + m->marioObj->hitboxHeight * m->marioObj->oScaleX / 2,
                    m->pos[2]
                };
                if (o->oSubAction == 0) {
                    vec3f_dif(&o->oVelX, target, pos);
                    vec3f_set_mag(&o->oVelX, relerp_0_1_f(o->oTimer,
                        OMM_PEACH_VIBE_SPARKLE_PHASE_1_DURATION,
                        OMM_PEACH_VIBE_SPARKLE_PHASE_2_DURATION,
                        OMM_PEACH_VIBE_SPARKLE_PHASE_2_VEL_MIN,
                        OMM_PEACH_VIBE_SPARKLE_PHASE_2_VEL_MAX)
                    );
                    pos[0] += o->oVelX;
                    pos[1] += o->oVelY;
                    pos[2] += o->oVelZ;

                    // When collected, a Vibe sparkle restores 5% of Vibe gauge
                    if (vec3f_dist(pos, target) < OMM_PEACH_VIBE_SPARKLE_PHASE_2_TARGET_DIST) {
                        gOmmData->mario->peach.vibeGauge = clamp_s(gOmmData->mario->peach.vibeGauge - OMM_PEACH_VIBE_GAUGE_SPARKLE_INC, 0, OMM_PEACH_VIBE_GAUGE_MAX);
                        omm_spawn_peach_vibe_aura(m->marioObj);
                        o->oSubAction = 1;
                    }
                } else {
                    vec3f_copy(pos, target);
                }
            }
        } break;

        // Joy sparkle
        case 1: {
            data = geo_get_geo_data(o, sizeof(OmmPeachVibeSparkleGeoData), omm_peach_vibe_joy_sparkle_gfx, sizeof(omm_peach_vibe_joy_sparkle_gfx));
            Vec3f pos = {
                o->oParentRelativePosX * sins(o->oMoveAngleYaw),
                o->oParentRelativePosY,
                o->oParentRelativePosX * coss(o->oMoveAngleYaw)
            };
            o->oMoveAngleYaw += o->oAngleVelYaw;
            omm_bhv_peach_vibe_sparkle_init_buffers(data, pos);
            OMM_MEMMOV(data->pos + OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME, data->pos, sizeof(Vec3f) * OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME * (OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES - 1));
            vec3f_copy(data->pos[0], pos);
            if (o->oSubAction == 0 && o->oTimer >= OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES) {
                o->oSubAction = 1;
            }
        } break;
    }

    // Compute interpolated points
    f32 *p0 = data->pos[0];
    f32 *p1 = data->pos[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME];
    f32 *p2 = data->pos[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME * 2];
    for (s32 i = 1; i < OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME; ++i) {
        f32 t = (f32) i / (f32) OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME;
        vec3f_interpolate3(data->pos[i], t, p0, 0.f, p1, 1.f, p2, 2.f);
    }

    // Billboard plane
    Vec3f camN, camE1, camE2;
    camN[0] = gCamera->pos[0] - (p0[0] + m->pos[0] * o->oAction);
    camN[1] = gCamera->pos[1] - (p0[1] + m->pos[1] * o->oAction);
    camN[2] = gCamera->pos[2] - (p0[2] + m->pos[2] * o->oAction);
    vec3f_get_nullspace(camN, camE1, camE2, camN);

    // Update vertices
    Vtx *vtx = data->vtx;
    for (s32 k = OMM_PEACH_VIBE_SPARKLE_NUM_POINTS; k != 0; --k, vtx += 4) {
        s32 i = k - 1;
        f32 t = (
            (i < o->oTimer * OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME) *             // Display loaded points only
            (1.f - ((f32) o->oSubAction / (f32) OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES)) *   // Progressively hide points during unloading
            (1.f - ((f32) i / (f32) OMM_PEACH_VIBE_SPARKLE_NUM_POINTS)) /               // The more recent, the more visible
            (i == 0 ? 1.f : 1.5f)                                                       // Reduce the scale and opacity of tail points
        );

        // Vertices
        f32 scale = t * (1.f - 0.2f * o->oAction);
        vec2f_to_3d_plane(vtx[0].v.ob, (Vec2f) { -OMM_PEACH_VIBE_SPARKLE_HALF_SIZE * scale, -OMM_PEACH_VIBE_SPARKLE_HALF_SIZE * scale }, data->pos[i], camE1, gVec3fOne, camE2, gVec3fOne);
        vec2f_to_3d_plane(vtx[1].v.ob, (Vec2f) { +OMM_PEACH_VIBE_SPARKLE_HALF_SIZE * scale, -OMM_PEACH_VIBE_SPARKLE_HALF_SIZE * scale }, data->pos[i], camE1, gVec3fOne, camE2, gVec3fOne);
        vec2f_to_3d_plane(vtx[2].v.ob, (Vec2f) { +OMM_PEACH_VIBE_SPARKLE_HALF_SIZE * scale, +OMM_PEACH_VIBE_SPARKLE_HALF_SIZE * scale }, data->pos[i], camE1, gVec3fOne, camE2, gVec3fOne);
        vec2f_to_3d_plane(vtx[3].v.ob, (Vec2f) { -OMM_PEACH_VIBE_SPARKLE_HALF_SIZE * scale, +OMM_PEACH_VIBE_SPARKLE_HALF_SIZE * scale }, data->pos[i], camE1, gVec3fOne, camE2, gVec3fOne);

        // Opacity
        f32 opacity = t * (i == 0 ? 1.f : 0.75f) * (o->oOpacity / 255.f);
        vtx[0].v.cn[3] = OMM_PEACH_VIBE_SPARKLE_OPACITY * opacity;
        vtx[1].v.cn[3] = OMM_PEACH_VIBE_SPARKLE_OPACITY * opacity;
        vtx[2].v.cn[3] = OMM_PEACH_VIBE_SPARKLE_OPACITY * opacity;
        vtx[3].v.cn[3] = OMM_PEACH_VIBE_SPARKLE_OPACITY * opacity;
    }

    // Update object's graphics
    obj_set_pos(o, m->pos[0] * o->oAction, m->pos[1] * o->oAction, m->pos[2] * o->oAction);
    vec3f_set((f32 *) &o->oFaceAnglePitch, 0, 0, 0);
    obj_set_scale(o, 1.f, 1.f, 1.f);
}

const BehaviorScript omm_bhv_peach_vibe_sparkle[] = {
    OBJ_TYPE_SPECIAL,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_peach_vibe_sparkle_update,
    0x09000000,
};

const BehaviorScript omm_bhv_peach_vibe_joy_sparkle[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_peach_vibe_sparkle_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_peach_vibe_sparkle(struct Object *o, f32 x, f32 y, f32 z) {
    struct Object *sparkle = obj_spawn_from_geo(o, omm_geo_peach_vibe_sparkle, omm_bhv_peach_vibe_sparkle);
    obj_set_pos(sparkle, 0, 0, 0);
    obj_set_vel(sparkle, random_f32_around_zero(2.5f), random_f32_around_zero(2.5f), random_f32_around_zero(2.5f));
    obj_set_angle(sparkle, 0, 0, 0);
    obj_set_scale(sparkle, 1.f, 1.f, 1.f);
    obj_set_always_rendered(sparkle, true);
    obj_update_gfx(sparkle);
    vec3f_normalize(&sparkle->oVelX);
    sparkle->oParentRelativePosX = x;
    sparkle->oParentRelativePosY = y;
    sparkle->oParentRelativePosZ = z;
    sparkle->oOpacity = 0xFF;
    sparkle->oAction = 0;
    sparkle->oSubAction = 0;
    return sparkle;
}

struct Object *omm_spawn_peach_vibe_joy_sparkle(struct Object *o, f32 radius, f32 height, s16 yaw, s32 yawVel, u8 opacity) {
    struct Object *sparkle = obj_spawn_from_geo(o, omm_geo_peach_vibe_sparkle, omm_bhv_peach_vibe_joy_sparkle);
    obj_set_angle(sparkle, 0, 0, 0);
    obj_set_scale(sparkle, 1.f, 1.f, 1.f);
    obj_set_always_rendered(sparkle, true);
    sparkle->oParentRelativePosX = radius;
    sparkle->oParentRelativePosY = height;
    sparkle->oMoveAngleYaw = yaw;
    sparkle->oAngleVelYaw = yawVel;
    sparkle->oOpacity = opacity;
    sparkle->oAction = 1;
    sparkle->oSubAction = 0;
    return sparkle;
}

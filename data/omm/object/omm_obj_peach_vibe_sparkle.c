#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#define OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES               (16)
#define OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME     (4)
#define OMM_PEACH_VIBE_SPARKLE_NUM_POINTS               (OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES * OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME)
#define OMM_PEACH_VIBE_SPARKLE_BASE_OPACITY             (0.75f)
#define OMM_PEACH_VIBE_SPARKLE_BILLBOARD_SIZE           (128)
#define OMM_PEACH_VIBE_SPARKLE_BILLBOARD_HALF_SIZE      (OMM_PEACH_VIBE_SPARKLE_BILLBOARD_SIZE / 2)
#define OMM_PEACH_VIBE_SPARKLE_PHASE_1_DURATION         (15)
#define OMM_PEACH_VIBE_SPARKLE_PHASE_1_VEL_MULT         (4.f)
#define OMM_PEACH_VIBE_SPARKLE_PHASE_2_DURATION         (60)
#define OMM_PEACH_VIBE_SPARKLE_PHASE_2_VEL_MIN          (0.f)
#define OMM_PEACH_VIBE_SPARKLE_PHASE_2_VEL_MAX          (150.f)
#define OMM_PEACH_VIBE_SPARKLE_PHASE_2_TARGET_DIST      (80.f)

//
// Gfx data
//

static const Gfx omm_peach_vibe_sparkle_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_VIBE_SPARKLE, G_IM_FMT_RGBA,G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(NULL),
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
    gsSPDisplayList(NULL),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    Gfx *displayLists[1];
    Gfx gfx[array_length(omm_peach_vibe_sparkle_gfx)];
#if OMM_GAME_IS_SM64
    Gfx tri[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS * 3 + 1];
    Vtx vtx[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS * 8];
#else
    Gfx tri[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS * 2 + 1];
    Vtx vtx[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS * 4];
#endif
    Vec3f pos[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS];
    bool inited;
} OmmPeachVibeSparkleGeoData;

static const u32 sOmmPeachVibeSparkleGeoDataDisplayListsOffsets[] = {
    offsetof(OmmPeachVibeSparkleGeoData, gfx),
};

//
// Geo layouts
//

static void omm_geo_peach_vibe_sparkle_draw_billboard(Vtx **vtx, Gfx **tri, Vec3f billboardPos, Vec3f objPos, f32 camOffset, s16 angle, f32 scale, f32 opacity) {
    *tri = gfx_create_billboard(*tri, *vtx,
        -OMM_PEACH_VIBE_SPARKLE_BILLBOARD_HALF_SIZE * scale,
        -OMM_PEACH_VIBE_SPARKLE_BILLBOARD_HALF_SIZE * scale,
        OMM_PEACH_VIBE_SPARKLE_BILLBOARD_SIZE * scale,
        OMM_PEACH_VIBE_SPARKLE_BILLBOARD_SIZE * scale,
        0, 0, 128, 128,
        billboardPos, objPos,
        camOffset, angle,
        0xFF, 0xFF, 0xFF, 0xFF * opacity
    );
    *vtx += 4;
#if OMM_GAME_IS_SM64
    if (gOmmGlobals->isMirrorRoom) {
        billboardPos[0] = 2 * gOmmGlobals->mirrorX - billboardPos[0];
        *tri = gfx_create_billboard(*tri, *vtx,
            -OMM_PEACH_VIBE_SPARKLE_BILLBOARD_HALF_SIZE * scale,
            -OMM_PEACH_VIBE_SPARKLE_BILLBOARD_HALF_SIZE * scale,
            OMM_PEACH_VIBE_SPARKLE_BILLBOARD_SIZE * scale,
            OMM_PEACH_VIBE_SPARKLE_BILLBOARD_SIZE * scale,
            0, 0, 128, 128,
            billboardPos, objPos,
            camOffset, angle,
            0xFF, 0xFF, 0xFF, 0xFF * opacity
        );
        *vtx += 4;
    }
#endif
}

static Gfx *omm_geo_peach_vibe_sparkle_update_gfx(s32 callContext, UNUSED struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER && gCurrGraphNodeObject) {
        struct Object *o = gCurrGraphNodeObject;
        OmmPeachVibeSparkleGeoData *data = o->oGeoData;
        if (data) {
            Vtx *vtx = data->vtx;
            Gfx *tri = data->tri;
            u16 r = srandom_u16(o->oTimer);
            for (s32 k = OMM_PEACH_VIBE_SPARKLE_NUM_POINTS; k != 0; --k) {
                s32 i = k - 1;
                f32 t = (
                    (i < o->oTimer * OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME) *             // Display loaded points only
                    (1.f - ((f32) o->oSubAction / (f32) OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES)) *   // Progressively hide points during unloading
                    (1.f - ((f32) i / (f32) OMM_PEACH_VIBE_SPARKLE_NUM_POINTS)) /               // The more recent, the more visible
                    (i == 0 ? 1.f : 1.5f)                                                       // Reduce the scale and opacity of tail points
                );
                f32 scale = t * (1.f - 0.2f * o->oAction);
                f32 opacity = t * (i == 0 ? 1.f : 0.75f) * (o->oOpacity / 255.f);
                Vec3f sparklePos; vec3f_sum(sparklePos, &o->oPosX, data->pos[i]);
                omm_geo_peach_vibe_sparkle_draw_billboard(&vtx, &tri, sparklePos, &o->oPosX, 0.f, (r = srandom_u16(r)), scale, OMM_PEACH_VIBE_SPARKLE_BASE_OPACITY * opacity);
            }
            gSPEndDisplayList(tri);
        }
    }
    return NULL;
}

const GeoLayout omm_geo_peach_vibe_sparkle[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(0, omm_geo_peach_vibe_sparkle_update_gfx),
#if OMM_GAME_IS_SM64
        GEO_ASM(0, geo_link_geo_data_skip_mirror_obj),
#else
        GEO_ASM(0, geo_link_geo_data),
#endif
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_peach_vibe_sparkle_init_pos(OmmPeachVibeSparkleGeoData *data, f32 *pos0) {
    if (!data->inited) {
        for (s32 i = 0; i != OMM_PEACH_VIBE_SPARKLE_NUM_POINTS; ++i) {
            vec3f_copy(data->pos[i], pos0);
        }
        data->inited = true;
    }
}

static void bhv_omm_peach_vibe_sparkle_update() {
    struct Object *o = gCurrentObject;
    struct MarioState *m = gMarioState;
    OmmPeachVibeSparkleGeoData *data = geo_get_geo_data(o,
        sizeof(OmmPeachVibeSparkleGeoData),
        sOmmPeachVibeSparkleGeoDataDisplayListsOffsets,
        array_length(sOmmPeachVibeSparkleGeoDataDisplayListsOffsets)
    );

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
            vec3f_zero(&o->oPosX);
            bhv_omm_peach_vibe_sparkle_init_pos(data, &o->oParentRelativePosX);
            gfx_copy_and_fill_null(data->gfx, omm_peach_vibe_sparkle_gfx, sizeof(omm_peach_vibe_sparkle_gfx), data->tri);
            mem_mov(data->pos + OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME, data->pos, sizeof(Vec3f) * OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME * (OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES - 1));
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
                        omm_peach_vibe_increase(m, OMM_PEACH_VIBE_GAUGE_SPARKLE_INC);
                        omm_obj_spawn_peach_vibe_aura(m->marioObj, false);
                        o->oSubAction = 1;
                    }
                } else {
                    vec3f_copy(pos, target);
                }
            }
        } break;

        // Joy sparkle
        case 1: {
            Vec3f pos = {
                o->oParentRelativePosX * sins(o->oMoveAngleYaw),
                o->oParentRelativePosY,
                o->oParentRelativePosZ * coss(o->oMoveAngleYaw),
            };
            o->oMoveAngleYaw += o->oAngleVelYaw;
            vec3f_copy(&o->oPosX, m->pos);
            bhv_omm_peach_vibe_sparkle_init_pos(data, pos);
            gfx_copy_and_fill_null(data->gfx, omm_peach_vibe_joy_sparkle_gfx, sizeof(omm_peach_vibe_joy_sparkle_gfx), data->tri);
            mem_mov(data->pos + OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME, data->pos, sizeof(Vec3f) * OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME * (OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES - 1));
            vec3f_copy(data->pos[0], pos);
            if (o->oSubAction == 0 && o->oTimer >= OMM_PEACH_VIBE_SPARKLE_NUM_FRAMES) {
                o->oSubAction = 1;
            }
        } break;

        // Not a sparkle
        default: {
            obj_mark_for_deletion(o);
        } return;
    }

    // Compute interpolated points
    f32 *p0 = data->pos[0];
    f32 *p1 = data->pos[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME];
    f32 *p2 = data->pos[OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME * 2];
    for (s32 i = 1; i < OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME; ++i) {
        f32 t = (f32) i / (f32) OMM_PEACH_VIBE_SPARKLE_NUM_POINTS_PER_FRAME;
        vec3f_interpolate3(data->pos[i], t, p0, 0.f, p1, 1.f, p2, 2.f);
    }

    // Update gfx
    // omm_geo_peach_vibe_sparkle_update_gfx
    gSPEndDisplayList(data->tri);

    // Update object's graphics
    o->oFaceAnglePitch = 0;
    o->oFaceAngleYaw = 0;
    o->oFaceAngleRoll = 0;
    obj_scale(o, 1.f);
}

const BehaviorScript bhvOmmPeachVibeSparkle[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_peach_vibe_sparkle_update),
    BHV_END_LOOP(),
};

const BehaviorScript bhvOmmPeachVibeJoySparkle[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_peach_vibe_sparkle_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_peach_vibe_sparkle(struct Object *o, f32 x, f32 y, f32 z) {
    if (OMM_PLAYER_IS_PEACH) {
        struct Object *sparkle = obj_spawn_from_geo(o, omm_geo_peach_vibe_sparkle, bhvOmmPeachVibeSparkle);
        obj_set_xyz(sparkle, 0, 0, 0);
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
    return NULL;
}

struct Object *omm_obj_spawn_peach_vibe_joy_sparkle(struct Object *o, f32 radius, f32 height, s16 yaw, s32 yawVel, u8 opacity) {
    struct Object *sparkle = obj_spawn_from_geo(o, omm_geo_peach_vibe_sparkle, bhvOmmPeachVibeJoySparkle);
    obj_set_angle(sparkle, 0, 0, 0);
    obj_set_scale(sparkle, 1.f, 1.f, 1.f);
    obj_set_always_rendered(sparkle, true);
    sparkle->oParentRelativePosX = radius;
    sparkle->oParentRelativePosY = height;
    sparkle->oParentRelativePosZ = radius;
    sparkle->oMoveAngleYaw = yaw;
    sparkle->oAngleVelYaw = yawVel;
    sparkle->oOpacity = opacity;
    sparkle->oAction = 1;
    sparkle->oSubAction = 0;
    return sparkle;
}

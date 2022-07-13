#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_PEACH_VIBE_RAGE_AURA_POINTS_PER_SEGMENT   30
#define OMM_PEACH_VIBE_RAGE_AURA_NUM_SEGMENTS         30
#define OMM_PEACH_VIBE_RAGE_AURA_RADIUS              120,  5, 0xA59
#define OMM_PEACH_VIBE_RAGE_AURA_Y_MIN              -100,  8, 0x1E3
#define OMM_PEACH_VIBE_RAGE_AURA_Y_MID                20, 12, 0x814
#define OMM_PEACH_VIBE_RAGE_AURA_Y_MAX               200, 10, 0x3F7
#define OMM_PEACH_VIBE_RAGE_AURA_Y_MIN_OPACITY        10,  0, 0x000
#define OMM_PEACH_VIBE_RAGE_AURA_Y_MID_OPACITY         6,  1, 0x152
#define OMM_PEACH_VIBE_RAGE_AURA_Y_MAX_OPACITY         0,  0, 0x000

//
// Gfx data
//

static const Gfx omm_peach_vibe_rage_aura_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_VIBE_RAGE_AURA, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(null),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    Gfx gfx[OMM_ARRAY_SIZE(omm_peach_vibe_rage_aura_gfx)];
    Gfx tri[(OMM_PEACH_VIBE_RAGE_AURA_NUM_SEGMENTS + 0) * (OMM_PEACH_VIBE_RAGE_AURA_POINTS_PER_SEGMENT + 1) + 1];
    Vtx vtx[(OMM_PEACH_VIBE_RAGE_AURA_NUM_SEGMENTS + 1) * (OMM_PEACH_VIBE_RAGE_AURA_POINTS_PER_SEGMENT + 1)];
} OmmPeachVibeRageAuraGeoData;

//
// Geo layout
//

const GeoLayout omm_geo_peach_vibe_rage_aura[] = {
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

static f32 oscillate(f32 base, f32 gap, s16 freq) {
    return base + coss(gCurrentObject->oTimer * freq) * gap;
}

static f32 oscillate_min_mid_max(f32 t,
    f32 baseMin, f32 gapMin, s16 freqMin,
    f32 baseMid, f32 gapMid, s16 freqMid,
    f32 baseMax, f32 gapMax, s16 freqMax) {
    return (t < 0.5f) ?
        lerp_f(1.f - coss(t * 0x8000), oscillate(baseMin, gapMin, freqMin), oscillate(baseMid, gapMid, freqMid)) :
        lerp_f(0.f - coss(t * 0x8000), oscillate(baseMid, gapMid, freqMid), oscillate(baseMax, gapMax, freqMax));
}

static void omm_bhv_peach_vibe_rage_aura_update() {
    struct Object *o = gCurrentObject;
    OmmPeachVibeRageAuraGeoData *data = geo_get_geo_data(o, sizeof(OmmPeachVibeRageAuraGeoData), omm_peach_vibe_rage_aura_gfx, sizeof(omm_peach_vibe_rage_aura_gfx));

    // Global state
    s32 isRage = omm_peach_vibe_is_rage();
    o->oTimer *= (o->oAction == isRage);
    o->oAction = isRage;
    f32 alpha  = invlerp_0_1_s(o->oTimer, 15 * !isRage, 15 * isRage);

    // Vertices and triangles
    Vtx *vtx = data->vtx;
    Gfx *tri = data->tri;
    for (s32 i = 0; i <= OMM_PEACH_VIBE_RAGE_AURA_NUM_SEGMENTS; ++i) {
        f32 t = (f32) i / (f32) OMM_PEACH_VIBE_RAGE_AURA_NUM_SEGMENTS;
        f32 r = oscillate(OMM_PEACH_VIBE_RAGE_AURA_RADIUS) * sins(t * 0x8000) * gMarioObject->oScaleX;
        f32 y = oscillate_min_mid_max(t, OMM_PEACH_VIBE_RAGE_AURA_Y_MIN, OMM_PEACH_VIBE_RAGE_AURA_Y_MID, OMM_PEACH_VIBE_RAGE_AURA_Y_MAX) * gMarioObject->oScaleY;
        f32 op = sqr_f(oscillate_min_mid_max(t, OMM_PEACH_VIBE_RAGE_AURA_Y_MIN_OPACITY, OMM_PEACH_VIBE_RAGE_AURA_Y_MID_OPACITY, OMM_PEACH_VIBE_RAGE_AURA_Y_MAX_OPACITY) / 10.f);
        f32 tu = relerp_0_1_f(o->parentObj->oTimer % 60, 0, 60, 1.f, 0.f);
        f32 tv = relerp_0_1_f(o->parentObj->oTimer % 60, 0, 60, 1.f, 0.f) + t;

        // Vertex buffer
        // Current segment + next segment
        if (i < OMM_PEACH_VIBE_RAGE_AURA_NUM_SEGMENTS) {
            gSPVertex(tri++, vtx, 2 * (OMM_PEACH_VIBE_RAGE_AURA_POINTS_PER_SEGMENT + 1), 0);
        }

        // Vertices
        for (s32 j = 0; j <= OMM_PEACH_VIBE_RAGE_AURA_POINTS_PER_SEGMENT; ++j) {
            s32 a = (j * 0x10000) / OMM_PEACH_VIBE_RAGE_AURA_POINTS_PER_SEGMENT;

            // Vertices
            vtx->v.ob[0] = r * sins(a);
            vtx->v.ob[1] = y;
            vtx->v.ob[2] = r * coss(a);
            vtx->v.flag  = 0;
            vtx->v.tc[0] = (0x2000 * tu) + (a / 4);
            vtx->v.tc[1] = (0x2000 * tv);
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0xFF * op * alpha;

            // Triangles
            if (i < OMM_PEACH_VIBE_RAGE_AURA_NUM_SEGMENTS && j < OMM_PEACH_VIBE_RAGE_AURA_POINTS_PER_SEGMENT) {
                s32 p0 = j;
                s32 p1 = j + 1;
                s32 p2 = j + 1 + OMM_PEACH_VIBE_RAGE_AURA_POINTS_PER_SEGMENT;
                s32 p3 = j + 2 + OMM_PEACH_VIBE_RAGE_AURA_POINTS_PER_SEGMENT;
                gSP2Triangles(tri++, p0, p1, p3, 0, p0, p3, p2, 0);
            }
            vtx++;
        }
    }
    gSPEndDisplayList(tri);

    // Unload after deactivation
    if (!isRage && alpha == 0.f) {
        obj_mark_for_deletion(o);
        return;
    }

    // Update object
    f32 *marioRootPos = geo_get_marios_root_pos();
    obj_set_pos(o, marioRootPos[0], marioRootPos[1], marioRootPos[2]);
    obj_set_home(o, marioRootPos[0], marioRootPos[1], marioRootPos[2]);
    obj_set_angle(o, 0, 0, 0);
    obj_set_scale(o, 1.f, 1.f, 1.f);

    // Process interactions
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 100, 200, 0, 0, 0, 80);
    omm_obj_process_interactions(o, OBJ_INT_PRESET_PEACH_VIBE_RAGE_AURA);
}

const BehaviorScript omm_bhv_peach_vibe_rage_aura[] = {
    OBJ_TYPE_SPECIAL,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_peach_vibe_rage_aura_update,
    0x09000000
};

//
// Spawner
//

struct Object *omm_spawn_peach_vibe_rage_aura(struct Object *o) {
    struct Object *aura = obj_get_first_with_behavior(omm_bhv_peach_vibe_rage_aura);
    if (aura == NULL) {
        aura = obj_spawn_from_geo(o, omm_geo_peach_vibe_rage_aura, omm_bhv_peach_vibe_rage_aura);
        obj_set_always_rendered(aura, true);
        obj_set_angle(aura, 0, 0, 0);
        obj_scale(aura, 1.f);
    }
    return aura;
}

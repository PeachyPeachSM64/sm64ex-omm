#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_PERRY_BLAST_POINTS_PER_SEGMENT  30
#define OMM_PERRY_BLAST_NUM_SEGMENTS        30
#define OMM_PERRY_BLAST_DURATION            15

//
// Gfx data
//

static const Gfx omm_perry_blast_default_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_PERRY_BLAST, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_perry_blast_rage_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_PERRY_BLAST_RAGE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_perry_blast_gloom_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_PERRY_BLAST_GLOOM, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_perry_blast_calm_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_PERRY_BLAST_CALM, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_perry_blast_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
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
    Gfx gfx[OMM_ARRAY_SIZE(omm_perry_blast_gfx)];
    Gfx tri[(OMM_PERRY_BLAST_NUM_SEGMENTS + 0) * (OMM_PERRY_BLAST_POINTS_PER_SEGMENT + 1) + 1];
    Vtx vtx[(OMM_PERRY_BLAST_NUM_SEGMENTS + 1) * (OMM_PERRY_BLAST_POINTS_PER_SEGMENT + 1)];
} OmmPerryBigShotGeoData;

//
// Geo layout
//

const GeoLayout omm_geo_perry_blast[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SWITCH_CASE(OMM_PEACH_VIBE_TYPE_COUNT, geo_switch_anim_state),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_perry_blast_default_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_perry_blast_default_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_perry_blast_rage_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_perry_blast_gloom_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_perry_blast_calm_gfx),
        GEO_CLOSE_NODE(),
        GEO_TRANSLATE_NODE(LAYER_TRANSPARENT, 0, 0, 0),
        GEO_OPEN_NODE(),
            GEO_ASM(0, geo_link_geo_data),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_perry_blast_update() {
    struct Object *o = gCurrentObject;
    if (o->oTimer >= OMM_PERRY_BLAST_DURATION) {
        obj_mark_for_deletion(o);
        return;
    }

    // Update gfx
    OmmPerryBigShotGeoData *data = geo_get_geo_data(o, sizeof(OmmPerryBigShotGeoData), omm_perry_blast_gfx, sizeof(omm_perry_blast_gfx));
    f32 scale = relerp_0_1_f(o->oTimer, 0, OMM_PERRY_BLAST_DURATION, 0.f, 1.f) * OMM_PERRY_BLAST_RADIUS;
    f32 alpha = relerp_0_1_f(o->oTimer, 5, OMM_PERRY_BLAST_DURATION, 1.f, 0.f);
    obj_set_angle(o, 0, 0, 0);
    obj_set_scale(o, 1, 1, 1);
    
    // Vertices and triangles
    Vtx *vtx = data->vtx;
    Gfx *tri = data->tri;
    for (s32 i = 0; i <= OMM_PERRY_BLAST_NUM_SEGMENTS; ++i) {
        f32 t = (f32) i / (f32) OMM_PERRY_BLAST_NUM_SEGMENTS;
        f32 r = scale * sins(t * 0x8000);
        f32 y = scale * -coss(t * 0x8000);
        f32 tu = relerp_0_1_f((o->oTimer + o->oAction) % 20, 0, 20, 1.f, 0.f);
        f32 tv = relerp_0_1_f((o->oTimer + o->oAction) % 30, 0, 30, 1.f, 0.f) + t;

        // Vertex buffer
        // Current segment + next segment
        if (i < OMM_PERRY_BLAST_NUM_SEGMENTS) {
            gSPVertex(tri++, vtx, 2 * (OMM_PERRY_BLAST_POINTS_PER_SEGMENT + 1), 0);
        }

        // Vertices
        for (s32 j = 0; j <= OMM_PERRY_BLAST_POINTS_PER_SEGMENT; ++j) {
            s32 a = (j * 0x10000) / OMM_PERRY_BLAST_POINTS_PER_SEGMENT;

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
            vtx->v.cn[3] = 0xFF * alpha;

            // Triangles
            if (i < OMM_PERRY_BLAST_NUM_SEGMENTS && j < OMM_PERRY_BLAST_POINTS_PER_SEGMENT) {
                s32 p0 = j;
                s32 p1 = j + 1;
                s32 p2 = j + 1 + OMM_PERRY_BLAST_POINTS_PER_SEGMENT;
                s32 p3 = j + 2 + OMM_PERRY_BLAST_POINTS_PER_SEGMENT;
                gSP2Triangles(tri++, p0, p1, p3, 0, p0, p3, p2, 0);
            }
            vtx++;
        }
    }
    gSPEndDisplayList(tri);

    // Process interactions
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, scale, scale * 2.f, 0, 0, 0, scale);
    omm_obj_process_interactions(o, OMM_PERRY_BLAST_INT_FLAGS);
}

const BehaviorScript omm_bhv_perry_blast[] = {
    OBJ_TYPE_SPECIAL,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_perry_blast_update,
    0x09000000
};

//
// Spawner
//

struct Object *omm_spawn_perry_blast(struct Object *o, s32 type) {
    struct Object *blast = obj_spawn_from_geo(o, omm_geo_perry_blast, omm_bhv_perry_blast);
    obj_set_always_rendered(blast, true);
    blast->oPerryType = type;
    blast->oAction = random_u16();
    return blast;
}

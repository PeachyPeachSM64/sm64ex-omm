#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static Vtx omm_bowser_mad_aura_vtx[256];
static Gfx omm_bowser_mad_aura_tri[33];

static void omm_bowser_mad_aura_make_cylinder(Vtx *vtx, Gfx *tri, s16 u0, s16 v0, s16 u1, s16 v1) {
    for (s32 i = 0; i != 32; ++i) {

        // Vertices
        s16 a = 0x4000 + (i * 0x800);
        f32 x = sins(a) * 400.f;
        f32 z = coss(a) * 400.f;
        f32 t = i / 16.f;
        if (t > 1.f) t = 2.f - t;
        s16 u = (s16) lerp_f(t, u0, u1);
        vtx[2 * i + 0] = (Vtx) { { { x,    0, z }, 0, { u, v0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
        vtx[2 * i + 1] = (Vtx) { { { x, 1500, z }, 0, { u, v1 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };

        // Triangles
        s32 v0 = (2 * i + 0) % 64;
        s32 v1 = (2 * i + 1) % 64;
        s32 v2 = (2 * i + 2) % 64;
        s32 v3 = (2 * i + 3) % 64;
        gSP2Triangles(tri + i, v2, v1, v0, 0, v1, v2, v3, 0);
    }
    gSPEndDisplayList(tri + 32);
}

OMM_AT_STARTUP static void omm_bowser_mad_aura_init_vertices_and_triangles() {
    omm_bowser_mad_aura_make_cylinder(omm_bowser_mad_aura_vtx +   0, omm_bowser_mad_aura_tri,  512, 2944, 2560,    0);
    omm_bowser_mad_aura_make_cylinder(omm_bowser_mad_aura_vtx +  64, omm_bowser_mad_aura_tri, 3584, 2944, 5632,    0);
    omm_bowser_mad_aura_make_cylinder(omm_bowser_mad_aura_vtx + 128, omm_bowser_mad_aura_tri,  512, 6016, 2560, 3072);
    omm_bowser_mad_aura_make_cylinder(omm_bowser_mad_aura_vtx + 192, omm_bowser_mad_aura_tri, 3584, 6016, 5632, 3072);
}

static const Gfx omm_bowser_mad_aura_gfx_begin[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_MAD_AURA, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_mad_aura_gfx_end[] = {
    gsSPDisplayList(omm_bowser_mad_aura_tri),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_mad_aura_0_gfx[] = {
    gsSPDisplayList(omm_bowser_mad_aura_gfx_begin),
    gsSPVertex(omm_bowser_mad_aura_vtx + 0, 64, 0),
    gsSPDisplayList(omm_bowser_mad_aura_gfx_end),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_mad_aura_1_gfx[] = {
    gsSPDisplayList(omm_bowser_mad_aura_gfx_begin),
    gsSPVertex(omm_bowser_mad_aura_vtx + 64, 64, 0),
    gsSPDisplayList(omm_bowser_mad_aura_gfx_end),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_mad_aura_2_gfx[] = {
    gsSPDisplayList(omm_bowser_mad_aura_gfx_begin),
    gsSPVertex(omm_bowser_mad_aura_vtx + 128, 64, 0),
    gsSPDisplayList(omm_bowser_mad_aura_gfx_end),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_mad_aura_3_gfx[] = {
    gsSPDisplayList(omm_bowser_mad_aura_gfx_begin),
    gsSPVertex(omm_bowser_mad_aura_vtx + 192, 64, 0),
    gsSPDisplayList(omm_bowser_mad_aura_gfx_end),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

static Gfx *omm_geo_bowser_mad_aura_update_transparency(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    Gfx *gfx = NULL, *head = NULL;
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *asmNode = (struct GraphNodeGenerated *) node;
        asmNode->fnNode.node.flags = GRAPH_NODE_TYPE_FUNCTIONAL | (GRAPH_NODE_TYPE_400 * (gCurrGraphNodeObject->oOpacity != 0xFF)) | (asmNode->fnNode.node.flags & 0xFF);
        head = gfx = alloc_display_list(sizeof(Gfx) * 3);
        gDPSetAlphaCompare(head++, G_AC_NONE);
        gDPSetEnvColor(head++, 0xFF, 0xFF, 0xFF, gCurrGraphNodeObject->oOpacity);
        gSPEndDisplayList(head);
    }
    return gfx;
}

const GeoLayout omm_geo_bowser_mad_aura[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(1, omm_geo_bowser_mad_aura_update_transparency),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(4, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_mad_aura_0_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_mad_aura_1_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_mad_aura_2_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_mad_aura_3_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

const BehaviorScript omm_bhv_bowser_mad_aura[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0F1A0001,
    0x09000000
};

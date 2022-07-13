#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Vtx omm_break_particle_vertices[] = {
    { { { -32, 0, 0 }, 0, { 0, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 32, 0, 0 }, 0, { 992, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 32, 64, 0 }, 0, { 992, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -32, 64, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

static const Gfx omm_break_particle_triangles[] = {
    gsSPVertex(omm_break_particle_vertices, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_break_particle_shared_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(omm_break_particle_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_break_particle_1_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_BREAK_PARTICLE_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_break_particle_shared_gfx),
};

static const Gfx omm_break_particle_2_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_BREAK_PARTICLE_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_break_particle_shared_gfx),
};

static const Gfx omm_break_particle_3_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_BREAK_PARTICLE_3, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_break_particle_shared_gfx),
};

static const Gfx omm_break_particle_4_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_BREAK_PARTICLE_4, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_break_particle_shared_gfx),
};

static const Gfx omm_break_particle_5_gfx[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_BREAK_PARTICLE_5, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPBranchList(omm_break_particle_shared_gfx),
};

static Gfx omm_break_particle_set_env_color[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

static const Gfx omm_break_particle_reset_env_color[] = {
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsSPEndDisplayList(),
};

//
// Geo functions
//

static Gfx *omm_geo_break_particle_update_color(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        u8 r = (u8) ((gCurrGraphNodeObject->oAction >> 16) & 0xFF);
        u8 g = (u8) ((gCurrGraphNodeObject->oAction >>  8) & 0xFF);
        u8 b = (u8) ((gCurrGraphNodeObject->oAction >>  0) & 0xFF);
        struct GraphNodeDisplayList *displayListNode = (struct GraphNodeDisplayList *) node->next;
        ((Gfx *) displayListNode->displayList)->words.w1 = (uintptr_t) (((r & 0xFF) << 24llu) | ((g & 0xFF) << 16llu) | ((b & 0xFF) << 8llu) | 0xFF);
    }
    return NULL;
}

//
// Geo layout
//

const GeoLayout omm_geo_break_particle[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_break_particle_update_color),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_set_env_color),
            GEO_OPEN_NODE(),
                GEO_SWITCH_CASE(5, geo_switch_anim_state),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_1_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_2_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_3_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_4_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_5_gfx),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_reset_env_color),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_break_particle_update() {
    struct Object *o = gCurrentObject;
    o->oPosX += o->oVelX;
    o->oPosY += o->oVelY;
    o->oPosZ += o->oVelZ;
    if (o->oTimer >= 24) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript omm_bhv_break_particle[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_break_particle_update,
    0x09000000
};

//
// Spawner
//

struct Object *omm_spawn_break_particle(struct Object *o, u8 r, u8 g, u8 b, f32 offsetY, f32 velMin, f32 velMax, f32 velY, f32 scaleMin, f32 scaleMax) {
    f32 fvel                = velMin + (velMax - velMin) * random_float();
    s16 fangle              = random_u16();
    struct Object *particle = obj_spawn_from_geo(o, omm_geo_break_particle, omm_bhv_break_particle);
    particle->oAnimState    = random_u16() % 5;
    particle->oPosX         = o->oPosX;
    particle->oPosY         = o->oPosY + offsetY;
    particle->oPosZ         = o->oPosZ;
    particle->oVelX         = fvel * coss(fangle);
    particle->oVelZ         = fvel * sins(fangle);
    particle->oVelY         = random_f32_around_zero(2.f * velY);
    particle->oGravity      = 0;
    particle->oOpacity      = 255;
    particle->oAction       = (r << 16) | (g << 8) | (b << 0);
    obj_scale(particle, scaleMin + (scaleMax - scaleMin) * random_float());
    return particle;
}

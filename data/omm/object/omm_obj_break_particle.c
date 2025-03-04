#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Vtx omm_break_particle_vertices[] = {
    OMM_VTX_RECT(32, 0, 0, 2, 4),
    OMM_VTX_RECT(32, 0, 1, 2, 4),
    OMM_VTX_RECT(32, 0, 2, 2, 4),
    OMM_VTX_RECT(32, 0, 3, 2, 4),
    OMM_VTX_RECT(32, 1, 0, 2, 4),
    OMM_VTX_RECT(32, 1, 1, 2, 4),
    OMM_VTX_RECT(32, 1, 2, 2, 4),
    OMM_VTX_RECT(32, 1, 3, 2, 4),
};

static const Gfx omm_break_particle_triangles[] = {
    OMM_GFX_QUAD(omm_break_particle_vertices, 0),
    OMM_GFX_QUAD(omm_break_particle_vertices, 1),
    OMM_GFX_QUAD(omm_break_particle_vertices, 2),
    OMM_GFX_QUAD(omm_break_particle_vertices, 3),
    OMM_GFX_QUAD(omm_break_particle_vertices, 4),
    OMM_GFX_QUAD(omm_break_particle_vertices, 5),
    OMM_GFX_QUAD(omm_break_particle_vertices, 6),
    OMM_GFX_QUAD(omm_break_particle_vertices, 7),
};

OMM_GFX_DL_COMB(omm_break_particle, OMM_TEXTURE_EFFECT_BREAK_PARTICLE, omm_break_particle_triangles, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
OMM_GFX_DL_COMB(omm_break_particle, OMM_TEXTURE_EFFECT_BREAK_PARTICLE, omm_break_particle_triangles, 1, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
OMM_GFX_DL_COMB(omm_break_particle, OMM_TEXTURE_EFFECT_BREAK_PARTICLE, omm_break_particle_triangles, 2, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
OMM_GFX_DL_COMB(omm_break_particle, OMM_TEXTURE_EFFECT_BREAK_PARTICLE, omm_break_particle_triangles, 3, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
OMM_GFX_DL_COMB(omm_break_particle, OMM_TEXTURE_EFFECT_BREAK_PARTICLE, omm_break_particle_triangles, 4, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
OMM_GFX_DL_COMB(omm_break_particle, OMM_TEXTURE_EFFECT_BREAK_PARTICLE, omm_break_particle_triangles, 5, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
OMM_GFX_DL_COMB(omm_break_particle, OMM_TEXTURE_EFFECT_BREAK_PARTICLE, omm_break_particle_triangles, 6, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
OMM_GFX_DL_COMB(omm_break_particle, OMM_TEXTURE_EFFECT_BREAK_PARTICLE, omm_break_particle_triangles, 7, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);

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
                GEO_SWITCH_CASE(8, geo_switch_anim_state),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_0_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_1_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_2_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_3_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_4_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_5_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_6_gfx),
                    GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_break_particle_7_gfx),
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

static void bhv_omm_break_particle_update() {
    struct Object *o = gCurrentObject;
    o->oPosX += o->oVelX;
    o->oPosY += o->oVelY;
    o->oPosZ += o->oVelZ;
    if (o->oTimer >= 24) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmBreakParticle[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_break_particle_update),
    BHV_END_LOOP()
};

//
// Spawner
//

struct Object *omm_obj_spawn_break_particle(struct Object *o, u8 r, u8 g, u8 b, f32 offsetY, f32 velMin, f32 velMax, f32 velY, f32 scaleMin, f32 scaleMax) {
    f32 fvel                = velMin + (velMax - velMin) * random_float();
    s16 fangle              = random_u16();
    struct Object *particle = obj_spawn_from_geo(o, omm_geo_break_particle, bhvOmmBreakParticle);
    particle->oAnimState    = random_u16() % 8;
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

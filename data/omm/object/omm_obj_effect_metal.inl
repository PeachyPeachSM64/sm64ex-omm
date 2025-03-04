#ifdef GFX_PC_C
#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

static const Lights1 omm_obj_effect_metal_light = gdSPDefLights1(
    0x7F, 0x7F, 0x7F,
    0xFF, 0xFF, 0xFF,
    0x28, 0x28, 0x28
);

//
// Custom commands
//

static void omm_obj_effect_metal_gfx_nop() {}

//
// Display lists
//

static const Gfx omm_obj_effect_metal_gfx_enable[] = {

    // Enable metal texture and effect for the model
    gsSPSetGeometryMode(G_TEXTURE_GEN | G_LIGHTING),
    gsDPSetCombineMode(G_CC_MODULATERGBFADE, G_CC_MODULATERGBFADE),
    gsSPTexture(0x0F80, 0x07C0, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_obj_effect_metal_light.l, 1),
    gsSPLight(&omm_obj_effect_metal_light.a, 2),

    // Disable commands that could interfere
    gsXPSwapCmd(G_COPYMEM, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_TEXTURE, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_GEOMETRYMODE, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_MOVEMEM, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_SETOTHERMODE_L, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_SETOTHERMODE_H, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_SETTILESIZE, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_LOADBLOCK, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_LOADTILE, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_SETTILE, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_SETCOMBINE, omm_obj_effect_metal_gfx_nop),
    gsXPSwapCmd(G_SETTIMG, omm_obj_effect_metal_gfx_nop),
    gsSPEndDisplayList(),
};

static const Gfx omm_obj_effect_metal_gfx_enable_metal[] = {
    gsDPLoadTextureBlock(OMM_ASSET_METAL, G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32, 0, 0, 0, 6, 5, 0, 0),
    gsSPBranchList(omm_obj_effect_metal_gfx_enable),
};

static const Gfx omm_obj_effect_metal_gfx_enable_pink_gold[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_17, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 6, 6, 0, 0),
    gsSPBranchList(omm_obj_effect_metal_gfx_enable),
};

static const Gfx omm_obj_effect_metal_gfx_enable_crystal[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_18, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 6, 6, 0, 0),
    gsSPBranchList(omm_obj_effect_metal_gfx_enable),
};

static const Gfx omm_obj_effect_metal_gfx_enable_nebula[] = {
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_19, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 6, 6, 0, 0),
    gsSPBranchList(omm_obj_effect_metal_gfx_enable),
};

static const Gfx omm_obj_effect_metal_gfx_disable[] = {

    // Restore commands
    gsXPSwapCmd(G_COPYMEM, NULL),
    gsXPSwapCmd(G_TEXTURE, NULL),
    gsXPSwapCmd(G_GEOMETRYMODE, NULL),
    gsXPSwapCmd(G_MOVEMEM, NULL),
    gsXPSwapCmd(G_SETOTHERMODE_L, NULL),
    gsXPSwapCmd(G_SETOTHERMODE_H, NULL),
    gsXPSwapCmd(G_SETTILESIZE, NULL),
    gsXPSwapCmd(G_LOADBLOCK, NULL),
    gsXPSwapCmd(G_LOADTILE, NULL),
    gsXPSwapCmd(G_SETTILE, NULL),
    gsXPSwapCmd(G_SETCOMBINE, NULL),
    gsXPSwapCmd(G_SETTIMG, NULL),

    // Disable metal texture
    gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPTexture(0x0F80, 0x07C0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

//
// Geo functions
//

static const Gfx *omm_obj_effect_metal_enable_gfx(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *generated = (struct GraphNodeGenerated *) node;
        generated->fnNode.node.flags = (generated->fnNode.node.flags & 0xFF) | GRAPH_NODE_TYPE_FUNCTIONAL | (GRAPH_NODE_TYPE_400 * ((generated->parameter & 0xFF) != 0xFF));
        switch (generated->parameter >> 8) {
            case 0: return omm_obj_effect_metal_gfx_disable;
            case 1: return omm_obj_effect_metal_gfx_enable_metal;
            case 2: return omm_obj_effect_metal_gfx_enable_pink_gold;
            case 3: return omm_obj_effect_metal_gfx_enable_crystal;
            case 4: return omm_obj_effect_metal_gfx_enable_nebula;
        }
    }
    return NULL;
}

static bool omm_obj_effect_metal_get_parameter(struct Object *obj, bool enable, u32 *parameter) {
    struct MarioState *m = gMarioState;

    // Metal Yoshi (and tongue)
    if ((omm_obj_is_playable_yoshi(obj) || omm_obj_is_yoshi_tongue(obj)) &&     // Yoshi
        omm_mario_has_metal_cap(m) && !omm_mario_cap_is_flickering(m)           // Metal Mario
    ) {
        *parameter = ((enable ? 1 : 0) << 8) | 0xFF;
        return true;
    }

    // Sparkly Bowser (Bowser 4)
    if ((obj_has_geo_layout(obj, bowser_geo) || obj_has_geo_layout(obj, bowser2_geo)) &&    // Bowser
        (obj->behavior != bhvBowser || obj->oAction != 4) &&                                // Not dead
        omm_sparkly_is_bowser_4_battle()                                                    // Bowser 4
    ) {
        *parameter = ((enable ? gOmmSparklyMode + 1 : 0) << 8) | (obj->oOpacity & 0xFF);
        return true;
    }

    return false;
}

static struct GraphNodeGenerated *omm_obj_effect_metal_process_graph_node(struct Object *obj, bool enable) {
    u32 parameter;
    if (omm_obj_effect_metal_get_parameter(obj, enable, &parameter)) {
        static struct GraphNodeGenerated sOmmObjEffectMetalNodes[2];
        struct GraphNodeGenerated *node = &sOmmObjEffectMetalNodes[enable];
        node->fnNode.node.type = GRAPH_NODE_TYPE_GENERATED_LIST;
        node->fnNode.node.flags = obj->header.gfx.node.flags;
        node->fnNode.node.prev = NULL;
        node->fnNode.node.next = NULL;
        node->fnNode.node.parent = NULL;
        node->fnNode.node.children = NULL;
        node->fnNode.func = (GraphNodeFunc) omm_obj_effect_metal_enable_gfx;
        node->parameter = parameter;
        return node;
    }
    return NULL;
}

GEO_REGISTER_OBJECT_EFFECTS(omm_obj_effect_metal_process_graph_node);

#endif
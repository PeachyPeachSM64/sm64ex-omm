#ifdef GFX_PC_C
#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

//
// Custom commands
//

static void omm_sparkly_bowser_4_gfx_sp_texture() {
    gfx_sp_texture();
    sGfxRsp->texS = ((sGfxRdp->texTile.lrs >> G_TEXTURE_IMAGE_FRAC) << 6);
    sGfxRsp->texT = ((sGfxRdp->texTile.lrt >> G_TEXTURE_IMAGE_FRAC) << 6);
}

static void omm_sparkly_bowser_4_gfx_dp_set_texture_image() {
    sGfxRdp->texToLoad = OMM_SPARKLY_STAR_TEXTURE[gOmmSparklyMode];
}

static void omm_sparkly_bowser_4_gfx_dp_set_tile_size() {
    gfx_dp_set_tile_size();
    sGfxRsp->texS = ((sGfxRdp->texTile.lrs >> G_TEXTURE_IMAGE_FRAC) << 6);
    sGfxRsp->texT = ((sGfxRdp->texTile.lrt >> G_TEXTURE_IMAGE_FRAC) << 6);
}

static void omm_sparkly_bowser_4_gfx_dp_load_tile() {
    gfx_dp_load_tile();
    sGfxRsp->texS = ((sGfxRdp->texTile.lrs >> G_TEXTURE_IMAGE_FRAC) << 6);
    sGfxRsp->texT = ((sGfxRdp->texTile.lrt >> G_TEXTURE_IMAGE_FRAC) << 6);
}

//
// Display lists
//

static const Gfx omm_sparkly_bowser_4_gfx_enable[] = {
    gsXPSwapCmd(G_TEXTURE, omm_sparkly_bowser_4_gfx_sp_texture),
    gsXPSwapCmd(G_SETTIMG, omm_sparkly_bowser_4_gfx_dp_set_texture_image),
    gsXPSwapCmd(G_SETTILESIZE, omm_sparkly_bowser_4_gfx_dp_set_tile_size),
    gsXPSwapCmd(G_LOADTILE, omm_sparkly_bowser_4_gfx_dp_load_tile),
    gsSPSetGeometryMode(G_TEXTURE_GEN),
    gsSPEndDisplayList(),
};

static const Gfx omm_sparkly_bowser_4_gfx_disable[] = {
    gsXPSwapCmd(G_TEXTURE, NULL),
    gsXPSwapCmd(G_SETTIMG, NULL),
    gsXPSwapCmd(G_SETTILESIZE, NULL),
    gsXPSwapCmd(G_LOADTILE, NULL),
    gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsSPEndDisplayList(),
};

//
// Geo functions
//

static const Gfx *omm_sparkly_bowser_4_enable_gfx(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *generated = (struct GraphNodeGenerated *) node;
        struct Object *obj = (gCurGraphNodeHeldObject ? gCurGraphNodeHeldObject->objNode : gCurrGraphNodeObject);
        generated->fnNode.node.flags = (generated->fnNode.node.flags & 0xFF) | GRAPH_NODE_TYPE_FUNCTIONAL | (GRAPH_NODE_TYPE_400 * (obj->oOpacity != 0xFF));
        if (generated->parameter) {
            return omm_sparkly_bowser_4_gfx_enable;
        } else {
            return omm_sparkly_bowser_4_gfx_disable;
        }
    }
    return NULL;
}

void omm_sparkly_bowser_4_process_graph_node(struct Object *obj, bool enable, void (*geoFunc)(struct GraphNodeGenerated *)) {
    if (OMM_UNLIKELY(omm_sparkly_is_bowser_4_battle() && (obj_check_model(obj, MODEL_BOWSER) || obj_check_model(obj, MODEL_BOWSER2)))) {
        static struct GraphNodeGenerated sOmmSparklyBowser4Nodes[2];
        struct GraphNodeGenerated *node = &sOmmSparklyBowser4Nodes[enable];
        node->fnNode.node.type = GRAPH_NODE_TYPE_GENERATED_LIST;
        node->fnNode.node.flags = obj->header.gfx.node.flags;
        node->fnNode.node.prev = NULL;
        node->fnNode.node.next = NULL;
        node->fnNode.node.parent = NULL;
        node->fnNode.node.children = NULL;
        node->fnNode.func = (GraphNodeFunc) omm_sparkly_bowser_4_enable_gfx;
        node->parameter = enable;
        geoFunc(node);
    }
}

#endif
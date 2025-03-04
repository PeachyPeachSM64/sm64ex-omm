#ifdef GFX_PC_C
#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

//
// Custom commands
//

// Force the texture coords to (0, 0) for all vertices
static void omm_cappy_gfx_hide_logo() {
    if ((sGfxProc->texToLoad && strstr(sGfxProc->texToLoad, "logo")) || (sGfxProc->texLoaded[0] && strstr(sGfxProc->texLoaded[0], "logo"))) {
        sGfxProc->invTexWidth  = 0;
        sGfxProc->invTexHeight = 0;
    } else {
        sGfxProc->invTexWidth  = 1.f / ((sGfxProc->texTileSize.lrs - sGfxProc->texTileSize.uls + 4) >> 2);
        sGfxProc->invTexHeight = 1.f / ((sGfxProc->texTileSize.lrt - sGfxProc->texTileSize.ult + 4) >> 2);
    }
}

static void omm_cappy_gfx_dp_load_block() {
    gfx_dp_load_block();
    omm_cappy_gfx_hide_logo();
}

static void omm_cappy_gfx_dp_load_tile() {
    gfx_dp_load_tile();
    omm_cappy_gfx_hide_logo();
}

static void omm_cappy_gfx_dp_set_tile_size() {
    gfx_dp_set_tile_size();
    omm_cappy_gfx_hide_logo();
}

static void omm_cappy_gfx_dp_set_texture_image() {
    gfx_dp_set_texture_image();
    omm_cappy_gfx_hide_logo();
}

static void omm_cappy_gfx_sp_set_other_mode_l() {
    gfx_sp_set_other_mode_l();
}

static void omm_cappy_gfx_sp_set_other_mode_h() {
    gfx_sp_set_other_mode_h();
}

//
// Display lists
//

static const Gfx omm_cappy_gfx_enable[] = {
    gsXPSwapCmd(G_LOADBLOCK, omm_cappy_gfx_dp_load_block),
    gsXPSwapCmd(G_LOADTILE, omm_cappy_gfx_dp_load_tile),
    gsXPSwapCmd(G_SETTILESIZE, omm_cappy_gfx_dp_set_tile_size),
    gsXPSwapCmd(G_SETTIMG, omm_cappy_gfx_dp_set_texture_image),
    gsXPSwapCmd(G_SETOTHERMODE_L, omm_cappy_gfx_sp_set_other_mode_l),
    gsXPSwapCmd(G_SETOTHERMODE_H, omm_cappy_gfx_sp_set_other_mode_h),
    gsSPEndDisplayList(),
};

static const Gfx omm_cappy_gfx_disable[] = {
    gsXPSwapCmd(G_LOADBLOCK, NULL),
    gsXPSwapCmd(G_LOADTILE, NULL),
    gsXPSwapCmd(G_SETTILESIZE, NULL),
    gsXPSwapCmd(G_SETTIMG, NULL),
    gsXPSwapCmd(G_SETOTHERMODE_L, NULL),
    gsXPSwapCmd(G_SETOTHERMODE_H, NULL),
    gsSPEndDisplayList(),
};

//
// Geo functions
//

static const Gfx *omm_cappy_enable_gfx(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *generated = (struct GraphNodeGenerated *) node;
        generated->fnNode.node.flags = (generated->fnNode.node.flags & 0xFF) | GRAPH_NODE_TYPE_FUNCTIONAL | (GRAPH_NODE_TYPE_400 * (generated->parameter & 1));
        if (generated->parameter & 2) {
            return omm_cappy_gfx_enable;
        } else {
            return omm_cappy_gfx_disable;
        }
    }
    return NULL;
}

static struct GraphNodeGenerated *omm_cappy_process_graph_node(struct Object *obj, bool enable) {
    if (OMM_EXTRAS_CAPPY_AND_TIARA && omm_cappy_gfx_has_eyes(obj->oGraphNode)) {
        for (s32 i = 0; i != OMM_NUM_PLAYABLE_CHARACTERS; ++i) {
            bool isMario = obj_has_model(obj, omm_player_graphics_get_model(i));
            if (isMario || obj_has_model(obj, omm_player_graphics_get_normal_cap(i)) ||
                           obj_has_model(obj, omm_player_graphics_get_wing_cap(i)) ||
                           obj_has_model(obj, omm_player_graphics_get_metal_cap(i)) ||
                           obj_has_model(obj, omm_player_graphics_get_winged_metal_cap(i))) {
                static struct GraphNodeGenerated sOmmCappyNodes[2][2];
                struct GraphNodeGenerated *node = &sOmmCappyNodes[enable][isMario];
                node->fnNode.node.type = GRAPH_NODE_TYPE_GENERATED_LIST;
                node->fnNode.node.flags = obj->header.gfx.node.flags;
                node->fnNode.node.prev = NULL;
                node->fnNode.node.next = NULL;
                node->fnNode.node.parent = NULL;
                node->fnNode.node.children = NULL;
                node->fnNode.func = (GraphNodeFunc) omm_cappy_enable_gfx;
                node->parameter = (enable << 1) | (isMario ? ((gMarioState->marioBodyState->modelState >> 8) & 1) : (obj->oOpacity != 0xFF));
                return node;
            }
        }
    }
    return NULL;
}

GEO_REGISTER_OBJECT_EFFECTS(omm_cappy_process_graph_node);

#endif
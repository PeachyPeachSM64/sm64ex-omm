#ifdef GFX_PC_C
#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif
#define OMM_SHADOW_MARIO_OPACITY (0xDD)

static bool sOmmShadowMarioIsEnabled = false;

static const Lights1 omm_shadow_mario_light = gdSPDefLights1(
    0x7F, 0x7F, 0x7F,
    0xFF, 0xFF, 0xFF,
    0x28, 0x28, 0x28
);

//
// Custom commands
//

static void omm_shadow_mario_gfx_dp_set_texture_image() {
    gfx_dp_set_texture_image();
    if (sGfxProc->texToLoad) {
        str_t texToLoad;
        str_lwr(texToLoad, sizeof(str_t), sGfxProc->texToLoad);
        if (strstr(texToLoad, "omm_texture_cappy") || strstr(texToLoad, "wing")) {
            return;
        }
    }
    sGfxProc->texToLoad = OMM_TEXTURE_MISC_SHADOW_MARIO_BODY;
    gfx_combine_mode_changed(0x00A00200); // gsDPSetCombineMode(G_CC_DECALFADE, G_CC_DECALFADE)
}

static void omm_shadow_mario_gfx_sp_set_other_mode_l() {
    gfx_sp_set_other_mode_l();
    sGfxProc->otherModeL &= ~G_AC_DITHER;
}

static void omm_shadow_mario_gfx_sp_set_other_mode_h() {
    gfx_sp_set_other_mode_h();
    sGfxProc->otherModeL &= ~G_AC_DITHER;
}

static void omm_shadow_mario_gfx_sp_copymem() {
    mem_cpy(&sGfxProc->light, &omm_shadow_mario_light.l->l, sizeof(Light_t));
    mem_cpy(&sGfxProc->lightAmb, &omm_shadow_mario_light.a.l, sizeof(Light_t));
}

//
// Display lists
//

static const Gfx omm_shadow_mario_gfx_enable[] = {
    gsXPSwapCmd(G_SETTIMG, omm_shadow_mario_gfx_dp_set_texture_image),
    gsXPSwapCmd(G_SETOTHERMODE_L, omm_shadow_mario_gfx_sp_set_other_mode_l),
    gsXPSwapCmd(G_SETOTHERMODE_H, omm_shadow_mario_gfx_sp_set_other_mode_h),
    gsXPSwapCmd(G_COPYMEM, omm_shadow_mario_gfx_sp_copymem),
    gsSPEndDisplayList(),
};

static const Gfx omm_shadow_mario_gfx_disable[] = {
    gsXPSwapCmd(G_SETTIMG, NULL),
    gsXPSwapCmd(G_SETOTHERMODE_L, NULL),
    gsXPSwapCmd(G_SETOTHERMODE_H, NULL),
    gsXPSwapCmd(G_COPYMEM, NULL),
    gsSPEndDisplayList(),
};

//
// Geo functions
//

static const Gfx *omm_shadow_mario_enable_gfx(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *generated = (struct GraphNodeGenerated *) node;
        generated->fnNode.node.flags = (generated->fnNode.node.flags & 0xFF) | GRAPH_NODE_TYPE_FUNCTIONAL | GRAPH_NODE_TYPE_400;
        return generated->parameter ? omm_shadow_mario_gfx_enable : omm_shadow_mario_gfx_disable;
    }
    return NULL;
}

static struct GraphNodeGenerated *omm_shadow_mario_process_graph_node(struct Object *obj, bool enable) {
    if (sOmmShadowMarioIsEnabled && (
        obj_has_geo_layout(obj, mario_geo) ||
        obj_has_geo_layout(obj, marios_cap_geo) ||
        obj_has_geo_layout(obj, marios_wing_cap_geo) ||
        obj_has_geo_layout(obj, marios_metal_cap_geo) ||
        obj_has_geo_layout(obj, marios_winged_metal_cap_geo))
    ) {
        static struct GraphNodeGenerated sOmmShadowMarioNodes[2];
        struct GraphNodeGenerated *node = &sOmmShadowMarioNodes[enable];
        node->fnNode.node.type = GRAPH_NODE_TYPE_GENERATED_LIST;
        node->fnNode.node.flags = obj->header.gfx.node.flags;
        node->fnNode.node.prev = NULL;
        node->fnNode.node.next = NULL;
        node->fnNode.node.parent = NULL;
        node->fnNode.node.children = NULL;
        node->fnNode.func = (GraphNodeFunc) omm_shadow_mario_enable_gfx;
        node->parameter = enable;
        return node;
    }
    return NULL;
}

GEO_REGISTER_OBJECT_EFFECTS(omm_shadow_mario_process_graph_node);

//
// Update
//

// If enabled, make Mario always metal and slightly transparent
OMM_ROUTINE_PRE_RENDER(omm_shadow_mario_update) {
    if (gMarioObject) {
        if (OMM_CHEAT_SHADOW_MARIO && !OMM_EXTRAS_INVISIBLE_MODE) {
            sOmmShadowMarioIsEnabled = true;

            // Mario object
            struct MarioState *m = gMarioState;
            bool alpha = (m->marioBodyState->modelState & 0x100) != 0;
            u8 opacity = (alpha ? (m->marioBodyState->modelState & 0xFF) : 0xFF);
            m->marioBodyState->modelState &= ~0xFF;
            m->marioBodyState->modelState |= (0x400 * !alpha) | MODEL_STATE_METAL | 0x100 | min_s(opacity, OMM_SHADOW_MARIO_OPACITY);
            geo_preprocess_object_graph_node(gMarioObject);

            // Cap objects
            omm_array_for_each(omm_obj_get_cap_behaviors(), p_bhv) {
                const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
                for_each_object_with_behavior(obj, bhv) {
                    if (obj_has_model(obj, omm_player_graphics_get_normal_cap(OMM_PLAYER_MARIO))) {
                        obj->oGraphNode = gLoadedGraphNodes[omm_player_graphics_get_metal_cap(OMM_PLAYER_MARIO)];
                    } else if (obj_has_model(obj, omm_player_graphics_get_wing_cap(OMM_PLAYER_MARIO))) {
                        obj->oGraphNode = gLoadedGraphNodes[omm_player_graphics_get_winged_metal_cap(OMM_PLAYER_MARIO)];
                    }
                    obj->oOpacity = OMM_SHADOW_MARIO_OPACITY;
                    geo_preprocess_object_graph_node(obj);
                }
            }
        } else {
            sOmmShadowMarioIsEnabled = false;
        }
    }
}

#endif
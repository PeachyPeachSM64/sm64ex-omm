#if OMM_GAME_IS_SMSR
#ifdef GFX_PC_C
#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

static const Lights1 omm_obj_yoshi_mode_white_lights = gdSPDefLights1(
    0x7F, 0x7F, 0x7F,
    0xFF, 0xFF, 0xFF,
    0x28, 0x28, 0x28
);

static void omm_obj_yoshi_mode_gfx_set_white_lights() {
    if (GFX_C0(8, 8) == 6) {
        mem_cpy(&sGfxProc->light, &omm_obj_yoshi_mode_white_lights.l->l, sizeof(Light_t));
    } else {
        mem_cpy(&sGfxProc->lightAmb, &omm_obj_yoshi_mode_white_lights.a.l, sizeof(Light_t));
    }
    sGfxProc->lightChanged = true;
}

static OmmHMap_(const char *) sOmmObjYoshiModeTexCache = omm_hmap_zero;

static void omm_obj_yoshi_mode_gfx_set_tex_white_palette(const char *texname) {
    GfxTexture *tex = gfx_texture_find(texname);
    if (tex && tex->data) {
        s32 i = omm_hmap_find(sOmmObjYoshiModeTexCache, tex->hash);
        const char *texname = NULL;
        if (i == -1) {
            texname = mem_new(char, sizeof(str_t));
            str_fmt((char *) texname, sizeof(str_t), "YOSHI_WHITE_%08X", tex->hash);
            u32 size = tex->w * tex->h * 4;
            u8 cmax = 0;
            for (u32 j = 0; j != size; j += 4) {
                cmax = max_s(max_s(cmax, tex->data[j + 0]), max_s(tex->data[j + 1], tex->data[j + 2]));
            }
            u8 *data = mem_dup(tex->data, size);
            for (u32 j = 0; j != size; j += 4) {
                u8 pmax = max_3_s(tex->data[j + 0], tex->data[j + 1], tex->data[j + 2]);
                data[j + 0] = data[j + 1] = data[j + 2] = relerp_0_1_s(pmax, 0, cmax, 0, 0xFF);
            }
            gfx_texture_load(texname, data, tex->w, tex->h, false);
            omm_hmap_insert(sOmmObjYoshiModeTexCache, tex->hash, texname);
        } else {
            texname = omm_hmap_get(sOmmObjYoshiModeTexCache, const char *, i);
        }
        if (texname) {
            sGfxProc->texToLoad = texname;
        }
    }
}

//
// Custom commands
//

static void omm_obj_yoshi_mode_gfx_sp_copymem() {
    gfx_sp_copymem();
    omm_obj_yoshi_mode_gfx_set_white_lights();
}

static void omm_obj_yoshi_mode_gfx_sp_movemem() {
    gfx_sp_movemem();
    if (GFX_C0(0, 8) == G_MV_LIGHT) {
        omm_obj_yoshi_mode_gfx_set_white_lights();
    }
}

static void omm_obj_yoshi_mode_gfx_dp_set_texture_image() {
    gfx_dp_set_texture_image();
    omm_obj_yoshi_mode_gfx_set_tex_white_palette(sGfxProc->texToLoad);
}

//
// Display lists
//

static const Gfx omm_obj_yoshi_mode_gfx_enable[] = {
    gsXPSwapCmd(G_COPYMEM, omm_obj_yoshi_mode_gfx_sp_copymem),
    gsXPSwapCmd(G_MOVEMEM, omm_obj_yoshi_mode_gfx_sp_movemem),
    gsXPSwapCmd(G_SETTIMG, omm_obj_yoshi_mode_gfx_dp_set_texture_image),
    gsSPEndDisplayList(),
};

static const Gfx omm_obj_yoshi_mode_gfx_disable[] = {
    gsXPSwapCmd(G_COPYMEM, NULL),
    gsXPSwapCmd(G_MOVEMEM, NULL),
    gsXPSwapCmd(G_SETTIMG, NULL),
    gsSPEndDisplayList(),
};

//
// Geo functions
//

static const Gfx *omm_obj_yoshi_mode_enable_gfx(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *generated = (struct GraphNodeGenerated *) node;
        generated->fnNode.node.flags = (generated->fnNode.node.flags & 0xFF) | GRAPH_NODE_TYPE_FUNCTIONAL;
        if (generated->parameter) {
            return omm_obj_yoshi_mode_gfx_enable;
        } else {
            return omm_obj_yoshi_mode_gfx_disable;
        }
    }
    return NULL;
}

static struct GraphNodeGenerated *omm_obj_yoshi_mode_process_graph_node(struct Object *obj, bool enable) {
    struct MarioState *m = gMarioState;
    if (gOmmGlobals->yoshiMode && gOmmGlobals->booZeroLife) {
        if ((omm_obj_is_playable_yoshi(obj) || omm_obj_is_yoshi_tongue(obj)) &&     // Yoshi
            !(omm_mario_has_metal_cap(m) && !omm_mario_cap_is_flickering(m))        // Not Metal Mario
        ) {
            static struct GraphNodeGenerated sOmmObjYoshiModeNodes[2];
            struct GraphNodeGenerated *node = &sOmmObjYoshiModeNodes[enable];
            node->fnNode.node.type = GRAPH_NODE_TYPE_GENERATED_LIST;
            node->fnNode.node.flags = obj->header.gfx.node.flags;
            node->fnNode.node.prev = NULL;
            node->fnNode.node.next = NULL;
            node->fnNode.node.parent = NULL;
            node->fnNode.node.children = NULL;
            node->fnNode.func = (GraphNodeFunc) omm_obj_yoshi_mode_enable_gfx;
            node->parameter = enable;
            return node;
        }
    }
    return NULL;
}

GEO_REGISTER_OBJECT_EFFECTS(omm_obj_yoshi_mode_process_graph_node);

#endif
#endif
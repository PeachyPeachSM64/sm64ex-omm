#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define __shift                     (sizeof(void *) >> 3)
#define __offset(offset)            (((offset) & 3) | (((offset) & ~3) << __shift))
#define geo_cmd_get(type, offset)   (*(type *) &sGeoLayoutCommand[__offset(offset)])
#define geo_cmd_addr(offset)        ((uintptr_t) (sGeoLayoutCommand + __offset(offset)))
#define geo_cmd_adv(size)           sGeoLayoutCommand += ((size) << __shift)
#define geo_cmd_dl()                s32 layer = 0; void *displayList = NULL; if (params & 0x80) { displayList = geo_cmd_get(void *, 0); layer = (params & 0x0F); geo_cmd_adv(4); }
#define deg_to_s16(angle)           ((s16) ((((s32) (angle)) << 15) / 180))
#define gNode                       ((struct GraphNode *) node)

//
// Data
//

static struct GraphNode *sGraphNode;
static struct GraphNode **sGraphNodeCamera;
static struct GraphNode *sGraphNodeStack[64];
static struct AllocOnlyPool *sGraphNodeMemoryPool;
static s32 sGraphNodeStackIndex;
static uintptr_t sGeoLayoutStack[64];
static s32 sGeoLayoutStackIndex;
static s32 sGeoLayoutReturnIndex;
static u8 *sGeoLayoutCommand;

static void geo_layout_register_scene_graph_node(struct GraphNode *node) {
    sGraphNodeStack[sGraphNodeStackIndex] = node;
    if (!sGraphNodeStackIndex) {
        if (!sGraphNode) sGraphNode = node;
    } else if (sGraphNodeStack[sGraphNodeStackIndex - 1]->type == GRAPH_NODE_TYPE_OBJECT_PARENT) {
        ((struct GraphNodeObjectParent *) sGraphNodeStack[sGraphNodeStackIndex - 1])->sharedChild = node;
    } else {
        geo_add_child(sGraphNodeStack[sGraphNodeStackIndex - 1], node);
    }
}

//
// Commands
//

static void geo_layout_cmd_branch_and_link(void) {
    sGeoLayoutStack[sGeoLayoutStackIndex++] = geo_cmd_addr(8);
    sGeoLayoutStack[sGeoLayoutStackIndex++] = (sGraphNodeStackIndex << 16) + sGeoLayoutReturnIndex;
    sGeoLayoutReturnIndex = sGeoLayoutStackIndex;
    sGeoLayoutCommand = geo_cmd_get(void *, 4);
}

static void geo_layout_cmd_end(void) {
    sGeoLayoutStackIndex = sGeoLayoutReturnIndex;
    sGeoLayoutReturnIndex = sGeoLayoutStack[--sGeoLayoutStackIndex] & 0xFFFF;
    sGraphNodeStackIndex = sGeoLayoutStack[sGeoLayoutStackIndex] >> 16;
    sGeoLayoutCommand = (u8 *) sGeoLayoutStack[--sGeoLayoutStackIndex];
}

static void geo_layout_cmd_branch(void) {
    if (geo_cmd_get(u8, 1)) {
        sGeoLayoutStack[sGeoLayoutStackIndex++] = geo_cmd_addr(8);
    }
    sGeoLayoutCommand = geo_cmd_get(void *, 4);
}

static void geo_layout_cmd_return(void) {
    sGeoLayoutCommand = (u8 *) sGeoLayoutStack[--sGeoLayoutStackIndex];
}

static void geo_layout_cmd_open_node(void) {
    sGraphNodeStack[sGraphNodeStackIndex + 1] = sGraphNodeStack[sGraphNodeStackIndex];
    sGraphNodeStackIndex++;
    geo_cmd_adv(4);
}

static void geo_layout_cmd_close_node(void) {
    sGraphNodeStackIndex--;
    geo_cmd_adv(4);
}

static void geo_layout_cmd_assign_as_view(void) {
    geo_cmd_adv(4);
}

static void geo_layout_cmd_update_node_flags(void) {
    u8 operation = geo_cmd_get(u8, 1);
    u16 flagBits = geo_cmd_get(s16, 2);
    switch (operation) {
        case 0: sGraphNodeStack[sGraphNodeStackIndex]->flags = flagBits; break;
        case 1: sGraphNodeStack[sGraphNodeStackIndex]->flags |= flagBits; break;
        case 2: sGraphNodeStack[sGraphNodeStackIndex]->flags &= ~flagBits; break;
    }
    geo_cmd_adv(4);
}

static void geo_layout_cmd_node_root(void) {
    s16 x = geo_cmd_get(s16, 4);
    s16 y = geo_cmd_get(s16, 6);
    s16 width = geo_cmd_get(s16, 8);
    s16 height = geo_cmd_get(s16, 10);
    struct GraphNodeRoot *node = init_graph_node_root(sGraphNodeMemoryPool, NULL, 0, x, y, width, height);
    if (sGraphNodeMemoryPool) {
        node->views = alloc_only_pool_alloc(sGraphNodeMemoryPool, sizeof(struct GraphNode *));
    } else {
        node->views = OMM_MEMNEW(struct GraphNode *, 1);
    }
    node->numViews = 1;
    geo_layout_register_scene_graph_node(gNode);
    sGraphNodeCamera = node->views;
    geo_cmd_adv(12);
}

static void geo_layout_cmd_node_ortho_projection(void) {
    f32 scale = (f32) geo_cmd_get(s16, 2) / 100.f;
    struct GraphNodeOrthoProjection *node = init_graph_node_ortho_projection(sGraphNodeMemoryPool, NULL, scale);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(4);
}

static void geo_layout_cmd_node_perspective(void) {
    s16 fov = geo_cmd_get(s16, 2);
    s16 near = geo_cmd_get(s16, 4);
    s16 far = geo_cmd_get(s16, 6);
    GraphNodeFunc frustumFunc = NULL;
    if (geo_cmd_get(u8, 1)) {
        frustumFunc = (GraphNodeFunc) geo_cmd_get(void *, 0x08);
        geo_cmd_adv(4);
    }
    struct GraphNodePerspective *node = init_graph_node_perspective(sGraphNodeMemoryPool, NULL, fov, near, far, frustumFunc, 0);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(8);
}

static void geo_layout_cmd_node_start(void) {
    struct GraphNodeStart *node = init_graph_node_start(sGraphNodeMemoryPool, NULL);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(4);
}

static void geo_layout_cmd_nop3(void) {
    geo_cmd_adv(16);
}

static void geo_layout_cmd_node_master_list(void) {
    u8 enableZBuffer = geo_cmd_get(u8, 1);
    struct GraphNodeMasterList *node = init_graph_node_master_list(sGraphNodeMemoryPool, NULL, enableZBuffer);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(4);
}

static void geo_layout_cmd_node_level_of_detail(void) {
    s16 minDistance = geo_cmd_get(s16, 4);
    s16 maxDistance = geo_cmd_get(s16, 6);
    struct GraphNodeLevelOfDetail *node = init_graph_node_render_range(sGraphNodeMemoryPool, NULL, minDistance, maxDistance);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(8);
}

static void geo_layout_cmd_node_switch_case(void) {
    s16 numCases = geo_cmd_get(s16, 2);
    GraphNodeFunc switchFunc = geo_cmd_get(GraphNodeFunc, 4);
    struct GraphNodeSwitchCase *node = init_graph_node_switch_case(sGraphNodeMemoryPool, NULL, numCases, 0, switchFunc, 0);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(8);
}

static void geo_layout_cmd_node_camera(void) {
    s16 mode = geo_cmd_get(s16, 2);
    s16 posX = geo_cmd_get(s16, 4);
    s16 posY = geo_cmd_get(s16, 6);
    s16 posZ = geo_cmd_get(s16, 8);
    s16 focusX = geo_cmd_get(s16, 10);
    s16 focusY = geo_cmd_get(s16, 12);
    s16 focusZ = geo_cmd_get(s16, 14);
    GraphNodeFunc func = geo_cmd_get(GraphNodeFunc, 16);
    Vec3f pos = { posX, posY, posZ };
    Vec3f focus = { focusX, focusY, focusZ };
    struct GraphNodeCamera *node = init_graph_node_camera(sGraphNodeMemoryPool, NULL, pos, focus, func, mode);
    geo_layout_register_scene_graph_node(gNode);
    *sGraphNodeCamera = gNode;
    geo_cmd_adv(20);
}

static void geo_layout_cmd_node_translation_rotation(void) {
    Vec3s translation, rotation;
    u8 params = geo_cmd_get(u8, 1);
    s16 fieldLayout = ((params & 0x70) >> 4);
    switch (fieldLayout) {
        case 0: {
            translation[0] = geo_cmd_get(s16, 4);
            translation[1] = geo_cmd_get(s16, 6);
            translation[2] = geo_cmd_get(s16, 8);
            rotation[0] = deg_to_s16(geo_cmd_get(s16, 10));
            rotation[1] = deg_to_s16(geo_cmd_get(s16, 12));
            rotation[2] = deg_to_s16(geo_cmd_get(s16, 14));
            geo_cmd_adv(16);
        } break;
        case 1: {
            translation[0] = geo_cmd_get(s16, 2);
            translation[1] = geo_cmd_get(s16, 4);
            translation[2] = geo_cmd_get(s16, 6);
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            geo_cmd_adv(8);
        } break;
        case 2: {
            translation[0] = 0;
            translation[1] = 0;
            translation[2] = 0;
            rotation[0] = deg_to_s16(geo_cmd_get(s16, 2));
            rotation[1] = deg_to_s16(geo_cmd_get(s16, 4));
            rotation[2] = deg_to_s16(geo_cmd_get(s16, 6));
            geo_cmd_adv(8);
        } break;
        case 3: {
            translation[0] = 0;
            translation[1] = 0;
            translation[2] = 0;
            rotation[0] = 0;
            rotation[1] = deg_to_s16(geo_cmd_get(s16, 2));
            rotation[2] = 0;
            geo_cmd_adv(4);
        } break;
    }
    geo_cmd_dl();
    struct GraphNodeTranslationRotation *node = init_graph_node_translation_rotation(sGraphNodeMemoryPool, NULL, layer, displayList, translation, rotation);
    geo_layout_register_scene_graph_node(gNode);
}

static void geo_layout_cmd_node_translation(void) {
    Vec3s translation;
    u8 params = geo_cmd_get(u8, 1);
    translation[0] = geo_cmd_get(s16, 2);
    translation[1] = geo_cmd_get(s16, 4);
    translation[2] = geo_cmd_get(s16, 6);
    geo_cmd_adv(8);
    geo_cmd_dl();
    struct GraphNodeTranslation *node = init_graph_node_translation(sGraphNodeMemoryPool, NULL, layer, displayList, translation);
    geo_layout_register_scene_graph_node(gNode);
}

static void geo_layout_cmd_node_rotation(void) {
    Vec3s rotation;
    u8 params = geo_cmd_get(u8, 1);
    rotation[0] = deg_to_s16(geo_cmd_get(s16, 2));
    rotation[1] = deg_to_s16(geo_cmd_get(s16, 4));
    rotation[2] = deg_to_s16(geo_cmd_get(s16, 6));
    geo_cmd_adv(8);
    geo_cmd_dl();
    struct GraphNodeRotation *node = init_graph_node_rotation(sGraphNodeMemoryPool, NULL, layer, displayList, rotation);
    geo_layout_register_scene_graph_node(gNode);
}

static void geo_layout_cmd_node_scale(void) {
    u8 params = geo_cmd_get(u8, 1);
    f32 scale = geo_cmd_get(u32, 4) / 65536.f;
    geo_cmd_adv(8);
    geo_cmd_dl();
    struct GraphNodeScale *node = init_graph_node_scale(sGraphNodeMemoryPool, NULL, layer, displayList, scale);
    geo_layout_register_scene_graph_node(gNode);
}

static void geo_layout_cmd_nop2(void) {
    geo_cmd_adv(8);
}

static void geo_layout_cmd_node_animated_part(void) {
    Vec3s translation;
    s32 layer = geo_cmd_get(u8, 1);
    translation[0] = geo_cmd_get(s16, 2);
    translation[1] = geo_cmd_get(s16, 4);
    translation[2] = geo_cmd_get(s16, 6);
    void *displayList = geo_cmd_get(void *, 8);
    struct GraphNodeAnimatedPart *node = init_graph_node_animated_part(sGraphNodeMemoryPool, NULL, layer, displayList, translation);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(12);
}

static void geo_layout_cmd_node_billboard(void) {
    Vec3s translation;
    u8 params = geo_cmd_get(u8, 1);
    translation[0] = geo_cmd_get(s16, 2);
    translation[1] = geo_cmd_get(s16, 4);
    translation[2] = geo_cmd_get(s16, 6);
    geo_cmd_adv(8);
    geo_cmd_dl();
    struct GraphNodeBillboard *node = init_graph_node_billboard(sGraphNodeMemoryPool, NULL, layer, displayList, translation);
    geo_layout_register_scene_graph_node(gNode);
}

static void geo_layout_cmd_node_display_list(void) {
    s32 layer = geo_cmd_get(u8, 1);
    void *displayList = geo_cmd_get(void *, 4);
    struct GraphNodeDisplayList *node = init_graph_node_display_list(sGraphNodeMemoryPool, NULL, layer, displayList);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(8);
}

static void geo_layout_cmd_node_shadow(void) {
    u8 shadowType = geo_cmd_get(s16, 2);
    u8 shadowSolidity = geo_cmd_get(s16, 4);
    s16 shadowScale = geo_cmd_get(s16, 6);
    struct GraphNodeShadow *node = init_graph_node_shadow(sGraphNodeMemoryPool, NULL, shadowScale, shadowSolidity, shadowType);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(8);
}

static void geo_layout_cmd_node_object_parent(void) {
    struct GraphNodeObjectParent *node = init_graph_node_object_parent(sGraphNodeMemoryPool, NULL, &gObjParentGraphNode);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(4);
}

static void geo_layout_cmd_node_generated(void) {
    s16 parameter = geo_cmd_get(s16, 2);
    GraphNodeFunc func = geo_cmd_get(GraphNodeFunc, 4);
    struct GraphNodeGenerated *node = init_graph_node_generated(sGraphNodeMemoryPool, NULL, func, parameter);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(8);
}

static void geo_layout_cmd_node_background(void) {
    u16 background = geo_cmd_get(u16, 2);
    GraphNodeFunc func = geo_cmd_get(GraphNodeFunc, 4);
    struct GraphNodeBackground *node = init_graph_node_background(sGraphNodeMemoryPool, NULL, background, func);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(8);
}

static void geo_layout_cmd_nop(void) {
    geo_cmd_adv(8);
}

static void geo_layout_cmd_copy_view(void) {
    geo_cmd_adv(4);
}

static void geo_layout_cmd_node_held_obj(void) {
    Vec3s offset;
    offset[0] = geo_cmd_get(s16, 2);
    offset[1] = geo_cmd_get(s16, 4);
    offset[2] = geo_cmd_get(s16, 6);
    u8 unused = geo_cmd_get(u8, 1);
    GraphNodeFunc func = geo_cmd_get(GraphNodeFunc, 8);
    struct GraphNodeHeldObject *node = init_graph_node_held_object(sGraphNodeMemoryPool, NULL, NULL, offset, func, unused);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(12);
}

static void geo_layout_cmd_node_culling_radius(void) {
    s16 cullingRadius = geo_cmd_get(s16, 2);
    struct GraphNodeCullingRadius *node = init_graph_node_culling_radius(sGraphNodeMemoryPool, NULL, cullingRadius);
    geo_layout_register_scene_graph_node(gNode);
    geo_cmd_adv(4);
}

//
// Geo layout to Graph node
//

typedef void (*GeoLayoutCommandProc)(void);
static GeoLayoutCommandProc sGeoLayoutCmdTable[] = {
    geo_layout_cmd_branch_and_link,
    geo_layout_cmd_end,
    geo_layout_cmd_branch,
    geo_layout_cmd_return,
    geo_layout_cmd_open_node,
    geo_layout_cmd_close_node,
    geo_layout_cmd_assign_as_view,
    geo_layout_cmd_update_node_flags,
    geo_layout_cmd_node_root,
    geo_layout_cmd_node_ortho_projection,
    geo_layout_cmd_node_perspective,
    geo_layout_cmd_node_start,
    geo_layout_cmd_node_master_list,
    geo_layout_cmd_node_level_of_detail,
    geo_layout_cmd_node_switch_case,
    geo_layout_cmd_node_camera,
    geo_layout_cmd_node_translation_rotation,
    geo_layout_cmd_node_translation,
    geo_layout_cmd_node_rotation,
    geo_layout_cmd_node_animated_part,
    geo_layout_cmd_node_billboard,
    geo_layout_cmd_node_display_list,
    geo_layout_cmd_node_shadow,
    geo_layout_cmd_node_object_parent,
    geo_layout_cmd_node_generated,
    geo_layout_cmd_node_background,
    geo_layout_cmd_nop,
    geo_layout_cmd_copy_view,
    geo_layout_cmd_node_held_obj,
    geo_layout_cmd_node_scale,
    geo_layout_cmd_nop2,
    geo_layout_cmd_nop3,
    geo_layout_cmd_node_culling_radius,
};

struct GraphNode *geo_layout_to_graph_node(struct AllocOnlyPool *pool, const GeoLayout *geoLayout) {
    static OmmHMap sGraphNodeCache = omm_hmap_zero;

    // NULL check
    if (!geoLayout) {
        return NULL;
    }
    
    // Cache check
    if (!pool) {
        s32 i = omm_hmap_find(sGraphNodeCache, (uintptr_t) geoLayout);
        if (OMM_LIKELY(i != -1)) {
            return omm_hmap_get(sGraphNodeCache, struct GraphNode *, i);
        }
    }
    
    // Init graph node stack
    sGraphNode = NULL;
    sGraphNodeStack[0] = NULL;
    sGraphNodeStackIndex = 0;
    sGraphNodeMemoryPool = pool;

    // Init geo layout stack
    sGeoLayoutStack[0] = 0;
    sGeoLayoutStack[1] = 0;
    sGeoLayoutStackIndex = 2;
    sGeoLayoutReturnIndex = 2;
    sGeoLayoutCommand = (u8 *) geoLayout;

    // Process geo layout
    while (sGeoLayoutCommand) {
        sGeoLayoutCmdTable[*sGeoLayoutCommand]();
    }
#if OMM_CODE_DYNOS
    if (sGraphNode) {
        sGraphNode->georef = (const void *) geoLayout;
    }
#endif

    // Add the created graph node to the cache list
    if (!pool) {
        omm_hmap_insert(sGraphNodeCache, (uintptr_t) geoLayout, sGraphNode);
    }
    return sGraphNode;
}

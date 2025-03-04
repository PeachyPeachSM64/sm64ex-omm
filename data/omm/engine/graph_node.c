#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define gNode   ((struct GraphNode *) node)
#define gFnNode ((struct FnGraphNode *) node)

u16 gAreaUpdateCounter;
struct GraphNode gObjParentGraphNode;
struct GraphNodeRoot *gCurGraphNodeRoot = NULL;
struct GraphNodeMasterList *gCurGraphNodeMasterList = NULL;
struct GraphNodePerspective *gCurGraphNodeCamFrustum = NULL;
struct GraphNodeCamera *gCurGraphNodeCamera = NULL;
struct GraphNodeObject *gCurGraphNodeObject = NULL;
struct GraphNodeHeldObject *gCurGraphNodeHeldObject = NULL;

//
// Init
//

#define init_graph_node(typeName, typeIndex) { \
    if (!node) { \
    if (pool) { node = (struct typeName *) alloc_only_pool_alloc(pool, sizeof(struct typeName)); } \
    else      { node = (struct typeName *) mem_new(struct typeName, 1); } \
    } \
    gNode->type = (typeIndex); \
    gNode->flags = GRAPH_RENDER_ACTIVE; \
    gNode->prev = gNode; \
    gNode->next = gNode; \
    gNode->parent = NULL; \
    gNode->children = NULL; \
}

#define run_create() { \
    if (func) { \
        func(GEO_CONTEXT_CREATE, gNode, pool); \
    } \
}

struct GraphNodeRoot *init_graph_node_root(struct AllocOnlyPool *pool, struct GraphNodeRoot *node, s16 areaIndex, s16 x, s16 y, s16 width, s16 height) {
    init_graph_node(GraphNodeRoot, GRAPH_NODE_TYPE_ROOT);
    node->areaIndex = areaIndex;
    node->unk15 = 0;
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    node->views = NULL;
    node->numViews = 0;
    return node;
}

struct GraphNodeOrthoProjection *init_graph_node_ortho_projection(struct AllocOnlyPool *pool, struct GraphNodeOrthoProjection *node, f32 scale) {
    init_graph_node(GraphNodeOrthoProjection, GRAPH_NODE_TYPE_ORTHO_PROJECTION);
    node->scale = scale;
    return node;
}

struct GraphNodePerspective *init_graph_node_perspective(struct AllocOnlyPool *pool, struct GraphNodePerspective *node, f32 fov, s16 near, s16 far, GraphNodeFunc func, s32 unused) {
    init_graph_node(GraphNodePerspective, GRAPH_NODE_TYPE_PERSPECTIVE);
    node->fov = fov;
    node->near = near;
    node->far = far;
    node->fnNode.func = func;
    node->unused = unused;
    run_create();
    return node;
}

struct GraphNodeStart *init_graph_node_start(struct AllocOnlyPool *pool, struct GraphNodeStart *node) {
    init_graph_node(GraphNodeStart, GRAPH_NODE_TYPE_START);
    return node;
}

struct GraphNodeMasterList *init_graph_node_master_list(struct AllocOnlyPool *pool, struct GraphNodeMasterList *node, s16 enableZBuffer) {
    init_graph_node(GraphNodeMasterList, GRAPH_NODE_TYPE_MASTER_LIST);
    gNode->flags |= (GRAPH_RENDER_Z_BUFFER * (enableZBuffer));
    return node;
}

struct GraphNodeLevelOfDetail *init_graph_node_render_range(struct AllocOnlyPool *pool, struct GraphNodeLevelOfDetail *node, s16 minDistance, s16 maxDistance) {
    init_graph_node(GraphNodeLevelOfDetail, GRAPH_NODE_TYPE_LEVEL_OF_DETAIL);
    node->minDistance = minDistance;
    node->maxDistance = maxDistance;
    return node;
}

struct GraphNodeSwitchCase *init_graph_node_switch_case(struct AllocOnlyPool *pool, struct GraphNodeSwitchCase *node, s16 numCases, s16 selectedCase, GraphNodeFunc func, s32 unused) {
    init_graph_node(GraphNodeSwitchCase, GRAPH_NODE_TYPE_SWITCH_CASE);
    node->numCases = numCases;
    node->selectedCase = selectedCase;
    node->fnNode.func = func;
    node->unused = unused;
    run_create();
    return node;
}

struct GraphNodeCamera *init_graph_node_camera(struct AllocOnlyPool *pool, struct GraphNodeCamera *node, f32 *pos, f32 *focus, GraphNodeFunc func, s32 mode) {
    init_graph_node(GraphNodeCamera, GRAPH_NODE_TYPE_CAMERA);
    vec3f_copy(node->pos, pos);
    vec3f_copy(node->focus, focus);
    node->fnNode.func = func;
    node->config.mode = mode;
    node->config.camera = NULL;
    node->roll = 0;
    node->rollScreen = 0;
    run_create();
    return node;
}

struct GraphNodeTranslationRotation *init_graph_node_translation_rotation(struct AllocOnlyPool *pool, struct GraphNodeTranslationRotation *node, s32 drawingLayer, void *displayList, Vec3s translation, Vec3s rotation) {
    init_graph_node(GraphNodeTranslationRotation, GRAPH_NODE_TYPE_TRANSLATION_ROTATION);
    vec3s_copy(node->translation, translation);
    vec3s_copy(node->rotation, rotation);
    gNode->flags = (drawingLayer << 8) | (gNode->flags & 0xFF);
    node->displayList = displayList;
    return node;
}

struct GraphNodeTranslation *init_graph_node_translation(struct AllocOnlyPool *pool, struct GraphNodeTranslation *node, s32 drawingLayer, void *displayList, Vec3s translation) {
    init_graph_node(GraphNodeTranslation, GRAPH_NODE_TYPE_TRANSLATION);
    vec3s_copy(node->translation, translation);
    gNode->flags = (drawingLayer << 8) | (gNode->flags & 0xFF);
    node->displayList = displayList;
    return node;
}

struct GraphNodeRotation *init_graph_node_rotation(struct AllocOnlyPool *pool, struct GraphNodeRotation *node, s32 drawingLayer, void *displayList, Vec3s rotation) {
    init_graph_node(GraphNodeRotation, GRAPH_NODE_TYPE_ROTATION);
    vec3s_copy(node->rotation, rotation);
    gNode->flags = (drawingLayer << 8) | (gNode->flags & 0xFF);
    node->displayList = displayList;
    return node;
}

struct GraphNodeScale *init_graph_node_scale(struct AllocOnlyPool *pool, struct GraphNodeScale *node, s32 drawingLayer, void *displayList, f32 scale) {
    init_graph_node(GraphNodeScale, GRAPH_NODE_TYPE_SCALE);
    gNode->flags = (drawingLayer << 8) | (gNode->flags & 0xFF);
    node->scale = scale;
    node->displayList = displayList;
    return node;
}

struct GraphNodeObject *init_graph_node_object(struct AllocOnlyPool *pool, struct GraphNodeObject *node, struct GraphNode *sharedChild, Vec3f pos, Vec3s angle, Vec3f scale) {
    init_graph_node(GraphNodeObject, GRAPH_NODE_TYPE_OBJECT);
    vec3f_copy(node->pos, pos);
    vec3f_copy(node->scale, scale);
    vec3s_copy(node->angle, angle);
    node->sharedChild = sharedChild;
    node->throwMatrix = NULL;
    node->mAnimInfo.animID = 0;
    node->mAnimInfo.curAnim = NULL;
    node->mAnimInfo.curAnimRef = NULL;
    node->mAnimInfo.animFrame = 0;
    node->mAnimInfo.animFrameAccelAssist = 0;
    node->mAnimInfo.animAccel = ANIM_ACCEL_ONE;
    node->mAnimInfo.animTimer = 0;
    node->mAnimInfo.animFlags = 0;
    gNode->flags |= GRAPH_RENDER_HAS_ANIMATION;
    return node;
}

struct GraphNodeCullingRadius *init_graph_node_culling_radius(struct AllocOnlyPool *pool, struct GraphNodeCullingRadius *node, s16 radius) {
    init_graph_node(GraphNodeCullingRadius, GRAPH_NODE_TYPE_CULLING_RADIUS);
    node->cullingRadius = radius;
    return node;
}

struct GraphNodeAnimatedPart *init_graph_node_animated_part(struct AllocOnlyPool *pool, struct GraphNodeAnimatedPart *node, s32 drawingLayer, void *displayList, Vec3s translation) {
    init_graph_node(GraphNodeAnimatedPart, GRAPH_NODE_TYPE_ANIMATED_PART);
    vec3s_copy(node->translation, translation);
    gNode->flags = (drawingLayer << 8) | (gNode->flags & 0xFF);
    node->displayList = displayList;
    return node;
}

struct GraphNodeBillboard *init_graph_node_billboard(struct AllocOnlyPool *pool, struct GraphNodeBillboard *node, s32 drawingLayer, void *displayList, Vec3s translation) {
    init_graph_node(GraphNodeBillboard, GRAPH_NODE_TYPE_BILLBOARD);
    vec3s_copy(node->translation, translation);
    gNode->flags = (drawingLayer << 8) | (gNode->flags & 0xFF);
    node->displayList = displayList;
    return node;
}

struct GraphNodeDisplayList *init_graph_node_display_list(struct AllocOnlyPool *pool, struct GraphNodeDisplayList *node, s32 drawingLayer, void *displayList) {
    init_graph_node(GraphNodeDisplayList, GRAPH_NODE_TYPE_DISPLAY_LIST);
    gNode->flags = (drawingLayer << 8) | (gNode->flags & 0xFF);
    node->displayList = displayList;
    return node;
}

struct GraphNodeShadow *init_graph_node_shadow(struct AllocOnlyPool *pool, struct GraphNodeShadow *node, s16 shadowScale, u8 shadowSolidity, u8 shadowType) {
    init_graph_node(GraphNodeShadow, GRAPH_NODE_TYPE_SHADOW);
    node->shadowScale = shadowScale;
    node->shadowSolidity = shadowSolidity;
    node->shadowType = shadowType;
    return node;
}

struct GraphNodeObjectParent *init_graph_node_object_parent(struct AllocOnlyPool *pool, struct GraphNodeObjectParent *node, struct GraphNode *sharedChild) {
    init_graph_node(GraphNodeObjectParent, GRAPH_NODE_TYPE_OBJECT_PARENT);
    node->sharedChild = sharedChild;
    return node;
}

struct GraphNodeGenerated *init_graph_node_generated(struct AllocOnlyPool *pool, struct GraphNodeGenerated *node, GraphNodeFunc func, s32 parameter) {
    init_graph_node(GraphNodeGenerated, GRAPH_NODE_TYPE_GENERATED_LIST);
    node->fnNode.func = func;
    node->parameter = parameter;
    run_create();
    return node;
}

struct GraphNodeBackground *init_graph_node_background(struct AllocOnlyPool *pool, struct GraphNodeBackground *node, u16 background, GraphNodeFunc func) {
    init_graph_node(GraphNodeBackground, GRAPH_NODE_TYPE_BACKGROUND);
    node->background = (background << 16) | background;
    node->fnNode.func = func;
    node->unused = 0;
    run_create();
    return node;
}

struct GraphNodeHeldObject *init_graph_node_held_object(struct AllocOnlyPool *pool, struct GraphNodeHeldObject *node, struct Object *objNode, Vec3s translation, GraphNodeFunc func, UNUSED s32 playerIndex) {
    init_graph_node(GraphNodeHeldObject, GRAPH_NODE_TYPE_HELD_OBJ);
    vec3s_copy(node->translation, translation);
    node->objNode = objNode;
    node->fnNode.func = func;
    node->playerIndex = 0;
    run_create();
    return node;
}

//
// Geo child
//

struct GraphNode *geo_add_child(struct GraphNode *parent, struct GraphNode *node) {
    if (node) {
        node->parent = parent;
        struct GraphNode *parentFirstChild = parent->children;
        if (!parentFirstChild) {
            parent->children = node;
            node->prev = node;
            node->next = node;
        } else {
            struct GraphNode *parentLastChild = parentFirstChild->prev;
            node->prev = parentLastChild;
            node->next = parentFirstChild;
            parentFirstChild->prev = node;
            parentLastChild->next = node;
        }
    }
    return node;
}

struct GraphNode *geo_remove_child(struct GraphNode *node) {
    struct GraphNode *parent = node->parent;
    struct GraphNode **firstChild = &parent->children;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    if (*firstChild == node) {
        if (node->next == node) {
            *firstChild = NULL;
        } else {
            *firstChild = node->next;
        }
    }
    return parent;
}

struct GraphNode *geo_make_first_child(struct GraphNode *node) {
    struct GraphNode *parent = node->parent;
    struct GraphNode **firstChild = &parent->children;
    if (*firstChild != node) {
        if ((*firstChild)->prev != node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            struct GraphNode *lastSibling = (*firstChild)->prev;
            node->prev = lastSibling;
            node->next = *firstChild;
            (*firstChild)->prev = node;
            lastSibling->next = node;
        }
        *firstChild = node;
    }
    return parent;
}

//
// Geo utils
//

static Gfx *__geo_move_from_camera(s32 callContext, struct GraphNode *node, UNUSED void *context, f32 offsetScale) {
    if (callContext == GEO_CONTEXT_RENDER && gCurGraphNodeObject) {
        f32 dist = vec3f_length(gCurGraphNodeObject->cameraToObject);
        if (dist != 0) {
            struct GraphNodeGenerated *asGenerated = (struct GraphNodeGenerated *) node;
            struct GraphNodeTranslation *translationNode = (struct GraphNodeTranslation *) node->next;
            f32 offset = offsetScale * (s16) asGenerated->parameter;
            translationNode->translation[0] = (s16) ((offset * gCurGraphNodeObject->cameraToObject[0]) / (dist * gCurGraphNodeObject->scale[0]));
            translationNode->translation[1] = (s16) ((offset * gCurGraphNodeObject->cameraToObject[1]) / (dist * gCurGraphNodeObject->scale[1]));
            translationNode->translation[2] = (s16) ((offset * gCurGraphNodeObject->cameraToObject[2]) / (dist * gCurGraphNodeObject->scale[2]));
        }
    }
    return NULL;
}

Gfx *geo_move_from_camera(s32 callContext, struct GraphNode *node, void *context) {
    return gCurGraphNodeObject ? __geo_move_from_camera(callContext, node, context, gCurGraphNodeObject->scale[1]) : NULL;
}

Gfx *geo_move_from_camera_mario_scale(s32 callContext, struct GraphNode *node, void *context) {
    return gMarioObject ? __geo_move_from_camera(callContext, node, context, gMarioObject->oScaleY) : NULL;
}

static void geo_call_global_function_nodes_helper(struct GraphNode *node, s32 callContext) {
    struct GraphNode *head = node;
    do {
        if ((node->type & GRAPH_NODE_TYPE_FUNCTIONAL) && gFnNode->func) {
            gFnNode->func(callContext, node, NULL);
        }
        if (node->children) {
            struct GraphNode *dummy = NULL;
            struct GraphNode **globalPtr = NULL;
            switch (node->type) {
                case GRAPH_NODE_TYPE_MASTER_LIST: globalPtr = (struct GraphNode **) &gCurGraphNodeMasterList; break;
                case GRAPH_NODE_TYPE_PERSPECTIVE: globalPtr = (struct GraphNode **) &gCurGraphNodeCamFrustum; break;
                case GRAPH_NODE_TYPE_CAMERA: globalPtr = (struct GraphNode **) &gCurGraphNodeCamera; break;
                case GRAPH_NODE_TYPE_OBJECT: globalPtr = (struct GraphNode **) &gCurGraphNodeObject; break;
                default: globalPtr = &dummy; break;
            }
            *globalPtr = node;
            geo_call_global_function_nodes_helper(node->children, callContext);
            *globalPtr = NULL;
        }
        node = node->next;
    } while (node != head);
}

void geo_call_global_function_nodes(struct GraphNode *node, s32 callContext) {
    if ((node->flags & GRAPH_RENDER_ACTIVE) && node->children) {
        gCurGraphNodeRoot = (struct GraphNodeRoot *) node;
        geo_call_global_function_nodes_helper(node->children, callContext);
        gCurGraphNodeRoot = NULL;
    }
}

void geo_reset_object_node(struct GraphNodeObject *node) {
    init_graph_node_object(NULL, node, NULL, gVec3fZero, gVec3sZero, gVec3fOne);
    geo_add_child(&gObjParentGraphNode, &node->node);
    gNode->flags &= ~GRAPH_RENDER_ACTIVE;
}

void geo_obj_init(struct GraphNodeObject *node, void *sharedChild, Vec3f pos, Vec3s angle) {
    vec3f_copy(node->pos, pos);
    vec3s_copy(node->angle, angle);
    vec3f_set(node->scale, 1.f, 1.f, 1.f);
    node->sharedChild = sharedChild;
    node->unk4C = NULL;
    node->throwMatrix = NULL;
    node->mAnimInfo.curAnim = NULL;
    node->mAnimInfo.curAnimRef = NULL;
    gNode->flags |= GRAPH_RENDER_ACTIVE;
    gNode->flags &= ~GRAPH_RENDER_INVISIBLE;
    gNode->flags |= GRAPH_RENDER_HAS_ANIMATION;
    gNode->flags &= ~GRAPH_RENDER_BILLBOARD;
}

void geo_obj_init_spawninfo(struct GraphNodeObject *node, struct SpawnInfo *spawn) {
    vec3s_to_vec3f(node->pos, spawn->startPos);
    vec3s_copy(node->angle, spawn->startAngle);
    vec3f_set(node->scale, 1.f, 1.f, 1.f);
    node->mAreaIndex = spawn->areaIndex;
    node->mActiveAreaIndex = spawn->activeAreaIndex;
    node->sharedChild = spawn->mModel;
    node->unk4C = spawn;
    node->throwMatrix = NULL;
    node->mAnimInfo.curAnim = NULL;
    node->mAnimInfo.curAnimRef = NULL;
    gNode->flags |= GRAPH_RENDER_ACTIVE;
    gNode->flags &= ~GRAPH_RENDER_INVISIBLE;
    gNode->flags |= GRAPH_RENDER_HAS_ANIMATION;
    gNode->flags &= ~GRAPH_RENDER_BILLBOARD;
}

//
// Geo data
//

void *geo_get_geo_data(struct Object *o, s32 structSize, const u32 *displayListsOffsets, s32 numOffsets) {
    if (!o->oGeoData) {
        void *geoData = omm_memory_new(gOmmMemoryPoolGeoData, structSize, o);
        Gfx **displayLists = (Gfx **) geoData;
        for (s32 i = 0; i != numOffsets; ++i) {
            displayLists[i] = (Gfx *) ((uintptr_t) geoData + (uintptr_t) displayListsOffsets[i]);
        }
        o->oGeoData = (void *) geoData;
    }
    return o->oGeoData;
}

Gfx *geo_link_geo_data(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *asGenerated = (struct GraphNodeGenerated *) node;
        struct GraphNodeDisplayList *displayListNode = (struct GraphNodeDisplayList *) node->next;
        Gfx **displayLists = (Gfx **) gCurrGraphNodeObject->oGeoData;
        if (displayLists) {
            displayListNode->displayList = (void *) displayLists[asGenerated->parameter];
        } else {
            displayListNode->displayList = NULL;
        }
    }
    return NULL;
}

#if OMM_GAME_IS_SM64
Gfx *geo_link_geo_data_skip_mirror_obj(s32 callContext, struct GraphNode *node, void *context) {
    if (callContext == GEO_CONTEXT_RENDER) {
        if (gOmmGlobals->isMirrorObj) {
            struct GraphNodeDisplayList *displayListNode = (struct GraphNodeDisplayList *) node->next;
            displayListNode->displayList = NULL;
            return NULL;
        }
        return geo_link_geo_data(callContext, node, context);
    }
    return NULL;
}
#endif

Gfx *gfx_copy_and_fill_null(Gfx *dest, const Gfx *src, s32 size, const Gfx *gfx) {
    mem_cpy(dest, src, size);
    for (s32 i = 0, n = size / sizeof(Gfx); i != n; ++i) {
        Gfx *cmd = ((Gfx *) dest) + i;
        if (_SHIFTR(cmd->words.w0, 24, 8) == G_DL && cmd->words.w1 == (uintptr_t) NULL) {
            cmd->words.w1 = (uintptr_t) gfx;
            break;
        }
    }
    return dest;
}

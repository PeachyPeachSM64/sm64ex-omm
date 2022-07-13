#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "game/memory.h"
#include "math_util.h"

#define GRAPH_RENDER_ACTIVE                     (1 << 0)
#define GRAPH_RENDER_CHILDREN_FIRST             (1 << 1)
#define GRAPH_RENDER_BILLBOARD                  (1 << 2)
#define GRAPH_RENDER_Z_BUFFER                   (1 << 3)
#define GRAPH_RENDER_INVISIBLE                  (1 << 4)
#define GRAPH_RENDER_HAS_ANIMATION              (1 << 5)
#define GRAPH_RENDER_CYLBOARD                   (1 << 6)
#define GRAPH_RENDER_ALWAYS                     (1 << 7)

#define GRAPH_NODE_TYPE_FUNCTIONAL              0x100
#define GRAPH_NODE_TYPE_400                     0x400

#define GRAPH_NODE_TYPE_ROOT                    0x001
#define GRAPH_NODE_TYPE_ORTHO_PROJECTION        0x002
#define GRAPH_NODE_TYPE_PERSPECTIVE            (0x003 | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_MASTER_LIST             0x004
#define GRAPH_NODE_TYPE_START                   0x00A
#define GRAPH_NODE_TYPE_LEVEL_OF_DETAIL         0x00B
#define GRAPH_NODE_TYPE_SWITCH_CASE            (0x00C | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_CAMERA                 (0x014 | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_TRANSLATION_ROTATION    0x015
#define GRAPH_NODE_TYPE_TRANSLATION             0x016
#define GRAPH_NODE_TYPE_ROTATION                0x017
#define GRAPH_NODE_TYPE_OBJECT                  0x018
#define GRAPH_NODE_TYPE_ANIMATED_PART           0x019
#define GRAPH_NODE_TYPE_BILLBOARD               0x01A
#define GRAPH_NODE_TYPE_DISPLAY_LIST            0x01B
#define GRAPH_NODE_TYPE_SCALE                   0x01C
#define GRAPH_NODE_TYPE_SHADOW                  0x028
#define GRAPH_NODE_TYPE_OBJECT_PARENT           0x029
#define GRAPH_NODE_TYPE_GENERATED_LIST         (0x02A | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_BACKGROUND             (0x02C | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_HELD_OBJ               (0x02E | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_CULLING_RADIUS          0x02F

#define GEO_CONTEXT_CREATE                      0 // called when node is created from a geo command
#define GEO_CONTEXT_RENDER                      1 // called from rendering_graph_node.c
#define GEO_CONTEXT_AREA_UNLOAD                 2 // called when unloading an area
#define GEO_CONTEXT_AREA_LOAD                   3 // called when loading an area
#define GEO_CONTEXT_AREA_INIT                   4 // called when initializing the 8 areas
#define GEO_CONTEXT_HELD_OBJ                    5 // called when processing a GraphNodeHeldObj

#define DISPLAY_LISTS_NUM_LAYERS                8

typedef Gfx *(*GraphNodeFunc)(s32 callContext, struct GraphNode *node, void *context);

struct DisplayListNode {
    Mtx *transform[MAX_INTERPOLATED_FRAMES];
    void *displayList[MAX_INTERPOLATED_FRAMES];
    struct DisplayListNode *next;
};

struct FnGraphNode {
    struct GraphNode node;
    GraphNodeFunc func;
};

struct GraphNodeRoot {
    struct GraphNode node;
    u8 areaIndex;
    s8 unk15;
    s16 x;
    s16 y;
    s16 width;    // half width, 160
    s16 height;   // half height
    s16 numViews; // number of entries in mystery array
    struct GraphNode **views;
};

struct GraphNodeOrthoProjection {
    struct GraphNode node;
    f32 scale;
};

struct GraphNodePerspective {
    struct FnGraphNode fnNode;
    s32 unused;
    f32 fov;  // horizontal field of view in degrees
    s16 near; // near clipping plane
    s16 far;  // far clipping plane
    f32_ts _fov;
};

struct GraphNodeMasterList {
    struct GraphNode node;
    struct DisplayListNode *listHeads[DISPLAY_LISTS_NUM_LAYERS];
    struct DisplayListNode *listTails[DISPLAY_LISTS_NUM_LAYERS];
};

struct GraphNodeStart {
    struct GraphNode node;
};

struct GraphNodeLevelOfDetail {
    struct GraphNode node;
    s16 minDistance;
    s16 maxDistance;
};

struct GraphNodeSwitchCase {
    struct FnGraphNode fnNode;
    s32 unused;
    s16 numCases;
    s16 selectedCase;
};

struct GraphNodeCamera {
    struct FnGraphNode fnNode;
    union { s32 mode; struct Camera *camera; } config;
    Vec3f pos;
    Vec3f focus;
    Mat4 *matrixPtr; // pointer to look-at matrix of this camera as a Mat4
    s16 roll;        // roll in look at matrix. Doesn't account for light direction unlike rollScreen.
    s16 rollScreen;  // rolls screen while keeping the light direction consistent
    Vec3f_ts _pos;
    Vec3f_ts _focus;
    Mat4 *lookAt[MAX_INTERPOLATED_FRAMES];
};

struct GraphNodeTranslationRotation {
    struct GraphNode node;
    void *displayList;
    Vec3s translation;
    Vec3s rotation;
    Vec3s_ts _translation;
    Vec3s_ts _rotation;
};

struct GraphNodeTranslation {
    struct GraphNode node;
    void *displayList;
    Vec3s translation;
    Vec3s_ts _translation;
};

struct GraphNodeRotation {
    struct GraphNode node;
    void *displayList;
    Vec3s rotation;
    Vec3s_ts _rotation;
};

struct GraphNodeAnimatedPart {
    struct GraphNode node;
    void *displayList;
    Vec3s translation;
    Vec3s_ts _translation;
};

struct GraphNodeBillboard {
    struct GraphNode node;
    void *displayList;
    Vec3s translation;
    Vec3s_ts _translation;
};

struct GraphNodeDisplayList {
    struct GraphNode node;
    void *displayList;
};

struct GraphNodeScale {
    struct GraphNode node;
    void *displayList;
    f32 scale;
    f32_ts _scale;
};

struct GraphNodeShadow {
    struct GraphNode node;
    s16 shadowScale;   // diameter (when a circle) or side (when a square) of shadow
    u8 shadowSolidity; // opacity of shadow, 255 = opaque
    u8 shadowType;     // see ShadowType enum in shadow.h
};

struct GraphNodeObjectParent {
    struct GraphNode node;
    struct GraphNode *sharedChild;
};

struct GraphNodeGenerated {
    struct FnGraphNode fnNode;
    u32 parameter; // extra context for the function
};

struct GraphNodeBackground {
    struct FnGraphNode fnNode;
    s32 unused;
    s32 background; // background ID, or rgba5551 color if fnNode.func is null
    Vec3f_ts _cameraPos;
    Vec3f_ts _cameraFocus;
};

struct GraphNodeHeldObject {
    struct FnGraphNode fnNode;
    s32 playerIndex;
    struct Object *objNode;
    Vec3s translation;
    Vec3s_ts _translation;
    Vec3f_ts _shadowPos;
    f32_ts _shadowScale;
    Mat4_ts _throwMatrix;
};

struct GraphNodeCullingRadius {
    struct GraphNode node;
    s16 cullingRadius; // specifies the 'sphere radius' for purposes of frustum culling
};

extern u16 gAreaUpdateCounter;
extern struct GraphNode gObjParentGraphNode;
extern struct GraphNodeRoot *gCurGraphNodeRoot;
extern struct GraphNodeMasterList *gCurGraphNodeMasterList;
extern struct GraphNodePerspective *gCurGraphNodeCamFrustum;
extern struct GraphNodeCamera *gCurGraphNodeCamera;
extern struct GraphNodeObject *gCurGraphNodeObject;
extern struct GraphNodeHeldObject *gCurGraphNodeHeldObject;

struct GraphNodeRoot                *init_graph_node_root                 (struct AllocOnlyPool *pool, struct GraphNodeRoot *node, s16 areaIndex, s16 x, s16 y, s16 width, s16 height);
struct GraphNodeOrthoProjection     *init_graph_node_ortho_projection     (struct AllocOnlyPool *pool, struct GraphNodeOrthoProjection *node, f32 scale);
struct GraphNodePerspective         *init_graph_node_perspective          (struct AllocOnlyPool *pool, struct GraphNodePerspective *node, f32 fov, s16 near, s16 far, GraphNodeFunc func, s32 unused);
struct GraphNodeStart               *init_graph_node_start                (struct AllocOnlyPool *pool, struct GraphNodeStart *node);
struct GraphNodeMasterList          *init_graph_node_master_list          (struct AllocOnlyPool *pool, struct GraphNodeMasterList *node, s16 enableZBuffer);
struct GraphNodeLevelOfDetail       *init_graph_node_render_range         (struct AllocOnlyPool *pool, struct GraphNodeLevelOfDetail *node, s16 minDistance, s16 maxDistance);
struct GraphNodeSwitchCase          *init_graph_node_switch_case          (struct AllocOnlyPool *pool, struct GraphNodeSwitchCase *node, s16 numCases, s16 selectedCase, GraphNodeFunc func, s32 unused);
struct GraphNodeCamera              *init_graph_node_camera               (struct AllocOnlyPool *pool, struct GraphNodeCamera *node, f32 *pos, f32 *focus, GraphNodeFunc func, s32 mode);
struct GraphNodeTranslationRotation *init_graph_node_translation_rotation (struct AllocOnlyPool *pool, struct GraphNodeTranslationRotation *node, s32 layer, void *displayList, Vec3s translation, Vec3s rotation);
struct GraphNodeTranslation         *init_graph_node_translation          (struct AllocOnlyPool *pool, struct GraphNodeTranslation *node, s32 layer, void *displayList, Vec3s translation);
struct GraphNodeRotation            *init_graph_node_rotation             (struct AllocOnlyPool *pool, struct GraphNodeRotation *node, s32 layer, void *displayList, Vec3s rotation);
struct GraphNodeScale               *init_graph_node_scale                (struct AllocOnlyPool *pool, struct GraphNodeScale *node, s32 layer, void *displayList, f32 scale);
struct GraphNodeObject              *init_graph_node_object               (struct AllocOnlyPool *pool, struct GraphNodeObject *node, struct GraphNode *sharedChild, Vec3f pos, Vec3s angle, Vec3f scale);
struct GraphNodeCullingRadius       *init_graph_node_culling_radius       (struct AllocOnlyPool *pool, struct GraphNodeCullingRadius *node, s16 radius);
struct GraphNodeAnimatedPart        *init_graph_node_animated_part        (struct AllocOnlyPool *pool, struct GraphNodeAnimatedPart *node, s32 layer, void *displayList, Vec3s translation);
struct GraphNodeBillboard           *init_graph_node_billboard            (struct AllocOnlyPool *pool, struct GraphNodeBillboard *node, s32 layer, void *displayList, Vec3s translation);
struct GraphNodeDisplayList         *init_graph_node_display_list         (struct AllocOnlyPool *pool, struct GraphNodeDisplayList *node, s32 layer, void *displayList);
struct GraphNodeShadow              *init_graph_node_shadow               (struct AllocOnlyPool *pool, struct GraphNodeShadow *node, s16 shadowScale, u8 shadowSolidity, u8 shadowType);
struct GraphNodeObjectParent        *init_graph_node_object_parent        (struct AllocOnlyPool *pool, struct GraphNodeObjectParent *node, struct GraphNode *sharedChild);
struct GraphNodeGenerated           *init_graph_node_generated            (struct AllocOnlyPool *pool, struct GraphNodeGenerated *node, GraphNodeFunc func, s32 parameter);
struct GraphNodeBackground          *init_graph_node_background           (struct AllocOnlyPool *pool, struct GraphNodeBackground *node, u16 background, GraphNodeFunc func);
struct GraphNodeHeldObject          *init_graph_node_held_object          (struct AllocOnlyPool *pool, struct GraphNodeHeldObject *node, struct Object *objNode, Vec3s translation, GraphNodeFunc func, s32 playerIndex);

struct GraphNode *geo_add_child(struct GraphNode *parent, struct GraphNode *node);
struct GraphNode *geo_remove_child(struct GraphNode *node);
struct GraphNode *geo_make_first_child(struct GraphNode *node);

void  geo_move_from_camera(struct GraphNodeObject *node, struct GraphNodeTranslation *tnode, f32 offset);
void  geo_call_global_function_nodes(struct GraphNode *node, s32 callContext);
void  geo_reset_object_node(struct GraphNodeObject *node);
void  geo_obj_init(struct GraphNodeObject *node, void *sharedChild, Vec3f pos, Vec3s angle);
void  geo_obj_init_spawninfo(struct GraphNodeObject *node, struct SpawnInfo *spawn);
void  geo_obj_init_animation(struct GraphNodeObject *node, struct Animation **animPtrAddr);
void  geo_obj_init_animation_accel(struct GraphNodeObject *node, struct Animation **animPtrAddr, u32 animAccel);
s32   retrieve_animation_index(s32 frame, u16 **attributes);
s16   geo_update_animation_frame(struct AnimInfoStruct *obj, s32 *accelAssist);
void *geo_get_geo_data(struct Object *o, s32 size, const Gfx *gfxSrc, s32 gfxSize);
Gfx  *geo_link_geo_data(s32 callContext, struct GraphNode *node, void *context);

#endif

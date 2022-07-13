#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

/**
 * This file contains the code that processes the scene graph for rendering.
 * The scene graph is responsible for drawing everything except the HUD / text boxes.
 * First the root of the scene graph is processed when geo_process_root
 * is called from level_script.viewport2. The rest of the tree is traversed recursively
 * using the function geo_process_node_and_siblings, which switches over all
 * geo node types and calls a specialized function accordingly.
 * The types are defined in engine/graph_node.h
 *
 * The scene graph typically looks like:
 * - Root (viewport)
 *  - Master list
 *   - Ortho projection
 *    - Background (skybox)
 *  - Master list
 *   - Perspective
 *    - Camera
 *     - <area-specific display lists>
 *     - Object parent
 *      - <group with 240 object nodes>
 *  - Master list
 *   - Script node (Cannon overlay)
 */

//
// Data
//

static Mat4 sMatStack[32][MAX_INTERPOLATED_FRAMES];
static Mtx *sMtxStack[32][MAX_INTERPOLATED_FRAMES];
static s32 sMatStackIndex;
static bool sIsPreprocess;
#define PREPROCESS if (OMM_UNLIKELY(sIsPreprocess))
#define NOT_PREPROCESS if (OMM_LIKELY(!sIsPreprocess))
#if OMM_CODE_DEBUG
static bool sDisableRendering;
#define SHOULD_DISABLE_RENDERING (gOmmDebugSurface && !omm_is_main_menu() && !omm_is_ending_cutscene() && !omm_is_ending_cake_screen())
#endif

//
// Frame interpolation
//

static Gfx *sPerspectivePos[MAX_INTERPOLATED_FRAMES];
static Mtx *sPerspectiveMtx[MAX_INTERPOLATED_FRAMES];
static OmmArray sMtxTbl[MAX_INTERPOLATED_FRAMES]; // pos, mtx, gfx

void gfx_patch_interpolated_frame_mtx(s32 k) {

    // Patch perspective
    if (sPerspectivePos[k] && sPerspectiveMtx[k]) {
        gSPMatrix(sPerspectivePos[k], sPerspectiveMtx[k], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    }

    // Patch matrices and display lists
    omm_array_for_each(sMtxTbl[k], x) {
        Gfx *pos = (Gfx *) (x + 0)->as_ptr;
        Mtx *mtx = (Mtx *) (x + 1)->as_ptr;
        Gfx *gfx = (Gfx *) (x + 2)->as_ptr;
        if (OMM_LIKELY(pos && mtx && gfx)) {
            gSPMatrix(pos++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(pos++, gfx);
        }
        i_x += 2; x += 2;
    }

    // Clear values
    sPerspectivePos[k] = NULL;
    omm_array_clear(sMtxTbl[k]);
}

#define should_interpolate(ts) (!sIsPreprocess && is_frame_interpolation_enabled() && check_timestamp(ts))

//
// Shortcuts
//

#define gNode           ((struct GraphNode *) node)
#define gNodeLayer      (gNode->flags >> 8)
#define gDisplayList    (((struct GraphNodeDisplayList *) node)->displayList)
#define gChildren       (gNode->children)
#define gFnNode         ((struct FnGraphNode *) node)
#define gCurrMat(k)     sMatStack[sMatStackIndex][k]
#define gNextMat(k)     sMatStack[sMatStackIndex + 1][k]
#define gCurrMatLast    sMatStack[sMatStackIndex][gNumInterpolatedFrames - 1]       // The current matrix of last subframe (current frame, t = 1)
#define gNextMatLast    sMatStack[sMatStackIndex + 1][gNumInterpolatedFrames - 1]   // The next matrix of last subframe (current frame, t = 1)

//
// Mario
//

static Vec3f sMarioForeArmPos[2];
f32 *geo_get_marios_forearm_pos(bool isLeft) {
    return sMarioForeArmPos[isLeft];
}

static s32 sMarioHandFlag;
static Vec3f sMarioHandPos[2];
f32 *geo_get_marios_hand_pos(bool isLeft) {
    return sMarioHandPos[isLeft];
}

static Vec3f sMarioHeadPos;
f32 *geo_get_marios_head_pos() {
    return sMarioHeadPos;
}

static Vec3s sMarioHeadRot;
s16 *geo_get_marios_head_rot() {
    return sMarioHeadRot;
}

static Vec3f sMarioHeadUp;
f32 *geo_get_marios_head_up() {
    return sMarioHeadUp;
}

static Vec3f sMarioHeadForward;
f32 *geo_get_marios_head_forward() {
    return sMarioHeadForward;
}

static Vec3f sMarioHeadRight;
f32 *geo_get_marios_head_right() {
    return sMarioHeadRight;
}

static s32 sMarioRootFlag;
static Vec3f sMarioRootPos;
f32 *geo_get_marios_root_pos() {
    return sMarioRootPos;
}

static f32 sMarioHeight; // Vanilla Mario's height is 120
f32 geo_get_marios_height() {
    return sMarioHeight;
}

static void geo_fix_marios_anim_translation_y(struct Object *o, f32 nodety, f32 *ty) {
    if (o == gMarioObject && sMarioRootFlag < gNumInterpolatedFrames) {
        sMarioRootFlag++;

        // Fix current animation y translation
        if (nodety != 0) {
            f32 dty;
            if (gMarioObject->oAnimID == MARIO_ANIM_DIVE) { // MARIO_ANIM_DIVE is weird: it has spikes in its y translation, so let's handle it separately
                f32 df = clamp_0_1_f(obj_anim_get_frame(gMarioObject) / 19.f);
                dty = lerp_f(df, 265.f, 85.f);
            } else {
                dty = *ty - nodety;
            }
            s32 dta = (invlerp_0_1_f(dty, 60.f, 150.f) * 0x8000) - 0x4000;
            *ty -= ((f32) nodety) * (1.f - ((sins(dta) + 1.f) / 2.f));
        }
        
        // Hanging action offset
        if (omm_mario_is_hanging(gMarioState)) {
            *ty += 2.f * (160.f - sMarioHeight);
        }
        
        // Jumbo star cutscene animation offset
        if (gMarioObject->oAnimID == MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN) {
            *ty += 4.f * (sMarioHeight - 120.f + (60.f * (gMarioState->action != ACT_JUMBO_STAR_CUTSCENE)));
        }
    }
}

//
// Animation data
//

#define ANIM_TYPE_NONE                  0
#define ANIM_TYPE_TRANSLATION           1
#define ANIM_TYPE_VERTICAL_TRANSLATION  2
#define ANIM_TYPE_LATERAL_TRANSLATION   3
#define ANIM_TYPE_NO_TRANSLATION        4
#define ANIM_TYPE_ROTATION              5

typedef struct {
    u8 type;
    u8 enabled;
    s16 frame, _frame;
    f32 mult;
    u16 *attr;
    s16 *data;
} GeoAnimState;
static GeoAnimState sCurAnimState[1];

static void geo_set_animation_globals(struct GraphNodeObject *node, bool updateAnimation) {
    struct AnimInfoStruct *animInfo = &node->mAnimInfo;
    struct Animation *anim = animInfo->curAnim;

    // Update the animation current frame
    if (updateAnimation) {
        animInfo->animFrame = geo_update_animation_frame(animInfo, &animInfo->animFrameAccelAssist);
    }
    animInfo->animTimer = gAreaUpdateCounter;

    // Get previous frame
    // Prevent interpolation if the animation changed
    if (check_timestamp(animInfo->_animID) && animInfo->_animID.v == animInfo->animID && animInfo->_curAnim.v == animInfo->curAnim) {
        sCurAnimState->_frame = animInfo->_animFrame.v;
    } else {
        sCurAnimState->_frame = animInfo->animFrame;

        // Don't interpolate object's angles on animation change
        reset_timestamp(node->_angle);

        // Don't interpolate object's pos on animation change if the position offset is too high
        // Not very accurate as it uses the object's velocity instead of previous pos, but it should
        // be enough to fix captured Toad animations and Peach's y offset on some animations
        if (node == &gMarioObject->header.gfx) {
            vec3f_copy(&((struct Object *) node)->oVelX, gMarioState->vel);
        }
        f32 offset = vec3f_dist(node->pos, node->_pos.v) - vec3f_length(&((struct Object *) node)->oVelX);
        if (offset > 30.f) {
            reset_timestamp(node->_pos);
        }
    }
NOT_PREPROCESS {
    update_timestamp_s16(animInfo->_animID, animInfo->animID);
    update_timestamp_s16(animInfo->_animFrame, animInfo->animFrame);
    update_timestamp_ptr(animInfo->_curAnim, animInfo->curAnim);
}

    // Fill sCurAnimState
         if (anim->flags & ANIM_FLAG_HOR_TRANS)  sCurAnimState->type = ANIM_TYPE_VERTICAL_TRANSLATION;
    else if (anim->flags & ANIM_FLAG_VERT_TRANS) sCurAnimState->type = ANIM_TYPE_LATERAL_TRANSLATION;
    else if (anim->flags & ANIM_FLAG_6)          sCurAnimState->type = ANIM_TYPE_NO_TRANSLATION;
    else                                         sCurAnimState->type = ANIM_TYPE_TRANSLATION;
    sCurAnimState->enabled = !(anim->flags & ANIM_FLAG_5);
    sCurAnimState->frame   = animInfo->animFrame;
    sCurAnimState->mult    = (anim->mAnimYTransDivisor ? (f32) animInfo->animYTrans / (f32) anim->mAnimYTransDivisor : 1.f);
    sCurAnimState->attr    = (u16 *) anim->index;
    sCurAnimState->data    = (s16 *) anim->values;
}

static void geo_get_animation_data_as_vec3s(Vec3s dest, u16 **attr, s16 *data, s16 frame, bool x, bool y, bool z, bool updateAttr) {
    s32 index0 = retrieve_animation_index(frame, attr);
    s32 index1 = retrieve_animation_index(frame, attr);
    s32 index2 = retrieve_animation_index(frame, attr);
    dest[0] = (s16) (x ? data[index0] : 0);
    dest[1] = (s16) (y ? data[index1] : 0);
    dest[2] = (s16) (z ? data[index2] : 0);
    if (!updateAttr) *attr -= 6;
}

static void geo_get_animation_data_as_vec3f(Vec3f dest, u16 **attr, s16 *data, s16 frame, bool x, bool y, bool z, bool updateAttr) {
    s32 index0 = retrieve_animation_index(frame, attr);
    s32 index1 = retrieve_animation_index(frame, attr);
    s32 index2 = retrieve_animation_index(frame, attr);
    dest[0] = (f32) (x ? data[index0] : 0);
    dest[1] = (f32) (y ? data[index1] : 0);
    dest[2] = (f32) (z ? data[index2] : 0);
    if (!updateAttr) *attr -= 6;
}

static void geo_get_animation_translation(Vec3f dest, u8 type, u16 **attr, s16 *data, s16 frame, f32 mult, bool updateAttr) {
    switch (type) {
        case ANIM_TYPE_TRANSLATION:          geo_get_animation_data_as_vec3f(dest, attr, data, frame, 1, 1, 1, updateAttr); break;
        case ANIM_TYPE_VERTICAL_TRANSLATION: geo_get_animation_data_as_vec3f(dest, attr, data, frame, 0, 1, 0, updateAttr); break;
        case ANIM_TYPE_LATERAL_TRANSLATION:  geo_get_animation_data_as_vec3f(dest, attr, data, frame, 1, 0, 1, updateAttr); break;
        case ANIM_TYPE_NO_TRANSLATION:       geo_get_animation_data_as_vec3f(dest, attr, data, frame, 0, 0, 0, updateAttr); break;
        default:                             vec3f_zero(dest); break;
    }
    vec3f_mul(dest, mult);
}

//
// Display lists
//

static struct DisplayListNode sDisplayListNodePools[DISPLAY_LISTS_NUM_LAYERS][0x2000];

static void __geo_append_display_lists(Gfx **displayLists, s32 layer, bool repeatFirst) {
#if OMM_CODE_DEBUG
    if (sDisableRendering) return;
#endif
NOT_PREPROCESS {
    if (displayLists && displayLists[0] && OMM_LIKELY(gCurGraphNodeMasterList)) {
        if (OMM_UNLIKELY(!gCurGraphNodeMasterList->listHeads[layer])) {
            gCurGraphNodeMasterList->listHeads[layer] = sDisplayListNodePools[layer];
            gCurGraphNodeMasterList->listTails[layer] = sDisplayListNodePools[layer];
        }
        struct DisplayListNode *dlNode = gCurGraphNodeMasterList->listTails[layer];
        interpolate {
            dlNode->transform[k] = sMtxStack[sMatStackIndex][k];
            dlNode->displayList[k] = displayLists[repeatFirst ? 0 : k];
        }
        gCurGraphNodeMasterList->listTails[layer]++;
    }
}
}

OMM_INLINE void geo_append_display_list(Gfx *displayList, s16 layer) {
    __geo_append_display_lists(&displayList, layer, true);
}

OMM_INLINE void geo_append_display_lists(Gfx **displayLists, s16 layer) {
    __geo_append_display_lists(displayLists, layer, false);
}

//
// Master node
//

static void geo_process_master_list(struct GraphNodeMasterList *node) {
    static const u32 sRenderModeTable1Cycle[2][DISPLAY_LISTS_NUM_LAYERS] = {
        { G_RM_OPA_SURF,       G_RM_AA_OPA_SURF,    G_RM_AA_OPA_SURF,     G_RM_AA_OPA_SURF,
          G_RM_AA_TEX_EDGE,    G_RM_AA_XLU_SURF,    G_RM_AA_XLU_SURF,     G_RM_AA_XLU_SURF,     },
        { G_RM_ZB_OPA_SURF,    G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_DECAL, G_RM_AA_ZB_OPA_INTER,
          G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_INTER, },
    };
    static const u32 sRenderModeTable2Cycle[2][DISPLAY_LISTS_NUM_LAYERS] = {
        { G_RM_OPA_SURF2,       G_RM_AA_OPA_SURF2,    G_RM_AA_OPA_SURF2,     G_RM_AA_OPA_SURF2,
          G_RM_AA_TEX_EDGE2,    G_RM_AA_XLU_SURF2,    G_RM_AA_XLU_SURF2,     G_RM_AA_XLU_SURF2,     },
        { G_RM_ZB_OPA_SURF2,    G_RM_AA_ZB_OPA_SURF2, G_RM_AA_ZB_OPA_DECAL2, G_RM_AA_ZB_OPA_INTER2,
          G_RM_AA_ZB_TEX_EDGE2, G_RM_AA_ZB_XLU_SURF2, G_RM_AA_ZB_XLU_DECAL2, G_RM_AA_ZB_XLU_INTER2, },
    };
    if (!gCurGraphNodeMasterList && gChildren) {

        // Gather display list nodes
        gCurGraphNodeMasterList = node;
        OMM_MEMSET(node->listHeads, 0, sizeof(node->listHeads));
        geo_process_node_and_siblings(gChildren);
        
        // Enable Z buffer
        bool zBuffer = (node->node.flags & GRAPH_RENDER_Z_BUFFER) != 0;
        if (zBuffer) {
            gDPPipeSync(gDisplayListHead++);
            gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
        }

        // Process display list nodes
        for (s32 i = 0; i != DISPLAY_LISTS_NUM_LAYERS; ++i) {
            if (node->listHeads[i]) {
                gDPSetRenderMode(gDisplayListHead++, sRenderModeTable1Cycle[zBuffer][i], sRenderModeTable2Cycle[zBuffer][i]);
                struct DisplayListNode *head = node->listHeads[i];
                struct DisplayListNode *tail = node->listTails[i];
                for (struct DisplayListNode *dlNode = head; dlNode != tail; dlNode++) {
                    interpolate1 {
                        omm_array_add(sMtxTbl[k], ptr, gDisplayListHead);
                        omm_array_add(sMtxTbl[k], ptr, dlNode->transform[k]);
                        omm_array_add(sMtxTbl[k], ptr, dlNode->displayList[k]);
                    }
                    gSPMatrix(gDisplayListHead++, dlNode->transform[0], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
                    gSPDisplayList(gDisplayListHead++, dlNode->displayList[0]);
                }
            }
        }

        // Disable Z buffer
        if (zBuffer) {
            gDPPipeSync(gDisplayListHead++);
            gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER);
        }
        gCurGraphNodeMasterList = NULL;
    }
}

//
// Utils
//

OMM_INLINE Gfx *geo_exec_node_func(struct FnGraphNode *node, s32 callContext, void *context) {
    if (node->func) {
        return node->func(callContext, gNode, context);
    }
    return NULL;
}

OMM_INLINE bool __geo_inc_mat_stack() {
    sMatStackIndex++;
NOT_PREPROCESS {
    interpolate {
        Mtx *mtx = alloc_display_list(sizeof(Mtx));
        OMM_MEMCPY(mtx, sMatStack[sMatStackIndex][k], sizeof(Mtx));
        sMtxStack[sMatStackIndex][k] = mtx;
    }
}
    return false;
}

OMM_INLINE bool __geo_dec_mat_stack() {
    sMatStackIndex--;
    return true;
}

#define geo_update_mat_stack(...) { \
    __geo_inc_mat_stack(); \
    { __VA_ARGS__ } \
    __geo_dec_mat_stack(); \
}

static bool obj_is_in_view(struct Object *obj, Mat4 matrix) {
#if OMM_CODE_DEBUG
    // Do not render the following while gOmmDebugSurface is enabled:
    // - Objects with collision data
    // - Surface objects
    // - Level geometry, doors and level objects
    if (SHOULD_DISABLE_RENDERING) {
        if (obj->collisionData) {
            return false;
        }
        if (obj_get_list_index(obj) == OBJ_LIST_SURFACE) {
            return false;
        }
        for (s32 modelId = 0x03; modelId <= 0x53; ++modelId) {
            if (modelId == 0x17) modelId = 0x1C;
            if (modelId == 0x30) modelId = 0x35;
            if (obj_check_model(obj, modelId)) {
                return false;
            }
        }
    }
#endif
    
    // Render the object regardless of its position if it's always rendered
    if (obj_is_always_rendered(obj)) {
        return true;
    }

    // Do not render the object if it's invisible
    if (obj->oNodeFlags & GRAPH_RENDER_INVISIBLE) {
        return false;
    }

    // Retrieve the culling radius
    f32 cullingRadius = 300.f;
    struct GraphNode *geo = obj->oGraphNode;
    if (geo && geo->type == GRAPH_NODE_TYPE_CULLING_RADIUS) {
        cullingRadius = ((struct GraphNodeCullingRadius *) geo)->cullingRadius;
    }
    
    // Don't render if the object is close to, behind or too far away from the camera
    f32 tz = matrix[3][2];
    if (tz > +cullingRadius - 100.f ||
        tz < -cullingRadius - 20000.f) {
        return false;
    }
    
    // Compute the FoV and screen edge
    s16 halfFov = (s16) ((((((gCurGraphNodeCamFrustum->fov * GFX_DIMENSIONS_ASPECT_RATIO) / 2.f) + 1.f) * 0x8000) / 180.f) + 0.5f);
    f32 screenEdge = -tz * sins(halfFov) / coss(halfFov);
    
    // Check whether the object is horizontally in view
    f32 tx = matrix[3][0];
    if (tx > +cullingRadius + screenEdge ||
        tx < -cullingRadius - screenEdge) {
        return false;
    }

    // Render the object
    return true;
}

//
// Graph nodes
//

static void geo_process_ortho_projection(struct GraphNodeOrthoProjection *node) {
    if (gChildren) {

        // Compute ortho projection matrix
        f32 x0 = ((gCurGraphNodeRoot->x - gCurGraphNodeRoot->width ) >> 1) * node->scale;
        f32 x1 = ((gCurGraphNodeRoot->x + gCurGraphNodeRoot->width ) >> 1) * node->scale;
        f32 y0 = ((gCurGraphNodeRoot->y - gCurGraphNodeRoot->height) >> 1) * node->scale;
        f32 y1 = ((gCurGraphNodeRoot->y + gCurGraphNodeRoot->height) >> 1) * node->scale;
        Mtx *mtx = alloc_display_list(sizeof(Mtx));
        mtxf_ortho(mtx, x0, x1, y1, y0, -2.f, 2.f, 1.f);

        // Disable perspective, push matrix
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
        gSPMatrix(gDisplayListHead++, mtx, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);

        // Process children
        geo_process_node_and_siblings(gChildren);
    }
}

static void geo_process_perspective(struct GraphNodePerspective *node) {
    geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMatLast);
    if (gChildren) {
        f32 aspect = (f32) gCurGraphNodeRoot->width / (f32) gCurGraphNodeRoot->height;
        u16 perspNorm = 0;

        // Compute a perspective matrix for each sub-frame
        bool shouldInterpolate = should_interpolate(node->_fov);
        interpolate {
            f32 fov; interpolate_f32(fov, node->_fov.v, node->fov);
            sPerspectivePos[k] = gDisplayListHead;
            sPerspectiveMtx[k] = alloc_display_list(sizeof(Mtx));
            perspNorm = mtxf_perspective(sPerspectiveMtx[k], fov, aspect, node->near, node->far, 1.f);
        }

        // Push first sub-frame matrix and update timestamp
        gSPPerspNormalize(gDisplayListHead++, perspNorm);
        gSPMatrix(gDisplayListHead++, sPerspectiveMtx[0], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
NOT_PREPROCESS {
        update_timestamp_f32(node->_fov, node->fov);
}

        // Process children
        gCurGraphNodeCamFrustum = node;
        geo_process_node_and_siblings(gChildren);
        gCurGraphNodeCamFrustum = NULL;
    }
}

static void geo_process_level_of_detail(struct GraphNodeLevelOfDetail *node) {
    if (node->minDistance <= 0.f && 0.f < node->maxDistance) {
        geo_process_node_and_siblings(gChildren);
    }
}

static void geo_process_switch(struct GraphNodeSwitchCase *node) {
    geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMatLast);
PREPROCESS {

    // Mario forearm pos
    if (gFnNode->func == (GraphNodeFunc) geo_switch_mario_hand) {
        static bool sIsLeftHand = true;
        sMarioHandFlag = 1 + sIsLeftHand;
        vec3f_copy(sMarioForeArmPos[sIsLeftHand], gCurrMatLast[3]);
        sIsLeftHand = !sIsLeftHand;
    }

    // Mario head pos and vectors
    if (gFnNode->func == (GraphNodeFunc) geo_switch_mario_eyes) {
        vec3f_copy(sMarioHeadPos, gCurrMatLast[3]);
        Mat4 rot;
        mtxf_copy(rot, gCurrMatLast);
        vec3f_zero(rot[3]);
        Mat4 up = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 1, 0, 0, 1 } };
        mtxf_mul(up, up, rot);
        vec3f_copy(sMarioHeadUp, up[3]);
        vec3f_normalize(sMarioHeadUp);
        Mat4 forward = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 1, 0, 1 } };
        mtxf_mul(forward, forward, rot);
        vec3f_copy(sMarioHeadForward, forward[3]);
        vec3f_normalize(sMarioHeadForward);
        Mat4 right = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, -1, 1 } };
        mtxf_mul(right, right, rot);
        vec3f_copy(sMarioHeadRight, right[3]);
        vec3f_normalize(sMarioHeadRight);
    }
}

    // Process only the selected child
    struct GraphNode *selectedChild = gChildren;
    for (s32 i = 0; selectedChild && i < node->selectedCase; ++i) {
        selectedChild = selectedChild->next;
    }
    geo_process_node_and_siblings(selectedChild);
NOT_PREPROCESS {
    omm_cappy_gfx_draw_eyes(node, (void (*)(void *, s16)) geo_append_display_list);
}
}

static void geo_process_camera(struct GraphNodeCamera *node) {
    geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMatLast);
    
    // Roll matrix
    Mtx *mtxRoll = alloc_display_list(sizeof(Mtx));
    mtxf_rotate_xy(mtxRoll, node->rollScreen);
    gSPMatrix(gDisplayListHead++, mtxRoll, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);

    // Transform matrix
    bool shouldInterpolate = should_interpolate(node->_pos);
    interpolate {
        Vec3f pos; interpolate_vec3f(pos, node->_pos.v, node->pos);
        Vec3f focus; interpolate_vec3f(focus, node->_focus.v, node->focus);
        Mat4 mat; mtxf_lookat(mat, pos, focus, node->roll);
        mtxf_mul(gNextMat(k), mat, gCurrMat(k));
    }
NOT_PREPROCESS {
    update_timestamp_vec3f(node->_pos, node->pos);
    update_timestamp_vec3f(node->_focus, node->focus);
}

    // Process children
    geo_update_mat_stack(
        if (gChildren) {
#if OMM_CODE_DEBUG
            sDisableRendering = SHOULD_DISABLE_RENDERING;
#endif
            gCurGraphNodeCamera = node;
            interpolate {
                node->lookAt[k] = &gCurrMat(k);
            }
            node->matrixPtr = node->lookAt[gNumInterpolatedFrames - 1];
            geo_process_node_and_siblings(gChildren);
            gCurGraphNodeCamera = NULL;
        }
    )
}

static void geo_process_translation_rotation(struct GraphNodeTranslationRotation *node) {
    bool shouldInterpolate = should_interpolate(node->_translation);
    interpolate {

        // Translation
        Vec3s t3s; interpolate_vec3s(t3s, node->_translation.v, node->translation);
        Vec3f t3f; vec3s_to_vec3f(t3f, t3s);

        // Rotation
        Vec3s rot; interpolate_angles(rot, node->_rotation.v, node->rotation);
        Mat4 mat; mtxf_rotate_zxy_and_translate(mat, t3f, rot);

        // Transform
        mtxf_mul(gNextMat(k), mat, gCurrMat(k));
    }
NOT_PREPROCESS {
    update_timestamp_vec3s(node->_translation, node->translation);
    update_timestamp_vec3s(node->_rotation, node->rotation);
}

    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_translation(struct GraphNodeTranslation *node) {
    bool shouldInterpolate = should_interpolate(node->_translation);
    interpolate {

        // Translation
        Vec3s t3s; interpolate_vec3s(t3s, node->_translation.v, node->translation);
        Vec3f t3f; vec3s_to_vec3f(t3f, t3s);
        Mat4 mat; mtxf_translate(mat, t3f);

        // Transform
        mtxf_mul(gNextMat(k), mat, gCurrMat(k));
    }
NOT_PREPROCESS {
    update_timestamp_vec3s(node->_translation, node->translation);
}

    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_rotation(struct GraphNodeRotation *node) {
    bool shouldInterpolate = should_interpolate(node->_rotation);
    interpolate {

        // Rotation
        Vec3s rot; interpolate_angles(rot, node->_rotation.v, node->rotation);
        Mat4 mat; mtxf_rotate_zxy_and_translate(mat, gVec3fZero, rot);

        // Transform
        mtxf_mul(gNextMat(k), mat, gCurrMat(k));
    }
NOT_PREPROCESS {
    update_timestamp_vec3s(node->_rotation, node->rotation);
}

    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_scale(struct GraphNodeScale *node) {
    bool shouldInterpolate = should_interpolate(node->_scale);
    interpolate {

        // Scale
        f32 sf; interpolate_f32(sf, node->_scale.v, node->scale);

        // Transform
        mtxf_scale_vec3f(gNextMat(k), gCurrMat(k), (Vec3f) { sf, sf, sf });
    }
NOT_PREPROCESS {
    update_timestamp_f32(node->_scale, node->scale);
}

    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_billboard(struct GraphNodeBillboard *node) {
    bool shouldInterpolate = should_interpolate(node->_translation);
    interpolate {

        // Translation
        Vec3s t3s; interpolate_vec3s(t3s, node->_translation.v, node->translation);
        Vec3f t3f; vec3s_to_vec3f(t3f, t3s);

        // Transform
        mtxf_billboard(gNextMat(k), gCurrMat(k), t3f, gCurGraphNodeCamera->roll);

        // If held object, use its scale, otherwise, use the current object scale
        if (gCurGraphNodeHeldObject) {
            mtxf_scale_vec3f(gNextMat(k), gNextMat(k), gCurGraphNodeHeldObject->objNode->oGfxScale);
        } else if (gCurGraphNodeObject) {
            mtxf_scale_vec3f(gNextMat(k), gNextMat(k), gCurGraphNodeObject->scale);
        }
    }
NOT_PREPROCESS {
    update_timestamp_vec3s(node->_translation, node->translation);
}

    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_display_list(struct GraphNodeDisplayList *node) {
    geo_append_display_list(gDisplayList, gNodeLayer);
    geo_process_node_and_siblings(gChildren);
}

static void geo_process_generated_list(struct GraphNodeGenerated *node) {
    Gfx *gfx = geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMatLast);
PREPROCESS {

    // Mario head rotation
    if (gFnNode->func == (GraphNodeFunc) geo_mario_head_rotation) {
        struct GraphNodeRotation *rotNode = (struct GraphNodeRotation *) gNode->next;
        vec3s_add(sMarioHeadRot, rotNode->rotation);
    }
}
    // Process children
    geo_append_display_list(gfx, gNodeLayer);
    geo_process_node_and_siblings(gChildren);
}

static void geo_process_background(struct GraphNodeBackground *node) {
    Gfx *gfx[MAX_INTERPOLATED_FRAMES] = { NULL };

    // Create a background from a function...
    if (gFnNode->func) {
        bool shouldInterpolate = should_interpolate(node->_cameraPos);
        Vec3f cpos; vec3f_copy(cpos, gLakituState.pos);
        Vec3f cfocus; vec3f_copy(cfocus, gLakituState.focus);
        interpolate {
            Vec3f pos; interpolate_vec3f(pos, node->_cameraPos.v, cpos);
            Vec3f focus; interpolate_vec3f(focus, node->_cameraFocus.v, cfocus);
            vec3f_copy(gLakituState.pos, pos);
            vec3f_copy(gLakituState.focus, focus);
            gfx[k] = gFnNode->func(GEO_CONTEXT_RENDER, gNode, gCurrMat(k));
        }
        vec3f_copy(gLakituState.pos, cpos);
        vec3f_copy(gLakituState.focus, cfocus);
NOT_PREPROCESS {
        update_timestamp_vec3f(node->_cameraPos, gLakituState.pos);
        update_timestamp_vec3f(node->_cameraFocus, gLakituState.focus);
}
    }

    // ...or from a solid color
    if (*gfx) {
        geo_append_display_lists(gfx, gNodeLayer);
    } else if (gCurGraphNodeMasterList) {
        *gfx = alloc_display_list(sizeof(Gfx) * 6);
        geo_append_display_list(*gfx, LAYER_FORCE);
        gDPSetCycleType((*gfx)++, G_CYC_FILL);
        gDPSetFillColor((*gfx)++, node->background);
        gDPFillRectangle((*gfx)++, GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), 0, GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, SCREEN_HEIGHT - 1);
        gDPSetCycleType((*gfx)++, G_CYC_1CYCLE);
        gSPEndDisplayList((*gfx)++);
    }

    // Process children
    geo_process_node_and_siblings(gChildren);
}

static void geo_process_animated_part(struct GraphNodeAnimatedPart *node) {

    // Animation translations (previous, current)
    Vec3f pt3f; geo_get_animation_translation(pt3f, sCurAnimState->type, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->_frame, sCurAnimState->mult, false);
    Vec3f ct3f; geo_get_animation_translation(ct3f, sCurAnimState->type, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->frame, sCurAnimState->mult, true);

    // Animation rotations (previous, current)
    Vec3s prot, crot;
    if (sCurAnimState->type != ANIM_TYPE_NONE) {
        sCurAnimState->type = ANIM_TYPE_ROTATION;
        geo_get_animation_data_as_vec3s(prot, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->_frame, 1, 1, 1, false);
        geo_get_animation_data_as_vec3s(crot, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->frame, 1, 1, 1, true);
    } else {
        vec3s_copy(prot, gVec3sZero);
        vec3s_copy(crot, gVec3sZero);
    }
    
    // Interpolation
    bool shouldInterpolate = should_interpolate(node->_translation);
    Vec3f t3f; Vec3s rot;
    interpolate {
        
        // Translation
        Vec3s t3s; interpolate_vec3s(t3s, node->_translation.v, node->translation);
        Vec3f at3f; interpolate_vec3f(at3f, pt3f, ct3f);
        vec3s_to_vec3f(t3f, t3s);
        vec3f_add(t3f, at3f);
        
        // Rotation
        interpolate_angles(rot, prot, crot);
        
        // Transform
        geo_fix_marios_anim_translation_y(gCurrGraphNodeObject, t3s[1], &t3f[1]);
        Mat4 mat; mtxf_rotate_xyz_and_translate(mat, t3f, rot);
        mtxf_mul(gNextMat(k), mat, gCurrMat(k));
    }
NOT_PREPROCESS {
    update_timestamp_vec3s(node->_translation, node->translation);
}

PREPROCESS {

    // Mario root pos
    if (sMarioRootFlag == gNumInterpolatedFrames) {
        vec3f_copy(sMarioRootPos, gNextMatLast[3]);
        sMarioRootFlag++;
    }
    
    // Mario hand pos
    if (sMarioHandFlag) {
        vec3f_copy(sMarioHandPos[sMarioHandFlag - 1], gNextMatLast[3]);
        if (sMarioHandFlag == 1 && OMM_PLAYER_IS_PEACH) { // Right hand
            omm_peach_update_perry_graphics(gMarioState, gCurrMatLast, t3f, rot);
        }
        sMarioHandFlag = 0;
    }
}
    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_shadow(struct GraphNodeShadow *node) {
NOT_PREPROCESS {
    if (gCurGraphNodeCamera && gCurGraphNodeObject) {
        Vec3f shadowPos;
        f32 shadowScale;
        
        // Get shadow pos and scale from held object if there's one
        if (gCurGraphNodeHeldObject) {
            get_pos_from_transform_mtx(shadowPos, gCurrMatLast, *gCurGraphNodeCamera->matrixPtr);
            shadowScale = node->shadowScale;
        } else {
            vec3f_copy(shadowPos, gCurGraphNodeObject->pos);
            obj_fix_shadow_pos(gCurGraphNodeObject, shadowPos);
            shadowScale = node->shadowScale * gCurGraphNodeObject->scale[0];
        }
        
        // Translate and scale the shadow with the current animation frame
        if (sCurAnimState->enabled && (sCurAnimState->type == ANIM_TYPE_TRANSLATION || sCurAnimState->type == ANIM_TYPE_LATERAL_TRANSLATION)) {

            // If the first child is a scale node, use its value
            f32 objScale = 1.f;
            struct GraphNode *geo = gChildren;
            if (geo && geo->type == GRAPH_NODE_TYPE_SCALE) {
                objScale = ((struct GraphNodeScale *) geo)->scale;
            }

            // Retrieve the current frame translation
            Vec3f animTranslation;
            geo_get_animation_data_as_vec3f(animTranslation, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->frame, 1, 0, 1, 0);
            vec3f_mul(animTranslation, sCurAnimState->mult * objScale);

            // The shadow offset rotates along with the object
            f32 sinYaw = sins(gCurGraphNodeObject->angle[1]);
            f32 cosYaw = coss(gCurGraphNodeObject->angle[1]);
            shadowPos[0] += +animTranslation[0] * cosYaw + animTranslation[2] * sinYaw;
            shadowPos[2] += -animTranslation[0] * sinYaw + animTranslation[2] * cosYaw;
        }
        
        // Interpolate from previous shadow values
        Vec3f_ts *_shadowPos;
        f32_ts *_shadowScale;
        if (gCurGraphNodeHeldObject) {
            _shadowPos = &gCurGraphNodeHeldObject->_shadowPos;
            _shadowScale = &gCurGraphNodeHeldObject->_shadowScale;
        } else {
            _shadowPos = &gCurGraphNodeObject->_shadowPos;
            _shadowScale = &gCurGraphNodeObject->_shadowScale;
        }
        
        // Create shadow display lists and translate
        Gfx *shadows[MAX_INTERPOLATED_FRAMES] = { NULL };
        bool shouldInterpolate = should_interpolate(*_shadowPos);
        interpolate {

            // Position and scale
            Vec3f t3f; interpolate_vec3f(t3f, _shadowPos->v, shadowPos);
            f32 scale; interpolate_f32(scale, _shadowScale->v, shadowScale);

            // Create shadow
            shadows[k] = create_shadow_below_xyz(t3f[0], t3f[1], t3f[2], scale, node->shadowSolidity, node->shadowType);
            if (shadows[k]) {
                Mat4 mat; mtxf_translate(mat, t3f);
                mtxf_mul(gNextMat(k), mat, *(gCurGraphNodeCamera->lookAt[k]));
            }
        }
NOT_PREPROCESS {
        update_timestamp_vec3f(*_shadowPos, shadowPos);
        update_timestamp_f32(*_shadowScale, shadowScale);
}

        // Append shadows
        if (*shadows) {
            geo_update_mat_stack(
                geo_append_display_lists(shadows, shadow_get_layer());
            )
        }
    }
}
    // Process children
    geo_process_node_and_siblings(gChildren);
}

static void geo_process_object(struct Object *obj) {
    if (obj->oAreaIndex == gCurGraphNodeRoot->areaIndex) {
        struct GraphNodeObject *node = &obj->header.gfx;

        // Init animation state if the object has animations
        if (node->mAnimInfo.curAnim) {
#if OMM_CODE_DYNOS
            dynos_gfx_swap_animations(obj);
#endif
            geo_set_animation_globals(node, (node->node.flags & GRAPH_RENDER_HAS_ANIMATION) != 0);
#if OMM_CODE_DYNOS
            dynos_gfx_swap_animations(obj);
#endif
        }
        
        // Compute the object transform matrix from its throw matrix
        if (node->throwMatrix != NULL) {
            bool shouldInterpolate = should_interpolate(node->_throwMatrix);
            interpolate {
                Mat4 mat; interpolate_mtxf(mat, node->_throwMatrix.v, *node->throwMatrix);
                mtxf_mul(gNextMat(k), mat, gCurrMat(k));
            }
            update_timestamp_mtxf(node->_throwMatrix, *node->throwMatrix);
        }
        
        // Compute the object transform matrix from its position and cylboard
        else if (node->node.flags & GRAPH_RENDER_CYLBOARD) {
            bool shouldInterpolate = should_interpolate(node->_pos);
            interpolate {
                Vec3f pos; interpolate_vec3f(pos, node->_pos.v, node->pos);
                mtxf_cylboard(gNextMat(k), gCurrMat(k), pos, gCurGraphNodeCamera->roll);
            }
            update_timestamp_vec3f(node->_pos, node->pos);
        }
        
        // Compute the object transform matrix from its position and billboard
        else if (node->node.flags & GRAPH_RENDER_BILLBOARD) {
            bool shouldInterpolate = should_interpolate(node->_pos);
            interpolate {
                Vec3f pos; interpolate_vec3f(pos, node->_pos.v, node->pos);
                mtxf_billboard(gNextMat(k), gCurrMat(k), pos, gCurGraphNodeCamera->roll);
            }
            update_timestamp_vec3f(node->_pos, node->pos);
        }
        
        // Compute the object transform matrix from its position and angle
        else {
            bool shouldInterpolate = should_interpolate(node->_pos);
            interpolate {
                Vec3f pos; interpolate_vec3f(pos, node->_pos.v, node->pos);
                Vec3s angle; interpolate_angles(angle, (check_timestamp(node->_angle) ? node->_angle.v : node->angle), node->angle);
                Mat4 mat; mtxf_rotate_zxy_and_translate(mat, pos, angle);
                mtxf_mul(gNextMat(k), mat, gCurrMat(k));
            }
            update_timestamp_vec3f(node->_pos, node->pos);
            update_timestamp_vec3s(node->_angle, node->angle);
        }
        
        // Scale
        bool shouldInterpolate = should_interpolate(node->_scale);
        interpolate { 
            Vec3f scale; interpolate_vec3f(scale, node->_scale.v, node->scale);
            mtxf_scale_vec3f(gNextMat(k), gNextMat(k), scale);
        }
        node->throwMatrix = &gNextMatLast;
        vec3f_copy(node->cameraToObject, gNextMatLast[3]);
NOT_PREPROCESS {
        update_timestamp_vec3f(node->_scale, node->scale);
}

        // Render the object if it's in view
        if (obj_is_in_view(obj, gNextMatLast)) {
            geo_update_mat_stack(
                if (node->sharedChild) {
                    gCurGraphNodeObject = (struct GraphNodeObject *) node;
                    node->sharedChild->parent = gNode;
                    geo_process_node_and_siblings(node->sharedChild);
                    node->sharedChild->parent = NULL;
                    gCurGraphNodeObject = NULL;
                }

                // Process children
                geo_process_node_and_siblings(gChildren);
            )
        } else if (obj != gMarioObject && obj != gOmmData->mario->capture.obj) {
NOT_PREPROCESS {
            reset_timestamp(node->_angle);
            reset_timestamp(node->_pos);
            reset_timestamp(node->_scale);
            reset_timestamp(node->_throwMatrix);
}
        }
        sCurAnimState->type = ANIM_TYPE_NONE;
        node->throwMatrix = NULL;
    }
}

static void geo_process_object_parent(struct GraphNodeObjectParent *node) {
#if OMM_CODE_DEBUG
    sDisableRendering = false;
#endif
    if (node->sharedChild) {
        node->sharedChild->parent = (struct GraphNode *) node;
        geo_process_node_and_siblings(node->sharedChild);
        node->sharedChild->parent = NULL;
    }
    geo_process_node_and_siblings(gChildren);
}

static void geo_process_held_object(struct GraphNodeHeldObject *node) {
NOT_PREPROCESS {
    geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMatLast);
    struct Object *heldObj = node->objNode;
    if (heldObj && heldObj->oGraphNode) {
        bool shouldInterpolate = should_interpolate(node->_translation);
        interpolate {

            // Interpolate throw matrix, copy current translation
            interpolate_mtxf(gNextMat(k), node->_throwMatrix.v, *gCurGraphNodeObject->throwMatrix);
            vec3f_copy(gNextMat(k)[3], gCurrMat(k)[3]);

            // Compute and add node translation
            Vec3s t3s; interpolate_vec3s(t3s, node->_translation.v, node->translation);
            Vec3f t3f; vec3s_to_vec3f(t3f, t3s);
            vec3f_mul(t3f, 0.25f);
            Mat4 mat; mtxf_translate(mat, t3f);
            mtxf_mul(gNextMat(k), mat, gNextMat(k));

            // Scale
            Vec3f scale; interpolate_vec3f(scale, heldObj->header.gfx._scale.v, heldObj->oGfxScale);
            mtxf_scale_vec3f(gNextMat(k), gNextMat(k), scale);
        }

        // Re-execute node function with GEO_CONTEXT_HELD_OBJ
        geo_exec_node_func(gFnNode, GEO_CONTEXT_HELD_OBJ, gNextMatLast);
        update_timestamp_vec3s(node->_translation, node->translation);
        update_timestamp_mtxf(node->_throwMatrix, *gCurGraphNodeObject->throwMatrix);

        geo_update_mat_stack(

            // Make a back-up of the current anim state before processing the held object
            GeoAnimState curAnimState;
            OMM_MEMCPY(&curAnimState, sCurAnimState, sizeof(sCurAnimState));

            // Init animation state if the held object has animations
            if (heldObj->oCurrAnim) {
#if OMM_CODE_DYNOS
                dynos_gfx_swap_animations(heldObj);
#endif
                geo_set_animation_globals(&heldObj->header.gfx, (heldObj->oNodeFlags & GRAPH_RENDER_HAS_ANIMATION) != 0);
#if OMM_CODE_DYNOS
                dynos_gfx_swap_animations(heldObj);
#endif
            } else {
                sCurAnimState->type = ANIM_TYPE_NONE;
            }

            // Process held object
            gCurGraphNodeHeldObject = node;
            geo_process_node_and_siblings(heldObj->oGraphNode);
            gCurGraphNodeHeldObject = NULL;

            // Recover the current anim state
            OMM_MEMCPY(sCurAnimState, &curAnimState, sizeof(curAnimState));
        )
    }
}
    // Process children
    geo_process_node_and_siblings(gChildren);
}

static void geo_process_culling_radius(struct GraphNodeCullingRadius *node) {
PREPROCESS {
    static Vec3f sUpVector;
    static Vec3f sFwdVector;
    static Vec3f sLatVector;
    static Vec3f sTorsoPos;
    static Vec3f sLeftLegPos;
    static Vec3f sLeftFootPos;
    static Vec3f sRightLegPos;
    static Vec3f sRightFootPos;
    static Vec3s sPeachDressRot;

    // Use this node as a special GEO_ASM that works only during preprocessing
    // Each index is mapped to a specific function
    switch (node->cullingRadius) {

        // Compute the 'up' vector
        case 1: {
            vec3f_copy(sUpVector, gCurrMatLast[3]);
            vec3f_sub(sUpVector, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute the 'forward' vector
        case 2: {
            vec3f_copy(sFwdVector, gCurrMatLast[3]);
            vec3f_sub(sFwdVector, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute the 'lateral' vector
        case 3: {
            vec3f_copy(sLatVector, gCurrMatLast[3]);
            vec3f_sub(sLatVector, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute torso position
        case 4: {
            vec3f_copy(sTorsoPos, gCurrMatLast[3]);
            vec3f_sub(sTorsoPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute left leg position
        case 5: {
            vec3f_copy(sLeftLegPos, gCurrMatLast[3]);
            vec3f_sub(sLeftLegPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute left foot position
        case 6: {
            vec3f_copy(sLeftFootPos, gCurrMatLast[3]);
            vec3f_sub(sLeftFootPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute right leg position
        case 7: {
            vec3f_copy(sRightLegPos, gCurrMatLast[3]);
            vec3f_sub(sRightLegPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute right foot position
        case 8: {
            vec3f_copy(sRightFootPos, gCurrMatLast[3]);
            vec3f_sub(sRightFootPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute Peach's dress rotation, set hips rotation, set legs visibility
        case 9: {

            // Origin (torso)
            Vec3f o = {
                sTorsoPos[0],
                sTorsoPos[1],
                sTorsoPos[2]
            };

            // Up
            Vec3f u = {
                sUpVector[0] - o[0],
                sUpVector[1] - o[1],
                sUpVector[2] - o[2],
            };
            vec3f_normalize(u);

            // Down
            Vec3f d = {
                o[0] - sUpVector[0],
                o[1] - sUpVector[1],
                o[2] - sUpVector[2],
            };
            vec3f_normalize(d);

            // Forward
            Vec3f v = {
                sFwdVector[0] - o[0],
                sFwdVector[1] - o[1],
                sFwdVector[2] - o[2],
            };
            vec3f_normalize(v);

            // Lateral
            Vec3f n = {
                sLatVector[0] - o[0],
                sLatVector[1] - o[1],
                sLatVector[2] - o[2],
            };
            vec3f_normalize(n);

            // Left leg
            Vec3f ll = {
                sLeftLegPos[0],
                sLeftLegPos[1],
                sLeftLegPos[2],
            };
            vec3f_project_point(ll, ll, o, n);

            // Left foot
            Vec3f lf = {
                sLeftFootPos[0],
                sLeftFootPos[1],
                sLeftFootPos[2],
            };
            vec3f_project_point(lf, lf, o, n);

            // Right leg
            Vec3f rl = {
                sRightLegPos[0],
                sRightLegPos[1],
                sRightLegPos[2],
            };
            vec3f_project_point(rl, rl, o, n);

            // Right foot
            Vec3f rf = {
                sRightFootPos[0],
                sRightFootPos[1],
                sRightFootPos[2],
            };
            vec3f_project_point(rf, rf, o, n);

            // Left leg vector
            Vec3f l = {
                lf[0] - ll[0],
                lf[1] - ll[1],
                lf[2] - ll[2]
            };
            vec3f_normalize(l);

            // Right leg vector
            Vec3f r = {
                rf[0] - rl[0],
                rf[1] - rl[1],
                rf[2] - rl[2]
            };
            vec3f_normalize(r);

            // Angle left
            f32 ldot = l[0] * d[0] + l[1] * d[1] + l[2] * d[2];
            s16 la = (s16) ((acosf(ldot) / M_PI) * 0x8000);
            Vec3f ldcross; vec3f_cross(ldcross, l, d);
            if (vec3f_dot(n, ldcross) < 0) {
                la = -la;
            }

            // Angle right
            f32 rdot = r[0] * d[0] + r[1] * d[1] + r[2] * d[2];
            s16 ra = (s16) ((acosf(rdot) / M_PI) * 0x8000);
            Vec3f rdcross; vec3f_cross(rdcross, r, d);
            if (vec3f_dot(n, rdcross) < 0) {
                ra = -ra;
            }

            // Dress pitch
            s16 ua = min_s(la, ra);
            s16 va = max_s(la, ra);
            f32 da = (f32) ((u16) (va - ua));
            f32 ta = (0.50f * (1.f - clamp_0_1_f((da - 0x2000) / 0x4000)));
            f32 pa = ((ua + da * ta) / 3.f);
            sPeachDressRot[0] = 0;
            sPeachDressRot[1] = 0;
            sPeachDressRot[2] = ((s16) pa - 0x0800);

            // Legs visibility
            u16 dlp = (u16) abs_s(la - (sPeachDressRot[2] * 2));
            u16 drp = (u16) abs_s(ra - (sPeachDressRot[2] * 2));
            s32 animId = gCurrGraphNodeObject->oAnimID;
            if (animId == MARIO_ANIM_WALKING || animId == MARIO_ANIM_RUNNING) {
                gCurrGraphNodeObject->oAnimState = ((dlp > 0x2800) << 1) | ((drp > 0x2800) << 0);
            } else {
                gCurrGraphNodeObject->oAnimState = ((dlp > 0x3000) << 1) | ((drp > 0x3000) << 0);
            }

            // Set hips pitch
            struct GraphNodeRotation *rotNode = (struct GraphNodeRotation *) node->node.next;
            rotNode->rotation[0] = -(sPeachDressRot[0] * 4) / 3;
            rotNode->rotation[1] = -(sPeachDressRot[1] * 4) / 3;
            rotNode->rotation[2] = -(sPeachDressRot[2] * 4) / 3;
        } break;

        // Set Peach's dress rotation
        case 10: {
            struct GraphNodeRotation *rotNode = (struct GraphNodeRotation *) node->node.next;
            rotNode->rotation[0] = -(sPeachDressRot[0] * 2) / 3;
            rotNode->rotation[1] = -(sPeachDressRot[1] * 2) / 3;
            rotNode->rotation[2] = -(sPeachDressRot[2] * 2) / 3;
        } break;
        
        // Compute head rotation
        case 11: {
            if (sCurAnimState->attr && sCurAnimState->type == ANIM_TYPE_ROTATION) {
                geo_get_animation_data_as_vec3s(sMarioHeadRot, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->frame, 1, 1, 1, 0);
            } else {
                vec3s_zero(sMarioHeadRot);
            }
        } break;

        // Set Peach's hair rotation
        case 12: {
            struct GraphNodeRotation *rotNode = (struct GraphNodeRotation *) node->node.next;
            rotNode->rotation[0] = 0;
            rotNode->rotation[1] = -sMarioHeadRot[1] / 2;
            rotNode->rotation[2] = -sMarioHeadRot[2] / (1 + (sMarioHeadRot[2] > 0));
        } break;

        // Switch between Peach's crown and Tiara
        case 13: {
            struct GraphNode *crownNode = node->node.next;
            struct GraphNode *tiaraNode = node->node.next->next;
            if (OMM_EXTRAS_CAPPY_AND_TIARA) {
                crownNode->flags &= ~GRAPH_RENDER_ACTIVE;
                tiaraNode->flags |= GRAPH_RENDER_ACTIVE;
            } else {
                crownNode->flags |= GRAPH_RENDER_ACTIVE;
                tiaraNode->flags &= ~GRAPH_RENDER_ACTIVE;
            }
        } break;

        // Switch between Perry's closed and open states
        case 14: {
            struct GraphNode *closeNode = node->node.next;
            struct GraphNode *openNode = node->node.next->next;
            if (gCurrGraphNodeObject->oAction) {
                closeNode->flags &= ~GRAPH_RENDER_ACTIVE;
                openNode->flags |= GRAPH_RENDER_ACTIVE;
            } else {
                closeNode->flags |= GRAPH_RENDER_ACTIVE;
                openNode->flags &= ~GRAPH_RENDER_ACTIVE;
            }
        } break;

        // Rotate Peach's hand to look like she's holding a sword
        case 15: {
            struct GraphNodeRotation *rot = (struct GraphNodeRotation *) node->node.next;
            struct Object *perry = omm_peach_get_perry_object();
            if (perry && (perry->oPerryFlags & OBJ_INT_PERRY_SWORD)) {
                rot->rotation[0] = perry->oPerryRightHandRot(0);
                rot->rotation[1] = perry->oPerryRightHandRot(1);
                rot->rotation[2] = perry->oPerryRightHandRot(2);
            } else {
                rot->rotation[0] = 0;
                rot->rotation[1] = 0;
                rot->rotation[2] = 0;
            }
        } break;
    }
}
    // Process children
    geo_process_node_and_siblings(gChildren);
}


static void geo_try_process_children(struct GraphNode *node) {
    geo_process_node_and_siblings(gChildren);
}

//
// Process
//

// Why do you want to use slow switch-cases when you can use a fast look-up table of function pointers?
typedef void (*GeoProcessFunction)(struct GraphNode *);
static const void *sGeoProcessFunctions[] = {
    geo_try_process_children,
    geo_try_process_children,
    geo_process_ortho_projection,
    geo_process_perspective,
    geo_process_master_list,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_process_level_of_detail,
    geo_process_switch,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_process_camera,
    geo_process_translation_rotation,
    geo_process_translation,
    geo_process_rotation,
    geo_process_object,
    geo_process_animated_part,
    geo_process_billboard,
    geo_process_display_list,
    geo_process_scale,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_process_shadow,
    geo_process_object_parent,
    geo_process_generated_list,
    geo_try_process_children,
    geo_process_background,
    geo_try_process_children,
    geo_process_held_object,
    geo_process_culling_radius,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
    geo_try_process_children,
};

void geo_process_node_and_siblings(struct GraphNode *firstNode) {
    for (struct GraphNode *node = firstNode; node && (node->type & 0xFF) < 0x30; node = node->next) {
        if (node->flags & GRAPH_RENDER_ACTIVE) {
            ((GeoProcessFunction) (sGeoProcessFunctions[node->type & 0x3F]))(node);
        } else if (node->type == GRAPH_NODE_TYPE_OBJECT) {
            ((struct GraphNodeObject *) node)->throwMatrix = NULL;
        }
        if (node->next == firstNode || (node->parent && node->parent->type == GRAPH_NODE_TYPE_SWITCH_CASE)) {
            break;
        }
    }
}

void geo_process_root(struct GraphNodeRoot *node, Vp *viewport1, Vp *viewport2, s32 clearColor) {
    if (node->node.flags & GRAPH_RENDER_ACTIVE) {

        // Init viewport
        static Vp viewport[1];
        vec3s_set(viewport->vp.vtrans, node->x     << 2, node->y      << 2, 511);
        vec3s_set(viewport->vp.vscale, node->width << 2, node->height << 2, 511);
        if (viewport1) {
            clear_framebuffer(clearColor);
            make_viewport_clip_rect(viewport1);
            OMM_MEMCPY(viewport, viewport1, sizeof(Vp));
        } else if (viewport2) {
            clear_framebuffer(clearColor);
            make_viewport_clip_rect(viewport2);
        }
        gSPViewport(gDisplayListHead++, viewport);

        // Init matrix stack
        static Mtx mtx0[MAX_INTERPOLATED_FRAMES];
        sMatStackIndex = 0;
        interpolate {
            mtxf_identity(gCurrMat(k));
            mtxf_to_mtx(&mtx0[k], gCurrMat(k));
            sMtxStack[sMatStackIndex][k] = &mtx0[k];
        }
        gSPMatrix(gDisplayListHead++, &mtx0[0], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        sCurAnimState->type = ANIM_TYPE_NONE;

        // Process the node tree
#if OMM_CODE_DEBUG
        sDisableRendering = false;
#endif
        sIsPreprocess = false;
        sMarioRootFlag = 0;
        gCurGraphNodeRoot = node;
        geo_process_node_and_siblings(gChildren);
        gCurGraphNodeRoot = NULL;
    }
}

//
// Preprocess
//

static void __geo_preprocess_object_graph_node(struct Object *obj) {
    OMM_MEMSET(sCurAnimState, 0, sizeof(sCurAnimState));
    sMatStackIndex = 0;
    sMarioHandFlag = 0;
    sMarioRootFlag = 0;

    // Init globals
    Mat4 curGraphNodeCameraMatrix = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
    struct GraphNodeCamera curGraphNodeCamera = { .config.camera = gCamera, .matrixPtr = &curGraphNodeCameraMatrix };
    gCurGraphNodeCamera = &curGraphNodeCamera;
    gCurGraphNodeObject = (struct GraphNodeObject *) obj;
    mtxf_identity(gCurrMatLast);

    // Init the base matrix
    if (obj->oThrowMatrix) {
        mtxf_copy(gCurrMatLast, *obj->oThrowMatrix);
    } else {
        mtxf_rotate_zxy_and_translate(gCurrMatLast, obj->oGfxPos, obj->oGfxAngle);
    }
    mtxf_scale_vec3f(gCurrMatLast, gCurrMatLast, obj->oGfxScale);

    // Init animation state if the object has animations
    // Make a copy of the animation state, it will be restored at the end of preprocessing
    struct AnimInfoStruct animInfoBackUp = obj->oAnimInfo;
    if (obj->oCurrAnim) {
#if OMM_CODE_DYNOS
        dynos_gfx_swap_animations(obj);
#endif
        geo_set_animation_globals(&obj->header.gfx, (obj->oNodeFlags & GRAPH_RENDER_HAS_ANIMATION) != 0);
#if OMM_CODE_DYNOS
        dynos_gfx_swap_animations(obj);
#endif
    }
    
    // Preprocess
    sIsPreprocess = true;
    gCurGraphNodeObject = (struct GraphNodeObject *) obj;
    obj->oGraphNode->parent = &obj->header.gfx.node;
    geo_process_node_and_siblings(obj->oGraphNode);
    obj->oGraphNode->parent = NULL;
    gCurGraphNodeObject = NULL;
    sIsPreprocess = false;

    // Restore animation info
    obj->oAnimInfo = animInfoBackUp;
}

void geo_preprocess_object_graph_node(struct Object *o) {
    if (o && o->oGraphNode) {

        // Disable interpolation during preprocessing
        s32 numInterpolatedFrames = gNumInterpolatedFrames;
        gNumInterpolatedFrames = 1;

        // Retrieve Mario's height
        if (o == gMarioObject) {
            static OmmMap sMarioHeightMap = omm_map_zero;
            s32 i = omm_map_find_key(sMarioHeightMap, ptr, o->oGraphNode);
            if (i == -1) {

                // Preprocess with A-pose animation
                // Back-up the current animation and restore it after processing
                // Setting gMarioCurrAnimAddr to NULL forces the game to reload the DMA table
                struct AnimInfoStruct animInfo;
                OMM_MEMCPY(&animInfo, &o->oAnimInfo, sizeof(struct AnimInfoStruct));
                gMarioCurrAnimAddr = NULL;
                obj_anim_play(gMarioObject, MARIO_ANIM_A_POSE, 1.f);
                __geo_preprocess_object_graph_node(o);
                gMarioCurrAnimAddr = NULL;
                OMM_MEMCPY(&o->oAnimInfo, &animInfo, sizeof(struct AnimInfoStruct));

                // Compute and insert height for the object's graph node
                f32 marioHeight = 35.f + (sMarioHeadPos[1] - gMarioState->pos[1]);
                omm_map_add(sMarioHeightMap, ptr, o->oGraphNode, f32, marioHeight);
                i = omm_map_count(sMarioHeightMap) - 1;
            }
            sMarioHeight = omm_map_get_val(sMarioHeightMap, f32, i);
        }

        // Preprocess
        __geo_preprocess_object_graph_node(o);
        gNumInterpolatedFrames = numInterpolatedFrames;
    }
}

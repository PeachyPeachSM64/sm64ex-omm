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

// Previous frame matrices
static Mat4 sMatStack0[32];
static Mtx *sMtxStack0[32];

// Current frame matrices
static Mat4 sMatStack1[32];
static Mtx *sMtxStack1[32];

static s32 sMatStackIndex;
static s32 sCurrObjectSlot;
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

#define check_timestamp(_ts)                        ((_ts).ts == (gGlobalTimer - 1))
#define reset_timestamp(_ts)                        {(_ts).ts = 0;}
#define update_timestamp_s16(_ts, x)                {(_ts).ts = gGlobalTimer; (_ts).v = (s16) (x);}
#define update_timestamp_s32(_ts, x)                {(_ts).ts = gGlobalTimer; (_ts).v = (s32) (x);}
#define update_timestamp_u16(_ts, x)                {(_ts).ts = gGlobalTimer; (_ts).v = (u16) (x);}
#define update_timestamp_u32(_ts, x)                {(_ts).ts = gGlobalTimer; (_ts).v = (u32) (x);}
#define update_timestamp_f32(_ts, x)                {(_ts).ts = gGlobalTimer; (_ts).v = (f32) (x);}
#define update_timestamp_vec3f(_ts, x)              {(_ts).ts = gGlobalTimer; vec3f_copy((_ts).v, x);}
#define update_timestamp_vec3s(_ts, x)              {(_ts).ts = gGlobalTimer; vec3s_copy((_ts).v, x);}
#define update_timestamp_mtxf(_ts, x)               {(_ts).ts = gGlobalTimer; mtxf_copy((_ts).v, x);}
#define update_timestamp_ptr(_ts, x)                {(_ts).ts = gGlobalTimer; (_ts).v = (void *) (x);}
#define should_interpolate(_ts)                     (!sIsPreprocess && gFrameInterpolation && check_timestamp(_ts))

#define _o(_ts)                                     ((_ts)[sCurrObjectSlot])
#define check_timestamp_o(_ts)                      (_o(_ts).ts == (gGlobalTimer - 1))
#define reset_timestamp_o(_ts)                      {_o(_ts).ts = 0;}
#define update_timestamp_s16_o(_ts, x)              {_o(_ts).ts = gGlobalTimer; _o(_ts).v = (s16) (x);}
#define update_timestamp_s32_o(_ts, x)              {_o(_ts).ts = gGlobalTimer; _o(_ts).v = (s32) (x);}
#define update_timestamp_u16_o(_ts, x)              {_o(_ts).ts = gGlobalTimer; _o(_ts).v = (u16) (x);}
#define update_timestamp_u32_o(_ts, x)              {_o(_ts).ts = gGlobalTimer; _o(_ts).v = (u32) (x);}
#define update_timestamp_f32_o(_ts, x)              {_o(_ts).ts = gGlobalTimer; _o(_ts).v = (f32) (x);}
#define update_timestamp_vec3f_o(_ts, x)            {_o(_ts).ts = gGlobalTimer; vec3f_copy(_o(_ts).v, x);}
#define update_timestamp_vec3s_o(_ts, x)            {_o(_ts).ts = gGlobalTimer; vec3s_copy(_o(_ts).v, x);}
#define update_timestamp_mtxf_o(_ts, x)             {_o(_ts).ts = gGlobalTimer; mtxf_copy(_o(_ts).v, x);}
#define update_timestamp_ptr_o(_ts, x)              {_o(_ts).ts = gGlobalTimer; _o(_ts).v = (void *) (x);}
#define should_interpolate_o(_ts)                   (!sIsPreprocess && gFrameInterpolation && check_timestamp_o(_ts))

// Camera
static Mat4  sCameraInvMat0;
static Mat4  sCameraInvMat1;
static Vec3f sCameraPos0;
static Vec3f sCameraPos1;
static Vec3f sCameraFocus0;
static Vec3f sCameraFocus1;
static s16   sCameraRoll;

// Perspective
static Gfx  *sPerspectivePos;
static f32   sPerspectiveAspect;
static f32   sPerspectiveNear;
static f32   sPerspectiveFar;
static f32   sPerspectiveFov0;
static f32   sPerspectiveFov1;

// Background
static Gfx  *sBackgroundGfx;
static void *sBackgroundNode;
static void *sBackgroundRoot;
static u8   *sBackgroundPoolEnd;
static Vec3f sBackgroundCamPos0;
static Vec3f sBackgroundCamPos1;
static Vec3f sBackgroundCamFocus0;
static Vec3f sBackgroundCamFocus1;

// Shadows
typedef struct {
    Vtx *vtx;
    Vtx vtx0[4];
    Vtx vtx1[4];
} InterpShadow;
static OmmArray sShadowTbl;
static s32 sShadowTblSize;

// Display list nodes
typedef struct {
    Vec3f translation;
    Vec3s rotation;
    Vec3f shear;
    Vec3f scale;
} InterpMtxComponents;
typedef struct {
    Gfx *pos;
    Gfx *gfx;
    Mtx  mtx;
    union {
        Mtx *raw;
        InterpMtxComponents comp;
    } mtx0, mtx1;
    bool interpMtxComponents;
    bool applyCameraTransform;
} InterpMtx;
static OmmArray sMtxTbl;
static s32 sMtxTblSize;
static bool sInterpMtxComponents = false;

void gfx_interpolate_frame_mtx(f32 t) {

    // Interpolate perspective
    if (sPerspectivePos) {
        static Mtx sPerspectiveMtx[1];
        f32 fov = lerp_f(t, sPerspectiveFov0, sPerspectiveFov1);
        u16 perspNorm = mtxf_perspective(sPerspectiveMtx, fov, sPerspectiveAspect, sPerspectiveNear, sPerspectiveFar, 1.f);
        gSPPerspNormalize(sPerspectivePos + 0, perspNorm);
        gSPMatrix(sPerspectivePos + 1, sPerspectiveMtx, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    }

    // Interpolate dl nodes matrices
    omm_array_for_each(sMtxTbl, p) {
        if (i_p >= sMtxTblSize) break;
        InterpMtx *imtx = (InterpMtx *) p->as_ptr;
        Gfx *pos = imtx->pos;
        Gfx *gfx = imtx->gfx;
        Mtx *mtx = &imtx->mtx;
        if (OMM_LIKELY(pos && gfx)) {

            // Matrix
            if (imtx->interpMtxComponents) {
                InterpMtxComponents comp;
                vec3f_interpolate(comp.translation, imtx->mtx0.comp.translation, imtx->mtx1.comp.translation, t);
                vec3s_interpolate_angles(comp.rotation, imtx->mtx0.comp.rotation, imtx->mtx1.comp.rotation, t);
                vec3f_interpolate(comp.shear, imtx->mtx0.comp.shear, imtx->mtx1.comp.shear, t);
                vec3f_interpolate(comp.scale, imtx->mtx0.comp.scale, imtx->mtx1.comp.scale, t);
                mtxf_transform(mtx->m, comp.translation, comp.rotation, comp.shear, comp.scale);
                if (imtx->applyCameraTransform) {
                    Vec3f pos; vec3f_interpolate(pos, sCameraPos0, sCameraPos1, t);
                    Vec3f focus; vec3f_interpolate(focus, sCameraFocus0, sCameraFocus1, t);
                    Mat4 mat; mtxf_lookat(mat, pos, focus, sCameraRoll);
                    mtxf_mul(mtx->m, mtx->m, mat);
                }
            } else {
                mtxf_interpolate_fast(mtx->m, imtx->mtx0.raw->m, imtx->mtx1.raw->m, t);
            }
            gSPMatrix(pos + 0, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

            // Background
            if (OMM_UNLIKELY(gfx == sBackgroundGfx)) {
                gGfxPoolEnd = sBackgroundPoolEnd;
                gCurGraphNodeRoot = sBackgroundRoot;
                vec3f_interpolate(gLakituState.pos, sBackgroundCamPos0, sBackgroundCamPos1, t);
                vec3f_interpolate(gLakituState.focus, sBackgroundCamFocus0, sBackgroundCamFocus1, t);
                gfx = ((struct FnGraphNode *) sBackgroundNode)->func(GEO_CONTEXT_RENDER, (struct GraphNode *) sBackgroundNode, NULL);
            }

            // Display list
            gSPDisplayList(pos + 1, gfx);
        }
    }

    // Interpolate shadows
    omm_array_for_each(sShadowTbl, q) {
        if (i_q >= sShadowTblSize) break;
        InterpShadow *ishadow = (InterpShadow *) q->as_ptr;
        if (ishadow->vtx) {
            for (s32 i = 0; i != 4; ++i) {
                vtxv_interpolate((Vtx_t *) (ishadow->vtx + i), (Vtx_t *) (ishadow->vtx0 + i), (Vtx_t *) (ishadow->vtx1 + i), t);
            }
        }
    }
}

void gfx_clear_frame_mtx() {
    sPerspectivePos = NULL;
    sBackgroundGfx = NULL;
    sShadowTblSize = 0;
    sMtxTblSize = 0;
}

//
// Shortcuts
//

#define gNode           ((struct GraphNode *) node)
#define gNodeLayer      (gNode->flags >> 8)
#define gDisplayList    (((struct GraphNodeDisplayList *) node)->displayList)
#define gChildren       (gNode->children)
#define gFnNode         ((struct FnGraphNode *) node)
#define gCurrMat0       sMatStack0[sMatStackIndex]
#define gNextMat0       sMatStack0[sMatStackIndex + 1]
#define gCurrMat1       sMatStack1[sMatStackIndex]
#define gNextMat1       sMatStack1[sMatStackIndex + 1]

//
// Mario
//

static Vec3f sMarioForeArmPos[2];
f32 *geo_get_marios_forearm_pos(bool isLeft) {
    return sMarioForeArmPos[isLeft];
}

enum MarioHandFlag { MHF_NOT_HAND, MHF_RIGHT_HAND, MHF_LEFT_HAND };
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

enum MarioRootFlag { MRF_NOT_ROOT, MRF_ROOT, MRF_ROOT_ANIM_Y_FIXED, MRF_ROOT_POS_SAVED };
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
    if (o == gMarioObject && sMarioRootFlag < MRF_ROOT_ANIM_Y_FIXED) {
        sMarioRootFlag = MRF_ROOT;

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
        
        // Shell ride animation offset
        if (OMM_MOVESET_ODYSSEY && (gMarioState->action & ACT_FLAG_RIDING_SHELL)) {
            *ty += 168.f;
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

        // Don't interpolate object node angles on animation change
        reset_timestamp(node->_angle);

        // To properly interpolate object node pos on animation change,
        // use the object's previous pos and add the current pos offset
        vec3f_copy(node->_pos.v, node->_objPos.v);
        vec3f_add(node->_pos.v, node->pos);
        vec3f_sub(node->_pos.v, &((struct Object *) node)->oPosX);
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

static void geo_append_display_list(Gfx *displayList, s16 layer) {
#if OMM_CODE_DEBUG
    if (sDisableRendering) return;
#endif
NOT_PREPROCESS {
    if (displayList && OMM_LIKELY(gCurGraphNodeMasterList)) {
        if (OMM_UNLIKELY(!gCurGraphNodeMasterList->listHeads[layer])) {
            gCurGraphNodeMasterList->listHeads[layer] = sDisplayListNodePools[layer];
            gCurGraphNodeMasterList->listTails[layer] = sDisplayListNodePools[layer];
        }
        struct DisplayListNode *dlNode = gCurGraphNodeMasterList->listTails[layer];
        dlNode->transform0 = sMtxStack0[sMatStackIndex];
        dlNode->transform1 = sMtxStack1[sMatStackIndex];
        dlNode->displayList = displayList;
        dlNode->interpMtxComponents = sInterpMtxComponents;
        dlNode->applyCameraTransform = gCurGraphNodeCamera != NULL;
        gCurGraphNodeMasterList->listTails[layer]++;
    }
}
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
        omm_zero(node->listHeads, sizeof(node->listHeads));
        geo_process_node_and_siblings(gChildren);
        
        // Enable Z buffer
        bool zBuffer = (node->node.flags & GRAPH_RENDER_Z_BUFFER) != 0;
        if (zBuffer) {
            gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
        }

        // Process display list nodes
        for (s32 i = 0; i != DISPLAY_LISTS_NUM_LAYERS; ++i) {
            if (node->listHeads[i]) {
                gDPSetRenderMode(gDisplayListHead++, sRenderModeTable1Cycle[zBuffer][i], sRenderModeTable2Cycle[zBuffer][i]);
                struct DisplayListNode *head = node->listHeads[i];
                struct DisplayListNode *tail = node->listTails[i];
                for (struct DisplayListNode *dlNode = head; dlNode != tail; dlNode++) {
                    omm_array_grow(sMtxTbl, ptr, omm_new(InterpMtx, 1), sMtxTblSize + 1);
                    InterpMtx *imtx = (InterpMtx *) omm_array_get(sMtxTbl, ptr, sMtxTblSize);
                    imtx->pos = gDisplayListHead;
                    imtx->gfx = dlNode->displayList;
                    if (dlNode->interpMtxComponents) {
                        if (dlNode->applyCameraTransform) {
                            Mat4 m0; mtxf_mul(m0, dlNode->transform0->m, sCameraInvMat0);
                            Mat4 m1; mtxf_mul(m1, dlNode->transform1->m, sCameraInvMat1);
                            mtxf_get_components(m0, imtx->mtx0.comp.translation, imtx->mtx0.comp.rotation, imtx->mtx0.comp.shear, imtx->mtx0.comp.scale);
                            mtxf_get_components(m1, imtx->mtx1.comp.translation, imtx->mtx1.comp.rotation, imtx->mtx1.comp.shear, imtx->mtx1.comp.scale);
                            imtx->applyCameraTransform = true;
                        } else {
                            mtxf_get_components(dlNode->transform0->m, imtx->mtx0.comp.translation, imtx->mtx0.comp.rotation, imtx->mtx0.comp.shear, imtx->mtx0.comp.scale);
                            mtxf_get_components(dlNode->transform1->m, imtx->mtx1.comp.translation, imtx->mtx1.comp.rotation, imtx->mtx1.comp.shear, imtx->mtx1.comp.scale);
                            imtx->applyCameraTransform = false;
                        }
                        imtx->interpMtxComponents = true;
                    } else {
                        imtx->mtx0.raw = dlNode->transform0;
                        imtx->mtx1.raw = dlNode->transform1;
                        imtx->interpMtxComponents = false;
                    }
                    sMtxTblSize++;
                    gSPMatrix(gDisplayListHead++, dlNode->transform0, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
                    gSPDisplayList(gDisplayListHead++, dlNode->displayList);
                }
            }
        }

        // Disable Z buffer
        if (zBuffer) {
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
    sMtxStack0[sMatStackIndex] = alloc_display_list(sizeof(Mtx));
    sMtxStack1[sMatStackIndex] = alloc_display_list(sizeof(Mtx));
    omm_copy(sMtxStack0[sMatStackIndex]->m, sMatStack0[sMatStackIndex], sizeof(Mat4));
    omm_copy(sMtxStack1[sMatStackIndex]->m, sMatStack1[sMatStackIndex], sizeof(Mat4));
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
    geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMat1);
    if (gChildren) {
        f32 aspect = (f32) gCurGraphNodeRoot->width / (f32) gCurGraphNodeRoot->height;
        
        // Interpolation
        if (should_interpolate(node->_fov)) {
            sPerspectivePos    = gDisplayListHead;
            sPerspectiveAspect = aspect;
            sPerspectiveNear   = node->near;
            sPerspectiveFar    = node->far;
            sPerspectiveFov0   = node->_fov.v;
            sPerspectiveFov1   = node->fov;
        } else {
            sPerspectivePos    = NULL;
            sPerspectiveFov0   = node->fov;
        }
        
        // Perspective matrix
        Mtx *mtx = alloc_display_list(sizeof(Mtx));
        u16 perspNorm = mtxf_perspective(mtx, sPerspectiveFov0, aspect, node->near, node->far, 1.f);
        gSPPerspNormalize(gDisplayListHead++, perspNorm);
        gSPMatrix(gDisplayListHead++, mtx, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
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
    geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMat1);
PREPROCESS {

    // Mario forearm pos
    if (gFnNode->func == (GraphNodeFunc) geo_switch_mario_hand) {
        static bool sIsLeftHand = true;
        sMarioHandFlag = MHF_RIGHT_HAND + sIsLeftHand;
        vec3f_copy(sMarioForeArmPos[sIsLeftHand], gCurrMat1[3]);
        sIsLeftHand = !sIsLeftHand;
    }

    // Mario head pos and vectors
    if (gFnNode->func == (GraphNodeFunc) geo_switch_mario_eyes) {
        vec3f_copy(sMarioHeadPos, gCurrMat1[3]);
        Mat4 rot;
        mtxf_copy(rot, gCurrMat1);
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
    omm_cappy_gfx_draw_eyes(&node->fnNode.node, (void (*)(void *, s16)) geo_append_display_list);
}
}

static void geo_process_camera(struct GraphNodeCamera *node) {
    geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMat1);
    
    // Roll matrix
    Mtx *mtxRoll = alloc_display_list(sizeof(Mtx));
    mtxf_rotate_xy(mtxRoll, node->rollScreen);
    gSPMatrix(gDisplayListHead++, mtxRoll, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
NOT_PREPROCESS {
    sCameraRoll = node->roll;
}

    // Current frame
    Mat4 mat; mtxf_lookat(mat, node->pos, node->focus, node->roll);
    mtxf_mul(gNextMat1, mat, gCurrMat1);
NOT_PREPROCESS {
    vec3f_copy(sCameraPos1, node->pos);
    vec3f_copy(sCameraFocus1, node->focus);
}

    // Previous frame
    if (should_interpolate(node->_pos)) {
        mtxf_lookat(mat, node->_pos.v, node->_focus.v, node->roll);
NOT_PREPROCESS {
        vec3f_copy(sCameraPos0, node->_pos.v);
        vec3f_copy(sCameraFocus0, node->_focus.v);
}
    } else {
NOT_PREPROCESS {
        vec3f_copy(sCameraPos0, node->pos);
        vec3f_copy(sCameraFocus0, node->focus);
}
    }
    mtxf_mul(gNextMat0, mat, gCurrMat0);

    // Update timestamps
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
            node->lookAt0 = &gCurrMat0;
            node->lookAt1 = &gCurrMat1;
            node->matrixPtr = node->lookAt1;
NOT_PREPROCESS {
            mtxf_invert(sCameraInvMat0, *node->lookAt0);
            mtxf_invert(sCameraInvMat1, *node->lookAt1);
}
            geo_process_node_and_siblings(gChildren);
            gCurGraphNodeCamera = NULL;
        }
    )
}

static void geo_process_translation_rotation(struct GraphNodeTranslationRotation *node) {

    // Current frame
    Vec3f t3f; vec3s_to_vec3f(t3f, node->translation);
    Mat4 mat; mtxf_rotate_zxy_and_translate(mat, t3f, node->rotation);
    mtxf_mul(gNextMat1, mat, gCurrMat1);

    // Previous frame
    if (should_interpolate_o(node->_translation)) {
        vec3s_to_vec3f(t3f, _o(node->_translation).v);
        mtxf_rotate_zxy_and_translate(mat, t3f, _o(node->_rotation).v);
    }
    mtxf_mul(gNextMat0, mat, gCurrMat0);

    // Update timestamps
NOT_PREPROCESS {
    update_timestamp_vec3s_o(node->_translation, node->translation);
    update_timestamp_vec3s_o(node->_rotation, node->rotation);
}

    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_translation(struct GraphNodeTranslation *node) {

    // Current frame
    Vec3f t3f; vec3s_to_vec3f(t3f, node->translation);
    Mat4 mat; mtxf_translate(mat, t3f);
    mtxf_mul(gNextMat1, mat, gCurrMat1);

    // Previous frame
    if (should_interpolate_o(node->_translation)) {
        vec3s_to_vec3f(t3f, _o(node->_translation).v);
        mtxf_translate(mat, t3f);
    }
    mtxf_mul(gNextMat0, mat, gCurrMat0);

    // Update timestamps
NOT_PREPROCESS {
    update_timestamp_vec3s_o(node->_translation, node->translation);
}

    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_rotation(struct GraphNodeRotation *node) {

    // Current frame
    Mat4 mat; mtxf_rotate_zxy_and_translate(mat, gVec3fZero, node->rotation);
    mtxf_mul(gNextMat1, mat, gCurrMat1);

    // Previous frame
    if (should_interpolate_o(node->_rotation)) {
        mtxf_rotate_zxy_and_translate(mat, gVec3fZero, _o(node->_rotation).v);
    }
    mtxf_mul(gNextMat0, mat, gCurrMat0);

    // Update timestamps
NOT_PREPROCESS {
    update_timestamp_vec3s_o(node->_rotation, node->rotation);
}

    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_scale(struct GraphNodeScale *node) {

    // Current frame
    mtxf_scale_vec3f(gNextMat1, gCurrMat1, (Vec3f) { node->scale, node->scale, node->scale });

    // Previous frame
    if (should_interpolate_o(node->_scale)) {
        mtxf_scale_vec3f(gNextMat0, gCurrMat0, (Vec3f) { _o(node->_scale).v, _o(node->_scale).v, _o(node->_scale).v });
    } else {
        mtxf_scale_vec3f(gNextMat0, gCurrMat0, (Vec3f) { node->scale, node->scale, node->scale });
    }

    // Update timestamps
NOT_PREPROCESS {
    update_timestamp_f32_o(node->_scale, node->scale);
}

    // Process children
    geo_update_mat_stack(
        geo_append_display_list(gDisplayList, gNodeLayer);
        geo_process_node_and_siblings(gChildren);
    )
}

static void geo_process_billboard(struct GraphNodeBillboard *node) {

    // Current frame
    Vec3f t3f; vec3s_to_vec3f(t3f, node->translation);
    mtxf_billboard(gNextMat1, gCurrMat1, t3f, gCurGraphNodeCamera->roll);
    if (gCurGraphNodeHeldObject) {
        mtxf_scale_vec3f(gNextMat1, gNextMat1, gCurGraphNodeHeldObject->objNode->oGfxScale);
    } else if (gCurGraphNodeObject) {
        mtxf_scale_vec3f(gNextMat1, gNextMat1, gCurGraphNodeObject->scale);
    }

    // Previous frame
    if (should_interpolate_o(node->_translation)) {
        vec3s_to_vec3f(t3f, _o(node->_translation).v);
    }
    mtxf_billboard(gNextMat0, gCurrMat0, t3f, gCurGraphNodeCamera->roll);
    if (gCurGraphNodeHeldObject) {
        mtxf_scale_vec3f(gNextMat0, gNextMat0, gCurGraphNodeHeldObject->objNode->oGfxScale);
    } else if (gCurGraphNodeObject) {
        mtxf_scale_vec3f(gNextMat0, gNextMat0, gCurGraphNodeObject->scale);
    }

    // Update timestamps
NOT_PREPROCESS {
    update_timestamp_vec3s_o(node->_translation, node->translation);
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

    // Avoids a crash in the palette editor
    if (OMM_UNLIKELY(omm_is_main_menu() && gFnNode->func == (GraphNodeFunc) geo_mario_head_rotation)) {
        static struct Camera dummy = { .mode = CAMERA_MODE_FREE_ROAM };
        gCurGraphNodeCamera->config.camera = &dummy;
    }

    Gfx *gfx = geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMat1);
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
    Gfx *gfx = NULL;

    // Create a background from a function...
    if (gFnNode->func) {
        
        // Interpolation
        if (should_interpolate(node->_cameraPos)) {
            sBackgroundPoolEnd = gGfxPoolEnd;
            vec3f_copy(sBackgroundCamPos0, node->_cameraPos.v);
            vec3f_copy(sBackgroundCamFocus0, node->_cameraFocus.v);
        } else {
            vec3f_copy(sBackgroundCamPos0, gLakituState.pos);
            vec3f_copy(sBackgroundCamFocus0, gLakituState.focus);
        }
        vec3f_copy(sBackgroundCamPos1, gLakituState.pos);
        vec3f_copy(sBackgroundCamFocus1, gLakituState.focus);

        // Draw background
        vec3f_copy(gLakituState.pos, sBackgroundCamPos0);
        vec3f_copy(gLakituState.focus, sBackgroundCamFocus0);
        gfx = gFnNode->func(GEO_CONTEXT_RENDER, gNode, NULL);
        vec3f_copy(gLakituState.pos, sBackgroundCamPos1);
        vec3f_copy(gLakituState.focus, sBackgroundCamFocus1);

        // Save pointers
        if (should_interpolate(node->_cameraPos)) {
            sBackgroundGfx = gfx;
            sBackgroundNode = node;
            sBackgroundRoot = gCurGraphNodeRoot;
        }
NOT_PREPROCESS {
        update_timestamp_vec3f(node->_cameraPos, gLakituState.pos);
        update_timestamp_vec3f(node->_cameraFocus, gLakituState.focus);
}
    }

    // ...or from a solid color
    if (gfx) {
        geo_append_display_list(gfx, gNodeLayer);
    } else if (gCurGraphNodeMasterList) {
        gfx = alloc_display_list(sizeof(Gfx) * 6);
        geo_append_display_list(gfx, LAYER_FORCE);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        gDPSetFillColor(gfx++, node->background);
        gDPFillRectangle(gfx++, GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), 0, GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, SCREEN_HEIGHT - 1);
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gSPEndDisplayList(gfx++);
    }

    // Process children
    geo_process_node_and_siblings(gChildren);
}

static void geo_process_animated_part(struct GraphNodeAnimatedPart *node) {

    // Animation translations (previous, current)
    Vec3f t3f0; geo_get_animation_translation(t3f0, sCurAnimState->type, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->_frame, sCurAnimState->mult, false);
    Vec3f t3f1; geo_get_animation_translation(t3f1, sCurAnimState->type, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->frame, sCurAnimState->mult, true);

    // Animation rotations (previous, current)
    Vec3s rot0, rot1;
    if (sCurAnimState->type != ANIM_TYPE_NONE) {
        sCurAnimState->type = ANIM_TYPE_ROTATION;
        geo_get_animation_data_as_vec3s(rot0, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->_frame, 1, 1, 1, false);
        geo_get_animation_data_as_vec3s(rot1, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->frame, 1, 1, 1, true);
    } else {
        vec3s_copy(rot0, gVec3sZero);
        vec3s_copy(rot1, gVec3sZero);
    }

    // Current frame
    Vec3f t3f; vec3s_to_vec3f(t3f, node->translation);
    vec3f_add(t3f, t3f1);
    geo_fix_marios_anim_translation_y(gCurrGraphNodeObject, node->translation[1], &t3f[1]);
    Mat4 mat; mtxf_rotate_xyz_and_translate(mat, t3f, rot1);
    mtxf_mul(gNextMat1, mat, gCurrMat1);
    
    // Previous frame
    if (should_interpolate_o(node->_translation)) {
        vec3s_to_vec3f(t3f, _o(node->_translation).v);
        vec3f_add(t3f, t3f0);
        geo_fix_marios_anim_translation_y(gCurrGraphNodeObject, _o(node->_translation).v[1], &t3f[1]);
        mtxf_rotate_xyz_and_translate(mat, t3f, rot0);
    }
    mtxf_mul(gNextMat0, mat, gCurrMat0);

    // Mario's root
    if (sMarioRootFlag == MRF_ROOT) {
        sMarioRootFlag = MRF_ROOT_ANIM_Y_FIXED;
    }
    
    // Update timestamps
NOT_PREPROCESS {
    update_timestamp_vec3s_o(node->_translation, node->translation);
}

PREPROCESS {

    // Mario root pos
    if (sMarioRootFlag == MRF_ROOT_ANIM_Y_FIXED) {
        vec3f_copy(sMarioRootPos, gNextMat1[3]);
        sMarioRootFlag = MRF_ROOT_POS_SAVED;
    }
    
    // Mario hand pos
    if (sMarioHandFlag != MHF_NOT_HAND) {
        vec3f_copy(sMarioHandPos[sMarioHandFlag - MHF_RIGHT_HAND], gNextMat1[3]);
        if (sMarioHandFlag == MHF_RIGHT_HAND && OMM_PLAYER_IS_PEACH) { // Right hand
            omm_perry_update_graphics(gMarioState, gCurrMat1, t3f, rot1);
        }
        sMarioHandFlag = MHF_NOT_HAND;
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
            get_pos_from_transform_mtx(shadowPos, gCurrMat1, *gCurGraphNodeCamera->matrixPtr);
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

        // Current frame
        s32 scale = shadowScale;
        Vec3f t3f; vec3f_copy(t3f, shadowPos);
        Gfx *shadow1 = create_shadow_below_xyz(t3f[0], t3f[1], t3f[2], scale, node->shadowSolidity, node->shadowType);
        Mat4 mat; mtxf_translate(mat, t3f);
        mtxf_mul(gNextMat1, mat, *(gCurGraphNodeCamera->lookAt1));

        // Previous frame
        Gfx *shadow0;
        if (should_interpolate(*_shadowPos)) {
            scale = _shadowScale->v;
            vec3f_copy(t3f, _shadowPos->v);
            mtxf_translate(mat, t3f);
            shadow0 = create_shadow_below_xyz(t3f[0], t3f[1], t3f[2], scale, node->shadowSolidity, node->shadowType);
            if (shadow0 && shadow1) {
                omm_array_grow(sShadowTbl, ptr, omm_new(InterpShadow, 1), sShadowTblSize + 1);
                InterpShadow *ishadow = (InterpShadow *) omm_array_get(sShadowTbl, ptr, sShadowTblSize);
                ishadow->vtx = (Vtx *) ((shadow0 + 1)->words.w1);
                omm_copy(ishadow->vtx0, (Vtx *) ((shadow0 + 1)->words.w1), sizeof(Vtx) * 4);
                omm_copy(ishadow->vtx1, (Vtx *) ((shadow1 + 1)->words.w1), sizeof(Vtx) * 4);
                sShadowTblSize++;
            }
        } else {
            shadow0 = shadow1;
        }
        mtxf_mul(gNextMat0, mat, *(gCurGraphNodeCamera->lookAt0));

        // Append shadow display list
        geo_update_mat_stack(
            geo_append_display_list(shadow0, shadow_get_layer());
        )
NOT_PREPROCESS {
        update_timestamp_vec3f(*_shadowPos, shadowPos);
        update_timestamp_f32(*_shadowScale, shadowScale);
}
    }
}
    // Process children
    geo_process_node_and_siblings(gChildren);
}

static void geo_process_object(struct Object *obj) {
    if (obj->oAreaIndex == gCurGraphNodeRoot->areaIndex) {
        struct GraphNodeObject *node = &obj->header.gfx;
NOT_PREPROCESS {
        // Enable matrix components interpolation only for objects with animations 
        sInterpMtxComponents = (node->mAnimInfo.curAnim != NULL);
}
NOT_PREPROCESS {
        // Don't interpolate the frame the object spawns
        // Force a gfx update to avoid weird (0, 0, 0) interpolations on later frames
        if (obj_alloc_fields(obj) && !obj->oGfxInited) {
            obj_update_gfx(obj);
            update_timestamp_vec3f(node->_pos, node->pos);
            update_timestamp_vec3s(node->_angle, node->angle);
            update_timestamp_vec3f(node->_scale, node->scale);
            update_timestamp_vec3f(node->_objPos, &obj->oPosX);
            sInterpMtxComponents = false;
            obj->oGfxInited = true;
        }

        // Pitch around +90/-90 seems to cause a gimbal lock, making the matrix extraction return wrong yaw angles
        // HACK: Disable matrix components interpolation for that edge case
        if (((u16) node->angle[0] >= 0x3C00 && (u16) node->angle[0] <= 0x4400) ||
            ((u16) node->angle[0] >= 0xBC00 && (u16) node->angle[0] <= 0xC400)) {
            sInterpMtxComponents = false;
        }
}

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
NOT_PREPROCESS {
        update_timestamp_vec3f(node->_objPos, &obj->oPosX);
}
        
        // Compute the object transform matrix from its throw matrix
        if (node->throwMatrix) {
            mtxf_mul(gNextMat1, *node->throwMatrix, gCurrMat1);
            if (should_interpolate(node->_throwMatrix)) {
                mtxf_mul(gNextMat0, node->_throwMatrix.v, gCurrMat0);
            } else {
                mtxf_mul(gNextMat0, *node->throwMatrix, gCurrMat0);
            }
NOT_PREPROCESS {
            update_timestamp_mtxf(node->_throwMatrix, *node->throwMatrix);
}
        }
        
        // Compute the object transform matrix from its position and cylboard
        else if (node->node.flags & GRAPH_RENDER_CYLBOARD) {
            mtxf_cylboard(gNextMat1, gCurrMat1, node->pos, gCurGraphNodeCamera->roll);
            if (should_interpolate(node->_pos)) {
                mtxf_cylboard(gNextMat0, gCurrMat0, node->_pos.v, gCurGraphNodeCamera->roll);
            } else {
                mtxf_cylboard(gNextMat0, gCurrMat0, node->pos, gCurGraphNodeCamera->roll);
            }
NOT_PREPROCESS {
            update_timestamp_vec3f(node->_pos, node->pos);
}
        }
        
        // Compute the object transform matrix from its position and billboard
        else if (node->node.flags & GRAPH_RENDER_BILLBOARD) {
            mtxf_billboard(gNextMat1, gCurrMat1, node->pos, gCurGraphNodeCamera->roll);
            if (should_interpolate(node->_pos)) {
                mtxf_billboard(gNextMat0, gCurrMat0, node->_pos.v, gCurGraphNodeCamera->roll);
            } else {
                mtxf_billboard(gNextMat0, gCurrMat0, node->pos, gCurGraphNodeCamera->roll);
            }
NOT_PREPROCESS {
            update_timestamp_vec3f(node->_pos, node->pos);
}
        }
        
        // Compute the object transform matrix from its position and angle
        else {
            Mat4 mat; mtxf_rotate_zxy_and_translate(mat, node->pos, node->angle);
            mtxf_mul(gNextMat1, mat, gCurrMat1);
            if (should_interpolate(node->_pos)) {
                if (should_interpolate(node->_angle)) {
                    mtxf_rotate_zxy_and_translate(mat, node->_pos.v, node->_angle.v);
                } else {
                    mtxf_rotate_zxy_and_translate(mat, node->_pos.v, node->angle);
                }
            } else if (should_interpolate(node->_angle)) {
                mtxf_rotate_zxy_and_translate(mat, node->pos, node->_angle.v);
            }
            mtxf_mul(gNextMat0, mat, gCurrMat0);
NOT_PREPROCESS {
            update_timestamp_vec3f(node->_pos, node->pos);
            update_timestamp_vec3s(node->_angle, node->angle);
}
        }
        
        // Scale
        Vec3f s0; vec3f_copy(s0, (should_interpolate(node->_scale) ? node->_scale.v : node->scale));
        Vec3f s1; vec3f_copy(s1, node->scale);
        mtxf_scale_vec3f(gNextMat0, gNextMat0, s0);
        mtxf_scale_vec3f(gNextMat1, gNextMat1, s1);
        node->throwMatrix = &gNextMat1;
        vec3f_copy(node->cameraToObject, gNextMat1[3]);
NOT_PREPROCESS {
        update_timestamp_vec3f(node->_scale, node->scale);
}

        // Render the object if it's in view
        if (obj_is_in_view(obj, gNextMat1)) {
            geo_update_mat_stack(
                if (node->sharedChild) {
                    sCurrObjectSlot = (&obj->header.gfx == &gMirrorMario ? OBJECT_POOL_CAPACITY : obj_get_slot_index(obj));
                    gCurGraphNodeObject = (struct GraphNodeObject *) node;
                    node->sharedChild->parent = gNode;
                    omm_sparkly_bowser_4_process_graph_node(obj, true, geo_process_generated_list);
                    geo_process_node_and_siblings(node->sharedChild);
                    omm_sparkly_bowser_4_process_graph_node(obj, false, geo_process_generated_list);
                    node->sharedChild->parent = NULL;
                    gCurGraphNodeObject = NULL;
                    sCurrObjectSlot = 0;
                }

                // Process children
                geo_process_node_and_siblings(gChildren);
            )
        } else if (obj != gMarioObject && obj != gOmmMario->capture.obj) {
NOT_PREPROCESS {
            reset_timestamp(node->_angle);
            reset_timestamp(node->_pos);
            reset_timestamp(node->_scale);
            reset_timestamp(node->_throwMatrix);
}
        }
        sInterpMtxComponents = false;
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
    geo_exec_node_func(gFnNode, GEO_CONTEXT_RENDER, gCurrMat1);
    struct Object *heldObj = node->objNode;
    if (heldObj && heldObj->oGraphNode) {
NOT_PREPROCESS {
        // Don't interpolate the frame the object spawns
        // Force a gfx update to avoid weird (0, 0, 0) interpolations on later frames
        if (obj_alloc_fields(heldObj) && !heldObj->oGfxInited) {
            obj_update_gfx(heldObj);
            update_timestamp_vec3s(node->_translation, node->translation);
            update_timestamp_mtxf(node->_throwMatrix, *gCurGraphNodeObject->throwMatrix);
            update_timestamp_vec3f(heldObj->header.gfx._scale, heldObj->oGfxScale);
            heldObj->oGfxInited = true;
        }
}

        // Current frame
        mtxf_copy(gNextMat1, *gCurGraphNodeObject->throwMatrix);
        vec3f_copy(gNextMat1[3], gCurrMat1[3]);
        Vec3f t3f; vec3s_to_vec3f(t3f, node->translation);
        vec3f_mul(t3f, 0.25f);
        Mat4 mat; mtxf_translate(mat, t3f);
        mtxf_mul(gNextMat1, mat, gNextMat1);
        mtxf_scale_vec3f(gNextMat1, gNextMat1, heldObj->oGfxScale);

        // Previous frame
        if (should_interpolate(node->_translation)) {
            mtxf_copy(gNextMat0, node->_throwMatrix.v);
            vec3f_copy(gNextMat0[3], gCurrMat0[3]);
            Vec3f t3f; vec3s_to_vec3f(t3f, node->_translation.v);
            vec3f_mul(t3f, 0.25f);
            Mat4 mat; mtxf_translate(mat, t3f);
            mtxf_mul(gNextMat0, mat, gNextMat0);
            mtxf_scale_vec3f(gNextMat0, gNextMat0, heldObj->header.gfx._scale.v);
        } else {
            mtxf_copy(gNextMat0, *gCurGraphNodeObject->throwMatrix);
            vec3f_copy(gNextMat0[3], gCurrMat0[3]);
            Vec3f t3f; vec3s_to_vec3f(t3f, node->translation);
            vec3f_mul(t3f, 0.25f);
            Mat4 mat; mtxf_translate(mat, t3f);
            mtxf_mul(gNextMat0, mat, gNextMat0);
            mtxf_scale_vec3f(gNextMat0, gNextMat0, heldObj->oGfxScale);
        }

        // Re-execute node function with GEO_CONTEXT_HELD_OBJ
        geo_exec_node_func(gFnNode, GEO_CONTEXT_HELD_OBJ, gNextMat1);
        update_timestamp_vec3s(node->_translation, node->translation);
        update_timestamp_mtxf(node->_throwMatrix, *gCurGraphNodeObject->throwMatrix);
        update_timestamp_vec3f(heldObj->header.gfx._scale, heldObj->oGfxScale);

        geo_update_mat_stack(

            // Make a back-up of the current anim state before processing the held object
            GeoAnimState curAnimState;
            omm_copy(&curAnimState, sCurAnimState, sizeof(sCurAnimState));

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
NOT_PREPROCESS {
            update_timestamp_vec3f(heldObj->header.gfx._objPos, &heldObj->oPosX);
}

            // Process held object
            gCurGraphNodeHeldObject = node;
            geo_process_node_and_siblings(heldObj->oGraphNode);
            gCurGraphNodeHeldObject = NULL;

            // Recover the current anim state
            omm_copy(sCurAnimState, &curAnimState, sizeof(curAnimState));
        )
    }
}
    // Process children
    geo_process_node_and_siblings(gChildren);
}

#ifdef GEO_CULLING_RADIUS
#undef GEO_CULLING_RADIUS
#endif
#define GEO_CULLING_RADIUS(x) x
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
        case GEO_PREPROCESS_PEACH_UP_VEC: {
            vec3f_copy(sUpVector, gCurrMat1[3]);
            vec3f_sub(sUpVector, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute the 'forward' vector
        case GEO_PREPROCESS_PEACH_FORWARD_VEC: {
            vec3f_copy(sFwdVector, gCurrMat1[3]);
            vec3f_sub(sFwdVector, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute the 'lateral' vector
        case GEO_PREPROCESS_PEACH_LATERAL_VEC: {
            vec3f_copy(sLatVector, gCurrMat1[3]);
            vec3f_sub(sLatVector, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute torso position
        case GEO_PREPROCESS_PEACH_TORSO_POS: {
            vec3f_copy(sTorsoPos, gCurrMat1[3]);
            vec3f_sub(sTorsoPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute left leg position
        case GEO_PREPROCESS_PEACH_LEFT_LEG_POS: {
            vec3f_copy(sLeftLegPos, gCurrMat1[3]);
            vec3f_sub(sLeftLegPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute left foot position
        case GEO_PREPROCESS_PEACH_LEFT_FOOT_POS: {
            vec3f_copy(sLeftFootPos, gCurrMat1[3]);
            vec3f_sub(sLeftFootPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute right leg position
        case GEO_PREPROCESS_PEACH_RIGHT_LEG_POS: {
            vec3f_copy(sRightLegPos, gCurrMat1[3]);
            vec3f_sub(sRightLegPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute right foot position
        case GEO_PREPROCESS_PEACH_RIGHT_FOOT_POS: {
            vec3f_copy(sRightFootPos, gCurrMat1[3]);
            vec3f_sub(sRightFootPos, gCurrGraphNodeObject->oGfxPos);
        } break;

        // Compute Peach's dress rotation, set hips rotation, set legs visibility
        case GEO_PREPROCESS_PEACH_HIPS_ROT_AND_LEGS: {

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
        case GEO_PREPROCESS_PEACH_DRESS_ROT: {
            struct GraphNodeRotation *rotNode = (struct GraphNodeRotation *) node->node.next;
            rotNode->rotation[0] = -(sPeachDressRot[0] * 2) / 3;
            rotNode->rotation[1] = -(sPeachDressRot[1] * 2) / 3;
            rotNode->rotation[2] = -(sPeachDressRot[2] * 2) / 3;
        } break;
        
        // Compute head rotation
        case GEO_PREPROCESS_PEACH_HEAD_ROT: {
            if (sCurAnimState->attr && sCurAnimState->type == ANIM_TYPE_ROTATION) {
                geo_get_animation_data_as_vec3s(sMarioHeadRot, &sCurAnimState->attr, sCurAnimState->data, sCurAnimState->frame, 1, 1, 1, 0);
            } else {
                vec3s_zero(sMarioHeadRot);
            }
        } break;

        // Set Peach's hair rotation
        case GEO_PREPROCESS_PEACH_HAIR_ROT: {
            struct GraphNodeRotation *rotNode = (struct GraphNodeRotation *) node->node.next;
            rotNode->rotation[0] = 0;
            rotNode->rotation[1] = -sMarioHeadRot[1] / 2;
            rotNode->rotation[2] = -sMarioHeadRot[2] / (1 + (sMarioHeadRot[2] > 0));
        } break;

        // Switch between Peach's crown and Tiara
        case GEO_PREPROCESS_PEACH_CROWN_TIARA: {
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
        case GEO_PREPROCESS_PEACH_PERRY_STATE: {
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
        case GEO_PREPROCESS_PEACH_HAND_ROT: {
            struct GraphNodeRotation *rot = (struct GraphNodeRotation *) node->node.next;
            struct Object *perry = omm_perry_get_object();
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
            omm_copy(viewport, viewport1, sizeof(Vp));
        } else if (viewport2) {
            clear_framebuffer(clearColor);
            make_viewport_clip_rect(viewport2);
        }
        gSPViewport(gDisplayListHead++, viewport);

        // Init matrix stack
        static Mtx mtx0, mtx1;
        sMatStackIndex = 0;
        mtxf_identity(gCurrMat0);
        mtxf_identity(gCurrMat1);
        mtxf_to_mtx(&mtx0, gCurrMat0);
        mtxf_to_mtx(&mtx1, gCurrMat1);
        sMtxStack0[sMatStackIndex] = &mtx0;
        sMtxStack1[sMatStackIndex] = &mtx1;
        gSPMatrix(gDisplayListHead++, &mtx1, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        sCurAnimState->type = ANIM_TYPE_NONE;

        // Process the node tree
#if OMM_CODE_DEBUG
        sDisableRendering = false;
#endif
        sIsPreprocess = false;
        sMarioRootFlag = MRF_NOT_ROOT;
        gCurGraphNodeRoot = node;
        geo_process_node_and_siblings(gChildren);
        gCurGraphNodeRoot = NULL;
    }
}

//
// Preprocess
//

static void __geo_preprocess_object_graph_node(struct Object *obj) {
    omm_zero(sCurAnimState, sizeof(sCurAnimState));
    sMatStackIndex = 0;
    sMarioHandFlag = MHF_NOT_HAND;
    sMarioRootFlag = MRF_NOT_ROOT;

    // Init globals
    Mat4 curGraphNodeCameraMatrix = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
    struct GraphNodeCamera curGraphNodeCamera = { .config.camera = gCamera, .matrixPtr = &curGraphNodeCameraMatrix };
    gCurGraphNodeCamera = &curGraphNodeCamera;
    sCurrObjectSlot = (&obj->header.gfx == &gMirrorMario ? OBJECT_POOL_CAPACITY : obj_get_slot_index(obj));
    gCurGraphNodeObject = (struct GraphNodeObject *) obj;
    mtxf_identity(gCurrMat1);

    // Init the base matrix
    if (obj->oThrowMatrix) {
        mtxf_copy(gCurrMat1, *obj->oThrowMatrix);
    } else {
        mtxf_rotate_zxy_and_translate(gCurrMat1, obj->oGfxPos, obj->oGfxAngle);
    }
    mtxf_scale_vec3f(gCurrMat1, gCurrMat1, obj->oGfxScale);

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
    sCurrObjectSlot = (&obj->header.gfx == &gMirrorMario ? OBJECT_POOL_CAPACITY : obj_get_slot_index(obj));
    gCurGraphNodeObject = (struct GraphNodeObject *) obj;
    obj->oGraphNode->parent = &obj->header.gfx.node;
    geo_process_node_and_siblings(obj->oGraphNode);
    obj->oGraphNode->parent = NULL;
    gCurGraphNodeObject = NULL;
    sCurrObjectSlot = 0;
    sIsPreprocess = false;

    // Restore animation info
    obj->oAnimInfo = animInfoBackUp;
}

void geo_preprocess_object_graph_node(struct Object *o) {
    if (o && o->oGraphNode) {

        // Retrieve Mario's height
        if (o == gMarioObject) {
            static OmmMap sMarioHeightMap = omm_map_zero;
            s32 i = omm_map_find_key(sMarioHeightMap, ptr, o->oGraphNode);
            if (i == -1) {

                // Preprocess with A-pose animation
                // Back-up the current animation and restore it after processing
                // Setting gMarioCurrAnimAddr to NULL forces the game to reload the DMA table
                struct AnimInfoStruct animInfo;
                omm_copy(&animInfo, &o->oAnimInfo, sizeof(struct AnimInfoStruct));
                gMarioCurrAnimAddr = NULL;
                obj_anim_play(gMarioObject, MARIO_ANIM_A_POSE, 1.f);
                __geo_preprocess_object_graph_node(o);
                gMarioCurrAnimAddr = NULL;
                omm_copy(&o->oAnimInfo, &animInfo, sizeof(struct AnimInfoStruct));

                // Compute and insert height for the object's graph node
                f32 marioHeight = 35.f + (sMarioHeadPos[1] - gMarioState->pos[1]);
                omm_map_add(sMarioHeightMap, ptr, o->oGraphNode, f32, marioHeight);
                i = omm_map_count(sMarioHeightMap) - 1;
            }
            sMarioHeight = omm_map_get_val(sMarioHeightMap, f32, i);
        }

        // Preprocess
        __geo_preprocess_object_graph_node(o);
    }
}

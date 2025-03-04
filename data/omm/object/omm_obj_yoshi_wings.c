#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx
//

static const Vtx omm_yoshi_wings_left_vtx[] = {
    {{{   0,   0, 0 }, 0, {   0, 990 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{   0, 128, 0 }, 0, {   0,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  64,   0, 0 }, 0, { 990, 990 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  64, 128, 0 }, 0, { 990,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  64,   0, 0 }, 0, {   0, 990 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  64, 128, 0 }, 0, {   0,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{ 128,   0, 0 }, 0, { 990, 990 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{ 128, 128, 0 }, 0, { 990,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
};

static const Vtx omm_yoshi_wings_right_vtx[] = {
    {{{    0,   0, 0 }, 0, {   0, 990 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{    0, 128, 0 }, 0, {   0,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  -64,   0, 0 }, 0, { 990, 990 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  -64, 128, 0 }, 0, { 990,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  -64,   0, 0 }, 0, {   0, 990 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  -64, 128, 0 }, 0, {   0,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{ -128,   0, 0 }, 0, { 990, 990 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{ -128, 128, 0 }, 0, { 990,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
};

static const Gfx omm_yoshi_wings_left_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH | G_LIGHTING),
    gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_ASSET_WING_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_yoshi_wings_left_vtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 1, 3, 0),
    gsDPLoadTextureBlock(OMM_ASSET_WING_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_yoshi_wings_left_vtx + 4, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 1, 3, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_yoshi_wings_right_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH | G_LIGHTING),
    gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_ASSET_WING_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_yoshi_wings_right_vtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 1, 3, 0),
    gsDPLoadTextureBlock(OMM_ASSET_WING_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPVertex(omm_yoshi_wings_right_vtx + 4, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 1, 3, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

static Gfx *omm_geo_yoshi_wings_rotate(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *asGenerated = (struct GraphNodeGenerated *) node;
        struct GraphNodeRotation *rotNode = (struct GraphNodeRotation *) node->next;
        struct Object *wings = gCurrGraphNodeObject;
        struct Object *yoshi = wings->parentObj;
        s16 rot;
        if (wings->oAnimState) {
            rot = coss(yoshi->oTimer * 0x2000) * 0x1800;
        } else {
            rot = coss(yoshi->oTimer * 0x1000) * 0x1000;
        }
        if (asGenerated->parameter) {
            rotNode->rotation[1] = -rot;
        } else {
            rotNode->rotation[1] = rot;
        }
    }
    return NULL;
}

const GeoLayout omm_geo_yoshi_wings[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 5, 20, 30, 70, 0, -45),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_yoshi_wings_rotate),
            GEO_ROTATION_NODE(LAYER_OPAQUE, 0, 0, 0),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_wings_left_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
        GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, -5, 20, 30, 70, 0, 45),
        GEO_OPEN_NODE(),
            GEO_ASM(1, omm_geo_yoshi_wings_rotate),
            GEO_ROTATION_NODE(LAYER_OPAQUE, 0, 0, 0),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_wings_right_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_yoshi_wings_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    struct MarioState *m = gMarioState;
    if (!p || !p->activeFlags || !omm_mario_has_wing_cap(m) || !omm_obj_is_playable_yoshi(p)) {
        obj_mark_for_deletion(o);
        return;
    }

    // Gfx
    Mat4 objTransform;
    Vec3f translation, shear, scale; Vec3s rotation;
    geo_compute_capture_cappy_obj_transform(p, 2, objTransform);
    mtxf_get_components(objTransform, translation, rotation, shear, scale);
    obj_set_pos_vec3f(o, translation);
    obj_set_angle(o, rotation[2] - 0x8000, rotation[1] - 0x4000, -rotation[0]);
    obj_copy_scale(o, p);
    obj_update_gfx(o);
    obj_set_always_rendered(o, true);
    obj_copy_visibility_and_transparency(o, p);
    if (omm_mario_cap_is_flickering(m)) {
        o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    }
}

const BehaviorScript bhvOmmYoshiWings[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_yoshi_wings_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_yoshi_wings(struct Object *o) {
    struct Object *wings = obj_get_first_with_behavior(bhvOmmYoshiWings);
    if (!wings) {
        wings = obj_spawn_from_geo(o, omm_geo_yoshi_wings, bhvOmmYoshiWings);
        wings->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        wings->parentObj = o;
    }
    return wings;
}

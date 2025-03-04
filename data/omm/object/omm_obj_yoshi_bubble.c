#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Vtx omm_yoshi_bubble_vtx[] = {
    {{{ -80, -80, 0 }, 0, {   0, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  80, -80, 0 }, 0, { 992, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  80,  80, 0 }, 0, { 992,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{ -80,  80, 0 }, 0, {   0,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
};

static const Gfx omm_yoshi_bubble_gfx[] = {
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_MISC_YOSHI_BUBBLE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(omm_yoshi_bubble_vtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

static Gfx *omm_geo_yoshi_bubble_show_number(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        s32 number = gCurrGraphNodeObject->oBehParams2ndByte;
        struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *) node;
        switchCase->selectedCase = (number > 1);
    }
    return NULL;
}

const GeoLayout omm_geo_yoshi_bubble[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_ASM(25, geo_move_from_camera_mario_scale),
            GEO_TRANSLATE_NODE(0, 0, 0, 0),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_yoshi_bubble_gfx),
            GEO_CLOSE_NODE(),
            GEO_TRANSLATE_NODE(0, 50, -50, 0),
            GEO_OPEN_NODE(),
                GEO_SCALE(0, 0xA000),
                GEO_OPEN_NODE(),
                    GEO_SWITCH_CASE(2, omm_geo_yoshi_bubble_show_number),
                    GEO_OPEN_NODE(),
                        GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
                        GEO_BRANCH(1, omm_geo_number_no_billboard),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_yoshi_bubble_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    if (!p || p != gOmmCapture || omm_capture_get_type(p) != OMM_CAPTURE_YOSHI || !gOmmObject->state.heldObj) {
        obj_mark_for_deletion(o);
        return;
    }

    o->oMoveAnglePitch += o->oAngleVelPitch;
    o->oMoveAngleYaw += o->oAngleVelYaw;
    if (o->oMoveAnglePitch >= 0x10000) {
        o->oMoveAnglePitch &= 0xFFFF;
        o->oAngleVelPitch = 0x400 + (random_u16() & 0x1FF);
    }
    if (o->oMoveAngleYaw >= 0x10000) {
        o->oMoveAngleYaw &= 0xFFFF;
        o->oAngleVelYaw = 0x400 + (random_u16() & 0x1FF);
    }
    f32 scaleX = 1.f + 0.1f * sins(o->oMoveAnglePitch);
    f32 scaleY = 1.f + 0.1f * coss(o->oMoveAngleYaw);
    obj_scale_xyz(o, scaleX, scaleY, 1.f);
}

static void bhv_omm_yoshi_dummy_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    if (!p || !p->activeFlags || p->behavior != bhvOmmYoshiBubble) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmYoshiBubble[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_yoshi_bubble_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_yoshi_bubble(struct Object *o) {
    struct Object *bubble = obj_get_first_with_behavior(bhvOmmYoshiBubble);
    if (!bubble) {
        bubble = obj_spawn_from_geo(o, omm_geo_yoshi_bubble, bhvOmmYoshiBubble);
        bubble->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        bubble->parentObj = o;
        bubble->oMoveAnglePitch = random_u16();
        bubble->oAngleVelPitch = 0x400 + (random_u16() & 0x1FF);
        bubble->oMoveAngleYaw = random_u16();
        bubble->oAngleVelYaw = 0x400 + (random_u16() & 0x1FF);
    }
    return bubble;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Gfx data
//

static const Vtx omm_number_vertices[] = {
    { { { -32, -32, 0 }, 0, {    0, 1024 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { +32, -32, 0 }, 0, { 1024, 1024 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { +32, +32, 0 }, 0, { 1024,    0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -32, +32, 0 }, 0, {    0,    0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -56, -32, 0 }, 0, {    0, 1024 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { {  +8, -32, 0 }, 0, { 1024, 1024 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { {  +8, +32, 0 }, 0, { 1024,    0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -56, +32, 0 }, 0, {    0,    0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { {  -8, -32, 0 }, 0, {    0, 1024 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { +56, -32, 0 }, 0, { 1024, 1024 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { +56, +32, 0 }, 0, { 1024,    0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { {  -8, +32, 0 }, 0, {    0,    0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

#define omm_number_define_gfx(type, digit, offset) \
const Gfx omm_number_##type##_gfx[] = { \
    gsSPClearGeometryMode(G_LIGHTING), \
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0), \
    gsDPLoadTextureBlock(OMM_TEXTURE_HUD_##digit, G_IM_FMT_RGBA, G_IM_SIZ_32b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0), \
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON), \
    gsSPVertex(omm_number_vertices + offset, 4, 0), \
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0), \
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF), \
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE), \
    gsSPSetGeometryMode(G_LIGHTING), \
    gsSPEndDisplayList(), \
};

omm_number_define_gfx(1digit_0, 0, 0);
omm_number_define_gfx(1digit_1, 1, 0);
omm_number_define_gfx(1digit_2, 2, 0);
omm_number_define_gfx(1digit_3, 3, 0);
omm_number_define_gfx(1digit_4, 4, 0);
omm_number_define_gfx(1digit_5, 5, 0);
omm_number_define_gfx(1digit_6, 6, 0);
omm_number_define_gfx(1digit_7, 7, 0);
omm_number_define_gfx(1digit_8, 8, 0);
omm_number_define_gfx(1digit_9, 9, 0);

omm_number_define_gfx(2digits_0x, 0, 4);
omm_number_define_gfx(2digits_1x, 1, 4);
omm_number_define_gfx(2digits_2x, 2, 4);
omm_number_define_gfx(2digits_3x, 3, 4);
omm_number_define_gfx(2digits_4x, 4, 4);
omm_number_define_gfx(2digits_5x, 5, 4);
omm_number_define_gfx(2digits_6x, 6, 4);
omm_number_define_gfx(2digits_7x, 7, 4);
omm_number_define_gfx(2digits_8x, 8, 4);
omm_number_define_gfx(2digits_9x, 9, 4);

omm_number_define_gfx(2digits_x0, 0, 8);
omm_number_define_gfx(2digits_x1, 1, 8);
omm_number_define_gfx(2digits_x2, 2, 8);
omm_number_define_gfx(2digits_x3, 3, 8);
omm_number_define_gfx(2digits_x4, 4, 8);
omm_number_define_gfx(2digits_x5, 5, 8);
omm_number_define_gfx(2digits_x6, 6, 8);
omm_number_define_gfx(2digits_x7, 7, 8);
omm_number_define_gfx(2digits_x8, 8, 8);
omm_number_define_gfx(2digits_x9, 9, 8);

omm_number_define_gfx(1digit_0b, 0b, 0);
omm_number_define_gfx(1digit_1b, 1b, 0);
omm_number_define_gfx(1digit_2b, 2b, 0);
omm_number_define_gfx(1digit_3b, 3b, 0);
omm_number_define_gfx(1digit_4b, 4b, 0);
omm_number_define_gfx(1digit_5b, 5b, 0);
omm_number_define_gfx(1digit_6b, 6b, 0);
omm_number_define_gfx(1digit_7b, 7b, 0);
omm_number_define_gfx(1digit_8b, 8b, 0);
omm_number_define_gfx(1digit_9b, 9b, 0);

omm_number_define_gfx(2digits_0xb, 0b, 4);
omm_number_define_gfx(2digits_1xb, 1b, 4);
omm_number_define_gfx(2digits_2xb, 2b, 4);
omm_number_define_gfx(2digits_3xb, 3b, 4);
omm_number_define_gfx(2digits_4xb, 4b, 4);
omm_number_define_gfx(2digits_5xb, 5b, 4);
omm_number_define_gfx(2digits_6xb, 6b, 4);
omm_number_define_gfx(2digits_7xb, 7b, 4);
omm_number_define_gfx(2digits_8xb, 8b, 4);
omm_number_define_gfx(2digits_9xb, 9b, 4);

omm_number_define_gfx(2digits_x0b, 0b, 8);
omm_number_define_gfx(2digits_x1b, 1b, 8);
omm_number_define_gfx(2digits_x2b, 2b, 8);
omm_number_define_gfx(2digits_x3b, 3b, 8);
omm_number_define_gfx(2digits_x4b, 4b, 8);
omm_number_define_gfx(2digits_x5b, 5b, 8);
omm_number_define_gfx(2digits_x6b, 6b, 8);
omm_number_define_gfx(2digits_x7b, 7b, 8);
omm_number_define_gfx(2digits_x8b, 8b, 8);
omm_number_define_gfx(2digits_x9b, 9b, 8);

//
// Geo functions
//

static Gfx *omm_geo_number_switch_1digit(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        s32 number = gCurrGraphNodeObject->oBehParams2ndByte;
        struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *) node;
        if (number >= 0 && number < 10) {
            switchCase->selectedCase = number;
        } else {
            switchCase->selectedCase = 10;
        }
    }
    return NULL;
}

static Gfx *omm_geo_number_switch_2digits_left(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        s32 number = gCurrGraphNodeObject->oBehParams2ndByte;
        struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *) node;
        if (number >= 10) {
            switchCase->selectedCase = number / 10;
        } else {
            switchCase->selectedCase = 10;
        }
    }
    return NULL;
}

static Gfx *omm_geo_number_switch_2digits_right(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        s32 number = gCurrGraphNodeObject->oBehParams2ndByte;
        struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *) node;
        if (number >= 10) {
            switchCase->selectedCase = number % 10;
        } else {
            switchCase->selectedCase = 10;
        }
    }
    return NULL;
}

//
// Geo layout
//

const GeoLayout omm_geo_number[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(11, omm_geo_number_switch_1digit),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_0_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_1_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_2_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_3_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_4_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_5_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_6_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_7_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_8_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_9_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
            GEO_CLOSE_NODE(),
            GEO_SWITCH_CASE(11, omm_geo_number_switch_2digits_left),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_0x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_1x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_2x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_3x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_4x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_5x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_6x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_7x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_8x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_9x_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
            GEO_CLOSE_NODE(),
            GEO_SWITCH_CASE(11, omm_geo_number_switch_2digits_right),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x0_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x1_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x2_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x3_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x4_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x5_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x6_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x7_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x8_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x9_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_number_no_billboard[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SWITCH_CASE(11, omm_geo_number_switch_1digit),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_0_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_1_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_2_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_3_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_4_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_5_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_6_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_7_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_8_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_9_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
        GEO_CLOSE_NODE(),
        GEO_SWITCH_CASE(11, omm_geo_number_switch_2digits_left),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_0x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_1x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_2x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_3x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_4x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_5x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_6x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_7x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_8x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_9x_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
        GEO_CLOSE_NODE(),
        GEO_SWITCH_CASE(11, omm_geo_number_switch_2digits_right),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x0_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x1_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x2_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x3_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x4_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x5_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x6_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x7_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x8_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x9_gfx),
            GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_number_b[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(11, omm_geo_number_switch_1digit),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_0b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_1b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_2b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_3b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_4b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_5b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_6b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_7b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_8b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_1digit_9b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
            GEO_CLOSE_NODE(),
            GEO_SWITCH_CASE(11, omm_geo_number_switch_2digits_left),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_0xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_1xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_2xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_3xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_4xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_5xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_6xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_7xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_8xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_9xb_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
            GEO_CLOSE_NODE(),
            GEO_SWITCH_CASE(11, omm_geo_number_switch_2digits_right),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x0b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x1b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x2b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x3b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x4b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x5b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x6b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x7b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x8b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, omm_number_2digits_x9b_gfx),
                GEO_DISPLAY_LIST(LAYER_ALPHA, NULL),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_star_number_loop() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    if (!OMM_EXTRAS_SHOW_STAR_NUMBER || !p || !p->activeFlags || !omm_obj_is_star_or_key(p) || obj_is_dormant(p)) {
        obj_mark_for_deletion(o);
        return;
    }

    obj_set_xyz(o, p->oPosX, p->oPosY + 150.f * o->oScaleY, p->oPosZ);
    obj_set_angle(o, 0, 0, 0);
    obj_set_scale(o, 1.f, 1.f, 1.f);
    o->oNodeFlags = p->oNodeFlags;
}

const BehaviorScript bhvOmmStarNumber[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_star_number_loop),
    BHV_END_LOOP(),
};

static void bhv_omm_blue_coins_number_loop() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    struct Object *p = m->marioObj->platform;
    if (p && p->behavior == bhvBlueCoinSwitch && p->oAction == BLUE_COIN_SWITCH_ACT_IDLE) {
        o->oBehParams2ndByte = obj_get_count_with_behavior(bhvHiddenBlueCoin);
        obj_set_xyz(o, m->pos[0], m->pos[1] + 200, m->pos[2]);
        obj_set_angle(o, 0, 0, 0);
        obj_set_scale(o, 0.8f, 0.8f, 0.8f);
    } else {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmBlueCoinsNumber[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_blue_coins_number_loop),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_orange_number(struct Object *o, s32 num, f32 offsetY, s32 soundBits, s32 variation, const GeoLayout *geoLayout) {
    struct Object *number = spawn_object_relative(num, 0, 0, 0, o, MODEL_NONE, bhvOrangeNumber);
    number->oGraphNode = geo_layout_to_graph_node(NULL, geoLayout);
    number->oPosY += offsetY;
    if (soundBits != NO_SOUND) {
        play_sound(soundBits + (variation << 16), gGlobalSoundArgs);
    }
    return number;
}

struct Object *omm_obj_spawn_blue_coins_number(struct Object *o) {
    struct Object *number = obj_spawn_from_geo(o, omm_geo_number_b, bhvOmmBlueCoinsNumber);
    number->oBehParams2ndByte = 0;
    return number;
}

struct Object *omm_obj_spawn_star_number(struct Object *o) {
    u8 starIndex = (u8) (o->oBehParams >> 24);
    if (starIndex < OMM_NUM_STARS_MAX_PER_COURSE) {
        u8 starLevelFlags = omm_stars_get_level_flags(gCurrLevelNum, OMM_GAME_MODE);
        if (starLevelFlags & (1 << starIndex)) {
            s32 starNumber = 0;
            for (u8 i = 0; i <= starIndex; ++i) {
                starNumber += ((starLevelFlags >> i) & 1);
            }
            if (starNumber > 0) {
                struct Object *number = obj_spawn_from_geo(o, omm_geo_number, bhvOmmStarNumber);
                number->oBehParams2ndByte = starNumber;
                return number;
            }
        }
    }
    return NULL;
}

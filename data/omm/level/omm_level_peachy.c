#define ROOM_SIZE   2000
#define ROOM_LENGTH 16
#define N           (-ROOM_SIZE / 2)
#define P           (+ROOM_SIZE / 2)
#define NORMAL_U    { 0, +127, 0, 0xff }
#define NORMAL_N    { 0, 0, -127, 0xff }
#define NORMAL_S    { 0, 0, +127, 0xff }
#define NORMAL_E    { +127, 0, 0, 0xff }
#define NORMAL_W    { -127, 0, 0, 0xff }
#define TEX_SIZE    32
#define TEX_FRAC    0x20
#define TEX_UNIT    (TEX_SIZE * TEX_FRAC)
#define UV0         0x0000
#define UV1         ((TEX_SIZE - 1) * TEX_FRAC)
#define UVx(x)      ((TEX_SIZE * (x) - 1) * TEX_FRAC)

#ifdef GFX_PC_C
#if VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif
#if OMM_GAME_IS_SM64
extern Vtx omm_level_peachy_room_vtx[];
extern f32 omm_level_peachy_room_prev_z;
extern f32 omm_level_peachy_room_curr_z;

static void omm_level_peachy_room_update_tex_coords() {
    f32 z = lerp_f(sLerpDelta, omm_level_peachy_room_prev_z, omm_level_peachy_room_curr_z);

    // Carpet
    s32 cw = TEX_UNIT * ROOM_LENGTH;
    s32 cz = (s32) ((TEX_UNIT * z) / ROOM_SIZE) % cw;
    omm_level_peachy_room_vtx[0x0].n.tc[1] = cw - cz;
    omm_level_peachy_room_vtx[0x1].n.tc[1] = cw - cz;
    omm_level_peachy_room_vtx[0x2].n.tc[1] = omm_level_peachy_room_vtx[0x0].n.tc[1] + UVx(ROOM_LENGTH);
    omm_level_peachy_room_vtx[0x3].n.tc[1] = omm_level_peachy_room_vtx[0x1].n.tc[1] + UVx(ROOM_LENGTH);

    // Left wall
    s32 lw = (TEX_UNIT * ROOM_LENGTH) / 4;
    s32 lz = (s32) ((TEX_UNIT * z) / (ROOM_SIZE * 4)) % lw;
    omm_level_peachy_room_vtx[0x4].n.tc[0] = lz;
    omm_level_peachy_room_vtx[0x6].n.tc[0] = lz;
    omm_level_peachy_room_vtx[0x5].n.tc[0] = omm_level_peachy_room_vtx[0x4].n.tc[0] + UVx(ROOM_LENGTH / 4);
    omm_level_peachy_room_vtx[0x7].n.tc[0] = omm_level_peachy_room_vtx[0x6].n.tc[0] + UVx(ROOM_LENGTH / 4);

    // Right wall
    s32 rw = (TEX_UNIT * ROOM_LENGTH) / 4;
    s32 rz = (s32) ((TEX_UNIT * z) / (ROOM_SIZE * 4)) % rw;
    omm_level_peachy_room_vtx[0x8].n.tc[0] = rw - rz;
    omm_level_peachy_room_vtx[0xA].n.tc[0] = rw - rz;
    omm_level_peachy_room_vtx[0x9].n.tc[0] = omm_level_peachy_room_vtx[0x8].n.tc[0] + UVx(ROOM_LENGTH / 4);
    omm_level_peachy_room_vtx[0xB].n.tc[0] = omm_level_peachy_room_vtx[0xA].n.tc[0] + UVx(ROOM_LENGTH / 4);
}

//
// Custom commands
//

static void omm_level_peachy_room_gfx_sp_set_other_mode_h() {
    gfx_sp_set_other_mode_h();
    omm_level_peachy_room_update_tex_coords();
}

//
// Display lists
//

const Gfx omm_level_peachy_room_gfx_enable[] = {
    gsXPSwapCmd(G_SETOTHERMODE_H, omm_level_peachy_room_gfx_sp_set_other_mode_h),
    gsSPEndDisplayList(),
};

const Gfx omm_level_peachy_room_gfx_disable[] = {
    gsXPSwapCmd(G_SETOTHERMODE_H, NULL),
    gsSPEndDisplayList(),
};

#endif
#else
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#if OMM_GAME_IS_SM64
#include "levels/castle_courtyard/header.h"
extern const Gfx omm_level_peachy_room_gfx_enable[];
extern const Gfx omm_level_peachy_room_gfx_disable[];
static const BehaviorScript bhvOmmLevelPeachyRoom[];

//
// Textures
//

static const u8 OMM_TEXTURE_PEACHY_0[] = "peach/omm_texture_peachy_0.rgba32";
static const u8 OMM_TEXTURE_PEACHY_1[] = "peach/omm_texture_peachy_1.rgba32";
static const u8 OMM_TEXTURE_PEACHY_2[] = "peach/omm_texture_peachy_2.rgba32";
static const u8 OMM_TEXTURE_PEACHY_3[] = "peach/omm_texture_peachy_3.rgba32";

//
// Gfx data
//

static const Lights1 omm_level_peachy_room_lights = gdSPDefLights1(
    0x7f, 0x7f, 0x7f,
    0xff, 0xff, 0xff,
    0x28, 0x28, 0x28
);

Vtx omm_level_peachy_room_vtx[] = {
    { { { N, N, N * ROOM_LENGTH }, 0, { UV0, UV0 }, NORMAL_U } },
    { { { P, N, N * ROOM_LENGTH }, 0, { UV1, UV0 }, NORMAL_U } },
    { { { N, N, P * ROOM_LENGTH }, 0, { UV0, UVx(ROOM_LENGTH) }, NORMAL_U } },
    { { { P, N, P * ROOM_LENGTH }, 0, { UV1, UVx(ROOM_LENGTH) }, NORMAL_U } },
    { { { N, P, P * ROOM_LENGTH }, 0, { UV0, UV0 }, NORMAL_E } },
    { { { N, P, N * ROOM_LENGTH }, 0, { UVx(ROOM_LENGTH / 4), UV0 }, NORMAL_E } },
    { { { N, N, P * ROOM_LENGTH }, 0, { UV0, UV1 }, NORMAL_E } },
    { { { N, N, N * ROOM_LENGTH }, 0, { UVx(ROOM_LENGTH / 4), UV1 }, NORMAL_E } },
    { { { P, P, N * ROOM_LENGTH }, 0, { UV0, UV0 }, NORMAL_W } },
    { { { P, P, P * ROOM_LENGTH }, 0, { UVx(ROOM_LENGTH / 4), UV0 }, NORMAL_W } },
    { { { P, N, N * ROOM_LENGTH }, 0, { UV0, UV1 }, NORMAL_W } },
    { { { P, N, P * ROOM_LENGTH }, 0, { UVx(ROOM_LENGTH / 4), UV1 }, NORMAL_W } },
    { { { P, P, P * ROOM_LENGTH }, 0, { UV0, UV0 }, NORMAL_N } },
    { { { N, P, P * ROOM_LENGTH }, 0, { UV1, UV0 }, NORMAL_N } },
    { { { P, N, P * ROOM_LENGTH }, 0, { UV0, UV1 }, NORMAL_N } },
    { { { N, N, P * ROOM_LENGTH }, 0, { UV1, UV1 }, NORMAL_N } },
    { { { N, P, N * ROOM_LENGTH }, 0, { UV0, UV0 }, NORMAL_S } },
    { { { P, P, N * ROOM_LENGTH }, 0, { UV1, UV0 }, NORMAL_S } },
    { { { N, N, N * ROOM_LENGTH }, 0, { UV0, UV1 }, NORMAL_S } },
    { { { P, N, N * ROOM_LENGTH }, 0, { UV1, UV1 }, NORMAL_S } },
};

static const Gfx omm_level_peachy_room_tri[] = {
    gsSPVertex(omm_level_peachy_room_vtx + 0, 4, 0),
    gsSP2Triangles(2, 1, 0, 0, 1, 2, 3, 0),
    gsSPEndDisplayList(),
    gsSPVertex(omm_level_peachy_room_vtx + 4, 4, 0),
    gsSP2Triangles(2, 1, 0, 0, 1, 2, 3, 0),
    gsSPEndDisplayList(),
    gsSPVertex(omm_level_peachy_room_vtx + 8, 4, 0),
    gsSP2Triangles(2, 1, 0, 0, 1, 2, 3, 0),
    gsSPEndDisplayList(),
    gsSPVertex(omm_level_peachy_room_vtx + 12, 4, 0),
    gsSP2Triangles(2, 1, 0, 0, 1, 2, 3, 0),
    gsSPEndDisplayList(),
    gsSPVertex(omm_level_peachy_room_vtx + 16, 4, 0),
    gsSP2Triangles(2, 1, 0, 0, 1, 2, 3, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_level_peachy_room_gfx[] = {
    gsSPDisplayList(omm_level_peachy_room_gfx_enable),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPDisplayList(omm_level_peachy_room_gfx_disable),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetDepthSource(G_ZS_PIXEL),
    gsDPSetFogColor(0x00, 0x00, 0x00, 0xff),
    gsSPFogPosition(950, 1000),
    gsSPSetGeometryMode(G_FOG),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_ALPHA | G_CULL_BOTH),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_PASS2),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_level_peachy_room_lights.l, 1),
    gsSPLight(&omm_level_peachy_room_lights.a, 2),
    gsDPLoadTextureBlock(OMM_TEXTURE_PEACHY_0, G_IM_FMT_RGBA, G_IM_SIZ_32b, TEX_SIZE, TEX_SIZE, 0, G_TX_CLAMP, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_level_peachy_room_tri + 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_PEACHY_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, TEX_SIZE, TEX_SIZE, 0, 0, G_TX_CLAMP, 0, 0, 0, 0),
    gsSPDisplayList(omm_level_peachy_room_tri + 3),
    gsDPLoadTextureBlock(OMM_TEXTURE_PEACHY_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, TEX_SIZE, TEX_SIZE, 0, 0, G_TX_CLAMP, 0, 0, 0, 0),
    gsSPDisplayList(omm_level_peachy_room_tri + 6),
    gsDPLoadTextureBlock(OMM_TEXTURE_PEACHY_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, TEX_SIZE, TEX_SIZE, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0),
    gsSPDisplayList(omm_level_peachy_room_tri + 9),
    gsDPLoadTextureBlock(OMM_TEXTURE_PEACHY_3, G_IM_FMT_RGBA, G_IM_SIZ_32b, TEX_SIZE, TEX_SIZE, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0),
    gsSPDisplayList(omm_level_peachy_room_tri + 12),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_NOOP2),
    gsSPClearGeometryMode(G_FOG),
    gsSPEndDisplayList(),
};

//
// Geo layouts
//

static const GeoLayout omm_level_peachy_room_geo[] = {
    GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
    GEO_OPEN_NODE(),
        GEO_ZBUFFER(0),
        GEO_OPEN_NODE(),
            GEO_NODE_ORTHO(100),
            GEO_OPEN_NODE(),
                GEO_BACKGROUND_COLOR(0),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
        GEO_ZBUFFER(1),
        GEO_OPEN_NODE(),
            GEO_CAMERA_FRUSTUM_WITH_FUNC(45, 100, 20000, geo_camera_fov),
            GEO_OPEN_NODE(),
                GEO_CAMERA(CAMERA_MODE_8_DIRECTIONS, 0, 0, 0, 0, 0, 0, geo_camera_main),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_level_peachy_room_gfx),
                    GEO_RENDER_OBJ(),
                    GEO_ASM(0, geo_envfx_main),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Collision
//

static const Collision omm_level_peachy_room_collision[] = {
    COL_INIT(),
    COL_VERTEX_INIT(24),
    COL_VERTEX(N, N, N * ROOM_LENGTH),
    COL_VERTEX(P, N, N * ROOM_LENGTH),
    COL_VERTEX(N, N, P * ROOM_LENGTH),
    COL_VERTEX(P, N, P * ROOM_LENGTH),
    COL_VERTEX(N, P, N * ROOM_LENGTH),
    COL_VERTEX(P, P, N * ROOM_LENGTH),
    COL_VERTEX(N, P, P * ROOM_LENGTH),
    COL_VERTEX(P, P, P * ROOM_LENGTH),
    COL_VERTEX(N, P, P * ROOM_LENGTH),
    COL_VERTEX(N, P, N * ROOM_LENGTH),
    COL_VERTEX(N, N, P * ROOM_LENGTH),
    COL_VERTEX(N, N, N * ROOM_LENGTH),
    COL_VERTEX(P, P, N * ROOM_LENGTH),
    COL_VERTEX(P, P, P * ROOM_LENGTH),
    COL_VERTEX(P, N, N * ROOM_LENGTH),
    COL_VERTEX(P, N, P * ROOM_LENGTH),
    COL_VERTEX(P, P, P * ROOM_LENGTH),
    COL_VERTEX(N, P, P * ROOM_LENGTH),
    COL_VERTEX(P, N, P * ROOM_LENGTH),
    COL_VERTEX(N, N, P * ROOM_LENGTH),
    COL_VERTEX(N, P, N * ROOM_LENGTH),
    COL_VERTEX(P, P, N * ROOM_LENGTH),
    COL_VERTEX(N, N, N * ROOM_LENGTH),
    COL_VERTEX(P, N, N * ROOM_LENGTH),
    COL_TRI_INIT(SURFACE_DEFAULT, 12),
    COL_TRI(2, 1, 0),
    COL_TRI(1, 2, 3),
    COL_TRI(4, 5, 6),
    COL_TRI(7, 6, 5),
    COL_TRI(10, 9, 8),
    COL_TRI(9, 10, 11),
    COL_TRI(14, 13, 12),
    COL_TRI(13, 14, 15),
    COL_TRI(18, 17, 16),
    COL_TRI(17, 18, 19),
    COL_TRI(22, 21, 20),
    COL_TRI(21, 22, 23),
    COL_TRI_STOP(),
    COL_END(),
};

//
// Behavior script
//

static void bhv_omm_level_peachy_room_update_obj_coords(f32 dz) {
    static const s32 objLists[] = {
        OBJ_LIST_DESTRUCTIVE,
        OBJ_LIST_GENACTOR,
        OBJ_LIST_PUSHABLE,
        OBJ_LIST_LEVEL,
        OBJ_LIST_DEFAULT,
        OBJ_LIST_SURFACE,
        OBJ_LIST_POLELIKE,
        OBJ_LIST_SPAWNER,
        OBJ_LIST_UNIMPORTANT,
        0
    };
    for_each_until_null(const s32, objList, objLists) {
        for_each_object_in_list(obj, *objList) {
            if (obj->behavior != bhvOmmLevelPeachyRoom &&
                obj->behavior != bhvOmmPerryTrail &&
                obj->behavior != bhvOmmPeachVibeGloomTearSmall) {
                obj->oPosZ += dz;
                obj->oGfxPos[2] += dz;
                obj_set_dormant(obj, !((N * (ROOM_LENGTH + 2)) < obj->oPosZ && obj->oPosZ < (P * (ROOM_LENGTH + 2))));
            }
        }
    }
}

f32 omm_level_peachy_room_prev_z = 0;
f32 omm_level_peachy_room_curr_z = 0;
static void bhv_omm_level_peachy_room_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;

    // "Infinite" corridor
    omm_level_peachy_room_prev_z = o->oPosZ;
    if (m->pos[2] < 0 || o->oPosZ > 0) {
        o->oVelZ = -min_f(m->pos[2], o->oPosZ);
        o->oPosZ += o->oVelZ;
        m->pos[2] += o->oVelZ;
        m->marioObj->oPosZ += o->oVelZ;
        m->marioObj->oGfxPos[2] += o->oVelZ;
    } else {
        o->oVelZ = 0;
    }
    omm_level_peachy_room_curr_z = o->oPosZ;
    bhv_omm_level_peachy_room_update_obj_coords(o->oVelZ);

    // Warps
    for_each_object_with_behavior(obj, bhvWarp) {
        obj->hitboxRadius = ROOM_SIZE;
        obj->hitboxHeight = ROOM_SIZE * 0.45f;
        obj->hitboxDownOffset = 0;
    }
    
    // Hide the HUD
    gHudDisplay.flags = HUD_DISPLAY_NONE;
}

static const BehaviorScript bhvOmmLevelPeachyRoom[] = {
    OBJ_TYPE_DEFAULT,
    0x08000000,
    0x0C000000, (uintptr_t) bhv_omm_level_peachy_room_update,
    0x09000000,
};

//
// Objects
//

const LevelScript omm_level_peachy_room_objects[] = {
    OBJECT(MODEL_NONE, 0, 0, 0, 0, 0, 0, 0, bhvOmmLevelPeachyRoom),
    OBJECT(MODEL_NONE, 0, N, (s16) (P * (ROOM_LENGTH - 1.0f)), 0,0, 0, 0x000A0000, bhvSpinAirborneCircleWarp),
    OBJECT(MODEL_WOODEN_SIGNPOST, 0, N, (s16) (P * (ROOM_LENGTH - 0.5f)), 0, 180, 0, OMM_DIALOG_PEACHY_ROOM << 16, bhvMessagePanel),
    OBJECT(MODEL_NONE, 0, (s16) (N * 0.6f), (s16) (P * ROOM_LENGTH + ROOM_SIZE * 0.96f), 0, 0, 0, 0x00100000, bhvWarp),
    RETURN(),
};

//
// Script
//

static const LevelScript omm_level_peachy_room[] = {
    END_AREA(),
    AREA(4, omm_level_peachy_room_geo),
        WARP_NODE(0x0A, LEVEL_CASTLE_COURTYARD, 4, 0x0A, WARP_NO_CHECKPOINT),
        UNPACK(WARP_NODE, 0x10, OMM_LEVEL_RETURN_TO_CASTLE),
        UNPACK(WARP_NODE, 0xF1, OMM_LEVEL_RETURN_TO_CASTLE),
        JUMP_LINK(script_func_global_13),
        JUMP_LINK(omm_level_peachy_room_objects),
        TERRAIN(omm_level_peachy_room_collision),
        TERRAIN_TYPE(TERRAIN_STONE),
    END_AREA(),
    RETURN(),
};

LEVEL_CMD_BRANCH(
    level_script_find(level_castle_courtyard_entry, omm_static_array_of(LevelScript) { END_AREA() }, 1),
    level_script_find(level_castle_courtyard_entry, omm_static_array_of(LevelScript) { FREE_LEVEL_POOL() }, 1),
    omm_level_peachy_room
);

#endif
#endif
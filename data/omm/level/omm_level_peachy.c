#define ROOM_SIZE       (2000)
#define ROOM_LENGTH     (16)
#define N               (-ROOM_SIZE / 2)
#define P               (+ROOM_SIZE / 2)
#define NORMAL_U        { 0, +127, 0, 0xFF }
#define NORMAL_N        { 0, 0, -127, 0xFF }
#define NORMAL_S        { 0, 0, +127, 0xFF }
#define NORMAL_E        { +127, 0, 0, 0xFF }
#define NORMAL_W        { -127, 0, 0, 0xFF }
#define TEX_SIZE        (32)
#define TEX_FRAC        (0x20)
#define TEX_UNIT        (TEX_SIZE * TEX_FRAC)
#define UV0             (0x0000)
#define UV1             ((TEX_SIZE - 1) * TEX_FRAC)
#define UVx(x)          ((TEX_SIZE * (x) - 1) * TEX_FRAC)
#define PAINTING_TILES  (30)
#define PAINTING_SIZE   (0.52f)
#define PAINTING_DOWN   ((1.f - PAINTING_SIZE) / 2.f - 1.f / 16.f)
#define PAINTING_UP     (PAINTING_DOWN + PAINTING_SIZE)
#define PAINTING_LEFT   ((1.f - PAINTING_SIZE) / 2.f)
#define PAINTING_RIGHT  (PAINTING_LEFT + PAINTING_SIZE)
#define PAINTING_X(x)   (N + ((P - N) * (x)))
#define PAINTING_Y(y)   (N + ((P - N) * (y)))
#define PAINTING_U(u)   (UV0 + ((UV1 - UV0) * (u)))
#define PAINTING_V(v)   (UV0 + ((UV1 - UV0) * (1.f - (v))))
#define PAINTING_N(z)   { 0, 0, -sign_f(z) * 127, 0xFF }

#ifdef GFX_PC_C
#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif
#if OMM_GAME_IS_SM64 || OMM_GAME_IS_SM74
extern Vtx omm_level_peachy_room_carpet_vtx[];
extern Vtx omm_level_peachy_room_walls_vtx[];
extern f32 omm_level_peachy_room_prev_z;
extern f32 omm_level_peachy_room_curr_z;

static void omm_level_peachy_room_update_tex_coords() {
    f32 z = lerp_f(sFps->lerp, omm_level_peachy_room_prev_z, omm_level_peachy_room_curr_z);

    // Carpet
    s32 cw = TEX_UNIT * ROOM_LENGTH;
    s32 cz = (s32) ((TEX_UNIT * z) / ROOM_SIZE) % cw;
    omm_level_peachy_room_carpet_vtx[0x0].n.tc[1] = cw - cz;
    omm_level_peachy_room_carpet_vtx[0x1].n.tc[1] = cw - cz;
    omm_level_peachy_room_carpet_vtx[0x2].n.tc[1] = omm_level_peachy_room_carpet_vtx[0x0].n.tc[1] + UVx(ROOM_LENGTH);
    omm_level_peachy_room_carpet_vtx[0x3].n.tc[1] = omm_level_peachy_room_carpet_vtx[0x1].n.tc[1] + UVx(ROOM_LENGTH);

    // Left wall
    s32 lw = (TEX_UNIT * ROOM_LENGTH) / 4;
    s32 lz = (s32) ((TEX_UNIT * z) / (ROOM_SIZE * 4)) % lw;
    omm_level_peachy_room_walls_vtx[0x0].n.tc[0] = lz;
    omm_level_peachy_room_walls_vtx[0x2].n.tc[0] = lz;
    omm_level_peachy_room_walls_vtx[0x1].n.tc[0] = omm_level_peachy_room_walls_vtx[0x0].n.tc[0] + UVx(ROOM_LENGTH / 4);
    omm_level_peachy_room_walls_vtx[0x3].n.tc[0] = omm_level_peachy_room_walls_vtx[0x2].n.tc[0] + UVx(ROOM_LENGTH / 4);

    // Right wall
    s32 rw = (TEX_UNIT * ROOM_LENGTH) / 4;
    s32 rz = (s32) ((TEX_UNIT * z) / (ROOM_SIZE * 4)) % rw;
    omm_level_peachy_room_walls_vtx[0x4].n.tc[0] = rw - rz;
    omm_level_peachy_room_walls_vtx[0x6].n.tc[0] = rw - rz;
    omm_level_peachy_room_walls_vtx[0x5].n.tc[0] = omm_level_peachy_room_walls_vtx[0x4].n.tc[0] + UVx(ROOM_LENGTH / 4);
    omm_level_peachy_room_walls_vtx[0x7].n.tc[0] = omm_level_peachy_room_walls_vtx[0x6].n.tc[0] + UVx(ROOM_LENGTH / 4);
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
#include "behavior_commands.h"
#if OMM_GAME_IS_SM64 || OMM_GAME_IS_SM74
#include "data/omm/omm_constants.h"
#include "level_commands.h"
#include "levels/bits/header.h"
#include "levels/castle_courtyard/header.h"
extern const Gfx omm_level_peachy_room_gfx_enable[];
extern const Gfx omm_level_peachy_room_gfx_disable[];
static const BehaviorScript bhvOmmLevelPeachyRoom[];
extern struct Cutscene sCutsceneEnterPainting[];

f32 omm_level_peachy_room_prev_z = 0;
f32 omm_level_peachy_room_curr_z = 0;

//
// Textures
//

#if OMM_GAME_IS_SM74
static const u8 OMM_TEXTURE_PEACHY_0[] = "menu/sm74/room_0.rgba32";
static const u8 OMM_TEXTURE_PEACHY_1[] = "menu/sm74/room_1.rgba32";
static const u8 OMM_TEXTURE_PEACHY_2[] = "menu/sm74/room_2.rgba32";
static const u8 OMM_TEXTURE_PEACHY_3[] = "menu/sm74/room_2.rgba32";
#else
static const u8 OMM_TEXTURE_PEACHY_0[] = "peach/omm_texture_peachy_0.rgba32";
static const u8 OMM_TEXTURE_PEACHY_1[] = "peach/omm_texture_peachy_1.rgba32";
static const u8 OMM_TEXTURE_PEACHY_2[] = "peach/omm_texture_peachy_2.rgba32";
static const u8 OMM_TEXTURE_PEACHY_3[] = "peach/omm_texture_peachy_3.rgba32";
#endif

//
// Gfx data
//

static const Lights1 omm_level_peachy_room_lights = gdSPDefLights1(
    0x7F, 0x7F, 0x7F,
    0xFF, 0xFF, 0xFF,
    0x28, 0x28, 0x28
);

Vtx omm_level_peachy_room_carpet_vtx[] = {
    { { { N, N, N * ROOM_LENGTH }, 0, { UV0, UV0 }, NORMAL_U } },
    { { { P, N, N * ROOM_LENGTH }, 0, { UV1, UV0 }, NORMAL_U } },
    { { { N, N, P * ROOM_LENGTH }, 0, { UV0, UVx(ROOM_LENGTH) }, NORMAL_U } },
    { { { P, N, P * ROOM_LENGTH }, 0, { UV1, UVx(ROOM_LENGTH) }, NORMAL_U } },
};

static const Gfx omm_level_peachy_room_carpet_tri[] = {
    gsSPVertex(omm_level_peachy_room_carpet_vtx, 4, 0),
    gsSP2Triangles(2, 1, 0, 0, 1, 2, 3, 0),
    gsSPEndDisplayList(),
};

Vtx omm_level_peachy_room_walls_vtx[] = {
    { { { N, P, P * ROOM_LENGTH }, 0, { UV0, UV0 }, NORMAL_E } },
    { { { N, P, N * ROOM_LENGTH }, 0, { UVx(ROOM_LENGTH / 4), UV0 }, NORMAL_E } },
    { { { N, N, P * ROOM_LENGTH }, 0, { UV0, UV1 }, NORMAL_E } },
    { { { N, N, N * ROOM_LENGTH }, 0, { UVx(ROOM_LENGTH / 4), UV1 }, NORMAL_E } },
    { { { P, P, N * ROOM_LENGTH }, 0, { UV0, UV0 }, NORMAL_W } },
    { { { P, P, P * ROOM_LENGTH }, 0, { UVx(ROOM_LENGTH / 4), UV0 }, NORMAL_W } },
    { { { P, N, N * ROOM_LENGTH }, 0, { UV0, UV1 }, NORMAL_W } },
    { { { P, N, P * ROOM_LENGTH }, 0, { UVx(ROOM_LENGTH / 4), UV1 }, NORMAL_W } },
};

static const Gfx omm_level_peachy_room_walls_tri[] = {
    gsSPVertex(omm_level_peachy_room_walls_vtx, 8, 0),
    gsSP2Triangles(2, 1, 0, 0, 1, 2, 3, 0),
    gsSP2Triangles(6, 5, 4, 0, 5, 6, 7, 0),
    gsSPEndDisplayList(),
};

static Vtx omm_level_peachy_room_painting_1_vtx[8 + (PAINTING_TILES + 1) * (PAINTING_TILES + 1)];

static Gfx omm_level_peachy_room_painting_1_tri[6 + (PAINTING_TILES + 1) * PAINTING_TILES] = { gsSPEndDisplayList() };

static Vtx omm_level_peachy_room_painting_2_vtx[8 + (PAINTING_TILES + 1) * (PAINTING_TILES + 1)];

static Gfx omm_level_peachy_room_painting_2_tri[6 + (PAINTING_TILES + 1) * PAINTING_TILES] = { gsSPEndDisplayList() };

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
    gsSPDisplayList(omm_level_peachy_room_carpet_tri),
    gsDPLoadTextureBlock(OMM_TEXTURE_PEACHY_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, TEX_SIZE, TEX_SIZE, 0, 0, G_TX_CLAMP, 0, 0, 0, 0),
    gsSPDisplayList(omm_level_peachy_room_walls_tri),
    gsDPLoadTextureBlock(OMM_TEXTURE_PEACHY_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, TEX_SIZE, TEX_SIZE, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0),
    gsSPDisplayList(omm_level_peachy_room_painting_1_tri),
    gsDPLoadTextureBlock(OMM_TEXTURE_PEACHY_3, G_IM_FMT_RGBA, G_IM_SIZ_32b, TEX_SIZE, TEX_SIZE, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0),
    gsSPDisplayList(omm_level_peachy_room_painting_2_tri),
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

    // Vertices
    COL_VERTEX_INIT(24),

    // Carpet
    COL_VERTEX(N, N, N * ROOM_LENGTH - 200),
    COL_VERTEX(P, N, N * ROOM_LENGTH - 200),
    COL_VERTEX(N, N, P * ROOM_LENGTH + 200),
    COL_VERTEX(P, N, P * ROOM_LENGTH + 200),

    // Ceiling
    COL_VERTEX(N, P, N * ROOM_LENGTH),
    COL_VERTEX(P, P, N * ROOM_LENGTH),
    COL_VERTEX(N, P, P * ROOM_LENGTH),
    COL_VERTEX(P, P, P * ROOM_LENGTH),

    // Walls
    COL_VERTEX(N, P, P * ROOM_LENGTH),
    COL_VERTEX(N, P, N * ROOM_LENGTH),
    COL_VERTEX(N, N, P * ROOM_LENGTH),
    COL_VERTEX(N, N, N * ROOM_LENGTH),
    COL_VERTEX(P, P, N * ROOM_LENGTH),
    COL_VERTEX(P, P, P * ROOM_LENGTH),
    COL_VERTEX(P, N, N * ROOM_LENGTH),
    COL_VERTEX(P, N, P * ROOM_LENGTH),

    // Painting 1
    COL_VERTEX(PAINTING_X(0), PAINTING_Y(0), P * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(0), PAINTING_Y(1), P * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(1), PAINTING_Y(1), P * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(1), PAINTING_Y(0), P * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(PAINTING_LEFT),  PAINTING_Y(PAINTING_DOWN), P * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(PAINTING_LEFT),  PAINTING_Y(PAINTING_UP),   P * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(PAINTING_RIGHT), PAINTING_Y(PAINTING_UP),   P * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(PAINTING_RIGHT), PAINTING_Y(PAINTING_DOWN), P * ROOM_LENGTH),

    // Painting 2
    COL_VERTEX(PAINTING_X(0), PAINTING_Y(0), N * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(0), PAINTING_Y(1), N * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(1), PAINTING_Y(1), N * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(1), PAINTING_Y(0), N * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(PAINTING_LEFT),  PAINTING_Y(PAINTING_DOWN), N * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(PAINTING_LEFT),  PAINTING_Y(PAINTING_UP),   N * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(PAINTING_RIGHT), PAINTING_Y(PAINTING_UP),   N * ROOM_LENGTH),
    COL_VERTEX(PAINTING_X(PAINTING_RIGHT), PAINTING_Y(PAINTING_DOWN), N * ROOM_LENGTH),

    // Triangles
    COL_TRI_INIT(SURFACE_DEFAULT, 16),

    // Carpet
    COL_TRI(2, 1, 0),
    COL_TRI(1, 2, 3),

    // Ceiling
    COL_TRI(4, 5, 6),
    COL_TRI(7, 6, 5),

    // Walls
    COL_TRI(10, 9, 8),
    COL_TRI(9, 10, 11),
    COL_TRI(14, 13, 12),
    COL_TRI(13, 14, 15),

    // Painting 1
    COL_TRI(16, 21, 20),
    COL_TRI(16, 17, 21),
    COL_TRI(17, 22, 21),
    COL_TRI(17, 18, 22),
    COL_TRI(18, 23, 22),
    COL_TRI(18, 19, 23),
    COL_TRI(19, 20, 23),
    COL_TRI(19, 16, 20),

    // Painting 2
    COL_TRI(29, 24, 28),
    COL_TRI(25, 24, 29),
    COL_TRI(30, 25, 29),
    COL_TRI(26, 25, 30),
    COL_TRI(31, 26, 30),
    COL_TRI(27, 26, 31),
    COL_TRI(28, 27, 31),
    COL_TRI(24, 27, 28),
    COL_TRI_STOP(),
    COL_END(),
};

//
// Cutscenes
//

static void cutscene_enter_peachy_painting(struct Camera *c) {
    struct Object *o = obj_get_first_with_behavior(bhvOmmLevelPeachyRoom);
    if (o) {
        set_fov_function(CAM_FOV_APP_20);
        sStatusFlags |= CAM_FLAG_SMOOTH_MOVEMENT;
        Vec3s paintingAngle; vec3s_set(paintingAngle, 0, sign_f(o->oHomeZ) * 0x8000, 0);
        Vec3f focusOffset; vec3f_zero(focusOffset);
        Vec3f paintingPos; vec3f_copy(paintingPos, &o->oHomeX);
        Vec3f focus;
        offset_rotated(focus, paintingPos, focusOffset, paintingAngle);
        approach_vec3f_asymptotic(c->focus, focus, 0.1f, 0.1f, 0.1f);
        focusOffset[2] = -sign_f(o->oHomeZ) * 3000;
        offset_rotated(focus, paintingPos, focusOffset, paintingAngle);
        approach_vec3f_asymptotic(c->pos, focus, 0.2f, 0.1f, 0.2f);
        c->mode = CAMERA_MODE_CLOSE;
    } else {
        c->cutscene = 0;
        gCutsceneTimer = CUTSCENE_STOP;
        sStatusFlags |= CAM_FLAG_SMOOTH_MOVEMENT;
    }
}

static void cutscene_enter_paintings(struct Camera *c) {
    if (obj_get_first_with_behavior(bhvOmmLevelPeachyRoom)) {
        cutscene_enter_peachy_painting(c);
    } else {
        cutscene_enter_painting(c);
    }
}

//
// Behavior script
//

static void bhv_omm_level_peachy_room_update_painting_vtx(struct Object *o, Vtx *vtx, f32 z0) {

    // Wall
    *(vtx++) = (Vtx) { { { PAINTING_X(0), PAINTING_Y(0), z0 }, 0, { PAINTING_U(0), PAINTING_V(0) }, PAINTING_N(z0) } };
    *(vtx++) = (Vtx) { { { PAINTING_X(0), PAINTING_Y(1), z0 }, 0, { PAINTING_U(0), PAINTING_V(1) }, PAINTING_N(z0) } };
    *(vtx++) = (Vtx) { { { PAINTING_X(1), PAINTING_Y(1), z0 }, 0, { PAINTING_U(1), PAINTING_V(1) }, PAINTING_N(z0) } };
    *(vtx++) = (Vtx) { { { PAINTING_X(1), PAINTING_Y(0), z0 }, 0, { PAINTING_U(1), PAINTING_V(0) }, PAINTING_N(z0) } };
    *(vtx++) = (Vtx) { { { PAINTING_X(PAINTING_LEFT),  PAINTING_Y(PAINTING_DOWN), z0 }, 0, { PAINTING_U(PAINTING_LEFT),  PAINTING_V(PAINTING_DOWN) }, PAINTING_N(z0) } };
    *(vtx++) = (Vtx) { { { PAINTING_X(PAINTING_LEFT),  PAINTING_Y(PAINTING_UP),   z0 }, 0, { PAINTING_U(PAINTING_LEFT),  PAINTING_V(PAINTING_UP)   }, PAINTING_N(z0) } };
    *(vtx++) = (Vtx) { { { PAINTING_X(PAINTING_RIGHT), PAINTING_Y(PAINTING_UP),   z0 }, 0, { PAINTING_U(PAINTING_RIGHT), PAINTING_V(PAINTING_UP)   }, PAINTING_N(z0) } };
    *(vtx++) = (Vtx) { { { PAINTING_X(PAINTING_RIGHT), PAINTING_Y(PAINTING_DOWN), z0 }, 0, { PAINTING_U(PAINTING_RIGHT), PAINTING_V(PAINTING_DOWN) }, PAINTING_N(z0) } };

    // Painting
    for (s32 j = 0; j <= PAINTING_TILES; ++j) {
        f32 y = PAINTING_DOWN + PAINTING_SIZE * (f32) j / (f32) PAINTING_TILES;
        for (s32 i = 0; i <= PAINTING_TILES; ++i) {
            f32 x = PAINTING_LEFT + PAINTING_SIZE * (f32) i / (f32) PAINTING_TILES;
            f32 px = PAINTING_X(x);
            f32 py = PAINTING_Y(y);
            f32 d = sqrtf(sqr_f(px - o->oVelX) + sqr_f(py - o->oVelY));
            f32 t = max_f(0, o->oAction - d / 50.f);
            f32 z = z0 + sign_f(z0) * relerp_0_1_f(t, 30, 60, 40, 0) * sins(0x2000 * t);
            if (i == 0 || j == 0 || i == PAINTING_TILES || j == PAINTING_TILES) z = z0;
            *(vtx++) = (Vtx) { { { px, py, z }, 0, { PAINTING_U(x), PAINTING_V(y) }, PAINTING_N(z0) } };
        }
    }
}

static void bhv_omm_level_peachy_room_update_painting_tri(Gfx *tri, Vtx *vtx, bool invert) {
    typedef struct { Vec3f n; Vtx *v; } VtxNormal;
    typedef struct { Vec3f n; s32 v0, v1, v2; } TriNormal;

    // Wall
    gSPVertex(tri++, vtx, 8, 0);
    if (invert) {
        gSP2Triangles(tri++, 5, 0, 4, 0, 1, 0, 5, 0);
        gSP2Triangles(tri++, 6, 1, 5, 0, 2, 1, 6, 0);
        gSP2Triangles(tri++, 7, 2, 6, 0, 3, 2, 7, 0);
        gSP2Triangles(tri++, 4, 3, 7, 0, 0, 3, 4, 0);
    } else {
        gSP2Triangles(tri++, 0, 5, 4, 0, 0, 1, 5, 0);
        gSP2Triangles(tri++, 1, 6, 5, 0, 1, 2, 6, 0);
        gSP2Triangles(tri++, 2, 7, 6, 0, 2, 3, 7, 0);
        gSP2Triangles(tri++, 3, 4, 7, 0, 3, 0, 4, 0);
    }
    vtx += 8;

    // Painting
    VtxNormal vtxNormals[(PAINTING_TILES + 1) * (PAINTING_TILES + 1)] = {0};
    TriNormal triNormals[PAINTING_TILES * PAINTING_TILES * 2] = {0};
    for (s32 i = 0, k = 0, v = 0; i != PAINTING_TILES; ++i, vtx += (PAINTING_TILES + 1), v += (PAINTING_TILES + 1)) {
        gSPVertex(tri++, vtx, 2 * (PAINTING_TILES + 1), 0);
        for (s32 j = 0; j != PAINTING_TILES; ++j, k += 2) {
            s32 v0 = j;
            s32 v1 = j + 1;
            s32 v2 = j + PAINTING_TILES + 1;
            s32 v3 = j + PAINTING_TILES + 2;
            if (invert) {
                gSP2Triangles(tri++, v0, v1, v2, 0, v3, v2, v1, 0);
            } else {
                gSP2Triangles(tri++, v2, v1, v0, 0, v1, v2, v3, 0);
            }

            // Compute triangle normals
            TriNormal *tn0 = triNormals + k;
            TriNormal *tn1 = triNormals + k + 1;
            VtxNormal *vn0 = vtxNormals + v + v0;
            VtxNormal *vn1 = vtxNormals + v + v1;
            VtxNormal *vn2 = vtxNormals + v + v2;
            VtxNormal *vn3 = vtxNormals + v + v3;
            vn0->v = vtx + v0;
            vn1->v = vtx + v1;
            vn2->v = vtx + v2;
            vn3->v = vtx + v3;
            if (invert) {
                vec3f_get_normal(tn0->n, vn0->v->n.ob, vn1->v->n.ob, vn2->v->n.ob);
                vec3f_get_normal(tn1->n, vn3->v->n.ob, vn2->v->n.ob, vn1->v->n.ob);
                tn0->v0 = v + v0; tn0->v1 = v + v1; tn0->v2 = v + v2;
                tn1->v0 = v + v3; tn1->v1 = v + v2; tn1->v2 = v + v1;
            } else {
                vec3f_get_normal(tn0->n, vn2->v->n.ob, vn1->v->n.ob, vn0->v->n.ob);
                vec3f_get_normal(tn1->n, vn1->v->n.ob, vn2->v->n.ob, vn3->v->n.ob);
                tn0->v0 = v + v2; tn0->v1 = v + v1; tn0->v2 = v + v0;
                tn1->v0 = v + v1; tn1->v1 = v + v2; tn1->v2 = v + v3;
            }
        }
    }
    gSPEndDisplayList(tri);

    // Recompute vertex normals
    for (s32 i = 0; i != array_length(triNormals); ++i) {
        TriNormal *tn = triNormals + i;
        VtxNormal *vn0 = vtxNormals + tn->v0;
        VtxNormal *vn1 = vtxNormals + tn->v1;
        VtxNormal *vn2 = vtxNormals + tn->v2;
        vec3f_add(vn0->n, tn->n);
        vec3f_add(vn1->n, tn->n);
        vec3f_add(vn2->n, tn->n);
    }
    for (s32 i = 0; i != array_length(vtxNormals); ++i) {
        VtxNormal *vn = vtxNormals + i;
        vec3f_normalize(vn->n);
        vn->v->n.n[0] = (s8) (127.f * vn->n[0]);
        vn->v->n.n[1] = (s8) (127.f * vn->n[1]);
        vn->v->n.n[2] = (s8) (127.f * vn->n[2]);
    }
}

static void bhv_omm_level_peachy_room_update_paintings(struct Object *o) {
    bhv_omm_level_peachy_room_update_painting_vtx(o, omm_level_peachy_room_painting_1_vtx, P * ROOM_LENGTH);
    bhv_omm_level_peachy_room_update_painting_tri(omm_level_peachy_room_painting_1_tri, omm_level_peachy_room_painting_1_vtx, false);
    bhv_omm_level_peachy_room_update_painting_vtx(o, omm_level_peachy_room_painting_2_vtx, N * ROOM_LENGTH);
    bhv_omm_level_peachy_room_update_painting_tri(omm_level_peachy_room_painting_2_tri, omm_level_peachy_room_painting_2_vtx, true);
}

static void bhv_omm_level_peachy_room_update() {
    omm_secrets_unlock(OMM_GAME_IS_SM74 ? OMM_SECRET_SM74_SECRET : OMM_SECRET_PEACHY_ROOM);

    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;

    // Hide the HUD
    gHudDisplay.flags = HUD_DISPLAY_FLAG_EFFECTS;

    // Update paintings
    bhv_omm_level_peachy_room_update_paintings(o);

    // If the cutscene is playing, advance the ripple effect timer
    if (o->oAction > 0) {
        o->oAction++;
        return;
    }

    // "Infinite" corridor
    if (!OMM_GAME_IS_SM64 || true /* not all nebulas */) {
        f32 dz = (m->pos[2] < 0 || o->oPosZ > 0) * -min_f(m->pos[2], o->oPosZ);
        if (dz != 0) {
            o->oPosZ += dz;
            m->pos[2] += dz;
            m->marioObj->oPosZ += dz;
            m->marioObj->oGfxPos[2] += dz;
            geo_preprocess_object_graph_node(m->marioObj);
            for_each_object_in_all_lists(obj) {
                if (obj->activeFlags &&
                    obj != m->marioObj &&
                    obj->behavior != bhvOmmLevelPeachyRoom &&
                    obj->behavior != bhvOmmPerryTrail
                ) {
                    obj->oPosZ += dz;
                    obj->oGfxPos[2] += dz;
                    if (obj->behavior != bhvOmmCappy) {
                        s32 zLimit = P * (ROOM_LENGTH + 1);
                        obj_set_dormant(obj, !(-zLimit < obj->oPosZ && obj->oPosZ < +zLimit));
                    }
                }
            }
        }
        omm_level_peachy_room_curr_z = o->oPosZ;
    }

    // If Mario enters a painting, start the cutscene
    if (abs_f(m->pos[2]) >= P * ROOM_LENGTH + 50) {
        bool exit2 = (m->pos[2] < 0);
        o->oAction = 1;
        o->oVelX = m->pos[0];
        o->oVelY = m->pos[1] + 30;
        o->oHomeX = 0;
        o->oHomeY = PAINTING_Y((PAINTING_UP + PAINTING_DOWN) / 2);
        o->oHomeZ = (exit2 ? N : P) * ROOM_LENGTH;
        mem_cpy(sCutsceneEnterPainting, __EXPAND(array_of(struct Cutscene) { { cutscene_enter_paintings, CUTSCENE_LOOP } }), sizeof(struct Cutscene));
        start_cutscene(gCamera, CUTSCENE_ENTER_PAINTING);
        if (exit2 && OMM_GAME_IS_SM64) initiate_warp(OMM_LEVEL_RETURN_TO_CASTLE, 0 /* warp to nebula stars ending */);
        else initiate_warp(OMM_LEVEL_RETURN_TO_CASTLE, 0);
        play_transition_after_delay(WARP_TRANSITION_FADE_INTO_COLOR, 30, 0xFF, 0xFF, 0xFF, 45);
        level_set_transition(74, basic_update);
        omm_mario_unpossess_object_before_warp(m);
        omm_mario_set_action(m, ACT_OMM_WARPING, 0, 0xFFFF);
        m->marioObj->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
        return;
    }

    // Update the signpost text
    struct Object *signpost = obj_get_first_with_behavior(bhvMessagePanel);
    if (signpost) {
#if OMM_GAME_IS_SM74
        s16 dialogId = OMM_DIALOG_SM74_ROOM_2;
#else
        s16 dialogId = (false /* all nebulas */ ? OMM_DIALOG_PEACHY_ROOM_FINAL : OMM_DIALOG_PEACHY_ROOM);
#endif
        signpost->oBehParams = dialogId << 16;
        signpost->oBehParams2ndByte = dialogId;
    }

    // Update the credits text
    struct Object *credits = obj_get_first_with_behavior(bhvSignOnWall);
    if (credits) {
#if OMM_GAME_IS_SM74
        s16 dialogId = OMM_DIALOG_SM74_ROOM_1;
#else
        s16 dialogId = OMM_DIALOG_CREDITS;
#endif
        credits->oBehParams = dialogId << 16;
        credits->oBehParams2ndByte = dialogId;
    }

#if OMM_GAME_IS_SM64
    // Trigger an automatic dialog if entering through warp 0x0B
    if (!o->oBehParams2ndByte && sWarpDest.nodeId == 0x0B && !omm_is_transition_active() && m->action != ACT_TELEPORT_FADE_IN) {
        omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, OMM_DIALOG_PEACHY_ROOM_FINAL, 0);
        audio_play_puzzle_jingle();
        o->oBehParams2ndByte = 1;
    }
#endif
}

static const BehaviorScript bhvOmmLevelPeachyRoom[] = {
    OBJ_TYPE_DEFAULT,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_level_peachy_room_update),
    BHV_END_LOOP(),
};

OMM_ROUTINE_UPDATE(bhv_omm_level_peachy_room_update_prev_z) {
    omm_level_peachy_room_prev_z = omm_level_peachy_room_curr_z;
}

//
// Sign on wall
//

static const Vtx sign_on_wall_vtx[] = {
    { { { -75, 200, 1 }, 0, { 990,   0 }, { 0x00, 0x00, 0x7F, 0xFF } } },
    { { {  75, 200, 1 }, 0, {   0,   0 }, { 0x00, 0x00, 0x7F, 0xFF } } },
    { { {  75,  50, 1 }, 0, {   0, 990 }, { 0x00, 0x00, 0x7F, 0xFF } } },
    { { { -75,  50, 1 }, 0, { 990, 990 }, { 0x00, 0x00, 0x7F, 0xFF } } },
};

static const Lights1 sign_on_wall_lights = gdSPDefLights1(
    0x5f, 0x5f, 0x5f,
    0xff, 0xff, 0xff,
    0x28, 0x28, 0x28
);

static const Gfx sign_on_wall_gfx[] = {
    gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA),
    gsSPClearGeometryMode(G_SHADING_SMOOTH | G_CULL_BOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock("levels/castle_inside/4.rgba16", G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0),
    gsSPLight(&sign_on_wall_lights.l, 1),
    gsSPLight(&sign_on_wall_lights.a, 2),
    gsSPVertex(sign_on_wall_vtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 2, 3, 0x0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const GeoLayout sign_on_wall_geo[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_DISPLAY_LIST(LAYER_ALPHA, sign_on_wall_gfx),
    GEO_CLOSE_NODE(),
    GEO_RETURN(),
};

//
// Objects
//

const LevelScript omm_level_peachy_room_objects[] = {
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_04, sign_on_wall_geo),
    OBJECT(MODEL_NONE, 0, 0, 0, 0, 0, 0, 0, bhvOmmLevelPeachyRoom),
    OBJECT(MODEL_NONE, 0, N / 2, (s16) (P * (ROOM_LENGTH - 1.f)), 0, 0, 0, 0x000A0000, bhvFadingWarp),
#if OMM_GAME_IS_SM64
    OBJECT(MODEL_NONE, 0, N / 2, (s16) (P * (ROOM_LENGTH - 2.f)), 0, 180, 0, 0x000B0000, bhvFadingWarp),
#endif
    OBJECT(MODEL_WOODEN_SIGNPOST, 0, N, (s16) (P * (ROOM_LENGTH - 0.5f)), 0, 180, 0, 0, bhvMessagePanel),
    OBJECT(MODEL_LEVEL_GEOMETRY_04, 0, N, (s16) (P * ROOM_LENGTH), 0, 180, 0, 0, bhvSignOnWall),
    RETURN(),
};

//
// Script
//

#if OMM_GAME_IS_SM74

static const LevelScript omm_level_sm74_room[] = {
    END_AREA(),
    AREA(SM74_MODE_EXTREME, omm_level_peachy_room_geo),
        WARP_NODE(0x0A, LEVEL_ENDING, SM74_MODE_EXTREME, 0x0A, WARP_NO_CHECKPOINT),
        WARP_NODE(0x10, LEVEL_CASTLE_COURTYARD, SM74_MODE_EXTREME, 0x40, 0),
        WARP_NODE(WARP_NODE_DEATH, LEVEL_CASTLE_COURTYARD, SM74_MODE_EXTREME, 0x40, 0),
        JUMP_LINK(omm_level_peachy_room_objects),
        TERRAIN(omm_level_peachy_room_collision),
        TERRAIN_TYPE(TERRAIN_STONE),
    END_AREA(),
    FREE_LEVEL_POOL(),
    MARIO_POS(SM74_MODE_EXTREME, 0, 0, 0, 0),
    CALL(0, lvl_init_or_update),
    CALL_LOOP(1, lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(1),
    EXIT(),
};

static const LevelScript omm_level_warp_to_sm74_room[] = {
    SET_BACKGROUND_MUSIC(0, 0x2E),
    OBJECT(MODEL_NONE, -1585, 1343, 5750, 0, 0, 0, 0x00770000, bhvFadingWarp),
    WARP_NODE(0x77, LEVEL_ENDING, SM74_MODE_EXTREME, 0x0A, WARP_NO_CHECKPOINT),
    RETURN(),
};

LEVEL_CMD_BRANCH(
    level_script_find(level_ending_entry, array_of(LevelScript) { END_AREA() }, 1),
    level_script_find(level_ending_entry, array_of(LevelScript) { FREE_LEVEL_POOL() }, 1),
    omm_level_sm74_room
);

LEVEL_CMD_BRANCH(
    level_script_find(local_area_bits_2_, array_of(LevelScript) { SET_BACKGROUND_MUSIC(0, 0x2E) }, 1),
    level_script_find(local_area_bits_2_, array_of(LevelScript) { TERRAIN_TYPE(0) }, 1),
    omm_level_warp_to_sm74_room
);

#else

static const LevelScript omm_level_peachy_room[] = {
    END_AREA(),
    AREA(4, omm_level_peachy_room_geo),
        WARP_NODE(0x0A, LEVEL_CASTLE_COURTYARD, 4, 0x0A, WARP_NO_CHECKPOINT),
        WARP_NODE(0x0B, LEVEL_CASTLE_COURTYARD, 4, 0x0B, WARP_NO_CHECKPOINT),
        UNPACK(WARP_NODE, 0x10, OMM_LEVEL_RETURN_TO_CASTLE, 0),
        UNPACK(WARP_NODE, WARP_NODE_DEATH, OMM_LEVEL_RETURN_TO_CASTLE, 0),
        JUMP_LINK(omm_level_peachy_room_objects),
        TERRAIN(omm_level_peachy_room_collision),
        TERRAIN_TYPE(TERRAIN_STONE),
    END_AREA(),
    RETURN(),
};

LEVEL_CMD_BRANCH(
    level_script_find(level_castle_courtyard_entry, array_of(LevelScript) { END_AREA() }, 1),
    level_script_find(level_castle_courtyard_entry, array_of(LevelScript) { FREE_LEVEL_POOL() }, 1),
    omm_level_peachy_room
);

//
// Room trigger
//

static struct { Vec3f pos; f32 radius; } OMM_PEACHY_ROOM_TRIGGERS[3] = {
    { {   374, -1387, -5020 }, 310 },
    { { -2488, -2866, -4968 }, 310 },
    { { -4741, -3057, -2985 }, 310 },
};

static const s32 OMM_PEACHY_ROOM_ALL_TRIGGERS = ((1 << array_length(OMM_PEACHY_ROOM_TRIGGERS)) - 1);

static const Vtx omm_peachy_room_trigger_vtx[] = {

    // Painting
    { { { -256, -256, 0 }, 0, { 230 << 5, 857 << 5 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { +256, -256, 0 }, 0, { 793 << 5, 857 << 5 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -256, +256, 0 }, 0, { 230 << 5, 294 << 5 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { +256, +256, 0 }, 0, { 793 << 5, 294 << 5 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },

    // Light
    { { { -256, -256,   0 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x5A, 0xFF } } },
    { { { +256, -256,   0 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x5A, 0xFF } } },
    { { { +256, +256,   0 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x5A, 0xFF } } },
    { { { -256, +256,   0 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x5A, 0xFF } } },
    { { { -256, -256, 100 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x5A, 0x00 } } },
    { { { +256, -256, 100 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x5A, 0x00 } } },
    { { { +256, +256, 100 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x5A, 0x00 } } },
    { { { -256, +256, 100 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x5A, 0x00 } } },
};

static const Gfx omm_peachy_room_trigger_tri[] = {

    // Painting
    gsSPVertex(omm_peachy_room_trigger_vtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 1, 3, 0),
    gsSPEndDisplayList(),

    // Light
    gsSPVertex(omm_peachy_room_trigger_vtx + 4, 8, 0),
    gsSP2Triangles(0, 1, 4, 0, 4, 1, 5, 0),
    gsSP2Triangles(1, 2, 5, 0, 5, 2, 6, 0),
    gsSP2Triangles(2, 3, 6, 0, 6, 3, 7, 0),
    gsSP2Triangles(3, 0, 7, 0, 7, 0, 4, 0),
    gsSPEndDisplayList(),
};

// Painting
static const Gfx omm_peachy_room_trigger_gfx_0[] = {
    gsSPClearGeometryMode(G_CULL_BOTH | G_LIGHTING),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPLoadTextureBlock(OMM_TEXTURE_PEACHY_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1024, 1024, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(omm_peachy_room_trigger_tri),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPEndDisplayList(),
};

// Light
static const Gfx omm_peachy_room_trigger_gfx_1[] = {
    gsSPClearGeometryMode(G_CULL_BOTH | G_LIGHTING),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPDisplayList(omm_peachy_room_trigger_tri + 3),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsSPEndDisplayList(),
};

static const GeoLayout omm_peachy_room_trigger_geo[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_peachy_room_trigger_gfx_0),
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peachy_room_trigger_gfx_1),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

static void bhv_omm_peachy_room_trigger_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;

    // Check triggers
    if (omm_mario_is_ground_pound_landing(m)) {
        for (u32 i = 0; i != array_length(OMM_PEACHY_ROOM_TRIGGERS); ++i) {
            if (!(o->oBehParams2ndByte & (1 << i))) {
                f32 y = OMM_PEACHY_ROOM_TRIGGERS[i].pos[1];
                if (y - 10 < m->pos[1] && m->pos[1] < y + 10) {
                    f32 hdist = vec3f_hdist(OMM_PEACHY_ROOM_TRIGGERS[i].pos, m->pos);
                    if (hdist < OMM_PEACHY_ROOM_TRIGGERS[i].radius) {
                        o->oBehParams2ndByte |= (1 << i);
                        if (o->oBehParams2ndByte == OMM_PEACHY_ROOM_ALL_TRIGGERS) {
                            audio_play_puzzle_jingle();
                        } else {
                            play_sound(SOUND_GENERAL2_RIGHT_ANSWER, gGlobalSoundArgs);
                        }
                        break;
                    }
                }
            }
        }
    }

    // Wall warp
    if (!obj_get_first_with_behavior_and_field_s32(bhvOmmWallWarp, _FIELD(oWallWarpKind), 2)) {
        omm_obj_spawn_wall_warp(
            o, 2, o->oPosX, o->oPosY, o->oPosZ, 256, 256, o->oFaceAngleYaw,
            LEVEL_CASTLE_COURTYARD, 4, 0x0A, 30,
            WARP_TRANSITION_FADE_INTO_CIRCLE, 20, 0x000000,
            1.f, 1.f, 1.f, omm_peachy_room_trigger_geo, OMM_SPARKLY_MODE_IS_LUNATIC ? omm_geo_sparkly_star_3_sparkle : NULL
        );
    }
}

#define OMM_PEACHY_ROOM_TRIGGER_DEFINED
const BehaviorScript bhvOmmPeachyRoomTrigger[] = {
    OBJ_TYPE_DEFAULT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_SET_INT(oBehParams2ndByte, 0),
    BHV_SCALE(0, 100),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_peachy_room_trigger_update),
    BHV_END_LOOP(),
};

#endif // OMM_GAME_IS_SM64
#endif // OMM_GAME_IS_SM64 || OMM_GAME_IS_SM74

#ifndef OMM_PEACHY_ROOM_TRIGGER_DEFINED

const BehaviorScript bhvOmmPeachyRoomTrigger[] = {
    OBJ_TYPE_DEFAULT,
    BHV_BREAK(),
};

#endif // OMM_PEACHY_ROOM_TRIGGER_DEFINED

#endif // GFX_PC_C

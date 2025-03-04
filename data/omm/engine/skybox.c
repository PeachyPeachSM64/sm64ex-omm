#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define SKYBOX_WIDTH        (4 * SCREEN_WIDTH)
#define SKYBOX_HEIGHT       (4 * SCREEN_HEIGHT)
#define SKYBOX_TILE_WIDTH   (SCREEN_WIDTH / 2)
#define SKYBOX_TILE_HEIGHT  (SCREEN_HEIGHT / 2)
#define SKYBOX_COLS         (10)
#define SKYBOX_IMAGE_SIZE   (248)
#define SKYBOX_FOV          (90.f)

static const char *SKYBOX_TEXTURES[] = {
    "textures/skyboxes/water",
    "textures/skyboxes/bitfs",
    "textures/skyboxes/wdw",
    "textures/skyboxes/cloud_floor",
    "textures/skyboxes/ccm",
    "textures/skyboxes/ssl",
    "textures/skyboxes/bbh",
    "textures/skyboxes/bidw",
    "textures/skyboxes/clouds",
    "textures/skyboxes/bits",
#if OMM_HAS_CUSTOM_SKYBOXES
#define MIO0_SEG(skybox, _2) "textures/skyboxes/" #skybox,
#include "textures/skyboxes/Skybox_Rules.ld"
#undef MIO0_SEG
#endif
};

static const u8 SKYBOX_COLORS[][3] = {
    { 0x50, 0x64, 0x5A },
    { 0xFF, 0xFF, 0xFF },
};

static const Mtx MTX_IDENTITY = {{
    { 1.f, 0.f, 0.f, 0.f },
    { 0.f, 1.f, 0.f, 0.f },
    { 0.f, 0.f, 1.f, 0.f },
    { 0.f, 0.f, 0.f, 1.f },
}};

static f32 calculate_skybox_scaled_x(u16 yaw) {
    return fmodf(SKYBOX_WIDTH - (SCREEN_WIDTH * 360.f * yaw) / (SKYBOX_FOV * 65536.f), SKYBOX_WIDTH);
}

static f32 calculate_skybox_scaled_y(s16 pitch) {
    return clamp_f((360.f * 360.f * pitch) / (SKYBOX_FOV * 65536.f) + 5 * SKYBOX_TILE_HEIGHT, SCREEN_HEIGHT + 1, SKYBOX_HEIGHT);
}

static s32 get_top_left_tile_idx(f32 scaledX, f32 scaledY) {
    s32 tileCol = scaledX / SKYBOX_TILE_WIDTH; // 0 to 7
    s32 tileRow = (SKYBOX_HEIGHT - scaledY) / SKYBOX_TILE_HEIGHT; // 0 to 5
    return tileRow * SKYBOX_COLS + tileCol;
}

static Vtx *make_skybox_rect(s32 tileIndex, s32 colorIndex) {
    Vtx *verts = alloc_display_list(4 * sizeof(Vtx));
    if (verts != NULL) {
        s16 w = SKYBOX_IMAGE_SIZE / (SKYBOX_COLS - 2);
        s16 h = SKYBOX_IMAGE_SIZE / (SKYBOX_COLS - 2);
        s16 tx = ((tileIndex % SKYBOX_COLS) * w) % SKYBOX_IMAGE_SIZE;
        s16 ty = ((tileIndex / SKYBOX_COLS) * h) % SKYBOX_IMAGE_SIZE;
        s16 tw = SKYBOX_TILE_WIDTH + 5 * (tx == 0);
        s16 th = SKYBOX_TILE_HEIGHT;
        s16 x = (tileIndex % SKYBOX_COLS) * SKYBOX_TILE_WIDTH - 5 * (tx == 0);
        s16 y = SKYBOX_HEIGHT - (tileIndex / SKYBOX_COLS) * th;
        const u8 *color = SKYBOX_COLORS[colorIndex];
        make_vertex(verts, 0, x,      y,      -1,  tx      << 5,  ty      << 5, color[0], color[1], color[2], 0xFF);
        make_vertex(verts, 1, x,      y - th, -1,  tx      << 5, (ty + h) << 5, color[0], color[1], color[2], 0xFF);
        make_vertex(verts, 2, x + tw, y - th, -1, (tx + w) << 5, (ty + h) << 5, color[0], color[1], color[2], 0xFF);
        make_vertex(verts, 3, x + tw, y,      -1, (tx + w) << 5,  ty      << 5, color[0], color[1], color[2], 0xFF);
    }
    return verts;
}

static void draw_skybox_tile_grid(Gfx **dlist, const char *texture, s32 upperLeftTile, s32 colorIndex) {
    for (s32 row = 0; row < 3; row++)
    for (s32 col = 0; col < 3; col++) {
        s32 tileIndex = upperLeftTile + row * SKYBOX_COLS + col;
        Vtx *verts = make_skybox_rect(tileIndex, colorIndex);
        if (verts != NULL) {
            gLoadBlockTexture((*dlist)++, SKYBOX_IMAGE_SIZE, SKYBOX_IMAGE_SIZE, G_IM_FMT_RGBA, texture);
            gSPVertex((*dlist)++, verts, 4, 0);
            gSPDisplayList((*dlist)++, dl_draw_quad_verts_0123);
        }
    }
}

static void *create_skybox_ortho_matrix(f32 scaledX, f32 scaledY) {
    Mtx *mtx = alloc_display_list(sizeof(Mtx));
    if (mtx != NULL) {
        f32 left = scaledX;
        f32 right = scaledX + SCREEN_WIDTH;
        f32 bottom = scaledY - SCREEN_HEIGHT;
        f32 top = scaledY;
        f32 center = (scaledX + SCREEN_WIDTH / 2);
        f32 half_width = ((4.f / 3.f) / GFX_DIMENSIONS_ASPECT_RATIO) * (SCREEN_WIDTH / 2);
        if (half_width < SCREEN_WIDTH / 2) {
            left = center - half_width;
            right = center + half_width;
        }
        guOrtho(mtx, left, right, bottom, top, 0.f, 3.f, 1.f);
    }
    return mtx;
}

static Gfx *init_skybox_display_list(const char *texture, f32 scaledX, f32 scaledY, s32 upperLeftTile, s32 colorIndex) {
    Gfx *skybox = alloc_display_list(128 * sizeof(Gfx));
    Mtx *ortho = create_skybox_ortho_matrix(scaledX, scaledY);
    if (skybox != NULL && ortho != NULL) {
        Gfx *dlist = skybox;
        gSPDisplayList(dlist++, dl_skybox_begin);
        gSPMatrix(dlist++, ortho, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
        gSPMatrix(dlist++, &MTX_IDENTITY, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPTexture(dlist++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPSetTile(dlist++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD);
        gDPSetTileSize(dlist++, 0, 0, 0, (SKYBOX_IMAGE_SIZE - 1) << G_TEXTURE_IMAGE_FRAC, (SKYBOX_IMAGE_SIZE - 1) << G_TEXTURE_IMAGE_FRAC);
        draw_skybox_tile_grid(&dlist, texture, upperLeftTile, colorIndex);
        gSPDisplayList(dlist++, dl_skybox_end);
        gSPEndDisplayList(dlist);
    }
    return skybox;
}

Gfx *create_skybox_facing_camera(UNUSED s8 player, s8 background, UNUSED f32 fov, f32 posX, f32 posY, f32 posZ, f32 focX, f32 focY, f32 focZ) {
    f32 cameraFaceX = focX - posX;
    f32 cameraFaceY = focY - posY;
    f32 cameraFaceZ = focZ - posZ;
    s32 colorIndex = 1;

#if OMM_GAME_IS_SM64
    // If the first star is not collected in JRB, make the sky darker and slightly green
    if (background == 8 && !(save_file_get_star_flags(gCurrSaveFileNum - 1, COURSE_JRB - 1) & 1)) {
        colorIndex = 0;
    }
#endif

    u16 yaw = atan2s(cameraFaceZ, cameraFaceX);
    s16 pitch = atan2s(sqrtf(cameraFaceX * cameraFaceX + cameraFaceZ * cameraFaceZ), cameraFaceY);
    f32 scaledX = calculate_skybox_scaled_x(yaw);
    f32 scaledY = calculate_skybox_scaled_y(pitch);
    s32 upperLeftTile = get_top_left_tile_idx(scaledX, scaledY);
    return init_skybox_display_list(SKYBOX_TEXTURES[background], scaledX, scaledY, upperLeftTile, colorIndex);
}

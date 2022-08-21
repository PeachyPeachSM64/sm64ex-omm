#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Frame interpolation
//

static Gfx *sTransitionPos = NULL;
static Vtx sTransitionVtx0[8];
static Vtx sTransitionVtx1[8];
static Vtx sTransitionVtx[8];

void gfx_interpolate_frame_transition(f32 t) {
    if (sTransitionPos) {
        for (s32 i = 0; i != 8; ++i) {
            vtxv_interpolate((Vtx_t *) (sTransitionVtx + i), (Vtx_t *) (sTransitionVtx0 + i), (Vtx_t *) (sTransitionVtx1 + i), t);
        }
    }
}

void gfx_clear_frame_transition() {
    sTransitionPos = NULL;
}

//
// Utils
//

static s32 sTransitionColorFadeTimer = 0;
static s32 sTransitionTextureFadeTimer = 0;
static const void *sTransitionTextures[] = {
    texture_transition_star_half,
    texture_transition_circle_half,
    texture_transition_mario,
    texture_transition_bowser_half,
};

static bool set_and_reset_transition_fade_timer(s32 transTime) {
    sTransitionColorFadeTimer++;
    if (sTransitionColorFadeTimer == transTime) {
        sTransitionColorFadeTimer = 0;
        sTransitionTextureFadeTimer = 0;
        return true;
    }
    return false;
}

static u8 get_transition_color_fade_alpha(bool fadeOut, s32 fadeTimer, s32 transTime) {
    f32 t = clamp_f(fadeTimer / (f32) (transTime - 1), 0.f, 1.f);
    return (u8) ((fadeOut ? 1.f - t : t) * 255.9f);
}

static void vertex_transition_color(Vtx *vtx, struct WarpTransitionData *transData, u8 alpha) {
    u8 r = transData->red;
    u8 g = transData->green;
    u8 b = transData->blue;
    make_vertex(vtx, 0, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 0, -1, 0, 0, r, g, b, alpha);
    make_vertex(vtx, 1, GFX_DIMENSIONS_FROM_RIGHT_EDGE(0), 0, -1, 0, 0, r, g, b, alpha);
    make_vertex(vtx, 2, GFX_DIMENSIONS_FROM_RIGHT_EDGE(0), SCREEN_HEIGHT, -1, 0, 0, r, g, b, alpha);
    make_vertex(vtx, 3, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), SCREEN_HEIGHT, -1, 0, 0, r, g, b, alpha);
}

static bool render_fade_transition_color(s32 transTime, struct WarpTransitionData *transData, bool fadeOut) {

    // Current frame
    u8 alpha1 = get_transition_color_fade_alpha(fadeOut, sTransitionColorFadeTimer, transTime);
    vertex_transition_color(sTransitionVtx1, transData, alpha1);

    // Previous frame
    if (gFrameInterpolation) {
        sTransitionPos = gDisplayListHead;
        u8 alpha0 = get_transition_color_fade_alpha(fadeOut, sTransitionColorFadeTimer - 1, transTime);
        vertex_transition_color(sTransitionVtx0, transData, alpha0);
    } else {
        omm_copy(sTransitionVtx0, sTransitionVtx1, sizeof(sTransitionVtx1));
    }

    // Vertices
    omm_copy(sTransitionVtx, sTransitionVtx0, sizeof(sTransitionVtx0));

    // Display list
    gSPDisplayList(gDisplayListHead++, dl_proj_mtx_fullscreen);
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gSPVertex(gDisplayListHead++, sTransitionVtx, 4, 0);
    gSPDisplayList(gDisplayListHead++, dl_draw_quad_verts_0123);
    gSPDisplayList(gDisplayListHead++, dl_screen_transition_end);

    // Update fade timer
    return set_and_reset_transition_fade_timer(transTime);
}

static s16 calc_tex_transition_radius(s32 transTime, struct WarpTransitionData *transData, f32 t) {
    f32 texRadius = transData->endTexRadius - transData->startTexRadius;
    f32 radiusTime = (max_f(sTransitionColorFadeTimer - 1 + t, 0.f) * texRadius) / (f32) (transTime - 1);
    f32 result = transData->startTexRadius + radiusTime;
    return (s16) (result + 0.5f);
}

static f32 calc_tex_transition_time(s32 transTime, struct WarpTransitionData *transData) {
    f32 startX = transData->startTexX;
    f32 startY = transData->startTexY;
    f32 endX = transData->endTexX;
    f32 endY = transData->endTexY;
    f32 sqrtfXY = sqrtf(sqr_f(startX - endX) + sqr_f(startY - endY));
    f32 result = ((f32) sTransitionColorFadeTimer * sqrtfXY) / (f32) (transTime - 1);
    return result;
}

static u16 convert_tex_transition_angle_to_pos(struct WarpTransitionData *transData) {
    f32 x = transData->endTexX - transData->startTexX;
    f32 y = transData->endTexY - transData->startTexY;
    return atan2s(x, y);
}

static s16 center_tex_transition_x(struct WarpTransitionData *transData, f32 texTransTime, u16 texTransPos) {
    f32 x = transData->startTexX + coss(texTransPos) * texTransTime;
    return (s16) (x + 0.5f);
}

static s16 center_tex_transition_y(struct WarpTransitionData *transData, f32 texTransTime, u16 texTransPos) {
    f32 y = transData->startTexY + sins(texTransPos) * texTransTime;
    return (s16) (y + 0.5f);
}

static void make_tex_transition_vertex(Vtx *vtx, s32 n, struct WarpTransitionData *transData, s16 centerTransX, s16 centerTransY, s16 texRadius1, s16 texRadius2, s16 tx, s16 ty) {
    u8 r = transData->red;
    u8 g = transData->green;
    u8 b = transData->blue;
    u16 zeroTimer = sTransitionTextureFadeTimer;
    f32 centerX = texRadius1 * coss(zeroTimer) - texRadius2 * sins(zeroTimer) + centerTransX;
    f32 centerY = texRadius1 * sins(zeroTimer) + texRadius2 * coss(zeroTimer) + centerTransY;
    s16 x = round_float(centerX);
    s16 y = round_float(centerY);
    make_vertex(vtx, n, x, y, -1, tx * 32, ty * 32, r, g, b, 255);
}

static void load_tex_transition_vertex(Vtx *vtx, struct WarpTransitionData *transData, s16 centerTransX, s16 centerTransY, s16 texTransRadius, s8 transTexType) {
    switch (transTexType) {
        case TRANS_TYPE_MIRROR: {
            make_tex_transition_vertex(vtx, 0, transData, centerTransX, centerTransY, -texTransRadius, -texTransRadius, -31, 63);
            make_tex_transition_vertex(vtx, 1, transData, centerTransX, centerTransY, +texTransRadius, -texTransRadius,  31, 63);
            make_tex_transition_vertex(vtx, 2, transData, centerTransX, centerTransY, +texTransRadius, +texTransRadius,  31,  0);
            make_tex_transition_vertex(vtx, 3, transData, centerTransX, centerTransY, -texTransRadius, +texTransRadius, -31,  0);
        } break;
        case TRANS_TYPE_CLAMP: {
            make_tex_transition_vertex(vtx, 0, transData, centerTransX, centerTransY, -texTransRadius, -texTransRadius,  0, 63);
            make_tex_transition_vertex(vtx, 1, transData, centerTransX, centerTransY, +texTransRadius, -texTransRadius, 63, 63);
            make_tex_transition_vertex(vtx, 2, transData, centerTransX, centerTransY, +texTransRadius, +texTransRadius, 63,  0);
            make_tex_transition_vertex(vtx, 3, transData, centerTransX, centerTransY, -texTransRadius, +texTransRadius,  0,  0);
        } break;
    }
    make_tex_transition_vertex(vtx, 4, transData, centerTransX, centerTransY, -2000, -2000, 0, 0);
    make_tex_transition_vertex(vtx, 5, transData, centerTransX, centerTransY, +2000, -2000, 0, 0);
    make_tex_transition_vertex(vtx, 6, transData, centerTransX, centerTransY, +2000, +2000, 0, 0);
    make_tex_transition_vertex(vtx, 7, transData, centerTransX, centerTransY, -2000, +2000, 0, 0);
}

static bool render_textured_transition(s32 transTime, struct WarpTransitionData *transData, s32 texID, s32 transTexType) {
    f32 texTransTime = calc_tex_transition_time(transTime, transData);
    u16 texTransPos  = convert_tex_transition_angle_to_pos(transData);
    s16 centerTransX = center_tex_transition_x(transData, texTransTime, texTransPos);
    s16 centerTransY = center_tex_transition_y(transData, texTransTime, texTransPos);

    // Current frame
    s16 texTransRadius1 = calc_tex_transition_radius(transTime, transData, 1);
    load_tex_transition_vertex(sTransitionVtx1, transData, centerTransX, centerTransY, texTransRadius1, transTexType);

    // Previous frame
    if (gFrameInterpolation) {
        sTransitionPos = gDisplayListHead;
        s16 texTransRadius0 = calc_tex_transition_radius(transTime, transData, 0);
        load_tex_transition_vertex(sTransitionVtx0, transData, centerTransX, centerTransY, texTransRadius0, transTexType);
    } else {
        omm_copy(sTransitionVtx0, sTransitionVtx1, sizeof(sTransitionVtx1));
    }

    // Vertices
    omm_copy(sTransitionVtx, sTransitionVtx0, sizeof(sTransitionVtx0));

    // Display list
    gSPDisplayList(gDisplayListHead++, dl_proj_mtx_fullscreen)
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
    gSPVertex(gDisplayListHead++, sTransitionVtx, 8, 0);
    gSPDisplayList(gDisplayListHead++, dl_transition_draw_filled_region);
    gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    switch (transTexType) {
    case TRANS_TYPE_MIRROR: gDPLoadTextureBlock(gDisplayListHead++, sTransitionTextures[texID], G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0, G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR, 5, 6, G_TX_NOLOD, G_TX_NOLOD); break;
    case TRANS_TYPE_CLAMP:  gDPLoadTextureBlock(gDisplayListHead++, sTransitionTextures[texID], G_IM_FMT_IA, G_IM_SIZ_8b, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD); break;
    }
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPVertex(gDisplayListHead++, sTransitionVtx, 4, 0);
    gSPDisplayList(gDisplayListHead++, dl_draw_quad_verts_0123);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    gSPDisplayList(gDisplayListHead++, dl_screen_transition_end);

    // Update fade timer
    sTransitionTextureFadeTimer += transData->texTimer;
    return set_and_reset_transition_fade_timer(transTime);
}

//
// Screen transition
//

s32 render_screen_transition(UNUSED s8 fadeTimer, s8 transType, u8 transTime, struct WarpTransitionData *transData) {
    switch (transType) {
        case WARP_TRANSITION_FADE_FROM_COLOR:  return render_fade_transition_color(transTime, transData, true);
        case WARP_TRANSITION_FADE_INTO_COLOR:  return render_fade_transition_color(transTime, transData, false);
        case WARP_TRANSITION_FADE_FROM_STAR:   return render_textured_transition(transTime, transData, TEX_TRANS_STAR, TRANS_TYPE_MIRROR);
        case WARP_TRANSITION_FADE_INTO_STAR:   return render_textured_transition(transTime, transData, TEX_TRANS_STAR, TRANS_TYPE_MIRROR);
        case WARP_TRANSITION_FADE_FROM_CIRCLE: return render_textured_transition(transTime, transData, TEX_TRANS_CIRCLE, TRANS_TYPE_MIRROR);
        case WARP_TRANSITION_FADE_INTO_CIRCLE: return render_textured_transition(transTime, transData, TEX_TRANS_CIRCLE, TRANS_TYPE_MIRROR);
        case WARP_TRANSITION_FADE_FROM_MARIO:  return render_textured_transition(transTime, transData, TEX_TRANS_MARIO, TRANS_TYPE_CLAMP);
        case WARP_TRANSITION_FADE_INTO_MARIO:  return render_textured_transition(transTime, transData, TEX_TRANS_MARIO, TRANS_TYPE_CLAMP);
        case WARP_TRANSITION_FADE_FROM_BOWSER: return render_textured_transition(transTime, transData, TEX_TRANS_BOWSER, TRANS_TYPE_MIRROR);
        case WARP_TRANSITION_FADE_INTO_BOWSER: return render_textured_transition(transTime, transData, TEX_TRANS_BOWSER, TRANS_TYPE_MIRROR);
    }
    return true;
}

//
// Cannon circle
//

static Gfx *render_cannon_circle_base(void) {
    static Vtx sCannonCircleVtx[8];
    static Gfx sCannonCircleGfx[20];

    // Vertices
    Vtx *vtx = sCannonCircleVtx;
    make_vertex(vtx, 0, 0, 0, -1, -1152, 1824, 0, 0, 0, 255);
    make_vertex(vtx, 1, SCREEN_WIDTH, 0, -1, 1152, 1824, 0, 0, 0, 255);
    make_vertex(vtx, 2, SCREEN_WIDTH, SCREEN_HEIGHT, -1, 1152, 192, 0, 0, 0, 255);
    make_vertex(vtx, 3, 0, SCREEN_HEIGHT, -1, -1152, 192, 0, 0, 0, 255);
    make_vertex(vtx, 4, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 0, -1, 0, 0, 0, 0, 0, 255);
    make_vertex(vtx, 5, GFX_DIMENSIONS_FROM_RIGHT_EDGE(0), 0, -1, 0, 0, 0, 0, 0, 255);
    make_vertex(vtx, 6, GFX_DIMENSIONS_FROM_RIGHT_EDGE(0), SCREEN_HEIGHT, -1, 0, 0, 0, 0, 0, 255);
    make_vertex(vtx, 7, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), SCREEN_HEIGHT, -1, 0, 0, 0, 0, 0, 255);

    // Display list
    Gfx *gfx = sCannonCircleGfx;
    gSPDisplayList(gfx++, dl_proj_mtx_fullscreen);
    gDPSetCombineLERP(gfx++, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    gDPSetTextureFilter(gfx++, G_TF_BILERP);
    gDPLoadTextureBlock(gfx++, sTransitionTextures[TEX_TRANS_CIRCLE], G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0, G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR, 5, 6, G_TX_NOLOD, G_TX_NOLOD);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPVertex(gfx++, VIRTUAL_TO_PHYSICAL(vtx), 4, 0);
    gSPDisplayList(gfx++, dl_draw_quad_verts_0123);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetCombineLERP(gfx++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gSPVertex(gfx++, VIRTUAL_TO_PHYSICAL(vtx + 4), 4, 4);
    gSP2Triangles(gfx++, 4, 0, 3, 0, 4, 3, 7, 0);
    gSP2Triangles(gfx++, 1, 5, 6, 0, 1, 6, 2, 0);
    gSPDisplayList(gfx++, dl_screen_transition_end);
    gSPEndDisplayList(gfx);
    return sCannonCircleGfx;
}

Gfx *geo_cannon_circle_base(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx) {
    if (callContext == GEO_CONTEXT_RENDER && gCurrentArea && gCurrentArea->camera->mode == CAMERA_MODE_INSIDE_CANNON) {
        struct GraphNodeGenerated *graphNode = (struct GraphNodeGenerated *) node;
        graphNode->fnNode.node.flags = (graphNode->fnNode.node.flags & 0xFF) | 0x500;
        return render_cannon_circle_base();
    }
    return NULL;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_STAR_GLOW_RADIUS_IN         (32)
#define OMM_STAR_GLOW_RADIUS_OUT        (128)
#define OMM_STAR_GLOW_ALPHA_IN          (0xDC)
#define OMM_STAR_GLOW_ALPHA_OUT         (0x00)
#define OMM_STAR_GLOW_NUM_POINTS        (16)

#define OMM_STAR_NUM_RAYS               (6)
#define OMM_STAR_RAY_RADIUS_MIN         (150)
#define OMM_STAR_RAY_RADIUS_MAX         (240)
#define OMM_STAR_RAY_DELTA_ANGLE_MIN    (0x0200)
#define OMM_STAR_RAY_DELTA_ANGLE_MAX    (0x0800)
#define OMM_STAR_RAY_ALPHA_MIN          (0x80)
#define OMM_STAR_RAY_ALPHA_MAX          (0xFF)
#define OMM_STAR_RAY_DURATION_MIN       (24)
#define OMM_STAR_RAY_DURATION_MAX       (48)

//
// Gfx data
//

static const Lights1 omm_star_light = gdSPDefLights1(
    0x7F, 0x7F, 0x7F,
    0xFF, 0xFF, 0xFF,
    0x28, 0x28, 0x28
);

//
// Body
//

static const Vtx omm_star_body_vertices[] = {
    { { { 0.000000, 0.000000, 64.000000 }, 0, { 0, 0 }, { 0, 0, 127, 0xFF } } },
    { { { 0.000000, 0.000000, -64.000000 }, 0, { 0, 0 }, { 0, 0, -127, 0xFF } } },
    { { { 0.000000, 256.000000, 0.000000 }, 0, { 0, 0 }, { 0, 127, 0, 0xFF } } },
    { { { 243.446182, 79.183044, 0.000000 }, 0, { 0, 0 }, { 120, 39, 0, 0xFF } } },
    { { { 150.600082, -207.015976, 0.000000 }, 0, { 0, 0 }, { 74, -102, 0, 0xFF } } },
    { { { -150.282333, -207.246750, 0.000000 }, 0, { 0, 0 }, { -74, -102, 0, 0xFF } } },
    { { { -243.567368, 78.809509, 0.000000 }, 0, { 0, 0 }, { -120, 39, 0, 0xFF } } },
    { { { 75.141167, 103.623375, 0.000000 }, 0, { 0, 0 }, { 74, 102, 0, 0xFF } } },
    { { { 121.783684, -39.404755, 0.000000 }, 0, { 0, 0 }, { 120, -39, 0, 0xFF } } },
    { { { 0.000000, -128.000000, 0.000000 }, 0, { 0, 0 }, { 0, -127, 0, 0xFF } } },
    { { { -121.723091, -39.591522, 0.000000 }, 0, { 0, 0 }, { -120, -39, 0, 0xFF } } },
    { { { -75.300041, 103.507988, 0.000000 }, 0, { 0, 0 }, { -74, 102, 0, 0xFF } } },
};

static const Gfx omm_star_body_triangles[] = {
    gsSPVertex(omm_star_body_vertices, 12, 0),
    gsSP2Triangles(0, 7, 2, 0, 0, 3, 7, 0),
    gsSP2Triangles(0, 8, 3, 0, 0, 4, 8, 0),
    gsSP2Triangles(0, 9, 4, 0, 0, 5, 9, 0),
    gsSP2Triangles(0, 10, 5, 0, 0, 6, 10, 0),
    gsSP2Triangles(0, 11, 6, 0, 0, 2, 11, 0),
    gsSP2Triangles(1, 2, 7, 0, 1, 7, 3, 0),
    gsSP2Triangles(1, 3, 8, 0, 1, 8, 4, 0),
    gsSP2Triangles(1, 4, 9, 0, 1, 9, 5, 0),
    gsSP2Triangles(1, 5, 10, 0, 1, 10, 6, 0),
    gsSP2Triangles(1, 6, 11, 0, 1, 11, 2, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_opaque_gfx[] = {
    gsSPSetGeometryMode(G_TEXTURE_GEN),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0),
    gsSPTexture(0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_star_light.l, 1),
    gsSPLight(&omm_star_light.a, 2),
    gsSPDisplayList(omm_star_body_triangles),
    gsSPTexture(0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_body_transparent_gfx[] = {
    gsSPSetGeometryMode(G_TEXTURE_GEN),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0x80),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0),
    gsSPTexture(0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_star_light.l, 1),
    gsSPLight(&omm_star_light.a, 2),
    gsSPDisplayList(omm_star_body_triangles),
    gsSPTexture(0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

#define omm_star_body_texture_gfx(x) \
static const Gfx omm_star_body_##x##_texture_gfx[] = { \
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_BODY_##x, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0), \
    gsSPEndDisplayList(), \
}
omm_star_body_texture_gfx(0);
omm_star_body_texture_gfx(1);
omm_star_body_texture_gfx(2);
omm_star_body_texture_gfx(3);
omm_star_body_texture_gfx(4);
omm_star_body_texture_gfx(5);
omm_star_body_texture_gfx(6);
omm_star_body_texture_gfx(7);
omm_star_body_texture_gfx(8);
omm_star_body_texture_gfx(9);
omm_star_body_texture_gfx(10);
omm_star_body_texture_gfx(11);
omm_star_body_texture_gfx(12);
omm_star_body_texture_gfx(13);
omm_star_body_texture_gfx(14);
omm_star_body_texture_gfx(15);
omm_star_body_texture_gfx(16);
omm_star_body_texture_gfx(17);
omm_star_body_texture_gfx(18);
omm_star_body_texture_gfx(19);

//
// Eyes
//

static const Vtx omm_star_eyes_vertices[] = {
    { { { 0, 0, 64 }, 0, { 4300, 2867 }, { 0, 0, 127, 0xFF } } },
    { { { 0, 128, 32 }, 0, { 4198, -1536 }, { 0, 127, 0, 0xFF } } },
    { { { 75.1411665, 103.623375, 32 }, 0, { 204.5, -614.5 }, { 74, 102, 0, 0xFF } } },
    { { { 121.723091, 39.591522, 32 }, 0, { -2560, 1535.5 }, { 120, 39, 0, 0xFF } } },
    { { { 121.783684, -39.4047545, 32 }, 0, { -2560, 4198 }, { 120, -39, 0, 0xFF } } },
    { { { 75.300041, -103.507988, 32 }, 0, { 204.5, 6348.5 }, { 74, -102, 0, 0xFF } } },
    { { { 0, -128, 32 }, 0, { 4300, 7270 }, { 0, -127, 0, 0xFF } } },
    { { { 0, 128, 32 }, 0, { 4198, -1536 }, { 0, 127, 0, 0xFF } } },
    { { { -75.300041, 103.507988, 32 }, 0, { 204.5, -614.5 }, { -74, 102, 0, 0xFF } } },
    { { { -121.783684, 39.4047545, 32 }, 0, { -2560, 1535.5 }, { -120, 39, 0, 0xFF } } },
    { { { -121.723091, -39.591522, 32 }, 0, { -2560, 4198 }, { -120, -39, 0, 0xFF } } },
    { { { -75.1411665, -103.623375, 32 }, 0, { 204.5, 6348.5 }, { -74, -102, 0, 0xFF } } },
    { { { 0, -128, 32 }, 0, { 4300, 7270 }, { 0, -127, 0, 0xFF } } },

};

static const Gfx omm_star_eyes_triangles[] = {
    gsSPVertex(omm_star_eyes_vertices, 13, 0),
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSP2Triangles(0, 4, 3, 0, 0, 5, 4, 0),
    gsSP2Triangles(0, 6, 5, 0, 0, 7, 8, 0),
    gsSP2Triangles(0, 8, 9, 0, 0, 9, 10, 0),
    gsSP2Triangles(0, 10, 11, 0, 0, 11, 12, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_eyes_opaque_gfx[] = {
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_EYE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 2, 2, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_star_light.l, 1),
    gsSPLight(&omm_star_light.a, 2),
    gsSPDisplayList(omm_star_eyes_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_star_eyes_transparent_gfx[] = {
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0x80),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_STAR_EYE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 2, 2, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPLight(&omm_star_light.l, 1),
    gsSPLight(&omm_star_light.a, 2),
    gsSPDisplayList(omm_star_eyes_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Glow
//

static const Gfx omm_star_glow_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPDisplayList(NULL),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

//
// Rays
//

static const Gfx omm_star_rays_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPDisplayList(NULL),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    Gfx *displayLists[2];
    struct {
        Gfx gfx[array_length(omm_star_glow_gfx)];
        Gfx tri[(OMM_STAR_GLOW_NUM_POINTS / 2) * 4 + 1];
        Vtx vtx[(OMM_STAR_GLOW_NUM_POINTS / 2) * 7];
    } glow;
    struct {
        Gfx gfx[array_length(omm_star_rays_gfx)];
        Gfx tri[OMM_STAR_NUM_RAYS * 2 + 1];
        Vtx vtx[OMM_STAR_NUM_RAYS * 3];
        struct {
            s32 radius;
            s32 angle;
            s32 delta;
            s32 alpha;
            s32 duration;
            s32 timer;
        } ray[OMM_STAR_NUM_RAYS];
    } rays;
    struct {
        const char *texture;
        u8 r, g, b;
    } star;
} OmmStarGeoData;

static const u32 sOmmStarGeoDataDisplayListsOffsets[] = {
    offsetof(OmmStarGeoData, glow.gfx),
    offsetof(OmmStarGeoData, rays.gfx),
};

//
// Update
//

static void omm_geo_star_compute_color(OmmStarGeoData *data, const char *texture) {
    static OmmMap sOmmStarTexColors = omm_map_zero;

    // Retrieve the color from the texture
    omm_map_for_each(sOmmStarTexColors, tex, color) {
        if (strcmp(tex->as_ptr, texture) == 0) {
            data->star.r = (color->as_u32 >> 16) & 0xFF;
            data->star.g = (color->as_u32 >>  8) & 0xFF;
            data->star.b = (color->as_u32 >>  0) & 0xFF;
            return;
        }
    }

    // Not found, compute new color and register it
    sys_path_t filename;
    str_cat(filename, sizeof(filename), FS_TEXTUREDIR "/", texture, ".png");
    s32 w, h;
    u8 *p = fs_load_png(filename, &w, &h);
    if (p) {
        u32 r = 0;
        u32 g = 0;
        u32 b = 0;
        for (s32 i = 0; i != w * h; ++i) {
            r += (u32) p[4 * i + 0];
            g += (u32) p[4 * i + 1];
            b += (u32) p[4 * i + 2];
        }
        data->star.r = (u8) (r / (w * h));
        data->star.g = (u8) (g / (w * h));
        data->star.b = (u8) (b / (w * h));
        stbi_image_free(p);
        omm_map_add(sOmmStarTexColors, ptr, mem_dup(texture, strlen(texture) + 1), u32, (data->star.r << 16) | (data->star.g << 8) | (data->star.b << 0));
    }
}

static Gfx *omm_geo_star_update(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (gCurrGraphNodeObject && callContext == GEO_CONTEXT_RENDER) {
        struct Object *o = gCurrGraphNodeObject;
        OmmStarGeoData *data = geo_get_geo_data(o,
            sizeof(OmmStarGeoData),
            sOmmStarGeoDataDisplayListsOffsets,
            array_length(sOmmStarGeoDataDisplayListsOffsets)
        );
        bool isStarSelectScreen = obj_get_first_with_behavior(bhvOmmActSelectStar) != NULL;

        // Update rays
        if (!isStarSelectScreen && !omm_is_game_paused() && !omm_is_main_menu()) {
            for (s32 i = 0; i != OMM_STAR_NUM_RAYS; ++i) {
                if (data->rays.ray[i].timer-- == 0) {
                    bool offset = (data->rays.ray[i].duration == 0);
                    data->rays.ray[i].radius = lerp_s(random_float(), OMM_STAR_RAY_RADIUS_MIN, OMM_STAR_RAY_RADIUS_MAX + 1);
                    data->rays.ray[i].angle = random_u16();
                    data->rays.ray[i].delta = lerp_s(random_float(), OMM_STAR_RAY_DELTA_ANGLE_MIN, OMM_STAR_RAY_DELTA_ANGLE_MAX + 1);
                    data->rays.ray[i].alpha = lerp_s(random_float(), OMM_STAR_RAY_ALPHA_MIN, OMM_STAR_RAY_ALPHA_MAX + 1);
                    data->rays.ray[i].duration = lerp_s(random_float(), OMM_STAR_RAY_DURATION_MIN, OMM_STAR_RAY_DURATION_MAX + 1);
                    data->rays.ray[i].timer = data->rays.ray[i].duration - offset * i * OMM_STAR_RAY_DURATION_MIN / OMM_STAR_NUM_RAYS;
                }
            }
        }

        // Update star color
        const char *texture = (const char *) ((const Gfx *) ((struct GraphNodeDisplayList *) node->next)->displayList)->words.w1;
        if (texture != data->star.texture) {
            omm_geo_star_compute_color(data, texture);
            data->star.texture = texture;
        }

        // Glow gfx
        Vtx *glowVtx = data->glow.vtx;
        Gfx *glowTri = data->glow.tri;
        for (s32 i = 0; !isStarSelectScreen && i != OMM_STAR_GLOW_NUM_POINTS; i += 2) {
            u16 a0 = ((i + 0) * 0x10000) / OMM_STAR_GLOW_NUM_POINTS;
            u16 a1 = ((i + 1) * 0x10000) / OMM_STAR_GLOW_NUM_POINTS;
            u16 a2 = ((i + 2) * 0x10000) / OMM_STAR_GLOW_NUM_POINTS;

            // Triangles
            gSPVertex(glowTri++, glowVtx, 7, 0);
            gSP2Triangles(glowTri++, 0, 2, 1, 0, 0, 3, 2, 0);
            gSP2Triangles(glowTri++, 1, 5, 4, 0, 2, 5, 1, 0);
            gSP2Triangles(glowTri++, 2, 6, 5, 0, 3, 6, 2, 0);

            // Vertices
            *(glowVtx++) = (Vtx) { { {                                   0,                                   0, 0 }, 0, { 0, 0 }, { data->star.r, data->star.g, data->star.b, OMM_STAR_GLOW_ALPHA_IN  } } };
            *(glowVtx++) = (Vtx) { { { OMM_STAR_GLOW_RADIUS_IN  * sins(a0), OMM_STAR_GLOW_RADIUS_IN  * coss(a0), 0 }, 0, { 0, 0 }, { data->star.r, data->star.g, data->star.b, OMM_STAR_GLOW_ALPHA_IN  } } };
            *(glowVtx++) = (Vtx) { { { OMM_STAR_GLOW_RADIUS_IN  * sins(a1), OMM_STAR_GLOW_RADIUS_IN  * coss(a1), 0 }, 0, { 0, 0 }, { data->star.r, data->star.g, data->star.b, OMM_STAR_GLOW_ALPHA_IN  } } };
            *(glowVtx++) = (Vtx) { { { OMM_STAR_GLOW_RADIUS_IN  * sins(a2), OMM_STAR_GLOW_RADIUS_IN  * coss(a2), 0 }, 0, { 0, 0 }, { data->star.r, data->star.g, data->star.b, OMM_STAR_GLOW_ALPHA_IN  } } };
            *(glowVtx++) = (Vtx) { { { OMM_STAR_GLOW_RADIUS_OUT * sins(a0), OMM_STAR_GLOW_RADIUS_OUT * coss(a0), 0 }, 0, { 0, 0 }, { data->star.r, data->star.g, data->star.b, OMM_STAR_GLOW_ALPHA_OUT } } };
            *(glowVtx++) = (Vtx) { { { OMM_STAR_GLOW_RADIUS_OUT * sins(a1), OMM_STAR_GLOW_RADIUS_OUT * coss(a1), 0 }, 0, { 0, 0 }, { data->star.r, data->star.g, data->star.b, OMM_STAR_GLOW_ALPHA_OUT } } };
            *(glowVtx++) = (Vtx) { { { OMM_STAR_GLOW_RADIUS_OUT * sins(a2), OMM_STAR_GLOW_RADIUS_OUT * coss(a2), 0 }, 0, { 0, 0 }, { data->star.r, data->star.g, data->star.b, OMM_STAR_GLOW_ALPHA_OUT } } };
        }
        gSPEndDisplayList(glowTri++);
        gfx_copy_and_fill_null(data->glow.gfx, omm_star_glow_gfx, sizeof(omm_star_glow_gfx), data->glow.tri);

        // Rays gfx
        Vtx *raysVtx = data->rays.vtx;
        Gfx *raysTri = data->rays.tri;
        for (s32 i = 0; !isStarSelectScreen && i != OMM_STAR_NUM_RAYS; ++i) {
            if (data->rays.ray[i].duration != 0) {
                f32 t = invlerp_0_1_s(data->rays.ray[i].timer, data->rays.ray[i].duration, 0);
                s32 rad = data->rays.ray[i].radius;
                u16 a0 = data->rays.ray[i].angle - data->rays.ray[i].delta;
                u16 a1 = data->rays.ray[i].angle + data->rays.ray[i].delta;
                s32 r = ((data->star.r * 4) + 0xFF) / 5;
                s32 g = ((data->star.g * 4) + 0xFF) / 5;
                s32 b = ((data->star.b * 4) + 0xFF) / 5;
                s32 a = data->rays.ray[i].alpha * sins(0x8000 * t);

                // Triangles
                gSPVertex(raysTri++, raysVtx, 3, 0);
                gSP1Triangle(raysTri++, 0, 2, 1, 0);

                // Vertices
                *(raysVtx++) = (Vtx) { { {              0,              0, 0 }, 0, { 0, 0 }, { r, g, b, a } } };
                *(raysVtx++) = (Vtx) { { { rad * sins(a0), rad * coss(a0), 0 }, 0, { 0, 0 }, { r, g, b, 0 } } };
                *(raysVtx++) = (Vtx) { { { rad * sins(a1), rad * coss(a1), 0 }, 0, { 0, 0 }, { r, g, b, 0 } } };
            }
        }
        gSPEndDisplayList(raysTri++);
        gfx_copy_and_fill_null(data->rays.gfx, omm_star_rays_gfx, sizeof(omm_star_rays_gfx), data->rays.tri);
    }
    return NULL;
}

//
// Geo layouts
//

#define omm_geo_star_opaque(x) \
const GeoLayout omm_geo_star_##x##_opaque[] = { \
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100), \
    GEO_OPEN_NODE(), \
        GEO_SCALE(0, 0x4000), \
        GEO_OPEN_NODE(), \
            GEO_ASM(0, omm_geo_star_update), \
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_##x##_texture_gfx), \
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_star_body_opaque_gfx), \
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_star_eyes_opaque_gfx), \
        GEO_CLOSE_NODE(), \
        GEO_BILLBOARD(), \
        GEO_OPEN_NODE(), \
            GEO_ASM(60, geo_move_from_camera), \
            GEO_TRANSLATE_NODE(0, 0, 0, 0), \
            GEO_OPEN_NODE(), \
                GEO_ASM(0, geo_link_geo_data), \
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL), \
            GEO_CLOSE_NODE(), \
            GEO_ASM(1, geo_link_geo_data), \
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL), \
        GEO_CLOSE_NODE(), \
    GEO_CLOSE_NODE(), \
    GEO_END(), \
}
omm_geo_star_opaque(0);
omm_geo_star_opaque(1);
omm_geo_star_opaque(2);
omm_geo_star_opaque(3);
omm_geo_star_opaque(4);
omm_geo_star_opaque(5);
omm_geo_star_opaque(6);
omm_geo_star_opaque(7);
omm_geo_star_opaque(8);
omm_geo_star_opaque(9);
omm_geo_star_opaque(10);
omm_geo_star_opaque(11);
omm_geo_star_opaque(12);
omm_geo_star_opaque(13);
omm_geo_star_opaque(14);
omm_geo_star_opaque(15);
omm_geo_star_opaque(16);
omm_geo_star_opaque(17);
omm_geo_star_opaque(18);
omm_geo_star_opaque(19);

#define omm_geo_star_transparent(x) \
const GeoLayout omm_geo_star_##x##_transparent[] = { \
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100), \
    GEO_OPEN_NODE(), \
        GEO_SCALE(0, 0x4000), \
        GEO_OPEN_NODE(), \
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_##x##_texture_gfx), \
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_body_transparent_gfx), \
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_star_eyes_transparent_gfx), \
        GEO_CLOSE_NODE(), \
    GEO_CLOSE_NODE(), \
    GEO_END(), \
}
omm_geo_star_transparent(0);
omm_geo_star_transparent(1);
omm_geo_star_transparent(2);
omm_geo_star_transparent(3);
omm_geo_star_transparent(4);
omm_geo_star_transparent(5);
omm_geo_star_transparent(6);
omm_geo_star_transparent(7);
omm_geo_star_transparent(8);
omm_geo_star_transparent(9);
omm_geo_star_transparent(10);
omm_geo_star_transparent(11);
omm_geo_star_transparent(12);
omm_geo_star_transparent(13);
omm_geo_star_transparent(14);
omm_geo_star_transparent(15);
omm_geo_star_transparent(16);
omm_geo_star_transparent(17);
omm_geo_star_transparent(18);
omm_geo_star_transparent(19);

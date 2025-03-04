#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_EYES_GAP              (0.90f)
#define OMM_CAPPY_EYES_TEXCOORDS_MULT   (1.05f)
#define OMM_CAPPY_EYES_WH_RATIO         (0.75f)
#define OMM_CAPPY_EYES_DEPTH            (16)
#define OMM_CAPPY_EYES_CURVE            (12)

#define OMM_TEXTURE_CAPPY               (count < 12 ? OMM_TEXTURE_CAPPY_32     : (count < 20 ? OMM_TEXTURE_CAPPY_64     : OMM_TEXTURE_CAPPY_128    ))
#define OMM_TEXTURE_CAPPY_EYE           (count < 12 ? OMM_TEXTURE_CAPPY_EYE_32 : (count < 20 ? OMM_TEXTURE_CAPPY_EYE_64 : OMM_TEXTURE_CAPPY_EYE_128))

Lights1 omm_cappy_eye_top_light = gdSPDefLights1(
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
    0x28, 0x28, 0x28
);

Lights1 omm_cappy_eye_bottom_light = gdSPDefLights1(
    0xFF, 0x40, 0x80,
    0xFF, 0x40, 0x80,
    0x28, 0x28, 0x28
);

//
// Cappy eyes
//

typedef struct { u32 id; s32 scale; f32 x, y, z; s16 pitch, yaw, roll; f32 radius; s32 count; } OmmCappyEyesParams;
static const OmmCappyEyesParams OMM_CAPPY_EYES_PARAMS[] = {

  ///////////
 // Mario //
///////////

// SM64 --------------------------------------------------------------- //
{ 0xBD98920E, 0x10000, 235,  93,   0, 0x3800, 0x4000, 0x8000, 44,  8 }, // Mario
{ 0x007C0068, 0x10000,   0,  73, 135, 0xF800, 0x0000, 0x0000, 44,  8 }, // Regular cap
{ 0x00A40078, 0x10000,   0,  73, 135, 0xF800, 0x0000, 0x0000, 44,  8 }, // Wing cap

// Moonshine ---------------------------------------------------------- //
{ 0xDB2EEEDE, 0x10000, 245,  93,   0, 0x3900, 0x4000, 0x8000, 44, 32 }, // Mario
{ 0x285C1E70, 0x10000,   0,  96, 114, 0x0000, 0x0000, 0x0000, 40, 32 }, // Regular cap
{ 0x450636A8, 0x10000,   0,  96, 114, 0x0000, 0x0000, 0x0000, 40, 32 }, // Wing cap

// Render96 ----------------------------------------------------------- //
{ 0x446EE43E, 0x044D7, 210,  86,   0, 0x3800, 0x4000, 0x8000, 40, 32 }, // Mario (v3.25) + color
{ 0x3C78E810, 0x044D7, 210,  86,   0, 0x3800, 0x4000, 0x8000, 40, 32 }, // Mario (v3.25)
{ 0x2FE48D64, 0x10000, 229,  73,   0, 0x3F00, 0x4000, 0x8000, 44, 32 }, // Mario (v3.2)
{ 0x065C057C, 0x10000,  -3,  72, 138, 0xF700, 0x0000, 0x0000, 44, 32 }, // Regular cap
{ 0x4AEE422A, 0x10000,  -3,  72, 138, 0xF700, 0x0000, 0x0000, 44, 32 }, // Wing cap

// Low-Poly Promo ----------------------------------------------------- //
{ 0x656C57A8, 0x10000, 242,  94,   0, 0x3F00, 0x4000, 0x8000, 44,  8 }, // Mario
{ 0x00D000B0, 0x10000,  -3,  71, 136, 0xF200, 0x0000, 0x0000, 44,  8 }, // Regular cap
{ 0x139E1086, 0x10000,  -3,  71, 136, 0xF200, 0x0000, 0x0000, 44,  8 }, // Wing cap

  ///////////
 // Luigi //
///////////

// Render96 ----------------------------------------------------------- //
{ 0x2790CAA4, 0x044D7, 208,  84,   0, 0x3B00, 0x4000, 0x8000, 36, 32 }, // Luigi (v3.25) + color
{ 0x2498CB98, 0x044D7, 208,  84,   0, 0x3B00, 0x4000, 0x8000, 36, 32 }, // Luigi (v3.25)
{ 0xEB8EB218, 0x10000, 239,  96,   0, 0x3B00, 0x4000, 0x8000, 36, 32 }, // Luigi (v3.2)
{ 0x0DEA0BC0, 0x10000,   0,  55, 102, 0xF500, 0x0000, 0x0000, 36, 32 }, // Regular cap
{ 0x56434B90, 0x10000,   0,  55, 102, 0xF500, 0x0000, 0x0000, 36, 32 }, // Wing cap

// Low-Poly Promo ----------------------------------------------------- //
{ 0x35CC2EC8, 0x10000, 187,  73,   0, 0x3F00, 0x4000, 0x8000, 36,  8 }, // Luigi
{ 0x010A00E4, 0x10000,  -2,  71, 136, 0xF000, 0x0000, 0x0000, 44,  8 }, // Regular cap
{ 0x13F510D4, 0x10000,  -2,  71, 136, 0xF000, 0x0000, 0x0000, 44,  8 }, // Wing cap

// VaniLla ------------------------------------------------------------ //
{ 0x2D642ABE, 0x10000, 200,  74,   0, 0x3900, 0x4000, 0x8000, 36,  8 }, // Luigi
{ 0x007C0068, 0x10000,   0,  73, 135, 0xF800, 0x0000, 0x0000, 44,  8 }, // Regular cap
{ 0x00A40078, 0x10000,   0,  73, 135, 0xF800, 0x0000, 0x0000, 44,  8 }, // Wing cap

  ///////////
 // Wario //
///////////

// Render96 ----------------------------------------------------------- //
{ 0x9290DDB8, 0x044D7, 175, 105,   0, 0x3800, 0x4000, 0x8000, 40, 32 }, // Wario (v3.25)
{ 0x20CEA0D0, 0x10000, 200, 182,  -4, 0x3F00, 0x4000, 0x8000, 40, 32 }, // Wario (v3.2)
{ 0x06FA061E, 0x10000,   2,  60, 136, 0x0000, 0x0000, 0x0000, 40, 32 }, // Regular cap
{ 0x4BDB431D, 0x10000,   2,  60, 136, 0x0000, 0x0000, 0x0000, 40, 32 }, // Wing cap

// Low-Poly Promo ----------------------------------------------------- //
{ 0xD968BF5E, 0x10000, 217, 171,  -3, 0x3600, 0x4000, 0x8000, 40,  8 }, // Wario
{ 0x00D200AC, 0x10000,   0,  87, 154, 0x0800, 0x0000, 0x0000, 44,  8 }, // Regular cap
{ 0x13A11080, 0x10000,   0,  87, 154, 0x0800, 0x0000, 0x0000, 44,  8 }, // Wing cap

// VaniLla ------------------------------------------------------------ //
{ 0x291422A8, 0x10000, 214, 108,   0, 0x3600, 0x4000, 0x8000, 44,  8 }, // Wario
{ 0x003C0030, 0x10000,   0,  59, 130, 0xFC00, 0x0000, 0x0000, 40,  8 }, // Regular cap
{ 0x004C0038, 0x10000,   0,  59, 130, 0xFC00, 0x0000, 0x0000, 40,  8 }, // Wing cap

};

static const OmmCappyEyesParams *omm_cappy_gfx_get_eyes_params(u32 id) {
    array_for_each_(const OmmCappyEyesParams, params, OMM_CAPPY_EYES_PARAMS) {
        if (params->id == id) {
            return params;
        }
    }
    return NULL;
}

//
// Cappy eyes display list
//

static Vtx omm_cappy_gfx_get_vertex(Vec3f ori, Vec3f hrz, Vec3f vrt, Vec3f fwd, f32 gap, f32 radius, f32 scale, f32 sign, s32 count, s32 i, bool back) {
    s16 a    = (s16) ((65536.f * (i - 1)) / count);
    f32 texu = (i == 0 ? 0.5f : (1.f - ((1.f + sins(a)) / 2.f)) * sign - ((sign - 1.f) / 2.f));
    f32 texv = (i == 0 ? 0.5f : (1.f - ((1.f + coss(a)) / 2.f)));
    f32 hrzv = ((i != 0) * radius * sins(a) - sign * gap) * -OMM_CAPPY_EYES_WH_RATIO;
    f32 vrtv = ((i != 0) * radius * coss(a));
    f32 fwdv = -((1.f - coss(((i != 0) * (radius != 0.f) * sins(-sign * a) + 1.f) * 0x2000)) * OMM_CAPPY_EYES_CURVE * scale + (back ? OMM_CAPPY_EYES_DEPTH * scale : 0.f));
    if (texu < 0) texu = ((texu + 0.5f) * OMM_CAPPY_EYES_TEXCOORDS_MULT) - 0.5f;
    else          texu = ((texu - 0.5f) * OMM_CAPPY_EYES_TEXCOORDS_MULT) + 0.5f;
    if (texv < 0) texv = ((texv + 0.5f) * OMM_CAPPY_EYES_TEXCOORDS_MULT) - 0.5f;
    else          texv = ((texv - 0.5f) * OMM_CAPPY_EYES_TEXCOORDS_MULT) + 0.5f;
    Vtx vtx = { { { 0, 0, 0 }, 0, { (s16) (texu * 0x1000), (s16) (texv * 0x1000) }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
    vtx.v.ob[0] = hrzv * hrz[0] + vrtv * vrt[0] + fwdv * fwd[0] + ori[0];
    vtx.v.ob[1] = hrzv * hrz[1] + vrtv * vrt[1] + fwdv * fwd[1] + ori[1];
    vtx.v.ob[2] = hrzv * hrz[2] + vrtv * vrt[2] + fwdv * fwd[2] + ori[2];
    return vtx;
}

static Vtx omm_cappy_gfx_get_pupil_vertex(Vec3f ori, Vec3f hrz, Vec3f vrt, Vec3f fwd, f32 gap, f32 radius, f32 scale, f32 sign, s32 count, s32 i, f32 v) {
    s16 a    = (s16) ((65536.f * i) / count);
    f32 texu = (1.f - ((1.f + sins(a)) / 2.f)) * sign - ((sign - 1.f) / 2.f);
    f32 texv = (1.f - ((1.f + v) / 2.f));
    f32 hrzv = (radius * sins(a) - sign * gap) * -OMM_CAPPY_EYES_WH_RATIO;
    f32 vrtv = (radius * v) / 2.f;
    f32 fwdv = 0.01f - (1.f - coss((sins(-sign * a) + 1.f) * 0x2000)) * OMM_CAPPY_EYES_CURVE * scale;
    if (texu < 0) texu = ((texu + 0.5f) * OMM_CAPPY_EYES_TEXCOORDS_MULT) - 0.5f;
    else          texu = ((texu - 0.5f) * OMM_CAPPY_EYES_TEXCOORDS_MULT) + 0.5f;
    if (texv < 0) texv = ((texv + 0.5f) * OMM_CAPPY_EYES_TEXCOORDS_MULT) - 0.5f;
    else          texv = ((texv - 0.5f) * OMM_CAPPY_EYES_TEXCOORDS_MULT) + 0.5f;
    Vtx vtx = { { { 0, 0, 0 }, 0, { (s16) (texu * 0x1000), (s16) (texv * 0x1000) }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
    vtx.v.ob[0] = hrzv * hrz[0] + vrtv * vrt[0] + fwdv * fwd[0] + ori[0];
    vtx.v.ob[1] = hrzv * hrz[1] + vrtv * vrt[1] + fwdv * fwd[1] + ori[1];
    vtx.v.ob[2] = hrzv * hrz[2] + vrtv * vrt[2] + fwdv * fwd[2] + ori[2];
    return vtx;
}

static Gfx **omm_cappy_gfx_get_display_lists(u32 id, bool metal) {
    static OmmMap sOmmCappyDisplayLists = omm_map_zero;
    u32 displayListsId = (id & ~1) | (metal << 0);
    Gfx **gfx = NULL;

    // Retrieve existing display lists
    s32 i = omm_map_find_key(sOmmCappyDisplayLists, u32, displayListsId);
    if (OMM_LIKELY(i != -1)) {
        return (Gfx **) omm_map_get_val(sOmmCappyDisplayLists, ptr, i);
    }

    // Find params from id
    const OmmCappyEyesParams *params = omm_cappy_gfx_get_eyes_params(id);
    if (params) {
        Vec3f fwd, vrt, hrz, ori;

        // Extract params
        f32 scale = (f32) 0x10000 / (f32) params->scale;
        f32 radius = params->radius * scale;
        s32 count = params->count;

        // Origin
        vec3f_mul(vec3f_set(ori, params->x, params->y, params->z), scale);

        // Forward axis
        vec3f_set_dist_and_angle(gVec3fZero, fwd, 1.f, params->pitch, params->yaw);

        // Vertical axis
        vec3f_rotate_zxy(vrt, gVec3fY, 0, 0, params->roll);
        vec3f_project_vector(vrt, vrt, fwd);
        vec3f_normalize(vrt);

        // Horizontal axis
        vec3f_rotate_around_n(hrz, fwd, vrt, 0x4000);

        // Data
        Vtx *vtx0 = mem_new(Vtx, count * 12);
        Vtx *vtx1 = mem_new(Vtx, count * 4);
        Gfx *tri0 = mem_new(Gfx, count * 6 + 1);
        Gfx *tri1 = mem_new(Gfx, count * 7 + 1);

        // Display lists
        const Gfx gfx_[4][0x20] = { {
/* 0x00 */  gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
/* 0x01 */  gsSPClearGeometryMode(G_TEXTURE_GEN | G_CULL_BOTH),
/* 0x02 */  gsSPSetGeometryMode(G_CULL_BACK),
/* 0x03 */  gsDPSetCombineLERP(TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, ENVIRONMENT, TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, ENVIRONMENT), // G_CC_BLENDRGBFADEA
/* 0x04 */  gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
/* 0x05 */  gsDPLoadTextureBlock(OMM_TEXTURE_CAPPY, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0),
/* 0x0C */  gsSPDisplayList(tri0),
/* 0x0D */  gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
/* 0x0E */  gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT), // G_CC_SHADEFADEA
/* 0x0F */  gsSPEndDisplayList(),
        }, {
/* 0x00 */  gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
/* 0x01 */  gsSPClearGeometryMode(G_TEXTURE_GEN | G_CULL_BOTH),
/* 0x02 */  gsSPSetGeometryMode(G_CULL_BACK),
/* 0x03 */  gsDPSetCombineLERP(TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, ENVIRONMENT, TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, ENVIRONMENT), // G_CC_BLENDRGBFADEA
/* 0x04 */  gsXPPushTexture(),
/* 0x05 */  gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
/* 0x06 */  gsDPLoadTextureBlock(OMM_TEXTURE_CAPPY, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0),
/* 0x0D */  gsSPDisplayList(tri0),
/* 0x0E */  gsXPPopTexture(),
/* 0x0F */  gsSPSetGeometryMode(G_TEXTURE_GEN),
/* 0x10 */  gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, ENVIRONMENT, 0, 0, 0, TEXEL0, 0, 0, 0, ENVIRONMENT), // G_CC_DECALFADE
/* 0x11 */  gsSPEndDisplayList(),
        }, {
/* 0x00 */  gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
/* 0x01 */  gsSPClearGeometryMode(G_TEXTURE_GEN | G_CULL_BOTH),
/* 0x02 */  gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_ALPHA | G_CULL_BACK),
/* 0x03 */  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
/* 0x04 */  gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
/* 0x05 */  gsDPLoadTextureBlock(OMM_TEXTURE_CAPPY_EYE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0),
/* 0x0C */  gsSPDisplayList(tri1),
/* 0x0D */  gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
/* 0x0E */  gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT), // G_CC_SHADEFADEA
/* 0x0F */  gsSPClearGeometryMode(G_TEXTURE_ALPHA),
/* 0x10 */  gsSPEndDisplayList(),
        }, {
/* 0x00 */  gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
/* 0x01 */  gsSPClearGeometryMode(G_TEXTURE_GEN | G_CULL_BOTH),
/* 0x02 */  gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_ALPHA | G_CULL_BACK),
/* 0x03 */  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
/* 0x04 */  gsXPPushTexture(),
/* 0x05 */  gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
/* 0x06 */  gsDPLoadTextureBlock(OMM_TEXTURE_CAPPY_EYE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0),
/* 0x0D */  gsSPDisplayList(tri1),
/* 0x0E */  gsXPPopTexture(),
/* 0x0F */  gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT), // G_CC_SHADEFADEA
/* 0x10 */  gsSPClearGeometryMode(G_TEXTURE_ALPHA),
/* 0x11 */  gsDPPipeSync(), // gsDPPipeSync if not fully opaque, gsSPEndDisplayList otherwise
/* 0x12 */  gsSPSetGeometryMode(G_TEXTURE_GEN),
/* 0x13 */  gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, ENVIRONMENT, 0, 0, 0, TEXEL0, 0, 0, 0, ENVIRONMENT), // G_CC_DECALFADE
/* 0x14 */  gsSPEndDisplayList(),
        } };
        gfx    = mem_new(Gfx *, 2);
        gfx[0] = mem_dup(gfx_[0 + metal], sizeof(gfx_[0 + metal]));
        gfx[1] = mem_dup(gfx_[2 + metal], sizeof(gfx_[2 + metal]));

        // Triangles and vertices
        for (s32 sign = -1; sign <= +1; sign += 2) {
            for (s32 i = 1; i <= count; ++i) {

                // Eyes
                gSPVertex(tri0++, vtx0, 6, 0);
                gSP2Triangles(tri0++, 0, 2, 4, 0, 5, 3, 1, 0); // Front & Back
                gSP2Triangles(tri0++, 2, 3, 4, 0, 5, 4, 3, 0); // Border
                *(vtx0++) = omm_cappy_gfx_get_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius, radius, scale, sign, count,     0, 0);
                *(vtx0++) = omm_cappy_gfx_get_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius,    0.f, scale, sign, count, i + 0, 1);
                *(vtx0++) = omm_cappy_gfx_get_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius, radius, scale, sign, count, i + 0, 0);
                *(vtx0++) = omm_cappy_gfx_get_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius, radius, scale, sign, count, i + 0, 1);
                *(vtx0++) = omm_cappy_gfx_get_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius, radius, scale, sign, count, i + 1, 0);
                *(vtx0++) = omm_cappy_gfx_get_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius, radius, scale, sign, count, i + 1, 1);

                // Pupils
                if (i <= count / 2) {
                    s32 j0 = count / 4 + (i - 1) % (count / 2);
                    s32 j1 = j0 + 1;
                    gSPLight(tri1++, &omm_cappy_eye_top_light.l, 1);
                    gSPLight(tri1++, &omm_cappy_eye_top_light.a, 2);
                    gSPVertexTC(tri1++, vtx1, 2, 0);
                    *(vtx1++) = omm_cappy_gfx_get_pupil_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius, radius, scale, sign, count, j0, 1);
                    *(vtx1++) = omm_cappy_gfx_get_pupil_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius, radius, scale, sign, count, j1, 1);
                    gSPLight(tri1++, &omm_cappy_eye_bottom_light.l, 1);
                    gSPLight(tri1++, &omm_cappy_eye_bottom_light.a, 2);
                    gSPVertexTC(tri1++, vtx1, 2, 2);
                    *(vtx1++) = omm_cappy_gfx_get_pupil_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius, radius, scale, sign, count, j0, -1);
                    *(vtx1++) = omm_cappy_gfx_get_pupil_vertex(ori, hrz, vrt, fwd, OMM_CAPPY_EYES_GAP * radius, radius, scale, sign, count, j1, -1);
                    gSP2Triangles(tri1++, 2, 1, 0, 0, 1, 2, 3, 0);
                }
            }
        }
        gSPEndDisplayList(tri0);
        gSPEndDisplayList(tri1);
    }

    // Add to cache
    omm_map_add(sOmmCappyDisplayLists, u32, displayListsId, ptr, (ptr) gfx);
    return gfx;
}

static void omm_cappy_gfx_draw(u32 id, u8 alpha, bool metal, void (*append)(void *, s16)) {
    Gfx **displayLists = omm_cappy_gfx_get_display_lists(id, metal);
    if (displayLists) {
        gDPSetEnvColor(displayLists[0], 0xFF, 0xFF, 0xFF, alpha);
        if (alpha != 0xFF) {
            append(displayLists[0], LAYER_TRANSPARENT);
            gDPSetEnvColor(displayLists[1], 0xFF, 0xFF, 0xFF, alpha);
            gDPPipeSync(displayLists[1] + 0x11);
            append(displayLists[1], LAYER_TRANSPARENT);
        } else {
            append(displayLists[0], LAYER_OPAQUE);
            gDPSetEnvColor(displayLists[1], 0xFF, 0xFF, 0xFF, alpha);
            gSPEndDisplayList(displayLists[1] + 0x11);
            append(displayLists[1], LAYER_TRANSPARENT);
        }
    }
}

void omm_cappy_gfx_draw_eyes(struct GraphNode *node, void (*append)(void *, s16)) {
    if (OMM_EXTRAS_CAPPY_AND_TIARA && gCurGraphNodeObject && node && node->type == GRAPH_NODE_TYPE_SWITCH_CASE) {
        struct GraphNodeSwitchCase *scNode = (struct GraphNodeSwitchCase *) node;

        // Mario's cap
        if (scNode->fnNode.func == (GraphNodeFunc) geo_switch_mario_cap_on_off) {

            // Cap must be on Mario's head
            struct MarioBodyState *bodyState = gMarioState->marioBodyState;
            if (bodyState->capState & 1) {
                return;
            }

            // States
            u32 id = omm_cappy_gfx_get_graph_node_identifier(gCurGraphNodeObject->sharedChild);
            u8 alpha = ((bodyState->modelState & 0x100) ? (bodyState->modelState & 0xFF) : 0xFF);
            bool metal = (bodyState->modelState & MODEL_STATE_METAL) != 0;
            omm_cappy_gfx_draw(id, alpha, metal, append);
            return;
        }

        // Cap object
        if (scNode->fnNode.func == (GraphNodeFunc) geo_switch_anim_state) {

            // Object's model must be a cap model
            struct Object *o = gCurrGraphNodeObject;
            for (s32 i = 0; i != OMM_NUM_PLAYABLE_CHARACTERS; ++i) {
                if (obj_has_model(o, omm_player_graphics_get_normal_cap(i)) ||
                    obj_has_model(o, omm_player_graphics_get_wing_cap(i)) ||
                    obj_has_model(o, omm_player_graphics_get_metal_cap(i)) ||
                    obj_has_model(o, omm_player_graphics_get_winged_metal_cap(i))) {

                    // States
                    u32 id = omm_cappy_gfx_get_graph_node_identifier(gCurGraphNodeObject->sharedChild);
                    omm_cappy_gfx_draw(id, o->oOpacity, false, append);
                    return;
                }
            }
        }
    }
}

//
// Graph node identifier
// Compute a unique identification number for a given graph node,
// based on some values that constitute the graph node
//

static void omm_cappy_gfx_process_display_list(const Gfx *displayList, u16 *vtxCount, u16 *triCount) {
    if (displayList) {
        for (const Gfx *gfx = displayList;; ++gfx) {
            switch (gfx->words.w0 >> 24) {
                case G_VTX:      *vtxCount += (u16) ((gfx->words.w0 >> 12) & 0xFF); break;
                case G_VTXTC:    *vtxCount += (u16) ((gfx->words.w0 >> 12) & 0xFF); break;
                case G_VTXEXT:   *vtxCount += (u16) ((gfx->words.w0 >> 8) & 0xFFFF); break;
                case G_VTXEXTTC: *vtxCount += (u16) ((gfx->words.w0 >> 8) & 0xFFFF); break;
                case G_TRI1:     *triCount += 1; break;
                case G_TRI2:     *triCount += 2; break;
                case G_TRIEXT:   *triCount += (u16) ((gfx->words.w0 >> 8) & 0xFFFF); break;
                case G_ENDDL:    return;
                case G_DL: {
                    omm_cappy_gfx_process_display_list((const Gfx *) gfx->words.w1, vtxCount, triCount);
                    if ((gfx->words.w0 >> 16) & 1) {
                        return;
                    }
                } break;
            }
        }
    }
}

static void omm_cappy_gfx_process_graph_node(struct GraphNode *node, u16 *vtxCount, u16 *triCount) {
    if (node) {
        struct GraphNode *curr = node;
        do {
            if (curr->type == GRAPH_NODE_TYPE_TRANSLATION_ROTATION ||
                curr->type == GRAPH_NODE_TYPE_TRANSLATION ||
                curr->type == GRAPH_NODE_TYPE_ROTATION ||
                curr->type == GRAPH_NODE_TYPE_ANIMATED_PART ||
                curr->type == GRAPH_NODE_TYPE_BILLBOARD ||
                curr->type == GRAPH_NODE_TYPE_DISPLAY_LIST ||
                curr->type == GRAPH_NODE_TYPE_SCALE) {
                omm_cappy_gfx_process_display_list(((struct GraphNodeDisplayList *) curr)->displayList, vtxCount, triCount);
            }
            omm_cappy_gfx_process_graph_node(curr->children, vtxCount, triCount);
        } while ((curr = curr->next) != node);
    }
}

u32 omm_cappy_gfx_get_graph_node_identifier(struct GraphNode *node) {
    static OmmMap sGraphNodeIdentifiers = omm_map_zero;
    s32 i = omm_map_find_key(sGraphNodeIdentifiers, ptr, node);
    if (OMM_UNLIKELY(i == -1)) {
        i = omm_map_count(sGraphNodeIdentifiers);
        u16 vtxCount = 0, triCount = 0;
        omm_cappy_gfx_process_graph_node(node, &vtxCount, &triCount);
        u32 identifier = (u32) ((vtxCount << 16u) | (triCount << 0u));
        omm_map_add(sGraphNodeIdentifiers, ptr, node, u32, identifier);
    }
    return omm_map_get_val(sGraphNodeIdentifiers, u32, i);
}

bool omm_cappy_gfx_has_eyes(struct GraphNode *node) {
    u32 id = omm_cappy_gfx_get_graph_node_identifier(node);
    return omm_cappy_gfx_get_display_lists(id, false) != NULL;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include <unistd.h>

#ifdef DEBUG
#define ASSERT(x) SDL_assert(x)
#else
#define ASSERT(x)
#endif

#if defined(RAPI_GL) || defined(RAPI_GL_LEGACY)
#define GFX_OPEN_GL 1
#elif defined(RAPI_D3D11) || defined(RAPI_D3D12)
#define GFX_OPEN_GL 0
#else
#error "Unknown Render API"
#endif

#if !OMM_GAME_IS_R96A && !OMM_GAME_IS_SMMS && !QOL_FEATURE_KING_BOBOMB_BLINK
#define KING_BOBOMB_TEXTURE_FIX 1
#else
#define KING_BOBOMB_TEXTURE_FIX 0
#endif

#if OMM_GAME_IS_R96A && defined(RAPI_GL) && defined(TRANSPARENCY_GL)
#include "GL/glew.h"
#define GL_TRANSPARENCY 1
#else
#define GL_TRANSPARENCY 0
#endif

#define GFX_RATIO_X                         (gfx_current_dimensions.width / (f32) SCREEN_WIDTH)
#define GFX_RATIO_Y                         (gfx_current_dimensions.height / (f32) SCREEN_HEIGHT)
#define GFX_MAX_VERTS                       64
#define GFX_MAX_TRIS                        256

#define GFX_OP                              ((u8) (sGfxCmd->words.w0 >> 24))
#define GFX_W0                              sGfxCmd->words.w0
#define GFX_W1                              sGfxCmd->words.w1
#define GFX_W1P                             ((void *) sGfxCmd->words.w1)
#define GFX_C0(off, siz)                    ((sGfxCmd->words.w0 >> (off)) & ((1U << siz) - 1))
#define GFX_C1(off, siz)                    ((sGfxCmd->words.w1 >> (off)) & ((1U << siz) - 1))

#define mtx_stack_offset(x)                 ((x) * (sizeof(Mat4) / sizeof(f32)))
#define mtx_stack_inc(x)                    sRSP->mtx += mtx_stack_offset(x)
#define mtx_stack_dec(x)                    sRSP->mtx -= mtx_stack_offset(x)
#define mtx_mul(c, a, b)                    mtxf_mul((void *) (c), (void *) (a), (void *) (b))
#define gfx_adjust_x_for_aspect_ratio(x)    ((x) * gfx_adjust_for_aspect_ratio)
#define gfx_color_comb_comp(x)              ((x) < 6 ? (x) : ((x) == G_CCMUX_TEXEL0_ALPHA ? CC_TEXEL0A : CC_0))
#define gfx_color_comb(a, b, c, d)          ((gfx_color_comb_comp(a) << 0) | (gfx_color_comb_comp(b) << 3) | (gfx_color_comb_comp(c) << 6) | (gfx_color_comb_comp(d) << 9))
#define tex_u_normalize(tu)                 ((((tu) - (sRDP->texTile.uls << 3)) * INV32 + sRDP->texOffset) * sRDP->invTexWidth)
#define tex_v_normalize(tv)                 ((((tv) - (sRDP->texTile.ult << 3)) * INV32 + sRDP->texOffset) * sRDP->invTexHeight)

//
// Structs
//

typedef struct { u16 x, y, w, h; } XYWH;
typedef struct { Vec4f p; Vec3f n; Vec2f tc; Vec4f color; u8 tti, rej; const Vtx_t *v; } Vertex;
typedef struct { u32 hash, id; u8 cms, cmt, lin; } GfxTexture;
typedef struct { u32 id; void *prg; u8 mapShaderInput[2][4]; } ColorCombiner;

typedef struct {

    // Matrices
    Mat4 mtxStack[64];
    f32 *mtx;
    Mat4 mtxP;
    Mat4 mtxMP;
    
    // Lights
    Light_t light, lightAmb;
    Vec3f lightCoeffs, lookAtXCoeffs, lookAtYCoeffs;
    bool lightsChanged;

    // Vertices
    u32 geometryMode;
    s16 fogMul, fogOff;
    u16 texOn, texS, texT;
    Vertex vtx[GFX_MAX_VERTS + 4];
} RealitySignalProcessor;

typedef struct {

    // Textures
    const char *texToLoad;
    const char *texLoaded;
    struct { u8 tti, cms, cmt; u16 uls, ult, lrs, lrt; } texTile;
    f32 invTexWidth, invTexHeight, texOffset;
    bool texChanged;
    
    // Colors
    u32 combineMode, otherModeL, otherModeH;
    Vec4f envColor, primColor, fogColor, fillColor;
    f32 *envColors[3];
    f32 *primColors[3];
    XYWH viewport;
    void *zBufAddress;
    void *colorImageAddress;
} RealityDisplayProcessor;

typedef struct {
    f32 buffer[GFX_MAX_TRIS * 3 * 26];
    s32 length;
    s32 tris;
} VertexBufferObject;

typedef struct {
    bool dropFrame;
    void *shaderProgram;
    GfxTexture *texture;
} RenderingState;

static struct GfxWindowManagerAPI *sGfxWapi;
static struct GfxRenderingAPI *sGfxRapi;
static RealitySignalProcessor sRSP[1];
static RealityDisplayProcessor sRDP[1];
static VertexBufferObject sVBO[1];
static RenderingState sRenderState[1];
static OmmHMap sGfxTextureCache = omm_hmap_zero;
struct GfxDimensions gfx_current_dimensions;
static f32 gfx_adjust_for_aspect_ratio;
#if KING_BOBOMB_TEXTURE_FIX
extern const Gfx king_bobomb_seg5_dl_0500B278[];
static bool sKingBobombFix = false;
#endif

static const f32 INV32 = 1.f / 32.f;
static const f32 INV256 = 1.f / 256.f;
static f32 *gVec4fZero3[3] = { gVec4fZero, gVec4fZero, gVec4fZero };
static f32 *gVec4fOne3[3] = { gVec4fOne, gVec4fOne, gVec4fOne };

// Install Counter Strike source
static const u8 sMissingTexture[] = {
    0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
};

//
// STBI
//

#pragma GCC push_options
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION
#pragma GCC pop_options

//
// Texture cache
//

OMM_INLINE u32 string_hash(const char *str) {
    u32 hash = 0;
    for (; *str; str++) {
        hash = (hash * 31) + *str;
    }
    return hash;
}

OMM_INLINE GfxTexture *gfx_texture_find(const char *name) {
#if OMM_CODE_DYNOS
    extern void *dynos_gfx_texture_find(OmmHMap *, struct GfxRenderingAPI *, const void *);
    GfxTexture *tex = (GfxTexture *) dynos_gfx_texture_find(&sGfxTextureCache, sGfxRapi, (const void *) name);
    if (tex) {
        return tex;
    }
#endif
    s32 i = omm_hmap_find(sGfxTextureCache, string_hash(name));
    if (i != -1) {
        return omm_hmap_get(sGfxTextureCache, GfxTexture *, i);
    }
    return NULL;
}

OMM_INLINE GfxTexture *gfx_texture_create(const char *name) {
    GfxTexture *tex = OMM_MEMNEW(GfxTexture, 1);
    tex->hash = string_hash(name);
    tex->id = sGfxRapi->new_texture();
    tex->cms = 0;
    tex->cmt = 0;
    tex->lin = 0;
    sGfxRapi->select_texture(0, tex->id);
    sGfxRapi->set_sampler_parameters(0, false, 0, 0);
    return tex;
}

OMM_INLINE void gfx_texture_insert(GfxTexture *tex) {
    omm_hmap_insert(sGfxTextureCache, tex->hash, tex);
}

//
// Texture load
//

static void gfx_texture_load(const char *filename) {
    u64 imgsize = 0;
    u8 *imgdata = fs_load_file(filename, &imgsize);
    if (imgdata) {
        s32 w, h;
        u8 *data = stbi_load_from_memory(imgdata, imgsize, &w, &h, NULL, 4);
        free(imgdata);
        if (data) {
            sGfxRapi->upload_texture(data, w, h);
            stbi_image_free(data);
            return;
        }
    }
    fprintf(stderr, "Cannot load texture: \"%s\"\n", filename);
    sGfxRapi->upload_texture(sMissingTexture, 4, 4);
}

OMM_INLINE void gfx_texture_import() {
    GfxTexture *tex = gfx_texture_find(sRDP->texLoaded);
    if (OMM_UNLIKELY(!tex)) {
        tex = gfx_texture_create(sRDP->texLoaded);
        gfx_texture_insert(tex);
        OMM_STRING(filename, SYS_MAX_PATH, FS_TEXTUREDIR "/%s.png", sRDP->texLoaded);
        gfx_texture_load(filename);
    }
    sRenderState->texture = tex;
    sGfxRapi->select_texture(0, tex->id);
}

//
// Texture preload
//

static bool gfx_texture_preload(UNUSED void *user, const char *path) {
    if (!strstr(path, ".png") && !strstr(path, ".PNG")) {
        return true;
    }

    // Remove the extension
    OMM_STRING(texname, SYS_MAX_PATH, "%s", path);
    char *dot = strrchr(texname, '.');
    if (dot) *dot = 0;
    
    // Remove the prefix
    char *name = texname;
    if (!strncmp(FS_TEXTUREDIR "/", name, sizeof(FS_TEXTUREDIR "/") - 1)) {
        name += sizeof(FS_TEXTUREDIR "/") - 1;
    }

    // Create, insert and load the texture
    GfxTexture *tex = gfx_texture_find(name);
    if (!tex) {
        tex = gfx_texture_create(name);
        gfx_texture_insert(tex);
        gfx_texture_load(path);
    }
    return true;
}

void gfx_precache_textures(void) {
    fs_walk(FS_TEXTUREDIR, gfx_texture_preload, NULL, true);
}

//
// Gfx
//

static Gfx *sGfxCmd;

static void gfx_run_dl(Gfx *cmd);

static void gfx_noop() {
}

OMM_INLINE void gfx_flush() {
    if (sVBO->length) {
        sGfxRapi->draw_triangles(sVBO->buffer, sVBO->length, sVBO->tris);
        sVBO->length = 0;
        sVBO->tris = 0;
    }
}

static void gfx_geometry_mode_changed() {
    static bool lastDepthTest = false;
    bool depthTest = (sRSP->geometryMode & G_ZBUFFER) != 0;
    if (depthTest != lastDepthTest) {
        gfx_flush();
        sGfxRapi->set_depth_test(depthTest);
        lastDepthTest = depthTest;
    }
}

static void gfx_other_mode_l_changed() {
    static bool lastDepthMask = false;
    bool depthMask = (sRDP->otherModeL & Z_UPD) == Z_UPD;
    if (depthMask != lastDepthMask) {
        gfx_flush();
        sGfxRapi->set_depth_mask(depthMask);
        lastDepthMask = depthMask;
    }
    static bool lastDecalMode = false;
    bool decalMode = (sRDP->otherModeL & ZMODE_DEC) == ZMODE_DEC;
    if (decalMode != lastDecalMode) {
        gfx_flush();
        sGfxRapi->set_zmode_decal(decalMode);
        lastDecalMode = decalMode;
    }
}

OMM_INLINE void gfx_other_mode_h_changed() {
    sRDP->texOffset = 0.5f * ((sRDP->otherModeH & (3U << G_MDSFT_TEXTFILT)) != G_TF_POINT);
}

OMM_INLINE void gfx_tex_tile_changed() {
    sRDP->invTexWidth  = 1.f / ((sRDP->texTile.lrs - sRDP->texTile.uls + 4) >> 2);
    sRDP->invTexHeight = 1.f / ((sRDP->texTile.lrt - sRDP->texTile.ult + 4) >> 2);
    sRDP->texTile.tti++;
}

OMM_INLINE void gfx_viewport_changed() {
    gfx_flush();
    sGfxRapi->set_viewport(sRDP->viewport.x, sRDP->viewport.y, sRDP->viewport.w, sRDP->viewport.h);
}

OMM_INLINE void gfx_scissor_changed(s32 x, s32 y, s32 w, s32 h) {
    gfx_flush();
    sGfxRapi->set_scissor(x, y, w, h);
}

//
// Color combiners
//

static void *gfx_lookup_or_create_shader_program(u32 shaderId) {
    void *prg = sGfxRapi->lookup_shader(shaderId);
    if (!prg) {
        sGfxRapi->unload_shader(sRenderState->shaderProgram);
        prg = sGfxRapi->create_and_load_new_shader(shaderId);
        sRenderState->shaderProgram = prg;
    }
    return prg;
}

static ColorCombiner *gfx_lookup_or_create_color_combiner(u32 id) {
    ASSERT(id); // just in case

    // Last cached
    static ColorCombiner *cc = NULL;
    if (cc && cc->id == id) {
        return cc;
    }

    // Find in cache
    static OmmHMap sGfxColorCombiners = omm_hmap_zero;
    s32 i = omm_hmap_find(sGfxColorCombiners, id);
    if (i != -1) {
        cc = omm_hmap_get(sGfxColorCombiners, ColorCombiner *, i);
        return cc;
    }

    // Create new color combiner
    gfx_flush();
    cc = OMM_MEMNEW(ColorCombiner, 1);
    u8 c[2][4];
    u32 shaderId = (id & 0xFF000000);
    for (s32 i = 0; i != 4; ++i) {
        c[0][i] = (id >> (i * 3)) & 7;
        c[1][i] = (id >> (12 + i * 3)) & 7;
    }
    for (s32 i = 0; i != 2; ++i) {
        if (c[i][0] == c[i][1] || c[i][2] == CC_0) {
            c[i][0] = c[i][1] = c[i][2] = 0;
        }
        u8 inputNumber[8] = {0};
        s32 nextInputNumber = SHADER_INPUT_1;
        for (s32 j = 0; j != 4; ++j) {
            s32 val = 0;
            switch (c[i][j]) {
                case CC_0: break;
                case CC_TEXEL0: val = SHADER_TEXEL0; break;
                case CC_TEXEL1: val = SHADER_TEXEL1; break;
                case CC_TEXEL0A: val = SHADER_TEXEL0A; break;
                case CC_PRIM:
                case CC_SHADE:
                case CC_ENV: {
                    if (inputNumber[c[i][j]] == 0) {
                        cc->mapShaderInput[i][nextInputNumber - 1] = c[i][j];
                        inputNumber[c[i][j]] = nextInputNumber++;
                    }
                    val = inputNumber[c[i][j]];
                } break;
            }
            shaderId |= val << (i * 12 + j * 3);
        }
    }
    cc->id = id;
    cc->prg = gfx_lookup_or_create_shader_program(shaderId);
    omm_hmap_insert(sGfxColorCombiners, id, cc);
    return cc;
}

//
// Matrix
//

static void gfx_sp_matrix_projection_load(Mat4 mat) {
    mtxf_copy(sRSP->mtxP, mat);
}

static void gfx_sp_matrix_projection_mul(Mat4 mat) {
    mtx_mul(sRSP->mtxP, mat, sRSP->mtxP);
}

static void gfx_sp_matrix_modelview_load_push(Mat4 mat) {
    mtx_stack_inc(1);
    mtxf_copy(sRSP->mtx, mat);
    sRSP->lightsChanged = true;
}

static void gfx_sp_matrix_modelview_load_nopush(Mat4 mat) {
    mtxf_copy(sRSP->mtx, mat);
    sRSP->lightsChanged = true;
}

static void gfx_sp_matrix_modelview_mul_push(Mat4 mat) {
    mtx_stack_inc(1);
    mtx_mul(sRSP->mtx, mat, sRSP->mtx - mtx_stack_offset(1));
    sRSP->lightsChanged = true;
}

static void gfx_sp_matrix_modelview_mul_nopush(Mat4 mat) {
    mtx_mul(sRSP->mtx, mat, sRSP->mtx);
    sRSP->lightsChanged = true;
}

static void (*gfx_sp_matrix_[])(Mat4) = {
    gfx_sp_matrix_modelview_mul_nopush,
    gfx_sp_matrix_modelview_mul_push,
    gfx_sp_matrix_modelview_load_nopush,
    gfx_sp_matrix_modelview_load_push,
    gfx_sp_matrix_projection_mul,
    gfx_sp_matrix_projection_mul,
    gfx_sp_matrix_projection_load,
    gfx_sp_matrix_projection_load,
};

static void gfx_sp_matrix() {
    u8 params = GFX_C0(0, 3) ^ G_MTX_PUSH;
    void *mtx = GFX_W1P;
    gfx_sp_matrix_[params](mtx);
    mtx_mul(sRSP->mtxMP, sRSP->mtx, sRSP->mtxP);
}

static void gfx_sp_pop_matrix() {
    u32 count = GFX_W1 >> 6;
    mtx_stack_dec(count);
    mtx_mul(sRSP->mtxMP, sRSP->mtx, sRSP->mtxP);
}

//
// Vertex
//

OMM_INLINE void gfx_sp_vertex_get_light_coeffs(const Light_t *light, Vec3f coeffs) {
    Vec3f lightDir = { light->dir[0] / 127.f, light->dir[1] / 127.f, light->dir[2] / 127.f };
    vec3f_mult_mtx(coeffs, lightDir, (void *) sRSP->mtx);
    vec3f_normalize(coeffs);
}

OMM_INLINE void gfx_sp_vertex_compute_tex_coords(Vertex *v) {
    s32 tu, tv;
    if (sRSP->geometryMode & G_TEXTURE_GEN) {
        s32 dotx = 128 + vec3f_dot(v->n, sRSP->lookAtXCoeffs);
        s32 doty = 128 + vec3f_dot(v->n, sRSP->lookAtYCoeffs);
        tu = (dotx * sRSP->texS) >> 9;
        tv = (doty * sRSP->texT) >> 9;
    } else {
        tu = (v->v->tc[0] * sRSP->texS) >> 16;
#if KING_BOBOMB_TEXTURE_FIX
        tv = (v->v->tc[1] * sRSP->texT) >> (sKingBobombFix ? 15 : 16);
#else
        tv = (v->v->tc[1] * sRSP->texT) >> 16;
#endif
    }
    v->tc[0] = tex_u_normalize(tu);
    v->tc[1] = tex_v_normalize(tv);
    v->tti = sRDP->texTile.tti;
}

static void gfx_sp_vertex() {
    s32 vtxCount = GFX_C0(12, 8);
    s32 vtxDest = GFX_C0(1, 7) - vtxCount;
    const Vtx *vtx = GFX_W1P;
    Vertex *d = &sRSP->vtx[vtxDest];

    // If the lights changed, re-compute light coeffs
    if (sRSP->lightsChanged && (sRSP->geometryMode & G_LIGHTING)) {
        static const Light_t sLookAtX = { { 0, 0, 0 }, 0, { 0, 0, 0 }, 0, { 127, 0, 0 }, 0 };
        static const Light_t sLookAtY = { { 0, 0, 0 }, 0, { 0, 0, 0 }, 0, { 0, 127, 0 }, 0 };
        gfx_sp_vertex_get_light_coeffs(&sLookAtX, sRSP->lookAtXCoeffs);
        gfx_sp_vertex_get_light_coeffs(&sLookAtY, sRSP->lookAtYCoeffs);
        gfx_sp_vertex_get_light_coeffs(&sRSP->light, sRSP->lightCoeffs);
        sRSP->lightsChanged = false;
    }

    // Load current matrix into XMM registers
    __m128 m0 = _mm_load_ps(sRSP->mtxMP[0]);
    __m128 m1 = _mm_load_ps(sRSP->mtxMP[1]);
    __m128 m2 = _mm_load_ps(sRSP->mtxMP[2]);
    __m128 m3 = _mm_load_ps(sRSP->mtxMP[3]);

    // Transform vertices
    for (s32 i = vtxCount; i; --i, vtx++, d++) {
        const Vtx_t *v = &vtx->v;
        d->v = v;

        // Load current vertex into XMM registers and transform
        __m128 v0 = _mm_set1_ps(v->ob[0]);
        __m128 v1 = _mm_set1_ps(v->ob[1]);
        __m128 v2 = _mm_set1_ps(v->ob[2]);
        __m128 v3 = _mm_set1_ps(1.f);
        __m128 d3 = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(m0, v0), _mm_mul_ps(m1, v1)),
            _mm_add_ps(_mm_mul_ps(m2, v2), _mm_mul_ps(m3, v3))
        );
        _mm_store_ps(d->p, d3);
        d->p[0] = gfx_adjust_x_for_aspect_ratio(d->p[0]);

        // Vertex normal
        d->n[0] = vtx->n.n[0];
        d->n[1] = vtx->n.n[1];
        d->n[2] = vtx->n.n[2];

        // Colors
        if (sRSP->geometryMode & G_LIGHTING) {
            s32 rl = sRSP->light.col[0];
            s32 gl = sRSP->light.col[1];
            s32 bl = sRSP->light.col[2];
            s32 ra = sRSP->lightAmb.col[0];
            s32 ga = sRSP->lightAmb.col[1];
            s32 ba = sRSP->lightAmb.col[2];

            // Performing integer operations makes things faster
            s32 intensity = clamp_s(vec3f_dot(d->n, sRSP->lightCoeffs), 0, 128);
            s32 r = (ra * (128 - intensity) + ((rl * intensity * 5) >> 2)) >> 7;
            s32 g = (ga * (128 - intensity) + ((gl * intensity * 5) >> 2)) >> 7;
            s32 b = (ba * (128 - intensity) + ((bl * intensity * 5) >> 2)) >> 7;
            d->color[0] = clamp_s(r, 0, 256) * INV256;
            d->color[1] = clamp_s(g, 0, 256) * INV256;
            d->color[2] = clamp_s(b, 0, 256) * INV256;
        } else {
            d->color[0] = v->cn[0] * INV256;
            d->color[1] = v->cn[1] * INV256;
            d->color[2] = v->cn[2] * INV256;
        }

        // Texture coords
        if (sRSP->texOn) {
            gfx_sp_vertex_compute_tex_coords(d);
        }
        
        // Trivial clip rejection
        d->rej = (
            ((d->p[0] < -d->p[3]) << 0) |
            ((d->p[0] > +d->p[3]) << 1) |
            ((d->p[1] < -d->p[3]) << 2) |
            ((d->p[1] > +d->p[3]) << 3) |
            ((d->p[2] < -d->p[3]) << 4) |
            ((d->p[2] > +d->p[3]) << 5)
        );
        
        // Fog/Alpha
        if (sRSP->geometryMode & G_FOG) {
            f32 winv = 1.f / max_f(d->p[3], 0.001f);
            f32 fogz = clamp_f(d->p[2] * winv * sRSP->fogMul + sRSP->fogOff, 0, 256);
            d->color[3] = fogz * INV256;
        } else {
            d->color[3] = v->cn[3] * INV256;
        }
    }
}

//
// Triangle
//

static void gfx_sp_tri(u8 v1i, u8 v2i, u8 v3i) {
    Vertex *v1 = &sRSP->vtx[v1i];
    Vertex *v2 = &sRSP->vtx[v2i];
    Vertex *v3 = &sRSP->vtx[v3i];
    if (v1->rej & v2->rej & v3->rej) {
        return;
    }
    
    // Culling test
    if (sRSP->geometryMode & G_CULL_BOTH) {
        f32 dxv = (v2->p[0] / v2->p[3]);
        f32 dyv = (v2->p[1] / v2->p[3]);
        f32 dx1 = (v1->p[0] / v1->p[3]) - dxv;
        f32 dy1 = (v1->p[1] / v1->p[3]) - dyv;
        f32 dx2 = (v3->p[0] / v3->p[3]) - dxv;
        f32 dy2 = (v3->p[1] / v3->p[3]) - dyv;
        f32 cross = dx1 * dy2 - dy1 * dx2;
        
        // If one vertex lies behind the eye, negating cross will give the correct result.
        // If all vtx lie behind the eye, the triangle will be rejected anyway.
        if ((v1->p[3] < 0) ^ (v2->p[3] < 0) ^ (v3->p[3] < 0)) {
            cross = -cross;
        }

        if (((sRSP->geometryMode & G_CULL_FRONT) && cross <= 0) ||
            ((sRSP->geometryMode & G_CULL_BACK ) && cross >= 0)) {
            return;
        }
    }
    
    // Color combiner
    static bool useFog, texEdge, useNoise, useAlpha;
    static u32 ccId, geometryMode = 0, combineMode = 0, otherModeL = 0;
    if (geometryMode != sRSP->geometryMode || combineMode != sRDP->combineMode || otherModeL != sRDP->otherModeL) {
        gfx_flush();
        useFog = (sRDP->otherModeL >> 30) == G_BL_CLR_FOG;
        texEdge = (sRDP->otherModeL & CVG_X_ALPHA) == CVG_X_ALPHA;
        useNoise = (sRDP->otherModeL & G_AC_DITHER) == G_AC_DITHER;
        useAlpha = texEdge || ((sRDP->otherModeL & (G_BL_A_MEM << 18)) == 0);
        texEdge &= !(sRSP->geometryMode & G_TEXTURE_ALPHA);
        ccId = sRDP->combineMode;
        if (useFog) ccId |= SHADER_OPT_FOG;
        if (texEdge) ccId |= SHADER_OPT_TEXTURE_EDGE;
        if (useNoise) ccId |= SHADER_OPT_NOISE;
        if (useAlpha) ccId |= SHADER_OPT_ALPHA;
        else ccId &= ~0xFFF000;
        sGfxRapi->set_use_alpha(useAlpha);
        geometryMode = sRSP->geometryMode;
        combineMode = sRDP->combineMode;
        otherModeL = sRDP->otherModeL;
    }
    ColorCombiner *cc = gfx_lookup_or_create_color_combiner(ccId);
#if GL_TRANSPARENCY
    if ((sRDP->otherModeL & G_AC_COVERAGE) == G_AC_COVERAGE) {
        glDepthMask(GL_TRUE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        glEnable(GL_SAMPLE_ALPHA_TO_ONE);
    } else {
        glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        glDisable(GL_SAMPLE_ALPHA_TO_ONE);
    }
#endif

    // Shader
    if (cc->prg != sRenderState->shaderProgram) {
        gfx_flush();
        sGfxRapi->unload_shader(sRenderState->shaderProgram);
        sGfxRapi->load_shader(cc->prg);
        sRenderState->shaderProgram = cc->prg;
    }

    // Textures
    u8 nInputs; bool usedTextures[2];
    sGfxRapi->shader_get_info(cc->prg, &nInputs, usedTextures);
    if (*usedTextures) {
        if (sRDP->texChanged) {
            gfx_flush();
            gfx_texture_import();
            sRDP->texChanged = false;
        }
        bool lin = configFiltering && ((sRDP->otherModeH & (3U << G_MDSFT_TEXTFILT)) != G_TF_POINT);
        if (lin != sRenderState->texture->lin || sRDP->texTile.cms != sRenderState->texture->cms || sRDP->texTile.cmt != sRenderState->texture->cmt) {
            gfx_flush();
            sGfxRapi->set_sampler_parameters(0, lin, sRDP->texTile.cms, sRDP->texTile.cmt);
            sRenderState->texture->lin = lin;
            sRenderState->texture->cms = sRDP->texTile.cms;
            sRenderState->texture->cmt = sRDP->texTile.cmt;
        }
    }
    
    // Vertex colors
    Vertex *va[3] = { v1, v2, v3 };
    f32 *va_colors[3] = { v1->color, v2->color, v3->color };
    f32 **colors[8][2]; // input, rgb/a
    for (s32 j = 0; j != nInputs; ++j) {

        // RGB
        switch (cc->mapShaderInput[0][j]) {
            case CC_PRIM:  colors[j][0] = sRDP->primColors; break;
            case CC_SHADE: colors[j][0] = va_colors; break;
            case CC_ENV:   colors[j][0] = sRDP->envColors; break;
            default:       colors[j][0] = gVec4fZero3; break;
        }
        
        // Alpha
        if (useAlpha) {
            switch (cc->mapShaderInput[1][j]) {
                case CC_PRIM:  colors[j][1] = sRDP->primColors; break;
                case CC_SHADE: colors[j][1] = (useFog ? gVec4fOne3 : va_colors); break;
                case CC_ENV:   colors[j][1] = sRDP->envColors; break;
                default:       colors[j][1] = gVec4fZero3; break;
            }
        }
    }
    
    // Fill VBO
    f32 *buffer = sVBO->buffer + sVBO->length;
    for (s32 i = 0; i != 3; ++i) {

        // x, y, z, w
#if GFX_OPEN_GL
        vec4f_copy(buffer, va[i]->p);
        buffer += 4;
#else
        f32 w = va[i]->p[3];
        f32 z = (va[i]->p[2] + w) / 2.f;
        *(buffer++) = va[i]->p[0];
        *(buffer++) = va[i]->p[1];
        *(buffer++) = z;
        *(buffer++) = w;
#endif
        
        // Texture coords
        if (*usedTextures) {
            if (OMM_UNLIKELY(va[i]->tti != sRDP->texTile.tti)) {
                gfx_sp_vertex_compute_tex_coords(va[i]);
            }
            vec2f_copy(buffer, va[i]->tc);
            buffer += 2;
        }
        
        // Fog
        if (useFog) {
            vec3f_copy(buffer, sRDP->fogColor);
            buffer += 3;
            *(buffer++) = va[i]->color[3];
        }
        
        // Colors
        for (s32 j = 0; j != nInputs; ++j) {
            vec3f_copy(buffer, colors[j][0][i]);
            buffer += 3;
            if (useAlpha) {
                *(buffer++) = colors[j][1][i][3];
            }
        }
    }
    
    // Draw triangles
    sVBO->length = (s32) (buffer - sVBO->buffer);
    if (++sVBO->tris == GFX_MAX_TRIS) {
        gfx_flush();
    }
}

static void gfx_sp_tri1() {
    gfx_sp_tri(GFX_C0(16, 8) / 2, GFX_C0(8, 8) / 2, GFX_C0(0, 8) / 2);
}

static void gfx_sp_tri2() {
    gfx_sp_tri(GFX_C0(16, 8) / 2, GFX_C0(8, 8) / 2, GFX_C0(0, 8) / 2);
    gfx_sp_tri(GFX_C1(16, 8) / 2, GFX_C1(8, 8) / 2, GFX_C1(0, 8) / 2);
}

//
// RSP commands
//

OMM_INLINE void gfx_sp_set_other_mode(u32 shift, u32 bits, u64 mode) {
    u64 mask = ((1llu << bits) - 1llu) << shift;
    u64 omhl = ((u64) sRDP->otherModeL) | (((u64) sRDP->otherModeH) << 32llu);
    omhl = (omhl & ~mask) | mode;
    sRDP->otherModeL = (u32) omhl;
    sRDP->otherModeH = (u32) (omhl >> 32);
    gfx_other_mode_l_changed();
    gfx_other_mode_h_changed();
}

static void gfx_sp_set_other_mode_l() {
    gfx_sp_set_other_mode(31 - GFX_C0(8, 8) - GFX_C0(0, 8), GFX_C0(0, 8) + 1, GFX_W1);
}

static void gfx_sp_set_other_mode_h() {
    gfx_sp_set_other_mode(63 - GFX_C0(8, 8) - GFX_C0(0, 8), GFX_C0(0, 8) + 1, (u64) GFX_W1 << 32);
}

static void gfx_sp_movemem() {
    u8 index = GFX_C0(0, 8);
    void *data = GFX_W1P;
    switch (index) {
        case G_MV_VIEWPORT: {
            const Vp_t *viewport = data;
            f32 w = viewport->vscale[0] / 2.f;
            f32 h = viewport->vscale[1] / 2.f;
            f32 x = (viewport->vtrans[0] / 4.f) - w / 2.f;
            f32 y = SCREEN_HEIGHT - ((viewport->vtrans[1] / 4.f) + h / 2.f);
            sRDP->viewport.x = x * GFX_RATIO_X;
            sRDP->viewport.y = y * GFX_RATIO_Y;
            sRDP->viewport.w = w * GFX_RATIO_X;
            sRDP->viewport.h = h * GFX_RATIO_Y;
            gfx_viewport_changed();
        } break;

        case G_MV_LIGHT: {
            if (GFX_C0(8, 8) == 6) {
                OMM_MEMCPY(&sRSP->light, data, sizeof(Light_t));
            } else {
                OMM_MEMCPY(&sRSP->lightAmb, data, sizeof(Light_t));
            }
            break;
        }
    }
}

static void gfx_sp_moveword() {
    if (GFX_C0(16, 8) == G_MW_FOG) {
        u32 data = GFX_W1;
        sRSP->fogMul = (s16) (data >> 16);
        sRSP->fogOff = (s16) data;
    }
}

static void gfx_sp_geometry_mode() {
    u32 clr = GFX_C0(0, 24);
    u32 set = GFX_W1;
    sRSP->geometryMode &= clr;
    sRSP->geometryMode |= set;
    gfx_geometry_mode_changed();
}

static void gfx_sp_texture() {
    u16 on = GFX_C0(1, 1);
    u16 sc = GFX_C1(16, 16);
    u16 tc = GFX_C1(0, 16);
    sRSP->texOn = on;
    sRSP->texS = sc;
    sRSP->texT = tc;
}

static void gfx_sp_dl() {
    if (GFX_C0(16, 1) == 0) {
        Gfx *cmd = sGfxCmd;
        gfx_run_dl(GFX_W1P);
        sGfxCmd = cmd;
    } else {
        sGfxCmd = GFX_W1P;
        sGfxCmd--;
    }
}

static void gfx_sp_end_dl() {
    sGfxCmd = NULL;
}

//
// RDP commands
//

static void gfx_dp_set_combine_mode() {
    u8 rgb_a = GFX_C0(20, 4);
    u8 rgb_b = GFX_C1(28, 4);
    u8 rgb_c = GFX_C0(15, 5);
    u8 rgb_d = GFX_C1(15, 3);
    u8 alpha_a = GFX_C0(12, 3);
    u8 alpha_b = GFX_C1(12, 3);
    u8 alpha_c = GFX_C0(9, 3);
    u8 alpha_d = GFX_C1(9, 3);
    sRDP->combineMode = gfx_color_comb(rgb_a, rgb_b, rgb_c, rgb_d) | (gfx_color_comb(alpha_a, alpha_b, alpha_c, alpha_d) << 12);
}

static void gfx_dp_set_env_color() {
    u8 r = GFX_C1(24, 8);
    u8 g = GFX_C1(16, 8);
    u8 b = GFX_C1(8, 8);
    u8 a = GFX_C1(0, 8);
    sRDP->envColor[0] = r * INV256;
    sRDP->envColor[1] = g * INV256;
    sRDP->envColor[2] = b * INV256;
    sRDP->envColor[3] = a * INV256;
}

static void gfx_dp_set_prim_color() {
    u8 r = GFX_C1(24, 8);
    u8 g = GFX_C1(16, 8);
    u8 b = GFX_C1(8, 8);
    u8 a = GFX_C1(0, 8);
    sRDP->primColor[0] = r * INV256;
    sRDP->primColor[1] = g * INV256;
    sRDP->primColor[2] = b * INV256;
    sRDP->primColor[3] = a * INV256;
}

static void gfx_dp_set_fog_color() {
    u8 r = GFX_C1(24, 8);
    u8 g = GFX_C1(16, 8);
    u8 b = GFX_C1(8, 8);
    sRDP->fogColor[0] = r * INV256;
    sRDP->fogColor[1] = g * INV256;
    sRDP->fogColor[2] = b * INV256;
}

static void gfx_dp_set_fill_color() {
    u32 packed_color = GFX_W1;
    u32 r = (packed_color >> 11) & 0x1F;
    u32 g = (packed_color >>  6) & 0x1F;
    u32 b = (packed_color >>  1) & 0x1F;
    u32 a = (packed_color >>  0) & 0x01;
    sRDP->fillColor[0] = r * INV32;
    sRDP->fillColor[1] = g * INV32;
    sRDP->fillColor[2] = b * INV32;
    sRDP->fillColor[3] = a;
}

static void gfx_dp_set_z_image() {
    sRDP->zBufAddress = GFX_W1P;
}

static void gfx_dp_set_color_image() {
    sRDP->colorImageAddress = GFX_W1P;
}

static void gfx_dp_set_scissor() {
    s32 ulx = GFX_C0(12, 12) >> 2;
    s32 uly = GFX_C0( 0, 12) >> 2;
    s32 lrx = GFX_C1(12, 12) >> 2;
    s32 lry = GFX_C1( 0, 12) >> 2;
    s32 x = GFX_RATIO_X * ulx;
    s32 y = GFX_RATIO_Y * (SCREEN_HEIGHT - lry);
    s32 w = GFX_RATIO_X * (lrx - ulx);
    s32 h = GFX_RATIO_Y * (lry - uly);
    gfx_scissor_changed(x, y, w, h);
}

static void gfx_dp_set_texture_image() {
    void *addr = GFX_W1P;
    sRDP->texToLoad = addr;
}

static void gfx_dp_set_tile() {
    if (GFX_C1(24, 3) == G_TX_RENDERTILE) {
        u8 cms = GFX_C1(8, 2);
        u8 cmt = GFX_C1(18, 2);
        sRDP->texTile.cms = cms;
        sRDP->texTile.cmt = cmt;
        sRDP->texChanged = true;
    }
}

static void gfx_dp_set_tile_size() {
    if (OMM_LIKELY(GFX_C1(24, 3) == G_TX_RENDERTILE)) {
        u16 uls = GFX_C0(12, 12);
        u16 ult = GFX_C0(0, 12);
        u16 lrs = GFX_C1(12, 12);
        u16 lrt = GFX_C1(0, 12);
        sRDP->texTile.uls = uls;
        sRDP->texTile.ult = ult;
        sRDP->texTile.lrs = lrs;
        sRDP->texTile.lrt = lrt;
        sRDP->texChanged = true;
        gfx_tex_tile_changed();
    }
}

static void gfx_dp_load_block() {
    if (OMM_LIKELY(GFX_C1(24, 3) == G_TX_LOADTILE)) {
        sRDP->texLoaded = sRDP->texToLoad;
        sRDP->texChanged = true;
    }
}

static void gfx_dp_load_tile() {
    if (OMM_LIKELY(GFX_C1(24, 3) == G_TX_LOADTILE)) {
        u32 uls = GFX_C0(12, 12);
        u32 ult = GFX_C0(0, 12);
        u32 lrs = GFX_C1(12, 12);
        u32 lrt = GFX_C1(0, 12);
        sRDP->texTile.uls = uls;
        sRDP->texTile.ult = ult;
        sRDP->texTile.lrs = lrs;
        sRDP->texTile.lrt = lrt;
        sRDP->texLoaded = sRDP->texToLoad;
        sRDP->texChanged = true;
        gfx_tex_tile_changed();
    }
}

static void gfx_dp_draw_rectangle(s32 ulx, s32 uly, s32 lrx, s32 lry) {
    f32 ulxf = +(ulx / (2.f * SCREEN_WIDTH )) - 1.f;
    f32 ulyf = -(uly / (2.f * SCREEN_HEIGHT)) + 1.f;
    f32 lrxf = +(lrx / (2.f * SCREEN_WIDTH )) - 1.f;
    f32 lryf = -(lry / (2.f * SCREEN_HEIGHT)) + 1.f;
    ulxf = gfx_adjust_x_for_aspect_ratio(ulxf);
    lrxf = gfx_adjust_x_for_aspect_ratio(lrxf);
    
    Vertex* ul = &sRSP->vtx[GFX_MAX_VERTS + 0];
    Vertex* ll = &sRSP->vtx[GFX_MAX_VERTS + 1];
    Vertex* lr = &sRSP->vtx[GFX_MAX_VERTS + 2];
    Vertex* ur = &sRSP->vtx[GFX_MAX_VERTS + 3];
    ul->p[0] = ulxf; ul->p[1] = ulyf; ul->p[2] = -1.f; ul->p[3] = 1.f;
    ll->p[0] = ulxf; ll->p[1] = lryf; ll->p[2] = -1.f; ll->p[3] = 1.f;
    lr->p[0] = lrxf; lr->p[1] = lryf; lr->p[2] = -1.f; lr->p[3] = 1.f;
    ur->p[0] = lrxf; ur->p[1] = ulyf; ur->p[2] = -1.f; ur->p[3] = 1.f;
    
    // The coordinates for texture rectangle shall bypass the viewport setting
    XYWH defViewport = { 0, 0, gfx_current_dimensions.width, gfx_current_dimensions.height };
    XYWH lastViewport = sRDP->viewport;
    u32 lastGeometryMode = sRSP->geometryMode;
    u32 lastOtherModeH = sRDP->otherModeH;

    // Draw rectangle
    sRSP->geometryMode = 0;
    sRDP->otherModeH = (sRDP->otherModeH & ~(3U << G_MDSFT_TEXTFILT)) | G_TF_POINT;
    sRDP->viewport = defViewport;
    gfx_geometry_mode_changed();
    gfx_other_mode_h_changed();
    gfx_viewport_changed();
    gfx_sp_tri(GFX_MAX_VERTS + 0, GFX_MAX_VERTS + 1, GFX_MAX_VERTS + 3);
    gfx_sp_tri(GFX_MAX_VERTS + 1, GFX_MAX_VERTS + 2, GFX_MAX_VERTS + 3);

    // Restore state
    sRSP->geometryMode = lastGeometryMode;
    sRDP->otherModeH = lastOtherModeH;
    sRDP->viewport = lastViewport;
    gfx_geometry_mode_changed();
    gfx_other_mode_h_changed();
    gfx_viewport_changed();
}

static void gfx_dp_texture_rectangle() {
    bool flip = GFX_OP == G_TEXRECTFLIP;
    s32 lrx = ((s32) (GFX_C0(0, 24) << 8)) >> 8;
    s32 lry = ((s32) (GFX_C1(0, 24) << 8)) >> 8;
    ++sGfxCmd;
    s32 ulx = ((s32) (GFX_C0(0, 24) << 8)) >> 8;
    s32 uly = ((s32) (GFX_C1(0, 24) << 8)) >> 8;
    ++sGfxCmd;
    s32 uls = GFX_C0(16, 16);
    s32 ult = GFX_C0(0, 16);
    s32 dsdx = GFX_C1(16, 16);
    s32 dtdy = GFX_C1(0, 16);

    // Don't ask
    u32 lastCombineMode = sRDP->combineMode;
    if ((sRDP->otherModeH & (3U << G_MDSFT_CYCLETYPE)) == G_CYC_COPY) {
        sRDP->combineMode = gfx_color_comb(0, 0, 0, G_CCMUX_TEXEL0) | (gfx_color_comb(0, 0, 0, G_ACMUX_TEXEL0) << 12);
        lrx += 1 << 2;
        lry += 1 << 2;
        dsdx >>= 2;
    }

    dsdx *= (flip ? -1 : +1);
    dtdy *= (flip ? -1 : +1);
    s16 w = (flip ? lry - uly : lrx - ulx);
    s16 h = (flip ? lrx - ulx : lry - uly);
    f32 lrs = ((uls << 7) + dsdx * w) >> 7;
    f32 lrt = ((ult << 7) + dtdy * h) >> 7;
    
    Vertex* ul = &sRSP->vtx[GFX_MAX_VERTS + 0];
    Vertex* ll = &sRSP->vtx[GFX_MAX_VERTS + 1];
    Vertex* lr = &sRSP->vtx[GFX_MAX_VERTS + 2];
    Vertex* ur = &sRSP->vtx[GFX_MAX_VERTS + 3];
    f32 ulsf = tex_u_normalize(uls);
    f32 ultf = tex_v_normalize(ult);
    f32 lrsf = tex_u_normalize(lrs);
    f32 lrtf = tex_v_normalize(lrt);
    ul->tc[0] = ulsf;
    ul->tc[1] = ultf;
    lr->tc[0] = lrsf;
    lr->tc[1] = lrtf;
    ll->tc[0] = flip ? lrsf : ulsf;
    ll->tc[1] = flip ? ultf : lrtf;
    ur->tc[0] = flip ? ulsf : lrsf;
    ur->tc[1] = flip ? lrtf : ultf;
    ul->tti = lr->tti = ll->tti = ur->tti = sRDP->texTile.tti;

    gfx_dp_draw_rectangle(ulx, uly, lrx, lry);
    sRDP->combineMode = lastCombineMode;
}

static void gfx_dp_fill_rectangle() {
    s32 lrx = ((s32) (GFX_C0(0, 24) << 8)) >> 8;
    s32 lry = ((s32) (GFX_C1(0, 24) << 8)) >> 8;
    ++sGfxCmd;
    s32 ulx = ((s32) (GFX_C0(0, 24) << 8)) >> 8;
    s32 uly = ((s32) (GFX_C1(0, 24) << 8)) >> 8;

    // Don't clear the Z buffer here since we already did it with glClear
    if (sRDP->colorImageAddress == sRDP->zBufAddress) {
        return;
    }

    // ¯\_(ツ)_/¯
    u32 mode = (sRDP->otherModeH & (3U << G_MDSFT_CYCLETYPE));
    if (mode == G_CYC_COPY || mode == G_CYC_FILL) {
        lrx += 1 << 2;
        lry += 1 << 2;
    }

    vec4f_copy(sRSP->vtx[GFX_MAX_VERTS + 0].color, sRDP->fillColor);
    vec4f_copy(sRSP->vtx[GFX_MAX_VERTS + 1].color, sRDP->fillColor);
    vec4f_copy(sRSP->vtx[GFX_MAX_VERTS + 2].color, sRDP->fillColor);
    vec4f_copy(sRSP->vtx[GFX_MAX_VERTS + 3].color, sRDP->fillColor);
    
    u32 lastCombineMode = sRDP->combineMode;
    sRDP->combineMode = gfx_color_comb(0, 0, 0, G_CCMUX_SHADE) | (gfx_color_comb(0, 0, 0, G_ACMUX_SHADE) << 12);
    gfx_dp_draw_rectangle(ulx, uly, lrx, lry);
    sRDP->combineMode = lastCombineMode;
}

//
// Gfx run
//

static void (*sGfxCmdTable[])() = {
    gfx_noop,
    gfx_sp_vertex,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_sp_tri1,
    gfx_sp_tri2,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_sp_texture,
    gfx_sp_pop_matrix,
    gfx_sp_geometry_mode,
    gfx_sp_matrix,
    gfx_sp_moveword,
    gfx_sp_movemem,
    gfx_noop,
    gfx_sp_dl,
    gfx_sp_end_dl,
    gfx_noop,
    gfx_noop,
    gfx_sp_set_other_mode_l,
    gfx_sp_set_other_mode_h,
    gfx_dp_texture_rectangle,
    gfx_dp_texture_rectangle,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_dp_set_scissor,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_noop,
    gfx_dp_set_tile_size,
    gfx_dp_load_block,
    gfx_dp_load_tile,
    gfx_dp_set_tile,
    gfx_dp_fill_rectangle,
    gfx_dp_set_fill_color,
    gfx_dp_set_fog_color,
    gfx_noop,
    gfx_dp_set_prim_color,
    gfx_dp_set_env_color,
    gfx_dp_set_combine_mode,
    gfx_dp_set_texture_image,
    gfx_dp_set_z_image,
    gfx_dp_set_color_image,
};

static void gfx_run_dl(Gfx *cmd) {
#if KING_BOBOMB_TEXTURE_FIX
    sKingBobombFix = (cmd == king_bobomb_seg5_dl_0500B278);
#endif
    for (sGfxCmd = cmd;; sGfxCmd++) {
        sGfxCmdTable[GFX_OP & 0x3F]();
        if (OMM_UNLIKELY(!sGfxCmd)) {
            return;
        }
    }
}

static void __gfx_start_frame() {
    omm_debug_start_counter(OMM_COUNTER_FRM);
    sGfxWapi->handle_events();
    sGfxWapi->get_dimensions(&gfx_current_dimensions.width, &gfx_current_dimensions.height);
    gfx_current_dimensions.aspect_ratio = (f32) gfx_current_dimensions.width / (f32) gfx_current_dimensions.height;
    gfx_adjust_for_aspect_ratio = (4.f / 3.f) / gfx_current_dimensions.aspect_ratio;
}

static void __gfx_end_frame() {
    omm_debug_end_counter(OMM_COUNTER_FRM);
    if (!sRenderState->dropFrame) {
        sGfxRapi->finish_render();
        sGfxWapi->swap_buffers_end();
    }
}

//
// Frame interpolation
//

s32 gNumInterpolatedFrames;

#if defined(WAPI_DXGI)
static void gfx_dxgi_handle_events() {
    static s32 sNumInterpolatedFrames = 0;
    extern u64 *frame_timestamp;
    if (sNumInterpolatedFrames != gNumInterpolatedFrames) {
        *frame_timestamp = 0;
    }
    sNumInterpolatedFrames = gNumInterpolatedFrames;
}
#endif

static void gfx_init_frame_interpolation() {
    if (omm_is_main_menu() || omm_is_game_paused()) {
        gNumInterpolatedFrames = 1;
    } else {
        gNumInterpolatedFrames = gOmmFPS + 1;
    }
}

extern void gfx_patch_interpolated_frame_mtx(s32 k);
extern void gfx_patch_interpolated_frame_transition(s32 k);
extern void gfx_patch_interpolated_frame_dialog(s32 k);
extern void gfx_patch_interpolated_frame_hud(s32 k);
extern void gfx_patch_interpolated_frame_effects(s32 k);
extern void gfx_patch_interpolated_frame_paintings(s32 k);
static void gfx_patch_interpolated_frame(s32 k) {
    gfx_patch_interpolated_frame_mtx(k);
    gfx_patch_interpolated_frame_transition(k);
    gfx_patch_interpolated_frame_dialog(k);
    gfx_patch_interpolated_frame_hud(k);
    gfx_patch_interpolated_frame_effects(k);
    gfx_patch_interpolated_frame_paintings(k);
}

static void gfx_render_interpolated_frames() {
    for (s32 k = 1; k < gNumInterpolatedFrames; ++k) {
        __gfx_start_frame();
        gfx_patch_interpolated_frame(k);
        run_display_list(gGfxSPTask);
        __gfx_end_frame();
    }
}

static void gfx_set_config() {
#if OMM_GAME_IS_R96A
    config60FPS = false;
    configInternalResolutionBool = false;
    configCustomInternalResolution = 1;
    configForce4by3 = false;
#endif
#if defined(RAPI_RT64)
    configRT64TargetFPS = 30;
#endif
}

//
// Public
//

struct GfxRenderingAPI *gfx_get_current_rendering_api(void) {
    return sGfxRapi;
}

void gfx_init(struct GfxWindowManagerAPI *wapi, struct GfxRenderingAPI *rapi, const char *window_title) {
    sGfxWapi = wapi;
    sGfxRapi = rapi;
    sGfxWapi->init(window_title);
    sGfxRapi->init();
#if defined(WAPI_DXGI)
    sGfxWapi->handle_events = gfx_dxgi_handle_events;
#endif
    sRDP->envColors[0] = sRDP->envColor;
    sRDP->envColors[1] = sRDP->envColor;
    sRDP->envColors[2] = sRDP->envColor;
    sRDP->primColors[0] = sRDP->primColor;
    sRDP->primColors[1] = sRDP->primColor;
    sRDP->primColors[2] = sRDP->primColor;
    static u32 sPrecompiledShaders[] = {
        0x01200200, 0x00000045, 0x00000200, 0x01200a00,
        0x00000a00, 0x01a00045, 0x00000551, 0x01045045,
        0x05a00a00, 0x01200045, 0x05045045, 0x01045a00,
        0x01a00a00, 0x0000038d, 0x01081081, 0x0120038d,
        0x03200045, 0x03200a00, 0x01a00a6f, 0x01141045,
        0x07a00a00, 0x05200200, 0x03200200, 0x09200200,
        0x0920038d, 0x09200045
    };
    for (s32 i = 0; i != (s32) OMM_ARRAY_SIZE(sPrecompiledShaders); ++i) {
        gfx_lookup_or_create_shader_program(sPrecompiledShaders[i]);
    }
}

void gfx_start_frame(void) {
    gfx_init_frame_interpolation();
    __gfx_start_frame();
    gfx_set_config();
}

void gfx_run(Gfx *commands) {
    sRSP->mtx = (f32 *) &sRSP->mtxStack[8];
    sRSP->lightsChanged = true;
    if (!sGfxWapi->start_frame()) {
        sRenderState->dropFrame = true;
        return;
    }
    sRenderState->dropFrame = false;
    sGfxRapi->start_frame();
    omm_debug_start_counter(OMM_COUNTER_GFX);
    gfx_run_dl(commands);
    omm_debug_end_counter(OMM_COUNTER_GFX);
    omm_debug_start_counter(OMM_COUNTER_RDR);
    gfx_flush();
    sGfxRapi->end_frame();
    sGfxWapi->swap_buffers_begin();
    omm_debug_end_counter(OMM_COUNTER_RDR);
}

void gfx_end_frame(void) {
    __gfx_end_frame();
    gfx_render_interpolated_frames();
    gfx_set_config();
}

void gfx_shutdown(void) {
    if (sGfxRapi) {
        if (sGfxRapi->shutdown) sGfxRapi->shutdown();
        sGfxRapi = NULL;
    }
    if (sGfxWapi) {
        if (sGfxWapi->shutdown) sGfxWapi->shutdown();
        sGfxWapi = NULL;
    }
}

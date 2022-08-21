#define GFX_PC_C
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include <unistd.h>
#if OMM_WAPI_DXGI
#include <windows.h>
#endif

#ifdef DEBUG
#define ASSERT(x) SDL_assert(x)
#else
#define ASSERT(x)
#endif

#if OMM_GAME_IS_SM64
#define LEVEL_PALETTES 1
#else
#define LEVEL_PALETTES 0
#endif

#if OMM_GAME_IS_R96X && OMM_RAPI_GL && defined(TRANSPARENCY_GL)
#include "GL/glew.h"
#define GL_TRANSPARENCY 1
#else
#define GL_TRANSPARENCY 0
#endif

#define GAME_UPDATES_PER_SECOND             30
#define GAME_UPDATE_DURATION                (1.0 / GAME_UPDATES_PER_SECOND)
#if OMM_WAPI_DXGI
#define UNLIMITED_FPS                       1000
#else
#define UNLIMITED_FPS                       1000000
#endif

#define GFX_TEXTURE_RAW_FILEPATH            "res/gfx.raw"
#define GFX_RATIO_X                         (gfx_current_dimensions.width / (f32) SCREEN_WIDTH)
#define GFX_RATIO_Y                         (gfx_current_dimensions.height / (f32) SCREEN_HEIGHT)
#define GFX_MAX_VERTS                       64
#define GFX_MAX_TRIS                        256
#define GFX_MAX_TEXTURES                    8192
#define GFX_NO_FOG                          (omm_world_is_flooded() || omm_world_is_frozen() || omm_world_is_shadow())

#define GFX_OP                              ((u8) (sGfxCmd->words.w0 >> 24))
#define GFX_W0                              sGfxCmd->words.w0
#define GFX_W1                              sGfxCmd->words.w1
#define GFX_W1P                             ((void *) sGfxCmd->words.w1)
#define GFX_C0(off, siz)                    ((sGfxCmd->words.w0 >> (off)) & ((1U << siz) - 1))
#define GFX_C1(off, siz)                    ((sGfxCmd->words.w1 >> (off)) & ((1U << siz) - 1))
#define GFX_CMD_SIZE                        0x40
#define GFX_CMD_BITS                        0x3F

#define mtx_stack_offset(x)                 ((x) * (sizeof(Mat4) / sizeof(f32)))
#define mtx_stack_inc(x)                    sGfxRsp->mtx += mtx_stack_offset(x)
#define mtx_stack_dec(x)                    sGfxRsp->mtx -= mtx_stack_offset(x)
#define mtx_mul(c, a, b)                    mtxf_mul((void *) (c), (void *) (a), (void *) (b))
#define gfx_adjust_x_for_aspect_ratio(x)    ((x) * gfx_adjust_for_aspect_ratio)
#define gfx_color_comb_comp(x)              ((x) < 6 ? (x) : ((x) == G_CCMUX_TEXEL0_ALPHA ? CC_TEXEL0A : CC_0))
#define gfx_color_comb(a, b, c, d)          ((gfx_color_comb_comp(a) << 0) | (gfx_color_comb_comp(b) << 3) | (gfx_color_comb_comp(c) << 6) | (gfx_color_comb_comp(d) << 9))
#define tex_u_normalize(tu)                 ((((tu) - (sGfxRdp->texTile.uls << 3)) * INV32 + sGfxRdp->texOffset) * sGfxRdp->invTexWidth)
#define tex_v_normalize(tv)                 ((((tv) - (sGfxRdp->texTile.ult << 3)) * INV32 + sGfxRdp->texOffset) * sGfxRdp->invTexHeight)

//
// Structs
//

typedef void (*GfxCmdFunc)(); 
typedef struct { u16 x, y, w, h; } XYWH;
typedef struct { Vec4f p; Vec3f n; Vec2f tc; Vec4f color; u8 tti, rej; const Vtx_t *v; } Vertex;
typedef struct { u32 hash, id, w, h; u8 cms, cmt, lin, pal, *data; } GfxTexture;
typedef struct { GfxTexture pool[GFX_MAX_TEXTURES]; u32 index; } GfxTexturePool;
typedef struct { OmmHMap pngs; FILE* file; u8 *data; u32 size; s32 texw, texh; } GfxTexturePreLoad;
typedef struct { u32 id; struct ShaderProgram *prg; u8 mapShaderInput[2][4]; } GfxColorCombiner;
typedef struct { struct ShaderProgram *prg; u8 numInputs; bool usedTextures[2]; } GfxShaderProgram;

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
    bool fog, alpha;
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

static struct GfxWindowManagerAPI *sGfxWapi;
static struct GfxRenderingAPI *sGfxRapi;
static GfxColorCombiner *sGfxColorCombiner = NULL;
static GfxShaderProgram sGfxShaderProgram[1];
static RealitySignalProcessor sGfxRsp[1];
static RealityDisplayProcessor sGfxRdp[1];
static VertexBufferObject sGfxVbo[1];
static GfxTexture *sGfxCurrTexture = NULL;
static GfxTexturePool sGfxTexturePool[1];
static GfxTexturePreLoad sGfxTexturePreLoad[1];
static OmmHMap sGfxTextureCache = omm_hmap_zero;
static GfxCmdFunc sGfxCmdTable[GFX_CMD_SIZE];
struct GfxDimensions gfx_current_dimensions;
static f32 gfx_adjust_for_aspect_ratio;

static const f32 INV32 = 1.f / 32.f;
static const f32 INV256 = 1.f / 256.f;
static f32 *gVec4fZero3[3] = { gVec4fZero, gVec4fZero, gVec4fZero };
static f32 *gVec4fOne3[3] = { gVec4fOne, gVec4fOne, gVec4fOne };

static s32 sTargetFps = 0;
static f64 sDeltaTime = 0;
static f64 sStartTime = 0;
static f64 sCurrTime  = 0;
static f32 sLerpDelta = 0;

OMM_INLINE f64 gfx_get_current_time() {
    return (f64) SDL_GetPerformanceCounter() / (f64) SDL_GetPerformanceFrequency();
}

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

//
// Palettes
//

static bool gfx_texture_has_palette(const char *texname) {
    static const char *sOmmTexturePatternsWithPalette[] = {
#define OMM_PALETTE_(id, str) OMM_PALETTE_##id,
#define OMM_PALETTE_LEVEL_(id, str) OMM_PALETTE_##id,
#include "data/omm/omm_defines_palettes.inl"
#undef OMM_PALETTE_
#undef OMM_PALETTE_LEVEL_
        NULL
    };
    for_each_until_null(const char *, pattern, sOmmTexturePatternsWithPalette) {
        if (strstr(texname, *pattern)) {
            return true;
        }
    }
    return false;
}

#if LEVEL_PALETTES

enum {
    GFX_LEVEL_PALETTE_FLOODED = (1 << 0),
    GFX_LEVEL_PALETTE_FROZEN  = (1 << 1),
    GFX_LEVEL_PALETTE_SHADOW  = (1 << 2),
};

static bool gfx_texture_is_level_palette(const char *texname) {
    static const char *sOmmTexturePatternsWithLevelPalette[] = {
#define OMM_PALETTE_(id, str)
#define OMM_PALETTE_LEVEL_(id, str) OMM_PALETTE_##id,
#include "data/omm/omm_defines_palettes.inl"
#undef OMM_PALETTE_
#undef OMM_PALETTE_LEVEL_
        NULL
    };
    for_each_until_null(const char *, pattern, sOmmTexturePatternsWithLevelPalette) {
        if (strstr(texname, *pattern)) {
            return true;
        }
    }
    return false;
}

#define set_r(r) data[i + 0] = clamp_s(r, 0x00, 0xFF)
#define set_g(g) data[i + 1] = clamp_s(g, 0x00, 0xFF)
#define set_b(b) data[i + 2] = clamp_s(b, 0x00, 0xFF)
static void gfx_texture_load_level_palette(GfxTexture *tex) {
    u8 pal = (
        (GFX_LEVEL_PALETTE_FLOODED * omm_world_is_flooded()) |
        (GFX_LEVEL_PALETTE_FROZEN  * omm_world_is_frozen() ) |
        (GFX_LEVEL_PALETTE_SHADOW  * omm_world_is_shadow() )
    );
    if (gfx_texture_is_level_palette(sGfxRdp->texLoaded) && tex->data && tex->pal != pal) {
        if (pal) {
            u32 size = tex->w * tex->h * 4;
            u8 *data = omm_dup(tex->data, size);
            for (u32 i = 0; i != size; i += 4) {
                s32 r = data[i + 0];
                s32 g = data[i + 1];
                s32 b = data[i + 2];
                if (pal & GFX_LEVEL_PALETTE_FLOODED) {
                    s32 y = (r + r + g + g + g + b) / 6;
                    set_r(y / 2);
                    set_g(y / 2);
                    set_b(y);
                }
                if (pal & GFX_LEVEL_PALETTE_FROZEN) {
                    s32 y = (r + r + g + g + g + b) / 6;
                    set_r((y + 0x0FF) / 2);
                    set_g((y + 0x0FF) / 2);
                    set_b((y + 0x1FF) / 3);
                }
                if (pal & GFX_LEVEL_PALETTE_SHADOW) {
                    set_r(0);
                    set_g(0);
                    set_b(0);
                }
            }
            sGfxRapi->upload_texture(data, tex->w, tex->h);
            omm_free(data);
        } else {
            sGfxRapi->upload_texture(tex->data, tex->w, tex->h);
        }
        tex->pal = pal;
    }
}

#endif

//
// Texture preload
//

static bool gfx_texture_preload_init() {
    omm_zero(sGfxTexturePreLoad, sizeof(GfxTexturePreLoad));
    omm_hmap_insert(sGfxTexturePreLoad->pngs, 1, omm_new(s32, 1));
    if (gOmmPreloadTextures == 2) {
        omm_cat_paths(gfxTextureRawFilename, SYS_MAX_PATH, sys_exe_path(), GFX_TEXTURE_RAW_FILEPATH);
        sGfxTexturePreLoad->file = fopen(gfxTextureRawFilename, "rb");
    }
    return sGfxTexturePreLoad->file != NULL;
}

static void gfx_texture_preload_create_raw() {
    if (gOmmPreloadTextures == 2) {
        omm_cat_paths(gfxTextureRawFilename, SYS_MAX_PATH, sys_exe_path(), GFX_TEXTURE_RAW_FILEPATH);
        sGfxTexturePreLoad->file = fopen(gfxTextureRawFilename, "wb");
    }
}

static void gfx_texture_preload_end() {
    if (sGfxTexturePreLoad->file) {
        fclose(sGfxTexturePreLoad->file);
    }
    omm_free(sGfxTexturePreLoad->data);
    omm_hmap_delete(sGfxTexturePreLoad->pngs);
    omm_zero(sGfxTexturePreLoad, sizeof(GfxTexturePreLoad));
}

static const char *gfx_texture_preload_read_raw() {
    static char texname[SYS_MAX_PATH];
    if (sGfxTexturePreLoad->file) {
        u32 len;
        if (fread(&len, sizeof(u32), 1, sGfxTexturePreLoad->file) == 0) {
            return NULL;
        }
        if (fread(texname, sizeof(char), len, sGfxTexturePreLoad->file) < len) {
            return NULL;
        }
        if (fread(&sGfxTexturePreLoad->texw, sizeof(s32), 1, sGfxTexturePreLoad->file) == 0) {
            return NULL;
        }
        if (fread(&sGfxTexturePreLoad->texh, sizeof(s32), 1, sGfxTexturePreLoad->file) == 0) {
            return NULL;
        }
        u32 size = sGfxTexturePreLoad->texw * sGfxTexturePreLoad->texh * 4;
        if (OMM_UNLIKELY(size > sGfxTexturePreLoad->size)) {
            omm_free(sGfxTexturePreLoad->data);
            sGfxTexturePreLoad->data = omm_new(u8, size);
            sGfxTexturePreLoad->size = size;
        }
        if (fread(sGfxTexturePreLoad->data, sizeof(u8), size, sGfxTexturePreLoad->file) < size) {
            return NULL;
        }
        return texname;
    }
    return NULL;
}

static void gfx_texture_preload_write_raw(const char *texname, const u8 *data, s32 texw, s32 texh) {
    if (sGfxTexturePreLoad->file) {
        u32 len = strlen(texname) + 1;
        fwrite(&len, sizeof(u32), 1, sGfxTexturePreLoad->file);
        fwrite(texname, sizeof(char), len, sGfxTexturePreLoad->file);
        fwrite(&texw, sizeof(s32), 1, sGfxTexturePreLoad->file);
        fwrite(&texh, sizeof(s32), 1, sGfxTexturePreLoad->file);
        fwrite(data, sizeof(u8), texw * texh * 4, sGfxTexturePreLoad->file);
    }
}

OMM_ROUTINE_UPDATE(gfx_texture_preload_opt_update) {
    configPrecacheRes = (gOmmPreloadTextures != 0);
}

//
// Texture load
//

static GfxTexture *gfx_texture_create(const char *texname, const u8 *data, s32 texw, s32 texh) {

    // Create gfx texture entry
    GfxTexture *tex = &sGfxTexturePool->pool[sGfxTexturePool->index++];
    tex->hash = string_hash(texname);
    tex->id   = sGfxRapi->new_texture();
    tex->w    = texw;
    tex->h    = texh;
    tex->cms  = 0;
    tex->cmt  = 0;
    tex->lin  = 0;
    tex->pal  = 0;
    tex->data = (gfx_texture_has_palette(texname) ? omm_dup(data, texw * texh * 4) : NULL);
    omm_hmap_insert(sGfxTextureCache, tex->hash, tex);

    // Send it to the rendering api
    sGfxRapi->select_texture(0, tex->id);
    sGfxRapi->set_sampler_parameters(0, false, 0, 0);
    sGfxRapi->upload_texture(data, texw, texh);
    return tex;
}

static GfxTexture *gfx_texture_load_png(const char *texname, const char *filename) {

    // Load texture data from PNG
    s32 texw, texh;
    u8 *data = fs_load_png(filename, &texw, &texh);
    if (data) {
        gfx_texture_preload_write_raw(texname, data, texw, texh);
        GfxTexture *tex = gfx_texture_create(texname, data, texw, texh);
        omm_free(data);
        return tex;
    }

    // Missing texture
    fprintf(stderr, "Cannot load texture \"%s\" from file \"%s\"\n", texname, filename);
    return gfx_texture_create(texname, sMissingTexture, 4, 4);
}

OMM_INLINE void gfx_texture_import() {
    GfxTexture *tex = gfx_texture_find(sGfxRdp->texLoaded);
    if (OMM_UNLIKELY(!tex)) {
        omm_str_cat(filename, SYS_MAX_PATH, FS_TEXTUREDIR "/", sGfxRdp->texLoaded, ".png");
        tex = gfx_texture_load_png(sGfxRdp->texLoaded, filename);
    }
    sGfxCurrTexture = tex;
    sGfxRapi->select_texture(0, tex->id);
#if LEVEL_PALETTES
    gfx_texture_load_level_palette(tex);
#endif
}

//
// Precaching
//

static bool gfx_texture_precache(UNUSED void *user, const char *path) {

    // Check if it's a png file
    if (strstr(path, ".png") || strstr(path, ".PNG")) {
        omm_str_cpy(texname, SYS_MAX_PATH, path);
        texname[strlen(texname) - 4] = 0;

        // Check if it starts with "gfx/"
        if (strstr(texname, FS_TEXTUREDIR "/") == texname) {
            const char *name = texname + sizeof(FS_TEXTUREDIR "/") - 1;

            // Check if new hash
            u32 hash = string_hash(name);
            if (omm_hmap_find(sGfxTexturePreLoad->pngs, hash) == -1) {

                // Smart precache
                // - No spaces or "unused"
                // - Contains ".rgba16", ".rgba32", ".ia4", ".ia8" or ".ia16"
                // - If "textures/", don't precache "/intro_raw/", "/ipl3_raw/", "/skybox" or "/special/"
                if (!strstr(name, " ") &&
                    !strstr(name, "unused") && (
                     strstr(name, ".rgba16") ||
                     strstr(name, ".rgba32") ||
                     strstr(name, ".ia4") ||
                     strstr(name, ".ia8") ||
                     strstr(name, ".ia16")) && (
                    !strstr(name, "textures/") || (
                    !strstr(name, "/intro_raw/") &&
                    !strstr(name, "/ipl3_raw/") &&
                    !strstr(name, "/skybox") &&
                    !strstr(name, "/special/")))
                ) {
                    const char **entry = omm_new(const char *, 2);
                    entry[0] = omm_dup(name, strlen(name) + 1);
                    entry[1] = omm_dup(path, strlen(path) + 1);
                    omm_hmap_insert(sGfxTexturePreLoad->pngs, hash, entry);
                }
            }
        }
    }
    return true;
}

f32 gfx_texture_do_precache() {
    f64  start = gfx_get_current_time();
    s32  count = omm_hmap_count(sGfxTexturePreLoad->pngs) - 1;
    s32 *index = omm_hmap_get(sGfxTexturePreLoad->pngs, s32 *, count);
    while (*index < count && gfx_get_current_time() - start < GAME_UPDATE_DURATION) {
        const char **entry = omm_hmap_get(sGfxTexturePreLoad->pngs, const char **, (*index)++);
        const char *name = entry[0];
        const char *path = entry[1];
        if (!gfx_texture_find(name)) {
            gfx_texture_load_png(name, path);
        }
    }
    if (*index >= count) {
        gfx_texture_preload_end();
    }
    return (count ? (f32) *index / (f32) count : 1.f);
}

void gfx_precache_textures() {
    if (gfx_texture_preload_init()) {
        const char *texname = NULL;
        while ((texname = gfx_texture_preload_read_raw()) != NULL) {
            gfx_texture_create(texname, sGfxTexturePreLoad->data, sGfxTexturePreLoad->texw, sGfxTexturePreLoad->texh);
        }
    } else if (gOmmPreloadTextures) {
        gfx_texture_preload_create_raw();
        fs_walk(FS_TEXTUREDIR, gfx_texture_precache, NULL, true);
    }
}

//
// Gfx
//

static Gfx *sGfxCmd;

static void gfx_run_dl(Gfx *cmd);

static void gfx_noop() {
}

OMM_INLINE void gfx_flush() {
    if (sGfxVbo->length) {
        sGfxRapi->draw_triangles(sGfxVbo->buffer, sGfxVbo->length, sGfxVbo->tris);
        sGfxVbo->length = 0;
        sGfxVbo->tris = 0;
    }
}

//
// Color combiners
//

static void *gfx_lookup_or_create_shader_program(u32 shaderId) {
    void *prg = sGfxRapi->lookup_shader(shaderId);
    if (!prg) {
        sGfxRapi->unload_shader(sGfxShaderProgram->prg);
        prg = sGfxRapi->create_and_load_new_shader(shaderId);
        sGfxShaderProgram->prg = prg;
    }
    return prg;
}

static GfxColorCombiner *gfx_lookup_or_create_color_combiner(u32 id) {
    ASSERT(id); // just in case

    // Last cached
    static GfxColorCombiner *cc4[4] = { NULL, NULL, NULL, NULL };
    static s32 cci = 0;
    if (cc4[0] && cc4[0]->id == id) return cc4[0];
    if (cc4[1] && cc4[1]->id == id) return cc4[1];
    if (cc4[2] && cc4[2]->id == id) return cc4[2];
    if (cc4[3] && cc4[3]->id == id) return cc4[3];
    GfxColorCombiner **cc = &cc4[(cci++) & 3];

    // Find in cache
    static OmmHMap sGfxColorCombiners = omm_hmap_zero;
    s32 i = omm_hmap_find(sGfxColorCombiners, id);
    if (i != -1) {
        *cc = omm_hmap_get(sGfxColorCombiners, GfxColorCombiner *, i);
        return *cc;
    }

    // Create new color combiner
    *cc = omm_new(GfxColorCombiner, 1);
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
                        (*cc)->mapShaderInput[i][nextInputNumber - 1] = c[i][j];
                        inputNumber[c[i][j]] = nextInputNumber++;
                    }
                    val = inputNumber[c[i][j]];
                } break;
            }
            shaderId |= val << (i * 12 + j * 3);
        }
    }
    (*cc)->id = id;
    (*cc)->prg = gfx_lookup_or_create_shader_program(shaderId);
    omm_hmap_insert(sGfxColorCombiners, id, *cc);
    return *cc;
}

//
// Changes
//

#define if_changed(name, x, ...) { \
    static u32 _##name = 0; \
    u32 name = (x); \
    if (_##name != name) { \
        _##name = name; \
        __VA_ARGS__ \
    } \
}

static void gfx_update_color_combiner() {
    gfx_flush();

    // Color effects
    bool edge = (sGfxRdp->otherModeL & CVG_X_ALPHA) == CVG_X_ALPHA;
    bool noise = (sGfxRdp->otherModeL & G_AC_DITHER) == G_AC_DITHER;
    sGfxRdp->fog = (sGfxRdp->otherModeL >> 30) == G_BL_CLR_FOG && !GFX_NO_FOG;
    sGfxRdp->alpha = edge || ((sGfxRdp->otherModeL & (G_BL_A_MEM << 18)) == 0);
    edge &= !(sGfxRsp->geometryMode & G_TEXTURE_ALPHA);

    // Color combiner id
    u32 ccId = sGfxRdp->combineMode;
    if (edge) ccId |= SHADER_OPT_TEXTURE_EDGE;
    if (noise) ccId |= SHADER_OPT_NOISE;
    if (sGfxRdp->fog) ccId |= SHADER_OPT_FOG;
    if (sGfxRdp->alpha) ccId |= SHADER_OPT_ALPHA;
    else ccId &= ~0xFFF000;
    if_changed(alpha, sGfxRdp->alpha,
        sGfxRapi->set_use_alpha(sGfxRdp->alpha);
    )

    // Color combiner
    sGfxColorCombiner = gfx_lookup_or_create_color_combiner(ccId);
#if GL_TRANSPARENCY
    if ((sGfxRdp->otherModeL & G_AC_COVERAGE) == G_AC_COVERAGE) {
        glDepthMask(GL_TRUE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        glEnable(GL_SAMPLE_ALPHA_TO_ONE);
    } else {
        glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        glDisable(GL_SAMPLE_ALPHA_TO_ONE);
    }
#endif
}

OMM_INLINE void gfx_update_shader() {
    if (sGfxColorCombiner->prg != sGfxShaderProgram->prg) {
        gfx_flush();
        sGfxRapi->unload_shader(sGfxShaderProgram->prg);
        sGfxRapi->load_shader(sGfxColorCombiner->prg);
        sGfxRapi->shader_get_info(sGfxColorCombiner->prg, &sGfxShaderProgram->numInputs, sGfxShaderProgram->usedTextures);
        sGfxShaderProgram->prg = sGfxColorCombiner->prg;
    }
}

OMM_INLINE void gfx_geometry_mode_changed(u32 clr, u32 set) {
    sGfxRsp->geometryMode &= clr;
    sGfxRsp->geometryMode |= set;
    if_changed(depthTest, (sGfxRsp->geometryMode & G_ZBUFFER) != 0,
        gfx_flush();
        sGfxRapi->set_depth_test(depthTest);
    )
    if_changed(textureAlpha, (sGfxRsp->geometryMode & G_TEXTURE_ALPHA) != 0,
        gfx_update_color_combiner();
    )
}

OMM_INLINE void gfx_combine_mode_changed(u32 mode) {
    sGfxRdp->combineMode = mode;
    if_changed(combineMode, sGfxRdp->combineMode,
        gfx_update_color_combiner();
    )
}

OMM_INLINE void gfx_other_mode_l_changed(u32 oml) {
    sGfxRdp->otherModeL = oml;
    if_changed(depthMask, (sGfxRdp->otherModeL & Z_UPD) == Z_UPD,
        gfx_flush();
        sGfxRapi->set_depth_mask(depthMask);
    )
    if_changed(decalMode, (sGfxRdp->otherModeL & ZMODE_DEC) == ZMODE_DEC,
        gfx_flush();
        sGfxRapi->set_zmode_decal(decalMode);
    )
#if GL_TRANSPARENCY
    if_changed(otherModeLCCFlags, (sGfxRdp->otherModeL & (0xC0000000 | CVG_X_ALPHA | G_AC_DITHER | (G_BL_A_MEM << 18) | G_AC_COVERAGE)),
        gfx_update_color_combiner();
    )
#else
    if_changed(otherModeLCCFlags, (sGfxRdp->otherModeL & (0xC0000000 | CVG_X_ALPHA | G_AC_DITHER | (G_BL_A_MEM << 18))),
        gfx_update_color_combiner();
    )
#endif
}

OMM_INLINE void gfx_other_mode_h_changed(u32 omh) {
    sGfxRdp->otherModeH = omh;
    sGfxRdp->texOffset = 0.5f * ((omh & (3U << G_MDSFT_TEXTFILT)) != G_TF_POINT);
}

OMM_INLINE void gfx_tex_tile_changed(u16 uls, u16 ult, u16 lrs, u16 lrt) {
    sGfxRdp->texTile.uls  = uls;
    sGfxRdp->texTile.ult  = ult;
    sGfxRdp->texTile.lrs  = lrs;
    sGfxRdp->texTile.lrt  = lrt;
    sGfxRdp->invTexWidth  = 1.f / ((lrs - uls + 4) >> 2);
    sGfxRdp->invTexHeight = 1.f / ((lrt - ult + 4) >> 2);
    sGfxRdp->texTile.tti++;
}

OMM_INLINE void gfx_viewport_changed(u16 x, u16 y, u16 w, u16 h) {
    gfx_flush();
    sGfxRdp->viewport.x = x;
    sGfxRdp->viewport.y = y;
    sGfxRdp->viewport.w = w;
    sGfxRdp->viewport.h = h;
    sGfxRapi->set_viewport(x, y, w, h);
}

OMM_INLINE void gfx_scissor_changed(s32 x, s32 y, s32 w, s32 h) {
    gfx_flush();
    sGfxRapi->set_scissor(x, y, w, h);
}

//
// Matrix
//

static void gfx_sp_matrix_projection_load(Mat4 mat) {
    mtxf_copy(sGfxRsp->mtxP, mat);
}

static void gfx_sp_matrix_projection_mul(Mat4 mat) {
    mtx_mul(sGfxRsp->mtxP, mat, sGfxRsp->mtxP);
}

static void gfx_sp_matrix_modelview_load_push(Mat4 mat) {
    mtx_stack_inc(1);
    mtxf_copy(sGfxRsp->mtx, mat);
    sGfxRsp->lightsChanged = true;
}

static void gfx_sp_matrix_modelview_load_nopush(Mat4 mat) {
    mtxf_copy(sGfxRsp->mtx, mat);
    sGfxRsp->lightsChanged = true;
}

static void gfx_sp_matrix_modelview_mul_push(Mat4 mat) {
    mtx_stack_inc(1);
    mtx_mul(sGfxRsp->mtx, mat, sGfxRsp->mtx - mtx_stack_offset(1));
    sGfxRsp->lightsChanged = true;
}

static void gfx_sp_matrix_modelview_mul_nopush(Mat4 mat) {
    mtx_mul(sGfxRsp->mtx, mat, sGfxRsp->mtx);
    sGfxRsp->lightsChanged = true;
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
    mtx_mul(sGfxRsp->mtxMP, sGfxRsp->mtx, sGfxRsp->mtxP);
}

static void gfx_sp_pop_matrix() {
    u32 count = GFX_W1 >> 6;
    mtx_stack_dec(count);
    mtx_mul(sGfxRsp->mtxMP, sGfxRsp->mtx, sGfxRsp->mtxP);
}

//
// Vertex
//

OMM_INLINE void gfx_sp_vertex_compute_tex_coords(Vertex *v) {
    s32 tu, tv;
    if (sGfxRsp->geometryMode & G_TEXTURE_GEN) {
        s32 dotx = 128 + vec3f_dot(v->n, sGfxRsp->lookAtXCoeffs);
        s32 doty = 128 + vec3f_dot(v->n, sGfxRsp->lookAtYCoeffs);
        tu = (dotx * sGfxRsp->texS) >> 9;
        tv = (doty * sGfxRsp->texT) >> 9;
    } else {
        tu = (v->v->tc[0] * sGfxRsp->texS) >> 16;
        tv = (v->v->tc[1] * sGfxRsp->texT) >> 16;
    }
    v->tc[0] = tex_u_normalize(tu);
    v->tc[1] = tex_v_normalize(tv);
    v->tti = sGfxRdp->texTile.tti;
}

static void gfx_sp_vertex() {
    s32 vtxCount = GFX_C0(12, 8);
    s32 vtxDest = GFX_C0(1, 7) - vtxCount;
    const Vtx *vtx = GFX_W1P;
    Vertex *d = &sGfxRsp->vtx[vtxDest];

    // If the lights changed, re-compute light coeffs
    if (sGfxRsp->lightsChanged && (sGfxRsp->geometryMode & G_LIGHTING)) {
        Vec3f lightDir = { sGfxRsp->light.dir[0] / 127.f, sGfxRsp->light.dir[1] / 127.f, sGfxRsp->light.dir[2] / 127.f };
        vec3f_normalize(vec3f_mult_mtx(sGfxRsp->lightCoeffs, lightDir, (void *) sGfxRsp->mtx));
        vec3f_normalize(vec3f_mult_mtx(sGfxRsp->lookAtXCoeffs, gVec3fX, (void *) sGfxRsp->mtx));
        vec3f_normalize(vec3f_mult_mtx(sGfxRsp->lookAtYCoeffs, gVec3fY, (void *) sGfxRsp->mtx));
        sGfxRsp->lightsChanged = false;
    }

    // Load current matrix into XMM registers
    __m128 m0 = _mm_loadu_ps(sGfxRsp->mtxMP[0]);
    __m128 m1 = _mm_loadu_ps(sGfxRsp->mtxMP[1]);
    __m128 m2 = _mm_loadu_ps(sGfxRsp->mtxMP[2]);
    __m128 m3 = _mm_loadu_ps(sGfxRsp->mtxMP[3]);

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
        _mm_storeu_ps(d->p, d3);
        d->p[0] = gfx_adjust_x_for_aspect_ratio(d->p[0]);

        // Vertex normal
        d->n[0] = vtx->n.n[0];
        d->n[1] = vtx->n.n[1];
        d->n[2] = vtx->n.n[2];

        // Colors
        if (sGfxRsp->geometryMode & G_LIGHTING) {
            s32 rl = sGfxRsp->light.col[0];
            s32 gl = sGfxRsp->light.col[1];
            s32 bl = sGfxRsp->light.col[2];
            s32 ra = sGfxRsp->lightAmb.col[0];
            s32 ga = sGfxRsp->lightAmb.col[1];
            s32 ba = sGfxRsp->lightAmb.col[2];

            // Performing integer operations makes things faster
            s32 intensity = clamp_s(vec3f_dot(d->n, sGfxRsp->lightCoeffs), 0, 128);
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
        if (sGfxRsp->texOn) {
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
        if ((sGfxRsp->geometryMode & G_FOG) && !GFX_NO_FOG) {
            f32 winv = 1.f / max_f(d->p[3], 0.001f);
            f32 fogz = clamp_f(d->p[2] * winv * sGfxRsp->fogMul + sGfxRsp->fogOff, 0, 256);
            d->color[3] = fogz * INV256;
        } else {
            d->color[3] = v->cn[3] * INV256;
        }
    }

    // Update shader program now
    gfx_update_shader();
}

//
// Triangle
//

static void gfx_sp_tri(u8 v1i, u8 v2i, u8 v3i) {
    Vertex *v1 = &sGfxRsp->vtx[v1i];
    Vertex *v2 = &sGfxRsp->vtx[v2i];
    Vertex *v3 = &sGfxRsp->vtx[v3i];
    if (v1->rej & v2->rej & v3->rej) {
        return;
    }
    
    // Culling test
    if (sGfxRsp->geometryMode & G_CULL_BOTH) {
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

        if (((sGfxRsp->geometryMode & G_CULL_FRONT) && cross <= 0) ||
            ((sGfxRsp->geometryMode & G_CULL_BACK ) && cross >= 0)) {
            return;
        }
    }

    // Textures
    bool useTexture = *(sGfxShaderProgram->usedTextures);
    if (useTexture) {
        if (sGfxRdp->texChanged) {
            gfx_flush();
            gfx_texture_import();
            sGfxRdp->texChanged = false;
        }
        bool lin = configFiltering && ((sGfxRdp->otherModeH & (3U << G_MDSFT_TEXTFILT)) != G_TF_POINT);
        if (lin != sGfxCurrTexture->lin || sGfxRdp->texTile.cms != sGfxCurrTexture->cms || sGfxRdp->texTile.cmt != sGfxCurrTexture->cmt) {
            gfx_flush();
            sGfxRapi->set_sampler_parameters(0, lin, sGfxRdp->texTile.cms, sGfxRdp->texTile.cmt);
            sGfxCurrTexture->lin = lin;
            sGfxCurrTexture->cms = sGfxRdp->texTile.cms;
            sGfxCurrTexture->cmt = sGfxRdp->texTile.cmt;
        }
    }

    // Vertex colors
    Vertex *va[3] = { v1, v2, v3 };
    f32 *va_colors[3] = { v1->color, v2->color, v3->color };
    f32 **colors[8][2]; // input, rgb/a
    for (s32 j = 0; j != sGfxShaderProgram->numInputs; ++j) {

        // RGB
        switch (sGfxColorCombiner->mapShaderInput[0][j]) {
            case CC_PRIM:  colors[j][0] = sGfxRdp->primColors; break;
            case CC_SHADE: colors[j][0] = va_colors; break;
            case CC_ENV:   colors[j][0] = sGfxRdp->envColors; break;
            default:       colors[j][0] = gVec4fZero3; break;
        }
        
        // Alpha
        if (sGfxRdp->alpha) {
            switch (sGfxColorCombiner->mapShaderInput[1][j]) {
                case CC_PRIM:  colors[j][1] = sGfxRdp->primColors; break;
                case CC_SHADE: colors[j][1] = (sGfxRdp->fog ? gVec4fOne3 : va_colors); break;
                case CC_ENV:   colors[j][1] = sGfxRdp->envColors; break;
                default:       colors[j][1] = gVec4fZero3; break;
            }
        }
    }
    
    // Fill VBO
    f32 *buffer = sGfxVbo->buffer + sGfxVbo->length;
    for (s32 i = 0; i != 3; ++i) {

        // x, y, z, w
        vec4f_copy(buffer, va[i]->p);
#if OMM_RAPI_D3D
        buffer[2] = (buffer[2] + buffer[3]) / 2.f;
#endif
        buffer += 4;
        
        // Texture coords
        if (useTexture) {
            if (OMM_UNLIKELY(va[i]->tti != sGfxRdp->texTile.tti)) {
                gfx_sp_vertex_compute_tex_coords(va[i]);
            }
            vec2f_copy(buffer, va[i]->tc);
            buffer += 2;
        }
        
        // Fog
        if (sGfxRdp->fog) {
            vec3f_copy(buffer, sGfxRdp->fogColor);
            buffer += 3;
            *(buffer++) = va[i]->color[3];
        }
        
        // Colors
        for (s32 j = 0; j != sGfxShaderProgram->numInputs; ++j) {
            vec3f_copy(buffer, colors[j][0][i]);
            buffer += 3;
            if (sGfxRdp->alpha) {
                *(buffer++) = colors[j][1][i][3];
            }
        }
    }
    
    // Draw triangles
    sGfxVbo->length = (s32) (buffer - sGfxVbo->buffer);
    if (++sGfxVbo->tris == GFX_MAX_TRIS) {
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
    u64 omhl = ((u64) sGfxRdp->otherModeL) | (((u64) sGfxRdp->otherModeH) << 32llu);
    omhl = (omhl & ~mask) | mode;
    sGfxRdp->otherModeL = (u32) omhl;
    sGfxRdp->otherModeH = (u32) (omhl >> 32);
    gfx_other_mode_l_changed((u32) omhl);
    gfx_other_mode_h_changed((u32) (omhl >> 32));
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
            gfx_viewport_changed(x * GFX_RATIO_X, y * GFX_RATIO_Y, w * GFX_RATIO_X, h * GFX_RATIO_Y);
        } break;

        case G_MV_LIGHT: {
            if (GFX_C0(8, 8) == 6) {
                omm_copy(&sGfxRsp->light, data, sizeof(Light_t));
            } else {
                omm_copy(&sGfxRsp->lightAmb, data, sizeof(Light_t));
            }
#if LEVEL_PALETTES
            // Why isn't SA using white lights when everything else in the game does?...
            static const Lights1 sa_lights = gdSPDefLights1(0x00, 0x7f, 0x99, 0x00, 0xd4, 0xff, 0x28, 0x28, 0x28);
            static const Lights1 sa_lights_frozen = gdSPDefLights1(0x99, 0x99, 0x99, 0xff, 0xff, 0xff, 0x28, 0x28, 0x28);
            if (omm_world_is_frozen()) {
                if (omm_same(data, &sa_lights.l, sizeof(Light_t))) omm_copy(&sGfxRsp->light, &sa_lights_frozen.l, sizeof(Light_t));
                if (omm_same(data, &sa_lights.a, sizeof(Ambient_t))) omm_copy(&sGfxRsp->lightAmb, &sa_lights_frozen.a, sizeof(Light_t));
            }
#endif
            break;
        }
    }
}

static void gfx_sp_moveword() {
    if (GFX_C0(16, 8) == G_MW_FOG) {
        u32 data = GFX_W1;
        sGfxRsp->fogMul = (s16) (data >> 16);
        sGfxRsp->fogOff = (s16) data;
    }
}

static void gfx_sp_geometry_mode() {
    u32 clr = GFX_C0(0, 24);
    u32 set = GFX_W1;
    gfx_geometry_mode_changed(clr, set);
}

static void gfx_sp_texture() {
    u16 on = GFX_C0(1, 1);
    u16 sc = GFX_C1(16, 16);
    u16 tc = GFX_C1(0, 16);
    sGfxRsp->texOn = on;
    sGfxRsp->texS = sc;
    sGfxRsp->texT = tc;
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
    gfx_combine_mode_changed(gfx_color_comb(rgb_a, rgb_b, rgb_c, rgb_d) | (gfx_color_comb(alpha_a, alpha_b, alpha_c, alpha_d) << 12));
}

static void gfx_dp_set_env_color() {
    u8 r = GFX_C1(24, 8);
    u8 g = GFX_C1(16, 8);
    u8 b = GFX_C1(8, 8);
    u8 a = GFX_C1(0, 8);
    sGfxRdp->envColor[0] = r * INV256;
    sGfxRdp->envColor[1] = g * INV256;
    sGfxRdp->envColor[2] = b * INV256;
    sGfxRdp->envColor[3] = a * INV256;
}

static void gfx_dp_set_prim_color() {
    u8 r = GFX_C1(24, 8);
    u8 g = GFX_C1(16, 8);
    u8 b = GFX_C1(8, 8);
    u8 a = GFX_C1(0, 8);
    sGfxRdp->primColor[0] = r * INV256;
    sGfxRdp->primColor[1] = g * INV256;
    sGfxRdp->primColor[2] = b * INV256;
    sGfxRdp->primColor[3] = a * INV256;
}

static void gfx_dp_set_fog_color() {
    u8 r = GFX_C1(24, 8);
    u8 g = GFX_C1(16, 8);
    u8 b = GFX_C1(8, 8);
    sGfxRdp->fogColor[0] = r * INV256;
    sGfxRdp->fogColor[1] = g * INV256;
    sGfxRdp->fogColor[2] = b * INV256;
}

static void gfx_dp_set_fill_color() {
    u32 packed_color = GFX_W1;
    u32 r = (packed_color >> 11) & 0x1F;
    u32 g = (packed_color >>  6) & 0x1F;
    u32 b = (packed_color >>  1) & 0x1F;
    u32 a = (packed_color >>  0) & 0x01;
    sGfxRdp->fillColor[0] = r * INV32;
    sGfxRdp->fillColor[1] = g * INV32;
    sGfxRdp->fillColor[2] = b * INV32;
    sGfxRdp->fillColor[3] = a;
}

static void gfx_dp_set_z_image() {
    sGfxRdp->zBufAddress = GFX_W1P;
}

static void gfx_dp_set_color_image() {
    sGfxRdp->colorImageAddress = GFX_W1P;
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
    if (OMM_LIKELY(addr)) sGfxRdp->texToLoad = addr;
}

static void gfx_dp_set_tile() {
    if (GFX_C1(24, 3) == G_TX_RENDERTILE) {
        u8 cms = GFX_C1(8, 2);
        u8 cmt = GFX_C1(18, 2);
        sGfxRdp->texTile.cms = cms;
        sGfxRdp->texTile.cmt = cmt;
        sGfxRdp->texChanged = true;
    }
}

static void gfx_dp_set_tile_size() {
    if (OMM_LIKELY(GFX_C1(24, 3) == G_TX_RENDERTILE)) {
        u16 uls = GFX_C0(12, 12);
        u16 ult = GFX_C0(0, 12);
        u16 lrs = GFX_C1(12, 12);
        u16 lrt = GFX_C1(0, 12);
        sGfxRdp->texChanged = true;
        gfx_tex_tile_changed(uls, ult, lrs, lrt);
    }
}

static void gfx_dp_load_block() {
    if (OMM_LIKELY(GFX_C1(24, 3) == G_TX_LOADTILE)) {
        sGfxRdp->texLoaded = sGfxRdp->texToLoad;
        sGfxRdp->texChanged = true;
    }
}

static void gfx_dp_load_tile() {
    if (OMM_LIKELY(GFX_C1(24, 3) == G_TX_LOADTILE)) {
        u32 uls = GFX_C0(12, 12);
        u32 ult = GFX_C0(0, 12);
        u32 lrs = GFX_C1(12, 12);
        u32 lrt = GFX_C1(0, 12);
        sGfxRdp->texLoaded = sGfxRdp->texToLoad;
        sGfxRdp->texChanged = true;
        gfx_tex_tile_changed(uls, ult, lrs, lrt);
    }
}

static void gfx_dp_draw_rectangle(s32 ulx, s32 uly, s32 lrx, s32 lry) {
    f32 ulxf = +(ulx / (2.f * SCREEN_WIDTH )) - 1.f;
    f32 ulyf = -(uly / (2.f * SCREEN_HEIGHT)) + 1.f;
    f32 lrxf = +(lrx / (2.f * SCREEN_WIDTH )) - 1.f;
    f32 lryf = -(lry / (2.f * SCREEN_HEIGHT)) + 1.f;
    ulxf = gfx_adjust_x_for_aspect_ratio(ulxf);
    lrxf = gfx_adjust_x_for_aspect_ratio(lrxf);
    
    Vertex* ul = &sGfxRsp->vtx[GFX_MAX_VERTS + 0];
    Vertex* ll = &sGfxRsp->vtx[GFX_MAX_VERTS + 1];
    Vertex* lr = &sGfxRsp->vtx[GFX_MAX_VERTS + 2];
    Vertex* ur = &sGfxRsp->vtx[GFX_MAX_VERTS + 3];
    ul->p[0] = ulxf; ul->p[1] = ulyf; ul->p[2] = -1.f; ul->p[3] = 1.f;
    ll->p[0] = ulxf; ll->p[1] = lryf; ll->p[2] = -1.f; ll->p[3] = 1.f;
    lr->p[0] = lrxf; lr->p[1] = lryf; lr->p[2] = -1.f; lr->p[3] = 1.f;
    ur->p[0] = lrxf; ur->p[1] = ulyf; ur->p[2] = -1.f; ur->p[3] = 1.f;
    
    // The coordinates for texture rectangle shall bypass the viewport setting
    XYWH lastViewport = sGfxRdp->viewport;
    u32 lastGeometryMode = sGfxRsp->geometryMode;
    u32 lastOtherModeH = sGfxRdp->otherModeH;

    // Draw rectangle
    gfx_geometry_mode_changed(0, 0);
    gfx_other_mode_h_changed((sGfxRdp->otherModeH & ~(3U << G_MDSFT_TEXTFILT)) | G_TF_POINT);
    gfx_viewport_changed(0, 0, gfx_current_dimensions.width, gfx_current_dimensions.height);
    gfx_update_shader();
    gfx_sp_tri(GFX_MAX_VERTS + 0, GFX_MAX_VERTS + 1, GFX_MAX_VERTS + 3);
    gfx_sp_tri(GFX_MAX_VERTS + 1, GFX_MAX_VERTS + 2, GFX_MAX_VERTS + 3);

    // Restore state
    gfx_geometry_mode_changed(0, lastGeometryMode);
    gfx_other_mode_h_changed(lastOtherModeH);
    gfx_viewport_changed(lastViewport.x, lastViewport.y, lastViewport.w, lastViewport.h);
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
    u32 lastCombineMode = sGfxRdp->combineMode;
    if ((sGfxRdp->otherModeH & (3U << G_MDSFT_CYCLETYPE)) == G_CYC_COPY) {
        gfx_combine_mode_changed(gfx_color_comb(0, 0, 0, G_CCMUX_TEXEL0) | (gfx_color_comb(0, 0, 0, G_ACMUX_TEXEL0) << 12));
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
    
    Vertex* ul = &sGfxRsp->vtx[GFX_MAX_VERTS + 0];
    Vertex* ll = &sGfxRsp->vtx[GFX_MAX_VERTS + 1];
    Vertex* lr = &sGfxRsp->vtx[GFX_MAX_VERTS + 2];
    Vertex* ur = &sGfxRsp->vtx[GFX_MAX_VERTS + 3];
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
    ul->tti = lr->tti = ll->tti = ur->tti = sGfxRdp->texTile.tti;

    gfx_dp_draw_rectangle(ulx, uly, lrx, lry);
    gfx_combine_mode_changed(lastCombineMode);
}

static void gfx_dp_fill_rectangle() {
    s32 lrx = ((s32) (GFX_C0(0, 24) << 8)) >> 8;
    s32 lry = ((s32) (GFX_C1(0, 24) << 8)) >> 8;
    ++sGfxCmd;
    s32 ulx = ((s32) (GFX_C0(0, 24) << 8)) >> 8;
    s32 uly = ((s32) (GFX_C1(0, 24) << 8)) >> 8;

    // Don't clear the Z buffer here since we already did it with glClear
    if (sGfxRdp->colorImageAddress == sGfxRdp->zBufAddress) {
        return;
    }

    // ¯\_(ツ)_/¯
    u32 mode = (sGfxRdp->otherModeH & (3U << G_MDSFT_CYCLETYPE));
    if (mode == G_CYC_COPY || mode == G_CYC_FILL) {
        lrx += 1 << 2;
        lry += 1 << 2;
    }

    vec4f_copy(sGfxRsp->vtx[GFX_MAX_VERTS + 0].color, sGfxRdp->fillColor);
    vec4f_copy(sGfxRsp->vtx[GFX_MAX_VERTS + 1].color, sGfxRdp->fillColor);
    vec4f_copy(sGfxRsp->vtx[GFX_MAX_VERTS + 2].color, sGfxRdp->fillColor);
    vec4f_copy(sGfxRsp->vtx[GFX_MAX_VERTS + 3].color, sGfxRdp->fillColor);
    
    u32 lastCombineMode = sGfxRdp->combineMode;
    gfx_combine_mode_changed(gfx_color_comb(0, 0, 0, G_CCMUX_SHADE) | (gfx_color_comb(0, 0, 0, G_ACMUX_SHADE) << 12));
    gfx_dp_draw_rectangle(ulx, uly, lrx, lry);
    gfx_combine_mode_changed(lastCombineMode);
}

//
// Extra commands
//

static void gfx_xp_swap_cmd() {
    static GfxCmdFunc sGfxCmdSwap[GFX_CMD_SIZE] = { NULL };
    u8 cmd = GFX_C0(0, 8) & GFX_CMD_BITS;
    void *func = GFX_W1P;
    if (func) {
        sGfxCmdSwap[cmd] = sGfxCmdTable[cmd];
        sGfxCmdTable[cmd] = (GfxCmdFunc) func;
    } else {
        sGfxCmdTable[cmd] = sGfxCmdSwap[cmd];
    }
}

//
// Gfx run
//

static GfxCmdFunc sGfxCmdTable[] = {
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
    gfx_xp_swap_cmd,
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
    for (sGfxCmd = cmd;; sGfxCmd++) {
#if OMM_CODE_DEBUG
        u8 op = GFX_OP & GFX_CMD_BITS;
        omm_profiler_start_gfx_op(op);
        sGfxCmdTable[op]();
        omm_profiler_stop_gfx_op(op);
#else
        sGfxCmdTable[GFX_OP & GFX_CMD_BITS]();
#endif
        if (OMM_UNLIKELY(!sGfxCmd)) {
            return;
        }
    }
}

//
// Frame interpolation
//

static s32 gfx_get_refresh_rate() {
    static s32 sWindowRefreshRate = 0;
    if (!sWindowRefreshRate) {
#if OMM_WAPI_DXGI
        DEVMODE mode;
        if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &mode)) {
            sWindowRefreshRate = (s32) mode.dmDisplayFrequency;
        }
#elif OMM_WAPI_SDL
        SDL_DisplayMode mode;
        if (SDL_GetCurrentDisplayMode(0, &mode) == 0) {
            sWindowRefreshRate = (s32) mode.refresh_rate;
        }
#else
        sWindowRefreshRate = 60;
#endif
    }
    return sWindowRefreshRate;
}

static s32 gfx_get_target_frame_rate() {
    switch (gOmmFrameRate) {
        case OMM_FPS_30:   return GAME_UPDATES_PER_SECOND;
        case OMM_FPS_60:   return GAME_UPDATES_PER_SECOND * 2;
        case OMM_FPS_AUTO: return gfx_get_refresh_rate();
        case OMM_FPS_INF:  return UNLIMITED_FPS;
    }
    ASSERT(0);
    return GAME_UPDATES_PER_SECOND;
}

static s32 gfx_get_num_frames_to_draw() {
    s32 i = (s32) ((sStartTime - ((s64) sStartTime)) * GAME_UPDATES_PER_SECOND);
    return ((sTargetFps * (i + 1)) / GAME_UPDATES_PER_SECOND) - ((sTargetFps * i) / GAME_UPDATES_PER_SECOND);
}

bool gFrameInterpolation = false;
static void gfx_init_frame_interpolation() {
    if (gOmmFrameRate == OMM_FPS_30 || omm_is_main_menu() || gMenuMode == 0 || gMenuMode == 1) {
        sTargetFps = GAME_UPDATES_PER_SECOND;
        sDeltaTime = GAME_UPDATE_DURATION;
        gFrameInterpolation = false;
    } else {
        sTargetFps = gfx_get_target_frame_rate();
        sDeltaTime = GAME_UPDATE_DURATION / gfx_get_num_frames_to_draw();
        gFrameInterpolation = true;
    }
}

extern void gfx_interpolate_frame_mtx(f32 t);
extern void gfx_interpolate_frame_transition(f32 t);
extern void gfx_interpolate_frame_dialog(f32 t);
extern void gfx_interpolate_frame_hud(f32 t);
extern void gfx_interpolate_frame_effects(f32 t);
extern void gfx_interpolate_frame_paintings(f32 t);
static void gfx_patch_interpolated_frame(f32 t) {
    gfx_interpolate_frame_mtx(t);
    gfx_interpolate_frame_transition(t);
    gfx_interpolate_frame_dialog(t);
    gfx_interpolate_frame_hud(t);
    gfx_interpolate_frame_effects(t);
    gfx_interpolate_frame_paintings(t);
}

extern void gfx_clear_frame_mtx();
extern void gfx_clear_frame_transition();
extern void gfx_clear_frame_dialog();
extern void gfx_clear_frame_hud();
extern void gfx_clear_frame_effects();
extern void gfx_clear_frame_paintings();
static void gfx_render_interpolated_frames() {
    f64 tStartTime = sCurrTime = gfx_get_current_time();
    do {
        // Handle events
        sGfxWapi->handle_events();
        sGfxWapi->get_dimensions(&gfx_current_dimensions.width, &gfx_current_dimensions.height);
        gfx_current_dimensions.aspect_ratio = (f32) gfx_current_dimensions.width / (f32) gfx_current_dimensions.height;
        gfx_adjust_for_aspect_ratio = (4.f / 3.f) / gfx_current_dimensions.aspect_ratio;

        // Start frame
        omm_profiler_start(OMM_PRF_FRM);
        sGfxRsp->mtx = (f32 *) &sGfxRsp->mtxStack[8];
        sGfxRsp->lightsChanged = true;
        if (sGfxWapi->start_frame()) {

            // Patch interpolations
            sLerpDelta = (f32) ((sCurrTime - tStartTime) * GAME_UPDATES_PER_SECOND);
            gfx_patch_interpolated_frame(sLerpDelta);
            
            // Fill drawing buffer
            sGfxRapi->start_frame();
            omm_profiler_start(OMM_PRF_GFX);
            gfx_run_dl(gGfxPool->buffer);
            gfx_flush();
            omm_profiler_stop(OMM_PRF_GFX);

            // Render frame
            omm_profiler_start(OMM_PRF_RDR);
            sGfxRapi->end_frame();
            sGfxWapi->swap_buffers_begin();
            omm_profiler_stop(OMM_PRF_RDR);

            // Swap buffers and sleep
            omm_profiler_stop(OMM_PRF_FRM);
            omm_profiler_frame_drawn();
            sGfxRapi->finish_render();
            sGfxWapi->swap_buffers_end();

            // start                                     begin     curr                 end
            //   [....................|....................|========|--------------------]
            //                                              \_diff_/ \______remain______/
            //                                               \__________avail__________/
            //    \________________________GAME_UPDATE_DURATION_______________________/
            f64 t_start  = sStartTime;
            f64 t_begin  = sCurrTime;
            f64 t_curr   = gfx_get_current_time();
            f64 t_end    = t_start + GAME_UPDATE_DURATION;
            f64 t_avail  = t_end - t_begin;
            f64 t_diff   = t_curr - t_begin;
            f64 t_remain = t_avail - t_diff;
            f64 t_sleep  = (t_diff > t_remain ? t_remain : MIN(sDeltaTime, t_avail) - t_diff);
            if (t_sleep > 0) usleep(1000000.0 * t_sleep);
        }

        // Update current
        sCurrTime = gfx_get_current_time();
    } while (sCurrTime - sStartTime < 0.96 * GAME_UPDATE_DURATION);

    // Clear interpolated stuff
    gfx_clear_frame_mtx();
    gfx_clear_frame_transition();
    gfx_clear_frame_dialog();
    gfx_clear_frame_hud();
    gfx_clear_frame_effects();
    gfx_clear_frame_paintings();
}

static void gfx_set_config() {
#if OMM_GAME_IS_R96X
    config60FPS = false;
    configInternalResolutionBool = false;
    configCustomInternalResolution = 1;
    configForce4by3 = false;
#endif
}

#if OMM_WAPI_DXGI
static void gfx_dxgi_handle_events() {
    extern f64 FRAME_INTERVAL_US_DENOMINATOR;
    extern u64 *frame_timestamp;
    static s32 sCurrTargetFps = 0;
    if (sCurrTargetFps != sTargetFps) {
        FRAME_INTERVAL_US_DENOMINATOR = (3.0 * sTargetFps) / GAME_UPDATES_PER_SECOND;
        *frame_timestamp = 0;
        sCurrTargetFps = sTargetFps;
    }
}
#endif

//
// Public
//

struct GfxRenderingAPI *gfx_get_current_rendering_api() {
    return sGfxRapi;
}

void gfx_init(struct GfxWindowManagerAPI *wapi, struct GfxRenderingAPI *rapi, const char *window_title) {
    sGfxWapi = wapi;
    sGfxRapi = rapi;
    sGfxWapi->init(window_title);
    sGfxRapi->init();
#if OMM_WAPI_DXGI
    sGfxWapi->handle_events = gfx_dxgi_handle_events;
#endif
    sGfxRdp->envColors[0] = sGfxRdp->envColor;
    sGfxRdp->envColors[1] = sGfxRdp->envColor;
    sGfxRdp->envColors[2] = sGfxRdp->envColor;
    sGfxRdp->primColors[0] = sGfxRdp->primColor;
    sGfxRdp->primColors[1] = sGfxRdp->primColor;
    sGfxRdp->primColors[2] = sGfxRdp->primColor;
    static u32 sPrecompiledShaders[] = {
        0x01200200, 0x00000045, 0x00000200, 0x01200a00,
        0x00000a00, 0x01a00045, 0x00000551, 0x01045045,
        0x05a00a00, 0x01200045, 0x05045045, 0x01045a00,
        0x01a00a00, 0x0000038d, 0x01081081, 0x0120038d,
        0x03200045, 0x03200a00, 0x01a00a6f, 0x01141045,
        0x07a00a00, 0x05200200, 0x03200200, 0x09200200,
        0x0920038d, 0x09200045
    };
    for (s32 i = 0; i != (s32) omm_static_array_length(sPrecompiledShaders); ++i) {
        gfx_lookup_or_create_shader_program(sPrecompiledShaders[i]);
    }
    configPrecacheRes = (gOmmPreloadTextures != 0);
}

void gfx_start_frame() {
    sStartTime = gfx_get_current_time();
    gfx_init_frame_interpolation();
    gfx_set_config();
}

void gfx_run(UNUSED Gfx *commands) {
}

void gfx_end_frame() {
    gfx_render_interpolated_frames();
    gfx_set_config();
}

void gfx_shutdown() {
    if (sGfxRapi) {
        if (sGfxRapi->shutdown) sGfxRapi->shutdown();
        sGfxRapi = NULL;
    }
    if (sGfxWapi) {
        if (sGfxWapi->shutdown) sGfxWapi->shutdown();
        sGfxWapi = NULL;
    }
}

//
// Extra
//

#include "data/omm/peachy/omm_sparkly_bowser_4_gfx.inl"
#include "data/omm/peachy/omm_peach_tiara_gfx.inl"
#include "data/omm/level/omm_level_peachy.c"

#define GFX_PC_C
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/engine/gfx_texture.h"
#include "data/omm/system/omm_thread.h"
#include "gfx_patches.inl"
#include <unistd.h>

// Disable optimization (for testing purposes)
#if 0
#undef OMM_OPTIMIZE
#define OMM_OPTIMIZE
#endif

// Enable level palettes in SM64
#if OMM_GAME_IS_SM64
#define LEVEL_PALETTES 1
#else
#define LEVEL_PALETTES 0
#endif

// Enable GL transparency (alpha to coverage/order-independent transparency) on some Render96 builds
#if OMM_GAME_IS_R96X && OMM_GFX_API_GL && defined(TRANSPARENCY_GL)
#include "GL/glew.h"
#define GL_TRANSPARENCY 1
#else
#define GL_TRANSPARENCY 0
#endif

//
// Constants
//

#define INV_32                              (1.f / 32.f)
#define INV_64                              (1.f / 64.f)
#define INV_256                             (1.f / 256.f)

#define GAME_UPDATES_PER_SECOND             (30)
#define GAME_UPDATE_DURATION                (1.0 / (f64) gGameUpdatesPerSecond)
#if OMM_GFX_API_DX
#define UNLIMITED_FPS                       (1000)
#else
#define UNLIMITED_FPS                       (1000000)
#endif

#define GFX_TEXTURE_PRECACHE_MODE_NONE      (0)
#define GFX_TEXTURE_PRECACHE_MODE_RAW       (1)
#define GFX_TEXTURE_PRECACHE_MODE_PNG       (2)
#define GFX_TEXTURE_RAW_FILENAME            "gfx." OMM_GAME_CODE ".raw"

#define GFX_RATIO_X                         (gfx_current_dimensions.width / (f32) SCREEN_WIDTH)
#define GFX_RATIO_Y                         (gfx_current_dimensions.height / (f32) SCREEN_HEIGHT)
#define GFX_MAX_STACK                       (64)
#define GFX_MAX_VERTS                       (64)
#define GFX_MAX_TRIS                        (256)
#define GFX_NO_FOG                          (omm_world_is_flooded() || omm_world_is_frozen() || omm_world_is_dark())
#define GFX_CCMUX_SIZE                      (0x20)
#define GFX_CCMUX_BITS                      (0x1F)

#define GFX_OP                              ((u8) (sGfxCmd->words.w0 >> 24))
#define GFX_W0                              (sGfxCmd->words.w0)
#define GFX_W1                              (sGfxCmd->words.w1)
#define GFX_W1P                             ((void *) sGfxCmd->words.w1)
#define GFX_C0(off, siz)                    ((sGfxCmd->words.w0 >> (off)) & ((1U << siz) - 1))
#define GFX_C1(off, siz)                    ((sGfxCmd->words.w1 >> (off)) & ((1U << siz) - 1))
#define GFX_CMD_SIZE                        (0x40)
#define GFX_CMD_BITS                        (0x3F)

//
// Inlines
//

#define mtx_stack_inc(x)                    sGfxProc->mtx += (x)
#define mtx_stack_dec(x)                    sGfxProc->mtx -= (x)
#define mtx_mul(c, a, b)                    mtxf_mul((void *) (c), (void *) (a), (void *) (b))
#define gfx_adjust_x_for_aspect_ratio(x)    ((x) * sGfxAdjustForAspectRatio)
#define tex_u_normalize(tu)                 ((((tu) - (sGfxProc->texTileSize.uls << 3)) * INV_32 + sGfxProc->texOffset) * sGfxProc->invTexWidth)
#define tex_v_normalize(tv)                 ((((tv) - (sGfxProc->texTileSize.ult << 3)) * INV_32 + sGfxProc->texOffset) * sGfxProc->invTexHeight)

//
// Structs
//

typedef struct GfxWindowManagerAPI GfxWindowManagerAPI;
typedef struct GfxRenderingAPI GfxRenderingAPI;
typedef void (*GfxCmdFunc)();

typedef struct {
    u16 x, y, w, h;
} GfxViewport;

typedef struct {
    u16 uls, ult, lrs, lrt;
    u8 cms, cmt;
} GfxTexTileSize;

typedef struct {
    const Vtx *vtx;
    Vec4f p;
    Vec3f n;
    Vec2f tc;
    Vec4f color;
    u8 rej, shouldUpdateTC;
} GfxVertex;

typedef struct {
    sys_path_t name;
    sys_path_t path;
    u64 size;
    struct {
        s32 width;
        s32 height;
        u8 *data;
    } decompressed;
} PngEntry;

typedef struct GfxTextureNode {
    GfxTexture *tex;
    struct GfxTextureNode *next;
} GfxTextureNode;

typedef struct {
    bool precache;
    u8 mode;
    GfxTexture *tex;
    GfxTextureNode head[1];
    GfxTextureNode *curr;
    GfxTextureNode *tail;
    FILE *raw;
    OmmHMap_(PngEntry *) png;
} GfxTexturePrecache;

typedef struct {
    struct ShaderProgram *prg;
    u8 mapShaderInput[2][4];
    u32 id;
} GfxColorCombiner;

typedef struct {
    struct ShaderProgram *prg;
    bool usedTextures[2];
    u8 numInputs;
} GfxShaderProgram;

typedef struct {
    f32 buffer[GFX_MAX_TRIS * 3 * 26];
    s32 length;
    s32 tris;
} GfxVertexBufferObject;

typedef struct {
    u16 tcOn, tcS, tcT;
    GfxTexTileSize texTileSize;
    const char *texToLoad;
    const char *texLoaded[2];
    GfxTexture *textures[2];
} GfxTextureState;

//
// Processor
//

typedef struct {

    // Matrices
    Mat4 *mtx;
    Mat4 mtxP;
    ALIGNED16 Mat4 mtxMP;

    // Lights
    Light_t light, lightAmb;
    Vec3f lightCoeffs, lookAtXCoeffs, lookAtYCoeffs;
    bool lightChanged, lookAtChanged;

    // Vertices
    bool invertCulling;
    s16 fogMul, fogOff;
    u16 tcOn, tcS, tcT;
    u32 geometryMode;

    // Textures
    GfxTexTileSize texTileSize;
    f32 invTexWidth, invTexHeight, texOffset;
    s32 texTile;
    const char *texToLoad;
    const char *texLoaded[2];
    GfxTexture *textures[2];
    bool texChanged[2];

    // Colors
    bool fog, alpha, shouldUpdateCC;
    u32 combineMode, otherModeL, otherModeH;
    Vec4f envColor, primColor, fogColor, fillColor, transpColor;
#if LEVEL_PALETTES
    Vec4f palColor;
#endif
    GfxColorCombiner *colorCombiner;
    GfxShaderProgram shaderProgram[1];
    GfxViewport viewport;
    void *zBufAddress;
    void *colorImageAddress;

#if OMM_GAME_IS_R96X
    s32 wdwSwapVtx;
#endif
} GfxProcessor;

//
// Data
//

static Mat4                     sGfxMtx[GFX_MAX_STACK];
static GfxVertex                sGfxVtx[GFX_MAX_VERTS + 4];
static GfxVertex               *sGfxVtxExt = NULL;
static GfxProcessor             sGfxProc[1];
static GfxVertexBufferObject    sGfxVbo[1];
static GfxWindowManagerAPI     *sGfxWapi;
static GfxRenderingAPI         *sGfxRapi;
static GfxTextureState          sGfxState[1];
static GfxTexturePrecache       sGfxPrecache[1];
static OmmHMap_(GfxTexture *)   sGfxCache = omm_hmap_zero;
static GfxCmdFunc               sGfxCmdTable[GFX_CMD_SIZE];
struct GfxDimensions            gfx_current_dimensions;
static f32                      sGfxAdjustForAspectRatio;

#if !OMM_CODE_DEV
static const
#endif
u32 gGameUpdatesPerSecond = GAME_UPDATES_PER_SECOND;
static struct {
    u32 target;
    f32 lerp;
    u32 frames;
    f64 tFrame;
    f64 tLast;
} sFps[1];

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
// Profiling
//

#if OMM_CODE_DEBUG

static const struct { u8 op; u32 mult; const char *name; } OMM_GFX_PROFILER_OPS[GFX_CMD_SIZE] = {
[GFX_CMD_BITS & G_VTX           ] = { GFX_CMD_BITS & G_VTX           , 1, "sp_vertex"            },
[GFX_CMD_BITS & G_VTXTC         ] = { GFX_CMD_BITS & G_VTX           , 1, NULL                   },
[GFX_CMD_BITS & G_VTXEXT        ] = { GFX_CMD_BITS & G_VTXEXT        , 1, "sp_vertex_ext"        },
[GFX_CMD_BITS & G_VTXEXTTC      ] = { GFX_CMD_BITS & G_VTXEXT        , 1, NULL                   },
[GFX_CMD_BITS & G_TRI1          ] = { GFX_CMD_BITS & G_TRI1          , 1, "sp_tri"               },
[GFX_CMD_BITS & G_TRI2          ] = { GFX_CMD_BITS & G_TRI1          , 2, NULL                   },
[GFX_CMD_BITS & G_TRIEXT        ] = { GFX_CMD_BITS & G_TRIEXT        , 1, "sp_tri_ext"           },
[GFX_CMD_BITS & G_INVERTCULL    ] = { GFX_CMD_BITS & G_INVERTCULL    , 1, "xp_invert_culling"    },
[GFX_CMD_BITS & G_PUSHTEX       ] = { GFX_CMD_BITS & G_PUSHTEX       , 1, "xp_push_texture"      },
[GFX_CMD_BITS & G_POPTEX        ] = { GFX_CMD_BITS & G_POPTEX        , 1, "xp_pop_texture"       },
[GFX_CMD_BITS & G_COPYMEM       ] = { GFX_CMD_BITS & G_COPYMEM       , 1, "sp_copymem"           },
[GFX_CMD_BITS & G_TEXTURE       ] = { GFX_CMD_BITS & G_TEXTURE       , 1, "sp_texture"           },
[GFX_CMD_BITS & G_POPMTX        ] = { GFX_CMD_BITS & G_POPMTX        , 1, "sp_pop_matrix"        },
[GFX_CMD_BITS & G_GEOMETRYMODE  ] = { GFX_CMD_BITS & G_GEOMETRYMODE  , 1, "sp_geometry_mode"     },
[GFX_CMD_BITS & G_MTX           ] = { GFX_CMD_BITS & G_MTX           , 1, "sp_matrix"            },
[GFX_CMD_BITS & G_MOVEWORD      ] = { GFX_CMD_BITS & G_MOVEWORD      , 1, "sp_moveword"          },
[GFX_CMD_BITS & G_MOVEMEM       ] = { GFX_CMD_BITS & G_MOVEMEM       , 1, "sp_movemem"           },
[GFX_CMD_BITS & G_SETOTHERMODE_L] = { GFX_CMD_BITS & G_SETOTHERMODE_L, 1, "sp_set_other_mode_l"  },
[GFX_CMD_BITS & G_SETOTHERMODE_H] = { GFX_CMD_BITS & G_SETOTHERMODE_H, 1, "sp_set_other_mode_h"  },
[GFX_CMD_BITS & G_TEXRECT       ] = { GFX_CMD_BITS & G_TEXRECT       , 1, "dp_texture_rectangle" },
[GFX_CMD_BITS & G_TEXRECTFLIP   ] = { GFX_CMD_BITS & G_TEXRECT       , 1, NULL                   },
[GFX_CMD_BITS & G_SETSCISSOR    ] = { GFX_CMD_BITS & G_SETSCISSOR    , 1, "dp_set_scissor"       },
[GFX_CMD_BITS & G_SWAPCMD       ] = { GFX_CMD_BITS & G_SWAPCMD       , 1, "xp_swap_cmd"          },
[GFX_CMD_BITS & G_SETTILESIZE   ] = { GFX_CMD_BITS & G_SETTILESIZE   , 1, "dp_set_tile_size"     },
[GFX_CMD_BITS & G_LOADBLOCK     ] = { GFX_CMD_BITS & G_LOADBLOCK     , 1, "dp_load_block"        },
[GFX_CMD_BITS & G_LOADTILE      ] = { GFX_CMD_BITS & G_LOADTILE      , 1, "dp_load_tile"         },
[GFX_CMD_BITS & G_SETTILE       ] = { GFX_CMD_BITS & G_SETTILE       , 1, "dp_set_tile"          },
[GFX_CMD_BITS & G_FILLRECT      ] = { GFX_CMD_BITS & G_FILLRECT      , 1, "dp_fill_rectangle"    },
[GFX_CMD_BITS & G_SETFILLCOLOR  ] = { GFX_CMD_BITS & G_SETFILLCOLOR  , 1, "dp_set_fill_color"    },
[GFX_CMD_BITS & G_SETFOGCOLOR   ] = { GFX_CMD_BITS & G_SETFOGCOLOR   , 1, "dp_set_fog_color"     },
[GFX_CMD_BITS & G_SETPRIMCOLOR  ] = { GFX_CMD_BITS & G_SETPRIMCOLOR  , 1, "dp_set_prim_color"    },
[GFX_CMD_BITS & G_SETENVCOLOR   ] = { GFX_CMD_BITS & G_SETENVCOLOR   , 1, "dp_set_env_color"     },
[GFX_CMD_BITS & G_SETCOMBINE    ] = { GFX_CMD_BITS & G_SETCOMBINE    , 1, "dp_set_combine_mode"  },
[GFX_CMD_BITS & G_SETTIMG       ] = { GFX_CMD_BITS & G_SETTIMG       , 1, "dp_set_texture_image" },
[GFX_CMD_BITS & G_SETZIMG       ] = { GFX_CMD_BITS & G_SETZIMG       , 1, "dp_set_z_image"       },
[GFX_CMD_BITS & G_SETCIMG       ] = { GFX_CMD_BITS & G_SETCIMG       , 1, "dp_set_color_image"   },
};

static struct {
    u64 curr;
    u64 sum;
    f64 disp[2];
    u32 count;
} sOmmGfxProfiler[GFX_CMD_SIZE];

OMM_OPTIMIZE OMM_INLINE void omm_gfx_profiler_update_op(u8 op) {
    static u8 sPrevOp = 0;
    sOmmGfxProfiler[op].count++;
    if (OMM_LIKELY(sPrevOp != op)) {
        u64 curr = SDL_GetPerformanceCounter();
        sOmmGfxProfiler[sPrevOp].sum += curr - sOmmGfxProfiler[sPrevOp].curr;
        sOmmGfxProfiler[op].curr = curr;
        sPrevOp = op;
    }
}

void omm_gfx_profiler_update(u32 refreshRate) {
    f64 freq = SDL_GetPerformanceFrequency();
    f64 disp0[GFX_CMD_SIZE] = {0};
    f64 disp1[GFX_CMD_SIZE] = {0};
    for (u32 i = 0; i != GFX_CMD_SIZE; ++i) {
        u8 iop = OMM_GFX_PROFILER_OPS[i].op;
        if (iop) {
            disp0[iop] += (f64) sOmmGfxProfiler[i].sum / (freq * refreshRate);
            disp1[iop] += (f64) (sOmmGfxProfiler[i].count * OMM_GFX_PROFILER_OPS[i].mult) / refreshRate;
        }
    }
    for (u32 i = 0; i != GFX_CMD_SIZE; ++i) {
        u8 iop = OMM_GFX_PROFILER_OPS[i].op;
        if (iop) {
            sOmmGfxProfiler[i].disp[0] = disp0[iop];
            sOmmGfxProfiler[i].disp[1] = disp1[iop];
            sOmmGfxProfiler[i].sum = 0;
            sOmmGfxProfiler[i].count = 0;
        }
    }
}

const char *omm_gfx_profiler_get_display_info(u8 op, f64 *disp0, f64 *disp1) {
    const char *name = OMM_GFX_PROFILER_OPS[op].name;
    if (name) {
        *disp0 = sOmmGfxProfiler[op].disp[0];
        *disp1 = sOmmGfxProfiler[op].disp[1];
    }
    return name;
}

#endif

//
// Texture cache
//

OMM_INLINE GfxTexture *gfx_texture_find(const char *name) {
    extern void *omm_models_find_texture(OmmHMap *, struct GfxRenderingAPI *, const void *);
    GfxTexture *tex = (GfxTexture *) omm_models_find_texture(&sGfxCache, sGfxRapi, (const void *) name);
    if (tex) {
        return tex;
    }
    s32 i = omm_hmap_find(sGfxCache, str_hash(name));
    if (i != -1) {
        return omm_hmap_get(sGfxCache, GfxTexture *, i);
    }
    return NULL;
}

//
// Palettes
//

bool gfx_texture_has_palette(const char *texname) {
    static const char *sOmmTexturePatternsWithPalette[] = {
#define OMM_PALETTE_(id, str) OMM_PALETTE_##id,
#define OMM_PALETTE_LEVEL_(id, str) OMM_PALETTE_##id,
#include "data/omm/omm_defines_palettes.inl"
#undef OMM_PALETTE_
#undef OMM_PALETTE_LEVEL_
    };
    array_for_each_(const char *, pattern, sOmmTexturePatternsWithPalette) {
        if (strstr(texname, *pattern)) {
            return true;
        }
    }
    return false;
}

#if LEVEL_PALETTES

#define GFX_LEVEL_PALETTE_FLOODED   (1 << 0)
#define GFX_LEVEL_PALETTE_FROZEN    (1 << 1)
#define GFX_LEVEL_PALETTE_DARK      (1 << 2)
#define GFX_LEVEL_PALETTE_COLOR     (1 << 3)

static bool gfx_texture_is_level_palette(const char *texname) {
    static const char *sOmmTexturePatternsWithLevelPalette[] = {
#define OMM_PALETTE_(id, str)
#define OMM_PALETTE_LEVEL_(id, str) OMM_PALETTE_##id,
#include "data/omm/omm_defines_palettes.inl"
#undef OMM_PALETTE_
#undef OMM_PALETTE_LEVEL_
    };
    array_for_each_(const char *, pattern, sOmmTexturePatternsWithLevelPalette) {
        if (strstr(texname, *pattern)) {
            return true;
        }
    }
    return false;
}

#define set_r(r_) data[i + 0] = r = clamp_s(r_, 0x00, 0xFF)
#define set_g(g_) data[i + 1] = g = clamp_s(g_, 0x00, 0xFF)
#define set_b(b_) data[i + 2] = b = clamp_s(b_, 0x00, 0xFF)
static void gfx_texture_load_level_palette(GfxTexture *tex, const char *texname) {
    u32 pal = (
        (GFX_LEVEL_PALETTE_FLOODED * (omm_world_is_flooded() == true)) |
        (GFX_LEVEL_PALETTE_FROZEN  * (omm_world_is_frozen()  == true)) |
        (GFX_LEVEL_PALETTE_DARK    * (omm_world_is_dark()    == true)) |
        (GFX_LEVEL_PALETTE_COLOR   * (sGfxProc->palColor[3]  != 0))
    );
    if (pal & GFX_LEVEL_PALETTE_COLOR) {
        u8 rmod = (u8) (sGfxProc->palColor[0] * 127.9f);
        u8 gmod = (u8) (sGfxProc->palColor[1] * 127.9f);
        u8 bmod = (u8) (sGfxProc->palColor[2] * 127.9f);
        pal |= (((u32) rmod) << 24);
        pal |= (((u32) gmod) << 16);
        pal |= (((u32) bmod) <<  8);
    }
    if (tex->data && tex->pal != pal && gfx_texture_is_level_palette(texname)) {
        if (pal) {
            u32 size = tex->w * tex->h * 4;
            u8 *data = mem_dup(tex->data, size);
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
                if (pal & GFX_LEVEL_PALETTE_DARK) {
                    set_r(0);
                    set_g(0);
                    set_b(0);
                }
                if (pal & GFX_LEVEL_PALETTE_COLOR) {
                    set_r(r * sGfxProc->palColor[0]);
                    set_g(g * sGfxProc->palColor[1]);
                    set_b(b * sGfxProc->palColor[2]);
                }
            }
            sGfxRapi->upload_texture(data, tex->w, tex->h);
            mem_del(data);
        } else {
            sGfxRapi->upload_texture(tex->data, tex->w, tex->h);
        }
        tex->pal = pal;
    }
}

void gfx_texture_set_level_palette_color(f32 r, f32 g, f32 b) {
    sGfxProc->palColor[0] = clamp_0_1_f(r);
    sGfxProc->palColor[1] = clamp_0_1_f(g);
    sGfxProc->palColor[2] = clamp_0_1_f(b);
    sGfxProc->palColor[3] = 1;
}

#endif

//
// Precaching (raw)
//

static const char *gfx_texture_precache_read_raw(u64 *loaded_bytes, sys_path_t texname, u8 **data, s32 *texw, s32 *texh) {
    if (sGfxPrecache->mode == GFX_TEXTURE_PRECACHE_MODE_RAW && sGfxPrecache->raw) {
        u64 read_bytes = 0;

        // Texture name length
        u32 len;
        if (fread(&len, sizeof(u32), 1, sGfxPrecache->raw) == 0) {
            return NULL;
        }
        read_bytes += sizeof(u32) * 1;

        // Texture name
        if (fread(texname, sizeof(char), len, sGfxPrecache->raw) < len) {
            return NULL;
        }
        read_bytes += sizeof(char) * len;

        // Texture width
        if (fread(texw, sizeof(s32), 1, sGfxPrecache->raw) == 0) {
            return NULL;
        }
        read_bytes += sizeof(s32) * 1;

        // Texture height
        if (fread(texh, sizeof(s32), 1, sGfxPrecache->raw) == 0) {
            return NULL;
        }
        read_bytes += sizeof(s32) * 1;

        // Texture data
        u32 size = *texw * *texh * 4;
        *data = mem_new(u8, size);
        if (fread(*data, sizeof(u8), size, sGfxPrecache->raw) < size) {
            mem_del(*data);
            return NULL;
        }
        read_bytes += sizeof(u8) * size;

        *loaded_bytes += read_bytes;
        return texname;
    }
    return NULL;
}

static void gfx_texture_precache_write_raw(const char *texname, const u8 *data, s32 texw, s32 texh) {
    if (sGfxPrecache->mode == GFX_TEXTURE_PRECACHE_MODE_PNG && sGfxPrecache->raw) {
        u32 len = strlen(texname) + 1;
        fwrite(&len, sizeof(u32), 1, sGfxPrecache->raw);
        fwrite(texname, sizeof(char), len, sGfxPrecache->raw);
        fwrite(&texw, sizeof(s32), 1, sGfxPrecache->raw);
        fwrite(&texh, sizeof(s32), 1, sGfxPrecache->raw);
        fwrite(data, sizeof(u8), texw * texh * 4, sGfxPrecache->raw);
    }
}

//
// Texture load
//

static GfxTexture *gfx_texture_create(const char *texname, const u8 *data, s32 texw, s32 texh) {

    // Create gfx texture entry
    GfxTexture *tex = mem_new(GfxTexture, 1);
    tex->hash = str_hash(texname);
    tex->id   = (sGfxPrecache->precache ? 0 : sGfxRapi->new_texture());
    tex->w    = texw;
    tex->h    = texh;
    tex->cms  = 0;
    tex->cmt  = 0;
    tex->lin  = 0;
    tex->pal  = 0;
    tex->ptr  = (sGfxPrecache->precache ? (u8 *) data : NULL);
    tex->data = (gfx_texture_has_palette(texname) ? mem_dup(data, texw * texh * 4) : NULL);
    omm_hmap_insert(sGfxCache, tex->hash, tex);

    // Send it to the rendering api
    if (!sGfxPrecache->precache) {
        sGfxRapi->select_texture(0, tex->id);
        sGfxRapi->set_sampler_parameters(0, false, 0, 0);
        sGfxRapi->upload_texture(data, tex->w, tex->h);
    }
    return tex;
}

static void gfx_texture_create_node(GfxTexture *tex) {
    GfxTextureNode *node = mem_new(GfxTextureNode, 1);
    node->tex = tex;
    node->next = NULL;
    sGfxPrecache->tail->next = node;
    sGfxPrecache->tail = node;
}

static GfxTexture *gfx_texture_load(const char *texname, u8 *data, s32 texw, s32 texh, bool whiten) {
    if (whiten) {
        for (s32 i = 0; i != 4 * texw * texh; i += 4) {
            data[i + 0] = data[i + 1] = data[i + 2] = 0xFF;
        }
    }
    gfx_texture_precache_write_raw(texname, data, texw, texh);
    GfxTexture *tex = gfx_texture_create(texname, data, texw, texh);
    if (sGfxPrecache->precache) {
        gfx_texture_create_node(tex);
    } else {
        sGfxPrecache->tex = tex;
        mem_del(data);
    }
    return tex;
}

static GfxTexture *gfx_texture_load_png(const char *texname, const char *filename) {

    // Whiten flag
    char *whiten = strstr((char *) filename, OMM_TEXTURE_WHITEN);
    if (whiten) snprintf(whiten, 8, ".png");

    // Load texture data from PNG
    s32 texw, texh;
    u8 *data = fs_load_png(filename, &texw, &texh);
    if (data) {
        return gfx_texture_load(texname, data, texw, texh, whiten != NULL);
    }

    // Find the texture in ROM
    sys_path_t assetname;
    str_cpy(assetname, sizeof(assetname), texname);
    whiten = strstr(assetname, OMM_TEXTURE_WHITEN);
    if (whiten) *whiten = 0;
    data = rom_asset_load_texture(assetname, &texw, &texh);
    if (data) {
        return gfx_texture_load(texname, data, texw, texh, whiten != NULL);
    }

    // Missing texture
    if (sGfxPrecache->precache) return NULL;
    fprintf(stderr, "Cannot load texture \"%s\" from file \"%s\"\n", texname, filename);
    fflush(stderr);
    return gfx_texture_create(texname, sMissingTexture, 4, 4);
}

OMM_INLINE void gfx_texture_import(s32 tile) {
    const char *texname = sGfxProc->texLoaded[tile];
    if (texname) {
        GfxTexture *tex = gfx_texture_find(texname);
        if (OMM_UNLIKELY(!tex)) {
            sys_path_t filename;
            str_cat(filename, sizeof(filename), FS_TEXTUREDIR "/", texname, ".png");
            tex = gfx_texture_load_png(texname, filename);
        }
        sGfxProc->textures[tile] = tex;
        sGfxRapi->select_texture(tile, tex->id);
#if LEVEL_PALETTES
        gfx_texture_load_level_palette(tex, texname);
#endif
    }
}

OMM_INLINE void gfx_texture_import_loading_screen(s32 tile) {
    GfxTexture *tex = sGfxPrecache->tex;
    if (tex) {
        sGfxProc->textures[tile] = tex;
        sGfxRapi->select_texture(tile, tex->id);
    }
}

//
// Precaching
//

static bool gfx_texture_should_precache(const char *name) {

    // Ignore spaces
    if (strstr(name, " ")) {
        return false;
    }

    // Precache "actors/mario/" and "actors/mario_cap/"
    if (strstr(name, "actors/mario/") || strstr(name, "actors/mario_cap/")) {
        return true;
    }

    // Ignore "unused"
    if (strstr(name, "unused")) {
        return false;
    }

    // Precache "/skyboxes/"
    if (strstr(name, "/skyboxes/")) {
        return true;
    }

    // Don't precache "COURSE_"
    if (strstr(name, "COURSE_")) {
        return false;
    }

    // Don't precache loading screen textures
    if (strstr(name, OMM_TEXTURE_MISC_LOADING_SCREEN_0) || strstr(name, OMM_TEXTURE_MISC_LOADING_SCREEN_1)) {
        return false;
    }

    // Ignore textures not terminating with ".rgba16", ".rgba32", ".ia4", ".ia8" or ".ia16"
    if (!strstr(name, ".rgba16") && !strstr(name, ".rgba32") && !strstr(name, ".ia4") && !strstr(name, ".ia8") && !strstr(name, ".ia16")) {
        return false;
    }

    // If not "textures/", precache
    if (!strstr(name, "textures/")) {
        return true;
    }

    // Don't precache "/intro_raw/", "/ipl3_raw/", "/skybox" or "/special/"
    if (strstr(name, "/intro_raw/") || strstr(name, "/ipl3_raw/") || strstr(name, "/skybox") || strstr(name, "/special/")) {
        return false;
    }

    // OK
    return true;
}

static bool gfx_texture_build_precache_from_fs(UNUSED void *user, const char *path) {

    // Check if it's a png file
    if (strstr(path, ".png") || strstr(path, ".PNG")) {
        sys_path_t texname;
        str_cpy(texname, sizeof(texname), path);
        texname[strlen(texname) - 4] = 0;

        // Check if it starts with "gfx/"
        if (strstr(texname, FS_TEXTUREDIR "/") == texname) {
            char *name = texname + sizeof(FS_TEXTUREDIR "/") - 1;

            // Remove the extension of skybox textures
            if (strstr(name, "/skyboxes/")) {
                char *ext = strchr(name, '.');
                if (ext) *ext = 0;
            }

            // Check if new hash
            u32 hash = str_hash(name);
            if (omm_hmap_find(sGfxPrecache->png, hash) == -1 && gfx_texture_should_precache(name)) {
                PngEntry *entry = mem_new(PngEntry, 1);
                str_cpy(entry->name, sizeof(sys_path_t), name);
                str_cpy(entry->path, sizeof(sys_path_t), path);
                entry->size = (u64) fs_fsize(path);
                omm_hmap_insert(sGfxPrecache->png, hash, entry);
            }
        }
    }
    return true;
}

static bool gfx_texture_build_precache_from_rom(const char *name, s32 width, s32 height, u8 *data) {
    u32 hash = str_hash(name);
    if (omm_hmap_find(sGfxPrecache->png, hash) == -1 && gfx_texture_should_precache(name)) {
        PngEntry *entry = mem_new(PngEntry, 1);
        str_cpy(entry->name, sizeof(sys_path_t), name);
        entry->decompressed.width = width;
        entry->decompressed.height = height;
        entry->decompressed.data = data;
        entry->size = width * height * 4 * sizeof(u8);
        omm_hmap_insert(sGfxPrecache->png, hash, entry);
        return true;
    }
    return false;
}

static void gfx_texture_precache_init() {

    // Initialize pointers
    mem_zero(sGfxPrecache, sizeof(GfxTexturePrecache));
    sGfxPrecache->curr = sGfxPrecache->head;
    sGfxPrecache->tail = sGfxPrecache->head;

    // Open the gfx.raw file if texture caching is set to Permanent, otherwise delete it
    sys_path_t filepath;
    fs_get_game_path(filepath, GFX_TEXTURE_RAW_FILENAME);
    if (gOmmTextureCaching == OMM_TEXTURE_CACHING_PERMANENT) {
        sGfxPrecache->raw = fopen(filepath, "rb");
        sGfxPrecache->mode = GFX_TEXTURE_PRECACHE_MODE_RAW;
    } else if (fs_sys_file_exists(filepath)) {
        fs_sys_delete_file(filepath);
    }

    // If no gfx.raw file, precache textures from ROM and texture packs
    if (!sGfxPrecache->raw && gOmmTextureCaching != OMM_TEXTURE_CACHING_DISABLED) {
        fs_walk(FS_TEXTUREDIR, gfx_texture_build_precache_from_fs, NULL, true, FS_DIR_READ);
        rom_asset_precache_textures(gfx_texture_build_precache_from_rom);
        sGfxPrecache->mode = GFX_TEXTURE_PRECACHE_MODE_PNG;

        // Attempt to create a gfx.raw file if texture caching is set to Permanent
        if (gOmmTextureCaching == OMM_TEXTURE_CACHING_PERMANENT) {
            sGfxPrecache->raw = fopen(filepath, "wb");
        }
    }
}

void gfx_texture_precache_start(u64 *loaded_bytes) {
    sGfxPrecache->precache = true;

    // Read textures from gfx.raw
    if (sGfxPrecache->mode == GFX_TEXTURE_PRECACHE_MODE_RAW && sGfxPrecache->raw) {
        s32 texw, texh; u8 *data;
        sys_path_t texname;
        while (gfx_texture_precache_read_raw(loaded_bytes, texname, &data, &texw, &texh) != NULL) {
            GfxTexture *tex = gfx_texture_create(texname, data, texw, texh);
            gfx_texture_create_node(tex);
        }
    }

    // Read textures from PNG entries
    if (sGfxPrecache->mode == GFX_TEXTURE_PRECACHE_MODE_PNG) {
        omm_hmap_for_each(sGfxPrecache->png, _, v) {
            PngEntry *entry = (PngEntry *) *v;
            const char *name = entry->name;
            if (!gfx_texture_find(name)) {
                if (entry->decompressed.data) {
                    gfx_texture_precache_write_raw(name, entry->decompressed.data, entry->decompressed.width, entry->decompressed.height);
                    GfxTexture *tex = gfx_texture_create(name, entry->decompressed.data, entry->decompressed.width, entry->decompressed.height);
                    gfx_texture_create_node(tex);
                } else {
                    gfx_texture_load_png(name, entry->path);
                }
            }
            *loaded_bytes += entry->size;
        }

        // If a gfx.raw file was created, close it now to not add textures from model packs
        if (sGfxPrecache->raw) {
            fclose(sGfxPrecache->raw);
            sGfxPrecache->raw = NULL;
        }
    }
}

static void gfx_texture_precache_upload_texture(GfxTexture *tex) {
    if (tex && tex->ptr) {
        tex->id = sGfxRapi->new_texture();
        sGfxRapi->select_texture(0, tex->id);
        sGfxRapi->set_sampler_parameters(0, false, 0, 0);
        sGfxRapi->upload_texture(tex->ptr, tex->w, tex->h);
        mem_del(tex->ptr);
    }
}

static void gfx_texture_precache_upload() {
    if (sGfxPrecache->precache) {
        const GfxTextureNode *tail = sGfxPrecache->tail;
        while (sGfxPrecache->curr != tail) {
            gfx_texture_precache_upload_texture(sGfxPrecache->curr->tex);
            sGfxPrecache->curr = sGfxPrecache->curr->next;
        }
    }
}

void gfx_texture_precache_end() {

    // Close the gfx.raw file
    if (sGfxPrecache->raw) {
        fclose(sGfxPrecache->raw);
    }

    // Upload all remaining textures and free nodes
    for (GfxTextureNode *node = sGfxPrecache->head->next; node;) {
        gfx_texture_precache_upload_texture(node->tex);
        GfxTextureNode *next = node->next;
        mem_del(node);
        node = next;
    }

    // Free PNG entries
    omm_hmap_for_each(sGfxPrecache->png, _, v) {
        PngEntry *entry = (PngEntry *) *v;
        mem_del(entry);
    }
    omm_hmap_delete(sGfxPrecache->png);

    // End of precaching
    mem_zero(sGfxPrecache, sizeof(GfxTexturePrecache));
}

u64 gfx_texture_precache_get_size() {
    u64 size = 0;

    // Size of gfx.raw file
    if (sGfxPrecache->mode == GFX_TEXTURE_PRECACHE_MODE_RAW && sGfxPrecache->raw) {
        fseeko(sGfxPrecache->raw, 0, SEEK_END);
        size = (u64) ftello(sGfxPrecache->raw);
        fseeko(sGfxPrecache->raw, 0, SEEK_SET);
    }

    // Sum of size of PNG entries
    if (sGfxPrecache->mode == GFX_TEXTURE_PRECACHE_MODE_PNG) {
        omm_hmap_for_each(sGfxPrecache->png, _, v) {
            PngEntry *entry = (PngEntry *) *v;
            size += entry->size;
        }
    }

    return size;
}

//
// Gfx
//

static Gfx *sGfxCmd;

static void gfx_run_dl(Gfx *cmd);

OMM_INLINE void gfx_flush() {
    if (sGfxVbo->length) {
        sGfxRapi->draw_triangles(sGfxVbo->buffer, sGfxVbo->length, sGfxVbo->tris);
        sGfxVbo->length = 0;
        sGfxVbo->tris = 0;
    }
}

//
// Shaders
//

static void *gfx_lookup_or_create_shader_program(u32 shaderId) {
    void *prg = sGfxRapi->lookup_shader(shaderId);
    if (!prg) {
        sGfxRapi->unload_shader(sGfxProc->shaderProgram->prg);
        prg = sGfxRapi->create_and_load_new_shader(shaderId);
        sGfxRapi->shader_get_info(prg, &sGfxProc->shaderProgram->numInputs, sGfxProc->shaderProgram->usedTextures);
        sGfxProc->shaderProgram->prg = prg;
    }
    return prg;
}

OMM_INLINE void gfx_update_shader() {
    if (sGfxProc->colorCombiner->prg != sGfxProc->shaderProgram->prg) {
        gfx_flush();
        sGfxRapi->unload_shader(sGfxProc->shaderProgram->prg);
        sGfxRapi->load_shader(sGfxProc->colorCombiner->prg);
        sGfxRapi->shader_get_info(sGfxProc->colorCombiner->prg, &sGfxProc->shaderProgram->numInputs, sGfxProc->shaderProgram->usedTextures);
        sGfxProc->shaderProgram->prg = sGfxProc->colorCombiner->prg;
    }
}

//
// Color combiners
//

static GfxColorCombiner *gfx_lookup_or_create_color_combiner(u32 id) {

    // Find in cache
    static OmmHMap_(GfxColorCombiner *) sGfxColorCombiners = omm_hmap_zero;
    s32 i = omm_hmap_find(sGfxColorCombiners, id + 1);
    if (i != -1) {
        return omm_hmap_get(sGfxColorCombiners, GfxColorCombiner *, i);
    }

    // Create new color combiner
    GfxColorCombiner *cc = mem_new(GfxColorCombiner, 1);
    u8 cci[2][4];
    u32 shaderId = (id & 0xFF000000);
    for (s32 input = 0; input != 4; ++input) {
        cci[0][input] = (id >> (input * 3)) & 7;
        cci[1][input] = (id >> (12 + input * 3)) & 7;
    }
    for (s32 alpha = 0; alpha != 2; ++alpha) {
        if (cci[alpha][0] == cci[alpha][1] || cci[alpha][2] == CC_0) {
            cci[alpha][0] = cci[alpha][1] = cci[alpha][2] = 0;
        }
        u8 inputNumber[8] = {0};
        u32 nextInputNumber = SHADER_INPUT_1;
        for (s32 input = 0; input != 4; ++input) {
            u32 val = 0;
            switch (cci[alpha][input]) {
                case CC_0: break;
                case CC_TEXEL0: val = SHADER_TEXEL0; break;
                case CC_TEXEL1: val = SHADER_TEXEL1; break;
                case CC_TEXEL0A: val = SHADER_TEXEL0A; break;
                case CC_PRIM:
                case CC_SHADE:
                case CC_ENV:
                case CC_LOD: {
                    if (inputNumber[cci[alpha][input]] == 0) {
                        cc->mapShaderInput[alpha][nextInputNumber - 1] = cci[alpha][input];
                        inputNumber[cci[alpha][input]] = nextInputNumber++;
                    }
                    val = inputNumber[cci[alpha][input]];
                } break;
            }
            shaderId |= val << (alpha * 12 + input * 3);
        }
    }
    cc->id = id;
    cc->prg = gfx_lookup_or_create_shader_program(shaderId);
    omm_hmap_insert(sGfxColorCombiners, id + 1, cc);
    return cc;
}

static void gfx_update_color_combiner() {
    gfx_flush();

    // Color effects
    u32 edge = (sGfxProc->otherModeL & CVG_X_ALPHA) == CVG_X_ALPHA;
    u32 noise = (sGfxProc->otherModeL & G_AC_DITHER) == G_AC_DITHER;
    u32 transp = (sGfxProc->geometryMode & G_TRANSPARENCY) != 0;
    u32 fog = sGfxProc->fog = (sGfxProc->otherModeL >> 30) == G_BL_CLR_FOG && !GFX_NO_FOG;
    u32 alpha = sGfxProc->alpha = (transp || edge || ((sGfxProc->otherModeL & (G_BL_A_MEM << 18)) == 0));
    edge &= !transp && !(sGfxProc->geometryMode & G_TEXTURE_ALPHA);
    sGfxProc->transpColor[3] = 1.f - transp * ((sGfxProc->geometryMode & G_TRANSPARENCY) >> 3) * INV_64;

    // Color combiner id
    u32 ccId = (sGfxProc->combineMode | (edge * SHADER_OPT_TEXTURE_EDGE) | (noise * SHADER_OPT_NOISE) | (fog * SHADER_OPT_FOG) | (alpha * SHADER_OPT_ALPHA)) & ~(!alpha * 0xFFF000);
    sGfxRapi->set_use_alpha(alpha);

    // Color combiner
    sGfxProc->colorCombiner = gfx_lookup_or_create_color_combiner(ccId);
#if GL_TRANSPARENCY
    if ((sGfxProc->otherModeL & G_AC_COVERAGE) == G_AC_COVERAGE) {
        glDepthMask(GL_TRUE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        glEnable(GL_SAMPLE_ALPHA_TO_ONE);
    } else {
        glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        glDisable(GL_SAMPLE_ALPHA_TO_ONE);
    }
#endif

    // Update shader
    gfx_update_shader();
}

//
// Changes
//

#define if_changed(prev, curr, flags) if ((((prev) ^ (curr)) & (flags)) != 0)

OMM_INLINE void gfx_geometry_mode_changed(u32 clr, u32 set) {
    u32 prevGeometryMode = sGfxProc->geometryMode;
    sGfxProc->geometryMode &= clr;
    sGfxProc->geometryMode |= set;
    if_changed(prevGeometryMode, sGfxProc->geometryMode, G_ZBUFFER) {
        gfx_flush();
        sGfxRapi->set_depth_test((sGfxProc->geometryMode & G_ZBUFFER) != 0);
    }
    if_changed(prevGeometryMode, sGfxProc->geometryMode, G_TEXTURE_ALPHA | G_TRANSPARENCY) {
        sGfxProc->shouldUpdateCC = true;
    }
}

OMM_INLINE void gfx_combine_mode_changed(u32 mode) {
    if (sGfxProc->combineMode != mode) {
        sGfxProc->combineMode = mode;
        sGfxProc->shouldUpdateCC = true;
    }
}

OMM_INLINE void gfx_other_mode_l_changed(u32 oml) {
    u32 prevOtherModeL = sGfxProc->otherModeL;
    sGfxProc->otherModeL = oml;
    if_changed(prevOtherModeL, sGfxProc->otherModeL, Z_UPD) {
        gfx_flush();
        sGfxRapi->set_depth_mask((sGfxProc->otherModeL & Z_UPD) == Z_UPD);
    }
    if_changed(prevOtherModeL, sGfxProc->otherModeL, ZMODE_DEC) {
        gfx_flush();
        sGfxRapi->set_zmode_decal((sGfxProc->otherModeL & ZMODE_DEC) == ZMODE_DEC);
    }
    if_changed(prevOtherModeL, sGfxProc->otherModeL, 0xC0000000 | CVG_X_ALPHA | G_AC_DITHER | (G_BL_A_MEM << 18) | G_AC_COVERAGE) {
        sGfxProc->shouldUpdateCC = true;
    }
}

OMM_INLINE void gfx_other_mode_h_changed(u32 omh) {
    sGfxProc->otherModeH = omh;
    sGfxProc->texOffset = 0.5f * ((omh & (3U << G_MDSFT_TEXTFILT)) != G_TF_POINT);
}

OMM_INLINE void gfx_tex_tile_size_changed(u16 uls, u16 ult, u16 lrs, u16 lrt) {
    sGfxProc->texTileSize.uls  = uls;
    sGfxProc->texTileSize.ult  = ult;
    sGfxProc->texTileSize.lrs  = lrs;
    sGfxProc->texTileSize.lrt  = lrt;
    sGfxProc->invTexWidth  = 1.f / ((lrs - uls + 4) >> 2);
    sGfxProc->invTexHeight = 1.f / ((lrt - ult + 4) >> 2);
}

OMM_INLINE void gfx_viewport_changed(u16 x, u16 y, u16 w, u16 h) {
    gfx_flush();
    sGfxProc->viewport.x = x;
    sGfxProc->viewport.y = y;
    sGfxProc->viewport.w = w;
    sGfxProc->viewport.h = h;
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
    mtxf_copy(sGfxProc->mtxP, mat);
#if OMM_GAME_IS_R96X
    if (Cheats.EnableCheats && Cheats.ChaosMode) {
        for (s32 axis = 0; axis != 2; ++axis) {
            if (Cheats.ChaosInvertedScreen & (1 << axis)) {
                sGfxProc->mtxP[axis][axis] *= -1;
            }
        }
    }
#endif
}

static void gfx_sp_matrix_projection_mul(Mat4 mat) {
    mtx_mul(sGfxProc->mtxP, mat, sGfxProc->mtxP);
}

static void gfx_sp_matrix_modelview_load_push(Mat4 mat) {
    mtx_stack_inc(1);
    mtxf_copy(*sGfxProc->mtx, mat);
    sGfxProc->lightChanged = true;
    sGfxProc->lookAtChanged = true;
}

static void gfx_sp_matrix_modelview_load_nopush(Mat4 mat) {
    mtxf_copy(*sGfxProc->mtx, mat);
    sGfxProc->lightChanged = true;
    sGfxProc->lookAtChanged = true;
}

static void gfx_sp_matrix_modelview_mul_push(Mat4 mat) {
    mtx_stack_inc(1);
    mtx_mul(*sGfxProc->mtx, mat, *(sGfxProc->mtx - 1));
    sGfxProc->lightChanged = true;
    sGfxProc->lookAtChanged = true;
}

static void gfx_sp_matrix_modelview_mul_nopush(Mat4 mat) {
    mtx_mul(*sGfxProc->mtx, mat, *sGfxProc->mtx);
    sGfxProc->lightChanged = true;
    sGfxProc->lookAtChanged = true;
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
    mtx_mul(sGfxProc->mtxMP, *sGfxProc->mtx, sGfxProc->mtxP);
}

static void gfx_sp_pop_matrix() {
    u32 count = GFX_W1 >> 6;
    mtx_stack_dec(count);
    mtx_mul(sGfxProc->mtxMP, *sGfxProc->mtx, sGfxProc->mtxP);
}

//
// Vertex
//

OMM_INLINE void gfx_compute_light_coeffs(Vec3f coeffs, Vec3f lightdir) {
    vec3f_normalize(vec3f_mult_mtx(coeffs, lightdir, (void *) sGfxProc->mtx));
}

OMM_OPTIMIZE static void gfx_sp_vertex_compute_tex_coords_and_colors(GfxVertex *v) {

    // Coeffs for shading effect
    if (sGfxProc->lightChanged && (sGfxProc->geometryMode & G_LIGHTING)) {
        Vec3f lightDir = { sGfxProc->light.dir[0] / 127.f, sGfxProc->light.dir[1] / 127.f, sGfxProc->light.dir[2] / 127.f };
        gfx_compute_light_coeffs(sGfxProc->lightCoeffs, lightDir);
        sGfxProc->lightChanged = false;
    }

    // Coeffs for metallic effect
    if (sGfxProc->lookAtChanged && (sGfxProc->geometryMode & G_TEXTURE_GEN)) {
        gfx_compute_light_coeffs(sGfxProc->lookAtXCoeffs, gVec3fX);
        gfx_compute_light_coeffs(sGfxProc->lookAtYCoeffs, gVec3fY);
        sGfxProc->lookAtChanged = false;
    }

    // Vertex normal
    v->n[0] = (f32) v->vtx->n.n[0];
    v->n[1] = (f32) v->vtx->n.n[1];
    v->n[2] = (f32) v->vtx->n.n[2];

    // Texture coords
    if (sGfxProc->tcOn) {
        s32 tu, tv;
        if (sGfxProc->geometryMode & G_TEXTURE_GEN) {
            s32 dotx = 128 + vec3f_dot(v->n, sGfxProc->lookAtXCoeffs);
            s32 doty = 128 + vec3f_dot(v->n, sGfxProc->lookAtYCoeffs);
            if (sGfxProc->geometryMode & G_TEXTURE_GEN_INVERT) {
                tu = (dotx * sGfxProc->tcS) >> 9;
                tv = (doty * sGfxProc->tcT) >> 9;
            } else {
                tu = (doty * sGfxProc->tcS) >> 9;
                tv = (dotx * sGfxProc->tcT) >> 9;
            }
        } else {
            tu = (v->vtx->v.tc[0] * sGfxProc->tcS) >> 16;
            tv = (v->vtx->v.tc[1] * sGfxProc->tcT) >> 16;
        }
        v->tc[0] = tex_u_normalize(tu);
        v->tc[1] = tex_v_normalize(tv);
    }

    // Colors
    if ((sGfxProc->geometryMode & G_LIGHTING) != 0
#if OMM_GAME_IS_R96X
        && !(Cheats.EnableCheats && Cheats.ChaosRainbow)
#endif
    ) {
        s32 rl = sGfxProc->light.col[0];
        s32 gl = sGfxProc->light.col[1];
        s32 bl = sGfxProc->light.col[2];
        s32 ra = sGfxProc->lightAmb.col[0];
        s32 ga = sGfxProc->lightAmb.col[1];
        s32 ba = sGfxProc->lightAmb.col[2];

        // Performing integer operations makes things faster
        s32 intensity = clamp_s(vec3f_dot(v->n, sGfxProc->lightCoeffs), 0, 128);
        s32 r = (ra * (128 - intensity) + ((rl * intensity * 5) >> 2)) >> 7;
        s32 g = (ga * (128 - intensity) + ((gl * intensity * 5) >> 2)) >> 7;
        s32 b = (ba * (128 - intensity) + ((bl * intensity * 5) >> 2)) >> 7;
        v->color[0] = clamp_s(r, 0, 256) * INV_256;
        v->color[1] = clamp_s(g, 0, 256) * INV_256;
        v->color[2] = clamp_s(b, 0, 256) * INV_256;
    } else {
        v->color[0] = v->vtx->v.cn[0] * INV_256;
        v->color[1] = v->vtx->v.cn[1] * INV_256;
        v->color[2] = v->vtx->v.cn[2] * INV_256;
    }

#if OMM_GAME_IS_R96X
    // Make the whole screen more red during Chaos: Bowser Time!, as if the entire world was burning
    if (Cheats.EnableCheats && Cheats.ChaosBowserTime) {
        v->color[1] /= 2;
        v->color[2] /= 2;
    }
#endif

    // Fog/Alpha
    if ((sGfxProc->geometryMode & G_FOG) && !GFX_NO_FOG) {
        f32 winv = 1.f / max_f(v->p[3], 0.001f);
        f32 fogz = clamp_f(v->p[2] * winv * sGfxProc->fogMul + sGfxProc->fogOff, 0, 256);
        v->color[3] = fogz * INV_256;
    } else {
        v->color[3] = v->vtx->v.cn[3] * INV_256;
    }
}

OMM_OPTIMIZE static void gfx_sp_vertex_load(GfxVertex *vBuffer, u32 vCount, bool shouldUpdateTC) {
    const Vtx *vtx = GFX_W1P;
#if OMM_GAME_IS_R96X
    if (sGfxProc->wdwSwapVtx) {
        extern const Vtx *wdw_swap_vtx(const Vtx *vtx, u32 count, s32 areaIndex);
        vtx = wdw_swap_vtx(vtx, vCount, sGfxProc->wdwSwapVtx);
    }
#endif
    GfxVertex *v = vBuffer;

    // Load current matrix into XMM registers
    __m128 m0 = _mm_load_ps(sGfxProc->mtxMP[0]);
    __m128 m1 = _mm_load_ps(sGfxProc->mtxMP[1]);
    __m128 m2 = _mm_load_ps(sGfxProc->mtxMP[2]);
    __m128 m3 = _mm_load_ps(sGfxProc->mtxMP[3]);

    // Transform vertices
    for (s32 i = vCount; i; --i, vtx++, v++) {
        v->vtx = vtx;
        v->shouldUpdateTC = true;

        // Load current vertex into XMM registers and transform
        __m128 v0 = _mm_set1_ps(vtx->v.ob[0]);
        __m128 v1 = _mm_set1_ps(vtx->v.ob[1]);
        __m128 v2 = _mm_set1_ps(vtx->v.ob[2]);
        __m128 d3 = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(m0, v0), _mm_mul_ps(m1, v1)),
            _mm_add_ps(_mm_mul_ps(m2, v2), m3)
        );
        _mm_storeu_ps(v->p, d3);
        v->p[0] = gfx_adjust_x_for_aspect_ratio(v->p[0]);

        // Trivial clip rejection
        v->rej = (
            ((v->p[0] < -v->p[3]) << 0) |
            ((v->p[0] > +v->p[3]) << 1) |
            ((v->p[1] < -v->p[3]) << 2) |
            ((v->p[1] > +v->p[3]) << 3) |
            ((v->p[2] < -v->p[3]) << 4) |
            ((v->p[2] > +v->p[3]) << 5)
        );

        // Tex coords and colors
        if (shouldUpdateTC) {
            gfx_sp_vertex_compute_tex_coords_and_colors(v);
            v->shouldUpdateTC = false;
        }
    }
}

static void gfx_sp_vertex() {
    u32 vCount = GFX_C0(12, 8);
    u32 vFirst = GFX_C0(1, 7) - vCount;
    gfx_sp_vertex_load(sGfxVtx + vFirst, vCount, OMM_GFX_API_DX);
}

static void gfx_sp_vertex_tc() {
    u32 vCount = GFX_C0(12, 8);
    u32 vFirst = GFX_C0(1, 7) - vCount;
    gfx_sp_vertex_load(sGfxVtx + vFirst, vCount, true);
}

OMM_OPTIMIZE static void gfx_sp_vertex_ext_update_buffer(u32 vCount) {
    static u32 sGfxVtxExtSize = 0;
    if (OMM_UNLIKELY(vCount > sGfxVtxExtSize)) {
        GfxVertex *newBuffer = mem_new(GfxVertex, vCount);
        if (sGfxVtxExtSize) {
            mem_cpy(newBuffer, sGfxVtxExt, sGfxVtxExtSize * sizeof(GfxVertex));
        }
        mem_del(sGfxVtxExt);
        sGfxVtxExt = newBuffer;
        sGfxVtxExtSize = vCount;
    }
}

static void gfx_sp_vertex_ext() {
    u32 vCount = GFX_C0(8, 16);
    u32 vLast = GFX_C0(0, 8);
    gfx_sp_vertex_ext_update_buffer(vCount);
    gfx_sp_vertex_load(sGfxVtxExt, vCount, OMM_GFX_API_DX);
    if (OMM_LIKELY(vLast)) {
        mem_cpy(sGfxVtx, sGfxVtxExt + vCount - vLast, sizeof(GfxVertex) * vLast);
    }
}

static void gfx_sp_vertex_ext_tc() {
    u32 vCount = GFX_C0(8, 16);
    u32 vLast = GFX_C0(0, 8);
    gfx_sp_vertex_ext_update_buffer(vCount);
    gfx_sp_vertex_load(sGfxVtxExt, vCount, true);
    if (OMM_LIKELY(vLast)) {
        mem_cpy(sGfxVtx, sGfxVtxExt + vCount - vLast, sizeof(GfxVertex) * vLast);
    }
}

//
// Triangle
//

OMM_OPTIMIZE static void gfx_sp_tri(GfxVertex *vBuffer, u32 v1i, u32 v2i, u32 v3i) {
    GfxVertex *v1 = vBuffer + v1i;
    GfxVertex *v2 = vBuffer + v2i;
    GfxVertex *v3 = vBuffer + v3i;
    if (v1->rej & v2->rej & v3->rej) {
        return;
    }

    // Culling test
    if (sGfxProc->geometryMode & G_CULL_BOTH) {
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

#if OMM_GAME_IS_R96X
        // Invert culling for each inverted axis during Chaos: Inverted Screen
        if (Cheats.EnableCheats && Cheats.ChaosMode) {
            for (s32 axis = 0; axis != 2; ++axis) {
                if (Cheats.ChaosInvertedScreen & (1 << axis)) {
                    cross *= -1;
                }
            }
        }
#endif

        // Front culling, remove triangles that are facing the camera
        // Back culling, remove triangles that point to the focus
        if (((sGfxProc->geometryMode & G_CULL_FRONT) && (sGfxProc->invertCulling ? -1 : +1) * cross <= 0) ||
            ((sGfxProc->geometryMode & G_CULL_BACK ) && (sGfxProc->invertCulling ? -1 : +1) * cross >= 0)) {
            return;
        }
    }

#if !OMM_GFX_API_DX

    // Update vertices tex coords and colors
    if (OMM_LIKELY(v1->shouldUpdateTC)) {
        gfx_sp_vertex_compute_tex_coords_and_colors(v1);
        v1->shouldUpdateTC = false;
    }
    if (OMM_LIKELY(v2->shouldUpdateTC)) {
        gfx_sp_vertex_compute_tex_coords_and_colors(v2);
        v1->shouldUpdateTC = false;
    }
    if (OMM_LIKELY(v3->shouldUpdateTC)) {
        gfx_sp_vertex_compute_tex_coords_and_colors(v3);
        v1->shouldUpdateTC = false;
    }

#endif

    // Update color combiner
    if (OMM_UNLIKELY(sGfxProc->shouldUpdateCC)) {
        gfx_update_color_combiner();
        sGfxProc->shouldUpdateCC = false;
    }

    // Textures
    bool *usedTextures = sGfxProc->shaderProgram->usedTextures;
    bool useTexture = usedTextures[0] || usedTextures[1];
    for (s32 tile = 0; tile != 2; ++tile) {
        if (usedTextures[tile]) {
            if (sGfxProc->texChanged[tile]) {
                gfx_flush();
                if (OMM_UNLIKELY(sGfxPrecache->precache)) {
                    gfx_texture_import_loading_screen(tile);
                } else {
                    gfx_texture_import(tile);
                }
                sGfxProc->texChanged[tile] = false;
            }
            bool lin = configFiltering && ((sGfxProc->otherModeH & (3U << G_MDSFT_TEXTFILT)) != G_TF_POINT);
            if (lin != sGfxProc->textures[tile]->lin || sGfxProc->texTileSize.cms != sGfxProc->textures[tile]->cms || sGfxProc->texTileSize.cmt != sGfxProc->textures[tile]->cmt) {
                gfx_flush();
                sGfxRapi->set_sampler_parameters(tile, lin, sGfxProc->texTileSize.cms, sGfxProc->texTileSize.cmt);
                sGfxProc->textures[tile]->lin = lin;
                sGfxProc->textures[tile]->cms = sGfxProc->texTileSize.cms;
                sGfxProc->textures[tile]->cmt = sGfxProc->texTileSize.cmt;
            }
        }
    }

    // Fill VBO
    GfxVertex *va[3] = { v1, v2, v3 };
    f32 *buffer = sGfxVbo->buffer + sGfxVbo->length;
    for (s32 i = 0; i != 3; ++i) {

        // x, y, z, w
        vec4f_copy(buffer, va[i]->p);
#if OMM_GFX_API_DX
        buffer[2] = (buffer[2] + buffer[3]) / 2.f;
#endif
        buffer += 4;

        // Texture coords
        if (useTexture) {
            vec2f_copy(buffer, va[i]->tc);
            buffer += 2;
        }

        // Fog
        if (sGfxProc->fog) {
            vec3f_copy(buffer, sGfxProc->fogColor);
            buffer += 3;
            *(buffer++) = va[i]->color[3];
        }

        // Colors
        for (s32 input = 0; input != sGfxProc->shaderProgram->numInputs; ++input) {
            for (s32 alpha = 0; alpha != 1 + sGfxProc->alpha; ++alpha) {
                f32 *color = gVec4fZero; Vec4f lod;
                switch (sGfxProc->colorCombiner->mapShaderInput[alpha][input]) {
                    case CC_PRIM:  color = sGfxProc->primColor; break;
                    case CC_SHADE: color = va[i]->color; break;
                    case CC_ENV:   color = sGfxProc->envColor; break;
                    case CC_LOD: { color = lod; lod[0] = lod[1] = lod[2] = lod[3] = clamp_0_1_f((va[i]->p[3] - 3000.f) / 3000.f); } break;
                }
                if (!alpha) {
                    *(buffer++) = color[0];
                    *(buffer++) = color[1];
                    *(buffer++) = color[2];
                } else if (sGfxProc->transpColor[3] < 1.f) {
                    *(buffer++) = sGfxProc->transpColor[3];
                } else if (sGfxProc->fog && color == va[i]->color) {
                    *(buffer++) = 1.f;
                } else {
                    *(buffer++) = color[3];
                }
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
    gfx_sp_tri(sGfxVtx, GFX_C0(16, 8) >> 1, GFX_C0(8, 8) >> 1, GFX_C0(0, 8) >> 1);
}

static void gfx_sp_tri2() {
    gfx_sp_tri(sGfxVtx, GFX_C0(16, 8) >> 1, GFX_C0(8, 8) >> 1, GFX_C0(0, 8) >> 1);
    gfx_sp_tri(sGfxVtx, GFX_C1(16, 8) >> 1, GFX_C1(8, 8) >> 1, GFX_C1(0, 8) >> 1);
}

OMM_OPTIMIZE static void gfx_sp_tri_ext() {
    u32 count = GFX_C0(8, 16);
    u32 *tris = GFX_W1P;
    for (u32 i = 0; i != count; ++i, tris += 3) {
        gfx_sp_tri(sGfxVtxExt, *(tris + 0), *(tris + 1), *(tris + 2));
    }
}

//
// RSP commands
//

OMM_INLINE void gfx_sp_set_other_mode(u32 shift, u32 siz, u64 mode) {
    u64 mask = ((1llu << siz) - 1llu) << shift;
    u64 omhl = ((u64) sGfxProc->otherModeL) | (((u64) sGfxProc->otherModeH) << 32llu);
    omhl = (omhl & ~mask) | mode;
    gfx_other_mode_l_changed((u32) omhl);
    gfx_other_mode_h_changed((u32) (omhl >> 32));
}

static void gfx_sp_set_other_mode_l() {
    u32 off = GFX_C0(8, 8);
    u32 siz = GFX_C0(0, 8);
    gfx_sp_set_other_mode(31 - off - siz, siz + 1, GFX_W1);
}

static void gfx_sp_set_other_mode_h() {
    u32 off = GFX_C0(8, 8);
    u32 siz = GFX_C0(0, 8);
    gfx_sp_set_other_mode(63 - off - siz, siz + 1, (u64) GFX_W1 << 32);
}

// Color palettes from sm64coopdx
static void gfx_sp_copymem() {
    const Lights1 *light = (const Lights1 *) omm_mario_colors_part_to_light((GFX_C0(16, 8) - 12) / 6);
    if (light) {
        if (GFX_C0(8, 8) == 6) {
            mem_cpy(&sGfxProc->light, &light->l->l, sizeof(Light_t));
        } else {
            mem_cpy(&sGfxProc->lightAmb, &light->a.l, sizeof(Light_t));
        }
        sGfxProc->lightChanged = true;
    }
}

static void gfx_sp_movemem() {
    u8 index = GFX_C0(0, 8);
    void *data = GFX_W1P;

    // Light
    if (index == G_MV_LIGHT) {
        if (GFX_C0(8, 8) == 6) {
            mem_cpy(&sGfxProc->light, data, sizeof(Light_t));
        } else {
            mem_cpy(&sGfxProc->lightAmb, data, sizeof(Light_t));
        }
#if LEVEL_PALETTES
        // Why isn't SA using white lights when everything else in the game does?...
        static const Lights1 sa_lights = gdSPDefLights1(0x00, 0x7f, 0x99, 0x00, 0xd4, 0xff, 0x28, 0x28, 0x28);
        static const Lights1 sa_lights_frozen = gdSPDefLights1(0x99, 0x99, 0x99, 0xff, 0xff, 0xff, 0x28, 0x28, 0x28);
        if (omm_world_is_frozen()) {
            if (mem_eq(data, &sa_lights.l, sizeof(Light_t))) mem_cpy(&sGfxProc->light, &sa_lights_frozen.l, sizeof(Light_t));
            if (mem_eq(data, &sa_lights.a, sizeof(Ambient_t))) mem_cpy(&sGfxProc->lightAmb, &sa_lights_frozen.a, sizeof(Light_t));
        }
#endif
        sGfxProc->lightChanged = true;
        return;
    }

    // Viewport
    if (index == G_MV_VIEWPORT) {
        const Vp_t *viewport = data;
        f32 w = viewport->vscale[0] / 2.f;
        f32 h = viewport->vscale[1] / 2.f;
        f32 x = (viewport->vtrans[0] / 4.f) - w / 2.f;
        f32 y = SCREEN_HEIGHT - ((viewport->vtrans[1] / 4.f) + h / 2.f);
        gfx_viewport_changed(x * GFX_RATIO_X, y * GFX_RATIO_Y, w * GFX_RATIO_X, h * GFX_RATIO_Y);
        return;
    }
}

static void gfx_sp_moveword() {
    if (GFX_C0(16, 8) == G_MW_FOG) {
        u32 data = GFX_W1;
        sGfxProc->fogMul = (s16) (data >> 16);
        sGfxProc->fogOff = (s16) data;
    }
}

static void gfx_sp_geometry_mode() {
    u32 clr = GFX_C0(0, 24);
    u32 set = GFX_W1;
    gfx_geometry_mode_changed(clr, set);
}

static void gfx_sp_texture() {
    u16 tcOn = GFX_C0(1, 1);
    u16 tcS = GFX_C1(16, 16);
    u16 tcT = GFX_C1(0, 16);
    sGfxProc->tcOn = tcOn;
    sGfxProc->tcS = tcS;
    sGfxProc->tcT = tcT;
}

static void gfx_sp_dl() {
    if (GFX_C0(16, 1)) {
        sGfxCmd = gfx_patch_display_list(GFX_W1P);
        sGfxCmd--;
    } else {
        Gfx *cmd = sGfxCmd;
        gfx_run_dl(gfx_patch_display_list(GFX_W1P));
        sGfxCmd = cmd;
    }
}

static void gfx_sp_end_dl() {
    sGfxCmd = NULL;
}

//
// RDP commands
//

OMM_INLINE u32 gfx_color_comb(u8 a, u8 b, u8 c, u8 d) {
    const u8 gfx_color_comb_comp[GFX_CCMUX_SIZE] = {
        [G_CCMUX_TEXEL0]       = CC_TEXEL0,
        [G_CCMUX_TEXEL1]       = CC_TEXEL1,
        [G_CCMUX_PRIMITIVE]    = CC_PRIM,
        [G_CCMUX_SHADE]        = CC_SHADE,
        [G_CCMUX_ENVIRONMENT]  = CC_ENV,
        [G_CCMUX_TEXEL0_ALPHA] = CC_TEXEL0A,
        [G_CCMUX_LOD_FRACTION] = CC_LOD,
    };
    return (gfx_color_comb_comp[a & GFX_CCMUX_BITS] << 0) |
           (gfx_color_comb_comp[b & GFX_CCMUX_BITS] << 3) |
           (gfx_color_comb_comp[c & GFX_CCMUX_BITS] << 6) |
           (gfx_color_comb_comp[d & GFX_CCMUX_BITS] << 9);
}

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
    sGfxProc->envColor[0] = r * INV_256;
    sGfxProc->envColor[1] = g * INV_256;
    sGfxProc->envColor[2] = b * INV_256;
    sGfxProc->envColor[3] = a * INV_256;
}

static void gfx_dp_set_prim_color() {
    u8 r = GFX_C1(24, 8);
    u8 g = GFX_C1(16, 8);
    u8 b = GFX_C1(8, 8);
    u8 a = GFX_C1(0, 8);
    sGfxProc->primColor[0] = r * INV_256;
    sGfxProc->primColor[1] = g * INV_256;
    sGfxProc->primColor[2] = b * INV_256;
    sGfxProc->primColor[3] = a * INV_256;
}

static void gfx_dp_set_fog_color() {
    u8 r = GFX_C1(24, 8);
    u8 g = GFX_C1(16, 8);
    u8 b = GFX_C1(8, 8);
    sGfxProc->fogColor[0] = r * INV_256;
    sGfxProc->fogColor[1] = g * INV_256;
    sGfxProc->fogColor[2] = b * INV_256;
}

static void gfx_dp_set_fill_color() {
    u32 packed_color = GFX_W1;
    u32 r = (packed_color >> 11) & 0x1F;
    u32 g = (packed_color >>  6) & 0x1F;
    u32 b = (packed_color >>  1) & 0x1F;
    u32 a = (packed_color >>  0) & 0x01;
    sGfxProc->fillColor[0] = r * INV_32;
    sGfxProc->fillColor[1] = g * INV_32;
    sGfxProc->fillColor[2] = b * INV_32;
    sGfxProc->fillColor[3] = a;
}

static void gfx_dp_set_z_image() {
#if OMM_GAME_IS_R96X
    extern const Gfx wdw_area_1_start_gfx[];
    extern const Gfx wdw_area_2_start_gfx[];
    extern const Gfx wdw_end_gfx[];
    if (GFX_W1P == wdw_area_1_start_gfx) { sGfxProc->wdwSwapVtx = 1; return; }
    if (GFX_W1P == wdw_area_2_start_gfx) { sGfxProc->wdwSwapVtx = 2; return; }
    if (GFX_W1P == wdw_end_gfx) { sGfxProc->wdwSwapVtx = 0; return; }
#endif
    sGfxProc->zBufAddress = GFX_W1P;
}

static void gfx_dp_set_color_image() {
    sGfxProc->colorImageAddress = GFX_W1P;
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
    if (OMM_LIKELY(addr)) sGfxProc->texToLoad = addr;
}

static void gfx_dp_set_tile() {
    u8 index = GFX_C1(24, 3);

    // Set tile number
    if (index == G_TX_LOADTILE) {
        sGfxProc->texTile = clamp_s(GFX_C0(0, 9) >> 8, 0, 1);
        return;
    }
    if (index == G_TX_LOADTILE_TILE_1) {
        sGfxProc->texTile = 1;
        return;
    }

    // Set tile size
    if (index == G_TX_RENDERTILE) {
        u8 cms = GFX_C1(8, 2);
        u8 cmt = GFX_C1(18, 2);
        sGfxProc->texTileSize.cms = cms;
        sGfxProc->texTileSize.cmt = cmt;
        sGfxProc->texChanged[0] = true;
        sGfxProc->texChanged[1] = true;
        return;
    }

    // Bowser/Peach painting
    if (index == G_TX_BOWSER_PEACH_PAINTING) {
        return;
    }

    // Something unknown that should be ignored
    if (index == G_TX_UNKNOWN_5) {
        return;
    }

    // Not supported
    sys_fatal("gfx_dp_set_tile: This feature is not supported. Index: %u", index);
}

static void gfx_dp_set_tile_size() {
    u16 uls = GFX_C0(12, 12);
    u16 ult = GFX_C0(0, 12);
    u16 lrs = GFX_C1(12, 12);
    u16 lrt = GFX_C1(0, 12);
    sGfxProc->texChanged[0] = true;
    sGfxProc->texChanged[1] = true;
    gfx_tex_tile_size_changed(uls, ult, lrs, lrt);
}

static void gfx_dp_load_block() {
    sGfxProc->texLoaded[sGfxProc->texTile] = sGfxProc->texToLoad;
    sGfxProc->texChanged[sGfxProc->texTile] = true;
}

static void gfx_dp_load_tile() {
    u32 uls = GFX_C0(12, 12);
    u32 ult = GFX_C0(0, 12);
    u32 lrs = GFX_C1(12, 12);
    u32 lrt = GFX_C1(0, 12);
    sGfxProc->texLoaded[sGfxProc->texTile] = sGfxProc->texToLoad;
    sGfxProc->texChanged[sGfxProc->texTile] = true;
    gfx_tex_tile_size_changed(uls, ult, lrs, lrt);
}

static void gfx_dp_draw_rectangle(s32 ulx, s32 uly, s32 lrx, s32 lry) {
    Vec2f ulf = {
        +(ulx / (2.f * SCREEN_WIDTH )) - 1.f,
        -(uly / (2.f * SCREEN_HEIGHT)) + 1.f
    };
    Vec2f lrf = {
        +(lrx / (2.f * SCREEN_WIDTH )) - 1.f,
        -(lry / (2.f * SCREEN_HEIGHT)) + 1.f
    };
#if OMM_GAME_IS_R96X
    if (Cheats.EnableCheats && Cheats.ChaosMode) {
        for (s32 axis = 0; axis != 2; ++axis) {
            if (Cheats.ChaosInvertedScreen & (1 << axis)) {
                ulf[axis] *= -1;
                lrf[axis] *= -1;
            }
        }
    }
#endif
    ulf[0] = gfx_adjust_x_for_aspect_ratio(ulf[0]);
    lrf[0] = gfx_adjust_x_for_aspect_ratio(lrf[0]);

    GfxVertex* ul = sGfxVtx + (GFX_MAX_VERTS + 0);
    GfxVertex* ll = sGfxVtx + (GFX_MAX_VERTS + 1);
    GfxVertex* lr = sGfxVtx + (GFX_MAX_VERTS + 2);
    GfxVertex* ur = sGfxVtx + (GFX_MAX_VERTS + 3);
    ul->p[0] = ulf[0]; ul->p[1] = ulf[1]; ul->p[2] = -1.f; ul->p[3] = 1.f; ul->shouldUpdateTC = false;
    ll->p[0] = ulf[0]; ll->p[1] = lrf[1]; ll->p[2] = -1.f; ll->p[3] = 1.f; ll->shouldUpdateTC = false;
    lr->p[0] = lrf[0]; lr->p[1] = lrf[1]; lr->p[2] = -1.f; lr->p[3] = 1.f; lr->shouldUpdateTC = false;
    ur->p[0] = lrf[0]; ur->p[1] = ulf[1]; ur->p[2] = -1.f; ur->p[3] = 1.f; ur->shouldUpdateTC = false;

    // The coordinates for texture rectangle shall bypass the viewport setting
    GfxViewport lastViewport = sGfxProc->viewport;
    u32 lastGeometryMode = sGfxProc->geometryMode;
    u32 lastOtherModeH = sGfxProc->otherModeH;

    // Draw rectangle
    gfx_geometry_mode_changed(0, 0);
    gfx_other_mode_h_changed((sGfxProc->otherModeH & ~(3U << G_MDSFT_TEXTFILT)) | G_TF_POINT);
    gfx_viewport_changed(0, 0, gfx_current_dimensions.width, gfx_current_dimensions.height);
    gfx_sp_tri(sGfxVtx, GFX_MAX_VERTS + 0, GFX_MAX_VERTS + 1, GFX_MAX_VERTS + 3);
    gfx_sp_tri(sGfxVtx, GFX_MAX_VERTS + 1, GFX_MAX_VERTS + 2, GFX_MAX_VERTS + 3);

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
    u32 lastCombineMode = sGfxProc->combineMode;
    if ((sGfxProc->otherModeH & (3U << G_MDSFT_CYCLETYPE)) == G_CYC_COPY) {
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

    GfxVertex* ul = sGfxVtx + (GFX_MAX_VERTS + 0);
    GfxVertex* ll = sGfxVtx + (GFX_MAX_VERTS + 1);
    GfxVertex* lr = sGfxVtx + (GFX_MAX_VERTS + 2);
    GfxVertex* ur = sGfxVtx + (GFX_MAX_VERTS + 3);
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
    if (sGfxProc->colorImageAddress == sGfxProc->zBufAddress) {
        return;
    }

    // ¯\_(ツ)_/¯
    u32 mode = (sGfxProc->otherModeH & (3U << G_MDSFT_CYCLETYPE));
    if (mode == G_CYC_COPY || mode == G_CYC_FILL) {
        lrx += 1 << 2;
        lry += 1 << 2;
    }

    vec4f_copy(sGfxVtx[GFX_MAX_VERTS + 0].color, sGfxProc->fillColor);
    vec4f_copy(sGfxVtx[GFX_MAX_VERTS + 1].color, sGfxProc->fillColor);
    vec4f_copy(sGfxVtx[GFX_MAX_VERTS + 2].color, sGfxProc->fillColor);
    vec4f_copy(sGfxVtx[GFX_MAX_VERTS + 3].color, sGfxProc->fillColor);

    u32 lastCombineMode = sGfxProc->combineMode;
    gfx_combine_mode_changed(gfx_color_comb(0, 0, 0, G_CCMUX_SHADE) | (gfx_color_comb(0, 0, 0, G_ACMUX_SHADE) << 12));
    gfx_dp_draw_rectangle(ulx, uly, lrx, lry);
    gfx_combine_mode_changed(lastCombineMode);
}

//
// Extra commands
//

static void gfx_xp_swap_cmd() {
    static GfxCmdFunc sGfxCmdSwap[GFX_CMD_SIZE] = {0};
    u8 cmd = GFX_C0(0, 8) & GFX_CMD_BITS;
    void *func = GFX_W1P;
    if (func) {
        sGfxCmdSwap[cmd] = sGfxCmdTable[cmd];
        sGfxCmdTable[cmd] = (GfxCmdFunc) func;
    } else {
        sGfxCmdTable[cmd] = sGfxCmdSwap[cmd];
    }
}

static void gfx_xp_invert_culling() {
    sGfxProc->invertCulling = (GFX_W1 != 0);
}

static void gfx_xp_push_texture() {
    sGfxState->tcOn = sGfxProc->tcOn;
    sGfxState->tcS = sGfxProc->tcS;
    sGfxState->tcT = sGfxProc->tcT;
    sGfxState->texToLoad = sGfxProc->texToLoad;
    sGfxState->texLoaded[0] = sGfxProc->texLoaded[0];
    sGfxState->texLoaded[1] = sGfxProc->texLoaded[1];
    sGfxState->textures[0] = sGfxProc->textures[0];
    sGfxState->textures[1] = sGfxProc->textures[1];
    sGfxState->texTileSize = sGfxProc->texTileSize;
}

static void gfx_xp_pop_texture() {
    sGfxProc->tcOn = sGfxState->tcOn;
    sGfxProc->tcS = sGfxState->tcS;
    sGfxProc->tcT = sGfxState->tcT;
    sGfxProc->texToLoad = sGfxState->texToLoad;
    sGfxProc->texLoaded[0] = sGfxState->texLoaded[0];
    sGfxProc->texLoaded[1] = sGfxState->texLoaded[1];
    sGfxProc->textures[0] = sGfxState->textures[0];
    sGfxProc->textures[1] = sGfxState->textures[1];
    sGfxProc->texTileSize = sGfxState->texTileSize;
    sGfxProc->texChanged[0] = true;
    sGfxProc->texChanged[1] = true;
    gfx_tex_tile_size_changed(sGfxProc->texTileSize.uls, sGfxProc->texTileSize.ult, sGfxProc->texTileSize.lrs, sGfxProc->texTileSize.lrt);
}

//
// Gfx run
//

static GfxCmdFunc sGfxCmdTable[GFX_CMD_SIZE] = {
[GFX_CMD_BITS & G_VTX           ] = gfx_sp_vertex,
[GFX_CMD_BITS & G_VTXTC         ] = gfx_sp_vertex_tc,
[GFX_CMD_BITS & G_VTXEXT        ] = gfx_sp_vertex_ext,
[GFX_CMD_BITS & G_VTXEXTTC      ] = gfx_sp_vertex_ext_tc,
[GFX_CMD_BITS & G_TRI1          ] = gfx_sp_tri1,
[GFX_CMD_BITS & G_TRI2          ] = gfx_sp_tri2,
[GFX_CMD_BITS & G_TRIEXT        ] = gfx_sp_tri_ext,
[GFX_CMD_BITS & G_INVERTCULL    ] = gfx_xp_invert_culling,
[GFX_CMD_BITS & G_PUSHTEX       ] = gfx_xp_push_texture,
[GFX_CMD_BITS & G_POPTEX        ] = gfx_xp_pop_texture,
[GFX_CMD_BITS & G_COPYMEM       ] = gfx_sp_copymem,
[GFX_CMD_BITS & G_TEXTURE       ] = gfx_sp_texture,
[GFX_CMD_BITS & G_POPMTX        ] = gfx_sp_pop_matrix,
[GFX_CMD_BITS & G_GEOMETRYMODE  ] = gfx_sp_geometry_mode,
[GFX_CMD_BITS & G_MTX           ] = gfx_sp_matrix,
[GFX_CMD_BITS & G_MOVEWORD      ] = gfx_sp_moveword,
[GFX_CMD_BITS & G_MOVEMEM       ] = gfx_sp_movemem,
[GFX_CMD_BITS & G_DL            ] = gfx_sp_dl,
[GFX_CMD_BITS & G_ENDDL         ] = gfx_sp_end_dl,
[GFX_CMD_BITS & G_SETOTHERMODE_L] = gfx_sp_set_other_mode_l,
[GFX_CMD_BITS & G_SETOTHERMODE_H] = gfx_sp_set_other_mode_h,
[GFX_CMD_BITS & G_TEXRECT       ] = gfx_dp_texture_rectangle,
[GFX_CMD_BITS & G_TEXRECTFLIP   ] = gfx_dp_texture_rectangle,
[GFX_CMD_BITS & G_SETSCISSOR    ] = gfx_dp_set_scissor,
[GFX_CMD_BITS & G_SWAPCMD       ] = gfx_xp_swap_cmd,
[GFX_CMD_BITS & G_SETTILESIZE   ] = gfx_dp_set_tile_size,
[GFX_CMD_BITS & G_LOADBLOCK     ] = gfx_dp_load_block,
[GFX_CMD_BITS & G_LOADTILE      ] = gfx_dp_load_tile,
[GFX_CMD_BITS & G_SETTILE       ] = gfx_dp_set_tile,
[GFX_CMD_BITS & G_FILLRECT      ] = gfx_dp_fill_rectangle,
[GFX_CMD_BITS & G_SETFILLCOLOR  ] = gfx_dp_set_fill_color,
[GFX_CMD_BITS & G_SETFOGCOLOR   ] = gfx_dp_set_fog_color,
[GFX_CMD_BITS & G_SETPRIMCOLOR  ] = gfx_dp_set_prim_color,
[GFX_CMD_BITS & G_SETENVCOLOR   ] = gfx_dp_set_env_color,
[GFX_CMD_BITS & G_SETCOMBINE    ] = gfx_dp_set_combine_mode,
[GFX_CMD_BITS & G_SETTIMG       ] = gfx_dp_set_texture_image,
[GFX_CMD_BITS & G_SETZIMG       ] = gfx_dp_set_z_image,
[GFX_CMD_BITS & G_SETCIMG       ] = gfx_dp_set_color_image,
};

OMM_OPTIMIZE static void gfx_run_dl(Gfx *cmd) {
    for (sGfxCmd = cmd;; sGfxCmd++) {
        u8 op = GFX_OP & GFX_CMD_BITS;
#if OMM_CODE_DEBUG
        omm_gfx_profiler_update_op(op);
#endif
        GfxCmdFunc func = sGfxCmdTable[op];
        if (OMM_LIKELY(func)) {
            func();
        }
        if (OMM_UNLIKELY(!sGfxCmd)) {
#if OMM_CODE_DEBUG
            omm_gfx_profiler_update_op(0);
#endif
            return;
        }
    }
}

//
// Frame interpolation
//

static u32 gfx_get_refresh_rate() {
    static u32 sWindowRefreshRate = 0;
    if (!sWindowRefreshRate) {
#if OMM_GFX_API_DX
        sWindowRefreshRate = UNLIMITED_FPS; // Let the VSync give the pace
#elif OMM_GFX_API_GL
        SDL_DisplayMode mode;
        if (SDL_GetCurrentDisplayMode(0, &mode) == 0) {
            sWindowRefreshRate = (u32) mode.refresh_rate;
        }
#else
        sWindowRefreshRate = 60;
#endif
    }
    return sWindowRefreshRate;
}

static u32 gfx_get_target_frame_rate() {
    switch (gOmmFrameRate) {
        case OMM_FPS_30:   return GAME_UPDATES_PER_SECOND;
        case OMM_FPS_60:   return 60;
        case OMM_FPS_AUTO: return gfx_get_refresh_rate();
        case OMM_FPS_INF:  return UNLIMITED_FPS;
    }
    return GAME_UPDATES_PER_SECOND;
}

bool gFrameInterpolation = false;
static void gfx_init_frame_interpolation() {
    if (gOmmFrameRate == OMM_FPS_30 || omm_is_main_menu() || ((gMenuMode == 0 || gMenuMode == 1) && !gOmmGlobals->cameraSnapshotMode)) {
        sFps->target = GAME_UPDATES_PER_SECOND;
        gFrameInterpolation = false;
    } else {
        sFps->target = gfx_get_target_frame_rate();
        gFrameInterpolation = true;
    }
}

static u32 gfx_get_num_frames_to_draw() {
    if (sFps->target % gGameUpdatesPerSecond == 0) {
        return sFps->target / gGameUpdatesPerSecond;
    }
    u64 framesCurr = (u64) (sFps->tFrame * (f64) sFps->target);
    u64 framesNext = (u64) ((sFps->tFrame + GAME_UPDATE_DURATION) * (f64) sFps->target);
    return (u32) MAX(1, framesNext - framesCurr);
}

static void gfx_update_fps(f64 tCurr) {
    f32 fps = (f32) ((f64) sFps->frames / MAX(0.001, tCurr - sFps->tLast));
    omm_profiler_update_fps(fps, gGameUpdatesPerSecond);
    sFps->tLast = tCurr;
    sFps->frames = 0;
}

extern void gfx_interpolate_frame_mtx(f32 t);
extern void gfx_interpolate_frame_bhv_omm_wing_trail(f32 t);
extern void gfx_interpolate_frame_transition(f32 t);
extern void gfx_interpolate_frame_dialog(f32 t);
extern void gfx_interpolate_frame_hud(f32 t);
extern void gfx_interpolate_frame_effects(f32 t);
extern void gfx_interpolate_frame_paintings(f32 t);
static void gfx_patch_interpolated_frame(f32 t) {
    gfx_interpolate_frame_mtx(t);
    gfx_interpolate_frame_bhv_omm_wing_trail(t);
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
    bool isUnlimited = sFps->target == UNLIMITED_FPS;
    bool is30Fps = sFps->target == GAME_UPDATES_PER_SECOND;

    f64 tCurr = gfx_get_current_time();
    f64 tTarget = sFps->tFrame + GAME_UPDATE_DURATION;
    f64 tStart = tCurr;
    f64 tExpected = 0;

    u32 framesToDraw = gfx_get_num_frames_to_draw();

    // Interpolate and render frames
    // Make sure to draw at least one frame to prevent the game from freezing completely
    // (including inputs and window events) if the game update duration is greater than 33ms
    do {

        // Handle events
        sGfxWapi->handle_events();
        sGfxWapi->get_dimensions(&gfx_current_dimensions.width, &gfx_current_dimensions.height);
        gfx_current_dimensions.aspect_ratio = (f32) gfx_current_dimensions.width / (f32) gfx_current_dimensions.height;
        sGfxAdjustForAspectRatio = (4.f / 3.f) / gfx_current_dimensions.aspect_ratio;

        // Start frame
        if (sGfxWapi->start_frame()) {
            omm_profiler_start(OMM_PRF_FRM);
            sGfxProc->mtx = sGfxMtx + 8;
            sGfxProc->lightChanged = true;
            sGfxProc->lookAtChanged = true;

            // Patch interpolations
            sFps->lerp = (is30Fps ? 1.f : clamp_0_1_f((tCurr - sFps->tFrame) / GAME_UPDATE_DURATION));
            gfx_patch_interpolated_frame(sFps->lerp);

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

            // Swap buffers
            omm_profiler_stop(OMM_PRF_FRM);
            sGfxRapi->finish_render();
            sGfxWapi->swap_buffers_end();
            sFps->frames++;

            // Wait for the next frame
            if (!isUnlimited) {
                f64 tNow = gfx_get_current_time();
                f64 tElapsed = tNow - tStart;
                tExpected += (tTarget - tCurr) / (f64) framesToDraw;
                f64 tSleep = (tExpected - tElapsed);
                if (tSleep > 0.0) {
                    usleep((useconds_t) (tSleep * 1000000.0));
                }
                framesToDraw--;
            }
        }
    } while ((tCurr = gfx_get_current_time()) < tTarget && framesToDraw > 0);

    // Update FPS every second
    if ((tCurr = gfx_get_current_time()) >= sFps->tLast + 1.0) {
        gfx_update_fps(tCurr);
    }

    // Advance frame time
    sFps->tFrame = round(tCurr * (f64) gGameUpdatesPerSecond) / (f64) gGameUpdatesPerSecond;

    // Clear interpolated stuff
    gfx_clear_frame_mtx();
    gfx_clear_frame_transition();
    gfx_clear_frame_dialog();
    gfx_clear_frame_hud();
    gfx_clear_frame_effects();
    gfx_clear_frame_paintings();
#if LEVEL_PALETTES
    sGfxProc->palColor[3] = 0;
#endif
}

static void gfx_set_config() {
#if OMM_GAME_IS_R96X
    configBillboard = false;
    config60FPS = false;
    configInternalResolutionBool = false;
    configCustomInternalResolution = 1;
    configForce4by3 = false;
    configDrawDistance = 509;
#endif
}

#if OMM_GFX_API_DX
static void gfx_dxgi_handle_events() {
    extern f64 FRAME_INTERVAL_US_DENOMINATOR;
    extern u64 *frame_timestamp;
    static u32 sCurrTargetFps = 0;
    if (sCurrTargetFps != sFps->target) {
        FRAME_INTERVAL_US_DENOMINATOR = (3.0 * sFps->target) / (f64) GAME_UPDATES_PER_SECOND;
        *frame_timestamp = 0;
        sCurrTargetFps = sFps->target;
    }
}
#endif

//
// Public
//

struct GfxRenderingAPI *gfx_get_current_rendering_api() {
    return sGfxRapi;
}

void gfx_init(struct GfxWindowManagerAPI *wapi, struct GfxRenderingAPI *rapi, const char *_) {
    const char window_title[] = OMM_GAME_NAME " ("
#if defined(RAPI_D3D11)
    "DirectX 11"
#elif defined(RAPI_D3D12)
    "DirectX 12"
#elif defined(RAPI_GL)
    "OpenGL 2.1"
#elif defined(RAPI_GL_LEGACY)
    "OpenGL 1.1"
#endif
    ")"
    " - Odyssey Mario's Moveset v" OMM_VERSION;
    sGfxWapi = wapi;
    sGfxRapi = rapi;
    sGfxWapi->init(window_title);
    sGfxRapi->init();
#if OMM_GFX_API_DX
    sGfxWapi->handle_events = gfx_dxgi_handle_events;
#endif
    vec4f_copy(sGfxProc->envColor, gVec4fOne);
    vec4f_copy(sGfxProc->primColor, gVec4fOne);
    vec4f_copy(sGfxProc->fogColor, gVec4fOne);
    vec4f_copy(sGfxProc->fillColor, gVec4fOne);
    vec4f_copy(sGfxProc->transpColor, gVec4fOne);
    for_each_in_(u32, precompShader, {
        0x01200200, 0x00000045, 0x00000200, 0x01200a00,
        0x00000a00, 0x01a00045, 0x00000551, 0x01045045,
        0x05a00a00, 0x01200045, 0x05045045, 0x01045a00,
        0x01a00a00, 0x0000038d, 0x01081081, 0x0120038d,
        0x03200045, 0x03200a00, 0x01a00a6f, 0x01141045,
        0x07a00a00, 0x05200200, 0x03200200, 0x09200200,
        0x0920038d, 0x09200045
    }) {
        gfx_lookup_or_create_shader_program(*precompShader);
    }
    gfx_texture_precache_init();
}

void gfx_precache_textures() {
}

void gfx_start_frame() {
    gfx_init_frame_interpolation();
    gfx_set_config();
}

void gfx_run(UNUSED Gfx *commands) {
}

void gfx_end_frame() {
    gfx_texture_precache_upload();
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

#include "data/omm/mario/omm_shadow_mario.inl"
#include "data/omm/cappy/omm_cappy_gfx.inl"
#include "data/omm/peachy/omm_peach_tiara_gfx.inl"
#include "data/omm/level/omm_level_peachy.c"
#include "data/omm/object/omm_obj_effect_metal.inl"
#include "data/omm/object/omm_obj_yoshi_mode_gfx.inl"

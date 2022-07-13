#ifndef _GBI_H_
#define _GBI_H_

#include <PR/ultratypes.h>
#define GBI_FLOATS

// Micro-code: F3DEX2 Extended
#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif
#ifndef F3DEX_GBI_2
#define F3DEX_GBI_2
#endif
#ifndef F3DEX_GBI_2E
#define F3DEX_GBI_2E
#endif
#undef F3D_NEW
#undef F3D_OLD

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

// RSP commands
#define G_NOOP                              0x00
#define G_RDPHALF_2                         0xf1
#define G_SETOTHERMODE_H                    0xe3
#define G_SETOTHERMODE_L                    0xe2
#define G_RDPHALF_1                         0xe1
#define G_SPNOOP                            0xe0
#define G_ENDDL                             0xdf
#define G_DL                                0xde
#define G_LOAD_UCODE                        0xdd
#define G_MOVEMEM                           0xdc
#define G_MOVEWORD                          0xdb
#define G_MTX                               0xda
#define G_GEOMETRYMODE                      0xd9
#define G_POPMTX                            0xd8
#define G_TEXTURE                           0xd7
#define G_DMA_IO                            0xd6
#define G_SPECIAL_1                         0xd5
#define G_SPECIAL_2                         0xd4
#define G_SPECIAL_3                         0xd3
#define G_VTX                               0x01
#define G_MODIFYVTX                         0x02
#define G_CULLDL                            0x03
#define G_BRANCH_Z                          0x04
#define G_TRI1                              0x05
#define G_TRI2                              0x06
#define G_QUAD                              0x07
#define G_LINE3D                            0x08

// RDP commands
#define G_SETCIMG                           0xff
#define G_SETZIMG                           0xfe
#define G_SETTIMG                           0xfd
#define G_SETCOMBINE                        0xfc
#define G_SETENVCOLOR                       0xfb
#define G_SETPRIMCOLOR                      0xfa
#define G_SETBLENDCOLOR                     0xf9
#define G_SETFOGCOLOR                       0xf8
#define G_SETFILLCOLOR                      0xf7
#define G_FILLRECT                          0xf6
#define G_SETTILE                           0xf5
#define G_LOADTILE                          0xf4
#define G_LOADBLOCK                         0xf3
#define G_SETTILESIZE                       0xf2
#define G_LOADTLUT                          0xf0
#define G_RDPSETOTHERMODE                   0xef
#define G_SETPRIMDEPTH                      0xee
#define G_SETSCISSOR                        0xed
#define G_SETCONVERT                        0xec
#define G_SETKEYR                           0xeb
#define G_SETKEYGB                          0xea
#define G_RDPFULLSYNC                       0xe9
#define G_RDPTILESYNC                       0xe8
#define G_RDPPIPESYNC                       0xe7
#define G_RDPLOADSYNC                       0xe6
#define G_TEXRECTFLIP                       0xe5
#define G_TEXRECT                           0xe4
#define G_TRI_SHADE_TXTR_ZBUFF              0xcf
#define G_TRI_SHADE_TXTR                    0xce
#define G_TRI_SHADE_ZBUFF                   0xcd
#define G_TRI_SHADE                         0xcc
#define G_TRI_TXTR_ZBUFF                    0xcb
#define G_TRI_TXTR                          0xca
#define G_TRI_FILL_ZBUFF                    0xc9
#define G_TRI_FILL                          0xc8

#define G_TEXTURE_IMAGE_FRAC                2
#define G_MAXFBZ                            0x3fff
#define GPACK_RGBA5551(r, g, b, a)          ((((r) << 8) & 0xf800) | (((g) << 3) & 0x7c0) | (((b) >> 2) & 0x3e) | ((a) & 0x1))
#define GPACK_ZDZ(z, dz)                    ((z) << 2 | (dz))

// G_MTX: parameter flags
#define G_MTX_MODELVIEW                     0x00 
#define G_MTX_PROJECTION                    0x04
#define G_MTX_MUL                           0x00
#define G_MTX_LOAD                          0x02
#define G_MTX_NOPUSH                        0x00
#define G_MTX_PUSH                          0x01

// Flags for G_SETGEOMETRYMODE
#define G_ZBUFFER                           0x00000001
#define G_SHADE                             0x00000004
#define G_TEXTURE_ENABLE                    0x00000000
#define G_SHADING_SMOOTH                    0x00200000
#define G_CULL_FRONT                        0x00000200
#define G_CULL_BACK                         0x00000400
#define G_CULL_BOTH                         0x00000600
#define G_FOG                               0x00010000
#define G_LIGHTING                          0x00020000
#define G_TEXTURE_GEN                       0x00040000
#define G_TEXTURE_GEN_LINEAR                0x00080000
#define G_LOD                               0x00100000
#define G_TEXTURE_ALPHA                     0x00400000
#define G_CLIPPING                          0x00800000

// G_SETIMG fmt: set image formats
#define G_IM_FMT_RGBA                       0
#define G_IM_FMT_YUV                        1
#define G_IM_FMT_CI                         2
#define G_IM_FMT_IA                         3
#define G_IM_FMT_I                          4

// G_SETIMG siz: set image pixel size
#define G_IM_SIZ_4b                         0
#define G_IM_SIZ_4b_BYTES                   0
#define G_IM_SIZ_4b_SHIFT                   2
#define G_IM_SIZ_4b_INCR                    3
#define G_IM_SIZ_4b_TILE_BYTES              G_IM_SIZ_4b_BYTES
#define G_IM_SIZ_4b_LINE_BYTES              G_IM_SIZ_4b_BYTES
#define G_IM_SIZ_4b_LOAD_BLOCK              G_IM_SIZ_16b
#define G_IM_SIZ_8b                         1
#define G_IM_SIZ_8b_BYTES                   1
#define G_IM_SIZ_8b_SHIFT                   1
#define G_IM_SIZ_8b_INCR                    1
#define G_IM_SIZ_8b_TILE_BYTES              G_IM_SIZ_8b_BYTES
#define G_IM_SIZ_8b_LINE_BYTES              G_IM_SIZ_8b_BYTES
#define G_IM_SIZ_8b_LOAD_BLOCK              G_IM_SIZ_16b
#define G_IM_SIZ_16b                        2
#define G_IM_SIZ_16b_BYTES                  2
#define G_IM_SIZ_16b_SHIFT                  0
#define G_IM_SIZ_16b_INCR                   0
#define G_IM_SIZ_16b_TILE_BYTES             G_IM_SIZ_16b_BYTES
#define G_IM_SIZ_16b_LINE_BYTES             G_IM_SIZ_16b_BYTES
#define G_IM_SIZ_16b_LOAD_BLOCK             G_IM_SIZ_16b
#define G_IM_SIZ_32b                        3
#define G_IM_SIZ_32b_BYTES                  4
#define G_IM_SIZ_32b_SHIFT                  0
#define G_IM_SIZ_32b_INCR                   0
#define G_IM_SIZ_32b_TILE_BYTES             2
#define G_IM_SIZ_32b_LINE_BYTES             2
#define G_IM_SIZ_32b_LOAD_BLOCK             G_IM_SIZ_32b
#define G_IM_SIZ_DD                         4

// G_SETCOMBINE: color combine modes
#define G_CCMUX_COMBINED                    0
#define G_CCMUX_TEXEL0                      1
#define G_CCMUX_TEXEL1                      2
#define G_CCMUX_PRIMITIVE                   3
#define G_CCMUX_SHADE                       4
#define G_CCMUX_ENVIRONMENT                 5
#define G_CCMUX_CENTER                      6
#define G_CCMUX_SCALE                       6
#define G_CCMUX_COMBINED_ALPHA              7
#define G_CCMUX_TEXEL0_ALPHA                8
#define G_CCMUX_TEXEL1_ALPHA                9
#define G_CCMUX_PRIMITIVE_ALPHA             10
#define G_CCMUX_SHADE_ALPHA                 11
#define G_CCMUX_ENV_ALPHA                   12
#define G_CCMUX_LOD_FRACTION                13
#define G_CCMUX_PRIM_LOD_FRAC               14
#define G_CCMUX_NOISE                       7
#define G_CCMUX_K4                          7
#define G_CCMUX_K5                          15
#define G_CCMUX_1                           6
#define G_CCMUX_0                           31

// Alpha combiner constants
#define G_ACMUX_COMBINED                    0
#define G_ACMUX_TEXEL0                      1
#define G_ACMUX_TEXEL1                      2
#define G_ACMUX_PRIMITIVE                   3
#define G_ACMUX_SHADE                       4
#define G_ACMUX_ENVIRONMENT                 5
#define G_ACMUX_LOD_FRACTION                0
#define G_ACMUX_PRIM_LOD_FRAC               6
#define G_ACMUX_1                           6
#define G_ACMUX_0                           7

// Common CC cycle 1 modes
#define G_CC_PRIMITIVE                      0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE
#define G_CC_SHADE                          0, 0, 0, SHADE, 0, 0, 0, SHADE
#define G_CC_MODULATEI                      TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE
#define G_CC_MODULATEIDECALA                TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0
#define G_CC_MODULATEIFADE                  TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT
#define G_CC_MODULATERGB                    G_CC_MODULATEI
#define G_CC_MODULATERGBDECALA              G_CC_MODULATEIDECALA
#define G_CC_MODULATERGBFADE                G_CC_MODULATEIFADE
#define G_CC_MODULATEIA                     TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0
#define G_CC_MODULATEIFADEA                 TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0
#define G_CC_MODULATEFADE                   TEXEL0, 0, SHADE, 0, ENVIRONMENT, 0, TEXEL0, 0
#define G_CC_MODULATERGBA                   G_CC_MODULATEIA
#define G_CC_MODULATERGBFADEA               G_CC_MODULATEIFADEA
#define G_CC_MODULATEI_PRIM                 TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE
#define G_CC_MODULATEIA_PRIM                TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0
#define G_CC_MODULATEIDECALA_PRIM           TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, TEXEL0
#define G_CC_MODULATERGB_PRIM               G_CC_MODULATEI_PRIM
#define G_CC_MODULATERGBA_PRIM              G_CC_MODULATEIA_PRIM
#define G_CC_MODULATERGBDECALA_PRIM         G_CC_MODULATEIDECALA_PRIM
#define G_CC_FADE                           SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0
#define G_CC_FADEA                          TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0
#define G_CC_DECALRGB                       0, 0, 0, TEXEL0, 0, 0, 0, SHADE
#define G_CC_DECALRGBA                      0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0
#define G_CC_DECALFADE                      0, 0, 0, TEXEL0, 0, 0, 0, ENVIRONMENT
#define G_CC_DECALFADEA                     0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0
#define G_CC_BLENDI                         ENVIRONMENT, SHADE, TEXEL0, SHADE, 0, 0, 0, SHADE
#define G_CC_BLENDIA                        ENVIRONMENT, SHADE, TEXEL0, SHADE, TEXEL0, 0, SHADE, 0
#define G_CC_BLENDIDECALA                   ENVIRONMENT, SHADE, TEXEL0, SHADE, 0, 0, 0, TEXEL0
#define G_CC_BLENDRGBA                      TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, SHADE
#define G_CC_BLENDRGBDECALA                 TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, TEXEL0
#define G_CC_BLENDRGBFADEA                  TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, ENVIRONMENT
#define G_CC_ADDRGB                         TEXEL0, 0, TEXEL0, SHADE, 0, 0, 0, SHADE
#define G_CC_ADDRGBDECALA                   TEXEL0, 0, TEXEL0, SHADE, 0, 0, 0, TEXEL0
#define G_CC_ADDRGBFADE                     TEXEL0, 0, TEXEL0, SHADE, 0, 0, 0, ENVIRONMENT
#define G_CC_REFLECTRGB                     ENVIRONMENT, 0, TEXEL0, SHADE, 0, 0, 0, SHADE
#define G_CC_REFLECTRGBDECALA               ENVIRONMENT, 0, TEXEL0, SHADE, 0, 0, 0, TEXEL0
#define G_CC_HILITERGB                      PRIMITIVE, SHADE, TEXEL0, SHADE, 0, 0, 0, SHADE
#define G_CC_HILITERGBA                     PRIMITIVE, SHADE, TEXEL0, SHADE, PRIMITIVE, SHADE, TEXEL0, SHADE
#define G_CC_HILITERGBDECALA                PRIMITIVE, SHADE, TEXEL0, SHADE, 0, 0, 0, TEXEL0
#define G_CC_SHADEDECALA                    0, 0, 0, SHADE, 0, 0, 0, TEXEL0
#define G_CC_SHADEFADEA                     0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT
#define G_CC_BLENDPE                        PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, SHADE, 0
#define G_CC_BLENDPEDECALA                  PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, TEXEL0
#define _G_CC_BLENDPE                       ENVIRONMENT, PRIMITIVE, TEXEL0, PRIMITIVE, TEXEL0, 0, SHADE, 0
#define _G_CC_BLENDPEDECALA                 ENVIRONMENT, PRIMITIVE, TEXEL0, PRIMITIVE, 0, 0, 0, TEXEL0
#define _G_CC_TWOCOLORTEX                   PRIMITIVE, SHADE, TEXEL0, SHADE, 0, 0, 0, SHADE
#define _G_CC_SPARSEST                      PRIMITIVE, TEXEL0, LOD_FRACTION, TEXEL0, PRIMITIVE, TEXEL0, LOD_FRACTION, TEXEL0
#define G_CC_TEMPLERP                       TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0
#define G_CC_TRILERP                        TEXEL1, TEXEL0, LOD_FRACTION, TEXEL0, TEXEL1, TEXEL0, LOD_FRACTION, TEXEL0
#define G_CC_INTERFERENCE                   TEXEL0, 0, TEXEL1, 0, TEXEL0, 0, TEXEL1, 0
#define G_CC_1CYUV2RGB                      TEXEL0, K4, K5, TEXEL0, 0, 0, 0, SHADE
#define G_CC_YUV2RGB                        TEXEL1, K4, K5, TEXEL1, 0, 0, 0, 0

// Common CC cycle 2 modes
#define G_CC_PASS2                          0, 0, 0, COMBINED, 0, 0, 0, COMBINED
#define G_CC_MODULATEI2                     COMBINED, 0, SHADE, 0, 0, 0, 0, SHADE
#define G_CC_MODULATEIA2                    COMBINED, 0, SHADE, 0, COMBINED, 0, SHADE, 0
#define G_CC_MODULATERGB2                   G_CC_MODULATEI2
#define G_CC_MODULATERGBA2                  G_CC_MODULATEIA2
#define G_CC_MODULATEI_PRIM2                COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE
#define G_CC_MODULATEIA_PRIM2               COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0
#define G_CC_MODULATERGB_PRIM2              G_CC_MODULATEI_PRIM2
#define G_CC_MODULATERGBA_PRIM2             G_CC_MODULATEIA_PRIM2
#define G_CC_DECALRGB2                      0, 0, 0, COMBINED, 0, 0, 0, SHADE
#define G_CC_BLENDI2                        ENVIRONMENT, SHADE, COMBINED, SHADE, 0, 0, 0, SHADE
#define G_CC_BLENDIA2                       ENVIRONMENT, SHADE, COMBINED, SHADE, COMBINED, 0, SHADE, 0
#define G_CC_CHROMA_KEY2                    TEXEL0, CENTER, SCALE, 0, 0, 0, 0, 0
#define G_CC_HILITERGB2                     ENVIRONMENT, COMBINED, TEXEL0, COMBINED, 0, 0, 0, SHADE
#define G_CC_HILITERGBA2                    ENVIRONMENT, COMBINED, TEXEL0, COMBINED, ENVIRONMENT, COMBINED, TEXEL0, COMBINED
#define G_CC_HILITERGBDECALA2               ENVIRONMENT, COMBINED, TEXEL0, COMBINED, 0, 0, 0, TEXEL0
#define G_CC_HILITERGBPASSA2                ENVIRONMENT, COMBINED, TEXEL0, COMBINED, 0, 0, 0, COMBINED

// G_SETOTHERMODE_L sft: shift count
#define G_MDSFT_ALPHACOMPARE                0
#define G_MDSFT_ZSRCSEL                     2
#define G_MDSFT_RENDERMODE                  3
#define G_MDSFT_BLENDER                     16

// G_SETOTHERMODE_H sft: shift count
#define G_MDSFT_BLENDMASK                   0
#define G_MDSFT_ALPHADITHER                 4
#define G_MDSFT_RGBDITHER                   6
#define G_MDSFT_COMBKEY                     8
#define G_MDSFT_TEXTCONV                    9
#define G_MDSFT_TEXTFILT                    12
#define G_MDSFT_TEXTLUT                     14
#define G_MDSFT_TEXTLOD                     16
#define G_MDSFT_TEXTDETAIL                  17
#define G_MDSFT_TEXTPERSP                   19
#define G_MDSFT_CYCLETYPE                   20
#define G_MDSFT_COLORDITHER                 22
#define G_MDSFT_PIPELINE                    23

// G_SETOTHERMODE_H gPipelineMode
#define G_PM_1PRIMITIVE                     (1 << G_MDSFT_PIPELINE)
#define G_PM_NPRIMITIVE                     (0 << G_MDSFT_PIPELINE)

// G_SETOTHERMODE_H gSetCycleType
#define G_CYC_1CYCLE                        (0 << G_MDSFT_CYCLETYPE)
#define G_CYC_2CYCLE                        (1 << G_MDSFT_CYCLETYPE)
#define G_CYC_COPY                          (2 << G_MDSFT_CYCLETYPE)
#define G_CYC_FILL                          (3 << G_MDSFT_CYCLETYPE)

// G_SETOTHERMODE_H gSetTexturePersp
#define G_TP_NONE                           (0 << G_MDSFT_TEXTPERSP)
#define G_TP_PERSP                          (1 << G_MDSFT_TEXTPERSP)

// G_SETOTHERMODE_H gSetTextureDetail
#define G_TD_CLAMP                          (0 << G_MDSFT_TEXTDETAIL)
#define G_TD_SHARPEN                        (1 << G_MDSFT_TEXTDETAIL)
#define G_TD_DETAIL                         (2 << G_MDSFT_TEXTDETAIL)

// G_SETOTHERMODE_H gSetTextureLOD
#define G_TL_TILE                           (0 << G_MDSFT_TEXTLOD)
#define G_TL_LOD                            (1 << G_MDSFT_TEXTLOD)

// G_SETOTHERMODE_H gSetTextureLUT
#define G_TT_NONE                           (0 << G_MDSFT_TEXTLUT)
#define G_TT_RGBA16                         (2 << G_MDSFT_TEXTLUT)
#define G_TT_IA16                           (3 << G_MDSFT_TEXTLUT)

// G_SETOTHERMODE_H gSetTextureFilter
#define G_TF_POINT                          (0 << G_MDSFT_TEXTFILT)
#define G_TF_AVERAGE                        (3 << G_MDSFT_TEXTFILT)
#define G_TF_BILERP                         (2 << G_MDSFT_TEXTFILT)

// G_SETOTHERMODE_H gSetTextureConvert
#define G_TC_CONV                           (0 << G_MDSFT_TEXTCONV)
#define G_TC_FILTCONV                       (5 << G_MDSFT_TEXTCONV)
#define G_TC_FILT                           (6 << G_MDSFT_TEXTCONV)

// G_SETOTHERMODE_H gSetCombineKey
#define G_CK_NONE                           (0 << G_MDSFT_COMBKEY)
#define G_CK_KEY                            (1 << G_MDSFT_COMBKEY)

// G_SETOTHERMODE_H gSetColorDither
#define G_CD_MAGICSQ                        (0 << G_MDSFT_RGBDITHER)
#define G_CD_BAYER                          (1 << G_MDSFT_RGBDITHER)
#define G_CD_NOISE                          (2 << G_MDSFT_RGBDITHER)
#define G_CD_DISABLE                        (3 << G_MDSFT_RGBDITHER)
#define G_CD_ENABLE                         G_CD_NOISE

// G_SETOTHERMODE_H gSetAlphaDither
#define G_AD_PATTERN                        (0 << G_MDSFT_ALPHADITHER)
#define G_AD_NOTPATTERN                     (1 << G_MDSFT_ALPHADITHER)
#define G_AD_NOISE                          (2 << G_MDSFT_ALPHADITHER)
#define G_AD_DISABLE                        (3 << G_MDSFT_ALPHADITHER)

// G_SETOTHERMODE_L gSetAlphaCompare
#define G_AC_NONE                           (0 << G_MDSFT_ALPHACOMPARE)
#define G_AC_THRESHOLD                      (1 << G_MDSFT_ALPHACOMPARE)
#define G_AC_DITHER                         (3 << G_MDSFT_ALPHACOMPARE)
#if defined(RAPI_GL) && defined(TRANSPARENCY_GL)
#define	G_AC_COVERAGE                       (5 << G_MDSFT_ALPHACOMPARE)
#else
#define G_AC_COVERAGE                       (0 << G_MDSFT_ALPHACOMPARE)
#endif

// G_SETOTHERMODE_L gSetDepthSource
#define G_ZS_PIXEL                          (0 << G_MDSFT_ZSRCSEL)
#define G_ZS_PRIM                           (1 << G_MDSFT_ZSRCSEL)

// G_SETOTHERMODE_L gSetRenderMode
#define AA_EN                               0x8
#define Z_CMP                               0x10
#define Z_UPD                               0x20
#define IM_RD                               0x40
#define CLR_ON_CVG                          0x80
#define CVG_DST_CLAMP                       0
#define CVG_DST_WRAP                        0x100
#define CVG_DST_FULL                        0x200
#define CVG_DST_SAVE                        0x300
#define ZMODE_OPA                           0
#define ZMODE_INTER                         0x400
#define ZMODE_XLU                           0x800
#define ZMODE_DEC                           0xc00
#define CVG_X_ALPHA                         0x1000
#define ALPHA_CVG_SEL                       0x2000
#define FORCE_BL                            0x4000
#define TEX_EDGE                            0x0000

#define G_BL_CLR_IN                         0
#define G_BL_CLR_MEM                        1
#define G_BL_CLR_BL                         2
#define G_BL_CLR_FOG                        3
#define G_BL_1MA                            0
#define G_BL_A_MEM                          1
#define G_BL_A_IN                           0
#define G_BL_A_FOG                          1
#define G_BL_A_SHADE                        2
#define G_BL_1                              2
#define G_BL_0                              3

#define GBL_c1(m1a, m1b, m2a, m2b) \
    (m1a) << 30 | (m1b) << 26 | (m2a) << 22 | (m2b) << 18

#define GBL_c2(m1a, m1b, m2a, m2b) \
    (m1a) << 28 | (m1b) << 24 | (m2a) << 20 | (m2b) << 16

#define RM_AA_ZB_OPA_SURF(clk) \
    AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_RA_ZB_OPA_SURF(clk) \
    AA_EN | Z_CMP | Z_UPD | CVG_DST_CLAMP | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_AA_ZB_XLU_SURF(clk) \
    AA_EN | Z_CMP | IM_RD | CVG_DST_WRAP | CLR_ON_CVG | \
    FORCE_BL | ZMODE_XLU | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_ZB_OPA_DECAL(clk) \
    AA_EN | Z_CMP | IM_RD | CVG_DST_WRAP | ALPHA_CVG_SEL | \
    ZMODE_DEC | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_RA_ZB_OPA_DECAL(clk) \
    AA_EN | Z_CMP | CVG_DST_WRAP | ALPHA_CVG_SEL | \
    ZMODE_DEC | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_AA_ZB_XLU_DECAL(clk) \
    AA_EN | Z_CMP | IM_RD | CVG_DST_WRAP | CLR_ON_CVG | \
    FORCE_BL | ZMODE_DEC | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_ZB_OPA_INTER(clk) \
    AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | \
    ALPHA_CVG_SEL | ZMODE_INTER | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_RA_ZB_OPA_INTER(clk) \
    AA_EN | Z_CMP | Z_UPD | CVG_DST_CLAMP | \
    ALPHA_CVG_SEL | ZMODE_INTER | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_AA_ZB_XLU_INTER(clk) \
    AA_EN | Z_CMP | IM_RD | CVG_DST_WRAP | CLR_ON_CVG | \
    FORCE_BL | ZMODE_INTER | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_ZB_XLU_LINE(clk) \
    AA_EN | Z_CMP | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA | \
    ALPHA_CVG_SEL | FORCE_BL | ZMODE_XLU | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_ZB_DEC_LINE(clk) \
    AA_EN | Z_CMP | IM_RD | CVG_DST_SAVE | CVG_X_ALPHA | \
    ALPHA_CVG_SEL | FORCE_BL | ZMODE_DEC | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_ZB_TEX_EDGE(clk) \
    AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | \
    CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_AA_ZB_TEX_INTER(clk) \
    AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | \
    CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_INTER | TEX_EDGE | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_AA_ZB_SUB_SURF(clk) \
    AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_FULL | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_AA_ZB_PCL_SURF(clk) \
    AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | \
    ZMODE_OPA | G_AC_DITHER | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_ZB_OPA_TERR(clk) \
    AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_ZB_TEX_TERR(clk) \
    AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | \
    CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_ZB_SUB_TERR(clk) \
    AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_FULL | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_OPA_SURF(clk) \
    AA_EN | IM_RD | CVG_DST_CLAMP | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_RA_OPA_SURF(clk) \
    AA_EN | CVG_DST_CLAMP | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_AA_XLU_SURF(clk) \
    AA_EN | IM_RD | CVG_DST_WRAP | CLR_ON_CVG | FORCE_BL | \
    ZMODE_OPA | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_XLU_LINE(clk) \
    AA_EN | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA | \
    ALPHA_CVG_SEL | FORCE_BL | ZMODE_OPA | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_DEC_LINE(clk) \
    AA_EN | IM_RD | CVG_DST_FULL | CVG_X_ALPHA | \
    ALPHA_CVG_SEL | FORCE_BL | ZMODE_OPA | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_TEX_EDGE(clk) \
    AA_EN | IM_RD | CVG_DST_CLAMP | \
    CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_AA_SUB_SURF(clk) \
    AA_EN | IM_RD | CVG_DST_FULL | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define RM_AA_PCL_SURF(clk) \
    AA_EN | IM_RD | CVG_DST_CLAMP | \
    ZMODE_OPA | G_AC_DITHER | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_OPA_TERR(clk) \
    AA_EN | IM_RD | CVG_DST_CLAMP | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_TEX_TERR(clk) \
    AA_EN | IM_RD | CVG_DST_CLAMP | \
    CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_AA_SUB_TERR(clk) \
    AA_EN | IM_RD | CVG_DST_FULL | \
    ZMODE_OPA | ALPHA_CVG_SEL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_ZB_OPA_SURF(clk) \
    Z_CMP | Z_UPD | CVG_DST_FULL | ALPHA_CVG_SEL | \
    ZMODE_OPA | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
    
#define RM_ZB_XLU_SURF(clk) \
    Z_CMP | IM_RD | CVG_DST_FULL | FORCE_BL | ZMODE_XLU | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
    
#define RM_ZB_OPA_DECAL(clk) \
    Z_CMP | CVG_DST_FULL | ALPHA_CVG_SEL | ZMODE_DEC | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
    
#define RM_ZB_XLU_DECAL(clk) \
    Z_CMP | IM_RD | CVG_DST_FULL | FORCE_BL | ZMODE_DEC | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
    
#define RM_ZB_CLD_SURF(clk) \
    Z_CMP | IM_RD | CVG_DST_SAVE | FORCE_BL | ZMODE_XLU | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
    
#define RM_ZB_OVL_SURF(clk) \
    Z_CMP | IM_RD | CVG_DST_SAVE | FORCE_BL | ZMODE_DEC | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
    
#define RM_ZB_PCL_SURF(clk) \
    Z_CMP | Z_UPD | CVG_DST_FULL | ZMODE_OPA | \
    G_AC_DITHER | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)

#define RM_OPA_SURF(clk) \
    CVG_DST_CLAMP | FORCE_BL | ZMODE_OPA | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)

#define RM_XLU_SURF(clk) \
    IM_RD | CVG_DST_FULL | FORCE_BL | ZMODE_OPA | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_TEX_EDGE(clk) \
    CVG_DST_CLAMP | CVG_X_ALPHA | ALPHA_CVG_SEL | FORCE_BL |\
    ZMODE_OPA | TEX_EDGE | AA_EN | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)

#define RM_CLD_SURF(clk) \
    IM_RD | CVG_DST_SAVE | FORCE_BL | ZMODE_OPA | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define RM_PCL_SURF(clk) \
    CVG_DST_FULL | FORCE_BL | ZMODE_OPA | \
    G_AC_DITHER | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)

#define RM_ADD(clk) \
    IM_RD | CVG_DST_SAVE | FORCE_BL | ZMODE_OPA | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_A_FOG, G_BL_CLR_MEM, G_BL_1)

#define RM_NOOP(clk) \
    GBL_c##clk(0, 0, 0, 0)

#define RM_VISCVG(clk) \
    IM_RD | FORCE_BL | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_0, G_BL_CLR_BL, G_BL_A_MEM)

#define RM_OPA_CI(clk) \
    CVG_DST_CLAMP | ZMODE_OPA | \
    GBL_c##clk(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)

#define RM_CUSTOM_AA_ZB_XLU_SURF(clk) \
    RM_AA_ZB_XLU_SURF(clk) | Z_UPD

#define G_RM_AA_ZB_OPA_SURF                 RM_AA_ZB_OPA_SURF(1)
#define G_RM_AA_ZB_OPA_SURF2                RM_AA_ZB_OPA_SURF(2)
#define G_RM_AA_ZB_XLU_SURF                 RM_AA_ZB_XLU_SURF(1)
#define G_RM_AA_ZB_XLU_SURF2                RM_AA_ZB_XLU_SURF(2)
#define G_RM_AA_ZB_OPA_DECAL                RM_AA_ZB_OPA_DECAL(1)
#define G_RM_AA_ZB_OPA_DECAL2               RM_AA_ZB_OPA_DECAL(2)
#define G_RM_AA_ZB_XLU_DECAL                RM_AA_ZB_XLU_DECAL(1)
#define G_RM_AA_ZB_XLU_DECAL2               RM_AA_ZB_XLU_DECAL(2)
#define G_RM_AA_ZB_OPA_INTER                RM_AA_ZB_OPA_INTER(1)
#define G_RM_AA_ZB_OPA_INTER2               RM_AA_ZB_OPA_INTER(2)
#define G_RM_AA_ZB_XLU_INTER                RM_AA_ZB_XLU_INTER(1)
#define G_RM_AA_ZB_XLU_INTER2               RM_AA_ZB_XLU_INTER(2)
#define G_RM_AA_ZB_XLU_LINE                 RM_AA_ZB_XLU_LINE(1)
#define G_RM_AA_ZB_XLU_LINE2                RM_AA_ZB_XLU_LINE(2)
#define G_RM_AA_ZB_DEC_LINE                 RM_AA_ZB_DEC_LINE(1)
#define G_RM_AA_ZB_DEC_LINE2                RM_AA_ZB_DEC_LINE(2)
#define G_RM_AA_ZB_TEX_EDGE                 RM_AA_ZB_TEX_EDGE(1)
#define G_RM_AA_ZB_TEX_EDGE2                RM_AA_ZB_TEX_EDGE(2)
#define G_RM_AA_ZB_TEX_INTER                RM_AA_ZB_TEX_INTER(1)
#define G_RM_AA_ZB_TEX_INTER2               RM_AA_ZB_TEX_INTER(2)
#define G_RM_AA_ZB_SUB_SURF                 RM_AA_ZB_SUB_SURF(1)
#define G_RM_AA_ZB_SUB_SURF2                RM_AA_ZB_SUB_SURF(2)
#define G_RM_AA_ZB_PCL_SURF                 RM_AA_ZB_PCL_SURF(1)
#define G_RM_AA_ZB_PCL_SURF2                RM_AA_ZB_PCL_SURF(2)
#define G_RM_AA_ZB_OPA_TERR                 RM_AA_ZB_OPA_TERR(1)
#define G_RM_AA_ZB_OPA_TERR2                RM_AA_ZB_OPA_TERR(2)
#define G_RM_AA_ZB_TEX_TERR                 RM_AA_ZB_TEX_TERR(1)
#define G_RM_AA_ZB_TEX_TERR2                RM_AA_ZB_TEX_TERR(2)
#define G_RM_AA_ZB_SUB_TERR                 RM_AA_ZB_SUB_TERR(1)
#define G_RM_AA_ZB_SUB_TERR2                RM_AA_ZB_SUB_TERR(2)
#define G_RM_RA_ZB_OPA_SURF                 RM_RA_ZB_OPA_SURF(1)
#define G_RM_RA_ZB_OPA_SURF2                RM_RA_ZB_OPA_SURF(2)
#define G_RM_RA_ZB_OPA_DECAL                RM_RA_ZB_OPA_DECAL(1)
#define G_RM_RA_ZB_OPA_DECAL2               RM_RA_ZB_OPA_DECAL(2)
#define G_RM_RA_ZB_OPA_INTER                RM_RA_ZB_OPA_INTER(1)
#define G_RM_RA_ZB_OPA_INTER2               RM_RA_ZB_OPA_INTER(2)
#define G_RM_AA_OPA_SURF                    RM_AA_OPA_SURF(1)
#define G_RM_AA_OPA_SURF2                   RM_AA_OPA_SURF(2)
#define G_RM_AA_XLU_SURF                    RM_AA_XLU_SURF(1)
#define G_RM_AA_XLU_SURF2                   RM_AA_XLU_SURF(2)
#define G_RM_AA_XLU_LINE                    RM_AA_XLU_LINE(1)
#define G_RM_AA_XLU_LINE2                   RM_AA_XLU_LINE(2)
#define G_RM_AA_DEC_LINE                    RM_AA_DEC_LINE(1)
#define G_RM_AA_DEC_LINE2                   RM_AA_DEC_LINE(2)
#define G_RM_AA_TEX_EDGE                    RM_AA_TEX_EDGE(1)
#define G_RM_AA_TEX_EDGE2                   RM_AA_TEX_EDGE(2)
#define G_RM_AA_SUB_SURF                    RM_AA_SUB_SURF(1)
#define G_RM_AA_SUB_SURF2                   RM_AA_SUB_SURF(2)
#define G_RM_AA_PCL_SURF                    RM_AA_PCL_SURF(1)
#define G_RM_AA_PCL_SURF2                   RM_AA_PCL_SURF(2)
#define G_RM_AA_OPA_TERR                    RM_AA_OPA_TERR(1)
#define G_RM_AA_OPA_TERR2                   RM_AA_OPA_TERR(2)
#define G_RM_AA_TEX_TERR                    RM_AA_TEX_TERR(1)
#define G_RM_AA_TEX_TERR2                   RM_AA_TEX_TERR(2)
#define G_RM_AA_SUB_TERR                    RM_AA_SUB_TERR(1)
#define G_RM_AA_SUB_TERR2                   RM_AA_SUB_TERR(2)
#define G_RM_RA_OPA_SURF                    RM_RA_OPA_SURF(1)
#define G_RM_RA_OPA_SURF2                   RM_RA_OPA_SURF(2)
#define G_RM_ZB_OPA_SURF                    RM_ZB_OPA_SURF(1)
#define G_RM_ZB_OPA_SURF2                   RM_ZB_OPA_SURF(2)
#define G_RM_ZB_XLU_SURF                    RM_ZB_XLU_SURF(1)
#define G_RM_ZB_XLU_SURF2                   RM_ZB_XLU_SURF(2)
#define G_RM_ZB_OPA_DECAL                   RM_ZB_OPA_DECAL(1)
#define G_RM_ZB_OPA_DECAL2                  RM_ZB_OPA_DECAL(2)
#define G_RM_ZB_XLU_DECAL                   RM_ZB_XLU_DECAL(1)
#define G_RM_ZB_XLU_DECAL2                  RM_ZB_XLU_DECAL(2)
#define G_RM_ZB_CLD_SURF                    RM_ZB_CLD_SURF(1)
#define G_RM_ZB_CLD_SURF2                   RM_ZB_CLD_SURF(2)
#define G_RM_ZB_OVL_SURF                    RM_ZB_OVL_SURF(1)
#define G_RM_ZB_OVL_SURF2                   RM_ZB_OVL_SURF(2)
#define G_RM_ZB_PCL_SURF                    RM_ZB_PCL_SURF(1)
#define G_RM_ZB_PCL_SURF2                   RM_ZB_PCL_SURF(2)
#define G_RM_OPA_SURF                       RM_OPA_SURF(1)
#define G_RM_OPA_SURF2                      RM_OPA_SURF(2)
#define G_RM_XLU_SURF                       RM_XLU_SURF(1)
#define G_RM_XLU_SURF2                      RM_XLU_SURF(2)
#define G_RM_CLD_SURF                       RM_CLD_SURF(1)
#define G_RM_CLD_SURF2                      RM_CLD_SURF(2)
#define G_RM_TEX_EDGE                       RM_TEX_EDGE(1)
#define G_RM_TEX_EDGE2                      RM_TEX_EDGE(2)
#define G_RM_PCL_SURF                       RM_PCL_SURF(1)
#define G_RM_PCL_SURF2                      RM_PCL_SURF(2)
#define G_RM_ADD                            RM_ADD(1)
#define G_RM_ADD2                           RM_ADD(2)
#define G_RM_NOOP                           RM_NOOP(1)
#define G_RM_NOOP2                          RM_NOOP(2)
#define G_RM_VISCVG                         RM_VISCVG(1)
#define G_RM_VISCVG2                        RM_VISCVG(2)
#define G_RM_OPA_CI                         RM_OPA_CI(1)
#define G_RM_OPA_CI2                        RM_OPA_CI(2)
#define G_RM_CUSTOM_AA_ZB_XLU_SURF          RM_CUSTOM_AA_ZB_XLU_SURF(1)
#define G_RM_CUSTOM_AA_ZB_XLU_SURF2         RM_CUSTOM_AA_ZB_XLU_SURF(2)
#define G_RM_FOG_SHADE_A                    GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA)
#define G_RM_FOG_PRIM_A                     GBL_c1(G_BL_CLR_FOG, G_BL_A_FOG, G_BL_CLR_IN, G_BL_1MA)
#define G_RM_PASS                           GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)

// G_SETCONVERT: K0-5
#define G_CV_K0                             175
#define G_CV_K1                             -43
#define G_CV_K2                             -89
#define G_CV_K3                             222
#define G_CV_K4                             114
#define G_CV_K5                             42

// G_SETSCISSOR: interlace mode
#define G_SC_NON_INTERLACE                  0
#define G_SC_ODD_INTERLACE                  3
#define G_SC_EVEN_INTERLACE                 2

// Display list pushing
#define G_DL_PUSH                           0x00
#define G_DL_NOPUSH                         0x01

// C-specific section
#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

typedef struct {
    f32 ob[3]; // x, y, z
    u16 flag;
    s16 tc[2]; // texture coords
    u8 cn[4]; // color and alpha
} Vtx_t;

typedef struct {
    f32 ob[3]; // x, y, z
    u16 flag;
    s16 tc[2]; // texture coords
    s8 n[3]; // normal
    u8 a; // alpha
} Vtx_tn;

typedef union {
    Vtx_t v;
    Vtx_tn n;
    u64 force_structure_alignment;
} Vtx;

typedef struct {
    void *SourceImagePointer;
    void *TlutPointer;
    s16 Stride;
    s16 SubImageWidth;
    s16 SubImageHeight;
    s8 SourceImageType;
    s8 SourceImageBitSize;
    s16 SourceImageOffsetS;
    s16 SourceImageOffsetT;
    s8 dummy[4]; 
} uSprite_t;

typedef union {    
    uSprite_t s;
    u64 force_structure_allignment[3];
} uSprite;

typedef struct {
    u8 flag;
    u8 v[3];
} Tri;

typedef struct {
    f32 m[4][4];
} Mtx;

typedef struct {
    s16 vscale[4]; // scale, 2 bits fraction
    s16 vtrans[4]; // translate, 2 bits fraction
} Vp_t;

typedef union {
    Vp_t vp;
    u64 force_structure_alignment;
} Vp;

typedef struct {
    u8 col[3]; // diffuse light
    s8 pad1;
    u8 colc[3];
    s8 pad2;
    s8 dir[3];
    s8 pad3;
} Light_t;

typedef struct {
    u8 col[3]; // ambient light
    s8 pad1;
    u8 colc[3];
    s8 pad2;
} Ambient_t;

typedef struct {
    s32 x1, y1, x2, y2;
} Hilite_t;

typedef union {
    Light_t l;
    u64 force_structure_alignment[2];
} Light;

typedef union {
    Ambient_t l;
    u64 force_structure_alignment[1];
} Ambient;

typedef struct {
    Ambient a;
    Light l[7];
} Lightsn;

typedef struct {
    Ambient a;
    Light l[1];
} Lights0;

typedef struct {
    Ambient a;
    Light l[1];
} Lights1;

typedef struct {
    Ambient a;
    Light l[2];
} Lights2;

typedef struct {
    Ambient a;
    Light l[3];
} Lights3;

typedef struct {
    Ambient a;
    Light l[4];
} Lights4;

typedef struct {
    Ambient a;
    Light l[5];
} Lights5;

typedef struct {
    Ambient a;
    Light l[6];
} Lights6;

typedef struct {
    Ambient a;
    Light l[7];
} Lights7;

typedef struct {
    Light l[2];
} LookAt;

typedef union {
    Hilite_t h;
    s64 force_structure_alignment[4];
} Hilite;

typedef struct {
    uintptr_t w0;
    uintptr_t w1;
} Gwords;

typedef union {
    Gwords words;
    u64 force_structure_alignment;
} Gfx;

#define G_MAXZ                  0x03ff

// MOVEMEM indices
#define G_MV_MMTX               2
#define G_MV_PMTX               6
#define G_MV_VIEWPORT           8
#define G_MV_LIGHT              10
#define G_MV_POINT              12
#define G_MV_MATRIX             14
#define G_MVO_LOOKATX           (0 * 24)
#define G_MVO_LOOKATY           (1 * 24)
#define G_MVO_L0                (2 * 24)
#define G_MVO_L1                (3 * 24)
#define G_MVO_L2                (4 * 24)
#define G_MVO_L3                (5 * 24)
#define G_MVO_L4                (6 * 24)
#define G_MVO_L5                (7 * 24)
#define G_MVO_L6                (8 * 24)
#define G_MVO_L7                (9 * 24)

// MOVEWORD indices
#define G_MW_MATRIX             0x00
#define G_MW_NUMLIGHT           0x02
#define G_MW_CLIP               0x04
#define G_MW_SEGMENT            0x06
#define G_MW_FOG                0x08
#define G_MW_LIGHTCOL           0x0a
#define G_MW_FORCEMTX           0x0c
#define G_MW_PERSPNORM          0x0e
#define G_MWO_NUMLIGHT          0x00
#define G_MWO_CLIP_RNX          0x04
#define G_MWO_CLIP_RNY          0x0c
#define G_MWO_CLIP_RPX          0x14
#define G_MWO_CLIP_RPY          0x1c
#define G_MWO_SEGMENT_0         0x00
#define G_MWO_SEGMENT_1         0x01
#define G_MWO_SEGMENT_2         0x02
#define G_MWO_SEGMENT_3         0x03
#define G_MWO_SEGMENT_4         0x04
#define G_MWO_SEGMENT_5         0x05
#define G_MWO_SEGMENT_6         0x06
#define G_MWO_SEGMENT_7         0x07
#define G_MWO_SEGMENT_8         0x08
#define G_MWO_SEGMENT_9         0x09
#define G_MWO_SEGMENT_A         0x0a
#define G_MWO_SEGMENT_B         0x0b
#define G_MWO_SEGMENT_C         0x0c
#define G_MWO_SEGMENT_D         0x0d
#define G_MWO_SEGMENT_E         0x0e
#define G_MWO_SEGMENT_F         0x0f
#define G_MWO_FOG               0x00
#define G_MWO_aLIGHT_1          0x00
#define G_MWO_bLIGHT_1          0x04
#define G_MWO_aLIGHT_2          0x18
#define G_MWO_bLIGHT_2          0x1c
#define G_MWO_aLIGHT_3          0x30
#define G_MWO_bLIGHT_3          0x34
#define G_MWO_aLIGHT_4          0x48
#define G_MWO_bLIGHT_4          0x4c
#define G_MWO_aLIGHT_5          0x60
#define G_MWO_bLIGHT_5          0x64
#define G_MWO_aLIGHT_6          0x78
#define G_MWO_bLIGHT_6          0x7c
#define G_MWO_aLIGHT_7          0x90
#define G_MWO_bLIGHT_7          0x94
#define G_MWO_aLIGHT_8          0xa8
#define G_MWO_bLIGHT_8          0xac
#define G_MWO_MATRIX_XX_XY_I    0x00
#define G_MWO_MATRIX_XZ_XW_I    0x04
#define G_MWO_MATRIX_YX_YY_I    0x08
#define G_MWO_MATRIX_YZ_YW_I    0x0c
#define G_MWO_MATRIX_ZX_ZY_I    0x10
#define G_MWO_MATRIX_ZZ_ZW_I    0x14
#define G_MWO_MATRIX_WX_WY_I    0x18
#define G_MWO_MATRIX_WZ_WW_I    0x1c
#define G_MWO_MATRIX_XX_XY_F    0x20
#define G_MWO_MATRIX_XZ_XW_F    0x24
#define G_MWO_MATRIX_YX_YY_F    0x28
#define G_MWO_MATRIX_YZ_YW_F    0x2c
#define G_MWO_MATRIX_ZX_ZY_F    0x30
#define G_MWO_MATRIX_ZZ_ZW_F    0x34
#define G_MWO_MATRIX_WX_WY_F    0x38
#define G_MWO_MATRIX_WZ_WW_F    0x3c
#define G_MWO_POINT_RGBA        0x10
#define G_MWO_POINT_ST          0x14
#define G_MWO_POINT_XYSCREEN    0x18
#define G_MWO_POINT_ZSCREEN     0x1c

#define gdSPDefLights0(ar,ag,ab) \
    { {{ {ar,ag,ab},0,{ar,ag,ab},0}}, \
     {{{ { 0, 0, 0},0,{ 0, 0, 0},0,{ 0, 0, 0},0}}} }

#define gdSPDefLights1(ar,ag,ab,r1,g1,b1,x1,y1,z1) \
    { {{ {ar,ag,ab},0,{ar,ag,ab},0}}, \
     {{{ {r1,g1,b1},0,{r1,g1,b1},0,{x1,y1,z1},0}}} }

#define gdSPDefLights2(ar,ag,ab,r1,g1,b1,x1,y1,z1,r2,g2,b2,x2,y2,z2) \
    { {{ {ar,ag,ab},0,{ar,ag,ab},0}}, \
     {{{ {r1,g1,b1},0,{r1,g1,b1},0,{x1,y1,z1},0}}, \
      {{ {r2,g2,b2},0,{r2,g2,b2},0,{x2,y2,z2},0}}} }

#define gdSPDefLights3(ar,ag,ab,r1,g1,b1,x1,y1,z1,r2,g2,b2,x2,y2,z2,r3,g3,b3,x3,y3,z3) \
    { {{ {ar,ag,ab},0,{ar,ag,ab},0}}, \
     {{{ {r1,g1,b1},0,{r1,g1,b1},0,{x1,y1,z1},0}}, \
      {{ {r2,g2,b2},0,{r2,g2,b2},0,{x2,y2,z2},0}}, \
      {{ {r3,g3,b3},0,{r3,g3,b3},0,{x3,y3,z3},0}}} }

#define gdSPDefLights4(ar,ag,ab,r1,g1,b1,x1,y1,z1,r2,g2,b2,x2,y2,z2,r3,g3,b3,x3,y3,z3,r4,g4,b4,x4,y4,z4) \
    { {{ {ar,ag,ab},0,{ar,ag,ab},0}}, \
     {{{ {r1,g1,b1},0,{r1,g1,b1},0,{x1,y1,z1},0}}, \
      {{ {r2,g2,b2},0,{r2,g2,b2},0,{x2,y2,z2},0}}, \
      {{ {r3,g3,b3},0,{r3,g3,b3},0,{x3,y3,z3},0}}, \
      {{ {r4,g4,b4},0,{r4,g4,b4},0,{x4,y4,z4},0}}} }

#define gdSPDefLights5(ar,ag,ab,r1,g1,b1,x1,y1,z1,r2,g2,b2,x2,y2,z2,r3,g3,b3,x3,y3,z3,r4,g4,b4,x4,y4,z4,r5,g5,b5,x5,y5,z5) \
    { {{ {ar,ag,ab},0,{ar,ag,ab},0}}, \
     {{{ {r1,g1,b1},0,{r1,g1,b1},0,{x1,y1,z1},0}}, \
      {{ {r2,g2,b2},0,{r2,g2,b2},0,{x2,y2,z2},0}}, \
      {{ {r3,g3,b3},0,{r3,g3,b3},0,{x3,y3,z3},0}}, \
      {{ {r4,g4,b4},0,{r4,g4,b4},0,{x4,y4,z4},0}}, \
      {{ {r5,g5,b5},0,{r5,g5,b5},0,{x5,y5,z5},0}}} }

#define gdSPDefLights6(ar,ag,ab,r1,g1,b1,x1,y1,z1,r2,g2,b2,x2,y2,z2,r3,g3,b3,x3,y3,z3,r4,g4,b4,x4,y4,z4,r5,g5,b5,x5,y5,z5,r6,g6,b6,x6,y6,z6) \
    { {{ {ar,ag,ab},0,{ar,ag,ab},0}}, \
     {{{ {r1,g1,b1},0,{r1,g1,b1},0,{x1,y1,z1},0}}, \
      {{ {r2,g2,b2},0,{r2,g2,b2},0,{x2,y2,z2},0}}, \
      {{ {r3,g3,b3},0,{r3,g3,b3},0,{x3,y3,z3},0}}, \
      {{ {r4,g4,b4},0,{r4,g4,b4},0,{x4,y4,z4},0}}, \
      {{ {r5,g5,b5},0,{r5,g5,b5},0,{x5,y5,z5},0}}, \
      {{ {r6,g6,b6},0,{r6,g6,b6},0,{x6,y6,z6},0}}} }

#define gdSPDefLights7(ar,ag,ab,r1,g1,b1,x1,y1,z1,r2,g2,b2,x2,y2,z2,r3,g3,b3,x3,y3,z3,r4,g4,b4,x4,y4,z4,r5,g5,b5,x5,y5,z5,r6,g6,b6,x6,y6,z6,r7,g7,b7,x7,y7,z7) \
    { {{ {ar,ag,ab},0,{ar,ag,ab},0}}, \
     {{{ {r1,g1,b1},0,{r1,g1,b1},0,{x1,y1,z1},0}}, \
      {{ {r2,g2,b2},0,{r2,g2,b2},0,{x2,y2,z2},0}}, \
      {{ {r3,g3,b3},0,{r3,g3,b3},0,{x3,y3,z3},0}}, \
      {{ {r4,g4,b4},0,{r4,g4,b4},0,{x4,y4,z4},0}}, \
      {{ {r5,g5,b5},0,{r5,g5,b5},0,{x5,y5,z5},0}}, \
      {{ {r6,g6,b6},0,{r6,g6,b6},0,{x6,y6,z6},0}}, \
      {{ {r7,g7,b7},0,{r7,g7,b7},0,{x7,y7,z7},0}}} }

#define gdSPDefLookAt(rightx,righty,rightz,upx,upy,upz) \
    { {{ {{0,0,0},0,{0,0,0},0,{rightx,righty,rightz},0}}, \
       { {{0,0x80,0},0,{0,0x80,0},0,{upx,upy,upz},0}}}   }

#define gDma0p(pkt, c, s, l) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL((c), 24, 8) | _SHIFTL((l), 0, 24); \
    _g->words.w1 = (uintptr_t)(s); \
}

#define gsDma0p(c, s, l) \
{{ \
    _SHIFTL((c), 24, 8) | _SHIFTL((l), 0, 24), (uintptr_t)(s) \
}}

#define gDma1p(pkt, c, s, l, p) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | _SHIFTL((l), 0, 16)); \
    _g->words.w1 = (uintptr_t)(s); \
}

#define gsDma1p(c, s, l, p) \
{{ \
    (_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | _SHIFTL((l), 0, 16)), (uintptr_t)(s) \
}}

#define gDma2p(pkt, c, adrs, len, idx, ofs) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL((c),24,8) | _SHIFTL(((len)-1)/8,19,5) | _SHIFTL((ofs)/8,8,8) | _SHIFTL((idx),0,8)); \
    _g->words.w1 = (uintptr_t)(adrs); \
}

#define gsDma2p(c, adrs, len, idx, ofs) \
{{ \
    (_SHIFTL((c),24,8) | _SHIFTL(((len)-1)/8,19,5) | _SHIFTL((ofs)/8,8,8) | _SHIFTL((idx),0,8)), (uintptr_t)(adrs) \
}}

#define gSPNoOp(pkt)                gDma0p(pkt, G_SPNOOP, 0, 0)
#define gsSPNoOp()                  gsDma0p(G_SPNOOP, 0, 0)
#define gSPMatrix(pkt, m, p)        gDma2p((pkt),G_MTX,(m),sizeof(Mtx),(p)^G_MTX_PUSH,0)
#define gsSPMatrix(m, p)            gsDma2p(     G_MTX,(m),sizeof(Mtx),(p)^G_MTX_PUSH,0)

#define gSPVertex(pkt, v, n, v0) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_VTX,24,8) | _SHIFTL((n),12,8) | _SHIFTL((v0)+(n),1,7); \
    _g->words.w1 = (uintptr_t)(v); \
}

#define gsSPVertex(v, n, v0) \
{{ \
    (_SHIFTL(G_VTX,24,8) | _SHIFTL((n),12,8) | _SHIFTL((v0)+(n),1,7)), (uintptr_t)(v) \
}}

#define gSPViewport(pkt, v)         gDma2p((pkt), G_MOVEMEM, (v), sizeof(Vp), G_MV_VIEWPORT, 0)
#define gsSPViewport(v)             gsDma2p(      G_MOVEMEM, (v), sizeof(Vp), G_MV_VIEWPORT, 0)

#define gSPDisplayList(pkt,dl)      gDma1p(pkt,G_DL,dl,0,G_DL_PUSH)
#define gsSPDisplayList(   dl)      gsDma1p(   G_DL,dl,0,G_DL_PUSH)

#define gSPBranchList(pkt,dl)       gDma1p(pkt,G_DL,dl,0,G_DL_NOPUSH)
#define gsSPBranchList(   dl)       gsDma1p(   G_DL,dl,0,G_DL_NOPUSH)

#define gSPSprite2DBase(pkt, s)     gDma1p(pkt, G_SPRITE2D_BASE, s, sizeof(uSprite), 0)
#define gsSPSprite2DBase(s)         gsDma1p(G_SPRITE2D_BASE, s, sizeof(uSprite), 0)

#define gImmp0(pkt, c) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL((c), 24, 8); \
}

#define gsImmp0(c) \
{{ \
    _SHIFTL((c), 24, 8) \
}}

#define gImmp1(pkt, c, p0) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL((c), 24, 8); \
    _g->words.w1 = (uintptr_t)(p0); \
}

#define gsImmp1(c, p0) \
{{ \
    _SHIFTL((c), 24, 8), (uintptr_t)(p0) \
}}

#define gImmp2(pkt, c, p0, p1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL((c), 24, 8); \
    _g->words.w1 = _SHIFTL((p0), 16, 16) | _SHIFTL((p1), 8, 8); \
}

#define gsImmp2(c, p0, p1) \
{{ \
    _SHIFTL((c), 24, 8),  _SHIFTL((p0), 16, 16) | _SHIFTL((p1), 8, 8)\
}}

#define gImmp3(pkt, c, p0, p1, p2) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL((c), 24, 8); \
    _g->words.w1 = (_SHIFTL((p0), 16, 16) | _SHIFTL((p1), 8, 8) | _SHIFTL((p2), 0, 8)); \
}

#define gsImmp3(c, p0, p1, p2) \
{{ \
    _SHIFTL((c), 24, 8), (_SHIFTL((p0), 16, 16) | _SHIFTL((p1), 8, 8) | _SHIFTL((p2), 0, 8))\
}}

#define gImmp21(pkt, c, p0, p1, dat) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL((c), 24, 8)  | _SHIFTL((p0), 8, 16) | _SHIFTL((p1), 0, 8)); \
    _g->words.w1 = (uintptr_t) (dat); \
}

#define gsImmp21(c, p0, p1, dat) \
{{ \
    _SHIFTL((c), 24, 8) | _SHIFTL((p0), 8, 16) | _SHIFTL((p1), 0, 8), (uintptr_t) (dat) \
}}

#define gMoveWd(pkt, index, offset, data)   gDma1p((pkt), G_MOVEWORD, data, offset, index)
#define gsMoveWd(    index, offset, data)   gsDma1p(      G_MOVEWORD, data, offset, index)

#define gSPSprite2DScaleFlip(pkt, sx, sy, fx, fy) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_SPRITE2D_SCALEFLIP, 24, 8) | _SHIFTL((fx), 8, 8) | _SHIFTL((fy), 0, 8)); \
    _g->words.w1 = (_SHIFTL((sx), 16, 16) | _SHIFTL((sy),  0, 16)); \
}

#define gsSPSprite2DScaleFlip(sx, sy, fx, fy) \
{{ \
    (_SHIFTL(G_SPRITE2D_SCALEFLIP, 24, 8) | _SHIFTL((fx), 8, 8) | _SHIFTL((fy), 0, 8)), \
    (_SHIFTL((sx), 16, 16) | _SHIFTL((sy),  0, 16)) \
}}

#define gSPSprite2DDraw(pkt, px, py) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_SPRITE2D_DRAW, 24, 8)); \
    _g->words.w1 = (_SHIFTL((px), 16, 16) | _SHIFTL((py),  0, 16)); \
}

#define gsSPSprite2DDraw(px, py) \
{{ \
    (_SHIFTL(G_SPRITE2D_DRAW, 24, 8)), (_SHIFTL((px), 16, 16) | _SHIFTL((py),  0, 16)) \
}}

#define __gsSP1Triangle_w1(v0, v1, v2)  (_SHIFTL((v0)*2,16,8) | _SHIFTL((v1)*2,8,8) | _SHIFTL((v2)*2,0,8))
#define __gsSP1Triangle_w1f(v0, v1, v2, flag) \
    (((flag) == 0) ? __gsSP1Triangle_w1(v0, v1, v2): \
     ((flag) == 1) ? __gsSP1Triangle_w1(v1, v2, v0): \
                     __gsSP1Triangle_w1(v2, v0, v1))

#define __gsSPLine3D_w1(v0, v1, wd)     (_SHIFTL((v0)*2,16,8) | _SHIFT((v1)*2,8,8) | _SHIFT((wd),0,8))
#define __gsSPLine3D_w1f(v0, v1, wd, flag) \
    (((flag) == 0) ? __gsSPLine3D_w1(v0, v1, wd): \
                     __gsSPLine3D_w1(v1, v0, wd))

#define __gsSP1Quadrangle_w1f(v0, v1, v2, v3, flag) \
    (((flag) == 0) ? __gsSP1Triangle_w1(v0, v1, v2): \
     ((flag) == 1) ? __gsSP1Triangle_w1(v1, v2, v3): \
     ((flag) == 2) ? __gsSP1Triangle_w1(v2, v3, v0): \
                     __gsSP1Triangle_w1(v3, v0, v1))

#define __gsSP1Quadrangle_w2f(v0, v1, v2, v3, flag) \
    (((flag) == 0) ? __gsSP1Triangle_w1(v0, v2, v3): \
     ((flag) == 1) ? __gsSP1Triangle_w1(v1, v3, v0): \
     ((flag) == 2) ? __gsSP1Triangle_w1(v2, v0, v1): \
                     __gsSP1Triangle_w1(v3, v1, v2))

#define gSP1Triangle(pkt, v0, v1, v2, flag) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_TRI1, 24, 8) | __gsSP1Triangle_w1f(v0, v1, v2, flag); \
    _g->words.w1 = 0; \
}

#define gsSP1Triangle(v0, v1, v2, flag) \
{{ \
    _SHIFTL(G_TRI1, 24, 8) | __gsSP1Triangle_w1f(v0, v1, v2, flag), 0 \
}}

#define gSPLine3D(pkt, v0, v1, flag) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_LINE3D, 24, 8) | __gsSPLine3D_w1f(v0, v1, 0, flag); \
    _g->words.w1 = 0; \
}

#define gsSPLine3D(v0, v1, flag) \
{{ \
    _SHIFTL(G_LINE3D, 24, 8) | __gsSPLine3D_w1f(v0, v1, 0, flag), 0 \
}}

#define gSPLineW3D(pkt, v0, v1, wd, flag) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_LINE3D, 24, 8) | __gsSPLine3D_w1f(v0, v1, wd, flag); \
    _g->words.w1 = 0; \
}

#define gsSPLineW3D(v0, v1, wd, flag) \
{{ \
    _SHIFTL(G_LINE3D, 24, 8) | __gsSPLine3D_w1f(v0, v1, wd, flag), 0 \
}}

#define gSP1Quadrangle(pkt, v0, v1, v2, v3, flag) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_QUAD, 24, 8) | __gsSP1Quadrangle_w1f(v0, v1, v2, v3, flag)); \
    _g->words.w1 =                           __gsSP1Quadrangle_w2f(v0, v1, v2, v3, flag); \
}

#define gsSP1Quadrangle(v0, v1, v2, v3, flag) \
{{ \
    (_SHIFTL(G_QUAD, 24, 8) | __gsSP1Quadrangle_w1f(v0, v1, v2, v3, flag)), \
                              __gsSP1Quadrangle_w2f(v0, v1, v2, v3, flag) \
}}

#define gSP2Triangles(pkt, v00, v01, v02, flag0, v10, v11, v12, flag1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TRI2, 24, 8) | __gsSP1Triangle_w1f(v00, v01, v02, flag0)); \
    _g->words.w1 =                           __gsSP1Triangle_w1f(v10, v11, v12, flag1); \
}

#define gsSP2Triangles(v00, v01, v02, flag0, v10, v11, v12, flag1) \
{{ \
    (_SHIFTL(G_TRI2, 24, 8) | __gsSP1Triangle_w1f(v00, v01, v02, flag0)), \
                              __gsSP1Triangle_w1f(v10, v11, v12, flag1) \
}}

#define gSPCullDisplayList(pkt,vstart,vend) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_CULLDL, 24, 8) | _SHIFTL((vstart)*2, 0, 16); \
    _g->words.w1 = _SHIFTL((vend)*2, 0, 16); \
}

#define gsSPCullDisplayList(vstart,vend) \
{{ \
    _SHIFTL(G_CULLDL, 24, 8) | _SHIFTL((vstart)*2, 0, 16), \
    _SHIFTL((vend)*2, 0, 16) \
}}

#define gSPSegment(pkt, segment, base)  gMoveWd(pkt, G_MW_SEGMENT, (segment)*4, base)
#define gsSPSegment(segment, base)      gsMoveWd(    G_MW_SEGMENT, (segment)*4, base)

#define FR_NEG_FRUSTRATIO_1     0x00000001
#define FR_POS_FRUSTRATIO_1     0x0000ffff
#define FR_NEG_FRUSTRATIO_2     0x00000002
#define FR_POS_FRUSTRATIO_2     0x0000fffe
#define FR_NEG_FRUSTRATIO_3     0x00000003
#define FR_POS_FRUSTRATIO_3     0x0000fffd
#define FR_NEG_FRUSTRATIO_4     0x00000004
#define FR_POS_FRUSTRATIO_4     0x0000fffc
#define FR_NEG_FRUSTRATIO_5     0x00000005
#define FR_POS_FRUSTRATIO_5     0x0000fffb
#define FR_NEG_FRUSTRATIO_6     0x00000006
#define FR_POS_FRUSTRATIO_6     0x0000fffa

#define gSPClipRatio(pkt, r) \
{ \
    gMoveWd(pkt, G_MW_CLIP, G_MWO_CLIP_RNX, FR_NEG_##r); \
    gMoveWd(pkt, G_MW_CLIP, G_MWO_CLIP_RNY, FR_NEG_##r); \
    gMoveWd(pkt, G_MW_CLIP, G_MWO_CLIP_RPX, FR_POS_##r); \
    gMoveWd(pkt, G_MW_CLIP, G_MWO_CLIP_RPY, FR_POS_##r); \
}

#define gsSPClipRatio(r) \
    gsMoveWd(G_MW_CLIP, G_MWO_CLIP_RNX, FR_NEG_##r), \
    gsMoveWd(G_MW_CLIP, G_MWO_CLIP_RNY, FR_NEG_##r), \
    gsMoveWd(G_MW_CLIP, G_MWO_CLIP_RPX, FR_POS_##r), \
    gsMoveWd(G_MW_CLIP, G_MWO_CLIP_RPY, FR_POS_##r)

#define gSPForceMatrix(pkt, mptr) \
{ \
    gDma2p((pkt),G_MOVEMEM,(mptr),sizeof(Mtx),G_MV_MATRIX,0); \
    gMoveWd((pkt), G_MW_FORCEMTX,0,0x00010000); \
}

#define gsSPForceMatrix(mptr) \
    gsDma2p(G_MOVEMEM,(mptr),sizeof(Mtx),G_MV_MATRIX,0), \
    gsMoveWd(G_MW_FORCEMTX,0,0x00010000)

#define gSPModifyVertex(pkt, vtx, where, val) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_MODIFYVTX,24,8) | _SHIFTL((where),16,8) | _SHIFTL((vtx)*2,0,16)); \
    _g->words.w1 = (u32)(val); \
}
#define gsSPModifyVertex(vtx, where, val) \
{{ \
    _SHIFTL(G_MODIFYVTX,24,8) | _SHIFTL((where),16,8) | _SHIFTL((vtx)*2,0,16), (u32)(val) \
}}

#define NUML(n)         ((n)*24)
#define NUMLIGHTS_0     1
#define NUMLIGHTS_1     1
#define NUMLIGHTS_2     2
#define NUMLIGHTS_3     3
#define NUMLIGHTS_4     4
#define NUMLIGHTS_5     5
#define NUMLIGHTS_6     6
#define NUMLIGHTS_7     7

#define gSPNumLights(pkt, n)    gMoveWd(pkt, G_MW_NUMLIGHT, G_MWO_NUMLIGHT, NUML(n))
#define gsSPNumLights(n)        gsMoveWd(    G_MW_NUMLIGHT, G_MWO_NUMLIGHT, NUML(n))

#define LIGHT_1         1
#define LIGHT_2         2
#define LIGHT_3         3
#define LIGHT_4         4
#define LIGHT_5         5    
#define LIGHT_6         6
#define LIGHT_7         7
#define LIGHT_8         8

#define gSPLight(pkt, l, n)     gDma2p((pkt),G_MOVEMEM,(l),sizeof(Light),G_MV_LIGHT,(n)*24+24)
#define gsSPLight(l, n)         gsDma2p(     G_MOVEMEM,(l),sizeof(Light),G_MV_LIGHT,(n)*24+24)

#define gSPLightColor(pkt, n, col) \
{ \
    gMoveWd(pkt, G_MW_LIGHTCOL, G_MWO_a##n, col); \
    gMoveWd(pkt, G_MW_LIGHTCOL, G_MWO_b##n, col); \
}

#define gsSPLightColor(n, col) \
    gsMoveWd(G_MW_LIGHTCOL, G_MWO_a##n, col), \
    gsMoveWd(G_MW_LIGHTCOL, G_MWO_b##n, col)

#define gSPSetLights0(pkt,name) \
{ \
    gSPNumLights(pkt,NUMLIGHTS_0); \
    gSPLight(pkt,&name.l[0],1); \
    gSPLight(pkt,&name.a,2); \
}

#define gsSPSetLights0(name) \
    gsSPNumLights(NUMLIGHTS_0), \
    gsSPLight(&name.l[0],1), \
    gsSPLight(&name.a,2)

#define gSPSetLights1(pkt,name) \
{ \
    gSPNumLights(pkt,NUMLIGHTS_1); \
    gSPLight(pkt,&name.l[0],1); \
    gSPLight(pkt,&name.a,2); \
}

#define gsSPSetLights1(name) \
    gsSPNumLights(NUMLIGHTS_1), \
    gsSPLight(&name.l[0],1), \
    gsSPLight(&name.a,2)

#define gSPSetLights2(pkt,name) \
{ \
    gSPNumLights(pkt,NUMLIGHTS_2); \
    gSPLight(pkt,&name.l[0],1); \
    gSPLight(pkt,&name.l[1],2); \
    gSPLight(pkt,&name.a,3); \
}

#define gsSPSetLights2(name) \
    gsSPNumLights(NUMLIGHTS_2), \
    gsSPLight(&name.l[0],1), \
    gsSPLight(&name.l[1],2), \
    gsSPLight(&name.a,3)

#define gSPSetLights3(pkt,name) \
{ \
    gSPNumLights(pkt,NUMLIGHTS_3); \
    gSPLight(pkt,&name.l[0],1); \
    gSPLight(pkt,&name.l[1],2); \
    gSPLight(pkt,&name.l[2],3); \
    gSPLight(pkt,&name.a,4); \
}

#define gsSPSetLights3(name) \
    gsSPNumLights(NUMLIGHTS_3), \
    gsSPLight(&name.l[0],1), \
    gsSPLight(&name.l[1],2), \
    gsSPLight(&name.l[2],3), \
    gsSPLight(&name.a,4)

#define gSPSetLights4(pkt,name) \
{ \
    gSPNumLights(pkt,NUMLIGHTS_4); \
    gSPLight(pkt,&name.l[0],1); \
    gSPLight(pkt,&name.l[1],2); \
    gSPLight(pkt,&name.l[2],3); \
    gSPLight(pkt,&name.l[3],4); \
    gSPLight(pkt,&name.a,5); \
}

#define gsSPSetLights4(name) \
    gsSPNumLights(NUMLIGHTS_4), \
    gsSPLight(&name.l[0],1), \
    gsSPLight(&name.l[1],2), \
    gsSPLight(&name.l[2],3), \
    gsSPLight(&name.l[3],4), \
    gsSPLight(&name.a,5)

#define gSPSetLights5(pkt,name) \
{ \
    gSPNumLights(pkt,NUMLIGHTS_5); \
    gSPLight(pkt,&name.l[0],1); \
    gSPLight(pkt,&name.l[1],2); \
    gSPLight(pkt,&name.l[2],3); \
    gSPLight(pkt,&name.l[3],4); \
    gSPLight(pkt,&name.l[4],5); \
    gSPLight(pkt,&name.a,6); \
}

#define gsSPSetLights5(name) \
    gsSPNumLights(NUMLIGHTS_5), \
    gsSPLight(&name.l[0],1), \
    gsSPLight(&name.l[1],2), \
    gsSPLight(&name.l[2],3), \
    gsSPLight(&name.l[3],4), \
    gsSPLight(&name.l[4],5), \
    gsSPLight(&name.a,6)

#define gSPSetLights6(pkt,name) \
{ \
    gSPNumLights(pkt,NUMLIGHTS_6); \
    gSPLight(pkt,&name.l[0],1); \
    gSPLight(pkt,&name.l[1],2); \
    gSPLight(pkt,&name.l[2],3); \
    gSPLight(pkt,&name.l[3],4); \
    gSPLight(pkt,&name.l[4],5); \
    gSPLight(pkt,&name.l[5],6); \
    gSPLight(pkt,&name.a,7); \
}

#define gsSPSetLights6(name) \
    gsSPNumLights(NUMLIGHTS_6), \
    gsSPLight(&name.l[0],1), \
    gsSPLight(&name.l[1],2), \
    gsSPLight(&name.l[2],3), \
    gsSPLight(&name.l[3],4), \
    gsSPLight(&name.l[4],5), \
    gsSPLight(&name.l[5],6), \
    gsSPLight(&name.a,7)

#define gSPSetLights7(pkt,name) \
{ \
    gSPNumLights(pkt,NUMLIGHTS_7); \
    gSPLight(pkt,&name.l[0],1); \
    gSPLight(pkt,&name.l[1],2); \
    gSPLight(pkt,&name.l[2],3); \
    gSPLight(pkt,&name.l[3],4); \
    gSPLight(pkt,&name.l[4],5); \
    gSPLight(pkt,&name.l[5],6); \
    gSPLight(pkt,&name.l[6],7); \
    gSPLight(pkt,&name.a,8); \
}

#define gsSPSetLights7(name) \
    gsSPNumLights(NUMLIGHTS_7), \
    gsSPLight(&name.l[0],1), \
    gsSPLight(&name.l[1],2), \
    gsSPLight(&name.l[2],3), \
    gsSPLight(&name.l[3],4), \
    gsSPLight(&name.l[4],5), \
    gsSPLight(&name.l[5],6), \
    gsSPLight(&name.l[6],7), \
    gsSPLight(&name.a,8)

#define gSPLookAtX(pkt, l)  gDma2p((pkt),G_MOVEMEM,(l),sizeof(Light),G_MV_LIGHT,G_MVO_LOOKATX)
#define gsSPLookAtX(l)      gsDma2p(     G_MOVEMEM,(l),sizeof(Light),G_MV_LIGHT,G_MVO_LOOKATX)
#define gSPLookAtY(pkt, l)  gDma2p((pkt),G_MOVEMEM,(l),sizeof(Light),G_MV_LIGHT,G_MVO_LOOKATY)
#define gsSPLookAtY(l)      gsDma2p(     G_MOVEMEM,(l),sizeof(Light),G_MV_LIGHT,G_MVO_LOOKATY)

#define gSPLookAt(pkt, la) \
{ \
    gSPLookAtX(pkt,la) \
    gSPLookAtY(pkt,(s8 *)(la)+16) \
}

#define gsSPLookAt(la) \
    gsSPLookAtX(la), \
    gsSPLookAtY((s8 *)(la)+16)

#define gDPSetHilite1Tile(pkt, tile, hilite, width, height) \
    gDPSetTileSize(pkt, tile, (hilite)->h.x1 & 0xfff, (hilite)->h.y1 & 0xfff, \
        ((((width)-1)*4)+(hilite)->h.x1) & 0xfff, ((((height)-1)*4)+(hilite)->h.y1) & 0xfff)

#define gDPSetHilite2Tile(pkt, tile, hilite, width, height) \
    gDPSetTileSize(pkt, tile, (hilite)->h.x2 & 0xfff, (hilite)->h.y2 & 0xfff, \
        ((((width)-1)*4)+(hilite)->h.x2) & 0xfff, ((((height)-1)*4)+(hilite)->h.y2) & 0xfff)

#define gSPFogFactor(pkt, fm, fo) \
    gMoveWd(pkt, G_MW_FOG, G_MWO_FOG, \
        (_SHIFTL(fm,16,16) | _SHIFTL(fo,0,16)))

#define gsSPFogFactor(fm, fo) \
    gsMoveWd(G_MW_FOG, G_MWO_FOG, \
        (_SHIFTL(fm,16,16) | _SHIFTL(fo,0,16)))

#define gSPFogPosition(pkt, min, max) \
    gMoveWd(pkt, G_MW_FOG, G_MWO_FOG, \
        (_SHIFTL((128000/((max)-(min))),16,16) | \
        _SHIFTL(((500-(min))*256/((max)-(min))),0,16)))

#define gsSPFogPosition(min, max) \
    gsMoveWd(G_MW_FOG, G_MWO_FOG, \
        (_SHIFTL((128000/((max)-(min))),16,16) | \
        _SHIFTL(((500-(min))*256/((max)-(min))),0,16)))

#define gSPTexture(pkt, s, t, level, tile, on) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXTURE,24,8) | _SHIFTL(0,16,8) | _SHIFTL((level),11,3) | _SHIFTL((tile),8,3) | _SHIFTL((on),1,7)); \
    _g->words.w1 = (_SHIFTL((s),16,16) | _SHIFTL((t),0,16)); \
}

#define gsSPTexture(s, t, level, tile, on) \
{{ \
    (_SHIFTL(G_TEXTURE,24,8) | _SHIFTL(0,16,8) | _SHIFTL((level),11,3) | _SHIFTL((tile),8,3) | _SHIFTL((on),1,7)), \
    (_SHIFTL((s),16,16) | _SHIFTL((t),0,16)) \
}}

#define gSPTextureL(pkt, s, t, level, xparam, tile, on) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXTURE,24,8) | _SHIFTL((xparam),16,8) | _SHIFTL((level),11,3) | _SHIFTL((tile),8,3) | _SHIFTL((on),1,7)); \
    _g->words.w1 = (_SHIFTL((s),16,16) | _SHIFTL((t),0,16)); \
}
#define gsSPTextureL(s, t, level, xparam, tile, on) \
{{ \
    (_SHIFTL(G_TEXTURE,24,8) | _SHIFTL((xparam),16,8) | _SHIFTL((level),11,3) | _SHIFTL((tile),8,3) | _SHIFTL((on),1,7)), \
    (_SHIFTL((s),16,16) | _SHIFTL((t),0,16)) \
}}

#define gSPPerspNormalize(pkt, s)   gMoveWd(pkt, G_MW_PERSPNORM, 0, (s))
#define gsSPPerspNormalize(s)       gsMoveWd(    G_MW_PERSPNORM, 0, (s))
#define gSPPopMatrixN(pkt, n, num)  gDma2p((pkt),G_POPMTX,(num)*64,64,2,0)
#define gsSPPopMatrixN(n, num)      gsDma2p(     G_POPMTX,(num)*64,64,2,0)
#define gSPPopMatrix(pkt, n)        gSPPopMatrixN((pkt), (n), 1)
#define gsSPPopMatrix(n)            gsSPPopMatrixN(      (n), 1)

#define gSPEndDisplayList(pkt) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_ENDDL, 24, 8); \
    _g->words.w1 = 0; \
}

#define gsSPEndDisplayList() \
{{ \
    _SHIFTL(G_ENDDL, 24, 8), 0 \
}}

#define gSPGeometryMode(pkt, c, s) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_GEOMETRYMODE,24,8) | _SHIFTL(~(u32)(c),0,24);\
    _g->words.w1 = (u32)(s); \
}

#define gsSPGeometryMode(c, s) \
{{ \
    (_SHIFTL(G_GEOMETRYMODE,24,8) | _SHIFTL(~(u32)(c),0,24)),(u32)(s) \
}}

#define gSPSetGeometryMode(pkt, word)       gSPGeometryMode((pkt),0,(word))
#define gsSPSetGeometryMode(word)           gsSPGeometryMode(0,(word))
#define gSPClearGeometryMode(pkt, word)     gSPGeometryMode((pkt),(word),0)
#define gsSPClearGeometryMode(word)         gsSPGeometryMode((word),0)
#define gSPLoadGeometryMode(pkt, word)      gSPGeometryMode((pkt),-1,(word))
#define gsSPLoadGeometryMode(word)          gsSPGeometryMode(-1,(word))
#define gsSPGeometryModeSetFirst(c, s)      gsSPGeometryMode(c, s)

#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(cmd,24,8) | _SHIFTL(32-(sft)-(len),8,8) | _SHIFTL((len)-1,0,8)); \
    _g->words.w1 = (u32)(data); \
}

#define gsSPSetOtherMode(cmd, sft, len, data) \
{{ \
    _SHIFTL(cmd,24,8) | _SHIFTL(32-(sft)-(len),8,8) | _SHIFTL((len)-1,0,8), (u32)(data) \
}}

#define gDPPipelineMode(pkt, mode)          gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_PIPELINE, 1, mode)
#define gsDPPipelineMode(mode)              gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_PIPELINE, 1, mode)
#define gDPSetCycleType(pkt, type)          gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_CYCLETYPE, 2, type)
#define gsDPSetCycleType(type)              gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_CYCLETYPE, 2, type)
#define gDPSetTexturePersp(pkt, type)       gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTPERSP, 1, type)
#define gsDPSetTexturePersp(type)           gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTPERSP, 1, type)
#define gDPSetTextureDetail(pkt, type)      gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTDETAIL, 2, type)
#define gsDPSetTextureDetail(type)          gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTDETAIL, 2, type)
#define gDPSetTextureLOD(pkt, type)         gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTLOD, 1, type)
#define gsDPSetTextureLOD(type)             gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTLOD, 1, type)
#define gDPSetTextureLUT(pkt, type)         gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTLUT, 2, type)
#define gsDPSetTextureLUT(type)             gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTLUT, 2, type)
#define gDPSetTextureFilter(pkt, type)      gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTFILT, 2, type)
#define gsDPSetTextureFilter(type)          gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTFILT, 2, type)
#define gDPSetTextureConvert(pkt, type)     gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTCONV, 3, type)
#define gsDPSetTextureConvert(type)         gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTCONV, 3, type)
#define gDPSetCombineKey(pkt, type)         gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_COMBKEY, 1, type)
#define gsDPSetCombineKey(type)             gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_COMBKEY, 1, type)
#define gDPSetColorDither(pkt, mode)        gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_RGBDITHER, 2, mode)
#define gsDPSetColorDither(mode)            gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_RGBDITHER, 2, mode)
#define gDPSetAlphaDither(pkt, mode)        gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_ALPHADITHER, 2, mode)
#define gsDPSetAlphaDither(mode)            gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_ALPHADITHER, 2, mode)
#define gDPSetAlphaCompare(pkt, type)       gSPSetOtherMode(pkt, G_SETOTHERMODE_L, G_MDSFT_ALPHACOMPARE, 2, type)
#define gsDPSetAlphaCompare(type)           gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ALPHACOMPARE, 2, type)
#define gDPSetDepthSource(pkt, src)         gSPSetOtherMode(pkt, G_SETOTHERMODE_L, G_MDSFT_ZSRCSEL, 1, src)
#define gsDPSetDepthSource(src)             gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ZSRCSEL, 1, src)
#define gDPSetRenderMode(pkt, c0, c1)       gSPSetOtherMode(pkt, G_SETOTHERMODE_L, G_MDSFT_RENDERMODE, 29, (c0) | (c1))
#define gsDPSetRenderMode(c0, c1)           gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_RENDERMODE, 29, (c0) | (c1))

#define gSetImage(pkt, cmd, fmt, siz, width, i) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width)-1, 0, 12); \
    _g->words.w1 = (uintptr_t)(i); \
}

#define gsSetImage(cmd, fmt, siz, width, i) \
{{ \
    _SHIFTL(cmd, 24, 8) | _SHIFTL(fmt, 21, 3) | \
    _SHIFTL(siz, 19, 2) | _SHIFTL((width)-1, 0, 12), \
    (uintptr_t)(i) \
}}

#define gDPSetColorImage(pkt, f, s, w, i)   gSetImage(pkt, G_SETCIMG, f, s, w, i)
#define gsDPSetColorImage(f, s, w, i)       gsSetImage(G_SETCIMG, f, s, w, i)
#define gDPSetDepthImage(pkt, i)            gSetImage(pkt, G_SETZIMG, 0, 0, 1, i)
#define gsDPSetDepthImage(i)                gsSetImage(G_SETZIMG, 0, 0, 1, i)
#define gDPSetMaskImage(pkt, i)             gDPSetDepthImage(pkt, i)
#define gsDPSetMaskImage(i)                 gsDPSetDepthImage(i)
#define gDPSetTextureImage(pkt, f, s, w, i) gSetImage(pkt, G_SETTIMG, f, s, w, i)
#define gsDPSetTextureImage(f, s, w, i)     gsSetImage(G_SETTIMG, f, s, w, i)

#define gDPSetCombine(pkt, muxs0, muxs1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_SETCOMBINE, 24, 8) | _SHIFTL(muxs0, 0, 24);\
    _g->words.w1 = (u32)(muxs1); \
}

#define gsDPSetCombine(muxs0, muxs1) \
{{ \
    _SHIFTL(G_SETCOMBINE, 24, 8) | _SHIFTL(muxs0, 0, 24), (u32)(muxs1) \
}}

#define GCCc0w0(saRGB0, mRGB0, saA0, mA0) \
    (_SHIFTL((saRGB0), 20, 4) | _SHIFTL((mRGB0), 15, 5) | \
     _SHIFTL((saA0), 12, 3) | _SHIFTL((mA0), 9, 3))

#define GCCc1w0(saRGB1, mRGB1) \
    (_SHIFTL((saRGB1), 5, 4) | _SHIFTL((mRGB1), 0, 5))

#define GCCc0w1(sbRGB0, aRGB0, sbA0, aA0) \
    (_SHIFTL((sbRGB0), 28, 4) | _SHIFTL((aRGB0), 15, 3) | \
     _SHIFTL((sbA0), 12, 3) | _SHIFTL((aA0), 9, 3))

#define GCCc1w1(sbRGB1, saA1, mA1, aRGB1, sbA1, aA1) \
    (_SHIFTL((sbRGB1), 24, 4) | _SHIFTL((saA1), 21, 3) | \
     _SHIFTL((mA1), 18, 3) | _SHIFTL((aRGB1), 6, 3) | \
     _SHIFTL((sbA1), 3, 3) | _SHIFTL((aA1), 0, 3))

#define gDPSetCombineLERP(pkt, a0, b0, c0, d0, Aa0, Ab0, Ac0, Ad0, a1, b1, c1, d1, Aa1, Ab1, Ac1, Ad1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = \
        _SHIFTL(G_SETCOMBINE, 24, 8) | \
        _SHIFTL(GCCc0w0(G_CCMUX_##a0, G_CCMUX_##c0, G_ACMUX_##Aa0, G_ACMUX_##Ac0) | \
                GCCc1w0(G_CCMUX_##a1, G_CCMUX_##c1), 0, 24); \
    _g->words.w1 = \
         (u32) (GCCc0w1(G_CCMUX_##b0, G_CCMUX_##d0, G_ACMUX_##Ab0, G_ACMUX_##Ad0) | \
                GCCc1w1(G_CCMUX_##b1, G_ACMUX_##Aa1, G_ACMUX_##Ac1, G_CCMUX_##d1, \
                        G_ACMUX_##Ab1, G_ACMUX_##Ad1)); \
}

#define gsDPSetCombineLERP(a0, b0, c0, d0, Aa0, Ab0, Ac0, Ad0, a1, b1, c1, d1, Aa1, Ab1, Ac1, Ad1) \
{{ \
    _SHIFTL(G_SETCOMBINE, 24, 8) | \
    _SHIFTL(GCCc0w0(G_CCMUX_##a0, G_CCMUX_##c0, G_ACMUX_##Aa0, G_ACMUX_##Ac0) | \
            GCCc1w0(G_CCMUX_##a1, G_CCMUX_##c1), 0, 24), \
     (u32) (GCCc0w1(G_CCMUX_##b0, G_CCMUX_##d0, G_ACMUX_##Ab0, G_ACMUX_##Ad0) | \
            GCCc1w1(G_CCMUX_##b1, G_ACMUX_##Aa1, G_ACMUX_##Ac1, G_CCMUX_##d1, \
                    G_ACMUX_##Ab1, G_ACMUX_##Ad1)) \
}}

#define gDPSetCombineMode(pkt, a, b)    gDPSetCombineLERP(pkt, a, b)
#define gsDPSetCombineMode(a, b)        gsDPSetCombineLERP(a, b)

#define gDPSetColor(pkt, c, d) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(c, 24, 8); \
    _g->words.w1 = (u32)(d); \
}

#define gsDPSetColor(c, d) \
{{ \
    _SHIFTL(c, 24, 8), (u32)(d) \
}}

#define DPRGBColor(pkt, cmd, r, g, b, a)    gDPSetColor(pkt, cmd, (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)))
#define sDPRGBColor(cmd, r, g, b, a)        gsDPSetColor(cmd, (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)))
#define gDPSetEnvColor(pkt, r, g, b, a)     DPRGBColor(pkt, G_SETENVCOLOR, r,g,b,a)
#define gsDPSetEnvColor(r, g, b, a)         sDPRGBColor(G_SETENVCOLOR, r,g,b,a)
#define gDPSetBlendColor(pkt, r, g, b, a)   DPRGBColor(pkt, G_SETBLENDCOLOR, r,g,b,a)
#define gsDPSetBlendColor(r, g, b, a)       sDPRGBColor(G_SETBLENDCOLOR, r,g,b,a)
#define gDPSetFogColor(pkt, r, g, b, a)     DPRGBColor(pkt, G_SETFOGCOLOR, r,g,b,a)
#define gsDPSetFogColor(r, g, b, a)         sDPRGBColor(G_SETFOGCOLOR, r,g,b,a)
#define gDPSetFillColor(pkt, d)             gDPSetColor(pkt, G_SETFILLCOLOR, (d))
#define gsDPSetFillColor(d)                 gsDPSetColor(G_SETFILLCOLOR, (d))
#define gDPSetPrimDepth(pkt, z, dz)         gDPSetColor(pkt, G_SETPRIMDEPTH, _SHIFTL(z, 16, 16) | _SHIFTL(dz, 0, 16))
#define gsDPSetPrimDepth(z, dz)             gsDPSetColor(G_SETPRIMDEPTH, _SHIFTL(z, 16, 16) | _SHIFTL(dz, 0, 16))

#define gDPSetPrimColor(pkt, m, l, r, g, b, a) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 =    (_SHIFTL(G_SETPRIMCOLOR, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
    _g->words.w1 =  (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); \
}

#define gsDPSetPrimColor(m, l, r, g, b, a) \
{{ \
    (_SHIFTL(G_SETPRIMCOLOR, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)), \
    (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)) \
}}

#define gDPSetOtherMode(pkt, mode0, mode1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_RDPSETOTHERMODE,24,8) | _SHIFTL(mode0,0,24);\
    _g->words.w1 = (u32)(mode1); \
}

#define gsDPSetOtherMode(mode0, mode1) \
{{ \
    _SHIFTL(G_RDPSETOTHERMODE,24,8) | _SHIFTL(mode0,0,24), (u32)(mode1) \
}}

#define G_TX_LOADTILE               7
#define G_TX_RENDERTILE             0
#define G_TX_NOMIRROR               0
#define G_TX_WRAP                   0
#define G_TX_MIRROR                 0x1
#define G_TX_CLAMP                  0x2
#define G_TX_NOMASK                 0
#define G_TX_NOLOD                  0
#define G_TX_DXT_FRAC               11
#define G_TX_LDBLK_MAX_TXL          2047

#define TXL2WORDS(txls, b_txl)      MAX(1, ((txls)*(b_txl)/8))
#define CALC_DXT(width, b_txl)      (((1 << G_TX_DXT_FRAC) + TXL2WORDS(width, b_txl) - 1) / TXL2WORDS(width, b_txl))
#define TXL2WORDS_4b(txls)          MAX(1, ((txls)/16))
#define CALC_DXT_4b(width)          (((1 << G_TX_DXT_FRAC) + TXL2WORDS_4b(width) - 1) / TXL2WORDS_4b(width))

#define gDPLoadTileGeneric(pkt, c, tile, uls, ult, lrs, lrt) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(c, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12); \
    _g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12); \
}

#define gsDPLoadTileGeneric(c, tile, uls, ult, lrs, lrt) \
{{ \
    _SHIFTL(c, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12), \
    _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12)\
}}

#define gDPSetTileSize(pkt, t, uls, ult, lrs, lrt)      gDPLoadTileGeneric(pkt, G_SETTILESIZE, t, uls, ult, lrs, lrt)
#define gsDPSetTileSize(t, uls, ult, lrs, lrt)          gsDPLoadTileGeneric(G_SETTILESIZE, t, uls, ult, lrs, lrt)
#define gDPLoadTile(pkt, t, uls, ult, lrs, lrt)         gDPLoadTileGeneric(pkt, G_LOADTILE, t, uls, ult, lrs, lrt)
#define gsDPLoadTile(t, uls, ult, lrs, lrt)             gsDPLoadTileGeneric(G_LOADTILE, t, uls, ult, lrs, lrt)

#define gDPSetTile(pkt, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = \
        _SHIFTL(G_SETTILE, 24, 8) | \
        _SHIFTL(fmt, 21, 3) | \
        _SHIFTL(siz, 19, 2) | \
        _SHIFTL(line, 9, 9) | \
        _SHIFTL(tmem, 0, 9); \
    _g->words.w1 = \
        _SHIFTL(tile, 24, 3) | \
        _SHIFTL(palette, 20, 4) | \
        _SHIFTL(cmt, 18, 2) | \
        _SHIFTL(maskt, 14, 4) | \
        _SHIFTL(shiftt, 10, 4) | \
        _SHIFTL(cms, 8, 2) | \
        _SHIFTL(masks, 4, 4) | \
        _SHIFTL(shifts, 0, 4); \
}

#define gsDPSetTile(fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts) \
{{ \
    (_SHIFTL(G_SETTILE, 24, 8) | \
     _SHIFTL(fmt, 21, 3) | \
     _SHIFTL(siz, 19, 2) | \
     _SHIFTL(line, 9, 9) | \
     _SHIFTL(tmem, 0, 9)), \
    (_SHIFTL(tile, 24, 3) | \
     _SHIFTL(palette, 20, 4) | \
     _SHIFTL(cmt, 18, 2) | \
     _SHIFTL(maskt, 14, 4) | \
     _SHIFTL(shiftt, 10, 4) | \
     _SHIFTL(cms, 8, 2) | \
     _SHIFTL(masks, 4, 4) | \
     _SHIFTL(shifts, 0, 4)) \
}}

#define gDPLoadBlock(pkt, tile, uls, ult, lrs, dxt) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = ( \
        _SHIFTL(G_LOADBLOCK, 24, 8) | \
        _SHIFTL(uls, 12, 12) | \
        _SHIFTL(ult, 0, 12)); \
    _g->words.w1 = ( \
        _SHIFTL(tile, 24, 3) | \
        _SHIFTL((MIN(lrs,G_TX_LDBLK_MAX_TXL)), 12, 12) | \
        _SHIFTL(dxt, 0, 12)); \
}

#define gsDPLoadBlock(tile, uls, ult, lrs, dxt) \
{{ \
    (_SHIFTL(G_LOADBLOCK, 24, 8) | \
     _SHIFTL(uls, 12, 12) | \
     _SHIFTL(ult, 0, 12)), \
    (_SHIFTL(tile, 24, 3) | \
     _SHIFTL((MIN(lrs,G_TX_LDBLK_MAX_TXL)), 12, 12) | \
     _SHIFTL(dxt, 0, 12)) \
}}

#define gDPLoadTLUTCmd(pkt, tile, count) \
{ \
    Gfx *_g = (Gfx *)pkt; \
    _g->words.w0 = _SHIFTL(G_LOADTLUT, 24, 8); \
    _g->words.w1 = _SHIFTL((tile), 24, 3) | _SHIFTL((count), 14, 10);\
}

#define gsDPLoadTLUTCmd(tile, count) \
{{ \
    _SHIFTL(G_LOADTLUT, 24, 8), \
    _SHIFTL((tile), 24, 3) | _SHIFTL((count), 14, 10) \
}}

#define gDPLoadTextureBlock(pkt, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT) -1, CALC_DXT(width, siz##_BYTES)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES)+7)>>3, 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define gDPLoadTextureBlockYuv(pkt, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT) -1, CALC_DXT(width, siz##_BYTES)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * 1)+7)>>3, 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define gDPLoadTextureBlockS(pkt, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, cmt, maskt,    shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1,0); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES)+7)>>3, 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define gDPLoadMultiBlockS(pkt, timg, tmem, rtile, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt,    shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1,0); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES)+7)>>3, tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}


#define gDPLoadTextureBlockYuvS(pkt, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, cmt, maskt,    shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1,0); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * 1)+7)>>3, 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define _gDPLoadTextureBlock(pkt, timg, tmem, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, CALC_DXT(width, siz##_BYTES)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES)+7)>>3, tmem, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define _gDPLoadTextureBlockTile(pkt, timg, tmem, rtile, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, CALC_DXT(width, siz##_BYTES)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES)+7)>>3, tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define gDPLoadMultiBlock(pkt, timg, tmem, rtile, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, CALC_DXT(width, siz##_BYTES)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES)+7)>>3, tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define gsDPLoadTextureBlock(timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, siz##_LOAD_BLOCK, 1, timg), \
    gsDPSetTile(fmt, siz##_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, CALC_DXT(width, siz##_BYTES)), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, siz, ((((width) * siz##_LINE_BYTES)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define gsDPLoadTextureBlockS(timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, siz##_LOAD_BLOCK, 1, timg), \
    gsDPSetTile(fmt, siz##_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, 0 ),\
    gsDPPipeSync(), \
    gsDPSetTile(fmt, siz, ((((width) * siz##_LINE_BYTES)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define _gsDPLoadTextureBlock(timg, tmem, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, siz##_LOAD_BLOCK, 1, timg), \
    gsDPSetTile(fmt, siz##_LOAD_BLOCK, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, CALC_DXT(width, siz##_BYTES)), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, siz, ((((width) * siz##_LINE_BYTES)+7)>>3), tmem, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define _gsDPLoadTextureBlockTile(timg, tmem, rtile, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, siz##_LOAD_BLOCK, 1, timg), \
    gsDPSetTile(fmt, siz##_LOAD_BLOCK, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, CALC_DXT(width, siz##_BYTES)), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, siz, ((((width) * siz##_LINE_BYTES)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define gsDPLoadMultiBlock(timg, tmem, rtile, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, siz##_LOAD_BLOCK, 1, timg), \
    gsDPSetTile(fmt, siz##_LOAD_BLOCK, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, CALC_DXT(width, siz##_BYTES)), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, siz, ((((width) * siz##_LINE_BYTES)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define gsDPLoadMultiBlockS(timg, tmem, rtile, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, siz##_LOAD_BLOCK, 1, timg), \
    gsDPSetTile(fmt, siz##_LOAD_BLOCK, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, 0 ),\
    gsDPPipeSync(), \
    gsDPSetTile(fmt, siz, ((((width) * siz##_LINE_BYTES)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define gDPLoadTextureBlock_4b(pkt, timg, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_16b, 1, timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, CALC_DXT_4b(width)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define gDPLoadTextureBlock_4bS(pkt, timg, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_16b, 1, timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, 0 ); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define gDPLoadMultiBlock_4b(pkt, timg, tmem, rtile, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_16b, 1, timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_16b, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, CALC_DXT_4b(width)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define gDPLoadMultiBlock_4bS(pkt, timg, tmem, rtile, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_16b, 1, timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_16b, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, 0 ); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define _gDPLoadTextureBlock_4b(pkt, timg, tmem, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_16b, 1, timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_16b, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, CALC_DXT_4b(width)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), tmem, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}

#define gsDPLoadTextureBlock_4b(timg, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, G_IM_SIZ_16b, 1, timg), \
    gsDPSetTile(fmt, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, CALC_DXT_4b(width)), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define gsDPLoadTextureBlock_4bS(timg, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, G_IM_SIZ_16b, 1, timg), \
    gsDPSetTile(fmt, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1,0),\
    gsDPPipeSync(), \
    gsDPSetTile(fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define gsDPLoadMultiBlock_4b(timg, tmem, rtile, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, G_IM_SIZ_16b, 1, timg), \
    gsDPSetTile(fmt, G_IM_SIZ_16b, 0, tmem, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, CALC_DXT_4b(width)), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define gsDPLoadMultiBlock_4bS(timg, tmem, rtile, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, G_IM_SIZ_16b, 1, timg), \
    gsDPSetTile(fmt, G_IM_SIZ_16b, 0, tmem, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1,0),\
    gsDPPipeSync(), \
    gsDPSetTile(fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define _gsDPLoadTextureBlock_4b(timg, tmem, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, G_IM_SIZ_16b, 1, timg), \
    gsDPSetTile(fmt, G_IM_SIZ_16b, 0, tmem, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, CALC_DXT_4b(width)), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), tmem, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)

#define gDPLoadTextureTile(pkt, timg, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz, width, timg); \
    gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_TILE_BYTES)+7)>>3), 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadTile(    pkt, G_TX_LOADTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_LINE_BYTES)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC) \
}

#define gDPLoadMultiTile(pkt, timg, tmem, rtile, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz, width, timg); \
    gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_TILE_BYTES)+7)>>3), tmem, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadTile(    pkt, G_TX_LOADTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_LINE_BYTES)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, rtile, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC) \
}

#define gsDPLoadTextureTile(timg, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, siz, width, timg), \
    gsDPSetTile(fmt, siz, (((((lrs)-(uls)+1) * siz##_TILE_BYTES)+7)>>3), 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadTile(    G_TX_LOADTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, siz, (((((lrs)-(uls)+1) * siz##_LINE_BYTES)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks,     shifts), \
    gsDPSetTileSize(G_TX_RENDERTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC)

#define gsDPLoadMultiTile(timg, tmem, rtile, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, siz, width, timg), \
    gsDPSetTile(fmt, siz, (((((lrs)-(uls)+1) * siz##_TILE_BYTES)+7)>>3), tmem, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadTile(    G_TX_LOADTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, siz, (((((lrs)-(uls)+1) * siz##_LINE_BYTES)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(rtile, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC)

#define gDPLoadTextureTile_4b(pkt, timg, fmt, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_8b, ((width)>>1), timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_8b, (((((lrs)-(uls)+1)>>1)+7)>>3), 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadTile(    pkt, G_TX_LOADTILE, (uls)<<(G_TEXTURE_IMAGE_FRAC-1), (ult)<<(G_TEXTURE_IMAGE_FRAC), (lrs)<<(G_TEXTURE_IMAGE_FRAC-1), (lrt)<<(G_TEXTURE_IMAGE_FRAC)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, (((((lrs)-(uls)+1)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC) \
}

#define gDPLoadMultiTile_4b(pkt, timg, tmem, rtile, fmt, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_8b, ((width)>>1), timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_8b, (((((lrs)-(uls)+1)>>1)+7)>>3), tmem, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadTile(    pkt, G_TX_LOADTILE, (uls)<<(G_TEXTURE_IMAGE_FRAC-1), (ult)<<(G_TEXTURE_IMAGE_FRAC), (lrs)<<(G_TEXTURE_IMAGE_FRAC-1), (lrt)<<(G_TEXTURE_IMAGE_FRAC)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, (((((lrs)-(uls)+1)>>1)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, rtile, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC) \
}

#define gsDPLoadTextureTile_4b(timg, fmt, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, G_IM_SIZ_8b, ((width)>>1), timg), \
    gsDPSetTile(fmt, G_IM_SIZ_8b, (((((lrs)-(uls)+1)>>1)+7)>>3), 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadTile(    G_TX_LOADTILE, (uls)<<(G_TEXTURE_IMAGE_FRAC-1), (ult)<<(G_TEXTURE_IMAGE_FRAC), (lrs)<<(G_TEXTURE_IMAGE_FRAC-1), (lrt)<<(G_TEXTURE_IMAGE_FRAC)), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, G_IM_SIZ_4b, (((((lrs)-(uls)+1)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(G_TX_RENDERTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC)

#define gsDPLoadMultiTile_4b(timg, tmem, rtile, fmt, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    gsDPSetTextureImage(fmt, G_IM_SIZ_8b, ((width)>>1), timg), \
    gsDPSetTile(fmt, G_IM_SIZ_8b, (((((lrs)-(uls)+1)>>1)+7)>>3), tmem, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPLoadSync(), \
    gsDPLoadTile(    G_TX_LOADTILE, (uls)<<(G_TEXTURE_IMAGE_FRAC-1), (ult)<<(G_TEXTURE_IMAGE_FRAC), (lrs)<<(G_TEXTURE_IMAGE_FRAC-1), (lrt)<<(G_TEXTURE_IMAGE_FRAC)), \
    gsDPPipeSync(), \
    gsDPSetTile(fmt, G_IM_SIZ_4b, (((((lrs)-(uls)+1)>>1)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts), \
    gsDPSetTileSize(rtile, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC)

#define gDPLoadTLUT_pal16(pkt, pal, dram) \
{ \
    gDPSetTextureImage(pkt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, dram); \
    gDPTileSync(pkt); \
    gDPSetTile(pkt, 0, 0, 0, (256+(((pal)&0xf)*16)), G_TX_LOADTILE, 0 , 0, 0, 0, 0, 0, 0); \
    gDPLoadSync(pkt); \
    gDPLoadTLUTCmd(pkt, G_TX_LOADTILE, 15); \
    gDPPipeSync(pkt) \
}

#define gsDPLoadTLUT_pal16(pal, dram) \
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, dram), \
    gsDPTileSync(), \
    gsDPSetTile(0, 0, 0, (256+(((pal)&0xf)*16)), G_TX_LOADTILE, 0 , 0, 0, 0, 0, 0, 0), \
    gsDPLoadSync(), \
    gsDPLoadTLUTCmd(G_TX_LOADTILE, 15), \
    gsDPPipeSync()

#define gDPLoadTLUT_pal256(pkt, dram) \
{ \
    gDPSetTextureImage(pkt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, dram); \
    gDPTileSync(pkt); \
    gDPSetTile(pkt, 0, 0, 0, 256, G_TX_LOADTILE, 0 , 0, 0, 0, 0, 0, 0); \
    gDPLoadSync(pkt); \
    gDPLoadTLUTCmd(pkt, G_TX_LOADTILE, 255); \
    gDPPipeSync(pkt) \
}

#define gsDPLoadTLUT_pal256(dram) \
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, dram), \
    gsDPTileSync(), \
    gsDPSetTile(0, 0, 0, 256, G_TX_LOADTILE, 0 , 0, 0, 0, 0, 0, 0), \
    gsDPLoadSync(), \
    gsDPLoadTLUTCmd(G_TX_LOADTILE, 255), \
    gsDPPipeSync()

#define gDPLoadTLUT(pkt, count, tmemaddr, dram) \
{ \
    gDPSetTextureImage(pkt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, dram); \
    gDPTileSync(pkt); \
    gDPSetTile(pkt, 0, 0, 0, tmemaddr, G_TX_LOADTILE, 0 , 0, 0, 0, 0, 0, 0); \
    gDPLoadSync(pkt); \
    gDPLoadTLUTCmd(pkt, G_TX_LOADTILE, ((count)-1)); \
    gDPPipeSync(pkt); \
}

#define gsDPLoadTLUT(count, tmemaddr, dram) \
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, dram), \
    gsDPTileSync(), \
    gsDPSetTile(0, 0, 0, tmemaddr, G_TX_LOADTILE, 0 , 0, 0, 0, 0, 0, 0), \
    gsDPLoadSync(), \
    gsDPLoadTLUTCmd(G_TX_LOADTILE, ((count)-1)), \
    gsDPPipeSync()

#define gDPSetScissor(pkt, mode, ulx, uly, lrx, lry) \
{ \
    Gfx *_g = (Gfx *)pkt; \
    _g->words.w0 = _SHIFTL(G_SETSCISSOR, 24, 8) | _SHIFTL((s32)((f32)(ulx)*4.0F), 12, 12) | _SHIFTL((s32)((f32)(uly)*4.0F), 0, 12); \
    _g->words.w1 = _SHIFTL(mode, 24, 2) | _SHIFTL((s32)((f32)(lrx)*4.0F), 12, 12) | _SHIFTL((s32)((f32)(lry)*4.0F), 0, 12); \
}

#define gDPSetScissorFrac(pkt, mode, ulx, uly, lrx, lry) \
{ \
    Gfx *_g = (Gfx *)pkt; \
    _g->words.w0 = _SHIFTL(G_SETSCISSOR, 24, 8) | _SHIFTL((s32)((ulx)), 12, 12) | _SHIFTL((s32)((uly)), 0, 12); \
    _g->words.w1 = _SHIFTL(mode, 24, 2) | _SHIFTL((s32)((lrx)), 12, 12) | _SHIFTL((s32)((lry)), 0, 12); \
}

#define gsDPSetScissor(mode, ulx, uly, lrx, lry) \
{{ \
    _SHIFTL(G_SETSCISSOR, 24, 8) | \
    _SHIFTL((s32)((f32)(ulx)*4.0F), 12, 12) | \
    _SHIFTL((s32)((f32)(uly)*4.0F), 0, 12), \
    _SHIFTL(mode, 24, 2) | \
    _SHIFTL((s32)((f32)(lrx)*4.0F), 12, 12) | \
    _SHIFTL((s32)((f32)(lry)*4.0F), 0, 12) \
}}

#define gsDPSetScissorFrac(mode, ulx, uly, lrx, lry) \
{{ \
    _SHIFTL(G_SETSCISSOR, 24, 8) | \
    _SHIFTL((s32)((ulx)), 12, 12) | \
    _SHIFTL((s32)((uly)), 0, 12), \
    _SHIFTL(mode, 24, 2) | \
    _SHIFTL((s32)(lrx), 12, 12) | \
    _SHIFTL((s32)(lry), 0, 12) \
}}

#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) \
{ \
    Gfx *_g0 = (Gfx *)(pkt), *_g1 = (Gfx *)(pkt); \
    _g0->words.w0 = _SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL((lrx), 2, 22); \
    _g0->words.w1 = _SHIFTL((lry), 2, 22); \
    _g1->words.w0 = _SHIFTL((ulx), 2, 22); \
    _g1->words.w1 = _SHIFTL((uly), 2, 22); \
}

#define gsDPFillRectangle(ulx, uly, lrx, lry) \
{{ \
    (_SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL((lrx), 2, 22)), \
    _SHIFTL((lry), 2, 22), \
}}, \
{{ \
    _SHIFTL((ulx), 2, 22), \
    _SHIFTL((uly), 2, 22), \
}}

#define gDPSetConvert(pkt, k0, k1, k2, k3, k4, k5) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_SETCONVERT, 24, 8) | _SHIFTL(k0, 13, 9) | _SHIFTL(k1, 4, 9) | _SHIFTR(k2, 5, 4)); \
    _g->words.w1 = (_SHIFTL(k2, 27, 5) | _SHIFTL(k3, 18, 9) | _SHIFTL(k4, 9, 9) | _SHIFTL(k5, 0, 9)); \
}

#define gsDPSetConvert(k0, k1, k2, k3, k4, k5) \
{{ \
    (_SHIFTL(G_SETCONVERT, 24, 8) | \
     _SHIFTL(k0, 13, 9) | _SHIFTL(k1, 4, 9) | _SHIFTR(k2, 5, 4)), \
    (_SHIFTL(k2, 27, 5) | _SHIFTL(k3, 18, 9) | _SHIFTL(k4, 9, 9) | \
     _SHIFTL(k5, 0, 9)) \
}}

#define gDPSetKeyR(pkt, cR, sR, wR) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_SETKEYR, 24, 8); \
    _g->words.w1 = (_SHIFTL(wR, 16, 12) | _SHIFTL(cR, 8, 8) | _SHIFTL(sR, 0, 8)); \
}

#define gsDPSetKeyR(cR, sR, wR) \
{{ \
    _SHIFTL(G_SETKEYR, 24, 8), \
    _SHIFTL(wR, 16, 12) | _SHIFTL(cR, 8, 8) | _SHIFTL(sR, 0, 8) \
}}

#define gDPSetKeyGB(pkt, cG, sG, wG, cB, sB, wB) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_SETKEYGB, 24, 8) | _SHIFTL(wG, 12, 12) | _SHIFTL(wB, 0, 12)); \
    _g->words.w1 = (_SHIFTL(cG, 24, 8) | _SHIFTL(sG, 16, 8) |  _SHIFTL(cB, 8, 8) | _SHIFTL(sB, 0, 8)); \
}

#define gsDPSetKeyGB(cG, sG, wG, cB, sB, wB) \
{{ \
    (_SHIFTL(G_SETKEYGB, 24, 8) | _SHIFTL(wG, 12, 12) | \
     _SHIFTL(wB, 0, 12)), \
    (_SHIFTL(cG, 24, 8) | _SHIFTL(sG, 16, 8) | _SHIFTL(cB, 8, 8) | \
     _SHIFTL(sB, 0, 8)) \
}}

#define gDPNoParam(pkt, cmd) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8); \
    _g->words.w1 = 0; \
}

#define gsDPNoParam(cmd) \
{{ \
    _SHIFTL(cmd, 24, 8), 0 \
}}

#define gDPParam(pkt, cmd, param) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8); \
    _g->words.w1 = (param); \
}

#define gsDPParam(cmd, param) \
{{ \
    _SHIFTL(cmd, 24, 8), (param) \
}}

#define gsDPTextureRectangle(xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{{ \
    (_SHIFTL(G_TEXRECT, 24, 8) | \
     _SHIFTL(xh, 12, 12) | \
     _SHIFTL(yh, 0, 12)), \
    (_SHIFTL(tile, 24, 3) | \
     _SHIFTL(xl, 12, 12) | \
     _SHIFTL(yl, 0, 12)), \
}}, \
{{ \
    _SHIFTL(s, 16, 16) | \
    _SHIFTL(t, 0, 16), \
    _SHIFTL(dsdx, 16, 16) | \
    _SHIFTL(dtdy, 0, 16) \
}}

#define gDPTextureRectangle(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy)\
{ \
    Gfx *_g = (Gfx *)(pkt); \
    if (pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(xh, 12, 12) | _SHIFTL(yh, 0, 12)); \
    _g->words.w1 = (_SHIFTL(tile, 24, 3) | _SHIFTL(xl, 12, 12) | _SHIFTL(yl, 0, 12)); \
    _g ++; \
    _g->words.w0 = (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16)); \
    _g->words.w1 = (_SHIFTL(dsdx, 16, 16) | _SHIFTL(dtdy, 0, 16)); \
}

#define gsDPTextureRectangleFlip(xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{{ \
    (_SHIFTL(G_TEXRECTFLIP, 24, 8) | \
     _SHIFTL(xh, 12, 12) | \
     _SHIFTL(yh, 0, 12)), \
    (_SHIFTL(tile, 24, 3) | \
     _SHIFTL(xl, 12, 12) | \
     _SHIFTL(yl, 0, 12)), \
}}, \
{{ \
    _SHIFTL(s, 16, 16) | \
    _SHIFTL(t, 0, 16), \
    _SHIFTL(dsdx, 16, 16) | \
    _SHIFTL(dtdy, 0, 16) \
}}

#define gDPTextureRectangleFlip(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy)\
{ \
    Gfx *_g = (Gfx *)(pkt); \
    if (pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXRECTFLIP, 24, 8) | _SHIFTL(xh, 12, 12) | _SHIFTL(yh, 0, 12)); \
    _g->words.w1 = (_SHIFTL(tile, 24, 3) | _SHIFTL(xl, 12, 12) | _SHIFTL(yl, 0, 12)); \
    _g ++; \
    _g->words.w0 = (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16)); \
    _g->words.w1 = (_SHIFTL(dsdx, 16, 16) | _SHIFTL(dtdy, 0, 16)); \
}

#define gSPTextureRectangle(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy)\
{ \
    Gfx *_g0 = (Gfx *)(pkt), *_g1 = (Gfx *)(pkt), *_g2 = (Gfx *)(pkt); \
    _g0->words.w0 = _SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL((xh), 0, 24); \
    _g0->words.w1 = _SHIFTL((yh), 0, 24); \
    _g1->words.w0 = (_SHIFTL(tile, 24, 3) | _SHIFTL((xl), 0, 24)); \
    _g1->words.w1 = _SHIFTL((yl), 0, 24); \
    _g2->words.w0 = (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16)); \
    _g2->words.w1 = (_SHIFTL(dsdx, 16, 16) | _SHIFTL(dtdy, 0, 16)); \
}

#define gsSPTextureRectangle(xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{{ \
    (_SHIFTL(G_TEXRECT, 24, 8) | \
     _SHIFTL((xh), 0, 24)), \
     _SHIFTL((yh), 0, 24), \
}}, \
{{ \
    (_SHIFTL((tile), 24, 3) | \
     _SHIFTL((xl), 0, 24)), \
     _SHIFTL((yl), 0, 24), \
}}, \
{{ \
    _SHIFTL(s, 16, 16) | \
    _SHIFTL(t, 0, 16), \
    _SHIFTL(dsdx, 16, 16) | \
    _SHIFTL(dtdy, 0, 16) \
}}

#define gSPTextureRectangleFlip(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{ \
    Gfx *_g0 = (Gfx *)(pkt), *_g1 = (Gfx *)(pkt), *_g2 = (Gfx *)(pkt); \
    _g0->words.w0 = _SHIFTL(G_TEXRECTFLIP, 24, 8) | _SHIFTL((xh), 0, 24); \
    _g0->words.w1 = _SHIFTL((yh), 0, 24); \
    _g1->words.w0 = (_SHIFTL(tile, 24, 3) | _SHIFTL((xl), 0, 24)); \
    _g1->words.w1 = _SHIFTL((yl), 0, 24); \
    _g2->words.w0 = (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16)); \
    _g2->words.w1 = (_SHIFTL(dsdx, 16, 16) | _SHIFTL(dtdy, 0, 16)); \
}

#define gsDPWord(wordhi, wordlo) \
    gsImmp1(G_RDPHALF_1, (uintptr_t)(wordhi)), \
    gsImmp1(G_RDPHALF_2, (uintptr_t)(wordlo))

#define gDPWord(pkt, wordhi, wordlo) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    gImmp1(pkt, G_RDPHALF_1, (uintptr_t)(wordhi)); \
    gImmp1(pkt, G_RDPHALF_2, (uintptr_t)(wordlo)); \
}

#define gDPFullSync(pkt)        gDPNoParam(pkt, G_RDPFULLSYNC)
#define gsDPFullSync()          gsDPNoParam(G_RDPFULLSYNC)
#define gDPTileSync(pkt)        gDPNoParam(pkt, G_RDPTILESYNC)
#define gsDPTileSync()          gsDPNoParam(G_RDPTILESYNC)
#define gDPPipeSync(pkt)        gDPNoParam(pkt, G_RDPPIPESYNC)
#define gsDPPipeSync()          gsDPNoParam(G_RDPPIPESYNC)
#define gDPLoadSync(pkt)        gDPNoParam(pkt, G_RDPLOADSYNC)
#define gsDPLoadSync()          gsDPNoParam(G_RDPLOADSYNC)
#define gDPNoOp(pkt)            gDPNoParam(pkt, G_NOOP)
#define gsDPNoOp()              gsDPNoParam(G_NOOP)
#define gDPNoOpTag(pkt, tag)    gDPParam(pkt, G_NOOP, tag)
#define gsDPNoOpTag(tag)        gsDPParam(G_NOOP, tag)

#endif /* _LANGUAGE_C */
#endif /* _GBI_H_ */

#ifndef OMM_MACROS_H
#define OMM_MACROS_H
#undef USE_SYSTEM_MALLOC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <xmmintrin.h>

// Must be here, but can't use SM64 typenames
typedef struct { unsigned int ts; short v; } s16_ts;
typedef struct { unsigned int ts; int v; } s32_ts;
typedef struct { unsigned int ts; unsigned short v; } u16_ts;
typedef struct { unsigned int ts; unsigned int v; } u32_ts;
typedef struct { unsigned int ts; float v; } f32_ts;
typedef struct { unsigned int ts; float v[3]; } Vec3f_ts;
typedef struct { unsigned int ts; short v[3]; } Vec3s_ts;
typedef struct { unsigned int ts; float v[4][4]; } Mat4_ts;
typedef struct { unsigned int ts; void *v; } ptr_ts;

//
// str
//

#define str_rep(str, c0, c1) { \
    for (char *str_p = (str); (str_p = strchr(str_p, c0)) != NULL; *(str_p++) = c1); \
}

#define str_cpy(dst, dst_siz, src) { \
    const char *src_p = (src); \
    char *dst_p = (dst); \
    s32 dst_len = (s32) (dst_siz); \
    if (src_p && dst_p) { \
        s32 src_len = (s32) strlen(src_p); \
        memcpy(dst_p, src_p, min(src_len + 1, dst_len)); \
    } \
}

#define str_cat(dst, dst_siz, ...) { \
    char *dst_p = (dst); \
    s32 dst_len = (s32) (dst_siz); \
    if (dst_p && dst_len) { \
        const char *src_list[] = { __VA_ARGS__, NULL }; \
        for (const char **src_p = src_list; dst_len > 1 && *src_p; ++src_p) { \
            if (*src_p) { \
                s32 src_len = (s32) strlen(*src_p); \
                src_len = min(src_len, dst_len - 1); \
                if (src_len) { \
                    memcpy(dst_p, *src_p, src_len); \
                    dst_p += src_len; \
                    dst_len -= src_len; \
                } \
            } \
        } \
        *dst_p = 0; \
    } \
}

//
// Versions (auto-detected by omm.mk)
// SMEX : sm64ex-nightly
// SMMS : sm64ex-nightly + Moonshine
// R96X : Render96
// XALO : sm64ex-alo
// SM74 : Super Mario 74
// SMSR : Super Mario Star Road
//

#if   defined(SMEX)
#define DEF(smex, smms, r96x, xalo, sm74, smsr) smex
#elif defined(SMMS)
#define DEF(smex, smms, r96x, xalo, sm74, smsr) smms
#elif defined(R96X)
#define DEF(smex, smms, r96x, xalo, sm74, smsr) r96x
#elif defined(XALO)
#define DEF(smex, smms, r96x, xalo, sm74, smsr) xalo
#elif defined(SM74)
#define DEF(smex, smms, r96x, xalo, sm74, smsr) sm74
#elif defined(SMSR)
#define DEF(smex, smms, r96x, xalo, sm74, smsr) smsr
#else
#error "No version defined!"
#endif

//
// OMM macros
// Depending on which repo/conversion/hack we are building the game,
// some code must be enabled or disabled for OMM to work properly.
// The macros OMM_BOWSER, OMM_DEBUG and OMM_DEV can be passed to the make
// command with values 0 or 1 to enable or disable the corresponding code.
//

// OMM_GAME_* | Attributes a numeric value to every game version/type/save/mode
#define OMM_GAME_SM64           0
#define OMM_GAME_SMEX           0
#define OMM_GAME_SMMS           1
#define OMM_GAME_R96X           2
#define OMM_GAME_XALO           3
#define OMM_GAME_SM74           4
#define OMM_GAME_SMSR           5
#define OMM_GAME_VERSION        DEF(OMM_GAME_SMEX, OMM_GAME_SMMS, OMM_GAME_R96X, OMM_GAME_XALO, OMM_GAME_SM74, OMM_GAME_SMSR)
#define OMM_GAME_TYPE           DEF(OMM_GAME_SM64, OMM_GAME_SMMS, OMM_GAME_SM64, OMM_GAME_SM64, OMM_GAME_SM74, OMM_GAME_SMSR)
#define OMM_GAME_SAVE           DEF(OMM_GAME_SM64, OMM_GAME_SMMS, OMM_GAME_R96X, OMM_GAME_SM64, OMM_GAME_SM74, OMM_GAME_SMSR)
#define OMM_GAME_MODE           DEF(0, 0, 0, 0, sm74_mode__omm_save, 0)
#define OMM_GAME_IS_SM64        (OMM_GAME_TYPE == OMM_GAME_SM64)
#define OMM_GAME_IS_SMEX        (OMM_GAME_VERSION == OMM_GAME_SMEX)
#define OMM_GAME_IS_SMMS        (OMM_GAME_VERSION == OMM_GAME_SMMS)
#define OMM_GAME_IS_R96X        (OMM_GAME_VERSION == OMM_GAME_R96X)
#define OMM_GAME_IS_XALO        (OMM_GAME_VERSION == OMM_GAME_XALO)
#define OMM_GAME_IS_SM74        (OMM_GAME_VERSION == OMM_GAME_SM74)
#define OMM_GAME_IS_SMSR        (OMM_GAME_VERSION == OMM_GAME_SMSR)

// OMM_BOWSER | Replaces Vanilla Bowser with OMM Bowser
#if defined(OMM_BOWSER)
#define OMM_CODE_BOWSER         DEF(OMM_BOWSER, OMM_BOWSER, OMM_BOWSER, OMM_BOWSER, OMM_BOWSER, OMM_BOWSER)
#else
#define OMM_CODE_BOWSER         DEF(1, 0, 1, 1, 0, 0)
#endif

// OMM_DEBUG | Enables some debug stuff
#if defined(OMM_DEBUG)
#define OMM_CODE_DEBUG          DEF(OMM_DEBUG, OMM_DEBUG, OMM_DEBUG, OMM_DEBUG, OMM_DEBUG, OMM_DEBUG)
#else
#define OMM_CODE_DEBUG          DEF(0, 0, 0, 0, 0, 0)
#endif

// OMM_DEV | If set, enables super secret features (dev branch only)
#if defined(OMM_DEV)
#define OMM_CODE_DEV            DEF(OMM_DEV, OMM_DEV, OMM_DEV, OMM_DEV, OMM_DEV, OMM_DEV)
#else
#define OMM_CODE_DEV            DEF(0, 0, 0, 0, 0, 0)
#endif

// EXT_OPTIONS_MENU | If set, enables options types implementation in file omm_options.c
#if defined(EXT_OPTIONS_MENU)
#define OMM_OPT_TYPES_IMPL      DEF(1, 1, 1, 0, 0, 0)
#else
#define OMM_OPT_TYPES_IMPL      DEF(0, 0, 0, 0, 0, 0)
#endif

// Render API
#if defined(RAPI_GL) || defined(RAPI_GL_LEGACY)
#define OMM_RAPI_GL             1
#define OMM_RAPI_D3D            0
#elif defined(RAPI_D3D11) || defined(RAPI_D3D12)
#define OMM_RAPI_GL             0
#define OMM_RAPI_D3D            1
#else
#error "Unknown Render API"
#endif

// Window API
#if defined(WAPI_SDL1) || defined(WAPI_SDL2)
#define OMM_WAPI_SDL            1
#define OMM_WAPI_DXGI           0
#elif defined(WAPI_DXGI)
#define OMM_WAPI_SDL            0
#define OMM_WAPI_DXGI           1
#else
#error "Unknown Window API"
#endif

// Windows build | Some features are exclusive to the Windows OS
#if defined(_WIN32)
#define WINDOWS_BUILD           1
#else
#define WINDOWS_BUILD           0
#endif

//
// Macros
//

#if   defined(SMEX)
#include "omm_macros_smex.h"
#elif defined(SMMS)
#include "omm_macros_smms.h"
#elif defined(R96X)
#include "omm_macros_r96x.h"
#elif defined(XALO)
#include "omm_macros_xalo.h"
#elif defined(SM74)
#include "omm_macros_sm74.h"
#elif defined(SMSR)
#include "omm_macros_smsr.h"
#endif
#define X_BUTTON 0x0040
#define Y_BUTTON 0x0080

//
// DynOS defines
// OMM has new models that must be added to DynOS actors list
//

#if defined(DYNOS) && defined(DYNOS_CPP_H)
#include "object/omm_object_data.h"
#endif

//
// Some useful macros
// (don't use the macros starting with underscores)
//

// Count the number of args inside a variadic macro (up to 8)
#ifdef _MSC_VER
#define N_ARGS(...) __EXPAND_ARGS(__ARGS_AUG(__VA_ARGS__))
#define __ARGS_AUG(...) unused, __VA_ARGS__
#define __EXPAND(...) __VA_ARGS__
#define __EXPAND_ARGS(...) __EXPAND(__COUNT_ARGS(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0))
#define __COUNT_ARGS(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, count, ...) count
#else
#define N_ARGS(...) __COUNT_ARGS(0, ##__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __EXPAND(...) __VA_ARGS__
#define __COUNT_ARGS(_0_, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, count, ...) count
#endif

// Turn things into a string
#undef __STRINGIFY
#undef STRINGIFY
#define __STRINGIFY(...) #__VA_ARGS__
#define STRINGIFY(...) __STRINGIFY(__VA_ARGS__)

// Concatenate two macros
#undef __CAT
#undef CAT
#define __CAT(A, B) A##B
#define CAT(A, B) __CAT(A, B)

// Unpack macro arguments
#undef UNPACK
#define UNPACK(macro, ...) macro(__VA_ARGS__)

#endif

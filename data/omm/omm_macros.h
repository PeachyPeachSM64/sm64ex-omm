#ifndef OMM_MACROS_H
#define OMM_MACROS_H
#undef USE_SYSTEM_MALLOC

#include <ctype.h>
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

#if defined(r96x)
#define omm_GraphNode_extra_fields \
bool noBillboard;
#else
#define omm_GraphNode_extra_fields \
const void *georef; \
bool noBillboard;
#endif

#define omm_AnimInfo_extra_fields \
const void *curAnimRef; \
s16 animFlags; \
s16_ts _animID; \
ptr_ts _curAnim; \
s16_ts _animFrame;

#define omm_GraphNodeObject_extra_fields \
Vec3s_ts _angle; \
Vec3f_ts _pos; \
Vec3f_ts _scale; \
Vec3f_ts _objPos; \
Vec3f_ts _shadowPos; \
f32_ts _shadowScale; \
Mat4_ts _throwMatrix;

//
// Games (auto-detected by omm.mk)
// smex : sm64ex-nightly
// r96x : Render96
// smms : Super Mario 64 Moonshine
// sm74 : Super Mario 74
// smsr : Super Mario Star Road
// smgs : Super Mario 64: The Green Stars
//

#define OMM_GAME_SM64 0
#define OMM_GAME_SMEX 0
#define OMM_GAME_R96X 1
#define OMM_GAME_SMMS 2
#define OMM_GAME_SM74 3
#define OMM_GAME_SMSR 4
#define OMM_GAME_SMGS 5
#include "omm_games.inl"
#if   defined(smex)
#include "omm_macros_smex.h"
#elif defined(smms)
#include "omm_macros_smms.h"
#elif defined(r96x)
#include "omm_macros_r96x.h"
#elif defined(sm74)
#include "omm_macros_sm74.h"
#elif defined(smsr)
#include "omm_macros_smsr.h"
#elif defined(smgs)
#include "omm_macros_smgs.h"
#endif
#define OMM_GAME_IS_SMEX (OMM_GAME_SAVE == OMM_GAME_SMEX)
#define OMM_GAME_IS_R96X (OMM_GAME_SAVE == OMM_GAME_R96X)
#define OMM_GAME_IS_SMMS (OMM_GAME_SAVE == OMM_GAME_SMMS)
#define OMM_GAME_IS_SM74 (OMM_GAME_SAVE == OMM_GAME_SM74)
#define OMM_GAME_IS_SMSR (OMM_GAME_SAVE == OMM_GAME_SMSR)
#define OMM_GAME_IS_SMGS (OMM_GAME_SAVE == OMM_GAME_SMGS)
#define OMM_GAME_IS_SM64 (OMM_GAME_TYPE == OMM_GAME_SM64) // Vanilla Super Mario 64
#define OMM_GAME_IS_RF14 (OMM_GAME_RF14) // Refresh 14+ code
#if !OMM_GAME_IS_SM74
#define MODE_INDEX UNUSED
#else
#define MODE_INDEX
#endif

// Buttons
#define X_BUTTON 0x0040
#define Y_BUTTON 0x0080
#define SPIN_BUTTON 0x100000

// OMM_DEBUG | Enables some debug stuff
#if defined(OMM_DEBUG)
#define OMM_CODE_DEBUG OMM_DEBUG
#else
#define OMM_CODE_DEBUG 0
#endif
#if OMM_CODE_DEBUG
#define DEBUG_ONLY
#else
#define DEBUG_ONLY UNUSED
#endif

// OMM_DEV | If set, enables super secret features (dev branch only)
#if defined(OMM_DEV)
#define OMM_CODE_DEV OMM_DEV
#else
#define OMM_CODE_DEV 0
#endif

// Gfx API
#if defined(RAPI_GL) || defined(RAPI_GL_LEGACY)
#if defined(WAPI_SDL1) || defined(WAPI_SDL2)
#define OMM_GFX_API_GL 1
#define OMM_GFX_API_DX 0
#else
#error "Cannot mix OpenGL render API with DirectX window API"
#endif
#elif defined(RAPI_D3D11) || defined(RAPI_D3D12)
#if defined(WAPI_DXGI)
#define OMM_GFX_API_GL 0
#define OMM_GFX_API_DX 1
#else
#error "Cannot mix DirectX render API with SDL window API"
#endif
#else
#error "Unknown graphics API"
#endif

// Windows build | Some features are exclusive to the Windows OS
#if defined(_WIN32)
#define WINDOWS_BUILD 1
#else
#define WINDOWS_BUILD 0
#endif

// Optimization
#ifdef __clang__
#define OMM_OPTIMIZE
#elif __GNUC__
#define OMM_OPTIMIZE __attribute__((optimize("Ofast")))
#else
#define OMM_OPTIMIZE
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

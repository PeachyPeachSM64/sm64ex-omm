#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#if OMM_CODE_DEBUG

#define SIN_0000 0.000f
#define SIN_1000 0.383f
#define SIN_2000 0.707f
#define SIN_3000 0.924f
#define SIN_4000 1.000f
#define SIN_5000 0.924f
#define SIN_6000 0.707f
#define SIN_7000 0.383f
#define SIN_8000 0.000f
#define SIN_9000 -0.383f
#define SIN_A000 -0.707f
#define SIN_B000 -0.924f
#define SIN_C000 -1.000f
#define SIN_D000 -0.924f
#define SIN_E000 -0.707f
#define SIN_F000 -0.383f
#define COS_0000 SIN_4000
#define COS_1000 SIN_5000
#define COS_2000 SIN_6000
#define COS_3000 SIN_7000
#define COS_4000 SIN_8000
#define COS_5000 SIN_9000
#define COS_6000 SIN_A000
#define COS_7000 SIN_B000
#define COS_8000 SIN_C000
#define COS_9000 SIN_D000
#define COS_A000 SIN_E000
#define COS_B000 SIN_F000
#define COS_C000 SIN_0000
#define COS_D000 SIN_1000
#define COS_E000 SIN_2000
#define COS_F000 SIN_3000

//
// Gfx data
//

static const Lights1 omm_debug_hitbox_light = gdSPDefLights1(
    0x40, 0xc0, 0xff,
    0x40, 0xc0, 0xff,
    0x28, 0x28, 0x28
);

static const Lights1 omm_debug_hurtbox_light = gdSPDefLights1(
    0xff, 0x40, 0x40,
    0xff, 0x40, 0x40,
    0x28, 0x28, 0x28
);

static const Lights1 omm_debug_wallbox_light = gdSPDefLights1(
    0x40, 0xc0, 0x40,
    0x40, 0xc0, 0x40,
    0x28, 0x28, 0x28
);

static const Vtx omm_debug_box_vertices[] = {
    { { {                0, 100.f,                0 }, 0, { 0, 0 }, {                    0x00, 0x7f,                    0x00, 0xff } } },
    { { { 100.f * COS_0000, 100.f, 100.f * SIN_0000 }, 0, { 0, 0 }, { (s8) (COS_0000 * 127.f), 0x00, (s8) (SIN_0000 * 127.f), 0xff } } },
    { { { 100.f * COS_1000, 100.f, 100.f * SIN_1000 }, 0, { 0, 0 }, { (s8) (COS_1000 * 127.f), 0x00, (s8) (SIN_1000 * 127.f), 0xff } } },
    { { { 100.f * COS_2000, 100.f, 100.f * SIN_2000 }, 0, { 0, 0 }, { (s8) (COS_2000 * 127.f), 0x00, (s8) (SIN_2000 * 127.f), 0xff } } },
    { { { 100.f * COS_3000, 100.f, 100.f * SIN_3000 }, 0, { 0, 0 }, { (s8) (COS_3000 * 127.f), 0x00, (s8) (SIN_3000 * 127.f), 0xff } } },
    { { { 100.f * COS_4000, 100.f, 100.f * SIN_4000 }, 0, { 0, 0 }, { (s8) (COS_4000 * 127.f), 0x00, (s8) (SIN_4000 * 127.f), 0xff } } },
    { { { 100.f * COS_5000, 100.f, 100.f * SIN_5000 }, 0, { 0, 0 }, { (s8) (COS_5000 * 127.f), 0x00, (s8) (SIN_5000 * 127.f), 0xff } } },
    { { { 100.f * COS_6000, 100.f, 100.f * SIN_6000 }, 0, { 0, 0 }, { (s8) (COS_6000 * 127.f), 0x00, (s8) (SIN_6000 * 127.f), 0xff } } },
    { { { 100.f * COS_7000, 100.f, 100.f * SIN_7000 }, 0, { 0, 0 }, { (s8) (COS_7000 * 127.f), 0x00, (s8) (SIN_7000 * 127.f), 0xff } } },
    { { { 100.f * COS_8000, 100.f, 100.f * SIN_8000 }, 0, { 0, 0 }, { (s8) (COS_8000 * 127.f), 0x00, (s8) (SIN_8000 * 127.f), 0xff } } },
    { { {                0, 100.f,                0 }, 0, { 0, 0 }, {                    0x00, 0x7f,                    0x00, 0xff } } },
    { { { 100.f * COS_8000, 100.f, 100.f * SIN_8000 }, 0, { 0, 0 }, { (s8) (COS_8000 * 127.f), 0x00, (s8) (SIN_8000 * 127.f), 0xff } } },
    { { { 100.f * COS_9000, 100.f, 100.f * SIN_9000 }, 0, { 0, 0 }, { (s8) (COS_9000 * 127.f), 0x00, (s8) (SIN_9000 * 127.f), 0xff } } },
    { { { 100.f * COS_A000, 100.f, 100.f * SIN_A000 }, 0, { 0, 0 }, { (s8) (COS_A000 * 127.f), 0x00, (s8) (SIN_A000 * 127.f), 0xff } } },
    { { { 100.f * COS_B000, 100.f, 100.f * SIN_B000 }, 0, { 0, 0 }, { (s8) (COS_B000 * 127.f), 0x00, (s8) (SIN_B000 * 127.f), 0xff } } },
    { { { 100.f * COS_C000, 100.f, 100.f * SIN_C000 }, 0, { 0, 0 }, { (s8) (COS_C000 * 127.f), 0x00, (s8) (SIN_C000 * 127.f), 0xff } } },
    { { { 100.f * COS_D000, 100.f, 100.f * SIN_D000 }, 0, { 0, 0 }, { (s8) (COS_D000 * 127.f), 0x00, (s8) (SIN_D000 * 127.f), 0xff } } },
    { { { 100.f * COS_E000, 100.f, 100.f * SIN_E000 }, 0, { 0, 0 }, { (s8) (COS_E000 * 127.f), 0x00, (s8) (SIN_E000 * 127.f), 0xff } } },
    { { { 100.f * COS_F000, 100.f, 100.f * SIN_F000 }, 0, { 0, 0 }, { (s8) (COS_F000 * 127.f), 0x00, (s8) (SIN_F000 * 127.f), 0xff } } },
    { { { 100.f * COS_0000, 100.f, 100.f * SIN_0000 }, 0, { 0, 0 }, { (s8) (COS_0000 * 127.f), 0x00, (s8) (SIN_0000 * 127.f), 0xff } } },
    { { {                0,   0.f,                0 }, 0, { 0, 0 }, {                    0x00, 0x80,                    0x00, 0xff } } },
    { { { 100.f * COS_0000,   0.f, 100.f * SIN_0000 }, 0, { 0, 0 }, { (s8) (COS_0000 * 127.f), 0x00, (s8) (SIN_0000 * 127.f), 0xff } } },
    { { { 100.f * COS_1000,   0.f, 100.f * SIN_1000 }, 0, { 0, 0 }, { (s8) (COS_1000 * 127.f), 0x00, (s8) (SIN_1000 * 127.f), 0xff } } },
    { { { 100.f * COS_2000,   0.f, 100.f * SIN_2000 }, 0, { 0, 0 }, { (s8) (COS_2000 * 127.f), 0x00, (s8) (SIN_2000 * 127.f), 0xff } } },
    { { { 100.f * COS_3000,   0.f, 100.f * SIN_3000 }, 0, { 0, 0 }, { (s8) (COS_3000 * 127.f), 0x00, (s8) (SIN_3000 * 127.f), 0xff } } },
    { { { 100.f * COS_4000,   0.f, 100.f * SIN_4000 }, 0, { 0, 0 }, { (s8) (COS_4000 * 127.f), 0x00, (s8) (SIN_4000 * 127.f), 0xff } } },
    { { { 100.f * COS_5000,   0.f, 100.f * SIN_5000 }, 0, { 0, 0 }, { (s8) (COS_5000 * 127.f), 0x00, (s8) (SIN_5000 * 127.f), 0xff } } },
    { { { 100.f * COS_6000,   0.f, 100.f * SIN_6000 }, 0, { 0, 0 }, { (s8) (COS_6000 * 127.f), 0x00, (s8) (SIN_6000 * 127.f), 0xff } } },
    { { { 100.f * COS_7000,   0.f, 100.f * SIN_7000 }, 0, { 0, 0 }, { (s8) (COS_7000 * 127.f), 0x00, (s8) (SIN_7000 * 127.f), 0xff } } },
    { { { 100.f * COS_8000,   0.f, 100.f * SIN_8000 }, 0, { 0, 0 }, { (s8) (COS_8000 * 127.f), 0x00, (s8) (SIN_8000 * 127.f), 0xff } } },
    { { {                0,   0.f,                0 }, 0, { 0, 0 }, {                    0x00, 0x80,                    0x00, 0xff } } },
    { { { 100.f * COS_8000,   0.f, 100.f * SIN_8000 }, 0, { 0, 0 }, { (s8) (COS_8000 * 127.f), 0x00, (s8) (SIN_8000 * 127.f), 0xff } } },
    { { { 100.f * COS_9000,   0.f, 100.f * SIN_9000 }, 0, { 0, 0 }, { (s8) (COS_9000 * 127.f), 0x00, (s8) (SIN_9000 * 127.f), 0xff } } },
    { { { 100.f * COS_A000,   0.f, 100.f * SIN_A000 }, 0, { 0, 0 }, { (s8) (COS_A000 * 127.f), 0x00, (s8) (SIN_A000 * 127.f), 0xff } } },
    { { { 100.f * COS_B000,   0.f, 100.f * SIN_B000 }, 0, { 0, 0 }, { (s8) (COS_B000 * 127.f), 0x00, (s8) (SIN_B000 * 127.f), 0xff } } },
    { { { 100.f * COS_C000,   0.f, 100.f * SIN_C000 }, 0, { 0, 0 }, { (s8) (COS_C000 * 127.f), 0x00, (s8) (SIN_C000 * 127.f), 0xff } } },
    { { { 100.f * COS_D000,   0.f, 100.f * SIN_D000 }, 0, { 0, 0 }, { (s8) (COS_D000 * 127.f), 0x00, (s8) (SIN_D000 * 127.f), 0xff } } },
    { { { 100.f * COS_E000,   0.f, 100.f * SIN_E000 }, 0, { 0, 0 }, { (s8) (COS_E000 * 127.f), 0x00, (s8) (SIN_E000 * 127.f), 0xff } } },
    { { { 100.f * COS_F000,   0.f, 100.f * SIN_F000 }, 0, { 0, 0 }, { (s8) (COS_F000 * 127.f), 0x00, (s8) (SIN_F000 * 127.f), 0xff } } },
    { { { 100.f * COS_0000,   0.f, 100.f * SIN_0000 }, 0, { 0, 0 }, { (s8) (COS_0000 * 127.f), 0x00, (s8) (SIN_0000 * 127.f), 0xff } } },
    { { { 100.f * COS_0000, 100.f, 100.f * SIN_0000 }, 0, { 0, 0 }, { (s8) (COS_0000 * 127.f), 0x00, (s8) (SIN_0000 * 127.f), 0xff } } },
    { { { 100.f * COS_0000,   0.f, 100.f * SIN_0000 }, 0, { 0, 0 }, { (s8) (COS_0000 * 127.f), 0x00, (s8) (SIN_0000 * 127.f), 0xff } } },
    { { { 100.f * COS_1000, 100.f, 100.f * SIN_1000 }, 0, { 0, 0 }, { (s8) (COS_1000 * 127.f), 0x00, (s8) (SIN_1000 * 127.f), 0xff } } },
    { { { 100.f * COS_1000,   0.f, 100.f * SIN_1000 }, 0, { 0, 0 }, { (s8) (COS_1000 * 127.f), 0x00, (s8) (SIN_1000 * 127.f), 0xff } } },
    { { { 100.f * COS_2000, 100.f, 100.f * SIN_2000 }, 0, { 0, 0 }, { (s8) (COS_2000 * 127.f), 0x00, (s8) (SIN_2000 * 127.f), 0xff } } },
    { { { 100.f * COS_2000,   0.f, 100.f * SIN_2000 }, 0, { 0, 0 }, { (s8) (COS_2000 * 127.f), 0x00, (s8) (SIN_2000 * 127.f), 0xff } } },
    { { { 100.f * COS_3000, 100.f, 100.f * SIN_3000 }, 0, { 0, 0 }, { (s8) (COS_3000 * 127.f), 0x00, (s8) (SIN_3000 * 127.f), 0xff } } },
    { { { 100.f * COS_3000,   0.f, 100.f * SIN_3000 }, 0, { 0, 0 }, { (s8) (COS_3000 * 127.f), 0x00, (s8) (SIN_3000 * 127.f), 0xff } } },
    { { { 100.f * COS_4000, 100.f, 100.f * SIN_4000 }, 0, { 0, 0 }, { (s8) (COS_4000 * 127.f), 0x00, (s8) (SIN_4000 * 127.f), 0xff } } },
    { { { 100.f * COS_4000,   0.f, 100.f * SIN_4000 }, 0, { 0, 0 }, { (s8) (COS_4000 * 127.f), 0x00, (s8) (SIN_4000 * 127.f), 0xff } } },
    { { { 100.f * COS_4000, 100.f, 100.f * SIN_4000 }, 0, { 0, 0 }, { (s8) (COS_4000 * 127.f), 0x00, (s8) (SIN_4000 * 127.f), 0xff } } },
    { { { 100.f * COS_4000,   0.f, 100.f * SIN_4000 }, 0, { 0, 0 }, { (s8) (COS_4000 * 127.f), 0x00, (s8) (SIN_4000 * 127.f), 0xff } } },
    { { { 100.f * COS_5000, 100.f, 100.f * SIN_5000 }, 0, { 0, 0 }, { (s8) (COS_5000 * 127.f), 0x00, (s8) (SIN_5000 * 127.f), 0xff } } },
    { { { 100.f * COS_5000,   0.f, 100.f * SIN_5000 }, 0, { 0, 0 }, { (s8) (COS_5000 * 127.f), 0x00, (s8) (SIN_5000 * 127.f), 0xff } } },
    { { { 100.f * COS_6000, 100.f, 100.f * SIN_6000 }, 0, { 0, 0 }, { (s8) (COS_6000 * 127.f), 0x00, (s8) (SIN_6000 * 127.f), 0xff } } },
    { { { 100.f * COS_6000,   0.f, 100.f * SIN_6000 }, 0, { 0, 0 }, { (s8) (COS_6000 * 127.f), 0x00, (s8) (SIN_6000 * 127.f), 0xff } } },
    { { { 100.f * COS_7000, 100.f, 100.f * SIN_7000 }, 0, { 0, 0 }, { (s8) (COS_7000 * 127.f), 0x00, (s8) (SIN_7000 * 127.f), 0xff } } },
    { { { 100.f * COS_7000,   0.f, 100.f * SIN_7000 }, 0, { 0, 0 }, { (s8) (COS_7000 * 127.f), 0x00, (s8) (SIN_7000 * 127.f), 0xff } } },
    { { { 100.f * COS_8000, 100.f, 100.f * SIN_8000 }, 0, { 0, 0 }, { (s8) (COS_8000 * 127.f), 0x00, (s8) (SIN_8000 * 127.f), 0xff } } },
    { { { 100.f * COS_8000,   0.f, 100.f * SIN_8000 }, 0, { 0, 0 }, { (s8) (COS_8000 * 127.f), 0x00, (s8) (SIN_8000 * 127.f), 0xff } } },
    { { { 100.f * COS_8000, 100.f, 100.f * SIN_8000 }, 0, { 0, 0 }, { (s8) (COS_8000 * 127.f), 0x00, (s8) (SIN_8000 * 127.f), 0xff } } },
    { { { 100.f * COS_8000,   0.f, 100.f * SIN_8000 }, 0, { 0, 0 }, { (s8) (COS_8000 * 127.f), 0x00, (s8) (SIN_8000 * 127.f), 0xff } } },
    { { { 100.f * COS_9000, 100.f, 100.f * SIN_9000 }, 0, { 0, 0 }, { (s8) (COS_9000 * 127.f), 0x00, (s8) (SIN_9000 * 127.f), 0xff } } },
    { { { 100.f * COS_9000,   0.f, 100.f * SIN_9000 }, 0, { 0, 0 }, { (s8) (COS_9000 * 127.f), 0x00, (s8) (SIN_9000 * 127.f), 0xff } } },
    { { { 100.f * COS_A000, 100.f, 100.f * SIN_A000 }, 0, { 0, 0 }, { (s8) (COS_A000 * 127.f), 0x00, (s8) (SIN_A000 * 127.f), 0xff } } },
    { { { 100.f * COS_A000,   0.f, 100.f * SIN_A000 }, 0, { 0, 0 }, { (s8) (COS_A000 * 127.f), 0x00, (s8) (SIN_A000 * 127.f), 0xff } } },
    { { { 100.f * COS_B000, 100.f, 100.f * SIN_B000 }, 0, { 0, 0 }, { (s8) (COS_B000 * 127.f), 0x00, (s8) (SIN_B000 * 127.f), 0xff } } },
    { { { 100.f * COS_B000,   0.f, 100.f * SIN_B000 }, 0, { 0, 0 }, { (s8) (COS_B000 * 127.f), 0x00, (s8) (SIN_B000 * 127.f), 0xff } } },
    { { { 100.f * COS_C000, 100.f, 100.f * SIN_C000 }, 0, { 0, 0 }, { (s8) (COS_C000 * 127.f), 0x00, (s8) (SIN_C000 * 127.f), 0xff } } },
    { { { 100.f * COS_C000,   0.f, 100.f * SIN_C000 }, 0, { 0, 0 }, { (s8) (COS_C000 * 127.f), 0x00, (s8) (SIN_C000 * 127.f), 0xff } } },
    { { { 100.f * COS_C000, 100.f, 100.f * SIN_C000 }, 0, { 0, 0 }, { (s8) (COS_C000 * 127.f), 0x00, (s8) (SIN_C000 * 127.f), 0xff } } },
    { { { 100.f * COS_C000,   0.f, 100.f * SIN_C000 }, 0, { 0, 0 }, { (s8) (COS_C000 * 127.f), 0x00, (s8) (SIN_C000 * 127.f), 0xff } } },
    { { { 100.f * COS_D000, 100.f, 100.f * SIN_D000 }, 0, { 0, 0 }, { (s8) (COS_D000 * 127.f), 0x00, (s8) (SIN_D000 * 127.f), 0xff } } },
    { { { 100.f * COS_D000,   0.f, 100.f * SIN_D000 }, 0, { 0, 0 }, { (s8) (COS_D000 * 127.f), 0x00, (s8) (SIN_D000 * 127.f), 0xff } } },
    { { { 100.f * COS_E000, 100.f, 100.f * SIN_E000 }, 0, { 0, 0 }, { (s8) (COS_E000 * 127.f), 0x00, (s8) (SIN_E000 * 127.f), 0xff } } },
    { { { 100.f * COS_E000,   0.f, 100.f * SIN_E000 }, 0, { 0, 0 }, { (s8) (COS_E000 * 127.f), 0x00, (s8) (SIN_E000 * 127.f), 0xff } } },
    { { { 100.f * COS_F000, 100.f, 100.f * SIN_F000 }, 0, { 0, 0 }, { (s8) (COS_F000 * 127.f), 0x00, (s8) (SIN_F000 * 127.f), 0xff } } },
    { { { 100.f * COS_F000,   0.f, 100.f * SIN_F000 }, 0, { 0, 0 }, { (s8) (COS_F000 * 127.f), 0x00, (s8) (SIN_F000 * 127.f), 0xff } } },
    { { { 100.f * COS_0000, 100.f, 100.f * SIN_0000 }, 0, { 0, 0 }, { (s8) (COS_0000 * 127.f), 0x00, (s8) (SIN_0000 * 127.f), 0xff } } },
    { { { 100.f * COS_0000,   0.f, 100.f * SIN_0000 }, 0, { 0, 0 }, { (s8) (COS_0000 * 127.f), 0x00, (s8) (SIN_0000 * 127.f), 0xff } } },
};

static const Gfx omm_debug_box_triangles[] = {
    gsSPVertex(omm_debug_box_vertices + 0, 10, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 0, 3, 2, 0x0),
    gsSP2Triangles(0, 4, 3, 0x0, 0, 5, 4, 0x0),
    gsSP2Triangles(0, 6, 5, 0x0, 0, 7, 6, 0x0),
    gsSP2Triangles(0, 8, 7, 0x0, 0, 9, 8, 0x0),
    gsSPVertex(omm_debug_box_vertices + 10, 10, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 0, 3, 2, 0x0),
    gsSP2Triangles(0, 4, 3, 0x0, 0, 5, 4, 0x0),
    gsSP2Triangles(0, 6, 5, 0x0, 0, 7, 6, 0x0),
    gsSP2Triangles(0, 8, 7, 0x0, 0, 9, 8, 0x0),
    gsSPVertex(omm_debug_box_vertices + 20, 10, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 2, 3, 0x0),
    gsSP2Triangles(0, 3, 4, 0x0, 0, 4, 5, 0x0),
    gsSP2Triangles(0, 5, 6, 0x0, 0, 6, 7, 0x0),
    gsSP2Triangles(0, 7, 8, 0x0, 0, 8, 9, 0x0),
    gsSPVertex(omm_debug_box_vertices + 30, 10, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 2, 3, 0x0),
    gsSP2Triangles(0, 3, 4, 0x0, 0, 4, 5, 0x0),
    gsSP2Triangles(0, 5, 6, 0x0, 0, 6, 7, 0x0),
    gsSP2Triangles(0, 7, 8, 0x0, 0, 8, 9, 0x0),
    gsSPVertex(omm_debug_box_vertices + 40, 10, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 1, 2, 3, 0x0),
    gsSP2Triangles(2, 4, 3, 0x0, 3, 4, 5, 0x0),
    gsSP2Triangles(4, 6, 5, 0x0, 5, 6, 7, 0x0),
    gsSP2Triangles(6, 8, 7, 0x0, 7, 8, 9, 0x0),
    gsSPVertex(omm_debug_box_vertices + 50, 10, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 1, 2, 3, 0x0),
    gsSP2Triangles(2, 4, 3, 0x0, 3, 4, 5, 0x0),
    gsSP2Triangles(4, 6, 5, 0x0, 5, 6, 7, 0x0),
    gsSP2Triangles(6, 8, 7, 0x0, 7, 8, 9, 0x0),
    gsSPVertex(omm_debug_box_vertices + 60, 10, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 1, 2, 3, 0x0),
    gsSP2Triangles(2, 4, 3, 0x0, 3, 4, 5, 0x0),
    gsSP2Triangles(4, 6, 5, 0x0, 5, 6, 7, 0x0),
    gsSP2Triangles(6, 8, 7, 0x0, 7, 8, 9, 0x0),
    gsSPVertex(omm_debug_box_vertices + 70, 10, 0),
    gsSP2Triangles(0, 2, 1, 0x0, 1, 2, 3, 0x0),
    gsSP2Triangles(2, 4, 3, 0x0, 3, 4, 5, 0x0),
    gsSP2Triangles(4, 6, 5, 0x0, 5, 6, 7, 0x0),
    gsSP2Triangles(6, 8, 7, 0x0, 7, 8, 9, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_debug_hitbox_gfx[] = {
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
    gsDPSetEnvColor(255, 255, 255, 100),
    gsSPLight(&omm_debug_hitbox_light.l, 1),
    gsSPLight(&omm_debug_hitbox_light.a, 2),
    gsSPDisplayList(omm_debug_box_triangles),
    gsDPSetEnvColor(255, 255, 255, 255),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_debug_hurtbox_gfx[] = {
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
    gsDPSetEnvColor(255, 255, 255, 100),
    gsSPLight(&omm_debug_hurtbox_light.l, 1),
    gsSPLight(&omm_debug_hurtbox_light.a, 2),
    gsSPDisplayList(omm_debug_box_triangles),
    gsDPSetEnvColor(255, 255, 255, 255),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_debug_wallbox_gfx[] = {
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
    gsDPSetEnvColor(255, 255, 255, 100),
    gsSPLight(&omm_debug_wallbox_light.l, 1),
    gsSPLight(&omm_debug_wallbox_light.a, 2),
    gsSPDisplayList(omm_debug_box_triangles),
    gsDPSetEnvColor(255, 255, 255, 255),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

static const GeoLayout omm_geo_debug_box[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 0),
    GEO_OPEN_NODE(),
        GEO_SWITCH_CASE(3, geo_switch_anim_state),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_debug_hitbox_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_debug_hurtbox_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_debug_wallbox_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behaviors
//

static void omm_bhv_debug_box_update() {
    struct Object *o = gCurrentObject;
    if (!o->parentObj || !o->parentObj->activeFlags) {
        obj_mark_for_deletion(o);
        return;
    }

    switch (o->oAnimState) {

        // Hitbox
        case 0: {
            if (o->parentObj == gOmmCapture) {
                o->oPosX   = o->parentObj->oPosX;
                o->oPosY   = o->parentObj->oPosY - gOmmData->mario->capture.hitboxOffset;
                o->oPosZ   = o->parentObj->oPosZ;
                o->oScaleX = gOmmData->mario->capture.hitboxRadius / 100.f;
                o->oScaleY = gOmmData->mario->capture.hitboxHeight / 100.f;
                o->oScaleZ = gOmmData->mario->capture.hitboxRadius / 100.f;
            } else {
                o->oPosX   = o->parentObj->oPosX;
                o->oPosY   = o->parentObj->oPosY - o->parentObj->hitboxDownOffset;
                o->oPosZ   = o->parentObj->oPosZ;
                o->oScaleX = o->parentObj->hitboxRadius / 100.f;
                o->oScaleY = o->parentObj->hitboxHeight / 100.f;
                o->oScaleZ = o->parentObj->hitboxRadius / 100.f;
            }
        } break;

        // Hurtbox
        case 1: {
            o->oPosX   = o->parentObj->oPosX;
            o->oPosY   = o->parentObj->oPosY - o->parentObj->hitboxDownOffset;
            o->oPosZ   = o->parentObj->oPosZ;
            o->oScaleX = o->parentObj->hurtboxRadius / 100.f;
            o->oScaleY = o->parentObj->hurtboxHeight / 100.f;
            o->oScaleZ = o->parentObj->hurtboxRadius / 100.f;
        } break;

        // Wallbox
        case 2: {
            if (o->parentObj == gOmmCapture) {
                o->oPosX   = o->parentObj->oPosX;
                o->oPosY   = o->parentObj->oPosY - gOmmData->mario->capture.hitboxOffset;
                o->oPosZ   = o->parentObj->oPosZ;
                o->oScaleX = gOmmData->mario->capture.hitboxWall   / 100.f;
                o->oScaleY = gOmmData->mario->capture.hitboxHeight / 100.f;
                o->oScaleZ = gOmmData->mario->capture.hitboxWall   / 100.f;
            } else {
                o->oPosX   = o->parentObj->oPosX;
                o->oPosY   = o->parentObj->oPosY - o->parentObj->hitboxDownOffset;
                o->oPosZ   = o->parentObj->oPosZ;
                o->oScaleX = o->parentObj->oWallHitboxRadius / 100.f;
                o->oScaleY = o->parentObj->hitboxHeight      / 100.f;
                o->oScaleZ = o->parentObj->oWallHitboxRadius / 100.f;
            }
        } break;
    }
    obj_set_angle(o, 0, 0, 0);
}

static const BehaviorScript omm_bhv_debug_box[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_debug_box_update,
    0x09000000,
};

//
// Spawner
//

static struct Object *omm_spawn_debug_box(struct Object *o, s32 type) {
    struct Object *box = obj_spawn_from_geo(o, omm_geo_debug_box, omm_bhv_debug_box);
    obj_set_always_rendered(box, true);
    box->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    box->oAnimState = type;
    return box;
}

//
// Code profiling
//

#if defined(_WIN32)
#define OMM_DEBUG_COUNTER_FUNCTIONS_DEFINED
#include <windows.h>

static struct {
    f64 start;
    f64 end;
    f64 sum;
    f64 disp;
} sCounters[OMM_NUM_COUNTERS];

void omm_debug_start_counter(s32 counter) {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    f64 freq = (s64) li.QuadPart;
    QueryPerformanceCounter(&li);
    sCounters[counter].start = (f64) li.QuadPart / freq;
}

void omm_debug_end_counter(s32 counter) {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    f64 freq = (s64) li.QuadPart;
    QueryPerformanceCounter(&li);
    sCounters[counter].end = (f64) li.QuadPart / freq;
    sCounters[counter].sum += (sCounters[counter].end - sCounters[counter].start);
}

static void omm_debug_update_counters() {
    omm_debug_end_counter(OMM_COUNTER_FPS);
    if (gGlobalTimer % 30 == 0) {
        for (s32 i = 0; i != OMM_NUM_COUNTERS; ++i) {
            sCounters[i].disp = sCounters[i].sum / (i != OMM_COUNTER_FPS ? 30.0 : 1.0);
            sCounters[i].sum = 0;
        }
    }
    if (gOmmDebugProfiler) {
        static const struct { s32 idx; const char *str; }
        sOmmDebugProfilerDisplay[] = {
            { OMM_COUNTER_OMM, "OMM" },
            { OMM_COUNTER_LVL, "LUL" },
            { OMM_COUNTER_PRE, "PRE" },
            { OMM_COUNTER_GEO, "GEO" },
            { OMM_COUNTER_GFX, "GF*" },
            { OMM_COUNTER_RDR, "RDR" },
            { OMM_COUNTER_FRM, "FRM" },
            { OMM_COUNTER_FPS, "FPS" },
        };
        for (s32 i = 0; i != OMM_NUM_COUNTERS; ++i) {
            s32 us = (s32) (1000000ll * sCounters[sOmmDebugProfilerDisplay[i].idx].disp);
            if (sOmmDebugProfilerDisplay[i].idx == OMM_COUNTER_FPS) {
                s32 curFPS = (s32) ((1000000.0 / max_s(1, us)) * (gNumInterpolatedFrames * 30.5));
                s32 maxUPS = (s32) (1.0 / max(0.0000001, sCounters[OMM_COUNTER_FRM].disp));

                // curFPS is the number of drawn frames per second.
                // maxUPS is the number of game updates the game could run per second if there was no sleep time.
                // As long as this number is above 30, the game won't suffer any slowdown.
                OMM_PRINT_TEXT(-60, -14 + 18 * (OMM_NUM_COUNTERS - i), "%s %d I %d", sOmmDebugProfilerDisplay[i].str, min_s(curFPS, gNumInterpolatedFrames * 30), maxUPS);
            } else {
                OMM_PRINT_TEXT(-60, -14 + 18 * (OMM_NUM_COUNTERS - i), "%s %d", sOmmDebugProfilerDisplay[i].str, us);
            }
        }
    }
    omm_debug_start_counter(OMM_COUNTER_FPS);
}

#else

static void omm_debug_update_counters() {
}

#endif

//
// Update
//

OMM_ROUTINE_UPDATE(omm_debug_update) {
    struct MarioState *m = gMarioState;

    // Debug boxes
    if (gObjectLists && !omm_is_main_menu()) {

        // Register existing boxes and unload unused ones
        bool objHasBox[OBJECT_POOL_CAPACITY][3] = { false };
        for_each_object_with_behavior(box, omm_bhv_debug_box) {
            struct Object *p = box->parentObj;
            s32 boxType = box->oAnimState;
            if (p && p->activeFlags != ACTIVE_FLAG_DEACTIVATED && (
                (boxType == 0 && gOmmDebugHitbox) ||
                (boxType == 1 && gOmmDebugHurtbox) ||
                (boxType == 2 && gOmmDebugWallbox))
            ) {
                s32 objIndex = (s32) (((uintptr_t) p - (uintptr_t) gObjectPool) / sizeof(struct Object));
                objHasBox[objIndex][boxType] = true;
            } else {
                obj_mark_for_deletion(box);
            }
        }
        
        // Create boxes for each object
        static const s32 sObjTypes[] = {
            OBJ_LIST_PLAYER,
            OBJ_LIST_DESTRUCTIVE,
            OBJ_LIST_GENACTOR,
            OBJ_LIST_PUSHABLE,
            OBJ_LIST_LEVEL,
            OBJ_LIST_DEFAULT,
            OBJ_LIST_SURFACE,
            OBJ_LIST_POLELIKE,
            OBJ_LIST_SPAWNER,
            -1
        };
        for (const s32 *i = sObjTypes; *i != -1; ++i) {
            struct Object *head = (struct Object *) &gObjectLists[*i];
            struct Object *next = (struct Object *) head->header.next; 
            while (next != head) {
                if (next->oIntangibleTimer == 0) {
                    struct Object *obj = next;
                    if (obj == gMarioObject && m->action == ACT_OMM_POSSESSION) {
                        obj = gOmmCapture;
                    }
                    s32 objIndex = (s32) (((uintptr_t) obj - (uintptr_t) gObjectPool) / sizeof(struct Object));

                    // Hitbox
                    if (gOmmDebugHitbox && !objHasBox[objIndex][0]) {
                        omm_spawn_debug_box(obj, 0);
                    }

                    // Hurtbox
                    if (gOmmDebugHurtbox && !objHasBox[objIndex][1]) {
                        omm_spawn_debug_box(obj, 1);
                    }

                    // Wallbox
                    if (gOmmDebugWallbox && !objHasBox[objIndex][2]) {
                        omm_spawn_debug_box(obj, 2);
                    }
                }
                next = (struct Object *) next->header.next; 
            }
        }
    }

    // Mario info
    if (gOmmDebugMario) {
        OMM_PRINT_TEXT(-60, 58, "POS %d %d %d", (s32) m->pos[0], (s32) m->pos[1], (s32) m->pos[2]);
        OMM_PRINT_TEXT(-60, 40, "PEAK %d YAW %04X", (s32) (OMM_MOVESET_CLASSIC ? m->peakHeight : gOmmData->mario->state.peakHeight), (u16) m->faceAngle[1]);
        OMM_PRINT_TEXT(-60, 22, "UEL %d %d %d FWD %d", (s32) m->vel[0], (s32) m->vel[1], (s32) m->vel[2], (s32) m->forwardVel);
        OMM_PRINT_TEXT(-60,  4, "L %d C %d A %d R %d", (s32) gCurrLevelNum, (s32) gCurrCourseNum, (s32) gCurrAreaIndex, (s32) (m->floor ? m->floor->room : 0));
    }

    // Cappy info
    if (gOmmDebugCappy && gLoadedGraphNodes) {
        s32 sCapModels[5] = {
            omm_player_get_selected_model(),
            omm_player_get_selected_normal_cap(),
            omm_player_get_selected_wing_cap(),
            omm_player_get_selected_metal_cap(),
            omm_player_get_selected_winged_metal_cap()
        };
        for (s32 i = 0; i != 5; ++i) {
            u32 id = omm_cappy_gfx_get_graph_node_identifier(gLoadedGraphNodes[sCapModels[i]]);
            OMM_PRINT_TEXT(-60, 76 - 18 * i, "%08X", id);
        }
    }

    // Execution times
    omm_debug_update_counters();
}

#endif

#if !defined(OMM_DEBUG_COUNTER_FUNCTIONS_DEFINED)

void omm_debug_start_counter(s32) {
}

void omm_debug_end_counter(s32) {
}

#endif

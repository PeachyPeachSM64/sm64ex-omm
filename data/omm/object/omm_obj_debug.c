#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#if OMM_CODE_DEBUG
#include "behavior_commands.h"

#define SIN_0000 (0.000f)
#define SIN_1000 (0.383f)
#define SIN_2000 (0.707f)
#define SIN_3000 (0.924f)
#define SIN_4000 (1.000f)
#define SIN_5000 (0.924f)
#define SIN_6000 (0.707f)
#define SIN_7000 (0.383f)
#define SIN_8000 (0.000f)
#define SIN_9000 (-0.383f)
#define SIN_A000 (-0.707f)
#define SIN_B000 (-0.924f)
#define SIN_C000 (-1.000f)
#define SIN_D000 (-0.924f)
#define SIN_E000 (-0.707f)
#define SIN_F000 (-0.383f)
#define COS_0000 (SIN_4000)
#define COS_1000 (SIN_5000)
#define COS_2000 (SIN_6000)
#define COS_3000 (SIN_7000)
#define COS_4000 (SIN_8000)
#define COS_5000 (SIN_9000)
#define COS_6000 (SIN_A000)
#define COS_7000 (SIN_B000)
#define COS_8000 (SIN_C000)
#define COS_9000 (SIN_D000)
#define COS_A000 (SIN_E000)
#define COS_B000 (SIN_F000)
#define COS_C000 (SIN_0000)
#define COS_D000 (SIN_1000)
#define COS_E000 (SIN_2000)
#define COS_F000 (SIN_3000)

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
    GEO_NODE_START(),
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

static void bhv_omm_debug_box_update() {
    struct Object *o = gCurrentObject;
    if (!o->parentObj || !o->parentObj->activeFlags || o->parentObj->oIntangibleTimer != 0 || (o->parentObj == gMarioObject && omm_mario_is_capture(gMarioState))) {
        obj_mark_for_deletion(o);
        return;
    }

    switch (o->oAnimState) {

        // Hitbox
        case 0: {
            if (o->parentObj == gOmmCapture) {
                o->oPosX   = o->parentObj->oPosX;
                o->oPosY   = o->parentObj->oPosY;
                o->oPosZ   = o->parentObj->oPosZ;
                o->oScaleX = gOmmMario->capture.hitboxRadius / 100.f;
                o->oScaleY = gOmmMario->capture.hitboxHeight / 100.f;
                o->oScaleZ = gOmmMario->capture.hitboxRadius / 100.f;
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
                o->oPosY   = o->parentObj->oPosY;
                o->oPosZ   = o->parentObj->oPosZ;
                o->oScaleX = gOmmMario->capture.hitboxWall   / 100.f;
                o->oScaleY = gOmmMario->capture.hitboxHeight / 100.f;
                o->oScaleZ = gOmmMario->capture.hitboxWall   / 100.f;
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

static const BehaviorScript bhvOmmDebugBox[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_debug_box_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

static struct Object *omm_obj_spawn_debug_box(struct Object *o, s32 type) {
    struct Object *box = obj_spawn_from_geo(o, omm_geo_debug_box, bhvOmmDebugBox);
    obj_set_always_rendered(box, true);
    box->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    box->oAnimState = type;
    return box;
}

//
// Update
//

OMM_ROUTINE_UPDATE(omm_debug_update) {
    struct MarioState *m = gMarioState;

    // Debug boxes
    if (gObjectLists && !omm_is_main_menu()) {

        // Register existing boxes and unload unused ones
        bool objHasBox[OBJECT_POOL_CAPACITY][3] = { false };
        for_each_object_with_behavior(box, bhvOmmDebugBox) {
            struct Object *p = box->parentObj;
            s32 boxType = box->oAnimState;
            if (p && p->activeFlags && (
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
                    if (obj == gMarioObject && omm_mario_is_capture(m)) {
                        obj = gOmmCapture;
                    }
                    s32 objIndex = (s32) (((uintptr_t) obj - (uintptr_t) gObjectPool) / sizeof(struct Object));

                    // Hitbox
                    if (gOmmDebugHitbox && !objHasBox[objIndex][0]) {
                        omm_obj_spawn_debug_box(obj, 0);
                    }

                    // Hurtbox
                    if (gOmmDebugHurtbox && !objHasBox[objIndex][1]) {
                        omm_obj_spawn_debug_box(obj, 1);
                    }

                    // Wallbox
                    if (gOmmDebugWallbox && !objHasBox[objIndex][2]) {
                        omm_obj_spawn_debug_box(obj, 2);
                    }
                }
                next = (struct Object *) next->header.next;
            }
        }
    }

    // Mario info
    s32 y0 = 4;
    s32 x0 = GFX_DIMENSIONS_FROM_LEFT_EDGE(4);
    if (gOmmDebugMario) {
        struct Object *playAsCappy = omm_cappy_get_object_play_as();
        if (playAsCappy) {
            struct Surface *floor = NULL;
            find_floor(playAsCappy->oPosX, playAsCappy->oPosY, playAsCappy->oPosZ, &floor);
            omm_debug_text(x0, y0 + 72, "POS %d %d %d", (s32) playAsCappy->oPosX, (s32) playAsCappy->oPosY, (s32) playAsCappy->oPosZ);
            omm_debug_text(x0, y0 + 54, "UEL %d %d %d FWD %d", (s32) m->vel[0], (s32) m->vel[1], (s32) m->vel[2], (s32) m->forwardVel);
            omm_debug_text(x0, y0 + 36, "ANG %04X %04X %04X", (u16) m->faceAngle[0], (u16) m->faceAngle[1], (u16) m->faceAngle[2]);
            omm_debug_text(x0, y0 + 18, "PEAK %d FY %04d", (s32) (OMM_MOVESET_CLASSIC ? m->peakHeight : gOmmMario->state.peakHeight), (s32) (floor ? floor->normal.y * 1000.f : 0));
            omm_debug_text(x0, y0 +  0, "L %d C %d A %d R %d", (s32) gCurrLevelNum, (s32) gCurrCourseNum, (s32) gCurrAreaIndex, (s32) (floor ? floor->room : 0));
        } else {
            omm_debug_text(x0, y0 + 72, "POS %d %d %d", (s32) m->pos[0], (s32) m->pos[1], (s32) m->pos[2]);
            omm_debug_text(x0, y0 + 54, "UEL %d %d %d FWD %d", (s32) m->vel[0], (s32) m->vel[1], (s32) m->vel[2], (s32) m->forwardVel);
            omm_debug_text(x0, y0 + 36, "ANG %04X %04X %04X", (u16) m->faceAngle[0], (u16) m->faceAngle[1], (u16) m->faceAngle[2]);
            omm_debug_text(x0, y0 + 18, "PEAK %d FY %04d", (s32) (OMM_MOVESET_CLASSIC ? m->peakHeight : gOmmMario->state.peakHeight), (s32) (m->floor ? m->floor->normal.y * 1000.f : 0));
            omm_debug_text(x0, y0 +  0, "L %d C %d A %d R %d", (s32) gCurrLevelNum, (s32) gCurrCourseNum, (s32) gCurrAreaIndex, (s32) (m->floor ? m->floor->room : 0));
        }
        y0 += 90;
    }

    // Cappy info
    if (gOmmDebugCappy && gLoadedGraphNodes) {
        s32 playerIndex = omm_player_get_selected_index_model_and_sounds();
        struct { char type; s32 index; } sCapModels[5] = {
            { ',', omm_player_graphics_get_model(playerIndex) },
            { 'N', omm_player_graphics_get_normal_cap(playerIndex) },
            { 'W', omm_player_graphics_get_wing_cap(playerIndex) },
            { 'M', omm_player_graphics_get_metal_cap(playerIndex) },
            { '*', omm_player_graphics_get_winged_metal_cap(playerIndex) },
        };
        for (s32 i = 0; i != 5; ++i) {
            static struct Object dummy[1];
            dummy->activeFlags = ACTIVE_FLAG_ACTIVE;
            dummy->oGraphNode = gLoadedGraphNodes[sCapModels[i].index];
            omm_models_update_object(dummy);
            u32 id = omm_cappy_gfx_get_graph_node_identifier(dummy->oGraphNode);
            omm_debug_text(x0, y0 + 72 - 18 * i, "%c %08X", sCapModels[i].type, id);
        }
        y0 += 90;
    }

    // Surface info
    if (gOmmDebugSurface) {
        extern void omm_debug_surfaces_print_info(s32 x, s32 y);
        omm_debug_surfaces_print_info(x0, y0);
    }
}

#endif

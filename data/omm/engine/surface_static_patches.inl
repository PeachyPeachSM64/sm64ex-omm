#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

//
// LLL
//

static const Collision omm_lll_2_volcano_collision[] = {
    COL_INIT(),
    COL_VERTEX_INIT(36),
    COL_VERTEX(    0, 5248, -2926),
    COL_VERTEX( -429, 5248, -2942),
    COL_VERTEX( -429, 5248, -2931),
    COL_VERTEX( -787, 5248, -2942),
    COL_VERTEX(-2153, 5248, -2153),
    COL_VERTEX(-2942, 5248,  -787),
    COL_VERTEX(-2942, 5248,   788),
    COL_VERTEX(-2153, 5248,  2154),
    COL_VERTEX( -787, 5248,  2943),
    COL_VERTEX(  788, 5248,  2943),
    COL_VERTEX( 2154, 5248,  2154),
    COL_VERTEX( 2943, 5248,   788),
    COL_VERTEX( 2411, 5248,  -480),
    COL_VERTEX( 2943, 5248,  -787),
    COL_VERTEX( 2154, 5248, -2153),
    COL_VERTEX(  788, 5248, -2942),
    COL_VERTEX(  430, 5248, -2931),
    COL_VERTEX(  430, 5248, -2942),
    COL_VERTEX(    0, 8000, -2926),
    COL_VERTEX( -429, 8000, -2942),
    COL_VERTEX( -429, 8000, -2931),
    COL_VERTEX( -787, 8000, -2942),
    COL_VERTEX(-2153, 8000, -2153),
    COL_VERTEX(-2942, 8000,  -787),
    COL_VERTEX(-2942, 8000,   788),
    COL_VERTEX(-2153, 8000,  2154),
    COL_VERTEX( -787, 8000,  2943),
    COL_VERTEX(  788, 8000,  2943),
    COL_VERTEX( 2154, 8000,  2154),
    COL_VERTEX( 2943, 8000,   788),
    COL_VERTEX( 2411, 8000,  -480),
    COL_VERTEX( 2943, 8000,  -787),
    COL_VERTEX( 2154, 8000, -2153),
    COL_VERTEX(  788, 8000, -2942),
    COL_VERTEX(  430, 8000, -2931),
    COL_VERTEX(  430, 8000, -2942),
    COL_TRI_INIT(SURFACE_TRAPDOOR, 36), // Disable wall slide
    COL_TRI( 1,  0, 18),
    COL_TRI( 1, 18, 19),
    COL_TRI( 2,  1, 19),
    COL_TRI( 2, 19, 20),
    COL_TRI( 3,  2, 20),
    COL_TRI( 3, 20, 21),
    COL_TRI( 4,  3, 21),
    COL_TRI( 4, 21, 22),
    COL_TRI( 5,  4, 22),
    COL_TRI( 5, 22, 23),
    COL_TRI( 6,  5, 23),
    COL_TRI( 6, 23, 24),
    COL_TRI( 7,  6, 24),
    COL_TRI( 7, 24, 25),
    COL_TRI( 8,  7, 25),
    COL_TRI( 8, 25, 26),
    COL_TRI( 9,  8, 26),
    COL_TRI( 9, 26, 27),
    COL_TRI(10,  9, 27),
    COL_TRI(10, 27, 28),
    COL_TRI(11, 10, 28),
    COL_TRI(11, 28, 29),
    COL_TRI(12, 11, 29),
    COL_TRI(12, 29, 30),
    COL_TRI(13, 12, 30),
    COL_TRI(13, 30, 31),
    COL_TRI(14, 13, 31),
    COL_TRI(14, 31, 32),
    COL_TRI(15, 14, 32),
    COL_TRI(15, 32, 33),
    COL_TRI(16, 15, 33),
    COL_TRI(16, 33, 34),
    COL_TRI(17, 16, 34),
    COL_TRI(17, 34, 35),
    COL_TRI( 0, 17, 35),
    COL_TRI( 0, 35, 18),
    COL_TRI_STOP(),
    COL_END(),
};

//
// DDD
//

static const Collision omm_ddd_2_gap_collision[] = {
    COL_INIT(),
    COL_VERTEX_INIT(4),
    COL_VERTEX(4435, -4087, 5630),
    COL_VERTEX(3400, -4087, 5630),
    COL_VERTEX(4435, -4087, 5650),
    COL_VERTEX(3400, -4087, 5650),
    COL_TRI_INIT(SURFACE_DEFAULT, 2),
    COL_TRI(0, 1, 2),
    COL_TRI(1, 3, 2),
    COL_TRI_STOP(),
    COL_END(),
};

//
// BITFS
//

static const Collision omm_bitfs_1_gap_collision[] = {
    COL_INIT(),
    COL_VERTEX_INIT(16),
    COL_VERTEX(6661, 2899, -79),
    COL_VERTEX(6561, 2899,  20),
    COL_VERTEX(6561, 2899, 170),
    COL_VERTEX(6661, 2899, 271),
    COL_VERTEX(6811, 2899, 271),
    COL_VERTEX(6912, 2899, 170),
    COL_VERTEX(6912, 2899,  20),
    COL_VERTEX(6811, 2899, -79),
    COL_VERTEX(6661, 3072, -79),
    COL_VERTEX(6561, 3072,  20),
    COL_VERTEX(6561, 3072, 170),
    COL_VERTEX(6661, 3072, 271),
    COL_VERTEX(6811, 3072, 271),
    COL_VERTEX(6912, 3072, 170),
    COL_VERTEX(6912, 3072,  20),
    COL_VERTEX(6811, 3072, -79),
    COL_TRI_INIT(SURFACE_NO_CAM_COLLISION, 16),
    COL_TRI(1,  0,  8),
    COL_TRI(1,  8,  9),
    COL_TRI(2,  1,  9),
    COL_TRI(2,  9, 10),
    COL_TRI(3,  2, 10),
    COL_TRI(3, 10, 11),
    COL_TRI(4,  3, 11),
    COL_TRI(4, 11, 12),
    COL_TRI(5,  4, 12),
    COL_TRI(5, 12, 13),
    COL_TRI(6,  5, 13),
    COL_TRI(6, 13, 14),
    COL_TRI(7,  6, 14),
    COL_TRI(7, 14, 15),
    COL_TRI(0,  7, 15),
    COL_TRI(0, 15,  8),
    COL_TRI_STOP(),
    COL_END(),
};

//
// Castle
//

static const Collision omm_castle_2_clock_collision[] = {
    COL_INIT(),
    COL_VERTEX_INIT(12),
    COL_VERTEX(-410, 2253, 7400),
    COL_VERTEX(-410, 2253, 7130),
    COL_VERTEX(-410, 2714, 7400),
    COL_VERTEX(-410, 2714, 7130),
    COL_VERTEX(   0, 2253, 7400),
    COL_VERTEX(   0, 2253, 7130),
    COL_VERTEX(   0, 2714, 7400),
    COL_VERTEX(   0, 2714, 7130),
    COL_VERTEX(-410, 2550, 7400),
    COL_VERTEX(-410, 2550, 7130),
    COL_VERTEX(   0, 2550, 7400),
    COL_VERTEX(   0, 2550, 7130),
    COL_TRI_INIT(SURFACE_DEFAULT, 6),
    COL_TRI(2, 1, 0),
    COL_TRI(1, 2, 3),
    COL_TRI(4, 5, 6),
    COL_TRI(6, 5, 7),
    COL_TRI(8, 9, 10),
    COL_TRI(10, 9, 11),
    COL_TRI_STOP(),
    COL_END(),
};

//
// Patches
//

static struct { s32 levelAreaIndex; const Collision *collision; } sOmmSurfaceStaticPatches[] = {
    { LEVEL_AREA_INDEX(LEVEL_LLL, 2), omm_lll_2_volcano_collision },
    { LEVEL_AREA_INDEX(LEVEL_DDD, 2), omm_ddd_2_gap_collision },
    { LEVEL_AREA_INDEX(LEVEL_BITFS, 1), omm_bitfs_1_gap_collision },
    { LEVEL_AREA_INDEX(LEVEL_CASTLE, 2), omm_castle_2_clock_collision },
};

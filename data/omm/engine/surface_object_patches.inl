#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

//
// Door
//

static const Collision omm_door_collision[] = {
    COL_INIT(),
    COL_VERTEX_INIT(8),
    COL_VERTEX(-80, 256,  30),
    COL_VERTEX( 80, 256,  30),
    COL_VERTEX( 80,   0,  30),
    COL_VERTEX(-80,   0,  30),
    COL_VERTEX(-80, 256, -30),
    COL_VERTEX( 80,   0, -30),
    COL_VERTEX( 80, 256, -30),
    COL_VERTEX(-80,   0, -30),
    COL_TRI_INIT(SURFACE_DEFAULT, 4),
    COL_TRI(2, 1, 0),
    COL_TRI(3, 2, 0),
    COL_TRI(6, 5, 4),
    COL_TRI(5, 7, 4),
    COL_TRI_STOP(),
    COL_END(),
};

//
// Thwomp
//

static const Collision omm_thwomp_collision[] = {
    COL_INIT(),
    COL_VERTEX_INIT(8),
    COL_VERTEX(-150,   3, -150),
    COL_VERTEX(+150,   3, -150),
    COL_VERTEX(+150,   3, +150),
    COL_VERTEX(-150,   3, +150),
    COL_VERTEX(-150, 303, -150),
    COL_VERTEX(+150, 303, -150),
    COL_VERTEX(+150, 303, +150),
    COL_VERTEX(-150, 303, +150),
    COL_TRI_INIT(SURFACE_NO_CAM_COLLISION, 12),
    COL_TRI(0, 1, 2),
    COL_TRI(0, 2, 3),
    COL_TRI(4, 7, 6),
    COL_TRI(4, 6, 5),
    COL_TRI(0, 5, 1),
    COL_TRI(0, 4, 5),
    COL_TRI(3, 4, 0),
    COL_TRI(3, 7, 4),
    COL_TRI(2, 7, 3),
    COL_TRI(2, 6, 7),
    COL_TRI(1, 6, 2),
    COL_TRI(1, 5, 6),
    COL_TRI_STOP(),
    COL_END(),
};

//
// Patches
//

static const Collision *sOmmSurfaceObjectPatches[][2] = {
    { door_seg3_collision_0301CE78, omm_door_collision },
    { thwomp_seg5_collision_0500B7D0, omm_thwomp_collision },
    { thwomp_seg5_collision_0500B92C, omm_thwomp_collision },
};

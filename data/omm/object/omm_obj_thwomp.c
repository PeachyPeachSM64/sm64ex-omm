#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Collision
//

const Collision omm_thwomp_collision[] = {
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

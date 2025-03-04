#ifndef OMM_ENGINE_H
#define OMM_ENGINE_H

#include "animation.h"
#include "headers/include/PR/gbi.h"
#include "headers/src/engine/math_util.h"
#include "headers/src/engine/geo_layout.h"
#include "headers/src/engine/graph_node.h"
#include "headers/src/engine/graph_node_o.h"
#include "headers/src/engine/behavior_script.h"
#include "headers/src/engine/level_script.h"
#include "headers/src/engine/surface_load.h"
#include "headers/src/engine/surface_collision.h"
#include "headers/src/game/rendering_graph_node.h"
#include "headers/src/game/save_file.h"
#include "headers/src/game/shadow.h"
#include "headers/src/menu/file_select.h"

//
// Surface Pool
//

typedef struct { OmmArray data; s32 count; } OmmSurfaceArray;
typedef OmmSurfaceArray OmmSurfaceHashMap[NUM_CELLS][NUM_CELLS][3];
extern OmmSurfaceHashMap gOmmSurfaces[2];

#define OMM_NUM_SURFACE_ARRAYS  (sizeof(gOmmSurfaces) / sizeof(OmmSurfaceArray)) // (2 * NUM_CELLS * NUM_CELLS * 3)

#define gOmmFloors(dyn, cx, cz) (&gOmmSurfaces[dyn][cz][cx][SURFACE_FLOOR])
#define gOmmCeils(dyn, cx, cz)  (&gOmmSurfaces[dyn][cz][cx][SURFACE_CEIL])
#define gOmmWalls(dyn, cx, cz)  (&gOmmSurfaces[dyn][cz][cx][SURFACE_WALL])

#endif

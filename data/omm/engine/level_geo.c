#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "game/envfx_snow.h"
#include "game/skybox.h"

Gfx *geo_envfx_main(s32 callContext, struct GraphNode *node, Mat4 mtxf) {
    Gfx *gfx = NULL;
    if (callContext == GEO_CONTEXT_RENDER && gCurGraphNodeCamera != NULL) {
        struct GraphNodeGenerated *execNode = (struct GraphNodeGenerated *) node;
        if (GET_HIGH_U16_OF_32(execNode->parameter) != gAreaUpdateCounter) {
            s32 snowMode = GET_LOW_U16_OF_32(execNode->parameter);
            Vec3s marioPos; vec3f_to_vec3s(marioPos, gPlayerCameraState->pos);
            Vec3s camFrom; vec3f_to_vec3s(camFrom, gCurGraphNodeCamera->pos);
            Vec3s camTo; vec3f_to_vec3s(camTo, gCurGraphNodeCamera->focus);
            Gfx *particleList = envfx_update_particles(snowMode, marioPos, camTo, camFrom);
            if (particleList != NULL) {
                if (snowMode >= ENVFX_BUBBLE_START) {
                    gfx = particleList;
                } else {
                    Mtx *mtx = alloc_display_list(sizeof(Mtx));
                    gfx = alloc_display_list(2 * sizeof(Gfx));
                    mtxf_to_mtx(mtx, mtxf);
                    gSPMatrix(&gfx[0], mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
                    gSPBranchList(&gfx[1], particleList);
                }
                execNode->fnNode.node.flags = (execNode->fnNode.node.flags & 0xFF) | 0x400;
            }
            SET_HIGH_U16_OF_32(execNode->parameter, gAreaUpdateCounter);
        }
    } else if (callContext == GEO_CONTEXT_AREA_INIT) {
        envfx_update_particles(ENVFX_MODE_NONE, gVec3sZero, gVec3sZero, gVec3sZero);
    }
    return gfx;
}

Gfx *geo_skybox_main(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx) {
    Gfx *gfx = NULL;
    struct GraphNodeBackground *backgroundNode = (struct GraphNodeBackground *) node;
    if (callContext == GEO_CONTEXT_AREA_LOAD) {
        backgroundNode->unused = 0;
    } else if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeCamera *camNode = (struct GraphNodeCamera *) gCurGraphNodeRoot->views[0];
        struct GraphNodePerspective *camFrustum = (struct GraphNodePerspective *) camNode->fnNode.node.parent;
        gfx = create_skybox_facing_camera(
            0, backgroundNode->background, camFrustum->fov,
            gLakituState.pos[0], gLakituState.pos[1], gLakituState.pos[2],
            gLakituState.focus[0], gLakituState.focus[1], gLakituState.focus[2]
        );
    }
    return gfx;
}

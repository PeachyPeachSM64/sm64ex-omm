#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Frame interpolation
//

static Vtx *sPaintingPos[MAX_INTERPOLATED_FRAMES][2];
static Vtx  sPaintingVtx[MAX_INTERPOLATED_FRAMES][2 * 264 * 3];
static Vtx  sPaintingPre[MAX_INTERPOLATED_FRAMES][2 * 264 * 3];
static s32  sPaintingCnt[MAX_INTERPOLATED_FRAMES];
static u32  sPaintingTs;

void gfx_patch_interpolated_frame_paintings(s32 k) {
    if (sPaintingPos[k][0]) {
        if (sPaintingPos[k][1]) {
            for (s32 i = 0; i != sPaintingCnt[k]; ++i) {
                s32 h = sPaintingCnt[k] / 2;
                s32 x = i / h;
                s32 y = i % h;
                sPaintingPos[k][x][y] = sPaintingVtx[k][i];
            }
        } else {
            for (s32 i = 0; i != sPaintingCnt[k]; ++i) {
                sPaintingPos[k][0][i] = sPaintingVtx[k][i];
            }
        }
        sPaintingPos[k][0] = NULL;
        sPaintingPos[k][1] = NULL;
        sPaintingCnt[k] = 0;
        sPaintingTs = gGlobalTimer;
    }
}

void gfx_interpolate_painting(Vtx *vtx, s32 numVtx) {
    bool shouldInterpolate = is_frame_interpolation_enabled() && (sPaintingTs >= gGlobalTimer - 1);
    interpolate {
        if (sPaintingCnt[k] >= numVtx * 2) { sPaintingCnt[k] = 0; }
        for (s32 i = 0; i != numVtx; ++i) {
            sPaintingVtx[k][sPaintingCnt[k] + i] = vtx[i];
            interpolate_f32(sPaintingVtx[k][sPaintingCnt[k] + i].n.ob[0], sPaintingPre[k][sPaintingCnt[k] + i].n.ob[0], vtx[i].n.ob[0]);
            interpolate_f32(sPaintingVtx[k][sPaintingCnt[k] + i].n.ob[1], sPaintingPre[k][sPaintingCnt[k] + i].n.ob[1], vtx[i].n.ob[1]);
            interpolate_f32(sPaintingVtx[k][sPaintingCnt[k] + i].n.ob[2], sPaintingPre[k][sPaintingCnt[k] + i].n.ob[2], vtx[i].n.ob[2]);
            interpolate_s32(sPaintingVtx[k][sPaintingCnt[k] + i].n.n [0], sPaintingPre[k][sPaintingCnt[k] + i].n.n [0], vtx[i].n.n [0]);
            interpolate_s32(sPaintingVtx[k][sPaintingCnt[k] + i].n.n [1], sPaintingPre[k][sPaintingCnt[k] + i].n.n [1], vtx[i].n.n [1]);
            interpolate_s32(sPaintingVtx[k][sPaintingCnt[k] + i].n.n [2], sPaintingPre[k][sPaintingCnt[k] + i].n.n [2], vtx[i].n.n [2]);
            sPaintingPre[k][sPaintingCnt[k] + i] = vtx[i];
        }
        sPaintingPos[k][sPaintingCnt[k] / numVtx] = vtx;
    }
    OMM_MEMCPY(vtx, &sPaintingVtx[0][sPaintingCnt[0]], sizeof(Vtx) * numVtx);
    interpolate { sPaintingCnt[k] += numVtx; }
}

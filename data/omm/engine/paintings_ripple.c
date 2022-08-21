#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Frame interpolation
//

typedef struct {
    Vtx vtx[0x400];
    s32 count;
} VtxBuffer;

static Vtx      *sPaintingVtx[8];
static VtxBuffer sPaintingVtx0[8];
static VtxBuffer sPaintingVtx1[8];
static s32       sPaintingCount = 0;

void gfx_interpolate_frame_paintings(f32 t) {
    for (s32 i = 0; i != sPaintingCount; ++i) {
        if (sPaintingVtx[i]) {
            Vtx       *vtx = sPaintingVtx[i];
            VtxBuffer *vb0 = &sPaintingVtx0[i];
            VtxBuffer *vb1 = &sPaintingVtx1[i];
            for (s32 j = 0; j != vb0->count; ++j) {
                vtxn_interpolate((Vtx_tn *) (vtx + j), (Vtx_tn *) (vb0->vtx + j), (Vtx_tn *) (vb1->vtx + j), t);
            }
        }
    }
}

void gfx_clear_frame_paintings() {
    omm_zero(sPaintingVtx, sizeof(sPaintingVtx));
    sPaintingCount = 0;
}

void gfx_interpolate_painting(Vtx *vtx, s32 numVtx) {
    if (gFrameInterpolation && sPaintingCount < 8) {
        VtxBuffer *vb0 = &sPaintingVtx0[sPaintingCount];
        VtxBuffer *vb1 = &sPaintingVtx1[sPaintingCount];

        // Previous frame
        if (vb0->count == numVtx) {
            sPaintingVtx[sPaintingCount] = vtx;
            omm_copy(vb0->vtx, vb1->vtx, sizeof(Vtx) * numVtx);
        } else {
            omm_copy(vb0->vtx, vtx, sizeof(Vtx) * numVtx);
        }
        vb0->count = numVtx;
        
        // Current frame
        omm_copy(vb1->vtx, vtx, sizeof(Vtx) * numVtx);
        vb1->count = numVtx;
        
        // Increase painting count
        sPaintingCount++;
    }
}

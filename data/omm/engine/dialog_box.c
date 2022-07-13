#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

extern s8 gDialogBoxState;
extern s8 gDialogBoxType;
extern s16 gDialogScrollOffsetY;
extern f32 gDialogBoxAngle;
extern f32 gDialogBoxScale;

//
// Frame interpolation
//

typedef struct {
    Gfx *pos;
    Mtx mtx[2];
    f32 pre;
    f32 val;
} Gfxf32;

static struct {
    Gfxf32 off;
    Gfxf32 rot;
    Gfxf32 sca;
} sDialogBox[MAX_INTERPOLATED_FRAMES];

void gfx_patch_interpolated_frame_dialog(s32 k) {
    
    // Offset mode (text scrolling)
    if (sDialogBox[k].off.pos) {
        Gfx *pos = sDialogBox[k].off.pos;
        Mtx *mtx = sDialogBox[k].off.mtx;
        Vec3f t3f = { 0.f, sDialogBox[k].off.val, 0.f };
        mtxf_translate(mtx->m, t3f);
        gSPMatrix(pos, mtx, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
        gDialogScrollOffsetY = sDialogBox[k].off.pre;
        sDialogBox[k].off.pos = NULL;
    }

    // Rotation mode (npc dialog box opening/closing)
    if (sDialogBox[k].rot.pos) {
        Gfx *pos = sDialogBox[k].rot.pos;
        Mtx *mtx = sDialogBox[k].rot.mtx;
        Vec3f s3f = { 1.f / sDialogBox[k].sca.val, 1.f / sDialogBox[k].sca.val, 1.f };
        mtxf_scale_vec3f(mtx->m, gMat4Identity, s3f);
        gSPMatrix(pos++, mtx++, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
        guRotate(mtx, sDialogBox[k].rot.val * 4.f, 0.f, 0.f, 1.f);
        gSPMatrix(pos, mtx, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
        sDialogBox[k].rot.pos = NULL;
    }

    // Zoom mode (signpost dialog box opening/closing)
    if (sDialogBox[k].sca.pos) {
        Gfx *pos = sDialogBox[k].sca.pos;
        Mtx *mtx = sDialogBox[k].sca.mtx;
        Vec3f t3f = { 65.f - (65.f / sDialogBox[k].sca.val), (40.f / sDialogBox[k].sca.val) - 40.f, 0.f };
        mtxf_translate(mtx->m, t3f);
        gSPMatrix(pos++, mtx++, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
        Vec3f s3f = { 1.f / sDialogBox[k].sca.val, 1.f / sDialogBox[k].sca.val, 1.f };
        mtxf_scale_vec3f(mtx->m, gMat4Identity, s3f);
        gSPMatrix(pos, mtx, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
        sDialogBox[k].sca.pos = NULL;
    }
}

//
// Dialog box
//

void render_dialog_box_type(struct DialogEntry *dialog, s8 linesPerBox) {
    static s32 sDialogBoxState = -1;
    bool shouldInterpolate = is_frame_interpolation_enabled() && gDialogBoxState == sDialogBoxState;
    sDialogBoxState = gDialogBoxState;
    create_dl_translation_matrix(MENU_MTX_NOPUSH, dialog->leftOffset, dialog->width, 0);

    // Compute interpolated values and update previous values
    interpolate {
        interpolate_f32(sDialogBox[k].off.val, sDialogBox[k].off.pre, gDialogScrollOffsetY);
        interpolate_f32(sDialogBox[k].rot.val, sDialogBox[k].rot.pre, gDialogBoxAngle);
        interpolate_f32(sDialogBox[k].sca.val, sDialogBox[k].sca.pre, gDialogBoxScale);
        sDialogBox[k].off.pre = gDialogScrollOffsetY;
        sDialogBox[k].rot.pre = gDialogBoxAngle;
        sDialogBox[k].sca.pre = gDialogBoxScale;
    }

    // Set dialog box position
    switch (gDialogBoxType) {

        // Rotation mode
        case 0: {
            if (gDialogBoxState == 0 || gDialogBoxState == 3) {
                interpolate { sDialogBox[k].rot.pos = gDisplayListHead; }
                create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.f / sDialogBox[0].sca.val, 1.f / sDialogBox[0].sca.val, 1.f);
                create_dl_rotation_matrix(MENU_MTX_NOPUSH, sDialogBox[0].rot.val * 4.f, 0.f, 0.f, 1.f);
            }
            gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 150);
        } break;

        // Zoom mode
        case 1: {
            if (gDialogBoxState == 0 || gDialogBoxState == 3) {
                interpolate { sDialogBox[k].sca.pos = gDisplayListHead; }
                create_dl_translation_matrix(MENU_MTX_NOPUSH, 65.f - (65.f / sDialogBox[0].sca.val), (40.f / sDialogBox[0].sca.val) - 40.f, 0.f);
                create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.f / sDialogBox[0].sca.val, 1.f / sDialogBox[0].sca.val, 1.f);
            }
            gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 150);
        } break;
    }

    // Draw dialog box
    create_dl_translation_matrix(MENU_MTX_PUSH, -7.f, 5.f, 0.f);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.1f, ((f32) linesPerBox / 5.f) + 0.1f, 1.f);
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // Offset mode
    if (gDialogBoxState == 2) {
        interpolate { sDialogBox[k].off.pos = gDisplayListHead; }
        if (is_frame_interpolation_enabled()) {
            create_dl_translation_matrix(MENU_MTX_NOPUSH, 0, sDialogBox[0].off.val, 0);
            gDialogScrollOffsetY = 0;
        }
    }
}

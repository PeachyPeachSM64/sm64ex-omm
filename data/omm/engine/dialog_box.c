#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Frame interpolation
//

typedef struct {
    s32 pos;
    s32 state;
    s32 type;
    f32 offset;
    f32 angle;
    f32 scale;
} DialogBoxState;

static DialogBoxState sDialogBox0[1] = { { .type = 0xFF } }; // Prevent the game from interpolating the dialog box on initialization
static DialogBoxState sDialogBox1[1] = { { .type = 0xFF } }; // Prevent the game from interpolating the dialog box on initialization
static Gfx *sDialogBoxPos;
static Mtx sDialogBoxMtx[7];
static s32 sDialogLinesPerBox;

static Gfx *render_dialog_box(Gfx *pos, f32 t);

void gfx_interpolate_frame_dialog(f32 t) {
    if (sDialogBoxPos) {
        render_dialog_box(sDialogBoxPos, t);
    }
}

void gfx_clear_frame_dialog() {
    sDialogBoxPos = NULL;
    gDialogScrollOffsetY = sDialogBox1->offset;
    gDialogBoxState = sDialogBox1->state;
    gDialogTextPos = sDialogBox1->pos;
}

//
// Dialog box
//

static Gfx *render_dialog_box(Gfx *pos, f32 t) {
    switch (sDialogBox0->type) {

        // Rotation mode
        case 0: {
            if (sDialogBox0->state == 0 || sDialogBox0->state == 3) {
                f32 scale = lerp_f(t, sDialogBox0->scale, sDialogBox1->scale);
                f32 angle = lerp_f(t, sDialogBox0->angle, sDialogBox1->angle);
                pos = gfx_create_scale_matrix(pos, &sDialogBoxMtx[0], false, 1.f / scale, 1.f / scale, 1.f);
                pos = gfx_create_rotation_matrix(pos, &sDialogBoxMtx[1], false, angle * 4.f, 0.f, 0.f, 1.f);
            }
            gDPSetEnvColor(pos++, 0, 0, 0, 150);
        } break;

        // Zoom mode
        case 1: {
            if (sDialogBox0->state == 0 || sDialogBox0->state == 3) {
                f32 scale = lerp_f(t, sDialogBox0->scale, sDialogBox1->scale);
                f32 x = 65.f * (1.f - (1.f / scale));
                f32 y = 40.f * ((1.f / scale) - 1.f);
                pos = gfx_create_translation_matrix(pos, &sDialogBoxMtx[2], false, x, y, 0.f);
                pos = gfx_create_scale_matrix(pos, &sDialogBoxMtx[3], false, 1.f / scale, 1.f / scale, 1.f);
            }
            gDPSetEnvColor(pos++, 255, 255, 255, 150);
        } break;
    }

    // Draw dialog box
    pos = gfx_create_translation_matrix(pos, &sDialogBoxMtx[4], true, -7.f, 5.f, 0.f);
    pos = gfx_create_scale_matrix(pos, &sDialogBoxMtx[5], false, 1.1f, ((f32) sDialogLinesPerBox / 5.f) + 0.1f, 1.f);
    gSPDisplayList(pos++, dl_draw_text_bg_box);
    gSPPopMatrix(pos++, G_MTX_MODELVIEW);

    // Offset mode
    if (sDialogBox0->state == 2) {
        f32 y1 = (sDialogBox1->state == 2 ? sDialogBox1->offset : sDialogBox0->offset + 2.f * sDialogLinesPerBox);
        f32 y = lerp_f(t, sDialogBox0->offset, y1);
        pos = gfx_create_translation_matrix(pos, &sDialogBoxMtx[6], false, 0.f, y, 0.f);
        gDialogScrollOffsetY = 0;
        gDialogBoxState = 2;
        gDialogTextPos = sDialogBox0->pos;
    }

    return pos;
}

void render_dialog_box_type(struct DialogEntry *dialog, s8 linesPerBox) {
    create_dl_translation_matrix(MENU_MTX_NOPUSH, dialog->leftOffset, dialog->width, 0);
    bool interpolate = gFrameInterpolation && (sDialogBox1->type == gDialogBoxType);

    // Close dialog box if pressed B
    if (gDialogBoxState == 1 && (gPlayer1Controller->buttonPressed & B_BUTTON)) {
        handle_special_dialog_text(gDialogID);
        gDialogLineNum = 1;
        gDialogResponse = 1;
        gDialogBoxState = 3;
    }

    // Previous frame
    if (interpolate) {
        sDialogBoxPos       = gDisplayListHead;
        sDialogBox0->pos    = sDialogBox1->pos;
        sDialogBox0->state  = sDialogBox1->state;
        sDialogBox0->type   = sDialogBox1->type;
        sDialogBox0->offset = sDialogBox1->offset;
        sDialogBox0->angle  = sDialogBox1->angle;
        sDialogBox0->scale  = sDialogBox1->scale;
    } else {
        sDialogBox0->pos    = gDialogTextPos;
        sDialogBox0->state  = gDialogBoxState;
        sDialogBox0->type   = gDialogBoxType;
        sDialogBox0->offset = gDialogScrollOffsetY;
        sDialogBox0->angle  = gDialogBoxOpenTimer;
        sDialogBox0->scale  = gDialogBoxScale;
    }

    // Current frame
    sDialogBox1->pos    = gDialogTextPos;
    sDialogBox1->state  = gDialogBoxState;
    sDialogBox1->type   = gDialogBoxType;
    sDialogBox1->offset = gDialogScrollOffsetY;
    sDialogBox1->angle  = gDialogBoxOpenTimer;
    sDialogBox1->scale  = gDialogBoxScale;

    // Render dialog box
    sDialogLinesPerBox = linesPerBox;
    gDisplayListHead = render_dialog_box(gDisplayListHead, 0);
}

// Fix a frame-perfect softlock with scrolling textboxes
OMM_ROUTINE_PRE_RENDER(dialog_box_update) {
    if (gDialogID != -1 && !omm_is_game_paused()) {
        if (sDialogBox0->state != sDialogBox1->state) {
            gPlayer1Controller->buttonPressed = 0;
            gPlayer2Controller->buttonPressed = 0;
            gPlayer3Controller->buttonPressed = 0;
        } else {
            gPlayer3Controller->buttonPressed &= ~B_BUTTON;
        }
    }
}

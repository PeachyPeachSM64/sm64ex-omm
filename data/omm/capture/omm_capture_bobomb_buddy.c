#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_bobomb_buddy_init(UNUSED struct Object *o) {
    gOmmObject->state.actionState = 0;
    gOmmObject->state.actionTimer = 15;
    return true;
}

void omm_cappy_bobomb_buddy_end(UNUSED struct Object *o) {
}

f32 omm_cappy_bobomb_buddy_get_top(struct Object *o) {
    return 94.f * o->oScaleY;
}

//
// Update
//

s32 omm_cappy_bobomb_buddy_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;

    // States
    if (gOmmObject->state.actionTimer == 0) {

        // Check cannon
        if (gOmmObject->state.actionState == 0 && omm_mario_lock(gMarioState, -1)) {
            if (cur_obj_nearest_object_with_behavior(bhvCannonClosed)) {
                gOmmObject->state.actionState = 1;
            } else {
                gOmmObject->state.actionState = 5;
            }
        }

        // Start cannon dialog
        else if (gOmmObject->state.actionState == 1) {
            if (obj_dialog_start(gCurrCourseNum == COURSE_BOB ? DIALOG_004 : DIALOG_047)) {
                gOmmObject->state.actionState = 2;
            }
        }

        // Wait for the cannon dialog to end
        else if (gOmmObject->state.actionState == 2) {
            if (obj_dialog_update()) {
                gOmmObject->state.actionState = 3;
                gOmmObject->state.actionTimer = 5;
            }
        }

        // Start cannon opening cutscene
        else if (gOmmObject->state.actionState == 3) {
            if (obj_cutscene_start(CUTSCENE_PREPARE_CANNON, cur_obj_nearest_object_with_behavior(bhvCannonClosed))) {
                save_file_set_cannon_unlocked();
                gOmmObject->state.actionState = 4;
            }
        }

        // Wait for the cannon cutscene to end
        else if (gOmmObject->state.actionState == 4) {
            if (obj_cutscene_update()) {
                gOmmObject->state.actionState = 5;
                gOmmObject->state.actionTimer = 10;
            }
        }

        // Start dialog
        else if (gOmmObject->state.actionState == 5) {
            if (obj_dialog_start(gCurrCourseNum == COURSE_BOB ? DIALOG_105 : DIALOG_106)) {
                gOmmObject->state.actionState = 6;
            }
        }

        // Wait for the dialog to end
        else if (gOmmObject->state.actionState == 6) {
            if (obj_dialog_update()) {
                gOmmObject->state.actionState = 7;
                gOmmObject->state.actionTimer = 5;
            }
        }

        // Release Mario
        else if (gOmmObject->state.actionState == 7 && omm_mario_unlock(gMarioState)) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
            gOmmObject->state.actionState = 8;
        }

    } else {
        gOmmObject->state.actionTimer--;
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_random_blink(o, &o->oBobombBuddyBlinkTimer);

    // Cappy values
    gOmmObject->cappy.offset[1] = 94.f;
    gOmmObject->cappy.scale     = 1.2f;

    // OK
    POBJ_RETURN_OK;
}

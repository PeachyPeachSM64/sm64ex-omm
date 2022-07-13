#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_bobomb_buddy_init(UNUSED struct Object *o) {
    gOmmData->object->state.actionState = 0;
    gOmmData->object->state.actionTimer = 15;
    return true;
}

void cappy_bobomb_buddy_end(UNUSED struct Object *o) {
}

f32 cappy_bobomb_buddy_get_top(struct Object *o) {
    return 94.f * o->oScaleY;
}

//
// Update
//

s32 cappy_bobomb_buddy_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;

    // States
    if (gOmmData->object->state.actionTimer == 0) {

        // Check cannon
        if (gOmmData->object->state.actionState == 0 && omm_mario_lock(gMarioState, -1)) {
            if (cur_obj_nearest_object_with_behavior(bhvCannonClosed) != NULL) {
                gOmmData->object->state.actionState = 1;
            } else {
                gOmmData->object->state.actionState = 5;
            }
        }

        // Start cannon dialog
        else if (gOmmData->object->state.actionState == 1) {
            if (obj_dialog_start(gCurrCourseNum == COURSE_BOB ? DIALOG_004 : DIALOG_047)) {
                gOmmData->object->state.actionState = 2;
            }
        }

        // Wait for the cannon dialog to end
        else if (gOmmData->object->state.actionState == 2) {
            if (obj_dialog_update()) {
                gOmmData->object->state.actionState = 3;
                gOmmData->object->state.actionTimer = 5;
            }
        }

        // Start cannon opening cutscene
        else if (gOmmData->object->state.actionState == 3) {
            if (obj_cutscene_start(CUTSCENE_PREPARE_CANNON, cur_obj_nearest_object_with_behavior(bhvCannonClosed))) {
                save_file_set_cannon_unlocked();
                gOmmData->object->state.actionState = 4;
            }
        }

        // Wait for the cannon cutscene to end
        else if (gOmmData->object->state.actionState == 4) {
            if (obj_cutscene_update()) {
                gOmmData->object->state.actionState = 5;
                gOmmData->object->state.actionTimer = 10;
            }
        }

        // Start dialog
        else if (gOmmData->object->state.actionState == 5) {
            if (obj_dialog_start(gCurrCourseNum == COURSE_BOB ? DIALOG_105 : DIALOG_106)) {
                gOmmData->object->state.actionState = 6;
            }
        }

        // Wait for the dialog to end
        else if (gOmmData->object->state.actionState == 6) {
            if (obj_dialog_update()) {
                gOmmData->object->state.actionState = 7;
                gOmmData->object->state.actionTimer = 5;
            }
        }

        // Release Mario
        else if (gOmmData->object->state.actionState == 7 && omm_mario_unlock(gMarioState)) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
            gOmmData->object->state.actionState = 8;
        }

    } else {
        gOmmData->object->state.actionTimer--;
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_random_blink(o, &o->oBobombBuddyBlinkTimer);

    // Cappy values
    gOmmData->object->cappy.offset[1] = 94.f;
    gOmmData->object->cappy.scale     = 1.2f;

    // OK
    POBJ_RETURN_OK;
}

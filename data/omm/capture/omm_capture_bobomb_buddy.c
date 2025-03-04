#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_bobomb_buddy_init(UNUSED struct Object *o) {
    gOmmObject->state.actionTimer = 15;
    return true;
}

void omm_cappy_bobomb_buddy_end(UNUSED struct Object *o) {
}

u64 omm_cappy_bobomb_buddy_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_BOBOMB_BUDDY_CANNON;
}

f32 omm_cappy_bobomb_buddy_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_bobomb_buddy_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_QUICKSAND;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;
    POBJ_SET_TALKING;

    // States
    if (gOmmObject->state.actionTimer == 0) {

        // Check cannon
        if (gOmmObject->state.actionState == 0 && omm_mario_lock(gMarioState, -1)) {
            if (obj_get_nearest_with_behavior(o, bhvCannonClosed)) {
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
            if (obj_cutscene_start(CUTSCENE_PREPARE_CANNON, obj_get_nearest_with_behavior(o, bhvCannonClosed))) {
                omm_save_file_set_cannon_unlocked(gCurrSaveFileNum - 1, OMM_GAME_MODE, gCurrCourseNum - 1);
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
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6);
            gOmmObject->state.actionState = 8;
        }

    } else {
        gOmmObject->state.actionTimer--;
    }
    pobj_stop_if_unpossessed();

    // OK
    pobj_return_ok;
}

void omm_cappy_bobomb_buddy_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    obj_random_blink(o, &o->oBobombBuddyBlinkTimer);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

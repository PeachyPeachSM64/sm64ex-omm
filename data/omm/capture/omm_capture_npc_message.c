#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_npc_message_init(UNUSED struct Object *o) {
    gOmmObject->state.actionState = 0;
    gOmmObject->state.actionTimer = 15;
    return true;
}

void omm_cappy_npc_message_end(UNUSED struct Object *o) {
}

f32 omm_cappy_npc_message_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_npc_message_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = 50.f;
    o->hitboxHeight = 100.f;
    o->hitboxDownOffset = 0.f;
    o->oWallHitboxRadius = 40.f;

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;

    // States
    if (gOmmObject->state.actionTimer == 0) {

        // Check lock
        if (gOmmObject->state.actionState == 0 && omm_mario_lock(gMarioState, -1)) {
            gOmmObject->state.actionState = 1;
        }

        // Start dialog
        else if (gOmmObject->state.actionState == 1) {
            s16 dialogId = (o->oBhvArgs2ndByte != 0 ? (s16) o->oBhvArgs2ndByte : (s16) o->oToadMessageDialogId);
            if (obj_dialog_start(dialogId)) {
                gOmmObject->state.actionState = 2;
            }
        }

        // Wait for the dialog to end
        else if (gOmmObject->state.actionState == 2) {
            if (obj_dialog_update()) {
                gOmmObject->state.actionState = 3;
                gOmmObject->state.actionTimer = 5;
            }
        }

        // Release Mario
        else if (gOmmObject->state.actionState == 3 && omm_mario_unlock(gMarioState)) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
            gOmmObject->state.actionState = 4;
        }

    } else {
        gOmmObject->state.actionTimer--;
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    o->oOpacity = 0xFF;
    obj_update_gfx(o);

    // Cappy values
    gOmmObject->cappy.offset[1] = omm_capture_get_hitbox_height(o);
    gOmmObject->cappy.offset[2] = omm_capture_get_hitbox_radius(o);
    gOmmObject->cappy.scale     = omm_capture_get_gravity(o);

    // OK
    POBJ_RETURN_OK;
}

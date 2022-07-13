#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_npc_message_init(UNUSED struct Object *o) {
    gOmmData->object->state.actionState = 0;
    gOmmData->object->state.actionTimer = 15;
    return true;
}

void cappy_npc_message_end(UNUSED struct Object *o) {
}

f32 cappy_npc_message_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 cappy_npc_message_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = 50.f;
    o->hitboxHeight = 100.f;
    o->hitboxDownOffset = 0.f;
    o->oWallHitboxRadius = 40.f;

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;

    // States
    if (gOmmData->object->state.actionTimer == 0) {

        // Check lock
        if (gOmmData->object->state.actionState == 0 && omm_mario_lock(gMarioState, -1)) {
            gOmmData->object->state.actionState = 1;
        }

        // Start dialog
        else if (gOmmData->object->state.actionState == 1) {
            s16 dialogId = (o->oBehParams2ndByte != 0 ? (s16) o->oBehParams2ndByte : (s16) o->oToadMessageDialogId);
            if (obj_dialog_start(dialogId)) {
                gOmmData->object->state.actionState = 2;
            }
        }

        // Wait for the dialog to end
        else if (gOmmData->object->state.actionState == 2) {
            if (obj_dialog_update()) {
                gOmmData->object->state.actionState = 3;
                gOmmData->object->state.actionTimer = 5;
            }
        }

        // Release Mario
        else if (gOmmData->object->state.actionState == 3 && omm_mario_unlock(gMarioState)) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
            gOmmData->object->state.actionState = 4;
        }

    } else {
        gOmmData->object->state.actionTimer--;
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    o->oOpacity = 0xFF;
    obj_update_gfx(o);

    // Cappy values
    gOmmData->object->cappy.offset[1] = omm_capture_get_hitbox_height(o);
    gOmmData->object->cappy.offset[2] = omm_capture_get_hitbox_radius(o);
    gOmmData->object->cappy.scale     = omm_capture_get_gravity(o);

    // OK
    POBJ_RETURN_OK;
}

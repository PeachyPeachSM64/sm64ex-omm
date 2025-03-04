#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_npc_message_init(UNUSED struct Object *o) {
    gOmmObject->state.actionTimer = 15;
    return true;
}

void omm_cappy_npc_message_end(UNUSED struct Object *o) {
}

u64 omm_cappy_npc_message_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_BOBOMB_BUDDY;
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
    o->oWallHitboxRadius = 40.f;

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

        // Check lock
        if (gOmmObject->state.actionState == 0 && omm_mario_lock(gMarioState, -1)) {
            gOmmObject->state.actionState = 1;
        }

        // Start dialog
        else if (gOmmObject->state.actionState == 1) {
            s16 dialogId1stByte = (o->oBehParams >> 24) & 0xFF;
            s16 dialogId = (dialogId1stByte != 0 ? dialogId1stByte : o->oBehParams2ndByte);
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
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6);
            gOmmObject->state.actionState = 4;
        }

    } else {
        gOmmObject->state.actionTimer--;
    }
    pobj_stop_if_unpossessed();

    // OK
    pobj_return_ok;
}

void omm_cappy_npc_message_update_gfx(struct Object *o) {

    // Gfx
    o->oOpacity = 0xFF;
    obj_update_gfx(o);
    obj_random_blink(o, &o->oBobombBuddyBlinkTimer);

    // Cappy transform
    gOmmObject->cappy.object = o;
#if OMM_GAME_IS_SMSR
    gOmmObject->cappy.tra_y = 50.f;
    gOmmObject->cappy.tra_z = 8.f;
    gOmmObject->cappy.scale = 1.25f;
    gOmmObject->cappy.o_gfx = true;
#endif
}

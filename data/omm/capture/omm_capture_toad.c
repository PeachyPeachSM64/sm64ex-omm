#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/omm_constants.h"

static const struct { s32 starReq; s16 dialogIdBefore; s16 dialogIdAfter; } OMM_CAPPY_TOAD_PARAMS[] = {
    { TOAD_STAR_1_REQUIREMENT, TOAD_STAR_1_DIALOG, TOAD_STAR_1_DIALOG_AFTER },
    { TOAD_STAR_2_REQUIREMENT, TOAD_STAR_2_DIALOG, TOAD_STAR_2_DIALOG_AFTER },
    { TOAD_STAR_3_REQUIREMENT, TOAD_STAR_3_DIALOG, TOAD_STAR_3_DIALOG_AFTER },
};

//
// Init
//

bool omm_cappy_toad_init(struct Object *o) {
    o->oOpacity = 0xFF;
    if (o->oToadMessageRecentlyTalked) {
        gOmmObject->state.actionState = 3;
    } else {
        gOmmObject->state.actionTimer = 15;
    }
    gOmmObject->toad.dialogId = o->oToadMessageDialogId;
    gOmmObject->toad.spawnStar = -1;

    // Toad dialog and star
    u8 starFlags = omm_stars_get_flags();
    s32 starCount = omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE);
    for (s32 i = 0; i != array_length(OMM_CAPPY_TOAD_PARAMS); ++i) {
        if (gOmmObject->toad.dialogId == OMM_CAPPY_TOAD_PARAMS[i].dialogIdBefore) {
            if (starCount >= OMM_CAPPY_TOAD_PARAMS[i].starReq) {
                if (starFlags & (1u << i)) {
                    gOmmObject->toad.dialogId = OMM_CAPPY_TOAD_PARAMS[i].dialogIdAfter;
                    gOmmObject->toad.spawnStar = -1;
                } else {
                    gOmmObject->toad.dialogId = OMM_CAPPY_TOAD_PARAMS[i].dialogIdBefore;
                    gOmmObject->toad.spawnStar = i;
                }
            }
            break;
        }
    }
    return true;
}

void omm_cappy_toad_end(struct Object *o) {
    obj_anim_play(o, 6, 1.f);
    obj_reset_hitbox(o, 80, 100, 0, 0, 30, 0);
    obj_drop_to_floor(o);
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    o->oOpacity = 0xFF;
    o->oToadMessageState = 3;
    o->oToadMessageRecentlyTalked = 1;
    o->oToadMessageDialogId = gOmmObject->toad.dialogId;
    o->oInteractionSubtype = 0;
}

u64 omm_cappy_toad_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_TOAD;
}

f32 omm_cappy_toad_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_toad_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_OPEN_DOORS;
    POBJ_SET_TALKING * (gOmmObject->state.actionState >= 0 && gOmmObject->state.actionState <= 2);

    // States
    if (gOmmObject->state.actionTimer == 0) {

        // Start dialog
        if (gOmmObject->state.actionState == 0) {
            if (omm_mario_lock(gMarioState, -1) && obj_dialog_start(gOmmObject->toad.dialogId)) {
                audio_play_toads_jingle();
                gOmmObject->state.actionState = 1;
            }
        }

        // Wait for the dialog to end
        else if (gOmmObject->state.actionState == 1) {
            if (obj_dialog_update()) {
                gOmmObject->state.actionState = 2;
                gOmmObject->state.actionTimer = 5;
            }
        }

        // Spawn star
        else if (gOmmObject->state.actionState == 2) {
            if (omm_mario_unlock(gMarioState)) {
                if (gOmmObject->toad.spawnStar != -1) {
                    bhv_spawn_star_no_level_exit(gOmmObject->toad.spawnStar);
                    gOmmObject->toad.dialogId = OMM_CAPPY_TOAD_PARAMS[gOmmObject->toad.spawnStar].dialogIdAfter;
                }
                gOmmObject->state.actionState = 3;
            }
        }

        // Inputs
        else if (pobj_process_inputs(o)) {
            pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
            if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
                obj_play_sound(o, POBJ_SOUND_JUMP_1);
            }
        }

    } else {
        gOmmObject->state.actionTimer = max_s(0, gOmmObject->state.actionTimer - 1);
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // OK
    pobj_return_ok;
}

void omm_cappy_toad_update_gfx(struct Object *o) {
    pobj_freeze_gfx_during_star_dance();

    // Gfx
    obj_update_gfx(o);

    // Jump
    if (!obj_is_on_ground(o)) {
        obj_anim_play(o, 0, 1.f);
        obj_anim_clamp_frame(o, 18, 24);
        o->oGfxAngle[1] += -0x1400;
    }

    // Walk
    else if (POBJ_IS_WALKING) {
        obj_anim_play(o, 1, 1.f);
        obj_anim_set_frame(o, (((s32) (o->oAnimFrame + 10)) % 18) + 8);
        f32 vn = sqrtf(sqr_f(o->oVelX) + sqr_f(o->oVelZ));
        o->oGfxAngle[1] += 0x8000;
        o->oGfxPos[0] -= (o->oVelX / vn) * 9.78f * (16.36f + o->oAnimFrame);
        o->oGfxPos[2] -= (o->oVelZ / vn) * 9.78f * (16.36f + o->oAnimFrame);
    }

    // Idle
    else {
        obj_anim_play(o, 4, 1.f);
        o->oGfxAngle[1] += -0x1400;
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}

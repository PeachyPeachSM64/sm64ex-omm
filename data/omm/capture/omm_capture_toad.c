#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

/* Stars requirement, Star index, Dialog before star, Dialog after star */
static const u32 sCappyToadParams[][4] = {
    { 12, 0, DIALOG_082, DIALOG_154 },
    { 25, 1, DIALOG_076, DIALOG_155 },
    { 35, 2, DIALOG_083, DIALOG_156 },
};

//
// Init
//

bool omm_cappy_toad_init(struct Object *o) {
    if (o->oBhvArgs2ndByte) return false;
    gOmmObject->state.actionState = (o->oToadMessageRecentlyTalked ? 3 : 0);
    gOmmObject->state.actionTimer = (o->oToadMessageRecentlyTalked ? 0 : 15);
    gOmmObject->toad.dialogId = o->oToadMessageDialogId;
    gOmmObject->toad.spawnStar = -1;
    o->oOpacity = 255;

    // Toad dialog and star
    u32 saveFlags = save_file_get_flags();
    u32 starCount = save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1);
    for (s8 i = 0; i != 3; ++i) {
        if ((u32) gOmmObject->toad.dialogId == sCappyToadParams[i][2]) {
            if (starCount >= sCappyToadParams[i][0]) {
                if (saveFlags & (1u << (24u + sCappyToadParams[i][1]))) {
                    gOmmObject->toad.dialogId = sCappyToadParams[i][3];
                    gOmmObject->toad.spawnStar = -1;
                } else {
                    gOmmObject->toad.dialogId = sCappyToadParams[i][2];
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
    o->oOpacity = 81;
    o->oToadMessageState = 0;
    o->oToadMessageRecentlyTalked = 1;
    o->oToadMessageDialogId = gOmmObject->toad.dialogId;
}

f32 omm_cappy_toad_get_top(struct Object *o) {
    return 100.f * o->oScaleY;
}

//
// Update
//

s32 omm_cappy_toad_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;

    // States
    if (!obj_update_door(o) && gOmmObject->state.actionTimer == 0) {

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
                    gOmmObject->toad.dialogId = sCappyToadParams[gOmmObject->toad.spawnStar][3];
                }
                gOmmObject->state.actionState = 3;
            }
        }

        // Inputs
        else if (!omm_mario_is_locked(gMarioState)) {
            pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
            if (pobj_jump(o, 0, 1) == POBJ_RESULT_JUMP_START) {
                obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
            }
        }

    } else {
        gOmmObject->state.actionTimer = max_s(0, gOmmObject->state.actionTimer - 1);
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS(

    // Doors
    obj_open_door(o, obj);

    );
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);

    // Jump
    if (!obj_is_on_ground(o)) {
        obj_anim_play(o, 0, 1.f);
        obj_anim_clamp_frame(o, 18, 24);
        o->oGfxAngle[1] += -0x1400;
        gOmmObject->cappy.offset[0] = +12.f;
        gOmmObject->cappy.offset[1] = 106.f;
        gOmmObject->cappy.offset[2] = -16.f;
        gOmmObject->cappy.angle[0]  = -0x1000;
        gOmmObject->cappy.angle[1]  = +0x1800;
        gOmmObject->cappy.scale     = 1.f;
    }

    // Run
    else if (o->oForwardVel > 1.f) {
        obj_anim_play(o, 1, 1.f);
        obj_anim_set_frame(o, (((s32) (obj_anim_get_frame(o) + 10)) % 18) + 8);
        f32 vn = sqrtf(sqr_f(o->oVelX) + sqr_f(o->oVelZ));
        o->oGfxAngle[1] += 0x8000;
        o->oGfxPos[0]   -= (o->oVelX / vn) * 9.78f * (16.36f + obj_anim_get_frame(o));
        o->oGfxPos[2]   -= (o->oVelZ / vn) * 9.78f * (16.36f + obj_anim_get_frame(o));
        gOmmObject->cappy.offset[0] = +8.f;
        gOmmObject->cappy.offset[1] = 98.f;
        gOmmObject->cappy.angle[0]  = -0x400;
        gOmmObject->cappy.angle[2]  = +0x400;
        gOmmObject->cappy.scale     = 1.f;
    }

    // Idle
    else {
        obj_anim_play(o, 4, 1.f);
        o->oGfxAngle[1] += -0x1400;
        gOmmObject->cappy.offset[1] = 94.f;
        gOmmObject->cappy.offset[2] = -10.f;
        gOmmObject->cappy.angle[0]  = -0x800;
        gOmmObject->cappy.scale     = 1.f;
    }

    // OK
    POBJ_RETURN_OK;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static bool is_ghost_hunt_boo(struct Object *o) {
    return
        o->behavior == bhvGhostHuntBoo &&
        o->oBooParentBigBoo != NULL;
}

static bool is_merry_go_round_boo(struct Object *o) {
    return
        o->behavior == bhvMerryGoRoundBoo &&
        o->parentObj != NULL &&
        o->parentObj->behavior == bhvMerryGoRoundBooManager;
}

static bool is_boo_with_cage(struct Object *o) {
    return
        o->behavior == bhvBooWithCage;
}

//
// Init
//

bool cappy_boo_init(struct Object *o) {
    if (o->oAction != 1 &&
        o->oAction != 2 &&
        o->oAction != 5) {
        return false;
    }

    gOmmData->object->state.actionState = 0;
    gOmmData->object->state.actionTimer = 0;
    gOmmData->object->state.actionFlag = false;
    gOmmData->object->boo.gfxOffsetY = 0.f;
    o->oBooTargetOpacity = 255;
    return true;
}

void cappy_boo_end(struct Object *o) {
    o->oTimer = 0;
    o->oAction = 6;
    o->oBooTargetOpacity = 255;
    o->oGfxPos[1] -= gOmmData->object->boo.gfxOffsetY;
    obj_play_sound(o, SOUND_OBJ_BOO_LAUGH_LONG);

    // Ghost Hunt/Merry Go Round boo
    // Remove its link with the manager/big boo,
    // and set its behavior param as a regular boo
    if (is_ghost_hunt_boo(o) || is_merry_go_round_boo(o)) {
        o->parentObj = o;
        o->oBooParentBigBoo = NULL;
        o->oBehParams &= 0xFF00FFFF;
        o->oBehParams |= 0x00010000;
        o->oBehParams2ndByte = 1;
    }
}

f32 cappy_boo_get_top(struct Object *o) {
    return 80.f * o->oBooBaseScale;
}

//
// Update
//

static void cappy_boo_update_once(struct Object *o) {
    if (gOmmData->object->state.actionFlag) {
        return;
    }

    // Ghost Hunt boo
    // Mark it as dead
    if (is_ghost_hunt_boo(o)) {
        o->oBooParentBigBoo->oBigBooNumMinionBoosKilled++;
        if (o->oBooParentBigBoo->oBigBooNumMinionBoosKilled == 5) {
            audio_play_puzzle_jingle();
        }
    }

    // Merry-Go-Round boo
    // Mark it as dead
    // The jingle is handled by the manager
    if (is_merry_go_round_boo(o)) {
        o->parentObj->oMerryGoRoundBooManagerNumBoosKilled++;
    }

    // Boo with cage
    // Trigger the BBH warp
    if (is_boo_with_cage(o)) {
        gMarioState->usedObj = o;
        gMarioState->interactObj = o;
        level_trigger_warp(gMarioState, WARP_OP_UNKNOWN_02);
    }

    // Collect the coin inside the boo
    for_each_object_with_behavior(coin, bhvCoinInsideBoo) {
        if (coin->parentObj == o) {
            switch (obj_get_coin_type(coin)) {
                case 0:  coin->oDamageOrCoinValue = 1; break;
                case 1:  coin->oDamageOrCoinValue = 2; break;
                case 2:  coin->oDamageOrCoinValue = 5; break;
                default: coin->oDamageOrCoinValue = 0; break;
            }
            omm_mario_interact_coin(gMarioState, coin);
            obj_mark_for_deletion(coin);
        }
    }

    // Done
    gOmmData->object->state.actionFlag = true;
}

static void cappy_boo_update_opacity_and_scale(struct Object *o) {

    // Opacity
    if (o->oOpacity < o->oBooTargetOpacity) {
        o->oOpacity = min_s(o->oOpacity + 20, o->oBooTargetOpacity);
    } else {
        o->oOpacity = max_s(o->oOpacity - 20, o->oBooTargetOpacity);
    }

    // Scale
    f32 scale = (0.75f + 0.25f * ((o->oOpacity - 55.f) / 200.f)) * o->oBooBaseScale;
    o->oScaleX = scale * (1.f + sins(o->oBooOscillationTimer) * 0.06f);
    o->oScaleY = scale * (1.f - sins(o->oBooOscillationTimer) * 0.06f);
    o->oScaleZ = scale * (1.f + sins(o->oBooOscillationTimer) * 0.06f);
    o->oGfxPos[1] += sins(o->oBooOscillationTimer / 2) * 6.f;
    o->oBooOscillationTimer += 0x400;
}

s32 cappy_boo_update(struct Object *o) {
    cappy_boo_update_once(o);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    bool moveThroughWalls = (gOmmData->object->state.actionState != 0);
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_IMMUNE_TO_FIRE * moveThroughWalls;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_MOVE_THROUGH_WALLS * moveThroughWalls;

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        pobj_move(o, gOmmData->object->state.actionState, false, false);
        if (pobj_jump(o, 0, 6) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, SOUND_OBJ_MR_BLIZZARD_ALERT);
        }

        // Move through walls + small speed boost
        // Duration: 30 frames + 15 frames CD
        if (POBJ_B_BUTTON_PRESSED) {
            if (gOmmData->object->state.actionTimer == 0) {
                gOmmData->object->state.actionTimer = 45;
                play_sound(SOUND_GENERAL_VANISH_SFX, o->oCameraToObject);
            }
        }
    }

    // States
    gOmmData->object->state.actionState = (gOmmData->object->state.actionTimer / 15) != 0;
    gOmmData->object->state.actionTimer = max_s(0, gOmmData->object->state.actionTimer - 1);
    o->oBooTargetOpacity = 255 - (200 * gOmmData->object->state.actionState);

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    o->oIntangibleTimer = -1 * gOmmData->object->state.actionState;
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    cappy_boo_update_opacity_and_scale(o);
    if (o->oDistToFloor < 50.f) {
        gOmmData->object->boo.gfxOffsetY = (50.f - (o->oDistToFloor));
    } else {
        gOmmData->object->boo.gfxOffsetY = max_f(0.f, gOmmData->object->boo.gfxOffsetY - 2.f);
    }
    o->oGfxPos[1] += gOmmData->object->boo.gfxOffsetY;

    // Cappy values
    gOmmData->object->cappy.copyGfx   = true;
    gOmmData->object->cappy.offset[1] = 50.f;
    gOmmData->object->cappy.offset[2] = 8.f;
    gOmmData->object->cappy.scale     = 1.25f * !is_boo_with_cage(o);

    // OK
    POBJ_RETURN_OK;
}

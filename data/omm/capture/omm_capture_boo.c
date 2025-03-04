#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

OMM_INLINE bool is_ghost_hunt_boo(struct Object *o) {
    return o->behavior == bhvGhostHuntBoo && o->oBooParentBigBoo;
}

OMM_INLINE bool is_merry_go_round_boo(struct Object *o) {
    return o->behavior == bhvMerryGoRoundBoo && o->parentObj && o->parentObj->behavior == bhvMerryGoRoundBooManager;
}

OMM_INLINE bool is_boo_with_cage(struct Object *o) {
    return o->behavior == bhvBooWithCage;
}

//
// Init
//

bool omm_cappy_boo_init(struct Object *o) {
    if (o->oAction != 1 &&
        o->oAction != 2 &&
        o->oAction != 5) {
        return false;
    }

    o->oNumLootCoins = 0;
    o->oBooTargetOpacity = 255;

    // Collect the coin inside the boo
    struct Object *coin = obj_get_first_with_behavior_and_parent(o, bhvCoinInsideBoo);
    if (coin) {
        switch (obj_get_coin_type(coin)) {
            case 0:  o->oNumLootCoins = 1; break; // Yellow coin
            case 1:  o->oNumLootCoins = 2; break; // Red coin
            case 2:  o->oNumLootCoins = -1; break; // Blue coin
            default: o->oNumLootCoins = 0; break;
        }
        obj_mark_for_deletion(coin);
    }
    return true;
}

void omm_cappy_boo_end(struct Object *o) {
    o->oTimer = 0;
    o->oAction = 6;
    o->oNumLootCoins = 0;
    o->oBooTargetOpacity = 255;
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

u64 omm_cappy_boo_get_type(struct Object *o) {
    return is_boo_with_cage(o) ? OMM_CAPTURE_BIG_BOO : OMM_CAPTURE_BOO;
}

f32 omm_cappy_boo_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o) * (o->oBooBaseScale / o->oScaleY);
}

//
// Update
//

static void omm_cappy_boo_update_once(struct Object *o) {
    if (gOmmObject->state.actionFlag) {
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

    // Collect the coins from the Boo
    if (o->oNumLootCoins != 0) {
        obj_collect_coins(o, o->oNumLootCoins);
        o->oNumLootCoins = 0;
    }

    // Done
    gOmmObject->state.actionFlag = true;
}

static void omm_cappy_boo_update_opacity_and_scale(struct Object *o) {

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

s32 omm_cappy_boo_update(struct Object *o) {
    omm_cappy_boo_update_once(o);
    obj_scale(o, o->oBooBaseScale);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    bool moveThroughWalls = (gOmmObject->state.actionState != 0);
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_FLOATING;
    POBJ_SET_INTANGIBLE * moveThroughWalls;
    POBJ_SET_IMMUNE_TO_FIRE * moveThroughWalls;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_MOVE_THROUGH_WALLS * moveThroughWalls;
    POBJ_SET_ABLE_TO_OPEN_DOORS * !moveThroughWalls;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, gOmmObject->state.actionState, false, false);
        if (pobj_jump(o, 6) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_MR_BLIZZARD);
        }

        // Move through walls + small speed boost
        // Duration: 30 frames + 15 frames CD
        if (POBJ_B_BUTTON_PRESSED) {
            if (gOmmObject->state.actionTimer == 0) {
                gOmmObject->state.actionTimer = 45;
                obj_play_sound(o, POBJ_SOUND_VANISH);
            }
        }
    }

    // States
    gOmmObject->state.actionState = (gOmmObject->state.actionTimer / 15) != 0;
    gOmmObject->state.actionTimer = max_s(0, gOmmObject->state.actionTimer - 1);
    o->oBooTargetOpacity = 255 - (200 * gOmmObject->state.actionState);

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

void omm_cappy_boo_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    omm_cappy_boo_update_opacity_and_scale(o);

    // Cappy transform
    gOmmObject->cappy.tra_y = 50.f;
    gOmmObject->cappy.tra_z = 8.f;
    gOmmObject->cappy.scale = 1.25f * !is_boo_with_cage(o);
    gOmmObject->cappy.o_gfx = true;
}

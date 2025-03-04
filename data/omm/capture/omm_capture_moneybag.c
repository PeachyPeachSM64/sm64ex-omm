#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_moneybag_init(struct Object *o) {
    if (o->oAction == MONEYBAG_ACT_DISAPPEAR ||
        o->oAction == MONEYBAG_ACT_DEATH) {
        return false;
    }

    if (o->parentObj && (o->oAction == MONEYBAG_ACT_APPEAR || o->oAction == MONEYBAG_ACT_UNUSED_APPEAR)) {
        obj_mark_for_deletion(o->parentObj);
        o->parentObj = NULL;
    }
    o->oOpacity = 0xFF;
    obj_anim_play(o, 0, 1.f);
    return true;
}

void omm_cappy_moneybag_end(struct Object *o) {
    o->oOpacity = 0xFF;
    o->oHomeY = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
    o->oAction = MONEYBAG_ACT_MOVE_AROUND;
    o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
    obj_anim_play(o, 2, 1.f);
}

u64 omm_cappy_moneybag_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_MONEY_BAG;
}

f32 omm_cappy_moneybag_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_moneybag_update(struct Object *o) {
    bool wasOnGround = obj_is_on_ground(o);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_BOING_2);
        }

        // Throw a coin
        // Hold B to charge, throw a stronger, faster, wider blue coin at max charge
        pobj_charge_attack(15, o, 120, 60);
        pobj_release_attack(15,
            if (omm_obj_spawn_moneybag_coin(o, _power_)) {
                gOmmObject->state.actionFlag = true;
            } else {
                play_sound(POBJ_SOUND_WRONG, gGlobalSoundArgs);
            }
        );
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

    // Animation, sound and particles
    if (gOmmObject->state.actionFlag) {
        obj_anim_play(o, 3, 1.f);
        if (obj_anim_is_at_end(o)) {
            gOmmObject->state.actionFlag = false;
        }
    } else if (obj_is_on_ground(o)) {
        if (!wasOnGround) {
            obj_anim_play_with_sound(o, 1, 1.f, 0, true);
        } else if (o->oAnimID != 1 || obj_anim_is_past_frame(o, 4) || POBJ_IS_WALKING) {
            obj_anim_play(o, POBJ_IS_WALKING ? 4 : 0, 1.f);
        }
    } else if (o->oVelY <= 0.f) {
        obj_anim_play(o, 2, 1.f);
    } else if (wasOnGround) {
        obj_anim_play_with_sound(o, 1, 1.f, 0, true);
        obj_anim_set_frame(o, 4);
    } else if (o->oAnimID != 1 || obj_anim_is_at_end(o)) {
        obj_anim_play(o, 2, 1.f);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_moneybag_update_gfx(struct Object *o) {

    // Gfx
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

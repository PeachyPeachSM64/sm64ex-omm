#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "levels/ssl/header.h"

#define OMM_CAPPY_THWOMP_ACTION_DEFAULT     (0)
#define OMM_CAPPY_THWOMP_ACTION_RISING      (1)
#define OMM_CAPPY_THWOMP_ACTION_FALLING     (2)
#define OMM_CAPPY_THWOMP_ACTION_LANDING     (3)

static void omm_cappy_thwomp_adjust_grindel_yaw(struct Object *o, s32 initUpdateEnd) {
    if (o->behavior == bhvHorizontalGrindel) {
        switch (initUpdateEnd) {
            case 0: o->oFaceAngleYaw -= 0x4000; break;
            case 1: o->oGfxAngle[1] += 0x4000; break;
            case 2: break;
        }
    } else if (obj_has_geo_layout(o, ssl_geo_000734)) {
        switch (initUpdateEnd) {
            case 0: o->oFaceAngleYaw -= 0x4000; break;
            case 1: o->oGfxAngle[1] += 0x4000; break;
            case 2: o->oFaceAngleYaw = o->oMoveAngleYaw += 0x4000; break;
        }
    }
}

//
// Init
//

bool omm_cappy_thwomp_init(struct Object *o) {
    omm_cappy_thwomp_adjust_grindel_yaw(o, 0);
    return true;
}

void omm_cappy_thwomp_end(struct Object *o) {
    omm_cappy_thwomp_adjust_grindel_yaw(o, 2);
    o->oVelY = 0.f;
    o->oHomeY = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
    if (o->behavior == bhvHorizontalGrindel) {
        o->oHorizontalGrindelTargetYaw = o->oMoveAngleYaw;
        o->oHorizontalGrindelOnGround = FALSE;
    } else {
        o->oAction = 2;
    }
}

u64 omm_cappy_thwomp_get_type(struct Object *o) {
    if (o->behavior == bhvThwomp) return OMM_CAPTURE_THWOMP;
    if (o->behavior == bhvThwomp2) return OMM_CAPTURE_THWOMP;
    if (o->behavior == bhvGrindel) return OMM_CAPTURE_GRINDEL;
    return 0;
}

f32 omm_cappy_thwomp_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_thwomp_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_CANNON * (gOmmObject->state.actionState == OMM_CAPPY_THWOMP_ACTION_DEFAULT);
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;
    POBJ_SET_ATTACKING_BREAKABLE * !obj_is_on_ground(o);
    POBJ_SET_GROUND_POUNDING;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 0) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Big jump and ground pound
        if (POBJ_B_BUTTON_PRESSED) {
            gOmmObject->state.actionState = OMM_CAPPY_THWOMP_ACTION_RISING;
            gOmmObject->state.actionTimer = 0;
            if (POBJ_NONZERO_ANALOG) {
                o->oFaceAngleYaw = gOmmMario->capture.stickYaw;
                gOmmObject->state.actionFlag = true;
            } else {
                gOmmObject->state.actionFlag = false;
            }
            o->oVelY = pobj_get_jump_velocity(o) * 2.f;
            o->oFloor = NULL;
            omm_mario_lock(gMarioState, -1);
            obj_play_sound(o, POBJ_SOUND_JUMP_KING_BOBOMB);
        }
    }

    // Thwomp attack
    switch (gOmmObject->state.actionState) {
        case OMM_CAPPY_THWOMP_ACTION_RISING: {
            omm_mario_lock(gMarioState, -1);
            pobj_apply_gravity(o, 1.f);
            if (o->oVelY <= 0.f) {
                gOmmObject->state.actionState = OMM_CAPPY_THWOMP_ACTION_FALLING;
                obj_set_forward_and_y_vel(o, 0.f, 0.f);
            } else if (gOmmObject->state.actionFlag) {
                obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, pobj_get_walk_speed(o));
            }
        } break;

        case OMM_CAPPY_THWOMP_ACTION_FALLING: {
            omm_mario_lock(gMarioState, -1);
            pobj_apply_gravity(o, 1.f);
            if (obj_is_on_ground(o)) {
                obj_spawn_white_puff(o, POBJ_SOUND_THWOMP);
                cur_obj_shake_screen(SHAKE_POS_LARGE);
                gOmmObject->state.actionState = OMM_CAPPY_THWOMP_ACTION_LANDING;
                omm_obj_spawn_whomp_shockwave(o, o->oPosX, o->oPosY, o->oPosZ);
            }
        } break;

        case OMM_CAPPY_THWOMP_ACTION_LANDING: {
            omm_mario_lock(gMarioState, -1);
            if (gOmmObject->state.actionTimer++ == 20) {
                gOmmObject->state.actionState = OMM_CAPPY_THWOMP_ACTION_DEFAULT;
                omm_mario_unlock(gMarioState);
            }
        } break;
    }

    // Movement
    pobj_hop(o, 2.f);
    bool wasOnGround = obj_is_on_ground(o);
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    if (!wasOnGround && obj_is_on_ground(o)) {
        obj_play_sound(o, POBJ_SOUND_POUNDING);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_thwomp_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    omm_cappy_thwomp_adjust_grindel_yaw(o, 1);

    // Cappy transform
    f32 height = omm_capture_get_hitbox_height(o) / o->oScaleY;
    gOmmObject->cappy.tra_y = height;
    gOmmObject->cappy.scale = height / 120.f;
}

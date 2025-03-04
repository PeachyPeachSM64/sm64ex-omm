#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_snufit_init(struct Object *o) {
    obj_scale(o, 1.f);
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    o->oPosY -= 60.f * o->oScaleY;
    o->oSnufitScale = 1.f;
    o->oSnufitBodyScale = 1000.f;
    return true;
}

void omm_cappy_snufit_end(struct Object *o) {
    obj_scale(o, 1.f);
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    o->oPosY += 60.f * o->oScaleY;
    o->oAction = SNUFIT_ACT_IDLE;
    o->oSnufitRecoil = 0;
    o->oSnufitBullets = 0;
    o->oSnufitScale = 1.f;
    o->oSnufitBodyScale = 1000.f;
    o->oSnufitBodyScalePeriod = 0x8000;
    o->oSnufitBodyBaseScale = 167;

    // Compute new home pos, so the snufit pos matches its orbit
    o->oHomeX = o->oPosX - 100.f * coss(o->oSnufitCircularPeriod);
    o->oHomeY = o->oPosY -   8.f * coss(4000 * gGlobalTimer);
    o->oHomeZ = o->oPosZ - 100.f * sins(o->oSnufitCircularPeriod);
}

u64 omm_cappy_snufit_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_SNUFIT;
}

f32 omm_cappy_snufit_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_snufit_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_FLOATING;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Shoot 3 balls
        if (POBJ_B_BUTTON_PRESSED) {
            omm_obj_spawn_snufit_ball(o, 0, (random_u16() & 15) == 0);
            omm_obj_spawn_snufit_ball(o, 3, (random_u16() & 15) == 0);
            omm_obj_spawn_snufit_ball(o, 6, (random_u16() & 15) == 0);
        }

        // Hold B for at least 1 second to start to rapid-fire
        if (POBJ_B_BUTTON_DOWN) {
            if (gOmmObject->state.actionTimer++ >= 15) {
                gOmmObject->state.actionState = 1;
                if (gOmmObject->state.actionTimer & 1) {
                    omm_obj_spawn_snufit_ball(o, 0, (random_u16() & 15) == 0);
                }
            }
        } else {
            gOmmObject->state.actionState = 0;
            gOmmObject->state.actionTimer = 0;
        }
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

void omm_cappy_snufit_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    o->oGfxPos[1] += 60.f * o->oScaleY + 8.f * coss(4000 * gGlobalTimer);

    // Cappy transform
    gOmmObject->cappy.tra_y = 30.f;
    gOmmObject->cappy.scale = 0.8f;
    gOmmObject->cappy.o_gfx = true;
}

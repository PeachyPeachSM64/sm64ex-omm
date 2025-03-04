#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_fly_guy_init(struct Object* o) {
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    obj_scale(o, 1.5f);
    obj_update_gfx(o);
    return true;
}

void omm_cappy_fly_guy_end(struct Object *o) {
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    obj_scale(o, 1.5f);
    obj_update_gfx(o);
    o->oFlyGuyLungeYDecel = 0;
    o->oFlyGuyLungeTargetPitch = 0;
    o->oFlyGuyTargetRoll = 0;
    o->oAction = 0;
}

u64 omm_cappy_fly_guy_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_FLY_GUY;
}

f32 omm_cappy_fly_guy_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_fly_guy_update(struct Object *o) {

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

        // Propeller states
        if (obj_is_on_ground(o)) {
            gOmmObject->state.actionState = 0;
        } else if (gOmmObject->state.actionState == 1 && o->oVelY < 0.f) {
            gOmmObject->state.actionState = 2;
        }

        // Move and jump
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Propeller jump
        else if (POBJ_A_BUTTON_PRESSED && !obj_is_on_ground(o) && (gOmmObject->state.actionState == 0)) {
            o->oVelY = pobj_get_jump_velocity(o) * 2.5f;
            omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_1, o->oCameraToObject);
            gOmmObject->state.actionState = 1;
        }

        // Float
        if (POBJ_A_BUTTON_DOWN && !obj_is_on_ground(o) && o->oVelY < 0.f) {
            if (!gOmmObject->state.actionFlag) {
                omm_sound_play(OMM_SOUND_EFFECT_PROPELLER_2, o->oCameraToObject);
            }
            gOmmObject->state.actionFlag = true;
        } else {
            gOmmObject->state.actionFlag = false;
        }

        // Fireball
        if (POBJ_B_BUTTON_PRESSED) {
            omm_obj_spawn_fly_guy_fireball(o);
        }
    }
    pobj_stop_if_unpossessed();

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, gOmmObject->state.actionFlag ? 0.25f : 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();
    if (POBJ_IS_BOUNCING || POBJ_IS_STAR_DANCING) {
        gOmmObject->state.actionState = 0;
    }

    // Animation, sound and particles
    if (gOmmObject->state.actionState == 1) { // Propeller jump
        spawn_object(o, MODEL_NONE, bhvMistParticleSpawner);
        obj_anim_play(o, 0, 4.f);
    } else {
        obj_anim_play(o, 0, 1.f);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_fly_guy_update_gfx(struct Object *o) {

    // Gfx
    vec3f_copy(o->oGfxPos, &o->oPosX);
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

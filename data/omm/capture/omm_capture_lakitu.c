#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_LAKITU_DURATION   (600)
#define OMM_CAPPY_LAKITU_FLICKER    (90)

//
// Init
//

bool omm_cappy_lakitu_init(struct Object *o) {
    if (o->oAction == ENEMY_LAKITU_ACT_UNINITIALIZED) {
        return false;
    }

    // Throw spiny
    if (o->prevObj) {
        o->oSubAction = ENEMY_LAKITU_SUB_ACT_NO_SPINY;
        o->oEnemyLakituSpinyCooldown = 100 + (random_u16() % 100);
        o->oEnemyLakituFaceForwardCountdown = 0;
        o->prevObj = NULL;
    }

    gOmmObject->state.actionTimer = OMM_CAPPY_LAKITU_DURATION;
    return true;
}

void omm_cappy_lakitu_end(struct Object *o) {
    obj_anim_play(o, 1, 1.f);
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    o->oDistanceToMario = 0;
    o->oForwardVel = 0;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
}

u64 omm_cappy_lakitu_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_LAKITU;
}

f32 omm_cappy_lakitu_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_lakitu_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_FLOATING;
    POBJ_SET_ABLE_TO_MOVE_ON_WATER;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);

        // Time's up
        if (gOmmObject->state.actionTimer-- <= 0) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 0);
            obj_destroy(o);
            pobj_stop_if_unpossessed();
        }

        // Move up, fight gravity
        if (POBJ_A_BUTTON_DOWN) {
            o->oVelY = min_f(o->oVelY + pobj_get_gravity(o) * -3.f, pobj_get_jump_velocity(o));
        }

        // Prepare spiny
        pobj_charge_attack(15, o->oHeldProjectile, 120, 0,
            obj_anim_play(o, 3, 2.f);
            if (!o->oHeldProjectile) {
                o->oHeldProjectile = omm_obj_spawn_spiny_ball(o);
            }
            o->oHeldProjectile->oObjectPower = _power_;
            obj_scale(o->oHeldProjectile, o->oScaleY * min_f(2.f * _power_, 1.f));
            gOmmObject->state.actionState = 1;
        );

        // Throw spiny
        pobj_release_attack(15,
            obj_anim_play_with_sound(o, 2, 1.5f, POBJ_SOUND_THROW_LAKITU, true);
            o->oHeldProjectile = NULL;
            gOmmObject->state.actionState = 2;
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
    obj_play_sound(o, POBJ_SOUND_LAKITU_FLY);
    switch (gOmmObject->state.actionState) {
        case 0: obj_anim_play(o, 1, 1.f); break;
        case 1: obj_anim_extend(o); break;
        case 2: if (obj_anim_is_near_end(o)) { gOmmObject->state.actionState = 0; } break;
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_lakitu_update_gfx(struct Object *o) {
    if (gOmmObject->state.actionState != 0) {
        pobj_freeze_gfx_during_star_dance();
    }

    // Gfx
    obj_update_gfx(o);
    obj_update_blink_state(o, &o->oEnemyLakituBlinkTimer, 20, 40, 4);
    if (!POBJ_IS_STAR_DANCING && gOmmObject->state.actionTimer <= OMM_CAPPY_LAKITU_FLICKER && (gOmmObject->state.actionTimer & 1) == 0) {
        o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    } else {
        o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}

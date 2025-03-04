#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_mr_blizzard_init(struct Object *o) {
    if (o->oAction != MR_BLIZZARD_ACT_SPAWN_SNOWBALL &&
        o->oAction != MR_BLIZZARD_ACT_THROW_SNOWBALL &&
        o->oAction != MR_BLIZZARD_ACT_ROTATE &&
        o->oAction != MR_BLIZZARD_ACT_JUMP) {
        return false;
    }

    // Remove the snowball from its hand
    if (o->oMrBlizzardHeldObj) {
        obj_mark_for_deletion(o->oMrBlizzardHeldObj);
        o->oMrBlizzardHeldObj = NULL;
        o->prevObj = NULL;
    }
    o->oMrBlizzardGraphYOffset = 24.f;
    o->oFaceAngleRoll = 0;
    o->oGraphYOffset = o->oMrBlizzardGraphYOffset - 40.f * (1.f - o->oMrBlizzardScale);
    obj_scale(o, o->oMrBlizzardScale);
    return true;
}

void omm_cappy_mr_blizzard_end(struct Object *o) {

    // Turns the Mr. Blizzard into a snowball thrower one
    o->oBehParams2ndByte = MR_BLIZZARD_STYPE_NO_CAP;
    o->oAction = MR_BLIZZARD_ACT_SPAWN_SNOWBALL;
    o->oFaceAngleRoll = 0;
    o->oMrBlizzardDizziness = 0.f;
    o->oMrBlizzardChangeInDizziness = 0.f;
    obj_anim_play(o, 0, 1.f);

    // Remove the snowball from its hand and create a new fresh one
    if (o->oMrBlizzardHeldObj) {
        obj_mark_for_deletion(o->oMrBlizzardHeldObj);
        o->oMrBlizzardHeldObj = NULL;
        o->prevObj = NULL;
    }
    o->oMrBlizzardHeldObj = spawn_object_relative(0, -70, (s16) (o->oMrBlizzardGraphYOffset + 153.f), 0, o, MODEL_WHITE_PARTICLE, bhvMrBlizzardSnowball);
    o->prevObj = o->oMrBlizzardHeldObj;
}

u64 omm_cappy_mr_blizzard_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_MR_BLIZZARD;
}

f32 omm_cappy_mr_blizzard_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_mr_blizzard_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 0) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_WALKING_SNOW_SAND);
            obj_play_sound(o, POBJ_SOUND_JUMP_MR_BLIZZARD);
            obj_spawn_particles(o, 8, MODEL_WHITE_PARTICLE, 0, 8, 4, 15, 8, -3, 0.4f, 0.3f);
        }

        // Prepare snowball
        pobj_charge_attack(15, o->oHeldProjectile, 120, -20,
            if (!o->oHeldProjectile) {
                o->oHeldProjectile = omm_obj_spawn_snowball(o);
            }
            o->oHeldProjectile->oObjectPower = _power_;
            obj_scale(o->oHeldProjectile, o->oScaleY * lerp_f(o->oHeldProjectile->oObjectPower, 1.f, 3.f));
        );

        // Throw snowball
        pobj_release_attack(15,
            obj_anim_play_with_sound(o, 1, 1.5f, POBJ_SOUND_JUMP_2, true);
            o->oHeldProjectile = NULL;
            gOmmObject->state.actionState = 1;
        );
    }

    // Movement
    if (pobj_hop(o, 2.f)) {
        obj_play_sound(o, POBJ_SOUND_WALKING_SNOW_SAND);
        obj_spawn_particles(o, 8, MODEL_WHITE_PARTICLE, 0, 8, 4, 15, 8, -3, 0.4f, 0.3f);
    }
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    obj_anim_play(o, gOmmObject->state.actionState, gOmmObject->state.actionState ? 1.5f : 1);
    obj_anim_clamp_frame(o, 5 + 10 * (1 - gOmmObject->state.actionState), 127);
    if (gOmmObject->state.actionState && obj_anim_is_near_end(o)) {
        gOmmObject->state.actionState = 0;
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_mr_blizzard_update_gfx(struct Object *o) {
    pobj_freeze_gfx_during_star_dance();

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

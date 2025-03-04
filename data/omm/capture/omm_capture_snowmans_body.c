#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_snowmans_body_init(struct Object *o) {
    if (o->oAction > 2) {
        return false;
    }

    if (o->oAction != 0) {
        gOmmObject->state.actionState = 3;
    } else {
        gOmmObject->state.actionTimer = 15;
    }
    gOmmObject->snowmans_body.peakHeight = o->oPosY;
    gOmmObject->snowmans_body.headFound = false;
    return true;
}

void omm_cappy_snowmans_body_end(struct Object *o) {
    if (gOmmObject->snowmans_body.headFound) {
        o->oAction = 2;
        o->oSnowmansBodyScale = 1.f;
        return;
    }

    // Explode and respawn
    pobj_reset_home();
    f32 scale = o->oScaleX;
    obj_spawn_particles(o, 16, MODEL_WHITE_PARTICLE, 200 * scale, 0, 60 * scale, -30 * scale, 90 * scale, -4.f * scale, 3.f * scale, 1.f * scale);
    obj_destroy(o);
}

u64 omm_cappy_snowmans_body_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_SNOWMAN;
}

f32 omm_cappy_snowmans_body_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_snowmans_body_update(struct Object *o) {

    // Snowman's head
    struct Object *snowmansHead = obj_get_nearest_with_behavior(o, bhvSnowmansHead);
    f32 distToHead = (snowmansHead ? obj_get_distance(o, snowmansHead) : (LEVEL_BOUNDARY_MAX * 2.f));

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o) * (distToHead < 500.f ? 0.1f : 1.f);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;
    POBJ_SET_ATTACKING_BREAKABLE;
    POBJ_SET_GROUND_POUNDING;
    POBJ_SET_TALKING * (gOmmObject->state.actionState >= 0 && gOmmObject->state.actionState <= 2);

    // States
    if (gOmmObject->state.actionTimer == 0) {

        // Start dialog
        if (gOmmObject->state.actionState == 0) {
            if (omm_mario_lock(gMarioState, -1) && obj_dialog_start(DIALOG_110)) {
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

        // Unlock Mario
        else if (gOmmObject->state.actionState == 2) {
            if (omm_mario_unlock(gMarioState)) {
                gOmmObject->state.actionState = 3;
            }
        }

        // Inputs
        else if (pobj_process_inputs(o)) {
            pobj_move(o, false, false, false);
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
    o->oFaceAnglePitch += (o->oForwardVel / o->oScaleX) * 0x40;

    // Peak height
    if (obj_is_on_ground(o)) {
        f32 diff = (gOmmObject->snowmans_body.peakHeight - o->oPosY);
        gOmmObject->snowmans_body.peakHeight = o->oPosY;

        // Break the snowman if fell from too high
        if (diff > 1000.f && !POBJ_IS_TALKING) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6);
            pobj_return_unpossess;
        }
    }

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Snowman's head
    // Distance must be computed again after movement
    distToHead = (snowmansHead ? obj_get_distance(o, snowmansHead) : (LEVEL_BOUNDARY_MAX * 2.f));
    if (distToHead < 280.f && o->oScaleX == 1.f && obj_is_on_ground(o)) {
        gOmmObject->snowmans_body.headFound = true;
        omm_mario_unpossess_object_with_yaw(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6, obj_get_object1_angle_yaw_to_object2(snowmansHead, o));
        gMarioState->invincTimer = 30;
        pobj_return_unpossess;
    }

    // Scale
    if (obj_is_on_ground(o)) {
        f32 scaleInc = 0.002f * POBJ_ABS_FORWARD_VEL / pobj_get_walk_speed(o);
        obj_scale(o, min_f(o->oScaleX + scaleInc, 1.f));
    }

    // Animation, sound and particles
    if (obj_is_on_ground(o) && obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, 400.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALKING_SNOW_SAND, OBJ_PARTICLE_NONE)) {
        f32 scale = o->oScaleX;
        obj_spawn_particles(o, 8, MODEL_WHITE_PARTICLE, 0, 10 * scale, 5 * scale, 20 * scale, 10 * scale, -2 * sqrtf(scale), 0.5f * scale, 0.25f * scale);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_snowmans_body_update_gfx(struct Object *o) {

    // Gfx
    o->oMoveAnglePitch = o->oFaceAnglePitch;
    o->oGraphYOffset = o->oScaleX * 180.f;
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.tra_y = +360.f * coss(o->oFaceAnglePitch);
    gOmmObject->cappy.tra_z = -360.f * sins(o->oFaceAnglePitch);
    gOmmObject->cappy.rot_x = -o->oFaceAnglePitch;
    gOmmObject->cappy.scale = 2.f;
}

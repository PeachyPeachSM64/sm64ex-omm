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

    gOmmObject->state.actionState = (o->oAction == 0 ?  0 : 3);
    gOmmObject->state.actionTimer = (o->oAction == 0 ? 15 : 0);
    gOmmObject->state.initialPos[0] = o->oHomeX;
    gOmmObject->state.initialPos[1] = o->oHomeY;
    gOmmObject->state.initialPos[2] = o->oHomeZ;
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
    o->oHomeX = gOmmObject->state.initialPos[0];
    o->oHomeY = gOmmObject->state.initialPos[1];
    o->oHomeZ = gOmmObject->state.initialPos[2];
    f32 scale = o->oScaleX;
    obj_spawn_particles(o, 16, MODEL_WHITE_PARTICLE, 200 * scale, 0, 60 * scale, -30 * scale, 90 * scale, -4.f * scale, 3.f * scale, 1.f * scale);
    obj_destroy(o);
}

f32 omm_cappy_snowmans_body_get_top(struct Object *o) {
    return 360.f * o->oScaleY;
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
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o) * (distToHead < 500.f ? 0.1f : 1.f);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_WIND;
    POBJ_SET_ATTACKING;

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
        else if (!omm_mario_is_locked(gMarioState)) {
            pobj_move(o, false, false, false);
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

    // Peak height
    if (obj_is_on_ground(o)) {
        f32 diff = (gOmmObject->snowmans_body.peakHeight - o->oPosY);
        gOmmObject->snowmans_body.peakHeight = o->oPosY;

        // Break the snowman if fell from too high
        if (diff > 1000.f) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
            POBJ_RETURN_UNPOSSESS;
        }
    }

    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Snowman's head
    // Distance must be computed again after movement
    distToHead = (snowmansHead ? obj_get_distance(o, snowmansHead) : (LEVEL_BOUNDARY_MAX * 2.f));
    if (distToHead < 285.f && o->oScaleX == 1.f && obj_is_on_ground(o)) {
        gOmmObject->snowmans_body.headFound = true;
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, true, 6);
        gMarioState->invincTimer = 30;
        POBJ_RETURN_UNPOSSESS;
    }

    // Scale
    if (obj_is_on_ground(o)) {
        f32 scaleInc = (o->oForwardVel / (omm_capture_get_walk_speed(o))) * 0.002f;
        o->oScaleX = min_f(o->oScaleX + scaleInc, 1.f);
        o->oScaleY = o->oScaleX;
        o->oScaleZ = o->oScaleX;
    }
    f32 scale = o->oScaleX;

    // Gfx
    o->oFaceAnglePitch += (o->oForwardVel / scale) * 0x40;
    o->oMoveAnglePitch = o->oFaceAnglePitch;
    o->oGraphYOffset = scale * 180.0f;
    obj_update_gfx(o);
    if (obj_is_on_ground(o)) {
        f32 walkDistance = gOmmObject->state.walkDistance;
        obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, 400.f, o->oForwardVel, SOUND_OBJ_SNOW_SAND1, OBJ_PARTICLE_NONE);
        if (gOmmObject->state.walkDistance < walkDistance) {
            obj_spawn_particles(o, 8, MODEL_WHITE_PARTICLE, 0, 10 * scale, 5 * scale, 20 * scale, 10 * scale, -2 * sqrtf(scale), 0.5f * scale, 0.25f * scale);
        }
    }

    // Cappy values
    gOmmObject->cappy.offset[1] = +360.f * coss(o->oFaceAnglePitch);
    gOmmObject->cappy.offset[2] = -360.f * sins(o->oFaceAnglePitch);
    gOmmObject->cappy.angle[0]  = -o->oFaceAnglePitch;
    gOmmObject->cappy.scale     = 2.f;

    // OK
    POBJ_RETURN_OK;
}

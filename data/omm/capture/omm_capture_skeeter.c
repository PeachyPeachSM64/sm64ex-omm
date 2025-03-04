#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_skeeter_init(UNUSED struct Object *o) {
    return true;
}

void omm_cappy_skeeter_end(struct Object *o) {
    o->oHomeY = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
    o->oAction = SKEETER_ACT_IDLE;
    o->oSkeeterTurnAwayFromWall = 0;
    o->oSkeeterUnkFC = 10.f;
    o->oSkeeterUnk1AC = 0;
}

u64 omm_cappy_skeeter_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_SKEETER;
}

f32 omm_cappy_skeeter_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_skeeter_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_MOVE_ON_WATER;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {

        // Sliding
        if (gOmmObject->state.actionState) {
            pobj_move(o, true, POBJ_B_BUTTON_DOWN, false);
        } else {
            pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
        }

        // Jump
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_2);
        }
    }

    // Friction
    if (o->oFloorType == OBJ_FLOOR_TYPE_WATER) {
        vec3f_set(gOmmObject->state.friction, 0.98f, 0.98f, 0.98f);
    } else {
        vec3f_set(gOmmObject->state.friction, 0.80f, 0.80f, 0.80f);
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Sliding state
    if (o->oFloorType == OBJ_FLOOR_TYPE_WATER) {
        gOmmObject->state.actionState = 1;
    } else if (o->oFloorType == OBJ_FLOOR_TYPE_GROUND) {
        gOmmObject->state.actionState = 0;
    }

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    bool actionFlag = POBJ_NONZERO_ANALOG;
    if (o->oFloorType == OBJ_FLOOR_TYPE_WATER) {
        if (!POBJ_IS_WALKING) {
            obj_anim_play(o, 1, 1.f);
        } else if ((actionFlag && !gOmmObject->state.actionFlag) || o->oAnimID != 0) {
            obj_anim_play_with_sound(o, 0, 1.f, POBJ_SOUND_SHALLOW_WATER_SPLASH, true);
        } else {
            spawn_object_relative_with_scale(0, -130, 0, -190, 0.5f, o, MODEL_SMALL_WATER_SPLASH, bhvWaterDropletSplash);
            spawn_object_relative_with_scale(0, +130, 0, -190, 0.5f, o, MODEL_SMALL_WATER_SPLASH, bhvWaterDropletSplash);
            spawn_object_relative_with_scale(0, -180, 0, +130, 0.5f, o, MODEL_SMALL_WATER_SPLASH, bhvWaterDropletSplash);
            spawn_object_relative_with_scale(0, +180, 0, +130, 0.5f, o, MODEL_SMALL_WATER_SPLASH, bhvWaterDropletSplash);
            obj_play_sound(o, POBJ_SOUND_RIDING_SHELL_WATER);
        }
    } else if (o->oFloorType == OBJ_FLOOR_TYPE_GROUND) {
        if (POBJ_IS_WALKING) {
            obj_anim_play(o, 2, max_f(1.f, (o->oVelY <= 0.f) * POBJ_ABS_FORWARD_VEL * 1.5f / pobj_get_walk_speed(o)));
            obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 12.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALK_SKEETER, OBJ_PARTICLE_NONE);
        } else {
            obj_anim_play(o, 3, 1.f);
        }
    } else {
        obj_anim_play(o, 2, 1.f);
    }
    gOmmObject->state.actionFlag = actionFlag;

    // OK
    pobj_return_ok;
}

void omm_cappy_skeeter_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

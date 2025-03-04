#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_cappy_penguin_small_change_behavior(struct Object *o) {
    if (o->behavior == bhvPenguinBaby) {
        o->behavior = bhvSmallPenguin;
    }
}

//
// Init
//

bool omm_cappy_penguin_small_init(struct Object *o) {
    omm_cappy_penguin_small_change_behavior(o);
    return true;
}

void omm_cappy_penguin_small_end(struct Object *o) {
    omm_cappy_penguin_small_change_behavior(o);
    o->oHeldState = HELD_FREE;
    o->oAction = 0;
}

u64 omm_cappy_penguin_small_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_SMALL_PENGUIN;
}

f32 omm_cappy_penguin_small_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_penguin_small_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_MOVE_ON_WATER * (gOmmObject->state.actionState == 1 && obj_is_on_ground(o) && !obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ)));
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS * (gOmmObject->state.actionState == 0);

    // Stop sliding
    if (gOmmObject->state.actionState == 1 && !POBJ_IS_RUNNING && obj_is_on_ground(o)) {
        gOmmObject->state.actionState = 2;
        omm_mario_lock(gMarioState, 10);
    }

    // Inputs
    if (pobj_process_inputs(o)) {
        gOmmObject->state.actionState = (gOmmObject->state.actionState & 1);
        bool isSliding = gOmmObject->state.actionState == 1;

        // Max speed multiplier
        if (obj_is_on_ground(o)) {
            if ((gMarioState->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE) {
                o->oSmallPenguinUnk104 = 1.80f;
            } else switch (mario_get_floor_class(gMarioState)) {
                case SURFACE_CLASS_NOT_SLIPPERY:  o->oSmallPenguinUnk104 = 0.75f; break;
                case SURFACE_CLASS_DEFAULT:       o->oSmallPenguinUnk104 = 1.00f; break;
                case SURFACE_CLASS_SLIPPERY:      o->oSmallPenguinUnk104 = 1.25f; break;
                case SURFACE_CLASS_VERY_SLIPPERY: o->oSmallPenguinUnk104 = 1.50f; break;
            }
        }

        // Move and jump
        pobj_move_max_vel(o, pobj_get_max_speed(o, false, isSliding, false) * (isSliding ? o->oSmallPenguinUnk104 : 1.f));
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Start/stop sliding
        if (POBJ_B_BUTTON_PRESSED) {
            if (!isSliding) {
                obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, clamp_f(o->oForwardVel * 1.5f, pobj_get_walk_speed(o) * 1.5f, pobj_get_dash_speed(o) * o->oSmallPenguinUnk104));
                o->oVelY = max_f(o->oVelY, pobj_get_jump_velocity(o) / 2.f);
                o->oFloor = NULL;
                gOmmObject->state.actionState = 1;
                omm_mario_lock(gMarioState, 10);
                obj_play_sound(o, POBJ_SOUND_PENGUIN_DIVE);
            } else if (obj_is_on_ground(o)) {
                gOmmObject->state.actionState = 2;
                omm_mario_lock(gMarioState, 10);
            }
        }
    }

    // Friction
    switch (gOmmObject->state.actionState) {
        case 0: vec3f_set(gOmmObject->state.friction, 0.80f, 0.80f, 0.80f); break;
        case 1: vec3f_set(gOmmObject->state.friction, 0.99f, 0.99f, 0.99f); break;
        case 2: vec3f_set(gOmmObject->state.friction, 0.70f, 0.70f, 0.70f); break;
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions(

    // Tuxie's mother
    // Trigger a dialog when approaching
    if (obj->behavior == bhvTuxiesMother && o->behavior == bhvSmallPenguin && obj_is_on_ground(o)) {
        if (obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX)) {
            obj->prevObj = o;
            obj->oAction = 1;
            obj->oSubAction = 0;
            obj->oInteractStatus = INT_STATUS_INTERACTED;
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
        }
    }

    );
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    switch (gOmmObject->state.actionState) {
        case 0: obj_anim_play(o, (!obj_is_on_ground(o) || POBJ_IS_WALKING) ? 0 : 3, 1.0f); break;
        case 1: obj_anim_play(o, 1, 1.5f); break;
        case 2: obj_anim_play(o, 2, 2.0f); break;
    }
    if (gOmmObject->state.actionState <= 1 && obj_is_on_ground(o)) {
        bool isSliding = gOmmObject->state.actionState == 1;
        obj_make_step_sound_and_particle(o,
            &gOmmObject->state.walkDistance,
            !isSliding * pobj_get_walk_speed(o) * 9.f,
            !isSliding * POBJ_ABS_FORWARD_VEL,
             isSliding ? POBJ_SOUND_SLIDING_GROUND : POBJ_SOUND_WALK_PENGUIN,
             isSliding * OBJ_PARTICLE_MIST
        );
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_penguin_small_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

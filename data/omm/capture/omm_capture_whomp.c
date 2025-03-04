#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_WHOMP_ACTION_DEFAULT          (0)
#define OMM_CAPPY_WHOMP_ACTION_BEGIN_ATTACK     (1)
#define OMM_CAPPY_WHOMP_ACTION_ATTACK           (2)
#define OMM_CAPPY_WHOMP_ACTION_FALLING          (3)
#define OMM_CAPPY_WHOMP_ACTION_LAND             (4)
#define OMM_CAPPY_WHOMP_ACTION_END_ATTACK       (5)

//
// Init
//

bool omm_cappy_whomp_init(struct Object *o) {
    if (o->oAction != 0 &&
        o->oAction != 1 &&
        o->oAction != 2 &&
        o->oAction != 3 &&
        o->oAction != 6 &&
        o->oAction != 7) {
        return false;
    }

    if (o->oAction == 6) {
        gOmmObject->state.actionState = OMM_CAPPY_WHOMP_ACTION_END_ATTACK;
    }
    return true;
}

void omm_cappy_whomp_end(struct Object *o) {
    o->oAction = 1;
    o->oSubAction = 0;
    o->oTimer = 0;
    o->oFaceAnglePitch = 0;
    o->oNumLootCoins = 0;
}

u64 omm_cappy_whomp_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_WHOMP;
}

f32 omm_cappy_whomp_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_whomp_update(struct Object *o) {

    // Collect the remaining coins from the Whomp
    if (o->oNumLootCoins != 0) {
        obj_collect_coins(o, o->oNumLootCoins);
        o->oNumLootCoins = 0;
    }

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Body slam
        if (POBJ_B_BUTTON_PRESSED) {
            gOmmObject->state.actionState = OMM_CAPPY_WHOMP_ACTION_BEGIN_ATTACK;
            o->oVelY = pobj_get_jump_velocity(o) * (obj_is_on_ground(o) ? 1.f : 1.5f);
            obj_anim_play_with_sound(o, 1, 3.f, 0, true);
        }
    }

    // Hitbox
    bool bodySlam = (gOmmObject->state.actionState != OMM_CAPPY_WHOMP_ACTION_DEFAULT);
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o) * (bodySlam ? 0.25f : 1.f);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    bool falling = (gOmmObject->state.actionState == OMM_CAPPY_WHOMP_ACTION_FALLING || gOmmObject->state.actionState == OMM_CAPPY_WHOMP_ACTION_LAND);
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_CANNON * !bodySlam;
    POBJ_SET_INVULNERABLE * bodySlam;
    POBJ_SET_IMMUNE_TO_FIRE * bodySlam;
    POBJ_SET_ATTACKING_BREAKABLE * falling;
    POBJ_SET_GROUND_POUNDING * falling;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Actions
    switch (gOmmObject->state.actionState) {

        // Begin Body slam
        case OMM_CAPPY_WHOMP_ACTION_BEGIN_ATTACK: {
            omm_mario_lock(gMarioState, -1);
            if (obj_anim_is_near_end(o)) {
                gOmmObject->state.actionState = OMM_CAPPY_WHOMP_ACTION_ATTACK;
            }
        } break;

        // Body slam
        case OMM_CAPPY_WHOMP_ACTION_ATTACK: {
            omm_mario_lock(gMarioState, -1);
            o->oFaceAnglePitch = min_s(o->oFaceAnglePitch + 0x800, 0x4000);
            if (o->oFaceAnglePitch == 0x4000) {
                gOmmObject->state.actionState = OMM_CAPPY_WHOMP_ACTION_FALLING;
            } else {
                pobj_apply_gravity(o, 1.f);
            }
        } break;

        // Falling
        case OMM_CAPPY_WHOMP_ACTION_FALLING: {
            omm_mario_lock(gMarioState, -1);
            if (obj_is_on_ground(o)) {
                f32 x = o->oPosX + sins(o->oFaceAngleYaw) * omm_capture_get_hitbox_height(o) / 2.f;
                f32 y = o->oPosY;
                f32 z = o->oPosZ + coss(o->oFaceAngleYaw) * omm_capture_get_hitbox_height(o) / 2.f;
                obj_spawn_white_puff_at(x, y, z, POBJ_SOUND_WHOMP);
                cur_obj_shake_screen(SHAKE_POS_LARGE);
                gOmmObject->state.actionState = OMM_CAPPY_WHOMP_ACTION_LAND;
                gOmmObject->state.actionTimer = 0;
                omm_obj_spawn_whomp_shockwave(o, x, y, z);
            }
        } break;

        // Landing
        case OMM_CAPPY_WHOMP_ACTION_LAND: {
            omm_mario_lock(gMarioState, -1);
            if (gOmmObject->state.actionTimer++ == 20) {
                gOmmObject->state.actionState = OMM_CAPPY_WHOMP_ACTION_END_ATTACK;
            }
        } break;

        // Getting up
        case OMM_CAPPY_WHOMP_ACTION_END_ATTACK: {
            omm_mario_lock(gMarioState, -1);
            o->oFaceAnglePitch = max_s(o->oFaceAnglePitch - 0x400, 0);
            if (o->oFaceAnglePitch == 0 && omm_mario_unlock(gMarioState)) {
                gOmmObject->state.actionState = OMM_CAPPY_WHOMP_ACTION_DEFAULT;
            }
        } break;

        // Default
        default: {
            obj_anim_play(o, 0, max_f(1.f, (o->oVelY <= 0.f) * POBJ_ABS_FORWARD_VEL * 3.f / pobj_get_walk_speed(o)));
            if (obj_is_on_ground(o)) {
                obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 9.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_POUNDING, OBJ_PARTICLE_NONE);
            }
        } break;
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_whomp_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

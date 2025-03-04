#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_BULLY_ACT_MOVE                (0)
#define OMM_CAPPY_BULLY_ACT_CHARGE              (1)
#define OMM_CAPPY_BULLY_ACT_GROUND_POUND_START  (2)
#define OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL   (3)
#define OMM_CAPPY_BULLY_ACT_GROUND_POUND_LAND   (4)

//
// Init
//

bool omm_cappy_bully_init(struct Object *o) {
    if (o->oAction == BULLY_ACT_LAVA_DEATH ||
        o->oAction == BULLY_ACT_DEATH_PLANE_DEATH) {
        return false;
    }

    return true;
}

void omm_cappy_bully_end(struct Object *o) {
    obj_anim_play(o, 1, 1.f);
    obj_set_vel(o, 0.f, 0.f, 0.f);
    o->oAction = BULLY_ACT_PATROL;
}

u64 omm_cappy_bully_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_BULLY;
}

f32 omm_cappy_bully_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_bully_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND * (gOmmObject->state.actionState < OMM_CAPPY_BULLY_ACT_GROUND_POUND_START);
    POBJ_SET_AFFECTED_BY_CANNON * (gOmmObject->state.actionState < OMM_CAPPY_BULLY_ACT_GROUND_POUND_START);
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES * (gOmmObject->state.actionState == OMM_CAPPY_BULLY_ACT_CHARGE);
    POBJ_SET_ABLE_TO_OPEN_DOORS;
    POBJ_SET_ATTACKING_BREAKABLE * (gOmmObject->state.actionState >= OMM_CAPPY_BULLY_ACT_CHARGE);
    POBJ_SET_GROUND_POUNDING * (gOmmObject->state.actionState >= OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL);

    // Inputs
    if (pobj_process_inputs(o)) {
        gOmmObject->state.actionTimer--;
        switch (gOmmObject->state.actionState) {
            case OMM_CAPPY_BULLY_ACT_MOVE: {
                pobj_move(o, false, false, false);
                if (POBJ_B_BUTTON_PRESSED) {
                    if (!obj_is_on_ground(o)) {
                        gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_GROUND_POUND_START;
                        obj_play_sound(o, POBJ_SOUND_THROW_1);
                    } else if (gOmmObject->state.actionTimer <= 0) {
                        gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_CHARGE;
                        gOmmObject->state.actionTimer = 90;
                    }
                } else if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
                    obj_play_sound(o, POBJ_SOUND_JUMP_1);
                }
            } break;

            case OMM_CAPPY_BULLY_ACT_CHARGE: {
                if (gOmmMario->capture.stickMag < 0.1f) {
                    gOmmMario->capture.stickYaw = o->oFaceAngleYaw;
                }
                gOmmMario->capture.stickMag = 1.f;
                pobj_move(o, false, true, false);
                if (gOmmObject->state.actionTimer > 0) {
                    if (POBJ_B_BUTTON_PRESSED && !obj_is_on_ground(o)) {
                        gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_GROUND_POUND_START;
                        gOmmObject->state.actionTimer = 0;
                        obj_play_sound(o, POBJ_SOUND_THROW_1);
                    } else if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
                        obj_play_sound(o, POBJ_SOUND_JUMP_1);
                    }
                } else {
                    gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_MOVE;
                    gOmmObject->state.actionTimer = 15;
                }
            } break;

            case OMM_CAPPY_BULLY_ACT_GROUND_POUND_START: {
                obj_set_vel(o, 0.f, 0.f, 0.f);
                if (o->oAnimFrame >= 6) {
                    gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL;
                }
            } break;

            case OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL: {
                obj_set_vel(o, 0.f, pobj_get_terminal_velocity(o), 0.f);
                if (obj_is_on_ground(o)) {
                    cur_obj_shake_screen(SHAKE_POS_MEDIUM);
                    obj_spawn_white_puff(o, POBJ_SOUND_BULLY);
                    omm_obj_spawn_whomp_shockwave(o, o->oPosX, o->oPosY, o->oPosZ);
                    gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_GROUND_POUND_LAND;
                }
            } break;

            case OMM_CAPPY_BULLY_ACT_GROUND_POUND_LAND: {
                omm_mario_lock(gMarioState, -1);
                obj_set_forward_vel(o, 0, 0, 0);
            } break;
        }
    }

    // Ground pound land
    else if (gOmmObject->state.actionState == OMM_CAPPY_BULLY_ACT_GROUND_POUND_LAND) {
        if (obj_anim_is_at_end(o)) {
            omm_mario_unlock(gMarioState);
            gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_MOVE;
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

    // Activate the Big Bully if that's the last minion alive
    if (o->oBullySubtype == BULLY_STYPE_MINION &&
        o->parentObj != NULL &&
        o->parentObj->oAction == BULLY_ACT_INACTIVE &&
        o->parentObj->oBullyKBTimerAndMinionKOCounter >= 2) {
        o->parentObj->oBullyKBTimerAndMinionKOCounter = 3;
    }

    // Animation, sound and particles
    switch (gOmmObject->state.actionState) {
        case OMM_CAPPY_BULLY_ACT_MOVE: {
            obj_anim_play(o, 0, relerp_0_1_f(POBJ_ABS_FORWARD_VEL, 0.f, pobj_get_walk_speed(o), 0.75f, 1.5f));
            if (obj_is_on_ground(o)) {
                obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 8.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALK_BULLY_1, OBJ_PARTICLE_NONE);
            }
        } break;

        case OMM_CAPPY_BULLY_ACT_CHARGE: {
            obj_anim_play(o, 1, 2.f);
            if (obj_is_on_ground(o)) {
                obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 8.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALK_BULLY_2, OBJ_PARTICLE_NONE);
            }
            obj_make_step_sound_and_particle(o, NULL, 0, 0, 0, OBJ_PARTICLE_MIST); o->oPosY += 60.f * o->oScaleY;
            obj_make_step_sound_and_particle(o, NULL, 0, 0, 0, OBJ_PARTICLE_SMOKE); o->oPosY -= 60.f * o->oScaleY;
        } break;

        case OMM_CAPPY_BULLY_ACT_GROUND_POUND_START: {
            obj_anim_play(o, 2, 1.f);
        } break;

        case OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL: {
            obj_anim_play(o, 2, 1.f);
            obj_anim_set_frame(o, 6);
            obj_make_step_sound_and_particle(o, NULL, 0, 0, 0, OBJ_PARTICLE_MIST);
        } break;

        case OMM_CAPPY_BULLY_ACT_GROUND_POUND_LAND: {
            obj_anim_play(o, 2, 1.f);
        } break;
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_bully_update_gfx(struct Object *o) {
    if (gOmmObject->state.actionState == OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL) {
        pobj_freeze_gfx_during_star_dance();
    }

    // Gfx
    obj_update_gfx(o);
    if (gOmmObject->state.actionState == OMM_CAPPY_BULLY_ACT_CHARGE) {
        o->oGfxAngle[0] = 0x800;
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}

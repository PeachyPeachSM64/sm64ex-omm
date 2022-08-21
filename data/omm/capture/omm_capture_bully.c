#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

enum {
    OMM_CAPPY_BULLY_ACT_MOVE,
    OMM_CAPPY_BULLY_ACT_CHARGE,
    OMM_CAPPY_BULLY_ACT_GROUND_POUND_START,
    OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL,
    OMM_CAPPY_BULLY_ACT_GROUND_POUND_LAND,
};

//
// Init
//

bool omm_cappy_bully_init(struct Object *o) {
    if (o->oAction == BULLY_ACT_LAVA_DEATH ||
        o->oAction == BULLY_ACT_DEATH_PLANE_DEATH) {
        return false;
    }
    gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_MOVE;
    gOmmObject->state.actionTimer = 0;
    return true;
}

void omm_cappy_bully_end(struct Object *o) {
    obj_anim_play(o, 1, 1.f);
    obj_set_vel(o, 0.f, 0.f, 0.f);
    o->oAction = BULLY_ACT_PATROL;
}

f32 omm_cappy_bully_get_top(struct Object *o) {
    return 115.f * o->oScaleY;
}

//
// Update
//

s32 omm_cappy_bully_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_SAND;
    POBJ_SET_IMMUNE_TO_WIND;

    // Inputs
    if (!obj_update_door(o) && !omm_mario_is_locked(gMarioState)) {
        gOmmObject->state.actionTimer--;
        switch (gOmmObject->state.actionState) {
            case OMM_CAPPY_BULLY_ACT_MOVE: {
                pobj_move(o, false, false, false);
                if (POBJ_B_BUTTON_PRESSED) {
                    if (!obj_is_on_ground(o)) {
                        gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_GROUND_POUND_START;
                        obj_play_sound(o, SOUND_ACTION_THROW);
                    } else if (gOmmObject->state.actionTimer <= 0) {
                        gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_CHARGE;
                        gOmmObject->state.actionTimer = 90;
                    }
                } else if (pobj_jump(o, 0, 1) == POBJ_RESULT_JUMP_START) {
                    obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
                }
            } break;

            case OMM_CAPPY_BULLY_ACT_CHARGE: {
                if (gOmmMario->capture.stickMag < 0.1f) {
                    gOmmMario->capture.stickYaw = o->oFaceAngleYaw;
                }
                gOmmMario->capture.stickMag = 1.f;
                pobj_move(o, false, true, false);
                if (gOmmObject->state.actionTimer > 0) {
                    POBJ_SET_IMMUNE_TO_FIRE;
                    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
                    POBJ_SET_ATTACKING;
                } else {
                    gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_MOVE;
                    gOmmObject->state.actionTimer = 15;
                }
            } break;

            case OMM_CAPPY_BULLY_ACT_GROUND_POUND_START: {
                POBJ_SET_IMMUNE_TO_FIRE;
                POBJ_SET_ATTACKING;
                obj_set_vel(o, 0.f, 0.f, 0.f);
            } break;

            case OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL: {
                POBJ_SET_IMMUNE_TO_FIRE;
                POBJ_SET_ATTACKING;
                obj_set_vel(o, 0.f, omm_capture_get_terminal_velocity(o), 0.f);
            } break;

            case OMM_CAPPY_BULLY_ACT_GROUND_POUND_LAND: {
                POBJ_SET_IMMUNE_TO_FIRE;
                POBJ_SET_ATTACKING;
                omm_mario_lock(gMarioState, -1);
                obj_set_forward_vel(o, 0, 0, 0);
            } break;
        }
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Lava must be handled separately
    if (obj_is_on_ground(o) && o->oFloor->type == SURFACE_BURNING && !OMM_CHEAT_WALK_ON_LAVA) {
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BURNT, false, 0);
        omm_mario_set_action(gMarioState, ACT_LAVA_BOOST, 0, 0);
        o->oAction = BULLY_ACT_LAVA_DEATH;
        POBJ_STOP_IF_UNPOSSESSED;
    }

    // Interactions
    POBJ_INTERACTIONS(

    // Doors
    obj_open_door(o, obj);

    );
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    switch (gOmmObject->state.actionState) {
        case OMM_CAPPY_BULLY_ACT_MOVE: {
            obj_anim_play(o, 0, relerp_0_1_f(o->oForwardVel, 0.f, omm_capture_get_walk_speed(o), 0.75f, 1.5f));
            if (obj_is_on_ground(o)) {
                obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, omm_capture_get_walk_speed(o) * 8.f, o->oForwardVel, SOUND_OBJ_BULLY_WALK, OBJ_PARTICLE_NONE);
            }
        } break;

        case OMM_CAPPY_BULLY_ACT_CHARGE: {
            o->oGfxAngle[0] = 0x800;
            obj_anim_play(o, 1, 2.f);
            if (obj_is_on_ground(o)) {
                obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, omm_capture_get_walk_speed(o) * 8.f, o->oForwardVel, SOUND_OBJ_BULLY_WALKING, OBJ_PARTICLE_NONE);
                obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, 0, 0, -1, OBJ_PARTICLE_MIST); o->oPosY += 60.f * o->oScaleY;
                obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, 0, 0, -1, OBJ_PARTICLE_SMOKE); o->oPosY -= 60.f * o->oScaleY;
            }
        } break;

        case OMM_CAPPY_BULLY_ACT_GROUND_POUND_START: {
            obj_anim_play(o, 2, 1.f);
            if (obj_anim_get_frame(o) >= 6) {
                gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL;
            }
        } break;

        case OMM_CAPPY_BULLY_ACT_GROUND_POUND_FALL: {
            obj_anim_play(o, 2, 1.f);
            obj_anim_set_frame(o, 6);
            obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, 0, 0, -1, OBJ_PARTICLE_MIST);
            if (obj_is_on_ground(o)) {
                cur_obj_shake_screen(SHAKE_POS_MEDIUM);
                obj_spawn_white_puff(o, SOUND_OBJ2_BULLY_ATTACKED);
                omm_spawn_shockwave_whomp(o, o->oPosX, o->oPosY, o->oPosZ);
                gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_GROUND_POUND_LAND;
            }
        } break;

        case OMM_CAPPY_BULLY_ACT_GROUND_POUND_LAND: {
            obj_anim_play(o, 2, 1.f);
            if (obj_anim_is_at_end(o)) {
                omm_mario_unlock(gMarioState);
                gOmmObject->state.actionState = OMM_CAPPY_BULLY_ACT_MOVE;
            }
        } break;
    }

    // Cappy values
    gOmmObject->cappy.copyGfx   = true;
    gOmmObject->cappy.offset[1] = 115.f;
    gOmmObject->cappy.scale     = 1.f;

    // OK
    POBJ_RETURN_OK;
}

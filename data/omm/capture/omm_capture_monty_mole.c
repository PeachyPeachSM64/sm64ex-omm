#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_monty_mole_init(struct Object *o) {
    if (!o->oMontyMoleCurrentHole) {
        return false;
    }

    // Construct the list of Monty mole holes
    gOmmObject->monty_mole.count = 0;
    for_each_object_with_behavior(obj, bhvMontyMoleHole) {
        gOmmObject->monty_mole.holes[gOmmObject->monty_mole.count] = obj;
        if (obj == o->oMontyMoleCurrentHole) {
            gOmmObject->monty_mole.current = gOmmObject->monty_mole.count;
        }
        gOmmObject->monty_mole.count++;
    }

    // Remove all rocks from Monty moles
    obj_deactivate_all_with_behavior(bhvMontyMoleRock);
    for_each_object_with_behavior(obj, bhvMontyMole) {
        obj->prevObj = NULL;
    }

    obj_drop_to_floor(o);
    return true;
}

void omm_cappy_monty_mole_end(struct Object *o) {
    obj_drop_to_floor(o);
    monty_mole_spawn_dirt_particles(40, 15);
    o->oAction = MONTY_MOLE_ACT_SELECT_HOLE;
    o->oVelY = 0;
}

u64 omm_cappy_monty_mole_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_MONTY_MOLE;
}

f32 omm_cappy_monty_mole_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

static s32 omm_cappy_monty_mole_update_pos_and_vel(struct Object *o, bool applyGravity) {
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    if (applyGravity) {
        pobj_apply_gravity(o, 1.f);
        if (!obj_is_on_ground(o)) {
            gOmmObject->state.actionState = MONTY_MOLE_ACT_SELECT_HOLE;
        }
    }
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();
    pobj_return_ok;
}

s32 omm_cappy_monty_mole_update(struct Object *o) {
    o->oMontyMoleTargetHole = NULL;
    bool idleOrMove = (gOmmObject->state.actionState == MONTY_MOLE_ACT_SELECT_HOLE);
    bool hiddenInHole = (gOmmObject->state.actionState == MONTY_MOLE_ACT_HIDE);

    // Hitbox
    o->hitboxRadius = !hiddenInHole * omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = !hiddenInHole * omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER * idleOrMove;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND * idleOrMove;
    POBJ_SET_AFFECTED_BY_CANNON * idleOrMove;
    POBJ_SET_INVULNERABLE * hiddenInHole;
    POBJ_SET_IMMUNE_TO_FIRE * hiddenInHole;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS * hiddenInHole;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // States
    switch (gOmmObject->state.actionState) {

        // Idle/Move
        case MONTY_MOLE_ACT_SELECT_HOLE: {

            // Move the mole if not locked
            if (pobj_process_inputs(o)) {
                pobj_move(o, false, false, false);
                obj_set_angle(o, 0, o->oFaceAngleYaw, 0);

                // Jump into hole
                if (POBJ_A_BUTTON_PRESSED) {
                    gOmmObject->state.actionState = MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE;
                    gOmmObject->state.actionTimer = 0;
                    monty_mole_spawn_dirt_particles(0, 10);
                }

                // Throw rock
                // Hold B to increase strength, speed and size of projectile
                else if (obj_is_on_ground(o)) {
                    pobj_charge_attack(15, o, 150, 0);
                    pobj_release_attack(15,
                        omm_obj_spawn_monty_mole_rock(o, _power_);
                        obj_anim_play_with_sound(o, 8, 1.f, 0, true);
                        obj_anim_clamp_frame(o, 8, 99);
                    );
                } else {
                    gOmmObject->state._powerTimer = 0;
                }
            }

            // Update pos and vel
            s32 returnValue = omm_cappy_monty_mole_update_pos_and_vel(o, true);
            if (returnValue) return returnValue;
        } break;

        // Jump into hole (first part)
        case MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE: {
            omm_mario_lock(gMarioState, -1);

            // Update pos and vel
            f32 terminalVelocity = pobj_get_terminal_velocity(o);
            obj_set_vel(o, 0.f, max_f(40.f - 6.f * gOmmObject->state.actionTimer++, terminalVelocity), 0.f);
            s32 returnValue = omm_cappy_monty_mole_update_pos_and_vel(o, false);
            if (returnValue) return returnValue;
            o->oFaceAnglePitch = (s16) relerp_0_1_f(o->oVelY, -terminalVelocity / 4.f, terminalVelocity / 4.f, 0x0000, 0x8000);

            // Update state
            if (o->oVelY < 0.f && o->oDistToFloor < 120.f) {
                gOmmObject->state.actionState = MONTY_MOLE_ACT_JUMP_INTO_HOLE;
                monty_mole_spawn_dirt_particles(-80, 15);
                obj_play_sound(o, POBJ_SOUND_MONTY_MOLE_DIG);
            }
        } break;

        // Jump into hole (second part)
        case MONTY_MOLE_ACT_JUMP_INTO_HOLE: {
            omm_mario_lock(gMarioState, -1);

            // Update pos and vel
            pobj_apply_gravity(o, 1.f);
            s32 returnValue = omm_cappy_monty_mole_update_pos_and_vel(o, false);
            if (returnValue) return returnValue;

            // Update state
            if (obj_is_on_ground(o)) {
                o->oSubAction = -1;
                gOmmObject->state.actionState = MONTY_MOLE_ACT_HIDE;
                omm_mario_unlock(gMarioState);
            }
        } break;

        // Hide
        case MONTY_MOLE_ACT_HIDE: {

            // Update pos and vel
            obj_set_vel(o, 0.f, 0.f, 0.f);
            obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
            s32 returnValue = omm_cappy_monty_mole_update_pos_and_vel(o, true);
            if (returnValue) return returnValue;
            if (gOmmObject->state.actionState != MONTY_MOLE_ACT_HIDE) {
                gOmmObject->state.actionTimer = 0;
                o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
                break;
            }

            // Select the nearest hole in the direction pointed by the left stick in a 45 degrees arc
            if (gPlayer1Controller->stickMag > 60) {
                u16 nearestHoleAngleDiff = 0x1000;
                for (s32 i = 0; i != gOmmObject->monty_mole.count; ++i) {
                    struct Object *hole = gOmmObject->monty_mole.holes[i];
                    if (hole != o->oMontyMoleCurrentHole) {
                        s16 holeAngle = atan2s(hole->oPosZ - o->oPosZ, hole->oPosX - o->oPosX);
                        u16 angleDiff = (u16) abs_s((s32) ((s16) ((s32) holeAngle - (s32) gMarioState->intendedYaw)));
                        if (angleDiff < nearestHoleAngleDiff) {
                            nearestHoleAngleDiff = angleDiff;
                            o->oSubAction = i;
                        }
                    }
                }
            }

            // If no hole selected, select the next hole in the list
            if (o->oSubAction == -1) {
                o->oSubAction = (gOmmObject->monty_mole.current + 1) % gOmmObject->monty_mole.count;
            }

            // If it's not the same as current, target that hole
            struct Object *currHole = o->oMontyMoleCurrentHole;
            struct Object *nextHole = gOmmObject->monty_mole.holes[o->oSubAction];
            if (currHole != nextHole) {
                o->oMontyMoleTargetHole = nextHole;

                // Press B to teleport to the target hole
                if (POBJ_B_BUTTON_PRESSED) {
                    currHole->oMontyMoleHoleCooldown = 0;

                    // If that hole is occupied, move the Monty mole that occupies it
                    for_each_object_with_behavior(mole, bhvMontyMole) {
                        if (mole->oMontyMoleCurrentHole == nextHole) {
                            currHole->oMontyMoleHoleCooldown = -1;
                            mole->oPosX += (currHole->oPosX - nextHole->oPosX);
                            mole->oPosY += (currHole->oPosY - nextHole->oPosY);
                            mole->oPosZ += (currHole->oPosZ - nextHole->oPosZ);
                            mole->oFloorHeight += (currHole->oPosY - nextHole->oPosY);
                            mole->oMontyMoleCurrentHole = currHole;
                            mole->oAction = MONTY_MOLE_ACT_SELECT_HOLE;
                            mole->oVelY = 0;
                            mole->oIntangibleTimer = -1;
                            mole->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                            mole->prevObj = NULL;
                            monty_mole_spawn_dirt_particles(80, 15);
                            break;
                        }
                    }

                    // Move to the selected hole
                    nextHole->oMontyMoleHoleCooldown = -1;
                    o->oPosX = nextHole->oPosX;
                    o->oPosY = nextHole->oPosY;
                    o->oPosZ = nextHole->oPosZ;
                    o->oFloorHeight = nextHole->oPosY;
                    o->oMontyMoleCurrentHole = nextHole;
                    gOmmObject->monty_mole.current = o->oSubAction;
                    o->oSubAction = -1;
                }
            }

            // Rise from hole
            if (POBJ_A_BUTTON_PRESSED) {
                gOmmObject->state.actionState = MONTY_MOLE_ACT_RISE_FROM_HOLE;
                gOmmObject->state.actionTimer = 0;
                monty_mole_spawn_dirt_particles(0, 10);
                obj_play_sound(o, POBJ_SOUND_MONTY_MOLE_DIG);
            }
        } break;

        // Rise from hole
        case MONTY_MOLE_ACT_RISE_FROM_HOLE: {
            omm_mario_lock(gMarioState, -1);

            // Update pos and vel
            obj_set_vel(o, 0.f, 0.f, 0.f);
            s32 returnValue = omm_cappy_monty_mole_update_pos_and_vel(o, true);
            if (returnValue) return returnValue;
            if (gOmmObject->state.actionState != MONTY_MOLE_ACT_RISE_FROM_HOLE) {
                omm_mario_unlock(gMarioState);
                gOmmObject->state.actionTimer = 0;
                o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
                break;
            }

            // Update state
            if (gOmmObject->state.actionTimer++ >= 18) {
                omm_mario_unlock(gMarioState);
                gOmmObject->state.actionState = MONTY_MOLE_ACT_SELECT_HOLE;
                gOmmObject->state.actionTimer = 0;
            }
        } break;
    }

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // OK
    pobj_return_ok;
}

void omm_cappy_monty_mole_update_gfx(struct Object *o) {

    // Gfx
    switch (gOmmObject->state.actionState) {

        // Idle/Move
        case MONTY_MOLE_ACT_SELECT_HOLE: {
            obj_update_gfx(o);
            o->oGfxPos[1] += 50.f;
            if (!obj_is_on_ground(o)) {
                o->oGfxAngle[0] = (s16) relerp_0_1_f(o->oVelY, 0.f, pobj_get_terminal_velocity(o) / 2.f, 0x0000, 0x8000);
                obj_anim_play(o, 1, 1.f);
            } else if (o->oAnimID != 8 || obj_anim_is_near_end(o)) {
                if (POBJ_IS_WALKING) {
                    obj_anim_play(o, 4, 1.f);
                    if (obj_anim_is_near_end(o)) {
                        monty_mole_spawn_dirt_particles(0, 10);
                    }
                } else {
                    obj_anim_play(o, 3, 1.f);
                }
            }
        } break;

        // Jump into hole (first part)
        case MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE: {
            pobj_freeze_gfx_during_star_dance();
            obj_update_gfx(o);
            o->oGfxPos[1] += 50.f;
            obj_anim_play(o, 0, 1.f);
            obj_anim_extend(o);
        } break;

        // Jump into hole (second part)
        case MONTY_MOLE_ACT_JUMP_INTO_HOLE: {
            pobj_freeze_gfx_during_star_dance();
            obj_update_gfx(o);
            o->oGfxPos[1] += 50.f;
            obj_anim_play(o, 1, 1.f);
        } break;

        // Hide
        case MONTY_MOLE_ACT_HIDE: {
            obj_update_gfx(o);
            o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        } break;

        // Rise from hole
        case MONTY_MOLE_ACT_RISE_FROM_HOLE: {
            pobj_freeze_gfx_during_star_dance();
            obj_update_gfx(o);
            obj_anim_play(o, 1, 1.f);
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            o->oGfxPos[1] += gOmmObject->state.actionTimer * 3;
        } break;
    }

    // Update hole
    vec3f_set(&o->oMontyMoleCurrentHole->oPosX, o->oPosX, o->oFloorHeight, o->oPosZ);
    obj_set_angle(o->oMontyMoleCurrentHole, 0, 0, 0);
    obj_update_gfx(o->oMontyMoleCurrentHole);
    obj_orient_graph(o->oMontyMoleCurrentHole, o->oFloor->normal.x, o->oFloor->normal.y, o->oFloor->normal.z);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

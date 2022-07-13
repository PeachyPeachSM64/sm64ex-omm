#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_monty_mole_init(struct Object *o) {
    if (!o->oMontyMoleCurrentHole) {
        return false;
    }
    gOmmData->object->state.actionState = MONTY_MOLE_ACT_SELECT_HOLE;
    gOmmData->object->state.actionTimer = 0;

    // Remove all rocks from Monty moles
    obj_deactivate_all_with_behavior(bhvMontyMoleRock);
    for_each_object_with_behavior(obj, bhvMontyMole) {
        obj->prevObj = NULL;
    }
    return true;
}

void cappy_monty_mole_end(struct Object *o) {
    obj_anim_play(o, 1, 1.f);
    obj_drop_to_floor(o);
    o->oPosY += 120.f;
    o->oVelY = -4.f;
    o->oGravity = 0.f;
    o->oMoveFlags = 0;
    o->oFaceAnglePitch = 0x8000;
    o->oAction = MONTY_MOLE_ACT_HIDE;
    obj_update_gfx(o);
    monty_mole_spawn_dirt_particles(-80, 15);
}

f32 cappy_monty_mole_get_top(struct Object *o) {
    return 50.f * o->oScaleY;
}

//
// Update
//

static s32 cappy_monty_mole_update_pos_and_vel(struct Object *o) {
    Vec3f previousPos = { o->oPosX, o->oPosY, o->oPosZ };
    perform_object_step(o, POBJ_STEP_FLAGS);
    if ((gOmmData->object->state.actionState == MONTY_MOLE_ACT_SELECT_HOLE ||
         gOmmData->object->state.actionState == MONTY_MOLE_ACT_THROW_ROCK) &&
        !obj_is_on_ground(o)) {
        vec3f_copy(&o->oPosX, previousPos);
        obj_drop_to_floor(o);
    }
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;
    POBJ_RETURN_OK;
}

s32 cappy_monty_mole_update(struct Object *o) {

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_IMMUNE_TO_SAND;
    POBJ_SET_IMMUNE_TO_WIND;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;

    // States
    switch (gOmmData->object->state.actionState) {

        // Idle/Move
        case MONTY_MOLE_ACT_SELECT_HOLE: {

            // Move the mole if not locked
            if (!omm_mario_is_locked(gMarioState)) {
                pobj_move(o, false, false, false);
                obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
                if (POBJ_A_BUTTON_PRESSED) {
                    gOmmData->object->state.actionState = MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE;
                    gOmmData->object->state.actionTimer = 0;
                    monty_mole_spawn_dirt_particles(0, 10);
                } else if (POBJ_B_BUTTON_PRESSED) {
                    gOmmData->object->state.actionState = MONTY_MOLE_ACT_THROW_ROCK;
                    gOmmData->object->state.actionTimer = 0;
                }
            }

            // Update pos and vel
            o->oVelY = 0.f;
            s32 returnValue = cappy_monty_mole_update_pos_and_vel(o);
            if (returnValue) return returnValue;

            // Update gfx
            obj_update_gfx(o);
            o->oGfxPos[1] += 50.f;
            if (abs_f(o->oForwardVel) > 1.f) {
                obj_anim_play(o, 4, 1.f);
                if (obj_anim_is_near_end(o)) {
                    monty_mole_spawn_dirt_particles(0, 10);
                }
            } else {
                obj_anim_play(o, 3, 1.f);
            }
        } break;

        // Jump into hole (first part)
        case MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE: {
            omm_mario_lock(gMarioState, -1);

            // Update pos and vel
            obj_set_vel(o, 0.f, 40.f - 6.f * gOmmData->object->state.actionTimer++, 0.f);
            s32 returnValue = cappy_monty_mole_update_pos_and_vel(o);
            if (returnValue) return returnValue;
            o->oFaceAnglePitch = -atan2s(o->oVelY, -4.f);

            // Update gfx
            obj_update_gfx(o);
            o->oGfxPos[1] += 50.f;
            obj_anim_play(o, 0, 1.f);
            obj_anim_extend(o);

            // Update state
            if (o->oVelY < 0.f && o->oDistToFloor < 120.f) {
                gOmmData->object->state.actionState = MONTY_MOLE_ACT_JUMP_INTO_HOLE;
                monty_mole_spawn_dirt_particles(-80, 15);
                obj_play_sound(o, SOUND_OBJ2_MONTY_MOLE_APPEAR);
            }
        } break;
        
        // Jump into hole (second part)
        case MONTY_MOLE_ACT_JUMP_INTO_HOLE: {
            omm_mario_lock(gMarioState, -1);

            // Update pos and vel
            obj_set_vel(o, 0.f, approach_f32(o->oVelY, -4.f, 0.5f, 0.5f), 0.f);
            s32 returnValue = cappy_monty_mole_update_pos_and_vel(o);
            if (returnValue) return returnValue;

            // Update gfx
            obj_update_gfx(o);
            o->oGfxPos[1] += 50.f;
            obj_anim_play(o, 1, 1.f);

            // Update state
            if (obj_is_on_ground(o)) {
                o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                gOmmData->object->state.actionState = MONTY_MOLE_ACT_HIDE;
                omm_mario_unlock(gMarioState);
            }
        } break;
        
        // Hide
        case MONTY_MOLE_ACT_HIDE: {
            POBJ_SET_INVULNERABLE;
            POBJ_SET_IMMUNE_TO_FIRE;
            
            // Update pos and vel
            obj_set_vel(o, 0.f, 0.f, 0.f);
            obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
            s32 returnValue = cappy_monty_mole_update_pos_and_vel(o);
            if (returnValue) return returnValue;

            // Update gfx
            obj_update_gfx(o);
            o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;

            // Update state
            if (!POBJ_A_BUTTON_DOWN) {
                gOmmData->object->state.actionState = MONTY_MOLE_ACT_RISE_FROM_HOLE;
                gOmmData->object->state.actionTimer = 0;
                monty_mole_spawn_dirt_particles(0, 10);
                obj_play_sound(o, SOUND_OBJ2_MONTY_MOLE_APPEAR);
            }
        } break;
        
        // Rise from hole
        case MONTY_MOLE_ACT_RISE_FROM_HOLE: {
            omm_mario_lock(gMarioState, -1);

            // Update pos and vel
            obj_set_vel(o, 0.f, 0.f, 0.f);
            s32 returnValue = cappy_monty_mole_update_pos_and_vel(o);
            if (returnValue) return returnValue;

            // Update gfx
            obj_update_gfx(o);
            obj_anim_play(o, 1, 1.f);
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            o->oGfxPos[1] += gOmmData->object->state.actionTimer++ * 3;

            // Update state
            if (gOmmData->object->state.actionTimer >= 18) {
                omm_mario_unlock(gMarioState);
                gOmmData->object->state.actionState = MONTY_MOLE_ACT_SELECT_HOLE;
                gOmmData->object->state.actionTimer = 0;
            }
        } break;
        
        // Throw a rock
        case MONTY_MOLE_ACT_THROW_ROCK: {
            omm_mario_lock(gMarioState, -1);
            
            // Update pos and vel
            o->oVelY = 0.f;
            s32 returnValue = cappy_monty_mole_update_pos_and_vel(o);
            if (returnValue) return returnValue;

            // Update gfx
            obj_update_gfx(o);
            o->oGfxPos[1] += 50.f;
            obj_anim_play(o, 8, 1.f);

            // Throw a rock at frame 10
            if (gOmmData->object->state.actionTimer++ == 10) {
                omm_spawn_rock(o);
                obj_play_sound(o, SOUND_OBJ_MONTY_MOLE_ATTACK);
            } else if (obj_anim_is_at_end(o)) {
                omm_mario_unlock(gMarioState);
                gOmmData->object->state.actionState = MONTY_MOLE_ACT_SELECT_HOLE;
            }
        } break;
    }
    
    // Update hole
    vec3f_set(&o->oMontyMoleCurrentHole->oPosX, o->oPosX, o->oFloorHeight, o->oPosZ);
    obj_set_angle(o->oMontyMoleCurrentHole, 0, 0, 0);
    obj_update_gfx(o->oMontyMoleCurrentHole);
    obj_orient_graph(o->oMontyMoleCurrentHole, o->oFloor->normal.x, o->oFloor->normal.y, o->oFloor->normal.z);

    // Hitbox
    o->hitboxRadius = !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE) * omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE) * omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Cappy values
    gOmmData->object->cappy.copyGfx   = true;
    gOmmData->object->cappy.offset[1] = 50.f;
    gOmmData->object->cappy.scale     = 0.6f * !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE);

    // OK
    POBJ_RETURN_OK;
}

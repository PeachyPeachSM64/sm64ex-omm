#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_chain_chomp_init(struct Object *o) {
    if (o->behavior == bhvChainChomp) {
        gOmmData->object->state.initialPos[0] = o->oHomeX;
        gOmmData->object->state.initialPos[2] = o->oHomeZ;
        gOmmData->object->chain_chomp.isFreed = false;
    } else { // Freed
        gOmmData->object->chain_chomp.isFreed = true;
    }
    gOmmData->object->chain_chomp.isBiting = false;
    o->oChainChompMaxDistFromPivotPerChainPart = 750.f / 5.f;
    return true;
}

void cappy_chain_chomp_end(struct Object *o) {
    if (o->behavior == bhvChainChomp) {
        if (gOmmData->object->chain_chomp.isFreed) { // If freed, sets up a new behavior
            o->behavior      = omm_bhv_chain_chomp_free;
            o->curBhvCommand = omm_bhv_chain_chomp_free;
            o->bhvStackIndex = 0;
        } else {
            o->oHomeX = gOmmData->object->state.initialPos[0];
            o->oHomeZ = gOmmData->object->state.initialPos[2];
        }
    }
    o->oWallHitboxRadius = 0.f;
}

f32 cappy_chain_chomp_get_top(struct Object *o) {
    return 245.f * o->oScaleY;
}

//
// Update
//

s32 cappy_chain_chomp_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_SAND;
    POBJ_SET_IMMUNE_TO_WIND;
    POBJ_SET_ATTACKING;

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        gOmmData->object->chain_chomp.isBiting = false;
        pobj_move(o, false, false, false);
        switch (pobj_jump(o, 1.5f, 1)) {
            case POBJ_RESULT_HOP_SMALL: {
                obj_play_sound(o, SOUND_GENERAL_CHAIN_CHOMP1);
            } break;
            case POBJ_RESULT_HOP_LARGE: {
                obj_play_sound(o, SOUND_GENERAL_CHAIN_CHOMP1);
                obj_play_sound(o, SOUND_OBJ_GOOMBA_ALERT);
            } break;
        }

        // Bite
        if (POBJ_B_BUTTON_PRESSED) {
            obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, omm_capture_get_dash_speed(o));
            play_sound(SOUND_GENERAL_CHAIN_CHOMP2, o->oCameraToObject);
            gOmmData->object->chain_chomp.isBiting = true;
            omm_mario_lock(gMarioState, 30);
        }
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // If not freed, restrict position
    if (!gOmmData->object->chain_chomp.isFreed) {
        f32 dx = o->oPosX - gOmmData->object->state.initialPos[0];
        f32 dz = o->oPosZ - gOmmData->object->state.initialPos[2];
        f32 dist = sqrtf(sqr_f(dx) + sqr_f(dz));
        if (dist > 750.f) {

            // Break free if enough speed
            if (o->oForwardVel > 90.f) {
                for_each_object_with_behavior(woodenPost, bhvWoodenPost) {
                    if (woodenPost->parentObj == o) {
                        set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
                        obj_spawn_white_puff(woodenPost, SOUND_GENERAL_WALL_EXPLOSION);
                        obj_spawn_triangle_break_particles(woodenPost, OBJ_SPAWN_TRI_BREAK_PRESET_DIRT);
                        obj_mark_for_deletion(woodenPost);
                        gOmmData->object->chain_chomp.isFreed = true;
                    }
                }
            }

            // Restrict position
            else {
                dx *= (750.f / dist);
                dz *= (750.f / dist);
                o->oPosX = gOmmData->object->state.initialPos[0] + dx;
                o->oPosZ = gOmmData->object->state.initialPos[2] + dz;
                o->oHomeX = o->oPosX;
                o->oHomeZ = o->oPosZ;
            }
        }
    }

    // Break chain chomp gate if hit strong enough
    if (gOmmData->object->chain_chomp.isBiting) {
        if (o->oWall && o->oWall->object && o->oWall->object->behavior == bhvChainChompGate) {
            o->oChainChompHitGate = true;
            audio_play_puzzle_jingle();
        }
    }

    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_anim_play(o, 0, (gOmmData->object->chain_chomp.isBiting ? 2.f : 1.f));
    bhv_chain_chomp_update_chain_parts(o, gOmmData->object->chain_chomp.isFreed);

    // Cappy values
    gOmmData->object->cappy.offset[1] = 245.f;
    gOmmData->object->cappy.scale     = 2.f;

    // OK
    POBJ_RETURN_OK;
}

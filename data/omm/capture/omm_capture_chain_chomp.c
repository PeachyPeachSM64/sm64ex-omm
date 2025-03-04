#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_chain_chomp_init(struct Object *o) {
    if (o->behavior == bhvChainChomp) {
        if (o->oChainChompReleaseStatus != CHAIN_CHOMP_NOT_RELEASED) {
            return false;
        }
        gOmmObject->chain_chomp.isFreed = false;
    } else { // Freed
        gOmmObject->chain_chomp.isFreed = true;
    }
    gOmmObject->chain_chomp.isBiting = false;
    o->oChainChompMaxDistFromPivotPerChainPart = 750.f / 5.f;
    o->oFaceAnglePitch = 0;
    o->oMoveAnglePitch = 0;
    return true;
}

void omm_cappy_chain_chomp_end(struct Object *o) {
    if (o->behavior == bhvChainChomp) {
        if (gOmmObject->chain_chomp.isFreed) { // If freed, set up a new behavior
            o->behavior      = bhvOmmChainChompFree;
            o->curBhvCommand = bhvOmmChainChompFree;
            o->bhvStackIndex = 0;
        } else {
            pobj_reset_home();
        }
    }
    o->oWallHitboxRadius = 0.f;
    o->oFaceAnglePitch = 0;
    o->oMoveAnglePitch = 0;
}

u64 omm_cappy_chain_chomp_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_CHAIN_CHOMP;
}

f32 omm_cappy_chain_chomp_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o) * 1.25f;
}

//
// Update
//

s32 omm_cappy_chain_chomp_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_QUICKSAND;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;
    POBJ_SET_ATTACKING_BREAKABLE;
    POBJ_SET_GROUND_POUNDING;

    // Inputs
    if (pobj_process_inputs(o)) {
        gOmmObject->chain_chomp.isBiting = false;
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 0) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_CHAIN);
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Bite
        if (POBJ_B_BUTTON_PRESSED) {
            obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, pobj_get_dash_speed(o));
            obj_play_sound(o, POBJ_SOUND_CHAIN_CHOMP_BARK);
            gOmmObject->chain_chomp.isBiting = true;
            omm_mario_lock(gMarioState, 20);
        }
    }

    // Movement
    if (pobj_hop(o, 1.5f)) {
        obj_play_sound(o, POBJ_SOUND_CHAIN);
    }
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // If not freed, restrict position
    if (!gOmmObject->chain_chomp.isFreed) {
        struct Object *woodenPost = obj_get_first_with_behavior_and_parent(o, bhvWoodenPost);
        bool freeChainChomp = false;

        // Check if the post is fully pounded
        if (woodenPost && woodenPost->oWoodenPostOffsetY <= -190.f) {
            freeChainChomp = true;
        }

        // Check if the chain chomp is far enough and has enough speed to break free
        else {
            f32 dx = o->oPosX - gOmmObject->state._initialHome[0];
            f32 dz = o->oPosZ - gOmmObject->state._initialHome[2];
            f32 dist = sqrtf(sqr_f(dx) + sqr_f(dz));
            if (dist > 750.f) {

                // Break free if enough speed
                if (o->oForwardVel > 90.f) {
                    freeChainChomp = true;
                }

                // Restrict position
                else {
                    dx *= (750.f / dist);
                    dz *= (750.f / dist);
                    o->oPosX = gOmmObject->state._initialHome[0] + dx;
                    o->oPosZ = gOmmObject->state._initialHome[2] + dz;
                    o->oHomeX = o->oPosX;
                    o->oHomeZ = o->oPosZ;
                }
            }
        }

        // Break free?
        if (freeChainChomp) {
            set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
            if (woodenPost) {
                obj_destroy(woodenPost);
            } else {
                obj_spawn_white_puff_at(
                    gOmmObject->state._initialHome[0],
                    gOmmObject->state._initialHome[1],
                    gOmmObject->state._initialHome[2],
                    POBJ_SOUND_BREAK_BOX
                );
            }
            gOmmObject->chain_chomp.isFreed = true;
        }
    }

    // Break chain chomp gate if hit strong enough
    if (gOmmObject->chain_chomp.isBiting) {
        if (o->oWall && o->oWall->object && o->oWall->object->behavior == bhvChainChompGate) {
            o->oChainChompHitGate = true;
            audio_play_puzzle_jingle();
        }
    }

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();
    omm_obj_process_interactions(o, POBJ_INT_PRESET_CHAIN_CHOMP);

    // Animation, sound and particles
    obj_anim_play(o, 0, (gOmmObject->chain_chomp.isBiting ? 2.f : 1.f));

    // OK
    pobj_return_ok;
}

void omm_cappy_chain_chomp_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    bhv_chain_chomp_update_chain_parts(o, gOmmObject->chain_chomp.isFreed);

    // Cappy transform
    gOmmObject->cappy.tra_y = OMM_GAME_IS_SMMS ? 235.f : 245.f;
    gOmmObject->cappy.scale = 2.f;
}

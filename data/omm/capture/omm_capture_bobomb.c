#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static bool omm_cappy_bobomb_can_respawn_safely(struct Object *o) {
    struct Surface *floor = NULL;
    find_floor(o->oPosX, o->oPosY + 50.f, o->oPosZ, &floor);

    // Out of bounds -> NOT safe
    if (!floor) {
        return false;
    }

    // Lethal floor -> NOT safe
    if (SURFACE_IS_LETHAL(floor->type)) {
        return false;
    }

    // Static floor -> Safe
    if (!floor->object) {
        return true;
    }

#if OMM_GAME_IS_SM64
    // BITFS platforms -> NOT safe
    if (floor->object->behavior == bhvBitfsSinkingPlatforms ||
        floor->object->behavior == bhvBitfsSinkingCagePlatform) {
        return false;
    }
#endif

    // Safe
    return true;
}

//
// Init
//

bool omm_cappy_bobomb_init(struct Object* o) {
    obj_scale(o, 1.f);
    return true;
}

void omm_cappy_bobomb_end(struct Object *o) {
    o->oBehParams &= 0xFF00FFFF;
    o->oBehParams |= (BOBOMB_BP_STYPE_GENERIC << 16);
    o->oBehParams2ndByte = BOBOMB_BP_STYPE_GENERIC;
    o->oCurrAnim = NULL;
    obj_anim_play(o, 0, 1.f);

    // As Bob-omb's respawn point is set to its home position,
    // reset the latter to its previous value if the floor
    // under it is not convenient
    if (!omm_cappy_bobomb_can_respawn_safely(o)) {
        pobj_reset_home();
    }
}

u64 omm_cappy_bobomb_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_BOBOMB;
}

f32 omm_cappy_bobomb_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_bobomb_update(struct Object *o) {

    // The special bob-omb in CS BITS is slightly bigger than a regular bob-omb,
    // has infinite explosions, can climb slopes, but never inflates
    bool isSparklyHardBits = OMM_SPARKLY_MODE_IS_HARD && gCurrLevelNum == LEVEL_BITS;
    if (isSparklyHardBits) obj_scale(o, 1.1f);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES * isSparklyHardBits;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Explosion
        if (POBJ_B_BUTTON_PRESSED && !gOmmObject->state.actionFlag && gOmmObject->state.actionTimer == 0) {
            if (isSparklyHardBits) {
                omm_obj_spawn_explosion(o, 60, 1.f, 4.f, 10, omm_geo_explosion);
                omm_obj_spawn_explosion(o, 60, 1.f, 4.f, 10, omm_geo_explosion_2);
            } else {
                omm_obj_spawn_explosion(o, 60, 1.f * o->oScaleY, 4.f * o->oScaleY, 10, omm_geo_explosion);
            }

            // Make the bob-omb bloat after each explosion
            o->oScaleX *= 1.2f;
            o->oScaleY *= 1.2f;
            o->oScaleZ *= 1.2f;
            gOmmObject->state.actionFlag = true;
            gOmmObject->state.actionTimer = 30;
            gOmmObject->state.actionState += !isSparklyHardBits;

            // If airborne, do a double jump
            if (!obj_is_on_ground(o)) {
                o->oVelY = 1.6f * pobj_get_jump_velocity(o);
            }
        } else {
            gOmmObject->state.actionTimer = max_s(0, gOmmObject->state.actionTimer - 1);
        }
    }
    pobj_stop_if_unpossessed();

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Landing after 3 booms makes the bob-omb explode and ejects Mario
    if (obj_is_on_ground(o)) {
        gOmmObject->state.actionFlag = false;
        if (gOmmObject->state.actionState == 3) {
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 0);
            obj_destroy(o);
        }
    }
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();
    if (POBJ_IS_BOUNCING || POBJ_IS_STAR_DANCING) {
        gOmmObject->state.actionFlag = false;
    }

    // Animation, sound and particles
    obj_anim_play(o, 0, max_f(1.f, (o->oVelY <= 0.f) * POBJ_ABS_FORWARD_VEL * 2.f / pobj_get_walk_speed(o)));
    if (obj_is_on_ground(o)) {
        obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 11.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALK_BOBOMB, OBJ_PARTICLE_NONE);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_bobomb_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    obj_random_blink(o, &o->oBobombBlinkTimer);

    // Cappy transform
    gOmmObject->cappy.object = o;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Interactions
//

static bool omm_cappy_is_obj_targetable(struct Object *o, struct MarioState *m) {
    return o->behavior == bhvTreasureChestBottom || (
           omm_obj_check_interaction(o, m, false) &&
          !omm_obj_is_intangible_to_cappy(o) && (
           omm_obj_is_coin(o) ||
           omm_obj_is_weak(o) ||
           omm_obj_is_bully(o) ||
           omm_obj_is_grabbable(o) ||
           omm_obj_is_unagis_tail(o) ||
           omm_obj_is_mushroom_1up(o) ||
           omm_obj_is_exclamation_box(o) ||
          (omm_obj_is_star_or_key(o) && OMM_CHEAT_CAPPY_CAN_COLLECT_STARS) ||
          (omm_capture_get_data(o) && OMM_CAP_CAPPY_CAPTURE)));
}

struct Object *omm_cappy_find_target(struct Object *cappy, struct MarioState *m, f32 distanceMax) {
    struct Object *target = NULL;
    struct Object *targetCoin = NULL;
    f32 distanceMin = distanceMax;
    f32 distanceCoinMin = distanceMax;
    for_each_object_in_cappy_lists(obj) {
        if (obj != cappy && omm_cappy_is_obj_targetable(obj, m)) {
            f32 distToCappy = obj_get_distance(obj, cappy) - (cappy->hitboxRadius + obj->hitboxRadius);
            if (omm_obj_is_coin(obj)) {
                if (distToCappy < distanceCoinMin) {
                    distanceCoinMin = distToCappy;
                    targetCoin = obj;
                }
            } else {
                if (distToCappy < distanceMin) {
                    distanceMin = distToCappy;
                    target = obj;
                }
            }
        }
    }

    // Prioritize coins
    return (targetCoin ? targetCoin : target);
}

bool omm_cappy_is_mario_available(struct MarioState *m, bool isCapture) {

    // Not holding/riding
    if ((m->heldObj   != NULL) ||
        (m->riddenObj != NULL)) {
        return false;
    }

    // No cutscene/automatic action
    if (((m->action & ACT_GROUP_MASK) == ACT_GROUP_CUTSCENE) ||
        ((m->action & ACT_GROUP_MASK) == ACT_GROUP_AUTOMATIC && !isCapture)) {
        return false;
    }

    // Lava-boosting is ok, but only the falling part
    if (m->vel[1] <= 0.f && (
        m->action == ACT_LAVA_BOOST ||
        m->action == ACT_LAVA_BOOST_LAND)) {
        return true;
    }

#if OMM_GAME_IS_R96A
    // Wario's shoulder bash
    if (m->action == ACT_WARIO_CHARGE) {
        return true;
    }
#endif

    // Not intangible/interacting
    if (((m->action & ACT_FLAG_ON_POLE)      != 0 && !isCapture) ||
        ((m->action & ACT_FLAG_HANGING)      != 0 && !isCapture) ||
        ((m->action & ACT_FLAG_INTANGIBLE)   != 0)               ||
        ((m->action & ACT_FLAG_INVULNERABLE) != 0)               ||
        ((m->action & ACT_FLAG_RIDING_SHELL) != 0)) {
        return false;
    }

    // OK
    return true;
}

void omm_cappy_try_to_target_next_coin(struct Object *cappy) {
    if (cappy->oCappyFlags & CAPPY_FLAG_HOMING_ATTACK) {
        struct Object *target = omm_cappy_find_target(cappy, gMarioState, CAPPY_HOMING_ATTACK_VEL * CAPPY_HOMING_ATTACK_DURATION);
        if (target) {
            f32 dx = target->oPosX - cappy->oPosX;
            f32 dy = target->oPosY - cappy->oPosY;
            f32 dz = target->oPosZ - cappy->oPosZ;
            f32 dv = max_f(0.1f, sqrtf(sqr_f(dx) + sqr_f(dy) + sqr_f(dz)));
            cappy->oVelX = CAPPY_HOMING_ATTACK_VEL * (dx / dv);
            cappy->oVelY = CAPPY_HOMING_ATTACK_VEL * (dy / dv);
            cappy->oVelZ = CAPPY_HOMING_ATTACK_VEL * (dz / dv);
            cappy->oCappyLifeTimer = CAPPY_LIFETIME - CAPPY_HOMING_ATTACK_DURATION - 1;
        } else {
            omm_cappy_return_to_mario(cappy);
        }
    }
}

void omm_cappy_bounce_back(struct Object *cappy) {
    if (cappy->oCappyBehavior < OMM_CAPPY_BHV_SPIN_GROUND && !(cappy->oCappyFlags & CAPPY_FLAG_PLAY_AS)) {
        spawn_object(cappy, MODEL_NONE, bhvHorStarParticleSpawner);
        play_sound(SOUND_OBJ_DEFAULT_DEATH, cappy->oCameraToObject);
        omm_cappy_return_to_mario(cappy);
    }
}

void omm_cappy_process_interactions(struct Object *cappy, struct MarioState *m) {

    // Is Cappy tangible for Mario?
    if ((cappy->oCappyFlags & CAPPY_FLAG_INTERACT_MARIO) && !gOmmData->mario->cappy.bounced) {

        // Mario must be available and not swimming
        if (omm_cappy_is_mario_available(m, false) && !(m->action & ACT_FLAG_SWIMMING)) {

            // Hitbox overlapping
            struct Object sMarioHitbox = {
                .oPosX = m->pos[0],
                .oPosY = m->pos[1],
                .oPosZ = m->pos[2],
                .hitboxRadius = 50.f,
                .hitboxHeight = m->marioObj->hitboxHeight,
                .hitboxDownOffset = 0.f
            };
            if (obj_detect_hitbox_overlap(cappy, &sMarioHitbox, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX)) {
                if ((m->action & ACT_FLAG_AIR) || (!OMM_CHEAT_WALK_ON_LAVA && (m->floor->type == SURFACE_BURNING) && !omm_mario_has_metal_cap(m))) {
                    gOmmData->mario->cappy.bounced = !OMM_CHEAT_UNLIMITED_CAPPY_BOUNCES;
                    if (m->controller->buttonPressed & A_BUTTON) {
                        omm_mario_set_action(m, (m->action & ACT_FLAG_METAL_WATER) ? ACT_OMM_METAL_WATER_TRIPLE_JUMP : ACT_OMM_GROUND_CAPPY_BOUNCE, 1, 0);
                        m->particleFlags |= PARTICLE_SPARKLES; // Frame perfect boys get sparkles
                    } else {
                        omm_mario_set_action(m, (m->action & ACT_FLAG_METAL_WATER) ? ACT_OMM_METAL_WATER_CAPPY_BOUNCE : ACT_OMM_CAPPY_BOUNCE, 0, 0);
                    }
                } else {
                    omm_mario_set_action(m, (m->action & ACT_FLAG_METAL_WATER) ? ACT_OMM_METAL_WATER_TRIPLE_JUMP : ACT_OMM_GROUND_CAPPY_BOUNCE, 1, 0);
                }
                omm_mario_init_next_action(m);
                omm_cappy_return_to_mario(cappy);
                spawn_object(m->marioObj, MODEL_NONE, bhvHorStarParticleSpawner);
                play_sound(SOUND_GENERAL_BOING1, m->marioObj->oCameraToObject);
                return;
            }
        }
    }

    // Treasure chests
    // They are normally intangible and their hitbox
    // is too large to directly interact with them
    for_each_object_with_behavior(obj, bhvTreasureChestBottom) {
        if (vec3f_is_inside_cylinder(&cappy->oPosX, &obj->oPosX, 150.f * obj->oScaleX, 160.f * obj->oScaleY, 0.f)) {
            if (obj->oAction == 0 || (obj->oAction == 2 && obj->oTimer > 30)) {
                if (obj->parentObj->oTreasureChestUnkF4 == obj->oBehParams2ndByte) {
                    play_sound(SOUND_GENERAL2_RIGHT_ANSWER, gGlobalSoundArgs);
                    obj->parentObj->oTreasureChestUnkF4++;
                    obj->parentObj->oTreasureChestUnkF8 = 0;
                    obj->oAction = 1;
                } else {
                    play_sound(SOUND_MENU_CAMERA_BUZZ, gGlobalSoundArgs);
                    obj->parentObj->oTreasureChestUnkF4 = 1;
                    obj->parentObj->oTreasureChestUnkF8 = 1;
                    obj->oAction = 2;
                    omm_spawn_damage_mario(m->marioObj, INTERACT_SHOCK, obj->oDamageOrCoinValue);
                }
            }
            omm_cappy_bounce_back(cappy);
            return;
        }
    }

    // Object interactions
    omm_obj_process_interactions(cappy, OBJ_INT_PRESET_CAPPY);
}

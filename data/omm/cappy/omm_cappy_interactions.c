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
           omm_obj_is_water_diamond(o) ||
           omm_obj_is_mushroom_1up(o) ||
           omm_obj_is_exclamation_box(o) ||
          (omm_obj_is_star_or_key(o) && OMM_CHEAT_CAPPY_CAN_COLLECT_STARS) ||
          (omm_behavior_data_get_capture(o->behavior) && OMM_CAP_CAPPY_CAPTURE)));
}

struct Object *omm_cappy_find_target(f32 *origin, struct Object *cappy, struct MarioState *m, Vec3f direction, f32 distanceMax) {
    struct Object *target = NULL;
    struct Object *targetCoin = NULL;
    f32 distanceMin = distanceMax;
    f32 distanceCoinMin = distanceMax;
    for_each_object_in_cappy_lists(obj) {
        if (obj != cappy && omm_cappy_is_obj_targetable(obj, m)) {

            // Cappy's homing attack range is greater if the targeted object is in
            // the same direction as Cappy's displacement (D-pad or Cappy's velocity)
            Vec3f dirToObj = {
                obj->oPosX - origin[0],
                obj->oPosY - origin[1],
                obj->oPosZ - origin[2]
            };
            f32 distToObj = vec3f_length(dirToObj);
            f32 dotObjDirCappyDir = vec3f_dot(vec3f_normalize(dirToObj), direction);
            f32 distToCappy = (
                (distToObj - (cappy->hitboxRadius + obj->hitboxRadius)) /
                relerp_0_1_f(dotObjDirCappyDir, 0.f, 1.f, 1.f, OMM_CAPPY_HOMING_ATTACK_RANGE_MULT)
            );

            // Compare distances
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
    if (m->heldObj || m->riddenObj) {
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

#if OMM_GAME_IS_R96X
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

void omm_cappy_try_to_target_next_object(struct Object *cappy, struct MarioState *m) {
    if (cappy->oCappyFlags & OMM_CAPPY_FLAG_HOMING_ATTACK) {
        f32 velocity = OMM_CAPPY_HOMING_ATTACK_VELOCITY;
        s32 duration = OMM_CAPPY_HOMING_ATTACK_DURATION;
        Vec3f direction = { cappy->oVelX, cappy->oVelY, cappy->oVelZ };
        vec3f_normalize(direction);
        struct Object *target = omm_cappy_find_target(&cappy->oPosX, cappy, m, direction, velocity * duration);
        if (target) {
            f32 dx = target->oPosX - cappy->oPosX;
            f32 dy = target->oPosY - cappy->oPosY;
            f32 dz = target->oPosZ - cappy->oPosZ;
            f32 dv = max_f(0.1f, sqrtf(sqr_f(dx) + sqr_f(dy) + sqr_f(dz)));
            if (dv > velocity * duration) {
                cappy->oVelX = dx / duration;
                cappy->oVelY = dy / duration;
                cappy->oVelZ = dz / duration;
            } else {
                cappy->oVelX = velocity * dx / dv;
                cappy->oVelY = velocity * dy / dv;
                cappy->oVelZ = velocity * dz / dv;
            }
            cappy->oCappyLifeTimer = OMM_CAPPY_LIFETIME - duration - 1;
        } else {
            omm_cappy_return_to_mario(cappy);
        }
    }
}

void omm_cappy_bounce_back(struct Object *cappy) {
    if (cappy->oCappyBehavior < OMM_CAPPY_BHV_SPIN_GROUND && !(cappy->oCappyFlags & OMM_CAPPY_FLAG_PLAY_AS)) {
        if (!omm_mario_is_capture(gMarioState)) {
            spawn_object(cappy, MODEL_NONE, bhvHorStarParticleSpawner);
            obj_play_sound(cappy, SOUND_OBJ_DEFAULT_DEATH);
        }
        omm_cappy_return_to_mario(cappy);
    }
}

void omm_cappy_process_interactions(struct Object *cappy, struct MarioState *m) {

    // Is Cappy tangible for Mario?
    if ((cappy->oCappyFlags & OMM_CAPPY_FLAG_INTERACT_MARIO) && !gOmmMario->cappy.bounced) {

        // Mario must be available and not swimming
        if (omm_cappy_is_mario_available(m, false) && (!(m->action & ACT_FLAG_SWIMMING) || (m->pos[1] > m->waterLevel - OMM_MARIO_WATER_JUMP_SURFACE_OFFSET))) {

            // Hitbox overlapping
            struct Object marioHitbox = {
                .oPosX = m->pos[0],
                .oPosY = m->pos[1],
                .oPosZ = m->pos[2],
                .hitboxRadius = m->marioObj->hitboxRadius * 1.35f,
                .hitboxHeight = m->marioObj->hitboxHeight,
                .hitboxDownOffset = 0.f
            };
            if (obj_detect_hitbox_overlap(cappy, &marioHitbox, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX)) {
                bool bounce = (m->action & ACT_FLAG_AIR) || (m->action & ACT_FLAG_SWIMMING) || (!OMM_CHEAT_WALK_ON_LAVA && m->floor->type == SURFACE_BURNING);
                bool vault = !bounce || m->framesSinceA < OMM_CAPPY_SUPER_BOUNCE_FRAMES;
                bool water = (m->action & ACT_FLAG_METAL_WATER);
                gOmmMario->cappy.bounced |= bounce && !OMM_CHEAT_UNLIMITED_CAPPY_BOUNCES;
                PFX((m->action & ACT_FLAG_SWIMMING) ? PARTICLE_WATER_SPLASH : 0);
                omm_mario_set_action(m, vault ?
                    (water ? ACT_OMM_METAL_WATER_TRIPLE_JUMP  : ACT_OMM_CAPPY_VAULT ) :
                    (water ? ACT_OMM_METAL_WATER_CAPPY_BOUNCE : ACT_OMM_CAPPY_BOUNCE),
                    vault, 0
                );
                omm_mario_init_next_action(m);
                omm_cappy_return_to_mario(cappy);
                spawn_object(m->marioObj, MODEL_NONE, bhvHorStarParticleSpawner);
                SFX(SOUND_GENERAL_BOING1);
                omm_stats_increase(cappyBounces, 1);
                return;
            }
        }
    }

    // Treasure chests
    // They are normally intangible and their hitbox
    // is too large to directly interact with them
    for_each_object_with_behavior(obj, bhvTreasureChestBottom) {
        if (omm_obj_interact_treasure_chest(cappy, obj)) {
            omm_cappy_bounce_back(cappy);
            return;
        }
    }

    // Object interactions
    omm_obj_process_interactions(cappy, omm_mario_is_milk(m) ? OBJ_INT_PRESET_CAPPY_STRONG : OBJ_INT_PRESET_CAPPY);
}

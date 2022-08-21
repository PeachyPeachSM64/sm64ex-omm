#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

OMM_INLINE u32 omm_obj_get_behavior_types(struct Object *o) {
    if (obj_alloc_fields(o) && o->oBhvPointer != o->behavior) {
        o->oBhvTypes = omm_behavior_data_get_types(o->behavior);
        o->oBhvPointer = o->behavior;
    }
    return o->oBhvTypes;
}

//
// Lists
//

s32 gOmmCappyObjectLists[] = {
    OBJ_LIST_LEVEL,
    OBJ_LIST_SURFACE,
    OBJ_LIST_POLELIKE,
    OBJ_LIST_PUSHABLE,
    OBJ_LIST_GENACTOR,
    OBJ_LIST_DESTRUCTIVE,
    -1
};

s32 gOmmInteractionObjectLists[] = {
    OBJ_LIST_DESTRUCTIVE,
    OBJ_LIST_GENACTOR,
    OBJ_LIST_PUSHABLE,
    OBJ_LIST_LEVEL,
    OBJ_LIST_DEFAULT,
    OBJ_LIST_SURFACE,
    OBJ_LIST_POLELIKE,
    -1
};

//
// Types
//

bool omm_obj_is_interactible(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_INTERACTIBLE) != 0;
}

bool omm_obj_is_coin(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_COIN) != 0;
}

bool omm_obj_is_water_ring(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_WATER_RING) != 0;
}

bool omm_obj_is_mushroom_1up(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_MUSHROOM_1UP) != 0 && !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE);
}

bool omm_obj_is_star_or_key(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_STAR_OR_KEY) != 0;
}

bool omm_obj_is_star_model(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_STAR_MODEL) != 0;
}

bool omm_obj_is_cap(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_CAP) != 0;
}

bool omm_obj_is_secret(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_SECRET) != 0;
}

bool omm_obj_is_weak(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_WEAK) != 0;
}

bool omm_obj_is_strong(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_STRONG) != 0;
}

bool omm_obj_is_destructible(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_DESTRUCTIBLE) != 0;
}

bool omm_obj_is_breakable(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_BREAKABLE) != 0;
}

bool omm_obj_is_boss(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_BOSS) != 0;
}

bool omm_obj_is_invulnerable(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_INVULNERABLE) != 0;
}

bool omm_obj_is_grabbable(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_GRABBABLE) != 0;
}

bool omm_obj_is_goomba(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_GOOMBA) != 0;
}

bool omm_obj_is_bully(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_BULLY) != 0;
}

bool omm_obj_is_flame(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_FLAME) != 0;
}

bool omm_obj_is_intangible_to_cappy(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_NO_CAPPY) != 0;
}

bool omm_obj_is_kickable_board(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_KICKABLE_BOARD) != 0;
}

bool omm_obj_is_wf_star_wall(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_WF_STAR_WALL) != 0;
}

bool omm_obj_is_exclamation_box(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_EXCLAMATION_BOX) != 0 && (o->oAction == 2);
}

bool omm_obj_is_unagis_tail(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_UNAGIS_TAIL) != 0 && (o->oBhvArgs2ndByte == -4);
}

bool omm_obj_is_collectible(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_COLLECTIBLE) != 0;
}

//
// Object behaviors
//

static void omm_obj_init_behaviors_array(OmmArray *arr, s32 type) {
    if (OMM_UNLIKELY(!arr->p)) {
        for_each_until_null(const BehaviorScript *, bhv, omm_get_behaviors_list()) {
            if (omm_behavior_data_get_types(*bhv) & type) {
                omm_array_add(*arr, ptr, *bhv);
            }
        }
    }
}

OmmArray omm_obj_get_coin_behaviors() {
    static OmmArray sOmmCoinBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmCoinBehaviors, BHV_TYPE_COIN);
    return sOmmCoinBehaviors;
}

OmmArray omm_obj_get_star_or_key_behaviors() {
    static OmmArray sOmmStarOrKeyBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmStarOrKeyBehaviors, BHV_TYPE_STAR_OR_KEY);
    return sOmmStarOrKeyBehaviors;
}

OmmArray omm_obj_get_star_model_behaviors() {
    static OmmArray sOmmStarModelBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmStarModelBehaviors, BHV_TYPE_STAR_MODEL);
    return sOmmStarModelBehaviors;
}

OmmArray omm_obj_get_cap_behaviors() {
    static OmmArray sOmmCapBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmCapBehaviors, BHV_TYPE_CAP);
    return sOmmCapBehaviors;
}

OmmArray omm_obj_get_goomba_behaviors() {
    static OmmArray sOmmGoombaBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmGoombaBehaviors, BHV_TYPE_GOOMBA);
    return sOmmGoombaBehaviors;
}

OmmArray omm_obj_get_player_behaviors() {
    static OmmArray sOmmPlayerBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmPlayerBehaviors, BHV_TYPE_PLAYER);
    return sOmmPlayerBehaviors;
}

OmmArray omm_obj_get_bowser_behaviors() {
    static OmmArray sOmmBowserBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmBowserBehaviors, BHV_TYPE_BOWSER);
    return sOmmBowserBehaviors;
}

OmmArray omm_obj_get_sparkly_enemy_behaviors() {
    static OmmArray sOmmSparklyEnemyBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmSparklyEnemyBehaviors, BHV_TYPE_SPARKLY_ENEMY);
    return sOmmSparklyEnemyBehaviors;
}

//
// Perry attacks (include Vibes)
//

#define MAX_PERRY_ATTACKS 32
typedef struct {
    struct Object *obj;
    const BehaviorScript *bhv;
    Vec3f pos;
    s32 spk;
    s32 act;
} PerryAttack;
static PerryAttack sPerryAttacks[MAX_PERRY_ATTACKS];

OMM_ROUTINE_LEVEL_ENTRY(omm_obj_init_perry_attacks) {
    omm_zero(sPerryAttacks, sizeof(sPerryAttacks));
}

OMM_ROUTINE_PRE_RENDER(omm_obj_update_perry_attacks) {
    if (OMM_LIKELY(gMarioObject)) {

        // Check objects attacked by Perry
        for (s32 i = 0; i != MAX_PERRY_ATTACKS; ++i) {
            PerryAttack *patk = &sPerryAttacks[i];
            if (patk->obj) {
                if (patk->obj->behavior == patk->bhv) {
                    if (!patk->obj->activeFlags || patk->obj->oAction == patk->act) {
                        patk->pos[0] = patk->obj->oPosX;
                        patk->pos[1] = patk->obj->oPosY + max_f(0.f, patk->obj->hitboxHeight - patk->obj->hitboxDownOffset);
                        patk->pos[2] = patk->obj->oPosZ;
                        patk->obj = NULL;
                    }
                } else {
                    patk->obj = NULL;
                    patk->spk = 0;
                }
            }
        }

        // Spawn sparkles from dead objects
        for (s32 i = 0; i != MAX_PERRY_ATTACKS; ++i) {
            PerryAttack *patk = &sPerryAttacks[i];
            if (!patk->obj && patk->spk) {
                for (s32 j = 0; j != patk->spk; ++j) {
                    omm_spawn_peach_vibe_sparkle(gMarioObject, patk->pos[0], patk->pos[1], patk->pos[2]);
                }
                patk->spk = 0;
            }
        }
    }
}

static void omm_obj_handle_perry_attack(struct Object *o, u32 interactionFlags) {
    if (interactionFlags & OBJ_INT_PERRY_ATTACK) {
        const OmmBhvDataPerryAttack *dpatk = omm_behavior_data_get_perry_attack(o->behavior);
        if (dpatk && dpatk->vibeSparkles > 0) {
            s32 j = -1;

            // Look for an existing object
            for (s32 k = 0; k != MAX_PERRY_ATTACKS; ++k) {
                PerryAttack *patk = &sPerryAttacks[k];
                if (!patk->obj) {
                    j = k;
                } else if (patk->obj == o) {
                    patk->bhv = o->behavior;
                    patk->spk = dpatk->vibeSparkles;
                    patk->act = dpatk->deathAction;
                    return;
                }
            }

            // Not found, add it to the list
            if (j != -1) {
                PerryAttack *patk = &sPerryAttacks[j];
                patk->obj = o;
                patk->bhv = o->behavior;
                patk->spk = dpatk->vibeSparkles;
                patk->act = dpatk->deathAction;
            }
        }
    }
}

//
// Interactions
//

#define OMM_CAPPY_ONLY_CODE(...) if (isCappy) { __VA_ARGS__ }
static bool sStopAndReturn;

bool omm_obj_check_interaction(struct Object *o, struct MarioState *m, bool ignoreTangibility) {
    return !(o->oInteractStatus & INT_STATUS_INTERACTED) &&     // Not interacted
            (o->oIntangibleTimer == 0 || ignoreTangibility) &&  // Not intangible
            (o->activeFlags != ACTIVE_FLAG_DEACTIVATED) &&      // Not unloading/unloaded
            (o != gOmmCapture) &&                               // Not a capture
            (o != m->usedObj || !omm_mario_is_picking_up(m)) && // Not used or not picking up
            (o != m->heldObj) &&                                // Not held
            (o != m->riddenObj);                                // Not ridden
}

static void omm_obj_interact_attract_coin(struct Object *o, struct Object *target, u32 interactionFlags, bool strong) {
    f32 oCenterY = o->oPosY + o->hitboxHeight / 2.f - o->hitboxDownOffset;
    f32 dx = target->oPosX - o->oPosX;
    f32 dy = target->oPosY - oCenterY;
    f32 dz = target->oPosZ - o->oPosZ;
    f32 distX = max_f(0.f, abs_f(dx) - o->hitboxRadius);
    f32 distY = max_f(0.f, abs_f(dy) - o->hitboxHeight / 2.f);
    f32 distZ = max_f(0.f, abs_f(dz) - o->hitboxRadius);
    f32 distToObj = sqrtf(distX * distX + distY * distY + distZ * distZ);
    f32 distPullMax = OBJ_INT_ATTRACT_COINS_RANGE_GET(interactionFlags);
    if (distToObj < distPullMax) {
        f32 velPullMax = max_f(50.f, abs_f(o->oForwardVel) * 1.25f);
        f32 velPull = velPullMax * (strong ? 1.f : relerp_0_1_f(distToObj, 0.f, distPullMax, 1.f, 0.f));
        Vec3f dv = { dx, dy, dz };
        vec3f_normalize(dv);
        vec3f_mul(dv, velPull);
        Vec4f vel = { target->oVelX, target->oVelY, target->oVelZ, target->oForwardVel };
        target->oVelX = -dv[0];
        target->oVelY = -dv[1];
        target->oVelZ = -dv[2];
        target->oWallHitboxRadius = target->hitboxRadius / 2.f;
        perform_object_step(target, 0);
        target->oVelX = vel[0];
        target->oVelY = vel[1];
        target->oVelZ = vel[2];
        target->oForwardVel = vel[3];
    }
}

static void omm_obj_interact_heal_mario(struct Object *target, u32 interactionFlags) {
    if ((interactionFlags & OBJ_INT_HEAL_MARIO) && !omm_obj_is_collectible(target)) {
        omm_health_heal_mario(gMarioState);
        gMarioState->healCounter += 4;
    }
}

static bool omm_obj_interact_defeat_in_one_hit(struct Object *o, struct Object *target) {

    // King Bob-omb
    if (target->behavior == bhvKingBobomb && target->oHealth) {
        obj_play_sound(target, SOUND_OBJ_KING_BOBOMB);
        obj_spawn_white_puff(target, SOUND_OBJ2_KING_BOBOMB_DAMAGE);
        set_camera_shake_from_point(SHAKE_POS_SMALL, target->oPosX, target->oPosY, target->oPosZ);
        obj_set_forward_vel(target, 0, 0, 0);
        target->oAction = 7;
        target->oHealth = 0;
        return true;
    }

    // King Whomp
    if (target->behavior == bhvWhompKingBoss && target->oHealth) {
        set_camera_shake_from_point(SHAKE_POS_SMALL, target->oPosX, target->oPosY, target->oPosZ);
        obj_set_forward_vel(target, 0, 0, 0);
        obj_destroy(target);
        return true;
    }

    // Big Boos
    if (target->behavior == bhvBalconyBigBoo ||
        target->behavior == bhvGhostHuntBigBoo ||
        target->behavior == bhvMerryGoRoundBigBoo) {
        target->oHealth = 1;
        target->oInteractStatus = (ATTACK_GROUND_POUND_OR_TWIRL | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
        return true;
    }

    // Big Bullies
    if (target->behavior == bhvBigBully ||
        target->behavior == bhvBigBullyWithMinions ||
        target->behavior == bhvBigChillBully) {
        target->oForwardVel = 150.f;
        target->oMoveAngleYaw = obj_get_object1_angle_yaw_to_object2(o->parentObj ? o->parentObj : o, target);
        target->oFaceAngleYaw = target->oMoveAngleYaw + 0x8000;
        target->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
        play_sound(SOUND_OBJ_BULLY_METAL, target->oCameraToObject);
        return true;
    }

    // Eyerock hands
    if (target->behavior == bhvEyerokHand) {
        set_camera_shake_from_point(SHAKE_POS_SMALL, target->oPosX, target->oPosY, target->oPosZ);
        obj_set_forward_vel(target, 0, 0, 0);
        obj_destroy(target);
        return true;
    }

    // Wiggler
    if (target->behavior == bhvWigglerHead) {
        target->oHealth = 2;
        target->oInteractStatus = (ATTACK_GROUND_POUND_OR_TWIRL | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
        return true;
    }

    // Bowser
    if (target->behavior == bhvBowserBodyAnchor) {
        struct Object *bowser = target->parentObj;
        if (bowser && bowser->behavior == bhvBowser && bowser->oHealth && gIsBowserInteractible[bowser->oAction]) {
#if OMM_GAME_IS_R96X
            // Spamba Bowser
            if (bowser->oInteractType == INTERACT_DAMAGE) {
                return false;
            }
#endif
            // Fake a Bowser throw, set his health to 1 and spawn a mine below him
            bowser->oAction = 1;
            bowser->oPrevAction = 1;
            bowser->oSubAction = 1;
            bowser->oTimer = 0;
            bowser->oHealth = 1;
            bowser->oBowserCameraState = 10;
            obj_anim_play(bowser, 2, 1.f);
            spawn_object(bowser, MODEL_NONE, bhvBowserBomb);
            return true;
        }
        return false;
    }

    return false;
}

bool omm_obj_process_one_surface_interaction(struct Object *o, struct Object *target, u32 interactionFlags) {
    struct MarioState *m = gMarioState;
    bool isCappy = (o == omm_cappy_get_object());
    bool interacted = false;

    // Target must not be the same as current object
    // Target must be tangible to Cappy if current object is Cappy
    // Target must be valid for interaction
    if (target != o && (!isCappy || !omm_obj_is_intangible_to_cappy(target)) && omm_obj_check_interaction(target, m, true)) {

OMM_CAPPY_ONLY_CODE(
        // Capture
        if (omm_cappy_is_mario_available(m, true) && omm_mario_possess_object(m, target, false, false)) {
            omm_cappy_unload();
            sStopAndReturn = true;
            return true;
        }
);
        // Interactible
        if (omm_obj_is_interactible(target)) {
            interacted = true;

            // Defeat enemies in one hit
            if (interactionFlags & OBJ_INT_ATTACK_ONE_HIT) {
                if (omm_obj_is_boss(target) && omm_obj_interact_defeat_in_one_hit(o, target)) {
                    omm_obj_handle_perry_attack(target, interactionFlags);
                    sStopAndReturn = true;
                    return true;
                }
            }

            // Attackable objects
            if (interactionFlags & (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG)) {

                // Kickable board
                if (omm_obj_is_kickable_board(target)) {
                    target->oInteractStatus = INT_STATUS_INTERACTED;
                    target->oAction = 2;
OMM_CAPPY_ONLY_CODE(
                    omm_cappy_bounce_back(o);
);
                }
            }

            // Destructible objects
            if (interactionFlags & OBJ_INT_ATTACK_DESTRUCTIBLE) {
                                
                // Destructible
                if (omm_obj_is_destructible(target)) {
                    omm_obj_handle_perry_attack(target, interactionFlags);
                    omm_obj_interact_heal_mario(target, interactionFlags);
                    target->oInteractStatus = INT_STATUS_INTERACTED;
                    obj_destroy(target);
OMM_CAPPY_ONLY_CODE(
                    omm_cappy_bounce_back(o);
);
                }

                // WF star wall
                if (omm_obj_is_wf_star_wall(target)) {
                    omm_obj_handle_perry_attack(target, interactionFlags);
                    omm_obj_interact_heal_mario(target, interactionFlags);
                    target->oInteractStatus = INT_STATUS_INTERACTED;
                    audio_play_puzzle_jingle();
                    obj_destroy(target);
OMM_CAPPY_ONLY_CODE(
                    omm_cappy_bounce_back(o);
);
                }
            }
        }
    }
    return interacted;
}

bool omm_obj_process_one_object_interaction(struct Object *o, struct Object *target, u32 interactionFlags) {
    struct MarioState *m = gMarioState;
    bool isCappy = (o == omm_cappy_get_object());
    bool interacted = false;

    // Target must not be the same as current object
    // Target must be valid for interaction
    if (target != o && omm_obj_check_interaction(target, m, false)) {

        // Attract coins
        if (omm_obj_is_coin(target)) {
            if (interactionFlags & OBJ_INT_ATTRACT_COINS_STRONG) {
                omm_obj_interact_attract_coin(o, target, interactionFlags, true);
            } else if (interactionFlags & OBJ_INT_ATTRACT_COINS_WEAK) {
                omm_obj_interact_attract_coin(o, target, interactionFlags, false);
            }
        }

        // Target must be tangible to Cappy if current object is Cappy
        // Target hitbox must overlap with current object hitbox
        if ((!isCappy || !omm_obj_is_intangible_to_cappy(target)) && obj_detect_hitbox_overlap(o, target, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX_HURTBOX_MAX)) {

OMM_CAPPY_ONLY_CODE(
            // Capture
            if (omm_cappy_is_mario_available(m, true) && omm_mario_possess_object(m, target, true, false)) {
                omm_cappy_unload();
                sStopAndReturn = true;
                return true;
            }
);
            // Interactible
            if (omm_obj_is_interactible(target)) {
                interacted = true;

                // Defeat enemies in one hit
                if (interactionFlags & OBJ_INT_ATTACK_ONE_HIT) {
                    if (omm_obj_is_boss(target) && omm_obj_interact_defeat_in_one_hit(o, target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        sStopAndReturn = true;
                        return true;
                    }
                }

                // Triggers
                if (interactionFlags & OBJ_INT_COLLECT_TRIGGERS) {
                        
                    // Water rings
                    if (omm_obj_is_water_ring(target)) {
                        target->oInteractStatus = INT_STATUS_INTERACTED;
                        m->healCounter += 4 * target->oDamageOrCoinValue;
                    }

                    // Mushrooms 1up
                    if (omm_obj_is_mushroom_1up(target)) {
                        target->oPosX = m->pos[0];
                        target->oPosY = m->pos[1] + 60.f;
                        target->oPosZ = m->pos[2];
                    }

                    // Secrets
                    if (omm_obj_is_secret(target)) {
                        target->oPosX = m->pos[0];
                        target->oPosY = m->pos[1] + 60.f;
                        target->oPosZ = m->pos[2];
                    }
                }

                // Coins
                if (interactionFlags & OBJ_INT_COLLECT_COINS) {
                    if (omm_obj_is_coin(target)) {
                        omm_mario_interact_coin(m, target);
OMM_CAPPY_ONLY_CODE(
                        omm_cappy_try_to_target_next_coin(o);
);
                    }
                }

                // Stars/Keys
                if (interactionFlags & OBJ_INT_COLLECT_STARS) {
                    if (omm_obj_is_star_or_key(target)) {
                        omm_mario_interact_star_or_key(m, target);
OMM_CAPPY_ONLY_CODE(
                        omm_cappy_return_to_mario(o);
);
                    }
                }

                // Caps
                if (interactionFlags & OBJ_INT_COLLECT_CAPS) {
                    if (omm_obj_is_cap(target)) {
                        if (!omm_mario_interact_cap(m, target)) {
                            interact_cap(m, INTERACT_CAP, target);
                        }
                    }
                }

OMM_CAPPY_ONLY_CODE(
                // Unagi's tail
                if (omm_obj_is_unagis_tail(target)) {
                    if (target->parentObj->oAnimState != 0) {
                        target->parentObj->oAnimState = 0;
                        target->oBhvArgs = target->parentObj->oBhvArgs;
                        obj_spawn_star(target, target->oPosX, target->oPosY, target->oPosZ, 6833.f, -3654.f, 2230.f, (target->oBhvArgs >> 24) & 0x1F, false);
                        omm_cappy_return_to_mario(o);
                        sStopAndReturn = true;
                        return true;
                    }
                }
);

                // Grabbable
                if (interactionFlags & OBJ_INT_GRAB_OBJECTS) {
                    if (omm_obj_is_grabbable(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        if (!isCappy || (omm_cappy_is_mario_available(m, false) && !(m->action & (ACT_FLAG_AIR | ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)))) {
                            if (omm_mario_check_grab(m, target, true)) {
                                vec3f_set(m->vel, 0, 0, 0);
OMM_CAPPY_ONLY_CODE(
                                omm_cappy_return_to_mario(o);
);
                                sStopAndReturn = true;
                                return true;
                            }
                        }
                    }
                }

                // Weak
                if (interactionFlags & OBJ_INT_ATTACK_WEAK) {
                    if (omm_obj_is_weak(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        if (interactionFlags & OBJ_INT_ATTACK_KNOCKBACK) {
                            switch (omm_obj_get_behavior_types(target) & BHV_TYPE_KNOCKABLE) {
                                case BHV_TYPE_KNOCKABLE_0: obj_set_knockback(target, o, 0, 20.f, 30.f); break;
                                case BHV_TYPE_KNOCKABLE_1: obj_set_knockback(target, o, 1, 30.f, 0.f); break;
                            }
                        } else {
                            if ((interactionFlags & OBJ_INT_ATTACK_STRONG) && (
                                (omm_obj_is_goomba(target) && (target->oGoombaSize & 1)) ||
                                (target->behavior == bhvWigglerHead))) {
                                target->oInteractStatus = (ATTACK_GROUND_POUND_OR_TWIRL | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                            } else {
                                target->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                            }
                        }
                        omm_obj_interact_heal_mario(target, interactionFlags);
OMM_CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                        return true;
                    }
                }

                // Strong
                if (interactionFlags & OBJ_INT_ATTACK_STRONG) {
                    if (omm_obj_is_strong(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        if (interactionFlags & OBJ_INT_ATTACK_KNOCKBACK) {
                            switch (omm_obj_get_behavior_types(target) & BHV_TYPE_KNOCKABLE) {
                                case BHV_TYPE_KNOCKABLE_0: obj_set_knockback(target, o, 0, 20.f, 30.f); break;
                                case BHV_TYPE_KNOCKABLE_1: obj_set_knockback(target, o, 1, 30.f, 0.f); break;
                            }
                        } else {
                            target->oInteractStatus = INT_STATUS_INTERACTED;
                            obj_destroy(target);
                        }
                        omm_obj_interact_heal_mario(target, interactionFlags);
OMM_CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                        return true;
                    }
                }

                // Destructible
                if (interactionFlags & OBJ_INT_ATTACK_DESTRUCTIBLE) {
                    if (omm_obj_is_destructible(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        omm_obj_interact_heal_mario(target, interactionFlags);
                        target->oInteractStatus = INT_STATUS_INTERACTED;
                        obj_destroy(target);
OMM_CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                        return true;
                    }
                }

                // Breakable
                if (interactionFlags & OBJ_INT_ATTACK_BREAKABLE) {
                    if (omm_obj_is_breakable(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        if (omm_obj_is_destructible(target)) {
                            target->oInteractStatus = INT_STATUS_INTERACTED;
                            obj_destroy(target);
                        } else {
                            target->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                            set_camera_shake_from_hit(SHAKE_ATTACK);
                        }
                        omm_obj_interact_heal_mario(target, interactionFlags);
OMM_CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                        return true;
                    }
                }

                // Bullies
                if (interactionFlags & (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG)) {
                    if (omm_obj_is_bully(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        omm_obj_interact_heal_mario(target, interactionFlags);
                        target->oForwardVel = ((interactionFlags & OBJ_INT_ATTACK_STRONG) != 0 ? 4800.f : 3600.f) / target->hitboxRadius;
                        target->oMoveAngleYaw = obj_get_object1_angle_yaw_to_object2(o->parentObj ? o->parentObj : o, target);
                        target->oFaceAngleYaw = target->oMoveAngleYaw + 0x8000;
                        target->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                        target->oInteractStatus |= (interactionFlags & (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG)) << 24; // Attack type
                        play_sound(SOUND_OBJ_BULLY_METAL, target->oCameraToObject);
OMM_CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                    }
                }

                // Flames
                if (interactionFlags & OBJ_INT_ATTACK_FLAMES) {
                    if (omm_obj_is_flame(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        omm_obj_interact_heal_mario(target, interactionFlags);
                        obj_spawn_white_puff(target, SOUND_GENERAL_FLAME_OUT);
                        obj_mark_for_deletion(target);
                    }
                }
            }
        }
    }
    return interacted;
}

struct Object *omm_obj_process_interactions(struct Object *o, u32 interactionFlags) {
    struct Object *interacted = NULL;
    bool isCappy = (o == omm_cappy_get_object());
    bool notInteracted = ((interactionFlags & OBJ_INT_NOT_INTERACTED) != 0);
    sStopAndReturn = false;

    // Surfaces
    if (isCappy || (interactionFlags & OBJ_INT_PRESET_ATTACK_SURFACE)) {
        s32 rn = (isCappy ? 1 : min_s(1 + ((o->hitboxRadius - 1.f) / (CELL_SIZE >> 1)), 16));
        s32 yn = (isCappy ? 1 : min_s(1 + ((o->hitboxHeight - 1.f) / (CELL_SIZE >> 4)), 16));
        f32 dr = o->hitboxRadius / rn;
        f32 dy = o->hitboxHeight / yn;
        for (s32 yi = 0; yi <= yn; ++yi) {
            for (s32 ri = 0; ri != rn; ++ri) {
                s32 an = ((ri == 0) ? 1 : ((ri + 1) * 4));
                s32 da = 0x10000 / an;
                for (s32 ai = 0; ai != an; ++ai) {
                    struct WallCollisionData hitbox;
                    hitbox.x = o->oPosX + (dr * ri) * sins(da * ai);
                    hitbox.y = o->oPosY;
                    hitbox.z = o->oPosZ + (dr * ri) * coss(da * ai);
                    hitbox.offsetY = (dy * yi) - o->hitboxDownOffset;
                    hitbox.radius = dr;
                    if (find_wall_collisions(&hitbox) != 0) {
                        for (s32 k = 0; k != hitbox.numWalls; ++k) {
                            struct Object *obj = hitbox.walls[k]->object;
                            if (obj && omm_obj_process_one_surface_interaction(o, obj, interactionFlags)) {
                                interacted = obj;
                                if (sStopAndReturn) {
                                    return (notInteracted ? NULL : interacted);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Objects
    if (obj_check_hitbox(o, OBJ_OVERLAP_FLAG_HITBOX)) {
        for_each_object_in_interaction_lists(obj) {
            if (omm_obj_process_one_object_interaction(o, obj, interactionFlags)) {
                interacted = obj;
                if (sStopAndReturn) {
                    return (notInteracted ? NULL : interacted);
                }
            }
        }
    }

    return (notInteracted ? NULL : interacted);
}

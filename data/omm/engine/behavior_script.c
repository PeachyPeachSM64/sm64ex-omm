#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#define __bits(type)                ((1llu << (8llu * sizeof(type))) - 1llu)
#define __shift(type, j)            (32llu - ((8llu * sizeof(type)) * ((j) + 1llu)))
#define bhv_cmd_get(o, type, i, j)  (type) ((o->curBhvCommand[i] >> __shift(type, j)) & __bits(type))
#define bhv_cmd_get_ptr(o, type, i) (type) (o->curBhvCommand[i])
#define bhv_cmd_get_addr(o, i)      (uintptr_t) (&(o->curBhvCommand[i]))

typedef void (*NativeBhvFunc)(void);

void stub_behavior_script_2(void) {}

OMM_INLINE void bhv_stack_push(struct Object *o, uintptr_t bhvAddr) {
    o->bhvStack[(o->bhvStackIndex)++] = bhvAddr;
}

OMM_INLINE uintptr_t bhv_stack_pop(struct Object *o) {
    return o->bhvStack[--(o->bhvStackIndex)];
}

//
// Objects update by function
//

typedef bool (*UpdateFunc)(NativeBhvFunc func);
static OmmHMap sBhvUpdateMap = omm_hmap_zero;

#pragma GCC push_options
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define o gCurrentObject
#define m gMarioState

// Destroy the bob-omb if outside of Mario's range after being thrown or attacked
static bool bhv_bobomb__destroy_if_too_far(NativeBhvFunc func) {
    if (obj_get_distance(o, m->marioObj) > 4000 && (
        o->oHeldState == HELD_THROWN ||
        o->oHeldState == HELD_DROPPED ||
        o->oAction == BOBOMB_ACT_LAUNCHED ||
        o->oAction == BOBOMB_ACT_EXPLODE ||
        o->oAction == BOBOMB_ACT_LAVA_DEATH ||
        o->oAction == BOBOMB_ACT_DEATH_PLANE_DEATH)) {
        obj_destroy(o);
        return true;
    }
    return false;
}

// Extra action for Boo after being uncaptured
static bool bhv_boo__act_uncaptured(NativeBhvFunc func) {
    if (o->oAction == 6) {
        o->oIntangibleTimer = -1;
        o->oOpacity = abs_s(relerp_s(o->oTimer, 0, 60, 255, -255));
        obj_scale(o, ((o->oOpacity / 255.f) * 0.4f + 0.6f) * o->oBooBaseScale);
        if (o->oTimer < 30) {
            o->oPosY = max_f(o->oPosY, find_floor(o->oPosX, o->oPosY, o->oPosZ, &o->oFloor) + 40.f);
        } else if (o->oTimer == 30) {
            obj_drop_to_floor(o);
            o->oPosY += 40;
        } else if (o->oTimer >= 60) {
            o->oAction = 1;
            o->oIntangibleTimer = 0;
        }
        return true;
    }
    return false;
}

static struct {
    bool isValid;
    Vec3f throwPos;
    Vec3f bowserPos;
} sBowserThrow[1] = {0};

// Check if a Bowser throw was a perfect throw
static bool bhv_bowser_bomb__check_perfect_throw(NativeBhvFunc func) {
    if (o->oInteractStatus & INT_STATUS_HIT_MINE) {
        if (sBowserThrow->isValid) {
            struct Object *bowser = obj_get_nearest_with_behavior(o, bhvBowser);
            if (bowser) {
                s16 angleBowser = atan2s(
                    bowser->oPosZ - sBowserThrow->bowserPos[2],
                    bowser->oPosX - sBowserThrow->bowserPos[0]
                );
                s16 angleBomb = atan2s(
                    o->oPosZ - sBowserThrow->throwPos[2],
                    o->oPosX - sBowserThrow->throwPos[0]
                );
                s16 angleDiff = angleBowser - angleBomb;
                if (-0x100 < angleDiff && angleDiff < 0x100) { // 1/128, a less than 3 degrees window
                    omm_secrets_unlock(OMM_SECRET_BOWSER_THROW);
                }
            }
        }
        sBowserThrow->isValid = false;
    }
    return false;
}

// Bowser stuff
static bool bhv_bowser__pre_update(NativeBhvFunc func) {
    switch (o->oAction) {

        // Bowser intro
        // Bug fix: Make Mario look at Bowser
        case 5:
        case 6: {
            if (m->action == ACT_READING_NPC_DIALOG) {
                m->usedObj = o;
            }
        } break;

        // Bowser walk
        case 14: {
            o->oBowserFlags |= 0x40000;
        } break;

        // Bowser jump
        // Look directly at Bowser to make him charge instead of jumping
        case 13: {
            if ((o->oBowserFlags & 0x40000) && o->oSubAction == 0 && o->oTimer == 0) {
                s16 angleToBowser = obj_get_object1_angle_yaw_to_object2(m->marioObj, o);
                u16 angleDiff = (u16) abs_s((s16) (angleToBowser - m->faceAngle[1]));
                if (angleDiff < 0x1000) {
                    o->oAction = 7;
                    o->oPrevAction = 7;
                    o->oSubAction = 0;
                    o->oTimer = 0;
                }
            }
        } break;

        // Bowser defeated
        case 4: {
            omm_stats_increase(bowsersDefeated, o->oSubAction == 0);

            // Extra action for Bowser after being OHKOed
            if (o->oBowserCameraState == 10 && o->oSubAction == 0) {
                cur_obj_init_animation_with_sound(16);
                obj_set_angle(o, 0, obj_get_object1_angle_yaw_to_object2(o, m->marioObj), 0);
                obj_set_forward_and_y_vel(o, -40.f, 80.f);
                o->oBowserActionTimer = 0;
                o->oMoveFlags = 0;
                o->oSubAction = 1;
            }

            // Failsafe to keep Bowser inside the arena after being defeated
            if (o->oFloor && !SURFACE_IS_LETHAL(o->oFloor->type)) {
                Vec3f prevPos;
                obj_pos_as_vec3f(o, prevPos);
                func();
                struct Surface *floor = NULL;
                find_floor(o->oPosX, o->oPosY + 100.f, o->oPosZ, &floor);
                if (!floor || SURFACE_IS_LETHAL(floor->type)) {
                    o->oPosX = prevPos[0];
                    o->oPosZ = prevPos[2];
                    o->oPosY = max_f(o->oPosY, find_floor_height(prevPos[0], o->oPosY + 100.f, prevPos[2]));
                    obj_update_gfx(o);
                }
                return true;
            }
        } break;
    }

    // Not a throw
    if (o->oAction != 1) {
        sBowserThrow->isValid = false;
    }

    // Check if Bowser is thrown
    if (o->oHeldState == HELD_THROWN) {
        func();
        if (abs_s(o->oBowserHeldAngleVelYaw) >= 0xF80) {
            vec3f_copy(sBowserThrow->throwPos, m->pos);
            sBowserThrow->bowserPos[0] = o->oPosX - sins(o->oMoveAngleYaw);
            sBowserThrow->bowserPos[1] = o->oPosY;
            sBowserThrow->bowserPos[2] = o->oPosZ - coss(o->oMoveAngleYaw);
            sBowserThrow->isValid = true;
        }
        return true;
    }

    return false;
}

// Don't respawn a star if already collected
static bool bhv_star__dont_respawn(NativeBhvFunc func) {
    if (omm_stars_is_collected(o->oBehParams >> 24)) {
        obj_mark_for_deletion(o);
        return true;
    }
    return false;
}

// Fix Bowser's electric shockwave hitbox
static bool bhv_bowser_shock_wave__fix_hitbox(NativeBhvFunc func) {
    o->oDistanceToMario = 0.f;
    func();
    if (m->invincTimer == 0 && m->action != ACT_SHOCKED && o->oBowserShockwaveScale > 0.f && o->oTimer < 70) {

        // Check vertical distance
        if ((m->pos[1] - o->oPosY) < 40.f &&
            (o->oPosY - m->pos[1]) < (m->marioObj->hitboxHeight + 40.f)) {

            // Check horizontal distance
            f32 hDistToMario = obj_get_horizontal_distance(o, m->marioObj) / o->oBowserShockwaveScale;
            if ((1.9f < hDistToMario && hDistToMario < 2.4f) ||
                (4.0f < hDistToMario && hDistToMario < 4.8f)) {
                omm_obj_spawn_damage_mario(m->marioObj, INTERACT_SHOCK, 1);
            }
        }
    }
    return true;
}

// Fix Bowser's body anchor hitbox
static bool bhv_bowser_body_anchor__fix_hitbox(NativeBhvFunc func) {
    struct Object *bowser = o->parentObj;
    if (bowser && bowser->oAction == 4 && bowser->oSubAction >= 3) {
        cur_obj_become_intangible();
        return true;
    }
    return false;
}

// Fix Bowser's tail anchor position, hitbox and grab properties
static bool bhv_bowser_tail_anchor__fix_hitbox(NativeBhvFunc func) {
    struct Object *bowser = o->parentObj;
    if (bowser) {
        if (bowser->oAction == 4 && bowser->oSubAction >= 3) {
            cur_obj_become_intangible();
            return true;
        }
        if (OMM_MOVESET_ODYSSEY) {
            o->oPosX = bowser->oPosX - 200.f * bowser->oScaleX * sins(bowser->oFaceAngleYaw);
            o->oPosY = bowser->oPosY;
            o->oPosZ = bowser->oPosZ - 200.f * bowser->oScaleZ * coss(bowser->oFaceAngleYaw);
            o->oParentRelativePosX = 0.f;
            o->oParentRelativePosY = 0.f;
            o->oParentRelativePosZ = 0.f;
            o->oIntangibleTimer = 0;
            o->oInteractStatus = 0;
            obj_set_scale(o, bowser->oScaleX, bowser->oScaleY, bowser->oScaleZ);
            obj_reset_hitbox(o, 80, 100, 0, 0, 0, 0);
            if (o->oAction == 1) {
                o->oAction = (o->oTimer <= 30);
            } else if (bowser->oAction == 19) {
                bowser->oIntangibleTimer = -1;
                o->oAction = 0;
            }
            return true;
        }
    }
    return false;
}

// Never unload Bowser's sub during Non-Stop mode and Sparkly Stars
static bool bhv_bowsers_sub__non_stop_sparkly_stars_dont_unload(NativeBhvFunc func) {
    if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE || OMM_SPARKLY_MODE_IS_ENABLED) {
        return true;
    }
    return false;
}

// Fix glitched interactions with Bullies
static bool bhv_bully__fix_interactions(NativeBhvFunc func) {
    switch (o->oAction) {
        case BULLY_ACT_LAVA_DEATH:
        case BULLY_ACT_DEATH_PLANE_DEATH: {
            o->oInteractStatus = 0;
        } break;

        case BULLY_ACT_BACK_UP: {
            if (o->oTimer >= 15) {
                o->oMoveAngleYaw = o->oFaceAngleYaw;
                o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
                o->oAction = BULLY_ACT_PATROL;
            }
        } break;
    }
    return false;
}

// Fix moving coins ceiling collision and tangibility
static bool bhv_moving_coin__fix_collision_and_tangibility(NativeBhvFunc func) {
    if ((o->oNodeFlags & GRAPH_RENDER_ACTIVE) && !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE)) {
        if (o->oVelY > 0.f) {
            f32 floorHeight = find_floor(o->oPosX, o->oPosY + 50.f, o->oPosZ, NULL);
            f32 ceilHeight = find_ceil(o->oPosX, o->oPosY, o->oPosZ, NULL) - 50.f;
            if (o->oPosY >= ceilHeight && floorHeight < ceilHeight) {
                o->oPosY = ceilHeight;
                o->oVelY = 0.f;
            }
        }
        if (o->oVelY <= 0.f) {
            o->oIntangibleTimer = 0;
        }
    }
    return false;
}

// Skip the Chain Chomp cutscene
static bool bhv_chain_chomp__skip_cutscene(NativeBhvFunc func) {
    if (o->oAction == CHAIN_CHOMP_ACT_MOVE) {
        switch (o->oChainChompReleaseStatus) {
            case CHAIN_CHOMP_RELEASED_TRIGGER_CUTSCENE: {
                o->oChainChompReleaseStatus = CHAIN_CHOMP_RELEASED_LUNGE_AROUND;
                o->oChainChompNumLunges = 1;
                o->oWallHitboxRadius = 200.f;
                o->oMoveAngleYaw = obj_get_object1_angle_yaw_to_object2(o, m->marioObj);
                o->oAngleVelYaw = 0;
                o->oForwardVel = 0.f;
                o->oGravity = -4.f;
                o->oTimer = 60;
                obj_drop_to_floor(o);
            } break;

            case CHAIN_CHOMP_RELEASED_END_CUTSCENE: {
                mem_pool_free(gObjectMemoryPool, o->oChainChompSegments);
                for_each_object_with_behavior(obj, bhvChainChompChainPart) {
                    if (obj->parentObj == o) {
                        obj_mark_for_deletion(obj);
                    }
                }
                obj_mark_for_deletion(o);
            } return true;
        }
    }
    return false;
}

// Make sure the Chain Chomp chain part is unloaded when it should be
static bool bhv_chain_chomp_chain_part__unload(NativeBhvFunc func) {
    struct Object *chainChomp = o->parentObj;
    if (!chainChomp || !chainChomp->activeFlags || (chainChomp->behavior != bhvChainChomp && chainChomp->behavior != bhvOmmChainChompFree)) {
        obj_mark_for_deletion(o);
        return true;
    }
    struct Object *pivot = chainChomp->parentObj;
    if (o->oBehParams2ndByte != CHAIN_CHOMP_CHAIN_PART_BP_PIVOT && (!chainChomp->oChainChompSegments || !pivot || !pivot->activeFlags)) {
        obj_mark_for_deletion(o);
        return true;
    }
    obj_copy_visibility_and_transparency(o, chainChomp);
    return false;
}

// Fix the checkerboard elevator group object crash
static bool bhv_checkerboard_elevator_group__fix_crash(NativeBhvFunc func) {
    o->oGraphNode = NULL;
    return false;
}

// Update progressively the amp radius of rotation
static bool bhv_circling_amp__update_radius_of_rotation(NativeBhvFunc func) {
    static const f32 sAmpRadiusOfRotation[] = { 200.f, 300.f, 400.f };
    if (o->oAction == HOLD_ACT_SPECIAL) {
        cur_obj_become_tangible();
        obj_set_home(o, o->oPosX, o->oPosY, o->oPosZ);
        o->oAmpRadiusOfRotation = 0;
        o->oAnimState = 1;
        o->oAction = AMP_ACT_IDLE;
    }
    o->oAmpRadiusOfRotation = min_f(o->oAmpRadiusOfRotation + 5.f, sAmpRadiusOfRotation[o->oBehParams2ndByte]);
    return false;
}

// Clams don't deal damage and can't interact with captures
static bool bhv_clam__no_damage_no_capture_interact(NativeBhvFunc func) {
    extern struct ObjectHitbox sClamShellHitbox;
    sClamShellHitbox.damageOrCoinValue = 0;
    func();
    if (omm_mario_is_capture(m)) {
        cur_obj_become_intangible();
    } else if (o->oAction == 0 || o->oAnimFrame < 30) {
        cur_obj_become_tangible();
    }
    return true;
}

// Boos always carry blue coins, unless inside a specific level
// Change the behavior and fix the coin scale when leaving the Boo
static bool bhv_coin_inside_boo__always_blue_fix_scale(NativeBhvFunc func) {
    if (gCurrLevelNum == OMM_LEVEL_YELLOW_COIN_BOO) {
        o->oGraphNode = geo_layout_to_graph_node(NULL, yellow_coin_geo);
        obj_scale(o, 1.f);
    } else {
        o->oGraphNode = geo_layout_to_graph_node(NULL, blue_coin_geo);
        obj_scale(o, 0.7f);
    }
    if (o->oAction == 1) {
        obj_set_behavior(o, bhvYellowCoin);
        obj_scale(o, 1.f);
        bhv_moving_coin__fix_collision_and_tangibility(func);
    }
    return false;
}

// Always spawn DDD poles during Non-Stop mode
static bool bhv_ddd_pole__non_stop_always_spawn(NativeBhvFunc func) {
    if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) {
        o->hitboxDownOffset = 100.f;
        o->oDDDPoleMaxOffset = 100.f * o->oBehParams2ndByte;
        return true;
    }
    return false;
}

// Exclamation box fix and QoL
static bool bhv_exclamation_box__fix_hitbox_and_respawn_koopa_shells(NativeBhvFunc func) {
    func();

    // Fix the exclamation box hitbox
    if (o->oAction == 2) {
        o->hitboxHeight *= 1.8f;
    }

    // Make koopa shells respawn
    if (o->oAction == 4) {
        if (o->activeFlags) {
            omm_stats_increase(exclamationBoxesBroken, 1);
        } else if (o->oBehParams2ndByte == 3) {
            o->activeFlags = ACTIVE_FLAG_ACTIVE;
            o->oAction = 5;
            cur_obj_hide();
        }
    }
    return true;
}

// Make Eyerok trigger its dialog and unload faster
static bool bhv_eyerok_boss__die_faster(NativeBhvFunc func) {
    if (o->oAction == EYEROK_BOSS_ACT_DIE) {
        if (o->oTimer > 45 && o->oTimer < 60) {
            o->oTimer = 60;
        }
        if (o->oTimer > 60) {
            o->oTimer = 121;
        }
    }
    return false;
}

// Unload the falling platform if it goes under the floor lowest point to fix the collision vertex underflow
static bool bhv_falling_bowser_platform__unload_under_floor_limit(NativeBhvFunc func) {
    if (o->oPosY < FLOOR_LOWER_LIMIT) {
        obj_mark_for_deletion(o);
        return true;
    }
    return false;
}

// Fix the falling pillar displacement
static bool bhv_falling_pillar__fix_move_yaw(NativeBhvFunc func) {
    o->oMoveAngleYaw = o->oFaceAngleYaw;
    return false;
}

// Remove the limit of active fire piranha plants
// Make the fire piranha plants die faster (use the regular piranha plant death animation)
static bool bhv_fire_piranha_plant__no_active_limit_and_die_faster(NativeBhvFunc func) {
    sNumActiveFirePiranhaPlants = 0;
    if (o->oAction == FIRE_PIRANHA_PLANT_ACT_HIDE && o->oFirePiranhaPlantDeathSpinTimer) {
        if (o->oFirePiranhaPlantDeathSpinTimer > 0) {
            if (o->oSubAction == 0) {
                obj_play_sound(o, SOUND_OBJ2_PIRANHA_PLANT_DYING);
                for (s32 i = 0; i != 20; ++i) {
                    spawn_object(o, MODEL_PURPLE_MARBLE, bhvPurpleParticle);
                }
                o->oSubAction = 1;
            }
            cur_obj_become_intangible();
            obj_anim_play(o, 2, 1.5f);
            o->oInteractStatus = 0;
            if (obj_anim_is_near_end(o)) {
                obj_play_sound(o, SOUND_OBJ_ENEMY_DEFEAT_SHRINK);
                o->oFirePiranhaPlantDeathSpinTimer = -1;
                o->oTimer = 0;
            }
        } else {
            cur_obj_become_intangible();
            o->oFirePiranhaPlantScale -= 0.1f;
            if (o->oFirePiranhaPlantScale <= 0) {
                o->oFirePiranhaPlantDeathSpinTimer = 0;
                o->oFirePiranhaPlantActive = TRUE;
                o->oHealth = 0;
                return false;
            }
            obj_scale(o, o->oFirePiranhaPlantScale);
        }
        return true;
    }
    return false;
}

// Enable the fire spitter hitbox so Cappy can target and interact with it
static bool bhv_fire_spitter__enable_hitbox(NativeBhvFunc func) {
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 160, 320, 0, 0, 0, 0);
    return false;
}

// Make flamethrowers always active
static bool bhv_flamethrower__always_active(NativeBhvFunc func) {
    o->oDistanceToMario = 0.f;
    return false;
}

// Fix a lot of Goomba issues
static bool bhv_goomba__update(NativeBhvFunc func) {

    // Make all Goombas standalone
    if (o->parentObj && o->parentObj != o) {
        set_object_respawn_info_bits(o->parentObj, (o->oBehParams2ndByte & GOOMBA_BP_TRIPLET_FLAG_MASK) >> 2);
        o->parentObj->oBehParams = o->parentObj->oBehParams | (o->oBehParams2ndByte & GOOMBA_BP_TRIPLET_FLAG_MASK) << 6;
        o->oBehParams = (o->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK) << 16;
        o->oBehParams2ndByte = (o->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK);
        o->parentObj = o;
        o->prevObj = NULL;
    }

    // Destroy at death barrier
    if (obj_is_on_ground(o) && SURFACE_IS_LETHAL(o->oFloor->type)) {
        obj_destroy(o);
        return true;
    }

    // Fix Metal Mario jump/ground pound interactions
    if ((o->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK) == GOOMBA_SIZE_HUGE && (m->flags & MARIO_METAL_CAP) && obj_check_if_collided_with_object(o, gMarioObject)) {
        if (omm_mario_is_ground_pounding(m) || omm_mario_is_ground_pound_landing(m)) {
            o->oInteractStatus = (INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED | ATTACK_GROUND_POUND_OR_TWIRL);
        } else if (m->vel[1] < 0.f && m->pos[1] > o->oPosY) {
            o->oInteractStatus = (INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED | ATTACK_FROM_ABOVE);
            m->pos[1] = o->oPosY + o->hitboxHeight;
            m->vel[1] = 30.f;
            m->flags &= ~MARIO_UNKNOWN_08;
            obj_play_sound(m->marioObj, SOUND_ACTION_BOUNCE_OFF_OBJECT);
        }
    }

    // Update
    func();

    // Make Goombas intangible during death actions
    if (o->oAction >= 100) {
        o->oIntangibleTimer = -1;
    }

    return true;
}

void bhv_omm_sparkly_grand_star_spawn_sparkles() {
    for (u32 sparklyMode = 1; sparklyMode != OMM_SPARKLY_MODE_COUNT; ++sparklyMode) {
        if (obj_has_geo_layout(o, OMM_SPARKLY_STAR_GEO_OPAQUE[sparklyMode])) {
            if ((o->oTimer % 4) == 0) {
                omm_obj_spawn_sparkly_star_sparkle(o, sparklyMode, o->oGraphYOffset, 12.f, 0.8f, 80.f);
                break;
            }
        }
    }
}

// Make the Grand Star intangible until the cutscene is done
// Spawn sparkles if it's a Sparkly Grand Star
static bool bhv_grand_star__set_intangible_and_sparkles(NativeBhvFunc func) {
    if (o->oAction < 2) {
        cur_obj_become_intangible();
    }
    bhv_omm_sparkly_grand_star_spawn_sparkles();
    return false;
}

// Destroy heave-hos by ground pounding them with Metal Mario (BPQHTKXMeCA&t=160s)
static bool bhv_heave_ho__destroy_metal(NativeBhvFunc func) {
    if (o->oHeldState == HELD_FREE && (m->flags & MARIO_METAL_CAP) && omm_mario_is_ground_pound_landing(m) && obj_check_if_collided_with_object(m->marioObj, o)) {
        obj_destroy(o);
        return true;
    }
    return false;
}

// Make boxes spawned by a purple switch indestructible
static bool bhv_hidden_object__indestructible(NativeBhvFunc func) {
    o->oInteractStatus &= ~INT_STATUS_WAS_ATTACKED;
    if (m->marioObj->platform == o) m->marioObj->platform = NULL;
    func();
    obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    o->oIntangibleTimer = -1;
    return true;
}

// Reset vertical velocity
static bool bhv_homing_amp__reset_y_vel(NativeBhvFunc func) {
    if (o->oAction == HOLD_ACT_SPECIAL) {
        cur_obj_become_tangible();
        o->oAnimState = 1;
        o->oAction = HOMING_AMP_ACT_GIVE_UP;
    }
    o->oVelY = 0.f;
    return false;
}

// Skip Hoot dialog
static bool bhv_hoot__skip_dialog(NativeBhvFunc func) {
    if (o->oHootAvailability == HOOT_AVAIL_WANTS_TO_TALK) {
        o->oHootAvailability = HOOT_AVAIL_READY_TO_FLY;
        cur_obj_become_tangible();
    }
    return false;
}

// Fix intro Lakitu's model
static bool bhv_intro_lakitu__fix_model(NativeBhvFunc func) {
    o->oGraphNode = geo_layout_to_graph_node(NULL, lakitu_geo);
    return false;
}

// Always make the jumping box spawn coins when destroyed
static bool bhv_jumping_box__always_spawn_coins(NativeBhvFunc func) {
    func();
    if (o->oHeldState == HELD_FREE && o->oAction == 1 && !o->activeFlags) {
        create_sound_spawner(SOUND_GENERAL_BREAK_BOX);
        obj_explode_and_spawn_coins(46.f, 1);
    }
    return true;
}

// Teleports KtQ back to his path if he falls off
static bool bhv_koopa_the_quick__path_check_failsafe(NativeBhvFunc func) {
    func();
    if (omm_obj_is_koopa_the_quick(o)) {
        obj_path_check_failsafe(o);
    }
    return true;
}

// Don't destroy the Koopa shell after leaving it, but can't interact with it for 15 frames
static bool bhv_koopa_shell__dont_destroy(NativeBhvFunc func) {
    if (o->oAction == 1 && (o->oInteractStatus & INT_STATUS_STOP_RIDING)) {
        o->oPosY = m->pos[1] - 50;
        o->oVelY = 0;
        o->oAction = 0;
        o->oInteractStatus = 0;
        o->oIntangibleTimer = 15;
    }
    return false;
}

// Underwater Koopa shell fix and QoL
static bool bhv_koopa_shell_underwater__dont_destroy_and_fall(NativeBhvFunc func) {

    // Don't destroy the underwater Koopa shell after leaving it
    if ((o->oInteractStatus & INT_STATUS_STOP_RIDING) || o->oHeldState == HELD_DROPPED || o->oHeldState == HELD_THROWN) {
        obj_set_angle(o, 0, m->faceAngle[1], 0);
        if (o->oHeldState == HELD_THROWN) {
            obj_set_forward_and_y_vel(o, 40, 20);
        } else {
            obj_set_forward_and_y_vel(o, 0, 0);
        }
        o->oInteractStatus = 0;
        o->oHeldState = HELD_FREE;
        cur_obj_become_tangible();
        cur_obj_enable_rendering();
    }

    func();

    // Give the underwater Koopa shell proper physics
    if (o->oHeldState == HELD_FREE) {
        obj_set_forward_and_y_vel(o,
            o->oForwardVel * 0.98f,
            (obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ)) ? max_f(o->oVelY - 2, -20) : max_f(o->oVelY - 4, -75))
        );
        f32 prevVel = o->oVelY;
        perform_object_step(o, 0);
        if (obj_is_on_ground(o)) {
            if (abs_f(prevVel) > 5.f) {
                obj_set_forward_and_y_vel(o, o->oForwardVel / 2.f, abs_f(prevVel) / 2.f);
            } else {
                obj_set_forward_and_y_vel(o, 0, 0);
            }
        }
        obj_update_gfx(o);
    } else {
        obj_set_vel(o, 0, 0, 0);
    }
    return true;
}

// Make Mad pianos always tangible
static bool bhv_mad_piano__always_tangible(NativeBhvFunc func) {
    static struct ObjectHitbox sMadPianoHitbox = { INTERACT_MR_BLIZZARD, 0, 3, 99, 0, 200, 150, 200, 150 };
    obj_set_hitbox(o, &sMadPianoHitbox);
    obj_set_home(o, o->oPosX, o->oPosY, o->oPosZ);
    cur_obj_become_tangible();
    return false;
}

// Fix the water ring hitbox
static bool bhv_manta_ray_water_ring__fix_hitbox(NativeBhvFunc func) {
    cur_obj_init_animation(0);
    o->oWaterRingScalePhaseX = (s32) (random_u16() & 0xFFF) + 0x1000;
    o->oWaterRingScalePhaseY = (s32) (random_u16() & 0xFFF) + 0x1000;
    o->oWaterRingScalePhaseZ = (s32) (random_u16() & 0xFFF) + 0x1000;
    o->oWaterRingNormalX = 0.f;
    o->oWaterRingNormalY = 1.f;
    o->oWaterRingNormalZ = 0.f;
    vec3f_rotate_zxy(
        &o->oWaterRingNormalX,
        &o->oWaterRingNormalX,
        o->oFaceAnglePitch,
        o->oFaceAngleYaw,
        o->oFaceAngleRoll
    );
    o->oWaterRingMarioDistInFront = water_ring_calc_mario_dist();
    o->oOpacity = 150;
    return true;
}

// Always spawn Mips when all stars are collected
static bool bhv_mips__spawn_at_max_stars(NativeBhvFunc func) {
    if (m->numStars >= omm_stars_get_total_star_count(OMM_GAME_MODE) || (o->oFlags & OBJ_FLAG_CAPTURE_AFTER_WARP)) {
        o->oBehParams2ndByte = OMM_NUM_MIPS_STARS - 1;
        o->oMipsForwardVelocity = 50.f;
        o->oInteractionSubtype = INT_SUBTYPE_HOLDABLE_NPC;
        o->oGravity = 15.f;
        o->oFriction = 0.89f;
        o->oBuoyancy = 1.2f;
        cur_obj_init_animation(0);
        return true;
    }
    return false;
}

// Reduce moving yellow coin RNG
static bool bhv_moving_yellow_coin__less_rng(NativeBhvFunc func) {
    o->oForwardVel /= 2.f;
    return false;
}

// Mr. Blizzard bug fixes
static bool bhv_mr_blizzard__fix_tangibility_and_die_faster(NativeBhvFunc func) {

    // Make Mr. Blizzard intangible when he's hidden
    if (obj_is_hidden(o) || o->oAction == MR_BLIZZARD_ACT_DEATH) {
        cur_obj_become_intangible();
    } else {
        cur_obj_become_tangible();
    }

    func();

    // Make Mr. Blizzard spin and die faster
    if (o->oAction == MR_BLIZZARD_ACT_DEATH) {
        o->oTimer++;
        return false;
    }

    // Properly reset dizziness if hidden in the snow
    if (o->oMrBlizzardGraphYOffset < 0.f) {
        o->oFaceAngleRoll = 0;
        o->oMrBlizzardDizziness = 0.f;
        o->oMrBlizzardChangeInDizziness = 0.f;
    } else {
        o->oMrBlizzardChangeInDizziness += sign_0_f(o->oMrBlizzardChangeInDizziness) * 4.f;
    }
    return true;
}

// Mr I bug fixes
static bool bhv_mr_i__update(NativeBhvFunc func) {

    // Prevent that stupidly bad placed Mr. I in BBH from shooting Mario as soon as he opens the door
    if (omm_mario_is_capture(m) && POBJ_IS_OPENING_DOORS) {
        o->oMrIUnk104 = 0;
    }

    // Make Mr. I instantly target Mario if he's close enough
    if (o->oAction <= 1 && obj_get_distance(o, m->marioObj) < 300.f * o->oScaleX) {
        s16 angleToMario = obj_get_object1_angle_yaw_to_object2(o, m->marioObj);
        o->oFaceAngleYaw = angleToMario;
        o->oMoveAngleYaw = angleToMario;
        o->oAction = 2;
        o->oMrIUnkFC = 0;
        o->oMrIUnk100 = 0;
        o->oMrIUnk104 = 0;
        o->oMrIUnk108 = (s32) (random_float() * 50.f + 50.f);
    }

    // Make Mr. I die faster
    if (o->oAction == 3) {
        func();
        o->oTimer++;
    }

    // Update
    func();

    // Fix the model
    if (!o->oGraphNode) {
        o->oGraphNode = geo_layout_to_graph_node(NULL, mr_i_geo);
    }
    o->oGraphYOffset = 100.f * o->oScaleY;

    // Fix the iris
    struct Object *iris = obj_get_first_with_behavior_and_parent(o, bhvMrIBody);
    if (!iris && omm_models_get_model_pack_index(o->oGraphNode->georef) == -1) {
        iris = obj_spawn_from_geo(o, mr_i_iris_geo, bhvMrIBody);
        iris->parentObj = o;
    }

    return true;
}

// Mr. I iris bug fixes
static bool bhv_mr_i_body__copy_parent_offset_or_unload(NativeBhvFunc func) {
    struct Object *mrI = o->parentObj;

    // Make sure the iris is unloaded when it should be
    if (!mrI || !mrI->activeFlags || mrI->behavior != bhvMrI || !mrI->oGraphNode || omm_models_get_model_pack_index(mrI->oGraphNode->georef) != -1) {
        obj_mark_for_deletion(o);
        return true;
    }

    // Update
    func();

    // Copy the body offset
    o->oGraphYOffset = mrI->oGraphYOffset;

    obj_copy_visibility_and_transparency(o, mrI);
    return true;
}

// Fix the glitchy camera when Mario is holding a pole due to it being slightly above the ground
static bool bhv_pole_base__fix_camera(NativeBhvFunc func) {
    f32 fy = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
    if (fy < o->oPosY && o->oPosY < fy + 5) {
        o->oPosY = fy;
    }
    return false;
}

// Teleports the racing penguin back to his path if he falls off
static bool bhv_racing_penguin__path_check_failsafe(NativeBhvFunc func) {
    func();
    if (o->oAction == RACING_PENGUIN_ACT_RACE) {
        obj_path_check_failsafe(o);
    }
    return true;
}

// Make piranha plants die faster
static bool bhv_piranha_plant__die_faster(NativeBhvFunc func) {
    switch (o->oAction) {
        case PIRANHA_PLANT_ACT_ATTACKED: {
            cur_obj_become_intangible();
            obj_anim_play(o, 2, 1.5f);
            o->oInteractStatus = 0;
            if (obj_anim_is_near_end(o)) {
                obj_play_sound(o, SOUND_OBJ_ENEMY_DEFEAT_SHRINK);
                o->oPiranhaPlantScale = 1.f;
                o->oAction = PIRANHA_PLANT_ACT_SHRINK_AND_DIE;
            }
            if (o->activeFlags & ACTIVE_FLAG_FAR_AWAY) {
                o->oAction = PIRANHA_PLANT_ACT_IDLE;
            }
        } return true;

        case PIRANHA_PLANT_ACT_SHRINK_AND_DIE: {
            o->oPiranhaPlantScale = relerp_0_1_f(o->oTimer, 0, 15, 1, 0);
            if (o->oPiranhaPlantScale == 0) {
                cur_obj_spawn_loot_blue_coin();
                o->oAction = PIRANHA_PLANT_ACT_WAIT_TO_RESPAWN;
            }
            obj_scale(o, o->oPiranhaPlantScale);
            if (o->activeFlags & ACTIVE_FLAG_FAR_AWAY) {
                o->oAction = PIRANHA_PLANT_ACT_IDLE;
            }
        } return true;
    }
    return false;
}

// Make the pokey manager object intangible
static bool bhv_pokey__set_intangible(NativeBhvFunc func) {
    cur_obj_become_intangible();
    return false;
}

// Pokey body part bug fixes
static bool bhv_pokey_body_part__unload_or_make_intangible(NativeBhvFunc func) {
    struct Object *pokey = o->parentObj;

    // Make sure the Pokey body part is unloaded when it should be
    if (o->oHealth > 0 && (!pokey || !pokey->activeFlags || pokey->behavior != bhvPokey)) {
        obj_mark_for_deletion(o);
        return true;
    }

    // Make the body part intangible if Pokey is held
    o->oIntangibleTimer = -1 * (pokey->oHeldState != HELD_FREE);

    obj_copy_visibility_and_transparency(o, pokey);
    return false;
}

// Allow any type of ground pound to trigger purple switches
static bool bhv_purple_switch__any_ground_pound_trigger(NativeBhvFunc func) {
    if (o->oAction == PURPLE_SWITCH_IDLE && omm_mario_is_ground_pound_landing(m) &&
        o == m->marioObj->platform && obj_get_horizontal_distance(o, m->marioObj) < 85.f * o->oScaleX) {
        o->oAction = PURPLE_SWITCH_PRESSED;
        return true;
    }
    return false;
}

// Make recovery hearts slowly heal Mario when spinning
static bool bhv_recovery_heart__heal_mario_for_1_segment(NativeBhvFunc func) {
    if (o->oSpinningHeartTotalSpin + o->oAngleVelYaw >= 0x10000) {
        omm_health_heal_mario(m, true);
    }
    return false;
}

// Disable respawner during "defeat x enemies" challenges
static bool bhv_respawner__sparkly_stars_disable(NativeBhvFunc func) {
    if (omm_sparkly_context_get_data(OMM_SPARKLY_DATA_OBJECT_TYPE) == OMM_SPARKLY_DATA_OBJECT_TYPE_ENEMIES) {
        obj_mark_for_deletion(o);
        return true;
    }
    return false;
}

// Fix Skeeters home position
static bool bhv_skeeter__fix_home_pos(NativeBhvFunc func) {
    func();
    obj_set_home(o, o->oPosX, o->oPosY, o->oPosZ);
    return true;
}

// Skip the SL snowman dialog if Sparkly Stars are enabled
static bool bhv_sl_snowman_wind__sparkly_stars_always_blow(NativeBhvFunc func) {
    if (OMM_SPARKLY_MODE_IS_ENABLED) {
        o->oSubAction = SL_SNOWMAN_WIND_ACT_BLOWING;
    }
    return false;
}

// Clear the "drop immediately" flag if the penguin is already released
static bool bhv_small_penguin__clear_flag(NativeBhvFunc func) {
    if (m->heldObj != o) {
        o->oInteractionSubtype &= ~INT_SUBTYPE_DROP_IMMEDIATELY;
    }
    return false;
}

// Big Snowman bug fixes
static bool bhv_snowmans_bottom__fix_pos_yaw_and_checkpoint(NativeBhvFunc func) {
    switch (o->oAction) {

        // Fix a softlock that can happen if Mario is not in the 400 units radius after starting the dialog
        case 0: {
            if (m->action == ACT_READING_NPC_DIALOG && m->usedObj == o && vec3f_dist(m->pos, &o->oPosX) > 400) {
                Vec3f dv = {
                    m->pos[0] - o->oPosX,
                    m->pos[1] - o->oPosY,
                    m->pos[2] - o->oPosZ,
                };
                vec3f_set_mag(dv, 399.f);
                m->pos[0] = o->oPosX + dv[0];
                m->pos[1] = o->oPosY + dv[1];
                m->pos[2] = o->oPosZ + dv[2];
            }
        } break;

        // No need to touch the checkpoint to lead the bottom to its head after the end of its path
        case 1: {
            o->oSnowmansBottomUnk1AC = 1;
        } break;

        // Fix snowman's body yaw after reaching the head
        case 3: {
            struct Object *snowmansHead = obj_get_nearest_with_behavior(o, bhvSnowmansHead);
            if (snowmansHead) {
                obj_set_angle(o, 0, obj_get_object1_angle_yaw_to_object2(o, snowmansHead), 0);
            }
        } break;
    }
    return false;
}

// Don't spawn the CCM snowman as a whole if Non-Stop is enabled
static bool bhv_snowmans_head__non_stop_dont_spawn_whole(NativeBhvFunc func) {
    if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) {
        cur_obj_scale(0.7f);
        o->oGravity = 5.f;
        o->oFriction = 0.999f;
        o->oBuoyancy = 2.f;
        return true;
    }
    return false;
}

// Don't spawn spawned star if already collected (init needs to be called first)
static bool bhv_spawned_star__dont_respawn(NativeBhvFunc func) {
    func();
    bhv_star__dont_respawn(func);
    return true;
}

// Fixed spawned star loop
static bool bhv_spawned_star__loop(NativeBhvFunc func) {

    // Don't respawn if already collected
    if (bhv_star__dont_respawn(func)) {
        return true;
    }

    // Actions
    switch (o->oAction) {

        // Init and moving to destination
        case 0: {

            // Init
            if (o->oTimer == 0) {
                cutscene_object(CUTSCENE_STAR_SPAWN, o);
                set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
                if (o->oBehParams2ndByte == 0) { // 100 coins, Toad, Mips
                    obj_set_home(o, m->marioObj->oPosX, m->marioObj->oPosY + 250.f, m->marioObj->oPosZ);
                } else { // Exclamation box
                    obj_set_home(o, o->oPosX, o->oPosY, o->oPosZ);
                }

                // Make sure the star doesn't end up below a floor or above a ceiling
                f32 floorHeight = find_floor(o->oHomeX, o->oHomeY + 100.f, o->oHomeZ, NULL);
                f32 ceilHeight = find_ceil(o->oHomeX, o->oHomeY - 100.f, o->oHomeZ, NULL);
                o->oHomeY = max_f(o->oHomeY, floorHeight + 60.f);
                o->oHomeY = min_f(o->oHomeY, ceilHeight - 60.f);

                o->oPrevPosX = o->oPosX;
                o->oPrevPosZ = o->oPosZ;
                o->oPosY = o->oHomeY;
                o->oVelY = 46.f;
                o->oGravity = -4.f;
                o->oAngleVelYaw = 0x800;
                spawn_mist_particles();
            }

            // Moving
            f32 t = clamp_0_1_f(o->oTimer / 24.f);
            o->oPosX = lerp_f(t, o->oPrevPosX, o->oHomeX);
            o->oPosY += o->oVelY;
            o->oPosZ = lerp_f(t, o->oPrevPosZ, o->oHomeZ);
            spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
            if (gCutsceneFocus == o) {
                cur_obj_play_sound_1(SOUND_ENV_STAR);
            }
            if (o->oTimer >= 24) {
                o->oAction = 1;
                o->oPosY = o->oHomeY;
                o->oVelY = 20.f;
                o->oGravity = -1.f;
                if (gCutsceneFocus == o) {
                    audio_play_star_jingle();
                }
            }
        } break;

        // Moving up and down
        case 1: {
            o->oVelY = max_f(o->oVelY, -4.f);
            o->oPosY += o->oVelY;
            spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
            if (o->oVelY < 0 && o->oPosY <= o->oHomeY) {
                o->oAction = 2;
                o->oPosY = o->oHomeY;
                o->oVelY = 0.f;
                o->oGravity = 0.f;
                gObjCutsceneDone = TRUE;
            }
        } break;

        // Immediately stop the cutscene for an early star grab
        case 2: {
            stop_cutscene(gCamera);
            clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
            o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
            o->oAction = 3;
        } break;

        // Idle
        case 3: {
            extern struct ObjectHitbox sSparkleSpawnStarHitbox;
            obj_set_hitbox(o, &sSparkleSpawnStarHitbox);
            o->oAngleVelYaw = max_s(o->oAngleVelYaw - 0x40, 0x400);
            if (o->oInteractStatus & INT_STATUS_INTERACTED) {
                o->oInteractStatus = 0;
                obj_mark_for_deletion(o);
                return true;
            }
        } break;
    }

    o->oVelY += o->oGravity;
    o->oFaceAngleYaw += o->oAngleVelYaw;
    o->oInteractStatus = 0;
    return true;
}

// Fixed star spawn loop
static bool bhv_star_spawn__loop(NativeBhvFunc func) {

    // Don't respawn if already collected
    if (bhv_star__dont_respawn(func)) {
        return true;
    }

    // Actions
    switch (o->oAction) {

        // Spinning in place
        case 0: {
            o->oPrevPosX = o->oPosX;
            o->oPrevPosY = o->oPosY;
            o->oPrevPosZ = o->oPosZ;
            o->oFaceAngleYaw += 0x1000;
            if (o->oTimer > 20) {
                o->oAction = 1;
            }
        } break;

        // Moving to destination
        case 1: {
            f32 t = clamp_0_1_f(o->oTimer / 30.f);
            o->oPosX = lerp_f(t, o->oPrevPosX, o->oHomeX);
            o->oPosY = lerp_f(t, o->oPrevPosY, o->oHomeY) + sins((o->oTimer * 0x8000) / 30) * 400.f;
            o->oPosZ = lerp_f(t, o->oPrevPosZ, o->oHomeZ);
            o->oFaceAngleYaw += 0x1000;
            spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
            if (gCutsceneFocus == o) {
                cur_obj_play_sound_1(SOUND_ENV_STAR);
            }
            if (o->oTimer >= 30) {
                o->oAction = 2;
                o->oForwardVel = 0;
                if (gCutsceneFocus == o) {
                    audio_play_star_jingle();
                }
            }
        } break;

        // Moving up and down
        case 2: {
            if (o->oTimer < 20) {
                o->oPosY += (20 - o->oTimer);
            } else {
                o->oPosY -= 10;
            }
            spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
            o->oFaceAngleYaw += 0x1000 - o->oTimer * 0x10;
            if (gCutsceneFocus == o) {
                cur_obj_play_sound_1(SOUND_ENV_STAR);
            }
            if (o->oPosY <= o->oHomeY) {
                if (gCutsceneFocus == o) {
                    cur_obj_play_sound_2(SOUND_GENERAL_STAR_APPEARS);
                }
                cur_obj_become_tangible();
                o->oPosY = o->oHomeY;
                o->oAction = 3;
            }
        } break;

        // Idle
        case 3: {
            o->oFaceAngleYaw += 0x800;
            if (o->oTimer == 20) {
                gObjCutsceneDone = TRUE;
                clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
            }
            if (o->oInteractStatus & INT_STATUS_INTERACTED) {
                o->oInteractStatus = 0;
                obj_mark_for_deletion(o);
                return true;
            }
        } break;
    }
    return true;
}

// After a capture, make the shark progressively return to its orbit
static bool bhv_sushi_shark__approach_orbit(NativeBhvFunc func) {
    if (o->oAction == 1) {
        f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);
        o->oPosY -= waterLevel;
        obj_approach_orbit(o, o->oSushiSharkUnkF4 / 0x80, 1700.f, 0x200, 200.f, 1.2f, 0x200, 0x400);
        o->oPosY += waterLevel;
        o->oFlags &= ~OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE;
        obj_update_gfx(o);
        f32 offset = (1.f - (o->oTimer / 180.f)) * 100.f * o->oScaleX * coss((0x10000 * o->oAnimFrame) / o->oCurrAnim->mLoopEnd);
        o->oGfxPos[0] = o->oPosX - offset * sins(o->oFaceAngleYaw + 0x4000);
        o->oGfxPos[2] = o->oPosZ - offset * coss(o->oFaceAngleYaw + 0x4000);
        if (m->marioObj->oPosY - waterLevel > -500.f && o->oPosY - waterLevel > -200.f) {
            spawn_object_with_scale(o, MODEL_WAVE_TRAIL, bhvObjectWaveTrail, 4.f);
        }
        if ((o->oTimer & 0xF) == 0) {
            cur_obj_play_sound_2(SOUND_OBJ_SUSHI_SHARK_WATER_SOUND);
        }
        o->oInteractStatus = 0;
        o->oSushiSharkUnkF4 += 0x80;
        if (o->oTimer == 180) {
            o->oFlags |= OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE;
            o->oAction = 0;
        }
        return true;
    }
    return false;
}

// Allow any type of ground pound to drain the water on top of Tiny THI
static bool bhv_thi_tiny_island_top__any_ground_pound_trigger(NativeBhvFunc func) {
    if (o->oAction == 0 && !gTHIWaterDrained && omm_mario_is_ground_pound_landing(m) && obj_get_distance(o, m->marioObj) < 500.f) {
        obj_spawn_particles(o, 30, MODEL_WHITE_PARTICLE_SMALL, 0, 40, 0, 20, 40, -4, 2, 0);
        obj_spawn_triangle_break_particles(o, OBJ_SPAWN_TRI_BREAK_PRESET_DIRT_SMALL);
        obj_play_sound(o, SOUND_GENERAL_ACTIVATE_CAP_SWITCH);
        o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        o->oAction++;
        return true;
    }
    return false;
}

// Make Tuxie's mother endlessly chase Mario if he's holding or capturing Tuxie
// Becomes very angry after some time...
static bool bhv_tuxies_mother__chase_mario(NativeBhvFunc func) {
    if (o->oAction == 2) {
        obj_set_home(o, m->pos[0], m->pos[1], m->pos[2]);
        if (omm_mario_is_capture(m) && gOmmCapture->behavior == bhvUnused20E0) {
            gOmmCapture->oHeldState = HELD_HELD;
        }
        if (o->oForwardVel > 0.f) {
            if (o->oTimer > 60) {
                func();
                cur_obj_move_standard(-78);
                obj_anim_set_speed(o, 2.f);
                obj_play_sound(o, SOUND_OBJ_BIG_PENGUIN_WALK * (o->oTimer % 5 == 0));
                o->oInteractType = INTERACT_DAMAGE;
                o->hurtboxRadius = o->hitboxRadius + 5.f;
                o->hurtboxHeight = o->hitboxHeight + 5.f;
                o->oDamageOrCoinValue = 3;
                return true;
            }
        } else {
            o->oInteractType = INTERACT_TEXT;
            o->hurtboxRadius = 0;
            o->hurtboxHeight = 0;
            o->oDamageOrCoinValue = 0;
            o->oTimer = 0;
        }
    }
    return false;
}

// Enable the tail hitbox so Cappy can target and interact with it
static bool bhv_unagi_subobject__enable_tail_hitbox(NativeBhvFunc func) {
    extern struct ObjectHitbox sUnagiHitbox;
    if (o->oBehParams2ndByte == -4) {
        obj_set_params(o, 0, 0, sUnagiHitbox.health, 0, true);
        obj_reset_hitbox(o, sUnagiHitbox.radius, sUnagiHitbox.height, 0, 0, 0, sUnagiHitbox.downOffset);
    }
    return false;
}

// Whomp bug fixes
static bool bhv_whomp__fix_act_5_and_always_active_if_not_boss(NativeBhvFunc func) {

    // Make regular Whomps always active and interactable
    if (o->oAction == 0 && o->oBehParams2ndByte == 0) {
        cur_obj_set_pos_to_home();
        o->oIntangibleTimer = 0;
        o->oAction = 1;
    }

    // Fix Whomp behavior if already landed before action 5
    if (o->oAction == 5 && obj_is_on_ground(o)) {
        if (o->oSubAction == 0) {
            o->oMoveFlags = (o->oMoveFlags & ~OBJ_MOVE_ON_GROUND) | OBJ_MOVE_LANDED;
        } else {
            o->oAction = 6;
        }
    }
    return false;
}

// Wiggler bug fixes
static bool bhv_wiggler__fix_audio_and_dialog_timer(NativeBhvFunc func) {

    // Fix the lowered background music volume when Wiggler is not talking
    if (o->oAction == WIGGLER_ACT_WALK) {
        if (o->oWigglerTextStatus == WIGGLER_TEXT_STATUS_COMPLETED_DIALOG) {
            music_unlower_volume(SEQ_PLAYER_LEVEL, 60);
            o->oWigglerTextStatus++;
        }
    }

    // Fix the (unnecessary) dialog delay after unsquishing
    if (o->oAction == WIGGLER_ACT_JUMPED_ON && o->oScaleY >= 4.f) {
        o->oTimer = max_s(30, o->oTimer);
    }
    return false;
}

// Wooden post bug fixes
static bool bhv_wooden_post__spawn_coins_if_pounded(NativeBhvFunc func) {

    // Make sure the Chain Chomp wooden post still references the Chain Chomp if captured
    if (o->parentObj != o && omm_mario_is_capture(m) && gOmmCapture == o->parentObj && o->oWoodenPostOffsetY + o->oWoodenPostSpeedY <= -190.f) {
        o->oWoodenPostOffsetY = -190.f;
        o->oWoodenPostSpeedY = 0;
    }

    // Make wooden posts spawn coins if fully pounded
    if (!(o->oBehParams & WOODEN_POST_BP_NO_COINS_MASK) && o->oWoodenPostOffsetY <= -190.f) {
        o->oPosY = o->oHomeY;
        obj_spawn_loot_yellow_coins(o, 5, 20.f);
        set_object_respawn_info_bits(o, 1);
        o->oBehParams |= WOODEN_POST_BP_NO_COINS_MASK;
        o->oPosY = o->oHomeY + o->oWoodenPostOffsetY;
    }
    return false;
}

#if OMM_GAME_IS_SM64

// Spawn Yoshi if all captures (except Yoshi) have been registered, but cannot talk if not all stars have been collected
static bool bhv_yoshi__spawn_if_all_captures(NativeBhvFunc func) {
    func();
    bool allCaptures = (omm_save_file_get_capture_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE) & OMM_ALL_CAPTURES_BUT_YOSHI) == OMM_ALL_CAPTURES_BUT_YOSHI;
    bool allStars = omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE) == omm_stars_get_total_star_count(OMM_GAME_MODE);
    if (allCaptures || allStars) {
        o->activeFlags = ACTIVE_FLAG_ACTIVE;
        if (!allStars) {
            o->oInteractionSubtype = 0;
        }
    }
    return true;
}

// Make Yoshi give something more interesting than 100 useless 1-ups
static bool bhv_yoshi__big_surprise(NativeBhvFunc func) {
    if (o->oAction == YOSHI_ACT_GIVE_PRESENT) {
        o->oAction = YOSHI_ACT_IDLE;
        omm_obj_spawn_problem(m->marioObj);
    }
    return false;
}

#endif

#undef o
#undef m
#pragma GCC pop_options

OMM_AT_STARTUP static void omm_setup_behavior_update_functions_map() {
    static const void *sBhvUpdateTable[][2] = {
        { bhv_big_bully_with_minions_loop,          bhv_bully__fix_interactions },
        { bhv_blue_coin_jumping_loop,               bhv_moving_coin__fix_collision_and_tangibility },
        { bhv_blue_coin_sliding_loop,               bhv_moving_coin__fix_collision_and_tangibility },
        { bhv_bobomb_loop,                          bhv_bobomb__destroy_if_too_far },
        { bhv_boo_loop,                             bhv_boo__act_uncaptured },
        { bhv_bowser_bomb_loop,                     bhv_bowser_bomb__check_perfect_throw },
        { bhv_bowser_course_red_coin_star_loop,     bhv_star__dont_respawn },
        { bhv_bowser_loop,                          bhv_bowser__pre_update },
        { bhv_bowser_shock_wave_loop,               bhv_bowser_shock_wave__fix_hitbox },
        { bhv_bowser_body_anchor_loop,              bhv_bowser_body_anchor__fix_hitbox },
        { bhv_bowser_tail_anchor_loop,              bhv_bowser_tail_anchor__fix_hitbox },
        { bhv_bowsers_sub_loop,                     bhv_bowsers_sub__non_stop_sparkly_stars_dont_unload },
        { bhv_bully_loop,                           bhv_bully__fix_interactions },
        { bhv_ccm_touched_star_spawn_loop,          bhv_star__dont_respawn },
        { bhv_chain_chomp_update,                   bhv_chain_chomp__skip_cutscene },
        { bhv_chain_chomp_chain_part_update,        bhv_chain_chomp_chain_part__unload },
        { bhv_checkerboard_elevator_group_init,     bhv_checkerboard_elevator_group__fix_crash },
        { bhv_circling_amp_loop,                    bhv_circling_amp__update_radius_of_rotation },
        { bhv_clam_loop,                            bhv_clam__no_damage_no_capture_interact },
        { bhv_coin_loop,                            bhv_moving_coin__fix_collision_and_tangibility },
        { bhv_coin_inside_boo_loop,                 bhv_coin_inside_boo__always_blue_fix_scale },
        { bhv_collect_star_init,                    bhv_star__dont_respawn },
        { bhv_collect_star_loop,                    bhv_star__dont_respawn },
        { bhv_ddd_pole_init,                        bhv_ddd_pole__non_stop_always_spawn },
        { bhv_exclamation_box_loop,                 bhv_exclamation_box__fix_hitbox_and_respawn_koopa_shells },
        { bhv_eyerok_boss_loop,                     bhv_eyerok_boss__die_faster },
        { bhv_falling_bowser_platform_loop,         bhv_falling_bowser_platform__unload_under_floor_limit },
        { bhv_falling_pillar_loop,                  bhv_falling_pillar__fix_move_yaw },
        { bhv_fire_piranha_plant_update,            bhv_fire_piranha_plant__no_active_limit_and_die_faster },
        { bhv_fire_spitter_update,                  bhv_fire_spitter__enable_hitbox },
        { bhv_flamethrower_loop,                    bhv_flamethrower__always_active },
        { bhv_goomba_update,                        bhv_goomba__update },
        { bhv_grand_star_loop,                      bhv_grand_star__set_intangible_and_sparkles },
        { bhv_heave_ho_loop,                        bhv_heave_ho__destroy_metal },
        { bhv_hidden_object_loop,                   bhv_hidden_object__indestructible },
        { bhv_hidden_red_coin_star_init_,           bhv_star__dont_respawn },
        { bhv_hidden_red_coin_star_loop_,           bhv_star__dont_respawn },
        { bhv_hidden_star_init,                     bhv_star__dont_respawn },
        { bhv_hidden_star_loop,                     bhv_star__dont_respawn },
        { bhv_homing_amp_loop,                      bhv_homing_amp__reset_y_vel },
        { bhv_hoot_loop,                            bhv_hoot__skip_dialog },
        { bhv_intro_lakitu_loop,                    bhv_intro_lakitu__fix_model },
        { bhv_jumping_box_loop,                     bhv_jumping_box__always_spawn_coins },
        { bhv_koopa_update,                         bhv_koopa_the_quick__path_check_failsafe },
        { bhv_koopa_shell_loop,                     bhv_koopa_shell__dont_destroy },
        { bhv_koopa_shell_underwater_loop,          bhv_koopa_shell_underwater__dont_destroy_and_fall },
        { bhv_mad_piano_update,                     bhv_mad_piano__always_tangible },
        { bhv_manta_ray_water_ring_init,            bhv_manta_ray_water_ring__fix_hitbox },
        { bhv_mips_init,                            bhv_mips__spawn_at_max_stars },
        { bhv_moving_blue_coin_loop,                bhv_moving_coin__fix_collision_and_tangibility },
        { bhv_moving_yellow_coin_init,              bhv_moving_yellow_coin__less_rng },
        { bhv_moving_yellow_coin_loop,              bhv_moving_coin__fix_collision_and_tangibility },
        { bhv_mr_blizzard_update,                   bhv_mr_blizzard__fix_tangibility_and_die_faster },
        { bhv_mr_i_loop,                            bhv_mr_i__update },
        { bhv_mr_i_body_loop,                       bhv_mr_i_body__copy_parent_offset_or_unload },
        { bhv_pole_base_loop,                       bhv_pole_base__fix_camera },
        { bhv_racing_penguin_update,                bhv_racing_penguin__path_check_failsafe },
        { bhv_piranha_plant_loop,                   bhv_piranha_plant__die_faster },
        { bhv_pokey_update,                         bhv_pokey__set_intangible },
        { bhv_pokey_body_part_update,               bhv_pokey_body_part__unload_or_make_intangible },
        { bhv_purple_switch_loop,                   bhv_purple_switch__any_ground_pound_trigger },
        { bhv_recovery_heart_loop,                  bhv_recovery_heart__heal_mario_for_1_segment },
        { bhv_respawner_loop,                       bhv_respawner__sparkly_stars_disable },
        { bhv_skeeter_update,                       bhv_skeeter__fix_home_pos },
        { bhv_sl_snowman_wind_loop,                 bhv_sl_snowman_wind__sparkly_stars_always_blow },
        { bhv_small_penguin_loop,                   bhv_small_penguin__clear_flag },
        { bhv_snowmans_bottom_loop,                 bhv_snowmans_bottom__fix_pos_yaw_and_checkpoint },
        { bhv_snowmans_head_init,                   bhv_snowmans_head__non_stop_dont_spawn_whole },
        { bhv_spawned_star_init,                    bhv_spawned_star__dont_respawn },
        { bhv_spawned_star_loop,                    bhv_spawned_star__loop },
        { bhv_star_spawn_init,                      bhv_star__dont_respawn },
        { bhv_star_spawn_loop,                      bhv_star_spawn__loop },
        { bhv_sushi_shark_loop,                     bhv_sushi_shark__approach_orbit },
        { bhv_thi_tiny_island_top_loop,             bhv_thi_tiny_island_top__any_ground_pound_trigger },
        { bhv_tuxies_mother_loop,                   bhv_tuxies_mother__chase_mario },
        { bhv_unagi_subobject_loop,                 bhv_unagi_subobject__enable_tail_hitbox },
        { bhv_whomp_loop,                           bhv_whomp__fix_act_5_and_always_active_if_not_boss },
        { bhv_wiggler_update,                       bhv_wiggler__fix_audio_and_dialog_timer },
        { bhv_wooden_post_update,                   bhv_wooden_post__spawn_coins_if_pounded },
#if OMM_GAME_IS_SM64
        { bhv_yoshi_init,                           bhv_yoshi__spawn_if_all_captures },
        { bhv_yoshi_loop,                           bhv_yoshi__big_surprise },
#endif
    };
    for (s32 i = 0; i != array_length(sBhvUpdateTable); ++i) {
        omm_hmap_insert(sBhvUpdateMap, (uintptr_t) sBhvUpdateTable[i][0], sBhvUpdateTable[i][1]);
    }
}

static void obj_update_behavior_func(struct Object *o, NativeBhvFunc func) {
    if (o->activeFlags) {
        obj_fix_hitbox(o);
        obj_safe_step(o, false);
        s32 i = omm_hmap_find(sBhvUpdateMap, (uintptr_t) func);
        if (func && (i == -1 || !omm_hmap_get(sBhvUpdateMap, UpdateFunc, i)(func))) {
            func();
        }
        obj_fix_hitbox(o);
        obj_safe_step(o, true);
    }
}

//
// Commands
//

static bool bhv_cmd_begin(struct Object *o) {
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_delay(struct Object *o) {
    if (o->bhvDelayTimer < bhv_cmd_get(o, s16, 0, 1) - 1) {
        o->bhvDelayTimer++;
    } else {
        o->bhvDelayTimer = 0;
        o->curBhvCommand++;
    }
    return false;
}

static bool bhv_cmd_call(struct Object *o) {
    bhv_stack_push(o, bhv_cmd_get_addr(o, 2));
    o->curBhvCommand = bhv_cmd_get_ptr(o, const BehaviorScript *, 1);
    return true;
}

static bool bhv_cmd_return(struct Object *o) {
    o->curBhvCommand = (const BehaviorScript *) bhv_stack_pop(o);
    return true;
}

static bool bhv_cmd_goto(struct Object *o) {
    o->curBhvCommand = bhv_cmd_get_ptr(o, const BehaviorScript *, 1);
    return true;
}

static bool bhv_cmd_begin_repeat(struct Object *o) {
    bhv_stack_push(o, bhv_cmd_get_addr(o, 1));
    bhv_stack_push(o, bhv_cmd_get(o, s16, 0, 1));
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_end_repeat(struct Object *o) {
    s32 count = bhv_stack_pop(o) - 1;
    if (count > 0) {
        o->curBhvCommand = (const BehaviorScript *) bhv_stack_pop(o);
        bhv_stack_push(o, bhv_cmd_get_addr(o, 0));
        bhv_stack_push(o, count);
    } else {
        bhv_stack_pop(o);
        o->curBhvCommand++;
    }
    return false;
}

static bool bhv_cmd_end_repeat_continue(struct Object *o) {
    bhv_cmd_end_repeat(o);
    return true;
}

static bool bhv_cmd_begin_loop(struct Object *o) {
    bhv_stack_push(o, bhv_cmd_get_addr(o, 1));
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_end_loop(struct Object *o) {
    o->curBhvCommand = (const BehaviorScript *) bhv_stack_pop(o);
    bhv_stack_push(o, bhv_cmd_get_addr(o, 0));
    return false;
}

static bool bhv_cmd_break(struct Object *o) {
    return false;
}

static bool bhv_cmd_break_unused(struct Object *o) {
    return false;
}

static bool bhv_cmd_call_native(struct Object *o) {
    NativeBhvFunc behaviorFunc = bhv_cmd_get_ptr(o, NativeBhvFunc, 1);
    obj_update_behavior_func(o, behaviorFunc);
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_add_float(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    f32 value = bhv_cmd_get(o, s16, 0, 1);
    obj_add_float(o, field, value);
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_set_float(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    f32 value = bhv_cmd_get(o, s16, 0, 1);
    obj_set_float(o, field, value);
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_add_int(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    s32 value = bhv_cmd_get(o, s16, 0, 1);
    obj_add_int(o, field, value);
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_set_int(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    s32 value = bhv_cmd_get(o, s16, 0, 1);
    obj_set_int(o, field, value);
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_or_int(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    u32 value = bhv_cmd_get(o, u16, 0, 1);
    obj_or_int(o, field, value);
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_bit_clear(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    u32 value = bhv_cmd_get(o, u16, 0, 1);
    obj_and_int(o, field, ~value);
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_set_int_rand_rshift(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    s32 value = bhv_cmd_get(o, s16, 0, 1);
    s32 shift = bhv_cmd_get(o, s16, 1, 0);
    obj_set_int(o, field, value + (random_u16() >> shift));
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_random_float(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    f32 value = bhv_cmd_get(o, s16, 0, 1);
    f32 range = bhv_cmd_get(o, s16, 1, 0);
    obj_set_float(o, field, value + (range * random_float()));
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_random_int(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    s32 value = bhv_cmd_get(o, s16, 0, 1);
    s32 range = bhv_cmd_get(o, s16, 1, 0);
    obj_set_int(o, field, value + (s32) (range * random_float()));
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_add_random_float(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    f32 value = bhv_cmd_get(o, s16, 0, 1);
    f32 range = bhv_cmd_get(o, s16, 1, 0);
    obj_set_float(o, field, obj_get_float(o, field) + value + (range * random_float()));
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_add_int_rand_rshift(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    s32 value = bhv_cmd_get(o, s16, 0, 1);
    s32 shift = bhv_cmd_get(o, s16, 1, 0);
    obj_set_int(o, field, obj_get_int(o, field) + value + (random_u16() >> shift));
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_nop_1(struct Object *o) {
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_nop_2(struct Object *o) {
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_nop_3(struct Object *o) {
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_set_model(struct Object *o) {
    o->oGraphNode = gLoadedGraphNodes[bhv_cmd_get(o, s16, 0, 1)];
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_spawn_child(struct Object *o) {
    struct Object *child = spawn_object_at_origin(o, 0, bhv_cmd_get(o, u32, 1, 0), bhv_cmd_get_ptr(o, const BehaviorScript *, 2));
    obj_copy_pos_and_angle(child, o);
    o->curBhvCommand += 3;
    return true;
}

static bool bhv_cmd_deactivate(struct Object *o) {
    o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    return false;
}

static bool bhv_cmd_drop_to_floor(struct Object *o) {
    if (omm_obj_is_goomba(o) && o->oGoombaStackParent) {
        o->oGoombaStackParent = NULL;
    } else {
        o->oPosY = find_floor_height(o->oPosX, o->oPosY + 200.f, o->oPosZ);
        o->oMoveFlags |= OBJ_MOVE_ON_GROUND;
    }
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_sum_float(struct Object *o) {
    s32 fieldDest = bhv_cmd_get(o, u8, 0, 1);
    s32 fieldSrc1 = bhv_cmd_get(o, u8, 0, 2);
    s32 fieldSrc2 = bhv_cmd_get(o, u8, 0, 3);
    obj_set_float(o, fieldDest, obj_get_float(o, fieldSrc1) + obj_get_float(o, fieldSrc2));
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_sum_int(struct Object *o) {
    s32 fieldDest = bhv_cmd_get(o, u8, 0, 1);
    s32 fieldSrc1 = bhv_cmd_get(o, u8, 0, 2);
    s32 fieldSrc2 = bhv_cmd_get(o, u8, 0, 3);
    obj_set_int(o, fieldDest, obj_get_int(o, fieldSrc1) + obj_get_int(o, fieldSrc2));
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_billboard(struct Object *o) {
    o->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_hide(struct Object *o) {
    cur_obj_hide();
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_set_hitbox(struct Object *o) {
    o->hitboxRadius = bhv_cmd_get(o, s16, 1, 0);
    o->hitboxHeight = bhv_cmd_get(o, s16, 1, 1);
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_nop_4(struct Object *o) {
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_delay_var(struct Object *o) {
    s32 num = obj_get_int(o, bhv_cmd_get(o, u8, 0, 1));
    if (o->bhvDelayTimer < num - 1) {
        o->bhvDelayTimer++;
    } else {
        o->bhvDelayTimer = 0;
        o->curBhvCommand++;
    }
    return false;
}

static bool bhv_cmd_begin_repeat_unused(struct Object *o) {
    bhv_stack_push(o, bhv_cmd_get_addr(o, 1));
    bhv_stack_push(o, bhv_cmd_get(o, u8, 0, 1));
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_load_animations(struct Object *o) {
    o->oAnimations = bhv_cmd_get_ptr(o, struct Animation **, 1);
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_animate(struct Object *o) {
    geo_obj_init_animation(&o->header.gfx, &(o->oAnimations[bhv_cmd_get(o, u8, 0, 1)]));
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_spawn_child_with_param(struct Object *o) {
    struct Object *child = spawn_object_at_origin(o, 0, bhv_cmd_get(o, u32, 1, 0), bhv_cmd_get_ptr(o, const BehaviorScript *, 2));
    child->oBehParams2ndByte = bhv_cmd_get(o, s16, 0, 1);
    obj_copy_pos_and_angle(child, o);
    o->curBhvCommand += 3;
    return true;
}

static bool bhv_cmd_load_collision_data(struct Object *o) {
    o->collisionData = bhv_cmd_get_ptr(o, void *, 1);
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_hitbox_with_offset(struct Object *o) {
    o->hitboxRadius = bhv_cmd_get(o, s16, 1, 0);
    o->hitboxHeight = bhv_cmd_get(o, s16, 1, 1);
    o->hitboxDownOffset = bhv_cmd_get(o, s16, 2, 0);
    o->curBhvCommand += 3;
    return true;
}

static bool bhv_cmd_spawn_obj(struct Object *o) {
    struct Object *object = spawn_object_at_origin(o, 0, bhv_cmd_get(o, u32, 1, 0), bhv_cmd_get_ptr(o, const BehaviorScript *, 2));
    obj_copy_pos_and_angle(object, o);
    o->prevObj = object;
    o->curBhvCommand += 3;
    return true;
}

static bool bhv_cmd_set_home(struct Object *o) {
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_set_hurtbox(struct Object *o) {
    o->hurtboxRadius = bhv_cmd_get(o, s16, 1, 0);
    o->hurtboxHeight = bhv_cmd_get(o, s16, 1, 1);
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_interact_type(struct Object *o) {
    o->oInteractType = bhv_cmd_get(o, u32, 1, 0);
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_obj_physics(struct Object *o) {
    o->oWallHitboxRadius = bhv_cmd_get(o, s16, 1, 0);
    o->oGravity          = bhv_cmd_get(o, s16, 1, 1) / 100.f;
    o->oBounciness       = bhv_cmd_get(o, s16, 2, 0) / 100.f;
    o->oDragStrength     = bhv_cmd_get(o, s16, 2, 1) / 100.f;
    o->oFriction         = bhv_cmd_get(o, s16, 3, 0) / 100.f;
    o->oBuoyancy         = bhv_cmd_get(o, s16, 3, 1) / 100.f;
    o->curBhvCommand += 5;
    return true;
}

static bool bhv_cmd_set_interact_subtype(struct Object *o) {
    o->oInteractionSubtype = bhv_cmd_get(o, u32, 1, 0);
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_scale(struct Object *o) {
    cur_obj_scale(bhv_cmd_get(o, s16, 0, 1) / 100.f);
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_parent_bit_clear(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    u32 value = bhv_cmd_get(o, u32, 1, 0);
    o->parentObj->OBJECT_FIELD_S32(field) &= ~value;
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_animate_texture(struct Object *o) {
    if ((gGlobalTimer % bhv_cmd_get(o, s16, 0, 1)) == 0) {
        o->oAnimState++;
    }
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_disable_rendering(struct Object *o) {
    o->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
    o->curBhvCommand++;
    return true;
}

static bool bhv_cmd_set_int_unused(struct Object *o) {
    s32 field = bhv_cmd_get(o, u8,  0, 1);
    s32 value = bhv_cmd_get(o, s16, 1, 1);
    obj_set_int(o, field, value);
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_spawn_water_droplet(struct Object *o) {
    spawn_water_droplet(o, bhv_cmd_get_ptr(o, void *, 1));
    o->curBhvCommand += 2;
    return true;
}

static bool bhv_cmd_cylboard(struct Object *o) {
    o->oNodeFlags |= GRAPH_RENDER_CYLBOARD;
    o->curBhvCommand++;
    return true;
}


//
// Behavior update
//

typedef bool (*BhvCommandProc)(struct Object *);
static BhvCommandProc sBehaviorCmdTable[] = {
/* 0x00 */ bhv_cmd_begin,
/* 0x01 */ bhv_cmd_delay,
/* 0x02 */ bhv_cmd_call,
/* 0x03 */ bhv_cmd_return,
/* 0x04 */ bhv_cmd_goto,
/* 0x05 */ bhv_cmd_begin_repeat,
/* 0x06 */ bhv_cmd_end_repeat,
/* 0x07 */ bhv_cmd_end_repeat_continue,
/* 0x08 */ bhv_cmd_begin_loop,
/* 0x09 */ bhv_cmd_end_loop,
/* 0x0A */ bhv_cmd_break,
/* 0x0B */ bhv_cmd_break_unused,
/* 0x0C */ bhv_cmd_call_native,
/* 0x0D */ bhv_cmd_add_float,
/* 0x0E */ bhv_cmd_set_float,
/* 0x0F */ bhv_cmd_add_int,
/* 0x10 */ bhv_cmd_set_int,
/* 0x11 */ bhv_cmd_or_int,
/* 0x12 */ bhv_cmd_bit_clear,
/* 0x13 */ bhv_cmd_set_int_rand_rshift,
/* 0x14 */ bhv_cmd_set_random_float,
/* 0x15 */ bhv_cmd_set_random_int,
/* 0x16 */ bhv_cmd_add_random_float,
/* 0x17 */ bhv_cmd_add_int_rand_rshift,
/* 0x18 */ bhv_cmd_nop_1,
/* 0x19 */ bhv_cmd_nop_2,
/* 0x1A */ bhv_cmd_nop_3,
/* 0x1B */ bhv_cmd_set_model,
/* 0x1C */ bhv_cmd_spawn_child,
/* 0x1D */ bhv_cmd_deactivate,
/* 0x1E */ bhv_cmd_drop_to_floor,
/* 0x1F */ bhv_cmd_sum_float,
/* 0x20 */ bhv_cmd_sum_int,
/* 0x21 */ bhv_cmd_billboard,
/* 0x22 */ bhv_cmd_hide,
/* 0x23 */ bhv_cmd_set_hitbox,
/* 0x24 */ bhv_cmd_nop_4,
/* 0x25 */ bhv_cmd_delay_var,
/* 0x26 */ bhv_cmd_begin_repeat_unused,
/* 0x27 */ bhv_cmd_load_animations,
/* 0x28 */ bhv_cmd_animate,
/* 0x29 */ bhv_cmd_spawn_child_with_param,
/* 0x2A */ bhv_cmd_load_collision_data,
/* 0x2B */ bhv_cmd_set_hitbox_with_offset,
/* 0x2C */ bhv_cmd_spawn_obj,
/* 0x2D */ bhv_cmd_set_home,
/* 0x2E */ bhv_cmd_set_hurtbox,
/* 0x2F */ bhv_cmd_set_interact_type,
/* 0x30 */ bhv_cmd_set_obj_physics,
/* 0x31 */ bhv_cmd_set_interact_subtype,
/* 0x32 */ bhv_cmd_scale,
/* 0x33 */ bhv_cmd_parent_bit_clear,
/* 0x34 */ bhv_cmd_animate_texture,
/* 0x35 */ bhv_cmd_disable_rendering,
/* 0x36 */ bhv_cmd_set_int_unused,
/* 0x37 */ bhv_cmd_spawn_water_droplet,
/* 0x38 */ bhv_cmd_cylboard,
/* 0x39 */ bhv_cmd_billboard,
/* 0x3A */ bhv_cmd_billboard,
/* 0x3B */ bhv_cmd_nop_1,
/* 0x3C */ bhv_cmd_nop_1,
/* 0x3D */ bhv_cmd_nop_1,
/* 0x3E */ bhv_cmd_nop_1,
/* 0x3F */ bhv_cmd_nop_1,
};

void obj_update(struct Object *o) {
    struct Object *currentObject = gCurrentObject;
    u32 objFlags = o->oFlags;

    // Don't update if dormant
    if (obj_is_dormant(o)) {
        return;
    }

#if OMM_GAME_IS_R96X
    // Update every 2 frames during Chaos: Slow Time
    if (o != gMarioObject && Cheats.EnableCheats && Cheats.ChaosSlowTime && (gGlobalTimer & 1)) {
        obj_load_collision_model(o);
        return;
    }
#endif

    // Distance from the object to Mario
    if (objFlags & OBJ_FLAG_COMPUTE_DIST_TO_MARIO) {
        o->oDistanceToMario = dist_between_objects(o, gMarioObject);
    }

    // Angle from the object to Mario
    if (objFlags & OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO) {
        o->oAngleToMario = obj_get_object1_angle_yaw_to_object2(o, gMarioObject);
    }

    // Reset timer on action change
    if (o->oPrevAction != o->oAction) {
        o->oPrevAction = o->oAction;
        o->oSubAction = 0;
        o->oTimer = 0;
    }

    // Execute the behavior script
    gCurrentObject = o;
    while (sBehaviorCmdTable[(*(o->curBhvCommand) >> 24) & 0x3F](o));
    gCurrentObject = o;

    // Increment timer
    if (o->oTimer < 0x3FFFFFFF) {
        o->oTimer++;
    }

    // Reset timer on action change
    if (o->oPrevAction != o->oAction) {
        o->oPrevAction = o->oAction;
        o->oSubAction = 0;
        o->oTimer = 0;
    }

    // Update object flags
    objFlags = o->oFlags;

    // Set face angle to move angle
    if (objFlags & OBJ_FLAG_SET_FACE_ANGLE_TO_MOVE_ANGLE) {
        obj_set_face_angle_to_move_angle(o);
    }

    // Set face yaw to move yaw
    if (objFlags & OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW) {
        o->oFaceAngleYaw = o->oMoveAngleYaw;
    }

    // Move horizontally
    if (objFlags & OBJ_FLAG_MOVE_XZ_USING_FVEL) {
        cur_obj_move_xz_using_fvel_and_yaw();
    }

    // Move vertically
    if (objFlags & OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL) {
        cur_obj_move_y_with_terminal_vel();
    }

    // Update transform from parent
    if (objFlags & OBJ_FLAG_TRANSFORM_RELATIVE_TO_PARENT) {
        obj_build_transform_relative_to_parent(o);
    }

    // Update throw matrix from transform
    if (objFlags & OBJ_FLAG_SET_THROW_MATRIX_FROM_TRANSFORM) {
        obj_set_throw_matrix_from_transform(o);
    }

    // Update gfx pos and angle
    if (objFlags & OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE) {
        obj_update_gfx(o);
    }

    // Update visibility
    if (o->oRoom != -1) {
        cur_obj_enable_rendering_if_mario_in_room();
    } else if (
        (objFlags & OBJ_FLAG_COMPUTE_DIST_TO_MARIO) &&
       !(objFlags & OBJ_FLAG_ACTIVE_FROM_AFAR) &&
        o->oHeldState == HELD_FREE &&
       !o->collisionData)
    {
        o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
        o->activeFlags &= ~ACTIVE_FLAG_FAR_AWAY;
    }

    // Restore current object
    gCurrentObject = currentObject;
}

void cur_obj_update(void) {
    obj_update(gCurrentObject);
}

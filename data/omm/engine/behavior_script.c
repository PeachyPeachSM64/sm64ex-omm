#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define __bits(type)                ((1llu << (8llu * sizeof(type))) - 1llu)
#define __shift(type, j)            (32llu - ((8llu * sizeof(type)) * ((j) + 1llu)))
#define bhv_cmd_get(type, i, j)     (type) ((gCurBhvCommand[i] >> __shift(type, j)) & __bits(type))
#define bhv_cmd_get_ptr(type, i)    (type) (gCurBhvCommand[i])
#define bhv_cmd_get_addr(i)         (uintptr_t) (&gCurBhvCommand[i])
#define o                           gCurrentObject

typedef void (*NativeBhvFunc)(void);

void stub_behavior_script_2(void) {}

OMM_INLINE void stack_push(uintptr_t bhvAddr) {
    o->bhvStack[(o->bhvStackIndex)++] = bhvAddr;
}

OMM_INLINE uintptr_t stack_pop(void) {
    return o->bhvStack[--(o->bhvStackIndex)];
}

//
// Knockback
//

static bool cur_obj_update_knockback() {
    if (obj_is_knocked_back(o)) {

        // Check the timestamp to not apply knockback multiple times per frame
        u16 timestamp = (u16) (o->oSubAction >> 16);
        u16 globtimer = (gGlobalTimer & 0xFFFF);
        if (timestamp != globtimer) {
            o->oTimer--;
            o->oSubAction &= 0xFFFF;
            o->oSubAction |= (globtimer << 16);
            switch (o->oAction >> 16) {

                // Bouncy knockback (mostly ground enemies)
                case 1: {
                    f32 velY = o->oVelY;
                    perform_object_step(o, 0);
                    o->oVelY -= 4.f;
                    if (obj_is_on_ground(o)) {
                        o->oVelX /= 2.f;
                        o->oVelZ /= 2.f;
                        o->oVelY = abs_f(velY) / 2.f;
                        o->oForwardVel = sqrtf(sqr_f(o->oVelX) + sqr_f(o->oVelZ));
                        if (o->oForwardVel < 4.f || o->oVelY < 4.f) {
                            o->oAction &= 0xFFFF;
                        }
                    }
                } break;

                // 1-dir knockback (mostly flying enemies)
                case 2: {
                    perform_object_step(o, 0);
                    if (o->oWall) {
                        Vec3f d = { o->oVelX, 0.f, o->oVelZ };
                        Vec3f n = { o->oWall->normal.x, 0.f, o->oWall->normal.z };
                        f32 dot = vec3f_dot(d, n);
                        o->oVelX = 0.75f * (d[0] - 2.f * dot * n[0]);
                        o->oVelY = 0.75f * o->oVelY;
                        o->oVelZ = 0.75f * (d[2] - 2.f * dot * n[2]);
                        o->oWall = NULL;
                    } else {
                        o->oVelX *= 0.95f;
                        o->oVelY *= 0.95f;
                        o->oVelZ *= 0.95f;
                    }
                    f32 dv = vec3f_length(&o->oVelX);
                    if (dv < 4.f) {
                        o->oAction &= 0xFFFF;
                    }
                } break;

                // End of knockback
                default: {
                    o->oSubAction &= 0xFFFF;
                    obj_set_vel(o, 0.f, 0.f, 0.f);
                    obj_set_home(o, o->oPosX, o->oPosY, o->oPosZ);
                    obj_set_knockback(o, NULL, -1, 0, 0);
                } break;
            }
        }
        return true;
    }
    return false;
}

//
// Various fixes
//

static void cur_obj_fix_hitbox() {
    if (OMM_MOVESET_ODYSSEY) {
        const OmmBhvDataHitbox *hitbox = omm_behavior_data_get_hitbox(o->behavior);
        if (hitbox) {
            o->hitboxRadius = hitbox->hitboxRadius * (hitbox->scale ? abs_f(o->oScaleX) : 1.f);
            o->hitboxHeight = hitbox->hitboxHeight * (hitbox->scale ? abs_f(o->oScaleY) : 1.f);
            o->hurtboxRadius = hitbox->hurtboxRadius * (hitbox->scale ? abs_f(o->oScaleX) : 1.f);
            o->hurtboxHeight = hitbox->hurtboxHeight * (hitbox->scale ? abs_f(o->oScaleY) : 1.f);
            o->hitboxDownOffset = hitbox->hitboxDownOffset * (hitbox->scale ? abs_f(o->oScaleY) : 1.f);
        }
    }
}

static void cur_obj_path_failsafe() {
    const Trajectory *traj = (const Trajectory *) o->oPathedPrevWaypoint;
    if (traj && traj[0] != TRAJECTORY_END() && traj[4] != TRAJECTORY_END()) {
        Vec3f p0 = { traj[1], traj[2], traj[3] };
        Vec3f p1 = { traj[5], traj[6], traj[7] };
        Vec3f p; f32 t;
        vec3f_get_projected_point_on_line(p, &t, &o->oPosX, p0, p1);
        if (o->oPosY < p[1] - 500) {
            omm_debug_log("Triggered failsafe!\n");
            vec3f_interpolate(&o->oPosX, p0, p1, clamp_0_1_f(t));
            o->oPosY = find_floor_height(o->oPosX, o->oPosY + 300, o->oPosZ);
        }
    }
}

//
// Objects update by function
//

typedef bool (*UpdateFunc)(NativeBhvFunc func);
static OmmHMap sBhvUpdateMap = omm_hmap_zero;

#pragma GCC push_options
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define m gMarioState
#define mo gMarioObject

static bool do_nothing(NativeBhvFunc func) {
    return false;
}

// Make 1-up always spawn, ignore save flags
static bool bhv_1up__always_spawn(NativeBhvFunc func) {
    bhv_1up_common_init();
    return true;
}

// Trigger the life-up cutscene or fully heal Mario
static bool bhv_1up__life_up(NativeBhvFunc func) {
    if ((o->oNodeFlags & GRAPH_RENDER_ACTIVE) && !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE) && o->oIntangibleTimer == 0) {
        if (obj_check_if_collided_with_object(o, mo)) {
#if OMM_GAME_IS_R96X
            // Green Demon effect
            if (Cheats.EnableCheats && Cheats.ChaosGreenDemon) {
                play_sound(SOUND_MENU_CAMERA_BUZZ, gDefaultSoundArgs);
                m->health = OMM_HEALTH_DEAD;
                m->healCounter = 0;
                m->hurtCounter = 0;
                obj_mark_for_deletion(o);
                return true;
            }
#endif
            omm_health_life_up(m);
            obj_mark_for_deletion(o);
            return true;
        }
    }
    return false;
}

// Respawn the blue coin switch if there is blue coins not collected
// Show the number of blue coins remaining if Mario stands on it
static bool bhv_blue_coin_switch__show_coins_and_respawn(NativeBhvFunc func) {
    switch (o->oAction) {

        // Any type of ground pound triggers blue coin switches
        case BLUE_COIN_SWITCH_ACT_IDLE: {
            o->oHomeY = o->oPosY;
            if (mo->platform == o) {
                if (omm_mario_is_ground_pound_landing(m)) {
                    play_sound(SOUND_GENERAL_SWITCH_DOOR_OPEN, o->oCameraToObject);
                    o->oAction = BLUE_COIN_SWITCH_ACT_RECEDING;
                    o->oVelY = -20.f;
                    o->oGravity = 0.f;
                    obj_load_collision_model(o);
                    return true;
                } else if (!obj_get_first_with_behavior(bhvOmmBlueCoinsNumber)) {
                    omm_spawn_blue_coins_number(m->marioObj);
                }
            }
        } break;

        // Change action instead of unloading after timer ends
        case BLUE_COIN_SWITCH_ACT_TICKING: {
            if (o->oTimer >= 239) {
                play_sound(SOUND_GENERAL_SWITCH_DOOR_OPEN, o->oCameraToObject);
                o->oAction = 3;
                o->oPosY = o->oHomeY - 100.f;
                o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
                obj_load_collision_model(o);
                return true;
            }
        } break;

        // Rise up again and return to idle action
        case 3: {
            o->oPosY = min_f(o->oPosY + 20.f, o->oHomeY);
            obj_load_collision_model(o);
            if (o->oPosY == o->oHomeY) {
                o->oAction = BLUE_COIN_SWITCH_ACT_IDLE;
            }
        } return true;
    }
    return false;
}

// Destroy the bob-omb if outside of Mario's range after being thrown or attacked
static bool bhv_bobomb__destroy_if_too_far(NativeBhvFunc func) {
    if (obj_get_distance(o, gMarioObject) > 4000 && (
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

// Look directly at Bowser to make him charge instead of jumping
// Extra action for Bowser after being KOed
static bool bhv_bowser__pre_update(NativeBhvFunc func) {
    switch (o->oAction) {

        // Bowser walk
        case 14: {
            o->oBowserFlags |= 0x40000;
        } break;

        // Bowser jump
        case 13: {
            if ((o->oBowserFlags & 0x40000) && o->oSubAction == 0 && o->oTimer == 0) {
                s16 angleToBowser = obj_get_object1_angle_yaw_to_object2(mo, o);
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
            if (o->oBowserCameraState == 10) {
                if (o->oSubAction == 0) {
                    cur_obj_init_animation_with_sound(16);
                    obj_set_angle(o, 0, obj_get_object1_angle_yaw_to_object2(o, mo), 0);
                    obj_set_forward_and_y_vel(o, -40.f, 80.f);
                    o->oBowserActionTimer = 0;
                    o->oMoveFlags = 0;
                    o->oSubAction++;
                } else {
                    Vec2f prevXZ = { o->oPosX, o->oPosZ };
                    func();
                    struct Surface *floor = NULL;
                    find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
                    if (!floor || SURFACE_IS_LETHAL(floor->type)) {
                        o->oPosX = o->oGfxPos[0] = prevXZ[0];
                        o->oPosZ = o->oGfxPos[2] = prevXZ[1];
                        o->oPosY = o->oGfxPos[1] = max_f(o->oPosY, find_floor_height(prevXZ[0], o->oPosY + 100.f, prevXZ[1]));
                    }
                }
                return true;
            }
        } break;
    }
    return false;
}

// Flag the red coins star and don't respawn it if already collected
static bool bhv_red_coin_star__set_flag_and_dont_respawn(NativeBhvFunc func) {
    if (!omm_stars_is_collected(o->oBhvArgs >> 24)) {
        func();
        if (!o->activeFlags) {
            u8 starIndex = (o->oBhvArgs >> 24) & 0xFF;
            for_each_until_null(const BehaviorScript *, bhv, omm_static_array_of(const BehaviorScript *) { bhvStar, bhvStarSpawnCoordinates, NULL }) {
                for_each_object_with_behavior(star, *bhv) {
                    if (((star->oBhvArgs >> 24) & 0xFF) == starIndex) {
                        star->activeFlags |= ACTIVE_FLAG_RED_COIN_STAR;
                    }
                }
            }
        }
    } else {
        obj_mark_for_deletion(o);
    }
    return true;
}

// Don't respawn a star if already collected
static bool bhv_star__dont_respawn(NativeBhvFunc func) {
    if (omm_stars_is_collected(o->oBhvArgs >> 24)) {
        obj_mark_for_deletion(o);
        return true;
    }
    return false;
}

// Fix Bowser's electric shockwave hitbox
static bool bhv_bowser_shock_wave__hitbox_fix(NativeBhvFunc func) {
    o->oDistanceToMario = 0.f;
    func();
    if (m->invincTimer == 0 && m->action != ACT_SHOCKED && o->oBowserShockwaveScale > 0.f && o->oTimer < 70) {

        // Check vertical distance
        if ((m->pos[1] - o->oPosY) < 40.f &&
            (o->oPosY - m->pos[1]) < (mo->hitboxHeight + 40.f)) {

            // Check horizontal distance
            f32 hDistToMario = obj_get_horizontal_distance(o, mo) / o->oBowserShockwaveScale;
            if ((1.9f < hDistToMario && hDistToMario < 2.4f) ||
                (4.0f < hDistToMario && hDistToMario < 4.8f)) {
                omm_spawn_damage_mario(mo, INTERACT_SHOCK, 1);
            }
        }
    }
    return true;
}

// Fix Bowser's tail anchor position, hitbox and grab properties
static bool bhv_bowser_tail_anchor__hitbox_fix(NativeBhvFunc func) {
    if (OMM_MOVESET_ODYSSEY) {
        struct Object *bowser = o->parentObj;
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
    return false;
}

// Always spawn and make Bowser's sub tangible during Non-Stop mode
static bool bhv_bowsers_sub__non_stop_always_spawn(NativeBhvFunc func) {
    if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) {
        obj_load_collision_model(o);
        return true;
    }
    return false;
}

// Fix the checkerboard elevator group object crash
static bool bhv_checkerboard_elevator_group__crash_fix(NativeBhvFunc func) {
    o->oGraphNode = NULL;
    return false;
}

// Boos always carry blue coins, unless inside Castle grounds
static bool bhv_coin_inside_boo__always_blue(NativeBhvFunc func) {
#if OMM_GAME_IS_SM64
    if (gCurrLevelNum == LEVEL_COURT) {
        o->oGraphNode = gLoadedGraphNodes[MODEL_YELLOW_COIN];
        obj_scale(o, 1.f);
        return false;
    }
#endif
    o->oGraphNode = gLoadedGraphNodes[MODEL_BLUE_COIN];
    obj_scale(o, 0.7f);
    return false;
}

// Always spawn DDD poles during Non-Stop mode
static bool bhv_ddd_pole__non_stop_always_spawn(NativeBhvFunc func) {
    if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) {
        o->hitboxDownOffset = 100.f;
        o->oDDDPoleMaxOffset = 100.f * o->oBhvArgs2ndByte;
        return true;
    }
    return false;
}

// Fix the exclamation box hitbox
// Make koopa shells respawn
static bool bhv_exclamation_box__hitbox_fix_and_respawn_koopa_shells(NativeBhvFunc func) {
    func();
    if (o->oAction == 2) {
        o->hitboxHeight *= 2.f;
    }
    if (o->oAction == 4 && o->oBhvArgs2ndByte == 3 && !o->activeFlags) {
        o->activeFlags = ACTIVE_FLAG_ACTIVE;
        o->oAction = 5;
        cur_obj_hide();
    }
    return true;
}

// Fix the falling pillar displacement
static bool bhv_falling_pillar__move_yaw_fix(NativeBhvFunc func) {
    o->oMoveAngleYaw = o->oFaceAngleYaw;
    return false;
}

// Make flamethrowers always active
static bool bhv_flamethrower__always_active(NativeBhvFunc func) {
    o->oDistanceToMario = 0.f;
    return false;
}

// Make all goombas independent and defeat them if they reach the death barrier
static bool bhv_goomba__make_standalone_and_destroy_at_death_barrier(NativeBhvFunc func) {
    if (o->parentObj && o->parentObj != o) {
        set_object_respawn_info_bits(o->parentObj, (o->oBhvArgs2ndByte & GOOMBA_BP_TRIPLET_FLAG_MASK) >> 2);
        o->parentObj->oBhvArgs = o->parentObj->oBhvArgs | (o->oBhvArgs2ndByte & GOOMBA_BP_TRIPLET_FLAG_MASK) << 6;
        o->oBhvArgs = (o->oGoombaSize & GOOMBA_BP_SIZE_MASK) << 16;
        o->oBhvArgs2ndByte = (o->oGoombaSize & GOOMBA_BP_SIZE_MASK);
        o->parentObj = o;
        o->prevObj = NULL;
    }
    if (obj_is_on_ground(o) && SURFACE_IS_LETHAL(o->oFloor->type)) {
        obj_destroy(o);
        return true;
    }
    return false;
}

// Make the blue coins not collected respawn with the switch
// Preview them if Mario is standing on the switch
static bool bhv_hidden_blue_coin__show_and_respawn(NativeBhvFunc func) {
    o->oTransparency = 0;
    if (o->oAction == HIDDEN_BLUE_COIN_ACT_WAITING) {
        if (mo->platform == o->oHiddenBlueCoinSwitch) {
            o->oTransparency = 0x80;
            cur_obj_enable_rendering();
        } else {
            cur_obj_disable_rendering();
        }
    } else if (o->oAction == HIDDEN_BLUE_COIN_ACT_ACTIVE && o->oTimer >= 239) {
        if (o->oInteractStatus & INT_STATUS_INTERACTED) {
            spawn_object(o, MODEL_SPARKLES, bhvGoldenCoinSparkles);
            obj_mark_for_deletion(o);
            return true;
        }
        o->oAction = HIDDEN_BLUE_COIN_ACT_INACTIVE;
        cur_obj_disable_rendering();
        cur_obj_become_intangible();
        return true;
    }
    return false;
}

// Make boxes spawned by a purple switch indestructible
static bool bhv_hidden_object__indestructible(NativeBhvFunc func) {
    o->oInteractStatus &= ~INT_STATUS_WAS_ATTACKED;
    if (mo->platform == o) mo->platform = NULL;
    func();
    obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    o->oIntangibleTimer = -1;
    return true;
}

// Make secrets visible
static bool bhv_hidden_star_trigger__reveal(NativeBhvFunc func) {
    if (OMM_EXTRAS_REVEAL_SECRETS) {
        o->oGraphNode = gLoadedGraphNodes[MODEL_PURPLE_MARBLE];
        o->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
    } else {
        o->oGraphNode = NULL;
        o->oNodeFlags &= ~GRAPH_RENDER_BILLBOARD;
    }
    return false;
}

// Fix intro Lakitu's model
static bool bhv_intro_lakitu__model_fix(NativeBhvFunc func) {
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
static bool bhv_koopa_the_quick__path_failsafe(NativeBhvFunc func) {
    func();
    if (o->oBhvArgs2ndByte == KOOPA_BP_KOOPA_THE_QUICK_BOB ||
        o->oBhvArgs2ndByte == KOOPA_BP_KOOPA_THE_QUICK_THI) {
        cur_obj_path_failsafe();
    }
    return true;
}

// Force King Bob-omb to release Mario during some actions to prevent softlocks
static bool bhv_king_bobomb__release_mario_if_attacked(NativeBhvFunc func) {
    if (m->action == ACT_GRABBED && o->oAction >= 7) {
        o->oInteractStatus &= ~INT_STATUS_GRABBED_MARIO;
        o->oIntangibleTimer = -1;
        o->oChuckyaUnk88 = 0;
        omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
    }
    return false;
}

// Don't destroy the Koopa shell after leaving it, but can't interact with it for 15 frames
static bool bhv_koopa_shell__dont_destroy(NativeBhvFunc func) {
    o->oSubAction = max_s(0, o->oSubAction - 1);
    if (o->oAction == 1 && (o->oInteractStatus & INT_STATUS_STOP_RIDING)) {
        o->oPosY = m->pos[1] - 50;
        o->oVelY = 0;
        o->oAction = 0;
        o->oPrevAction = 0;
        o->oSubAction = 15;
        o->oInteractStatus = 0;
    }
    return false;
}

// Make Mad pianos always tangible
static bool bhv_mad_piano__always_tangible(NativeBhvFunc func) {
    static struct ObjectHitbox sMadPianoHitbox = { INTERACT_MR_BLIZZARD, 0, 3, 99, 0, 200, 150, 200, 150 };
    obj_set_hitbox(o, &sMadPianoHitbox);
    obj_set_home(o, o->oPosX, o->oPosY, o->oPosZ);
    return false;
}

// Fix the water ring hitbox
static bool bhv_manta_ray_water_ring__hitbox_fix(NativeBhvFunc func) {
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

// Fix Monty moles interaction status to prevent a crash
static bool bhv_monty_mole__crash_fix(NativeBhvFunc func) {
    if (o->oAction == MONTY_MOLE_ACT_SELECT_HOLE) {
        o->oInteractStatus = 0;
    }
    return false;
}

// Remove the sound button on file select screen
static bool bhv_menu_button__replace_sound_button_by_character_select_button(NativeBhvFunc func) {
    if (obj_check_model(o, MODEL_MAIN_MENU_PURPLE_SOUND_BUTTON)) {
        struct Object *characterSelectButton = obj_get_first_with_behavior(bhvOmmMenuCharacterSelectButton);
        if (characterSelectButton) {
            characterSelectButton->oPosX = o->oParentRelativePosX;
            characterSelectButton->oPosY = o->oParentRelativePosY;
            o->oPosX = 30000.f;
            o->oPosY = 30000.f;
            o->oParentRelativePosX = 30000.f;
            o->oParentRelativePosY = 30000.f;
            o->oMenuButtonOrigPosX = 30000.f;
            o->oMenuButtonOrigPosY = 30000.f;
            o->oMenuButtonScale = 0.f;
            obj_set_dormant(o, true);
            return true;
        }
    }
    return false;
}

// Spawn the character select button on the file select screen
static bool bhv_menu_button_manager__spawn_character_select_button(NativeBhvFunc func) {
    struct Object *charSelButton = spawn_object_rel_with_rot(o, MODEL_NONE, bhvOmmMenuCharacterSelectButton, 6400.f, -3500.f, 0, 0, 0, 0);
    charSelButton->oMenuButtonScale = 1.f;
    charSelButton->oAnimState = 0;
    switch (omm_player_get_selected_index()) {
        case OMM_PLAYER_MARIO: charSelButton->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_menu_button_mario); break;
        case OMM_PLAYER_PEACH: charSelButton->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_menu_button_peach); break;
        case OMM_PLAYER_LUIGI: charSelButton->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_menu_button_luigi); break;
        case OMM_PLAYER_WARIO: charSelButton->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_menu_button_wario); break;
        default:               charSelButton->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_menu_button_mario); break;
    }
    return false;
}

// Always spawn Mips past 120 stars
static bool bhv_mips__spawn_at_120_stars(NativeBhvFunc func) {
    if (m->numStars >= 120) {
        o->oBhvArgs2ndByte = (OMM_GAME_IS_SMSR ? 0 : 1);
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

// Extra action for Motos when being attacked
static bool bhv_motos__attacked(NativeBhvFunc func) {
    s32 attackFlags = (o->oInteractStatus >> 24) & (OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_WEAK);
    if (attackFlags) {
        if (attackFlags & OBJ_INT_ATTACK_STRONG) {
            o->oHeldState = HELD_THROWN;
        } else {
            o->oMoveAngleYaw += 0x8000;
            o->oAction = 1;
        }
        if (m->action == ACT_GRABBED && m->usedObj == o) {
            o->oChuckyaUnk88 = 2;
        }
        o->oInteractStatus = 0;
        play_sound(SOUND_OBJ2_LARGE_BULLY_ATTACKED, o->oCameraToObject);
    }
    return false;
}

// Reduce moving yellow coin RNG
static bool bhv_moving_yellow_coin__less_rng(NativeBhvFunc func) {
    o->oForwardVel /= 2.f;
    return false;
}

// Make Mr. I instantly target Mario if he's close enough
static bool bhv_mr_i__target_mario_if_near(NativeBhvFunc func) {
    if (o->oAction <= 1 && obj_get_distance(o, mo) < 300.f) {
        s16 angleToMario = obj_get_object1_angle_yaw_to_object2(o, mo);
        o->oFaceAngleYaw = angleToMario;
        o->oMoveAngleYaw = angleToMario;
        o->oAction = 2;
        o->oMrIUnkFC = 0;
        o->oMrIUnk100 = 0;
        o->oMrIUnk104 = 0;
        o->oMrIUnk108 = (s32) (random_float() * 50.f + 50.f);
    }
    return false;
}

// Fix the glitchy camera when Mario is holding a pole due to it being slightly above the ground
static bool bhv_pole_base__camera_fix(NativeBhvFunc func) {
    f32 fy = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
    if (fy < o->oPosY && o->oPosY < fy + 5) {
        o->oPosY = fy;
    }
    return false;
}

// Teleports the racing penguin back to his path if he falls off
static bool bhv_racing_penguin__path_failsafe(NativeBhvFunc func) {
    func();
    if (o->oAction == RACING_PENGUIN_ACT_RACE) {
        cur_obj_path_failsafe();
    }
    return true;
}

// Allow any type of ground pound to trigger purple switches
static bool bhv_purple_switch__any_ground_pound_trigger(NativeBhvFunc func) {
    if (o->oAction == PURPLE_SWITCH_IDLE && omm_mario_is_ground_pound_landing(m) &&
        o == mo->platform && obj_get_horizontal_distance(o, mo) < 85.f * o->oScaleX) {
        o->oAction = PURPLE_SWITCH_PRESSED;
        return true;
    }
    return false;
}

// Make recovery hearts slowly heal Mario when spinning
static bool bhv_recovery_heart__heal_mario_for_1_segment(NativeBhvFunc func) {
    if (o->oSpinningHeartTotalSpin + o->oAngleVelYaw >= 0x10000) {
        omm_health_heal_mario(m);
    }
    return false;
}

// Fix red coin count, display and sound effect
static bool bhv_red_coin__count_display_sound_fix(NativeBhvFunc func) {
    if (!o->parentObj ||
        !o->parentObj->activeFlags || (
        o->parentObj->behavior != bhvHiddenRedCoinStar &&
        o->parentObj->behavior != bhvBowserCourseRedCoinStar)) {
        o->parentObj = NULL;
    }
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (o->parentObj) {
            o->parentObj->oHiddenStarTriggerCounter++;
            omm_spawn_orange_number(o, 1 + omm_level_get_num_red_coins(gCurrLevelNum, gCurrAreaIndex) - obj_get_count_with_behavior(bhvRedCoin));
            play_sound(SOUND_MENU_COLLECT_RED_COIN + (max_s(0, o->parentObj->oHiddenStarTriggerCounter - 1) << 16), gGlobalSoundArgs);
        }
        coin_collected();
        o->oInteractStatus = 0;
        return true;
    }
    return false;
}

// Disable respawner during "defeat x enemies" challenges
static bool bhv_respawner__sparkly_stars_disable(NativeBhvFunc func) {
    if (omm_sparkly_context_get_data(OMM_SPARKLY_DATA_ENEMIES)) {
        obj_mark_for_deletion(o);
        return true;
    }
    return false;
}

// Fix home position
static bool bhv_skeeter__home_fix(NativeBhvFunc func) {
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

// Fix a softlock that can happen if Mario is not in the 400 units radius after starting the dialog
// Fix snowman's body yaw after reaching the head
static bool bhv_snowmans_bottom__pos_and_yaw_fix(NativeBhvFunc func) {
    switch (o->oAction) {
        case 0: {
            if (m->action == ACT_READING_NPC_DIALOG &&
                m->usedObj == o && vec3f_dist(m->pos, &o->oPosX) > 400.f) {
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

// Make sure spawned stars don't go inside ceilings or below floors
static bool bhv_spawned_star__above_floors_and_below_ceilings(NativeBhvFunc func) {
    if (!bhv_star__dont_respawn(func)) {
        f32 floorHeight = find_floor_height(o->oPosX, o->oPosY - 100, o->oPosZ);
        f32 ceilHeight = find_ceil(o->oPosX, o->oPosY - 100, o->oPosZ, NULL);
        if (ceilHeight > floorHeight) {
            o->oPosY = min_s(o->oPosY, ceilHeight - 60);
            o->oPosY = max_s(o->oPosY, floorHeight + 60);
        }
    }
    return false;
}

// Allow any type of ground pound to drain the water on top of Tiny THI
static bool bhv_thi_tiny_island_top__any_ground_pound_trigger(NativeBhvFunc func) {
    if (o->oAction == 0 && !gTHIWaterDrained && omm_mario_is_ground_pound_landing(m) && obj_get_distance(o, mo) < 500.f) {
        obj_spawn_particles(o, 30, MODEL_WHITE_PARTICLE_SMALL, 0, 40, 0, 20, 40, -4, 2, 0);
        obj_spawn_triangle_break_particles(o, OBJ_SPAWN_TRI_BREAK_PRESET_DIRT_SMALL);
        play_sound(SOUND_GENERAL_ACTIVATE_CAP_SWITCH, o->oCameraToObject);
        o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        o->oAction++;
        return true;
    }
    return false;
}

// Make Tuxie's mother endlessly chase Mario if he's holding or capturing Tuxie
static bool bhv_tuxies_mother__chase_mario(NativeBhvFunc func) {
    if (o->oAction == 2 && m->action == ACT_OMM_POSSESSION && gOmmCapture->behavior == bhvUnused20E0) {
        gOmmCapture->oHeldState = HELD_HELD;
    }
    return false;
}

// Fix Cage Ukiki's path
static bool bhv_ukiki__always_select_nearest_waypoint(NativeBhvFunc func) {
    if (o->oBhvArgs2ndByte == UKIKI_CAGE) {
        obj_set_home(o, o->oPosX, o->oPosY, o->oPosZ);
        if (o->oAction == UKIKI_ACT_GO_TO_CAGE && o->oSubAction == UKIKI_SUB_ACT_CAGE_RUN_TO_CAGE && o->oPathedStartWaypoint && o->oTimer == 1) {
            struct Waypoint *nearest = obj_path_get_nearest_waypoint(o, o->oPathedStartWaypoint, 200.f);
            if (nearest) {
                o->oPathedPrevWaypoint = (struct Waypoint *) max((uintptr_t) o->oPathedStartWaypoint, (uintptr_t) (nearest - 1));
                o->oPathedPrevWaypointFlags = WAYPOINT_FLAGS_INITIALIZED;
            }
        }
    }
    return false;
}

// Make wooden posts spawn coins if fully pounded
static bool bhv_wooden_post__spawn_coins_if_pounded(NativeBhvFunc func) {
    if (!(o->oBhvArgs & WOODEN_POST_BP_NO_COINS_MASK) && o->oWoodenPostOffsetY <= -190.f) {
        o->oPosY = o->oHomeY;
        obj_spawn_loot_yellow_coins(o, 5, 20.f);
        set_object_respawn_info_bits(o, 1);
        o->oBhvArgs |= WOODEN_POST_BP_NO_COINS_MASK;
        o->oPosY = o->oHomeY + o->oWoodenPostOffsetY;
    }
    return false;
}

#if OMM_GAME_IS_SM64
// Make Yoshi give something more interesting than 100 useless 1-ups
static bool bhv_yoshi__big_surprise(NativeBhvFunc func) {
    if (o->oAction == YOSHI_ACT_GIVE_PRESENT) {
        o->oAction = YOSHI_ACT_IDLE;
        omm_spawn_problem(mo);
        omm_mario_set_action(m, (random_u16() & 1) ? ACT_HARD_FORWARD_GROUND_KB : ACT_HARD_BACKWARD_GROUND_KB, 1, 0);
        if (OMM_MOVESET_ODYSSEY) {
            m->health = OMM_HEALTH_DEAD;
        } else {
            m->healCounter = 0;
            m->hurtCounter = 64;
        }
    }
    return false;
}
#endif

#undef m
#undef mo
#pragma GCC pop_options

OMM_AT_STARTUP static void omm_setup_behavior_update_functions_map() {
    static const void *sBhvUpdateTable[][2] = {
        { bhv_1up_init,                             bhv_1up__always_spawn },
        { bhv_1up_loop,                             bhv_1up__life_up },
        { bhv_1up_hidden_loop,                      bhv_1up__life_up },
        { bhv_1up_hidden_in_pole_loop,              bhv_1up__life_up },
        { bhv_1up_jump_on_approach_loop,            bhv_1up__life_up },
        { bhv_1up_running_away_loop,                bhv_1up__life_up },
        { bhv_1up_sliding_loop,                     bhv_1up__life_up },
        { bhv_1up_walking_loop,                     bhv_1up__life_up },
        { bhv_blue_coin_switch_loop,                bhv_blue_coin_switch__show_coins_and_respawn },
        { bhv_bobomb_loop,                          bhv_bobomb__destroy_if_too_far },
        { bhv_boo_loop,                             bhv_boo__act_uncaptured },
        { bhv_bowser_loop,                          bhv_bowser__pre_update },
        { bhv_bowser_course_red_coin_star_loop,     bhv_red_coin_star__set_flag_and_dont_respawn },
        { bhv_bowser_shock_wave_loop,               bhv_bowser_shock_wave__hitbox_fix },
        { bhv_bowser_tail_anchor_loop,              bhv_bowser_tail_anchor__hitbox_fix},
        { bhv_bowsers_sub_loop,                     bhv_bowsers_sub__non_stop_always_spawn },
        { bhv_ccm_touched_star_spawn_loop,          bhv_star__dont_respawn },
        { bhv_checkerboard_elevator_group_init,     bhv_checkerboard_elevator_group__crash_fix },
        { bhv_coin_inside_boo_loop,                 bhv_coin_inside_boo__always_blue },
        { bhv_collect_star_init,                    bhv_star__dont_respawn },
        { bhv_collect_star_loop,                    bhv_star__dont_respawn },
        { bhv_ddd_pole_init,                        bhv_ddd_pole__non_stop_always_spawn },
        { bhv_exclamation_box_loop,                 bhv_exclamation_box__hitbox_fix_and_respawn_koopa_shells },
        { bhv_falling_pillar_loop,                  bhv_falling_pillar__move_yaw_fix },
        { bhv_flamethrower_loop,                    bhv_flamethrower__always_active },
        { bhv_goomba_update,                        bhv_goomba__make_standalone_and_destroy_at_death_barrier },
        { bhv_hidden_blue_coin_loop,                bhv_hidden_blue_coin__show_and_respawn },
        { bhv_hidden_object_loop,                   bhv_hidden_object__indestructible },
        { bhv_hidden_red_coin_star_init,            bhv_red_coin_star__set_flag_and_dont_respawn },
        { bhv_hidden_red_coin_star_loop,            bhv_red_coin_star__set_flag_and_dont_respawn },
        { bhv_hidden_star_init,                     bhv_star__dont_respawn },
        { bhv_hidden_star_loop,                     bhv_star__dont_respawn },
        { bhv_hidden_star_trigger_loop,             bhv_hidden_star_trigger__reveal },
        { bhv_intro_lakitu_loop,                    bhv_intro_lakitu__model_fix },
        { bhv_jumping_box_loop,                     bhv_jumping_box__always_spawn_coins },
        { bhv_koopa_update,                         bhv_koopa_the_quick__path_failsafe },
        { bhv_king_bobomb_loop,                     bhv_king_bobomb__release_mario_if_attacked },
        { bhv_koopa_shell_loop,                     bhv_koopa_shell__dont_destroy },
        { bhv_mad_piano_update,                     bhv_mad_piano__always_tangible },
        { bhv_manta_ray_water_ring_init,            bhv_manta_ray_water_ring__hitbox_fix },
        { bhv_monty_mole_update,                    bhv_monty_mole__crash_fix },
        { bhv_menu_button_loop,                     bhv_menu_button__replace_sound_button_by_character_select_button },
        { bhv_menu_button_manager_init,             bhv_menu_button_manager__spawn_character_select_button },
        { bhv_mips_init,                            bhv_mips__spawn_at_120_stars },
#if OMM_GAME_IS_R96X
        { bhv_motos_loop,                           bhv_motos__attacked },
#endif
        { bhv_moving_yellow_coin_init,              bhv_moving_yellow_coin__less_rng },
        { bhv_mr_i_loop,                            bhv_mr_i__target_mario_if_near },
        { bhv_pole_base_loop,                       bhv_pole_base__camera_fix },
        { bhv_racing_penguin_update,                bhv_racing_penguin__path_failsafe },
        { bhv_purple_switch_loop,                   bhv_purple_switch__any_ground_pound_trigger },
        { bhv_recovery_heart_loop,                  bhv_recovery_heart__heal_mario_for_1_segment },
        { bhv_red_coin_loop,                        bhv_red_coin__count_display_sound_fix },
        { bhv_respawner_loop,                       bhv_respawner__sparkly_stars_disable },
        { bhv_skeeter_update,                       bhv_skeeter__home_fix },
        { bhv_sl_snowman_wind_loop,                 bhv_sl_snowman_wind__sparkly_stars_always_blow },
        { bhv_snowmans_bottom_loop,                 bhv_snowmans_bottom__pos_and_yaw_fix },
        { bhv_snowmans_head_init,                   bhv_snowmans_head__non_stop_dont_spawn_whole },
        { bhv_spawned_star_loop,                    bhv_spawned_star__above_floors_and_below_ceilings },
        { bhv_star_spawn_init,                      bhv_star__dont_respawn },
        { bhv_star_spawn_loop,                      bhv_star__dont_respawn },
        { bhv_thi_tiny_island_top_loop,             bhv_thi_tiny_island_top__any_ground_pound_trigger },
        { bhv_tuxies_mother_loop,                   bhv_tuxies_mother__chase_mario },
        { bhv_ukiki_loop,                           bhv_ukiki__always_select_nearest_waypoint },
        { bhv_wooden_post_update,                   bhv_wooden_post__spawn_coins_if_pounded },
#if OMM_GAME_IS_SM64
        { bhv_yoshi_loop,                           bhv_yoshi__big_surprise },
#endif
    };
    for (s32 i = 0; i != omm_static_array_length(sBhvUpdateTable); ++i) {
        omm_hmap_insert(sBhvUpdateMap, (uintptr_t) sBhvUpdateTable[i][0], sBhvUpdateTable[i][1]);
    }
}

static bool should_do_safe_step() {
    return
        omm_obj_is_goomba(o) ||
        omm_obj_is_coin(o) || (
        o->behavior == bhvBobomb && (
        o->oAction == BOBOMB_ACT_PATROL ||
        o->oAction == BOBOMB_ACT_CHASE_MARIO
    ));
}

static void cur_obj_update_behavior_func(void (*func)(void)) {
    if (!cur_obj_update_knockback()) {
        bool doSafeStep = should_do_safe_step();
        if (doSafeStep) obj_safe_step(o, false);
        cur_obj_fix_hitbox();
        s32 i = omm_hmap_find(sBhvUpdateMap, (uintptr_t) func);
        if (i == -1 || !omm_hmap_get(sBhvUpdateMap, UpdateFunc, i)(func)) {
            if (func) func();
            cur_obj_fix_hitbox();
            if (doSafeStep) obj_safe_step(o, true);
        }
    }
}

//
// Commands
//

static bool bhv_cmd_begin() {
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_delay() {
    if (o->bhvDelayTimer < bhv_cmd_get(s16, 0, 1) - 1) {
        o->bhvDelayTimer++;
    } else {
        o->bhvDelayTimer = 0;
        gCurBhvCommand++;
    }
    return false;
}

static bool bhv_cmd_call() {
    stack_push(bhv_cmd_get_addr(2));
    gCurBhvCommand = bhv_cmd_get_ptr(const BehaviorScript *, 1);
    return true;
}

static bool bhv_cmd_return() {
    gCurBhvCommand = (const BehaviorScript *) stack_pop();
    return true;
}

static bool bhv_cmd_goto() {
    gCurBhvCommand = bhv_cmd_get_ptr(const BehaviorScript *, 1);
    return true;
}

static bool bhv_cmd_begin_repeat() {
    stack_push(bhv_cmd_get_addr(1));
    stack_push(bhv_cmd_get(s16, 0, 1));
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_end_repeat() {
    s32 count = stack_pop() - 1;
    if (count > 0) {
        gCurBhvCommand = (const BehaviorScript *) stack_pop();
        stack_push(bhv_cmd_get_addr(0));
        stack_push(count);
    } else {
        stack_pop();
        gCurBhvCommand++;
    }
    return false;
}

static bool bhv_cmd_end_repeat_continue() {
    bhv_cmd_end_repeat();
    return true;
}

static bool bhv_cmd_begin_loop() {
    stack_push(bhv_cmd_get_addr(1));
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_end_loop() {
    gCurBhvCommand = (const BehaviorScript *) stack_pop();
    stack_push(bhv_cmd_get_addr(0));
    return false;
}

static bool bhv_cmd_break() {
    return false;
}

static bool bhv_cmd_break_unused() {
    return false;
}

static bool bhv_cmd_call_native() {
    NativeBhvFunc behaviorFunc = bhv_cmd_get_ptr(NativeBhvFunc, 1);
    cur_obj_update_behavior_func(behaviorFunc);
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_add_float() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    f32 value = bhv_cmd_get(s16, 0, 1);
    cur_obj_add_float(field, value);
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_set_float() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    f32 value = bhv_cmd_get(s16, 0, 1);
    cur_obj_set_float(field, value);
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_add_int() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    s32 value = bhv_cmd_get(s16, 0, 1);
    cur_obj_add_int(field, value);
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_set_int() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    s32 value = bhv_cmd_get(s16, 0, 1);
    cur_obj_set_int(field, value);
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_or_int() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    u32 value = bhv_cmd_get(u16, 0, 1);
    cur_obj_or_int(field, value);
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_bit_clear() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    u32 value = bhv_cmd_get(u16, 0, 1);
    cur_obj_and_int(field, ~value);
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_set_int_rand_rshift() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    s32 value = bhv_cmd_get(s16, 0, 1);
    s32 shift = bhv_cmd_get(s16, 1, 0);
    cur_obj_set_int(field, value + (random_u16() >> shift));
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_random_float() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    f32 value = bhv_cmd_get(s16, 0, 1);
    f32 range = bhv_cmd_get(s16, 1, 0);
    cur_obj_set_float(field, value + (range * random_float()));
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_random_int() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    s32 value = bhv_cmd_get(s16, 0, 1);
    s32 range = bhv_cmd_get(s16, 1, 0);
    cur_obj_set_int(field, value + (s32) (range * random_float()));
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_add_random_float() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    f32 value = bhv_cmd_get(s16, 0, 1);
    f32 range = bhv_cmd_get(s16, 1, 0);
    cur_obj_set_float(field, cur_obj_get_float(field) + value + (range * random_float()));
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_add_int_rand_rshift() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    s32 value = bhv_cmd_get(s16, 0, 1);
    s32 shift = bhv_cmd_get(s16, 1, 0);
    cur_obj_set_int(field, cur_obj_get_int(field) + value + (random_u16() >> shift));
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_nop_1() {
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_nop_2() {
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_nop_3() {
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_set_model() {
    o->oGraphNode = gLoadedGraphNodes[bhv_cmd_get(s16, 0, 1)];
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_spawn_child() {
    struct Object *child = spawn_object_at_origin(o, 0, bhv_cmd_get(u32, 1, 0), bhv_cmd_get_ptr(const BehaviorScript *, 2));
    obj_copy_pos_and_angle(child, o);
    gCurBhvCommand += 3;
    return true;
}

static bool bhv_cmd_deactivate() {
    o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    return false;
}

static bool bhv_cmd_drop_to_floor() {
    if (omm_obj_is_goomba(o) && o->oGoombaStackParent) {
        o->oGoombaStackParent = NULL;
    } else {
        o->oPosY = find_floor_height(o->oPosX, o->oPosY + 200.f, o->oPosZ);
        o->oMoveFlags |= OBJ_MOVE_ON_GROUND;
    }
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_sum_float() {
    s32 fieldDest = bhv_cmd_get(u8, 0, 1);
    s32 fieldSrc1 = bhv_cmd_get(u8, 0, 2);
    s32 fieldSrc2 = bhv_cmd_get(u8, 0, 3);
    cur_obj_set_float(fieldDest, cur_obj_get_float(fieldSrc1) + cur_obj_get_float(fieldSrc2));
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_sum_int() {
    s32 fieldDest = bhv_cmd_get(u8, 0, 1);
    s32 fieldSrc1 = bhv_cmd_get(u8, 0, 2);
    s32 fieldSrc2 = bhv_cmd_get(u8, 0, 3);
    cur_obj_set_int(fieldDest, cur_obj_get_int(fieldSrc1) + cur_obj_get_int(fieldSrc2));
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_billboard() {
    o->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_hide() {
    cur_obj_hide();
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_set_hitbox() {
    o->hitboxRadius = bhv_cmd_get(s16, 1, 0);
    o->hitboxHeight = bhv_cmd_get(s16, 1, 1);
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_nop_4() {
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_delay_var() {
    s32 num = cur_obj_get_int(bhv_cmd_get(u8, 0, 1));
    if (o->bhvDelayTimer < num - 1) {
        o->bhvDelayTimer++;
    } else {
        o->bhvDelayTimer = 0;
        gCurBhvCommand++;
    }
    return false;
}

static bool bhv_cmd_begin_repeat_unused() {
    stack_push(bhv_cmd_get_addr(1));
    stack_push(bhv_cmd_get(u8, 0, 1));
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_load_animations() {
    o->oAnimations = bhv_cmd_get_ptr(struct Animation **, 1);
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_animate() {
    geo_obj_init_animation(&o->header.gfx, &(o->oAnimations[bhv_cmd_get(u8, 0, 1)]));
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_spawn_child_with_param() {
    struct Object *child = spawn_object_at_origin(o, 0, bhv_cmd_get(u32, 1, 0), bhv_cmd_get_ptr(const BehaviorScript *, 2));
    child->oBhvArgs2ndByte = bhv_cmd_get(s16, 0, 1);
    obj_copy_pos_and_angle(child, o);
    gCurBhvCommand += 3;
    return true;
}

static bool bhv_cmd_load_collision_data() {
    o->collisionData = bhv_cmd_get_ptr(void *, 1);
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_hitbox_with_offset() {
    o->hitboxRadius = bhv_cmd_get(s16, 1, 0);
    o->hitboxHeight = bhv_cmd_get(s16, 1, 1);
    o->hitboxDownOffset = bhv_cmd_get(s16, 2, 0);
    gCurBhvCommand += 3;
    return true;
}

static bool bhv_cmd_spawn_obj() {
    struct Object *object = spawn_object_at_origin(o, 0, bhv_cmd_get(u32, 1, 0), bhv_cmd_get_ptr(const BehaviorScript *, 2));
    obj_copy_pos_and_angle(object, o);
    o->prevObj = object;
    gCurBhvCommand += 3;
    return true;
}

static bool bhv_cmd_set_home() {
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_set_hurtbox() {
    o->hurtboxRadius = bhv_cmd_get(s16, 1, 0);
    o->hurtboxHeight = bhv_cmd_get(s16, 1, 1);
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_interact_type() {
    o->oInteractType = bhv_cmd_get(u32, 1, 0);
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_set_obj_physics() {
    o->oWallHitboxRadius = bhv_cmd_get(s16, 1, 0);
    o->oGravity          = bhv_cmd_get(s16, 1, 1) / 100.f;
    o->oBounciness       = bhv_cmd_get(s16, 2, 0) / 100.f;
    o->oDragStrength     = bhv_cmd_get(s16, 2, 1) / 100.f;
    o->oFriction         = bhv_cmd_get(s16, 3, 0) / 100.f;
    o->oBuoyancy         = bhv_cmd_get(s16, 3, 1) / 100.f;
    gCurBhvCommand += 5;
    return true;
}

static bool bhv_cmd_set_interact_subtype() {
    o->oInteractionSubtype = bhv_cmd_get(u32, 1, 0);
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_scale() {
    cur_obj_scale(bhv_cmd_get(s16, 0, 1) / 100.f);
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_parent_bit_clear() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    u32 value = bhv_cmd_get(u32, 1, 0);
    o->parentObj->OBJECT_FIELD_S32(field) &= ~value;
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_animate_texture() {
    if ((gGlobalTimer % bhv_cmd_get(s16, 0, 1)) == 0) {
        o->oAnimState++;
    }
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_disable_rendering() {
    o->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_set_int_unused() {
    s32 field = bhv_cmd_get(u8,  0, 1);
    s32 value = bhv_cmd_get(s16, 1, 1);
    cur_obj_set_int(field, value);
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_spawn_water_droplet() {
    spawn_water_droplet(o, bhv_cmd_get_ptr(void *, 1));
    gCurBhvCommand += 2;
    return true;
}

static bool bhv_cmd_cylboard() {
    o->oNodeFlags |= GRAPH_RENDER_CYLBOARD;
    gCurBhvCommand++;
    return true;
}

#if OMM_GAME_IS_R96X
static bool bhv_cmd_modelpack_no_billboard() {
    if (configBillboard) {
        o->oNodeFlags &= ~GRAPH_RENDER_BILLBOARD;
    } else {
        o->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
    }
    gCurBhvCommand++;
    return true;
}

static bool bhv_cmd_mr_i_fix() {
    if (configBillboard) {
        o->prevObj = spawn_object(o, MODEL_MR_I, bhvMrIBody);
    } else {
        o->oGraphNode = gLoadedGraphNodes[MODEL_MR_I];
        spawn_object(o, MODEL_MR_I_IRIS, bhvMrIBody);
    }
    gCurBhvCommand++;
    return true;
}
#endif

//
// Behavior update
//

typedef bool (*BhvCommandProc)(void);
static BhvCommandProc sBehaviorCmdTable[] = {
    bhv_cmd_begin,
    bhv_cmd_delay,
    bhv_cmd_call,
    bhv_cmd_return,
    bhv_cmd_goto,
    bhv_cmd_begin_repeat,
    bhv_cmd_end_repeat,
    bhv_cmd_end_repeat_continue,
    bhv_cmd_begin_loop,
    bhv_cmd_end_loop,
    bhv_cmd_break,
    bhv_cmd_break_unused,
    bhv_cmd_call_native,
    bhv_cmd_add_float,
    bhv_cmd_set_float,
    bhv_cmd_add_int,
    bhv_cmd_set_int,
    bhv_cmd_or_int,
    bhv_cmd_bit_clear,
    bhv_cmd_set_int_rand_rshift,
    bhv_cmd_set_random_float,
    bhv_cmd_set_random_int,
    bhv_cmd_add_random_float,
    bhv_cmd_add_int_rand_rshift,
    bhv_cmd_nop_1,
    bhv_cmd_nop_2,
    bhv_cmd_nop_3,
    bhv_cmd_set_model,
    bhv_cmd_spawn_child,
    bhv_cmd_deactivate,
    bhv_cmd_drop_to_floor,
    bhv_cmd_sum_float,
    bhv_cmd_sum_int,
    bhv_cmd_billboard,
    bhv_cmd_hide,
    bhv_cmd_set_hitbox,
    bhv_cmd_nop_4,
    bhv_cmd_delay_var,
    bhv_cmd_begin_repeat_unused,
    bhv_cmd_load_animations,
    bhv_cmd_animate,
    bhv_cmd_spawn_child_with_param,
    bhv_cmd_load_collision_data,
    bhv_cmd_set_hitbox_with_offset,
    bhv_cmd_spawn_obj,
    bhv_cmd_set_home,
    bhv_cmd_set_hurtbox,
    bhv_cmd_set_interact_type,
    bhv_cmd_set_obj_physics,
    bhv_cmd_set_interact_subtype,
    bhv_cmd_scale,
    bhv_cmd_parent_bit_clear,
    bhv_cmd_animate_texture,
    bhv_cmd_disable_rendering,
    bhv_cmd_set_int_unused,
    bhv_cmd_spawn_water_droplet,
    bhv_cmd_cylboard,
#if OMM_GAME_IS_R96X
    bhv_cmd_modelpack_no_billboard,
    bhv_cmd_mr_i_fix,
#else
    bhv_cmd_nop_1,
    bhv_cmd_nop_1,
#endif
    bhv_cmd_nop_1,
    bhv_cmd_nop_1,
    bhv_cmd_nop_1,
    bhv_cmd_nop_1,
    bhv_cmd_nop_1,
};

void cur_obj_update(void) {
    u32 objFlags = o->oFlags;

    // Don't update if dormant
    if (obj_is_dormant(o)) {
        return;
    }
    
    // Distance from the object to Mario
    if (objFlags & OBJ_FLAG_COMPUTE_DIST_TO_MARIO) {
        o->oDistanceToMario = dist_between_objects(o, gMarioObject);
    }

    // Angle from the object to Mario
    if (objFlags & OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO) {
        o->oAngleToMario = obj_angle_to_object(o, gMarioObject);
    }

    // Reset timer on action change
    if (o->oPrevAction != o->oAction) {
        o->oPrevAction = o->oAction;
        o->oSubAction = 0;
        o->oTimer = 0;
    }

    // Execute the behavior script
    gCurBhvCommand = o->curBhvCommand;
    while (sBehaviorCmdTable[(*gCurBhvCommand >> 24) & 0x3F]());
    o->curBhvCommand = gCurBhvCommand;

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

#if OMM_GAME_IS_XALO
    // Make trees more or less transparent depending on their distance to the camera
    if (o->behavior == bhvTree) {
        s32 opacityDist = ((s32) (-o->oCameraToObject[2] - 128)) >> 2;
        if (opacityDist < 0x00) opacityDist = 0xFF;
        o->oOpacity = clamp_s(opacityDist, 0x00, 0xFF);
    }
#endif

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
}

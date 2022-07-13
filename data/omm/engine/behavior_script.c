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

static bool bhv_1up_always_spawn(NativeBhvFunc func) {
    bhv_1up_common_init();
    return true;
}

static bool bhv_1up_life_up(NativeBhvFunc func) {
    if ((o->oNodeFlags & GRAPH_RENDER_ACTIVE) && !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE) && (o->oIntangibleTimer == 0)) {
        if (obj_check_if_collided_with_object(o, mo)) {
#if OMM_GAME_IS_R96A
            // Green Demon effect
            if (Cheats.EnableCheats && Cheats.ChaosGreenDemon) {
                play_sound(SOUND_MENU_CAMERA_BUZZ, gDefaultSoundArgs);
                m->health = 0xFF;
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

static bool bhv_blue_coin_switch_respawn(NativeBhvFunc func) {
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

static bool bhv_boo_uncaptured(NativeBhvFunc func) {
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

static bool bhv_bowser_ohkoed(NativeBhvFunc func) {
    if (o->oBowserCameraState == 10 && o->oAction == 4) {
        if (o->oSubAction == 0) {
            cur_obj_init_animation_with_sound(16);
            obj_set_angle(o, 0, obj_get_object1_angle_yaw_to_object2(o, m->marioObj), 0);
            obj_set_forward_and_y_vel(o, -40.f, 80.f);
            o->oBowserActionTimer = 0;
            o->oMoveFlags = 0;
            o->oSubAction++;
        } else {
            Vec2f prevXZ = { o->oPosX, o->oPosZ };
            func();
            struct Surface *floor = NULL;
            find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
            if (!floor || floor->type == SURFACE_BURNING || floor->type == SURFACE_DEATH_PLANE) {
                o->oPosX = o->oGfxPos[0] = prevXZ[0];
                o->oPosZ = o->oGfxPos[2] = prevXZ[1];
                o->oPosY = o->oGfxPos[1] = max_f(o->oPosY, find_floor_height(prevXZ[0], o->oPosY + 100.f, prevXZ[1]));
            }
        }
        return true;
    }
    return false;
}

static bool bhv_red_coin_star_flag_and_dont_respawn(NativeBhvFunc func) {
    if (!omm_stars_is_collected(o->oBehParams >> 24)) {
        func();
        if (o->activeFlags == ACTIVE_FLAG_DEACTIVATED) {
            u8 starIndex = (o->oBehParams >> 24) & 0xFF;
            for_each_until_null(const BehaviorScript *, bhv, OMM_ARRAY_OF(const BehaviorScript *) { bhvStar, bhvStarSpawnCoordinates, NULL }) {
                for_each_object_with_behavior(star, *bhv) {
                    if (((star->oBehParams >> 24) & 0xFF) == starIndex) {
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

static bool bhv_star_dont_respawn(NativeBhvFunc func) {
    if (omm_stars_is_collected(o->oBehParams >> 24)) {
        obj_mark_for_deletion(o);
        return true;
    }
    return false;
}

static bool bhv_bowser_shock_wave_fix(NativeBhvFunc func) {
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
                omm_spawn_damage_mario(m->marioObj, INTERACT_SHOCK, 1);
            }
        }
    }
    return true;
}

static bool bhv_bowser_tail_anchor_fix(NativeBhvFunc func) {
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

static bool bhv_bowsers_sub_non_stop_spawn(NativeBhvFunc func) {
    if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) {
        obj_load_collision_model(o);
        return true;
    }
    return false;
}

static bool bhv_checkerboard_elevator_group_fix(NativeBhvFunc func) {
    o->oGraphNode = NULL;
    return false;
}

static bool bhv_coin_inside_boo_always_blue(NativeBhvFunc func) {
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

static bool bhv_ddd_pole_non_stop_spawn(NativeBhvFunc func) {
    if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) {
        o->hitboxDownOffset = 100.f;
        o->oDDDPoleMaxOffset = 100.f * o->oBehParams2ndByte;
        return true;
    }
    return false;
}

static bool bhv_exclamation_box_fix(NativeBhvFunc func) {
    func();
    if (o->oAction == 2) {
        o->hitboxHeight *= 2.f;
    }
    return true;
}

static bool bhv_explosion_fix(NativeBhvFunc func) {
    o->hitboxRadius = 150.f;
    o->hitboxHeight = 200.f;
    o->hitboxDownOffset = 50.f;
    return false;
}

static bool bhv_flamethrower_always_active(NativeBhvFunc func) {
    o->oDistanceToMario = 0.f;
    return false;
}

static bool bhv_flamethrower_flame_fix(NativeBhvFunc func) {
    o->hitboxRadius = 60.f;
    o->hitboxHeight = 120.f;
    o->hitboxDownOffset = 60.f;
    return false;
}

static bool bhv_hidden_blue_coin_respawn(NativeBhvFunc func) {
    if (o->oAction == HIDDEN_BLUE_COIN_ACT_ACTIVE && o->oTimer >= 239) {
        o->oAction = HIDDEN_BLUE_COIN_ACT_INACTIVE;
        o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
        return true;
    }
    return false;
}

static bool bhv_hidden_star_trigger_reveal(NativeBhvFunc func) {
    if (OMM_EXTRAS_REVEAL_SECRETS) {
        o->oGraphNode = gLoadedGraphNodes[MODEL_PURPLE_MARBLE];
        o->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
    } else {
        o->oGraphNode = NULL;
        o->oNodeFlags &= ~GRAPH_RENDER_BILLBOARD;
    }
    return false;
}

static bool bhv_intro_lakitu_fix(NativeBhvFunc func) {
    o->oGraphNode = geo_layout_to_graph_node(NULL, lakitu_geo);
    return false;
}

static bool bhv_king_bobomb_fix(NativeBhvFunc func) {
    if (m->action == ACT_GRABBED && o->oAction >= 7) {
        o->oInteractStatus &= ~INT_STATUS_GRABBED_MARIO;
        o->oIntangibleTimer = -1;
        o->oChuckyaUnk88 = 0;
        omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
    }
    return false;
}

static bool bhv_mad_piano_always_tangible(NativeBhvFunc func) {
    static struct ObjectHitbox sMadPianoHitbox = { INTERACT_MR_BLIZZARD, 0, 3, 99, 0, 200, 150, 200, 150 };
    obj_set_hitbox(o, &sMadPianoHitbox);
    obj_set_home(o, o->oPosX, o->oPosY, o->oPosZ);
    return false;
}

static bool bhv_manta_ray_water_ring_fix(NativeBhvFunc func) {
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

static bool bhv_monty_mole_fix(NativeBhvFunc func) {
    if (o->oAction == MONTY_MOLE_ACT_SELECT_HOLE) {
        o->oInteractStatus = 0;
    }
    return false;
}

static bool bhv_menu_button_character_select(NativeBhvFunc func) {
    if (obj_check_model(o, MODEL_MAIN_MENU_PURPLE_SOUND_BUTTON)) {
        struct Object *characterSelectButton = obj_get_first_with_behavior(omm_bhv_menu_character_select_button);
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

static bool bhv_menu_button_manager_spawn_character_select(NativeBhvFunc func) {
    struct Object *charSelButton = spawn_object_rel_with_rot(o, MODEL_NONE, omm_bhv_menu_character_select_button, 6400.f, -3500.f, 0, 0, 0, 0);
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

static bool bhv_mips_spawn_at_120_stars(NativeBhvFunc func) {
    if (m->numStars >= 120) {
        o->oBehParams2ndByte = (OMM_GAME_IS_SMSR ? 0 : 1);
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

static bool bhv_motos_attacked(NativeBhvFunc func) {
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

static bool bhv_moving_yellow_coin_fix(NativeBhvFunc func) {
    o->oForwardVel /= 2.f;
    return false;
}

static bool bhv_mr_i_target_mario(NativeBhvFunc func) {
    if (o->oAction <= 1 && obj_get_distance(o, mo) < 300.f) {
        s16 angleToMario = obj_get_object1_angle_yaw_to_object2(o, mo);
        o->oFaceAngleYaw = angleToMario;
        o->oMoveAngleYaw = angleToMario;
        o->oAction = 2;
        o->oMrIUnkFC = 0;
        o->oMrIUnk100 = 0;
        o->oMrIUnk104 = 0;
        o->oMrIUnk108 = (s32)(random_float() * 50.f + 50.f);
    }
    return false;
}

static bool bhv_purple_switch_fix(NativeBhvFunc func) {
    if (o->oAction == PURPLE_SWITCH_IDLE && omm_mario_is_ground_pound_landing(m) &&
        o == mo->platform && obj_get_horizontal_distance(o, mo) < 85.f * o->oScaleX) {
        o->oAction = PURPLE_SWITCH_PRESSED;
        return true;
    }
    return false;
}

static bool bhv_recovery_heart_heal_mario_for_1_hp(NativeBhvFunc func) {
    if (o->oSpinningHeartTotalSpin + o->oAngleVelYaw >= 0x10000) {
        omm_health_heal_mario(m);
    }
    return false;
}

static bool bhv_red_coin_fix(NativeBhvFunc func) {
    if (o->parentObj == NULL ||
        o->parentObj->activeFlags == ACTIVE_FLAG_DEACTIVATED || (
        o->parentObj->behavior != bhvHiddenRedCoinStar &&
        o->parentObj->behavior != bhvBowserCourseRedCoinStar)) {
        o->parentObj = NULL;
    }
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (o->parentObj) {
            o->parentObj->oHiddenStarTriggerCounter++;
            omm_spawn_number(o, 1 + omm_level_get_num_red_coins(gCurrLevelNum, gCurrAreaIndex) - obj_get_count_with_behavior(bhvRedCoin));
            play_sound(SOUND_MENU_COLLECT_RED_COIN + (max_s(0, o->parentObj->oHiddenStarTriggerCounter - 1) << 16), gGlobalSoundArgs);
        }
        coin_collected();
        o->oInteractStatus = 0;
        return true;
    }
    return false;
}

static bool bhv_sl_snowman_wind_blow_if_sparkly_stars_enabled(NativeBhvFunc func) {
    if (OMM_SSM_IS_ENABLED) {
        o->oSubAction = SL_SNOWMAN_WIND_ACT_BLOWING;
    }
    return false;
}

static bool bhv_snowmans_bottom_fix(NativeBhvFunc func) {
    switch (o->oAction) {

        // Fix a softlock that can happen if Mario is not
        // in the 400 units radius after starting the dialog
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

static bool bhv_snowmans_head_non_stop_dont_spawn_whole(NativeBhvFunc func) {
    if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) {
        cur_obj_scale(0.7f);
        o->oGravity = 5.f;
        o->oFriction = 0.999f;
        o->oBuoyancy = 2.f;
        return true;
    }
    return false;
}

static bool bhv_thi_tiny_island_top_fix(NativeBhvFunc func) {
    if (o->oAction == 0 && !gTHIWaterDrained && omm_mario_is_ground_pound_landing(m) && obj_get_distance(o, mo) < 500.0f) {
        obj_spawn_particles(o, 30, MODEL_WHITE_PARTICLE_SMALL, 0, 40, 0, 20, 40, -4, 2, 0);
        obj_spawn_triangle_break_particles(o, OBJ_SPAWN_TRI_BREAK_PRESET_DIRT_SMALL);
        play_sound(SOUND_GENERAL_ACTIVATE_CAP_SWITCH, o->oCameraToObject);
        o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        o->oAction++;
        return true;
    }
    return false;
}

static bool bhv_tuxies_mother_chase_mario(NativeBhvFunc func) {
    if (o->oAction == 2 && m->action == ACT_OMM_POSSESSION && gOmmCapture->behavior == bhvUnused20E0) {
        gOmmCapture->oHeldState = HELD_HELD;
    }
    return false;
}

static bool bhv_ukiki_fix(NativeBhvFunc func) {
    if (o->oBehParams2ndByte == UKIKI_CAGE) {
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

static bool bhv_wooden_post_spawn_coins(NativeBhvFunc func) {
    if (!(o->oBehParams & WOODEN_POST_BP_NO_COINS_MASK) && o->oWoodenPostOffsetY <= -190.f) {
        o->oPosY = o->oHomeY;
        obj_spawn_loot_yellow_coins(o, 5, 20.f);
        set_object_respawn_info_bits(o, 1);
        o->oBehParams |= WOODEN_POST_BP_NO_COINS_MASK;
        o->oPosY = o->oHomeY + o->oWoodenPostOffsetY;
    }
    return false;
}

static bool bhv_yoshi_surprise(NativeBhvFunc func) {
#if OMM_GAME_IS_SM64
    if (o->oAction == YOSHI_ACT_GIVE_PRESENT) {
        o->oAction = YOSHI_ACT_IDLE;
        omm_spawn_problem(m->marioObj);
        omm_mario_set_action(m, (random_u16() & 1) ? ACT_HARD_FORWARD_GROUND_KB : ACT_HARD_BACKWARD_GROUND_KB, 1, 0);
        if (OMM_MOVESET_ODYSSEY) {
            m->health = OMM_HEALTH_DEAD;
        } else {
            m->healCounter = 0;
            m->hurtCounter = 64;
        }
    }
#endif
    return false;
}

#undef m
#undef mo
#pragma GCC pop_options

OMM_AT_STARTUP static void omm_setup_behavior_update_functions_map() {
    static const void *sBhvUpdateTable[][2] = {
        { bhv_1up_init,                         bhv_1up_always_spawn },
        { bhv_1up_loop,                         bhv_1up_life_up },
        { bhv_1up_hidden_loop,                  bhv_1up_life_up },
        { bhv_1up_hidden_in_pole_loop,          bhv_1up_life_up },
        { bhv_1up_jump_on_approach_loop,        bhv_1up_life_up },
        { bhv_1up_running_away_loop,            bhv_1up_life_up },
        { bhv_1up_sliding_loop,                 bhv_1up_life_up },
        { bhv_1up_walking_loop,                 bhv_1up_life_up },
        { bhv_blue_coin_switch_loop,            bhv_blue_coin_switch_respawn },
        { bhv_boo_loop,                         bhv_boo_uncaptured },
        { bhv_bowser_loop,                      bhv_bowser_ohkoed },
        { bhv_bowser_course_red_coin_star_loop, bhv_red_coin_star_flag_and_dont_respawn },
        { bhv_bowser_shock_wave_loop,           bhv_bowser_shock_wave_fix },
        { bhv_bowser_tail_anchor_loop,          bhv_bowser_tail_anchor_fix},
        { bhv_bowsers_sub_loop,                 bhv_bowsers_sub_non_stop_spawn },
        { bhv_ccm_touched_star_spawn_loop,      bhv_star_dont_respawn },
        { bhv_checkerboard_elevator_group_init, bhv_checkerboard_elevator_group_fix },
        { bhv_coin_inside_boo_loop,             bhv_coin_inside_boo_always_blue },
        { bhv_collect_star_init,                bhv_star_dont_respawn },
        { bhv_collect_star_loop,                bhv_star_dont_respawn },
        { bhv_ddd_pole_init,                    bhv_ddd_pole_non_stop_spawn },
        { bhv_exclamation_box_loop,             bhv_exclamation_box_fix },
        { bhv_explosion_loop,                   bhv_explosion_fix },
        { bhv_flamethrower_loop,                bhv_flamethrower_always_active },
        { bhv_flamethrower_flame_loop,          bhv_flamethrower_flame_fix },
        { bhv_hidden_blue_coin_loop,            bhv_hidden_blue_coin_respawn },
        { bhv_hidden_red_coin_star_init,        bhv_red_coin_star_flag_and_dont_respawn },
        { bhv_hidden_red_coin_star_loop,        bhv_red_coin_star_flag_and_dont_respawn },
        { bhv_hidden_star_init,                 bhv_star_dont_respawn },
        { bhv_hidden_star_loop,                 bhv_star_dont_respawn },
        { bhv_hidden_star_trigger_loop,         bhv_hidden_star_trigger_reveal },
        { bhv_init_room,                        do_nothing },
        { bhv_intro_lakitu_loop,                bhv_intro_lakitu_fix },
        { bhv_king_bobomb_loop,                 bhv_king_bobomb_fix },
        { bhv_mad_piano_update,                 bhv_mad_piano_always_tangible },
        { bhv_manta_ray_water_ring_init,        bhv_manta_ray_water_ring_fix },
        { bhv_monty_mole_update,                bhv_monty_mole_fix },
        { bhv_menu_button_loop,                 bhv_menu_button_character_select },
        { bhv_menu_button_manager_init,         bhv_menu_button_manager_spawn_character_select },
        { bhv_mips_init,                        bhv_mips_spawn_at_120_stars },
#if OMM_GAME_IS_R96A
        { bhv_motos_loop,                       bhv_motos_attacked },
#endif
        { bhv_moving_yellow_coin_init,          bhv_moving_yellow_coin_fix },
        { bhv_mr_i_loop,                        bhv_mr_i_target_mario },
        { bhv_purple_switch_loop,               bhv_purple_switch_fix },
        { bhv_recovery_heart_loop,              bhv_recovery_heart_heal_mario_for_1_hp },
        { bhv_red_coin_loop,                    bhv_red_coin_fix },
        { bhv_sl_snowman_wind_loop,             bhv_sl_snowman_wind_blow_if_sparkly_stars_enabled },
        { bhv_snowmans_bottom_loop,             bhv_snowmans_bottom_fix },
        { bhv_snowmans_head_init,               bhv_snowmans_head_non_stop_dont_spawn_whole },
        { bhv_spawned_star_loop,                bhv_star_dont_respawn },
        { bhv_star_spawn_init,                  bhv_star_dont_respawn },
        { bhv_star_spawn_loop,                  bhv_star_dont_respawn },
        { bhv_thi_tiny_island_top_loop,         bhv_thi_tiny_island_top_fix },
        { bhv_tuxies_mother_loop,               bhv_tuxies_mother_chase_mario },
        { bhv_ukiki_loop,                       bhv_ukiki_fix },
        { bhv_wooden_post_update,               bhv_wooden_post_spawn_coins },
        { bhv_yoshi_loop,                       bhv_yoshi_surprise },
        { load_object_collision_model,          do_nothing },
    };
    for (s32 i = 0; i != OMM_ARRAY_SIZE(sBhvUpdateTable); ++i) {
        omm_hmap_insert(sBhvUpdateMap, (uintptr_t) sBhvUpdateTable[i][0], sBhvUpdateTable[i][1]);
    }
}

static void cur_obj_update_behavior_func(void (*func)(void)) {
    if (!cur_obj_update_knockback()) {
        bool doSafeStep = (omm_obj_is_goomba(gCurrentObject) || omm_obj_is_coin(gCurrentObject));
        if (doSafeStep) obj_safe_step(gCurrentObject, 0);
        s32 i = omm_hmap_find(sBhvUpdateMap, (uintptr_t) func);
        if (i == -1 || !omm_hmap_get(sBhvUpdateMap, UpdateFunc, i)(func)) {
            if (func) func();
            if (doSafeStep) obj_safe_step(gCurrentObject, 1);
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
    child->oBehParams2ndByte = bhv_cmd_get(s16, 0, 1);
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

#if OMM_GAME_IS_R96A
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
#if OMM_GAME_IS_R96A
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

    // Copy face angle to move angle
    if (objFlags & OBJ_FLAG_SET_FACE_ANGLE_TO_MOVE_ANGLE) {
        obj_set_face_angle_to_move_angle(o);
    }

    // Copy face yaw to move yaw
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
        o->collisionData == NULL)
    {
        o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
        o->activeFlags &= ~ACTIVE_FLAG_FAR_AWAY;
    }
}

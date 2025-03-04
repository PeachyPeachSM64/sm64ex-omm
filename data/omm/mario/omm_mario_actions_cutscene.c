#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static bool omm_act_exit_launch_mario_until_land(struct MarioState *m, s32 endAction, s32 animID, f32 forwardVel, u16 delay) {
    if (m->actionTimer++ >= delay) {
        m->marioObj->oNodeFlags |= GRAPH_RENDER_ACTIVE;
        mario_set_forward_vel(m, forwardVel);
        ANM(animID, 1.f);
        if (perform_air_step(m, 0) == AIR_STEP_LANDED) {
            omm_mario_set_action(m, endAction, 0, 0);
            return true;
        }
        return false;
    }
    m->marioObj->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
    return false;
}

//////////
// SM64 //
//////////

static s32 omm_act_cutscene_cancels__star_dance(struct MarioState *m) {
    if (OMM_ALL_STARS) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    u32 actionArg = (m->prevAction & ACT_FLAG_METAL_WATER) || ((m->prevAction & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED);
    action_condition(OMM_STARS_NON_STOP, ACT_OMM_STAR_DANCE, actionArg, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_reading_automatic_dialog(struct MarioState *m) {
    if (OMM_STARS_NON_STOP) {
        s32 dialogId = (GET_HIGH_U16_OF_32(m->actionArg) != 0 ? GET_HIGH_U16_OF_32(m->actionArg) : GET_LOW_U16_OF_32(m->actionArg));
        if ((dialogId < DIALOG_022 || dialogId > DIALOG_029) && dialogId < OMM_DIALOG_START_INDEX) {
            m->actionState = 24;
        }
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_shocked(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY && m->actionArg == 0 && ++m->actionState > 45, ACT_FREEFALL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_squished(struct MarioState *m) {
    if (m->ceilHeight - m->floorHeight > 150.f) {
        m->input &= ~INPUT_SQUISHED;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_disappeared(struct MarioState *m) {
    action_condition(m->prevAction == ACT_OMM_DEATH, ACT_OMM_DEATH_SQUISHED, 0, RETURN_CANCEL); // Squished (Odyssey)

    // Squished (classic): turn Mario into a pancake
    if (m->prevAction == ACT_SQUISHED) {
        ANM(MARIO_ANIM_A_POSE, 1.f);
        stop_and_set_height_to_floor(m);
        m->squishTimer = 0xFF;
        obj_set_scale(m->marioObj, 2.f, 0.05f, 2.f);
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_exit(struct MarioState *m, bool deathExit, u32 endAction, s32 animID, f32 forwardVel, s32 delay, s32 soundBits) {
    if (!deathExit && soundBits) {
        play_sound_if_no_flag(m, soundBits, MARIO_MARIO_SOUND_PLAYED);
    }
    if (omm_act_exit_launch_mario_until_land(m, endAction, animID, forwardVel, delay)) {
        if (deathExit && soundBits) {
            SFX(soundBits);
        }
        omm_health_fully_heal_mario(m, false);
        m->healCounter = 31;
        m->actionArg = (m->action == ACT_SPECIAL_EXIT_AIRBORNE);
        omm_stats_increase(deaths, deathExit);
    } else if (deathExit) {
        if (OMM_MOVESET_ODYSSEY) {
            m->health = omm_health_odyssey(OMM_HEALTH_ODYSSEY_DEAD);
            gOmmMario->state.ticks = omm_health_to_ticks(m->health);
        } else {
            m->health = OMM_HEALTH_CLASSIC_1_SEGMENT;
        }
    }
    if (!deathExit) {
        m->marioObj->oGfxAngle[1] += 0x8000;
        PFX(PARTICLE_SPARKLES);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_unlocking_key_door(struct MarioState *m) {
    static struct Object *door1 = NULL;
    static struct Object *door2 = NULL;
    if (OMM_MOVESET_ODYSSEY) {
        static f32 sMarioPosX = 0;
        static f32 sMarioPosZ = 0;
        if (m->actionTimer++ == 0) {
            door1 = obj_get_nearest_with_behavior(m->marioObj, bhvDoorWarp);
            door2 = obj_get_nearest_with_behavior(door1, bhvDoorWarp);
            if (!door1 || !door2) {
                return omm_mario_set_action(m, ACT_IDLE, 0, 0);
            }

            // Point between the 2 doors
            Vec3f p1; obj_pos_as_vec3f(door1, p1);
            Vec3f p2; obj_pos_as_vec3f(door2, p2);
            Vec3f p; vec3f_mul(vec3f_sum(p, p1, p2), 0.5f);

            // Angles
            s16 angleMario = atan2s(m->pos[2] - p[2], m->pos[0] - p[0]);
            s16 angleDoor1 = atan2s(p1[2] - p[2], p1[0] - p[0]) + 0x4000;
            s16 angleDoor2 = atan2s(p2[2] - p[2], p2[0] - p[0]) + 0x4000;
            if (abs_s((s16) (angleMario - angleDoor1)) < abs_s((s16) (angleMario - angleDoor2))) {
                m->faceAngle[1] = angleDoor1 + 0x8000;
            } else {
                m->faceAngle[1] = angleDoor2 + 0x8000;
            }

            // Correct Mario pos
            m->pos[0] = sMarioPosX = p[0] + 15 * sins(m->faceAngle[1] + 0x8000);
            m->pos[2] = sMarioPosZ = p[2] + 15 * coss(m->faceAngle[1] + 0x8000);
            struct Object *key = spawn_object(m->marioObj, MODEL_BOWSER_KEY_CUTSCENE, bhvBowserKeyUnlockDoor);
            key->oFaceAngleYaw = m->faceAngle[1];
            key->oPosX = m->pos[0];
            key->oPosY = m->pos[1];
            key->oPosZ = m->pos[2];
            set_mario_animation(m, MARIO_ANIM_UNLOCK_DOOR);
        }

        // Set Mario pos
        m->pos[0] = sMarioPosX;
        m->pos[2] = sMarioPosZ;

        // Anim and sound
        update_mario_pos_for_anim(m);
        stop_and_set_height_to_floor(m);
        switch (m->marioObj->oAnimFrame) {
            case 79:  SFX(SOUND_GENERAL_DOOR_INSERT_KEY); break;
            case 111: SFX(SOUND_GENERAL_DOOR_TURN_KEY);   break;
        }
        if (obj_anim_is_at_end(m->marioObj)) {
            if (m->usedObj->oBehParams >> 24 == 1) {
                omm_save_file_set_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR);
                omm_save_file_clear_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, SAVE_FLAG_HAVE_KEY_2);
            } else {
                omm_save_file_set_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, SAVE_FLAG_UNLOCKED_BASEMENT_DOOR);
                omm_save_file_clear_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, SAVE_FLAG_HAVE_KEY_1);
            }
            bool isPullingDoor1 = (should_push_or_pull_door(m, door1) == 1);
            m->interactObj = m->usedObj = (isPullingDoor1 ? door1 : door2);
            omm_mario_set_action(m, ACT_PULLING_DOOR, 5, 0);
        }
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_reading_sign(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_ACTION_READ_SIGN, MARIO_ACTION_SOUND_PLAYED);
    switch (m->actionState) {

        // Start cutscene
        case 0: {
            if (m->usedObj->behavior != bhvOmmStatsBoard) {
                trigger_cutscene_dialog(1);
            }
            enable_time_stop();
            set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
            m->actionState = 1;
        } break;

        // Turn toward sign
        case 1: {
            m->faceAngle[1] += m->marioObj->oMarioPoleUnk108 / 11;
            m->pos[0] += m->marioObj->oMarioReadingSignDPosX / 11.f;
            m->pos[2] += m->marioObj->oMarioReadingSignDPosZ / 11.f;
            if (m->actionTimer++ == 10) {
                if (m->usedObj->behavior == bhvOmmStatsBoard) {
                    m->usedObj->oAction = 1;
                    m->actionState = 3;
                } else {
                    create_dialog_inverted_box(m->usedObj->oBehParams2ndByte);
                    m->actionState = 2;
                }
            }
        } break;

        // Wait for the dialog to end
        case 2: {
            if (gDialogID == -1 && gCamera->cutscene == 0) {
                disable_time_stop();
                set_mario_action(m, ACT_IDLE, 0);
            }
        } break;

        // Wait for the stats board to close
        case 3: {
            if (m->usedObj->oAction == 0) {
                disable_time_stop();
                set_mario_action(m, ACT_IDLE, 0);
            }
        } break;
    }

    // Update gfx
    vec3f_copy(m->marioObj->oGfxPos, m->pos);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1], 0);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

///////////////////
// Odyssey death //
///////////////////

#define MARIO_ANIM_NONE (0)
#define MARIO_ANIM_CURRENT (-1)
#define mGfx m->marioObj->header.gfx
#define mAnm m->marioObj->oAnimInfo

typedef union { struct Object *obj; Vec3f pos; } OmmLostCoin;
static OmmLostCoin sOmmLostCoins[20];
static s32 sOmmLostCoinsCount = 0;
static u8 sOmmStarsCurrFlags = 0;

static void omm_lost_coins_spawn(struct MarioState *m) {
    sOmmLostCoinsCount = (s32) clamp_s(m->numCoins, 0, 20);
    for (s32 i = 0; i != sOmmLostCoinsCount; ++i) {
        struct Object *coin   = spawn_object(m->marioObj, MODEL_YELLOW_COIN, bhvYellowCoin);
        coin->oHomeX          = m->pos[0];
        coin->oHomeY          = m->pos[1] + 80;
        coin->oHomeZ          = m->pos[2];
        coin->oFaceAnglePitch = 0;
        coin->oFaceAngleYaw   = 0;
        coin->oFaceAngleRoll  = 0;
        coin->oMoveAnglePitch = 0;
        coin->oMoveAngleYaw   = 0;
        coin->oMoveAngleRoll  = 0;
        coin->oVelY           = 52.f;
        coin->oForwardVel     = 8.f;
        sOmmLostCoins[i].obj  = coin;
    }
}

static void omm_lost_coins_update() {
    for (s32 i = 0; i != sOmmLostCoinsCount; ++i) {
        OmmLostCoin *coin = &sOmmLostCoins[i];
        f32 angle = 65536.f * (f32) i / (f32) sOmmLostCoinsCount;
        coin->obj->oVelX = coin->obj->oForwardVel * coss(angle);
        coin->obj->oVelY = max_f(coin->obj->oVelY - 4.f, -72.f);
        coin->obj->oVelZ = coin->obj->oForwardVel * sins(angle);

        // Move coin
        u8 isUnderwater = (find_water_level(coin->obj->oPosX, coin->obj->oPosZ) > coin->obj->oPosY);
        u8 numSteps = (isUnderwater ? 5 : 8);
        for (s32 step = 0; step != numSteps; ++step) {
            f32 x = coin->obj->oPosX;
            f32 y = coin->obj->oPosY;
            f32 z = coin->obj->oPosZ;
            coin->obj->oPosX += (coin->obj->oVelX / 8);
            coin->obj->oPosY += (coin->obj->oVelY / 8);
            coin->obj->oPosZ += (coin->obj->oVelZ / 8);

            // Walls
            struct WallCollisionData hitbox;
            hitbox.x = coin->obj->oPosX;
            hitbox.y = coin->obj->oPosY;
            hitbox.z = coin->obj->oPosZ;
            hitbox.offsetY = 0;
            hitbox.radius = 40;
            if (find_wall_collisions(&hitbox) != 0) {
                coin->obj->oPosX = hitbox.x;
                coin->obj->oPosY = hitbox.y;
                coin->obj->oPosZ = hitbox.z;
            }

            // Floor
            struct Surface *floor = NULL;
            f32 floorY = find_floor(coin->obj->oPosX, coin->obj->oPosY, coin->obj->oPosZ, &floor);
            if (floor && coin->obj->oPosY <= floorY) {
                coin->obj->oPosY = floorY;
                coin->obj->oVelY *= -0.85f;
                coin->obj->oForwardVel *= 0.8f;
                if (SURFACE_IS_DEATH_PLANE(floor->type)) {
                    coin->obj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                }
                break;
            } else if (!floor) {
                coin->obj->oPosX = x;
                coin->obj->oPosY = y;
                coin->obj->oPosZ = z;
                coin->obj->oForwardVel = 0;
                break;
            }
        }
    }
}

static void omm_lost_coins_save() {
    sOmmStarsCurrFlags = omm_stars_get_flags();
    for (s32 i = 0; i != sOmmLostCoinsCount; ++i) {
        struct Object *coin = sOmmLostCoins[i].obj;
        obj_pos_as_vec3f(coin, sOmmLostCoins[i].pos);
    }
}

static void omm_lost_coins_respawn() {
    if (OMM_STARS_NON_STOP && !OMM_SPARKLY_MODE_IS_LUNATIC && !omm_is_main_menu() && !time_trials_is_enabled()) {
        omm_stars_set_flags(sOmmStarsCurrFlags);
        struct MarioState *m = gMarioState;
        for (s32 i = 0; i != sOmmLostCoinsCount; ++i) {
            f32 x = sOmmLostCoins[i].pos[0];
            f32 y = sOmmLostCoins[i].pos[1];
            f32 z = sOmmLostCoins[i].pos[2];

            // Respawn coin only if not above OoB or death barrier
            struct Surface *floor = NULL;
            f32 floorHeight = find_floor(x, y, z, &floor);
            if (!floor || SURFACE_IS_DEATH_PLANE(floor->type)) {
                continue;
            }

            // Respawn coin
            struct Object *coin   = spawn_object(m->marioObj, MODEL_YELLOW_COIN, bhvYellowCoin);
            coin->respawnInfoType = OMM_OBJECT_LOST_COIN;
            coin->oPosX           = x;
            coin->oPosY           = floorHeight;
            coin->oPosZ           = z;
            coin->oHomeX          = x;
            coin->oHomeY          = floorHeight;
            coin->oHomeZ          = z;
            coin->oFaceAnglePitch = 0;
            coin->oFaceAngleYaw   = 0;
            coin->oFaceAngleRoll  = 0;
            coin->oMoveAnglePitch = 0;
            coin->oMoveAngleYaw   = 0;
            coin->oMoveAngleRoll  = 0;
        }
    }
    sOmmLostCoinsCount = 0;
    sOmmStarsCurrFlags = 0;
}

typedef struct {
    struct {
        s32 animID; // mAnm.animID if MARIO_ANIM_CURRENT
        f32 animAccel;
        s16 anglePitch; // mGfx.angle[0] if -1
        f32 frameMin; // mAnm.animFrame if -1
        f32 frameMax; // mAnm.animFrame if -1
        f32 yOffset;
    } anim1;
    struct {
        s32 animID; // mAnm.animID if MARIO_ANIM_CURRENT
        f32 animAccel;
        s16 anglePitch; // mGfx.angle[0] if -1
        f32 frameMin; // mAnm.animFrame if -1
        f32 frameMax; // mAnm.animFrame if -1
        f32 yVelInit; // m->vel[1] if -1
        f32 yVelDec;
        f32 yVelLimit;
    } anim2;
} OmmActDeathAnimParams;

static const OmmActDeathAnimParams *omm_act_death_get_anim_params(s32 deathType) {
    static const OmmActDeathAnimParams OMM_ACT_DEATH_ANIM_PARAMS[2][7] = {
    { { { MARIO_ANIM_AIR_FORWARD_KB,     1.00f, 0xE000,  0,  0, 16          },
        { MARIO_ANIM_BACKWARDS_WATER_KB, 1.00f, 0x0000,  0, 20, 40, -2, -40 } },   // Default
      { { MARIO_ANIM_DROWNING_PART2,     1.00f, 0x0000,  0,  0, 16          },
        { MARIO_ANIM_BACKWARDS_WATER_KB, 0.75f, 0x0000,  0, 20, 20, -1, -20 } },   // Water
      { { MARIO_ANIM_BEING_GRABBED,      2.00f, 0x0000,  0, 30,  0          },
        { MARIO_ANIM_BEING_GRABBED,      2.00f, 0x0000,  0, 30, -1, -2, -90 } },   // Fall
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  0          },
        { MARIO_ANIM_CURRENT,            1.00f,     -1, -1, -1,  0,  0,   0 } },   // Frozen
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  0          },
        { MARIO_ANIM_CURRENT,            1.00f,     -1, -1, -1,  0,  0,   0 } },   // Eaten by Bubba
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  8          },
        { MARIO_ANIM_DYING_IN_QUICKSAND, 1.00f, 0x0000,  0, 60,  0, -3,  -3 } },   // Quicksand
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  0          },
        { MARIO_ANIM_A_POSE,             1.00f, 0x0000,  0,  0,  0,  0,   0 } } }, // Squished
    { { { MARIO_ANIM_OMM_DEATH,          1.00f, 0x0000,  0,  5,  0          },
        { MARIO_ANIM_OMM_DEATH,          1.00f, 0x0000,  6, 91, 40, -2, -50 } },   // Default
      { { MARIO_ANIM_OMM_DROWN,          1.00f, 0x0000,  0,  5,  0          },
        { MARIO_ANIM_OMM_DROWN,          1.00f, 0x0000,  6, 91, 20, -1, -20 } },   // Water
      { { MARIO_ANIM_BEING_GRABBED,      2.00f, 0x0000,  0, 30,  0          },
        { MARIO_ANIM_BEING_GRABBED,      2.00f, 0x0000,  0, 30, -1, -2, -90 } },   // Fall
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  0          },
        { MARIO_ANIM_OMM_FREEZE,         1.00f, 0x0000,  0,  7,  0,  0,   0 } },   // Frozen
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  0          },
        { MARIO_ANIM_CURRENT,            1.00f,     -1, -1, -1,  0,  0,   0 } },   // Eaten by Bubba
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  8          },
        { MARIO_ANIM_DYING_IN_QUICKSAND, 1.00f, 0x0000,  0, 60,  0, -3,  -3 } },   // Quicksand
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  0          },
        { MARIO_ANIM_A_POSE,             1.00f, 0x0000,  0,  0,  0,  0,   0 } } }, // Squished
    };
    return &OMM_ACT_DEATH_ANIM_PARAMS[OMM_EXTRAS_SMO_ANIMATIONS][deathType];
}

static void omm_act_death_play_sound(struct MarioState *m, s32 type) {
    switch (type) {
        case 0: omm_sound_play(OMM_SOUND_EVENT_DEATH_MARIO, m->marioObj->oCameraToObject); break;
        case 1: omm_sound_play(OMM_SOUND_EVENT_DEATH_MARIO_WATER, m->marioObj->oCameraToObject); break;
        case 2: omm_sound_play(OMM_SOUND_EVENT_DEATH_MARIO_FALL, m->marioObj->oCameraToObject); break;
    }
}

static void omm_act_death_handler(struct MarioState *m, s32 type, bool lookAtCamera, u16 tAnim2, u16 tBowserFace, u16 tEnd, const OmmActDeathAnimParams *anims) {
    static f32 sMarioPosY;
    static f32 sAnim2Vel;

    // Params
    s32 anim1_animId     = (anims->anim1.animID == MARIO_ANIM_CURRENT ? mAnm.animID : anims->anim1.animID);
    f32 anim1_animAccel  = (anims->anim1.animAccel);
    s16 anim1_anglePitch = (anims->anim1.anglePitch == -1 ? mGfx.angle[0] : anims->anim1.anglePitch);
    f32 anim1_frameMin   = (anims->anim1.frameMin == -1 ? mAnm.animFrame : anims->anim1.frameMin);
    f32 anim1_frameMax   = (anims->anim1.frameMax == -1 ? mAnm.animFrame : anims->anim1.frameMax);
    f32 anim1_yOffset    = (anims->anim1.yOffset);
    s32 anim2_animId     = (anims->anim2.animID == MARIO_ANIM_CURRENT ? mAnm.animID : anims->anim2.animID);
    f32 anim2_animAccel  = (anims->anim2.animAccel);
    s16 anim2_anglePitch = (anims->anim2.anglePitch == -1 ? mGfx.angle[0] : anims->anim2.anglePitch);
    f32 anim2_frameMin   = (anims->anim2.frameMin == -1 ? mAnm.animFrame : anims->anim2.frameMin);
    f32 anim2_frameMax   = (anims->anim2.frameMax == -1 ? mAnm.animFrame : anims->anim2.frameMax);
    f32 anim2_yVelInit   = (anims->anim2.yVelInit == -1 ? m->vel[1] : anims->anim2.yVelInit);
    f32 anim2_yVelDec    = (anims->anim2.yVelDec);
    f32 anim2_yVelLimit  = (anims->anim2.yVelLimit);

    // Init
    // Background music is reduced
    // Odyssey Death sound is played
    // Mario looks at the camera
    // Spawn lost coins around Mario
    // Recall Cappy, remove caps
    if (m->actionTimer == 0) {
        music_lower_volume(SEQ_PLAYER_LEVEL, 60, 40);
        omm_act_death_play_sound(m, type);
        if (type != 2) {
            set_camera_shake_from_hit(SHAKE_FALL_DAMAGE);
        }
        if (lookAtCamera) {
            m->faceAngle[1] = m->area->camera->yaw;
            mGfx.angle[1] = m->area->camera->yaw;
        }
        omm_mario_unset_cap(m);
        omm_lost_coins_spawn(m);
        omm_cappy_return_to_mario(omm_cappy_get_object());
        sAnim2Vel = anim2_yVelInit;
        sMarioPosY = m->pos[1] + anim1_yOffset;
        gOmmMario->state.health.state = OMM_HEALTH_STATE_DAMAGE;
        gOmmMario->state.health.timer = 0;
        omm_stats_increase(deaths, 1);
    }

    // Fix the camera during the animation
    omm_mario_lock_camera(m, false);

    // Mario progressively loses all of his coins
    omm_lost_coins_update();
    if (m->numCoins > 0) {
        m->numCoins--;
        gHudDisplay.coins = m->numCoins;
    }

    // Update health
    if (OMM_MOVESET_ODYSSEY) {
        m->health = OMM_HEALTH_DEAD;
    } else {
        m->healCounter = 0;
        m->hurtCounter = 30;
    }

    // Animations
    if (m->actionTimer < tAnim2) {
        ANM(anim1_animId, anim1_animAccel);
        obj_anim_clamp_frame(m->marioObj, anim1_frameMin, anim1_frameMax);
        mGfx.angle[0] = anim1_anglePitch;
    } else {
        ANM(anim2_animId, anim2_animAccel);
        obj_anim_clamp_frame(m->marioObj, anim2_frameMin, anim2_frameMax);
        mGfx.angle[0] = anim2_anglePitch;
        sMarioPosY += sAnim2Vel;
        sAnim2Vel = max_f(sAnim2Vel + anim2_yVelDec, anim2_yVelLimit);
    }
    m->marioBodyState->eyeState = MARIO_EYES_DEAD;
    m->marioObj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    mGfx.angle[2] = 0;
    mGfx.pos[1] = sMarioPosY;

    // Fade-out, Bowser face
    // 30 frames long, black screen until tEnd
    if (m->actionTimer == tBowserFace) {
        play_transition(WARP_TRANSITION_FADE_INTO_BOWSER, 30, 0x00, 0x00, 0x00);
    }

    // Send the signal to restart the level
    if (m->actionTimer == tEnd) {
        if (time_trials_is_enabled()) {
            omm_restart_level();
        } else {
            omm_lost_coins_save();
            omm_add_routine(OMM_ROUTINE_TYPE_LEVEL_ENTRY, omm_lost_coins_respawn);
            omm_restart_area();
        }

        // Restore Mario's health
        m->health = OMM_HEALTH_DEAD;
        omm_health_set_max(m);
    }

    m->actionTimer++;
}

static s32 omm_act_death(struct MarioState *m) {
    omm_act_death_handler(m, 0, true, 24, 84, 124, omm_act_death_get_anim_params(0));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_water(struct MarioState *m) {
    omm_act_death_handler(m, 1, true, 24, 84, 124, omm_act_death_get_anim_params(1));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_fall(struct MarioState *m) {
    omm_act_death_handler(m, 2, false, 0, 60, 100, omm_act_death_get_anim_params(2));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_frozen(struct MarioState *m) {
    omm_act_death_handler(m, 0, OMM_EXTRAS_SMO_ANIMATIONS, 0, 84, 124, omm_act_death_get_anim_params(3));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_eaten_by_bubba(struct MarioState *m) {
    omm_act_death_handler(m, 0, false, 0, 84, 124, omm_act_death_get_anim_params(4));
    m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_quicksand(struct MarioState *m) {
    omm_act_death_handler(m, 2, false, 0, 60, 100, omm_act_death_get_anim_params(5));
    if (m->actionTimer < 75) SFX(SOUND_MOVING_QUICKSAND_DEATH);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_squished(struct MarioState *m) {
    omm_act_death_handler(m, 0, false, 0, 84, 124, omm_act_death_get_anim_params(6));
    m->squishTimer = 0xFF;
    mGfx.scale[0] = 2.00f;
    mGfx.scale[1] = 0.05f;
    mGfx.scale[2] = 2.00f;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

////////////////////////
// Odyssey star dance //
////////////////////////

typedef struct {
    s32 animID;
    s32 handState;
    s16 yawOffset;
    f32 yOffset;
} OmmStarDanceAnimFrame;

static const OmmStarDanceAnimFrame OMM_STAR_DANCE_ANIM_FRAMES[2][48] = {
{ { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS,      0x0000,  0 }, // 00
  { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS,      0x0000,  0 }, // 01
  { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS,      0x0000,  0 }, // 02
  { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS,      0x0080,  0 }, // 03
  { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS,      0x0100,  0 }, // 04
  { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS,      0x0200,  0 }, // 05
  { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS,      0x0400,  0 }, // 06
  { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS,      0x0800,  0 }, // 07
  { MARIO_ANIM_TWIRL,            MARIO_HAND_FISTS,      0x1000,  0 }, // 08
  { MARIO_ANIM_TWIRL,            MARIO_HAND_FISTS,      0x2000,  0 }, // 09
  { MARIO_ANIM_TWIRL,            MARIO_HAND_FISTS,      0x4000,  0 }, // 10
  { MARIO_ANIM_TWIRL,            MARIO_HAND_FISTS,      0x6000,  0 }, // 11
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0x8000,  0 }, // 12
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0xA000,  0 }, // 13
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0xC000,  0 }, // 14
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0xE000,  0 }, // 15
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0x0000,  0 }, // 16
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0x2000,  0 }, // 17
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0x4000,  0 }, // 18
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0x6000,  0 }, // 19
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0x8000,  0 }, // 20
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0xA000,  0 }, // 21
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0xC000,  0 }, // 22
  { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,       0xE000,  0 }, // 23
  { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS,      0xF000,  0 }, // 24
  { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS,      0xF800,  0 }, // 25
  { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS,      0xFC00,  0 }, // 26
  { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS,      0xFE00,  0 }, // 27
  { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS,      0xFF00,  0 }, // 28
  { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS,      0x0000,  0 }, // 29
  { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS,      0x0000,  0 }, // 30
  { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS,      0x0000,  0 }, // 31
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS,      0x0000, 16 }, // 32
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS,      0x0000, 24 }, // 33
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS,      0x0000, 28 }, // 34
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS,      0x0000, 30 }, // 35
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 31 }, // 36
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 37
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 38
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 39
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 40
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 41
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 42
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 43
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 44
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 45
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 46
  { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, }, // 47
{ { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 00
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 01
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 02
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0080,  0 }, // 03
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 04
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 05
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 06
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 07
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 08
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 09
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 10
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 11
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 12
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 13
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 14
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 15
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 16
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 17
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 18
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 19
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 20
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 21
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 22
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 23
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 24
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_OPEN,       0x0000,  0 }, // 25
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 26
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 27
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 28
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 29
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 30
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 31
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 32
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 33
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 34
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 35
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 36
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  0 }, // 37
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000,  8 }, // 38
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_FISTS,      0x0000, 16 }, // 39
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 22 }, // 40
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 26 }, // 41
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 29 }, // 42
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 31 }, // 43
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 44
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 45
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 46
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, }, // 47
};

static bool omm_act_star_dance_update(struct MarioState *m) {

    // Enable time stop and spawn the celebration star
    if (m->actionTimer == 0) {
        m->faceAngle[1] = m->area->camera->yaw;
        vec3s_set(m->marioObj->oGfxAngle, 0, m->area->camera->yaw, 0);
        disable_background_sound();
        audio_play_course_clear();
        f32 radius =  60.f * m->marioObj->oScaleX;
        f32 height = 160.f * m->marioObj->oScaleY;
        omm_obj_spawn_star_celebration(m->marioObj, radius, height, m->interactObj->behavior);
        set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        m->marioObj->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        m->marioObj->oAnimID = -1;
        m->marioObj->oCurrAnim = NULL;
    }

    // Display the text box "You got a star"
    else if (m->actionTimer == 39) {
#if OMM_GAME_IS_SMSR
#define gLastCompletedStarNum (gLastCompletedStarNum * (obj_get_first_with_behavior(bhvOmmStarCelebration)->oCelebStarBehavior != bhvCustomSMSRStarReplica))
#endif
        ustr_t courseName, actName;
        omm_render_effect_you_got_a_star_begin(
            OMM_TEXT_YOU_GOT_A_STAR,
            omm_level_get_course_name(courseName, gCurrLevelNum, OMM_GAME_MODE, false, false),
            omm_level_get_act_name(actName, gCurrLevelNum, gLastCompletedStarNum, OMM_GAME_MODE, false, false)
        );
    }

    // Here we go!
    else if (m->actionTimer == 40) {
        SFX(SOUND_MARIO_HERE_WE_GO);
        set_camera_shake_from_hit(SHAKE_GROUND_POUND);
    }

    // Resume action
    else if (m->actionTimer == 80) {
        obj_deactivate_all_with_behavior(bhvOmmStarCelebration);
        m->marioObj->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
        clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        enable_background_sound();
        audio_stop_course_clear();
        omm_render_effect_you_got_a_star_end();
        omm_health_fully_heal_mario(m, true);
        m->healCounter = OMM_BREATH_REFILL;
        return true;
    }

    // Fix the camera during the animation
    omm_mario_lock_camera(m, true);

    // Animation
    const OmmStarDanceAnimFrame *frame = &OMM_STAR_DANCE_ANIM_FRAMES[OMM_EXTRAS_SMO_ANIMATIONS][min_s(m->actionTimer, 47)];
    ANM(frame->animID, 1.f);
    m->marioBodyState->handState = frame->handState;
    vec3f_set(m->marioObj->oGfxPos, m->pos[0], m->pos[1] + frame->yOffset, m->pos[2]);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1] - frame->yawOffset, 0);
    m->actionTimer++;
    return false;
}

static s32 omm_act_star_dance(struct MarioState *m) {
    if (omm_act_star_dance_update(m)) {

#if OMM_GAME_IS_SM64
        // WF Tower transition
        if (gCurrCourseNum == COURSE_WF && gLastCompletedStarNum == 1) {
            omm_obj_spawn_wf_transition(m->marioObj);
        }
#endif

        // Resume action
        mario_set_forward_vel(m, 0.f);
        omm_mario_set_action(m, m->actionArg ? ACT_WATER_IDLE : ACT_FREEFALL, 0, 0);
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_warping(struct MarioState *m) {
    m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    if (m->actionArg) {
        m->actionArg--;
        if (m->actionArg == 0 && m->usedObj) {
            s16 destLevel = m->usedObj->oWallWarpDestLevel;
            s16 destArea = m->usedObj->oWallWarpDestArea;
            s16 destNode = m->usedObj->oWallWarpDestNode;
            initiate_warp(destLevel, destArea, destNode, 0);
        }
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

#if OMM_GAME_IS_R96X

static s32 omm_act_milk_power_up_down(struct MarioState *m, bool powerUp) {
    if (m->actionState == 0) {
        mario_stop_riding_and_holding(m);
        omm_cappy_return_to_mario(omm_cappy_get_object());
        omm_mario_unset_cap(m);
        omm_peach_vibe_deactivate(m);
        if (powerUp) {
            omm_secrets_unlock(OMM_SECRET_R96_SECRET);
            play_sound(SOUND_MENU_EXIT_PIPE | 0xFF00, gGlobalSoundArgs);
            gOmmGlobals->milkTimer = 900; // 30 seconds
        } else {
            r96_stop_music();
            play_sound(SOUND_MENU_ENTER_PIPE | 0xFF00, gGlobalSoundArgs);
            gOmmGlobals->milkTimer = 0;
        }
        m->actionState = 1;
    }

    // Don't move
    mario_set_forward_vel(m, 0.f);
    m->vel[1] = 0.f;

    // End the action after 1 second
    if (m->actionTimer++ >= 30) {
        m->squishTimer = 0;
        m->floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &m->floor);
        m->pos[1] = max_f(m->pos[1], m->floorHeight);

        // Underwater
        if (m->pos[1] < m->waterLevel - 100) {
            return omm_mario_set_action(m, ACT_WATER_IDLE, 0, 0);
        }

        // On ground
        if (m->pos[1] <= m->floorHeight + 10) {
            return omm_mario_set_action(m, ACT_IDLE, 0, 0);
        }

        // Airborne
        return omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
    }

    return OMM_MARIO_ACTION_RESULT_BREAK;
}

#endif

//////////////
// Cutscene //
//////////////

s32 omm_mario_execute_cutscene_action(struct MarioState *m) {
    gOmmMario->wallSlide.jumped = false;
    gOmmMario->peach.floated = false;
    gOmmMario->cappy.bounced = false;
    gOmmMario->state.airCombo = 0;
    gOmmMario->midairSpin.counter = 0;

    // Actions
    switch (m->action) {

        // SM64
        case ACT_DISAPPEARED:               return omm_act_disappeared(m);
        case ACT_INTRO_CUTSCENE:            return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_STAR_DANCE_EXIT:           return omm_act_cutscene_cancels__star_dance(m);
        case ACT_STAR_DANCE_WATER:          return omm_act_cutscene_cancels__star_dance(m);
        case ACT_FALL_AFTER_STAR_GRAB:      return omm_act_cutscene_cancels__star_dance(m);
        case ACT_READING_AUTOMATIC_DIALOG:  return omm_act_reading_automatic_dialog(m);
        case ACT_READING_NPC_DIALOG:        return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_STAR_DANCE_NO_EXIT:        return omm_act_cutscene_cancels__star_dance(m);
        case ACT_READING_SIGN:              return omm_act_reading_sign(m);
        case ACT_JUMBO_STAR_CUTSCENE:       return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_WAITING_FOR_DIALOG:        return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_DEBUG_FREE_MOVE:           return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_STANDING_DEATH:            return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_QUICKSAND_DEATH:           return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_ELECTROCUTION:             return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_SUFFOCATION:               return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_DEATH_ON_STOMACH:          return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_DEATH_ON_BACK:             return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_EATEN_BY_BUBBA:            return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_END_PEACH_CUTSCENE:        return omm_sparkly_act_ending_1(m);
        case ACT_CREDITS_CUTSCENE:          return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_END_WAVING_CUTSCENE:       return omm_sparkly_act_ending_2(m);
        case ACT_PULLING_DOOR:              return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_PUSHING_DOOR:              return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_WARP_DOOR_SPAWN:           return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_EMERGE_FROM_PIPE:          return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_SPAWN_SPIN_AIRBORNE:       return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_SPAWN_SPIN_LANDING:        return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_EXIT_AIRBORNE:             return omm_act_exit(m, false, ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_GENERAL_FALL, -32, 15, 0);
        case ACT_EXIT_LAND_SAVE_DIALOG:     return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_DEATH_EXIT:                return omm_act_exit(m, true, ACT_DEATH_EXIT_LAND, MARIO_ANIM_GENERAL_FALL, -32, 15, SOUND_MARIO_OOOF2);
        case ACT_UNUSED_DEATH_EXIT:         return omm_act_exit(m, true, ACT_FREEFALL_LAND_STOP, MARIO_ANIM_GENERAL_FALL, 0, 0, SOUND_MARIO_OOOF2);
        case ACT_FALLING_DEATH_EXIT:        return omm_act_exit(m, true, ACT_DEATH_EXIT_LAND, MARIO_ANIM_GENERAL_FALL, 0, 0, SOUND_MARIO_OOOF2);
        case ACT_SPECIAL_EXIT_AIRBORNE:     return omm_act_exit(m, false, ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_SINGLE_JUMP, -24, 11, SOUND_MARIO_YAHOO);
        case ACT_SPECIAL_DEATH_EXIT:        return omm_act_exit(m, true, ACT_HARD_BACKWARD_GROUND_KB, MARIO_ANIM_BACKWARD_AIR_KB, -24, 11, 0);
        case ACT_FALLING_EXIT_AIRBORNE:     return omm_act_exit(m, false, ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_GENERAL_FALL, 0, 0, 0);
        case ACT_UNLOCKING_KEY_DOOR:        return omm_act_unlocking_key_door(m);
        case ACT_UNLOCKING_STAR_DOOR:       return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_ENTERING_STAR_DOOR:        return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_SPAWN_NO_SPIN_AIRBORNE:    return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_SPAWN_NO_SPIN_LANDING:     return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_BBH_ENTER_JUMP:            return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_BBH_ENTER_SPIN:            return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_TELEPORT_FADE_OUT:         return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_TELEPORT_FADE_IN:          return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_SHOCKED:                   return omm_act_shocked(m);
        case ACT_SQUISHED:                  return omm_act_squished(m);
        case ACT_HEAD_STUCK_IN_GROUND:      return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_BUTT_STUCK_IN_GROUND:      return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_FEET_STUCK_IN_GROUND:      return OMM_MARIO_ACTION_RESULT_CONTINUE;
        case ACT_PUTTING_ON_CAP:            return OMM_MARIO_ACTION_RESULT_CONTINUE;

        // Odyssey
        case ACT_OMM_POSSESSION:            return omm_act_possession(m);
        case ACT_OMM_POSSESSION_UNDERWATER: return omm_act_possession(m);
        case ACT_OMM_DEATH:                 return omm_act_death(m);
        case ACT_OMM_DEATH_WATER:           return omm_act_death_water(m);
        case ACT_OMM_DEATH_FALL:            return omm_act_death_fall(m);
        case ACT_OMM_DEATH_FROZEN:          return omm_act_death_frozen(m);
        case ACT_OMM_DEATH_EATEN_BY_BUBBA:  return omm_act_death_eaten_by_bubba(m);
        case ACT_OMM_DEATH_QUICKSAND:       return omm_act_death_quicksand(m);
        case ACT_OMM_DEATH_SQUISHED:        return omm_act_death_squished(m);
        case ACT_OMM_STAR_DANCE:            return omm_act_star_dance(m);
        case ACT_OMM_SPARKLY_STAR_DANCE:    return omm_sparkly_act_star_dance(m);
        case ACT_OMM_WARPING:               return omm_act_warping(m);
#if OMM_GAME_IS_R96X
        case ACT_OMM_MILK_POWER_UP:         return omm_act_milk_power_up_down(m, true);
        case ACT_OMM_MILK_POWER_DOWN:       return omm_act_milk_power_up_down(m, false);
#endif
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

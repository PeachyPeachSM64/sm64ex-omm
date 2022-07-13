#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Lost Coins
//

typedef union { struct Object *obj; Vec3f pos; } OmmLostCoin;
static OmmLostCoin sOmmLostCoins[20];
static s32 sOmmLostCoinsCount = 0;
static u8 sOmmCurrStarBits = 0;

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
            if (floor != NULL && coin->obj->oPosY <= floorY) {
                coin->obj->oPosY = floorY;
                coin->obj->oVelY *= -0.85f;
                coin->obj->oForwardVel *= 0.8f;
                if (floor->type == SURFACE_DEATH_PLANE) {
                    coin->obj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                }
                break;
            } else if (floor == NULL) {
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
    sOmmCurrStarBits = omm_stars_get_bits();
    for (s32 i = 0; i != sOmmLostCoinsCount; ++i) {
        struct Object *coin = sOmmLostCoins[i].obj;
        f32 x = coin->oPosX;
        f32 y = coin->oPosY;
        f32 z = coin->oPosZ;
        sOmmLostCoins[i].pos[0] = x;
        sOmmLostCoins[i].pos[1] = y;
        sOmmLostCoins[i].pos[2] = z;
    }
}

static void omm_lost_coins_respawn() {
    if (OMM_STARS_NON_STOP && !OMM_SSM_IS_LUNATIC) {
#if OMM_CODE_TIME_TRIALS
        if (!time_trials_enabled()) omm_stars_set_bits(sOmmCurrStarBits);
#else
        omm_stars_set_bits(sOmmCurrStarBits);
#endif
        struct MarioState *m = gMarioState;
        for (s32 i = 0; i != sOmmLostCoinsCount; ++i) {
            f32 x = sOmmLostCoins[i].pos[0];
            f32 y = sOmmLostCoins[i].pos[1];
            f32 z = sOmmLostCoins[i].pos[2];

            // Respawn coin only if not above OoB or death barrier
            struct Surface *floor = NULL;
            f32 floorHeight = find_floor(x, y, z, &floor);
            if (floor       == NULL ||
                floor->type == SURFACE_DEATH_PLANE ||
                floor->type == SURFACE_VERTICAL_WIND) {
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
    sOmmCurrStarBits = 0;
}

//
// Deaths
//

#define MARIO_ANIM_NONE 0
#define MARIO_ANIM_CURRENT -1

typedef struct {
    struct {
        s32 animID; // mAnm.animID if MARIO_ANIM_CURRENT
        f32 animAccel;
        s16 anglePitch; // mGfx.angle[0] if 0xFFFF
        f32 frameMin; // mAnm.animFrame if -1
        f32 frameMax; // mAnm.animFrame if -1
        f32 yOffset;
    } anim1;
    struct {
        s32 animID; // mAnm.animID if MARIO_ANIM_CURRENT
        f32 animAccel;
        s16 anglePitch; // mGfx.angle[0] if 0xFFFF
        f32 frameMin; // mAnm.animFrame if -1
        f32 frameMax; // mAnm.animFrame if -1
        f32 yVelInit; // m->vel[1] if -1
        f32 yVelDec;
        f32 yVelLimit;
    } anim2;
} OmmActDeathAnimParams;

static void omm_play_death_sound(struct MarioState *m, s32 type) {
    switch (type) {
        case 0: omm_sound_play(OMM_SOUND_EVENT_DEATH_MARIO, m->marioObj->oCameraToObject); break;
        case 1: omm_sound_play(OMM_SOUND_EVENT_DEATH_MARIO_WATER, m->marioObj->oCameraToObject); break;
        case 2: omm_sound_play(OMM_SOUND_EVENT_DEATH_MARIO_FALL, m->marioObj->oCameraToObject); break;
    }
}

#define mGfx m->marioObj->header.gfx
#define mAnm m->marioObj->oAnimInfo
static void omm_act_death_handler(struct MarioState *m, s32 type, bool lookAtCamera, u16 tAnim2, u16 tBowserFace, u16 tEnd, const OmmActDeathAnimParams *anims) {
    static f32 sMarioPosY;
    static f32 sAnim2Vel;

    // Params
    s32 anim1_animId     = (anims->anim1.animID == MARIO_ANIM_CURRENT ? mAnm.animID : anims->anim1.animID);
    f32 anim1_animAccel  = (anims->anim1.animAccel);
    s16 anim1_anglePitch = (anims->anim1.anglePitch == (s16) 0xFFFF ? mGfx.angle[0] : anims->anim1.anglePitch);
    f32 anim1_frameMin   = (anims->anim1.frameMin == -1 ? mAnm.animFrame : anims->anim1.frameMin);
    f32 anim1_frameMax   = (anims->anim1.frameMax == -1 ? mAnm.animFrame : anims->anim1.frameMax);
    f32 anim1_yOffset    = (anims->anim1.yOffset);
    s32 anim2_animId     = (anims->anim2.animID == MARIO_ANIM_CURRENT ? mAnm.animID : anims->anim2.animID);
    f32 anim2_animAccel  = (anims->anim2.animAccel);
    s16 anim2_anglePitch = (anims->anim2.anglePitch == (s16) 0xFFFF ? mGfx.angle[0] : anims->anim2.anglePitch);
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
    if (m->actionTimer == 0) {
        music_lower_volume(SEQ_PLAYER_LEVEL, 60, 40);
        omm_play_death_sound(m, type);
        if (lookAtCamera) {
            m->faceAngle[1] = m->area->camera->yaw;
            mGfx.angle[1] = m->area->camera->yaw;
        }
        m->health = OMM_HEALTH_ODYSSEY_DEAD;
        omm_lost_coins_spawn(m);
        sAnim2Vel = anim2_yVelInit;
        sMarioPosY = m->pos[1] + anim1_yOffset;
    }

    // Fix the camera during the animation
    omm_mario_lock_camera(m, false);

    // Mario progressively loses all of his coins
    omm_lost_coins_update();
    if (m->numCoins > 0) {
        m->numCoins--;
        gHudDisplay.coins = m->numCoins;
    }

    // Animations
    if (m->actionTimer < tAnim2) {
        obj_anim_play(m->marioObj, anim1_animId, anim1_animAccel);
        obj_anim_clamp_frame(m->marioObj, anim1_frameMin, anim1_frameMax);
        mGfx.angle[0] = anim1_anglePitch;
    } else {
        obj_anim_play(m->marioObj, anim2_animId, anim2_animAccel);
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
    // 30 frames long, black screen until frame 150
    if (m->actionTimer == tBowserFace) {
        play_transition(WARP_TRANSITION_FADE_INTO_BOWSER, 30, 0x00, 0x00, 0x00);
    }

    // Send the signal to restart the level
    if (m->actionTimer == tEnd) {
#if OMM_CODE_TIME_TRIALS
        if (time_trials_enabled()) {
            omm_restart_level();
        } else {
            omm_lost_coins_save();
            omm_add_routine(OMM_ROUTINE_TYPE_LEVEL_ENTRY, omm_lost_coins_respawn);
            omm_restart_area();
        }
#else
        omm_lost_coins_save();
        omm_add_routine(OMM_ROUTINE_TYPE_LEVEL_ENTRY, omm_lost_coins_respawn);
        omm_restart_area();
#endif

        // Restore Mario's health
        if (OMM_MOVESET_ODYSSEY) {
            m->health = OMM_HEALTH_ODYSSEY_DEAD;
            omm_health_fully_heal_mario(m);
            omm_health_set(m, m->health);
        } else {
            m->health = 0x880;
        }
    }

    m->actionTimer++;
}

static const OmmActDeathAnimParams *omm_act_death_get_anim_params(s32 deathType) {
    static OmmActDeathAnimParams sOmmActDeathAnimParams[2][7] = {
    { { { MARIO_ANIM_AIR_FORWARD_KB,     1.00f, 0xE000,  0,  0, 16          },
        { MARIO_ANIM_BACKWARDS_WATER_KB, 1.00f, 0x0000,  0, 20, 40, -2, -40 } },   // Default
      { { MARIO_ANIM_DROWNING_PART2,     1.00f, 0x0000,  0,  0, 16          },
        { MARIO_ANIM_BACKWARDS_WATER_KB, 0.75f, 0x0000,  0, 20, 20, -1, -20 } },   // Water
      { { MARIO_ANIM_BEING_GRABBED,      2.00f, 0x0000,  0, 30,  0          },
        { MARIO_ANIM_BEING_GRABBED,      2.00f, 0x0000,  0, 30, -1, -2, -90 } },   // Fall
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  0          },
        { MARIO_ANIM_CURRENT,            1.00f, 0xFFFF, -1, -1,  0,  0,   0 } },   // Frozen
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  0          },
        { MARIO_ANIM_CURRENT,            1.00f, 0xFFFF, -1, -1,  0,  0,   0 } },   // Eaten by Bubba
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
        { MARIO_ANIM_CURRENT,            1.00f, 0xFFFF, -1, -1,  0,  0,   0 } },   // Eaten by Bubba
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  8          },
        { MARIO_ANIM_DYING_IN_QUICKSAND, 1.00f, 0x0000,  0, 60,  0, -3,  -3 } },   // Quicksand
      { { MARIO_ANIM_NONE,               0.00f, 0x0000,  0,  0,  0          },
        { MARIO_ANIM_A_POSE,             1.00f, 0x0000,  0,  0,  0,  0,   0 } } }, // Squished
    };
    return &sOmmActDeathAnimParams[OMM_EXTRAS_SMO_ANIMATIONS][deathType];
}

static s32 omm_act_death(struct MarioState *m) {
    omm_act_death_handler(m, 0, true, 24, 84, 150, omm_act_death_get_anim_params(0));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_water(struct MarioState *m) {
    omm_act_death_handler(m, 1, true, 24, 84, 150, omm_act_death_get_anim_params(1));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_fall(struct MarioState *m) {
    omm_act_death_handler(m, 2, false, 0, 60, 120, omm_act_death_get_anim_params(2));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_frozen(struct MarioState *m) {
    omm_act_death_handler(m, 0, OMM_EXTRAS_SMO_ANIMATIONS, 0, 84, 150, omm_act_death_get_anim_params(3));
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_eaten_by_bubba(struct MarioState *m) {
    omm_act_death_handler(m, 0, false, 0, 84, 150, omm_act_death_get_anim_params(4));
    m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_quicksand(struct MarioState *m) {
    omm_act_death_handler(m, 2, false, 0, 60, 120, omm_act_death_get_anim_params(5));
    if (m->actionTimer < 75) play_sound(SOUND_MOVING_QUICKSAND_DEATH, m->marioObj->oCameraToObject);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_squished(struct MarioState *m) {
    omm_act_death_handler(m, 0, false, 0, 84, 150, omm_act_death_get_anim_params(6));
    m->squishTimer = 0xFF;
    mGfx.scale[0] = 2.00f;
    mGfx.scale[1] = 0.05f;
    mGfx.scale[2] = 2.00f;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_shocked(struct MarioState *m) {
    action_condition(OMM_MOVESET_ODYSSEY && m->actionArg == 0 && ++m->actionState > 45, ACT_FREEFALL, 0, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_disappeared(struct MarioState *m) {
    action_condition(m->prevAction == ACT_OMM_DEATH, ACT_OMM_DEATH_SQUISHED, 0, RETURN_CANCEL); // Squished
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

//
// Odyssey star dance
//

typedef struct {
    s32 animID;
    s32 handState;
    s16 yawOffset;
    f32 yOffset;
} OmmStarDanceAnimFrame;

static const OmmStarDanceAnimFrame sOmmStarDanceAnimFrames[2][48] = {
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
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 20 }, // 40
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 26 }, // 41
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 42
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 43
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 44
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 45
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, // 46
  { MARIO_ANIM_OMM_STARDANCE,    MARIO_HAND_PEACE_SIGN, 0x0000, 32 }, }, // 47
};

static bool omm_update_star_dance(struct MarioState *m) {
    static const BehaviorScript *starBehavior = NULL;
    static struct Object *celebStar = NULL;

    // Enable time stop and spawn the celebration star
    if (m->actionTimer == 0) {
        starBehavior = m->interactObj->behavior;
        m->faceAngle[1] = m->area->camera->yaw;
        vec3s_set(m->marioObj->oGfxAngle, 0, m->area->camera->yaw, 0);
        disable_background_sound();
        audio_play_course_clear();
        f32 radius =  60.f * m->marioObj->oScaleX;
        f32 height = 160.f * m->marioObj->oScaleY;
        celebStar = omm_spawn_star_celebration(m->marioObj, radius, height);
        set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        m->marioObj->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    }

    // Display the text box "You got a star"
    else if (m->actionTimer == 39) {
#if OMM_GAME_IS_SMSR
#define gLastCompletedStarNum (gLastCompletedStarNum * (starBehavior != bhvCustomSMSRStarReplica))
#endif
        omm_render_effect_you_got_a_star_begin(OMM_TEXT_YOU_GOT_A_STAR, omm_level_get_name(gCurrLevelNum, false, false), omm_level_get_act_name(gCurrLevelNum, gLastCompletedStarNum, false, false));
    }
    
    // Here we go!
    else if (m->actionTimer == 40) {
        play_sound(SOUND_MARIO_HERE_WE_GO, m->marioObj->oCameraToObject);
        set_camera_shake_from_hit(SHAKE_GROUND_POUND);
    }
    
    // Resume action
    else if (m->actionTimer == 80) {
        obj_mark_for_deletion(celebStar);
        m->marioObj->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
        clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        enable_background_sound();
        audio_stop_course_clear();
        omm_render_effect_you_got_a_star_end();
        omm_health_fully_heal_mario(m);
        m->healCounter = OMM_O2_REFILL;
        starBehavior = NULL;
        return true;
    }

    // Fix the camera during the animation
    omm_mario_lock_camera(m, true);

    // Animation
    const OmmStarDanceAnimFrame *frame = &sOmmStarDanceAnimFrames[OMM_EXTRAS_SMO_ANIMATIONS][min_s(m->actionTimer, 47)];
    obj_anim_play(m->marioObj, frame->animID, 1.f);
    m->marioBodyState->handState = frame->handState;
    m->marioObj->oGfxPos[1] = m->pos[1] + frame->yOffset;
    m->marioObj->oGfxAngle[1] = m->faceAngle[1] - frame->yawOffset;
    m->actionTimer++;
    return false;
}

//
// Star dances
//

static s32 omm_act_star_dance_exit(struct MarioState *m) {
    if (OMM_ALL_STARS) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    u32 actionArg = (m->prevAction & ACT_FLAG_METAL_WATER) || ((m->prevAction & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED);
    action_condition(OMM_STARS_NON_STOP, ACT_OMM_STAR_DANCE, actionArg, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_star_dance_no_exit(struct MarioState *m) {
    if (OMM_ALL_STARS) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    u32 actionArg = (m->prevAction & ACT_FLAG_METAL_WATER) || ((m->prevAction & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED);
    action_condition(OMM_STARS_NON_STOP, ACT_OMM_STAR_DANCE, actionArg, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_star_dance_water(struct MarioState *m) {
    if (OMM_ALL_STARS) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    u32 actionArg = (m->prevAction & ACT_FLAG_METAL_WATER) || ((m->prevAction & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED);
    action_condition(OMM_STARS_NON_STOP, ACT_OMM_STAR_DANCE, actionArg, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_fall_after_star_grab(struct MarioState *m) {
    if (OMM_ALL_STARS) return OMM_MARIO_ACTION_RESULT_CONTINUE;
    u32 actionArg = (m->prevAction & ACT_FLAG_METAL_WATER) || ((m->prevAction & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED);
    action_condition(OMM_STARS_NON_STOP, ACT_OMM_STAR_DANCE, actionArg, RETURN_CANCEL);
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_star_dance(struct MarioState *m) {
    if (omm_update_star_dance(m)) {

#if OMM_GAME_IS_SM64
        // WF Tower transition
        if (gCurrCourseNum == COURSE_WF && gLastCompletedStarNum == 1) {
            omm_mario_set_action(m, ACT_OMM_TRANSITION_WF_TOWER, 0, 0);
            return OMM_MARIO_ACTION_RESULT_CANCEL;
        }
#endif

        // Resume action
        mario_set_forward_vel(m, 0.f);
        omm_mario_set_action(m, m->actionArg ? ACT_WATER_IDLE : ACT_FREEFALL, 0, 0);
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

static s32 omm_act_transition_wf_tower(struct MarioState *m) {
    switch (m->actionTimer) {
        case 15:
            play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 30, 0xFF, 0xFF, 0xFF);
            break;

        case 45:
            play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
            struct Object *whompKingBoss = obj_get_first_with_behavior(bhvWhompKingBoss);
            if (whompKingBoss) {
                stop_background_music(SEQ_EVENT_BOSS);
                obj_mark_for_deletion(whompKingBoss);
            }
            break;

        case 75:
            m->pos[0] = +840;
            m->pos[1] = +3584;
            m->pos[2] = +1230;
            m->intendedYaw = 0xA000;
            m->faceAngle[1] = 0xA000;
            m->marioObj->oFaceAngleYaw = 0xA000;
            m->marioObj->oGfxAngle[1] = 0xA000;
            mario_set_forward_vel(m, 0.f);
            omm_mario_set_action(m, ACT_FREEFALL, 0, 0xFFFF);
            play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 30, 0xFF, 0xFF, 0xFF);
            break;
    }
    m->actionTimer++;
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

//
// Exits
//

static bool omm_mario_launch_until_land(struct MarioState *m, s32 endAction, s32 animID, f32 forwardVel, u16 delay) {
    if (m->actionTimer++ >= delay) {
        m->marioObj->oNodeFlags |= GRAPH_RENDER_ACTIVE;
        mario_set_forward_vel(m, forwardVel);
        obj_anim_play(m->marioObj, animID, 1.f);
        if (perform_air_step(m, 0) == AIR_STEP_LANDED) {
            omm_mario_set_action(m, endAction, 0, 0);
            return true;
        }
        return false;
    }
    m->marioObj->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
    return false;
}

static s32 omm_act_exit_airborne(struct MarioState *m) {
    if (omm_mario_launch_until_land(m, ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_GENERAL_FALL, -32.f, 15)) {
        omm_health_fully_heal_mario(m);
        m->healCounter = 31;
    }
    m->marioObj->oGfxAngle[1] += 0x8000;
    m->particleFlags |= PARTICLE_SPARKLES;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_falling_exit_airborne(struct MarioState *m) {
    if (omm_mario_launch_until_land(m, ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_GENERAL_FALL, 0.f, 0)) {
        omm_health_fully_heal_mario(m);
        m->healCounter = 31;
    }
    m->marioObj->oGfxAngle[1] += 0x8000;
    m->particleFlags |= PARTICLE_SPARKLES;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_special_exit_airborne(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_MARIO_YAHOO, MARIO_MARIO_SOUND_PLAYED);
    if (omm_mario_launch_until_land(m, ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_SINGLE_JUMP, -24.f, 11)) {
        omm_health_fully_heal_mario(m);
        m->healCounter = 31;
        m->actionArg = 1;
    }
    m->marioObj->oGfxAngle[1] += 0x8000;
    m->particleFlags |= PARTICLE_SPARKLES;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_death_exit(struct MarioState *m) {
    if (omm_mario_launch_until_land(m, ACT_DEATH_EXIT_LAND, MARIO_ANIM_GENERAL_FALL, -32.f, 15)) {
        play_sound(SOUND_MARIO_OOOF2, m->marioObj->oCameraToObject);
        omm_health_fully_heal_mario(m);
        m->healCounter = 31;
    } else {
        omm_health_set(m, OMM_HEALTH_1_HP);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_unused_death_exit(struct MarioState *m) {
    if (omm_mario_launch_until_land(m, ACT_FREEFALL_LAND_STOP, MARIO_ANIM_GENERAL_FALL, 0.0f, 0)) {
        play_sound(SOUND_MARIO_OOOF2, m->marioObj->oCameraToObject);
        omm_health_fully_heal_mario(m);
        m->healCounter = 31;
    } else {
        omm_health_set(m, OMM_HEALTH_1_HP);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_falling_death_exit(struct MarioState *m) {
    if (omm_mario_launch_until_land(m, ACT_DEATH_EXIT_LAND, MARIO_ANIM_GENERAL_FALL, 0.0f, 0)) {
        play_sound(SOUND_MARIO_OOOF2, m->marioObj->oCameraToObject);
        omm_health_fully_heal_mario(m);
        m->healCounter = 31;
    } else {
        omm_health_set(m, OMM_HEALTH_1_HP);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

static s32 omm_act_special_death_exit(struct MarioState *m) {
    if (omm_mario_launch_until_land(m, ACT_HARD_BACKWARD_GROUND_KB, MARIO_ANIM_BACKWARD_AIR_KB, -24.0f, 11)) {
        omm_health_fully_heal_mario(m);
        m->healCounter = 31;
    } else {
        omm_health_set(m, OMM_HEALTH_1_HP);
    }
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

//
// Other
//

static s32 omm_act_warping(struct MarioState *m) {
    m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

//
// Cutscene
//

s32 omm_mario_execute_cutscene_action(struct MarioState *m) {
    gOmmData->mario->wallSlide.jumped = false;
    gOmmData->mario->peach.floated = false;
    gOmmData->mario->cappy.bounced = false;
    gOmmData->mario->state.airCombo = 0;
    gOmmData->mario->midairSpin.counter = 0;

    // Actions
    switch (m->action) {
        case ACT_SHOCKED:                   return omm_act_shocked(m);
        case ACT_DISAPPEARED:               return omm_act_disappeared(m);
        case ACT_STAR_DANCE_EXIT:           return omm_act_star_dance_exit(m);
        case ACT_STAR_DANCE_NO_EXIT:        return omm_act_star_dance_no_exit(m);
        case ACT_STAR_DANCE_WATER:          return omm_act_star_dance_water(m);
        case ACT_FALL_AFTER_STAR_GRAB:      return omm_act_fall_after_star_grab(m);
        case ACT_EXIT_AIRBORNE:             return omm_act_exit_airborne(m);
        case ACT_FALLING_EXIT_AIRBORNE:     return omm_act_falling_exit_airborne(m);
        case ACT_SPECIAL_EXIT_AIRBORNE:     return omm_act_special_exit_airborne(m);
        case ACT_DEATH_EXIT:                return omm_act_death_exit(m);
        case ACT_UNUSED_DEATH_EXIT:         return omm_act_unused_death_exit(m);
        case ACT_FALLING_DEATH_EXIT:        return omm_act_falling_death_exit(m);
        case ACT_SPECIAL_DEATH_EXIT:        return omm_act_special_death_exit(m);
        case ACT_END_PEACH_CUTSCENE:        return omm_ssa_end_cutscene_1(m);
        case ACT_END_WAVING_CUTSCENE:       return omm_ssa_end_cutscene_2(m);

        case ACT_OMM_POSSESSION:            return omm_act_possession(m);
        case ACT_OMM_DEATH:                 return omm_act_death(m);
        case ACT_OMM_DEATH_WATER:           return omm_act_death_water(m);
        case ACT_OMM_DEATH_FALL:            return omm_act_death_fall(m);
        case ACT_OMM_DEATH_FROZEN:          return omm_act_death_frozen(m);
        case ACT_OMM_DEATH_EATEN_BY_BUBBA:  return omm_act_death_eaten_by_bubba(m);
        case ACT_OMM_DEATH_QUICKSAND:       return omm_act_death_quicksand(m);
        case ACT_OMM_DEATH_SQUISHED:        return omm_act_death_squished(m);
        case ACT_OMM_STAR_DANCE:            return omm_act_star_dance(m);
        case ACT_OMM_SPARKLY_STAR_DANCE:    return omm_ssa_star_dance(m);
        case ACT_OMM_TRANSITION_WF_TOWER:   return omm_act_transition_wf_tower(m);
        case ACT_OMM_WARPING:               return omm_act_warping(m);
    }

    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Warp Pipes
//

struct WarpPipeInfo {
    u8 level;
    u8 area;
    u8 warpId;
    Vec3f pipePos;
    Vec3s pipeAngle;
    bool exitOnly;
};

struct WarpPipeInfo _WarpPipeInfo(u8 level, u8 area, u8 warpId, f32 pipePosX, f32 pipePosY, f32 pipePosZ, s16 pipeAngleP, s16 pipeAngleY, s16 pipeAngleR, bool exitOnly) {
    struct WarpPipeInfo wpi = { level, area, warpId, { pipePosX, pipePosY, pipePosZ }, { pipeAngleP, pipeAngleY, pipeAngleR }, exitOnly };
    return wpi;
}

static struct SpawnInfo *get_warp_pipe_spawn_info(struct WarpPipeInfo *wpi) {
    static OmmArray sWarpPipeSpawnInfo = omm_array_zero;
    omm_array_for_each(sWarpPipeSpawnInfo, p) {
        struct SpawnInfo *spawnInfo = (struct SpawnInfo *) p->as_ptr;
        if (spawnInfo->behaviorArg   == ((u32) (wpi->warpId << 16u) | (u32) (wpi->exitOnly << 8u)) &&
            spawnInfo->areaIndex     == wpi->area &&
            spawnInfo->startPos[0]   == wpi->pipePos[0] &&
            spawnInfo->startPos[1]   == wpi->pipePos[1] &&
            spawnInfo->startPos[2]   == wpi->pipePos[2] &&
            spawnInfo->startAngle[0] == wpi->pipeAngle[0] &&
            spawnInfo->startAngle[1] == wpi->pipeAngle[1] &&
            spawnInfo->startAngle[2] == wpi->pipeAngle[2]) {
            return spawnInfo;
        }
    }
    
    struct SpawnInfo *spawnInfo = omm_new(struct SpawnInfo, 1);
    spawnInfo->startPos[0]      = wpi->pipePos[0];
    spawnInfo->startPos[1]      = wpi->pipePos[1];
    spawnInfo->startPos[2]      = wpi->pipePos[2];
    spawnInfo->startAngle[0]    = wpi->pipeAngle[0];
    spawnInfo->startAngle[1]    = wpi->pipeAngle[1];
    spawnInfo->startAngle[2]    = wpi->pipeAngle[2];
    spawnInfo->areaIndex        = wpi->area;
    spawnInfo->activeAreaIndex  = wpi->area;
    spawnInfo->behaviorArg      = ((u32) (wpi->warpId << 16u) | (u32) (wpi->exitOnly << 8u));
    spawnInfo->behaviorScript   = (void *) bhvWarpPipe;
    spawnInfo->mModel           = geo_layout_to_graph_node(NULL, warp_pipe_geo);
    omm_array_add(sWarpPipeSpawnInfo, ptr, spawnInfo);
    return spawnInfo;
}

static struct Object *load_warp_pipe_spawn_info(struct WarpPipeInfo *wpi) {
    struct SpawnInfo *spawnInfo = get_warp_pipe_spawn_info(wpi);
    for (struct SpawnInfo *p = gAreas[wpi->area].objectSpawnInfos; p;) {
        if (p == spawnInfo) {
            return NULL;
        }
        p = p->next;
    }

    spawnInfo->next = gAreas[wpi->area].objectSpawnInfos;
    gAreas[wpi->area].objectSpawnInfos = spawnInfo;
    if (wpi->area == gCurrAreaIndex) {
        struct Object *pipe    = obj_spawn_from_geo(gMarioObject, warp_pipe_geo, bhvWarpPipe);
        pipe->oPosX            = spawnInfo->startPos[0];
        pipe->oPosY            = spawnInfo->startPos[1];
        pipe->oPosZ            = spawnInfo->startPos[2];
        pipe->oFaceAnglePitch  = spawnInfo->startAngle[0];
        pipe->oFaceAngleYaw    = spawnInfo->startAngle[1];
        pipe->oFaceAngleRoll   = spawnInfo->startAngle[2];
        pipe->oMoveAnglePitch  = spawnInfo->startAngle[0];
        pipe->oMoveAngleYaw    = spawnInfo->startAngle[1];
        pipe->oMoveAngleRoll   = spawnInfo->startAngle[2];
        pipe->oBhvArgs         = spawnInfo->behaviorArg;
        pipe->oIntangibleTimer = 0;
        pipe->oInteractStatus  = 0;
        return pipe;
    }
    return NULL;
}

static struct ObjectWarpNode *get_warp_pipe_warp_node(struct WarpPipeInfo *wpiFrom, struct WarpPipeInfo *wpiTo, struct Object *pipe) {
    static OmmArray sWarpPipeWarpNodes = omm_array_zero;
    omm_array_for_each(sWarpPipeWarpNodes, p) {
        struct ObjectWarpNode *warpNode = (struct ObjectWarpNode *) p->as_ptr;
        if (warpNode->node.id        == wpiFrom->warpId &&
            warpNode->node.destLevel == wpiTo->level    &&
            warpNode->node.destArea  == wpiTo->area     &&
            warpNode->node.destNode  == wpiTo->warpId) {
            return warpNode;
        }
    }

    struct ObjectWarpNode *warpNode = omm_new(struct ObjectWarpNode, 1);
    warpNode->node.id        = wpiFrom->warpId;
    warpNode->node.destLevel = wpiTo->level;
    warpNode->node.destArea  = wpiTo->area;
    warpNode->node.destNode  = wpiTo->warpId;
    warpNode->object         = pipe;
    omm_array_add(sWarpPipeWarpNodes, ptr, warpNode);
    return warpNode;
}

static void load_warp_pipe_warp_node(struct WarpPipeInfo *wpiFrom, struct WarpPipeInfo *wpiTo, struct Object *pipe) {
    struct ObjectWarpNode *warpNode = get_warp_pipe_warp_node(wpiFrom, wpiTo, pipe);
    for (struct ObjectWarpNode *p = gAreas[wpiFrom->area].warpNodes; p;) {
        if (p == warpNode) {
            return;
        }
        p = p->next;
    }

    warpNode->next = gAreas[wpiFrom->area].warpNodes;
    gAreas[wpiFrom->area].warpNodes = warpNode;
}

static void load_warp_pipe(struct WarpPipeInfo wpi1, struct WarpPipeInfo wpi2) {
    if (gCurrLevelNum == wpi1.level) {
        struct Object *pipe1 = load_warp_pipe_spawn_info(&wpi1);
        load_warp_pipe_warp_node(&wpi1, &wpi2, pipe1);
        if (gCurrLevelNum == wpi2.level) {
            struct Object *pipe2 = load_warp_pipe_spawn_info(&wpi2);
            load_warp_pipe_warp_node(&wpi2, &wpi1, pipe2);
        }
    }
}

static void omm_update_warp_pipes(struct MarioState *m) {
    static s32 sWarpPipeTimer = 20;

    // Load
#if OMM_GAME_IS_SM64
    if (OMM_STARS_NON_STOP) {
    load_warp_pipe(
        _WarpPipeInfo(LEVEL_JRB, 1, 0x32,  4900,  1440,  2500, 0x0000, 0x0000, 0x0000, false),
        _WarpPipeInfo(LEVEL_JRB, 2, 0x32,     0,  -351, -1100, 0x0000, 0x0000, 0x0000, false)
    );
    }
    load_warp_pipe(
        _WarpPipeInfo(LEVEL_LLL, 1, 0x32,  2040,   307,  3760, 0x0000, 0x8000, 0x0000,  true),
        _WarpPipeInfo(LEVEL_LLL, 2, 0x32, -1560,    90,   620, 0x0000, 0x0000, 0x0000, false)
    );
    load_warp_pipe(
        _WarpPipeInfo(LEVEL_SSL, 1, 0x32, -2040,   256,   714, 0x0000, 0x0000, 0x0000,  true),
        _WarpPipeInfo(LEVEL_SSL, 2, 0x32,     0,     0,  2880, 0x0000, 0x0000, 0x0000, false)
    );
    load_warp_pipe(
        _WarpPipeInfo(LEVEL_DDD, 2, 0x33,  6160,   110,  4430, 0x0000, 0xA000, 0x0000,  true),
        _WarpPipeInfo(LEVEL_DDD, 2, 0x32,  2030, -4088, -2780, 0x0000, 0x0000, 0x0000, false)
    );
    load_warp_pipe(
        _WarpPipeInfo(LEVEL_WDW, 1, 0x32, -3450,  3584, -3450, 0x0000, 0x2000, 0x0000,  true),
        _WarpPipeInfo(LEVEL_WDW, 2, 0x32,  -767,  -332,  1792, 0x0000, 0x0000, 0x0000, false)
    );
    if (OMM_STARS_NON_STOP) {
    load_warp_pipe(
        _WarpPipeInfo(LEVEL_THI, 1, 0x35,  2590,  3226, -2085, 0x0000, 0xC000, 0x0000,  true),
        _WarpPipeInfo(LEVEL_THI, 3, 0x35,     0,  2560,     0, 0x0000, 0x0000, 0x0000, false)
    );
    }
    load_warp_pipe(
        _WarpPipeInfo(LEVEL_PSS, 1, 0x3F, -7688, -4800,  6040, 0xC000, 0x4000, 0x0000, false),
        _WarpPipeInfo(0x1A,      4, 0x0A,     0,     0,     0, 0x0000, 0x0000, 0x0000,  true)
    );
#endif

    // Update
    sWarpPipeTimer = (sWarpPipeTimer + 1) * (m->action != ACT_EMERGE_FROM_PIPE);
    for_each_object_with_behavior(warpPipe, bhvWarpPipe) {
        warpPipe->oScaleX = 1.f;
        warpPipe->oScaleY = max_f(0.f, 1.f - ((warpPipe->oBhvArgs >> 8u) & 1) * (sWarpPipeTimer / 20.f));
        warpPipe->oScaleZ = 1.f;
        warpPipe->hitboxRadius = warpPipe->hitboxRadius * (warpPipe->oScaleY == 1.f);
        warpPipe->hitboxHeight = warpPipe->hitboxHeight * (warpPipe->oScaleY == 1.f);
        warpPipe->hitboxDownOffset = warpPipe->hitboxHeight * relerp_0_1_f(coss(warpPipe->oFaceAnglePitch) * coss(warpPipe->oFaceAngleRoll), -1.f, 1.f, 1.f, 0.f);
        obj_set_dormant(warpPipe, warpPipe->oScaleY <= 0.f);
    }
}

//
// Worlds
//

static s32 gPrevCourseNum = -1;
static bool sOmmWorldFrozen = false;
static bool sOmmWorldFlooded = false;
static bool sOmmWorldShadow = false;

#define omm_world_behavior_set_dormant(bhv, value)                          { for_each_object_with_behavior(obj, bhv) { obj_set_dormant(obj, value); } }
#define omm_world_behavior_set_dormant_cond(bhv, cond, value)               { for_each_object_with_behavior(obj, bhv) { if (cond) { obj_set_dormant(obj, value); } } }
#define omm_world_behavior_set_dormant_params(bhv, params, value)           { for_each_object_with_behavior(obj, bhv) { if (obj->oBhvArgs == params) { obj_set_dormant(obj, value); } } }
#define omm_world_non_stop_behavior_set_dormant(bhv, value)                 { if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) { omm_world_behavior_set_dormant(bhv, value); } }
#define omm_world_non_stop_behavior_set_dormant_cond(bhv, cond, value)      { if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) { omm_world_behavior_set_dormant_cond(bhv, cond, value); } }
#define omm_world_non_stop_behavior_set_dormant_params(bhv, params, value)  { if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) { omm_world_behavior_set_dormant_params(bhv, params, value); } }

static void omm_update_worlds(struct MarioState *m) {
    switch (gCurrCourseNum) {
#if OMM_GAME_IS_SMSR

        // Bob-omb Islands
        // Non-Stop mode:
        // - Hide bob-omb buddies post King Bob-omb
        case COURSE_BOB: {
            omm_world_non_stop_behavior_set_dormant_params(bhvBobombBuddy, 0x009E0000, true);
        } break;

        // Sky Land Resort
        // Non-Stop mode:
        // - Hide bob-omb buddies post King Whomp
        case COURSE_WF: {
            omm_world_non_stop_behavior_set_dormant_params(bhvBobombBuddy, 0x009F0000, true);
        } break;

        // Piranha Plant Pond
        case COURSE_JRB: {
        } break;

        // Chuckya Harbor
        // Non-Stop mode:
        // - Remove the stone block before the entrance of the underwater maze
        // - Add two Vanish cap boxes to collect the 6th star
        case COURSE_CCM: {
            struct Object *box = obj_get_first_with_behavior_and_field_f32(bhvPushableMetalBox, 0x06, 3968.f);
            if (box && !obj_is_dormant(box)) {
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_EXCLAMATION_BOX, bhvExclamationBox,  3800, 3800, -2900, 0, 0xC000, 0)->oBhvArgs2ndByte = 0x02;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_EXCLAMATION_BOX, bhvExclamationBox, -4200, 1720, -4300, 0, 0xE93E, 0)->oBhvArgs2ndByte = 0x02;
                obj_set_dormant(box, true);
            }
        } break;

        // Gloomy Garden
        // Non-Stop mode:
        // - Remove the inverted layer of wave platforms
        case COURSE_BBH: {
            omm_world_non_stop_behavior_set_dormant(bhvCustomSMSRRedWavePlatform, obj->oFaceAnglePitch & 0xFFFF);
        } break;

        // Colorful Coral Caverns
        case COURSE_HMC: {
        } break;

        // Koopa Canyon
        // Non-Stop mode:
        // - Remove the metal boxes blocking the entrance of the pyramid
        case COURSE_LLL: {
            omm_world_non_stop_behavior_set_dormant(bhvPushableMetalBox, obj->oFaceAngleRoll & 0xFFFF);
        } break;

        // Large Leaf Forest
        // Non-Stop mode:
        // - Hide mips post Wiggler
        case COURSE_SSL: {
            omm_world_non_stop_behavior_set_dormant_params(bhvCustomSMSRMipsMessage, 0x008B0000, true);
        } break;

        // Mad Musical Mess
        case COURSE_DDD: {
        } break;

        // Melting Snow Peaks
        case COURSE_SL: {
        } break;

        // Colossal Candy Clutter
        // Non-Stop mode:
        // - Hide the penguin at the end of the race
        case COURSE_WDW: {
            omm_world_non_stop_behavior_set_dormant(bhvSLWalkingPenguin, true);
        } break;

        // Cloudrail Station
        case COURSE_TTM: {
        } break;

        // Fatal Flame Falls
        case COURSE_THI: {
        } break;

        // Bob-omb Battle Factory
        // Non-Stop mode:
        // - Move the 6th star (The Engine Leak) to another place
        case COURSE_TTC: {
            struct Object *box = obj_get_first_with_behavior_and_field_s32(bhvExclamationBox, 0x40, 0x050E0000);
            if (box && !obj_is_dormant(box)) {
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_HMC_ROLLING_ROCK, bhvCustomSMSRToxicWastePlatform, 4679, -1100, -6850, 0x200, 0, -0x300);
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_HMC_ROLLING_ROCK, bhvCustomSMSRToxicWastePlatform, 4679, -1050, -7200, -0x400, 0, 0x580);
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_EXCLAMATION_BOX, bhvExclamationBox, 4679, -600, -7200, 0, 0, 0)->oBhvArgs2ndByte = 0x0E;
                obj_set_dormant(box, true);
            }
        } break;

        // Starlight Runway
        case COURSE_RR: {
        } break;

        // Star Leap Tower Grounds (Ending)
        case COURSE_CAKE_END: {
        } break;

#elif OMM_GAME_IS_SMMS

        // Sweet Sweet Rush
        // - Add more coins to be able to get the 100 coins star
        case COURSE_LLL: {
            struct Object *box3coins = obj_get_first_with_behavior_and_field_s32(bhvExclamationBox, 0x2F, 0x05);
            if (box3coins && !obj_is_dormant(box3coins)) {
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_EXCLAMATION_BOX, bhvExclamationBox, -2136,  993, -2347, 0, 0x0000, 0)->oBhvArgs2ndByte = 0x06;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,  -5391, -979,  3167, 0, 0x0000, 0)->oBhvArgs2ndByte = COIN_FORMATION_FLAG_RING;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,  -6900,  570,  1400, 0, 0x0000, 0)->oBhvArgs2ndByte = COIN_FORMATION_FLAG_RING;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,   4444, 5480, -4444, 0, 0x0000, 0)->oBhvArgs2ndByte = COIN_FORMATION_FLAG_RING;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,   -380,  570,  4200, 0, 0x0000, 0)->oBhvArgs2ndByte = COIN_FORMATION_FLAG_RING;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,   4600,  270,  4060, 0, 0x0000, 0)->oBhvArgs2ndByte = 0;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,  -6090,  625, -6300, 0, 0x4000, 0)->oBhvArgs2ndByte = 0;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,  -2730, 1000, -6400, 0, 0x0000, 0)->oBhvArgs2ndByte = COIN_FORMATION_FLAG_FLYING;
                obj_set_dormant(box3coins, true);
            }
        } break;

#elif OMM_GAME_IS_SM64

        // Bob-omb Battlefield
        // Non-Stop mode:
        // - Remove one bowling ball spawner
        // - Hide water bomb cannons
        // - Hide bob-omb buddies post King Bob-omb
        case COURSE_BOB: {
            omm_world_non_stop_behavior_set_dormant(bhvTtmBowlingBallSpawner, true);
            omm_world_non_stop_behavior_set_dormant(bhvWaterBombCannon, true);
            omm_world_non_stop_behavior_set_dormant_params(bhvBobombBuddy, 0x00020000, true);
            omm_world_non_stop_behavior_set_dormant_params(bhvBobombBuddy, 0x00030000, true);
        } break;

        // Whomp's Fortress
        // Non-Stop mode:
        // - Hide all objects related to the tower until Whomp King is defeated
        // - Hide the 6th star until the right wall is broken
        case COURSE_WF: {
            bool wkb = (obj_get_first_with_behavior(bhvWhompKingBoss) != NULL);
            bool bwr = (obj_get_first_with_behavior(bhvWfBreakableWallRight) != NULL);
            omm_world_non_stop_behavior_set_dormant(bhvKickableBoard, wkb);
            omm_world_non_stop_behavior_set_dormant(bhv1Up, wkb);
            omm_world_non_stop_behavior_set_dormant(bhvBulletBill, wkb);
            omm_world_non_stop_behavior_set_dormant(bhvTower, wkb);
            omm_world_non_stop_behavior_set_dormant(bhvBulletBillCannon, wkb);
            omm_world_non_stop_behavior_set_dormant(bhvTowerPlatformGroup, wkb);
            omm_world_non_stop_behavior_set_dormant(bhvTowerDoor, wkb);
            omm_world_non_stop_behavior_set_dormant(bhvWfSolidTowerPlatform, wkb);
            omm_world_non_stop_behavior_set_dormant(bhvWfSlidingTowerPlatform, wkb);
            omm_world_non_stop_behavior_set_dormant(bhvWfElevatorTowerPlatform, wkb);
            omm_world_non_stop_behavior_set_dormant_params(bhvStar, 0x01000000, wkb);
            omm_world_non_stop_behavior_set_dormant_params(bhvStar, 0x05000000, bwr);
        } break;

        // Jolly Roger Bay
        // Flooded variant, from exiting DDD or WDW, then entering JRB (120+ stars only):
        // - The entire level is underwater
        // - Hide all sunken ship parts, the buddies and their cannon, the goomba triplet and Unagi
        // Non-Stop mode:
        // - Hide some specific parts of the ship, depending on its state
        // - Keep only the Unagi with a star
        // - Update the whirpools
        case COURSE_JRB: {
            omm_world_non_stop_behavior_set_dormant_params(bhvUnagi, 0x00000000, true);
            omm_world_non_stop_behavior_set_dormant_params(bhvUnagi, 0x02020000, true);
            if (m->numStars >= 120 && (gPrevCourseNum == COURSE_DDD || gPrevCourseNum == COURSE_WDW) && gCurrAreaIndex == 1) {
                omm_world_behavior_set_dormant(bhvSunkenShipPart, true);
                omm_world_behavior_set_dormant(bhvSunkenShipPart2, true);
                omm_world_behavior_set_dormant(bhvInSunkenShip, true);
                omm_world_behavior_set_dormant(bhvInSunkenShip2, true);
                omm_world_behavior_set_dormant(bhvShipPart3, true);
                omm_world_behavior_set_dormant(bhvInSunkenShip3, true);
                omm_world_behavior_set_dormant(bhvJrbSlidingBox, true);
                omm_world_behavior_set_dormant(bhvCannon, true);
                omm_world_behavior_set_dormant(bhvWarpPipe, true);
                omm_world_behavior_set_dormant(bhvGoomba, true);
                omm_world_behavior_set_dormant(bhvGoombaTripletSpawner, true);
                omm_world_behavior_set_dormant(bhvBobombBuddy, true);
                omm_world_behavior_set_dormant(bhvBobombBuddyOpensCannon, true);
                sOmmWorldFlooded = true;
            } else {
                bool sunken = (gCurrAreaIndex == 1 && m->action != ACT_EMERGE_FROM_PIPE && !obj_is_dormant(obj_get_first_with_behavior(bhvInSunkenShip))) ||
                              (gCurrAreaIndex != 1 && gEnvironmentRegions[6] > -335);
                omm_world_non_stop_behavior_set_dormant(bhvInSunkenShip, !sunken);
                omm_world_non_stop_behavior_set_dormant(bhvSunkenShipPart, !sunken);
                omm_world_non_stop_behavior_set_dormant(bhvSunkenShipPart2, !sunken);
                omm_world_non_stop_behavior_set_dormant(bhvInSunkenShip, !sunken);
                omm_world_non_stop_behavior_set_dormant(bhvInSunkenShip2, !sunken);
                omm_world_non_stop_behavior_set_dormant(bhvShipPart3, sunken);
                omm_world_non_stop_behavior_set_dormant(bhvInSunkenShip3, sunken);
                omm_world_non_stop_behavior_set_dormant(bhvJrbSlidingBox, sunken);
                omm_world_non_stop_behavior_set_dormant(bhvJetStream, sunken);
                omm_world_non_stop_behavior_set_dormant(bhvWarpPipe, sunken);
                omm_world_non_stop_behavior_set_dormant_params(bhvStar, 0x05000000, sunken);
            }
            if (OMM_STARS_NON_STOP) {
                static struct Whirlpool sJrbWhirlpool;
                struct Object *jetstream = obj_get_first_with_behavior(bhvJetStream);
                if (jetstream && !obj_is_dormant(jetstream)) {
                    gAreas[gCurrAreaIndex].whirlpools[0] = &sJrbWhirlpool;
                    gAreas[gCurrAreaIndex].whirlpools[0]->pos[0] = jetstream->oPosX;
                    gAreas[gCurrAreaIndex].whirlpools[0]->pos[1] = jetstream->oPosY;
                    gAreas[gCurrAreaIndex].whirlpools[0]->pos[2] = jetstream->oPosZ;
                    gAreas[gCurrAreaIndex].whirlpools[0]->strength = -30;
                } else {
                    gAreas[gCurrAreaIndex].whirlpools[0] = NULL;
                }
            }
        } break;

        // Cool, Cool Mountain
        case COURSE_CCM: {
        } break;

        // Big Boo's Haunt
        // Non-Stop mode:
        // - Hide the main room stairs
        // - Hide the regular Boos (replaced by Ghost Hunt Boos)
        case COURSE_BBH: {
            omm_world_non_stop_behavior_set_dormant(bhvHiddenStaircaseStep, true);
            omm_world_non_stop_behavior_set_dormant(bhvBoo, true);
        } break;

        // Hazy Maze Cave
        case COURSE_HMC: {
        } break;

        // Lethal Lava Land
        case COURSE_LLL: {
        } break;

        // Shifting Sand Land
        // Non-Stop mode:
        // - Hide the late stars tweesters
        case COURSE_SSL: {
            omm_world_non_stop_behavior_set_dormant_params(bhvTweester, 0x00190000, true);
        } break;

        // Dire Dire Docks
        case COURSE_DDD: {
        } break;

        // Snowman's Land
        case COURSE_SL: {
        } break;

        // Wet-Dry World
        case COURSE_WDW: {
        } break;

        // Tall, Tall Mountain
        case COURSE_TTM: {
        } break;

        // Tiny-Huge Island
        // Non-Stop mode:
        // - Hide the warp pipe inside Wiggler's room until Wiggler is defeated
        case COURSE_THI: {
            omm_world_non_stop_behavior_set_dormant_cond(bhvWarpPipe, gCurrAreaIndex == 3, obj_get_first_with_behavior(bhvWigglerHead) != NULL);
        } break;

        // Tick Tock Clock
        case COURSE_TTC: {
        } break;

        // Rainbow Ride
        case COURSE_RR: {
        } break;

        // Bowser in the Sky
        // Shadow variant, from exiting BBH or BITDW, then entering BITS (120+ stars only):
        // - The level is engulfed by darkness: only objects are visible, everything else is pure black
        case COURSE_BITS: {
            if (gCurrLevelNum == LEVEL_BITS && m->numStars >= 120 && (gPrevCourseNum == COURSE_BBH || gPrevCourseNum == COURSE_BITDW)) {
                sOmmWorldShadow = true;
            }
        } break;

        // Secret Aquarium
        // Frozen variant, from exiting CCM or SL, then entering SA (120+ stars only):
        // - Turn the water into cold water
        // - Hide the fish groups
        case COURSE_SA: {
            if (m->numStars >= 120 && (gPrevCourseNum == COURSE_CCM || gPrevCourseNum == COURSE_SL)) {
                omm_world_behavior_set_dormant(bhvLargeFishGroup, true);
                sOmmWorldFrozen = true;
            }
        } break;

        // Castle
        case COURSE_NONE: {
            switch (LEVEL_AREA_INDEX(gCurrLevelNum, gCurrAreaIndex)) {

                // Inside
                // - Spawn a sign about level variants near JRB after 120 stars
                case AREA_CASTLE_LOBBY: {
                    if (m->numStars >= 120 && !obj_get_first_with_behavior_and_field_s32(bhvMessagePanel, 0x2F, OMM_DIALOG_LEVEL_VARIANTS) && omm_dialog_get_entry(NULL, OMM_DIALOG_LEVEL_VARIANTS, gOmmSparklyMode)) {
                        struct Object *sign = obj_spawn_from_geo(gMarioObject, wooden_signpost_geo, bhvMessagePanel);
                        obj_set_pos(sign, 4100, 307, 280);
                        obj_set_angle(sign, 0, 0xC000, 0);
                        obj_drop_to_floor(sign);
                        sign->oBhvArgs = (OMM_DIALOG_LEVEL_VARIANTS << 16);
                        sign->oBhvArgs2ndByte = OMM_DIALOG_LEVEL_VARIANTS;
                    }
                } break;

                // Upstairs
                // Odyssey Moveset only:
                // - Truly infinite stairs if less than 70 stars
                case AREA_CASTLE_TIPPY: {
                    bool trulyInfiniteStairs = (OMM_MOVESET_ODYSSEY && m->numStars < 70);
                    omm_world_behavior_set_dormant_params(bhvWarp, 0x0F0B0000, trulyInfiniteStairs);
                    if (trulyInfiniteStairs && m->floor->room == 6 && m->pos[2] < 800) {

                        // Displacement
                        f32 dz = 410;
                        f32 dy = (
                            find_floor_height(m->pos[0], m->pos[1] + 80, m->pos[2] + dz) -
                            find_floor_height(m->pos[0], m->pos[1] + 80, m->pos[2])
                        );

                        // Warp Mario and the camera
                        m->pos[1] += dy;
                        m->pos[2] += dz;
                        m->marioObj->oPosY += dy;
                        m->marioObj->oPosZ += dz;
                        m->marioObj->oGfxPos[1] += dy;
                        m->marioObj->oGfxPos[2] += dz;
                        omm_camera_warp(m->area->camera, 0, dy, dz);
                    }
                } break;
            }
        } break;

#else

        default: {
            OMM_UNUSED(m);
        } break;

#endif
    }

    // Set recovery hearts dormant in Sparkly Stars Lunatic mode
    omm_world_behavior_set_dormant(bhvRecoveryHeart, OMM_SPARKLY_MODE_IS_LUNATIC);
#if OMM_GAME_IS_SMSR
    omm_world_behavior_set_dormant(bhvCustomSMSRRecoveryBubbleWater, OMM_SPARKLY_MODE_IS_LUNATIC);
#endif
    
#if !OMM_GAME_IS_R96X
    // Disable Bowser objects outside of Bowser fights
    if (gCurrLevelNum != LEVEL_BOWSER_1 &&
        gCurrLevelNum != LEVEL_BOWSER_2 &&
        gCurrLevelNum != LEVEL_BOWSER_3 &&
        !omm_sparkly_is_bowser_4_battle()) {
        omm_world_behavior_set_dormant(bhvBowser, true);
        omm_world_behavior_set_dormant(bhvBowserBodyAnchor, true);
        omm_world_behavior_set_dormant(bhvBowserTailAnchor, true);
        omm_world_behavior_set_dormant(bhvBowserFlameSpawn, true);
        omm_world_behavior_set_dormant(bhvBowserBomb, true);
    }
#endif

    // Hide red coins star markers if Colored Stars
    omm_world_behavior_set_dormant(bhvRedCoinStarMarker, OMM_EXTRAS_COLORED_STARS);
}

// Water is always treated as cold water
static void omm_update_frozen(struct MarioState *m) {
    if (sOmmWorldFrozen && m->area) {
        m->area->terrainType = TERRAIN_SNOW;
    }
}

// Calls to find_water_level always return +20000
static void omm_update_flooded() {
    if (sOmmWorldFlooded) {
        gEnvironmentRegions = NULL;
    }
}

void omm_world_update(struct MarioState *m) {
    static s32 sCurrCourseNum = -1;
    if (gCurrCourseNum >= COURSE_MIN && gCurrCourseNum != sCurrCourseNum) {
        gPrevCourseNum = sCurrCourseNum;
        sCurrCourseNum = gCurrCourseNum;
    }
    sOmmWorldFrozen = false;
    sOmmWorldFlooded = false;
    sOmmWorldShadow = false;
    if (!omm_is_ending_cutscene()) {
        omm_update_warp_pipes(m);
        omm_update_worlds(m);
        omm_update_frozen(m);
        omm_update_flooded();
    }
}

bool omm_world_is_cold() {
    return gMarioState->area && (gMarioState->area->terrainType & TERRAIN_MASK) == TERRAIN_SNOW;
}

bool omm_world_is_frozen() {
    return sOmmWorldFrozen;
}

bool omm_world_is_flooded() {
    return sOmmWorldFlooded;
}

bool omm_world_is_shadow() {
    return sOmmWorldShadow;
}

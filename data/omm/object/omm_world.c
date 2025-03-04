#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"
#include <time.h>

static struct {

    // Course
    s32 prevCourseNum;
    s32 currCourseNum;

    // Level
    bool isFrozen;
    bool isFlooded;
    bool isDark;

    // Warp pipes
    OmmArray wpSpawnInfo;
    OmmArray wpWarpNodes;
    s32 wpTimer;

} sOmmWorld[1];

//
// Warp Pipes
//

struct WarpPipeInfo {
    s32 levelNum;
    s32 areaIndex;
    s32 warpId;
    Vec3f pipePos;
    Vec3s pipeAngle;
    bool exitOnly;
};

struct WarpPipeInfo _WarpPipeInfo(s32 levelNum, s32 areaIndex, s32 warpId, f32 pipePosX, f32 pipePosY, f32 pipePosZ, s16 pipeAngleP, s16 pipeAngleY, s16 pipeAngleR, bool exitOnly) {
    struct WarpPipeInfo wpi = { levelNum, areaIndex, warpId, { pipePosX, pipePosY, pipePosZ }, { pipeAngleP, pipeAngleY, pipeAngleR }, exitOnly };
    return wpi;
}

static struct SpawnInfo *get_warp_pipe_spawn_info(struct WarpPipeInfo *wpi) {
    omm_array_for_each(sOmmWorld->wpSpawnInfo, p) {
        struct SpawnInfo *spawnInfo = (struct SpawnInfo *) p->as_ptr;
        if (spawnInfo->behaviorArg   == ((u32) (wpi->warpId << 16u) | (u32) (wpi->exitOnly << 8u)) &&
            spawnInfo->areaIndex     == wpi->areaIndex &&
            spawnInfo->startPos[0]   == wpi->pipePos[0] &&
            spawnInfo->startPos[1]   == wpi->pipePos[1] &&
            spawnInfo->startPos[2]   == wpi->pipePos[2] &&
            spawnInfo->startAngle[0] == wpi->pipeAngle[0] &&
            spawnInfo->startAngle[1] == wpi->pipeAngle[1] &&
            spawnInfo->startAngle[2] == wpi->pipeAngle[2]) {
            return spawnInfo;
        }
    }

    struct SpawnInfo *spawnInfo = mem_new(struct SpawnInfo, 1);
    spawnInfo->startPos[0]      = wpi->pipePos[0];
    spawnInfo->startPos[1]      = wpi->pipePos[1];
    spawnInfo->startPos[2]      = wpi->pipePos[2];
    spawnInfo->startAngle[0]    = wpi->pipeAngle[0];
    spawnInfo->startAngle[1]    = wpi->pipeAngle[1];
    spawnInfo->startAngle[2]    = wpi->pipeAngle[2];
    spawnInfo->areaIndex        = wpi->areaIndex;
    spawnInfo->activeAreaIndex  = wpi->areaIndex;
    spawnInfo->behaviorArg      = ((u32) (wpi->warpId << 16u) | (u32) (wpi->exitOnly << 8u));
    spawnInfo->behaviorScript   = (void *) bhvWarpPipe;
    spawnInfo->mModel           = geo_layout_to_graph_node(NULL, warp_pipe_geo);
    omm_array_add(sOmmWorld->wpSpawnInfo, ptr, spawnInfo);
    return spawnInfo;
}

static struct Object *load_warp_pipe_spawn_info(struct WarpPipeInfo *wpi) {
    struct SpawnInfo *spawnInfo = get_warp_pipe_spawn_info(wpi);
    for (struct SpawnInfo *p = gAreas[wpi->areaIndex].objectSpawnInfos; p;) {
        if (p == spawnInfo) {
            return NULL;
        }
        p = p->next;
    }

    spawnInfo->next = gAreas[wpi->areaIndex].objectSpawnInfos;
    gAreas[wpi->areaIndex].objectSpawnInfos = spawnInfo;
    if (wpi->areaIndex == gCurrAreaIndex) {
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
        pipe->oBehParams       = spawnInfo->behaviorArg;
        pipe->oIntangibleTimer = 0;
        pipe->oInteractStatus  = 0;
        return pipe;
    }
    return NULL;
}

static struct ObjectWarpNode *get_warp_pipe_warp_node(struct WarpPipeInfo *wpiFrom, struct WarpPipeInfo *wpiTo, struct Object *pipe) {
    omm_array_for_each(sOmmWorld->wpWarpNodes, p) {
        struct ObjectWarpNode *warpNode = (struct ObjectWarpNode *) p->as_ptr;
        if (warpNode->node.id        == wpiFrom->warpId  &&
            warpNode->node.destLevel == wpiTo->levelNum  &&
            warpNode->node.destArea  == wpiTo->areaIndex &&
            warpNode->node.destNode  == wpiTo->warpId) {
            return warpNode;
        }
    }

    struct ObjectWarpNode *warpNode = mem_new(struct ObjectWarpNode, 1);
    warpNode->node.id        = wpiFrom->warpId;
    warpNode->node.destLevel = wpiTo->levelNum;
    warpNode->node.destArea  = wpiTo->areaIndex;
    warpNode->node.destNode  = wpiTo->warpId;
    warpNode->object         = pipe;
    omm_array_add(sOmmWorld->wpWarpNodes, ptr, warpNode);
    return warpNode;
}

static void load_warp_pipe_warp_node(struct WarpPipeInfo *wpiFrom, struct WarpPipeInfo *wpiTo, struct Object *pipe) {
    struct ObjectWarpNode *warpNode = get_warp_pipe_warp_node(wpiFrom, wpiTo, pipe);
    for (struct ObjectWarpNode *p = gAreas[wpiFrom->areaIndex].warpNodes; p;) {
        if (p == warpNode) {
            return;
        }
        p = p->next;
    }

    warpNode->next = gAreas[wpiFrom->areaIndex].warpNodes;
    gAreas[wpiFrom->areaIndex].warpNodes = warpNode;
}

static void load_warp_pipe(struct WarpPipeInfo wpi1, struct WarpPipeInfo wpi2) {
    if (gCurrLevelNum == wpi1.levelNum) {
        struct Object *pipe1 = load_warp_pipe_spawn_info(&wpi1);
        load_warp_pipe_warp_node(&wpi1, &wpi2, pipe1);
        if (gCurrLevelNum == wpi2.levelNum) {
            struct Object *pipe2 = load_warp_pipe_spawn_info(&wpi2);
            load_warp_pipe_warp_node(&wpi2, &wpi1, pipe2);
        }
    }
}

static void omm_update_warp_pipes(struct MarioState *m) {

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
#elif OMM_GAME_IS_SMMS
    load_warp_pipe(
        _WarpPipeInfo(LEVEL_SSL, 1, 0x32,  3660,  2895, -6524, 0x0000, 0x0000, 0x0000,  true),
        _WarpPipeInfo(LEVEL_SSL, 2, 0x32, -1612, -1612, -4875, 0x0000, 0x0000, 0x0000, false)
    );
#endif

    // Update
    sOmmWorld->wpTimer = (sOmmWorld->wpTimer + 1) * !omm_mario_is_emerging_from_pipe(m);
    for_each_object_with_behavior(warpPipe, bhvWarpPipe) {
        warpPipe->oScaleX = 1.f;
        warpPipe->oScaleY = max_f(0.f, 1.f - ((warpPipe->oBehParams >> 8u) & 1) * (sOmmWorld->wpTimer / 20.f));
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

#define omm_world_behavior_set_dormant(bhv, value)                          { for_each_object_with_behavior(obj, bhv) { obj_set_dormant(obj, value); } }
#define omm_world_behavior_set_dormant_cond(bhv, cond, value)               { for_each_object_with_behavior(obj, bhv) { if (cond) { obj_set_dormant(obj, value); } } }
#define omm_world_behavior_set_dormant_params(bhv, params, value)           { for_each_object_with_behavior(obj, bhv) { if (obj->oBehParams == params) { obj_set_dormant(obj, value); } } }
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
            struct Object *box = obj_get_first_with_behavior_and_field_f32(bhvPushableMetalBox, _FIELD(oPosX), 3968.f);
            if (box && !obj_is_dormant(box)) {
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_EXCLAMATION_BOX, bhvExclamationBox,  3800, 3800, -2900, 0, 0xC000, 0)->oBehParams2ndByte = 0x02;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_EXCLAMATION_BOX, bhvExclamationBox, -4200, 1720, -4300, 0, 0xE93E, 0)->oBehParams2ndByte = 0x02;
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
        // Non-Stop mode:
        // - Remove the rocks blocking the path to star 4
        case COURSE_HMC: {
            omm_world_non_stop_behavior_set_dormant(bhvCustomSMSRBreakableFloor, obj->oPosZ == 3275);
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
            struct Object *box = obj_get_first_with_behavior_and_field_s32(bhvExclamationBox, _FIELD(oBehParams), 0x050E0000);
            if (box && !obj_is_dormant(box)) {
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_HMC_ROLLING_ROCK, bhvCustomSMSRToxicWastePlatform, 4679, -1100, -6850, 0x200, 0, -0x300);
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_HMC_ROLLING_ROCK, bhvCustomSMSRToxicWastePlatform, 4679, -1050, -7200, -0x400, 0, 0x580);
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_EXCLAMATION_BOX, bhvExclamationBox, 4679, -600, -7200, 0, 0, 0)->oBehParams2ndByte = 0x0E;
                obj_set_dormant(box, true);
            }
        } break;

        // Starlight Runway
        case COURSE_RR: {
        } break;

        // Star Leap Tower Grounds (Ending)
        case COURSE_CAKE_END: {
        } break;

        // Zero Life Area
        case COURSE_NONE: {
            if (gCurrLevelNum == LEVEL_ZERO_LIFE) {
                omm_secrets_unlock(OMM_SECRET_SMSR_SECRET);
            }
        } break;

#elif OMM_GAME_IS_SMMS

        // Skyland Fortress
        // - Spawn the secret warp to Yellow Dunes
        case COURSE_HMC: {
            static const Vtx omm_smms_ssl_warp_vtx[] = {
                {{{ -160, -160, 0 }, 0, { 0, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
                {{{ +160, -160, 0 }, 0, { 992, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
                {{{ -160, +160, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
                {{{ +160, +160, 0 }, 0, { 992, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
            };
            static const Gfx omm_smms_ssl_warp_gfx[] = {
                gsSPClearGeometryMode(G_CULL_BOTH | G_LIGHTING),
                gsSPSetGeometryMode(G_CULL_BACK),
                gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
                gsDPLoadTextureBlock("menu/smms/painting.rgba32", G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
                gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
                gsSPVertex(omm_smms_ssl_warp_vtx, 4, 0),
                gsSP2Triangles(0, 1, 2, 0, 2, 1, 3, 0),
                gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
                gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
                gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
                gsSPEndDisplayList(),
            };
            static const GeoLayout omm_smms_ssl_warp_geo[] = {
                GEO_NODE_START(),
                GEO_OPEN_NODE(),
                    GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_smms_ssl_warp_gfx),
                GEO_CLOSE_NODE(),
                GEO_END(),
            };
            if (!obj_get_first_with_behavior_and_field_s32(bhvOmmWallWarp, _FIELD(oWallWarpKind), 0)) {
                omm_obj_spawn_wall_warp(
                    m->marioObj, 0, -860, 1640, -480, 160, 160, 0x4000,
                    LEVEL_SSL, 1, 0x0A, 30, WARP_TRANSITION_FADE_INTO_CIRCLE, 20, 0x000000,
                    1.f, 1.f, 1.f, omm_smms_ssl_warp_geo, NULL
                );
            }
        } break;

        // Sweet Sweet Rush
        // - Add more coins to be able to get the 100 coins star
        case COURSE_LLL: {
            struct Object *box3coins = obj_get_first_with_behavior_and_field_s32(bhvExclamationBox, _FIELD(oBehParams2ndByte), 0x05);
            if (box3coins && !obj_is_dormant(box3coins)) {
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_EXCLAMATION_BOX, bhvExclamationBox, -2136,  993, -2347, 0, 0x0000, 0)->oBehParams2ndByte = 0x06;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,  -5391, -979,  3167, 0, 0x0000, 0)->oBehParams2ndByte = COIN_FORMATION_FLAG_RING;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,  -6900,  570,  1400, 0, 0x0000, 0)->oBehParams2ndByte = COIN_FORMATION_FLAG_RING;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,   4444, 5480, -4444, 0, 0x0000, 0)->oBehParams2ndByte = COIN_FORMATION_FLAG_RING;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,   -380,  570,  4200, 0, 0x0000, 0)->oBehParams2ndByte = COIN_FORMATION_FLAG_RING;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,   4600,  270,  4060, 0, 0x0000, 0)->oBehParams2ndByte = 0;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,  -6090,  625, -6300, 0, 0x4000, 0)->oBehParams2ndByte = 0;
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE,            bhvCoinFormation,  -2730, 1000, -6400, 0, 0x0000, 0)->oBehParams2ndByte = COIN_FORMATION_FLAG_FLYING;
                obj_set_dormant(box3coins, true);
            }
        } break;

        // Yellow Dunes
        // - Hide the pipe in the sub-area while Eyerok is alive
        case COURSE_SSL: {
            omm_secrets_unlock(OMM_SECRET_SMMS_SECRET);
            if (gCurrAreaIndex == 1) {
                sOmmWorld->wpTimer = 20;
            } else {
                omm_world_behavior_set_dormant(bhvWarpPipe, obj_get_first_with_behavior(bhvEyerokBoss) != NULL);
            }
        } break;

#elif OMM_GAME_IS_SMGS

        // Molten Magma Galaxy
        case COURSE_SL: {
            struct Object *spark = obj_get_first_with_behavior(bhvStaticObject);
            if (!spark) {
                spark = spawn_object_abs_with_rot(m->marioObj, 0, MODEL_SPARKLES, bhvStaticObject, 2720, 0, 6780, 0, 0, 0);
            }
            spark->oAnimState++;
            spark->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
        } break;

        // Not Vanish Cap under the Moat
        case COURSE_VCUTM: {
            omm_secrets_unlock(OMM_SECRET_SMGS_SECRET);
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
#if OMM_GAME_IS_R96X
            omm_world_non_stop_behavior_set_dormant(bhvBetaBooKey, wkb);
#endif
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

            // Flooded JRB
            if (m->numStars >= 120 && (sOmmWorld->prevCourseNum == COURSE_DDD || sOmmWorld->prevCourseNum == COURSE_WDW) && gCurrAreaIndex == 1) {
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
                sOmmWorld->isFlooded = true;
            }

            // Main area
            else if (gCurrAreaIndex == 1) {
                bool sunken = !omm_stars_is_collected(0);
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
            }

            // Inside the sunken ship
            else {
                struct Object *trigger = obj_get_first_with_behavior(bhvTreasureChestsShip);
                bool sunken = (!omm_stars_is_collected(0) || trigger != NULL) && !omm_mario_is_emerging_from_pipe(m);
                omm_world_non_stop_behavior_set_dormant(bhvWarpPipe, sunken);
                if (!sunken && trigger) {
                    gEnvironmentRegions[6] = -335;
                    obj_unload_all_with_behavior(bhvTreasureChestTop);
                    obj_unload_all_with_behavior(bhvTreasureChestBottom);
                    obj_mark_for_deletion(trigger);
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
        // - Move the pillars coins down to be closer to the detectors
        case COURSE_SSL: {
            omm_world_non_stop_behavior_set_dormant_params(bhvTweester, 0x00190000, true);
            if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) {
                for_each_object_with_behavior(obj, bhvYellowCoin) {
                    if (obj->oPosY == 1200.f) {
                        struct Object *detector = obj_get_nearest_with_behavior_and_radius(obj, bhvPyramidPillarTouchDetector, 400.f);
                        if (detector) {
                            obj->oPosX = detector->oPosX;
                            obj->oPosY = detector->oPosY + 100.f;
                            obj->oPosZ = detector->oPosZ;
                        }
                    }
                }
            }
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
        // Dark variant, from exiting BBH or BITDW, then entering BITS (120+ stars only):
        // - The level is engulfed by darkness: only objects are visible, everything else is pure black
        case COURSE_BITS: {
            if (gCurrLevelNum == LEVEL_BITS && m->numStars >= 120 && (sOmmWorld->prevCourseNum == COURSE_BBH || sOmmWorld->prevCourseNum == COURSE_BITDW)) {
                sOmmWorld->isDark = true;
            }
        } break;

        // The Princess's Secret Slide
        // There is no cow level
        case COURSE_PSS: {
            if (m->numStars >= 120 && !obj_get_first_with_behavior(bhvOmmPeachyRoomTrigger)) {
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvOmmPeachyRoomTrigger, 6140, 6655, -5620, 0, 0xC000, 0);
            }
        } break;

        // Wing Mario over the Rainbow
        // Something appears in the sky after getting all 120 stars (Sparklies disabled) + Dorrie capture registered
        case COURSE_WMOTR: {
            if (m->numStars >= 120 && (omm_save_file_get_capture_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE) & OMM_CAPTURE_DORRIE)) {
                struct Object *dorrie = obj_get_first_with_behavior(bhvOmmDorrie);
                if (!OMM_SPARKLY_MODE_IS_ENABLED && !dorrie) {
                    obj_set_pos(obj_spawn_from_geo(m->marioObj, dorrie_geo, bhvOmmDorrie), 0, 2500, 0);
                } else if (OMM_SPARKLY_MODE_IS_ENABLED && dorrie) {
                    obj_mark_for_deletion(dorrie);
                }
            }
        } break;

        // Secret Aquarium
        // Frozen variant, from exiting CCM or SL, then entering SA (120+ stars only):
        // - Turn the water into cold water
        // - Hide the fish groups
        case COURSE_SA: {
            if (m->numStars >= 120 && (sOmmWorld->prevCourseNum == COURSE_CCM || sOmmWorld->prevCourseNum == COURSE_SL)) {
                omm_world_behavior_set_dormant(bhvLargeFishGroup, true);
                sOmmWorld->isFrozen = true;
            }
        } break;

        // Castle
        case COURSE_NONE: {
            switch (LEVEL_AREA_INDEX(gCurrLevelNum, gCurrAreaIndex)) {

                // Inside
                // - Spawn a sign about level variants near JRB after 120 stars
                // - Make the lobby Toad talk about the secret cheat after 120 stars and Peach unlocked
                case AREA_CASTLE_LOBBY: {
                    if (m->numStars >= 120 && !obj_get_first_with_behavior_and_field_s32(bhvMessagePanel, _FIELD(oBehParams2ndByte), OMM_DIALOG_LEVEL_VARIANTS)) {
                        struct Object *sign = obj_spawn_from_geo(m->marioObj, wooden_signpost_geo, bhvMessagePanel);
                        obj_set_xyz(sign, 4100, 307, 280);
                        obj_set_angle(sign, 0, 0xC000, 0);
                        obj_drop_to_floor(sign);
                        sign->oBehParams = (OMM_DIALOG_LEVEL_VARIANTS << 16);
                        sign->oBehParams2ndByte = OMM_DIALOG_LEVEL_VARIANTS;
                    }
                    if (m->numStars >= 120 && OMM_REWARD_IS_PLAYABLE_PEACH_UNLOCKED) {
                        struct Object *toad = obj_get_first_with_behavior_and_field_s32(bhvToadMessage, _FIELD(oToadMessageDialogId), DIALOG_133);
                        if (toad) {
                            toad->oBehParams2ndByte = OMM_DIALOG_TOAD_SECRET;
                            toad->oToadMessageDialogId = OMM_DIALOG_TOAD_SECRET;
                        }
                    }
                } break;

                // Basement
                // After 120 stars, make Peach appear?
                case AREA_CASTLE_BASEMENT: {
                    if (m->numStars >= 120 && !obj_get_first_with_behavior(bhvOmmPeach)) {
                        omm_obj_spawn_peach(m->marioObj);
                    }
                } break;

                // Upstairs
                // - Truly endless stairs if less than 70 stars (Odyssey moveset, Yoshi mode or capture)
                // - Make the upstairs Toad talk about the Yoshi mode if the Yoshi summon reward has been unlocked
                case AREA_CASTLE_TIPPY: {
                    bool trulyInfiniteStairs = (m->numStars < 70 && (OMM_MOVESET_ODYSSEY || gOmmGlobals->yoshiMode || omm_mario_is_capture(m)));
                    omm_world_behavior_set_dormant_params(bhvWarp, 0x0F0B0000, trulyInfiniteStairs);
                    if (trulyInfiniteStairs && m->floor->room == 6 && m->pos[2] < 800) {
                        struct InstantWarp *warp = &gCurrentArea->instantWarps[0];
                        if (warp->id != 0) {
                            Vec3f displacement = {
                                warp->displacement[0],
                                max_f(warp->displacement[1], (
                                    find_floor_height(m->pos[0], m->pos[1] + 80, m->pos[2] + warp->displacement[2]) -
                                    find_floor_height(m->pos[0], m->pos[1] + 80, m->pos[2])
                                )),
                                warp->displacement[2],
                            };
                            omm_process_instant_warp(m, displacement, warp->area);
                        }
                    }
                    // TODO: YOSHIMODE
                    // if (m->numStars >= 120 && OMM_REWARD_IS_SUMMON_YOSHI_UNLOCKED) {
                    //     struct Object *toad = obj_get_first_with_behavior_and_field_s32(bhvToadMessage, _FIELD(oToadMessageDialogId), DIALOG_137);
                    //     if (toad) {
                    //         toad->oBehParams2ndByte = OMM_DIALOG_TOAD_YOSHI;
                    //         toad->oToadMessageDialogId = OMM_DIALOG_TOAD_YOSHI;
                    //     }
                    // }
                } break;
            }
        } break;

#else

        default: {
        } break;

#endif
    }

    // Stats board
    if (gCurrLevelNum == OMM_STATS_BOARD_LEVEL && gCurrAreaIndex == OMM_STATS_BOARD_AREA && !obj_get_first_with_behavior(bhvOmmStatsBoard)) {
        omm_obj_spawn_stats_board(m->marioObj, OMM_STATS_BOARD_X, OMM_STATS_BOARD_Y, OMM_STATS_BOARD_Z, OMM_STATS_BOARD_ANGLE);
    }

    // Set recovery hearts dormant in Sparkly Stars Lunatic mode
    omm_world_behavior_set_dormant(bhvRecoveryHeart, OMM_SPARKLY_MODE_IS_LUNATIC);
#if OMM_GAME_IS_SMSR
    omm_world_behavior_set_dormant(bhvCustomSMSRRecoveryBubbleWater, OMM_SPARKLY_MODE_IS_LUNATIC);
#endif

#if !OMM_GAME_IS_R96X
    // Disable Bowser objects outside of Bowser fights
    if (!OMM_LEVEL_IS_BOWSER_FIGHT(gCurrLevelNum) && !omm_sparkly_is_bowser_4_battle()) {
        omm_world_behavior_set_dormant(bhvBowser, true);
        omm_world_behavior_set_dormant(bhvBowserBodyAnchor, true);
        omm_world_behavior_set_dormant(bhvBowserTailAnchor, true);
        omm_world_behavior_set_dormant(bhvBowserFlameSpawn, true);
    }
#endif

    // Hide red coins star markers if Colored Stars
    omm_world_behavior_set_dormant(bhvRedCoinStarMarker, OMM_EXTRAS_COLORED_STARS);
}

// Water is always treated as cold water
static void omm_update_frozen(struct MarioState *m) {
    if (sOmmWorld->isFrozen && m->area) {
        m->area->terrainType = TERRAIN_SNOW;
    }
}

// Calls to find_water_level always return +20000
static void omm_update_flooded() {
    if (sOmmWorld->isFlooded) {
        gEnvironmentRegions = NULL;
    }
}

void omm_world_update(struct MarioState *m) {
    if (gCurrCourseNum >= COURSE_MIN && gCurrCourseNum != sOmmWorld->currCourseNum) {
        sOmmWorld->prevCourseNum = sOmmWorld->currCourseNum;
        sOmmWorld->currCourseNum = gCurrCourseNum;
    }
    sOmmWorld->isFrozen = false;
    sOmmWorld->isFlooded = false;
    sOmmWorld->isDark = false;
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
    return sOmmWorld->isFrozen;
}

bool omm_world_is_flooded() {
    return sOmmWorld->isFlooded;
}

bool omm_world_is_dark() {
    return sOmmWorld->isDark;
}

OMM_ROUTINE_LEVEL_ENTRY(omm_world_on_level_entry) {
    if (omm_is_main_menu()) {
        sOmmWorld->prevCourseNum = COURSE_NONE;
        sOmmWorld->currCourseNum = COURSE_NONE;
    }
    sOmmWorld->isFrozen = false;
    sOmmWorld->isFlooded = false;
    sOmmWorld->isDark = false;
    sOmmWorld->wpTimer = 20;
}

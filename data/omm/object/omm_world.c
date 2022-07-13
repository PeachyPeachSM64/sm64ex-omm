#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Warp Pipes
//

struct WarpPipeInfo {
    u8 area;
    u8 warpId;
    Vec3f pipePos;
    Vec3s pipeAngle;
    bool exitOnly;
};

struct WarpPipeInfo _WarpPipeInfo(u8 area, u8 warpId, f32 pipePosX, f32 pipePosY, f32 pipePosZ, s16 pipeAngleP, s16 pipeAngleY, s16 pipeAngleR, bool exitOnly) {
    struct WarpPipeInfo wpi = { area, warpId, { pipePosX, pipePosY, pipePosZ }, { pipeAngleP, pipeAngleY, pipeAngleR }, exitOnly };
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
    
    struct SpawnInfo *spawnInfo = OMM_MEMNEW(struct SpawnInfo, 1);
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
    for (struct SpawnInfo *p = gAreas[wpi->area].objectSpawnInfos; p != NULL;) {
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
        pipe->oBehParams       = spawnInfo->behaviorArg;
        pipe->oIntangibleTimer = 0;
        pipe->oInteractStatus  = 0;
        return pipe;
    }
    return NULL;
}

static struct ObjectWarpNode *get_warp_pipe_warp_node(u8 level, struct WarpPipeInfo *wpiFrom, struct WarpPipeInfo *wpiTo, struct Object *pipe) {
    static OmmArray sWarpPipeWarpNodes = omm_array_zero;
    omm_array_for_each(sWarpPipeWarpNodes, p) {
        struct ObjectWarpNode *warpNode = (struct ObjectWarpNode *) p->as_ptr;
        if (warpNode->node.id        == wpiFrom->warpId &&
            warpNode->node.destLevel == level           &&
            warpNode->node.destArea  == wpiTo->area     &&
            warpNode->node.destNode  == wpiTo->warpId) {
            return warpNode;
        }
    }

    struct ObjectWarpNode *warpNode = OMM_MEMNEW(struct ObjectWarpNode, 1);
    warpNode->node.id        = wpiFrom->warpId;
    warpNode->node.destLevel = level;
    warpNode->node.destArea  = wpiTo->area;
    warpNode->node.destNode  = wpiTo->warpId;
    warpNode->object         = pipe;
    omm_array_add(sWarpPipeWarpNodes, ptr, warpNode);
    return warpNode;
}

static void load_warp_pipe_warp_node(u8 level, struct WarpPipeInfo *wpiFrom, struct WarpPipeInfo *wpiTo, struct Object *pipe) {
    struct ObjectWarpNode *warpNode = get_warp_pipe_warp_node(level, wpiFrom, wpiTo, pipe);
    for (struct ObjectWarpNode *p = gAreas[wpiFrom->area].warpNodes; p != NULL;) {
        if (p == warpNode) {
            return;
        }
        p = p->next;
    }

    warpNode->next = gAreas[wpiFrom->area].warpNodes;
    gAreas[wpiFrom->area].warpNodes = warpNode;
}

static void load_warp_pipe(u8 level, struct WarpPipeInfo wpi1, struct WarpPipeInfo wpi2) {
    if (gCurrLevelNum == level) {
        struct Object *pipe1 = load_warp_pipe_spawn_info(&wpi1);
        load_warp_pipe_warp_node(level, &wpi1, &wpi2, pipe1);
        struct Object *pipe2 = load_warp_pipe_spawn_info(&wpi2);
        load_warp_pipe_warp_node(level, &wpi2, &wpi1, pipe2);
    }
}

static void omm_update_warp_pipes(struct MarioState *m) {
    static s32 sWarpPipeTimer = 20;

    // Load
#if OMM_GAME_IS_SM64
    if (OMM_STARS_NON_STOP) {
    load_warp_pipe(LEVEL_JRB,
        _WarpPipeInfo(1, 0x32,  4900,  1440,  2500, 0x0000, 0x0000, 0x0000, false),
        _WarpPipeInfo(2, 0x32,     0,  -351, -1100, 0x0000, 0x0000, 0x0000, false)
    );
    }
    load_warp_pipe(LEVEL_LLL,
        _WarpPipeInfo(1, 0x32,  2040,   307,  3760, 0x0000, 0x8000, 0x0000,  true),
        _WarpPipeInfo(2, 0x32, -1560,    90,   620, 0x0000, 0x0000, 0x0000, false)
    );
    load_warp_pipe(LEVEL_SSL,
        _WarpPipeInfo(1, 0x32, -2040,   256,   714, 0x0000, 0x0000, 0x0000,  true),
        _WarpPipeInfo(2, 0x32,     0,     0,  2880, 0x0000, 0x0000, 0x0000, false)
    );
    load_warp_pipe(LEVEL_DDD,
        _WarpPipeInfo(2, 0x33,  6160,   110,  4430, 0x0000, 0xA000, 0x0000,  true),
        _WarpPipeInfo(2, 0x32,  2030, -4088, -2780, 0x0000, 0x0000, 0x0000, false)
    );
    load_warp_pipe(LEVEL_WDW,
        _WarpPipeInfo(1, 0x32, -3450,  3584, -3450, 0x0000, 0x2000, 0x0000,  true),
        _WarpPipeInfo(2, 0x32,  -767,  -332,  1792, 0x0000, 0x0000, 0x0000, false)
    );
    if (OMM_STARS_NON_STOP) {
    load_warp_pipe(LEVEL_THI,
        _WarpPipeInfo(1, 0x35,  2590,  3226, -2085, 0x0000, 0xC000, 0x0000,  true),
        _WarpPipeInfo(3, 0x35,     0,  2560,     0, 0x0000, 0x0000, 0x0000, false)
    );
    }
#endif

    // Update
    sWarpPipeTimer = (sWarpPipeTimer + 1) * (m->action != ACT_EMERGE_FROM_PIPE);
    for_each_object_with_behavior(warpPipe, bhvWarpPipe) {
        warpPipe->oScaleX = 1.f;
        warpPipe->oScaleY = max_f(0.f, 1.f - ((warpPipe->oBehParams >> 8u) & 1) * (sWarpPipeTimer / 20.f));
        warpPipe->oScaleZ = 1.f;
        warpPipe->hitboxRadius = warpPipe->hitboxRadius * (warpPipe->oScaleY == 1.f);
        warpPipe->hitboxHeight = warpPipe->hitboxHeight * (warpPipe->oScaleY == 1.f);
        warpPipe->hitboxDownOffset = warpPipe->hitboxHeight * relerp_0_1_f(coss(warpPipe->oFaceAnglePitch) * coss(warpPipe->oFaceAngleRoll), -1.f, 1.f, 1.f, 0.f);
        obj_set_dormant(warpPipe, warpPipe->oScaleY <= 0.f);
    }
}

//
// Power-up Boxes
//

static void load_power_up_box(u8 level, s8 area, u8 type, s16 x, s16 y, s16 z, s16 angle) {
    if (gCurrLevelNum == level && gCurrAreaIndex == area) {

        // Look for a box at (x, z)
        for_each_object_with_behavior(box, bhvExclamationBox) {
            if (box->oPosX == x && box->oPosZ == z) {
                return;
            }
        }

        // If not found, spawn it
        struct Object *box = obj_spawn_from_geo(gMarioObject, exclamation_box_geo, bhvExclamationBox);
        box->oPosX = x;
        box->oPosY = y;
        box->oPosZ = z;
        box->oHomeX = x;
        box->oHomeY = y;
        box->oHomeZ = z;
        box->oFaceAnglePitch = 0;
        box->oFaceAngleYaw = angle;
        box->oFaceAngleRoll = 0;
        box->oMoveAnglePitch = 0;
        box->oMoveAngleYaw = angle;
        box->oMoveAngleRoll = 0;
        box->oBehParams = 0;
        box->oBehParams2ndByte = type;
    }
}

static void omm_update_power_ups() {
    if (gCurrCourseNum < COURSE_MIN ||
        gCurrCourseNum > COURSE_STAGES_MAX ||
        save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1) != 0x7F) {
        return;
    }
    
#if OMM_GAME_IS_SM64
    // BOB
    load_power_up_box(LEVEL_BOB, 1, 0, -5300, 428, 5600, 0x0000);
    load_power_up_box(LEVEL_BOB, 1, 1, -5700, 428, 5200, 0x0000);
    load_power_up_box(LEVEL_BOB, 1, 2, -6100, 428, 5600, 0x0000);
    
    // WF
    load_power_up_box(LEVEL_WF, 1, 0, 3000, 556, 4600, 0x0000);
    load_power_up_box(LEVEL_WF, 1, 1, 3500, 556, 4600, 0x0000);
    load_power_up_box(LEVEL_WF, 1, 2, 4000, 556, 4600, 0x0000);
    
    // JRB
    load_power_up_box(LEVEL_JRB, 1, 0, -5800, 1340,   50, 0x0000);
    load_power_up_box(LEVEL_JRB, 1, 1, -5400, 1340, -350, 0x0000);
    load_power_up_box(LEVEL_JRB, 1, 2, -6200, 1340, -350, 0x0000);
    
    // CCM
    load_power_up_box(LEVEL_CCM, 1, 0,     0, 2860, -2700, 0x0000);
    load_power_up_box(LEVEL_CCM, 1, 1,  -500, 2860, -2700, 0x0000);
    load_power_up_box(LEVEL_CCM, 1, 2, -1000, 2860, -2700, 0x0000);
    
    // BBH
    load_power_up_box(LEVEL_BBH, 1, 0,  166, 96, 4000, 0x0000);
    load_power_up_box(LEVEL_BBH, 1, 1,  666, 96, 4000, 0x0000);
    load_power_up_box(LEVEL_BBH, 1, 2, 1166, 96, 4000, 0x0000);
    
    // HMC
    load_power_up_box(LEVEL_HMC, 1, 0, 3650, 300, 4970, 0x0000);
    load_power_up_box(LEVEL_HMC, 1, 1, 4050, 300, 4570, 0x0000);
    load_power_up_box(LEVEL_HMC, 1, 2, 4050, 300, 5370, 0x0000);
    
    // LLL
    load_power_up_box(LEVEL_LLL, 1, 0, -500, 607,  3700, 0x0000);
    load_power_up_box(LEVEL_LLL, 1, 1,    0, 607,  3700, 0x0000);
    load_power_up_box(LEVEL_LLL, 1, 2,  500, 607,  3700, 0x0000);
    load_power_up_box(LEVEL_LLL, 2, 0,    0, 382, -1440, 0x0000);
    load_power_up_box(LEVEL_LLL, 2, 1,  500, 382, -1440, 0x0000);
    load_power_up_box(LEVEL_LLL, 2, 2, 1000, 382, -1440, 0x0000);
    
    // SSL
    load_power_up_box(LEVEL_SSL, 1, 0, 2500, 300, 7050, 0x0000);
    load_power_up_box(LEVEL_SSL, 1, 1, 3000, 300, 7050, 0x0000);
    load_power_up_box(LEVEL_SSL, 1, 2, 3500, 300, 7050, 0x0000);
    load_power_up_box(LEVEL_SSL, 2, 0, -500, 300, 3600, 0x0000);
    load_power_up_box(LEVEL_SSL, 2, 1,    0, 300, 3600, 0x0000);
    load_power_up_box(LEVEL_SSL, 2, 2,  500, 300, 3600, 0x0000);
    
    // DDD
    load_power_up_box(LEVEL_DDD, 2, 0, 3400, 410, 5900, 0x0000);
    load_power_up_box(LEVEL_DDD, 2, 1, 3900, 410, 5900, 0x0000);
    load_power_up_box(LEVEL_DDD, 2, 2, 4400, 410, 5900, 0x0000);
    
    // SL
    load_power_up_box(LEVEL_SL, 1, 0, 5400, 1324, 4050, 0x0000);
    load_power_up_box(LEVEL_SL, 1, 1, 5400, 1324, 4550, 0x0000);
    load_power_up_box(LEVEL_SL, 1, 2, 5400, 1324, 5050, 0x0000);
    
    // WDW
    load_power_up_box(LEVEL_WDW, 1, 0, 4500, 300, 1550, 0x0000);
    load_power_up_box(LEVEL_WDW, 1, 1, 4500, 300, 2050, 0x0000);
    load_power_up_box(LEVEL_WDW, 1, 2, 4500, 300, 2550, 0x0000);
    
    // TTM
    load_power_up_box(LEVEL_TTM, 1, 0, -400, -4020, 5250, 0x0000);
    load_power_up_box(LEVEL_TTM, 1, 1,  100, -4020, 5250, 0x0000);
    load_power_up_box(LEVEL_TTM, 1, 2,  600, -4020, 5250, 0x0000);
    
    // THI
    load_power_up_box(LEVEL_THI, 1, 0, -6100, -2190,  700, 0x0000);
    load_power_up_box(LEVEL_THI, 1, 1, -6100, -2190, 1500, 0x0000);
    load_power_up_box(LEVEL_THI, 1, 2, -6500, -2190, 1100, 0x0000);
    load_power_up_box(LEVEL_THI, 2, 0,  -150,  -620, 1400, 0x0000);
    load_power_up_box(LEVEL_THI, 2, 1,  -650,  -620, 1400, 0x0000);
    load_power_up_box(LEVEL_THI, 2, 2,   350,  -620, 1400, 0x0000);
    
    // TTC
    load_power_up_box(LEVEL_TTC, 1, 0, -500, 281, -1800, 0x0000);
    load_power_up_box(LEVEL_TTC, 1, 1,    0, 281, -1800, 0x0000);
    load_power_up_box(LEVEL_TTC, 1, 2,  500, 281, -1800, 0x0000);
    
    // RR
    load_power_up_box(LEVEL_RR, 1, 0, 2245, -1533, 2071, 0x0000);
    load_power_up_box(LEVEL_RR, 1, 1, 2645, -1533, 1671, 0x0000);
    load_power_up_box(LEVEL_RR, 1, 2, 2645, -1533, 2471, 0x0000);
#endif
}

//
// Worlds
//

static s32 sOmmWorldPrevious = -1;
static s32 sOmmWorldCurrent = -1;
static bool sOmmWorldFrozen = false;
static bool sOmmWorldFlooded = false;
static bool sOmmWorldVanished = false;

#define omm_world_behavior_set_dormant(bhv, value)                          { for_each_object_with_behavior(obj, bhv) { obj_set_dormant(obj, value); } }
#define omm_world_behavior_set_dormant_cond(bhv, cond, value)               { for_each_object_with_behavior(obj, bhv) { if (cond) { obj_set_dormant(obj, value); } } }
#define omm_world_behavior_set_dormant_params(bhv, params, value)           { for_each_object_with_behavior(obj, bhv) { if (obj->oBehParams == params) { obj_set_dormant(obj, value); } } }
#define omm_world_non_stop_behavior_set_dormant(bhv, value)                 { if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) { omm_world_behavior_set_dormant(bhv, value); } }
#define omm_world_non_stop_behavior_set_dormant_cond(bhv, cond, value)      { if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) { omm_world_behavior_set_dormant_cond(bhv, cond, value); } }
#define omm_world_non_stop_behavior_set_dormant_params(bhv, params, value)  { if (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE) { omm_world_behavior_set_dormant_params(bhv, params, value); } }

static void omm_update_worlds(struct MarioState *m) {
    switch (sOmmWorldCurrent) {
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

        // Whomp Fortress
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
        // - Hide all sunken ship parts, the buddy's cannon, the goomba triplet and Unagi
        // Non-Stop mode:
        // - Hide some specific parts of the ship, depending on its state
        // - Keep only the Unagi with a star
        // - Update the whirpools
        case COURSE_JRB: {
            omm_world_non_stop_behavior_set_dormant_params(bhvUnagi, 0x00000000, true);
            omm_world_non_stop_behavior_set_dormant_params(bhvUnagi, 0x02020000, true);
            if (m->numStars >= 120 && (sOmmWorldPrevious == COURSE_DDD || sOmmWorldPrevious == COURSE_WDW) && gCurrAreaIndex == 1) {
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

        // Tic-Toc Clock
        case COURSE_TTC: {
        } break;

        // Rainbow Ride
        case COURSE_RR: {
        } break;

        // Bowser in the Sky
        // Vanished variant, from exiting BBH or VCUTM, then entering BITS (120+ stars only):
        // - All level geometry and surfaces are invisible (but collision is still there)
        case COURSE_BITS: {
            if (gCurrLevelNum == LEVEL_BITS && m->numStars >= 120 && (sOmmWorldPrevious == COURSE_BBH || sOmmWorldPrevious == COURSE_VCUTM)) {
                sOmmWorldVanished = true;
            }
        } break;

        // Secret Aquarium
        // Frozen variant, from exiting CCM or SL, then entering SA (120+ stars only):
        // - Turn the water into cold water
        case COURSE_SA: {
            if (m->numStars >= 120 && (sOmmWorldPrevious == COURSE_CCM || sOmmWorldPrevious == COURSE_SL)) {
                sOmmWorldFrozen = true;
            }
        } break;

#else

        default: {
            OMM_UNUSED(m);
        } break;

#endif
    }

    // Set recovery hearts dormant in Sparkly Stars Lunatic mode
    omm_world_behavior_set_dormant(bhvRecoveryHeart, OMM_SSM_IS_LUNATIC);
#if GAME_IS_SMSR
    omm_world_behavior_set_dormant(bhvCustomSMSRRecoveryBubbleWater, OMM_SSM_IS_LUNATIC);
#endif
    
#if !OMM_GAME_IS_R96A
    // Disable Bowser objects outside of Bowser fights
    if (gCurrLevelNum != LEVEL_BOWSER_1 &&
        gCurrLevelNum != LEVEL_BOWSER_2 &&
        gCurrLevelNum != LEVEL_BOWSER_3 &&
        !omm_ssd_is_bowser_4()) {
        omm_world_behavior_set_dormant(bhvBowser, true);
        omm_world_behavior_set_dormant(bhvBowserBodyAnchor, true);
        omm_world_behavior_set_dormant(bhvBowserTailAnchor, true);
        omm_world_behavior_set_dormant(bhvBowserFlameSpawn, true);
        omm_world_behavior_set_dormant(bhvBowserBomb, true);
    }
#endif
    
#if !OMM_GAME_IS_SMMS
    // Hide red coins star markers if Colored Stars
    omm_world_behavior_set_dormant(bhvRedCoinStarMarker, OMM_EXTRAS_COLORED_STARS);
#endif
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

// All objects with collision and static objects with level geometry are invisible
static void omm_update_vanished() {
    if (sOmmWorldVanished) {
        for_each_object_in_list(obj, OBJ_LIST_SURFACE) {
            obj->oGraphNode = NULL;
        }
        for_each_object_with_behavior(obj, bhvStaticObject) {
            s32 modelId = obj_get_model_id(obj);
            if (modelId >= MODEL_LEVEL_GEOMETRY_03 && modelId <= MODEL_LEVEL_GEOMETRY_16) {
                obj->oGraphNode = NULL;
            }
        }
    }
}

void omm_world_update(struct MarioState *m) {
    sOmmWorldPrevious = (((sOmmWorldCurrent != gCurrCourseNum) && (gCurrCourseNum >= COURSE_MIN)) ? sOmmWorldCurrent : sOmmWorldPrevious);
    sOmmWorldCurrent = ((gCurrCourseNum >= COURSE_MIN) ? gCurrCourseNum : sOmmWorldCurrent);
    sOmmWorldFrozen = false;
    sOmmWorldFlooded = false;
    sOmmWorldVanished = false;
    if (gCurrCourseNum >= COURSE_MIN && !omm_is_ending_cutscene()) {
        omm_update_warp_pipes(m);
        omm_update_power_ups();
        omm_update_worlds(m);
        omm_update_frozen(m);
        omm_update_flooded();
        omm_update_vanished();
    }
}

bool omm_world_is_frozen() {
    return sOmmWorldFrozen || (gMarioState->area && gMarioState->area->terrainType == TERRAIN_SNOW);
}

bool omm_world_is_flooded() {
    return sOmmWorldFlooded;
}

bool omm_world_is_vanished() {
    return sOmmWorldVanished;
}

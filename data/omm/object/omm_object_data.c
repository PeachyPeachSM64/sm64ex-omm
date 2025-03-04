#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

struct OmmData gOmmData[1];
s32 gMouseDeltaX;
s32 gMouseDeltaY;

//
// Reset
//

static void omm_data_reset_mario() {

    // State data
    gOmmData->mario->state.ticks = 0;
    gOmmData->mario->state.coins = 0;
    gOmmData->mario->state.breath = 0;
    gOmmData->mario->state.airCombo = 0;
    gOmmData->mario->state.peakHeight = FLOOR_LOWER_LIMIT;
    gOmmData->mario->state.poleObject = NULL;
    gOmmData->mario->state.health.state = OMM_HEALTH_STATE_NONE;
    gOmmData->mario->state.health.timer = 0;
    gOmmData->mario->state.freeze.dmg = 0;
    gOmmData->mario->state.freeze.gfx = 0;
    gOmmData->mario->state.freeze.sfx = 0;
    gOmmData->mario->state.previous.pos[0] = 0.f;
    gOmmData->mario->state.previous.pos[1] = 0.f;
    gOmmData->mario->state.previous.pos[2] = 0.f;
    gOmmData->mario->state.previous.vel[0] = 0.f;
    gOmmData->mario->state.previous.vel[1] = 0.f;
    gOmmData->mario->state.previous.vel[2] = 0.f;
    gOmmData->mario->state.previous.vel[3] = 0.f;
    gOmmData->mario->state.previous.angle[0] = 0;
    gOmmData->mario->state.previous.angle[1] = 0;
    gOmmData->mario->state.previous.angle[2] = 0;

    // Grab data
    gOmmData->mario->grab.obj = NULL;

    // Dialog data
    gOmmData->mario->dialog.obj = NULL;
    gOmmData->mario->dialog.state = 0;
    gOmmData->mario->dialog.id = -1;
    gOmmData->mario->dialog.turn = 0;
    gOmmData->mario->dialog.choice = 0;

    // Peach-only data
    gOmmData->mario->peach.floated = false;
    gOmmData->mario->peach.floatTimer = 0;
    gOmmData->mario->peach.vibeType = OMM_PEACH_VIBE_TYPE_NONE;
    gOmmData->mario->peach.vibeGauge = 0;
    gOmmData->mario->peach.vibeTimer = OMM_PEACH_VIBE_COOLDOWN;
    gOmmData->mario->peach.vibeGfxTimer = 0;
    gOmmData->mario->peach.joySpinYaw = 0;
    gOmmData->mario->peach.perryCharge = 0;
    gOmmData->mario->peach.perryBlast = false;
    gOmmData->mario->peach.perry = NULL;

    // Wall-slide moves data
    gOmmData->mario->wallSlide.jumped = false;
    gOmmData->mario->wallSlide.angle = 0;
    gOmmData->mario->wallSlide.height = CELL_HEIGHT_LIMIT;

    // Spin moves data
    gOmmData->mario->spin.pressed = false;
    gOmmData->mario->spin.yaw = 0;
    gOmmData->mario->spin.timer = 0;
    gOmmData->mario->spin._buffer = 0;
    gOmmData->mario->spin._counter = 0;
    gOmmData->mario->spin._checkpoint = -1;
    gOmmData->mario->spin._direction = 0;

    // Midair spin move data
    gOmmData->mario->midairSpin.timer = 0;
    gOmmData->mario->midairSpin.counter = 0;

    // Metal-water moves data
    gOmmData->mario->metalWater.punchType = 0;
    gOmmData->mario->metalWater.jumpNext = ACT_OMM_METAL_WATER_JUMP;
    gOmmData->mario->metalWater.jumpTimer = 0;

    // Cappy data
    gOmmData->mario->cappy.bounced = false;
    gOmmData->mario->cappy.cappy = NULL;

    // Capture data
    gOmmData->mario->capture.obj = NULL;
    gOmmData->mario->capture.prev = NULL;
    gOmmData->mario->capture.animPos[0][0] = 0.f;
    gOmmData->mario->capture.animPos[0][1] = 0.f;
    gOmmData->mario->capture.animPos[0][2] = 0.f;
    gOmmData->mario->capture.animPos[1][0] = 0.f;
    gOmmData->mario->capture.animPos[1][1] = 0.f;
    gOmmData->mario->capture.animPos[1][2] = 0.f;
    gOmmData->mario->capture.animPos[2][0] = 0.f;
    gOmmData->mario->capture.animPos[2][1] = 0.f;
    gOmmData->mario->capture.animPos[2][2] = 0.f;
    gOmmData->mario->capture.timer = 0;
    gOmmData->mario->capture.lockTimer = 0;
    gOmmData->mario->capture.starDance = false;
    gOmmData->mario->capture.openDoors = false;
    gOmmData->mario->capture.firstPerson = false;
    gOmmData->mario->capture.stickX = 0.f;
    gOmmData->mario->capture.stickY = 0.f;
    gOmmData->mario->capture.stickMag = 0.f;
    gOmmData->mario->capture.stickYaw = 0;
    gOmmData->mario->capture.buttonPressed = 0;
    gOmmData->mario->capture.buttonDown = 0;
#if OMM_CODE_DEBUG
    gOmmData->mario->capture.hitboxRadius = 0.f;
    gOmmData->mario->capture.hitboxHeight = 0.f;
    gOmmData->mario->capture.hitboxWall = 0.f;
#endif
}

static void omm_data_reset_object() {

    // State data
    gOmmData->object->state.actionFlag = false;
    gOmmData->object->state.actionState = 0;
    gOmmData->object->state.actionTimer = 0;
    gOmmData->object->state.walkDistance = 0.f;
    gOmmData->object->state.friction[0] = 0.f;
    gOmmData->object->state.friction[1] = 0.f;
    gOmmData->object->state.friction[2] = 0.f;
    gOmmData->object->state.heldObj = NULL;
    gOmmData->object->state._properties = 0;
    gOmmData->object->state._invincTimer = 0;
    gOmmData->object->state._cannonTimer = 0;
    gOmmData->object->state._bullyTimer = 0;
    gOmmData->object->state._powerTimer = 0;
    gOmmData->object->state._squishTimer = 0;
    gOmmData->object->state._squishScale[0] = 0.f;
    gOmmData->object->state._squishScale[1] = 0.f;
    gOmmData->object->state._squishScale[2] = 0.f;
    gOmmData->object->state._initialHome[0] = 0.f;
    gOmmData->object->state._initialHome[1] = 0.f;
    gOmmData->object->state._initialHome[2] = 0.f;
    gOmmData->object->state._camBehindMario = false;

    // Cappy data
    gOmmData->object->cappy.object = NULL;
    gOmmData->object->cappy.tra_x = 0.f;
    gOmmData->object->cappy.tra_y = 0.f;
    gOmmData->object->cappy.tra_z = 0.f;
    gOmmData->object->cappy.rot_x = 0;
    gOmmData->object->cappy.rot_y = 0;
    gOmmData->object->cappy.rot_z = 0;
    gOmmData->object->cappy.scale = 0.f;
    gOmmData->object->cappy.o_gfx = false;
    gOmmData->object->cappy.post_update = NULL;

    // Door data
    gOmmData->object->door.vel[0][0] = 0.f;
    gOmmData->object->door.vel[0][1] = 0.f;
    gOmmData->object->door.vel[1][0] = 0.f;
    gOmmData->object->door.vel[1][1] = 0.f;
    gOmmData->object->door.timer = 0;
}

static void omm_data_reset_allow() {
    gOmmData->allow->captures = true;
    gOmmData->allow->capModifier = false;
    gOmmData->allow->yoshiSummon = false;
    gOmmData->allow->vibes = false;
    gOmmData->allow->joyVibe = false;
}

static void omm_data_reset_level() {
    gRedCoinsCollected = 0;
    for (s32 areaIndex = 0; areaIndex != 8; ++areaIndex) {
        gOmmData->level[areaIndex].redCoins = 0;
        gOmmData->level[areaIndex].redCoinStarIndex = -1;
        gOmmData->level[areaIndex].secrets = 0;
        gOmmData->level[areaIndex].secretStarIndex = -1;
    }
}

OMM_ROUTINE_LEVEL_ENTRY(omm_data_reset) {
    omm_data_reset_mario();
    omm_data_reset_object();
    omm_data_reset_allow();
    omm_data_reset_level();
#if OMM_GAME_IS_R96X
    gOmmData->globals->milkTimer = 0;
#endif
}

//
// Init
//

OMM_AT_STARTUP void omm_data_init() {
    OMM_DO_ONCE {
        gOmmData->reset = omm_data_reset;
        gOmmData->reset_mario = omm_data_reset_mario;
        gOmmData->reset_object = omm_data_reset_object;
        gOmmData->reset();
        gOmmData->globals->yoshiMode = false;
#if OMM_GAME_IS_SMSR
        gOmmData->globals->booZeroLife = false;
#endif
        gOmmData->globals->cameraSnapshotMode = false;
        gOmmData->globals->cameraUpdate = false;
        gOmmData->globals->cameraNoInit = false;
        gOmmData->globals->configNoSave = false;
        gOmmData->globals->findFloorForCutsceneStar = false;
        gOmmData->globals->hideHudCamera = false;
        gOmmData->globals->hideHudRadar = false;
#if OMM_GAME_IS_SM64
        gOmmData->globals->isMirrorObj = false;
        gOmmData->globals->isMirrorRoom = false;
        gOmmData->globals->mirrorRoomX = 1700.f;
        gOmmData->globals->mirrorX = 4331.5f;
#endif
#if OMM_GAME_IS_R96X
        gOmmData->globals->milkTimer = 0;
#endif
        gOmmData->globals->marioTimer = 0;
        gOmmData->globals->mouseDeltaX = 0;
        gOmmData->globals->mouseDeltaY = 0;
        gOmmData->globals->mouseWheelX = 0;
        gOmmData->globals->mouseWheelY = 0;
        gOmmData->globals->instantWarp.warped = false;
    }
}

//
// Update
//

static void omm_data_update_mario() {
    struct MarioState *m = gMarioState;

    // State data
    gOmmData->mario->state.previous.pos[0] = m->pos[0];
    gOmmData->mario->state.previous.pos[1] = m->pos[1];
    gOmmData->mario->state.previous.pos[2] = m->pos[2];
    gOmmData->mario->state.previous.vel[0] = m->vel[0];
    gOmmData->mario->state.previous.vel[1] = m->vel[1];
    gOmmData->mario->state.previous.vel[2] = m->vel[2];
    gOmmData->mario->state.previous.vel[3] = m->forwardVel;
    gOmmData->mario->state.previous.angle[0] = m->faceAngle[0];
    gOmmData->mario->state.previous.angle[1] = m->faceAngle[1];
    gOmmData->mario->state.previous.angle[2] = m->faceAngle[2];

    // Peach-only data
    if (!OMM_PLAYER_IS_PEACH) {
        gOmmData->mario->peach.floated = false;
        gOmmData->mario->peach.floatTimer = 0;
        gOmmData->mario->peach.vibeType = OMM_PEACH_VIBE_TYPE_NONE;
        gOmmData->mario->peach.vibeGauge = 0;
        gOmmData->mario->peach.vibeTimer = OMM_PEACH_VIBE_COOLDOWN;
        gOmmData->mario->peach.vibeGfxTimer = 0;
        gOmmData->mario->peach.joySpinYaw = 0;
    }

    // Perry data
    if (OMM_PERRY_IS_AVAILABLE) {
        if (!omm_perry_get_object()) {
            gOmmData->mario->peach.perry = NULL;
        }
    } else {
        gOmmData->mario->peach.perryCharge = 0;
        gOmmData->mario->peach.perryBlast = false;
        gOmmData->mario->peach.perry = NULL;
    }

    // Cappy data
    if (!omm_cappy_get_object()) {
        gOmmData->mario->cappy.cappy = NULL;
    }

    // Capture data
    if (!omm_mario_is_capture(m)) {
        if (gOmmData->mario->capture.obj) {
            gOmmData->mario->capture.obj->oCaptureData = NULL;
            gOmmData->mario->capture.obj = NULL;
        }
        if (!(m->action & ACT_FLAG_AIR)) {
            gOmmData->mario->capture.prev = NULL;
        } else if (gOmmData->mario->capture.prev) {
            struct Object *o = gOmmData->mario->capture.prev;
            if (!o->activeFlags) {
                gOmmData->mario->capture.prev = NULL;
            } else {
                o->oFloorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &o->oFloor);
                if (obj_is_on_ground(o)) {
                    gOmmData->mario->capture.prev = NULL;
                }
            }
        }
    }

    // Warp data
    if (omm_is_main_menu() || omm_is_ending_cutscene()) {
#if OMM_GAME_IS_SMSR
        if (gCurrLevelNum == LEVEL_ZERO_LIFE &&
            gOmmData->mario->warp.state == POBJ_WARP_STATE_WARPING &&
            gOmmData->mario->warp.behavior == bhvBoo
        ) {
            gOmmData->globals->booZeroLife = true;
        }
#endif
        mem_zero(&gOmmData->mario->warp, sizeof(gOmmData->mario->warp));
    }
}

static void omm_data_update_object() {
}

static void omm_data_update_stats() {
    if (gMarioObject && !omm_is_game_paused()) {
        struct MarioState *m = gMarioState;

        // A presses
        omm_stats_increase(aPresses, (gPlayer1Controller->buttonPressed & A_BUTTON) != 0);

        // Actions
        // Only checks Mario's action; in SMO, capture actions (jumps) don't count as Mario actions
        static u32 sPrevAction = 0;
        if (m->action != sPrevAction) {
            omm_stats_increase(jumps, omm_mario_is_jumping(m));
            omm_stats_increase(attacks, omm_mario_is_attacking(m));
        }
        sPrevAction = m->action;

        // Timers
        if (omm_mario_is_capture(m) && gOmmData->mario->capture.obj) {
            omm_stats_increase(timeTotal[1],      1);
            omm_stats_increase(timeOnGround[1],   obj_is_on_ground(gOmmData->mario->capture.obj));
            omm_stats_increase(timeAirborne[1],   !obj_is_on_ground(gOmmData->mario->capture.obj));
            omm_stats_increase(timeUnderwater[1], obj_is_underwater(gOmmData->mario->capture.obj, find_water_level(gOmmData->mario->capture.obj->oPosX, gOmmData->mario->capture.obj->oPosZ)));
            omm_stats_increase(timeWingCap[1],    (m->flags & MARIO_WING_CAP) != 0);
            omm_stats_increase(timeMetalCap[1],   (m->flags & MARIO_METAL_CAP) != 0);
            omm_stats_increase(timeVanishCap[1],  (m->flags & MARIO_VANISH_CAP) != 0);
        } else {
            bool underwater = (m->action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)) != 0;
            bool airborne   = (m->action & ACT_FLAG_AIR) != 0;
            omm_stats_increase(timeTotal[0],      1);
            omm_stats_increase(timeOnGround[0],   !underwater && !airborne);
            omm_stats_increase(timeAirborne[0],   !underwater && airborne);
            omm_stats_increase(timeUnderwater[0], underwater);
            omm_stats_increase(timeWingCap[0],    (m->flags & MARIO_WING_CAP) != 0);
            omm_stats_increase(timeMetalCap[0],   (m->flags & MARIO_METAL_CAP) != 0);
            omm_stats_increase(timeVanishCap[0],  (m->flags & MARIO_VANISH_CAP) != 0);
        }
    }
}

static void omm_data_update_globals() {
    gMouseDeltaX = gOmmData->globals->mouseDeltaX * (BETTER_CAM_IS_PUPPY_CAM ? 1 : -1);
    gMouseDeltaY = gOmmData->globals->mouseDeltaY * (BETTER_CAM_IS_PUPPY_CAM ? 1 : -1);
    if (gOmmData->globals->marioTimer) {
        gPlayer1Controller->buttonPressed &= ~START_BUTTON;
        gPlayer2Controller->buttonPressed &= ~START_BUTTON;
        gPlayer3Controller->buttonPressed &= ~START_BUTTON;
    }
}

OMM_ROUTINE_UPDATE(omm_data_update) {
    omm_data_update_mario();
    omm_data_update_object();
    omm_data_update_stats();
    omm_data_update_globals();
}

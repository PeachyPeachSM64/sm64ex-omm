#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

struct OmmData *gOmmData = NULL;
s32 *gOmmPeachVibeTypeRef = NULL;

static void omm_data_reset_mario_fields() {

    // State data
    gOmmData->mario->state.hp = 0;
    gOmmData->mario->state.coins = 0;
    gOmmData->mario->state.o2 = 0;
    gOmmData->mario->state.airCombo = 0;
    gOmmData->mario->state.poleTimer = 0;
    gOmmData->mario->state.peakHeight = -11000.f;
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

    // Peach-only data
    gOmmData->mario->peach.floated = false;
    gOmmData->mario->peach.floatTimer = 0;
    gOmmData->mario->peach.vibeType = OMM_PEACH_VIBE_TYPE_NONE;
    gOmmData->mario->peach.vibeGauge = 0;
    gOmmData->mario->peach.vibeTimer = OMM_PEACH_VIBE_COOLDOWN;
    gOmmData->mario->peach.joySpinYaw = 0;
    gOmmData->mario->peach.perryCharge = 0;
    gOmmData->mario->peach.perryBlast = false;
    gOmmData->mario->peach.perry = NULL;

    // Wall-slide moves data
    gOmmData->mario->wallSlide.jumped = false;
    gOmmData->mario->wallSlide.angle = 0;
    gOmmData->mario->wallSlide.height = +20000.f;

    // Spin moves data
    gOmmData->mario->spin.yaw = 0;
    gOmmData->mario->spin.timer = 0;

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
    gOmmData->mario->capture.data = NULL;
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
    gOmmData->mario->capture.stickX = 0.f;
    gOmmData->mario->capture.stickY = 0.f;
    gOmmData->mario->capture.stickMag = 0.f;
    gOmmData->mario->capture.stickYaw = 0;
    gOmmData->mario->capture.buttonPressed = 0;
    gOmmData->mario->capture.buttonDown = 0;
#if OMM_CODE_DEBUG
    gOmmData->mario->capture.hitboxRadius = 0.f;
    gOmmData->mario->capture.hitboxHeight = 0.f;
    gOmmData->mario->capture.hitboxOffset = 0.f;
    gOmmData->mario->capture.hitboxWall = 0.f;
#endif
}

static void omm_data_reset_object_fields() {

    // State data
    gOmmData->object->state.properties = 0;
    gOmmData->object->state.actionFlag = false;
    gOmmData->object->state.actionState = 0;
    gOmmData->object->state.actionTimer = 0;
    gOmmData->object->state.squishTimer = 0;
    gOmmData->object->state.bullyTimer = 0;
    gOmmData->object->state.invincTimer = 0;
    gOmmData->object->state.walkDistance = 0.f;
    gOmmData->object->state.initialPos[0] = 0.f;
    gOmmData->object->state.initialPos[1] = 0.f;
    gOmmData->object->state.initialPos[2] = 0.f;
    gOmmData->object->state.camBehindMario = false;

    // Cappy data
    gOmmData->object->cappy.copyGfx = false;
    gOmmData->object->cappy.offset[0] = 0.f;
    gOmmData->object->cappy.offset[1] = 0.f;
    gOmmData->object->cappy.offset[2] = 0.f;
    gOmmData->object->cappy.angle[0] = 0;
    gOmmData->object->cappy.angle[1] = 0;
    gOmmData->object->cappy.angle[2] = 0;
    gOmmData->object->cappy.scale = 1.f;
}

static void omm_data_update_mario_fields() {

    // State data
    gOmmData->mario->state.previous.pos[0] = gMarioState->pos[0];
    gOmmData->mario->state.previous.pos[1] = gMarioState->pos[1];
    gOmmData->mario->state.previous.pos[2] = gMarioState->pos[2];
    gOmmData->mario->state.previous.vel[0] = gMarioState->vel[0];
    gOmmData->mario->state.previous.vel[1] = gMarioState->vel[1];
    gOmmData->mario->state.previous.vel[2] = gMarioState->vel[2];
    gOmmData->mario->state.previous.vel[3] = gMarioState->forwardVel;
    gOmmData->mario->state.previous.angle[0] = gMarioState->faceAngle[0];
    gOmmData->mario->state.previous.angle[1] = gMarioState->faceAngle[1];
    gOmmData->mario->state.previous.angle[2] = gMarioState->faceAngle[2];

    // Peach-only data
    if (OMM_PLAYER_IS_PEACH) {
        if (!omm_peach_get_perry_object()) {
            gOmmData->mario->peach.perry = NULL;
        }
    } else {
        gOmmData->mario->peach.floated = false;
        gOmmData->mario->peach.floatTimer = 0;
        gOmmData->mario->peach.vibeType = OMM_PEACH_VIBE_TYPE_NONE;
        gOmmData->mario->peach.vibeGauge = 0;
        gOmmData->mario->peach.vibeTimer = OMM_PEACH_VIBE_COOLDOWN;
        gOmmData->mario->peach.joySpinYaw = 0;
        gOmmData->mario->peach.perryCharge = 0;
        gOmmData->mario->peach.perryBlast = false;
        gOmmData->mario->peach.perry = NULL;
    }

    // Cappy data
    if (!omm_cappy_get_object()) {
        gOmmData->mario->cappy.cappy = NULL;
    }

    // Capture data
    if (gMarioState->action != ACT_OMM_POSSESSION) {
        gOmmData->mario->capture.data = NULL;
        gOmmData->mario->capture.obj = NULL;
        if (!(gMarioState->action & ACT_FLAG_AIR)) {
            gOmmData->mario->capture.prev = NULL;
        } else if (gOmmData->mario->capture.prev) {
            struct Object *o = gOmmData->mario->capture.prev;
            o->oFloorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &o->oFloor);
            if (obj_is_on_ground(o)) {
                gOmmData->mario->capture.prev = NULL;
            }
        }
    }
}

static void omm_data_update_object_fields() {
}

static void omm_data_update_fields() {
    omm_data_update_mario_fields();
    omm_data_update_object_fields();
}

OMM_ROUTINE_LEVEL_ENTRY(omm_data_reset_fields) {
    omm_data_reset_mario_fields();
    omm_data_reset_object_fields();
}

OMM_AT_STARTUP static void omm_data_init() {
    gOmmData = OMM_MEMNEW(struct OmmData, 1);
    gOmmData->reset = omm_data_reset_fields;
    gOmmData->resetMario = omm_data_reset_mario_fields;
    gOmmData->resetObject = omm_data_reset_object_fields;
    gOmmData->update = omm_data_update_fields;
    gOmmData->updateMario = omm_data_update_mario_fields;
    gOmmData->updateObject = omm_data_update_object_fields;
    gOmmData->reset();
    gOmmPeachVibeTypeRef = &gOmmData->mario->peach.vibeType;
}

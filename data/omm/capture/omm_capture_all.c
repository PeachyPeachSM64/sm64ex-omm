#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Capture Data
// These values are for an Object of scale 1.0f
//

typedef struct {
    const BehaviorScript *behavior;
    bool (*init)(struct Object *);
    s32 (*update)(struct Object *);
    void (*end)(struct Object *);
    f32 (*getTop)(struct Object *);
    f32 walkSpeed;
    f32 runSpeed;
    f32 dashSpeed;
    f32 jumpVelocity;
    f32 terminalVelocity;
    f32 gravity;
    f32 hitboxRadius;
    f32 hitboxHeight;
    f32 hitboxDownOffset;
    f32 wallHitboxRadius;
    bool referenceObject; // if true, reference the object as previous capture after leaving it
} OmmCaptureData;

#define ID(id) cappy_##id##_init, cappy_##id##_update, cappy_##id##_end, cappy_##id##_get_top
const OmmCaptureData sOmmCaptureData[] = {
    /*                                                           walk,   run,  dash,  jump,   fall,   grav,   rad,   hgt,  down,  wall  ref */
    { bhvGoomba,                    ID(goomba),                  14.f,  28.f,  28.f,  20.f,  -40.f, -2.65f,  50.f,  65.f,   0.f,  30.f, true },
#if OMM_GAME_IS_SMMS
    { bhvgoombone,                  ID(goomba),                  14.f,  28.f,  28.f,  20.f,  -40.f, -2.65f,  50.f,  65.f,   0.f,  30.f, true },
#endif
#if OMM_GAME_IS_SMSR
    { bhvCustomSMSRShyGuy,          ID(goomba),                  14.f,  28.f,  28.f,  20.f,  -40.f, -2.65f,  50.f,  65.f,   0.f,  30.f, true },
#endif
    { bhvKoopa,                     ID(koopa),                   18.f,  27.f,  36.f,  25.f,  -48.f, -2.65f,  60.f,  40.f,   0.f,  40.f, true },
    { bhvKoopaShell,                ID(koopa_shell),             40.f,  60.f,  60.f,  42.f,  -75.f, -4.00f,  90.f, 100.f,   0.f,  60.f, true },
    { bhvBobomb,                    ID(bobomb),                  24.f,  24.f,  24.f,  32.f,  -60.f, -4.00f,  60.f, 100.f,   0.f,  40.f, true },
    { bhvBobombBuddyOpensCannon,    ID(bobomb_buddy),             0.f,   0.f,   0.f,   0.f,    0.f,  0.00f,  60.f, 100.f,   0.f,  40.f, false },
    { bhvChainChomp,                ID(chain_chomp),             16.f,  16.f,  64.f,  24.f,  -36.f, -2.00f, 100.f, 160.f,   0.f,  80.f, true },
    { omm_bhv_chain_chomp_free,     ID(chain_chomp),             16.f,  16.f,  64.f,  24.f,  -36.f, -2.00f, 100.f, 160.f,   0.f,  80.f, true },

    { bhvBulletBill,                ID(bullet_bill),            100.f, 100.f, 200.f,   0.f,    0.f,  0.00f, 200.f, 400.f, 200.f, 200.f, true },
    { bhvHoot,                      ID(hoot),                    24.f,  24.f,  20.f,  20.f,  -16.f, -2.00f,  60.f, 100.f,   0.f,  40.f, true },
    { bhvSmallWhomp,                ID(whomp),                   16.f,  16.f,  16.f,  28.f,  -80.f, -4.00f, 100.f, 400.f,   0.f, 150.f, true },

    { bhvPenguinBaby,               ID(penguin_small),           16.f,  16.f,  48.f,  25.f,  -60.f, -3.60f,  40.f,  60.f,   0.f,  30.f, true },
    { bhvSmallPenguin,              ID(penguin_small),           16.f,  16.f,  48.f,  25.f,  -60.f, -3.60f,  40.f,  60.f,   0.f,  30.f, true },
    { bhvUnused20E0,                ID(penguin_small),           16.f,  16.f,  48.f,  25.f,  -60.f, -3.60f,  40.f,  60.f,   0.f,  30.f, true },
    { bhvSpindrift,                 ID(spindrift),               24.f,  24.f,  24.f,  36.f,  -60.f, -4.00f,  80.f, 160.f,   0.f,  40.f, true },
    { bhvMrBlizzard,                ID(mr_blizzard),             20.f,  20.f,  20.f,  50.f,  -75.f, -4.00f,  80.f, 200.f,   0.f,  60.f, true },
    { bhvSnowmansBottom,            ID(snowmans_body),           80.f,  80.f,  80.f,   0.f,  -90.f, -6.00f, 200.f, 400.f,   0.f, 200.f, true },

    { bhvBoo,                       ID(boo),                     24.f,  48.f,  48.f,  20.f,  -20.f, -2.00f,  80.f,  80.f,   0.f,  60.f, true },
    { bhvGhostHuntBoo,              ID(boo),                     24.f,  48.f,  48.f,  20.f,  -20.f, -2.00f,  80.f,  80.f,   0.f,  60.f, true },
    { bhvMerryGoRoundBoo,           ID(boo),                     24.f,  48.f,  48.f,  20.f,  -20.f, -2.00f,  80.f,  80.f,   0.f,  60.f, true },
    { bhvBooWithCage,               ID(boo),                     24.f,  48.f,  48.f,  20.f,  -20.f, -2.00f,  80.f,  80.f,   0.f,  60.f, true },
    { bhvScuttlebug,                ID(scuttlebug),              20.f,  40.f,  40.f,  30.f,  -60.f, -4.00f,  80.f,  90.f,   0.f,  60.f, true },
    { bhvMadPiano,                  ID(mad_piano),               20.f,  20.f,  20.f,  14.f,  -80.f, -2.00f, 200.f, 150.f,   0.f, 200.f, true },
    { bhvJumpingBox,                ID(crazy_box),               72.f,  72.f,  72.f, 120.f, -150.f, -8.00f, 150.f, 200.f,   0.f, 120.f, false },
    { bhvMrI,                       ID(mr_i),                    28.f,  28.f,  28.f,  28.f,  -40.f, -2.00f,  90.f, 180.f,   0.f,  65.f, true },
    
    { bhvSwoop,                     ID(swoop),                   20.f,  40.f,  40.f,  20.f,  -20.f, -2.00f,  50.f,  60.f,   0.f,  30.f, true },
    { bhvSnufit,                    ID(snufit),                  25.f,  25.f,  25.f,  36.f,  -60.f, -3.00f,  60.f, 100.f,   0.f,  35.f, true },
    { bhvMontyMole,                 ID(monty_mole),              15.f,  15.f,  15.f,   0.f,    0.f,  0.00f,  50.f,  50.f,   0.f,  30.f, true },
    
    { bhvSmallBully,                ID(bully),                   28.f,  28.f,  56.f,  36.f,  -75.f, -4.00f,  75.f, 120.f,   0.f,  50.f, true },
    { bhvSmallChillBully,           ID(bully),                   28.f,  28.f,  56.f,  36.f,  -75.f, -4.00f,  75.f, 120.f,   0.f,  50.f, true },
#if OMM_GAME_IS_R96A
    { bhvBlargg,                    ID(blargg),                  15.f,  15.f,  15.f,   0.f,    0.f,  0.00f, 120.f, 100.f,   0.f,  70.f, true },
    { bhvFriendlyBlargg,            ID(friendly_blargg),         20.f,  30.f,  30.f,   0.f,    0.f,  0.00f,  60.f,  50.f,   0.f,  35.f, true },
    { bhvMotos,                     ID(motos),                    8.f,   8.f,   8.f,  10.f,  -75.f, -1.50f,  50.f, 110.f,   0.f,  30.f, true },
#endif
    
    { bhvToadMessage,               ID(toad),                    16.f,  32.f,  32.f,  25.f,  -60.f, -3.60f,  60.f, 100.f,   0.f,  30.f, false },

    { omm_bhv_flaming_bobomb,       ID(flaming_bobomb),          24.f,  24.f,  24.f,  30.f,  -60.f, -3.60f,  50.f,  90.f,   0.f,  40.f, false },
    
    /*                                                                                               scale,   fwd,   top,               ref */
    { bhvBobombBuddy,               ID(npc_message),              0.f,   0.f,   0.f,   0.f,    0.f,  1.20f,   0.f,  94.f,   0.f,   0.f, false },
#if OMM_GAME_IS_SMSR
    { bhvYoshi,                     ID(npc_message),              0.f,   0.f,   0.f,   0.f,    0.f,  1.00f,  60.f, 200.f,   0.f,   0.f, false },
    { bhvCustomSMSRPeachMessage,    ID(npc_message),              0.f,   0.f,   0.f,   0.f,    0.f,  0.60f, -10.f, 180.f,   0.f,   0.f, false },
    { bhvCustomSMSRYoshiMessage,    ID(npc_message),              0.f,   0.f,   0.f,   0.f,    0.f,  1.00f,  60.f, 200.f,   0.f,   0.f, false },
    { bhvCustomSMSRStaticMessage,   ID(npc_message),              0.f,   0.f,   0.f,   0.f,    0.f,  1.25f,   8.f,  50.f,   0.f,   0.f, false },
    { bhvCustomSMSRMipsMessage,     ID(npc_message),              0.f,   0.f,   0.f,   0.f,    0.f,  0.50f, -12.f,  65.f,   0.f,   0.f, false },
#endif
};
static const s32 sOmmCaptureDataCount = sizeof(sOmmCaptureData) / sizeof(sOmmCaptureData[0]);
#undef ID

const void *omm_capture_get_data(struct Object *o) {
    for (s32 i = 0; i != sOmmCaptureDataCount; ++i) {
        if (o->behavior == sOmmCaptureData[i].behavior) {
            return (const void *) &sOmmCaptureData[i];
        }
    }
    return NULL;
}

bool omm_capture_init(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return false;
    return data->init(o);
}

s32 omm_capture_update(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0;
    return data->update(o);
}

void omm_capture_end(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return;
    return data->end(o);
}

f32 omm_capture_get_top(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->getTop(o);
}

f32 omm_capture_get_walk_speed(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->walkSpeed * (o != NULL ? o->oScaleX : 1.f);
}

f32 omm_capture_get_run_speed(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->runSpeed * (o != NULL ? o->oScaleX : 1.f);
}

f32 omm_capture_get_dash_speed(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->dashSpeed * (o != NULL ? o->oScaleX : 1.f);
}

f32 omm_capture_get_jump_velocity(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->jumpVelocity * (o != NULL ? o->oScaleY : 1.f);
}

f32 omm_capture_get_terminal_velocity(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->terminalVelocity * (o != NULL ? o->oScaleY : 1.f);
}

f32 omm_capture_get_gravity(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->gravity * (o != NULL ? o->oScaleY : 1.f);
}

f32 omm_capture_get_hitbox_radius(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->hitboxRadius * (o != NULL ? o->oScaleX : 1.f);
}

f32 omm_capture_get_hitbox_height(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->hitboxHeight * (o != NULL ? o->oScaleY : 1.f);
}

f32 omm_capture_get_hitbox_down_offset(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->hitboxDownOffset * (o != NULL ? o->oScaleY : 1.f);
}

f32 omm_capture_get_wall_hitbox_radius(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data) return 0.f;
    return data->wallHitboxRadius * (o != NULL ? o->oScaleX : 1.f);
}

bool omm_capture_should_reference_object(struct Object *o) {
    const OmmCaptureData *data = (const OmmCaptureData *) gOmmData->mario->capture.data;
    if (!data || !o) return false;
    return data->referenceObject;
}

void omm_capture_set_camera_behind_mario() {
    gOmmData->object->state.camBehindMario = true;
    if (gMarioState->area->camera->mode != CAMERA_MODE_BEHIND_MARIO) {
        if (BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM) {
            gMarioState->area->camera->mode = BETTER_CAM_MODE;
            gLakituState.mode = BETTER_CAM_MODE;
        } else {
            set_camera_mode(gMarioState->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
        }
    }
}

void omm_capture_reset_camera() {
    gOmmData->object->state.camBehindMario = false;
    if (gMarioState->area->camera->mode == CAMERA_MODE_BEHIND_MARIO) {
        if (BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM) {
            gMarioState->area->camera->mode = BETTER_CAM_MODE;
            gLakituState.mode = BETTER_CAM_MODE;
        } else {
            set_camera_mode(gMarioState->area->camera, gMarioState->area->camera->defMode, 1);
        }
    }
}

//
// Possessed object stuff
//

void pobj_move(struct Object *o, bool run, bool dash, bool stop) {
    if (gOmmData->mario->capture.stickMag > 0.f) {
        f32 magVel = gOmmData->mario->capture.stickMag;
        f32 maxVel = (stop ? 0.f : (dash ? omm_capture_get_dash_speed(o) : (run ? omm_capture_get_run_speed(o) : omm_capture_get_walk_speed(o))));
        f32 minVel = maxVel / 8.f;
        f32 tgtVel = maxVel * magVel;
        if (tgtVel >= o->oForwardVel) {
            o->oForwardVel += ((maxVel / 8.f) * (1.f - (max_f(0.f, o->oForwardVel) / tgtVel)));
        } else {
            gOmmData->mario->capture.stickMag = 0.f;
        }
        s32 handling = (OMM_CHEAT_SUPER_RESPONSIVE ? 0x8000 : (lerp_f(sqr_f(1.f - invlerp_0_1_f(o->oForwardVel, minVel, maxVel)), 0x1000, 0x4000) * magVel));
        s32 faceYaw = gOmmData->mario->capture.stickYaw - approach_s32((s16) (gOmmData->mario->capture.stickYaw - o->oFaceAngleYaw), 0, handling, handling);
        obj_set_forward_vel(o, faceYaw, 1.f, o->oForwardVel);
        o->oFaceAngleYaw = faceYaw;
        o->oMoveAngleYaw = faceYaw;
    }
}

void pobj_move_3d(struct Object *o, bool run, bool dash) {
    s32 facePitch = (s32) (gOmmData->mario->capture.stickY * 0x3000) - approach_s32((s16) ((s32) (gOmmData->mario->capture.stickY * 0x3000) - o->oFaceAnglePitch), 0, 0x300, 0x300);
    s32 faceYaw = o->oFaceAngleYaw - (s32) (gOmmData->mario->capture.stickX * 0x200);
    f32 maxVel = (dash ? omm_capture_get_dash_speed(o) : (run ? omm_capture_get_run_speed(o) : omm_capture_get_walk_speed(o)));
    f32 xyzVel = sqrtf(sqr_f(o->oVelX) + sqr_f(o->oVelY) + sqr_f(o->oVelZ));
    xyzVel += ((maxVel / 8.f) * (1.f - (max_f(0.f, xyzVel) / maxVel)));
    o->oVelX = xyzVel * +coss(facePitch) * sins(faceYaw);
    o->oVelY = xyzVel * -sins(facePitch);
    o->oVelZ = xyzVel * +coss(facePitch) * coss(faceYaw);
    o->oFaceAngleYaw = faceYaw;
    o->oFaceAnglePitch = facePitch;
    o->oMoveAngleYaw = faceYaw;
    o->oMoveAnglePitch = facePitch;
}

// If hopDiv is != 0, the object hops in place while moving, and jumps when A is held
// If numMaxJumps is > 1, the object can start a jump while being airborne
s32 pobj_jump(struct Object *o, f32 hopDiv, s32 numMaxJumps) {
    if (hopDiv != 0.f) {
        if (obj_is_on_ground(o)) {
            if (POBJ_A_BUTTON_DOWN) {
                o->oVelY = max_f(o->oVelY, omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER);
                o->oFloor = NULL;
                return POBJ_RESULT_HOP_LARGE;
            }
            if (o->oForwardVel > (omm_capture_get_walk_speed(o) / 8.f)) {
                o->oVelY = max_f(o->oVelY, omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER / hopDiv);
                o->oFloor = NULL;
                return POBJ_RESULT_HOP_SMALL;
            }
        }
    } else {
        static s32 sJumps = 0;
        static s32 sFrames = 0;
        sJumps *= !obj_is_on_ground(o);
        if (POBJ_A_BUTTON_PRESSED) {
            if (sJumps < numMaxJumps && (obj_is_on_ground(o) || numMaxJumps > 1)) {
                sJumps++;
                sFrames = 0;
                o->oVelY = max_f(o->oVelY, omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER);
                o->oFloor = NULL;
                return POBJ_RESULT_JUMP_START;
            }
        }
        if (POBJ_A_BUTTON_DOWN) {
            if (++sFrames < 6) {
                o->oVelY = max_f(o->oVelY, omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER);
                o->oFloor = NULL;
                return POBJ_RESULT_JUMP_HOLD;
            }
        }
        sFrames = 6;
    }
    return POBJ_RESULT_NONE;
}

void pobj_decelerate(struct Object *o, f32 decel, f32 decelIce) {
    if (gOmmData->mario->capture.stickMag == 0.f) {
        if (obj_is_on_ground(o) && o->oFloor->type == SURFACE_ICE) {
            o->oVelX *= decelIce;
            o->oVelZ *= decelIce;
            o->oForwardVel *= decelIce;
        } else {
            o->oVelX *= decel;
            o->oVelZ *= decel;
            o->oForwardVel *= decel;
        }
    }
}

void pobj_apply_gravity(struct Object *o, f32 mult) {
    if (OMM_CHEAT_MOON_JUMP && (gMarioState->controller->buttonDown & L_TRIG)) {
        o->oVelY = +25.f;
    } else {
        o->oVelY += (omm_capture_get_gravity(o) * mult);
    }
    if (o->oVelY < (omm_capture_get_terminal_velocity(o) * mult)) {
        o->oVelY = (omm_capture_get_terminal_velocity(o) * mult);
    }
}

void pobj_handle_special_floors(struct Object *o) {
    o->oFloorType = OBJ_FLOOR_TYPE_NONE;

    // Out of bounds
    if (o->oFloor == NULL) {
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
        obj_destroy(o);
        return;
    }

    // Squished
    if (gOmmData->object->state.squishTimer >= 6) {
        set_camera_shake_from_hit(SHAKE_SMALL_DAMAGE);
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_KNOCKED_BACK, true, 15);
        return;
    }

    // On ground
    bool isOnGround = obj_is_on_ground(o);
    if (isOnGround) {
        o->oFloorType = OBJ_FLOOR_TYPE_GROUND;
    }

    // Above water
    f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);
    if (!obj_is_underwater(o, waterLevel) && !POBJ_IS_ABOVE_WATER) {
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
        obj_destroy(o);
        return;
    }

    // Walk/Slide on water
    if (o->oPosY <= waterLevel && POBJ_IS_ABLE_TO_MOVE_ON_WATER) {
        o->oPosY = waterLevel;
        o->oVelY = max_f(o->oVelY, 0.f);
        o->oFloorHeight = waterLevel;
        o->oFloorType = OBJ_FLOOR_TYPE_WATER;
    }

    // Underwater
    if (obj_is_underwater(o, waterLevel) && !POBJ_IS_UNDER_WATER) {
        o->oPosY = waterLevel + max_f(((o->hitboxHeight / 2) - o->hitboxDownOffset), 60.f);
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
        obj_destroy(o);
        return;
    }

    // Special floors
    switch (o->oFloor->type) {
        case SURFACE_DEATH_PLANE:
        case SURFACE_VERTICAL_WIND:
            if (o->oDistToFloor <= 2048.f) {
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
                obj_destroy(o);
            }
            break;

        case SURFACE_INSTANT_QUICKSAND:
        case SURFACE_INSTANT_MOVING_QUICKSAND:
            if (o->oDistToFloor <= 10.f && !POBJ_IS_IMMUNE_TO_SAND) {
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
                obj_destroy(o);
            }
            break;

        case SURFACE_BURNING:
            if (o->oDistToFloor <= 10.f && !OMM_CHEAT_WALK_ON_LAVA && !POBJ_IS_IMMUNE_TO_LAVA) {
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
                obj_destroy(o);
            } else if (isOnGround) {
                o->oFloorType = OBJ_FLOOR_TYPE_LAVA;
            }
            break;
    }
}

//
// Interactions
//

#define INT_STATUS_ATTACKED         (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED)
#define INT_STATUS_HIT_FROM_ABOVE   (ATTACK_FROM_ABOVE | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED)
#define INT_STATUS_HIT_FROM_BELOW   (ATTACK_FROM_BELOW | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED)

static struct Object *sPObj;
static struct Object *sObj;
static bool sOverlapHitbox;
static bool sOverlapHurtbox;

static bool pobj_push_out_of_object_hitbox() {
    if (sOverlapHitbox) {

        // Push
        f32 dx = sPObj->oPosX - sObj->oPosX;
        f32 dz = sPObj->oPosZ - sObj->oPosZ;
        f32 d = sqrtf(sqr_f(dx) + sqr_f(dz));
        if (d == 0.f) {
            // This is unlikely, but we must
            // prevent a div/0 error
            return false;
        }
        f32 diff = (sPObj->hitboxRadius + sObj->hitboxRadius) - d;
        f32 pushX = (dx / d) * diff;
        f32 pushZ = (dz / d) * diff;

        // Make all SURFACE and POLELIKE objects unpushable, as well as invulnerable ones
        if (obj_is_surface(sObj) || !POBJ_IS_INVULNERABLE) {
            sPObj->oPosX += pushX;
            sPObj->oPosZ += pushZ;
        } else {
            sObj->oPosX -= pushX;
            sObj->oPosZ -= pushZ;
        }
        return true;
    }
    return false;
}

static bool pobj_push_out_of_object_hurtbox() {
    if (sOverlapHurtbox) {

        // Push
        f32 dx = sPObj->oPosX - sObj->oPosX;
        f32 dz = sPObj->oPosZ - sObj->oPosZ;
        f32 d = sqrtf(sqr_f(dx) + sqr_f(dz));
        if (d == 0.f) {
            // This is unlikely, but we must
            // prevent a div/0 error
            return false;
        }
        f32 diff = (sPObj->hitboxRadius + (sObj->hurtboxRadius > 0 ? sObj->hurtboxRadius : sObj->hitboxRadius)) - d;
        f32 pushX = (dx / d) * diff;
        f32 pushZ = (dz / d) * diff;

        // Make all SURFACE and POLELIKE objects unpushable, as well as invulnerable ones
        if (obj_is_surface(sObj) || !POBJ_IS_INVULNERABLE) {
            sPObj->oPosX += pushX;
            sPObj->oPosZ += pushZ;
        } else {
            sObj->oPosX -= pushX;
            sObj->oPosZ -= pushZ;
        }
        return true;
    }
    return false;
}

static bool pobj_interact_coin() {
    if (sOverlapHitbox) {
        omm_mario_interact_coin(gMarioState, sObj);
        return true;
    }
    return false;
}

static bool pobj_interact_star_or_key() {
    if (sOverlapHitbox) {
        return omm_mario_interact_star_or_key(gMarioState, sObj);
    }
    return false;
}

static bool pobj_mario_take_damage_and_unpossess() {

    // No damage, push out of object
    if (sOverlapHurtbox) {
        if (sObj->oDamageOrCoinValue == 0 || POBJ_IS_INVULNERABLE) {
            return pobj_push_out_of_object_hurtbox();
        }
    }

    // Damage Mario
    if (sOverlapHurtbox) {
        sObj->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_ATTACKED_MARIO;
        gMarioState->hurtCounter += 4 * sObj->oDamageOrCoinValue;
        gMarioState->interactObj = sObj;
        set_camera_shake_from_hit(SHAKE_SMALL_DAMAGE + min_s(sObj->oDamageOrCoinValue / 2, 2));
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_KNOCKED_BACK, obj_is_object2_pushing_object1_backwards(sPObj, sObj, true), 15);
        return true;
    }

    return false;
}

static bool pobj_interact_bounce_top() {

    // Hit and bounce
    if (sOverlapHitbox) {
        if (obj_is_object2_hit_from_above(sPObj, sObj)) {
            sObj->oInteractStatus = INT_STATUS_HIT_FROM_ABOVE;
            sPObj->oPosY = sObj->oPosY + sObj->hitboxHeight - sObj->hitboxDownOffset + 20.f;
            sPObj->oVelY = omm_capture_get_jump_velocity(sPObj) * POBJ_JUMP_MULTIPLIER * 1.2f;
            play_sound(SOUND_ACTION_BOUNCE_OFF_OBJECT, sPObj->oCameraToObject);
            return true;
        }
    }

    // Attack
    if (sOverlapHitbox) {
        if (POBJ_IS_ATTACKING) {
            sObj->oInteractStatus = INT_STATUS_ATTACKED;
            return true;
        }
    }

    // Damage Mario
    if (sOverlapHurtbox) {
        return pobj_mario_take_damage_and_unpossess(sPObj, sObj, gMarioState);
    }

    return false;
}

static bool pobj_interact_hit_from_below() {

    // Hit from below
    if (sOverlapHitbox) {
        if (obj_is_object2_hit_from_below(sPObj, sObj)) {
            sObj->oInteractStatus = INT_STATUS_HIT_FROM_BELOW;
            sPObj->oVelY = 0;
            play_sound(SOUND_ACTION_BOUNCE_OFF_OBJECT, sPObj->oCameraToObject);
            set_camera_shake_from_hit(SHAKE_HIT_FROM_BELOW);
            return true;
        }
    }

    // Hit from above
    if (sOverlapHitbox) {
        return pobj_interact_bounce_top(sPObj, sObj, gMarioState);
    }

    return false;
}

static bool pobj_mario_burn_and_unpossess() {

    // Burn Mario
    if (sOverlapHurtbox) {

        // No damage if immune or underwater
        if (POBJ_IS_IMMUNE_TO_FIRE || obj_is_underwater(sPObj, find_water_level(sPObj->oPosX, sPObj->oPosZ))) {
            return false;
        }

        sObj->oInteractStatus = INT_STATUS_INTERACTED;
        gMarioState->interactObj = sObj;
        gMarioState->marioObj->oMarioBurnTimer = 0;
        play_sound(SOUND_MARIO_ON_FIRE, gMarioState->marioObj->oCameraToObject);
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BURNT, false, 15);
        return true;
    }

    return false;
}

static bool pobj_interact_bully() {

    // Bully
    if (sOverlapHitbox) {
        f32 knockback = 3600.f / sObj->hitboxRadius;
        s16 angle = obj_get_object1_angle_yaw_to_object2(sPObj, sObj);

        // Bully the bully if sPObj attacks/jumps or is invulnerable
        if (POBJ_IS_INVULNERABLE || POBJ_IS_ATTACKING || obj_is_object2_hit_from_above(sPObj, sObj)) {

            // sPObj
            omm_mario_lock(gMarioState, 6);
            sPObj->oFaceAngleYaw = angle;
            sPObj->oMoveAngleYaw = angle;
            obj_set_forward_vel(sPObj, sPObj->oFaceAngleYaw, 0.2f, -knockback);
            pobj_push_out_of_object_hitbox();

            // Bully
            sObj->oFaceAngleYaw = angle + 0x8000;
            sObj->oMoveAngleYaw = angle;
            sObj->oForwardVel = knockback;
            sObj->oInteractStatus = INT_STATUS_ATTACKED;
            play_sound(SOUND_OBJ_BULLY_METAL, sObj->oCameraToObject);

        } else {

            // If sPObj was bullied during the last 60 frames, eject Mario
            if (gOmmData->object->state.bullyTimer > 0) {
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_KNOCKED_BACK, obj_is_object2_pushing_object1_backwards(sPObj, sObj, true), 15);
                return true;
            }

            // sPObj
            gOmmData->object->state.bullyTimer = 60;
            omm_mario_lock(gMarioState, 30);
            sPObj->oFaceAngleYaw = angle;
            sPObj->oMoveAngleYaw = angle;
            obj_set_forward_vel(sPObj, sPObj->oFaceAngleYaw, 1.f, -knockback);
            pobj_push_out_of_object_hitbox();
            obj_play_sound(sPObj, sPObj->oDeathSound);

            // Bully
            sObj->oFaceAngleYaw = angle + 0x8000;
            sObj->oMoveAngleYaw = angle + 0x8000;
            sObj->oInteractStatus = INT_STATUS_INTERACTED;
            obj_play_sound(sObj, SOUND_OBJ_BULLY_METAL);
        }
        return true;
    }

    return false;
}

static bool pobj_interact_breakable() {

    // Hit from below
    if (sOverlapHitbox) {
        if (obj_is_object2_hit_from_below(sPObj, sObj)) {
            sObj->oInteractStatus = INT_STATUS_HIT_FROM_BELOW;
            sPObj->oVelY = 0;
            set_camera_shake_from_hit(SHAKE_HIT_FROM_BELOW);
            return true;
        }
    }

    // Attack
    if (sOverlapHitbox) {
        if (POBJ_IS_ATTACKING) {
            sObj->oInteractStatus = INT_STATUS_ATTACKED;
            set_camera_shake_from_hit(SHAKE_ATTACK);
            return true;
        }
    }

    // Push
    if (sOverlapHitbox) {
        return pobj_push_out_of_object_hitbox();
    }

    return false;
}

static bool pobj_interact_grabbable() {

    // Is attackable (bob-ombs)
    if (sOverlapHitbox) {
        if ((sObj->oInteractionSubtype & INT_SUBTYPE_KICKABLE) && POBJ_IS_ATTACKING) {
            sObj->oInteractStatus = INT_STATUS_ATTACKED;
            return false;
        }
    }

    // Is destructible (small boxes)
    if (sOverlapHitbox) {
        if (POBJ_IS_ATTACKING) {
            if (sObj->behavior == bhvJumpingBox ||
                sObj->behavior == bhvBreakableBoxSmall) {
                obj_destroy(sObj);
                return true;
            }
        }
    }

    // Grabs Mario out of his possession
    if (sOverlapHitbox) {
        if ((sObj->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO) && obj_is_object1_facing_object2(sObj, sPObj, 0x2800)) {
            sObj->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_GRABBED_MARIO;
            sPObj->oFaceAngleYaw = sObj->oMoveAngleYaw;
            gMarioState->interactObj = sObj;
            gMarioState->usedObj = sObj;
            play_sound(SOUND_MARIO_OOOF, gMarioState->marioObj->oCameraToObject);
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_GRABBED, false, 15);
            return true;
        }
    }

    // Damage Mario
    if (sOverlapHurtbox) {
        return pobj_mario_take_damage_and_unpossess(sPObj, sObj, gMarioState);
    }

    return false;
}

static bool pobj_interact_clam_or_bubba() {

    // Eats Mario and his possession
    if (sOverlapHurtbox) {
        if (sObj->oInteractionSubtype & INT_SUBTYPE_EATS_MARIO) {
            sObj->oInteractStatus = INT_STATUS_INTERACTED;
            gMarioState->interactObj = sObj;
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_EATEN, false, 15);
            obj_destroy(sPObj);
            return true;
        }
    }

    // Damage Mario
    if (sOverlapHurtbox) {
        return pobj_mario_take_damage_and_unpossess(sPObj, sObj, gMarioState);
    }

    return false;
}

static bool pobj_interact_strong_wind() {

    // Blow Mario away, out of his possession
    if (sOverlapHitbox) {

        // Delete the projectile if immune to strong winds
        if (POBJ_IS_IMMUNE_TO_WIND) {
            sObj->oIntangibleTimer = -1;
            obj_mark_for_deletion(sObj);
            return false;
        }

        sObj->oInteractStatus = INT_STATUS_INTERACTED;
        gMarioState->interactObj = sObj;
        gMarioState->usedObj = sObj;
        gMarioState->unkC4 = 0.4f;
        sPObj->oFaceAngleYaw = sObj->oMoveAngleYaw + 0x8000;
        play_sound(SOUND_MARIO_WAAAOOOW, gMarioState->marioObj->oCameraToObject);
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_BLOWN, false, 15);
        return true;
    }

    return false;
}

static bool pobj_interact_tornado() {

    // Suck Mario out of his possession
    if (sOverlapHitbox) {

        // Same flag used to resist tornados
        if (POBJ_IS_IMMUNE_TO_WIND) {
            return false;
        }

        sObj->oInteractStatus = INT_STATUS_INTERACTED;
        gMarioState->interactObj = sObj;
        gMarioState->usedObj = sObj;
        gMarioState->marioObj->oMarioTornadoYawVel = 0x400;
        gMarioState->marioObj->oMarioTornadoPosY = sPObj->oPosY - sObj->oPosY;
        play_sound(SOUND_MARIO_WAAAOOOW, gMarioState->marioObj->oCameraToObject);
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_TORNADO, false, 15);
        return true;
    }

    return false;
}

bool pobj_process_interaction(struct Object *o, struct Object *obj, u32 interactType) {

    // Set up statics
    sPObj = o;
    sObj = obj;
    sOverlapHitbox = obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX);
    sOverlapHurtbox = obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HURTBOX_HITBOX_IF_ZERO);

    // Interaction
    switch (interactType) {
        case INTERACT_COIN:
            return pobj_interact_coin();

        case INTERACT_STAR_OR_KEY:
            return pobj_interact_star_or_key();

        case INTERACT_SNUFIT_BULLET:
        case INTERACT_MR_BLIZZARD:
        case INTERACT_UNKNOWN_08:
        case INTERACT_SHOCK:
        case INTERACT_DAMAGE:
            return pobj_mario_take_damage_and_unpossess();

        case INTERACT_KOOPA:
        case INTERACT_BOUNCE_TOP:
        case INTERACT_BOUNCE_TOP2:
            return pobj_interact_bounce_top();

        case INTERACT_HIT_FROM_BELOW:
            return pobj_interact_hit_from_below();

        case INTERACT_FLAME:
            return pobj_mario_burn_and_unpossess();

        case INTERACT_BULLY:
            return pobj_interact_bully();

        case INTERACT_BREAKABLE:
            return pobj_interact_breakable();

        case INTERACT_GRABBABLE:
            return pobj_interact_grabbable();

        case INTERACT_CLAM_OR_BUBBA:
            return pobj_interact_clam_or_bubba();

        case INTERACT_STRONG_WIND:
            return pobj_interact_strong_wind();

        case INTERACT_TORNADO:
            return pobj_interact_tornado();

        case INTERACT_DOOR:
        case INTERACT_WARP_DOOR:
        case INTERACT_POLE:
        case INTERACT_IGLOO_BARRIER:
        case INTERACT_KOOPA_SHELL:
        case INTERACT_TEXT:
            if (sObj->behavior == bhvMessagePanel) {
                break; // Do nothing
            }
            return pobj_push_out_of_object_hitbox();

        case INTERACT_CAP:
        case INTERACT_WARP:
        case INTERACT_WATER_RING:
        case INTERACT_WHIRLPOOL:
        case INTERACT_BBH_ENTRANCE:
        case INTERACT_CANNON_BASE:
        default:
            break; // Do nothing
    }
    return false;
}

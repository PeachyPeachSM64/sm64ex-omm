#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAM_NUM_DIRS            (8 * (OMM_CAMERA_CLASSIC ? 1 : gOmmCameraMode))
#define OMM_CAM_DELTA_ANGLE         (0x10000 / OMM_CAM_NUM_DIRS)
#define OMM_CAM_DIST_NUM_MODES      5
#define OMM_CAM_DIST_MODE_MIN       0
#define OMM_CAM_DIST_MODE_MAX       (OMM_CAM_DIST_NUM_MODES - 1)
#define OMM_CAM_DIST_MODE_MEDIUM    (OMM_CAM_DIST_MODE_MAX / 2)
#define OMM_CAM_DIST_MODE_HIGH      (OMM_CAM_DIST_MODE_MEDIUM + 1)
#define OMM_CAM_DIST_MODE_LOW       (OMM_CAM_DIST_MODE_MEDIUM - 1)
static const f32 sOmmCameraDistances[OMM_CAM_DIST_NUM_MODES] = { 400.f, 800.f, 1200.f, 1600.f, 2000.f };

//
// Data
//

static s32 sOmmCamFpMode      = 0;
static s16 sOmmCamPitch       = 0x0C00;
static s16 sOmmCamPitchFp     = 0x0000;
static s16 sOmmCamPitchTarget = 0x0C00;
static s16 sOmmCamYaw         = 0x0000;
static s16 sOmmCamYawTarget   = 0x0000;
static s32 sOmmCamDistMode    = OMM_CAM_DIST_MODE_MEDIUM;
static f32 sOmmCamDistance    = 0.f;
static f32 sOmmCamPos[3];
static f32 sOmmCamFocus[3];

OMM_INLINE s16 approach(s16 value, s16 target, s16 inc) {
    return target - approach_s32((s16) (target - value), 0, inc, inc);
}

OMM_INLINE s16 get_nearest_dir_angle(s16 value) {
    s32 va = (s32) value + 0x10000;
    s32 da = OMM_CAM_DELTA_ANGLE;
    s32 a0 = (((va / da) + 0) * da);
    s32 a1 = (((va / da) + 1) * da);
    s32 d0 = abs_s(va - a0);
    s32 d1 = abs_s(va - a1);
    if (d0 == min_s(d0, d1)) return (s16) a0;
    return (s16) a1;
}

//
// States
//

bool omm_camera_is_available(struct MarioState *m) {
    return !omm_is_ending_cutscene() && (!OMM_CAMERA_CLASSIC || omm_cappy_get_object_play_as()) && (m->action != ACT_IN_CANNON);
}

s16 omm_camera_get_intended_yaw(struct MarioState *m) {

    // OMM cam
    if (omm_camera_is_available(m)) {
        return atan2s(-m->controller->stickY, m->controller->stickX) + sOmmCamYaw;
    }

    // Better cam (not Puppy cam)
    if (BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM && gLakituState.mode == BETTER_CAM_MODE) {
        return atan2s(-m->controller->stickY, m->controller->stickX) - BETTER_CAM_YAW + 0x4000;
    }

    // Lakitu/Puppy cam
    return atan2s(-m->controller->stickY, m->controller->stickX) + m->area->camera->yaw;
}

s32 omm_camera_get_relative_dist_mode() {
    return sOmmCamDistMode - OMM_CAM_DIST_MODE_MEDIUM;
}

//
// Init
//

#if OMM_GAME_IS_SM64
// Level, Area, Pitch, Yaw, Distance
static const s16 sOmmCameraInitPresets[][5] = {
    { LEVEL_BITDW,   1, 0x1000, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_BITFS,   1, 0x1000, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_BITS,    1, 0x1000, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_WF,      1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_MEDIUM },
    { LEVEL_CCM,     1, 0x0C00, 0x8000, OMM_CAM_DIST_MODE_MEDIUM },
    { LEVEL_BBH,     1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_WDW,     1, 0x0C00, 0x2000, OMM_CAM_DIST_MODE_MEDIUM },
    { LEVEL_TTM,     1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_RR,      1, 0x0C00, 0xE000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_VCUTM,   1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_COURT,   1, 0x0C00, 0x8000, OMM_CAM_DIST_MODE_MEDIUM },
    { LEVEL_GROUNDS, 1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_MEDIUM },
};
static const s32 sOmmCameraInitPresetCount = sizeof(sOmmCameraInitPresets) / sizeof(sOmmCameraInitPresets[0]);

// Level, Area, xMin, xMax, yMin, yMax, zMin, zMax
static const s16 sOmmCameraNoColBoxes[][8] = {
    { LEVEL_WDW,   1,  450, 1400,   100, 3600, -2350, -1100 },
    { LEVEL_BITFS, 1, 2950, 4550, -1450, -600,  -350,   950 },
    { LEVEL_BITFS, 1, 2500, 3200, -1250,  450, -1200,  1200 },
};
static const s32 sOmmCameraNoColBoxCount = sizeof(sOmmCameraNoColBoxes) / sizeof(sOmmCameraNoColBoxes[0]);
#endif

void omm_camera_init() {
    if (omm_camera_is_available(gMarioState)) {
    
        // Default
        sOmmCamPitch = 0x0C00;
        sOmmCamYaw = gMarioState->faceAngle[1] + 0x8000;
        sOmmCamDistMode = OMM_CAM_DIST_MODE_MEDIUM;

#if OMM_GAME_IS_SM64
        // Per level
        for (s32 i = 0; i != sOmmCameraInitPresetCount; ++i) {
            if (sOmmCameraInitPresets[i][0] == gCurrLevelNum &&
                sOmmCameraInitPresets[i][1] == gCurrAreaIndex) {
                sOmmCamPitch = sOmmCameraInitPresets[i][2];
                sOmmCamYaw = sOmmCameraInitPresets[i][3];
                sOmmCamDistMode = sOmmCameraInitPresets[i][4];
                break;
            }
        }
#endif

        // Init
        sOmmCamPitchTarget = sOmmCamPitch;
        sOmmCamYawTarget = sOmmCamYaw;
        sOmmCamDistance = sOmmCameraDistances[sOmmCamDistMode];
        sOmmCamFpMode = 0;
        sOmmCamPitchFp = 0;
    }
}

OMM_ROUTINE_LEVEL_ENTRY(omm_camera_init_from_level_entry) {
    omm_camera_init();
    sOmmCamFpMode = 0;
}

//
// Collision
//

static bool omm_camera_check_point_in_no_col_box(Vec3f pos) {
#if OMM_GAME_IS_SM64
    for (s32 i = 0; i != sOmmCameraNoColBoxCount; ++i) {
        if (sOmmCameraNoColBoxes[i][0] == gCurrLevelNum &&
            sOmmCameraNoColBoxes[i][1] == gCurrAreaIndex &&
            sOmmCameraNoColBoxes[i][2] < pos[0] && pos[0] < sOmmCameraNoColBoxes[i][3] &&
            sOmmCameraNoColBoxes[i][4] < pos[1] && pos[1] < sOmmCameraNoColBoxes[i][5] &&
            sOmmCameraNoColBoxes[i][6] < pos[2] && pos[2] < sOmmCameraNoColBoxes[i][7]) {
            return true;
        }
    }
#else
    OMM_UNUSED(pos);
#endif
    return false;
}

static bool omm_camera_check_focus_behind_actual_wall(const RayCollisionData *hits, const RayHit *hit) {
    if (hits->count != -1) {
        for (s32 i = 0; i != hits->count; ++i) {
            if (hits->hits[i].ratio - hit->ratio < 0.15f) {
                return true;
            }
        }
        return false;
    }
    return true;
}

static void omm_camera_process_collisions() {
    Vec3f orig; vec3f_copy(orig, sOmmCamFocus);
    Vec3f dir; vec3f_dif(dir, sOmmCamPos, orig);
    RayCollisionData hits;
    if (find_collisions_on_ray(orig, dir, &hits, 1.1f, RAYCAST_FLAGS_CAMERA)) {

        // Before applying the collision, try to find another hit from above the focus,
        // to know if the first hit was against a fence or a small object
        RayCollisionData hitsAboveFocus = { .count = -1 };
        if (sOmmCamPos[1] > sOmmCamFocus[1]) {
            orig[1] = min_f(find_ceil(sOmmCamFocus[0], sOmmCamFocus[1], sOmmCamFocus[2], NULL) - 10.f, (sOmmCamPos[1] + sOmmCamFocus[1]) / 2);
            vec3f_dif(dir, sOmmCamPos, orig);
            find_collisions_on_ray(orig, dir, &hitsAboveFocus, 1.1f, RAYCAST_FLAGS_CAMERA);
        }

        for (s32 i = 0; i != hits.count; ++i) {
            RayHit *hit = hits.hits + i;
            if (!omm_camera_check_point_in_no_col_box(hit->pos) && omm_camera_check_focus_behind_actual_wall(&hitsAboveFocus, hit)) {
                sOmmCamPos[0] = hit->pos[0] + 8.f * hit->surf->normal.x;
                sOmmCamPos[1] = hit->pos[1] + 8.f * hit->surf->normal.y;
                sOmmCamPos[2] = hit->pos[2] + 8.f * hit->surf->normal.z;
                return;
            }
        }
    }
}

//
// Update
//

static void omm_camera_exit_first_person(struct Camera *c, struct MarioState *m, bool ignoreFpState, bool resetCamera) {
    if (sOmmCamFpMode || ignoreFpState) {
        sOmmCamFpMode = 0;
        update_mario_sound_and_camera(m);
        if (resetCamera) {
            reset_camera(c);
            init_camera(c);
        }
        gCameraMovementFlags &= ~(CAM_MOVING_INTO_MODE | CAM_MOVE_STARTED_EXITING_C_UP | CAM_MOVE_C_UP_MODE);
        c->mode = c->defMode;
        m->input &= ~INPUT_FIRST_PERSON;
        omm_mario_set_action(m, ACT_IDLE, 0, 0xFFFF);
    }
}

static void omm_camera_process_inputs(struct Camera *c, struct MarioState *m) {
    if (!sOmmCamFpMode) {

        // Camera rotation - C-left/right
        if (m->controller->buttonPressed & R_CBUTTONS) {
            play_sound(SOUND_MENU_CAMERA_TURN + 0x0100, gGlobalSoundArgs);
            sOmmCamYawTarget += OMM_CAM_DELTA_ANGLE;
        } else if (m->controller->buttonPressed & L_CBUTTONS) {
            play_sound(SOUND_MENU_CAMERA_TURN + 0x0100, gGlobalSoundArgs);
            sOmmCamYawTarget -= OMM_CAM_DELTA_ANGLE;
        }

        // Zooms-in - C-up
        if (m->controller->buttonPressed & U_CBUTTONS) {
            if (sOmmCamDistMode == OMM_CAM_DIST_MODE_MIN) {
                play_sound(SOUND_MENU_CAMERA_BUZZ + 0x0300, gGlobalSoundArgs);
            } else {
                play_sound(SOUND_MENU_CAMERA_ZOOM_IN + 0x0200, gGlobalSoundArgs);
                sOmmCamDistMode--;
            }
        }
    
        // Zooms-out - C-down
        else if (m->controller->buttonPressed & D_CBUTTONS) {
            if (sOmmCamDistMode == OMM_CAM_DIST_MODE_MAX) {
                play_sound(SOUND_MENU_CAMERA_BUZZ + 0x0300, gGlobalSoundArgs);
            } else {
                play_sound(SOUND_MENU_CAMERA_ZOOM_OUT + 0x0200, gGlobalSoundArgs);
                sOmmCamDistMode++;
            }
        }
    }

    // Modes - R button
    static s32 sButtonRHeld = 0;
    bool allowFp = ((m->action & ACT_FLAG_ALLOW_FIRST_PERSON) != 0);
    if (m->controller->buttonDown & R_TRIG) {
        sButtonRHeld = (sButtonRHeld * allowFp) + 1;
    } else {
        if (sButtonRHeld > 0 && !OMM_CAMERA_CLASSIC) {
            play_sound(SOUND_MENU_CLICK_CHANGE_VIEW, gGlobalSoundArgs);
            gOmmCameraMode = 3 - gOmmCameraMode;
        }
        omm_camera_exit_first_person(c, m, false, false);
        sButtonRHeld = 0;
    }

    // If held for more than 6 frames while Mario is in idle action, trigger the first person mode
    if (sButtonRHeld >= 6) {
        sOmmCamFpMode = 1;
        sOmmCamPitchFp = 0;
    }

    // First person mode
    if (sOmmCamFpMode) {
        sOmmCamFpMode &= allowFp;
        sOmmCamPitchFp *= allowFp;
        sButtonRHeld = 0;
    } else {
        sMarioCamState->headRotation[0] = approach(sMarioCamState->headRotation[0], 0, 0x400);
        sMarioCamState->headRotation[1] = approach(sMarioCamState->headRotation[1], 0, 0x400);
        sOmmCamPitchFp = 0;
    }
}

static void omm_camera_update_first_person_mode(struct MarioState *m) {
    omm_mario_set_action(m, ACT_FIRST_PERSON, 0, 0);

    // Pitch (from -89 deg to +89 deg)
    sOmmCamPitchFp += (s16) (m->controller->stickY * 10.f);
    sOmmCamPitchFp = clamp_s(sOmmCamPitchFp, -0x3C00, +0x3C00);

    // Yaw
    m->faceAngle[1] -= (s16) (m->controller->stickX * 10.f);

    // Mario's neck
    sMarioCamState->headRotation[0] = (sOmmCamPitchFp * 3) / 4;
    sMarioCamState->headRotation[1] = 0;

    // Pos
    Vec3f marioPos = { sMarioCamState->pos[0], sMarioCamState->pos[1] + 125, sMarioCamState->pos[2] };
    vec3f_set_dist_and_angle(marioPos, sOmmCamPos, 50, sOmmCamPitchFp, m->faceAngle[1] + 0x8000);

    // Focus
    struct Object *playAsCappy = omm_cappy_get_object_play_as();
    if (playAsCappy) {
        sOmmCamFocus[0] = playAsCappy->oPosX;
        sOmmCamFocus[1] = playAsCappy->oPosY + 125.f;
        sOmmCamFocus[2] = playAsCappy->oPosZ;
    } else {
        sOmmCamFocus[0] = sMarioCamState->pos[0];
        sOmmCamFocus[1] = sMarioCamState->pos[1] + 125;
        sOmmCamFocus[2] = sMarioCamState->pos[2];
    }
    
    // TOTWC entrance
    if (m->floor && m->floor->type == SURFACE_LOOK_UP_WARP) {
        if (save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1) >= OMM_CAMERA_LOOK_UP_WARP_STARS) {
            if (sMarioCamState->headRotation[0] <= -0x1800 && (m->faceAngle[1] < -0x6FFF || m->faceAngle[1] >= 0x7000)) {
                level_trigger_warp(m, WARP_OP_UNKNOWN_01);
            }
        }
    }
}

static void omm_camera_calc_y_offsets(f32 *camPosOffsetY, f32 *camFocOffsetY) {
    f32 curGeometryFloorHeight = sMarioGeometry.currFloorHeight;
    if (!(sMarioCamState->action & ACT_FLAG_METAL_WATER)) {
        curGeometryFloorHeight = max_f(curGeometryFloorHeight, sMarioGeometry.waterHeight);
    }
    f32 offset = curGeometryFloorHeight - sMarioCamState->pos[1];
    *camPosOffsetY = clamp_f(offset * 1.0f, -200.f, 200.f);
    *camFocOffsetY = clamp_f(offset * 0.9f, -200.f, 200.f);
}

static bool omm_camera_is_bowser_fight() {
    for_each_(const BehaviorScript *, bhv, 2, OMM_ARRAY_OF(const BehaviorScript *) { bhvBowser, omm_bhv_bowser }) {
        for_each_object_with_behavior(obj, *bhv) {
            if (!obj_is_dormant(obj)) {
#if OMM_GAME_IS_R96A
                // Spamba Bowser
                if (*bhv == bhvBowser && obj->oInteractType == INTERACT_DAMAGE) {
                    continue;
                }
#endif
                return true;
            }
        }
    }
    return false;
}

static bool omm_camera_is_slide(struct MarioState *m) {

    // Slide level
    if (gCurrLevelNum == OMM_LEVEL_SLIDE) {
        return true;
    }

    // Slide terrain
    if (m->area && (m->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE) {
        return true;
    }

    // Racing penguin
    for_each_object_with_behavior(obj, bhvRacingPenguin) {
        if (!obj_is_dormant(obj) && obj->oAction == RACING_PENGUIN_ACT_RACE) {
            return true;
        }
    }

    // Not a slide
    return false;
}

static void omm_camera_update_angles_from_state(struct MarioState *m, s16 camPitchTarget) {
    if (!omm_cappy_get_object_play_as()) {
        
        // Flying
        if ((m->action == ACT_FLYING) ||
            (m->action == ACT_SHOT_FROM_CANNON) || (
            (m->action == ACT_OMM_POSSESSION && gOmmData->object->state.camBehindMario))) {
            sOmmCamPitch = approach(sOmmCamPitch, (-m->faceAngle[0] * 0.75f) + 0xC00, max_f(m->forwardVel, 4.f) * 0x20);
            sOmmCamYaw = approach(sOmmCamYaw, m->faceAngle[1] + 0x8000, max_f(m->forwardVel, 4.f) * 0x80);
            sOmmCamYawTarget = sOmmCamYaw;
            return;
        }

        // Underwater
        if (m->action & ACT_FLAG_SWIMMING) {
            sOmmCamPitch = approach(sOmmCamPitch, (-m->faceAngle[0] * 0.75f) + 0xC00, max_f(m->forwardVel, 4.f) * 0x20);
            sOmmCamYaw = approach(sOmmCamYaw, m->faceAngle[1] + 0x8000, max_f(m->forwardVel, 8.f) * 0x80);
            sOmmCamYawTarget = sOmmCamYaw;
            return;
        }

        // Sliding states
        if (omm_camera_is_slide(m)) {
        
            // Ground states
            if ((m->action & ACT_FLAG_BUTT_OR_STOMACH_SLIDE) ||
                (m->action == ACT_DIVE_SLIDE) ||
                (m->action == ACT_CROUCH_SLIDE) ||
                (m->action == ACT_SLIDE_KICK_SLIDE) ||
                (m->action == ACT_BUTT_SLIDE) ||
                (m->action == ACT_STOMACH_SLIDE) ||
                (m->action == ACT_HOLD_BUTT_SLIDE) ||
                (m->action == ACT_HOLD_STOMACH_SLIDE) ||
                (m->action == ACT_OMM_ROLL)) {
                sOmmCamPitch = approach(sOmmCamPitch, camPitchTarget, 0x400);
                sOmmCamYaw = approach(sOmmCamYaw, m->faceAngle[1] + 0x8000, max_f(m->forwardVel, 4.f) * 0x80);
                sOmmCamYawTarget = sOmmCamYaw;
                return;
            }
        
            // Air states
            if ((m->action == ACT_DIVE) ||
                (m->action == ACT_SLIDE_KICK) ||
                (m->action == ACT_BUTT_SLIDE_AIR) ||
                (m->action == ACT_HOLD_BUTT_SLIDE_AIR) ||
                (m->action == ACT_OMM_ROLL_AIR)) {
                sOmmCamPitch = approach(sOmmCamPitch, camPitchTarget, 0x400);
                sOmmCamYaw = approach(sOmmCamYaw, sOmmCamYawTarget, 0x800);
                return;
            }
        }
    }
    
    // Default
    sOmmCamYawTarget = get_nearest_dir_angle(sOmmCamYawTarget);
    sOmmCamPitch = approach(sOmmCamPitch, camPitchTarget, 0x400);
    sOmmCamYaw = approach(sOmmCamYaw, sOmmCamYawTarget, 0x800);
}

static void omm_camera_update_transform(struct MarioState *m) {
    f32 camFocHeight = 120.f;
    f32 camPosOffsetY = 0.f;
    f32 camFocOffsetY = 0.f;
    f32 camDistTarget = sOmmCameraDistances[sOmmCamDistMode];
    s16 camPitchTarget = sOmmCamPitchTarget;

    // Compute Y offsets on ground and airborne
    if (!(m->action & ACT_FLAG_SWIMMING)) {
        omm_camera_calc_y_offsets(&camPosOffsetY, &camFocOffsetY);
    }

    // Bowser fight camera
    // The distance is increased and the pitch is set
    // to be high enough to show the whole arena
    if (OMM_CAMERA_IS_BOWSER_FIGHT) {
        camPitchTarget = 0x1800;
        camDistTarget *= 2.5f;
    }

    // Possessed Goomba tower
    // The camera focus and distance are computed to always show
    // the entire stack if the distance setting is at its max value
    if (sOmmCamDistMode == OMM_CAM_DIST_MODE_MAX && m->action == ACT_OMM_POSSESSION && omm_obj_is_goomba(gOmmCapture)) {
        f32 goombaHeight = omm_capture_get_top(gOmmCapture);
        camFocHeight = max_f(camFocHeight, goombaHeight / 1.8f);
        camDistTarget = max_f(camDistTarget, goombaHeight * 2.5f);
    }

    // Ceil-hanging
    // Move the camera from above Mario to below Mario
    if (omm_mario_is_hanging(m)) {
        camPitchTarget = -camPitchTarget / 2;
        camPosOffsetY = 0.f;
        camFocOffsetY = 0.f;
    }

    // Angles
    omm_camera_update_angles_from_state(m, camPitchTarget);

    // Focus
    struct Object *playAsCappy = omm_cappy_get_object_play_as();
    if (playAsCappy) {
        sOmmCamFocus[0] = playAsCappy->oPosX;
        sOmmCamFocus[1] = playAsCappy->oPosY + camFocHeight;
        sOmmCamFocus[2] = playAsCappy->oPosZ;
    } else {
        sOmmCamFocus[0] = m->pos[0];
        sOmmCamFocus[1] = m->pos[1] + camFocHeight;
        sOmmCamFocus[2] = m->pos[2];
    }
    
    // Position
    sOmmCamDistance = approach(sOmmCamDistance, camDistTarget, 0x80);
    Vec3f v = { 0, 0, sOmmCamDistance };
    vec3f_rotate_zxy(v, v, -(sOmmCamPitch + gLakituState.shakeMagnitude[0]), +(sOmmCamYaw + gLakituState.shakeMagnitude[1]), 0);
    sOmmCamPos[0] = sOmmCamFocus[0] + v[0];
    sOmmCamPos[1] = sOmmCamFocus[1] + v[1] + camPosOffsetY;
    sOmmCamPos[2] = sOmmCamFocus[2] + v[2];

    // Collisions
    omm_camera_process_collisions();

    // Offset
    sOmmCamFocus[1] += camFocOffsetY;
}

static void omm_camera_apply(struct Camera *c) {

    // Position
    vec3f_copy(c->pos, sOmmCamPos);
    vec3f_copy(gLakituState.pos, sOmmCamPos);
    vec3f_copy(gLakituState.goalPos, sOmmCamPos);

    // Focus
    vec3f_copy(c->focus, sOmmCamFocus);
    vec3f_copy(gLakituState.focus, sOmmCamFocus);
    vec3f_copy(gLakituState.goalFocus, sOmmCamFocus);

    // Angle
    c->yaw = gLakituState.yaw = sOmmCamYaw;

    // Mario transparency
    f32 dist = sqrtf(
        sqr_f(gLakituState.curFocus[0] - gLakituState.curPos[0]) +
        sqr_f(gLakituState.curFocus[1] - gLakituState.curPos[1]) +
        sqr_f(gLakituState.curFocus[2] - gLakituState.curPos[2])
    );
    if (dist < 300.f) {
        u8 alpha = (u8) ((max_f(dist - 100.f, 0) * 255.f) / 200.f);
        gMarioState->marioBodyState->modelState &= ~0xFF;
        gMarioState->marioBodyState->modelState |= (0x100 | alpha);
    }

    // Remove the cannon reticle after shooting
    if (c->mode == CAMERA_MODE_INSIDE_CANNON) {
        c->mode = c->defMode;
    }
}

bool omm_camera_update(struct Camera *c, struct MarioState *m) {
    if (!omm_camera_is_available(m)) {
        omm_camera_exit_first_person(c, m, false, true);
        return false;
    }

    // If Mario is in ACT_FIRST_PERSON when he shouldn't, 
    // disable the first person mode and reset the camera
    if (!sOmmCamFpMode && m->action == ACT_FIRST_PERSON) {
        omm_camera_exit_first_person(c, m, true, true);
    }

    // Init the camera
    gCamera = c;
    sStatusFlags &= ~CAM_FLAG_FRAME_AFTER_CAM_INIT;
    if (gCameraMovementFlags & CAM_MOVE_INIT_CAMERA) {
        init_camera(c);
        gCameraMovementFlags &= ~CAM_MOVE_INIT_CAMERA;
        sStatusFlags |= CAM_FLAG_FRAME_AFTER_CAM_INIT;
    }

    // Pre-update some camera values
    sMarioGeometry.prevFloorHeight = sMarioGeometry.currFloorHeight;
    sMarioGeometry.prevCeilHeight = sMarioGeometry.currCeilHeight;
    sMarioGeometry.prevFloor = sMarioGeometry.currFloor;
    sMarioGeometry.prevCeil = sMarioGeometry.currCeil;
    sMarioGeometry.prevFloorType = sMarioGeometry.currFloorType;
    sMarioGeometry.prevCeilType = sMarioGeometry.currCeilType;
    find_mario_floor_and_ceil(&sMarioGeometry);
    gCheckingSurfaceCollisionsForCamera = TRUE;
    vec3f_copy(c->pos, gLakituState.goalPos);
    vec3f_copy(c->focus, gLakituState.goalFocus);
    c->yaw = gLakituState.yaw;
    c->nextYaw = gLakituState.nextYaw;
    c->mode = gLakituState.mode;
    c->defMode = gLakituState.defMode;

    // Init yaws for "play as Cappy"
    struct Object *playAsCappy = omm_cappy_get_object_play_as();
    if (OMM_CAMERA_CLASSIC && !(playAsCappy->oCappyFlags & CAPPY_FLAG_CAMERA)) {
        sOmmCamYaw = c->yaw;
        sOmmCamYawTarget = c->yaw;
        playAsCappy->oCappyFlags |= CAPPY_FLAG_CAMERA;
    }

    // Play the current cutscene...
    if (c->cutscene != 0) {
        sYawSpeed = 0;
        play_cutscene(c);
        sFramesSinceCutsceneEnded = 0;
    }

    // ...or update the camera
    else {
        sFramesSinceCutsceneEnded = (sFramesSinceCutsceneEnded + 1) * (gRecentCutscene != 0);
        gRecentCutscene *= (sFramesSinceCutsceneEnded < 8);
        omm_camera_process_inputs(c, m);
        if (sOmmCamFpMode) omm_camera_update_first_person_mode(m);
        else omm_camera_update_transform(m);
        omm_camera_apply(c);
        sYawSpeed = 0x400;
    }

    // Start any Mario-related cutscenes
    // Don't trigger door camera events as long as OMM cam is enabled
    sMarioCamState->cameraEvent *= (sMarioCamState->cameraEvent != CAM_EVENT_DOOR);
    start_cutscene(c, get_cutscene_from_mario_status(c));

    // Update Lakitu
    update_lakitu(c);
    gLakituState.lastFrameAction = sMarioCamState->action;
    gCheckingSurfaceCollisionsForCamera = FALSE;
    return true;
}

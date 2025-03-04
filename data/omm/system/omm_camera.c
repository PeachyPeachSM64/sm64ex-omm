#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"
#undef newcam_init_settings
#undef puppycam_default_config

#define OMM_CAM_NUM_DIRS            OMM_CAM_DIRECTIONS[gOmmCameraMode]
#define OMM_CAM_DELTA_ANGLE         (0x10000 / OMM_CAM_NUM_DIRS)
#define OMM_CAM_FP_MIN_FRAMES       (6)
#define OMM_CAM_MODE_MAX_FRAMES     (15)
#define OMM_CAM_DIST_NUM_MODES      (5)
#define OMM_CAM_DIST_MODE_MIN       (0)
#define OMM_CAM_DIST_MODE_MAX       (OMM_CAM_DIST_NUM_MODES - 1)
#define OMM_CAM_DIST_MODE_MEDIUM    (OMM_CAM_DIST_MODE_MAX / 2)
#define OMM_CAM_DIST_MODE_HIGH      (OMM_CAM_DIST_MODE_MEDIUM + 1)
#define OMM_CAM_DIST_MODE_LOW       (OMM_CAM_DIST_MODE_MEDIUM - 1)

static const f32 OMM_CAM_DISTANCES[OMM_CAM_DIST_NUM_MODES] = {
    400.f, 800.f, 1200.f, 1600.f, 2000.f
};

static const s32 OMM_CAM_DIRECTIONS[] = {
    8, 8, 16, 64
};

#if OMM_CODE_DEBUG && 0
#define OMM_CAM_DEBUG(...) omm_printf(__VA_ARGS__)
#else
#define OMM_CAM_DEBUG(...)
#endif

//
// Data
//

s32 sOmmCamFpMode      = FALSE;
s32 sOmmCamFpEnabled   = TRUE;
s32 sOmmCamFpTimer     = 0;
s16 sOmmCamPitch       = 0x0C00;
s16 sOmmCamPitchFp     = 0x0000;
s16 sOmmCamPitchTarget = 0x0C00;
s16 sOmmCamYaw         = 0x0000;
s16 sOmmCamYawTarget   = 0x0000;
s32 sOmmCamDistMode    = OMM_CAM_DIST_MODE_MEDIUM;
f32 sOmmCamDistance    = 0.f;
f32 sOmmCamPos[3];
f32 sOmmCamFocus[3];

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
    return m->action != ACT_JUMBO_STAR_CUTSCENE && !omm_is_ending_cutscene() && (!OMM_CAMERA_CLASSIC || omm_cappy_get_object_play_as()) && (m->action != ACT_IN_CANNON);
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

void omm_camera_warp(struct Camera *c, Vec3f displacement) {
    warp_camera(displacement[0], displacement[1], displacement[2]);
    vec3f_add(c->pos, displacement);
    vec3f_add(c->focus, displacement);
    vec3f_add(gLakituState.pos, displacement);
    vec3f_add(gLakituState.focus, displacement);
    gLakituState.yaw = c->yaw;
    gLakituState.nextYaw = c->nextYaw;
}

//
// Init
//

#if OMM_GAME_IS_SM64
typedef struct { s32 levelNum; s32 areaIndex; s16 pitch; s16 yaw; s32 distMode; } OmmCameraInitPreset;
static const OmmCameraInitPreset OMM_CAM_INIT_PRESETS[] = {
    { LEVEL_BITDW,            1, 0x1000, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_BITFS,            1, 0x1000, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_BITS,             1, 0x1000, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_WF,               1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_MEDIUM },
    { LEVEL_CCM,              1, 0x0C00, 0x8000, OMM_CAM_DIST_MODE_MEDIUM },
    { LEVEL_BBH,              1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_WDW,              1, 0x0C00, 0x2000, OMM_CAM_DIST_MODE_MEDIUM },
    { LEVEL_TTM,              1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_RR,               1, 0x0C00, 0xE000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_VCUTM,            1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_HIGH },
    { LEVEL_CASTLE_COURTYARD, 1, 0x0C00, 0x8000, OMM_CAM_DIST_MODE_MEDIUM },
    { LEVEL_CASTLE_GROUNDS,   1, 0x0C00, 0x0000, OMM_CAM_DIST_MODE_MEDIUM },
};

typedef struct { s32 levelNum; s32 areaIndex; f32 xMin; f32 xMax; f32 yMin; f32 yMax; f32 zMin; f32 zMax; } OmmCameraNoColBox;
static const OmmCameraNoColBox OMM_CAM_NO_COL_BOXES[] = {
    { LEVEL_SL,    1, 3650, 5060,  1024, 2048,  3730,  5320 }, // SL - Ice sculpture
    { LEVEL_WDW,   1,  450, 1400,   100, 3600, -2350, -1100 }, // WDW - Elevator
    { LEVEL_WDW,   1, 3420, 4580,     0, 3200, -3810, -2650 }, // WDW - Big cage
    { LEVEL_BITFS, 1, 2950, 4550, -1450, -600,  -350,   950 }, // BITFS - Pole cage
    { LEVEL_BITFS, 1, 2500, 3200, -1250,  450, -1200,  1200 }, // BITFS - Elevator
    { LEVEL_BITFS, 1, 6000, 7500,  2500, 5500,  -650,   850 }, // BITFS - Warp to Bowser 2
};
#endif

void omm_camera_init() {
    if (!gOmmGlobals->cameraNoInit && !omm_is_main_menu() && omm_camera_is_available(gMarioState)) {

        // Default
        sOmmCamPitch = 0x0C00;
        sOmmCamYaw = gMarioState->faceAngle[1] + 0x8000;
        sOmmCamDistMode = OMM_CAM_DIST_MODE_MEDIUM;

#if OMM_GAME_IS_SM64
        // Per level
        for (u32 i = 0; i != array_length(OMM_CAM_INIT_PRESETS); ++i) {
            const OmmCameraInitPreset *preset = &OMM_CAM_INIT_PRESETS[i];
            if (preset->levelNum == gCurrLevelNum && preset->areaIndex == gCurrAreaIndex) {
                sOmmCamPitch = preset->pitch;
                sOmmCamYaw = preset->yaw;
                sOmmCamDistMode = preset->distMode;
                break;
            }
        }
#endif

        // Init
        sOmmCamPitchTarget = sOmmCamPitch;
        sOmmCamYawTarget = sOmmCamYaw;
        sOmmCamDistance = OMM_CAM_DISTANCES[sOmmCamDistMode];
        sOmmCamPitchFp = 0;
    }

    // Reset first person mode
    sOmmCamFpMode = FALSE;
    sOmmCamFpEnabled = TRUE;
    sOmmCamFpTimer = 0;
}

OMM_ROUTINE_LEVEL_ENTRY(omm_camera_init_from_level_entry) {
    omm_camera_init();

    // Disable the zoom-out camera thing
    extern u8 sZoomOutAreaMasks[];
    mem_zero(sZoomOutAreaMasks, sizeof(u8) * ((LEVEL_COUNT + 1) / 2));
}

//
// Collision
//

static bool omm_camera_should_ignore_surface(struct Surface *surf) {
    return surf->object && omm_obj_is_no_cam_col(surf->object);
}

static bool omm_camera_is_point_in_no_col_box(Vec3f pos) {
#if OMM_GAME_IS_SM64
    for (u32 i = 0; i != array_length(OMM_CAM_NO_COL_BOXES); ++i) {
        const OmmCameraNoColBox *box = &OMM_CAM_NO_COL_BOXES[i];
        if (box->levelNum == gCurrLevelNum &&
            box->areaIndex == gCurrAreaIndex &&
            box->xMin < pos[0] && pos[0] < box->xMax &&
            box->yMin < pos[1] && pos[1] < box->yMax &&
            box->zMin < pos[2] && pos[2] < box->zMax) {
            return true;
        }
    }
#endif
    return false;
}

static bool omm_camera_check_hit_above_focus(const RayHit *hitFocusToPos, const RayCollisionData *hitsAboveFocus) {
    if (hitsAboveFocus->count >= 0) {
        for (s32 i = 0; i != hitsAboveFocus->count; ++i) {
            const RayHit *hitAboveFocus = hitsAboveFocus->hits + i;
            if (hitAboveFocus->ratio - hitFocusToPos->ratio < 0.15f) {
                return true;
            }
        }
        return false;
    }
    return true;
}

static bool omm_camera_check_hit_past_focus(const RayHit *hitFocusToPos, const RayCollisionData *hitsPastFocus) {
    if (hitsPastFocus->count >= 0) {
        for (s32 i = 0; i != hitsPastFocus->count; ++i) {
            const RayHit *hitPastFocus = hitsPastFocus->hits + i;
            if (hitPastFocus->pos[1] - hitFocusToPos->pos[1] < 50.f) {
                return true;
            }
        }
        return false;
    }
    return true;
}

static bool are_surfaces_adjacent(struct Surface *surf1, struct Surface *surf2) {
    Vec3f verts1[3] = {
        { surf1->vertex1[0], surf1->vertex1[1], surf1->vertex1[2] },
        { surf1->vertex2[0], surf1->vertex2[1], surf1->vertex2[2] },
        { surf1->vertex3[0], surf1->vertex3[1], surf1->vertex3[2] },
    };
    Vec3f verts2[3] = {
        { surf2->vertex1[0], surf2->vertex1[1], surf2->vertex1[2] },
        { surf2->vertex2[0], surf2->vertex2[1], surf2->vertex2[2] },
        { surf2->vertex3[0], surf2->vertex3[1], surf2->vertex3[2] },
    };
    for (s32 i = 0; i != 3; ++i)
    for (s32 j = 0; j != 3; ++j) {
        f32 dx2 = sqr_f(verts1[i][0] - verts2[j][0]);
        f32 dy2 = sqr_f(verts1[i][1] - verts2[j][1]);
        f32 dz2 = sqr_f(verts1[i][2] - verts2[j][2]);
        if (dx2 + dy2 + dz2 < 4.f) {
            return true;
        }
    }
    return false;
}

// Check if the surface the ray hits is part of a pillar
// If it's small enough, ignore it
// Affected by hit ratio, so the pillar is less likely to be ignored the closer it is to the camera
static bool omm_camera_should_ignore_hit(RayHit *hitFocusToPos, f32 searchRadius, f32 maxPillarRadius) {
    OmmArray_(struct Surface *) surfaces = omm_array_zero;

    // Doors are not pillars
    struct Object *obj = hitFocusToPos->surf->object;
    if (obj && omm_obj_is_door(obj)) {
        return false;
    }

    // Find surfaces in hit cell and 8 neighbors
    s16 cx0 = ((s16) ((hitFocusToPos->pos[0] + LEVEL_BOUNDARY_MAX) / CELL_SIZE)) & NUM_CELLS_INDEX;
    s16 cz0 = ((s16) ((hitFocusToPos->pos[2] + LEVEL_BOUNDARY_MAX) / CELL_SIZE)) & NUM_CELLS_INDEX;
    for (s16 cz = max_s(0, cz0 - 1); cz <= min_s(cz0 + 1, NUM_CELLS - 1); ++cz)
    for (s16 cx = max_s(0, cx0 - 1); cx <= min_s(cx0 + 1, NUM_CELLS - 1); ++cx)
    for (s32 dyn = 0; dyn != 2; ++dyn)
    for (s32 type = 0; type != 3; ++type) {
        const OmmSurfaceArray *arr = &gOmmSurfaces[dyn][cz][cx][type];
        for (s32 i = 0; i != arr->count; ++i) {
            struct Surface *surf = omm_array_get(arr->data, ptr, i);

            // Reject hit surface
            if (surf == hitFocusToPos->surf) {
                continue;
            }

            // Reject surfaces that are already added
            if (omm_array_find(surfaces, ptr, surf) != -1) {
                continue;
            }

            // Reject surfaces that are not valid "walls"
            if (abs_f(surf->normal.y) > 0.4f) {
                continue;
            }

            // Reject surfaces that don't contain hit pos vertically
            if (surf->lowerY > hitFocusToPos->pos[1] || surf->upperY < hitFocusToPos->pos[1]) {
                continue;
            }

            // Reject surfaces that don't have at least one vertex inside radius horizontally
            f32 v1d2 = sqr_f(surf->vertex1[0] - hitFocusToPos->pos[0]) + sqr_f(surf->vertex1[2] - hitFocusToPos->pos[2]);
            f32 v2d2 = sqr_f(surf->vertex2[0] - hitFocusToPos->pos[0]) + sqr_f(surf->vertex2[2] - hitFocusToPos->pos[2]);
            f32 v3d2 = sqr_f(surf->vertex3[0] - hitFocusToPos->pos[0]) + sqr_f(surf->vertex3[2] - hitFocusToPos->pos[2]);
            f32 d2min = min_3_f(v1d2, v2d2, v3d2);
            if (d2min > sqr_f(searchRadius)) {
                continue;
            }

            // Keep that surface for now
            omm_array_add(surfaces, ptr, surf);
        }
    }

    // Select recursively all surfaces adjacent to each other, starting from the hit surface
    OmmArray_(struct Surface *) asurfaces = omm_array_zero;
    omm_array_add(asurfaces, ptr, hitFocusToPos->surf);
    for (bool done = false; !done;) {
        done = true;
        omm_array_for_each(asurfaces, p_asurf) {
            struct Surface *asurf = (struct Surface *) p_asurf->as_ptr;
            for (s32 i = omm_array_count(surfaces) - 1; i >= 0; --i) {
                struct Surface *surf = (struct Surface *) omm_array_get(surfaces, ptr, i);
                if (are_surfaces_adjacent(asurf, surf)) {
                    omm_array_add(asurfaces, ptr, surf);
                    omm_array_remove(surfaces, i);
                    p_asurf = asurfaces.p + i_p_asurf; // hack: since asurfaces grows during omm_array_add, asurfaces.p can be reallocated,
                                                       // so the pointer p_asurf needs to point to that new buffer
                    done = false;
                }
            }
        }
    }

    // Compute center of adjacent surfaces
    Vec2f center = { 0, 0 };
    {omm_array_for_each(asurfaces, p_asurf) {
        struct Surface *asurf = (struct Surface *) p_asurf->as_ptr;
        center[0] += asurf->vertex1[0] + asurf->vertex2[0] + asurf->vertex3[0];
        center[1] += asurf->vertex1[2] + asurf->vertex2[2] + asurf->vertex3[2];
    }}
    vec2f_mul(center, 1.f / (3.f * omm_array_count(asurfaces)));

    // Compute radius of circumcircle
    f32 ccr2 = 0.f;
    {omm_array_for_each(asurfaces, p_asurf) {
        struct Surface *asurf = (struct Surface *) p_asurf->as_ptr;
        f32 v1d2 = sqr_f(asurf->vertex1[0] - center[0]) + sqr_f(asurf->vertex1[2] - center[1]);
        f32 v2d2 = sqr_f(asurf->vertex2[0] - center[0]) + sqr_f(asurf->vertex2[2] - center[1]);
        f32 v3d2 = sqr_f(asurf->vertex3[0] - center[0]) + sqr_f(asurf->vertex3[2] - center[1]);
        ccr2 = max_3_f(v1d2, v2d2, v3d2);
    }}

    // Ignore hit if circumcircle radius is small enough
    omm_array_delete(asurfaces);
    omm_array_delete(surfaces);
    return ccr2 < sqr_f(maxPillarRadius * (1.f - hitFocusToPos->ratio));
}

static void omm_camera_process_collisions(bool isFlyingOrSwimming) {
    f32 maxPillarRadius = 96.f + sOmmCamDistance / 12.5f; // 128 to 256 with default camera distances
    f32 searchRadius = maxPillarRadius * 4.f;
    Vec3f pos; vec3f_copy(pos, sOmmCamPos);
    Vec3f focus; vec3f_copy(focus, sOmmCamFocus);

    // Focus to pos
    Vec3f origFocusToPos = { focus[0], focus[1], focus[2] };
    Vec3f dirFocusToPos = { pos[0] - origFocusToPos[0], pos[1] - origFocusToPos[1], pos[2] - origFocusToPos[2] };
    RayHit *hitFocusToPos = NULL;
    RayCollisionData hitsFocusToPos;
    if (find_collisions_on_ray(origFocusToPos, dirFocusToPos, &hitsFocusToPos, 1.1f, RAYCAST_FLAGS_CAMERA) > 0) {

        // Find collisions above the focus to know if hits are against a fence or a small object
        RayCollisionData hitsAboveFocus = { .count = -1 };
        if (!isFlyingOrSwimming && pos[1] > focus[1]) {
            Vec3f dirUp = { 0.f, 0.4f * (pos[1] - focus[1]), 0.f };
            Vec3f hitPosUp;
            find_first_hit_on_ray(focus, dirUp, hitPosUp, 5.f, 1.1f, RAYCAST_FLAGS_CAMERA);
            Vec3f origAboveFocus = { focus[0], hitPosUp[1], focus[2] };
            Vec3f dirAboveFocus = { pos[0] - origAboveFocus[0], pos[1] - origAboveFocus[1], pos[2] - origAboveFocus[2] };
            find_collisions_on_ray(origAboveFocus, dirAboveFocus, &hitsAboveFocus, 1.1f, RAYCAST_FLAGS_CAMERA);
        }

        // Find collisions past the focus to know if hits are against the side of a ceiling
        RayCollisionData hitsPastFocus = { .count = -1 };
        if (!isFlyingOrSwimming && pos[1] > focus[1]) {
            Vec3f dirForward = { 0.4f * (pos[0] - focus[0]), 0.f, 0.4f * (pos[2] - focus[2]) };
            Vec3f hitPosForward;
            find_first_hit_on_ray(focus, dirForward, hitPosForward, 5.f, 1.1f, RAYCAST_FLAGS_CAMERA);
            Vec3f origPastFocus = { hitPosForward[0], focus[1], hitPosForward[2] };
            Vec3f dirPastFocus = { pos[0] - origPastFocus[0], pos[1] - origPastFocus[1], pos[2] - origPastFocus[2] };
            find_collisions_on_ray(origPastFocus, dirPastFocus, &hitsPastFocus, 1.1f, RAYCAST_FLAGS_CAMERA);
        }

        // Check for hits and apply collision
        for (s32 i = 0; i != hitsFocusToPos.count; ++i) {
            hitFocusToPos = hitsFocusToPos.hits + i;
            OMM_CAM_DEBUG("HIT: %d %d %d\n",, (s32) hitFocusToPos->pos[0], (s32) hitFocusToPos->pos[1], (s32) hitFocusToPos->pos[2]);

            // Reject hits on a dynamic surface with a specific behavior
            if (omm_camera_should_ignore_surface(hitFocusToPos->surf)) {
                OMM_CAM_DEBUG("> Ignored: surface\n");
                continue;
            }

            // Reject hits inside a no collision box
            if (omm_camera_is_point_in_no_col_box(hitFocusToPos->pos)) {
                OMM_CAM_DEBUG("> Ignored: no col box\n");
                continue;
            }

            // Reject hits that doesn't connect from above focus to pos
            if (!omm_camera_check_hit_above_focus(hitFocusToPos, &hitsAboveFocus)) {
                OMM_CAM_DEBUG("> Ignored: no hit above focus (count: %d)\n",, hitsAboveFocus.count);
                continue;
            }

            // Reject hits that doesn't connect from past focus to pos
            if (!omm_camera_check_hit_past_focus(hitFocusToPos, &hitsPastFocus)) {
                OMM_CAM_DEBUG("> Ignored: no hit above focus (count: %d)\n",, hitsPastFocus.count);
                continue;
            }

            // Reject hits on a small pillar
            if (omm_camera_should_ignore_hit(hitFocusToPos, searchRadius, maxPillarRadius)) {
                OMM_CAM_DEBUG("> Ignored: small pillar\n");
                continue;
            }

            // Apply collision
            sOmmCamPos[0] = hitFocusToPos->pos[0] + 8.f * hitFocusToPos->surf->normal.x;
            sOmmCamPos[1] = hitFocusToPos->pos[1] + 8.f * hitFocusToPos->surf->normal.y;
            sOmmCamPos[2] = hitFocusToPos->pos[2] + 8.f * hitFocusToPos->surf->normal.z;
            break;
        }
    }
}

//
// Update
//

static void omm_camera_set_angles_from_mario_angles(struct MarioState *m) {
    bool swimming = (m->action & ACT_FLAG_SWIMMING) != 0;

    // Pitch
    if (swimming) {
        sOmmCamPitch = (-m->faceAngle[0] * 0.75f * (m->pos[1] < m->waterLevel - 100.f)) + 0xC00;
    }

    // Yaw
    if (swimming || OMM_CAMERA_INF_DIRECTIONS) {
        sOmmCamYaw = sOmmCamYawTarget = m->faceAngle[1] + 0x8000;
    }
}

static void omm_camera_exit_first_person(struct Camera *c, struct MarioState *m, bool ignoreFpState, bool resetCamera) {
    if (sOmmCamFpMode || ignoreFpState) {
        if (sOmmCamFpMode) {
            omm_camera_set_angles_from_mario_angles(m);
        }
        sOmmCamFpMode = FALSE;
        update_mario_sound_and_camera(m);
        music_unlower_volume(SEQ_PLAYER_LEVEL, 60);
        if (resetCamera) {
            reset_camera(c);
            init_camera(c);
        }
        gCameraMovementFlags &= ~(CAM_MOVING_INTO_MODE | CAM_MOVE_STARTED_EXITING_C_UP | CAM_MOVE_C_UP_MODE);
        c->mode = c->defMode;
        m->input &= ~INPUT_FIRST_PERSON;
        if (omm_mario_is_capture(m)) {
            gOmmMario->capture.firstPerson = false;
        } else if (m->action & ACT_FLAG_METAL_WATER) {
            omm_mario_set_action(m, ACT_OMM_METAL_WATER_IDLE, 0, 0xFFFF);
        } else if (m->action & ACT_FLAG_SWIMMING) {
            omm_mario_set_action(m, ACT_WATER_IDLE, 0, 0xFFFF);
        } else {
            omm_mario_set_action(m, ACT_IDLE, 0, 0xFFFF);
        }
    }
}

static void omm_camera_process_inputs(struct Camera *c, struct MarioState *m) {
    if (!sOmmCamFpMode) {

        // Camera rotation - C-left/right
        if (OMM_CAMERA_INF_DIRECTIONS) {
            sOmmCamYawTarget -= OMM_CAM_DELTA_ANGLE * OMM_CAMERA_X * clamp_f(((abs_f(m->controller->extStickY) < 50) * (abs_f(m->controller->extStickX) > abs_f(m->controller->extStickY)) * (m->controller->extStickX / 127.f)) + (m->controller->extStickX == 0) * (((m->controller->buttonDown & R_CBUTTONS) != 0) - ((m->controller->buttonDown & L_CBUTTONS) != 0)), -1.f, +1.f);
        } else if (m->controller->buttonPressed & R_CBUTTONS) {
            play_sound(SOUND_MENU_CAMERA_TURN + 0x0100, gGlobalSoundArgs);
            sOmmCamYawTarget -= OMM_CAM_DELTA_ANGLE * OMM_CAMERA_X;
        } else if (m->controller->buttonPressed & L_CBUTTONS) {
            play_sound(SOUND_MENU_CAMERA_TURN + 0x0100, gGlobalSoundArgs);
            sOmmCamYawTarget += OMM_CAM_DELTA_ANGLE * OMM_CAMERA_X;
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
    bool allowFp = omm_mario_allow_first_person(m);
    if (m->controller->buttonDown & R_TRIG) {
        sOmmCamFpTimer++;
    } else {
        if (!sOmmCamFpEnabled && sOmmCamFpTimer > 0 && sOmmCamFpTimer <= OMM_CAM_MODE_MAX_FRAMES && !OMM_CAMERA_CLASSIC) {
            play_sound(SOUND_MENU_CLICK_CHANGE_VIEW, gGlobalSoundArgs);
            gOmmCameraMode = 1 + (gOmmCameraMode % (gOmmCameraModeCount - 1));
        }
        omm_camera_exit_first_person(c, m, false, false);
        sOmmCamFpEnabled = FALSE;
        sOmmCamFpTimer = 0;
    }

    // First person mode (hold R for 6 frames)
    if (sOmmCamFpMode) {
        if (!allowFp) {
            omm_camera_set_angles_from_mario_angles(m);
            music_unlower_volume(SEQ_PLAYER_LEVEL, 60);
        }
        sOmmCamFpMode &= allowFp;
        sOmmCamPitchFp *= allowFp;
    } else if (allowFp && sOmmCamFpTimer >= OMM_CAM_FP_MIN_FRAMES) {
        music_lower_volume(SEQ_PLAYER_LEVEL, 60, 40);
        sOmmCamFpMode = TRUE;
        sOmmCamFpEnabled = TRUE;
        sOmmCamPitchFp = 0;
    } else {
        sMarioCamState->headRotation[0] = approach(sMarioCamState->headRotation[0], 0, 0x400);
        sMarioCamState->headRotation[1] = approach(sMarioCamState->headRotation[1], 0, 0x400);
        sOmmCamPitchFp = 0;
    }
}

static void omm_camera_update_first_person_mode(struct MarioState *m) {

    // Pitch (from -89 deg to +89 deg)
    sOmmCamPitchFp -= (s16) (m->controller->stickY * OMM_CAMERA_Y * 10);
    sOmmCamPitchFp = clamp_s(sOmmCamPitchFp, -0x3C00, +0x3C00);

    // Yaw
    m->faceAngle[1] -= (s16) (m->controller->stickX * OMM_CAMERA_X * 10);

    // Mario's neck
    sMarioCamState->headRotation[0] = (sOmmCamPitchFp * 3) / 4;
    sMarioCamState->headRotation[1] = 0;

    // Pos
    f32 height = 0.75f * max_f(omm_mario_is_capture(m) ? omm_capture_get_top(gOmmCapture) : m->marioObj->hitboxHeight, 160);
    Vec3f marioPos = { sMarioCamState->pos[0], sMarioCamState->pos[1] + height, sMarioCamState->pos[2] };
    vec3f_set_dist_and_angle(marioPos, sOmmCamPos, 50, sOmmCamPitchFp, m->faceAngle[1] + 0x8000);

    // Focus
    struct Object *playAsCappy = omm_cappy_get_object_play_as();
    if (playAsCappy) {
        sOmmCamFocus[0] = playAsCappy->oPosX;
        sOmmCamFocus[1] = playAsCappy->oPosY + height;
        sOmmCamFocus[2] = playAsCappy->oPosZ;
    } else {
        sOmmCamFocus[0] = sMarioCamState->pos[0];
        sOmmCamFocus[1] = sMarioCamState->pos[1] + height;
        sOmmCamFocus[2] = sMarioCamState->pos[2];
    }

    // Update action
    if (omm_mario_is_capture(m)) {
        gOmmMario->capture.firstPerson = true;
        gOmmCapture->oFaceAngleYaw = m->faceAngle[1];
    } else if (m->action & ACT_FLAG_METAL_WATER) {
        omm_mario_set_action(m, ACT_OMM_METAL_WATER_FIRST_PERSON, 0, 0);
    } else if (m->action & ACT_FLAG_SWIMMING) {
        omm_mario_set_action(m, ACT_OMM_WATER_FIRST_PERSON, 0, 0);
        m->faceAngle[0] = -sOmmCamPitchFp;
    } else {
        omm_mario_set_action(m, ACT_FIRST_PERSON, 0, 0);
    }

    // TOTWC entrance
    if (m->floor && m->floor->type == SURFACE_LOOK_UP_WARP) {
        if (omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE) >= OMM_CAMERA_LOOK_UP_WARP_STARS) {
            if (sMarioCamState->headRotation[0] <= -0x1800 && (m->faceAngle[1] < -0x6FFF || m->faceAngle[1] >= 0x7000)) {
                omm_mario_unpossess_object_before_warp(m);
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

bool omm_camera_is_bowser_fight() {
    for_each_in_(const BehaviorScript *, bhv, { bhvBowser, bhvOmmBowser }) {
        for_each_object_with_behavior(obj, *bhv) {
            if (!obj_is_dormant(obj)) {
#if OMM_GAME_IS_R96X
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

    // Flat floor is no slide
    if (!m->floor || m->floor->normal.y > 0.999f) {
        return false;
    }

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

static s16 omm_camera_get_slide_pitch(struct MarioState *m, s16 yaw) {
    if (m->floor) {
        Vec2f floorXZ = { m->floor->normal.x, m->floor->normal.z };
        Vec2f camXZ = { sins(yaw), coss(yaw) };
        f32 dotFloorCam = vec2f_dot(vec2f_normalize(floorXZ), vec2f_normalize(camXZ));
        return absf(acosf(m->floor->normal.y) / M_PI) * 0x5000 * max_f(0.f, dotFloorCam);
    }
    return 0;
}

static bool omm_camera_update_angles_from_state(struct MarioState *m, s16 camPitchTarget) {
    static s32 sWasFlyingSwimmingSliding = 0;
    if (!omm_cappy_get_object_play_as()) {

        // Flying
        if ((m->action == ACT_FLYING) ||
            (m->action == ACT_SHOT_FROM_CANNON) || (
            (omm_mario_is_capture(m) && gOmmObject->state._camBehindMario))) {
            sOmmCamPitch = approach(sOmmCamPitch, (-m->faceAngle[0] * 0.75f) + 0xC00, max_f(m->forwardVel, 4.f) * 0x40);
            sOmmCamYaw = approach(sOmmCamYaw, m->faceAngle[1] + 0x8000, max_f(m->forwardVel, 4.f) * 0x80);
            sOmmCamYawTarget = sOmmCamYaw;
            sWasFlyingSwimmingSliding = 1;
            return true;
        }

        // Swimming
        if (m->action & ACT_FLAG_SWIMMING) {
            sOmmCamPitch = approach(sOmmCamPitch, (-m->faceAngle[0] * 0.75f * (m->pos[1] < m->waterLevel - 100.f)) + 0xC00, 0x300);
            sOmmCamYaw = approach(sOmmCamYaw, m->faceAngle[1] + 0x8000, 0x400);
            sOmmCamYawTarget = sOmmCamYaw;
            sWasFlyingSwimmingSliding = 2;
            return true;
        }

        // Sliding
        if ((omm_camera_is_slide(m) || sWasFlyingSwimmingSliding == 3) && (
            (m->action & ACT_FLAG_BUTT_OR_STOMACH_SLIDE) ||
            (m->action == ACT_DIVE_SLIDE) ||
            (m->action == ACT_CROUCH_SLIDE) ||
            (m->action == ACT_SLIDE_KICK_SLIDE) ||
            (m->action == ACT_BUTT_SLIDE) ||
            (m->action == ACT_STOMACH_SLIDE) ||
            (m->action == ACT_HOLD_BUTT_SLIDE) ||
            (m->action == ACT_HOLD_STOMACH_SLIDE) ||
            (m->action == ACT_OMM_ROLL))) {
            sOmmCamYaw = approach(sOmmCamYaw, m->faceAngle[1] + 0x8000, max_f(m->forwardVel, 4.f) * 0x80);
            sOmmCamPitch = approach(sOmmCamPitch, camPitchTarget + omm_camera_get_slide_pitch(m, sOmmCamYaw + 0x8000), 0x400);
            sOmmCamYawTarget = sOmmCamYaw;
            sWasFlyingSwimmingSliding = 3;
            return false;
        }

        // Airborne
        if (sWasFlyingSwimmingSliding && (m->action & ACT_FLAG_AIR)) {
            sOmmCamPitch = approach(sOmmCamPitch, camPitchTarget, 0x400);
            sOmmCamYaw = approach(sOmmCamYaw, sOmmCamYawTarget, 0x800);
            return false;
        }
    }

    // Default
    if (!OMM_CAMERA_INF_DIRECTIONS) sOmmCamYawTarget = get_nearest_dir_angle(sOmmCamYawTarget);
    sOmmCamPitch = approach(sOmmCamPitch, camPitchTarget, 0x400);
    sOmmCamYaw = approach(sOmmCamYaw, sOmmCamYawTarget, 0x800);
    sWasFlyingSwimmingSliding = 0;
    return false;
}

static void omm_camera_update_transform(struct MarioState *m) {
    f32 camFocHeight = 120.f;
    f32 camPosOffsetY = 0.f;
    f32 camFocOffsetY = 0.f;
    f32 camDistTarget = OMM_CAM_DISTANCES[sOmmCamDistMode];
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

#if OMM_GAME_IS_R96X
    // Milk
    // Increase the distance to show big Mario
    if (omm_mario_is_milk(m)) {
        f32 t = invlerp_0_1_f(m->marioObj->oScaleY, 1.f, 4.f);
        camFocHeight += lerp_f(t, 0, 250);
        camDistTarget *= lerp_f(t, 1.f, 2.f);
    }
#endif

    // Captures
    // The camera focus and distance are computed to show the capture
    // in its entirety if the distance setting is at its max value
    if (sOmmCamDistMode == OMM_CAM_DIST_MODE_MAX && omm_mario_is_capture(m)) {
        f32 camDistCapture = omm_capture_get_top(gOmmCapture) * 3.2f;
        camFocHeight *= max_f(1.f, camDistCapture / camDistTarget);
        camDistTarget = max_f(camDistTarget, camDistCapture);
    }

    // Dorrie
    // Increase the distance to show the dino
    if (omm_mario_is_capture(m) && gOmmCapture->behavior == bhvDorrie) {
        camFocHeight += 500.f;
        camDistTarget *= 2.f;
    }

    // OMM Dorrie
    // Increase the distance to show the flying dino
    if (omm_mario_is_capture(m) && gOmmCapture->behavior == bhvOmmDorrie) {
        camFocHeight += 250.f;
        camDistTarget *= 1.5f;
    }

    // Ceil-hanging
    // Move the camera from above Mario to below Mario
    if (omm_mario_is_hanging(m) || (omm_mario_is_capture(m) && omm_capture_get_type(gOmmCapture) == OMM_CAPTURE_SWOOP && get_surface_from_data(gOmmObject->swoop.data))) {
        camPitchTarget = -camPitchTarget / 2;
        camPosOffsetY = 0.f;
        camFocOffsetY = 0.f;
        camFocHeight -= 50.f;
    }

#if OMM_GAME_IS_SM64
    // Endless stairs
    if (gCurrLevelNum == LEVEL_CASTLE && gCurrAreaIndex == 2 && m->numStars < 70 && m->floor != NULL && m->floor->room == 6 && m->pos[2] < 2540.f) {
        s16 newPitchTarget = relerp_0_1_f(coss(sOmmCamYaw + gLakituState.shakeMagnitude[1] + 0x8000), -1, 1, 0x400, 0x1400);
        camPitchTarget = relerp_0_1_f(m->pos[2], 2540, 2140, camPitchTarget, newPitchTarget);
    }
#endif

    // Angles
    bool isFlyingOrSwimming = omm_camera_update_angles_from_state(m, camPitchTarget);

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
    omm_camera_process_collisions(isFlyingOrSwimming);

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
    if (!sOmmCamFpMode && (m->action == ACT_FIRST_PERSON || gOmmMario->capture.firstPerson)) {
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
    if (OMM_CAMERA_CLASSIC && !(playAsCappy->oCappyFlags & OMM_CAPPY_FLAG_CAMERA)) {
        sOmmCamYaw = c->yaw;
        sOmmCamYawTarget = c->yaw;
        playAsCappy->oCappyFlags |= OMM_CAPPY_FLAG_CAMERA;
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

    // Fix Lakitu's camera position
    if (c->cutscene == 0 && c->mode != CAMERA_MODE_C_UP && (BETTER_CAM_IS_PUPPY_CAM || c->mode != BETTER_CAM_MODE)) {
        vec3f_copy(gLakituState.pos, gLakituState.curPos);
    }
    return true;
}

void omm_camera_update_settings() {
    configCameraAnalog = 0;
#if BETTER_CAM_IS_PUPPY_CAM
    configCameraOpacity = 0;
    configDebugCamera = 0;
    configMouse = configCameraMouse;
    puppycam_default_config();
    gPuppyCam.options.sensitivityX = configCameraXSens * 1.7f;
    gPuppyCam.options.sensitivityY = configCameraYSens * 1.7f;
    gPuppyCam.options.analogue = 0;
    gPuppyCam.options.opacityType = 0;
    gPuppyCam.options.debugCam = 0;
#else
    configCameraDegrade = 100;
    newcam_init_settings();
    extern s16 newcam_sensitivityX; newcam_sensitivityX = configCameraXSens * 2.2f;
    extern s16 newcam_sensitivityY; newcam_sensitivityY = configCameraYSens * 2.2f;
    extern s16 newcam_analogue; newcam_analogue = 0;
    extern s16 newcam_panlevel; newcam_panlevel = 0;
    extern s16 newcam_degrade; newcam_degrade = 100;
#endif
}

OMM_ROUTINE_UPDATE(omm_camera_update_invert_axis) {
    bool prevConfigCameraInvertX = configCameraInvertX;
    bool prevConfigCameraInvertY = configCameraInvertY;
    bool isFirstPersonCamera = (sOmmCamFpMode || gOmmGlobals->cameraSnapshotMode || gMarioState->action == ACT_IN_CANNON);
    bool isReadingStatsBoard = obj_get_first_with_behavior_and_field_s32(bhvOmmStatsBoard, _FIELD(oAction), 1) != NULL;

    // Lakitu/Mario cam
    // X axis is always inverted in 3rd person
    // Y axis is always inverted in 1st person
    if (OMM_CAMERA_CLASSIC && !BETTER_CAM_IS_ENABLED) {
        configCameraInvertX = !isFirstPersonCamera;
        configCameraInvertY = isFirstPersonCamera;
    }

    // ACT_READING_SIGN (but not the stats board)
    // This action uses the C-Up movement, Y axis is inverted
    else if (gMarioState->action == ACT_READING_SIGN && !isReadingStatsBoard && !omm_is_game_paused()) {
        gPlayer1Controller->stickX += (gOmmGlobals->mouseDeltaX * 1.6f) * BETTER_CAM_MOUSE_CAM;
        gPlayer1Controller->stickX *= (gOmmCameraInvert1stPerson & OMM_CAMERA_INVERT_X ? -1 : +1);
        gPlayer1Controller->stickY -= (gOmmGlobals->mouseDeltaY * 1.6f) * BETTER_CAM_MOUSE_CAM;
        gPlayer1Controller->stickY *= (gOmmCameraInvert1stPerson & OMM_CAMERA_INVERT_Y ? +1 : -1);
    }

    // 1st person
    // No inversion
    else if (isFirstPersonCamera) {
        configCameraInvertX = ((gOmmCameraInvert1stPerson & OMM_CAMERA_INVERT_X) != 0);
        configCameraInvertY = ((gOmmCameraInvert1stPerson & OMM_CAMERA_INVERT_Y) != 0);
    }

    // 3rd person
    // Better cam (not Puppycam) has its Y axis inverted
    else {
        configCameraInvertX = ((gOmmCameraInvert3rdPerson & OMM_CAMERA_INVERT_X) != 0);
        configCameraInvertY = ((gOmmCameraInvert3rdPerson & OMM_CAMERA_INVERT_Y) != 0) ^ (OMM_CAMERA_CLASSIC && BETTER_CAM_IS_ENABLED && !BETTER_CAM_IS_PUPPY_CAM);
    }

    // Update better cam settings if needed
    if (prevConfigCameraInvertX != configCameraInvertX || prevConfigCameraInvertY != configCameraInvertY) {
        omm_camera_update_settings();
    }
}

//
// Snapshot mode
//

typedef struct {
    Vec3f pos;
    s16 pitch;
    s16 yaw;
    s16 roll;
    f32 dist;
    struct Camera camera;
    struct LakituState lakituState;
    bool help;
} OmmCameraSnapshot;

static OmmCameraSnapshot sOmmCamSnapshot[1] = {{ .help = true }};

static void omm_camera_snapshot_process_inputs() {

    // Pitch
    if (abs_s(gPlayer1Controller->extStickX) < 50 && abs_s(gPlayer1Controller->extStickY) > abs_s(gPlayer1Controller->extStickX)) {
        sOmmCamSnapshot->pitch -= 0x200 * OMM_CAMERA_Y * (gPlayer1Controller->extStickY / 127.f);
    } else {
        if (gPlayer1Controller->buttonDown & U_CBUTTONS) {
            sOmmCamSnapshot->pitch -= 0x200 * OMM_CAMERA_Y;
        }
        if (gPlayer1Controller->buttonDown & D_CBUTTONS) {
            sOmmCamSnapshot->pitch += 0x200 * OMM_CAMERA_Y;
        }
    }
    sOmmCamSnapshot->pitch = clamp_s(sOmmCamSnapshot->pitch, -0x3FF0, +0x3FF0);

    // Yaw
    if (abs_s(gPlayer1Controller->extStickY) < 50 && abs_s(gPlayer1Controller->extStickX) > abs_s(gPlayer1Controller->extStickY)) {
        sOmmCamSnapshot->yaw -= 0x400 * OMM_CAMERA_X * (gPlayer1Controller->extStickX / 127.f);
    } else {
        if (gPlayer1Controller->buttonDown & L_CBUTTONS) {
            sOmmCamSnapshot->yaw += 0x400 * OMM_CAMERA_X;
        }
        if (gPlayer1Controller->buttonDown & R_CBUTTONS) {
            sOmmCamSnapshot->yaw -= 0x400 * OMM_CAMERA_X;
        }
    }

    // Roll
    if (gPlayer1Controller->buttonDown & U_JPAD) {
        sOmmCamSnapshot->roll = 0;
    } else {
        if (gPlayer1Controller->buttonDown & L_JPAD) {
            sOmmCamSnapshot->roll += 0x200;
        }
        if (gPlayer1Controller->buttonDown & R_JPAD) {
            sOmmCamSnapshot->roll -= 0x200;
        }
    }

    // XZ
    s16 moveYaw = atan2s(-gPlayer1Controller->stickY, gPlayer1Controller->stickX) + (sOmmCamSnapshot->yaw + 0x8000);
    sOmmCamSnapshot->pos[0] += 50.f * (gPlayer1Controller->stickMag / 64.f) * sins(moveYaw);
    sOmmCamSnapshot->pos[2] += 50.f * (gPlayer1Controller->stickMag / 64.f) * coss(moveYaw);
    sOmmCamSnapshot->pos[0] = clamp_f(sOmmCamSnapshot->pos[0], -LEVEL_BOUNDARY_MAX, +LEVEL_BOUNDARY_MAX);
    sOmmCamSnapshot->pos[2] = clamp_f(sOmmCamSnapshot->pos[2], -LEVEL_BOUNDARY_MAX, +LEVEL_BOUNDARY_MAX);

    // Y
    if (gPlayer1Controller->buttonDown & A_BUTTON) {
        sOmmCamSnapshot->pos[1] += 40.f;
    }
    if (gPlayer1Controller->buttonDown & B_BUTTON) {
        sOmmCamSnapshot->pos[1] -= 40.f;
    }
    sOmmCamSnapshot->pos[1] = clamp_f(sOmmCamSnapshot->pos[1], -LEVEL_BOUNDARY_MAX, +LEVEL_BOUNDARY_MAX);

    // Toggle help
    if (gPlayer1Controller->buttonPressed & D_JPAD) {
        sOmmCamSnapshot->help = !sOmmCamSnapshot->help;
    }
}

bool omm_camera_snapshot_mode_init() {
    if (omm_is_main_menu() || !gCurrentArea || !gCurrentArea->camera) {
        return false;
    }

    // Init snapshot camera
    gCamera = gCurrentArea->camera;
    vec3f_copy(sOmmCamSnapshot->pos, gCamera->pos);
    sOmmCamSnapshot->pitch = -atan2s(vec3f_hdist(gCamera->focus, gCamera->pos), gCamera->focus[1] - gCamera->pos[1]);
    sOmmCamSnapshot->yaw = atan2s(gCamera->focus[2] - gCamera->pos[2], gCamera->focus[0] - gCamera->pos[0]);
    sOmmCamSnapshot->roll = gLakituState.roll;
    sOmmCamSnapshot->dist = vec3f_dist(gCamera->pos, gCamera->focus);
    sOmmCamSnapshot->camera = *gCamera;
    sOmmCamSnapshot->lakituState = gLakituState;

    // Enter snapshot mode
    gOmmGlobals->cameraSnapshotMode = true;
    play_sound(SOUND_MENU_MARIO_CASTLE_WARP2 | 0xFF00, gGlobalSoundArgs);
    return true;
}

void omm_camera_snapshot_mode_update() {
    if (omm_is_main_menu() || !gCurrentArea || !gCurrentArea->camera) {
        gOmmGlobals->cameraSnapshotMode = false;
        return;
    }

    // Process inputs
    gCamera = gCurrentArea->camera;
    omm_camera_snapshot_process_inputs();

    // Exit shapshot mode and restore camera and lakitu state
    if (gPlayer1Controller->buttonPressed & (Z_TRIG | START_BUTTON)) {
        *gCamera = sOmmCamSnapshot->camera;
        gLakituState = sOmmCamSnapshot->lakituState;
        gOmmGlobals->cameraSnapshotMode = false;
        play_sound(SOUND_MENU_CHANGE_SELECT | 0xFF00, gGlobalSoundArgs);
        return;
    }

    // Update camera
    gCamera->yaw = sOmmCamSnapshot->yaw;
    gCamera->nextYaw = sOmmCamSnapshot->yaw;
    vec3f_copy(gCamera->pos, sOmmCamSnapshot->pos);
    vec3f_transform(
        gCamera->focus,
        gVec3fZ,
        sOmmCamSnapshot->pos,
        (Vec3s) { sOmmCamSnapshot->pitch, sOmmCamSnapshot->yaw, 0 },
        (Vec3f) { sOmmCamSnapshot->dist, sOmmCamSnapshot->dist, sOmmCamSnapshot->dist }
    );

    // Update lakitu state
    vec3f_copy(gLakituState.focus, gCamera->focus);
    vec3f_copy(gLakituState.curFocus, gCamera->focus);
    vec3f_copy(gLakituState.goalFocus, gCamera->focus);
    vec3f_copy(gLakituState.pos, gCamera->pos);
    vec3f_copy(gLakituState.curPos, gCamera->pos);
    vec3f_copy(gLakituState.goalPos, gCamera->pos);
    gLakituState.roll = sOmmCamSnapshot->roll;
    gLakituState.yaw = gCamera->yaw;
    gLakituState.nextYaw = gCamera->nextYaw;
    vec3s_zero(gLakituState.shakeMagnitude);
    gLakituState.shakePitchPhase = 0;
    gLakituState.shakePitchVel = 0;
    gLakituState.shakePitchDecay = 0;
    gLakituState.shakeRollPhase = 0;
    gLakituState.shakeRollVel = 0;
    gLakituState.shakeRollDecay = 0;
    gLakituState.shakeYawPhase = 0;
    gLakituState.shakeYawVel = 0;
    gLakituState.shakeYawDecay = 0;
    gLakituState.focHSpeed = 0;
    gLakituState.focVSpeed = 0;
    gLakituState.posHSpeed = 0;
    gLakituState.posVSpeed = 0;
    gLakituState.keyDanceRoll = 0;

    // Render help
    if (sOmmCamSnapshot->help) {
        s16 x = GFX_DIMENSIONS_FROM_LEFT_EDGE(4);
        s16 y = SCREEN_HEIGHT - 12;
        omm_render_string(x, y -  0, 0xE0, 0xE0, 0xE0, 0xFF, omm_text_convert(OMM_TEXT_CAMERA_SNAPSHOT_HELP_0, false), true);
        omm_render_string(x, y - 12, 0xE0, 0xE0, 0xE0, 0xFF, omm_text_convert(OMM_TEXT_CAMERA_SNAPSHOT_HELP_1, false), true);
        omm_render_string(x, y - 24, 0xE0, 0xE0, 0xE0, 0xFF, omm_text_convert(OMM_TEXT_CAMERA_SNAPSHOT_HELP_2, false), true);
        omm_render_string(x, y - 36, 0xE0, 0xE0, 0xE0, 0xFF, omm_text_convert(OMM_TEXT_CAMERA_SNAPSHOT_HELP_3, false), true);
        omm_render_string(x, y - 48, 0xE0, 0xE0, 0xE0, 0xFF, omm_text_convert(OMM_TEXT_CAMERA_SNAPSHOT_HELP_4, false), true);
        omm_render_string(x, y - 60, 0xE0, 0xE0, 0xE0, 0xFF, omm_text_convert(OMM_TEXT_CAMERA_SNAPSHOT_HELP_5, false), true);
        omm_render_string(x, y - 72, 0xE0, 0xE0, 0xE0, 0xFF, omm_text_convert(OMM_TEXT_CAMERA_SNAPSHOT_HELP_6, false), true);
    }
}

//
// Cutscenes
//

void stop_cutscene(struct Camera *c) {
    if (c && c->cutscene != 0) {
        sMarioCamState->cameraEvent = 0;
        sFramesSinceCutsceneEnded = 0;
        gCutsceneTimer = 0;
        sCutsceneShot = 0;
        gObjCutsceneDone = TRUE;
        gRecentCutscene = 0;
        c->cutscene = 0;
    }
}

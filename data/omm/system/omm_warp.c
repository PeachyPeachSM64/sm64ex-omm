#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/omm_constants.h"

static struct {
    s32 levelNum;
    s32 areaIndex;
    s32 actNum;
    bool exit;
    bool active;
    bool instant;
    s32 exitTimer;
    s32 targetArea;
    Warp *targetWarp;
} sOmmWarpState[1];

//
// Warps
//

bool omm_warp_to_level(s32 levelNum, s32 areaIndex, s32 actNum) {
    if (omm_level_can_warp(levelNum) && omm_level_get_entry_warp(levelNum, areaIndex)) {
        sOmmWarpState->levelNum = levelNum;
        sOmmWarpState->areaIndex = areaIndex;
        sOmmWarpState->actNum = actNum;
        sOmmWarpState->exit = false;
        sOmmWarpState->active = true;
        return true;
    }
    return false;
}

bool omm_restart_level() {
#if OMM_GAME_IS_SM74
    return omm_restart_area();
#else
    if (omm_warp_to_level(gCurrLevelNum,
#if OMM_GAME_IS_SM64
        (gCurrLevelNum == LEVEL_THI && gCurrAreaIndex == 2 ? 2 : 1),
#else
        1,
#endif
        gCurrActNum)) {
        omm_stats_increase(restarts, 1);
        return true;
    }
#endif
}

bool omm_restart_area() {
    for (s32 i = gCurrAreaIndex; i != 0; --i) {
        if (omm_warp_to_level(gCurrLevelNum, i, gCurrActNum)) {
            omm_stats_increase(restarts, 1);
            return true;
        }
    }
    return false;
}

bool omm_exit_level(s32 levelNum, s32 areaIndex, bool instant) {
    if (omm_level_can_warp(levelNum)) {

        // Close the pause menu if it was open
        if (optmenu_open) optmenu_toggle();
        level_set_transition(0, NULL);
        gDialogBoxState = 0;
        gMenuMode = -1;

        // Cancel out every music/sound/sequence
        music_unlower_volume(SEQ_PLAYER_LEVEL, 0);
        for (u16 seqid = 0; seqid != SEQ_COUNT; ++seqid) {
            stop_background_music(seqid);
        }
        audio_play_shell_music();
        audio_stop_shell_music();
        audio_stop_cap_music();
        func_80321080(0);
        fadeout_music(0);
        fadeout_level_music(0);

        // Play a fade-out transition, and trigger a "fake" warp
        if (!instant) {
            play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 16, 0, 0, 0);
            level_set_transition(30, NULL);
            warp_special(0);
        }
        sWarpDest.type = WARP_TYPE_NOT_WARPING;
        sOmmWarpState->levelNum = levelNum;
        sOmmWarpState->areaIndex = areaIndex;
        sOmmWarpState->exit = true;
        sOmmWarpState->active = true;
        sOmmWarpState->instant = instant;
        return true;
    }
    return false;
}

bool omm_return_to_castle(bool fadeOut, bool force) {
    if (force || (!omm_is_game_paused() && !omm_is_transition_active() && !omm_is_warping() && sCurrPlayMode != PLAY_MODE_CHANGE_LEVEL)) {
        initiate_warp(OMM_LEVEL_RETURN_TO_CASTLE, 0);
        if (fadeOut) {
            fade_into_special_warp(0, 0);
        } else {
            level_set_transition(0, NULL);
            warp_special(0);
        }
        gSavedCourseNum = COURSE_NONE;
        return true;
    }
    return false;
}

bool omm_is_warping() {
    return sOmmWarpState->active;
}

//
// Update
//

static void omm_exit_level_find_position(f32 *x0, f32 *y0, f32 *z0, s16 yaw, f32 dist) {
    for (f32 d = dist; d > 0.f; d -= 10.f) {
        f32 x = *x0 + d * sins(yaw + 0x8000);
        f32 z = *z0 + d * coss(yaw + 0x8000);
        f32 waterLevel = find_water_level(x, z);
        for (f32 dy = 0.f; dy <= 5000.f; dy += 100.f) {
            f32 y = *y0 + dy;
            struct Surface *floor;
            f32 floorY = find_floor(x, y, z, &floor);
            if (floor &&
                floorY > waterLevel &&
                floor->type != SURFACE_WARP &&
                floor->type != SURFACE_BURNING &&
                floor->type != SURFACE_DEATH_PLANE &&
                floor->type != SURFACE_VERTICAL_WIND &&
                floor->type != SURFACE_DEEP_QUICKSAND &&
                floor->type != SURFACE_INSTANT_QUICKSAND &&
                floor->type != SURFACE_INSTANT_MOVING_QUICKSAND) {
                *x0 = x;
                *y0 = floorY;
                *z0 = z;
                return;
            }
        }
    }
}

void *omm_update_warp(void *cmd, bool inited) {
    if (sOmmWarpState->active) {

        // Exit level
        if (sOmmWarpState->exit) {

            // Close the pause menu if it was open
            if (optmenu_open) optmenu_toggle();
            level_set_transition(0, NULL);
            gDialogBoxState = 0;
            gMenuMode = -1;

            // Phase 0 - Wait for the transition to end
            if (sOmmWarpState->targetArea == -1 && !sOmmWarpState->instant && omm_is_transition_active()) {
                sOmmWarpState->exitTimer = 14;
                return NULL;
            }

            // Phase 1 - Clear the previous level and set up the new level
            if (sOmmWarpState->targetArea == -1) {

                // Wait 14 more frames...
                if (sOmmWarpState->exitTimer-- > 0) {
                    return NULL;
                }

                // Bowser levels
                if (sOmmWarpState->levelNum == LEVEL_BOWSER_1) sOmmWarpState->levelNum = LEVEL_BITDW;
                if (sOmmWarpState->levelNum == LEVEL_BOWSER_2) sOmmWarpState->levelNum = LEVEL_BITFS;
                if (sOmmWarpState->levelNum == LEVEL_BOWSER_3) sOmmWarpState->levelNum = LEVEL_BITS;

                // Exit warp to Castle warp
                // Uses the death warp, as it's the only warp that exists for every stage in the game
                // In case it doesn't exist, fall back to the "Return to Castle" warp
                Warp *warp = omm_level_get_death_warp(sOmmWarpState->levelNum, sOmmWarpState->areaIndex);
                Warp *targetWarp = (
                    warp != NULL ?
                    omm_level_get_warp(warp->dstLevelNum, warp->dstAreaIndex, warp->dstId) :
                    omm_level_get_warp(OMM_LEVEL_RETURN_TO_CASTLE)
                );

                // Free everything from the current level
                clear_objects();
                clear_area_graph_nodes();
                clear_areas();
                main_pool_pop_state();

                // Reset Mario's state
                gMarioState->action = 0;
                gMarioState->healCounter = 0;
                gMarioState->hurtCounter = 0;
                gMarioState->numCoins = 0;
                gMarioState->input = 0;
                gMarioState->controller->buttonPressed = 0;
                gHudDisplay.coins = 0;
                gOmmData->reset();

                // Set up new level values
                gCurrLevelNum = targetWarp->srcLevelNum;
                gCurrCourseNum = omm_level_get_course(gCurrLevelNum);
                gSavedCourseNum = gCurrCourseNum;
                gCurrAreaIndex = targetWarp->srcAreaIndex;
                sOmmWarpState->targetArea = targetWarp->srcAreaIndex;
                sOmmWarpState->targetWarp = targetWarp;

                // Set up new level script
                sWarpDest.type = WARP_TYPE_NOT_WARPING;
                sWarpDest.levelNum = 0;
                sWarpDest.areaIdx = gCurrAreaIndex;
                sWarpDest.nodeId = 0;
                sWarpDest.arg = 0;
                return (void *) omm_level_get_script(gCurrLevelNum);

            } else {

                // Phase 2 - Set Mario spawn info after the MARIO_POS command
                if (*((u8 *) cmd) == 0x2B) {
                    gMarioSpawnInfo->areaIndex = sOmmWarpState->targetArea;
                    gCurrAreaIndex = sOmmWarpState->targetArea;
                }

                // Phase 3 - End level initialization
                if (sOmmWarpState->targetWarp && inited) {
#if OMM_GAME_IS_SM64
                    // SM64 only: Drain the moat before searching for a valid exit point
                    struct Object *moatDrainer = obj_get_first_with_behavior(bhvInvisibleObjectsUnderBridge);
                    if (moatDrainer) {
                        obj_update(moatDrainer);
                    }
#endif

                    // Find target position
                    f32 x = sOmmWarpState->targetWarp->x;
                    f32 y = sOmmWarpState->targetWarp->y;
                    f32 z = sOmmWarpState->targetWarp->z;
                    s16 yaw = sOmmWarpState->targetWarp->yaw;
                    omm_exit_level_find_position(&x, &y, &z, yaw, OMM_LEVEL_EXIT_DISTANCE);

                    // Init Mario
                    gMarioSpawnInfo->startPos[0] = (s16) x;
                    gMarioSpawnInfo->startPos[1] = (s16) y;
                    gMarioSpawnInfo->startPos[2] = (s16) z;
                    gMarioSpawnInfo->startAngle[0] = 0;
                    gMarioSpawnInfo->startAngle[1] = yaw;
                    gMarioSpawnInfo->startAngle[2] = 0;
                    gMarioSpawnInfo->areaIndex = gCurrAreaIndex;
                    init_mario();
                    set_mario_initial_action(gMarioState, MARIO_SPAWN_UNKNOWN_02, 0);
                    omm_health_set_max(gMarioState);

                    // Init transition
                    reset_camera(gCurrentArea->camera);
                    init_camera(gCurrentArea->camera);
                    omm_camera_init();
                    sDelayedWarpOp = WARP_OP_NONE;
                    gTimeStopState &= ~(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                    play_transition(WARP_TRANSITION_FADE_FROM_STAR, 15, 0, 0, 0);
                    play_sound(SOUND_MENU_MARIO_CASTLE_WARP, gGlobalSoundArgs);

                    // Set music
                    music_stop();
                    set_background_music(gCurrentArea->musicParam, gCurrentArea->musicParam2, 0);
                    sOmmWarpState->targetWarp = NULL;
                }

                // Phase 4 - Unlock Mario as soon as the second transition is ended
                if (!sOmmWarpState->targetWarp && !omm_is_transition_active()) {
                    sOmmWarpState->active = false;
                    sOmmWarpState->targetArea = -1;
                }
            }
        }

        // Warp to level
        else {

            // Phase 1 - Clear the previous level and set up the new level
            if (sOmmWarpState->targetArea == -1) {

                // Close the pause menu if it was open
                if (optmenu_open) optmenu_toggle();
                level_set_transition(0, NULL);
                gDialogBoxState = 0;
                gMenuMode = -1;

                // Cancel out every music/sound/sequence
                audio_mute(FALSE);
                music_unlower_volume(SEQ_PLAYER_LEVEL, 0);
                for (u16 seqid = 0; seqid != SEQ_COUNT; ++seqid) {
                stop_background_music(seqid);
                }
                audio_play_shell_music();
                audio_stop_shell_music();
                audio_stop_cap_music();
                func_80321080(0);
                fadeout_music(0);
                fadeout_level_music(0);

                // Free everything from the current level
                clear_objects();
                clear_area_graph_nodes();
                clear_areas();
                main_pool_pop_state();

                // Reset Mario's state
                gMarioState->action = 0;
                gMarioState->healCounter = 0;
                gMarioState->hurtCounter = 0;
                gMarioState->numCoins = 0;
                gMarioState->input = 0;
                gMarioState->controller->buttonPressed = 0;
                gHudDisplay.coins = 0;
                gOmmData->reset();

                // Set up new level values
                gCurrLevelNum = sOmmWarpState->levelNum;
                gCurrCourseNum = omm_level_get_course(gCurrLevelNum);
                gSavedCourseNum = gCurrCourseNum;
                gCurrActNum = max_s(1, sOmmWarpState->actNum * (gCurrCourseNum <= COURSE_STAGES_MAX));
                gCurrAreaIndex = sOmmWarpState->areaIndex;
                sOmmWarpState->targetArea = gCurrAreaIndex;

                // Set up new level script
                sWarpDest.type = WARP_TYPE_NOT_WARPING;
                sWarpDest.levelNum = 0;
                sWarpDest.areaIdx = gCurrAreaIndex;
                sWarpDest.nodeId = 0;
                sWarpDest.arg = 0;
                return (void *) omm_level_get_script(gCurrLevelNum);

            } else {

                // Phase 2 - Set Mario spawn info after the MARIO_POS command
                if (*((u8 *) cmd) == 0x2B) {
                    gMarioSpawnInfo->areaIndex = sOmmWarpState->targetArea;
                    gCurrAreaIndex = sOmmWarpState->targetArea;
                }

                // Phase 3 - End level initialization
                if (inited) {

                    // Init Mario
                    Warp *warp = omm_level_get_entry_warp(gCurrLevelNum, gCurrAreaIndex);
                    s16 spawnType = sSpawnTypeFromWarpBhv[warp->srcType];
                    gMarioSpawnInfo->startPos[0] = (s16) (warp->x + (spawnType == MARIO_SPAWN_DOOR_WARP) * 300.f * sins(warp->yaw));
                    gMarioSpawnInfo->startPos[1] = (s16) (warp->y);
                    gMarioSpawnInfo->startPos[2] = (s16) (warp->z + (spawnType == MARIO_SPAWN_DOOR_WARP) * 300.f * coss(warp->yaw));
                    gMarioSpawnInfo->startAngle[0] = 0;
                    gMarioSpawnInfo->startAngle[1] = warp->yaw;
                    gMarioSpawnInfo->startAngle[2] = 0;
                    gMarioSpawnInfo->areaIndex = gCurrAreaIndex;
                    init_mario();
                    set_mario_initial_action(gMarioState, spawnType, 0);
                    omm_health_set_max(gMarioState);

                    // Init transition
                    reset_camera(gCurrentArea->camera);
                    init_camera(gCurrentArea->camera);
                    omm_camera_init();
                    sDelayedWarpOp = WARP_OP_NONE;
                    switch (spawnType) {
                        case MARIO_SPAWN_UNKNOWN_03:           play_transition(WARP_TRANSITION_FADE_FROM_STAR,   0x10, 0x00, 0x00, 0x00); break;
                        case MARIO_SPAWN_DOOR_WARP:            play_transition(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00); break;
                        case MARIO_SPAWN_TELEPORT:             play_transition(WARP_TRANSITION_FADE_FROM_COLOR,  0x14, 0xFF, 0xFF, 0xFF); break;
                        case MARIO_SPAWN_SPIN_AIRBORNE:        play_transition(WARP_TRANSITION_FADE_FROM_COLOR,  0x1A, 0xFF, 0xFF, 0xFF); break;
                        case MARIO_SPAWN_SPIN_AIRBORNE_CIRCLE: play_transition(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00); break;
                        case MARIO_SPAWN_UNKNOWN_27:           play_transition(WARP_TRANSITION_FADE_FROM_COLOR,  0x10, 0x00, 0x00, 0x00); break;
                        default:                               play_transition(WARP_TRANSITION_FADE_FROM_STAR,   0x10, 0x00, 0x00, 0x00); break;
                    }

                    // Set music
                    music_stop();
                    set_background_music(gCurrentArea->musicParam, gCurrentArea->musicParam2, 0);
                    if (gMarioState->flags & MARIO_WING_CAP)   audio_play_wing_cap_music();
                    if (gMarioState->flags & MARIO_METAL_CAP)  audio_play_metal_cap_music();
                    if (gMarioState->flags & MARIO_VANISH_CAP) audio_play_vanish_cap_music();

                    if (OMM_LEVEL_IS_BOWSER_FIGHT(gCurrLevelNum)) {
                        sound_banks_enable(0, 0xFFFF); // Bowser levels sound fix
                    }

                    // Reset values
                    sOmmWarpState->active = false;
                    sOmmWarpState->targetArea = -1;
                }
            }
        }
    } else {
        sOmmWarpState->exitTimer = 0;
        sOmmWarpState->targetArea = -1;
        sOmmWarpState->targetWarp = NULL;
    }
    return NULL;
}

//
// Instant warp (endless stairs, linked areas)
//

static void omm_instant_warp_apply_displacement(struct Object *o, Vec3f displacement) {
    vec3f_add(&o->oPosX, displacement);
    vec3f_add(o->oGfxPos, displacement);
    o->oFlags |= OBJ_FLAG_INSTANT_WARP;
}

static void omm_instant_warp_move_objects_within_radius(Vec3f pos, f32 radius, s32 *objLists, Vec3f displacement) {
    for (s32 *objList = objLists; *objList != -1; ++objList) {
        for_each_object_in_list(obj, *objList) {
            if (obj != gMarioObject && obj->activeFlags && !(obj->oFlags & OBJ_FLAG_INSTANT_WARP) &&
                !(obj->oInteractType & (INTERACT_DOOR | INTERACT_WARP_DOOR | INTERACT_WARP)) &&
                obj_get_distance_vec3f(obj, pos) <= radius
            ) {
                omm_instant_warp_apply_displacement(obj, displacement);
            }
        }
    }
}

static void omm_instant_warp_move_objects_around_mario(struct MarioState *m, Vec3f displacement) {

    // Move Cappy (and its particles) if in range
    struct Object *cappy = omm_cappy_get_object();
    if (cappy && !(cappy->oFlags & OBJ_FLAG_INSTANT_WARP) && obj_get_distance_vec3f(cappy, m->pos) <= 1500) {
        omm_instant_warp_move_objects_within_radius(&cappy->oPosX, 500, array_of(s32) { OBJ_LIST_UNIMPORTANT, -1 }, displacement);
        omm_instant_warp_apply_displacement(cappy, displacement);
        omm_instant_warp_move_objects_within_radius(&cappy->oPosX, 500, array_of(s32) { OBJ_LIST_UNIMPORTANT, -1 }, displacement);
    }

    // Move objects in range
    omm_instant_warp_move_objects_within_radius(m->pos, 1000, gOmmAllObjectLists, displacement);
}

void omm_process_instant_warp(struct MarioState *m, Vec3f displacement, s16 areaIndex) {

    // Move the capture
    if (omm_mario_is_capture(m)) {
        vec3f_add(gOmmMario->capture.animPos[0], displacement);
        vec3f_add(gOmmMario->capture.animPos[1], displacement);
        vec3f_add(gOmmMario->capture.animPos[2], displacement);
        omm_instant_warp_apply_displacement(gOmmCapture, displacement);

        // Move the capture cap
        struct Object *cap = obj_get_first_with_behavior(bhvOmmPossessedObjectCap);
        if (cap) {
            omm_instant_warp_apply_displacement(cap, displacement);
        }
    }

    // Move objects before Mario
    omm_instant_warp_move_objects_around_mario(m, displacement);

    // Move Mario
    vec3f_add(m->pos, displacement);
    vec3f_add(gOmmMario->state.previous.pos, displacement);
    omm_instant_warp_apply_displacement(m->marioObj, displacement);
    m->floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &m->floor);

    // Move objects after Mario
    omm_instant_warp_move_objects_around_mario(m, displacement);

    // Load the destination area if needed
    s16 camYaw = gCurrentArea->camera->yaw;
    s16 camNextYaw = gCurrentArea->camera->nextYaw;
    change_area(areaIndex);
    m->area = gCurrentArea;

    // Move the camera
    gCurrentArea->camera->yaw = camYaw;
    gCurrentArea->camera->nextYaw = camNextYaw;
    omm_camera_warp(gCurrentArea->camera, displacement);

    // Smooth interp
    vec3f_copy(gOmmGlobals->instantWarp.displacement, displacement);
    gOmmGlobals->instantWarp.warped = true;
}

void omm_check_instant_warp() {
    struct MarioState *m = gMarioState;
    struct Surface *floor = m->floor;

#if OMM_GAME_IS_SM64
    // Endless stairs
    if (gCurrLevelNum == LEVEL_CASTLE && omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE) >= 70) {
        return;
    }
#endif

    if (floor != NULL && gCurrentArea->instantWarps != NULL) {
        s32 index = floor->type - SURFACE_INSTANT_WARP_1B;
        if (index >= INSTANT_WARP_INDEX_START && index < INSTANT_WARP_INDEX_STOP) {
            struct InstantWarp *warp = &gCurrentArea->instantWarps[index];
            if (warp->id != 0) {
                Vec3f displacement;
                vec3s_to_vec3f(displacement, warp->displacement);
                omm_process_instant_warp(m, displacement, warp->area);
            }
        }
    }
}

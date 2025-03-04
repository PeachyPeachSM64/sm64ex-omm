#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "levels/castle_inside/header.h"

#pragma GCC push_options
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool omm_sparkly_level_jrb__flooded_frozen(struct MarioState *m, const s32 *params) {
    // if (omm_world_is_flooded() && m->area) {
    //     m->area->terrainType = TERRAIN_SNOW;
    //     if (OMM_SPARKLY_STATE_IS_OK) {

    //         // Set dormant 'living' objects
    //         for_each_object_with_behavior(obj, bhvUnagi) { obj_set_dormant(obj, true); }
    //         for_each_object_with_behavior(obj, bhvUnagiSubobject) { obj_set_dormant(obj, true); }
    //         for_each_object_with_behavior(obj, bhvBobombBuddy) { obj_set_dormant(obj, true); }
    //         for_each_object_with_behavior(obj, bhvBobombBuddyOpensCannon) { obj_set_dormant(obj, true); }
    //         for_each_object_with_behavior(obj, bhvKoopaShellUnderwater) { obj_set_dormant(obj, true); }

    //         // Force Calm Vibe, and set freeze timer to about to damage Peach
    //         omm_peach_vibe_activate(m, OMM_PEACH_VIBE_TYPE_CALM);
    //         // gOmmMario->state.freeze.dmg = max_s(gOmmMario->state.freeze.dmg, OMM_FREEZING_WATER_DURATION - 3);
    //         // gOmmMario->state.freeze.gfx = OMM_FREEZING_WATER_DURATION;

    //         // TODO: Rework this

    //         // // If the star is spawned, reduce the Vibe gauge consumption to give just enough time to reach the star
    //         // if (gOmmSparklyContext->starSpawned) {
    //         //     if (omm_peach_vibe_is_calm() && !gOmmSparkly->timeStop && gOmmSparkly->marioUpdated) {
    //         //         gOmmPeach->vibeGauge = clamp_s(gOmmPeach->vibeGauge - OMM_PEACH_VIBE_GAUGE_CALM_FREEZE_DEC / 2, 0, OMM_PEACH_VIBE_GAUGE_MAX);
    //         //     }
    //         // }

    //         // // Collecting coins is the only way to refill the Vibe gauge
    //         // else {
    //         //     s32 dy = max_s(0, gOmmSparklyContext->coinsYellowDiff);
    //         //     s32 dr = max_s(0, gOmmSparklyContext->coinsRedDiff);
    //         //     s32 db = max_s(0, gOmmSparklyContext->coinsBlueDiff);
    //         //     s32 dl = max_s(0, gOmmSparklyContext->coinsLostDiff);
    //         //     s32 inc = (dy + dl + 2 * dr + 5 * db) * ((OMM_PEACH_VIBE_GAUGE_MAX * 3) / 100);
    //         //     gOmmPeach->vibeGauge = clamp_s(gOmmPeach->vibeGauge - inc, 0, OMM_PEACH_VIBE_GAUGE_MAX);
    //         // }
    //     }
    // } else {
    //     omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
    // }
    return true;
}

bool omm_sparkly_level_ddd__switch_and_boxes(struct MarioState *m, const s32 *params) {
    if (OMM_SPARKLY_STATE_IS_OK && gCurrAreaIndex == 2 && !gOmmSparklyContext->objectsSpawned) {
        f32 x = params[0];
        f32 y = params[1];
        f32 z = params[2];

        // Unload previous stuff
        obj_unload_all_with_behavior(bhvFloorSwitchHiddenObjects);
        obj_unload_all_with_behavior(bhvHiddenObject);

        // Purple switch
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_PURPLE_SWITCH, bhvFloorSwitchHiddenObjects, x, y, z, 0, 0, 0);

        // Boxes
        for_each_in_(Vec3f, boxPos, {
            { x - 200, y + 1600, z - 200 }, { x - 200, y + 1600, z +   0 }, { x - 200, y + 1600, z + 200 },
            { x +   0, y + 1600, z - 200 }, { x +   0, y + 1600, z +   0 }, { x +   0, y + 1600, z + 200 },
            { x + 200, y + 1600, z - 200 }, { x + 200, y + 1600, z +   0 }, { x + 200, y + 1600, z + 200 },
            { x - 100, y + 3400, z - 100 }, { x - 100, y + 3400, z + 100 },
            { x + 100, y + 3400, z - 100 }, { x + 100, y + 3400, z + 100 },
            { x +   0, y + 5200, z +   0 },
        }) {
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_BREAKABLE_BOX, bhvHiddenObject, (*boxPos)[0], (*boxPos)[1], (*boxPos)[2], 0, 0, 0);
        }

        gOmmSparklyContext->objectsSpawned = true;
    }
    return true;
}

bool omm_sparkly_level_sl__snowmen_at_top(struct MarioState *m, const s32 *params) {
    if (OMM_SPARKLY_STATE_IS_OK && !gOmmSparklyContext->successful) {
        f32 topX = params[1];
        f32 topY = params[2];
        f32 topZ = params[3];
        s32 maxSnowmenAtTop = params[0];
        s32 numSnowmenAtTop = 0;
        for_each_object_with_behavior(mrBlizzard, bhvMrBlizzard) {
            f32 dx = mrBlizzard->oPosX - topX;
            f32 dy = mrBlizzard->oPosY - topY;
            f32 dz = mrBlizzard->oPosZ - topZ;
            f32 dr = sqrtf(sqr_f(dx) + sqr_f(dz));
            numSnowmenAtTop += (dy > 0 && dr < 512);
        }
        if (numSnowmenAtTop >= maxSnowmenAtTop) {
            gOmmSparklyContext->successful = true;
            gOmmSparklyContext->spawner = obj_get_nearest_with_behavior(m->marioObj, bhvMrBlizzard);
        }
    }
    return true;
}

bool omm_sparkly_level_wdw__secret(struct MarioState *m, const s32 *params) {

    // Unlock secret
    if (m->action == ACT_OMM_SPARKLY_STAR_DANCE && gOmmSparklyContext->successful) {
        omm_secrets_unlock(OMM_SECRET_PEACH_SECRET_2);
    }

    // Set flag if on ground
    if ((m->action & ACT_GROUP_MASK) < ACT_GROUP_AIRBORNE && m->pos[1] < 4500.f) {
        gOmmSparklyContext->successful = true;
    }

    // Unset flag if...
    else if (
        omm_sparkly_are_regular_cheats_enabled(m, true) ||                          // Cheats
        (m->action == ACT_IN_CANNON || m->action == ACT_SHOT_FROM_CANNON) ||        // Cannon
        (m->action == ACT_RIDING_HOOT) ||                                           // Hoot
        (m->action & ACT_GROUP_MASK) != ACT_GROUP_AIRBORNE ||                       // Not in airborne action
        (m->flags & (MARIO_WING_CAP | MARIO_METAL_CAP | MARIO_VANISH_CAP)) != 0 ||  // Caps
        omm_mario_is_capture(m) ||                                                  // Capture
        omm_peach_vibe_is_joy()                                                     // Joy Vibe
    ) {
        gOmmSparklyContext->successful = false;
    }

    return true;
}

bool omm_sparkly_level_wdw__star_box_red_coins(struct MarioState *m, const s32 *params) {
    if (OMM_SPARKLY_STATE_IS_OK) {
        struct Object *starBox = obj_get_first_with_behavior(bhvOmmSparklyStarBox);

        // Outside
        if (gCurrAreaIndex == 1) {

            // Spawn star box
            if (!gOmmSparklyContext->objectsSpawned) {
                obj_unload_all_with_behavior(bhvBobombBuddy);
                obj_unload_all_with_behavior(bhvBobombBuddyOpensCannon);
                starBox = omm_obj_spawn_sparkly_star_box(m->marioObj, gOmmSparklyMode, 0, 3200, 3590, CELL_HEIGHT_LIMIT);
                gOmmSparklyContext->objectsSpawned = true;
            }

            // No cheesy grab
            if (starBox) {
                Vec3f boxMin[2] = { { -600, 3100, 2990 }, { 3530, -100, -3840 } };
                Vec3f boxMax[2] = { {  600, 5000, 4190 }, { 4610, 3100, -2760 } };
                starBox->oAction = (
                    vec3f_is_inside_box(&starBox->oPosX, boxMin[0], boxMax[0]) != vec3f_is_inside_box(m->pos, boxMin[0], boxMax[0]) ||
                    vec3f_is_inside_box(&starBox->oPosX, boxMin[1], boxMax[1]) != vec3f_is_inside_box(m->pos, boxMin[1], boxMax[1])
                );
            }

            // Don't return to area 1 after the star spawned
            if (gOmmSparklyContext->starSpawned) {
                omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
                gOmmSparklyContext->star = NULL;
            }
        }

        // Inside
        else if (gCurrAreaIndex == 2) {

            // Lower water level
            if (m->pos[2] > -2700) {
                if (*gEnvironmentLevels > -2600) {
                    *gEnvironmentLevels = (s32) approach_f32_symmetric(*gEnvironmentLevels, -2600, 40);
                    play_sound(SOUND_ENV_WATER_DRAIN, gGlobalSoundArgs);
                }
                for_each_object_with_behavior(obj, bhvWaterLevelDiamond) {
                    obj->oAction = WATER_LEVEL_DIAMOND_ACT_IDLE;
                }
                gWDWWaterLevelChanging = FALSE;
            }

            // Invalidate if anything but the star box collected a red coin
            if (gOmmSparklyContext->coinsRedDiff > 0) {
                omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
            } else if (starBox && starBox->oHeldState >= HELD_THROWN) {
                starBox->oIntangibleTimer = 0;
                omm_obj_process_interactions(starBox, OBJ_INT_COLLECT_COINS);
                starBox->oIntangibleTimer = -1;
                starBox->oInteractStatus = 0;
            }

            // Check if the Sparkly star is about to spawn
            struct Object *star = gOmmSparklyContext->star;
            if (starBox && gOmmSparklyContext->starSpawned && star) {

                // Spawn a dummy object to move the star spawn point to the center of the star box
                struct Object *dummy = obj_get_first_with_behavior(bhvStaticObject);
                if (!dummy) { dummy = spawn_object(m->marioObj, MODEL_NONE, bhvStaticObject); }
                obj_copy_pos(dummy, starBox);
                obj_reset_hitbox(dummy, 0, starBox->hitboxHeight / 2.f - 150.f, 0, 0, 0, 0);
                star->parentObj = dummy;

                // Explode star box if the Sparkly star is about to spawn
                if (star->oAction != 0 || star->oTimer >= 59) {
                    if (gOmmObject->state.heldObj == starBox) {
                        pobj_release_held_object();
                    }
                    switch (gOmmSparklyMode) {
                        case OMM_SPARKLY_MODE_NORMAL: obj_spawn_triangle_break_particles(starBox, OBJ_SPAWN_TRI_BREAK_PRESET_SWITCH_YELLOW); break;
                        case OMM_SPARKLY_MODE_HARD: obj_spawn_triangle_break_particles(starBox, OBJ_SPAWN_TRI_BREAK_PRESET_SWITCH_BLUE); break;
                        case OMM_SPARKLY_MODE_LUNATIC: obj_spawn_triangle_break_particles(starBox, OBJ_SPAWN_TRI_BREAK_PRESET_SWITCH_RED); break;
                    }
                    obj_create_sound_spawner(starBox, SOUND_GENERAL_BREAK_BOX);
                    obj_mark_for_deletion(starBox);
                }
            }

            // Invalidate if star box is missing
            if (!starBox && !gOmmSparklyContext->starSpawned) {
                omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
            }
        }

        // Update star box
        if (starBox && starBox->activeFlags) {
            starBox->oSparklyStarBoxMaxHeight = (gCurrAreaIndex == 2 ? -1300 : CELL_HEIGHT_LIMIT);
            if (!omm_is_game_paused() && gGlobalTimer % 2 == 0) {
                omm_obj_spawn_sparkly_star_sparkle(starBox, starBox->oSparklyStarMode, 150, 12.f, 0.5f, 150);
            }
        }
    }
    return true;
}

bool omm_sparkly_level_bitdw__goomba_and_coins(struct MarioState *m, const s32 *params) {

    // Verifying conditions...
    // Mario must be possessing a goomba or standing inside the safe zone without touching a coin
    if (!omm_mario_is_capture(m) || omm_capture_get_type(gOmmCapture) != OMM_CAPTURE_GOOMBA) {
        bool isInsideSafeZone = (m->pos[0] > -7925 && m->pos[0] < -6975 && m->pos[2] > +3350 && m->pos[2] < +4425);
        if (!isInsideSafeZone || (m->numCoins != 0 && !gOmmSparklyContext->starSpawned)) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        }
    }

    // Spawn a dormant goomba at the center of the safe zone
    if (!gOmmSparklyContext->objectsSpawned && OMM_SPARKLY_STATE_IS_OK) {
        struct Object *goomba = spawn_object_abs_with_rot(m->marioObj, 0, MODEL_GOOMBA, bhvGoomba, -7450, CELL_HEIGHT_LIMIT, 3887, 0, 0, 0);
        goomba->oBehParams = GOOMBA_SIZE_REGULAR << 16;
        goomba->oBehParams2ndByte = GOOMBA_SIZE_REGULAR;
        goomba->oFlags |= OBJ_FLAG_SPARKLY_NOT_ENEMY;
        goomba->parentObj = goomba;
        goomba->prevObj = NULL;
        obj_drop_to_floor(goomba);
        obj_set_dormant(goomba, true);
        gOmmSparklyContext->objectsSpawned = true;
    }

    // Possess the goomba as soon as it awakes
    if (OMM_SPARKLY_STATE_IS_OK && !gOmmSparklyContext->eventStarted && m->action != ACT_READING_SIGN) {
        struct Object *goomba = obj_get_nearest_with_behavior(m->marioObj, bhvGoomba);
        if (goomba && (goomba->oFlags & OBJ_FLAG_SPARKLY_NOT_ENEMY) && !obj_is_dormant(goomba)) {
            if (omm_mario_possess_object(m, goomba, OMM_MARIO_POSSESS_FORCE_CAPTURE)) {
                gOmmSparklyContext->eventStarted = true;
            }
        }
    }

    // Check Goomba stack requirement
    if (!gOmmSparklyContext->starSpawned) {
        s32 requiredStackedGoombas = params[0];
        if (!omm_mario_is_capture(m) || omm_capture_get_type(gOmmCapture) != OMM_CAPTURE_GOOMBA || gOmmObject->goomba.stackCount < requiredStackedGoombas) {
            gOmmSparklyContext->successful = false;
        }
    }

    // Start the timer as soon as Mario captures the goomba
    return omm_mario_is_capture(m) || gOmmSparklyContext->timerStarted;
}

bool omm_sparkly_level_bitfs__six_secrets(struct MarioState *m, const s32 *params) {
    if (!(m->action & ACT_FLAG_AIR) && (!omm_mario_is_capture(m) || obj_is_on_ground(gOmmCapture))) {
        gOmmSparklyContext->eventFlags = (u32) (m->pos[1] - FLOOR_LOWER_LIMIT);
    }

    // Spawn pillars, secrets and flamethrowers
    if (!gOmmSparklyContext->objectsSpawned) {
        omm_obj_spawn_bitfs_pillar(m->marioObj, -5528, -3100, -572);
        omm_obj_spawn_bitfs_pillar(m->marioObj, -5528, -3100, +572);
        omm_obj_spawn_bitfs_pillar(m->marioObj, +4300,  +250, -572);
        omm_obj_spawn_bitfs_pillar(m->marioObj, +4300,  +250, +572);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, -8000, -2800, -8000, 0, 0, 0);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, +8000, -2800, -8000, 0, 0, 0);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, +8000, -2800, +8000, 0, 0, 0);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, -8000, -2800, +8000, 0, 0, 0);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, +4300,  3000,     0, 0, 0, 0);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, -5528,  -300,     0, 0, 0, 0);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvFlamethrower, -5528,  -250, +600, 0x0000, 0x8000, 0x0000);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvFlamethrower, -5528,  -350, -600, 0x0000, 0x0000, 0x0000);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvFlamethrower, -5528, -1250, +600, 0x0000, 0x8000, 0x0000);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvFlamethrower, -5528, -2150, -600, 0x0000, 0x0000, 0x0000);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvFlamethrower, +4300, +3050, -600, 0x0000, 0x0000, 0x0000);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvFlamethrower, +4300, +2950, +600, 0x0000, 0x8000, 0x0000);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvFlamethrower, +4300, +2050, -600, 0x0000, 0x0000, 0x0000);
        spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvFlamethrower, +4300, +1150, +600, 0x0000, 0x8000, 0x0000);
        gOmmSparklyContext->objectsSpawned = true;
        return true;
    }

    // Update secrets hitboxes
    for_each_object_with_behavior(trigger, bhvHiddenStarTrigger) {
        if (trigger->oPosY < -2500.f) {
            trigger->hitboxRadius = 200.f;
            trigger->hitboxHeight = 300.f;
            trigger->hitboxDownOffset = 150.f;
        } else {
            trigger->hitboxRadius = 100.f;
            trigger->hitboxHeight = 150.f;
            trigger->hitboxDownOffset = 75.f;
        }
    }

    // Anti-cheese
    if (m->action != ACT_SHOCKED) {
        f32 marioLastGroundedY = ((f32) gOmmSparklyContext->eventFlags) + FLOOR_LOWER_LIMIT;
        struct Object *nearestSecret = obj_get_nearest_with_behavior_and_radius(m->marioObj, bhvHiddenStarTrigger, 500.f);
        if (!nearestSecret) nearestSecret = obj_get_nearest_with_behavior_and_radius(omm_cappy_get_object(), bhvHiddenStarTrigger, 500.f);
        if (nearestSecret && nearestSecret->oPosY > -2500.f && nearestSecret->oPosY < marioLastGroundedY) {
            omm_cappy_unload();
            omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
            update_mario_sound_and_camera(m);
            drop_and_set_mario_action(m, ACT_SHOCKED, (m->action & (ACT_FLAG_AIR | ACT_FLAG_ON_POLE | ACT_FLAG_HANGING)) == 0);
            obj_play_sound(m->marioObj, SOUND_MARIO_ATTACKED);
            omm_obj_spawn_amp_discharge(nearestSecret, 8.f);
        }
    }

    // A secret was collected
    if (OMM_SPARKLY_STATE_IS_OK && gOmmSparklyContext->secretsDiff < 0) {
        s32 index = 6 - gOmmSparklyContext->secrets;
        omm_obj_spawn_orange_number(m->marioObj,
            index, 80,
            SOUND_MENU_COLLECT_SECRET, index - 1,
            omm_geo_number
        );
    }
    gOmmSparklyContext->successful |= (gOmmSparklyContext->secrets == 0);
    return true;
}

bool omm_sparkly_level_bits__bobomb_and_enemies(struct MarioState *m, const s32 *params) {

    // Verifying conditions...
    // Mario must be possessing a bob-omb or standing inside the safe zone before starting or after the mission is complete
    bool isBobombCaptured = (omm_mario_is_capture(m) && omm_capture_get_type(gOmmCapture) == OMM_CAPTURE_BOBOMB);
    bool isInsideSafeZone = (m->pos[0] < -5400 && m->floorHeight > -5000 && m->floorHeight < -4500);
    if (!isBobombCaptured && (!isInsideSafeZone || (gOmmSparklyContext->timerStarted && !gOmmSparklyContext->starSpawned))) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
    }

    // Spawn a dormant bob-omb inside the safe zone
    if (!gOmmSparklyContext->objectsSpawned && OMM_SPARKLY_STATE_IS_OK) {
        struct Object *bobomb = spawn_object_abs_with_rot(m->marioObj, 0, MODEL_BLACK_BOBOMB, bhvBobomb, -6969, CELL_HEIGHT_LIMIT, 0, 0, 0, 0);
        bobomb->oFlags |= OBJ_FLAG_SPARKLY_NOT_ENEMY;
        bobomb->parentObj = bobomb;
        bobomb->prevObj = NULL;
        obj_drop_to_floor(bobomb);
        obj_set_dormant(bobomb, true);
        gOmmSparklyContext->objectsSpawned = true;
    }

    // Possess the goomba as soon as it awakes
    if (OMM_SPARKLY_STATE_IS_OK && !gOmmSparklyContext->eventStarted && m->action != ACT_READING_SIGN) {
        struct Object *bobomb = obj_get_nearest_with_behavior(m->marioObj, bhvBobomb);
        if (bobomb && (bobomb->oFlags & OBJ_FLAG_SPARKLY_NOT_ENEMY) && !obj_is_dormant(bobomb)) {
            if (omm_mario_possess_object(m, bobomb, OMM_MARIO_POSSESS_FORCE_CAPTURE)) {
                gOmmSparklyContext->eventStarted = true;
            }
        }
    }

    // Check remaining enemies
    s32 remainingEnemies = omm_sparkly_context_get_remaining_enemies();
    gOmmSparklyContext->successful |= (remainingEnemies == 0);

    // Start the timer as soon as Mario captures the bob-omb
    return omm_mario_is_capture(m) || gOmmSparklyContext->timerStarted;
}

bool omm_sparkly_level_bits__dark_all_star_rings(struct MarioState *m, const s32 *params) {
    // if (omm_world_is_dark()) {
    //     omm_sparkly_level__all_star_rings();
    // } else {
    //     omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
    // }
    return true;
}

bool omm_sparkly_level_vcutm__five_corners(struct MarioState *m, const s32 *params) {
    const Vec2f sVcutmZones[] = {
        { -6500, -6500 },
        { -6500, +6500 },
        { +4865, +6500 },
        { +4865, -6500 },
        { -2400, -6500 }
    };
    bool newZone = false;
    s32 numZones = 0;

    // Zones
    for (s32 i = 0; i != 5; ++i) {
        u32 flag = (1 << i);
        if (gOmmSparklyContext->eventFlags & flag) {
            numZones++;
        } else {
            f32 x = sVcutmZones[i][0];
            f32 z = sVcutmZones[i][1];
            f32 d = sqrtf(sqr_f(m->pos[0] - x) + sqr_f(m->pos[2] - z));
            if (d < 300) {
                gOmmSparklyContext->eventFlags |= flag;
                newZone = true;
                numZones++;
            }
        }
    }
    gOmmSparklyContext->successful |= (numZones == 5);

    // Spawn secret effect if Mario enters a new zone
    if (newZone && OMM_SPARKLY_STATE_IS_OK) {
        omm_obj_spawn_orange_number(m->marioObj,
            numZones, 80,
            SOUND_MENU_COLLECT_SECRET, numZones - 1,
            omm_geo_number
        );
    }
    return true;
}

bool omm_sparkly_level_basement__catch_mips(struct MarioState *m, const s32 *params) {

    // Unspawn the regular Mips and spawn the Crystal sparkling Mips
    if (!gOmmSparklyContext->objectsSpawned) {
        obj_deactivate_all_with_behavior(bhvMips);
        omm_obj_spawn_mips(m->marioObj, -1509, -1177, -1564, 60.f);
        gOmmSparklyContext->objectsSpawned = true;
        return true;
    }

    // Mips must be caught 3 times
    struct Object *mips = obj_get_first_with_behavior(bhvOmmMips);
    if (mips && OMM_SPARKLY_STATE_IS_OK && !gOmmSparklyContext->successful && mips->oMipsGrabbedCounter >= 3) {
        gOmmSparklyContext->successful = true;
        gOmmSparklyContext->spawner = mips;
    }

    // Make Mips leave the basement after the star spawned
    if (mips && mips->oAction == 2 && (!gOmmSparklyContext->star || gOmmSparklyContext->star->oAction != 0)) {
        mips->oAction = 0;
    }
    return true;
}

bool omm_sparkly_level_upstairs__toads_hide_and_seek(struct MarioState *m, const s32 *params) {
    bool makeStarDoorsIntangible = false;
    struct Object *starDoor = obj_get_nearest_with_behavior(m->marioObj, bhvStarDoor);
    if (starDoor) {
        f32 distToStarDoor = obj_get_distance(m->marioObj, starDoor);
        if (distToStarDoor > 1000.f) {
            gOmmSparklyContext->eventStarted = false;
        }
        if (!gOmmSparklyContext->eventStarted) {
            if (m->pos[2] > 4400 && m->wall) {
                gOmmSparklyContext->eventStarted = true;
            } else if (m->forwardVel < -300.f && m->pos[2] + m->vel[2] > 4600) {
                makeStarDoorsIntangible = true;
            }
        }
    }
    for_each_object_with_behavior(obj, bhvStarDoor) {
        if (makeStarDoorsIntangible) {
            obj->collisionData = NULL;
            obj->oSurfaces = NULL;
        } else {
            obj->collisionData = (void *) inside_castle_seg7_collision_star_door;
        }
    }
    return omm_sparkly_level__toads_hide_and_seek(m, params);
}

#pragma GCC pop_options

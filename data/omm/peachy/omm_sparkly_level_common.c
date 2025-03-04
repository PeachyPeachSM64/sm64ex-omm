#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#pragma GCC push_options
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool omm_sparkly_level__all_secrets(struct MarioState *m, const s32 *params) {
    gOmmSparklyContext->successful |= (gOmmSparklyContext->secrets == 0);
    return true;
}

bool omm_sparkly_level__all_mushrooms(struct MarioState *m, const s32 *params) {
    if (OMM_SPARKLY_STATE_IS_OK && gOmmSparklyContext->mushroomsDiff > 0) {
        bool captureRequired = params[1];
        if (!captureRequired || omm_mario_is_capture(m)) {
            s32 mushrooms = params[0];
            s32 collected = gOmmSparklyContext->mushrooms;
            omm_obj_spawn_orange_number(m->marioObj,
                collected, 80,
                SOUND_GENERAL_COLLECT_1UP, 0,
                omm_geo_number
            );
            gOmmSparklyContext->successful |= (collected == mushrooms);
        } else {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        }
    }
    return true;
}

bool omm_sparkly_level__all_boxes(struct MarioState *m, const s32 *params) {
    s32 boxes = params[0];
    s32 remain = omm_sparkly_context_get_remaining_boxes();
    gOmmSparklyContext->successful |= (remain == 0);
    return (remain > 0 && remain < boxes);
}

bool omm_sparkly_level__all_flames(struct MarioState *m, const s32 *params) {

    // Unload regular flames and spawn big flames
    if (!gOmmSparklyContext->objectsSpawned) {
        obj_unload_all_with_behavior(bhvFlame);
        s32 numFlamesToSpawn = params[0];
        for (s32 i = 0; i != numFlamesToSpawn; ++i) {
            f32 x = params[3 * i + 1];
            f32 y = params[3 * i + 2];
            f32 z = params[3 * i + 3];
            omm_obj_spawn_big_flame(m->marioObj, x, y, z);
        }
        gOmmSparklyContext->objectsSpawned = true;
        return true;
    }

    // Touch a flame to extinguish it
    if (OMM_SPARKLY_STATE_IS_OK) {
        gOmmSparklyContext->successful |= (omm_sparkly_context_get_remaining_flames() == 0);
    }
    return true;
}

bool omm_sparkly_level__eight_shards(struct MarioState *m, const s32 *params) {
    if (OMM_SPARKLY_STATE_IS_OK) {

        // Spawn shards
        if (!gOmmSparklyContext->objectsSpawned) {
            for (s32 i = 0; i != 8; ++i) {
                f32 x = params[3 * i + 0];
                f32 y = params[3 * i + 1];
                f32 z = params[3 * i + 2];
                spawn_object_abs_with_rot(m->marioObj, 0, MODEL_SPARKLES, bhvRedCoin, x, y, z, 0, 0, 0)->parentObj = NULL;
            }
            gOmmSparklyContext->objectsSpawned = true;
            return true;
        }

        // A shard was collected
        s32 starShards = gOmmSparklyContext->coinsRed;
        if (starShards > 0 && gOmmSparklyContext->coinsRedDiff > 0) {
            omm_obj_spawn_orange_number(m->marioObj,
                starShards, 80,
                SOUND_MENU_COLLECT_RED_COIN, starShards - 1,
                omm_geo_number
            );
            gOmmSparklyContext->successful |= (starShards == 8);
        }
    }
    return true;
}

bool omm_sparkly_level__eight_shards_and_lava(struct MarioState *m, const s32 *params) {
    s32 starShards = gOmmSparklyContext->coinsRed;
    struct Object *lava = obj_get_first_with_behavior(bhvOmmRisingLava);

    // Spawn lava object and star shards
    if (!lava) {
        f32 yMin = params[0];
        f32 yMax = params[1];
        f32 yVel = params[2];
        f32 radius = params[3];
        lava = omm_obj_spawn_rising_lava(m->marioObj, 0, 0, 0, yMin, yMax, yVel, radius, 16, 10);
        obj_set_dormant(lava, true);
        for (s32 i = 0; i != 8; ++i) {
            f32 x = params[3 * i + 4];
            f32 y = params[3 * i + 5];
            f32 z = params[3 * i + 6];
            struct Object *starShard = spawn_object_abs_with_rot(m->marioObj, 0, MODEL_SPARKLES, bhvRedCoin, x, y, z, 0, 0, 0);
            starShard->parentObj = NULL;
            obj_set_dormant(starShard, true);
        }
        return true;
    }

    // Update rising lava
    if (!obj_is_dormant(lava)) {
        if (OMM_SPARKLY_STATE_IS_OK && starShards > 0 && gOmmSparklyContext->coinsRedDiff > 0) {
            lava->oAction = 1;
            omm_obj_spawn_orange_number(m->marioObj,
                starShards, 80,
                SOUND_MENU_COLLECT_RED_COIN, starShards - 1,
                omm_geo_number
            );
        } else if (m->prevAction == ACT_OMM_SPARKLY_STAR_DANCE) {
            lava->oAction = 2;
        }
    }
    return true;
}

bool omm_sparkly_level__secrets_in_order(struct MarioState *m, const s32 *params) {
    s32 numSecrets = params[0];

    // Spawn secrets in order
    if (!gOmmSparklyContext->objectsSpawned) {
        for (s32 i = 0; i != numSecrets; ++i) {
            f32 x = params[3 * i + 1];
            f32 y = params[3 * i + 2];
            f32 z = params[3 * i + 3];
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, x, y, z, 0, 0, 0)->oBehParams = i + 1;
        }
        gOmmSparklyContext->objectsSpawned = true;
        return true;
    }

    // A secret was collected
    if (OMM_SPARKLY_STATE_IS_OK && gOmmSparklyContext->secretsDiff < 0) {
        s32 expectedIndex = numSecrets - gOmmSparklyContext->secrets;
        for_each_object_with_behavior(trigger, bhvHiddenStarTrigger) {
            if (trigger->oBehParams == expectedIndex) {
                expectedIndex = -1;
                omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
                break;
            }
        }
        if (expectedIndex != -1) {
            omm_obj_spawn_orange_number(m->marioObj,
                expectedIndex, 80,
                SOUND_MENU_COLLECT_SECRET, expectedIndex - 1,
                omm_geo_number
            );
        }
    }
    gOmmSparklyContext->successful |= (gOmmSparklyContext->secrets == 0);
    return true;
}

bool omm_sparkly_level__ten_goombas_stack(struct MarioState *m, const s32 *params) {
    s32 goombaSize = params[0];
    u64 goombaType = omm_cappy_goomba_get_type(array_of(struct Object) { { .oBehParams2ndByte = goombaSize } });
    if (OMM_SPARKLY_STATE_IS_OK && omm_mario_is_capture(m)) {
        if (omm_capture_get_type(gOmmCapture) == goombaType) {
            if (!gOmmSparklyContext->successful && gOmmObject->goomba.stackCount >= 9) {
                gOmmSparklyContext->successful = true;
                gOmmSparklyContext->spawner = gOmmObject->goomba.stackObj[gOmmObject->goomba.stackCount - 1];
            }
        } else {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        }
    }
    return true;
}

bool omm_sparkly_level__launch_cappy_at_target(struct MarioState *m, const s32 *params) {
    if (!gOmmSparklyContext->successful && !gOmmSparkly->gamePaused && !gOmmSparkly->transition) {
        struct Object *cappy = omm_cappy_get_object();
        Vec3f targetPos = { params[0], params[1], params[2] };
        if (OMM_SPARKLY_STATE_IS_OK && cappy && vec3f_dist(&cappy->oPosX, targetPos) < 20.f) {
            switch (gOmmSparklyContext->eventFlags++) {
                case  0: case 14: case  27: case  39: case  50:
                case 60: case 69: case  77: case  84: case  90:
                case 95: case 99: case 102: case 104: case 105: {
                    play_sound(SOUND_GENERAL_SHORT_STAR, cappy->oCameraToObject);
                    omm_obj_spawn_sparkly_star_sparkle(cappy, gOmmSparklyMode, 0, 10.f, 0.4f, 50.f);
                    omm_obj_spawn_sparkly_star_sparkle(cappy, gOmmSparklyMode, 0, 10.f, 0.4f, 50.f);
                    omm_obj_spawn_sparkly_star_sparkle(cappy, gOmmSparklyMode, 0, 10.f, 0.4f, 50.f);
                    omm_obj_spawn_sparkly_star_sparkle(cappy, gOmmSparklyMode, 0, 10.f, 0.4f, 50.f);
                    omm_obj_spawn_sparkly_star_sparkle(cappy, gOmmSparklyMode, 0, 10.f, 0.4f, 50.f);
                } break;

                case 106: {
                    play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
                    gOmmSparklyContext->successful = true;
                    gOmmSparklyContext->spawner = spawn_object(m->marioObj, MODEL_NONE, bhvStaticObject);
                    obj_set_pos(gOmmSparklyContext->spawner, params[0], params[3] - 150, params[2]);
                    obj_reset_hitbox(gOmmSparklyContext->spawner, 0, 0, 0, 0, 0, 0);
                } break;
            }
        } else {
            gOmmSparklyContext->eventFlags = 0;
        }
    }
    return true;
}

bool omm_sparkly_level__dont_touch_floor(struct MarioState *m, const s32 *params) {
    bool onGround = !(m->action & ACT_FLAG_AIR);
    bool autoStart = params[0];

    // Auto-start: Mario starts jumping as soon as the dialog box closes
    if (autoStart) {

        // When warp-landing, trigger the dialog box...
        if (!gOmmSparklyContext->entryDialog) {
            return false;
        }

        // ...then, enter the jumping action
        if (m->prevAction == ACT_READING_AUTOMATIC_DIALOG) {
            gOmmSparklyContext->eventStarted = true;
            omm_mario_set_action(m, ACT_LONG_JUMP, 0, 0xFFFF);
            mario_set_forward_vel(m, 48.f);
            return true;
        }
    }

    // After leaving the ground, don't touch it again
    if (OMM_SPARKLY_STATE_IS_OK) {
        if (!gOmmSparklyContext->eventStarted) {
            gOmmSparklyContext->eventStarted = (omm_mario_is_capture(m) || (((m->action & ACT_GROUP_MASK) != ACT_GROUP_CUTSCENE) && !onGround));
        } else if (onGround && m->action != ACT_READING_AUTOMATIC_DIALOG) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        }
        return gOmmSparklyContext->eventStarted || (autoStart && gOmmSparklyContext->entryDialog);
    }
    return true;
}

bool omm_sparkly_level__toads_on_pillars(struct MarioState *m, const s32 *params) {
    if (!omm_mario_is_capture(m) && OMM_SPARKLY_STATE_IS_OK && !gOmmSparklyContext->successful) {
        struct Object *pillar1 = obj_get_first_with_behavior(bhvWaterLevelPillar);
        struct Object *pillar2 = obj_get_next_with_behavior(pillar1, bhvWaterLevelPillar);
        if (pillar1 && pillar2) {
            struct Object *toad1 = obj_get_nearest_with_behavior(pillar1, bhvToadMessage);
            struct Object *toad2 = obj_get_nearest_with_behavior(pillar2, bhvToadMessage);
            if (toad1 && toad2 && (toad1 != toad2)) {
                f32 heightToad1 = toad1->oPosY - pillar1->oPosY;
                f32 heightToad2 = toad2->oPosY - pillar2->oPosY;
                if (abs_f(heightToad1) < 20.f && abs_f(heightToad2) < 20.f) {
                    f32 distToad1 = obj_get_horizontal_distance(toad1, pillar1);
                    f32 distToad2 = obj_get_horizontal_distance(toad2, pillar2);
                    if (distToad1 < 200 && distToad2 < 200) {
                        gOmmSparklyContext->successful = true;
                        gOmmSparklyContext->spawner = obj_get_nearest_with_behavior(m->marioObj, bhvToadMessage);
                    }
                }
            }
        }
    }
    return true;
}

bool omm_sparkly_level__toads_hide_and_seek(struct MarioState *m, const s32 *params) {

    // Spawn Toads
    if (!gOmmSparklyContext->objectsSpawned) {
        obj_deactivate_all_with_behavior(bhvToadMessage);
        s32 numToadsToSpawn = params[0];
        for (s32 i = 0; i != numToadsToSpawn; ++i) {
            s32 dialogId = params[5 * i + 1];
            f32 x = params[5 * i + 2];
            f32 y = params[5 * i + 3];
            f32 z = params[5 * i + 4];
            s16 yaw = params[5 * i + 5];
            struct Object *toad = spawn_object(m->marioObj, MODEL_TOAD, bhvToadMessage);
            toad->oBehParams2ndByte = dialogId;
            obj_set_xyz(toad, x, y, z);
            obj_set_home(toad, x, y, z);
            obj_set_angle(toad, 0, yaw, 0);
        }
        gOmmSparklyContext->objectsSpawned = true;
        return true;
    }

    // Update Toads
    for_each_object_with_behavior(toad, bhvToadMessage) {
        toad->oToadMessageDialogId = toad->oBehParams2ndByte;
        if (toad->oRoom != -1) {
            obj_reset_hitbox(toad, 1, 100, 0, 0, 50, 0);
        } else {
            obj_reset_hitbox(toad, 100, 100, 0, 0, 50, 0);
        }
    }
    return true;
}

bool omm_sparkly_level__invalidate_if_not_flooded(struct MarioState *m, const s32 *params) {
    if (omm_world_is_flooded()) {
        obj_unload_all_with_behavior(bhvBobombBuddy);
        obj_unload_all_with_behavior(bhvBobombBuddyOpensCannon);
        obj_unload_all_with_behavior(bhvKoopaShellUnderwater);
    } else {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
    }
    return true;
}

bool omm_sparkly_level__invalidate_if_not_frozen(struct MarioState *m, const s32 *params) {
    if (!omm_world_is_frozen()) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
    }
    return true;
}

bool omm_sparkly_level__inc_timer_with_coins(struct MarioState *m, const s32 *params) {
    if (OMM_SPARKLY_STATE_IS_OK) {
        s32 inc = params[0];
        s32 dy = max_s(0, gOmmSparklyContext->coinsYellowDiff);
        s32 dr = max_s(0, gOmmSparklyContext->coinsRedDiff);
        s32 db = max_s(0, gOmmSparklyContext->coinsBlueDiff);
        s32 dc = dy * 1 + dr * 2 + db * 5;
        gHudDisplay.timer += dc * inc;
    }
    return true;
}

bool omm_sparkly_level__shell_ride(struct MarioState *m, const s32 *params) {

    // Don't collect anything before riding the shell
    if (!gOmmSparklyContext->eventStarted && (
        gOmmSparklyContext->coinsYellow > 0 ||
        gOmmSparklyContext->coinsLost > 0 ||
        gOmmSparklyContext->coinsRed > 0 ||
        gOmmSparklyContext->coinsBlue > 0 ||
        gOmmSparklyContext->secretsDiff < 0 ||
        gOmmSparklyContext->mushroomsDiff > 0 ||
        omm_stars_get_flags() != 0)) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        return true;
    }

    // Keep riding the shell
    if (gOmmSparklyContext->eventStarted && !(m->action & ACT_FLAG_RIDING_SHELL)) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        return true;
    }

    // Start as soon as Mario rides a shell
    gOmmSparklyContext->eventStarted |= (m->action & ACT_FLAG_RIDING_SHELL) != 0;
    return true;
}

bool omm_sparkly_level__koopa_the_quick_race(struct MarioState *m, const s32 *params) {
    // if (!OMM_SPARKLY_STATE_IS_OK) {
    //     return true;
    // }

    // // No NS if Koopa the Quick is not here
    // struct Object *ktq = obj_get_first_with_behavior_and_field_s32(bhvKoopa, _FIELD(oBehParams2ndByte), KOOPA_BP_KOOPA_THE_QUICK_BOB);
    // if (!ktq) ktq = obj_get_first_with_behavior_and_field_s32(bhvKoopa, _FIELD(oBehParams2ndByte), KOOPA_BP_KOOPA_THE_QUICK_THI);
    // if (!ktq) {
    //     omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
    //     return true;
    // }

    // // Can't collect anything before the race has started
    // if (ktq->oAction < KOOPA_THE_QUICK_ACT_RACE && (
    //     gOmmSparklyContext->coinsYellow > 0 ||
    //     gOmmSparklyContext->coinsLost > 0 ||
    //     gOmmSparklyContext->coinsRed > 0 ||
    //     gOmmSparklyContext->coinsBlue > 0 ||
    //     gOmmSparklyContext->secretsDiff < 0 ||
    //     gOmmSparklyContext->mushroomsDiff > 0 ||
    //     omm_stars_get_flags() != 0)) {
    //     omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
    //     omm_debug_log("[fail] Collected object before race start\n");
    //     return true;
    // }

    // // Can't trigger Bob-omb King fight before the race has started
    // struct Object *kbo = obj_get_first_with_behavior(bhvKingBobomb);
    // if (ktq->oAction < KOOPA_THE_QUICK_ACT_RACE && kbo && kbo->oAction != 0) {
    //     omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
    //     omm_debug_log("[fail] Triggered king bob-omb before race start\n");
    //     return true;
    // }

    // // Check distance with Koopa the Quick when starting the race
    // if (ktq->oAction == KOOPA_THE_QUICK_ACT_SHOW_INIT_TEXT && obj_get_distance(ktq, gMarioObject) > 500.f) {
    //     omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
    //     omm_debug_log("[fail] Too far from ktq at race start\n");
    //     return true;
    // }

    // // Collect all stars...
    // bool allStars = true;
    // s32 numStarsToCollect = params[1];
    // for (s32 i = 0; i != numStarsToCollect; ++i) {
    //     s32 starIndex = params[i + 1];
    //     if (!omm_stars_is_collected(starIndex)) {
    //         allStars = false;
    //         break;
    //     }
    // }

    // // ...but before touching the flag
    // // This condition also invalidates the NS if Koopa the Quick
    // // reaches the flag before all stars have been collected
    // struct Object *flag = obj_get_first_with_behavior(bhvKoopaRaceEndpoint);
    // if (!flag || (!allStars && flag->oKoopaRaceEndpointRaceEnded)) {
    //     omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
    //     omm_debug_log("[fail] Touched flag before collecting all stars\n");
    //     return true;
    // }

    // // Spawn the NS after collecting Koopa the Quick's star
    // // No need to check if Mario lost or cheated, since
    // // Koopa the Quick won't spawn his star in these cases
    // s32 ktqStarIndex = params[0];
    // if (omm_stars_is_collected(ktqStarIndex) && !omm_mario_is_star_dancing(m)) {
    //     gOmmSparklyContext->successful = true;
    // }
    return true;
}

bool omm_sparkly_level__all_star_rings(struct MarioState *m, const s32 *params) {
    // if (!OMM_SPARKLY_STATE_IS_OK) {
    //     return true;
    // }

    // // Spawn rings
    // if (!gOmmSparklyContext->objectsSpawned) {
    //     s32 numStarRingsToSpawn = params[0];
    //     for (s32 i = 0; i != numStarRingsToSpawn; ++i) {
    //         f32 x = params[5 * i + 1];
    //         f32 y = params[5 * i + 2];
    //         f32 z = params[5 * i + 3];
    //         bool v = params[5 * i + 4];
    //         s32 yaw = params[5 * i + 5];
    //         omm_obj_spawn_star_ring(m->marioObj, x, y, z, v, yaw);
    //     }
    //     gOmmSparklyContext->objectsSpawned = true;
    //     return true;
    // }

    // // Spawn the star and stop the timer when all rings are collected
    // if (obj_get_count_with_behavior_and_field_s32(bhvOmmStarRing, _FIELD(oAction), 1) == 0) {
    //     gOmmSparklyContext->successful = true;
    //     return false;
    // }
    return true;
}

#pragma GCC pop_options

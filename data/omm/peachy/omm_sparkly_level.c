#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define NO_PARAM { 0 }

typedef struct {
    void (*func)(void);
    const s32 *params;
} OmmSparklyContextLevel;

static struct MarioState *sMario;
static const OmmSparklyData *sData;
static const OmmSparklyContextLevel *sLevel;
static bool sUpdateTimer;

//
// Timer
//

static void omm_sparkly_context_update_timer(struct MarioState *m, const OmmSparklyData *data, bool updateTimer) {

    // PSS / Koopa the Quick timer
    if (data->timer < 0) {
        if ((gHudDisplay.flags & HUD_DISPLAY_FLAG_TIMER) && gHudDisplay.timer >= -data->timer) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        }
        return;
    }

    // Countdown
    if (data->timer > 0) {
        if (OMM_SPARKLY_STATE_IS_OK) {

            // Start the timer...
            if (!gOmmSparklyContext->timerStarted) {
                if (updateTimer) {
                    s32 cap = 0;
                    switch (data->flags & OMM_SPARKLY_DATA_CAPS) {
                        case OMM_SPARKLY_DATA_WING_CAP: cap = MARIO_WING_CAP; audio_play_wing_cap_music(); break;
                        case OMM_SPARKLY_DATA_METAL_CAP: cap = MARIO_METAL_CAP; audio_play_metal_cap_music(); break;
                        case OMM_SPARKLY_DATA_VANISH_CAP: cap = MARIO_VANISH_CAP; audio_play_vanish_cap_music(); break;
                    }
                    if (cap) {
                        m->flags &= ~(MARIO_CAP_IN_HAND | MARIO_WING_CAP | MARIO_METAL_CAP | MARIO_VANISH_CAP);
                        m->flags |= (MARIO_CAP_ON_HEAD | cap);
                        m->capTimer = data->timer;
                        m->marioObj->oHomeX = m->pos[0];
                        m->marioObj->oHomeY = m->pos[1];
                        m->marioObj->oHomeZ = m->pos[2];
                    }
                    gHudDisplay.timer = data->timer;
                    gHudDisplay.flags |= HUD_DISPLAY_FLAG_TIMER;
                    gOmmSparklyContext->timerStarted = true;
                }
                return;
            }

            // ...and grab the star before it runs out
            if (gOmmSparkly->marioUpdated && !gOmmSparkly->gamePaused && !gOmmSparkly->transition && !gOmmSparkly->timeStop &&
                (!OMM_SPARKLY_MODE_IS_NORMAL || !gOmmSparklyContext->starSpawned) && !omm_mario_is_reading(m)) {
                if (gHudDisplay.timer > 0) {
                    gHudDisplay.timer -= updateTimer;
                    gHudDisplay.flags |= HUD_DISPLAY_FLAG_TIMER;
                } else {
                    omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
                }
            }
        } else {
            gHudDisplay.flags &= ~HUD_DISPLAY_FLAG_TIMER;
        }
    }
}

//
// Level-independent functions
//

static void all_secrets() {
    gOmmSparklyContext->successful |= (gOmmSparklyContext->secrets == 0);
}

static void all_mushrooms() {
    if (OMM_SPARKLY_STATE_IS_OK && gOmmSparklyContext->mushroomsDiff > 0) {
        bool captureRequired = sLevel->params[1];
        if (!captureRequired || sMario->action == ACT_OMM_POSSESSION) {
            s32 mushrooms = sLevel->params[0];
            s32 collected = gOmmSparklyContext->mushrooms;
            spawn_object_relative(collected, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
            play_sound(SOUND_GENERAL_COLLECT_1UP, gGlobalSoundArgs);
            gOmmSparklyContext->successful |= (collected == mushrooms);
        } else {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        }
    }
}

static void all_boxes() {
    s32 boxes = sLevel->params[0];
    s32 remain = omm_sparkly_context_get_remaining_boxes();
    gOmmSparklyContext->successful |= (remain == 0);
    sUpdateTimer = (remain > 0 && remain < boxes);
}

static void all_flames() {

    // Hide regular flames and spawn big flames
    if (!gOmmSparklyContext->objectsSpawned) {
        for_each_object_with_behavior(obj, bhvFlame) { obj_set_dormant(obj, true); }
        s32 numFlamesToSpawn = sLevel->params[0];
        for (s32 i = 0; i != numFlamesToSpawn; ++i) {
            f32 x = sLevel->params[3 * i + 1];
            f32 y = sLevel->params[3 * i + 2];
            f32 z = sLevel->params[3 * i + 3];
            omm_spawn_big_flame(sMario->marioObj, x, y, z);
        }
        gOmmSparklyContext->objectsSpawned = true;
        return;
    }

    // Touch a flame to extinguish it
    if (OMM_SPARKLY_STATE_IS_OK) {
        gOmmSparklyContext->successful |= (omm_sparkly_context_get_remaining_flames() == 0);
    }
}

static void eight_shards() {
    if (OMM_SPARKLY_STATE_IS_OK) {

        // Spawn shards
        if (!gOmmSparklyContext->objectsSpawned) {
            for (s32 i = 0; i != 8; ++i) {
                f32 x = sLevel->params[3 * i + 0];
                f32 y = sLevel->params[3 * i + 1];
                f32 z = sLevel->params[3 * i + 2];
                spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_SPARKLES, bhvRedCoin, x, y, z, 0, 0, 0)->parentObj = NULL;
            }
            gOmmSparklyContext->objectsSpawned = true;
            return;
        }

        // A shard was collected
        s32 starShards = gOmmSparklyContext->coinsRed;
        if (starShards > 0 && gOmmSparklyContext->coinsRedDiff > 0) {
            spawn_object_relative(starShards, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
            play_sound(SOUND_MENU_COLLECT_RED_COIN + (((u8) (starShards - 1)) << 16), gGlobalSoundArgs);
            gOmmSparklyContext->successful |= (starShards == 8);
        }
    }
}

static void eight_shards_and_lava() {
    s32 starShards = gOmmSparklyContext->coinsRed;
    struct Object *lava = obj_get_first_with_behavior(bhvOmmRisingLava);

    // Spawn lava object and star shards
    if (!lava) {
        f32 yMin = sLevel->params[0];
        f32 yMax = sLevel->params[1];
        f32 yVel = sLevel->params[2];
        f32 radius = sLevel->params[3];
        lava = omm_spawn_rising_lava(sMario->marioObj, 0, 0, 0, yMin, yMax, yVel, radius, 16, 10);
        obj_set_dormant(lava, true);
        for (s32 i = 0; i != 8; ++i) {
            f32 x = sLevel->params[3 * i + 4];
            f32 y = sLevel->params[3 * i + 5];
            f32 z = sLevel->params[3 * i + 6];
            struct Object *starShard = spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_SPARKLES, bhvRedCoin, x, y, z, 0, 0, 0);
            starShard->parentObj = NULL;
            obj_set_dormant(starShard, true);
        }
        return;
    }

    // Update rising lava
    if (!obj_is_dormant(lava)) {
        if (OMM_SPARKLY_STATE_IS_OK && starShards > 0 && gOmmSparklyContext->coinsRedDiff > 0) {
            lava->oAction = 1;
            spawn_object_relative(starShards, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
            play_sound(SOUND_MENU_COLLECT_RED_COIN + (((u8) (starShards - 1)) << 16), gGlobalSoundArgs);
        } else if (sMario->prevAction == ACT_OMM_SPARKLY_STAR_DANCE) {
            lava->oAction = 2;
        }
    }
}

static void secrets_in_order() {
    s32 numSecrets = sLevel->params[0];

    // Spawn secrets in order
    if (!gOmmSparklyContext->objectsSpawned) {
        for (s32 i = 0; i != numSecrets; ++i) {
            f32 x = sLevel->params[3 * i + 1];
            f32 y = sLevel->params[3 * i + 2];
            f32 z = sLevel->params[3 * i + 3];
            spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, x, y, z, 0, 0, 0)->oBhvArgs = i + 1;
        }
        gOmmSparklyContext->objectsSpawned = true;
        return;
    }
    
    // A secret was collected
    if (OMM_SPARKLY_STATE_IS_OK && gOmmSparklyContext->secretsDiff < 0) {
        s32 expectedIndex = numSecrets - gOmmSparklyContext->secrets;
        for_each_object_with_behavior(trigger, bhvHiddenStarTrigger) {
            if (trigger->oBhvArgs == expectedIndex) {
                expectedIndex = -1;
                omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
                break;
            }
        }
        if (expectedIndex != -1) {
            spawn_object_relative(expectedIndex, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
            play_sound(SOUND_MENU_COLLECT_SECRET + (((u8) expectedIndex - 1) << 16), gGlobalSoundArgs);
        }
    }
    gOmmSparklyContext->successful |= (gOmmSparklyContext->secrets == 0);
}

static void ten_goombas_stack() {
    if (OMM_SPARKLY_STATE_IS_OK && sMario->action == ACT_OMM_POSSESSION && gOmmCapture->behavior == bhvGoomba && gOmmObject->goomba.stackCount >= 9) {
        gOmmSparklyContext->successful = true;
    }
}

static void snowmen_at_top() {
    if (OMM_SPARKLY_STATE_IS_OK) {
        f32 topX = sLevel->params[1];
        f32 topY = sLevel->params[2];
        f32 topZ = sLevel->params[3];
        s32 maxSnowmenAtTop = sLevel->params[0];
        s32 numSnowmenAtTop = 0;
        for_each_object_with_behavior(mrBlizzard, bhvMrBlizzard) {
            f32 dx = mrBlizzard->oPosX - topX;
            f32 dy = mrBlizzard->oPosY - topY;
            f32 dz = mrBlizzard->oPosZ - topZ;
            f32 dr = sqrtf(sqr_f(dx) + sqr_f(dz));
            numSnowmenAtTop += (dy > 0 && dr < 512);
        }
        gOmmSparklyContext->successful = (numSnowmenAtTop >= maxSnowmenAtTop);
    }
}

static void launch_cappy_to_target() {
    if (!gOmmSparklyContext->successful && !gOmmSparkly->gamePaused && !gOmmSparkly->transition) {
        static s32 sCappyTimer = 0;
        struct Object *cappy = omm_cappy_get_object();
        Vec3f targetPos = { sLevel->params[0], sLevel->params[1], sLevel->params[2] };
        if (OMM_SPARKLY_STATE_IS_OK && cappy && vec3f_dist(&cappy->oPosX, targetPos) < 20.f) {
            switch (sCappyTimer++) {
                case  0: case 14: case  27: case  39: case  50:
                case 60: case 69: case  77: case  84: case  90:
                case 95: case 99: case 102: case 104: case 105: {
                    play_sound(SOUND_GENERAL_SHORT_STAR, gGlobalSoundArgs);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SPARKLY_MODE_HARD, 0, 10.f, 0.4f, 50.f);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SPARKLY_MODE_HARD, 0, 10.f, 0.4f, 50.f);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SPARKLY_MODE_HARD, 0, 10.f, 0.4f, 50.f);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SPARKLY_MODE_HARD, 0, 10.f, 0.4f, 50.f);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SPARKLY_MODE_HARD, 0, 10.f, 0.4f, 50.f);
                } break;

                case 106: {
                    play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
                    gOmmSparklyContext->successful = true;
                } break;
            }
        } else {
            sCappyTimer = 0;
        }
    }
}

static void dont_touch_floor() {
    bool onGround = !(sMario->action & ACT_FLAG_AIR);
    bool autoStart = sLevel->params[0];

    // Auto-start: Mario starts jumping as soon as the dialog box closes
    if (autoStart) {

        // When warp-landing, trigger the dialog box...
        if (!gOmmSparklyContext->entryDialog) {
            sUpdateTimer = false;
            return;
        }
        
        // ...then, enter the jumping action
        if (sMario->prevAction == ACT_READING_AUTOMATIC_DIALOG) {
            gOmmSparklyContext->eventStarted = true;
            omm_mario_set_action(sMario, ACT_LONG_JUMP, 0, 0xFFFF);
            mario_set_forward_vel(sMario, 48.f);
            return;
        }
    }
    
    // After leaving the ground, don't touch it again
    if (OMM_SPARKLY_STATE_IS_OK) {
        if (!gOmmSparklyContext->eventStarted) {
            gOmmSparklyContext->eventStarted = ((sMario->action == ACT_OMM_POSSESSION) || (((sMario->action & ACT_GROUP_MASK) != ACT_GROUP_CUTSCENE) && !onGround));
        } else if (onGround && sMario->action != ACT_READING_AUTOMATIC_DIALOG) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        }
        sUpdateTimer = gOmmSparklyContext->eventStarted || (autoStart && gOmmSparklyContext->entryDialog);
    }
}

static void toads_on_pillars() {
    if (sMario->action != ACT_OMM_POSSESSION && OMM_SPARKLY_STATE_IS_OK) {
        struct Object *toad1 = obj_get_first_with_behavior(bhvToadMessage);
        struct Object *toad2 = obj_get_next_with_behavior(toad1, bhvToadMessage);
        if (toad1 && toad2) {
            struct Object *toad1Pillar = obj_get_nearest_with_behavior(toad1, bhvWaterLevelPillar);
            struct Object *toad2Pillar = obj_get_nearest_with_behavior(toad2, bhvWaterLevelPillar);
            if (toad1Pillar && toad2Pillar && (toad1Pillar != toad2Pillar)) {
                f32 heightToad1 = toad1->oPosY - toad1Pillar->oPosY;
                f32 heightToad2 = toad2->oPosY - toad2Pillar->oPosY;
                if (abs_f(heightToad1) < 20.f && abs_f(heightToad2) < 20.f) {
                    f32 distToad1 = obj_get_horizontal_distance(toad1, toad1Pillar);
                    f32 distToad2 = obj_get_horizontal_distance(toad2, toad2Pillar);
                    gOmmSparklyContext->successful |= (distToad1 < 200 && distToad2 < 200);
                }
            }
        }
    }
}

static void toads_hide_and_seek() {
    static const BehaviorScript bhvToadMessage2[] = {
        0x00040000,
        0x11014049,
        0x27260000, (uintptr_t) toad_seg6_anims_0600FB58,
        0x28060000,
        0x2F000000, (uintptr_t) INTERACT_TEXT,
        0x10050000,
        0x0C000000, (uintptr_t) bhv_toad_message_init,
        0x08000000,
        0x0C000000, (uintptr_t) bhv_toad_message_loop,
        0x09000000
    };

    // Spawn Toads
    if (!gOmmSparklyContext->objectsSpawned) {
        obj_deactivate_all_with_behavior(bhvToadMessage);
        s32 numToadsToSpawn = sLevel->params[0];
        for (s32 i = 0; i != numToadsToSpawn; ++i) {
            s32 dialogId = sLevel->params[5 * i + 1];
            f32 x = sLevel->params[5 * i + 2];
            f32 y = sLevel->params[5 * i + 3];
            f32 z = sLevel->params[5 * i + 4];
            s16 yaw = sLevel->params[5 * i + 5];
            struct Object *toad = spawn_object(sMario->marioObj, MODEL_TOAD, bhvToadMessage2);
            toad->behavior = bhvToadMessage;
            toad->oBhvArgs = dialogId << 24;
            toad->oBhvArgs2ndByte = 1;
            obj_set_pos(toad, x, y, z);
            obj_set_home(toad, x, y, z);
            obj_set_angle(toad, 0, yaw, 0);
            obj_reset_hitbox(toad, 20, 100, 0, 0, 50, 0);
        }
        gOmmSparklyContext->objectsSpawned = true;
        return;
    }
    
    // Prevent Cappy from capturing Toad if Mario and Cappy aren't in the same room as Toad
    s32 marioRoom = obj_get_room(sMario->marioObj);
    s32 cappyRoom = obj_get_room(omm_cappy_get_object());
    for_each_object_with_behavior(toad, bhvToadMessage) {
        if (marioRoom == cappyRoom && marioRoom == obj_get_room(toad)) {
            toad->oBhvArgs2ndByte = 0;
        } else {
            toad->oBhvArgs2ndByte = 1;
        }
    }

#if OMM_GAME_IS_SM64
    // BLJ assist to TTC clock Toad
    if (OMM_SPARKLY_MODE_IS_HARD && gCurrLevelNum == LEVEL_CASTLE && gCurrAreaIndex == 2) {
        static f32 sBljAssist = 10000.f;
        if (sMario->pos[2] < 7100 && sMario->controller->stickMag > 60.f && (sMario->controller->buttonDown & Z_TRIG) && (
            sMario->action == ACT_LONG_JUMP || sMario->action == ACT_LONG_JUMP_LAND || sMario->action == ACT_LONG_JUMP_LAND_STOP)) {
            if (sMario->forwardVel < -300.f) {
                sBljAssist = min_f(sBljAssist, sMario->forwardVel);
            }
        } else {
            sBljAssist = 10000.f;
        }
        mario_set_forward_vel(sMario, min_f(sMario->forwardVel, sBljAssist));
    }
#endif
}

static void invalidate_if_not_flooded() {
    if (!omm_world_is_flooded()) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
    }
}

static void invalidate_if_not_frozen() {
    if (!omm_world_is_frozen()) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
    }
}

static void inc_timer_with_coins() {
    if (OMM_SPARKLY_STATE_IS_OK) {
        s32 inc = sLevel->params[0];
        s32 dy = max_s(0, gOmmSparklyContext->coinsYellowDiff);
        s32 dr = max_s(0, gOmmSparklyContext->coinsRedDiff);
        s32 db = max_s(0, gOmmSparklyContext->coinsBlueDiff);
        s32 dc = dy * 1 + dr * 2 + db * 5;
        gHudDisplay.timer += dc * inc;
    }
}

static void shell_ride() {

    // Don't collect anything before riding the shell
    if (!gOmmSparklyContext->eventStarted && (
        gOmmSparklyContext->coinsYellow > 0 ||
        gOmmSparklyContext->coinsLost > 0 ||
        gOmmSparklyContext->coinsRed > 0 ||
        gOmmSparklyContext->coinsBlue > 0 ||
        gOmmSparklyContext->secretsDiff < 0 ||
        gOmmSparklyContext->mushroomsDiff > 0 ||
        omm_stars_get_bits() != 0)) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        return;
    }

    // Keep riding the shell
    if (gOmmSparklyContext->eventStarted && !(sMario->action & ACT_FLAG_RIDING_SHELL)) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        return;
    }

    // Start as soon as Mario rides a shell
    gOmmSparklyContext->eventStarted |= (sMario->action & ACT_FLAG_RIDING_SHELL) != 0;
}

static void koopa_the_quick_race() {
    if (!OMM_SPARKLY_STATE_IS_OK) {
        return;
    }
    
    // No NS if Koopa the Quick is not here
    struct Object *ktq = obj_get_first_with_behavior_and_field_s32(bhvKoopa, 0x2F, KOOPA_BP_KOOPA_THE_QUICK_BOB);
    if (!ktq) ktq = obj_get_first_with_behavior_and_field_s32(bhvKoopa, 0x2F, KOOPA_BP_KOOPA_THE_QUICK_THI);
    if (!ktq) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
        return;
    }
    
    // Can't collect anything before the race has started
    if (ktq->oAction < KOOPA_THE_QUICK_ACT_RACE && (
        gOmmSparklyContext->coinsYellow > 0 ||
        gOmmSparklyContext->coinsLost > 0 ||
        gOmmSparklyContext->coinsRed > 0 ||
        gOmmSparklyContext->coinsBlue > 0 ||
        gOmmSparklyContext->secretsDiff < 0 ||
        gOmmSparklyContext->mushroomsDiff > 0 ||
        omm_stars_get_bits() != 0)) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        omm_debug_log("[fail] Collected object before race start\n");
        return;
    }

    // Can't trigger Bob-omb King fight before the race has started
    struct Object *kbo = obj_get_first_with_behavior(bhvKingBobomb);
    if (ktq->oAction < KOOPA_THE_QUICK_ACT_RACE && kbo && kbo->oAction != 0) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        omm_debug_log("[fail] Triggered king bob-omb before race start\n");
        return;
    }

    // Check distance with Koopa the Quick when starting the race
    if (ktq->oAction == KOOPA_THE_QUICK_ACT_SHOW_INIT_TEXT && obj_get_distance(ktq, gMarioObject) > 500.f) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        omm_debug_log("[fail] Too far from ktq at race start\n");
        return;
    }

    // Collect all stars...
    bool allStars = true;
    s32 numStarsToCollect = sLevel->params[1];
    for (s32 i = 0; i != numStarsToCollect; ++i) {
        s32 star = sLevel->params[i + 1];
        if (!omm_stars_is_collected(star)) {
            allStars = false;
            break;
        }
    }

    // ...but before touching the flag
    // This condition also invalidates the NS if Koopa the Quick
    // reaches the flag before all stars have been collected
    struct Object *flag = obj_get_first_with_behavior(bhvKoopaRaceEndpoint);
    if (!flag || (!allStars && flag->oKoopaRaceEndpointRaceEnded)) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        omm_debug_log("[fail] Touched flag before collecting all stars\n");
        return;
    }

    // Spawn the NS after collecting Koopa the Quick's star
    // No need to check if Mario lost or cheated, since
    // Koopa the Quick won't spawn his star in these cases
    s32 ktqStar = sLevel->params[0];
    if (omm_stars_is_collected(ktqStar) && !omm_mario_is_star_dancing(sMario)) {
        gOmmSparklyContext->successful = true;
    }
}

static void all_star_rings() {
    if (!OMM_SPARKLY_STATE_IS_OK) {
        return;
    }

    // Spawn rings
    if (!gOmmSparklyContext->objectsSpawned) {
        s32 numStarRingsToSpawn = sLevel->params[0];
        for (s32 i = 0; i != numStarRingsToSpawn; ++i) {
            f32 x = sLevel->params[5 * i + 1];
            f32 y = sLevel->params[5 * i + 2];
            f32 z = sLevel->params[5 * i + 3];
            bool v = sLevel->params[5 * i + 4];
            s32 yaw = sLevel->params[5 * i + 5];
            omm_spawn_star_ring(sMario->marioObj, x, y, z, v, yaw);
        }
        gOmmSparklyContext->objectsSpawned = true;
        return;
    }

    // Spawn the star and stop the timer when all rings are collected
    if (obj_get_count_with_behavior_and_field_s32(bhvOmmStarRing, 0x31, 1) == 0) {
        gOmmSparklyContext->successful = true;
        sUpdateTimer = false;
    }
}

//
// Level-specific functions
//

static void jrb__flooded_frozen() {
    if (omm_world_is_flooded() && sMario->area) {
        sMario->area->terrainType = TERRAIN_SNOW;
        if (OMM_SPARKLY_STATE_IS_OK) {

            // Set dormant 'living' objects
            for_each_object_with_behavior(obj, bhvUnagi) { obj_set_dormant(obj, true); }
            for_each_object_with_behavior(obj, bhvUnagiSubobject) { obj_set_dormant(obj, true); }
            for_each_object_with_behavior(obj, bhvBobombBuddy) { obj_set_dormant(obj, true); }
            for_each_object_with_behavior(obj, bhvBobombBuddyOpensCannon) { obj_set_dormant(obj, true); }

            // Force Calm Vibe, and set freeze timer to about to damage Peach
            omm_peach_vibe_activate(sMario, OMM_PEACH_VIBE_TYPE_CALM);
            gOmmMario->state.freeze.dmg = max_s(gOmmMario->state.freeze.dmg, OMM_FREEZING_WATER_TIMER_MAX - 3);
            gOmmMario->state.freeze.gfx = OMM_FREEZING_WATER_TIMER_MAX;

            // If the star is spawned, reduce the Vibe gauge consumption to give just enough time to reach the star
            if (gOmmSparklyContext->starSpawned) {
                if (omm_peach_vibe_is_calm() && !gOmmSparkly->timeStop && gOmmSparkly->marioUpdated) {
                    gOmmPeach->vibeGauge = clamp_s(gOmmPeach->vibeGauge - OMM_PEACH_VIBE_GAUGE_CALM_FREEZE_DEC / 2, 0, OMM_PEACH_VIBE_GAUGE_MAX);
                }
            }
            
            // Collecting coins is the only way to refill the Vibe gauge
            else {
                s32 dy = max_s(0, gOmmSparklyContext->coinsYellowDiff);
                s32 dr = max_s(0, gOmmSparklyContext->coinsRedDiff);
                s32 db = max_s(0, gOmmSparklyContext->coinsBlueDiff);
                s32 dl = max_s(0, gOmmSparklyContext->coinsLostDiff);
                s32 inc = (dy + dl + 2 * dr + 5 * db) * ((OMM_PEACH_VIBE_GAUGE_MAX * 3) / 100);
                gOmmPeach->vibeGauge = clamp_s(gOmmPeach->vibeGauge - inc, 0, OMM_PEACH_VIBE_GAUGE_MAX);
            }
        }
    } else {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
    }
}

static void wdw__bring_chuckya_to_point() {
    if (OMM_SPARKLY_STATE_IS_OK) {

        // Make the Chuckya follow Mario after some point
        for_each_object_with_behavior(obj, bhvChuckya) {
            if (gCurrAreaIndex == 2 || obj->oPosY < 500.f) {
                obj_set_home(obj, obj->oPosX, obj->oPosY, obj->oPosZ);
            }
        }

        // Update the Chuckya so Mario never ends up with an unloaded object in his hands
        s32 chuckyaFlagCurr = (gCurrAreaIndex - 1);
        s32 chuckyaFlagPrev = (2 - gCurrAreaIndex);
        if (gOmmSparklyContext->zones[chuckyaFlagPrev] && omm_mario_is_holding(sMario)) {
            struct Object *chuckya = obj_get_first_with_behavior(bhvChuckya);
            if (!chuckya)  chuckya = spawn_object(sMario->marioObj, MODEL_CHUCKYA, bhvChuckya);
            chuckya->oHeldState = HELD_HELD;
            sMario->heldObj = chuckya;
        }
        if (omm_mario_is_holding(sMario) && sMario->heldObj && sMario->heldObj->behavior == bhvChuckya) {
            gOmmSparklyContext->zones[chuckyaFlagCurr] = true;
        } else {
            gOmmSparklyContext->zones[chuckyaFlagCurr] = false;
        }
        gOmmSparklyContext->zones[chuckyaFlagPrev] = false;

        // As soon as Mario reaches the underwater city with a Chuckya in his hands, lower the water level and rescale the breakable boxes
        if (gCurrAreaIndex == 2 && sMario->pos[2] > -1400) {
            if (!gOmmSparklyContext->eventStarted && omm_mario_is_holding(sMario) && sMario->heldObj && sMario->heldObj->behavior == bhvChuckya) {
                play_sound(SOUND_GENERAL_WATER_LEVEL_TRIG, gGlobalSoundArgs);
                gOmmSparklyContext->eventStarted = true;
            }
            if (gOmmSparklyContext->eventStarted) {
                *gEnvironmentLevels = (s32) approach_f32_symmetric(*gEnvironmentLevels, -2600, 40);
                if (*gEnvironmentLevels > -2600) { play_sound(SOUND_ENV_WATER_DRAIN, gGlobalSoundArgs); }
                for_each_object_with_behavior(obj, bhvWaterLevelDiamond) { obj->oAction = WATER_LEVEL_DIAMOND_ACT_IDLE; }
                gWDWWaterLevelChanging = FALSE;
            }
            for_each_object_with_behavior(obj, bhvBreakableBox) {
                obj_scale(obj, 1.2f);
            }
        }

        // Check if Mario is inside the opening where the red coins star is
        if (gCurrAreaIndex == 2 && !(sMario->action & ACT_FLAG_SWIMMING) &&
            -1024 < sMario->pos[0] && sMario->pos[0] <  -512 &&
            -1800 < sMario->pos[1] && sMario->pos[1] < -1400 &&
            +3200 < sMario->pos[2] && sMario->pos[2] < +3712) {

            // If Mario's holding a Chuckya and the Crystal Star isn't spawned yet, spawn it and replace the Chuckya
            if (gOmmSparklyContext->zones[chuckyaFlagCurr] && !gOmmSparklyContext->successful) {
                obj_set_dormant(sMario->heldObj, true);
                gOmmSparklyContext->successful = true;
                omm_sparkly_context_spawn_star(sMario);
                struct Object *star = obj_get_first_with_behavior(bhvOmmSparklyStar);
                star->oAction = 4;
                sMario->usedObj = spawn_object(sMario->marioObj, MODEL_NONE, bhvOmmDummy);
                sMario->heldObj = NULL;
                mario_grab_used_object(sMario);
            }

            // If Mario released the Star, collect it
            struct Object *star = obj_get_first_with_behavior(bhvOmmSparklyStar);
            if (star && !sMario->heldObj) {
                star->oAction = 2;
            }
        }
        
        // If not, remove the Star from Mario's hands and replace it by the Chuckya
        else if (gOmmSparklyContext->successful) {
            omm_sparkly_context_check_star(NULL);
            gOmmSparklyContext->successful = false;
            if (sMario->heldObj) {
                obj_mark_for_deletion(sMario->heldObj);
                struct Object *chuckya = obj_get_first_with_behavior(bhvChuckya);
                if (!chuckya)  chuckya = spawn_object(sMario->marioObj, MODEL_CHUCKYA, bhvChuckya);
                chuckya->oHeldState = HELD_HELD;
                sMario->heldObj = chuckya;
                obj_set_dormant(sMario->heldObj, false);
            }
        }
    }
}

static void bitdw__goomba_and_coins() {

    // Verifying conditions...
    // Mario must be possessing a goomba or standing inside the safe zone without touching a coin
    if (sMario->action != ACT_OMM_POSSESSION || gOmmCapture->behavior != bhvGoomba) {
        bool isInsideSafeZone = (sMario->pos[0] > -7925 && sMario->pos[0] < -6975 && sMario->pos[2] > +3350 && sMario->pos[2] < +4425);
        if (!isInsideSafeZone || (sMario->numCoins != 0 && !gOmmSparklyContext->starSpawned)) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
        }
    }

    // Spawns a dormant goomba at the center of the safe zone
    static struct Object *sBitdwGoomba = NULL;
    if (!obj_get_first_with_behavior(bhvOmmSparklyStarHint) && OMM_SPARKLY_STATE_IS_OK) {
        sBitdwGoomba = spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_GOOMBA, bhvGoomba, -7450, +20000, 3887, 0, 0, 0);
        sBitdwGoomba->oBhvArgs = GOOMBA_SIZE_REGULAR << 16;
        sBitdwGoomba->oBhvArgs2ndByte = GOOMBA_SIZE_REGULAR;
        sBitdwGoomba->parentObj = sBitdwGoomba;
        sBitdwGoomba->prevObj = NULL;
        sBitdwGoomba->unused1 = 1;
        obj_drop_to_floor(sBitdwGoomba);
        obj_set_dormant(sBitdwGoomba, true);
    }

    // Possess the goomba as soon as it awakes
    if (OMM_SPARKLY_STATE_IS_OK && sMario->action != ACT_READING_SIGN) {
        struct Object *goomba = obj_get_nearest_with_behavior(sMario->marioObj, bhvGoomba);
        if (goomba && goomba == sBitdwGoomba && !obj_is_dormant(goomba)) {
            if (omm_mario_possess_object(sMario, goomba, false, true)) {
                sBitdwGoomba = NULL;
            }
        }
    }
    
    // Hard mode: 7 Goombas stack and 74 coins
    if (OMM_SPARKLY_MODE_IS_HARD && OMM_SPARKLY_STATE_IS_OK) {
        if (sMario->action == ACT_OMM_POSSESSION && gOmmCapture->behavior == bhvGoomba && gOmmObject->goomba.stackCount >= 6) {
            s32 collectedCoins = (
                gOmmSparklyContext->coinsYellow * 1 +
                gOmmSparklyContext->coinsBlue * 5 +
                gOmmSparklyContext->coinsRed * 2
            );
            if (collectedCoins >= 74) {
                gOmmSparklyContext->successful = true;
            }
        }
    }

    // Start the timer as soon as Mario captures the goomba
    sUpdateTimer = (sMario->action == ACT_OMM_POSSESSION || gOmmSparklyContext->timerStarted);
}

static void bitfs__six_secrets() {
    static f32 sMarioLastGroundedY = 0.f;
    if (!(sMario->action & ACT_FLAG_AIR) && (sMario->action != ACT_OMM_POSSESSION || obj_is_on_ground(gOmmCapture))) {
        sMarioLastGroundedY = sMario->pos[1];
    }

    // Spawn pillars, secrets and flamethrowers
    if (!gOmmSparklyContext->objectsSpawned) {
        omm_spawn_bitfs_pillar(sMario->marioObj, -5528, -3100, -572);
        omm_spawn_bitfs_pillar(sMario->marioObj, -5528, -3100, +572);
        omm_spawn_bitfs_pillar(sMario->marioObj, +4300,  +250, -572);
        omm_spawn_bitfs_pillar(sMario->marioObj, +4300,  +250, +572);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, -8000, -2800, -8000, 0, 0, 0);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, +8000, -2800, -8000, 0, 0, 0);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, +8000, -2800, +8000, 0, 0, 0);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, -8000, -2800, +8000, 0, 0, 0);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, +4300,  3000,     0, 0, 0, 0);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvHiddenStarTrigger, -5528,  -300,     0, 0, 0, 0);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvFlamethrower, -5528,  -250, +600, 0x0000, 0x8000, 0x0000);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvFlamethrower, -5528,  -350, -600, 0x0000, 0x0000, 0x0000);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvFlamethrower, -5528, -1250, +600, 0x0000, 0x8000, 0x0000);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvFlamethrower, -5528, -2150, -600, 0x0000, 0x0000, 0x0000);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvFlamethrower, +4300, +3050, -600, 0x0000, 0x0000, 0x0000);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvFlamethrower, +4300, +2950, +600, 0x0000, 0x8000, 0x0000);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvFlamethrower, +4300, +2050, -600, 0x0000, 0x0000, 0x0000);
        spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_NONE, bhvFlamethrower, +4300, +1150, +600, 0x0000, 0x8000, 0x0000);
        gOmmSparklyContext->objectsSpawned = true;
        return;
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
    if (sMario->action != ACT_SHOCKED) {
        struct Object *nearestSecretToMario = obj_get_nearest_with_behavior_and_radius(sMario->marioObj, bhvHiddenStarTrigger, 500.f);
        struct Object *nearestSecretToCappy = obj_get_nearest_with_behavior_and_radius(omm_cappy_get_object(), bhvHiddenStarTrigger, 500.f);
        if ((nearestSecretToMario && nearestSecretToMario->oPosY > -2500.f && nearestSecretToMario->oPosY < sMarioLastGroundedY) ||
            (nearestSecretToCappy && nearestSecretToCappy->oPosY > -2500.f && nearestSecretToCappy->oPosY < sMarioLastGroundedY)) {
            omm_cappy_unload();
            omm_spawn_damage_mario(sMario->marioObj, INTERACT_SHOCK, 1);
        }
    }
    
    // A secret was collected
    if (OMM_SPARKLY_STATE_IS_OK && gOmmSparklyContext->secretsDiff < 0) {
        s32 index = 6 - gOmmSparklyContext->secrets;
        spawn_object_relative(index, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
        play_sound(SOUND_MENU_COLLECT_SECRET + (((u8) index - 1) << 16), gGlobalSoundArgs);
    }
    gOmmSparklyContext->successful |= (gOmmSparklyContext->secrets == 0);
}

static void bits__bobomb_and_enemies() {

    // Verifying conditions...
    // Mario must be possessing a bob-omb or standing inside the safe zone before starting or after the mission is complete
    bool isBobombCaptured = (sMario->action == ACT_OMM_POSSESSION && gOmmCapture->behavior == bhvBobomb);
    bool isInsideSafeZone = (sMario->pos[0] < -5400 && sMario->floorHeight > -5000 && sMario->floorHeight < -4500);
    if (!isBobombCaptured && (!isInsideSafeZone || (gOmmSparklyContext->timerStarted && !gOmmSparklyContext->starSpawned))) {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_FAIL, 1);
    }

    // Spawns a dormant bob-omb inside the safe zone
    static struct Object *sBitsBobomb = NULL;
    if (!obj_get_first_with_behavior(bhvOmmSparklyStarHint) && OMM_SPARKLY_STATE_IS_OK) {
        sBitsBobomb = spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_BLACK_BOBOMB, bhvBobomb, -6969, +20000, 0, 0, 0, 0);
        sBitsBobomb->parentObj = sBitsBobomb;
        sBitsBobomb->prevObj = NULL;
        sBitsBobomb->unused1 = 1;
        obj_drop_to_floor(sBitsBobomb);
        obj_set_dormant(sBitsBobomb, true);
    }

    // Possess the bob-omb as soon as it awakes
    if (OMM_SPARKLY_STATE_IS_OK && sMario->action != ACT_READING_SIGN) {
        struct Object *bobomb = obj_get_nearest_with_behavior(sMario->marioObj, bhvBobomb);
        if (bobomb && bobomb == sBitsBobomb && !obj_is_dormant(bobomb)) {
            if (omm_mario_possess_object(sMario, bobomb, false, true)) {
                sBitsBobomb = NULL;
            }
        }
    }

    // Check remaining enemies
    s32 remainingEnemies = omm_sparkly_context_get_remaining_enemies();
    gOmmSparklyContext->successful |= (remainingEnemies == 0);

    // Start the timer as soon as Mario captures the bob-omb
    sUpdateTimer = (sMario->action == ACT_OMM_POSSESSION || gOmmSparklyContext->timerStarted);
}

static void bits__shadow_all_star_rings() {
    if (omm_world_is_shadow()) {
        all_star_rings();
    } else {
        omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
    }
}

static void vcutm__five_corners() {
    static const Vec2f sVcutmZones[] = {
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
        if (gOmmSparklyContext->zones[i]) {
            numZones++;
        } else {
            f32 x = sVcutmZones[i][0];
            f32 z = sVcutmZones[i][1];
            f32 d = sqrtf(sqr_f(sMario->pos[0] - x) + sqr_f(sMario->pos[2] - z));
            if (d < 300) {
                gOmmSparklyContext->zones[i] = true;
                newZone = true;
                numZones++;
            }
        }
    }
    gOmmSparklyContext->successful |= (numZones == 5);

    // Spawn secret effect if Mario enters a new zone
    if (newZone && OMM_SPARKLY_STATE_IS_OK) {
        spawn_object_relative(numZones, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
        play_sound(SOUND_MENU_COLLECT_SECRET + (((u8) numZones - 1) << 16), gGlobalSoundArgs);
    }
}

static void basement__catch_mips() {

    // Unspawn the regular Mips and spawn the Crystal sparkling Mips
    if (!gOmmSparklyContext->objectsSpawned) {
        obj_deactivate_all_with_behavior(bhvMips);
        omm_spawn_mips(sMario->marioObj, -1509, -1177, -1564, 60.f);
        gOmmSparklyContext->objectsSpawned = true;
        return;
    }
    
    // Mips must be caught 3 times
    struct Object *mips = obj_get_first_with_behavior(bhvOmmMips);
    if (mips && OMM_SPARKLY_STATE_IS_OK) {
        gOmmSparklyContext->successful |= (mips->oMipsGrabbedCounter >= 3);
    }
}

//
// Update per difficulty and level
//

#define level(mode_, index_, func_, ...) \
{ const s32 params[] = __VA_ARGS__; \
OmmSparklyContextLevel *lvl = omm_new(OmmSparklyContextLevel, 1); \
lvl->func = func_; \
lvl->params = omm_dup(params, sizeof(params)); \
omm_hmap_insert(sOmmSparklyContextLevels[mode_], index_ + 1, lvl); }

void omm_sparkly_context_update_level(struct MarioState *m, const OmmSparklyData *data) {
    static OmmHMap *sOmmSparklyContextLevels = NULL;
    if (!sOmmSparklyContextLevels) {
        sOmmSparklyContextLevels = omm_new(OmmHMap, 4);

// ------- Normal ------- //

        // Bob //
        level(OMM_SPARKLY_MODE_NORMAL, 0, all_secrets, NO_PARAM);

        // JRB //
        level(OMM_SPARKLY_MODE_NORMAL, 2, invalidate_if_not_flooded, NO_PARAM);

        // LLL //
        level(OMM_SPARKLY_MODE_NORMAL, 6, eight_shards_and_lava, { 0, 3570, 2, 4096,
            -1700,   60,  -440,
             2550,  630,  1120,
              250, 1210,  2640,
            -2080, 1915,  -300,
            -1600, 2440, -2150,
             1300, 3705, -2500,
              420, 2365,   575,
              662, 3300,   708,
        });

        // SL //
        level(OMM_SPARKLY_MODE_NORMAL, 9, snowmen_at_top, { 1, 0, 4860, 0 });

        // THI //
        level(OMM_SPARKLY_MODE_NORMAL, 12, ten_goombas_stack, NO_PARAM);

        // TTC //
        level(OMM_SPARKLY_MODE_NORMAL, 13, all_boxes, { 13 });

        // RR //
        level(OMM_SPARKLY_MODE_NORMAL, 14, secrets_in_order, { 6,
             620, -3350,  7360,
             615,  -740,  3670,
            2680,  1270,   295,
            3810,  2090,   295,
            3555,  4895, -2330,
            5120,  4900,  3325,
        });

        // BITDW //
        level(OMM_SPARKLY_MODE_NORMAL, 15, bitdw__goomba_and_coins, NO_PARAM);

        // BITFS //
        level(OMM_SPARKLY_MODE_NORMAL, 16, all_mushrooms, { 7, 1 });

        // BITS //
        level(OMM_SPARKLY_MODE_NORMAL, 17, inc_timer_with_coins, { 30 });

        // VCUTM //
        level(OMM_SPARKLY_MODE_NORMAL, 19, vcutm__five_corners, NO_PARAM);

        // COTMC //
        level(OMM_SPARKLY_MODE_NORMAL, 20, dont_touch_floor, { 1 });

        // SA //
        level(OMM_SPARKLY_MODE_NORMAL, 23, invalidate_if_not_frozen, NO_PARAM);

        // Inside //
        level(OMM_SPARKLY_MODE_NORMAL, 25, eight_shards, {
            -1024,  400,  -250,
            -1024,  800,  1400,
            -4500, 1200,  -450,
            -2280, -400, -3040,
             -128, -300, -2640,
             2000,  900, -1700,
             1910,  850, -2120,
            -5500,  600, -4300,
        });
        
        // Basement //
        level(OMM_SPARKLY_MODE_NORMAL, 26, toads_on_pillars, NO_PARAM);
        
        // Upstairs //
        level(OMM_SPARKLY_MODE_NORMAL, 27, eight_shards, {
            -1000, 1210,  1480,
            -4780, 3200, -3240,
             3538, 1680,  3880,
             -610, 1900,  2500,
             -200, 2800,  5000,
            -3400, 3600,  5886,
             3000, 2900,  5886,
             -200, 5400, -3700,
        });
        
        // Courtyard //
        level(OMM_SPARKLY_MODE_NORMAL, 28, secrets_in_order, { 8,
             1500,    0, -1740,
             -820,   20, -3630,
              820,   20, -3630,
             -820,   20,   200,
              820,   20,   200,
            -1500,    0, -1740,
             2040, -200,  -480,
            -2240, -200, -2815,
        });

// ------- Hard ------- //

        // BOB //
        level(OMM_SPARKLY_MODE_HARD, 0, all_secrets, NO_PARAM);

        // JRB //
        level(OMM_SPARKLY_MODE_HARD, 2, invalidate_if_not_flooded, NO_PARAM);

        // HMC //
        level(OMM_SPARKLY_MODE_HARD, 5, all_flames, { 17,
            4936, -357, -4146,
            5018, -460, -5559,
            1997,  666,  -235,
            1762, -460, -2610,
            4178, -255, -3737,
            2233, -460,   256,
            5510, -255, -3429,
            4690, -357,  -767,
            3462, -255, -1125,
            1762,  666,     0,
            1762, -460,   256,
            6482,  461,  3226,
            1075,  461,  6543,
            6912,  461,  6543,
            6912,  461,  3697,
            6482,  461,  7014,
            3817,  717,  1034,
        });

        // LLL //
        level(OMM_SPARKLY_MODE_HARD, 6, eight_shards_and_lava, { 0, 3570, 3, 4096,
            -1700,   60,  -440,
              180, 1050, -1360,
             2550,  630,  1120,
             -480, 1210,  2630,
              705, 2160,   330,
                0, 4690, -2800,
              662, 3900,   708,
             2800, 3000,   250,
        });
        
        // SSL //
        level(OMM_SPARKLY_MODE_HARD, 7, inc_timer_with_coins, { 45 });

        // SL //
        level(OMM_SPARKLY_MODE_HARD, 9, shell_ride, NO_PARAM);

        // WDW //
        level(OMM_SPARKLY_MODE_HARD, 10, wdw__bring_chuckya_to_point, NO_PARAM);

        // THI //
        level(OMM_SPARKLY_MODE_HARD, 12, ten_goombas_stack, NO_PARAM);

        // TTC //
        level(OMM_SPARKLY_MODE_HARD, 13, dont_touch_floor, { 0 });

        // RR //
        level(OMM_SPARKLY_MODE_HARD, 14, all_mushrooms, { 9, 0 });

        // BITDW //
        level(OMM_SPARKLY_MODE_HARD, 15, bitdw__goomba_and_coins, NO_PARAM);

        // BITFS //
        level(OMM_SPARKLY_MODE_HARD, 16, bitfs__six_secrets, NO_PARAM);

        // BITS //
        level(OMM_SPARKLY_MODE_HARD, 17, bits__bobomb_and_enemies, NO_PARAM);

        // VCUTM //
        level(OMM_SPARKLY_MODE_HARD, 19, vcutm__five_corners, NO_PARAM);

        // COTMC //
        level(OMM_SPARKLY_MODE_HARD, 20, dont_touch_floor, { 1 });

        // SA //
        level(OMM_SPARKLY_MODE_HARD, 23, invalidate_if_not_frozen, NO_PARAM);

        // Inside //
        level(OMM_SPARKLY_MODE_HARD, 25, toads_hide_and_seek, { 2,
            DIALOG_134,  -670,    0,  -320, 0xA000,
            DIALOG_135, -2300, -204, -2100, 0x0000,
        });

        // Basement //
        level(OMM_SPARKLY_MODE_HARD, 26, basement__catch_mips, NO_PARAM);

        // Upstairs //
        level(OMM_SPARKLY_MODE_HARD, 27, toads_hide_and_seek, { 2,
            DIALOG_076, -2260, 2253, 4850, 0xA000,
            DIALOG_083,  -205, 2253, 7200, 0x7000,
        });

        // Courtyard //
        level(OMM_SPARKLY_MODE_HARD, 28, launch_cappy_to_target, { 9, 433, -1735 });

// ------- Lunatic ------- //

        // BOB //
        level(OMM_SPARKLY_MODE_LUNATIC, 0, koopa_the_quick_race, { 1, 6, 0, 2, 3, 4, 5, 6 });

        // JRB //
        level(OMM_SPARKLY_MODE_LUNATIC, 2, jrb__flooded_frozen, NO_PARAM);

        // BITS //
        level(OMM_SPARKLY_MODE_LUNATIC, 15, bits__shadow_all_star_rings, { 30,
            -5430, -4725,     0, 1, 0x4000,
            -2960, -4715,     0, 0, 0x0000,
             -300, -4450,     0, 1, 0x8000,
             3900, -1700,     0, 1, 0x4000,
             7500, -1500,     0, 1, 0x4000,
             4100,  -700, -1830, 0, 0x0000,
             2060, -1600, -1560, 1, 0xA000,
               50, -1700,  -720, 0, 0x0000,
            -5000, -2885,  -850, 1, 0x4000,
            -3725, -1350,  -780, 0, 0x0000,
            -6850, -1150,  -800, 0, 0x0000,
            -4000,  -410, -1130, 1, 0x5000,
              240,  -160,  -900, 0, 0x0000,
              900,   -75,  -900, 1, 0xC000,
             1200,   175,  -900, 1, 0xC000,
             1500,   425,  -900, 1, 0xC000,
             1800,   675,  -900, 1, 0xC000,
             2100,   925,  -900, 1, 0xC000,
             2400,  1175,  -900, 1, 0xC000,
             6460,  3735, -1600, 1, 0x8000,
             1300,  2250,  -900, 1, 0xC000,
             -128,  2600,  -900, 0, 0x0000,
            -1760,  3300,  -900, 1, 0xC000,
            -8000,  2350,  -900, 1, 0xC000,
            -6460,  4700,  -900, 0, 0x0000,
            -4100,  3650,  -900, 0, 0x0000,
            -2500,  3650,  -900, 0, 0x0000,
             4350,  5555, -1000, 1, 0x4000,
              350,  7000, -3400, 1, 0x8000,
              350,  7200, -6030, 0, 0x0000,
        });
    }

    // Update
    sUpdateTimer = true;
    s32 i = omm_hmap_find(sOmmSparklyContextLevels[gOmmSparklyMode], data->index + 1);
    if (i != -1) {
        sMario = m;
        sData = data;
        sLevel = omm_hmap_get(sOmmSparklyContextLevels[gOmmSparklyMode], OmmSparklyContextLevel *, i);
        sLevel->func();
    }
    omm_sparkly_context_update_timer(m, data, sUpdateTimer);
}

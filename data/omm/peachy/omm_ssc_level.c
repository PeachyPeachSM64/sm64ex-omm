#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define END_PARAMS -1

#if DEBUG
#define debug_print(...) { printf(__VA_ARGS__); fflush(stdout); }
#else
#define debug_print(...)
#endif

static struct MarioState *sMario;
static const OmmSSD *sData;
static const s32 *sParams;
static bool sUpdateTimer;

//
// Timer
//

static void omm_ssc_update_timer(struct MarioState *m, const OmmSSD *data, bool updateTimer) {

    // PSS / Koopa the Quick timer
    if (data->timer < 0) {
        if ((gHudDisplay.flags & HUD_DISPLAY_FLAG_TIMER) && gHudDisplay.timer >= -data->timer) {
            omm_ssc_state_set(OMM_SSC_FAIL, 1);
        }
        return;
    }

    // Countdown
    if (data->timer > 0) {
        if (OMM_SSC_IS_OK) {

            // Start the timer...
            if (!OMM_SSC_IS_TIMER_STARTED) {
                if (updateTimer) {
                    s32 cap = 0;
                    switch (data->flags & OMM_SSD_CAPS) {
                        case OMM_SSD_WING_CAP: cap = MARIO_WING_CAP; audio_play_wing_cap_music(); break;
                        case OMM_SSD_METAL_CAP: cap = MARIO_METAL_CAP; audio_play_metal_cap_music(); break;
                        case OMM_SSD_VANISH_CAP: cap = MARIO_VANISH_CAP; audio_play_vanish_cap_music(); break;
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
                    omm_ssc_set(OMM_SSC_F_TIMER_STARTED, 1);
                }
                return;
            }

            // ...and grab the star before it runs out
            if (OMM_SSF_IS_MARIO_UPDATED && !OMM_SSF_IS_GAME_PAUSED && !OMM_SSF_IS_TRANSITION && !OMM_SSF_IS_TIME_STOP &&
                (!OMM_SSM_IS_NORMAL || !OMM_SSC_IS_STAR_SPAWNED) && !omm_mario_is_reading(m)) {
                if (gHudDisplay.timer > 0) {
                    gHudDisplay.timer -= updateTimer;
                    gHudDisplay.flags |= HUD_DISPLAY_FLAG_TIMER;
                } else {
                    omm_ssc_state_set(OMM_SSC_FAIL, 1);
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
    omm_ssc_set(OMM_SSC_F_SUCCESSFUL, omm_ssc_cnt(OMM_SSC_C_SECRET) == 0);
}

static void all_mushrooms() {
    if (OMM_SSC_IS_OK && omm_ssc_dif(OMM_SSC_C_MUSHROOM) > 0) {
        if (!sParams[1] || sMario->action == ACT_OMM_POSSESSION) {
            s32 mushrooms = omm_ssc_cnt(OMM_SSC_C_MUSHROOM);
            spawn_object_relative(mushrooms, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
            play_sound(SOUND_GENERAL_COLLECT_1UP, gGlobalSoundArgs);
            omm_ssc_set(OMM_SSC_F_SUCCESSFUL, mushrooms == sParams[0]);
        } else {
            omm_ssc_state_set(OMM_SSC_FAIL, 1);
        }
    }
}

static void all_boxes() {
    s32 boxes = omm_ssc_data_boxes();
    omm_ssc_set(OMM_SSC_F_SUCCESSFUL, boxes == 0);
    sUpdateTimer = (boxes > 0 && boxes < sParams[0]);
}

static void all_flames() {
    static struct { struct Object *obj; Vec3f pos; } sFlames[64];
    static s32 sNumFlames;
    if (!OMM_SSC_IS_OBJECTS_SPAWNED) {

        // Spawn flames
        for (const s32 *flamePos = sParams; *flamePos != END_PARAMS; flamePos += 3) {
            // spawn flames here
            // ...
        }
        
        // Make the flames bigger
        sNumFlames = 0;
        for_each_object_with_behavior(flame, bhvFlame) {
            flame->oScaleX *= 3.f;
            flame->oScaleY *= 3.f;
            flame->oScaleZ *= 3.f;
            flame->hitboxRadius *= 3.f;
            flame->hitboxHeight *= 12.f;
            flame->hurtboxRadius *= 3.f;
            flame->hurtboxHeight *= 12.f;
            flame->hitboxDownOffset *= 6.f;
            sFlames[sNumFlames].obj = flame;
            sFlames[sNumFlames].pos[0] = flame->oPosX;
            sFlames[sNumFlames].pos[1] = flame->oPosY;
            sFlames[sNumFlames].pos[2] = flame->oPosZ;
            sNumFlames++;
        }
        omm_ssc_set(OMM_SSC_F_OBJECTS_SPAWNED, 1);
        return;
    }

    // Extinguish a flame by touching it
    if (OMM_SSC_IS_OK) {

        // Unload the flame that burned Mario
        struct Object *o = sMario->interactObj;
        if (sMario->marioObj->oMarioBurnTimer != 0 && o != NULL && o->behavior == bhvFlame) {
            obj_spawn_white_puff(o, SOUND_GENERAL_FLAME_OUT);
            obj_mark_for_deletion(o);
            sMario->interactObj = NULL;
        }

        // Check if some flame(s) has(ve) been unloaded recently
        for (s32 i = 0; i != sNumFlames; ++i) {
            if (sFlames[i].obj != NULL && (!sFlames[i].obj->activeFlags || sFlames[i].obj->behavior != bhvFlame)) {
                play_sound(SOUND_GENERAL2_RIGHT_ANSWER, gGlobalSoundArgs);
                if (!OMM_SSM_IS_LUNATIC) {
                    static const BehaviorScript bhvFlame2[] = { 0x00040000, 0x04000000, (uintptr_t) bhvFlame };
                    o = spawn_object(sMario->marioObj, MODEL_BLUE_FLAME, bhvFlame2);
                    o->oPosX = sFlames[i].pos[0];
                    o->oPosY = sFlames[i].pos[1];
                    o->oPosZ = sFlames[i].pos[2];
                }
                sFlames[i].obj = NULL;
            }
        }
        omm_ssc_set(OMM_SSC_F_SUCCESSFUL, omm_ssc_data_flames() == 0);
    }
}

static void eight_shards() {
    if (OMM_SSC_IS_OK) {

        // Spawn shards
        if (!OMM_SSC_IS_OBJECTS_SPAWNED) {
            for (s32 i = 0; i != 8; ++i) {
                spawn_object_abs_with_rot(sMario->marioObj, 0,
                    MODEL_SPARKLES, bhvRedCoin,
                    sParams[(3 * i) + 0],
                    sParams[(3 * i) + 1],
                    sParams[(3 * i) + 2],
                    0, 0, 0
                )->parentObj = NULL;
            }
            omm_ssc_set(OMM_SSC_F_OBJECTS_SPAWNED, 1);
            return;
        }

        // A shard was collected
        s32 starShards = omm_ssc_cnt(OMM_SSC_C_COIN_R);
        if (starShards > 0 && omm_ssc_dif(OMM_SSC_C_COIN_R) > 0) {
            spawn_object_relative(starShards, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
            play_sound(SOUND_MENU_COLLECT_RED_COIN + (((u8) (starShards - 1)) << 16), gGlobalSoundArgs);
            omm_ssc_set(OMM_SSC_F_SUCCESSFUL, starShards == 8);
        }
    }
}

static void eight_shards_and_lava() {
    s32 starShards = omm_ssc_cnt(OMM_SSC_C_COIN_R);
    struct Object *lava = obj_get_first_with_behavior(omm_bhv_rising_lava);

    // Spawn lava object and star shards
    if (!lava) {
        lava = omm_spawn_rising_lava(sMario->marioObj, 0, 0, 0, sParams[0], sParams[1], sParams[2], sParams[3], 16, 10);
        obj_set_dormant(lava, true);
        for (s32 i = 0; i != 8; ++i) {
            struct Object *starShard = spawn_object_abs_with_rot(sMario->marioObj, 0,
                MODEL_SPARKLES, bhvRedCoin,
                sParams[4 + (3 * i) + 0],
                sParams[4 + (3 * i) + 1],
                sParams[4 + (3 * i) + 2],
                0, 0, 0
            );
            starShard->parentObj = NULL;
            obj_set_dormant(starShard, true);
        }
        return;
    }

    // Update rising lava
    if (!obj_is_dormant(lava)) {
        if (OMM_SSC_IS_OK && starShards > 0 && omm_ssc_dif(OMM_SSC_C_COIN_R) > 0) {
            lava->oAction = 1;
            spawn_object_relative(starShards, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
            play_sound(SOUND_MENU_COLLECT_RED_COIN + (((u8) (starShards - 1)) << 16), gGlobalSoundArgs);
        } else if (sMario->prevAction == ACT_OMM_SPARKLY_STAR_DANCE) {
            lava->oAction = 2;
        }
    }
}

static void eight_secrets_in_order() {

    // Spawn secrets in order
    if (!OMM_SSC_IS_OBJECTS_SPAWNED) {
        for (s32 i = 0; i != 8; ++i) {
            spawn_object_abs_with_rot(sMario->marioObj, 0,
                MODEL_NONE, bhvHiddenStarTrigger,
                sParams[(3 * i) + 0],
                sParams[(3 * i) + 1],
                sParams[(3 * i) + 2],
                0, 0, 0
            )->oBehParams = i + 1;
        }
        omm_ssc_set(OMM_SSC_F_OBJECTS_SPAWNED, 1);
        return;
    }
    
    // A secret was collected
    if (OMM_SSC_IS_OK && omm_ssc_dif(OMM_SSC_C_SECRET) < 0) {
        s32 expectedIndex = 8 - omm_ssc_cnt(OMM_SSC_C_SECRET);
        for_each_object_with_behavior(trigger, bhvHiddenStarTrigger) {
            if (trigger->oBehParams == expectedIndex) {
                expectedIndex = -1;
                omm_ssc_state_set(OMM_SSC_FAIL, 1);
                break;
            }
        }
        if (expectedIndex != -1) {
            spawn_object_relative(expectedIndex, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
            play_sound(SOUND_MENU_COLLECT_SECRET + (((u8) expectedIndex - 1) << 16), gGlobalSoundArgs);
        }
    }
    omm_ssc_set(OMM_SSC_F_SUCCESSFUL, omm_ssc_cnt(OMM_SSC_C_SECRET) == 0);
}

static void ten_goombas_stack() {
    if (OMM_SSC_IS_OK && sMario->action == ACT_OMM_POSSESSION && gOmmCapture->behavior == bhvGoomba && gOmmData->object->goomba.stackCount >= 9) {
        omm_ssc_set(OMM_SSC_F_SUCCESSFUL, 1);
    }
}

static void snowmen_at_top() {
    if (OMM_SSC_IS_OK) {
        s32 numSnowmenAtTop = 0;
        for_each_object_with_behavior(mrBlizzard, bhvMrBlizzard) {
            f32 dx = mrBlizzard->oPosX - sParams[1];
            f32 dy = mrBlizzard->oPosY - sParams[2];
            f32 dz = mrBlizzard->oPosZ - sParams[3];
            f32 dr = sqrtf(sqr_f(dx) + sqr_f(dz));
            numSnowmenAtTop += (dy > 0.f && dr < 512);
        }
        if (numSnowmenAtTop >= sParams[0]) {
            omm_ssc_set(OMM_SSC_F_SUCCESSFUL, 1);
        } else {
            omm_ssc_clr(OMM_SSC_F_SUCCESSFUL);
        }
    }
}

static void launch_cappy_to_target() {
    if (!OMM_SSC_IS_SUCCESSFUL && !OMM_SSF_IS_GAME_PAUSED && !OMM_SSF_IS_TRANSITION) {
        static s32 sCappyTimer = 0;
        struct Object *cappy = omm_cappy_get_object();
        Vec3f targetPos = { sParams[0], sParams[1], sParams[2] };
        if (OMM_SSC_IS_OK && cappy && vec3f_dist(&cappy->oPosX, targetPos) < 20.f) {
            switch (sCappyTimer++) {
                case  0: case 14: case  27: case  39: case  50:
                case 60: case 69: case  77: case  84: case  90:
                case 95: case 99: case 102: case 104: case 105: {
                    play_sound(SOUND_GENERAL_SHORT_STAR, gGlobalSoundArgs);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SSM_HARD, 0, 10.f, 0.4f, 50.f);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SSM_HARD, 0, 10.f, 0.4f, 50.f);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SSM_HARD, 0, 10.f, 0.4f, 50.f);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SSM_HARD, 0, 10.f, 0.4f, 50.f);
                    omm_spawn_sparkly_star_sparkle(cappy, OMM_SSM_HARD, 0, 10.f, 0.4f, 50.f);
                } break;

                case 106: {
                    play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
                    omm_ssc_set(OMM_SSC_F_SUCCESSFUL, 1);
                } break;
            }
        } else {
            sCappyTimer = 0;
        }
    }
}

static void dont_touch_floor() {
    bool onGround = !(sMario->action & ACT_FLAG_AIR);

    // Auto-start: Mario starts jumping as soon as the dialog box closes
    if (sParams[0]) {

        // When warp-landing, trigger the dialog box...
        if (!OMM_SSC_IS_ENTRY_DIALOG) {
            sUpdateTimer = false;
            return;
        }
        
        // ...then, enter the jumping action
        if (sMario->prevAction == ACT_READING_AUTOMATIC_DIALOG) {
            omm_ssc_set(OMM_SSC_F_EVENT_STARTED, 1);
            omm_mario_set_action(sMario, ACT_LONG_JUMP, 0, 0xFFFF);
            mario_set_forward_vel(sMario, 48.f);
            return;
        }
    }
    
    // After leaving the ground, don't touch it again
    if (OMM_SSC_IS_OK) {
        if (!OMM_SSC_IS_EVENT_STARTED) {
            omm_ssc_set(OMM_SSC_F_EVENT_STARTED, (sMario->action == ACT_OMM_POSSESSION) || (((sMario->action & ACT_GROUP_MASK) != ACT_GROUP_CUTSCENE) && !onGround));
        } else if (onGround && sMario->action != ACT_READING_AUTOMATIC_DIALOG) {
            omm_ssc_state_set(OMM_SSC_FAIL, 1);
        }
    }
}

static void toads_on_pillars() {
    if (sMario->action != ACT_OMM_POSSESSION && OMM_SSC_IS_OK) {
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
                    omm_ssc_set(OMM_SSC_F_SUCCESSFUL, distToad1 < 200 && distToad2 < 200);
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
    if (!OMM_SSC_IS_OBJECTS_SPAWNED) {
        obj_deactivate_all_with_behavior(bhvToadMessage);
        for (const s32 *toadParams = sParams; *toadParams != END_PARAMS; toadParams += 5) {
            struct Object *toad = spawn_object(sMario->marioObj, MODEL_TOAD, bhvToadMessage2);
            toad->behavior = bhvToadMessage;
            toad->oBehParams = toadParams[0] << 24;
            toad->oBehParams2ndByte = 1;
            obj_set_pos(toad, toadParams[1], toadParams[2], toadParams[3]);
            obj_set_home(toad, toadParams[1], toadParams[2], toadParams[3]);
            obj_set_angle(toad, 0, toadParams[4], 0);
            obj_reset_hitbox(toad, 20, 100, 0, 0, 50, 0);
        }
        omm_ssc_set(OMM_SSC_F_OBJECTS_SPAWNED, 1);
        return;
    }
    
    // Prevent Cappy from capturing Toad if Mario and Cappy aren't in the same room as Toad
    s32 marioRoom = obj_get_room(sMario->marioObj);
    s32 cappyRoom = obj_get_room(omm_cappy_get_object());
    for_each_object_with_behavior(toad, bhvToadMessage) {
        if (marioRoom == cappyRoom && marioRoom == obj_get_room(toad)) {
            toad->oBehParams2ndByte = 0;
        } else {
            toad->oBehParams2ndByte = 1;
        }
    }

#if OMM_GAME_IS_SM64
    // BLJ assist to TTC clock Toad
    if (OMM_SSM_IS_HARD && gCurrLevelNum == LEVEL_CASTLE && gCurrAreaIndex == 2) {
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
        omm_ssc_state_set(OMM_SSC_INVALID, 0);
    }
}

static void invalidate_if_not_frozen() {
    if (!omm_world_is_frozen()) {
        omm_ssc_state_set(OMM_SSC_INVALID, 0);
    }
}

static void inc_timer_with_coins() {
    if (OMM_SSC_IS_OK) {
        s32 difY = omm_ssc_dif(OMM_SSC_C_COIN_Y);
        s32 difB = omm_ssc_dif(OMM_SSC_C_COIN_B);
        s32 difR = omm_ssc_dif(OMM_SSC_C_COIN_R);
        s32 difC = max_s(difY, 0) * 1 + max_s(difR, 0) * 2 + max_s(difB, 0) * 5;
        gHudDisplay.timer += difC * sParams[0];
    }
}

static void koopa_the_quick_race() {
    if (!OMM_SSC_IS_OK) {
        return;
    }
    
    // No NS if Koopa the Quick is not here
    struct Object *ktq = obj_get_first_with_behavior_and_field_s32(bhvKoopa, 0x2F, KOOPA_BP_KOOPA_THE_QUICK_BOB);
    if (!ktq) ktq = obj_get_first_with_behavior_and_field_s32(bhvKoopa, 0x2F, KOOPA_BP_KOOPA_THE_QUICK_THI);
    if (!ktq) {
        omm_ssc_state_set(OMM_SSC_INVALID, 0);
        return;
    }
    
    // Can't collect anything before the race has started
    if (ktq->oAction < KOOPA_THE_QUICK_ACT_RACE && (
        omm_ssc_cnt(OMM_SSC_C_COIN_Y) > 0 ||
        omm_ssc_cnt(OMM_SSC_C_COIN_L) > 0 ||
        omm_ssc_cnt(OMM_SSC_C_COIN_R) > 0 ||
        omm_ssc_cnt(OMM_SSC_C_COIN_B) > 0 ||
        omm_ssc_dif(OMM_SSC_C_SECRET) < 0 ||
        omm_stars_get_bits() != 0)) {
        omm_ssc_state_set(OMM_SSC_INVALID, 1);
        debug_print("collected object before race start\n");
        return;
    }

    // Can't trigger Bob-omb King fight before the race has started
    struct Object *kbo = obj_get_first_with_behavior(bhvKingBobomb);
    if (ktq->oAction < KOOPA_THE_QUICK_ACT_RACE && kbo && kbo->oAction != 0) {
        omm_ssc_state_set(OMM_SSC_INVALID, 1);
        debug_print("triggered king bob-omb before race start\n");
        return;
    }

    // Check distance with Koopa the Quick when starting the race
    if (ktq->oAction == KOOPA_THE_QUICK_ACT_SHOW_INIT_TEXT && obj_get_distance(ktq, gMarioObject) > 500.f) {
        omm_ssc_state_set(OMM_SSC_INVALID, 1);
        debug_print("too far from ktq at race start\n");
        return;
    }

    // Collect all stars...
    bool allStars = true;
    for (const s32 *star = &sParams[1]; *star != END_PARAMS; ++star) {
        if (!omm_stars_is_collected(*star)) {
            allStars = false;
            break;
        }
    }

    // ...but before touching the flag
    // This condition also invalidates the NS if Koopa the Quick
    // reaches the flag before all stars have been collected
    struct Object *flag = obj_get_first_with_behavior(bhvKoopaRaceEndpoint);
    if (!flag || (!allStars && flag->oKoopaRaceEndpointRaceEnded)) {
        omm_ssc_state_set(OMM_SSC_INVALID, 1);
        debug_print("touched flag before collecting all stars\n");
        return;
    }

    // Spawn the NS after collecting Koopa the Quick's star
    // No need to check if Mario lost or cheated, since
    // Koopa the Quick won't spawn his star in these cases
    s32 ktqStar = sParams[0];
    if (omm_stars_is_collected(ktqStar) && !omm_mario_is_star_dancing(sMario)) {
        omm_ssc_set(OMM_SSC_F_SUCCESSFUL, 1);
    }
}

static void all_star_rings() {
    if (!OMM_SSC_IS_OK) {
        return;
    }

    // Spawn rings
    if (!OMM_SSC_IS_OBJECTS_SPAWNED) {
        for (const s32 *ringParams = sParams; *ringParams != END_PARAMS; ringParams += 5) {
            omm_spawn_star_ring(
                sMario->marioObj,
                ringParams[0],
                ringParams[1],
                ringParams[2],
                ringParams[3],
                ringParams[4]
            );
        }
        omm_ssc_set(OMM_SSC_F_OBJECTS_SPAWNED, 1);
        return;
    }

    // Spawn NS when all rings are collected
    if (obj_get_count_with_behavior_and_field_s32(omm_bhv_star_ring, 0x31, 1) == 0) {
        omm_ssc_set(OMM_SSC_F_SUCCESSFUL, 1);
    }
}

//
// Level-specific functions
//

static void jrb__flooded_frozen() {
    if (omm_world_is_flooded() && sMario->area) {
        sMario->area->terrainType = TERRAIN_SNOW;
        if (OMM_SSC_IS_OK) {

            // Set dormant 'living' objects
            for_each_object_with_behavior(obj, bhvUnagi) { obj_set_dormant(obj, true); }
            for_each_object_with_behavior(obj, bhvUnagiSubobject) { obj_set_dormant(obj, true); }
            for_each_object_with_behavior(obj, bhvBobombBuddy) { obj_set_dormant(obj, true); }
            for_each_object_with_behavior(obj, bhvBobombBuddyOpensCannon) { obj_set_dormant(obj, true); }

            // Force Calm Vibe, and set freeze timer to about to damage Peach
            omm_peach_vibe_activate(sMario, OMM_PEACH_VIBE_TYPE_CALM);
            gOmmData->mario->state.freeze.dmg = max_s(gOmmData->mario->state.freeze.dmg, OMM_FREEZING_WATER_TIMER_MAX - 3);
            gOmmData->mario->state.freeze.gfx = OMM_FREEZING_WATER_TIMER_MAX;

            // If the star is spawned, reduce the Vibe gauge consumption to give just enough time to reach the star
            if (OMM_SSC_IS_STAR_SPAWNED) {
                if (omm_peach_vibe_is_calm() && !OMM_SSF_IS_TIME_STOP && OMM_SSF_MARIO_UPDATED) {
                    gOmmData->mario->peach.vibeGauge = clamp_s(gOmmData->mario->peach.vibeGauge - OMM_PEACH_VIBE_GAUGE_CALM_FREEZE_DEC / 2, 0, OMM_PEACH_VIBE_GAUGE_MAX);
                }
            }
            
            // Collecting coins is the only way to refill the Vibe gauge
            else {
                s32 dy = max_s(0, omm_ssc_dif(OMM_SSC_C_COIN_Y));
                s32 dr = max_s(0, omm_ssc_dif(OMM_SSC_C_COIN_R));
                s32 db = max_s(0, omm_ssc_dif(OMM_SSC_C_COIN_B));
                s32 dl = max_s(0, omm_ssc_dif(OMM_SSC_C_COIN_L));
                s32 inc = (dy + dl + 2 * dr + 5 * db) * ((OMM_PEACH_VIBE_GAUGE_MAX * 3) / 100);
                gOmmData->mario->peach.vibeGauge = clamp_s(gOmmData->mario->peach.vibeGauge - inc, 0, OMM_PEACH_VIBE_GAUGE_MAX);
            }
        }
    } else {
        omm_ssc_state_set(OMM_SSC_INVALID, 0);
    }
}

static void wdw__bring_chuckya_to_point() {
    if (OMM_SSC_IS_OK) {

        // Make the Chuckya follow Mario after some point
        for_each_object_with_behavior(obj, bhvChuckya) {
            if (gCurrAreaIndex == 2 || obj->oPosY < 500.f) {
                obj_set_home(obj, obj->oPosX, obj->oPosY, obj->oPosZ);
            }
        }

        // Update the Chuckya so Mario never ends up with an unloaded object in his hands
        s32 chuckyaFlagCurr = OMM_SSC_F_ZONES << (gCurrAreaIndex - 1);
        s32 chuckyaFlagPrev = OMM_SSC_F_ZONES << (2 - gCurrAreaIndex);
        if (omm_ssc_get(chuckyaFlagPrev) && omm_mario_is_holding(sMario)) {
            struct Object *chuckya = obj_get_first_with_behavior(bhvChuckya);
            if (!chuckya)  chuckya = spawn_object(sMario->marioObj, MODEL_CHUCKYA, bhvChuckya);
            chuckya->oHeldState = HELD_HELD;
            sMario->heldObj = chuckya;
        }
        if (omm_mario_is_holding(sMario) && sMario->heldObj && sMario->heldObj->behavior == bhvChuckya) {
            omm_ssc_set(chuckyaFlagCurr, 1);
        } else {
            omm_ssc_clr(chuckyaFlagCurr);
        }
        omm_ssc_clr(chuckyaFlagPrev);

        // Check if Mario is inside the opening where the red coins star is
        if ((gCurrAreaIndex == 2) &&
            (-1024 < sMario->pos[0]) && (sMario->pos[0] <  -512) &&
            (-1800 < sMario->pos[1]) && (sMario->pos[1] < -1400) &&
            (+3200 < sMario->pos[2]) && (sMario->pos[2] < +3712)) {


            // If Mario's holding a Chuckya and the Crystal Star isn't spawned yet, spawn it and replace the Chuckya
            if (omm_ssc_get(chuckyaFlagCurr) && !OMM_SSC_IS_SUCCESSFUL) {
                obj_set_dormant(sMario->heldObj, true);
                omm_ssc_set(OMM_SSC_F_SUCCESSFUL, 1);
                omm_ssc_spawn_star(sMario);
                struct Object *star = obj_get_first_with_behavior(omm_bhv_sparkly_star);
                star->oAction = 4;
                sMario->usedObj = spawn_object(sMario->marioObj, MODEL_NONE, omm_bhv_dummy);
                sMario->heldObj = NULL;
                mario_grab_used_object(sMario);
            }

            // If Mario released the Star, collect it
            struct Object *star = obj_get_first_with_behavior(omm_bhv_sparkly_star);
            if (star && !sMario->heldObj) {
                star->oAction = 2;
            }
        }
        
        // If not, remove the Star from Mario's hands and replace it by the Chuckya
        else if (OMM_SSC_IS_SUCCESSFUL) {
            omm_ssc_check_star(NULL);
            omm_ssc_clr(OMM_SSC_F_SUCCESSFUL);
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
        if ((sMario->pos[0] < -7925) || (sMario->pos[0] > -6975) ||
            (sMario->pos[2] < +3350) || (sMario->pos[2] > +4425) ||
            (sMario->numCoins != 0 && !OMM_SSC_IS_STAR_SPAWNED)) {
            omm_ssc_state_set(OMM_SSC_FAIL, 1);
        }
    }

    // Spawns a dormant goomba at the center of the safe zone
    static struct Object *sBitdwGoomba = NULL;
    if (!obj_get_first_with_behavior(omm_bhv_sparkly_star_hint) && OMM_SSC_IS_OK) {
        f32 goombaX = (-7925 + -6975) / 2;
        f32 goombaZ = (+3350 + +4425) / 2;
        sBitdwGoomba = spawn_object_abs_with_rot(sMario->marioObj, 0, MODEL_GOOMBA, bhvGoomba, goombaX, +20000.f, goombaZ, 0, 0, 0);
        sBitdwGoomba->oBehParams = GOOMBA_SIZE_REGULAR << 16;
        sBitdwGoomba->oBehParams2ndByte = GOOMBA_SIZE_REGULAR;
        sBitdwGoomba->parentObj = sBitdwGoomba;
        sBitdwGoomba->prevObj = NULL;
        obj_drop_to_floor(sBitdwGoomba);
        obj_set_dormant(sBitdwGoomba, true);
    }

    // Possess the goomba as soon as it awakes
    if (OMM_SSC_IS_OK && sMario->action != ACT_READING_SIGN) {
        struct Object *goomba = obj_get_nearest_with_behavior(sMario->marioObj, bhvGoomba);
        if (goomba != NULL && goomba == sBitdwGoomba && !obj_is_dormant(goomba)) {
            if (omm_mario_possess_object(sMario, goomba, false)) {
                sBitdwGoomba = NULL;
            }
        }
    }
    
    // Hard mode: 7 Goombas stack and 74 coins
    if (OMM_SSM_IS_HARD && OMM_SSC_IS_OK) {
        if (sMario->action == ACT_OMM_POSSESSION && gOmmCapture->behavior == bhvGoomba && gOmmData->object->goomba.stackCount >= 6) {
            s32 collectedCoins = (
                omm_ssc_cnt(OMM_SSC_C_COIN_Y) * 1 +
                omm_ssc_cnt(OMM_SSC_C_COIN_B) * 5 +
                omm_ssc_cnt(OMM_SSC_C_COIN_R) * 2
            );
            if (collectedCoins >= 74) {
                omm_ssc_set(OMM_SSC_F_SUCCESSFUL, 1);
            }
        }
    }

    sUpdateTimer = (sMario->action == ACT_OMM_POSSESSION || OMM_SSC_IS_TIMER_STARTED);
}

static void bitfs__six_secrets() {
    static f32 sMarioLastGroundedY = 0.f;
    if (!(sMario->action & ACT_FLAG_AIR) && (sMario->action != ACT_OMM_POSSESSION || obj_is_on_ground(gOmmCapture))) {
        sMarioLastGroundedY = sMario->pos[1];
    }

    // Spawn pillars, secrets and flamethrowers
    if (!OMM_SSC_IS_OBJECTS_SPAWNED) {
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
        omm_ssc_set(OMM_SSC_F_OBJECTS_SPAWNED, 1);
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
    if (OMM_SSC_IS_OK && omm_ssc_dif(OMM_SSC_C_SECRET) < 0) {
        s32 index = 6 - omm_ssc_cnt(OMM_SSC_C_SECRET);
        spawn_object_relative(index, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
        play_sound(SOUND_MENU_COLLECT_SECRET + (((u8) index - 1) << 16), gGlobalSoundArgs);
    }
    omm_ssc_set(OMM_SSC_F_SUCCESSFUL, omm_ssc_cnt(OMM_SSC_C_SECRET) == 0);
}

static void bits__vanished_all_star_rings() {
    if (omm_world_is_vanished()) {
        all_star_rings();
    } else {
        omm_ssc_state_set(OMM_SSC_INVALID, 0);
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
        if (omm_ssc_get(OMM_SSC_F_ZONES << i)) {
            numZones++;
        } else {
            f32 x = sVcutmZones[i][0];
            f32 z = sVcutmZones[i][1];
            f32 d = sqrtf(sqr_f(sMario->pos[0] - x) + sqr_f(sMario->pos[2] - z));
            if (d < 300) {
                omm_ssc_set(OMM_SSC_F_ZONES << i, 1);
                newZone = true;
                numZones++;
            }
        }
    }
    omm_ssc_set(OMM_SSC_F_SUCCESSFUL, numZones == 5);

    // Spawn secret effect if Mario enters a new zone
    if (newZone && OMM_SSC_IS_OK) {
        spawn_object_relative(numZones, 0, 0, 0, sMario->marioObj, MODEL_NUMBER, bhvOrangeNumber)->oPosY += 80;
        play_sound(SOUND_MENU_COLLECT_SECRET + (((u8) numZones - 1) << 16), gGlobalSoundArgs);
    }
}

static void basement__catch_mips() {

    // Unspawn the regular Mips and spawn the super fast Mips
    if (!OMM_SSC_IS_OBJECTS_SPAWNED) {
        obj_deactivate_all_with_behavior(bhvMips);
        omm_spawn_mips(sMario->marioObj, -1509, -1177, -1564, 60.f);
        omm_ssc_set(OMM_SSC_F_OBJECTS_SPAWNED, 1);
        return;
    }
    
    // Mips must be caught 3 times
    struct Object *mips = obj_get_first_with_behavior(omm_bhv_mips);
    if (mips && OMM_SSC_IS_OK) {
        omm_ssc_set(OMM_SSC_F_SUCCESSFUL, mips->oMipsGrabbedCounter >= 3);
    }
}

//
// Update per difficulty and level
//

#define NO_PARAM { END_PARAMS }
void omm_ssc_update_level(struct MarioState *m, const OmmSSD *data) {
    static struct { void (*func)(void); const s32 params[256]; } sOmmSSCLevelFunc[3][29] = {
        { // Normal
        { /*  0 */ all_secrets,                     NO_PARAM },
        { /*  1 */ NULL,                            NO_PARAM },
        { /*  2 */ invalidate_if_not_flooded,       NO_PARAM },
        { /*  3 */ NULL,                            NO_PARAM },
        { /*  4 */ NULL,                            NO_PARAM },
        { /*  5 */ NULL,                            NO_PARAM },
        { /*  6 */ eight_shards_and_lava,           { 0, 3570, 2, 4096, -1700, 60, -440, 2550, 630, 1120, 250, 1210, 2640, -2080, 1915, -300, -1600, 2440, -2150, 1300, 3705, -2500, 420, 2365, 575, 662, 3300, 708, } },
        { /*  7 */ NULL,                            NO_PARAM },
        { /*  8 */ NULL,                            NO_PARAM },
        { /*  9 */ snowmen_at_top,                  { 1, 0, 4860, 0 } },
        { /* 10 */ NULL,                            NO_PARAM },
        { /* 11 */ NULL,                            NO_PARAM },
        { /* 12 */ ten_goombas_stack,               NO_PARAM },
        { /* 13 */ all_boxes,                       { 13 } },
        { /* 14 */ NULL,                            NO_PARAM },
        { /* 15 */ bitdw__goomba_and_coins,         NO_PARAM },
        { /* 16 */ all_mushrooms,                   { 7, 1 } },
        { /* 17 */ NULL,                            NO_PARAM },
        { /* 18 */ NULL,                            NO_PARAM },
        { /* 19 */ vcutm__five_corners,             NO_PARAM },
        { /* 20 */ dont_touch_floor,                { 1 } },
        { /* 21 */ NULL,                            NO_PARAM },
        { /* 22 */ NULL,                            NO_PARAM },
        { /* 23 */ invalidate_if_not_frozen,        NO_PARAM },
        { /* 24 */ NULL,                            NO_PARAM },
        { /* 25 */ eight_shards,                    { -1024, 400, -250, -1024, 800, 1400, -4500, 1200, -450, -2280, -400, -3040, -128, -300, -2640, 2000, 900, -1700, 1910, 850, -2120, -5500, 600, -4300 } },
        { /* 26 */ toads_on_pillars,                NO_PARAM },
        { /* 27 */ eight_shards,                    { -1000, 1210, 1480, -4780, 3200, -3240, 3538, 1680, 3880, -610, 1900, 2500, -200, 2800, 5000, -3400, 3600, 5886, 3000, 2900, 5886, -200, 5400, -3700 } },
        { /* 28 */ eight_secrets_in_order,          { 1500, 0, -1740, -820, 20, -3630, 820, 20, -3630, -820, 20, 200, 820, 20, 200, -1500, 0, -1740, 2040, -200, -480, -2240, -200, -2815, } },
        },
        { // Hard
        { /*  0 */ all_secrets,                     NO_PARAM },
        { /*  1 */ NULL,                            NO_PARAM },
        { /*  2 */ invalidate_if_not_flooded,       NO_PARAM },
        { /*  3 */ NULL,                            NO_PARAM },
        { /*  4 */ NULL,                            NO_PARAM },
        { /*  5 */ all_flames,                      NO_PARAM },
        { /*  6 */ eight_shards_and_lava,           { 0, 3570, 3, 4096, -1700, 60, -440, 180, 1050, -1360, 2550, 630, 1120, -480, 1210, 2630, 705, 2160, 330, 0, 4690, -2800, 662, 3900, 708, 2800, 3000, 250 } },
        { /*  7 */ inc_timer_with_coins,            { 45 } },
        { /*  8 */ NULL,                            NO_PARAM },
        { /*  9 */ snowmen_at_top,                  { 4, 0, 4860, 0 } },
        { /* 10 */ wdw__bring_chuckya_to_point,     NO_PARAM },
        { /* 11 */ NULL,                            NO_PARAM },
        { /* 12 */ ten_goombas_stack,               NO_PARAM },
        { /* 13 */ dont_touch_floor,                { 0 } },
        { /* 14 */ all_mushrooms,                   { 9, 0 } },
        { /* 15 */ bitdw__goomba_and_coins,         NO_PARAM },
        { /* 16 */ bitfs__six_secrets,              NO_PARAM },
        { /* 17 */ NULL,                            NO_PARAM },
        { /* 18 */ NULL,                            NO_PARAM },
        { /* 19 */ vcutm__five_corners,             NO_PARAM },
        { /* 20 */ dont_touch_floor,                { 1 } },
        { /* 21 */ NULL,                            NO_PARAM },
        { /* 22 */ NULL,                            NO_PARAM },
        { /* 23 */ invalidate_if_not_frozen,        NO_PARAM },
        { /* 24 */ NULL,                            NO_PARAM },
        { /* 25 */ toads_hide_and_seek,             { DIALOG_134, -670, 0, -320, 0xA000, DIALOG_135, -2300, -204, -2100, 0x0000, END_PARAMS } },
        { /* 26 */ basement__catch_mips,            NO_PARAM },
        { /* 27 */ toads_hide_and_seek,             { DIALOG_076, -2260, 2253, 4850, 0xA000, DIALOG_083, -205, 2253, 7200, 0x7000, END_PARAMS } },
        { /* 28 */ launch_cappy_to_target,          { 9, 433, -1735 } },
        },
        { // Lunatic
        { /*  0 */ koopa_the_quick_race,            { 1, 0, 2, 3, 4, 5, 6, END_PARAMS } },
        { /*  1 */ NULL,                            NO_PARAM },
        { /*  2 */ jrb__flooded_frozen,             NO_PARAM },
        { /*  3 */ NULL,                            NO_PARAM },
        { /*  4 */ NULL,                            NO_PARAM },
        { /*  5 */ NULL,                            NO_PARAM },
        { /*  6 */ NULL,                            NO_PARAM },
        { /*  7 */ NULL,                            NO_PARAM },
        { /*  8 */ NULL,                            NO_PARAM },
        { /*  9 */ NULL,                            NO_PARAM },
        { /* 10 */ NULL,                            NO_PARAM },
        { /* 11 */ NULL,                            NO_PARAM },
        { /* 12 */ NULL,                            NO_PARAM },
        { /* 13 */ NULL,                            NO_PARAM },
        { /* 14 */ NULL,                            NO_PARAM },
        { /* 15 */ bits__vanished_all_star_rings,   { -5430, -4725, 0, 1, 0x4000, -2960, -4715, 0, 0, 0x0000, -300, -4450, 0, 1, 0x8000, 3900, -1700, 0, 1, 0x4000, 7500, -1500, 0, 1, 0x4000, 4100, -700, -1830, 0, 0x0000, 2060, -1600, -1560, 1, 0xA000, 50, -1700, -720, 0, 0x0000, -5000, -2885, -850, 1, 0x4000, -3725, -1350, -780, 0, 0x0000, -6850, -1150, -800, 0, 0x0000, -4000, -410, -1130, 1, 0x5000, 240, -160, -900, 0, 0x0000, 900, -75, -900, 1, 0xC000, 1200, 175, -900, 1, 0xC000, 1500, 425, -900, 1, 0xC000, 1800, 675, -900, 1, 0xC000, 2100, 925, -900, 1, 0xC000, 2400, 1175, -900, 1, 0xC000, 6460, 3735, -1600, 1, 0x8000, 1300, 2250, -900, 1, 0xC000, -128, 2600, -900, 0, 0x0000, -1760, 3300, -900, 1, 0xC000, -8000, 2350, -900, 1, 0xC000, -6460, 4700, -900, 0, 0x0000, -4100, 3650, -900, 0, 0x0000, -2500, 3650, -900, 0, 0x0000, 4350, 5555, -1000, 1, 0x4000, 350, 7000, -3400, 1, 0x8000, 350, 7200, -6030, 0, 0x0000, END_PARAMS } },
        { /* 16 */ NULL,                            NO_PARAM },
        { /* 17 */ NULL,                            NO_PARAM },
        { /* 18 */ NULL,                            NO_PARAM },
        { /* 19 */ NULL,                            NO_PARAM },
        { /* 20 */ NULL,                            NO_PARAM },
        { /* 21 */ NULL,                            NO_PARAM },
        { /* 22 */ NULL,                            NO_PARAM },
        { /* 23 */ NULL,                            NO_PARAM },
        { /* 24 */ NULL,                            NO_PARAM },
        { /* 25 */ NULL,                            NO_PARAM },
        { /* 26 */ NULL,                            NO_PARAM },
        { /* 27 */ NULL,                            NO_PARAM },
        { /* 28 */ NULL,                            NO_PARAM },
        },
    };

    sUpdateTimer = true;
    s32 mode = gOmmSSM - 1;
    s32 star = data->index;
    if (sOmmSSCLevelFunc[mode][star].func) {
        sMario = m;
        sData = data;
        sParams = sOmmSSCLevelFunc[mode][star].params;
        sOmmSSCLevelFunc[mode][star].func();
    }
    omm_ssc_update_timer(m, data, sUpdateTimer);
}

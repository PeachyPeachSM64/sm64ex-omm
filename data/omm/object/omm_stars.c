#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

struct MacroPreset { const BehaviorScript *behavior; s16 model; s16 param; };
extern struct MacroPreset MacroObjectPresets[];

//
// Data
//

typedef struct {
    const BehaviorScript *behavior;
    s32 behParams;
} OmmStarData;
static OmmStarData **sOmmStarsData[LEVEL_COUNT][8] = { { NULL } };

static void omm_stars_add_data(s32 level, s32 area, s32 index, const BehaviorScript *behavior, s32 behParams) {
    sOmmStarsData[level][area][index] = OMM_MEMNEW(OmmStarData, 1);
    sOmmStarsData[level][area][index]->behavior = behavior;
    sOmmStarsData[level][area][index]->behParams = behParams;
}

//
// Preprocessing
//

#ifndef TOAD_STAR_1_DIALOG
#define TOAD_STAR_1_DIALOG DIALOG_082
#endif
#ifndef TOAD_STAR_2_DIALOG
#define TOAD_STAR_2_DIALOG DIALOG_076
#endif
#ifndef TOAD_STAR_3_DIALOG
#define TOAD_STAR_3_DIALOG DIALOG_083
#endif
#ifndef TOAD_STAR_1_DIALOG_AFTER
#define TOAD_STAR_1_DIALOG_AFTER DIALOG_154
#endif
#ifndef TOAD_STAR_2_DIALOG_AFTER
#define TOAD_STAR_2_DIALOG_AFTER DIALOG_155
#endif
#ifndef TOAD_STAR_3_DIALOG_AFTER
#define TOAD_STAR_3_DIALOG_AFTER DIALOG_156
#endif

static s32 sOmmStarsTargetLevelNum;
static s32 sOmmStarsTargetAreaIndex;
static s32 sOmmStarsCurrAreaIndex;
static s32 omm_stars_preprocess_level_cmd(u8 type, void *cmd) {
    if (type == LEVEL_CMD_AREA) {
        sOmmStarsCurrAreaIndex = level_cmd_get(cmd, u8, 2);
    } else if (sOmmStarsCurrAreaIndex == sOmmStarsTargetAreaIndex) {
        if (type == LEVEL_CMD_OBJECT_WITH_ACTS) {
            const BehaviorScript *behavior = level_cmd_get(cmd, const BehaviorScript *, 20);
            u32 behaviorArg = level_cmd_get(cmd, u32, 16);

            // Common objects
            if ((behavior == bhvStar) ||
                (behavior == bhvHiddenStar) ||
                (behavior == bhvHiddenRedCoinStar) ||
                (behavior == bhvBowserCourseRedCoinStar) ||

                // BOB
                (behavior == bhvKingBobomb) ||
                (behavior == bhvKoopa && (behaviorArg & 0x00FF0000)) ||

                // WF
                (behavior == bhvWhompKingBoss) ||

                // JRB
                (behavior == bhvUnagi && (behaviorArg & 0x00010000)) ||
                (behavior == bhvTreasureChestsJrb) ||

                // CCM
                (behavior == bhvCcmTouchedStarSpawn) ||
                (behavior == bhvTuxiesMother) ||
                (behavior == bhvRacingPenguin) ||
                (behavior == bhvSnowmansHead) ||

                // BBH
                (behavior == bhvGhostHuntBigBoo) ||
                (behavior == bhvMerryGoRoundBooManager) ||
                (behavior == bhvBalconyBigBoo) ||
                (behavior == bhvMrI && (behaviorArg & 0x00FF0000)) ||

                // LLL
                (behavior == bhvBigBully) ||
                (behavior == bhvBigBullyWithMinions) ||

                // SSL
                (behavior == bhvKlepto && (behaviorArg & 0x00FF0000)) ||
                (behavior == bhvEyerokBoss) ||

                // DDD
                (behavior == bhvTreasureChests) ||
                (behavior == bhvMantaRay) ||
                (behavior == bhvJetStreamRingSpawner) ||

                // SL
                (behavior == bhvBigChillBully) ||

                // TTM
                (behavior == bhvUkikiCage) ||

                // THI
                (behavior == bhvWigglerHead) ||
                (behavior == bhvFirePiranhaPlant && (behaviorArg & 0x00FF0000)) ||

#if OMM_GAME_IS_SMSR
                (behavior == bhvCustomSMSRStarReplica) ||
                (behavior == bhvCustomSMSRStarMoving) ||
#endif        
            0) {
                u8 starIndex = (u8) ((behaviorArg >> 24) & 0x07);
                omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, starIndex, behavior, behaviorArg);
            }

            // Exclamation boxes
            // These ones are unnecessary complex (and bugged)
            // Their 1st byte is the star index, and their 2nd byte
            // translates into another star index bitOr'ed with the first
            // Everything is fine as long as the first is 0 or the second is 8,
            // but it will give wrong star indexes if it's not the case...
            else if (behavior == bhvExclamationBox) {
                u8 starIndex = (u8) ((behaviorArg >> 24) & 0x07);
                switch ((behaviorArg >> 16) & 0xFF) {
                    case 0x08: omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, starIndex | 0, bhvExclamationBox, behaviorArg); break;
                    case 0x0A: omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, starIndex | 1, bhvExclamationBox, behaviorArg); break;
                    case 0x0B: omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, starIndex | 2, bhvExclamationBox, behaviorArg); break;
                    case 0x0C: omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, starIndex | 3, bhvExclamationBox, behaviorArg); break;
                    case 0x0D: omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, starIndex | 4, bhvExclamationBox, behaviorArg); break;
                    case 0x0E: omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, starIndex | 5, bhvExclamationBox, behaviorArg); break;
                }
            }

            // Toads
            else if (behavior == bhvToadMessage) {
                switch ((behaviorArg >> 24) & 0xFF) {
                    case TOAD_STAR_1_DIALOG: omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, 0, bhvToadMessage, behaviorArg); break;
                    case TOAD_STAR_2_DIALOG: omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, 1, bhvToadMessage, behaviorArg); break;
                    case TOAD_STAR_3_DIALOG: omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, 2, bhvToadMessage, behaviorArg); break;
                }
            }

            // Mips
            else if (behavior == bhvMips) {
                omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, 3, bhvMips, 0);
                omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, 4, bhvMips, 0);
            }
        } else if (type == LEVEL_CMD_MACRO_OBJECTS) {
            MacroObject *data = level_cmd_get(cmd, MacroObject *, 4);
            for (; *data != MACRO_OBJECT_END(); data += 5) {
                s32 presetId = (s32) ((data[0] & 0x1FF) - 0x1F);
                s32 starIndex = -1;
                switch (presetId) {
                    case macro_box_star_1: starIndex = 0; break;
                    case macro_box_star_2: starIndex = 1; break;
                    case macro_box_star_3: starIndex = 2; break;
                    case macro_box_star_4: starIndex = 3; break;
                    case macro_box_star_5: starIndex = 4; break;
                    case macro_box_star_6: starIndex = 5; break;
                }
                if (starIndex != -1) {
                    const BehaviorScript *behavior = MacroObjectPresets[presetId].behavior;
                    s32 presetParams = MacroObjectPresets[presetId].param;
                    s32 objParams = (data[4] & 0xFF00) + (presetParams & 0x00FF);
                    s32 behParams = ((objParams & 0x00FF) << 16) + (objParams & 0xFF00);
                    omm_stars_add_data(sOmmStarsTargetLevelNum, sOmmStarsTargetAreaIndex, starIndex, behavior, behParams);
                }
            }
        }
    }
    return LEVEL_SCRIPT_CONTINUE;
}

static void omm_stars_load_data(s32 level, s32 area) {
    if (!sOmmStarsData[level][area]) {
        sOmmStarsData[level][area] = OMM_MEMNEW(OmmStarData *, 8);

        // Level Stars
        sOmmStarsTargetLevelNum = level;
        sOmmStarsTargetAreaIndex = area;
        sOmmStarsCurrAreaIndex = 0;
        level_script_preprocess(omm_level_get_script(level), omm_stars_preprocess_level_cmd);

        // 100 Coins Star
        if (COURSE_IS_MAIN_COURSE(omm_level_get_course(level))) {
            omm_stars_add_data(level, area, 6, NULL, 0);
        }

        // Slide Secret Star
        if (level == OMM_LEVEL_SLIDE) {
            omm_stars_add_data(level, area, 1, NULL, 0);
        }
    }
}

static u8 omm_stars_get_bits_per_area(s32 level, s32 area) {
    omm_stars_load_data(level, area);
    u8 starBits = 0;
    for (s32 i = 0; i != 8; ++i) {
        if (sOmmStarsData[level][area][i]) {
            starBits |= (1 << i);
        }
    }
    return starBits;
}

static u32 omm_stars_get_color_per_course(s32 course) {
    static u32 *sOmmStarsColors = NULL;
    if (!sOmmStarsColors) {
        sOmmStarsColors = OMM_MEMNEW(u32, 18);
        static const char *sOmmStarsTextures[] = {
            OMM_TEXTURE_STAR_BODY_0,  OMM_TEXTURE_STAR_BODY_1,  OMM_TEXTURE_STAR_BODY_2,
            OMM_TEXTURE_STAR_BODY_3,  OMM_TEXTURE_STAR_BODY_4,  OMM_TEXTURE_STAR_BODY_5,
            OMM_TEXTURE_STAR_BODY_6,  OMM_TEXTURE_STAR_BODY_7,  OMM_TEXTURE_STAR_BODY_8,
            OMM_TEXTURE_STAR_BODY_9,  OMM_TEXTURE_STAR_BODY_10, OMM_TEXTURE_STAR_BODY_11,
            OMM_TEXTURE_STAR_BODY_12, OMM_TEXTURE_STAR_BODY_13, OMM_TEXTURE_STAR_BODY_14,
            OMM_TEXTURE_STAR_BODY_15, OMM_TEXTURE_STAR_BODY_16, OMM_TEXTURE_STAR_BODY_17,
        };
        for (s32 i = 0; i != 18; ++i) {
            OMM_STRING(filename, 256, "%s/%s/%s.png", OMM_EXE_FOLDER, OMM_GFX_FOLDER, sOmmStarsTextures[i]);
            s32 w, h;
            u8 *p = stbi_load(filename, &w, &h, NULL, 4);
            if (p) {
                u32 r = 0;
                u32 g = 0;
                u32 b = 0;
                for (s32 i = 0; i != w * h; ++i) {
                    r += (u32) p[4 * i + 0];
                    g += (u32) p[4 * i + 1];
                    b += (u32) p[4 * i + 2];
                }
                sOmmStarsColors[i] = (((u32) ((u8) (r / (w * h)))) << 24) | (((u32) ((u8) (g / (w * h)))) << 16) | (((u32) ((u8) (b / (w * h)))) << 8) | (0xFF);
                stbi_image_free(p);
            }
        }
    }
    return sOmmStarsColors[((course < COURSE_COUNT) ? clamp_s(course, 0, 16) : 17)];
}

//
// Stars
//

static u8 sOmmStarsBits = 0;

void omm_stars_init_bits() {
    sOmmStarsBits = 0;
}

u8 omm_stars_get_bits() {
    return sOmmStarsBits;
}

u8 omm_stars_get_bits_total(s32 level) {
#if OMM_GAME_IS_SM74
    return omm_stars_get_bits_per_area(level, sm74_mode__omm_stars_get_bits_total);
#else
    return omm_stars_get_bits_per_area(level, 0) |
           omm_stars_get_bits_per_area(level, 1) |
           omm_stars_get_bits_per_area(level, 2) |
           omm_stars_get_bits_per_area(level, 3) |
           omm_stars_get_bits_per_area(level, 4) |
           omm_stars_get_bits_per_area(level, 5) |
           omm_stars_get_bits_per_area(level, 6) |
           omm_stars_get_bits_per_area(level, 7);
#endif
}

u32 omm_stars_get_color(s32 level) {
    s32 course = (level <= 0 ? COURSE_COUNT : omm_level_get_course(level));
    return omm_stars_get_color_per_course(course);
}

bool omm_stars_is_collected(s32 index) {
    return OMM_STARS_NON_STOP && ((sOmmStarsBits >> index) & 1);
}

// All stars of a level are declared collected if
// - It's not a Bowser or a Castle level
// and
// - It's a Bowser fight
// or
// - Every star bit is 1 and there is an exit warp
bool omm_stars_all_collected(s32 level) {
    return (level != LEVEL_BITDW  ) &&
           (level != LEVEL_BITFS  ) &&
           (level != LEVEL_BITS   ) && 
           (level != LEVEL_GROUNDS) && 
           (level != LEVEL_CASTLE ) && 
           (level != LEVEL_COURT  ) && (
#if OMM_GAME_IS_SMSR
           (level == LEVEL_ENDING) ||
#endif
           (level == LEVEL_BOWSER_1) ||
           (level == LEVEL_BOWSER_2) ||
           (level == LEVEL_BOWSER_3) ||
           (omm_level_get_exit_warp(level, gCurrAreaIndex) &&
           (omm_stars_get_bits() == omm_stars_get_bits_total(level))));
}

void omm_stars_set_bits(u8 bits) {
    sOmmStarsBits |= bits;
}

//
// Update
//

static void unload_child_objects(struct Object *o, const BehaviorScript *behavior) {
    for (bool done = false; !done;) {
        done = true;
        for_each_object_with_behavior(child, behavior) {
            if (child->parentObj == o) {
                unload_object(child);
                done = false;
                break;
            }
        }
    }
}

static void unload_objects_with_param(const BehaviorScript *behavior, s32 behParams2ndByte) {
    for (bool done = false; !done;) {
        done = true;
        for_each_object_with_behavior(obj, behavior) {
            if (obj->oBehParams2ndByte == behParams2ndByte) {
                unload_object(obj);
                done = false;
                break;
            }
        }
    }
}

OMM_ROUTINE_UPDATE(omm_stars_update) {
    if ((gMarioObject && OMM_STARS_NON_STOP) &&
        (gMarioState->action != ACT_STAR_DANCE_EXIT) &&
        (gMarioState->action != ACT_STAR_DANCE_NO_EXIT) &&
        (gMarioState->action != ACT_STAR_DANCE_WATER) &&
        (gMarioState->action != ACT_FALL_AFTER_STAR_GRAB) &&
        (gMarioState->action != ACT_JUMBO_STAR_CUTSCENE) &&
        (gMarioState->action != ACT_OMM_STAR_DANCE) &&
        (gMarioState->action != ACT_OMM_SPARKLY_STAR_DANCE) &&
        (gMarioState->action != ACT_OMM_TRANSITION_WF_TOWER) &&
        (gMarioState->action != ACT_OMM_POSSESSION || gOmmData->mario->capture.timer != 0xFF)) {

        // Check and update for each star bit
        // Unload objects or set actions to prevent already collected stars from respawning
        omm_stars_load_data(gCurrLevelNum, gCurrAreaIndex);
        for (s32 i = 0; i != 8; ++i) {
            const OmmStarData *starData = sOmmStarsData[gCurrLevelNum][gCurrAreaIndex][i];
            if (starData && ((sOmmStarsBits >> i) & 1)) {
                for (bool done = false; !done;) {
                    done = true;
                    for_each_object_with_behavior(o, starData->behavior) {
                        if (starData->behParams == o->oBehParams) {

                            // Unload the object
                            if (starData->behavior == bhvExclamationBox ||
                                starData->behavior == bhvGhostHuntBigBoo ||
                                starData->behavior == bhvMerryGoRoundBooManager ||
                                starData->behavior == bhvBalconyBigBoo ||
                                starData->behavior == bhvMrI ||
                                starData->behavior == bhvBigBully ||
                                starData->behavior == bhvBigBullyWithMinions ||
                                starData->behavior == bhvKlepto ||
                                starData->behavior == bhvBigChillBully) {
                                unload_object(o);
                                done = false;
                                break;
                            }

                            // Stop the Boss music and unload
                            if (starData->behavior == bhvKingBobomb ||
                                starData->behavior == bhvWhompKingBoss ||
                                starData->behavior == bhvEyerokBoss) {
                                stop_background_music(SEQ_EVENT_BOSS);
                                unload_object(o);
                                done = false;
                                break;
                            }

                            // BOB/THI Koopa the Quick
                            // Unload him and his flag only if the race has not started
                            if (starData->behavior == bhvKoopa) {
                                if (o->oAction == KOOPA_THE_QUICK_ACT_WAIT_BEFORE_RACE) {
                                    unload_objects_with_param(bhvKoopaRaceEndpoint, 0);
                                    unload_objects_with_param(bhvKoopaFlag, 0);
                                    unload_object(o);
                                    done = false;
                                    break;
                                }
                            }

                            // JRB Unagi
                            // Unload it only if it's in its hole
                            if (starData->behavior == bhvUnagi) {
                                if (o->oAction == 3) {
                                    unload_object(o);
                                    done = false;
                                    break;
                                }
                            }

                            // JRB/DDD Treasure chests
                            // Make the treasure chests already opened
                            if (starData->behavior == bhvTreasureChestsJrb ||
                                starData->behavior == bhvTreasureChests) {
                                o->oAction = 2;
                                o->oTreasureChestUnkF4 = 0;
                                o->oTreasureChestUnkF8 = 0;
                                for_each_object_with_behavior(chestBottom, bhvTreasureChestBottom) {
                                    if (chestBottom->parentObj == o) {
                                        chestBottom->oAction = 1;
                                        for_each_object_with_behavior(chestTop, bhvTreasureChestTop) {
                                            if (chestTop->parentObj == chestBottom) {
                                                chestTop->oAction = 2;
                                                chestTop->oFaceAnglePitch = -0x4000;
                                            }
                                        }
                                    }
                                }
                                continue;
                            }

                            // CCM Racing penguin
                            // Unload him only if the race has not started
                            if (starData->behavior == bhvRacingPenguin) {
                                if (o->oAction == RACING_PENGUIN_ACT_WAIT_FOR_MARIO) {
                                    unload_object(o);
                                    done = false;
                                    break;
                                }
                            }

                            // CCM Tuxies' mother
                            // Stop asking for her baby
                            if (starData->behavior == bhvTuxiesMother) {
                                if (o->oAction == 0) {
                                    struct Object *tuxie = obj_get_first_with_behavior(bhvSmallPenguin);
                                    if (tuxie) obj_set_behavior(tuxie, bhvUnused20E0);
                                    o->oAction = 2;
                                }
                                continue;
                            }

                            // CCM Snowman
                            // Make the Snowman big again
                            // Unload snowman's bottom
                            if (starData->behavior == bhvSnowmansHead) {
                                if (o->oAction == 0) {
                                    unload_child_objects(o, bhvSnowmansBottom);
                                    spawn_object(o, MODEL_CCM_SNOWMAN_BASE, bhvBigSnowmanWhole);
                                    o->oPosY += 350;
                                    o->oAction = 1;
                                }
                                continue;
                            }

                            // DDD Manta ray
                            // Stop spawning water rings
                            if (starData->behavior == bhvMantaRay) {
                                unload_child_objects(o, bhvMantaRayWaterRing);
                                o->oAction = 1;
                                continue;
                            }

                            // DDD Jet stream ring spawner
                            // Stop spawning water rings
                            if (starData->behavior == bhvJetStreamRingSpawner) {
                                unload_child_objects(o, bhvJetStreamWaterRing);
                                o->oAction = JS_RING_SPAWNER_ACT_INACTIVE;
                                continue;
                            }

                            // TTM Star from Ukiki's cage
                            // Unload the cage's objects and cage Ukikis
                            if (starData->behavior == bhvUkikiCage) {
                                unload_objects_with_param(bhvUkiki, UKIKI_CAGE);
                                unload_objects_with_param(bhvMacroUkiki, UKIKI_CAGE);
                                unload_child_objects(o, bhvUkikiCageStar);
                                unload_child_objects(o, bhvUkikiCageChild);
                                unload_object(o);
                                done = false;
                                break;
                            }

                            // THI Wiggler
                            // Unload Wiggler's objects
                            if (starData->behavior == bhvWigglerHead) {
                                stop_background_music(SEQ_EVENT_BOSS);
                                unload_child_objects(o, bhvWigglerBody);
                                unload_object(o);
                                done = false;
                                break;
                            }

                            // THI Piranha plants
                            // Set the killed counter to 0
                            if (starData->behavior == bhvFirePiranhaPlant) {
                                sNumKilledFirePiranhaPlants = 0;
                                continue;
                            }

                            // Toad
                            // Set its dialog to the one after collecting the star
                            if (starData->behavior == bhvToadMessage) {
                                switch (o->oToadMessageDialogId) {
                                    case TOAD_STAR_1_DIALOG: o->oToadMessageDialogId = TOAD_STAR_1_DIALOG_AFTER; break;
                                    case TOAD_STAR_2_DIALOG: o->oToadMessageDialogId = TOAD_STAR_2_DIALOG_AFTER; break;
                                    case TOAD_STAR_3_DIALOG: o->oToadMessageDialogId = TOAD_STAR_3_DIALOG_AFTER; break;
                                }
                                continue;
                            }

                            // Mips
                            // Set status to "Already spawned star"
                            if (starData->behavior == bhvMips) {
                                o->oMipsStarStatus = MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR;
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/omm_constants.h"
#include "behavior_commands.h"

struct MacroPreset { const BehaviorScript *behavior; s16 model; s16 param; };
extern struct MacroPreset MacroObjectPresets[];

#define OMM_STAR_TYPE_DEFAULT           (0)
#define OMM_STAR_TYPE_EXCLAMATION_BOX   (1)
#define OMM_STAR_TYPE_BOSS              (2)
#define OMM_STAR_TYPE_KOOPA_THE_QUICK   (3)
#define OMM_STAR_TYPE_UNAGI             (4)
#define OMM_STAR_TYPE_TREASURE_CHEST    (5)
#define OMM_STAR_TYPE_RACING_PENGUIN    (6)
#define OMM_STAR_TYPE_TUXIES_MOTHER     (7)
#define OMM_STAR_TYPE_BIG_SNOWMAN       (8)
#define OMM_STAR_TYPE_MANTA_RAY         (9)
#define OMM_STAR_TYPE_JET_STREAM        (10)
#define OMM_STAR_TYPE_UKIKI_CAGE        (11)
#define OMM_STAR_TYPE_WIGGLER           (12)
#define OMM_STAR_TYPE_FIRE_PLANTS       (13)
#define OMM_STAR_TYPE_TOAD              (14)
#define OMM_STAR_TYPE_MIPS              (15)

#define NO_BHV_ARG                      (0xFFFFFFFF)

typedef struct { const BehaviorScript *behavior; s32 type; u32 arg; } OmmStarPreset;
static const OmmStarPreset OMM_STAR_PRESETS[] = {
// Stars
    { bhvStar,                      OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
    { bhvHiddenStar,                OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
    { bhvHiddenRedCoinStar,         OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
    { bhvBowserCourseRedCoinStar,   OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
#if OMM_GAME_IS_SMSR
    { bhvCustomSMSRStarReplica,     OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
    { bhvCustomSMSRStarMoving,      OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
#endif
// Exclamation box
    { bhvExclamationBox,            OMM_STAR_TYPE_EXCLAMATION_BOX,  NO_BHV_ARG },
// Toad
    { bhvToadMessage,               OMM_STAR_TYPE_TOAD,             NO_BHV_ARG },
// Mips
    { bhvMips,                      OMM_STAR_TYPE_MIPS,             NO_BHV_ARG },
// BOB
    { bhvKingBobomb,                OMM_STAR_TYPE_BOSS,             NO_BHV_ARG },
    { bhvKoopa,                     OMM_STAR_TYPE_KOOPA_THE_QUICK,  0x00FF0000 },
// WF
    { bhvWhompKingBoss,             OMM_STAR_TYPE_BOSS,             NO_BHV_ARG },
// JRB
    { bhvUnagi,                     OMM_STAR_TYPE_UNAGI,            0x00010000 },
    { bhvTreasureChestsJrb,         OMM_STAR_TYPE_TREASURE_CHEST,   NO_BHV_ARG },
// CCM
    { bhvCcmTouchedStarSpawn,       OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
    { bhvTuxiesMother,              OMM_STAR_TYPE_TUXIES_MOTHER,    NO_BHV_ARG },
    { bhvRacingPenguin,             OMM_STAR_TYPE_RACING_PENGUIN,   NO_BHV_ARG },
    { bhvSnowmansHead,              OMM_STAR_TYPE_BIG_SNOWMAN,      NO_BHV_ARG },
// BBH
    { bhvGhostHuntBigBoo,           OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
    { bhvMerryGoRoundBooManager,    OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
    { bhvBalconyBigBoo,             OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
    { bhvMrI,                       OMM_STAR_TYPE_DEFAULT,          0x00FF0000 },
// LLL
    { bhvBigBully,                  OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
    { bhvBigBullyWithMinions,       OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
// SSL
    { bhvKlepto,                    OMM_STAR_TYPE_DEFAULT,          0x00FF0000 },
    { bhvEyerokBoss,                OMM_STAR_TYPE_BOSS,             NO_BHV_ARG },
// DDD
    { bhvTreasureChests,            OMM_STAR_TYPE_TREASURE_CHEST,   NO_BHV_ARG },
    { bhvMantaRay,                  OMM_STAR_TYPE_MANTA_RAY,        NO_BHV_ARG },
    { bhvJetStreamRingSpawner,      OMM_STAR_TYPE_JET_STREAM,       NO_BHV_ARG },
// SL
    { bhvBigChillBully,             OMM_STAR_TYPE_DEFAULT,          NO_BHV_ARG },
// TTM
    { bhvUkikiCage,                 OMM_STAR_TYPE_UKIKI_CAGE,       NO_BHV_ARG },
// THI
    { bhvWigglerHead,               OMM_STAR_TYPE_WIGGLER,          NO_BHV_ARG },
    { bhvFirePiranhaPlant,          OMM_STAR_TYPE_FIRE_PLANTS,      0x00FF0000 },
};

static const OmmStarPreset *omm_stars_get_preset(const BehaviorScript *behavior, u32 behaviorArg) {
    for (u32 i = 0; i != array_length(OMM_STAR_PRESETS); ++i) {
        const OmmStarPreset *starPreset = OMM_STAR_PRESETS + i;
        if (starPreset->behavior == behavior && (starPreset->arg == NO_BHV_ARG || (behaviorArg & starPreset->arg) != 0)) {
            return starPreset;
        }
    }
    return NULL;
}

//
// Data
//

typedef struct { const OmmStarPreset *preset; s32 params; } OmmStarData;
static OmmStarData **sOmmStarsData[LEVEL_COUNT][8] = {0};

static void omm_stars_add_data(s32 levelNum, s32 areaIndex, u8 starIndex, const OmmStarPreset *starPreset, s32 behParams) {
    if (starIndex < OMM_NUM_STARS_MAX_PER_COURSE) {
        sOmmStarsData[levelNum][areaIndex][starIndex] = mem_new(OmmStarData, 1);
        sOmmStarsData[levelNum][areaIndex][starIndex]->preset = starPreset;
        sOmmStarsData[levelNum][areaIndex][starIndex]->params = behParams;
    }
}

//
// Preprocessing
//

static struct {
    s32 levelNum;
    s32 areaIndex;
    s32 currAreaIndex;
} sOmmStarsPreprocess[1];

static s32 omm_stars_preprocess_level_cmd(u8 type, void *cmd) {
    if (type == LEVEL_CMD_SLEEP || type == LEVEL_CMD_SLEEP_BEFORE_EXIT) {
        return LEVEL_SCRIPT_STOP;
    } else if (type == LEVEL_CMD_AREA) {
        sOmmStarsPreprocess->currAreaIndex = level_cmd_get(cmd, u8, 2);
    } else if (sOmmStarsPreprocess->currAreaIndex == sOmmStarsPreprocess->areaIndex) {
        if (type == LEVEL_CMD_OBJECT_WITH_ACTS) {
            const BehaviorScript *behavior = level_cmd_get(cmd, const BehaviorScript *, 20);
            u32 behaviorArg = level_cmd_get(cmd, u32, 16);
            const OmmStarPreset *starPreset = omm_stars_get_preset(behavior, behaviorArg);
            if (starPreset) {
                switch (starPreset->type) {

                    // Exclamation boxes
                    // These ones are unnecessary complex (and bugged)
                    // Their 1st byte is the star index, and their 2nd byte
                    // translates into another star index bitOr'ed with the first
                    // Everything is fine as long as the first is 0 or the second is 8,
                    // but it will give wrong star indexes if it's not the case...
                    case OMM_STAR_TYPE_EXCLAMATION_BOX: {
                        u8 starIndex = (u8) (behaviorArg >> 24);
                        switch ((behaviorArg >> 16) & 0xFF) {
                            case 0x08: omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, starIndex | 0, starPreset, behaviorArg); break;
                            case 0x0A: omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, starIndex | 1, starPreset, behaviorArg); break;
                            case 0x0B: omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, starIndex | 2, starPreset, behaviorArg); break;
                            case 0x0C: omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, starIndex | 3, starPreset, behaviorArg); break;
                            case 0x0D: omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, starIndex | 4, starPreset, behaviorArg); break;
                            case 0x0E: omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, starIndex | 5, starPreset, behaviorArg); break;
                        }
                    } break;

                    // Toads
                    case OMM_STAR_TYPE_TOAD: {
                        switch ((behaviorArg >> 24) & 0xFF) {
                            case TOAD_STAR_1_DIALOG: omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, 0, starPreset, behaviorArg); break;
                            case TOAD_STAR_2_DIALOG: omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, 1, starPreset, behaviorArg); break;
                            case TOAD_STAR_3_DIALOG: omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, 2, starPreset, behaviorArg); break;
                        }
                    } break;

                    // Mips
                    case OMM_STAR_TYPE_MIPS: {
                        for (s32 i = 0; i != OMM_NUM_MIPS_STARS; ++i) {
                            omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, 3 + i, starPreset, 0);
                        }
                    } break;

                    // Other types
                    default: {
                        u8 starIndex = (u8) (behaviorArg >> 24);
                        omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, starIndex, starPreset, behaviorArg);
                    } break;
                }
            }
        } else if (type == LEVEL_CMD_MACRO_OBJECTS) {
            MacroObject *data = level_cmd_get(cmd, MacroObject *, 4);
            for (; *data != MACRO_OBJECT_END(); data += 5) {
                s32 presetId = (s32) ((data[0] & 0x1FF) - 0x1F);
                u8 starIndex = 0xFF;
                switch (presetId) {
                    case macro_box_star_1: starIndex = 0; break;
                    case macro_box_star_2: starIndex = 1; break;
                    case macro_box_star_3: starIndex = 2; break;
                    case macro_box_star_4: starIndex = 3; break;
                    case macro_box_star_5: starIndex = 4; break;
                    case macro_box_star_6: starIndex = 5; break;
                }
                if (starIndex != 0xFF) {
                    const BehaviorScript *behavior = MacroObjectPresets[presetId].behavior;
                    s32 presetParams = MacroObjectPresets[presetId].param;
                    s32 objParams = (data[4] & 0xFF00) + (presetParams & 0x00FF);
                    s32 behParams = ((objParams & 0x00FF) << 16) + (objParams & 0xFF00);
                    const OmmStarPreset *starPreset = omm_stars_get_preset(behavior, behParams);
                    if (starPreset) {
                        omm_stars_add_data(sOmmStarsPreprocess->levelNum, sOmmStarsPreprocess->areaIndex, starIndex, starPreset, behParams);
                    }
                }
            }
        }
    }
    return LEVEL_SCRIPT_CONTINUE;
}

static void omm_stars_load_data(s32 levelNum, s32 areaIndex) {
    if (!sOmmStarsData[levelNum][areaIndex]) {
        sOmmStarsData[levelNum][areaIndex] = mem_new(OmmStarData *, OMM_NUM_STARS_MAX_PER_COURSE);
        if (omm_level_get_script(levelNum)) {

            // Level Stars
            sOmmStarsPreprocess->levelNum = levelNum;
            sOmmStarsPreprocess->areaIndex = areaIndex;
            sOmmStarsPreprocess->currAreaIndex = 0;
            level_script_preprocess(omm_level_get_script(levelNum), omm_stars_preprocess_level_cmd);

            // 100 Coins Star
            if (COURSE_IS_MAIN_COURSE(omm_level_get_course(levelNum))) {
                omm_stars_add_data(levelNum, areaIndex, 6, omm_stars_get_preset(bhvStar, 0), 0x06000000);
            }

            // Slide Secret Star
            if (levelNum == OMM_LEVEL_SLIDE) {
                omm_stars_add_data(levelNum, areaIndex, 1, omm_stars_get_preset(bhvStar, 0), 0x01000000);
            }
        }
    }
}

static u8 omm_stars_get_flags_per_area(s32 levelNum, s32 areaIndex) {
    omm_stars_load_data(levelNum, areaIndex);
    u8 starFlags = 0;
    for (s32 i = 0; i != OMM_NUM_STARS_MAX_PER_COURSE; ++i) {
        if (sOmmStarsData[levelNum][areaIndex][i]) {
            starFlags |= (1 << i);
        }
    }
    return starFlags;
}

static u32 omm_stars_get_color_per_course(s32 courseNum, MODE_INDEX s32 modeIndex) {
    static u32 sOmmStarsColors[OMM_STAR_COLOR_COUNT];
    OMM_DO_ONCE {
        for (s32 i = 0; i != OMM_STAR_COLOR_COUNT; ++i) {
            sys_path_t filename;
            str_cat(filename, sizeof(filename), FS_TEXTUREDIR "/", OMM_TEXTURE_STAR_BODY_[OMM_STAR_COLOR_[i]], ".png");
            s32 w, h;
            u8 *p = fs_load_png(filename, &w, &h);
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
    return sOmmStarsColors[clamp_s(courseNum, 0, 16) + OMM_STAR_COLOR_OFFSET(modeIndex)];
}

//
// Stars
//

u8 sOmmStarsFlags = 0;

OMM_ROUTINE_LEVEL_ENTRY(omm_stars_init) {
    sOmmStarsFlags = 0;
}

u8 omm_stars_get_flags() {
    return sOmmStarsFlags;
}

u8 omm_stars_get_level_flags(s32 levelNum, MODE_INDEX s32 modeIndex) {
#if OMM_GAME_IS_SM74
    return omm_stars_get_flags_per_area(levelNum, modeIndex + 1);
#else
    return omm_stars_get_flags_per_area(levelNum, 0) |
           omm_stars_get_flags_per_area(levelNum, 1) |
           omm_stars_get_flags_per_area(levelNum, 2) |
           omm_stars_get_flags_per_area(levelNum, 3) |
           omm_stars_get_flags_per_area(levelNum, 4) |
           omm_stars_get_flags_per_area(levelNum, 5) |
           omm_stars_get_flags_per_area(levelNum, 6) |
           omm_stars_get_flags_per_area(levelNum, 7);
#endif
}

u8 omm_stars_get_course_flags(s32 courseNum, s32 modeIndex) {
    return (
        (courseNum >= COURSE_BOB && courseNum <= COURSE_CAKE_END) ?
        omm_stars_get_level_flags(omm_level_from_course(courseNum), modeIndex) :
        omm_stars_get_level_flags(LEVEL_CASTLE_GROUNDS, modeIndex) |
        omm_stars_get_level_flags(LEVEL_CASTLE, modeIndex) |
        omm_stars_get_level_flags(LEVEL_CASTLE_COURTYARD, modeIndex)
    );
}

s32 omm_stars_get_total_star_count(s32 modeIndex) {
    s32 starCount = 0;
    for (s32 i = 0; i != omm_level_get_count(); ++i) {
        s32 levelNum = omm_level_get_list()[i];
        if (!OMM_LEVEL_IS_BOWSER_FIGHT(levelNum)) {
            u8 starLevelFlags = omm_stars_get_level_flags(levelNum, modeIndex);
            for (s32 i = 0; i != OMM_NUM_STARS_MAX_PER_COURSE; ++i) {
                starCount += ((starLevelFlags >> i) & 1);
            }
        }
    }
    return starCount;
}

u32 omm_stars_get_color(s32 levelNum, s32 modeIndex) {
    return omm_stars_get_color_per_course(omm_level_get_course(levelNum), modeIndex);
}

bool omm_stars_is_collected(s32 starIndex) {
    return OMM_STARS_NON_STOP && ((sOmmStarsFlags >> starIndex) & 1);
}

bool omm_stars_all_collected(s32 levelNum, s32 modeIndex) {

    // Bowser level or Castle: never collected
    if (OMM_LEVEL_IS_BOWSER_LEVEL(levelNum)) return false;
    if (OMM_LEVEL_IS_CASTLE_LEVEL(levelNum)) return false;

    // Bowser fight: always collected
    if (OMM_LEVEL_IS_BOWSER_FIGHT(levelNum)) return true;

    // No exit warp: never collected
    if (!omm_level_get_exit_warp(levelNum, modeIndex + 1)) return false;

    // No star in the level: never collected
    u8 starLevelFlags = omm_stars_get_level_flags(levelNum, modeIndex);
    if (!starLevelFlags) return false;

    // All stars collected?
    return (sOmmStarsFlags & starLevelFlags) == starLevelFlags;
}

void omm_stars_set_flags(u8 starFlags) {
    sOmmStarsFlags |= starFlags;
}

bool omm_stars_get_star_data(s32 levelNum, s32 areaIndex, s32 starIndex, const BehaviorScript **bhv, s32 *behParams) {
    omm_stars_load_data(levelNum, areaIndex);
    const OmmStarData *starData = sOmmStarsData[levelNum][areaIndex][starIndex];
    if (starData) {
        if (bhv) *bhv = starData->preset->behavior;
        if (behParams) *behParams = starData->params;
        return true;
    }
    return false;
}

//
// Update
//

static void omm_stars_unload_child_objects(struct Object *obj, const BehaviorScript *behavior) {
    struct Object *child;
    while ((child = obj_get_first_with_behavior_and_parent(obj, behavior)) != NULL) {
        unload_object(child);
    }
}

static void omm_stars_unload_objects_with_param(const BehaviorScript *behavior, s32 behParams2ndByte) {
    struct Object *obj;
    while ((obj = obj_get_first_with_behavior_and_field_s32(behavior, _FIELD(oBehParams2ndByte), behParams2ndByte)) != NULL) {
        unload_object(obj);
    }
}

static bool omm_stars_should_unload_object(struct Object *obj, const OmmStarPreset *starPreset) {
    switch (starPreset->type) {

        // Regular star
        // Unload the object
        case OMM_STAR_TYPE_DEFAULT:
        case OMM_STAR_TYPE_EXCLAMATION_BOX: {
            return true;
        } break;

        // Boss battle
        // Stop the Boss music and unload
        case OMM_STAR_TYPE_BOSS: {
            stop_background_music(SEQ_EVENT_BOSS);
            return true;
        } break;

        // Koopa the Quick
        // Unload him and his flag only if the race has not started
        case OMM_STAR_TYPE_KOOPA_THE_QUICK: {
            if (obj->oAction == KOOPA_THE_QUICK_ACT_WAIT_BEFORE_RACE) {
                omm_stars_unload_objects_with_param(bhvKoopaRaceEndpoint, 0);
                omm_stars_unload_objects_with_param(bhvKoopaFlag, 0);
                return true;
            }
        } break;

        // Unagi
        // Unload it only if it's in its hole
        case OMM_STAR_TYPE_UNAGI: {
            if (obj->oAction == 3) {
                return true;
            }
        } break;

        // Treasure chests
        // Make the treasure chests already opened
        case OMM_STAR_TYPE_TREASURE_CHEST: {
            obj->oAction = 2;
            obj->oTreasureChestUnkF4 = 0;
            obj->oTreasureChestUnkF8 = 0;
            for_each_object_with_behavior(chestBottom, bhvTreasureChestBottom) {
                if (chestBottom->parentObj == obj) {
                    chestBottom->oAction = 1;
                    for_each_object_with_behavior(chestTop, bhvTreasureChestTop) {
                        if (chestTop->parentObj == chestBottom) {
                            chestTop->oAction = 2;
                            chestTop->oFaceAnglePitch = -0x4000;
                        }
                    }
                }
            }
        } break;

        // Racing penguin
        // Unload him only if the race has not started
        case OMM_STAR_TYPE_RACING_PENGUIN: {
            if (obj->oAction == RACING_PENGUIN_ACT_WAIT_FOR_MARIO) {
                return true;
            }
        } break;

        // Tuxies' mother
        // Stop asking for her baby
        case OMM_STAR_TYPE_TUXIES_MOTHER: {
            if (obj->oAction == 0) {
                struct Object *tuxie = obj_get_first_with_behavior_and_field_s32(bhvSmallPenguin, _FIELD(oBehParams), obj->oBehParams & 0x00FF0000);
                if (tuxie) obj_set_behavior(tuxie, bhvUnused20E0);
                obj->oAction = 2;
            }
        } break;

        // Big Snowman
        // Make the Snowman big again
        // Unload snowman's bottom
        case OMM_STAR_TYPE_BIG_SNOWMAN: {
            if (obj->oAction == 0) {
                omm_stars_unload_child_objects(obj, bhvSnowmansBottom);
                spawn_object(obj, MODEL_CCM_SNOWMAN_BASE, bhvBigSnowmanWhole);
                obj->oPosY += 350;
                obj->oAction = 1;
            }
        } break;

        // Manta ray
        // Stop spawning water rings
        case OMM_STAR_TYPE_MANTA_RAY: {
            omm_stars_unload_child_objects(obj, bhvMantaRayWaterRing);
            obj->oAction = 1;
        } break;

        // Jet stream ring spawner
        // Stop spawning water rings
        case OMM_STAR_TYPE_JET_STREAM: {
            omm_stars_unload_child_objects(obj, bhvJetStreamWaterRing);
            obj->oAction = JS_RING_SPAWNER_ACT_INACTIVE;
        } break;

        // Star from Ukiki's cage
        // Unload the cage's objects and cage Ukikis
        case OMM_STAR_TYPE_UKIKI_CAGE: {
            omm_stars_unload_objects_with_param(bhvUkiki, UKIKI_CAGE);
            omm_stars_unload_objects_with_param(bhvMacroUkiki, UKIKI_CAGE);
            omm_stars_unload_child_objects(obj, bhvUkikiCageStar);
            omm_stars_unload_child_objects(obj, bhvUkikiCageChild);
            return true;
        } break;

        // Wiggler
        // Unload Wiggler's objects
        case OMM_STAR_TYPE_WIGGLER: {
            stop_background_music(SEQ_EVENT_BOSS);
            omm_stars_unload_child_objects(obj, bhvWigglerBody);
            return true;
        } break;

        // Piranha plants
        // Set the killed counter to 0
        case OMM_STAR_TYPE_FIRE_PLANTS: {
            sNumKilledFirePiranhaPlants = 0;
            return true;
        } break;

        // Toad
        // Set its dialog to the one after collecting the star
        case OMM_STAR_TYPE_TOAD: {
            switch (obj->oToadMessageDialogId) {
                case TOAD_STAR_1_DIALOG: obj->oToadMessageDialogId = TOAD_STAR_1_DIALOG_AFTER; break;
                case TOAD_STAR_2_DIALOG: obj->oToadMessageDialogId = TOAD_STAR_2_DIALOG_AFTER; break;
                case TOAD_STAR_3_DIALOG: obj->oToadMessageDialogId = TOAD_STAR_3_DIALOG_AFTER; break;
            }
        } break;

        // Mips
        // Set status to "Already spawned star"
        case OMM_STAR_TYPE_MIPS: {
            obj->oMipsStarStatus = MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR;
        } break;
    }
    return false;
}

OMM_ROUTINE_UPDATE(omm_stars_update) {
    if (gMarioObject &&                                     // Mario must be loaded
        !omm_is_main_menu() &&                              // Not in main menu
        OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE &&           // Non-Stop mode, but not during ending cutscene
        !omm_mario_is_star_dancing(gMarioState) &&          // Not star dancing
        gMarioState->action != ACT_JUMBO_STAR_CUTSCENE &&   // Not in jumbo star cutscene
        !obj_get_first_with_behavior(bhvOmmWfTransition)    // Not during WF transition
    ) {

        // Check and update for each star bit
        // Unload objects or set actions to prevent already collected stars from respawning
        omm_stars_load_data(gCurrLevelNum, gCurrAreaIndex);
        for (s32 i = 0; i != OMM_NUM_STARS_MAX_PER_COURSE; ++i) {
            const OmmStarData *starData = sOmmStarsData[gCurrLevelNum][gCurrAreaIndex][i];
            if (starData && ((sOmmStarsFlags >> i) & 1)) {
                const OmmStarPreset *starPreset = starData->preset;

                // Loop until there is no more object to unload
                // The loop must be restarted each time an object is unloaded
                for (bool done = false; !done;) {
                    done = true;
                    for_each_object_with_behavior(obj, starPreset->behavior) {
                        if (starData->params == obj->oBehParams && omm_stars_should_unload_object(obj, starPreset)) {
                            unload_object(obj);
                            done = false;
                            break;
                        }
                    }
                }
            }
        }
    }
}

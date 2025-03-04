#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/omm_constants.h"

struct MacroPreset { const BehaviorScript *behavior; s16 model; s16 param; };
extern struct MacroPreset MacroObjectPresets[];

//
// Data
//

#define STUB_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8) courseenum,
#define DEFINE_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8, _9, _10) courseenum,
s8 gLevelToCourseNumTable[] = { // index is levelIndex
#include "levels/level_defines.h"
};
#undef STUB_LEVEL
#undef DEFINE_LEVEL

s8 gCourseNumToLevelNumTable[] = { // index is courseNum
    [COURSE_NONE] = LEVEL_CASTLE,
    [COURSE_BOB] = LEVEL_BOB,
    [COURSE_WF] = LEVEL_WF,
    [COURSE_JRB] = LEVEL_JRB,
    [COURSE_CCM] = LEVEL_CCM,
    [COURSE_BBH] = LEVEL_BBH,
    [COURSE_HMC] = LEVEL_HMC,
    [COURSE_LLL] = LEVEL_LLL,
    [COURSE_SSL] = LEVEL_SSL,
    [COURSE_DDD] = LEVEL_DDD,
    [COURSE_SL] = LEVEL_SL,
    [COURSE_WDW] = LEVEL_WDW,
    [COURSE_TTM] = LEVEL_TTM,
    [COURSE_THI] = LEVEL_THI,
    [COURSE_TTC] = LEVEL_TTC,
    [COURSE_RR] = LEVEL_RR,
    [COURSE_BITDW] = LEVEL_BITDW,
    [COURSE_BITFS] = LEVEL_BITFS,
    [COURSE_BITS] = LEVEL_BITS,
    [COURSE_PSS] = LEVEL_PSS,
    [COURSE_COTMC] = LEVEL_COTMC,
    [COURSE_TOTWC] = LEVEL_TOTWC,
    [COURSE_VCUTM] = LEVEL_VCUTM,
    [COURSE_WMOTR] = LEVEL_WMOTR,
    [COURSE_SA] = LEVEL_SA,
    [COURSE_CAKE_END] = LEVEL_ENDING,
#if OMM_GAME_IS_R96X
    [COURSE_FOURTH_FLOOR] = LEVEL_FOURTH_FLOOR,
#endif
};

typedef struct {
    const LevelScript *script;
    OmmArray warps;
    u64 captures[8];
    s32 areas;
} OmmLevelData;

static struct {
    OmmLevelData data[LEVEL_COUNT];
    s32 list[LEVEL_COUNT]; // Ordered by Course Id, Castle levels (COURSE_NONE) come last
    s32 count;
    s32 levelNum; // preprocessing
    s32 areaIndex; // preprocessing
} sOmmLevel[1];

//
// Init
//

static Warp *omm_level_get_warp_data(s32 levelNum, s32 areaIndex, s32 id) {

    // Existing warp
    omm_array_for_each(sOmmLevel->data[levelNum].warps, p) {
        Warp *warp = (Warp *) p->as_ptr;
        if (warp->srcLevelNum == levelNum && warp->srcAreaIndex == areaIndex && warp->srcId == id) {
            return warp;
        }
    }

    // New warp
    Warp *warp = mem_new(Warp, 1);
    warp->srcLevelNum = levelNum;
    warp->srcAreaIndex = areaIndex;
    warp->srcId = id;
    warp->srcType = -1;
    omm_array_add(sOmmLevel->data[levelNum].warps, ptr, warp);
    return warp;
};

static s32 omm_level_preprocess_master_script(u8 type, void *cmd) {

    // Find the level scripts table
    if (type == LEVEL_CMD_JUMP_LINK) {
        sOmmLevel->areaIndex = 1;
        return LEVEL_SCRIPT_CONTINUE;
    }

    // Are we inside the level scripts table?
    if (!sOmmLevel->areaIndex) {
        return LEVEL_SCRIPT_CONTINUE;
    }

    // Register level scripts
    switch (type) {
        case LEVEL_CMD_EXECUTE: {
            const LevelScript *script = level_cmd_get(cmd, const LevelScript *, 12);
            if (sOmmLevel->levelNum >= 0 && sOmmLevel->levelNum < LEVEL_COUNT && !sOmmLevel->data[sOmmLevel->levelNum].script) {
                sOmmLevel->data[sOmmLevel->levelNum].script = script;
            }
            sOmmLevel->levelNum = -1;
        } return LEVEL_SCRIPT_RETURN;

        case LEVEL_CMD_EXIT:
        case LEVEL_CMD_SLEEP: {
        } return LEVEL_SCRIPT_STOP;

        case LEVEL_CMD_JUMP_IF: {
            sOmmLevel->levelNum = level_cmd_get(cmd, s32, 4);
            static const s32 EXCLUDED_LEVELS[] = { OMM_LEVEL_EXCLUDE_LIST };
            for (u32 i = 0; i != array_length(EXCLUDED_LEVELS); ++i) {
                if (sOmmLevel->levelNum == EXCLUDED_LEVELS[i]) {
                    sOmmLevel->levelNum = -1;
                    break;
                }
            }
        } break;
    }
    return LEVEL_SCRIPT_CONTINUE;
}

static void omm_level_register_capture(const BehaviorScript *bhv, u32 bhvParams) {
    struct Object obj[1] = {0};
    u32 bhvParams2ndByte = (bhvParams >> 16) & 0xFF;
    obj->behavior = bhv;
    obj->oBehParams = bhvParams;
    obj->oBehParams2ndByte = bhvParams2ndByte;
    sOmmLevel->data[sOmmLevel->levelNum].captures[sOmmLevel->areaIndex] |= omm_behavior_data_get_capture_type(obj);

    // Goomba triplets
    if (bhv == bhvGoombaTripletSpawner) {
        omm_level_register_capture(bhvGoomba, bhvParams);
    }

    // Koopa Shell
    if ((bhv == bhvKoopa && (bhvParams2ndByte == KOOPA_BP_NORMAL || bhvParams2ndByte == KOOPA_BP_TINY)) ||
        (bhv == bhvExclamationBox && bhvParams2ndByte == 3)) {
        omm_level_register_capture(bhvKoopaShell, bhvParams);
    }

    // Bub
    if (bhv == bhvChirpChirp || bhv == bhvChirpChirpUnused) {
        omm_level_register_capture(bhvBub, bhvParams);
    }

    // Moneybag
    if (bhv == bhvMoneybagHidden) {
        omm_level_register_capture(bhvMoneybag, bhvParams);
    }

    // Spiny
    if (bhv == bhvEnemyLakitu) {
        omm_level_register_capture(bhvSpiny, bhvParams);
    }

#if OMM_GAME_IS_SM64
    // Flaming Bobomb
    if (bhv != bhvOmmFlamingBobomb && OMM_LEVEL_IS_BOWSER_FIGHT(sOmmLevel->levelNum)) {
        omm_level_register_capture(bhvOmmFlamingBobomb, 0);
    }
#endif
}

static s32 omm_level_fill_data(u8 type, void *cmd) {
    switch (type) {
        case LEVEL_CMD_SLEEP:
        case LEVEL_CMD_SLEEP_BEFORE_EXIT: {
        } return LEVEL_SCRIPT_STOP;

        case LEVEL_CMD_AREA: {
            sOmmLevel->areaIndex = level_cmd_get(cmd, u8, 2);
            sOmmLevel->data[sOmmLevel->levelNum].areas |= (1 << sOmmLevel->areaIndex);
        } break;

        case LEVEL_CMD_OBJECT_WITH_ACTS: {
            const BehaviorScript *bhv = level_cmd_get(cmd, const BehaviorScript *, 20);
            u32 bhvParams = level_cmd_get(cmd, u32, 16);

            // Warps
            for (s32 i = 0; i != 20; ++i) {
                if (sWarpBhvSpawnTable[i] == bhv) {
                    s32 warpId = ((bhvParams >> 16) & 0xFF);
                    Warp *warp = omm_level_get_warp_data(sOmmLevel->levelNum, sOmmLevel->areaIndex, warpId);
                    if (warp->srcType == -1) {
                        warp->srcType = i;
                        warp->x = (f32) level_cmd_get(cmd, s16, 4);
                        warp->y = (f32) level_cmd_get(cmd, s16, 6);
                        warp->z = (f32) level_cmd_get(cmd, s16, 8);
                        warp->yaw = (level_cmd_get(cmd, s16, 12) * 0x8000) / 180;
                    }
                    break;
                }
            }

            // Capture
            omm_level_register_capture(bhv, bhvParams);
        } break;

        case LEVEL_CMD_WARP_NODE:
        case LEVEL_CMD_PAINTING_WARP_NODE: {
            Warp *warp = omm_level_get_warp_data(sOmmLevel->levelNum, sOmmLevel->areaIndex, level_cmd_get(cmd, u8, 2));
            if (warp->dstLevelNum == 0) {
                warp->dstLevelNum = (s32) level_cmd_get(cmd, u8, 3);
                warp->dstAreaIndex = (s32) level_cmd_get(cmd, u8, 4);
                warp->dstId = (s32) level_cmd_get(cmd, u8, 5);
            }
        } break;

        case LEVEL_CMD_MACRO_OBJECTS: {
            MacroObject *data = level_cmd_get(cmd, MacroObject *, 4);
            for (; *data != MACRO_OBJECT_END(); data += 5) {
                s32 presetId = (s32) ((data[0] & 0x1FF) - 0x1F);
                const BehaviorScript *bhv = MacroObjectPresets[presetId].behavior;
                u32 presetParams = (u32) ((u16) MacroObjectPresets[presetId].param);
                u32 objParams = (data[4] & 0xFF00) + (presetParams & 0x00FF);
                u32 bhvParams = ((objParams & 0x00FF) << 16) + (objParams & 0xFF00);

                // Capture
                omm_level_register_capture(bhv, bhvParams);
            }
        } break;
    }
    return LEVEL_SCRIPT_CONTINUE;
}

static void omm_level_init() {
    OMM_DO_ONCE {

        // Level scripts
        sOmmLevel->levelNum = -1;
        sOmmLevel->areaIndex = 0;
        level_script_preprocess(level_main_scripts_entry, omm_level_preprocess_master_script);

        // Level data
        for (sOmmLevel->levelNum = 0; sOmmLevel->levelNum != LEVEL_COUNT; ++sOmmLevel->levelNum) {
            if (sOmmLevel->data[sOmmLevel->levelNum].script) {
                level_script_preprocess(sOmmLevel->data[sOmmLevel->levelNum].script, omm_level_fill_data);
            }
        }

        // Level list ordered by course id
        for (s32 courseNum = COURSE_MIN; courseNum <= COURSE_MAX; ++courseNum) {
            for (s32 levelNum = 1; levelNum != LEVEL_COUNT; ++levelNum) {
                if (gLevelToCourseNumTable[levelNum - 1] == courseNum && sOmmLevel->data[levelNum].script) {
                    sOmmLevel->list[sOmmLevel->count++] = levelNum;
                }
            }
        }

        // Add all 3 Castle levels at the end
        sOmmLevel->list[sOmmLevel->count++] = LEVEL_CASTLE;
        sOmmLevel->list[sOmmLevel->count++] = LEVEL_CASTLE_GROUNDS;
        sOmmLevel->list[sOmmLevel->count++] = LEVEL_CASTLE_COURTYARD;
    }
}

//
// Common data
//

OMM_INLINE u8 *convert_text_and_copy(ustr_t dst, const char *str) {
    return omm_text_copy(dst, sizeof(ustr_t), omm_text_convert(str, false));
}

s32 omm_level_get_count() {
    omm_level_init();
    return sOmmLevel->count;
}

s32 *omm_level_get_list() {
    omm_level_init();
    return sOmmLevel->list;
}

s32 omm_level_get_course(s32 levelNum) {
    omm_level_init();
    return (s32) gLevelToCourseNumTable[levelNum - 1];
}

s32 omm_level_from_course(s32 courseNum) {
    omm_level_init();
    return (s32) gCourseNumToLevelNumTable[courseNum];
}

const LevelScript *omm_level_get_script(s32 levelNum) {
    omm_level_init();
    return sOmmLevel->data[levelNum].script;
}

s32 omm_level_get_areas(s32 levelNum) {
    omm_level_init();
    return sOmmLevel->data[levelNum].areas;
}

u64 omm_level_get_available_captures(s32 levelNum, s32 areaIndex) {
    omm_level_init();
    return sOmmLevel->data[levelNum].captures[areaIndex];
}

u64 omm_level_get_all_available_captures(MODE_INDEX s32 modeIndex) {
    omm_level_init();
    u64 captureFlags = 0;
    for (s32 i = 0; i != sOmmLevel->count; ++i) {
        s32 levelNum = sOmmLevel->list[i];
#if OMM_GAME_IS_SM74
        captureFlags |= sOmmLevel->data[levelNum].captures[modeIndex + 1];
#else
        for (s32 areaIndex = 0; areaIndex != 8; ++areaIndex) {
            captureFlags |= sOmmLevel->data[levelNum].captures[areaIndex];
        }
#endif
    }
    return captureFlags;
}

s32 omm_level_get_all_available_captures_count(s32 modeIndex) {
    s32 availableCaptureCount = 0;
    u64 availableCaptureFlags = omm_level_get_all_available_captures(modeIndex);
    for (u64 i = 0; i != OMM_MAX_CAPTURES; ++i) {
        availableCaptureCount += ((availableCaptureFlags & (1llu << i)) != 0);
    }
    return availableCaptureCount;
}

u8 *omm_level_get_course_name(ustr_t dst, s32 levelNum, MODE_INDEX s32 modeIndex, bool decaps, bool num) {
    omm_level_init();
    s32 courseNum = omm_level_get_course(levelNum);

    // Level name
    if (levelNum == LEVEL_BOWSER_1) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_BOWSER_1);
    } else if (levelNum == LEVEL_BOWSER_2) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_BOWSER_2);
    } else if (levelNum == LEVEL_BOWSER_3) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_BOWSER_3);
    } else if (courseNum < COURSE_BOB) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_CASTLE);
    } else if (courseNum > COURSE_CAKE_END) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_CASTLE);
    } else if (courseNum == COURSE_CAKE_END && *gCourseNameTable(modeIndex)[COURSE_CAKE_END - COURSE_BOB] == 0xFF) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_CASTLE);
    } else {
        omm_text_copy(dst, sizeof(ustr_t), gCourseNameTable(modeIndex)[courseNum - COURSE_BOB] + 3);
    }

    // Decaps
    if (decaps) {
        omm_text_decapitalize(dst);
    }

    // Course number
    if (num && (courseNum >= COURSE_BOB) && (courseNum <= COURSE_STAGES_MAX)) {
        mem_mov(dst + 5, dst, sizeof(ustr_t) - 5);
        dst[0] = ((courseNum / 10) == 0 ? 158 : (courseNum / 10));
        dst[1] = (courseNum % 10);
        dst[2] = 158;
        dst[3] = 159;
        dst[4] = 158;
    }

    return dst;
}

u8 *omm_level_get_act_name(ustr_t dst, s32 levelNum, s32 actNum, MODE_INDEX s32 modeIndex, bool decaps, bool num) {
    omm_level_init();
    s32 courseNum = omm_level_get_course(levelNum);

    // Star name
    if (courseNum < COURSE_BOB) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_ONE_SECRET_STAR);
#if OMM_GAME_IS_SMGS
    } else if (courseNum == COURSE_VCUTM) {
        switch (actNum - 1) {
            case 0x0A: convert_text_and_copy(dst, OMM_TEXT_LEVEL_VCUTM_STAR_1); break;
            case 0x0B: convert_text_and_copy(dst, OMM_TEXT_LEVEL_VCUTM_STAR_2); break;
            case 0x0C: convert_text_and_copy(dst, OMM_TEXT_LEVEL_VCUTM_STAR_3); break;
            case 0x0D: convert_text_and_copy(dst, OMM_TEXT_LEVEL_VCUTM_STAR_4); break;
            case 0x0E: convert_text_and_copy(dst, OMM_TEXT_LEVEL_VCUTM_STAR_5); break;
            case 0x0F: convert_text_and_copy(dst, OMM_TEXT_LEVEL_VCUTM_STAR_6); break;
            default:   convert_text_and_copy(dst, OMM_TEXT_EMPTY); break;
        }
#endif
    } else if (actNum < 0 || actNum > OMM_NUM_STARS_MAX_PER_COURSE || OMM_LEVEL_IS_BOWSER_FIGHT(levelNum)) { // Fake stars, invalid stars, Bowser fights
        convert_text_and_copy(dst, OMM_TEXT_EMPTY);
#if OMM_GAME_IS_SM64
    } else if (levelNum == LEVEL_PSS) {
        convert_text_and_copy(dst, actNum == 1 ? OMM_TEXT_LEVEL_PSS_STAR_1 : OMM_TEXT_LEVEL_PSS_STAR_2);
    } else if (courseNum > COURSE_STAGES_MAX) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_RED_COINS_STAR);
#else
#if OMM_GAME_IS_SMSR
    } else if (actNum == 0) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_STAR_REPLICA);
#endif
    } else if (courseNum > COURSE_STAGES_MAX) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_STAR__);
        dst[5] = (u8) actNum;
#endif
    } else if (actNum == OMM_NUM_STARS_MAX_PER_COURSE) {
        convert_text_and_copy(dst, OMM_TEXT_LEVEL_100_COINS_STAR);
    } else {
        omm_text_copy(dst, sizeof(ustr_t), gActNameTable(modeIndex)[(courseNum - COURSE_BOB) * OMM_NUM_ACTS_MAX_PER_COURSE + (actNum - 1)]);
    }

    // Decaps
    if (decaps) {
        omm_text_decapitalize(dst);
    }

    // Star number
    if (num && (courseNum >= COURSE_BOB) && (courseNum <= COURSE_STAGES_MAX)) {
        mem_mov(dst + 5, dst, sizeof(ustr_t) - 5);
        dst[0] = ((actNum / 10) == 0 ? 158 : (actNum / 10));
        dst[1] = (actNum % 10);
        dst[2] = 158;
        dst[3] = 159;
        dst[4] = 158;
    }

    return dst;
}

//
// Warps
//

Warp *omm_level_get_warp(s32 levelNum, s32 areaIndex, s32 id) {
    omm_level_init();
    omm_array_for_each(sOmmLevel->data[levelNum].warps, p) {
        Warp *warp = (Warp *) p->as_ptr;
        if (warp->srcLevelNum == levelNum && warp->srcAreaIndex == areaIndex && warp->srcId == id) {
            return warp;
        }
    }
    return NULL;
}

Warp *omm_level_get_entry_warp(s32 levelNum, s32 areaIndex) {
    omm_level_init();
    return omm_level_get_warp(levelNum, areaIndex, OMM_LEVEL_ENTRY_WARP(levelNum));
}

Warp *omm_level_get_exit_warp(s32 levelNum, s32 areaIndex) {
    omm_level_init();
    return omm_level_get_warp(levelNum, areaIndex, WARP_NODE_STAR_EXIT);
}

Warp *omm_level_get_death_warp(s32 levelNum, s32 areaIndex) {
    omm_level_init();
    Warp *warp = omm_level_get_warp(levelNum, areaIndex, WARP_NODE_DEATH);
    if (!warp) warp = omm_level_get_warp(levelNum, areaIndex, WARP_NODE_WARP_FLOOR);
    return warp;
}

bool omm_level_can_warp(s32 levelNum) {
    omm_level_init();
    return omm_level_get_course(levelNum) != COURSE_NONE;
}

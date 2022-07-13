#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Data
//

#define STUB_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8) courseenum,
#define DEFINE_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8, _9, _10) courseenum,
s8 gLevelToCourseNumTable[] = {
#include "levels/level_defines.h"
};
#undef STUB_LEVEL
#undef DEFINE_LEVEL

typedef struct {
    s16 area; // 0
    s16 id; // 1
    s16 type; // 2
    s16 x; // 3
    s16 y; // 4
    s16 z; // 5
    s16 yaw; // 6
    s16 destLevel; // 7
    s16 destArea; // 8
    s16 destId; // 9
} OmmWarpData;

typedef struct {
    const LevelScript *script;
    OmmWarpData warps[0x100];
    s32 warpCount;
    s32 areas;
    OmmArray redCoins[8];
} OmmLevelData;

static OmmLevelData sOmmLevelData[LEVEL_COUNT] = { 0 };
static s32 sOmmLevelList[LEVEL_COUNT] = { 0 }; // Ordered by Course Id, COURSE_NONE excluded
static s32 sOmmLevelCount = 0;
static s32 sCurrentLevelNum;

//
// Init
//

static OmmWarpData *omm_level_get_warp_data(s32 level, u8 area, u8 id) {

    // Existing warp
    for (s32 i = 0; i != sOmmLevelData[level].warpCount; ++i) {
        if (sOmmLevelData[level].warps[i].area == area &&
            sOmmLevelData[level].warps[i].id == id) {
            return &sOmmLevelData[level].warps[i];
        }
    }

    // New warp
    OmmWarpData *warp = &sOmmLevelData[level].warps[sOmmLevelData[level].warpCount++];
    if (warp->area == 0) {
        warp->area = area;
        warp->id = id;
        warp->type = -1;
        warp->x = 0;
        warp->y = 0;
        warp->z = 0;
        warp->yaw = 0;
        warp->destLevel = 0;
        warp->destArea = 0;
        warp->destId = 0;
    }
    return warp;
};

static s32 omm_level_preprocess_master_script(u8 type, void *cmd) {
    static bool sScriptExecLevelTable = false;
    static s32 sLevelNum = -1;
    if (sScriptExecLevelTable) {
        if (type == LEVEL_CMD_EXECUTE) {
            const LevelScript *script = level_cmd_get(cmd, const LevelScript *, 12);
            if (sLevelNum >= 0 && sLevelNum < LEVEL_COUNT && !sOmmLevelData[sLevelNum].script) {
                sOmmLevelData[sLevelNum].script = script;
            }
            sLevelNum = -1;
            return LEVEL_SCRIPT_RETURN;
        }
        if (type == LEVEL_CMD_EXIT || type == LEVEL_CMD_SLEEP) {
            return LEVEL_SCRIPT_STOP;
        }
        if (type == LEVEL_CMD_JUMP_IF) {
            sLevelNum = level_cmd_get(cmd, s32, 4);
        }
    } else if (type == LEVEL_CMD_JUMP_LINK) {
        sScriptExecLevelTable = true;
    }
    return LEVEL_SCRIPT_CONTINUE;
}

static s32 omm_level_fill_warp_data(u8 type, void *cmd) {
    static u8 sCurrentAreaIndex = 0;
    switch (type) {
        case LEVEL_CMD_SLEEP:
        case LEVEL_CMD_SLEEP_BEFORE_EXIT: {
        } return LEVEL_SCRIPT_STOP;

        case LEVEL_CMD_AREA: {
            sCurrentAreaIndex = level_cmd_get(cmd, u8, 2);
            sOmmLevelData[sCurrentLevelNum].areas |= (1 << sCurrentAreaIndex);
        } break;

        case LEVEL_CMD_OBJECT_WITH_ACTS: {
            const BehaviorScript *bhv = level_cmd_get(cmd, const BehaviorScript *, 20);

            // Red coin
            if (bhv == bhvRedCoin) {
                if (omm_array_find(sOmmLevelData[sCurrentLevelNum].redCoins[sCurrentAreaIndex], ptr, cmd) == -1) {
                    omm_array_add(sOmmLevelData[sCurrentLevelNum].redCoins[sCurrentAreaIndex], ptr, cmd);
                }
            }

            // Warps
            for (s32 i = 0; i != 20; ++i) {
                if (sWarpBhvSpawnTable[i] == bhv) {
                    OmmWarpData *warp = omm_level_get_warp_data(sCurrentLevelNum, sCurrentAreaIndex, ((level_cmd_get(cmd, u32, 16) >> 16) & 0xFF));
                    if (warp->type == -1) {
                        warp->type = i;
                        warp->x = level_cmd_get(cmd, s16, 4);
                        warp->y = level_cmd_get(cmd, s16, 6);
                        warp->z = level_cmd_get(cmd, s16, 8);
                        warp->yaw = (level_cmd_get(cmd, s16, 12) * 0x8000) / 180;
                    }
                    break;
                }
            }
        } break;

        case LEVEL_CMD_WARP_NODE:
        case LEVEL_CMD_PAINTING_WARP_NODE: {
            OmmWarpData *warp = omm_level_get_warp_data(sCurrentLevelNum, sCurrentAreaIndex, level_cmd_get(cmd, u8, 2));
            if (warp->destLevel == 0) {
                warp->destLevel = level_cmd_get(cmd, u8, 3);
                warp->destArea = level_cmd_get(cmd, u8, 4);
                warp->destId = level_cmd_get(cmd, u8, 5);
            }
        } break;

        case LEVEL_CMD_MACRO_OBJECTS: {
            MacroObject *data = level_cmd_get(cmd, MacroObject *, 4);
            for (; *data != MACRO_OBJECT_END(); data += 5) {
                s32 presetId = (s32) ((data[0] & 0x1FF) - 0x1F);

                // Red coin
                if (presetId == macro_red_coin) {
                    if (omm_array_find(sOmmLevelData[sCurrentLevelNum].redCoins[sCurrentAreaIndex], ptr, data) == -1) {
                        omm_array_add(sOmmLevelData[sCurrentLevelNum].redCoins[sCurrentAreaIndex], ptr, data);
                    }
                }
            }
        } break;
    }
    return LEVEL_SCRIPT_CONTINUE;
}

static void omm_level_init() {
    static bool sInited = false;
    if (OMM_UNLIKELY(!sInited)) {

        // Level scripts
        level_script_preprocess(level_main_scripts_entry, omm_level_preprocess_master_script);

        // Level warps
        for (sCurrentLevelNum = 0; sCurrentLevelNum != LEVEL_COUNT; ++sCurrentLevelNum) {
            if (sOmmLevelData[sCurrentLevelNum].script) {
                level_script_preprocess(sOmmLevelData[sCurrentLevelNum].script, omm_level_fill_warp_data);
            }
        }

        // Level list ordered by course id
        for (s32 i = COURSE_MIN; i <= COURSE_MAX; ++i) {
            if (i == COURSE_CAKE_END) continue;
            for (s32 j = 1; j != LEVEL_COUNT; ++j) {
                if (gLevelToCourseNumTable[j - 1] == i) {
                    sOmmLevelList[sOmmLevelCount++] = j;
                }
            }
        }

        // Done
        sInited = true;
    }
}

//
// Common data
//

OMM_INLINE void convert_text_and_set_buffer(u8 *buffer, const char *str) {
    u8 *str64 = omm_text_convert(str, false);
    OMM_MEMCPY(buffer, str64, omm_text_length(str64) + 1);
}

s32 omm_level_get_count() {
    omm_level_init();
    return sOmmLevelCount;
}

s32 *omm_level_get_list() {
    omm_level_init();
    return sOmmLevelList;
}

s32 omm_level_get_course(s32 level) {
    omm_level_init();
    return (s32) gLevelToCourseNumTable[level - 1];
}

const LevelScript *omm_level_get_script(s32 level) {
    omm_level_init();
    return sOmmLevelData[level].script;
}

s32 omm_level_get_areas(s32 level) {
    omm_level_init();
    return sOmmLevelData[level].areas;
}

s32 omm_level_get_num_red_coins(s32 level, s32 area) {
    omm_level_init();
    return omm_array_count(sOmmLevelData[level].redCoins[area]);
}

u8 *omm_level_get_name(s32 level, bool decaps, bool num) {
    omm_level_init();
    static u8 sBuffer[256];
    OMM_MEMSET(sBuffer, 0xFF, 256);
    s32 course = omm_level_get_course(level);

    // Level name
    if (level == LEVEL_BOWSER_1) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_BOWSER_1);
    } else if (level == LEVEL_BOWSER_2) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_BOWSER_2);
    } else if (level == LEVEL_BOWSER_3) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_BOWSER_3);
    } else if (course < COURSE_BOB) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_CASTLE);
    } else if (course >= COURSE_CAKE_END) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_CASTLE);
    } else {
#if OMM_GAME_IS_SM74
        const u8 *courseName = (sm74_mode__omm_level_get_name == 2 ?
                                (const u8 **) seg2_course_name_table_EE :
                                (const u8 **) seg2_course_name_table)[course - COURSE_BOB] + 3;
#else
        const u8 *courseName = ((const u8 **) seg2_course_name_table)[course - COURSE_BOB] + 3;
#endif
        OMM_MEMCPY(sBuffer, courseName, omm_text_length(courseName));
    }

    // Decaps
    if (decaps) {
        omm_text_decapitalize(sBuffer);
    }

    // Course number
    if (num && (course >= COURSE_BOB) && (course <= COURSE_STAGES_MAX)) {
        OMM_MEMMOV(sBuffer + 5, sBuffer, omm_text_length(sBuffer));
        sBuffer[0] = ((course / 10) == 0 ? 158 : (course / 10));
        sBuffer[1] = (course % 10);
        sBuffer[2] = 158;
        sBuffer[3] = 159;
        sBuffer[4] = 158;
    }

    return sBuffer;
}

u8 *omm_level_get_act_name(s32 level, s32 act, bool decaps, bool num) {
    omm_level_init();
    static u8 sBuffer[256];
    OMM_MEMSET(sBuffer, 0xFF, 256);
    s32 course = omm_level_get_course(level);

    // Star name
    if (course < COURSE_BOB) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_ONE_SECRET_STAR);
#if OMM_GAME_IS_SM64
    } else if (level == LEVEL_BITDW) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_RED_COINS_STAR);
    } else if (level == LEVEL_BITFS) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_RED_COINS_STAR);
    } else if (level == LEVEL_BITS) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_RED_COINS_STAR);
    } else if (course > COURSE_STAGES_MAX) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_EMPTY);
#else
#if OMM_GAME_IS_SMSR
    } else if (act == 0) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_STAR_REPLICA);
#endif
    } else if (course > COURSE_STAGES_MAX) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_STAR__);
        sBuffer[5] = act;
#endif
    } else if (act >= 7) {
        convert_text_and_set_buffer(sBuffer, OMM_TEXT_LEVEL_100_COINS_STAR);
    } else {
#if OMM_GAME_IS_SM74
        const u8 *actName = (sm74_mode__omm_level_get_act_name == 2 ?
                             (const u8 **) seg2_act_name_table_EE :
                             (const u8 **) seg2_act_name_table)[(course - COURSE_BOB) * 6 + (act - 1)];
#else
        const u8 *actName = ((const u8 **) seg2_act_name_table)[(course - COURSE_BOB) * 6 + (act - 1)];
#endif
        OMM_MEMCPY(sBuffer, actName, omm_text_length(actName));
    }

    // Decaps
    if (decaps) {
        omm_text_decapitalize(sBuffer);
    }

    // Star number
    if (num && (course >= COURSE_BOB) && (course <= COURSE_STAGES_MAX)) {
        OMM_MEMMOV(sBuffer + 5, sBuffer, omm_text_length(sBuffer));
        sBuffer[0] = ((act / 10) == 0 ? 158 : (act / 10));
        sBuffer[1] = (act % 10);
        sBuffer[2] = 158;
        sBuffer[3] = 159;
        sBuffer[4] = 158;
    }

    return sBuffer;
}

//
// Level Script Preprocessing
// By default,
// - Ifs are always true
// - Skips are always false
// - Loops break after the first loop
//


//
// Warps
//

s16 *omm_level_get_warp(s32 level, s32 area, u8 id) {
    omm_level_init();
    for (s32 i = 0; i != 0x100; ++i) {
        if (sOmmLevelData[level].warps[i].area == area &&
            sOmmLevelData[level].warps[i].id == id) {
            return (s16 *) &sOmmLevelData[level].warps[i];
        }
    }
    return NULL;
}

s16 *omm_level_get_entry_warp(s32 level, s32 area) {
    omm_level_init();
#if OMM_GAME_IS_SM64
    if (level == LEVEL_TTM && area > 2) return NULL;
#endif
    return omm_level_get_warp(level, area, OMM_LEVEL_ENTRY_WARP(level));
}

s16 *omm_level_get_exit_warp(s32 level, s32 area) {
    omm_level_init();
    return omm_level_get_warp(level, area, 0xF0);
}

s16 *omm_level_get_death_warp(s32 level, s32 area) {
    omm_level_init();
    s16 *warp = omm_level_get_warp(level, area, 0xF1);
    if (!warp) warp = omm_level_get_warp(level, area, 0xF3);
    return warp;
}

bool omm_level_can_warp(s32 level) {
    omm_level_init();
    return omm_level_get_course(level) != COURSE_NONE;
}

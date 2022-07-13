#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Save data
//

#define OMM_SSS_MODE_UNLOCKED       0
#define OMM_SSS_TIMER_STARTED       1
#define OMM_SSS_STARS               2
#define OMM_SSS_BOWSER_4_UNLOCKED   31
#define OMM_SSS_GRAND_STAR          32
#define OMM_SSS_MODE_COMPLETED      33
#define OMM_SSS_COUNT               34
#define OMM_SSS_TIMER_MAX           10799970 // 99h 59m 59s

#define CHECK_MODE(ret)             if (OMM_UNLIKELY(mode < 1 || mode >= OMM_SSM_COUNT)) { return ret; }
#define CHECK_INDEX_29(ret)         if (OMM_UNLIKELY(index < 0 || index >= 29)) { return ret; }
#define CHECK_INDEX_30(ret)         if (OMM_UNLIKELY(index < 0 || index >= 30)) { return ret; }
#define CHECK_GAME(ret)             if (gOmmSSD[mode][index].game != OMM_GAME_TYPE) { return ret; }

static bool sOmmSSSFlags[OMM_SSM_COUNT][OMM_SSS_COUNT];
static s32  sOmmSSSTimer[OMM_SSM_COUNT];

static s32 i2c(s32 input, bool shuffle, bool c2i) {
    static const char sDefaultTable[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static char sRandomTable[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    if (shuffle) {
        for (s32 i = 0; i != 62; ++i) {
            s32 j = rand() % 62;
            char c = sRandomTable[i];
            sRandomTable[i] = sRandomTable[j];
            sRandomTable[j] = c;
        }
    } else {
        OMM_MEMCPY(sRandomTable, sDefaultTable, 62);
    }
    if (c2i) {
        for (s32 i = 0; i != 62; ++i) {
            if (sRandomTable[i] == (char) input) {
                return i;
            }
        }
        return 0;
    }
    return (s32) sRandomTable[input];
}

static s32 read_chars(const char **data, s32 count, bool shuffle) {
    s32 output = 0;
    for (s32 i = 0, j = 1; i != count; ++i, j *= 62) {
        s32 value = i2c((*data)[i], shuffle, true);
        output += value * j;
    }
    (*data) += count;
    return output;
}

#define write(...) { *buffer += sprintf(*buffer, __VA_ARGS__); }
static void write_chars(char **buffer, s32 value, s32 count, bool shuffle) {
    for (s32 i = 0; i != count; ++i) {
        char c = i2c(value % 62, shuffle, false);
        write("%c", c);
        value /= 62;
    }
}

//
// Sparkly Stars Data
//

bool omm_ssd_is_available(s32 mode) {
    return ((OMM_SSX_BLOCK_AVAILABLE >> mode) & 1);
}

bool omm_ssd_is_unlocked(s32 mode) {
    CHECK_MODE(false);
    return sOmmSSSFlags[mode][OMM_SSS_MODE_UNLOCKED];
}

bool omm_ssd_is_selectible(s32 mode) {
    return mode == OMM_SSM_DISABLED || (
           omm_ssd_is_unlocked(mode) &&
           omm_ssd_is_available(mode));
}

bool omm_ssd_is_selected(s32 mode) {
    return gOmmSSM == mode;
}

bool omm_ssd_is_completed(s32 mode) {
    CHECK_MODE(false);
    return sOmmSSSFlags[mode][OMM_SSS_MODE_COMPLETED];
}

bool omm_ssd_is_timer_started(s32 mode) {
    CHECK_MODE(false);
    return sOmmSSSFlags[mode][OMM_SSS_TIMER_STARTED];
}

bool omm_ssd_is_star_collected(s32 mode, s32 index) {
    CHECK_MODE(false);
    CHECK_INDEX_29(false);
    return sOmmSSSFlags[mode][OMM_SSS_STARS + index];
}

bool omm_ssd_is_grand_star_collected(s32 mode) {
    CHECK_MODE(false);
    return sOmmSSSFlags[mode][OMM_SSS_GRAND_STAR];
}

bool omm_ssd_is_bowser_4_unlocked(s32 mode) {
    CHECK_MODE(false);
    return sOmmSSSFlags[mode][OMM_SSS_BOWSER_4_UNLOCKED];
}

bool omm_ssd_is_bowser_4() {
#if OMM_GAME_IS_SM64
    return gCurrLevelNum == LEVEL_GROUNDS && gCurrAreaIndex == 2;
#else
    return false;
#endif
}

s32 omm_ssd_get_timer(s32 mode) {
    CHECK_MODE(0);
    return sOmmSSSTimer[mode];
}

s32 omm_ssd_get_star_index(s32 mode, s32 level, s32 area) {
    CHECK_MODE(-1);
    for (s32 i = 0; i != 29; ++i) { // Stop at 29, don't return the Bowser 4 index
        if (gOmmSSD[mode][i].game == OMM_GAME_TYPE &&
            gOmmSSD[mode][i].level == level && (
            gOmmSSD[mode][i].area == 0 ||
            gOmmSSD[mode][i].area == area)) {
            return i;
        }
    }
    return -1;
}

s32 omm_ssd_get_star_level(s32 mode, s32 index) {
    CHECK_MODE(LEVEL_NONE);
    CHECK_INDEX_30(LEVEL_NONE);
    CHECK_GAME(LEVEL_NONE);
    return gOmmSSD[mode][index].level;
}

s32 omm_ssd_get_star_area(s32 mode, s32 index) {
    CHECK_MODE(0);
    CHECK_INDEX_30(0);
    CHECK_GAME(0);
    return gOmmSSD[mode][index].area;
}

u8 *omm_ssd_get_star_name(s32 mode, s32 index) {
    CHECK_MODE(NULL);
    CHECK_INDEX_30(NULL);
    CHECK_GAME(NULL);
    const u8 *starNames = omm_get_dialog_entry(NULL, OMM_DIALOG_SPARKLY_STAR_NAMES)->str;

    // Name start
    index += 30 * (mode - 1);
    const u8 *nameStart = starNames;
    for (s32 i = 0; i != index; ++i) { while (*(nameStart++) != DIALOG_CHAR_NEWLINE); }

    // Name end
    const u8 *nameEnd = nameStart;
    while (*(++nameEnd) != DIALOG_CHAR_NEWLINE);

    // Copy
    s32 length = (s32) (nameEnd - nameStart);
    static u8 sStarName[0x100];
    OMM_MEMSET(sStarName, 0xFF, sizeof(sStarName));
    OMM_MEMCPY(sStarName, nameStart, min_s(sizeof(sStarName) - 1, length));
    return sStarName;
}

u8 *omm_ssd_get_star_level_name(s32 mode, s32 index) {
    CHECK_MODE(NULL);
    CHECK_INDEX_30(NULL);
    s32 level = gOmmSSD[mode][index].level;
    s32 area = gOmmSSD[mode][index].area;
    s32 game = gOmmSSD[mode][index].game;
    u8 *name = omm_text_convert(OMM_TEXT_LEVEL_UNKNOWN, false);
    if (game == OMM_GAME_TYPE) {
        switch (level * 8 + area) {
#if OMM_GAME_IS_SM64
            case LEVEL_CASTLE  * 8 + 1: name = omm_text_convert(OMM_TEXT_LEVEL_CASTLE_INSIDE, false); break;
            case LEVEL_CASTLE  * 8 + 2: name = omm_text_convert(OMM_TEXT_LEVEL_CASTLE_UPSTAIRS, false); break;
            case LEVEL_CASTLE  * 8 + 3: name = omm_text_convert(OMM_TEXT_LEVEL_CASTLE_BASEMENT, false); break;
            case LEVEL_COURT   * 8 + 1: name = omm_text_convert(OMM_TEXT_LEVEL_CASTLE_COURTYARD, false); break;
            case LEVEL_GROUNDS * 8 + 1: name = omm_text_convert(OMM_TEXT_LEVEL_CASTLE_GROUNDS, false); break;
#endif
            case LEVEL_GROUNDS * 8 + 2: name = omm_text_convert(omm_ssd_is_grand_star_collected(mode) ? OMM_TEXT_LEVEL_BOWSER_4 : OMM_TEXT_LEVEL_UNKNOWN, false); break;
            default:                    name = omm_level_get_name(level, false, false); break;
        }
    }
    static u8 sStarLevelName[0x100];
    OMM_MEMSET(sStarLevelName, 0xFF, sizeof(sStarLevelName));
    OMM_MEMCPY(sStarLevelName, name, min_s(sizeof(sStarLevelName) - 1, omm_text_length(name)));
    return sStarLevelName;
}

s32 omm_ssd_get_star_count(s32 mode) {
    CHECK_MODE(0);
    s32 count = 0;
    for (s32 i = 0; i != 29; ++i) {
        count += omm_ssd_is_star_collected(mode, i);
    }
    return count + omm_ssd_is_grand_star_collected(mode);
}

void omm_ssd_disable() {
    gOmmSSM = OMM_SSM_DISABLED;
}

void omm_ssd_set_mode(s32 mode) {
    if (OMM_SPARKLY_STARS_MODE != (u32) mode) {
        OMM_SPARKLY_STARS_MODE = (u32) mode;
        gSaveFileModified = true;
    }
}

void omm_ssd_unlock_mode(s32 mode) {
    CHECK_MODE();
    sOmmSSSFlags[mode][OMM_SSS_MODE_UNLOCKED] = true;
    gSaveFileModified = true;
}

void omm_ssd_start_timer(s32 mode) {
    CHECK_MODE();
    sOmmSSSFlags[mode][OMM_SSS_TIMER_STARTED] = true;
    sOmmSSSTimer[mode] = 0;
    gSaveFileModified = true;
}

void omm_ssd_collect_star(s32 mode, s32 index) {
    CHECK_MODE();
    CHECK_INDEX_29();
    sOmmSSSFlags[mode][OMM_SSS_STARS + index] = true;
    gSaveFileModified = true;
}

void omm_ssd_collect_grand_star(s32 mode) {
    CHECK_MODE();
    sOmmSSSFlags[mode][OMM_SSS_GRAND_STAR] = true;
    gSaveFileModified = true;
}

void omm_ssd_unlock_bowser_4(s32 mode) {
    CHECK_MODE();
    sOmmSSSFlags[mode][OMM_SSS_BOWSER_4_UNLOCKED] = true;
    gSaveFileModified = true;
}

void omm_ssd_clear_mode(s32 mode) {
    CHECK_MODE();
    OMM_MEMSET(sOmmSSSFlags[mode], 0, OMM_SSS_MODE_COMPLETED); // Do NOT clear the 'completed' flag
    sOmmSSSTimer[mode] = 0;
    gSaveFileModified = true;
}

bool omm_ssd_read(const char **tokens) {
    if (strcmp(tokens[0], "sparkly_stars") == 0) {
        const char *data = tokens[1];

        // Seed
        s32 seed = read_chars(&data, 2, false);
        srand(seed);

        // Data 1
        bool flags1[OMM_SSS_COUNT];
        for (s32 i = 0; i != OMM_SSS_COUNT; ++i) {
            flags1[i] = (read_chars(&data, 1, true) == 1);
        }
        s32 timer1 = read_chars(&data, 4, true);
        s32 mode1 = read_chars(&data, 1, true);

        // Data 2
        bool flags2[OMM_SSS_COUNT];
        for (s32 i = 0; i != OMM_SSS_COUNT; ++i) {
            flags2[i] = (read_chars(&data, 1, true) == 1);
        }
        s32 timer2 = read_chars(&data, 4, true);
        s32 mode2 = read_chars(&data, 1, true);

        // Check modes
        if (mode1 == mode2 && mode1 >= OMM_SSM_NORMAL && mode1 < OMM_SSM_COUNT) {
        
            // Check flags
            if (OMM_MEMCMP(flags1, flags2, sizeof(flags1))) {
                OMM_MEMCPY(sOmmSSSFlags[mode1], flags1, sizeof(sOmmSSSFlags[mode1]));
            } else {
                OMM_MEMSET(sOmmSSSFlags[mode1], 0, sizeof(sOmmSSSFlags[mode1]));
            }
        
            // Check timers
            if (timer1 == timer2) {
                sOmmSSSTimer[mode1] = timer1;
            } else {
                sOmmSSSTimer[mode1] = OMM_SSS_TIMER_MAX;
            }
        }
        return true;
    }
    return false;
}

void omm_ssd_write(char **buffer) {
    srand(random_u16());
    write("[sparkly_stars]\n");
    for (s32 mode = OMM_SSM_NORMAL; mode != OMM_SSM_COUNT; ++mode) {
        if (omm_ssd_is_unlocked(mode)) {
            write("sparkly_stars = ");
            s32 seed = ((rand() % 3843) + 1);
            write_chars(buffer, seed, 2, false);
            srand(seed);
            for (s32 j = 0; j != 2; ++j) {
                for (s32 i = 0; i != OMM_SSS_COUNT; ++i) {
                    write_chars(buffer, sOmmSSSFlags[mode][i] ? 1 : 0, 1, true);
                }
                write_chars(buffer, sOmmSSSTimer[mode], 4, true);
                write_chars(buffer, mode, 1, true);
            }
            write("\n");
        }
    }
    write("\n");
}

bool omm_ssd_check_cheats(struct MarioState *m) {

    // No more problem after getting all 30 Sparkly Stars
    if (omm_ssd_is_completed(gOmmSSM)) {
        return false;
    }

    // Regular cheats
    // The option "Enable Cheats" is not counted as a cheat
    static const u8 sNoCheat[sizeof(Cheats) - sizeof(Cheats.EnableCheats)] = { 0 };
    const u8 *cheats = ((const u8 *) &Cheats) + sizeof(Cheats.EnableCheats);
    if (!OMM_MEMCMP(cheats, sNoCheat, sizeof(sNoCheat))) {
        return true;
    }

    // Debug move
    if (m->action == ACT_DEBUG_FREE_MOVE) {
        return true;
    }

    // OMM cheats
    if (OMM_CHEATS_ENABLED) {
        return true;
    }

#if OMM_GAME_IS_SM64
    // Castle grounds cheese (thanks, Petch)
    if (gCurrLevelNum == LEVEL_GROUNDS && gCurrAreaIndex == 1 && m->pos[1] > 6000.f) {
        if (m->action == ACT_OMM_POSSESSION && gOmmData->mario->capture.timer >= 20) {
            omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, false, 0);
            return true;
        }
        struct Object *o = NULL;
        o = obj_get_nearest_with_behavior_and_radius(m->marioObj, bhvRecoveryHeart, 100.f);
        o = (((m->floorHeight > (m->pos[1] -  40.f)) && (m->floor != NULL) && (m->floor->object != NULL)) ? m->floor->object : o);
        o = ((                                          (m->wall  != NULL) && (m->wall->object  != NULL)) ? m->wall->object  : o);
        o = (((m->ceilHeight  < (m->pos[1] + 200.f)) && (m->ceil  != NULL) && (m->ceil->object  != NULL)) ? m->ceil->object  : o);
        if (o) {
            obj_spawn_white_puff(o, SOUND_GENERAL_BREAK_BOX);
            obj_spawn_triangle_break_particles(o, OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_20);
            obj_mark_for_deletion(o);
            return true;
        }
    }
#endif

    // Nothing to report
    return false;
}

void omm_ssd_turn_off_cheats() {
    if (!omm_ssd_is_completed(gOmmSSM)) {
        OMM_MEMSET(&Cheats, 0, sizeof(Cheats));
        OMM_CHEATS_DISABLE;
    }
}

//
// Update
//

OMM_INLINE void omm_sparkly_update_flags(s32 mode, s32 from, s32 to, bool set) {
    CHECK_MODE();
    OMM_MEMSET(sOmmSSSFlags[mode] + from, set, 1 + to - from);
}

OMM_ROUTINE_UPDATE(omm_sparkly_update_save_data) {
    for (s32 mode = OMM_SSM_NORMAL; mode != OMM_SSM_COUNT; ++mode) {

        // If flag 0 is not set, unset flags 1~32
        if (!omm_ssd_is_unlocked(mode)) {
            omm_sparkly_update_flags(mode, OMM_SSS_TIMER_STARTED, OMM_SSS_GRAND_STAR, false);
        }

        // If flag 1 is not set, set total time to 0, and unset flags 2~32
        if (!omm_ssd_is_timer_started(mode)) {
            omm_sparkly_update_flags(mode, OMM_SSS_STARS, OMM_SSS_GRAND_STAR, false);
            sOmmSSSTimer[mode] = 0;
        }

        // If flags 2~30 are not all set, unset flags 31~32
        for (s32 i = 0; i != 29; ++i) {
            if (!omm_ssd_is_star_collected(mode, i)) {
                omm_sparkly_update_flags(mode, OMM_SSS_BOWSER_4_UNLOCKED, OMM_SSS_GRAND_STAR, false);
                break;
            }
        }

        // If flag 31 is not set, unset flag 32
        if (!omm_ssd_is_bowser_4_unlocked(mode)) {
            omm_sparkly_update_flags(mode, OMM_SSS_GRAND_STAR, OMM_SSS_GRAND_STAR, false);
        }

        // If flag 32 is set, set flag 33
        if (omm_ssd_is_grand_star_collected(mode)) {
            omm_sparkly_update_flags(mode, OMM_SSS_MODE_COMPLETED, OMM_SSS_MODE_COMPLETED, true);
        }

        // Unlock current mode and next mode if flag 33 is set, otherwise unset all flags of next mode (including flag 33)
        if (omm_ssd_is_completed(mode)) {
            omm_sparkly_update_flags(mode, OMM_SSS_MODE_UNLOCKED, OMM_SSS_MODE_UNLOCKED, true);
            omm_sparkly_update_flags(mode + 1, OMM_SSS_MODE_UNLOCKED, OMM_SSS_MODE_UNLOCKED, true);
        } else if (mode + 1 < OMM_SSM_COUNT) {
            omm_sparkly_update_flags(mode + 1, OMM_SSS_MODE_UNLOCKED, OMM_SSS_MODE_COMPLETED, false);
        }
    }

    // If Sparkly Stars mode is enabled, advance the timer
    if (OMM_SSM_IS_ENABLED) {
        s32 mode = gOmmSSM;
        if (!omm_ssd_is_grand_star_collected(mode)) {
            sOmmSSSTimer[mode] = min_s(sOmmSSSTimer[mode] + 1, OMM_SSS_TIMER_MAX);
        }
    }
}

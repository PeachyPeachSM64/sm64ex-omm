#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Save data
//

#define TIMER_MAX               10799970 // 99h 59m 59s
#define CHECK_MODE(ret)         if (OMM_UNLIKELY(mode < 1 || mode >= OMM_SPARKLY_MODE_COUNT)) { return ret; }
#define CHECK_INDEX_LAST(ret)   if (OMM_UNLIKELY(index < 0 || index >= omm_sparkly_get_bowser_4_index(mode))) { return ret; }
#define CHECK_INDEX_COUNT(ret)  if (OMM_UNLIKELY(index < 0 || index >= omm_sparkly_get_bowser_4_index(mode) + 1)) { return ret; }

typedef struct {
    bool unlocked;
    bool timerStarted;
    bool bowser4;
    bool grandStar;
    bool completed;
    bool stars[OMM_SPARKLY_STARS_MAX];
    s32 timer;
} OmmSparklySaveData;
static OmmSparklySaveData sOmmSparklySaveData[OMM_SPARKLY_MODE_COUNT];
#define sOmmSparklySave (&sOmmSparklySaveData[mode])

// REMOVE THIS IN V8.0.0
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
        omm_copy(sRandomTable, sDefaultTable, 62);
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

// REMOVE THIS IN V8.0.0
static s32 read_chars(const char **data, s32 count, bool shuffle) {
    s32 output = 0;
    for (s32 i = 0, j = 1; i != count; ++i, j *= 62) {
        s32 value = i2c((*data)[i], shuffle, true);
        output += value * j;
    }
    (*data) += count;
    return output;
}

// REMOVE THIS IN V8.0.0
static bool omm_sparkly_read_old(const char *data) {

    // Seed
    s32 seed = read_chars(&data, 2, false);
    srand(seed);

    // Data 1
    bool flags1[34];
    for (s32 i = 0; i != 34; ++i) {
        flags1[i] = (read_chars(&data, 1, true) == 1);
    }
    s32 timer1 = read_chars(&data, 4, true);
    s32 mode = read_chars(&data, 1, true);

    // Data 2
    bool flags2[34];
    for (s32 i = 0; i != 34; ++i) {
        flags2[i] = (read_chars(&data, 1, true) == 1);
    }
    s32 timer2 = read_chars(&data, 4, true);
    s32 mode2 = read_chars(&data, 1, true);

    // Check modes
    if (mode == mode2 && mode >= OMM_SPARKLY_MODE_NORMAL && mode < OMM_SPARKLY_MODE_COUNT) {

        // Check flags
        if (omm_same(flags1, flags2, sizeof(flags1))) {
            sOmmSparklySave->unlocked = flags1[0];
            sOmmSparklySave->timerStarted = flags1[1];
            sOmmSparklySave->bowser4 = flags1[31];
            sOmmSparklySave->grandStar = flags1[32];
            sOmmSparklySave->completed = flags1[33];
            omm_copy(sOmmSparklySave->stars, &flags1[2], sizeof(bool) * 29);
        } else {
            omm_zero(sOmmSparklySave, sizeof(OmmSparklySaveData));
        }
    
        // Check timers
        if (timer1 == timer2) {
            sOmmSparklySave->timer = timer1;
        } else {
            sOmmSparklySave->timer = TIMER_MAX;
        }
    }
    return true;
}

//
// Save file
//

extern u16 gRandomSeed;
static char D[] = "zE8QsdnmBh7a2FXpojt05OCrDgbvqRKNP6kZLTSGcyU3x49HVIJieuMflW1YwA";
static char R[] = "nwAGdbpWuTVNYSqEJxsU4cOQyMIv30ior9ZFD572zBlHPLmh8fagkeKX1RtCj6";
static u64 N = sizeof(D) - 1;

#define read() { char c = *(data++); for (u64 i = 0; i != N; ++i) { if (R[i] == c) { z += i * w; break; } } }
#define write(...) { *buffer += sprintf(*buffer, __VA_ARGS__); }
#define shuffle() { for (u64 i = 0; i != N; ++i) { u64 j = random_u16() % N; char c = R[i]; R[i] = R[j]; R[j] = c; } }
#define reset() { omm_copy(R, D, N); }
#define convert(x, b, s) { u64 z = x, y = (1llu << b##llu); while (y) { write("%c", R[z % N]); z /= N; y /= N; if (s) shuffle(); } }
#define invert(x, b, s) { u64 z = 0, w = 1, y = (1llu << b); while (y) { read(); w *= N; y /= N; if (s) shuffle(); } x = z; }

bool omm_sparkly_read(const char **tokens) {
    if (strcmp(tokens[0], "sparkly_stars") == 0) {
        const char *data = tokens[1];

        // Backwards compatibility
        // REMOVE THIS IN V8.0.0
        if (strlen(data) == 80) {
            return omm_sparkly_read_old(data);
        }

        // Retrieve seed
        reset();
        invert(gRandomSeed, 16, 0);

        // Read data
        OmmSparklySaveData saves[2]; s32 modes[2];
        for (s32 j = 0; j != 2; ++j) {
            omm_zero(&saves[j], sizeof(OmmSparklySaveData));

            // Flags
            u8 flags; invert(flags, 5, 1);
            saves[j].unlocked     = ((flags >> 0) & 1);
            saves[j].timerStarted = ((flags >> 1) & 1);
            saves[j].bowser4      = ((flags >> 2) & 1);
            saves[j].grandStar    = ((flags >> 3) & 1);
            saves[j].completed    = ((flags >> 4) & 1);

            // Stars
            s32 num; invert(num, 8, 1);
            while (num) {
                u8 bits; invert(bits, 5, 1);
                saves[j].stars[max_s(0, num - 1)] = ((bits >> 0) & 1);
                saves[j].stars[max_s(0, num - 2)] = ((bits >> 1) & 1);
                saves[j].stars[max_s(0, num - 3)] = ((bits >> 2) & 1);
                saves[j].stars[max_s(0, num - 4)] = ((bits >> 3) & 1);
                saves[j].stars[max_s(0, num - 5)] = ((bits >> 4) & 1);
                num = max_s(0, num - 5);
            }

            // Timer
            invert(saves[j].timer, 32, 1);

            // Mode
            invert(modes[j], 4, 1);
        }

        // Check matching
        if (modes[0] != modes[1]) {
            omm_log("[ERROR] Modes don't match!\n");
            return true;
        }
        if (!omm_same(&saves[0], &saves[1], sizeof(OmmSparklySaveData))) {
            omm_log("[ERROR] Save data don't match!\n");
            return true;
        }

        // Fill save struct
        s32 mode = modes[0];
        omm_copy(sOmmSparklySave, &saves[0], sizeof(OmmSparklySaveData));
        return true;
    }
    return false;
}

void omm_sparkly_write(char **buffer) {
    gRandomSeed = (u16) SDL_GetPerformanceCounter();
    write("[sparkly_stars]\n");
    for (s32 mode = OMM_SPARKLY_MODE_NORMAL; mode != OMM_SPARKLY_MODE_COUNT; ++mode) {
        if (omm_sparkly_is_unlocked(mode)) {
            write("sparkly_stars = ");
            reset();

            // Generate seed
            u16 seed = random_u16();
            convert(seed, 16, 0);

            // Write two copies of the data
            for (s32 j = 0; j != 2; ++j) {

                // Flags
                u8 flags = (sOmmSparklySave->unlocked     << 0) |
                           (sOmmSparklySave->timerStarted << 1) |
                           (sOmmSparklySave->bowser4      << 2) |
                           (sOmmSparklySave->grandStar    << 3) |
                           (sOmmSparklySave->completed    << 4);
                convert(flags, 5, 1);
                
                // Stars
                s32 num = OMM_SPARKLY_STARS_MAX;
                convert(num, 8, 1);
                while (num) {
                    u8 bits = (sOmmSparklySave->stars[max_s(0, num - 1)] << 0) |
                              (sOmmSparklySave->stars[max_s(0, num - 2)] << 1) |
                              (sOmmSparklySave->stars[max_s(0, num - 3)] << 2) |
                              (sOmmSparklySave->stars[max_s(0, num - 4)] << 3) |
                              (sOmmSparklySave->stars[max_s(0, num - 5)] << 4);
                    convert(bits, 5, 1);
                    num = max_s(0, num - 5);
                }

                // Timer
                convert(sOmmSparklySave->timer, 32, 1);

                // Mode
                convert(mode, 4, 1);
            }
            write("\n");
        }
    }
    write("\n");
}

#undef read
#undef write
#undef shuffle
#undef reset
#undef convert
#undef invert

//
// Save data
//

bool omm_sparkly_is_available(s32 mode) {
    return ((OMM_SPARKLY_BLOCK_AVAILABLE >> mode) & 1);
}

bool omm_sparkly_is_unlocked(s32 mode) {
    CHECK_MODE(false);
    return sOmmSparklySave->unlocked;
}

bool omm_sparkly_is_selectible(s32 mode) {
    return mode == OMM_SPARKLY_MODE_DISABLED || (
           omm_sparkly_is_unlocked(mode) &&
           omm_sparkly_is_available(mode));
}

bool omm_sparkly_is_selected(s32 mode) {
    return gOmmSparklyMode == mode;
}

bool omm_sparkly_is_completed(s32 mode) {
    CHECK_MODE(true);
    return sOmmSparklySave->completed;
}

bool omm_sparkly_is_timer_started(s32 mode) {
    CHECK_MODE(false);
    return sOmmSparklySave->timerStarted;
}

bool omm_sparkly_is_star_collected(s32 mode, s32 index) {
    CHECK_MODE(false);
    CHECK_INDEX_LAST(false);
    return sOmmSparklySave->stars[index];
}

bool omm_sparkly_is_grand_star_collected(s32 mode) {
    CHECK_MODE(false);
    return sOmmSparklySave->grandStar;
}

bool omm_sparkly_is_bowser_4_unlocked(s32 mode) {
    CHECK_MODE(false);
    return sOmmSparklySave->bowser4;
}

bool omm_sparkly_is_bowser_4_battle() {
#if OMM_GAME_IS_SM64
    return gCurrLevelNum == LEVEL_GROUNDS && gCurrAreaIndex == 2;
#else
    return false;
#endif
}

s32 omm_sparkly_get_timer(s32 mode) {
    CHECK_MODE(0);
    return sOmmSparklySave->timer;
}

void omm_sparkly_disable() {
    gOmmSparklyMode = OMM_SPARKLY_MODE_DISABLED;
}

void omm_sparkly_set_mode(s32 mode) {
    if (OMM_SPARKLY_STARS_MODE != (u32) mode) {
        OMM_SPARKLY_STARS_MODE = (u32) mode;
        gSaveFileModified = true;
    }
}

void omm_sparkly_unlock_mode(s32 mode) {
    CHECK_MODE();
    sOmmSparklySave->unlocked = true;
    gSaveFileModified = true;
}

void omm_sparkly_start_timer(s32 mode) {
    CHECK_MODE();
    sOmmSparklySave->timerStarted = true;
    sOmmSparklySave->timer = 0;
    gSaveFileModified = true;
}

void omm_sparkly_collect_star(s32 mode, s32 index) {
    CHECK_MODE();
    CHECK_INDEX_LAST();
    sOmmSparklySave->stars[index] = true;
    gSaveFileModified = true;
}

void omm_sparkly_collect_grand_star(s32 mode) {
    CHECK_MODE();
    sOmmSparklySave->grandStar = true;
    gSaveFileModified = true;
}

void omm_sparkly_unlock_bowser_4(s32 mode) {
    CHECK_MODE();
    sOmmSparklySave->bowser4 = true;
    gSaveFileModified = true;
}

void omm_sparkly_clear_mode(s32 mode) {
    CHECK_MODE();
    omm_zero(sOmmSparklySave->stars, sizeof(sOmmSparklySave->stars));
    sOmmSparklySave->timerStarted = false;
    sOmmSparklySave->bowser4 = false;
    sOmmSparklySave->grandStar = false;
    sOmmSparklySave->timer = 0;
    gSaveFileModified = true;
}

bool omm_sparkly_check_cheats(struct MarioState *m) {

    // No more problem after getting all Sparkly Stars
    if (omm_sparkly_is_completed(gOmmSparklyMode)) {
        return false;
    }

    // Regular cheats
    // The option "Enable Cheats" is not counted as a cheat
    static const u8 sNoCheat[sizeof(Cheats) - sizeof(Cheats.EnableCheats)] = { 0 };
    const u8 *cheats = ((const u8 *) &Cheats) + sizeof(Cheats.EnableCheats);
    if (!omm_same(cheats, sNoCheat, sizeof(sNoCheat))) {
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
        if (m->action == ACT_OMM_POSSESSION && gOmmMario->capture.timer >= 20) {
            omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, false, 0);
            return true;
        }
        struct Object *o = NULL;
        o = obj_get_nearest_with_behavior_and_radius(m->marioObj, bhvRecoveryHeart, 100.f);
        o = ((m->floorHeight > m->pos[1] -  40.f && m->floor && m->floor->object) ? m->floor->object : o);
        o = ((                                      m->wall  && m->wall->object ) ? m->wall->object  : o);
        o = ((m->ceilHeight  < m->pos[1] + 200.f && m->ceil  && m->ceil->object ) ? m->ceil->object  : o);
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

void omm_sparkly_turn_off_cheats() {
    if (!omm_sparkly_is_completed(gOmmSparklyMode)) {
        omm_zero(&Cheats, sizeof(Cheats));
        OMM_CHEATS_DISABLE;
    }
}

//
// Update
//

OMM_ROUTINE_UPDATE(omm_sparkly_update_save_data) {
    for (s32 mode = OMM_SPARKLY_MODE_NORMAL; mode != OMM_SPARKLY_MODE_COUNT; ++mode) {

        // If not unlocked, unset everything
        if (!omm_sparkly_is_unlocked(mode)) {
            sOmmSparklySave->timerStarted = false;
            sOmmSparklySave->bowser4 = false;
            sOmmSparklySave->grandStar = false;
            omm_zero(sOmmSparklySave->stars, sizeof(sOmmSparklySave->stars));
        }

        // If timer not started, unset other flags
        if (!omm_sparkly_is_timer_started(mode)) {
            sOmmSparklySave->bowser4 = false;
            sOmmSparklySave->grandStar = false;
            sOmmSparklySave->timer = 0;
            omm_zero(sOmmSparklySave->stars, sizeof(sOmmSparklySave->stars));
        }

        // If not all stars are collected, unflag Bowser 4 and Grand Star
        for (s32 i = 0; i != omm_sparkly_get_bowser_4_index(mode); ++i) {
            if (!omm_sparkly_is_star_collected(mode, i)) {
                sOmmSparklySave->bowser4 = false;
                sOmmSparklySave->grandStar = false;
                break;
            }
        }

        // If Bowser 4 is not unlocked, unflag Grand Star
        if (!omm_sparkly_is_bowser_4_unlocked(mode)) {
            sOmmSparklySave->grandStar = false;
        }

        // If the Grand Star is collected, set mode completed flag
        if (omm_sparkly_is_grand_star_collected(mode)) {
            sOmmSparklySave->completed = true;
        }

        // Unlock current mode and next mode if completed flag is set, otherwise unset every flag of next mode (including completed)
        if (omm_sparkly_is_completed(mode)) {
            sOmmSparklySave->unlocked = true;
            sOmmSparklySaveData[mode + 1].unlocked = true;
        } else if (mode + 1 < OMM_SPARKLY_MODE_COUNT) {
            omm_zero(&sOmmSparklySaveData[mode + 1], sizeof(OmmSparklySaveData));
        }
    }

    // If Sparkly Stars mode is enabled, advance the timer
    if (OMM_SPARKLY_MODE_IS_ENABLED) {
        s32 mode = gOmmSparklyMode;
        if (!omm_sparkly_is_grand_star_collected(mode)) {
            sOmmSparklySave->timer = min_s(sOmmSparklySave->timer + 1, TIMER_MAX);
        }
    }
}

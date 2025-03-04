#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define SPARKLY_STARS "sparkly_stars"

//
// Save data
//

#define TIMER_MAX               (10799970) // 99h 59m 59s
#define CHECK_MODE(ret)         if (sparklyMode < 1 || sparklyMode >= OMM_SPARKLY_MODE_COUNT) { return ret; }
#define CHECK_INDEX_LAST(ret)   if (starIndex < 0 || starIndex >= omm_sparkly_get_bowser_4_index(sparklyMode)) { return ret; }
#define CHECK_INDEX_COUNT(ret)  if (starIndex < 0 || starIndex >= omm_sparkly_get_num_stars(sparklyMode)) { return ret; }

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
#define sOmmSparklySave (&sOmmSparklySaveData[sparklyMode])

//
// Save file
//

extern u16 gRandomSeed;
static char D[] = "zE8QsdnmBh7a2FXpojt05OCrDgbvqRKNP6kZLTSGcyU3x49HVIJieuMflW1YwA";
static char R[] = "nwAGdbpWuTVNYSqEJxsU4cOQyMIv30ior9ZFD572zBlHPLmh8fagkeKX1RtCj6";
static u64 N = sizeof(D) - 1;

#define read() { char c = *(value++); for (u64 i = 0; i != N; ++i) { if (R[i] == c) { z += i * w; break; } } }
#define shuffle() { for (u64 i = 0; i != N; ++i) { u64 j = random_u16() % N; char c = R[i]; R[i] = R[j]; R[j] = c; } }
#define reset() { mem_cpy(R, D, N); }
#define convert(x, b, s) { u64 z = x, y = (1llu << b##llu); while (y) { omm_save_file_write_buffer("%c", R[z % N]); z /= N; y /= N; if (s) shuffle(); } }
#define invert(x, b, s) { u64 z = 0, w = 1, y = (1llu << b); while (y) { read(); w *= N; y /= N; if (s) shuffle(); } x = z; }

bool omm_sparkly_read(const char *name, const char *value, bool *invalid) {
    if (strcmp(name, SPARKLY_STARS) == 0) {
        if (!*value) {
            *invalid = true;
            return false;
        }

        // Retrieve seed
        reset();
        invert(gRandomSeed, 16, 0);

        // Read data
        OmmSparklySaveData saves[2]; s32 modes[2];
        for (s32 j = 0; j != 2; ++j) {
            mem_zero(&saves[j], sizeof(OmmSparklySaveData));

            // Flags
            u8 flags; invert(flags, 5, 1);
            saves[j].unlocked     = ((flags >> 0) & 1);
            saves[j].timerStarted = ((flags >> 1) & 1);
            saves[j].bowser4      = ((flags >> 2) & 1);
            saves[j].grandStar    = ((flags >> 3) & 1);
            saves[j].completed    = ((flags >> 4) & 1);

            // Stars
            s32 num; invert(num, 8, 1);
            if (num < 0 || num > OMM_SPARKLY_STARS_MAX) {
                omm_log_error("Invalid star count: %d\n",, num);
                return true;
            }
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
            omm_log_error("Modes don't match!\n");
            return true;
        }
        if (!mem_eq(&saves[0], &saves[1], sizeof(OmmSparklySaveData))) {
            omm_log_error("Save data don't match!\n");
            return true;
        }

        // Fill save struct
        s32 sparklyMode = modes[0];
        mem_cpy(sOmmSparklySave, &saves[0], sizeof(OmmSparklySaveData));
        return true;
    }
    return false;
}

void omm_sparkly_write() {
    gRandomSeed = (u16) SDL_GetPerformanceCounter();
    omm_save_file_write_buffer(OMM_SAVE_FILE_SECTION_SPARKLY_STARS "\n");
    for (s32 sparklyMode = OMM_SPARKLY_MODE_NORMAL; sparklyMode != OMM_SPARKLY_MODE_COUNT; ++sparklyMode) {
        if (omm_sparkly_is_unlocked(sparklyMode)) {
            omm_save_file_write_buffer(SPARKLY_STARS " = ");
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
                convert(sparklyMode, 4, 1);
            }
            omm_save_file_write_buffer("\n");
        }
    }
    omm_save_file_write_buffer("\n");
}

#undef read
#undef shuffle
#undef reset
#undef convert
#undef invert

//
// Save data
//

bool omm_sparkly_is_available(s32 sparklyMode) {
    if (sparklyMode >= OMM_SPARKLY_MODE_COUNT) { return false; } //TODO: Remove when Nebulas are implemented
    return OMM_SPARKLY_BLOCK_AVAILABLE & (1 << sparklyMode);
}

bool omm_sparkly_is_unlocked(s32 sparklyMode) {
    CHECK_MODE(false);
    return sOmmSparklySave->unlocked;
}

bool omm_sparkly_is_selectible(s32 sparklyMode) {
    return sparklyMode == OMM_SPARKLY_MODE_DISABLED || (
           omm_sparkly_is_unlocked(sparklyMode) &&
           omm_sparkly_is_available(sparklyMode) &&
           omm_sparkly_is_timer_started(sparklyMode));
}

bool omm_sparkly_is_selected(s32 sparklyMode) {
    return gOmmSparklyMode == sparklyMode;
}

bool omm_sparkly_is_completed(s32 sparklyMode) {
    if (sparklyMode >= OMM_SPARKLY_MODE_COUNT) { return false; } //TODO: Remove when Nebulas are implemented
    CHECK_MODE(true);
    return sOmmSparklySave->completed;
}

bool omm_sparkly_is_timer_started(s32 sparklyMode) {
    CHECK_MODE(false);
    return sOmmSparklySave->timerStarted;
}

bool omm_sparkly_is_star_collected(s32 sparklyMode, s32 starIndex) {
    CHECK_MODE(false);
    CHECK_INDEX_LAST(false);
    return sOmmSparklySave->stars[starIndex];
}

bool omm_sparkly_is_grand_star_collected(s32 sparklyMode) {
    CHECK_MODE(false);
    return sOmmSparklySave->grandStar;
}

bool omm_sparkly_is_bowser_4_unlocked(s32 sparklyMode) {
    CHECK_MODE(false);
    return sOmmSparklySave->bowser4;
}

bool omm_sparkly_is_bowser_4_battle() {
#if OMM_GAME_IS_SM64
    return gCurrLevelNum == OMM_SPARKLY_BOWSER_4_LEVEL && gCurrAreaIndex == OMM_SPARKLY_BOWSER_4_AREA;
#else
    return false;
#endif
}

s32 omm_sparkly_get_timer(s32 sparklyMode) {
    CHECK_MODE(0);
    return sOmmSparklySave->timer;
}

void omm_sparkly_disable() {
    gOmmSparklyMode = OMM_SPARKLY_MODE_DISABLED;
}

void omm_sparkly_set_mode(s32 sparklyMode) {
    if (OMM_SPARKLY_STARS_MODE != (u32) sparklyMode) {
        OMM_SPARKLY_STARS_MODE = (u32) sparklyMode;
        omm_save_file_do_save();
    }
}

void omm_sparkly_unlock_mode(s32 sparklyMode) {
    CHECK_MODE();
    sOmmSparklySave->unlocked = true;
    omm_save_file_do_save();
}

void omm_sparkly_start_timer(s32 sparklyMode) {
    CHECK_MODE();
    sOmmSparklySave->timerStarted = true;
    sOmmSparklySave->timer = 0;
    omm_save_file_do_save();
}

void omm_sparkly_collect_star(s32 sparklyMode, s32 starIndex) {
    CHECK_MODE();
    CHECK_INDEX_LAST();
    sOmmSparklySave->stars[starIndex] = true;
    omm_save_file_set_last_course(gCurrSaveFileNum - 1, OMM_GAME_MODE, gCurrCourseNum - 1);
    omm_save_file_do_save();
}

void omm_sparkly_collect_grand_star(s32 sparklyMode) {
    CHECK_MODE();
    sOmmSparklySave->grandStar = true;
    omm_sparkly_set_completed(sparklyMode);
    omm_save_file_set_last_course(gCurrSaveFileNum - 1, OMM_GAME_MODE, gCurrCourseNum - 1);
    omm_save_file_do_save();
}

void omm_sparkly_set_completed(s32 sparklyMode) {
    CHECK_MODE();
    sOmmSparklySave->completed = true;
    omm_save_file_do_save();
}

void omm_sparkly_unlock_bowser_4(s32 sparklyMode) {
    CHECK_MODE();
    sOmmSparklySave->bowser4 = true;
    omm_save_file_do_save();
}

void omm_sparkly_clear_mode(s32 sparklyMode) {
    CHECK_MODE();
    mem_zero(sOmmSparklySave->stars, sizeof(sOmmSparklySave->stars));
    sOmmSparklySave->timerStarted = false;
    sOmmSparklySave->bowser4 = false;
    sOmmSparklySave->grandStar = false;
    sOmmSparklySave->timer = 0;
    omm_save_file_do_save();
}

void omm_sparkly_clear_all() {
    mem_zero(sOmmSparklySaveData, sizeof(sOmmSparklySaveData));
    omm_save_file_do_save();
}

bool omm_sparkly_are_regular_cheats_enabled(struct MarioState *m, bool skipIfCheatsDisabled) {

    // Debug move
    if (m->action == ACT_DEBUG_FREE_MOVE) {
        return true;
    }

    // OMM cheats
    if (OMM_CHEATS_ENABLED) {
        return true;
    }

    // Enable cheats
    if (!Cheats.EnableCheats && skipIfCheatsDisabled) {
        return false;
    }

    // Cheats menu
    // The option "Enable Cheats" is not counted as a cheat
    static const u8 NO_CHEAT[sizeof(Cheats) - sizeof(Cheats.EnableCheats)] = {0};
    const u8 *cheats = ((const u8 *) &Cheats) + sizeof(Cheats.EnableCheats);
    if (!mem_eq(cheats, NO_CHEAT, sizeof(NO_CHEAT))) {
        return true;
    }

    // OK
    return false;
}

bool omm_sparkly_check_cheats(struct MarioState *m) {

    // No more problem after getting all Sparkly Stars
    if (OMM_SPARKLY_IS_MODE_COMPLETED) {
        return false;
    }

    // Check regular cheats
    if (omm_sparkly_are_regular_cheats_enabled(m, false)) {
        return true;
    }

    // What a stupid glitch :D
    if (omm_mario_is_capture(m) && omm_capture_get_type(gOmmCapture) == OMM_CAPTURE_CHAIN_CHOMP && gOmmCapture->oVelY > 100.f) {
        return true;
    }

#if OMM_GAME_IS_SM64
    // Castle grounds cheese (thanks, Petch)
    if (gCurrLevelNum == LEVEL_CASTLE_GROUNDS && gCurrAreaIndex == 1 && m->pos[1] > 6000.f) {
        if (omm_mario_is_capture(m) && gOmmMario->capture.timer >= 20) {
            omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
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
    if (!OMM_SPARKLY_IS_MODE_COMPLETED) {
        mem_zero(&Cheats, sizeof(Cheats));
        OMM_CHEATS_DISABLE;
    }
}

//
// Update
//

OMM_ROUTINE_UPDATE(omm_sparkly_update_save_data) {
    for (s32 sparklyMode = OMM_SPARKLY_MODE_NORMAL; sparklyMode != OMM_SPARKLY_MODE_COUNT; ++sparklyMode) {

        // If not unlocked, unset everything
        if (!omm_sparkly_is_unlocked(sparklyMode)) {
            sOmmSparklySave->timerStarted = false;
            sOmmSparklySave->bowser4 = false;
            sOmmSparklySave->grandStar = false;
            mem_zero(sOmmSparklySave->stars, sizeof(sOmmSparklySave->stars));
        }

        // If timer not started, unset other flags
        if (!omm_sparkly_is_timer_started(sparklyMode)) {
            sOmmSparklySave->bowser4 = false;
            sOmmSparklySave->grandStar = false;
            sOmmSparklySave->timer = 0;
            mem_zero(sOmmSparklySave->stars, sizeof(sOmmSparklySave->stars));
        }

        // If not all stars are collected, unflag Bowser 4 and Grand Star
        for (s32 i = 0; i != omm_sparkly_get_bowser_4_index(sparklyMode); ++i) {
            if (!omm_sparkly_is_star_collected(sparklyMode, i)) {
                sOmmSparklySave->bowser4 = false;
                sOmmSparklySave->grandStar = false;
                break;
            }
        }

        // If Bowser 4 is not unlocked, unflag Grand Star
        if (!omm_sparkly_is_bowser_4_unlocked(sparklyMode)) {
            sOmmSparklySave->grandStar = false;
        }

        // If the Grand Star is collected, set mode completed flag
        if (omm_sparkly_is_grand_star_collected(sparklyMode)) {
            sOmmSparklySave->completed = true;
        }

        // Unlock current mode and next mode if completed flag is set, otherwise unset every flag of next mode (including completed)
        if (omm_sparkly_is_completed(sparklyMode)) {
            sOmmSparklySave->unlocked = true;
            sOmmSparklySaveData[sparklyMode + 1].unlocked = true;
        } else if (sparklyMode + 1 < OMM_SPARKLY_MODE_COUNT) {
            mem_zero(&sOmmSparklySaveData[sparklyMode + 1], sizeof(OmmSparklySaveData));
        }
    }

    // If Sparkly Stars mode is enabled, advance the timer
    if (OMM_SPARKLY_MODE_IS_ENABLED) {
        s32 sparklyMode = gOmmSparklyMode;
        if (!omm_sparkly_is_grand_star_collected(sparklyMode)) {
            sOmmSparklySave->timer = min_s(sOmmSparklySave->timer + 1, TIMER_MAX);
        }
    }
}

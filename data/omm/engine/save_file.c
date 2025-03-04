#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#pragma GCC push_options
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define OMM_SAVEFILE_NAME   "omm_save_file.txt"
#define OMM_SAVEFILE_BACKUP "omm_save_file." OMM_VERSION ".backup"

u8 gLastCompletedCourseNum = COURSE_NONE;
u8 gLastCompletedLevelNum = LEVEL_NONE;
u8 gLastCompletedStarNum = 0;
u8 gCurrCourseStarFlags = 0;
u8 gGotFileCoinHiScore = 0;
u8 gSpecialTripleJump = 0;

//
// Flags and courses
//

typedef struct {
    const char *name;
    u32 value;
} OmmSaveFileNameValue;

static const OmmSaveFileNameValue OMM_SAVE_FILE_FLAGS[] = {
    { "wing_cap", SAVE_FLAG_HAVE_WING_CAP },
    { "metal_cap", SAVE_FLAG_HAVE_METAL_CAP },
    { "vanish_cap", SAVE_FLAG_HAVE_VANISH_CAP },
    { "bowser_key_1", SAVE_FLAG_HAVE_KEY_1 },
    { "bowser_key_2", SAVE_FLAG_HAVE_KEY_2 },
    { "moat_drained", SAVE_FLAG_MOAT_DRAINED },
    { "ddd_moved_back", SAVE_FLAG_DDD_MOVED_BACK },
    { "pps_door", SAVE_FLAG_UNLOCKED_PSS_DOOR },
    { "wf_door", SAVE_FLAG_UNLOCKED_WF_DOOR },
    { "jrb_door", SAVE_FLAG_UNLOCKED_JRB_DOOR },
    { "ccm_door", SAVE_FLAG_UNLOCKED_CCM_DOOR },
    { "8_star_door", SAVE_FLAG_UNLOCKED_BITDW_DOOR },
    { "30_star_door", SAVE_FLAG_UNLOCKED_BITFS_DOOR },
    { "50_star_door", SAVE_FLAG_UNLOCKED_50_STAR_DOOR },
    { "basement_door", SAVE_FLAG_UNLOCKED_BASEMENT_DOOR },
    { "upstairs_door", SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR },
};

static const OmmSaveFileNameValue OMM_SAVE_FILE_COURSES[] = {
    [COURSE_NONE] = { "castle", COURSE_NONE },
    [COURSE_BOB] = { "bob", COURSE_BOB },
    [COURSE_WF] = { "wf", COURSE_WF },
    [COURSE_JRB] = { "jrb", COURSE_JRB },
    [COURSE_CCM] = { "ccm", COURSE_CCM },
    [COURSE_BBH] = { "bbh", COURSE_BBH },
    [COURSE_HMC] = { "hmc", COURSE_HMC },
    [COURSE_LLL] = { "lll", COURSE_LLL },
    [COURSE_SSL] = { "ssl", COURSE_SSL },
    [COURSE_DDD] = { "ddd", COURSE_DDD },
    [COURSE_SL] = { "sl", COURSE_SL },
    [COURSE_WDW] = { "wdw", COURSE_WDW },
    [COURSE_TTM] = { "ttm", COURSE_TTM },
    [COURSE_THI] = { "thi", COURSE_THI },
    [COURSE_TTC] = { "ttc", COURSE_TTC },
    [COURSE_RR] = { "rr", COURSE_RR },
    [COURSE_BITDW] = { "bitdw", COURSE_BITDW },
    [COURSE_BITFS] = { "bitfs", COURSE_BITFS },
    [COURSE_BITS] = { "bits", COURSE_BITS },
    [COURSE_PSS] = { "pss", COURSE_PSS },
    [COURSE_COTMC] = { "cotmc", COURSE_COTMC },
    [COURSE_TOTWC] = { "totwc", COURSE_TOTWC },
    [COURSE_VCUTM] = { "vcutm", COURSE_VCUTM },
    [COURSE_WMOTR] = { "wmotr", COURSE_WMOTR },
    [COURSE_SA] = { "sa", COURSE_SA },
    [COURSE_CAKE_END] = { "ending", COURSE_CAKE_END },
};

//
// Extra fields
//

#define OMM_SAVE_FILE_FIELD_CAPTURES      "captures"
#define OMM_SAVE_FILE_FIELD_COLLECTIBLES  "collectibles"
#define OMM_SAVE_FILE_FIELD_LAST_COURSE   "last"

//
// Collectibles
//

#define OMM_SAVE_FILE_COLLECTIBLE_GET_B(collectibles, start, index)          (((collectibles) >> (start + index)) & 1)
#define OMM_SAVE_FILE_COLLECTIBLE_SET_B(collectibles, start, index, value)   collectibles &= ~(1llu << ((start) + (index))); collectibles |= (((u64) (value)) << ((start) + (index)))
#define OMM_SAVE_FILE_COLLECTIBLE_GET_I(collectibles, start, length)         (((collectibles) >> (start)) & ((1llu << (length)) - 1llu))
#define OMM_SAVE_FILE_COLLECTIBLE_SET_I(collectibles, start, length, value)  collectibles &= ~(((1llu << (length)) - 1llu) << (start)); collectibles |= ((((u64) (value)) & ((1llu << (length)) - 1llu)) << (start))

//
// Data
//

typedef struct {
    u8 stars;
    s16 score;
    bool cannon;
} OmmSaveCourse;

typedef struct {
    u32 flags;
    u64 collectibles;
    u64 captures;
    s32 lastCourseNum;
    OmmSaveCourse courses[array_length(OMM_SAVE_FILE_COURSES)];
    OmmStats stats[1];
} OmmSaveFile;

typedef struct {
    const char *name;
    s32 modes;
    s32 starsPerCourse;
    s32 lastFileIndex;
    s32 lastModeIndex;
    const char *collectibles;
    OmmSaveFile files[NUM_SAVE_FILES + 1][NUM_SAVE_MODES];
} OmmSaveBuffer;

static OmmSaveBuffer sOmmSaveBuffers[] = {
#define OMM_GAME(code, suffix, modes, stars, collectibles) [__EXPAND(CAT(OMM_GAME_, suffix))] = { code, modes, stars, 0, 0, collectibles, {} },
#include "data/omm/omm_games.inl"
#undef OMM_GAME
};

static bool sOmmSaveFileShouldWrite = false;

//
// Utils
//

#define sOmmSaveBuffer                  (&sOmmSaveBuffers[OMM_GAME_SAVE])
#define sOmmSaveFile                    (&sOmmSaveBuffer->files[fileIndex][modeIndex])

#define COURSE_INDEX_MIN                (COURSE_NONE - 1)
#define COURSE_INDEX_MAX                (COURSE_CAKE_END - 1)

#define CHECK_FILE_INDEX(i, fail)       if (i < 0 || i >= NUM_SAVE_FILES + 1) { fail; }
#define CHECK_MODE_INDEX(i, fail)       if (i < 0 || i >= sOmmSaveBuffer->modes) { fail; }
#define CHECK_COURSE_INDEX(i, fail)     if (i < COURSE_INDEX_MIN || i > COURSE_INDEX_MAX) { fail; }

#define READ_CHECK_INVALID(func, ...)   if (func(__VA_ARGS__, &invalid)) { continue; } else if (invalid) { fclose(f); return false; }
#define CHECK_VALID(cond)               if (!(cond)) { fclose(f); return false; }

//
// Post process
//

static void omm_save_file_clear_impossible_star_flags(s32 fileIndex, s32 modeIndex) {
    for (s32 i = 0; i != array_length(OMM_SAVE_FILE_COURSES); ++i) {
        s32 courseNum = OMM_SAVE_FILE_COURSES[i].value;
        u8 collected = sOmmSaveFile->courses[courseNum].stars;
        u8 starCourseFlags = omm_stars_get_course_flags(courseNum, modeIndex);
        if ((collected & starCourseFlags) != collected) {
            sOmmSaveFile->courses[courseNum].stars &= starCourseFlags;
            omm_save_file_do_save();
        }
    }
}

static bool omm_save_file_are_all_captures_registered(s32 gameIndex) {
    const OmmSaveBuffer *saveBuffer = &sOmmSaveBuffers[gameIndex];
    for (s32 modeIndex = 0; modeIndex != saveBuffer->modes; ++modeIndex) {
        for (s32 fileIndex = 0; fileIndex != NUM_SAVE_FILES; ++fileIndex) {
            const OmmSaveFile *saveFile = &saveBuffer->files[fileIndex][modeIndex];
            if (saveFile->captures == OMM_ALL_CAPTURES) {
                return true;
            }
        }
    }
    return false;
}

bool omm_save_file_are_all_captures_registered_sm64() {
    return (
        omm_save_file_are_all_captures_registered(OMM_GAME_SMEX) ||
        omm_save_file_are_all_captures_registered(OMM_GAME_R96X)
    );
}

static void omm_save_file_register_all_unavailable_captures(s32 fileIndex, s32 modeIndex) {
    u64 unavailableCaptureFlags = ~omm_level_get_all_available_captures(modeIndex) & OMM_ALL_CAPTURES;
    if ((sOmmSaveFile->captures & unavailableCaptureFlags) != unavailableCaptureFlags) {
        sOmmSaveFile->captures |= unavailableCaptureFlags;
        omm_save_file_do_save();
    }
}

// Triggered after loading or saving save files
// - Clear star flags that cannot be collected
// - Mark all unavailable captures as registered if there is at least one SM64 save file with
//   all captures registered in order to unlock Yoshi once all available captures are registered
static void omm_save_file_post_process() {
    bool shouldRegisterAllUnavailableCaptures = omm_save_file_are_all_captures_registered_sm64();
    for (s32 fileIndex = 0; fileIndex != NUM_SAVE_FILES; ++fileIndex) {
        for (s32 modeIndex = 0; modeIndex != sOmmSaveBuffer->modes; ++modeIndex) {
            omm_save_file_clear_impossible_star_flags(fileIndex, modeIndex);
            if (shouldRegisterAllUnavailableCaptures) {
                omm_save_file_register_all_unavailable_captures(fileIndex, modeIndex);
            }
        }
    }
}

//
// Write
//

char *gOmmSaveFileWriteBuffer = NULL;
s32 gOmmSaveFileWrittenLength = 0;

static bool omm_save_file_is_empty(const OmmSaveFile *savefile) {
    static const OmmSaveFile sOmmEmptySaveFile = {0};
    return mem_eq(savefile, &sOmmEmptySaveFile, sizeof(sOmmEmptySaveFile));
}

static void omm_save_file_write(const char *filename) {
    char saveFileWriteBuffer[OMM_SAVE_FILE_WRITE_BUFFER_LENGTH];
    gOmmSaveFileWriteBuffer = saveFileWriteBuffer;
    gOmmSaveFileWrittenLength = 0;

    // Set current save file as existing if not empty (i.e. at least 1 star, 1 collectible or 1 in-game flag)
    s32 fileIndex = (omm_is_main_menu() ? 0 : gCurrSaveFileNum) - 1;
    s32 modeIndex = OMM_GAME_MODE;
    if (fileIndex >= 0 && fileIndex < NUM_SAVE_FILES) {
        if (!omm_save_file_is_empty(sOmmSaveFile)) {
            sOmmSaveFile->flags |= SAVE_FLAG_FILE_EXISTS;
        }
    }

    // Header
    omm_save_file_write_buffer("#\n");
    omm_save_file_write_buffer("# ------------------------------------------------------------------------------------------------------------ #\n");
    omm_save_file_write_buffer("#                                                           _                                                  #\n");
    omm_save_file_write_buffer("#    ____     _                          __    __          |_|     _        __    __                     _     #\n");
    omm_save_file_write_buffer("#   / __ \\  _| |_  _______________  __   | \\  / |____  _ __ _  __ |/ ____   | \\  / | ____  _____ _______| |__  #\n");
    omm_save_file_write_buffer("#  / /  \\ \\/   \\ \\/ / __/ __/ _ \\ \\/ /   |  \\/  |\\__ \\| '_/| |/  \\  / __/   |  \\/  |/  \\ \\/ / _ \\ __/ _ \\  _/  #\n");
    omm_save_file_write_buffer("#  \\ \\__/ / (] |\\  /\\__ \\__ \\ __/\\  /    | \\  / |/ _  | |  | | () | \\__ \\   | \\  / | () \\  /  __/__ \\ __/ |__  #\n");
    omm_save_file_write_buffer("#   \\____/ \\___|/ / /___/___/___// /     |_|\\/|_|\\___/|_|  |_|\\__/  /___/   |_|\\/|_|\\__/ \\/ \\___/___/___/___/  #\n");
    omm_save_file_write_buffer("#              /_/              /_/                                                                            #\n");
    omm_save_file_write_buffer("#                                                                                              by PeachyPeach  #\n");
    omm_save_file_write_buffer("# ------------------------------------------------------------------------------------------------------------ #\n");
    omm_save_file_write_buffer("#\n");
    omm_save_file_write_buffer("# All games saves are stored in this file.\n");
    omm_save_file_write_buffer("# Lines starting with # are comments.\n");
    omm_save_file_write_buffer("# Do not change the version number!\n");
    omm_save_file_write_buffer("#\n");
    omm_save_file_write_buffer("# Games sections:\n");
    omm_save_file_write_buffer("# - Every game save starts with a pair [xxxx:yy]:\n");
    omm_save_file_write_buffer("#   - xxxx is the game code name\n");
    omm_save_file_write_buffer("#   - yy is the save file name\n");
    omm_save_file_write_buffer("# - Flags:\n");
    omm_save_file_write_buffer("#   - 0 = locked and 1 = unlocked\n");
    omm_save_file_write_buffer("# - Courses:\n");
    omm_save_file_write_buffer("#   - the first 7 digits are the stars (1 = collected, 0 = not)\n");
    omm_save_file_write_buffer("#   - the next digit is the cannon (1 = open, 0 = close)\n");
    omm_save_file_write_buffer("#   - the last number is the coin score\n");
    omm_save_file_write_buffer("# - Captures:\n");
    omm_save_file_write_buffer("#   - each digit is a capture (1 = registered, 0 = not)\n");
    omm_save_file_write_buffer("# - Collectibles:\n");
    omm_save_file_write_buffer("#   - Render96:\n");
    omm_save_file_write_buffer("#     - the first 10 digits are the Luigi keys (1 = collected, 0 = not)\n");
    omm_save_file_write_buffer("#     - the last 6 digits are the Wario coins (1 = collected, 0 = not)\n");
    omm_save_file_write_buffer("# - Stats:\n");
    omm_save_file_write_buffer("#   - various stats per save file (these should not be edited manually)\n");
    omm_save_file_write_buffer("#\n");
    omm_save_file_write_buffer("# The " OMM_SAVE_FILE_SECTION_SPARKLY_STARS ", " OMM_SAVE_FILE_SECTION_STATS " and " OMM_SAVE_FILE_SECTION_SECRETS " sections should not be edited manually.\n");
    omm_save_file_write_buffer("# The " OMM_SAVE_FILE_SECTION_MARIO_COLORS " and " OMM_SAVE_FILE_SECTION_PEACH_COLORS " sections can be edited in-game with the Palette Editor.\n");
    omm_save_file_write_buffer("#\n");
    omm_save_file_write_buffer("\n");

    // Version
    omm_save_file_write_buffer("version = " OMM_VERSION "\n\n");

    // Games
    for (s32 gameIndex = 0; gameIndex != array_length(sOmmSaveBuffers); ++gameIndex) {
        const OmmSaveBuffer *saveBuffer = &sOmmSaveBuffers[gameIndex];
        for (s32 fileIndex = 0; fileIndex != NUM_SAVE_FILES; ++fileIndex) {
            for (s32 modeIndex = 0; modeIndex != saveBuffer->modes; ++modeIndex) {
                const OmmSaveFile *saveFile = &saveBuffer->files[fileIndex][modeIndex];
                if (saveFile->flags & SAVE_FLAG_FILE_EXISTS) {

                    // Name
                    omm_save_file_write_buffer(OMM_SAVE_FILE_SECTION_GAME_DATA "\n", saveBuffer->name, fileIndex + 'A', modeIndex);

                    // Flags
                    for (s32 k = 0; k != array_length(OMM_SAVE_FILE_FLAGS); ++k) {
                        omm_save_file_write_buffer("%s = %d\n", OMM_SAVE_FILE_FLAGS[k].name, ((saveFile->flags & OMM_SAVE_FILE_FLAGS[k].value) != 0));
                    }

                    // Courses
                    for (s32 k = 0; k != array_length(OMM_SAVE_FILE_COURSES); ++k) {
                        omm_save_file_write_buffer("%s = ", OMM_SAVE_FILE_COURSES[k].name);
                        s32 courseNum = OMM_SAVE_FILE_COURSES[k].value;

                        // Stars
                        for (s32 b = 0; b != saveBuffer->starsPerCourse; ++b) {
                            omm_save_file_write_buffer("%d", ((saveFile->courses[courseNum].stars >> b) & 1));
                        }

                        // Cannon
                        omm_save_file_write_buffer(" %d", (saveFile->courses[courseNum].cannon != 0));

                        // Score
                        omm_save_file_write_buffer(" %hd\n", saveFile->courses[courseNum].score);
                    }

                    // Last course visited
                    omm_save_file_write_buffer("last = %s %d\n", OMM_SAVE_FILE_COURSES[saveFile->lastCourseNum].name, (fileIndex == saveBuffer->lastFileIndex && modeIndex == saveBuffer->lastModeIndex));

                    // Captures
                    omm_save_file_write_buffer(OMM_SAVE_FILE_FIELD_CAPTURES " = ");
                    for (u64 b = 0; b != OMM_MAX_CAPTURES; ++b) {
                        omm_save_file_write_buffer("%d", (s32) ((saveFile->captures >> b) & 1llu));
                    }
                    omm_save_file_write_buffer("\n");

                    // Collectibles
                    if (saveBuffer->collectibles) {
                        omm_save_file_write_buffer(OMM_SAVE_FILE_FIELD_COLLECTIBLES " =");
                        s32 numFields = (s32) strlen(saveBuffer->collectibles) / 5;
                        for (s32 i = 0; i != numFields; ++i) {
                            char fieldType = saveBuffer->collectibles[5 * i + 0];
                            s32 fieldStart = (((saveBuffer->collectibles[5 * i + 1] - '0') * 10) + (saveBuffer->collectibles[5 * i + 2] - '0'));
                            s32 fieldLength = (((saveBuffer->collectibles[5 * i + 3] - '0') * 10) + (saveBuffer->collectibles[5 * i + 4] - '0'));
                            switch (fieldType) {
                                case 'i': {
                                    u64 x = OMM_SAVE_FILE_COLLECTIBLE_GET_I(saveFile->collectibles, fieldStart, fieldLength);
                                    omm_save_file_write_buffer(" %llu", x);
                                } break;

                                case 'b': {
                                    omm_save_file_write_buffer(" ");
                                    for (s32 j = 0; j != fieldLength; ++j) {
                                        s32 bit = (s32) OMM_SAVE_FILE_COLLECTIBLE_GET_B(saveFile->collectibles, fieldStart, j);
                                        omm_save_file_write_buffer("%d", bit);
                                    }
                                } break;
                            }
                        }
                        omm_save_file_write_buffer("\n");
                    }

                    // Stats
                    omm_stats_write(saveFile->stats, OMM_STATS_PREFIX_LOCAL);
                    omm_save_file_write_buffer("\n");
                }
            }
        }
    }

    // Sparkly Stars data
    omm_sparkly_write();

    // Global stats
    omm_save_file_write_buffer(OMM_SAVE_FILE_SECTION_STATS "\n");
    omm_stats_write(gOmmStats, OMM_STATS_PREFIX_GLOBAL);
    omm_save_file_write_buffer("\n");

    // Secrets
    omm_secrets_write();

    // Mario/Peach colors
    omm_mario_colors_write();

    // Post process
    omm_save_file_post_process();

    // Write the save file
    sys_path_t filepath;
    FILE *f = fopen(fs_get_save_path(filepath, filename), "wb");
    if (f) {
        omm_log("Saving progress to \"%s\"\n",, filename);
        fwrite(saveFileWriteBuffer, sizeof(char), gOmmSaveFileWrittenLength, f);
        fclose(f);
    }

    // Done
    sOmmSaveFileShouldWrite = false;
}

void omm_save_file_do_save() {
    sOmmSaveFileShouldWrite = true;
}

//
// Read
//

#define TOKEN_LEN  (0x100)
#define NUM_TOKENS (4)

typedef struct { char args[NUM_TOKENS][TOKEN_LEN]; } Tokens;

static void new_token(Tokens *tokens, u32 *index, const char *temp, u32 *length) {
    if (*length) {
        mem_cpy(tokens->args[(*index)++], temp, *length);
    }
    *length = 0;
}

static Tokens tokenize(const char *buffer) {
    Tokens tokens; mem_zero(&tokens, sizeof(tokens));
    char temp[TOKEN_LEN];
    u32 index = 0;
    u32 length = 0;
    u32 maxlen = (s32) strlen(buffer);
    for (u32 i = 0, dq = 0; i <= maxlen && index < NUM_TOKENS; ++i) {
        char c = buffer[i];
        if (c == '"') {
            new_token(&tokens, &index, temp, &length);
            dq = !dq;
        } else if (c && (dq || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_') || (c == '.'))) {
            if (length < TOKEN_LEN - 1) {
                temp[length++] = c;
            }
        } else {
            new_token(&tokens, &index, temp, &length);
        }
    }
    return tokens;
}

static void trim_left(char *buffer) {
    s32 l = (s32) strlen(buffer);
    s32 i = 0;
    while (buffer[i] && (u8) buffer[i] <= (u8) ' ') ++i;
    mem_mov(buffer, buffer + i, l - i + 1);
}

static bool omm_save_file_load(const char *filename) {
    sys_path_t filepath;
    FILE *f = fopen(fs_get_save_path(filepath, filename), "r");
    if (f) {
        bool invalid = false;
        bool versionMatch = false;
        OmmSaveBuffer *saveBuffer = NULL;
        OmmSaveFile *saveFile = NULL;
        s32 fileIndex = 0, modeIndex = 0;
        char buffer[1024];
        while (fgets(buffer, 1024, f)) {
            trim_left(buffer);
            if (*buffer && *buffer != '#') {
                Tokens tokens = tokenize(buffer);

                // Version
                if (strcmp(tokens.args[0], "version") == 0 && strcmp(tokens.args[1], OMM_VERSION) == 0) {
                    versionMatch = true;
                }

                // Section
                if (buffer[0] == '[') {
                    saveFile = NULL;

                    // Game save
                    for (s32 i = 0; i != array_length(sOmmSaveBuffers); ++i) {
                        if (strcmp(tokens.args[0], sOmmSaveBuffers[i].name) == 0) {

                            // File index
                            fileIndex = tokens.args[1][0] - 'A';
                            CHECK_VALID(fileIndex >= 0 && fileIndex < NUM_SAVE_FILES);

                            // Mode index
                            modeIndex = tokens.args[1][1] - '0';
                            CHECK_VALID(modeIndex >= 0 && modeIndex < NUM_SAVE_MODES);

                            // Save file
                            saveBuffer = &sOmmSaveBuffers[i];
                            saveFile = &saveBuffer->files[fileIndex][modeIndex];
                            saveFile->flags |= SAVE_FLAG_FILE_EXISTS;
                            break;
                        }
                    }
                    continue;
                }

                // Sparkly Stars data
                READ_CHECK_INVALID(omm_sparkly_read, tokens.args[0], tokens.args[1]);

                // Global stats
                READ_CHECK_INVALID(omm_stats_read, gOmmStats, OMM_STATS_PREFIX_GLOBAL, tokens.args[0], tokens.args[1], tokens.args[2]);

                // Secrets
                READ_CHECK_INVALID(omm_secrets_read, tokens.args[0], tokens.args[1]);

                // Mario/Peach colors
                READ_CHECK_INVALID(omm_mario_colors_read, tokens.args[0], tokens.args[1], tokens.args[2]);

                // Game data
                if (saveFile) {

                    // Stats
                    READ_CHECK_INVALID(omm_stats_read, saveFile->stats, OMM_STATS_PREFIX_LOCAL, tokens.args[0], tokens.args[1], tokens.args[2]);

                    // Captures
                    if (strcmp(tokens.args[0], OMM_SAVE_FILE_FIELD_CAPTURES) == 0) {
                        CHECK_VALID((s32) strlen(tokens.args[1]) == OMM_MAX_CAPTURES);
                        for (u64 b = 0; b != OMM_MAX_CAPTURES; ++b) {
                            saveFile->captures |= ((u64) (tokens.args[1][b] == '1') << b);
                        }
                    }

                    // Collectibles
                    if (strcmp(tokens.args[0], OMM_SAVE_FILE_FIELD_COLLECTIBLES) == 0) {
                        if (saveBuffer->collectibles) {
                            saveFile->collectibles = 0;
                            s32 numFields = (s32) strlen(saveBuffer->collectibles) / 5;
                            for (s32 i = 0; i != numFields; ++i) {
                                char fieldType = saveBuffer->collectibles[5 * i + 0];
                                s32 fieldStart = (((saveBuffer->collectibles[5 * i + 1] - '0') * 10) + (saveBuffer->collectibles[5 * i + 2] - '0'));
                                s32 fieldLength = (((saveBuffer->collectibles[5 * i + 3] - '0') * 10) + (saveBuffer->collectibles[5 * i + 4] - '0'));
                                const char *field = tokens.args[1 + i];
                                switch (fieldType) {
                                    case 'i': {
                                        u64 value = 0;
                                        CHECK_VALID(*field);
                                        CHECK_VALID(sscanf(field, "%llu", &value) == 1);
                                        OMM_SAVE_FILE_COLLECTIBLE_SET_I(saveFile->collectibles, fieldStart, fieldLength, value);
                                    } break;

                                    case 'b': {
                                        CHECK_VALID((s32) strlen(field) >= fieldLength);
                                        for (s32 j = 0; j != fieldLength; ++j) {
                                            OMM_SAVE_FILE_COLLECTIBLE_SET_B(saveFile->collectibles, fieldStart, j, field[j] == '1');
                                        }
                                    } break;
                                }
                            }
                        }
                    }

                    // Flag
                    for (s32 i = 0; i != array_length(OMM_SAVE_FILE_FLAGS); ++i) {
                        if (strcmp(tokens.args[0], OMM_SAVE_FILE_FLAGS[i].name) == 0) {
                            CHECK_VALID(*tokens.args[1]);
                            saveFile->flags |= ((*tokens.args[1] == '1') ? OMM_SAVE_FILE_FLAGS[i].value : 0);
                            break;
                        }
                    }

                    // Course
                    for (s32 i = 0; i != array_length(OMM_SAVE_FILE_COURSES); ++i) {
                        if (strcmp(tokens.args[0], OMM_SAVE_FILE_COURSES[i].name) == 0) {
                            s32 courseNum = OMM_SAVE_FILE_COURSES[i].value;

                            // Stars
                            CHECK_VALID((s32) strlen(tokens.args[1]) == saveBuffer->starsPerCourse);
                            for (s32 b = 0; b != saveBuffer->starsPerCourse; ++b) {
                                saveFile->courses[courseNum].stars |= ((tokens.args[1][b] == '1') << b);
                            }

                            // Cannon
                            CHECK_VALID((s32) strlen(tokens.args[2]) == 1);
                            saveFile->courses[courseNum].cannon = (*tokens.args[2] == '1');

                            // Score
                            s32 coinScore = 0;
                            CHECK_VALID(*tokens.args[3]);
                            CHECK_VALID(sscanf(tokens.args[3], "%d", &coinScore) == 1);
                            saveFile->courses[courseNum].score = clamp_s(coinScore, 0, 999);
                            break;
                        }
                    }

                    // Last course visited
                    if (strcmp(tokens.args[0], OMM_SAVE_FILE_FIELD_LAST_COURSE) == 0) {
                        CHECK_VALID(*tokens.args[1]);

                        // Check course number
                        if (sscanf(tokens.args[1], "%d", &saveFile->lastCourseNum) != 1) {

                            // Check course name
                            for (s32 i = 0; i != array_length(OMM_SAVE_FILE_COURSES); ++i) {
                                if (strcmp(tokens.args[1], OMM_SAVE_FILE_COURSES[i].name) == 0) {
                                    saveFile->lastCourseNum = OMM_SAVE_FILE_COURSES[i].value;
                                    break;
                                }
                            }
                        }
                        saveFile->lastCourseNum = clamp_s(saveFile->lastCourseNum, COURSE_NONE, COURSE_CAKE_END) % COURSE_CAKE_END;

                        // Check the second arg
                        if (*tokens.args[2]) {
                            CHECK_VALID((s32) strlen(tokens.args[2]) == 1);
                            if (*tokens.args[2] == '1') {
                                saveBuffer->lastFileIndex = fileIndex;
                                saveBuffer->lastModeIndex = modeIndex;
                            }
                        }
                        continue;
                    }
                }
            }
        }
        fclose(f);

        // If the version of the save file is not found or is not the current version, try to load the backup file
        if (!versionMatch && omm_save_file_load(OMM_SAVEFILE_BACKUP)) {
            omm_log_warning("Version mismatch! Backup file successfully loaded.\n");
            omm_save_file_write(OMM_SAVEFILE_NAME);
            return true;
        }

        // Finish loading the save file
        controller_reconfigure();
        sOmmSaveFileShouldWrite = false;
        omm_save_file_post_process();
        return true;
    }
    return false;
}

void omm_save_file_load_all() {
    OMM_DO_ONCE {
        if (omm_save_file_load(OMM_SAVEFILE_NAME)) {
            omm_save_file_write(OMM_SAVEFILE_BACKUP);
        } else if (omm_save_file_load(OMM_SAVEFILE_BACKUP)) {
            omm_log_warning("Unable to load the save file! Backup file successfully loaded.\n");
            omm_save_file_write(OMM_SAVEFILE_NAME);
        }
    }
}

//
// Copy
//

void omm_save_file_copy(s32 fileIndex, s32 modeIndex, s32 destIndex) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_FILE_INDEX(destIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    mem_cpy(&sOmmSaveBuffer->files[destIndex][modeIndex], &sOmmSaveBuffer->files[fileIndex][modeIndex], sizeof(sOmmSaveBuffer->files[fileIndex][modeIndex]));
    omm_save_file_do_save();
}

//
// Erase
//

void omm_save_file_erase(s32 fileIndex, s32 modeIndex) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    mem_zero(sOmmSaveFile, sizeof(*sOmmSaveFile));
    omm_save_file_do_save();
}

//
// Get
//

bool omm_save_file_exists(s32 fileIndex, s32 modeIndex) {
    CHECK_FILE_INDEX(fileIndex, return false);
    CHECK_MODE_INDEX(modeIndex, return false);
    return (sOmmSaveFile->flags & SAVE_FLAG_FILE_EXISTS) != 0;
}

u32 omm_save_file_get_flags(s32 fileIndex, s32 modeIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_MODE_INDEX(modeIndex, return 0);
    if (gCurrCreditsEntry || gCurrDemoInput) return 0;
    return sOmmSaveFile->flags;
}

u32 omm_save_file_get_star_flags(s32 fileIndex, s32 modeIndex, s32 courseIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_MODE_INDEX(modeIndex, return 0);
    CHECK_COURSE_INDEX(courseIndex, return 0);
    return sOmmSaveFile->courses[courseIndex + 1].stars;
}

u32 omm_save_file_get_cannon_unlocked(s32 fileIndex, s32 modeIndex, s32 courseIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_MODE_INDEX(modeIndex, return 0);
    CHECK_COURSE_INDEX(courseIndex, return 0);
    return sOmmSaveFile->courses[courseIndex + 1].cannon;
}

s32 omm_save_file_get_course_star_count(s32 fileIndex, s32 modeIndex, s32 courseIndex) {
    s32 starCount = 0;
    u32 starSaveFlags = omm_save_file_get_star_flags(fileIndex, modeIndex, courseIndex);
    for (s32 i = 0; i != sOmmSaveBuffer->starsPerCourse; ++i) {
        starCount += ((starSaveFlags >> i) & 1);
    }
    return starCount;
}

s32 omm_save_file_get_course_coin_score(s32 fileIndex, s32 modeIndex, s32 courseIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_MODE_INDEX(modeIndex, return 0);
    CHECK_COURSE_INDEX(courseIndex, return 0);
    return sOmmSaveFile->courses[courseIndex + 1].score;
}

s32 omm_save_file_get_total_star_count(s32 fileIndex, s32 modeIndex) {
    s32 starCount = 0;
    for (s32 courseIndex = COURSE_INDEX_MIN; courseIndex <= COURSE_INDEX_MAX; ++courseIndex) {
        starCount += omm_save_file_get_course_star_count(fileIndex, modeIndex, courseIndex);
    }
    return starCount;
}

s32 omm_save_file_get_total_coin_score(s32 fileIndex, s32 modeIndex) {
    s32 coinCount = 0;
    for (s32 courseIndex = COURSE_INDEX_MIN; courseIndex <= COURSE_INDEX_MAX; ++courseIndex) {
        coinCount += omm_save_file_get_course_coin_score(fileIndex, modeIndex, courseIndex);
    }
    return coinCount;
}

s32 omm_save_file_get_last_file_index() {
    return sOmmSaveBuffer->lastFileIndex;
}

s32 omm_save_file_get_last_mode_index() {
    return sOmmSaveBuffer->lastModeIndex;
}

s32 omm_save_file_get_last_course_num(s32 fileIndex, s32 modeIndex) {
    CHECK_FILE_INDEX(fileIndex, return COURSE_NONE);
    CHECK_MODE_INDEX(modeIndex, return COURSE_NONE);
    return omm_save_file_exists(fileIndex, modeIndex) ? sOmmSaveFile->lastCourseNum : COURSE_NONE;
}

u64 omm_save_file_get_capture_flags(s32 fileIndex, s32 modeIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_MODE_INDEX(modeIndex, return 0);
    return sOmmSaveFile->captures;
}

s32 omm_save_file_get_capture_count(s32 fileIndex, s32 modeIndex) {
    s32 captureCount = 0;
    u64 captureFlags = omm_save_file_get_capture_flags(fileIndex, modeIndex) & omm_level_get_all_available_captures(modeIndex);
    for (u64 i = 0; i != OMM_MAX_CAPTURES; ++i) {
        captureCount += ((captureFlags & (1llu << i)) != 0);
    }
    return captureCount;
}

u32 omm_save_file_get_taken_luigi_key(s32 fileIndex, s32 modeIndex, s32 keyIndex) {
#if OMM_GAME_IS_R96X
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_MODE_INDEX(modeIndex, return 0);
    return OMM_SAVE_FILE_COLLECTIBLE_GET_B(sOmmSaveFile->collectibles, 0, keyIndex);
#else
    return 0;
#endif
}

u32 omm_save_file_get_taken_wario_coin(s32 fileIndex, s32 modeIndex, s32 coinIndex) {
#if OMM_GAME_IS_R96X
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_MODE_INDEX(modeIndex, return 0);
    return OMM_SAVE_FILE_COLLECTIBLE_GET_B(sOmmSaveFile->collectibles, NUM_KEYS, coinIndex);
#else
    return 0;
#endif
}

s32 omm_save_file_get_luigi_keys_count(s32 fileIndex, s32 modeIndex) {
#if OMM_GAME_IS_R96X
    s32 count = 0;
    for (s32 keyIndex = 0; keyIndex != NUM_KEYS; ++keyIndex) {
        count += omm_save_file_get_taken_luigi_key(fileIndex, modeIndex, keyIndex);
    }
    return count;
#else
    return 0;
#endif
}

s32 omm_save_file_get_wario_coins_count(s32 fileIndex, s32 modeIndex) {
#if OMM_GAME_IS_R96X
    s32 count = 0;
    for (s32 coinIndex = 0; coinIndex != NUM_WARIO_COINS; ++coinIndex) {
        count += omm_save_file_get_taken_wario_coin(fileIndex, modeIndex, coinIndex);
    }
    return count;
#else
    return 0;
#endif
}

OmmStats *omm_save_file_get_stats(s32 fileIndex, s32 modeIndex) {
    CHECK_FILE_INDEX(fileIndex, return NULL);
    CHECK_MODE_INDEX(modeIndex, return NULL);
    return sOmmSaveFile->stats;
}

//
// Set
//

void omm_save_file_set_flags(s32 fileIndex, s32 modeIndex, u32 flags) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    sOmmSaveFile->flags |= flags;
    omm_save_file_set_last_course(fileIndex, modeIndex, gCurrCourseNum - 1);
    omm_save_file_do_save();
}

void omm_save_file_set_star_flags(s32 fileIndex, s32 modeIndex, s32 courseIndex, u32 starFlags) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    CHECK_COURSE_INDEX(courseIndex, return);
    sOmmSaveFile->courses[courseIndex + 1].stars |= starFlags;
    omm_save_file_clear_impossible_star_flags(fileIndex, modeIndex);
    omm_save_file_set_last_course(fileIndex, modeIndex, courseIndex);
    omm_save_file_do_save();
}

void omm_save_file_set_cannon_unlocked(s32 fileIndex, s32 modeIndex, s32 courseIndex) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    CHECK_COURSE_INDEX(courseIndex, return);
    sOmmSaveFile->courses[courseIndex + 1].cannon = true;
    omm_save_file_set_last_course(fileIndex, modeIndex, courseIndex);
    omm_save_file_do_save();
}

void omm_save_file_set_last_course(s32 fileIndex, s32 modeIndex, s32 courseIndex) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    CHECK_COURSE_INDEX(courseIndex, return);
    if (!omm_is_main_menu() && fileIndex < NUM_SAVE_FILES) {
        sOmmSaveBuffer->lastFileIndex = fileIndex;
        sOmmSaveBuffer->lastModeIndex = modeIndex;
        sOmmSaveFile->lastCourseNum = clamp_s(courseIndex + 1, COURSE_NONE, COURSE_CAKE_END) % COURSE_CAKE_END;
    }
}

void omm_save_file_register_capture(s32 fileIndex, s32 modeIndex, u64 captureFlag) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    if ((captureFlag & OMM_ALL_CAPTURES) && (sOmmSaveFile->captures & captureFlag) != captureFlag) {
        sOmmSaveFile->captures |= captureFlag;
        omm_save_file_do_save();
    }
}

void omm_save_file_set_taken_luigi_key(s32 fileIndex, s32 modeIndex, s32 keyIndex) {
#if OMM_GAME_IS_R96X
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    OMM_SAVE_FILE_COLLECTIBLE_SET_B(sOmmSaveFile->collectibles, 0, keyIndex, 1);
    omm_save_file_set_last_course(fileIndex, modeIndex, gCurrCourseNum - 1);
    omm_save_file_do_save();
#endif
}

void omm_save_file_set_taken_wario_coin(s32 fileIndex, s32 modeIndex, s32 coinIndex) {
#if OMM_GAME_IS_R96X
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    OMM_SAVE_FILE_COLLECTIBLE_SET_B(sOmmSaveFile->collectibles, NUM_KEYS, coinIndex, 1);
    omm_save_file_set_last_course(fileIndex, modeIndex, gCurrCourseNum - 1);
    omm_save_file_do_save();
#endif
}

void omm_save_file_collect_star_or_key(s32 fileIndex, s32 modeIndex, s32 levelIndex, s32 starIndex, s32 numCoins) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    s32 courseIndex = omm_level_get_course(levelIndex + 1) - 1;
    CHECK_COURSE_INDEX(courseIndex, return);

    // Update globals
    gLastCompletedCourseNum = courseIndex + 1;
    gLastCompletedLevelNum = OMM_BOWSER_IN_THE_LEVEL(levelIndex + 1);
    gLastCompletedStarNum = starIndex + 1;
    gGotFileCoinHiScore = 0;

    // Star flag
    s32 starFlag = (1 << starIndex);
    omm_stars_set_flags(starFlag);
    switch (levelIndex + 1) {
        case LEVEL_BOWSER_1: {
            omm_save_file_set_flags(fileIndex, modeIndex, SAVE_FLAG_HAVE_KEY_1);
            omm_speedrun_split(OMM_SPLIT_BOWSER);
        } break;

        case LEVEL_BOWSER_2: {
            omm_save_file_set_flags(fileIndex, modeIndex, SAVE_FLAG_HAVE_KEY_2);
            omm_speedrun_split(OMM_SPLIT_BOWSER);
        } break;

        case LEVEL_BOWSER_3: {
            omm_save_file_set_flags(fileIndex, modeIndex, 0);
            omm_speedrun_split(OMM_SPLIT_BOWSER);
            omm_stats_increase(starsCollected, 1);
        } break;

        default: {
            omm_save_file_set_star_flags(fileIndex, modeIndex, courseIndex, starFlag);
            omm_speedrun_split(OMM_SPLIT_STAR);
            omm_stats_increase(starsCollected, 1);
        } break;
    }

    // New high score
    if (numCoins > omm_save_file_get_course_coin_score(fileIndex, modeIndex, courseIndex)) {
        sOmmSaveFile->courses[courseIndex + 1].score = numCoins;
        gGotFileCoinHiScore = 1;
    }
}

void omm_save_file_clear_flags(s32 fileIndex, s32 modeIndex, u32 flags) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    sOmmSaveFile->flags &= ~flags;
    sOmmSaveFile->flags |= SAVE_FLAG_FILE_EXISTS;
    omm_save_file_set_last_course(fileIndex, modeIndex, gCurrCourseNum - 1);
    omm_save_file_do_save();
}

void omm_save_file_clear_star_flags(s32 fileIndex, s32 modeIndex, s32 courseIndex, u32 starFlags) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_MODE_INDEX(modeIndex, return);
    CHECK_COURSE_INDEX(courseIndex, return);
    sOmmSaveFile->courses[courseIndex + 1].stars &= ~starFlags;
    omm_save_file_set_last_course(fileIndex, modeIndex, courseIndex);
    omm_save_file_do_save();
}

//
// Checkpoint
//

static struct { s32 levelNum, courseNum, actNum, areaIndex, warpNode; } sWarpCheckpoint;

static bool warp_node_is_painting_warp_node(struct WarpNode *warpNode) {
    if (gCurrentArea && gCurrentArea->paintingWarpNodes) {
        for (s32 i = 0; i != 45; ++i) {
            if (mem_eq(warpNode, &gCurrentArea->paintingWarpNodes[i], sizeof(struct WarpNode))) {
                return true;
            }
        }
    }
    return false;
}

bool warp_checkpoint_check(struct WarpNode *warpNode, s32 actIndex) {
    if (warpNode) {
        s32 destCourseNum = omm_level_get_course(warpNode->destLevel & 0x7F);
        if (sWarpCheckpoint.courseNum != COURSE_NONE &&
            sWarpCheckpoint.actNum == actIndex + 1 &&
            gSavedCourseNum == destCourseNum) {
            warpNode->destLevel = sWarpCheckpoint.levelNum;
            warpNode->destArea = sWarpCheckpoint.areaIndex;
            warpNode->destNode = sWarpCheckpoint.warpNode;
            return true;
        }
    }
    sWarpCheckpoint.courseNum = COURSE_NONE;
    return false;
}

void warp_checkpoint_check_if_should_set(struct WarpNode *warpNode, s32 courseIndex, s32 actIndex) {
#if OMM_GAME_IS_SM64
    if (gOmmSparkly->cheatDetected && gOmmSparklyCheats->counter >= 3) {
        initiate_warp(LEVEL_CASTLE, 4, 0x0A + (random_u16() % 4), sDelayedWarpArg);
        return;
    }
#endif
    if (warp_node_is_painting_warp_node(warpNode)) {
        omm_mario_unpossess_object_before_warp(gMarioState);
    }
    if (warpNode->destLevel & 0x80) {
        sWarpCheckpoint.levelNum = (warpNode->destLevel & 0x7F);
        sWarpCheckpoint.courseNum = courseIndex + 1;
        sWarpCheckpoint.actNum = actIndex + 1;
        sWarpCheckpoint.areaIndex = warpNode->destArea;
        sWarpCheckpoint.warpNode = warpNode->destNode;
    }
}

void warp_checkpoint_disable() {
    sWarpCheckpoint.courseNum = COURSE_NONE;
}

//
// Auto-save
//

OMM_ROUTINE_UPDATE(omm_save_file_auto_save) {
    if (sOmmSaveFileShouldWrite) {
        omm_save_file_write(OMM_SAVEFILE_NAME);
    }
}

//
// Complete save cheat
//

void omm_set_complete_save_file(s32 fileIndex, s32 modeIndex) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_FILE_INDEX(modeIndex, return);

    // Flags
    omm_save_file_set_flags(fileIndex, modeIndex, SAVE_FLAG_ALL_FLAGS);

    // Stars
    for (s32 levelNum = LEVEL_MIN; levelNum <= LEVEL_MAX; ++levelNum) {
        omm_save_file_set_star_flags(fileIndex, modeIndex, omm_level_get_course(levelNum) - 1, omm_stars_get_level_flags(levelNum, modeIndex));
    }

    // Cannons
    for (s32 courseIndex = COURSE_INDEX_MIN; courseIndex <= COURSE_INDEX_MAX; ++courseIndex) {
        omm_save_file_set_cannon_unlocked(fileIndex, modeIndex, courseIndex);
    }

    // Collectibles
#if OMM_GAME_IS_R96X
    for (s32 keyIndex = 0; keyIndex != NUM_KEYS; ++keyIndex) {
        omm_save_file_set_taken_luigi_key(fileIndex, modeIndex, keyIndex);
    }
    for (s32 coinIndex = 0; coinIndex != NUM_WARIO_COINS; ++coinIndex) {
        omm_save_file_set_taken_wario_coin(fileIndex, modeIndex, coinIndex);
    }
#endif

    // Captures
    omm_save_file_register_capture(fileIndex, modeIndex, omm_level_get_all_available_captures(modeIndex));

    // Unlock Peach
    if (!OMM_REWARD_IS_PLAYABLE_PEACH_UNLOCKED) {
        omm_sparkly_set_completed(OMM_SPARKLY_MODE_NORMAL);
    }
}

#pragma GCC pop_options

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_SETTINGS_FLAG   "[settings]"
#define OMM_SAVEFILE_NAME   "omm_save_file.txt"
#define OMM_SAVEFILE_BACKUP "omm_save_file.backup"

struct WarpCheckpoint gWarpCheckpoint;
u8 gLastCompletedCourseNum = LEVEL_CASTLE; // gLastCompletedLevelNum actually
u8 gLastCompletedStarNum = 0;
u8 gCurrCourseStarFlags = 0;
u8 gGotFileCoinHiScore = 0;
u8 gSpecialTripleJump = 0;
s8 gSaveFileModified = 0;

//
// Flags and courses
//

typedef struct {
    const char *name;
    u32 value;
} OmmNameValue;

static const OmmNameValue sOmmFlags[] = {
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
#define OMM_FLAGS_COUNT OMM_ARRAY_SIZE(sOmmFlags)

static const OmmNameValue sOmmCourses[] = {
    { "bob", COURSE_BOB },
    { "wf", COURSE_WF },
    { "jrb", COURSE_JRB },
    { "ccm", COURSE_CCM },
    { "bbh", COURSE_BBH },
    { "hmc", COURSE_HMC },
    { "lll", COURSE_LLL },
    { "ssl", COURSE_SSL },
    { "ddd", COURSE_DDD },
    { "sl", COURSE_SL },
    { "wdw", COURSE_WDW },
    { "ttm", COURSE_TTM },
    { "thi", COURSE_THI },
    { "ttc", COURSE_TTC },
    { "rr", COURSE_RR },
    { "bitdw", COURSE_BITDW },
    { "bitfs", COURSE_BITFS },
    { "bits", COURSE_BITS },
    { "pss", COURSE_PSS },
    { "cotmc", COURSE_COTMC },
    { "totwc", COURSE_TOTWC },
    { "vcutm", COURSE_VCUTM },
    { "wmotr", COURSE_WMOTR },
    { "sa", COURSE_SA },
    { "castle", COURSE_COUNT },
};
#define OMM_COURSES_COUNT OMM_ARRAY_SIZE(sOmmCourses)

//
// Collectibles
//

#define OMM_COLLECTIBLE_GET_B(collectibles, start, index)           (((collectibles) >> (start + index)) & 1)
#define OMM_COLLECTIBLE_SET_B(collectibles, start, index, value)    collectibles &= ~(1llu << ((start) + (index))); collectibles |= (((u64) (value)) << ((start) + (index)))
#define OMM_COLLECTIBLE_GET_I(collectibles, start, length)          (((collectibles) >> (start)) & ((1llu << (length)) - 1llu))
#define OMM_COLLECTIBLE_SET_I(collectibles, start, length, value)   collectibles &= ~(((1llu << (length)) - 1llu) << (start)); collectibles |= ((((u64) (value)) & ((1llu << (length)) - 1llu)) << (start))

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
    OmmSaveCourse courses[COURSE_COUNT];
} OmmSaveFile;

typedef struct {
    const char *name;
    s32 modes;
    s32 starsPerCourse;
    const char *collectibles;
    OmmSaveFile files[NUM_SAVE_FILES][2];
} OmmSaveBuffer;

static OmmSaveBuffer sOmmSaveBuffers[] = {
    { "smex", 1, 7, NULL, { { { 0 } } } },
    { "smms", 1, 7, NULL, { { { 0 } } } },
#if OMM_GAME_IS_R96A
    { "r96a", 1, 7, "b00" STRINGIFY(NUM_KEYS) "b" STRINGIFY(NUM_KEYS) STRINGIFY(NUM_WARIO_COINS), { { { 0 } } } },
#else
    { "r96a", 1, 7, "b0016b1616", { { { 0 } } } },
#endif
    { "xalo", 1, 7, NULL, { { { 0 } } } },
    { "sm74", 2, 7, NULL, { { { 0 } } } },
    { "smsr", 1, 7, NULL, { { { 0 } } } },
};

//
// Utils
//

#define sOmmCurrSaveFileIndex               (gCurrSaveFileNum - 1)
#define sOmmCurrCourseIndex                 (gCurrCourseNum - 1)
#define sOmmCurrSaveBuffer                  (&sOmmSaveBuffers[OMM_GAME_SAVE])
#define sOmmCurrSaveFile                    (&sOmmSaveBuffers[OMM_GAME_SAVE].files[sOmmCurrSaveFileIndex][OMM_GAME_MODE])

#define COURSE_CASTLE                       (COURSE_COUNT - 1)
#define CHECK_FILE_INDEX(i, fail)           if (i < 0 || i >= NUM_SAVE_FILES) { fail; }
#define CHECK_COURSE_INDEX(i, fail)         if (i < -1 || i >= COURSE_COUNT) { fail; }

//
// Read
//

typedef struct {
    char args[4][128];
} Token;

static Token tokenize(const char *buffer) {
    Token token; OMM_MEMSET(&token, 0, sizeof(token));
    char temp[128];
    s32 index = 0;
    s32 length = 0;
    s32 maxlen = strlen(buffer);
    for (s32 i = 0; i <= maxlen; ++i) {
        char c = buffer[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_')) {
            if (length < 127) {
                temp[length++] = c;
            }
        } else {
            if (length > 0) {
                OMM_MEMCPY(token.args[index++], temp, length);
                if (index >= 4) {
                    return token;
                }
            }
            length = 0;
        }
    }
    return token;
}

static void trim_left(char *buffer) {
    s32 l = strlen(buffer);
    s32 i = 0;
    while (buffer[i] != 0 && buffer[i] <= ' ') ++i;
    OMM_MEMMOV(buffer, buffer + i, l - i + 1);
}

void save_file_load_all() {
    static bool sSaveFileLoaded = false;
    static bool sSaveFileBackup = false;
    if (!sSaveFileLoaded) {
        OMM_STRING(filename, 256, "%s/%s", OMM_USER_FOLDER, (sSaveFileBackup ? OMM_SAVEFILE_BACKUP : OMM_SAVEFILE_NAME));
        FILE *f = fopen(filename, "r");
        bool loaded = false;
        if (f) {
            OmmSaveBuffer *saveBuffer = NULL;
            OmmSaveFile *saveFile = NULL;
            char buffer[1024];
            while (fgets(buffer, 1024, f)) {
                trim_left(buffer);
                if (buffer[0] != '#') {
                    Token token = tokenize(buffer);

                    // Settings flag, the savefile is successfully loaded
                    if (strstr(buffer, OMM_SETTINGS_FLAG)) {
                        loaded = true;
                    }

                    // Version
                    else if (buffer[0] == '[') {

                        // Buffer
                        for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmSaveBuffers); ++i) {
                            if (strcmp(sOmmSaveBuffers[i].name, token.args[0]) == 0) {

                                // File index
                                s32 fileIndex = token.args[1][0] - 'A';

                                // Mode index
                                s32 modeIndex = token.args[1][1] - '0';

                                // Save file
                                saveBuffer = &sOmmSaveBuffers[i];
                                saveFile = &saveBuffer->files[fileIndex][modeIndex];
                                saveFile->flags |= SAVE_FLAG_FILE_EXISTS;
                                break;
                            }
                        }
                    }

                    // Save data
                    else if (saveFile) {

                        // Settings data
                        READ_KBINDS(gOmmControlsButtonA, token.args);
                        READ_KBINDS(gOmmControlsButtonB, token.args);
                        READ_KBINDS(gOmmControlsButtonX, token.args);
                        READ_KBINDS(gOmmControlsButtonY, token.args);
                        READ_KBINDS(gOmmControlsButtonStart, token.args);
                        READ_KBINDS(gOmmControlsTriggerL, token.args);
                        READ_KBINDS(gOmmControlsTriggerR, token.args);
                        READ_KBINDS(gOmmControlsTriggerZ, token.args);
                        READ_KBINDS(gOmmControlsCUp, token.args);
                        READ_KBINDS(gOmmControlsCDown, token.args);
                        READ_KBINDS(gOmmControlsCLeft, token.args);
                        READ_KBINDS(gOmmControlsCRight, token.args);
                        READ_KBINDS(gOmmControlsDUp, token.args);
                        READ_KBINDS(gOmmControlsDDown, token.args);
                        READ_KBINDS(gOmmControlsDLeft, token.args);
                        READ_KBINDS(gOmmControlsDRight, token.args);
                        READ_KBINDS(gOmmControlsStickUp, token.args);
                        READ_KBINDS(gOmmControlsStickDown, token.args);
                        READ_KBINDS(gOmmControlsStickLeft, token.args);
                        READ_KBINDS(gOmmControlsStickRight, token.args);
                        READ_CHOICE(gOmmFPS, token.args);
                        READ_CHOICE_SC(gOmmCharacter, token.args);
                        READ_CHOICE_SC(gOmmMovesetType, token.args);
                        READ_CHOICE_SC(gOmmCapType, token.args);
                        READ_CHOICE_SC(gOmmStarsMode, token.args);
                        READ_CHOICE_SC(gOmmPowerUpsType, token.args);
                        READ_CHOICE_SC(gOmmCameraMode, token.args);
                        READ_CHOICE_SC(gOmmSparklyStarsMode, token.args);
                        READ_TOGGLE_SC(gOmmSparklyStarsAssist, token.args);
                        READ_TOGGLE_SC(gOmmSparklyStarsHint, token.args);
                        READ_CHOICE(gOmmExtrasMarioColors, token.args);
                        READ_CHOICE(gOmmExtrasPeachColors, token.args);
                        READ_TOGGLE_SC(gOmmExtrasSMOAnimations, token.args);
                        READ_TOGGLE_SC(gOmmExtrasCappyAndTiara, token.args);
                        READ_TOGGLE_SC(gOmmExtrasColoredStars, token.args);
                        READ_TOGGLE_SC(gOmmExtrasVanishingHUD, token.args);
                        READ_TOGGLE_SC(gOmmExtrasRevealSecrets, token.args);
                        READ_TOGGLE_SC(gOmmExtrasRedCoinsRadar, token.args);
                        READ_TOGGLE_SC(gOmmExtrasShowStarNumber, token.args);
                        READ_TOGGLE_SC(gOmmExtrasInvisibleMode, token.args);
#if OMM_CODE_DEV
                        READ_TOGGLE_SC(gOmmExtrasRender96Peach, token.args);
#endif
                        READ_TOGGLE_SC(gOmmExtrasCrystalStarsReward, token.args);
                        READ_TOGGLE_SC(gOmmExtrasNebulaStarsReward, token.args);
#if OMM_CODE_DEBUG
                        READ_TOGGLE_SC(gOmmDebugHitbox, token.args);
                        READ_TOGGLE_SC(gOmmDebugHurtbox, token.args);
                        READ_TOGGLE_SC(gOmmDebugWallbox, token.args);
                        READ_TOGGLE_SC(gOmmDebugSurface, token.args);
                        READ_TOGGLE_SC(gOmmDebugMario, token.args);
                        READ_TOGGLE_SC(gOmmDebugCappy, token.args);
                        READ_TOGGLE_SC(gOmmDebugProfiler, token.args);
#if OMM_CODE_DEV_DEBUG
                        READ_TOGGLE_SC(gOmmDebugGameSpeedEnable, token.args);
                        READ_TOGGLE_SC(gOmmDebugGameSpeedFps, token.args);
#endif
#endif
                
                        // Sparkly Stars data
                        if (omm_ssd_read(OMM_ARRAY_OF(const char*) { token.args[0], token.args[1] })) {
                            continue;
                        }

                        // Mario colors
                        if (omm_mario_colors_read(OMM_ARRAY_OF(const char*) { token.args[0], token.args[1] })) {
                            continue;
                        }

                        // Collectibles
                        if (strcmp(token.args[0], "collectibles") == 0) {
                            if (saveBuffer->collectibles) {
                                saveFile->collectibles = 0;
                                s32 numFields = strlen(saveBuffer->collectibles) / 5;
                                for (s32 i = 0; i != numFields; ++i) {
                                    char fieldType = saveBuffer->collectibles[5 * i + 0];
                                    s32 fieldStart = (((saveBuffer->collectibles[5 * i + 1] - '0') * 10) + (saveBuffer->collectibles[5 * i + 2] - '0'));
                                    s32 fieldLength = (((saveBuffer->collectibles[5 * i + 3] - '0') * 10) + (saveBuffer->collectibles[5 * i + 4] - '0'));
                                    const char *field = token.args[1 + i];
                                    switch (fieldType) {
                                        case 'i': {
                                            u64 x = 0;
                                            sscanf(field, "%llu", &x);
                                            OMM_COLLECTIBLE_SET_I(saveFile->collectibles, fieldStart, fieldLength, x);
                                        } break;

                                        case 'b': {
                                            for (s32 j = 0; j != fieldLength; ++j) {
                                                OMM_COLLECTIBLE_SET_B(saveFile->collectibles, fieldStart, j, field[j] == '1');
                                            }
                                        } break;
                                    }
                                }
                            }
                        }

                        // Flag
                        for (s32 i = 0; i != OMM_FLAGS_COUNT; ++i) {
                            if (strcmp(sOmmFlags[i].name, token.args[0]) == 0) {
                                saveFile->flags |= ((token.args[1][0] == '1') ? sOmmFlags[i].value : 0);
                                break;
                            }
                        }

                        // Course
                        for (s32 i = 0; i != OMM_COURSES_COUNT; ++i) {
                            if (strcmp(sOmmCourses[i].name, token.args[0]) == 0) {
                                s32 courseIndex = sOmmCourses[i].value - 1;

                                // Stars
                                for (s32 b = 0; b != saveBuffer->starsPerCourse; ++b) {
                                    saveFile->courses[courseIndex].stars |= ((token.args[1][b] == '1') << b);
                                }

                                // Cannon
                                saveFile->courses[courseIndex].cannon = (token.args[2][0] == '1');

                                // Score
                                sscanf(token.args[3], "%hd", &saveFile->courses[courseIndex].score);
                                break;
                            }
                        }
                    }
                }
            }
            fclose(f);
            controller_reconfigure();
        }

        // Something went wrong... load the backup!
        if (!loaded && !sSaveFileBackup) {
            sSaveFileBackup = true;
            save_file_load_all();
        }
        
        // Everything is ok
        else {

            // The backup was loaded, regenerate the save file
            if (loaded && sSaveFileBackup) {
                OMM_STRING(backupFilename, 256, "%s/%s", OMM_USER_FOLDER, OMM_SAVEFILE_BACKUP);
                FILE *backup = fopen(backupFilename, "rb");
                if (backup) {
                    OMM_STRING(saveFilename, 256, "%s/%s", OMM_USER_FOLDER, OMM_SAVEFILE_NAME);
                    FILE *save = fopen(saveFilename, "wb");
                    if (save) {
                        char buffer[0x1000]; s32 length = 0;
                        while ((length = (s32) fread(buffer, sizeof(char), 0x1000, backup)) != 0) {
                            fwrite(buffer, sizeof(char), length, save);
                        }
                        fclose(save);
                    }
                    fclose(backup);
                }
            }

            // Load complete
            sSaveFileLoaded = true;
            gSaveFileModified = false;
        }
    }
}

void save_file_reload() {
}

//
// Write
//

static bool save_file_is_empty(const OmmSaveFile *savefile) {
    static const OmmSaveFile sOmmEmptySaveFile = { 0, 0, { { 0, 0, 0 } } };
    return OMM_MEMCMP(savefile, &sOmmEmptySaveFile, sizeof(sOmmEmptySaveFile));
}

#define write(...) { head += sprintf(head, __VA_ARGS__); }
static void save_file_write() {
    char saveFileBuffer[0x10000];
    char *head = saveFileBuffer;

    // Set current save file as existing if not empty (i.e. at least 1 star, 1 collectible or 1 in-game flag)
    s32 fileIndex = (omm_is_main_menu() ? -1 : sOmmCurrSaveFileIndex);
    if (fileIndex >= 0 && fileIndex < NUM_SAVE_FILES) {
        if (!save_file_is_empty(&sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE])) {
            sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].flags |= SAVE_FLAG_FILE_EXISTS;
        }
    }

    // Header
    write("# Odyssey Mario's Moveset save file\n");
    write("# By PeachyPeach\n");
    write("# Comment starts with #\n");
    write("# All versions saves are stored in this file\n");
    write("# The syntax is the following:\n");
    write("# - Every version save starts with a pair [x:y]\n");
    write("#   - x is the version name\n");
    write("#   - y is the save file name\n");
    write("# - For flags, 0 = locked and 1 = unlocked\n");
    write("# - For courses,\n");
    write("#   - the first 7 numbers are the stars (1 = collected, 0 = not)\n");
    write("#   - the next number is the cannon (1 = open, 0 = close)\n");
    write("#   - the last number is the coin score\n");
    write("# - For collectibles, it depends on the version\n");
    write("#   - For Render96-alpha:\n");
    write("#     - the first number is the selected character (0 = Mario, 1 = Luigi, 2 = Wario)\n");
    write("#     - the next 10 numbers are the Luigi keys (1 = collected, 0 = not)\n");
    write("#     - the last 6 numbers are the Wario coins (1 = collected, 0 = not)\n");
    write("\n");

    // Versions
    for (s32 s = 0; s != OMM_ARRAY_SIZE(sOmmSaveBuffers); ++s) {
        const OmmSaveBuffer *saveBuffer = &sOmmSaveBuffers[s];
        for (s32 i = 0; i != NUM_SAVE_FILES; ++i) {
            for (s32 j = 0; j != saveBuffer->modes; ++j) {
                const OmmSaveFile *saveFile = &saveBuffer->files[i][j];
                if (saveFile->flags & SAVE_FLAG_FILE_EXISTS) {

                    // Name
                    write("[%s:%c%d]\n", saveBuffer->name, i + 'A', j);

                    // Flags
                    for (s32 k = 0; k != OMM_FLAGS_COUNT; ++k) {
                        write("%s = %d\n", sOmmFlags[k].name, ((saveFile->flags & sOmmFlags[k].value) != 0));
                    }

                    // Courses
                    for (s32 k = 0; k != OMM_COURSES_COUNT; ++k) {
                        write("%s = ", sOmmCourses[k].name);
                        s32 courseIndex = sOmmCourses[k].value - 1;

                        // Stars
                        for (s32 b = 0; b != saveBuffer->starsPerCourse; ++b) {
                            write("%d", ((saveFile->courses[courseIndex].stars >> b) & 1));
                        }

                        // Cannon
                        write(" %d", (saveFile->courses[courseIndex].cannon != 0));

                        // Score
                        write(" %hd\n", saveFile->courses[courseIndex].score);
                    }

                    // Collectibles
                    if (saveBuffer->collectibles) {
                        write("collectibles =");
                        s32 numFields = strlen(saveBuffer->collectibles) / 5;
                        for (s32 i = 0; i != numFields; ++i) {
                            char fieldType = saveBuffer->collectibles[5 * i + 0];
                            s32 fieldStart = (((saveBuffer->collectibles[5 * i + 1] - '0') * 10) + (saveBuffer->collectibles[5 * i + 2] - '0'));
                            s32 fieldLength = (((saveBuffer->collectibles[5 * i + 3] - '0') * 10) + (saveBuffer->collectibles[5 * i + 4] - '0'));
                            switch (fieldType) {
                                case 'i': {
                                    u64 x = OMM_COLLECTIBLE_GET_I(saveFile->collectibles, fieldStart, fieldLength);
                                    write(" %llu", x);
                                } break;

                                case 'b': {
                                    write(" ");
                                    for (s32 j = 0; j != fieldLength; ++j) {
                                        s32 bit = (s32) OMM_COLLECTIBLE_GET_B(saveFile->collectibles, fieldStart, j);
                                        write("%d", bit);
                                    }
                                } break;
                            }
                        }
                        write("\n");
                    }
                    write("\n");
                }
            }
        }
    }

    // Sparkly Stars data
    omm_ssd_write(&head);

    // Mario colors
    omm_mario_colors_write(&head);
        
    // Settings data
    write(OMM_SETTINGS_FLAG "\n");
    WRITE_KBINDS(gOmmControlsButtonA);
    WRITE_KBINDS(gOmmControlsButtonB);
    WRITE_KBINDS(gOmmControlsButtonX);
    WRITE_KBINDS(gOmmControlsButtonY);
    WRITE_KBINDS(gOmmControlsButtonStart);
    WRITE_KBINDS(gOmmControlsTriggerL);
    WRITE_KBINDS(gOmmControlsTriggerR);
    WRITE_KBINDS(gOmmControlsTriggerZ);
    WRITE_KBINDS(gOmmControlsCUp);
    WRITE_KBINDS(gOmmControlsCDown);
    WRITE_KBINDS(gOmmControlsCLeft);
    WRITE_KBINDS(gOmmControlsCRight);
    WRITE_KBINDS(gOmmControlsDUp);
    WRITE_KBINDS(gOmmControlsDDown);
    WRITE_KBINDS(gOmmControlsDLeft);
    WRITE_KBINDS(gOmmControlsDRight);
    WRITE_KBINDS(gOmmControlsStickUp);
    WRITE_KBINDS(gOmmControlsStickDown);
    WRITE_KBINDS(gOmmControlsStickLeft);
    WRITE_KBINDS(gOmmControlsStickRight);
    WRITE_CHOICE(gOmmFPS);
    WRITE_CHOICE_SC(gOmmCharacter);
    WRITE_CHOICE_SC(gOmmMovesetType);
    WRITE_CHOICE_SC(gOmmCapType);
    WRITE_CHOICE_SC(gOmmStarsMode);
    WRITE_CHOICE_SC(gOmmPowerUpsType);
    WRITE_CHOICE_SC(gOmmCameraMode);
    WRITE_CHOICE_SC(gOmmSparklyStarsMode);
    WRITE_TOGGLE_SC(gOmmSparklyStarsAssist);
    WRITE_TOGGLE_SC(gOmmSparklyStarsHint);
    WRITE_CHOICE(gOmmExtrasMarioColors);
    WRITE_CHOICE(gOmmExtrasPeachColors);
    WRITE_TOGGLE_SC(gOmmExtrasSMOAnimations);
    WRITE_TOGGLE_SC(gOmmExtrasCappyAndTiara);
    WRITE_TOGGLE_SC(gOmmExtrasColoredStars);
    WRITE_TOGGLE_SC(gOmmExtrasVanishingHUD);
    WRITE_TOGGLE_SC(gOmmExtrasRevealSecrets);
    WRITE_TOGGLE_SC(gOmmExtrasRedCoinsRadar);
    WRITE_TOGGLE_SC(gOmmExtrasShowStarNumber);
    WRITE_TOGGLE_SC(gOmmExtrasInvisibleMode);
#if OMM_CODE_DEV
    WRITE_TOGGLE_SC(gOmmExtrasRender96Peach);
#endif
    WRITE_TOGGLE_SC(gOmmExtrasCrystalStarsReward);
    WRITE_TOGGLE_SC(gOmmExtrasNebulaStarsReward);
#if OMM_CODE_DEBUG
    WRITE_TOGGLE_SC(gOmmDebugHitbox);
    WRITE_TOGGLE_SC(gOmmDebugHurtbox);
    WRITE_TOGGLE_SC(gOmmDebugWallbox);
    WRITE_TOGGLE_SC(gOmmDebugSurface);
    WRITE_TOGGLE_SC(gOmmDebugMario);
    WRITE_TOGGLE_SC(gOmmDebugCappy);
    WRITE_TOGGLE_SC(gOmmDebugProfiler);
#if OMM_CODE_DEV_DEBUG
    WRITE_TOGGLE_SC(gOmmDebugGameSpeedEnable);
    WRITE_TOGGLE_SC(gOmmDebugGameSpeedFps);
#endif
#endif
    write("\n");

    // Create a back-up
    OMM_STRING(backupFilename, 256, "%s/%s", OMM_USER_FOLDER, OMM_SAVEFILE_BACKUP);
    FILE *backup = fopen(backupFilename, "wb");
    if (backup) {
        fwrite(saveFileBuffer, sizeof(char), head - saveFileBuffer, backup);
        fclose(backup);
    }

    // Write the save file
    OMM_STRING(saveFilename, 256, "%s/%s", OMM_USER_FOLDER, OMM_SAVEFILE_NAME);
    FILE *save = fopen(saveFilename, "wb");
    if (save) {
        fwrite(saveFileBuffer, sizeof(char), head - saveFileBuffer, save);
        fclose(save);
    }

    // Done
    controller_reconfigure();
    gSaveFileModified = false;
}

void save_file_do_save(UNUSED s32 fileIndex) {
    gSaveFileModified = true;
}

//
// Copy
//

BAD_RETURN(s32) save_file_copy(s32 srcFileIndex, s32 dstFileIndex) {
    CHECK_FILE_INDEX(srcFileIndex, return);
    CHECK_FILE_INDEX(dstFileIndex, return);
    OMM_MEMCPY(&sOmmCurrSaveBuffer->files[dstFileIndex], &sOmmCurrSaveBuffer->files[srcFileIndex], sizeof(sOmmCurrSaveBuffer->files[dstFileIndex]));
    gSaveFileModified = true;
}

//
// Erase
//

void save_file_erase(s32 fileIndex) {
    CHECK_FILE_INDEX(fileIndex, return);
    OMM_MEMSET(&sOmmCurrSaveBuffer->files[fileIndex], 0, sizeof(sOmmCurrSaveBuffer->files[fileIndex]));
    gSaveFileModified = true;
}

//
// Get
//

s32 save_file_exists(s32 fileIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    return (sOmmCurrSaveBuffer->files[fileIndex][0].flags & SAVE_FLAG_FILE_EXISTS) ||
           (sOmmCurrSaveBuffer->files[fileIndex][1].flags & SAVE_FLAG_FILE_EXISTS);
}

u32 save_file_get_flags() {
    if (gCurrCreditsEntry || gCurrDemoInput) return 0;
    return (sOmmCurrSaveFile->flags);
}

u32 save_file_get_star_flags(s32 fileIndex, s32 courseIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_COURSE_INDEX(courseIndex, return 0);
    return (sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].courses[(courseIndex == -1) ? COURSE_CASTLE : courseIndex].stars);
}

u32 save_file_get_cannon_flags(s32 fileIndex, s32 courseIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_COURSE_INDEX(courseIndex, return 0);
    return (sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].courses[(courseIndex == -1) ? COURSE_CASTLE : courseIndex].cannon);
}

s32 save_file_is_cannon_unlocked() {
    return save_file_get_cannon_flags(sOmmCurrSaveFileIndex, sOmmCurrCourseIndex) != 0;
}

s32 save_file_get_course_star_count(s32 fileIndex, s32 courseIndex) {
    s32 count = 0;
    u8 starFlags = save_file_get_star_flags(fileIndex, courseIndex);
    for (s32 i = 0; i != sOmmCurrSaveBuffer->starsPerCourse; ++i) {
        count += ((starFlags >> i) & 1);
    }
    return count;
}

s32 save_file_get_total_star_count(s32 fileIndex, s32 minCourse, s32 maxCourse) {
    s32 count = 0;
    for (s32 i = minCourse; i <= maxCourse; ++i) {
        count += save_file_get_course_star_count(fileIndex, i);
    }
    if (maxCourse < COURSE_CASTLE) {
        count += save_file_get_course_star_count(fileIndex, -1);
    }
    return count;
}

s32 save_file_get_course_coin_score(s32 fileIndex, s32 courseIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    CHECK_COURSE_INDEX(courseIndex, return 0);
    return sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].courses[courseIndex].score;
}

u32 save_file_get_max_coin_score(s32 courseIndex) {
    s32 maxCoinScore = -1;
    s32 maxScoreFileNum = 0;
    for (s32 i = 0; i != NUM_SAVE_FILES; ++i) {
        if (save_file_get_star_flags(i, courseIndex) != 0) {
            s32 coinScore = save_file_get_course_coin_score(i, courseIndex);
            if (coinScore > maxCoinScore) {
                maxCoinScore = coinScore;
                maxScoreFileNum = i + 1;
            }
        }
    }
    return (maxScoreFileNum << 16) + max_s(maxCoinScore, 0);
}

s32 save_file_get_cap_pos(UNUSED Vec3s capPos) {
    return FALSE;
}

u16 save_file_get_sound_mode() {
    return 0;
}

//
// Set
//

void save_file_set_flags(u32 flags) {
    sOmmCurrSaveFile->flags |= flags;
    gSaveFileModified = true;
}

void save_file_clear_flags(u32 flags) {
    sOmmCurrSaveFile->flags &= ~flags;
    sOmmCurrSaveFile->flags |= SAVE_FLAG_FILE_EXISTS;
    gSaveFileModified = true;
}

void save_file_set_star_flags(s32 fileIndex, s32 courseIndex, u32 starFlags) {
    CHECK_FILE_INDEX(fileIndex, return);
    CHECK_COURSE_INDEX(courseIndex, return);
    sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].courses[(courseIndex == -1) ? COURSE_CASTLE : courseIndex].stars |= starFlags;
    gSaveFileModified = true;
}

void save_file_collect_star_or_key(s16 coinScore, s16 starIndex) {
    gLastCompletedCourseNum = OMM_BOWSER_IN_THE_LEVEL(gCurrLevelNum);
    gLastCompletedStarNum = starIndex + 1;
    gGotFileCoinHiScore = 0;
    gSaveFileModified = true;

    // Star flag
    s32 starFlag = (1 << starIndex);
    omm_stars_set_bits(starFlag);
    switch (gCurrLevelNum) {
        case LEVEL_BOWSER_1: {
            save_file_set_flags(SAVE_FLAG_HAVE_KEY_1);
            omm_speedrun_split(-1);
        } break;

        case LEVEL_BOWSER_2: {
            save_file_set_flags(SAVE_FLAG_HAVE_KEY_2);
            omm_speedrun_split(-1);
        } break;

        case LEVEL_BOWSER_3: {
            save_file_set_flags(0);
            omm_speedrun_split(-1);
        } break;

        default: {
            save_file_set_star_flags(sOmmCurrSaveFileIndex, sOmmCurrCourseIndex, starFlag);
            omm_speedrun_split(save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1));
        } break;
    }

    // New high score
    if (coinScore > save_file_get_course_coin_score(sOmmCurrSaveFileIndex, sOmmCurrCourseIndex)) {
        sOmmCurrSaveFile->courses[sOmmCurrCourseIndex].score = coinScore;
        gGotFileCoinHiScore = 1;
    }
}

void save_file_set_cannon_unlocked() {
    sOmmCurrSaveFile->courses[sOmmCurrCourseIndex].cannon = 1;
    gSaveFileModified = true;
}

void save_file_set_cap_pos(UNUSED s16 x, UNUSED s16 y, UNUSED s16 z) {
}

void save_file_set_sound_mode(UNUSED u16 mode) {
}

void save_file_move_cap_to_default_location() {
}

#if OMM_GAME_IS_R96A

//
// Luigi keys and Wario coins
//

s32 save_file_taken_key(s32 fileIndex, s32 keyId) {
    CHECK_FILE_INDEX(fileIndex, return FALSE);
    return OMM_COLLECTIBLE_GET_B(sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].collectibles, 0, keyId);
}

void save_file_register_key(s32 fileIndex, s32 keyId) {
    CHECK_FILE_INDEX(fileIndex, return);
    OMM_COLLECTIBLE_SET_B(sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].collectibles, 0, keyId, TRUE);
    gSaveFileModified = true;
}

s32 save_file_get_keys(s32 fileIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    s32 keys = 0;
    for (s32 i = 0; i != NUM_KEYS; ++i) {
        keys += OMM_COLLECTIBLE_GET_B(sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].collectibles, 0, i);
    }
    return keys;
}

s32 save_file_taken_wario_coin(s32 fileIndex, s32 coinId) {
    CHECK_FILE_INDEX(fileIndex, return FALSE);
    return OMM_COLLECTIBLE_GET_B(sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].collectibles, NUM_KEYS, coinId);
}

void save_file_register_wario_coin(s32 fileIndex, s32 coinId) {
    CHECK_FILE_INDEX(fileIndex, return);
    OMM_COLLECTIBLE_SET_B(sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].collectibles, NUM_KEYS, coinId, TRUE);
    gSaveFileModified = true;
}

s32 save_file_get_wario_coins(s32 fileIndex) {
    CHECK_FILE_INDEX(fileIndex, return 0);
    s32 coins = 0;
    for (s32 i = 0; i != NUM_WARIO_COINS; ++i) {
        coins += OMM_COLLECTIBLE_GET_B(sOmmCurrSaveBuffer->files[fileIndex][OMM_GAME_MODE].collectibles, NUM_KEYS, i);
    }
    return coins;
}

void save_file_update_player_model(UNUSED s32 fileIndex, UNUSED s32 character) {
}

s32 save_file_get_player_model(UNUSED s32 fileIndex) {
    return 0;
}

#endif

//
// Warp checkpoint
//

void disable_warp_checkpoint() {
    gWarpCheckpoint.courseNum = COURSE_NONE;
}

void check_if_should_set_warp_checkpoint(struct WarpNode *warpNode) {
    if (warpNode->destLevel & 0x80) {
        gWarpCheckpoint.actNum = gCurrActNum;
        gWarpCheckpoint.courseNum = gCurrCourseNum;
        gWarpCheckpoint.levelID = warpNode->destLevel & 0x7F;
        gWarpCheckpoint.areaNum = warpNode->destArea;
        gWarpCheckpoint.warpNode = warpNode->destNode;
    }
}

s32 check_warp_checkpoint(struct WarpNode *warpNode) {
    if (warpNode) {
        s16 currCourseNum = gLevelToCourseNumTable[(warpNode->destLevel & 0x7F) - 1];
        if ((gWarpCheckpoint.courseNum != COURSE_NONE) &&
            (gWarpCheckpoint.actNum == gCurrActNum) &&
            (gSavedCourseNum == currCourseNum)) {
            warpNode->destLevel = gWarpCheckpoint.levelID;
            warpNode->destArea = gWarpCheckpoint.areaNum;
            warpNode->destNode = gWarpCheckpoint.warpNode;
            return TRUE;
        }
    }
    gWarpCheckpoint.courseNum = COURSE_NONE;
    return FALSE;
}

//
// Auto-save
//

OMM_ROUTINE_UPDATE(omm_auto_save) {
    if (gSaveFileModified) {
        save_file_write();
    }
}

//
// Complete save cheat
//

void omm_set_complete_save_file(s32 fileIndex) {
    gCurrSaveFileNum = fileIndex + 1;
    for (gCurrAreaIndex = 1; gCurrAreaIndex <= DEF(1, 1, 1, 1, 2, 1); ++gCurrAreaIndex) {
        sWarpDest.areaIdx = (u8) gCurrAreaIndex;

        // Flags
        save_file_set_flags(SAVE_FLAG_FILE_EXISTS);
        for (s32 i = 0; i != OMM_FLAGS_COUNT; ++i) {
            save_file_set_flags(sOmmFlags[i].value);
        }

        // Stars
        for (s32 i = LEVEL_MIN; i <= LEVEL_MAX; ++i) {
            if (i != LEVEL_ENDING) {
                save_file_set_star_flags(fileIndex, omm_level_get_course(i) - 1, omm_stars_get_bits_total(i));
            }
        }

        // Cannons
        for (gCurrCourseNum = COURSE_MIN; gCurrCourseNum != COURSE_MAX; ++gCurrCourseNum) {
            save_file_set_cannon_unlocked();
        }

        // Collectibles
#if OMM_GAME_IS_R96A
        for (s32 i = 0; i != NUM_KEYS; ++i) {
            save_file_register_key(fileIndex, i);
        }
        for (s32 i = 0; i != NUM_WARIO_COINS; ++i) {
            save_file_register_wario_coin(fileIndex, i);
        }
#endif
    }
    gCurrAreaIndex = 1;
    sWarpDest.areaIdx = 1;

    // Unlock Peach
    // That's the "unlock Peach" cheat code that only works with OMM save files :)
    if (!omm_player_is_unlocked(OMM_PLAYER_PEACH)) {
        omm_ssd_read(OMM_ARRAY_OF(const char *) { "sparkly_stars", "XgU2yLgIIM1ihxJ8tC5qV35jzIrc5FoEXqezDLDUhBVCVF12sKsYkuPPgtoGfHk4UhwmwOF1d73inOZA" });
    }
}

#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// String conversion (very basic)
//

#if !OMM_GAME_IS_R96X
static const struct { const char *str; u8 c64; } sSm64CharMap[] = {
    { "0",   0x00 }, // 0
    { "1",   0x01 }, // 1
    { "2",   0x02 }, // 2
    { "3",   0x03 }, // 3
    { "4",   0x04 }, // 4
    { "5",   0x05 }, // 5
    { "6",   0x06 }, // 6
    { "7",   0x07 }, // 7
    { "8",   0x08 }, // 8
    { "9",   0x09 }, // 9
    { "A",   0x0A }, // A
    { "B",   0x0B }, // B
    { "C",   0x0C }, // C
    { "D",   0x0D }, // D
    { "E",   0x0E }, // E
    { "F",   0x0F }, // F
    { "G",   0x10 }, // G
    { "H",   0x11 }, // H
    { "I",   0x12 }, // I
    { "J",   0x13 }, // J
    { "K",   0x14 }, // K
    { "L",   0x15 }, // L
    { "M",   0x16 }, // M
    { "N",   0x17 }, // N
    { "O",   0x18 }, // O
    { "P",   0x19 }, // P
    { "Q",   0x1A }, // Q
    { "R",   0x1B }, // R
    { "S",   0x1C }, // S
    { "T",   0x1D }, // T
    { "U",   0x1E }, // U
    { "V",   0x1F }, // V
    { "W",   0x20 }, // W
    { "X",   0x21 }, // X
    { "Y",   0x22 }, // Y
    { "Z",   0x23 }, // Z
    { "a",   0x24 }, // a
    { "b",   0x25 }, // b
    { "c",   0x26 }, // c
    { "d",   0x27 }, // d
    { "e",   0x28 }, // e
    { "f",   0x29 }, // f
    { "g",   0x2A }, // g
    { "h",   0x2B }, // h
    { "i",   0x2C }, // i
    { "j",   0x2D }, // j
    { "k",   0x2E }, // k
    { "l",   0x2F }, // l
    { "m",   0x30 }, // m
    { "n",   0x31 }, // n
    { "o",   0x32 }, // o
    { "p",   0x33 }, // p
    { "q",   0x34 }, // q
    { "r",   0x35 }, // r
    { "s",   0x36 }, // s
    { "t",   0x37 }, // t
    { "u",   0x38 }, // u
    { "v",   0x39 }, // v
    { "w",   0x3A }, // w
    { "x",   0x3B }, // x
    { "y",   0x3C }, // y
    { "z",   0x3D }, // z
    { "\'",  0x3E }, // '
    { ".",   0x3F }, // .
    { "^",   0x50 }, // arrow up
    { "|",   0x51 }, // arrow down
    { "<",   0x52 }, // arrow left
    { ">",   0x53 }, // arrow right
    { "[A]", 0x54 }, // A button
    { "[B]", 0x55 }, // B button
    { "[C]", 0x56 }, // C button
    { "[Z]", 0x57 }, // Z button
    { "[R]", 0x58 }, // R button
    { ",",   0x6F }, // ,
    { " ",   0x9E }, // space
    { "-",   0x9F }, // -
    { "/",   0xD0 }, // /
    { "[%]", 0xE0 }, // 
    { "(",   0xE1 }, // (
    { ")(",  0xE2 }, // )(
    { ")",   0xE3 }, // )
    { "+",   0xE4 }, // double arrow
    { "&",   0xE5 }, // &
    { ":",   0xE6 }, // :
    { "!",   0xF2 }, // !
    { "%",   0xF3 }, // %
    { "?",   0xF4 }, // ?
    { "~",   0xF7 }, // ~
    { "$",   0xF9 }, // coin
    { "@",   0xFA }, // star filled
    { "*",   0xFB }, // multiply
    { "=",   0xFD }, // star empty
    { "\n",  0xFE }, // new line
    { NULL,  0xFF }, // terminator
};

static const char *omm_text_add_char(u8 *str64, const char *str, s32 *i) {
    for (s32 j = 0; sSm64CharMap[j].str; ++j) {
        if (strstr(str, sSm64CharMap[j].str) == str) {
            str64[(*i)++] = sSm64CharMap[j].c64;
            return str + strlen(sSm64CharMap[j].str);
        }
    }
    str64[(*i)++] = 0x9E;
    return str + 1;
}
#endif

static u8 *omm_text_alloc(s32 length, bool heapAlloc) {
    u8 *allocated = omm_memory_new(heapAlloc ? NULL : gOmmMemoryPoolStrings, length + 1, NULL);
    omm_set(allocated, 0xFF, length + 1);
    return allocated;
}

u8 *omm_text_convert(const char *str, bool heapAlloc) {
    u8 *str64 = NULL;
#if OMM_GAME_IS_R96X
    u8 *translated = get_key_string(str);
    if (translated == get_language()->none) {
        translated = getTranslatedText((char *) str);
        s32 length = omm_text_length(translated);
        str64 = omm_copy(omm_text_alloc(length, heapAlloc), translated, length);
        free(translated);
    } else {
        s32 length = omm_text_length(translated);
        str64 = omm_copy(omm_text_alloc(length, heapAlloc), translated, length);
    }
#else
    s32 length = strlen(str);
    str64 = omm_text_alloc(length, heapAlloc);
    for (s32 i = 0;;) {
        if (*str == 0) {
            str64[i] = 0xFF;
            break;
        }
        str = omm_text_add_char(str64, str, &i);
    }
#endif
    return str64;
}

u8 *omm_text_copy(const u8 *str64, bool heapAlloc) {
    s32 length = omm_text_length(str64);
    u8 *strCopy = omm_text_alloc(length, heapAlloc);
    omm_copy(strCopy, str64, length);
    return strCopy;
}

u8 *omm_text_capitalize(u8 *str64) {
    for (u8 *p = str64; *p != 0xFF; ++p) {
        if (*p >= 0x24 && *p <= 0x3D) {
            *p -= 26;
        }
    }
    return str64;
}

u8 *omm_text_decapitalize(u8 *str64) {
    bool wasSpace = true;
    for (u8 *p = str64; *p != 0xFF; ++p) {
        if (*p >= 0x0A && *p <= 0x23) {
            if (wasSpace) wasSpace = false;
            else *p += 26;
        } else if (*p >= 0x3F) {
            wasSpace = true;
        }
    }
    return str64;
}

u8 *omm_text_replace_char(u8 *str64, u8 from, u8 to) {
    for (u8 *p = str64; *p != 0xFF; ++p) {
        if (*p == from) {
            *p = to;
        }
    }
    return str64;
}

static s32 omm_text_length_to_skip(const u8 *str64) {
    static struct { const char *str; const u8 *str64; s32 length; } sTextToSkip[] = {
        { "Mario 64", NULL, 0 },
        { "Super Mario", NULL, 0 },
        { "Mario is red", NULL, 0 },
    };
    for (s32 i = 0; i != (s32) omm_static_array_length(sTextToSkip); ++i) {
        if (!sTextToSkip[i].str64) {
            sTextToSkip[i].str64 = omm_text_convert(sTextToSkip[i].str, true);
            sTextToSkip[i].length = omm_text_length(sTextToSkip[i].str64);
        }
        if (omm_same(str64, sTextToSkip[i].str64, sTextToSkip[i].length)) {
            return sTextToSkip[i].length;
        }
    }
    return 0;
}

static void omm_text_replace(u8 *str64, const char *from, const char *to) {
    const u8 *from64 = omm_text_convert(from, false);
    const u8 *to64 = omm_text_convert(to, false);
    s32 lenFrom = omm_text_length(from64);
    s32 lenCurr = omm_text_length(str64);
    for (; lenCurr >= lenFrom;) {
        s32 lenToSkip = omm_text_length_to_skip(str64);
        if (lenToSkip != 0) {
            str64 += lenToSkip;
            lenCurr -= lenToSkip;
        } else if (omm_same(str64, from64, lenFrom)) {
            omm_copy(str64, to64, lenFrom);
            str64 += lenFrom;
            lenCurr -= lenFrom;
        } else {
            str64++;
            lenCurr--;
        }
    }
}

static void omm_text_replace_mario_by_mario(UNUSED u8 *str64) {
}

static void omm_text_replace_mario_by_peach(u8 *str64) {
    omm_text_replace(str64, OMM_TEXT_MARIO, OMM_TEXT_PEACH);
    omm_text_replace(str64, OMM_TEXT_MARIO_UPPER, OMM_TEXT_PEACH_UPPER);
    omm_text_replace(str64, OMM_TEXT_MARIO_LOWER, OMM_TEXT_PEACH_LOWER);
    omm_text_replace(str64, OMM_TEXT_CAPPY, OMM_TEXT_TIARA);
    omm_text_replace(str64, OMM_TEXT_CAPPY_UPPER, OMM_TEXT_TIARA_UPPER);
    omm_text_replace(str64, OMM_TEXT_CAPPY_LOWER, OMM_TEXT_TIARA_LOWER);
}

static void omm_text_replace_mario_by_luigi(u8 *str64) {
    omm_text_replace(str64, OMM_TEXT_MARIO, OMM_TEXT_LUIGI);
    omm_text_replace(str64, OMM_TEXT_MARIO_UPPER, OMM_TEXT_LUIGI_UPPER);
    omm_text_replace(str64, OMM_TEXT_MARIO_LOWER, OMM_TEXT_LUIGI_LOWER);
}

static void omm_text_replace_mario_by_wario(u8 *str64) {
    omm_text_replace(str64, OMM_TEXT_MARIO, OMM_TEXT_WARIO);
    omm_text_replace(str64, OMM_TEXT_MARIO_UPPER, OMM_TEXT_WARIO_UPPER);
    omm_text_replace(str64, OMM_TEXT_MARIO_LOWER, OMM_TEXT_WARIO_LOWER);
}

static void (*omm_text_replace_func[])(u8 *) = {
    omm_text_replace_mario_by_mario,
    omm_text_replace_mario_by_peach,
    omm_text_replace_mario_by_luigi,
    omm_text_replace_mario_by_wario,
};

u8 *omm_text_get_string_for_selected_player(u8 *str64) {
    static OmmArray sPlayersStrings = omm_array_zero;
    s32 lenWithFFterm = omm_text_length(str64) + 1;

    // Try to find the string in the list
    omm_array_for_each(sPlayersStrings, p) {
        const u8 **pstr = (const u8 **) p->as_ptr;
        for (s32 i = 0; i != OMM_NUM_PLAYABLE_CHARACTERS; ++i) {
            if (omm_text_compare(str64, pstr[i]) == 0) {
                omm_copy(str64, pstr[omm_player_get_selected_index()], lenWithFFterm);
                return str64;
            }
        }
    }

    // It doesn't exist yet, lets add an entry
    const u8 **pstr = omm_new(const u8 *, OMM_NUM_PLAYABLE_CHARACTERS);
    for (s32 i = 0; i != OMM_NUM_PLAYABLE_CHARACTERS; ++i) {
        pstr[i] = omm_dup(str64, lenWithFFterm);
        omm_text_replace_func[i](pstr[i]);
    }
    omm_array_add(sPlayersStrings, ptr, pstr);
    omm_copy(str64, pstr[omm_player_get_selected_index()], lenWithFFterm);
    return str64;
}

s32 omm_text_length(const u8 *str64) {
    s32 length = 0;
    for (; str64 && *str64 != 255; str64++, length++);
    return length;
}

s32 omm_text_compare(const u8 *str1, const u8 *str2) {
    s32 len1 = omm_text_length(str1);
    s32 len2 = omm_text_length(str2);
    s32 len = min_s(len1, len2);
    for (s32 i = 0; i != len; ++i) {
        if (str1[i] < str2[i]) return -1;
        if (str1[i] > str2[i]) return +1;
    }
    if (len1 < len2) return -1;
    if (len1 > len2) return +1;
    return 0;
}

//
// Dialogs
//

#include "omm_text_dialogs.inl"
_Static_assert(OMM_DIALOG_END_INDEX <= 256, "Last dialog ID is over 255");
_Static_assert(omm_static_array_length(sOmmDialogEntriesRaw) == OMM_DIALOG_COUNT, "Missing dialogs");

typedef struct {
    s32 id;
    bool isMulti;
    struct DialogEntry *dialog[OMM_SPARKLY_MODE_COUNT - 1];
} OmmDialogEntry;
static OmmArray *sOmmDialogEntries = NULL;

OMM_ROUTINE_UPDATE(omm_load_dialog_entries) {
#if OMM_GAME_IS_R96X
    static OmmMap sOmmDialogEntriesPerLanguage = omm_map_zero;

    // Update sOmmDialogEntries to the corresponding language
    // If the language exists, assign the corresponding dialog entries to the global var
    // Otherwise, create a new entry, and return to load the entries next frame
    struct LanguageEntry *language = get_language();
    s32 languageIndex = omm_map_find_key(sOmmDialogEntriesPerLanguage, ptr, language);
    if (languageIndex != -1) {
        sOmmDialogEntries = omm_map_get_val(sOmmDialogEntriesPerLanguage, ptr, languageIndex);
    } else {
        omm_map_add(sOmmDialogEntriesPerLanguage, ptr, language, ptr, NULL);
        return;
    }

    // Wait for the dialog pool to be fully loaded
    if (!dialogPool) {
        return;
    }
#endif
    if (!sOmmDialogEntries) {
        sOmmDialogEntries = omm_new(OmmArray, 1);
        for (s32 i = 0; i != OMM_DIALOG_COUNT; ++i) {
            OmmDialogEntry *entry = omm_new(OmmDialogEntry, 1);
#if OMM_GAME_IS_R96X
            entry->id = OMM_DIALOG_START_INDEX + i;
            entry->isMulti = (dialogPool[entry->id]->linesPerBox == 0);
            entry->dialog[0] = dialogPool[entry->id];
#else
            struct OmmDialogEntryRaw *raw = &sOmmDialogEntriesRaw[i];
            entry->id = raw->id;
            entry->isMulti = (raw->linesPerBox == 0);
            entry->dialog[0] = omm_new(struct DialogEntry, 1);
            entry->dialog[0]->unused = raw->soundBits;
            entry->dialog[0]->linesPerBox = raw->linesPerBox;
            entry->dialog[0]->leftOffset = raw->leftOffset;
            entry->dialog[0]->width = raw->downOffset;
            entry->dialog[0]->str = omm_text_convert(raw->str, true);
#endif

            // Multi dialog entries (Sparkly Stars)
            if (entry->isMulti) {

                // Dialog data
                s32 allLinesPerBox = entry->dialog[0]->width;
                s32 leftOffset = (entry->dialog[0]->unused ? 30 : 95);
                s32 downOffset = 200;
                u32 soundBits = entry->dialog[0]->unused;
                u8 *strPtr = (u8 *) entry->dialog[0]->str;

                // Create a dialog entry for each mode if not empty (linesPerBox > 0)
                for (s32 j = 0, k = 1; j != OMM_SPARKLY_MODE_COUNT - 1; ++j, k *= 10) {
                    s32 linesPerBox = ((allLinesPerBox / k) % 10);
                    if (linesPerBox > 0) {
                        entry->dialog[j] = omm_new(struct DialogEntry, 1);
                        entry->dialog[j]->linesPerBox = linesPerBox;
                        entry->dialog[j]->leftOffset = leftOffset;
                        entry->dialog[j]->width = downOffset;
                        entry->dialog[j]->unused = soundBits;
                        entry->dialog[j]->str = strPtr;
                    } else {
                        entry->dialog[j] = NULL;
                    }
                }
                
                // Then, edit the str pointers to point exactly when the dialog should start for each Sparkly Stars mode
                for (s32 j = 1; *strPtr != 0xFF; ++strPtr) {
                    if (*strPtr == 0xE4) { // '+' symbol
                        *strPtr = 0xFF;
                        if (j < OMM_SPARKLY_MODE_COUNT - 1 && entry->dialog[j]) {
                            entry->dialog[j++]->str = strPtr + 2; // +2, because the '+' is followed by a '\n'
                        }
                    }
                }
            }
            
            omm_array_add(*sOmmDialogEntries, ptr, entry);
        }
#if OMM_GAME_IS_R96X
        omm_map_set_val(sOmmDialogEntriesPerLanguage, ptr, sOmmDialogEntries, languageIndex);
#endif
    }
}

static void omm_play_dialog_sound(struct DialogEntry *dialog) {
    if (dialog && dialog->unused > 1 && gDialogBoxAngle == 90.f) {
        play_sound(dialog->unused, gGlobalSoundArgs);
    }
}

struct DialogEntry *omm_dialog_get_entry(void **dialogTable, s16 dialogId, s32 mode) {

    // OMM dialog entry
    if (OMM_LIKELY(sOmmDialogEntries)) {
        omm_array_for_each(*sOmmDialogEntries, p) {
            OmmDialogEntry *entry = (OmmDialogEntry *) p->as_ptr;
            if (entry->id == dialogId) {
                struct DialogEntry *dialog = entry->dialog[entry->isMulti ? (mode - 1) : 0];
                omm_play_dialog_sound(dialog);
                return dialog;
            }
        }
    }
    
    // Regular dialog entry
#if OMM_GAME_IS_R96X
    OMM_UNUSED(dialogTable);
    struct DialogEntry *dialog = dialogPool[dialogId];
#else
    struct DialogEntry *dialog = (struct DialogEntry *) (dialogTable ? dialogTable[dialogId] : NULL);
#endif
    omm_play_dialog_sound(dialog);
    return dialog;
}

static s16 omm_get_bowser_dialog(bool isIntro, s16 defaultDialog) {
    struct Object *o = obj_get_first_with_behavior(bhvOmmBowser);
    if (o && o->oBhvArgs) {
        switch (gCurrLevelNum) {
            case LEVEL_BOWSER_1: return (isIntro ? OMM_DIALOG_BOWSER_1_INTRO : defaultDialog);
            case LEVEL_BOWSER_2: return (isIntro ? OMM_DIALOG_BOWSER_2_INTRO : defaultDialog);
            case LEVEL_BOWSER_3: return (isIntro ? OMM_DIALOG_BOWSER_3_INTRO : OMM_DIALOG_BOWSER_3_DEFEAT);
            case LEVEL_GROUNDS:  return (isIntro ? OMM_DIALOG_SPARKLY_BOWSER_4_INTRO : OMM_DIALOG_SPARKLY_BOWSER_4_DEFEAT);
        }
    }
    return defaultDialog;
}

OMM_ROUTINE_PRE_RENDER(omm_update_dialogs) {
    static s16 sDialogID = -1;
    
    // Dialog entry
    // Retrieve it, replace names and set it to a valid slot
    if (sDialogID != gDialogID) {
        if (gDialogID != -1) {
            switch (gDialogID) {
                case DIALOG_067: gDialogID = omm_get_bowser_dialog(true, DIALOG_067); break;
                case DIALOG_092: gDialogID = omm_get_bowser_dialog(true, DIALOG_092); break;
                case DIALOG_093: gDialogID = omm_get_bowser_dialog(true, DIALOG_093); break;
                case DIALOG_121: gDialogID = omm_get_bowser_dialog(false, DIALOG_121); break;
                case DIALOG_163: gDialogID = omm_get_bowser_dialog(false, DIALOG_163); break;
            }
            struct DialogEntry *entry = omm_dialog_get_entry((void **) gDialogTable, gDialogID, gOmmSparklyMode);
            omm_text_get_string_for_selected_player((u8 *) entry->str);
            gDialogID = min_s(gDialogID, DIALOG_COUNT);
            gDialogTable[gDialogID] = entry;
        }
        sDialogID = gDialogID;
    }

    // Ending cutscene dialog
    // Replace 'Mario' and 'Cappy' by the selected character name and cap name
    if (gCutsceneMsgIndex != -1) {
        omm_text_get_string_for_selected_player(gEndCutsceneStringsEn[gCutsceneMsgIndex]);
    }
}

#if OMM_GAME_IS_R96X
#include <dirent.h>

//
// For Render96: Auto-generates *.omm.json at execution time
//

OMM_AT_STARTUP static void omm_r96x_generate_json() {
    omm_cat_paths(dirname, SYS_MAX_PATH, sys_exe_path(), "res/texts");
    DIR *dir = opendir(dirname);
    if (dir) {

        // Scan the texts directory
        struct dirent *ent = NULL;
        while ((ent = readdir(dir)) != NULL) {

            // Look for json files
            if (strstr(ent->d_name, ".json")) {

                // Retrieve the language code
                const char *dot = strchr(ent->d_name, '.');
                if (dot) {
                    omm_str_cpy(lang, 16, ent->d_name);
                    lang[dot - ent->d_name] = 0;

                    // Check if the OMM text file already exists
                    omm_sprintf(filename, SYS_MAX_PATH, "%s/res/texts/%s.omm.json", sys_exe_path(), lang);
                    if (!fs_sys_file_exists(filename)) {

                        // Retrieve the language name
                        omm_cat_paths(langfilename, SYS_MAX_PATH, dirname, ent->d_name);
                        FILE *langf = fopen(langfilename, "rb");
                        if (langf) {
                            char buffer[0x100] = { 0 };
                            fread(buffer, 1, 0xFF, langf);
                            fclose(langf);
                            const char *langnamep = strstr(buffer, "\"langName\":");
                            if (langnamep) {
                                const char *langnameq0 = strchr(langnamep + 11, '\"');
                                const char *langnameq1 = langnameq0 ? strchr(langnameq0 + 1, '\"') : NULL;
                                if (langnameq0 && langnameq1) {
                                    omm_str_cpy(langname, 64, langnameq0 + 1);
                                    langname[langnameq1 - langnameq0 - 1] = 0;

                                    // Write the json
                                    FILE *f = fopen(filename, "w");
                                    if (f) {

                                        // Header
                                        fprintf(f, "{\n");
                                        fprintf(f, "  \"manifest\": {\n");
                                        fprintf(f, "    \"langName\": \"%s\",\n", langname);
                                        fprintf(f, "    \"langLogo\": \"none\"\n");
                                        fprintf(f, "  },\n");

                                        // Dialogs
                                        fprintf(f, "  \"dialogs\": [\n");
                                        for (s32 i = 0; i != omm_static_array_length(sOmmDialogEntriesRaw); ++i) {
                                            fprintf(f, "    {\n");
                                            fprintf(f, "      \"ID\": %d,\n", sOmmDialogEntriesRaw[i].id);
                                            if (sOmmDialogEntriesRaw[i].soundBits != 0) {
                                                fprintf(f, "      \"sound\": \"%d, %d, 0x%02X, 0x%02X, %d\",\n",
                                                    (s32) ((sOmmDialogEntriesRaw[i].soundBits >> 28) & 0x0F),
                                                    (s32) ((sOmmDialogEntriesRaw[i].soundBits >> 24) & 0x0F),
                                                    (u8) ((sOmmDialogEntriesRaw[i].soundBits >> 16) & 0xFF),
                                                    (u8) ((sOmmDialogEntriesRaw[i].soundBits >>  8) & 0xFF),
                                                    (u8) ((sOmmDialogEntriesRaw[i].soundBits >>  4) & 0x0F)
                                                );
                                            }
                                            fprintf(f, "      \"linesPerBox\": %d,\n", sOmmDialogEntriesRaw[i].linesPerBox);
                                            fprintf(f, "      \"leftOffset\": %d,\n", sOmmDialogEntriesRaw[i].leftOffset);
                                            fprintf(f, "      \"width\": %d,\n", sOmmDialogEntriesRaw[i].downOffset);
                                            fprintf(f, "      \"lines\": [\n");
                                            fprintf(f, "        \"");
                                            for (const char *c = sOmmDialogEntriesRaw[i].str;; ++c) {
                                                if (*c == 0) {
                                                    fprintf(f, "\"\n");
                                                    fprintf(f, "      ]\n");
                                                    break;
                                                } else if (*c == '\n') {
                                                    fprintf(f, "\",\n");
                                                    fprintf(f, "        \"");
                                                } else {
                                                    fprintf(f, "%c", *c);
                                                }
                                            }
                                            if (i < (s32) omm_static_array_length(sOmmDialogEntriesRaw) - 1) {
                                                fprintf(f, "    },\n");
                                            } else {
                                                fprintf(f, "    }\n");
                                            }
                                        }
                                        fprintf(f, "  ],\n");
                                        
                                        // Strings
                                        fprintf(f, "  \"strings\": {\n");
                                        static const char *sOmmStrings[][2] = {
#define OMM_TEXT_(id, str) { "OMM_TEXT_" #id, str },
#include "data/omm/omm_defines_texts.inl"
#undef OMM_TEXT_
                                        };
                                        for (s32 i = 0; i != omm_static_array_length(sOmmStrings); ++i) {
                                            fprintf(f, "    \"%s\": \"%s\"", sOmmStrings[i][0], sOmmStrings[i][1]);
                                            if (i < (s32) omm_static_array_length(sOmmStrings) - 1) {
                                                fprintf(f, ",\n");
                                            } else {
                                                fprintf(f, "\n");
                                            }
                                        }
                                        fprintf(f, "  }\n");

                                        // Footer
                                        fprintf(f, "}\n");
                                        fclose(f);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        closedir(dir);
    }
}

#endif
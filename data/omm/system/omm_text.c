#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// String conversion (very basic)
//

#if !OMM_GAME_IS_R96A
static const struct { const char *str; u8 c64; } sSm64CharMap[] = {
    { "0",   0x00 }, { "1",  0x01 }, { "2",   0x02 }, { "3",   0x03 }, { "4",   0x04 }, { "5",   0x05 },
    { "6",   0x06 }, { "7",  0x07 }, { "8",   0x08 }, { "9",   0x09 }, { "A",   0x0A }, { "B",   0x0B },
    { "C",   0x0C }, { "D",  0x0D }, { "E",   0x0E }, { "F",   0x0F }, { "G",   0x10 }, { "H",   0x11 },
    { "I",   0x12 }, { "J",  0x13 }, { "K",   0x14 }, { "L",   0x15 }, { "M",   0x16 }, { "N",   0x17 },
    { "O",   0x18 }, { "P",  0x19 }, { "Q",   0x1A }, { "R",   0x1B }, { "S",   0x1C }, { "T",   0x1D },
    { "U",   0x1E }, { "V",  0x1F }, { "W",   0x20 }, { "X",   0x21 }, { "Y",   0x22 }, { "Z",   0x23 },
    { "a",   0x24 }, { "b",  0x25 }, { "c",   0x26 }, { "d",   0x27 }, { "e",   0x28 }, { "f",   0x29 },
    { "g",   0x2A }, { "h",  0x2B }, { "i",   0x2C }, { "j",   0x2D }, { "k",   0x2E }, { "l",   0x2F },
    { "m",   0x30 }, { "n",  0x31 }, { "o",   0x32 }, { "p",   0x33 }, { "q",   0x34 }, { "r",   0x35 },
    { "s",   0x36 }, { "t",  0x37 }, { "u",   0x38 }, { "v",   0x39 }, { "w",   0x3A }, { "x",   0x3B },
    { "y",   0x3C }, { "z",  0x3D }, { "\'",  0x3E }, { ".",   0x3F }, { "^",   0x50 }, { "|",   0x51 },
    { "<",   0x52 }, { ">",  0x53 }, { "[A]", 0x54 }, { "[B]", 0x55 }, { "[C]", 0x56 }, { "[Z]", 0x57 },
    { "[R]", 0x58 }, { ",",  0x6F }, { " ",   0x9E }, { "-",   0x9F }, { "/",   0xD0 }, { "[%]", 0xE0 },
    { "(",   0xE1 }, { ")(", 0xE2 }, { ")",   0xE3 }, { "+",   0xE4 }, { "&",   0xE5 }, { ":",   0xE6 },
    { "!",   0xF2 }, { "%",  0xF3 }, { "?",   0xF4 }, { "~",   0xF7 }, { "$",   0xF9 }, { "@",   0xFA },
    { "*",   0xFB }, { "=",  0xFD }, { "\n",  0xFE }, { NULL,  0xFF },
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
    return omm_memory_new(heapAlloc ? NULL : gOmmMemoryPoolStrings, length + 1, NULL);
}

u8 *omm_text_convert(const char *str, bool heapAlloc) {
    u8 *str64 = NULL;
#if OMM_GAME_IS_R96A
    u8 *translated = get_key_string(str);
    if (translated == get_language()->none) {
        translated = getTranslatedText((char *) str);
        str64 = OMM_MEMCPY(omm_text_alloc(omm_text_length(translated) + 1, heapAlloc), translated, omm_text_length(translated) + 1);
        free(translated);
    } else {
        str64 = OMM_MEMCPY(omm_text_alloc(omm_text_length(translated) + 1, heapAlloc), translated, omm_text_length(translated) + 1);
    }
#else
    str64 = omm_text_alloc(strlen(str) + 1, heapAlloc);
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
    u8 *strCopy = omm_memory_new(heapAlloc ? NULL : gOmmMemoryPoolStrings, 0x800, NULL);
    OMM_MEMCPY(strCopy, str64, omm_text_length(str64) + 1);
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
    for (s32 i = 0; i != (s32) OMM_ARRAY_SIZE(sTextToSkip); ++i) {
        if (!sTextToSkip[i].str64) {
            sTextToSkip[i].str64 = omm_text_convert(sTextToSkip[i].str, true);
            sTextToSkip[i].length = omm_text_length(sTextToSkip[i].str64);
        }
        if (OMM_MEMCMP(str64, sTextToSkip[i].str64, sTextToSkip[i].length)) {
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
        } else if (OMM_MEMCMP(str64, from64, lenFrom)) {
            OMM_MEMCPY(str64, to64, lenFrom);
            str64 += lenFrom;
            lenCurr -= lenFrom;
        } else {
            str64++;
            lenCurr--;
        }
    }
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

u8 *omm_text_replace_names(u8 *str64, bool inplace) {
    static OmmMap sMarioStrings = omm_map_zero;
    if (inplace) {
        if (omm_map_find_key(sMarioStrings, ptr, str64) == -1) {
            omm_map_add(sMarioStrings, ptr, str64, ptr, OMM_MEMDUP(str64, omm_text_length(str64) + 1));
        }
    } else {
        u8 *temp = (u8 *) omm_memory_new(gOmmMemoryPoolStrings, omm_text_length(str64) + 1, NULL);
        OMM_MEMCPY(temp, str64, omm_text_length(str64) + 1);
        str64 = temp;
    }
    switch (omm_player_get_selected_index()) {
        case OMM_PLAYER_PEACH: omm_text_replace_mario_by_peach(str64); break;
        case OMM_PLAYER_LUIGI: omm_text_replace_mario_by_luigi(str64); break;
        case OMM_PLAYER_WARIO: omm_text_replace_mario_by_wario(str64); break;
        case OMM_PLAYER_MARIO: if (inplace) {
            s32 i = omm_map_find_key(sMarioStrings, ptr, str64);
            if (i != -1) {
                u8 *strMario = omm_map_get_val(sMarioStrings, ptr, i);
                OMM_MEMCPY(str64, strMario, omm_text_length(strMario) + 1);
            }
        } break;
    }
    return str64;
}

s32 omm_text_length(const u8 *str64) {
    s32 length = 0;
    for (; str64 && *str64 != 255; str64++, length++);
    return length;
}

//
// Dialogs
//

#include "omm_text_dialogs.inl"
_Static_assert(OMM_DIALOG_END_INDEX <= 256, "Last dialog ID is over 255");
_Static_assert(OMM_ARRAY_SIZE(sOmmDialogEntriesRaw) == OMM_DIALOG_COUNT, "Missing dialogs");

typedef struct {
    s32 id;
    bool isMulti;
    struct DialogEntry *dialog[OMM_SSM_COUNT - 1];
} OmmDialogEntry;
static OmmArray *sOmmDialogEntries = NULL;

OMM_ROUTINE_UPDATE(omm_load_dialog_entries) {
#if OMM_GAME_IS_R96A
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
        sOmmDialogEntries = OMM_MEMNEW(OmmArray, 1);
        for (s32 i = 0; i != OMM_DIALOG_COUNT; ++i) {
            OmmDialogEntry *entry = OMM_MEMNEW(OmmDialogEntry, 1);
#if OMM_GAME_IS_R96A
            entry->id = OMM_DIALOG_START_INDEX + i;
            entry->isMulti = (dialogPool[entry->id]->linesPerBox == 0);
            entry->dialog[0] = dialogPool[entry->id];
#else
            struct OmmDialogEntryRaw *raw = &sOmmDialogEntriesRaw[i];
            entry->id = raw->id;
            entry->isMulti = (raw->linesPerBox == 0);
            entry->dialog[0] = OMM_MEMNEW(struct DialogEntry, 1);
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
                for (s32 j = 0, k = 1; j != OMM_SSM_COUNT - 1; ++j, k *= 10) {
                    s32 linesPerBox = ((allLinesPerBox / k) % 10);
                    if (linesPerBox > 0) {
                        entry->dialog[j] = OMM_MEMNEW(struct DialogEntry, 1);
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
                        if (j < OMM_SSM_COUNT - 1 && entry->dialog[j]) {
                            entry->dialog[j++]->str = strPtr + 2; // +2, because the '+' is followed by a '\n'
                        }
                    }
                }
            }
            
            omm_array_add(*sOmmDialogEntries, ptr, entry);
        }
#if OMM_GAME_IS_R96A
        omm_map_set_val(sOmmDialogEntriesPerLanguage, ptr, sOmmDialogEntries, languageIndex);
#endif
    }
}

static void omm_play_dialog_sound(struct DialogEntry *dialog) {
    if (dialog && dialog->unused > 1 && gDialogBoxAngle == 90.f) {
        play_sound(dialog->unused, gGlobalSoundArgs);
    }
}

struct DialogEntry **omm_get_dialog_table() {
#if OMM_GAME_IS_SM74
    return (struct DialogEntry **) (sm74_mode__omm_get_dialog_table == 2 ? seg2_dialog_table_EE : seg2_dialog_table);
#elif OMM_GAME_IS_R96A
    return (struct DialogEntry **) dialogPool;
#else
    return (struct DialogEntry **) seg2_dialog_table;
#endif
}

struct DialogEntry *omm_get_dialog_entry(void **dialogTable, s16 dialogId) {

    // OMM dialog entry
    if (OMM_LIKELY(sOmmDialogEntries)) {
        omm_array_for_each(*sOmmDialogEntries, p) {
            OmmDialogEntry *entry = (OmmDialogEntry *) p->as_ptr;
            if (entry->id == dialogId) {
                struct DialogEntry *dialog = entry->dialog[entry->isMulti ? (gOmmSSM - 1) : 0];
                omm_play_dialog_sound(dialog);
                return dialog;
            }
        }
    }
    
    // Regular dialog entry
#if OMM_GAME_IS_R96A
    OMM_UNUSED(dialogTable);
    struct DialogEntry *dialog = dialogPool[dialogId];
#else
    struct DialogEntry *dialog = (struct DialogEntry *) (dialogTable ? dialogTable[dialogId] : NULL);
#endif
    omm_play_dialog_sound(dialog);
    return dialog;
}

static s16 omm_get_bowser_dialog(bool isIntro, s16 defaultDialog) {
    struct Object *o = obj_get_first_with_behavior(omm_bhv_bowser);
    if (o && o->oBehParams) {
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
            struct DialogEntry *entry = omm_get_dialog_entry((void **) omm_get_dialog_table(), gDialogID);
            omm_text_replace_names((u8 *) entry->str, true);
            gDialogID = min_s(gDialogID, DIALOG_COUNT);
            omm_get_dialog_table()[gDialogID] = entry;
        }
        sDialogID = gDialogID;
    }

    // Ending cutscene dialog
    // Replace 'Mario' and 'Cappy' by the selected character name and cap name
    if (gCutsceneMsgIndex != -1) {
        omm_text_replace_names(gEndCutsceneStringsEn[gCutsceneMsgIndex], true);
    }
}

#if OMM_GAME_IS_R96A

//
// For Render96: Auto-generates AM_us.omm.json at execution time
//

OMM_AT_STARTUP static void omm_r96a_generate_json() {
    OMM_STRING(filename, 256, "%s/res/texts/AM_us.omm.json", OMM_EXE_FOLDER);
    FILE *f = fopen(filename, "w");
    if (f) {

        // Header
        fprintf(f, "{\n");
        fprintf(f, "  \"manifest\": {\n");
        fprintf(f, "    \"langName\": \"English\",\n");
        fprintf(f, "    \"langLogo\": \"none\"\n");
        fprintf(f, "  },\n");

        // Dialogs
        fprintf(f, "  \"dialogs\": [\n");
        for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmDialogEntriesRaw); ++i) {
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
            if (i < (s32) OMM_ARRAY_SIZE(sOmmDialogEntriesRaw) - 1) {
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
        for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmStrings); ++i) {
            fprintf(f, "    \"%s\": \"%s\"", sOmmStrings[i][0], sOmmStrings[i][1]);
            if (i < (s32) OMM_ARRAY_SIZE(sOmmStrings) - 1) {
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

#endif
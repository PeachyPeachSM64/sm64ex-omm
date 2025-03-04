#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static struct OmmDialogEntry *sOmmDialogs[] = {
    gOmmDialogSigns,
    gOmmDialogNPCs,
    gOmmDialogBowser,
    gOmmDialogCheats,
    gOmmDialogSparklyStars1,
    gOmmDialogSparklyStars2,
    // gOmmDialogSparklyStars3, // TODO, nebulas
};

#if OMM_GAME_IS_R96X

static OmmMap_(struct LanguageEntry *, OmmHMap_(struct DialogEntry *) *) sOmmDialogEntriesPerLanguage = omm_map_zero;

static const char *omm_dialog_get_omm_entry_name(str_t dst, s16 id) {
    str_fmt(dst, sizeof(str_t), "OMM_DIALOG_ENTRY_%04X", (u16) id);
    return dst;
}

static OmmHMap_(struct DialogEntry *) *omm_dialog_load_omm_entries() {
    struct LanguageEntry *language = get_language();
    if (language) {
        s32 languageIndex = omm_map_find_key(sOmmDialogEntriesPerLanguage, ptr, language);
        if (languageIndex == -1) {
            OmmHMap_(struct DialogEntry *) *dialogEntries = mem_new(OmmHMap, 1);
            omm_map_add(sOmmDialogEntriesPerLanguage, ptr, language, ptr, dialogEntries);
            array_for_each_(struct OmmDialogEntry *, entries, sOmmDialogs) {
                for (struct OmmDialogEntry *entry = *entries; entry->id; entry++) {
                    str_t dialogEntryName;
                    struct DialogEntry *dialog = mem_new(struct DialogEntry, 1);
                    dialog->unused = (u32) entry->soundBits;
                    dialog->linesPerBox = (s8) entry->linesPerBox;
                    dialog->leftOffset = (s16) entry->leftOffset;
                    dialog->width = (s16) entry->downOffset;
                    dialog->str = get_key_string(omm_dialog_get_omm_entry_name(dialogEntryName, entry->id));
                    omm_hmap_insert(*dialogEntries, entry->id + 1, dialog);
                }
            }
            return dialogEntries;
        }
        return omm_map_get_val(sOmmDialogEntriesPerLanguage, ptr, languageIndex);
    }
    return NULL;
}

static struct DialogEntry *omm_dialog_get_omm_entry(s16 dialogId) {
    OmmHMap_(struct DialogEntry *) *dialogEntries = omm_dialog_load_omm_entries();
    if (dialogEntries) {
        s32 dialogIndex = omm_hmap_find(*dialogEntries, dialogId + 1);
        if (dialogIndex != -1) {
            return omm_hmap_get(*dialogEntries, struct DialogEntry *, dialogIndex);
        }
    }
    return NULL;
}

#else

static OmmHMap_(struct DialogEntry *) sOmmDialogEntries = omm_hmap_zero;

static void omm_dialog_load_omm_entries() {
    OMM_DO_ONCE {
        array_for_each_(struct OmmDialogEntry *, entries, sOmmDialogs) {
            for (struct OmmDialogEntry *entry = *entries; entry->id; entry++) {
                struct DialogEntry *dialog = mem_new(struct DialogEntry, 1);
                dialog->unused = (u32) entry->soundBits;
                dialog->linesPerBox = (s8) entry->linesPerBox;
                dialog->leftOffset = (s16) entry->leftOffset;
                dialog->width = (s16) entry->downOffset;
                dialog->str = omm_text_convert(entry->str, true);
                omm_hmap_insert(sOmmDialogEntries, entry->id + 1, dialog);
            }
        }
    }
}

static struct DialogEntry *omm_dialog_get_omm_entry(s16 dialogId) {
    omm_dialog_load_omm_entries();
    s32 i = omm_hmap_find(sOmmDialogEntries, dialogId + 1);
    if (i != -1) {
        return omm_hmap_get(sOmmDialogEntries, struct DialogEntry *, i);
    }
    return NULL;
}

#endif

struct DialogEntry *omm_dialog_get_entry(void **dialogTable, s16 dialogId) {
    return (
        dialogId >= OMM_DIALOG_START_INDEX ?
        omm_dialog_get_omm_entry(dialogId) :
        (dialogTable ? dialogTable[dialogId] : NULL)
    );
}

static s16 omm_dialog_get_bowser_dialog_id(bool isIntro, s16 defaultDialog) {
    if (omm_bowser_is_active()) {
        switch (gCurrLevelNum) {
            case LEVEL_BOWSER_1: return (isIntro ? OMM_DIALOG_BOWSER_1_INTRO : defaultDialog);
            case LEVEL_BOWSER_2: return (isIntro ? OMM_DIALOG_BOWSER_2_INTRO : defaultDialog);
            case LEVEL_BOWSER_3: return (isIntro ? OMM_DIALOG_BOWSER_3_INTRO : OMM_DIALOG_BOWSER_3_DEFEAT);
            case OMM_SPARKLY_BOWSER_4_LEVEL: return (isIntro ? OMM_DIALOG_SPARKLY_BOWSER_4_INTRO(gOmmSparklyMode) : OMM_DIALOG_SPARKLY_BOWSER_4_DEFEAT(gOmmSparklyMode));
        }
    }
    return defaultDialog;
}

OMM_ROUTINE_PRE_RENDER(omm_dialog_update) {
    static s16 sDialogID = -1;

    // Dialog entry
    // Retrieve it, replace names and set it to a valid slot
    if (sDialogID != gDialogID) {
        if (gDialogID != -1) {
            switch (gDialogID) {
                case DIALOG_067: gDialogID = omm_dialog_get_bowser_dialog_id(true, DIALOG_067); break;
                case DIALOG_092: gDialogID = omm_dialog_get_bowser_dialog_id(true, DIALOG_092); break;
                case DIALOG_093: gDialogID = omm_dialog_get_bowser_dialog_id(true, DIALOG_093); break;
                case DIALOG_121: gDialogID = omm_dialog_get_bowser_dialog_id(false, DIALOG_121); break;
                case DIALOG_163: gDialogID = omm_dialog_get_bowser_dialog_id(false, DIALOG_163); break;
            }
            struct DialogEntry *dialog = omm_dialog_get_entry((void **) gDialogTable(OMM_GAME_MODE), gDialogID);
            if (dialog) {
                omm_text_get_string_for_selected_player((u8 *) dialog->str);
                gDialogID = min_s(gDialogID, DIALOG_COUNT);
                gDialogTable(OMM_GAME_MODE)[gDialogID] = dialog;
                if (dialog->unused > 1) {
                    play_sound(dialog->unused | 0xFF00, gGlobalSoundArgs);
                }
            } else {
                gDialogID = -1;
            }
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

static void omm_r96x_json_write_string(FILE *f, const char *str) {
    for (; *str; str++) {
        switch (*str) {
            case '"': fputc('\\', f); fputc('"', f); break;
            case '\n': fputc('\\', f); fputc('n', f); break;
            default: fputc(*str, f); break;
        }
    }
}

void omm_r96x_generate_json() {
    sys_path_t dirpath;
    DIR *dir = opendir(fs_get_game_path(dirpath, "texts"));
    if (dir) {

        // Scan the texts directory
        struct dirent *ent = NULL;
        while ((ent = readdir(dir)) != NULL) {

            // Look for json files
            if (strstr(ent->d_name, ".json")) {

                // Retrieve the language code
                const char *dot = strchr(ent->d_name, '.');
                if (dot) {
                    str_t lang;
                    str_cpy(lang, sizeof(lang), ent->d_name);
                    lang[dot - ent->d_name] = 0;

                    // Check if the OMM text file already exists
                    sys_path_t filename;
                    str_fmt(filename, sizeof(filename), "%s/%s.omm.json", dirpath, lang);
                    if (!fs_sys_file_exists(filename)) {

                        // Retrieve the language name
                        sys_path_t langfilename;
                        fs_cat_paths(langfilename, dirpath, ent->d_name);
                        FILE *langf = fopen(langfilename, "rb");
                        if (langf) {
                            char buffer[0x100] = {0};
                            fread(buffer, 1, 0xFF, langf);
                            fclose(langf);
                            const char *langnamep = strstr(buffer, "\"langName\":");
                            if (langnamep) {
                                const char *langnameq0 = strchr(langnamep + 11, '\"');
                                const char *langnameq1 = langnameq0 ? strchr(langnameq0 + 1, '\"') : NULL;
                                if (langnameq0 && langnameq1) {
                                    str_t langname;
                                    str_cpy(langname, sizeof(langname), langnameq0 + 1);
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

                                        // Strings
                                        fprintf(f, "  \"strings\": {\n");
                                        static const char *sOmmStrings[][2] = {
#define OMM_TEXT_(id, str) { "OMM_TEXT_" #id, str },
#include "data/omm/omm_defines_texts.inl"
#undef OMM_TEXT_
                                        };
                                        for (s32 i = 0; i != array_length(sOmmStrings); ++i) {
                                            fprintf(f, "    \"%s\": \"", sOmmStrings[i][0]);
                                            omm_r96x_json_write_string(f, sOmmStrings[i][1]);
                                            fprintf(f, "\",\n");
                                        }

                                        // Dialogs
                                        // Render96 cannot load more than 256 dialog entries,
                                        // so OMM dialog entries have to be stored as text strings
                                        array_for_each_(struct OmmDialogEntry *, entries, sOmmDialogs) {
                                            for (struct OmmDialogEntry *entry = *entries; entry->id; entry++) {
                                                str_t dialogEntryName;
                                                fprintf(f, "    \"%s\": \"", omm_dialog_get_omm_entry_name(dialogEntryName, entry->id), buffer);
                                                omm_r96x_json_write_string(f, entry->str);
                                                fprintf(f, "\",\n");
                                            }
                                        }
                                        fprintf(f, "    \"END_OF_STRINGS\": \"\"\n");
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
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// LiveSplit auto-splitter support
// The file 'omm.asl' is needed to make it work
//
// Splits formatting:
// - For star splits, you must indicate the amount of stars needed in brackets:
//   * Use square brackets to split upon star collection: "[20]".
//   * Use parentheses to delay the split until the level exit: "(20)".
// - If a split has "Bowser" in its name but no star amount, it is interpreted as a Bowser
//   key split or a Grand Star split and the split is triggered on key/star collection.
// - If the final split has no star amount, it is interpreted as a Grand Star split
//   (i.e. after defeating the last Bowser) and immediately stops the timer.
//

#pragma GCC push_options
#pragma GCC optimize ("O0")
static volatile u8 sOmmSplitFlags[16] = { 'O', 'M', 'M', 'A', 'U', 'T', 'O', 'S', 'P', 'L', 'I', 'T', 0, 0, 0, 0 };
static volatile s32 *sOmmSplitIndex = (s32 *) (&sOmmSplitFlags[12]);
static volatile u32 sOmmSplitTimestamp = 0;
static OmmArray sOmmSplits = omm_array_zero;
#pragma GCC pop_options

typedef struct {
    s32 type;
    s32 value;
} OmmSplit;

//
// Init
//

static const char *omm_speedrun_lss_get_data(str_t dst, const char *buffer, const char *beginsWith, const char *endsWith) {
    const char *a = strstr(buffer, beginsWith);
    const char *b = strstr(buffer, endsWith);
    if (a && b) {
        a += strlen(beginsWith);
        s32 length = (s32) (b - a);
        mem_zero(dst, sizeof(str_t));
        mem_cpy(dst, a, clamp_s(length, 0, sizeof(str_t) - 1));
        return dst;
    }
    return NULL;
}

static OmmSplit *omm_speedrun_split_data_create_split(const char *s, bool lastSplit) {

    // Star or level exit split
    for_each_in_(const char *, brackets, { "[]", "()" }) {
        const char *a = strrchr(s, (*brackets)[0]);
        const char *b = strrchr(s, (*brackets)[1]);
        if (a && b && a < b) {
            s32 stars;
            if (sscanf(a + 1, "%d", &stars) == 1) {
                OmmSplit *split = mem_new(OmmSplit, 1);
                split->type = (*a == '(' ? OMM_SPLIT_EXIT : OMM_SPLIT_STAR);
                split->value = stars;
                return split;
            }
        }
    }

    // Bowser split
    str_t lower;
    str_lwr(lower, sizeof(lower), s);
    if (strstr(lower, "bowser") || lastSplit) {
        OmmSplit *split = mem_new(OmmSplit, 1);
        split->type = OMM_SPLIT_BOWSER;
        return split;
    }

    // Invalid split
    return NULL;
}

OMM_AT_STARTUP static void omm_speedrun_init() {
    sys_path_t filename;
    fs_cat_paths(filename, sys_exe_path(), "splits.lss");
    FILE *f = fopen(filename, "r");
    if (f) {
        omm_log("Extracting split data from file: \"%s\"\n",, filename);

        // Looking for game info and splits
        OmmArray splits = omm_array_zero;
        bool isSegment = false;
        str_t buffer;
        while (fgets(buffer, sizeof(buffer), f)) {

            // Game name
            str_t gameName;
            if (omm_speedrun_lss_get_data(gameName, buffer, "<GameName>", "</GameName>")) {
                omm_printf("Game: %s\n",, gameName);
                continue;
            }

            // Category name
            str_t categoryName;
            if (omm_speedrun_lss_get_data(categoryName, buffer, "<CategoryName>", "</CategoryName>")) {
                omm_printf("Category: %s\n",, categoryName);
                continue;
            }

            // Segment start
            str_t segmentStart;
            if (omm_speedrun_lss_get_data(segmentStart, buffer, "<Segment>", "")) {
                isSegment = true;
                continue;
            }

            // Segment end
            str_t segmentEnd;
            if (omm_speedrun_lss_get_data(segmentEnd, buffer, "</Segment>", "")) {
                isSegment = false;
                continue;
            }

            // Splits
            if (isSegment) {
                str_t splitName;
                if (omm_speedrun_lss_get_data(splitName, buffer, "<Name>", "</Name>")) {
                    omm_array_add(splits, ptr, mem_dup(splitName, strlen(splitName) + 1));
                }
            }
        }
        fflush(stdout);
        fclose(f);

        // Generating splits
        omm_printf("Splits:\n");
        omm_array_for_each(splits, p) {
            bool lastSplit = (i_p == omm_array_count(splits) - 1);
            const char *s = (const char *) p->as_ptr;
            OmmSplit *split = omm_speedrun_split_data_create_split(s, lastSplit);
            if (split) {
                omm_array_add(sOmmSplits, ptr, split);
                switch (split->type) {
                    case OMM_SPLIT_STAR: {
                        omm_printf("> %d Stars [Collect]: \"%s\"\n",, split->value, s);
                    } break;

                    case OMM_SPLIT_EXIT: {
                        omm_printf("> %d Stars (Level Exit): \"%s\"\n",, split->value, s);
                    } break;

                    case OMM_SPLIT_BOWSER: {
                        if (lastSplit) {
                            omm_printf("> Grand Star: \"%s\"\n",, s);
                        } else {
                            omm_printf("> Bowser Key: \"%s\"\n",, s);
                        }
                    } break;
                }
            } else {
                omm_printf_warning("Invalid split format: %s\n",, s);
            }
            mem_del(p->as_ptr);
        }
        omm_array_delete(splits);
    }
}

//
// Update
//

static bool omm_speedrun_should_split(const OmmSplit *split) {
    switch (split->type) {
        case OMM_SPLIT_STAR: return omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE) >= split->value;
        case OMM_SPLIT_EXIT: return omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE) >= split->value;
        case OMM_SPLIT_BOWSER: return true;
    }
    return false;
}

void omm_speedrun_split(s32 type) {
    if (sOmmSplitTimestamp != gGlobalTimer && *sOmmSplitIndex >= 0 && *sOmmSplitIndex < omm_array_count(sOmmSplits)) {
        const OmmSplit *split = (const OmmSplit *) omm_array_get(sOmmSplits, ptr, *sOmmSplitIndex);
        if (split->type == type && omm_speedrun_should_split(split)) {
            (*sOmmSplitIndex)++;
            sOmmSplitTimestamp = gGlobalTimer;
        }
    }
}

OMM_ROUTINE_UPDATE(omm_speedrun_update) {
    static s32 sPrevLevelNum = LEVEL_NONE;

    // Main menu: 'reset' (-1)
    if (omm_is_main_menu()) {
        *sOmmSplitIndex = -1;
        sPrevLevelNum = LEVEL_NONE;
        return;
    }

    // New file: 'start' (-2)
    // As soon as Mario loads in, set split index to 0
    if (*sOmmSplitIndex < 0) {
        if (!gMarioObject && !omm_save_file_exists(gCurrSaveFileNum - 1, OMM_GAME_MODE)) {
            *sOmmSplitIndex = -2;
        } else {
            *sOmmSplitIndex = 0;
        }
        sPrevLevelNum = gCurrLevelNum;
        return;
    }

    // Level exit: 'split'
    if (sPrevLevelNum != gCurrLevelNum) {
        omm_speedrun_split(OMM_SPLIT_EXIT);
        sPrevLevelNum = gCurrLevelNum;
    }
}

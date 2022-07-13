#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#pragma GCC push_options
#pragma GCC optimize ("O0")

//
// LiveSplit auto-splitter support
// The file 'omm.asl' is needed to make it work
//

static volatile u8 sOmmSplitFlags[16] = { 'O', 'M', 'M', 'A', 'U', 'T', 'O', 'S', 'P', 'L', 'I', 'T', 0, 0, 0, 0 };
static volatile s32 *sOmmSplitIndex = (s32 *) (&sOmmSplitFlags[12]);
static OmmArray sOmmSplits = omm_array_zero;

void omm_speedrun_split(s32 numStars) {
    if (0 <= *sOmmSplitIndex && *sOmmSplitIndex < omm_array_count(sOmmSplits) && omm_array_get(sOmmSplits, s32, *sOmmSplitIndex) == numStars) {
        (*sOmmSplitIndex)++;
    }
}

// Main menu/File select screen -> 'reset' (-1)
// 'start' (-2) or 'reset' (-1) but empty save and Mario not loaded -> 'start'
// As soon as Mario loads or if an existing save file is selected, set index to 0
OMM_ROUTINE_UPDATE(omm_speedrun_update) {
    if (omm_is_main_menu()) {
        *sOmmSplitIndex = -1;
    } else if (*sOmmSplitIndex < 0) {
        if (!gMarioObject && !save_file_exists(gCurrSaveFileNum - 1)) {
            *sOmmSplitIndex = -2;
        } else {
            *sOmmSplitIndex = 0;
        }
    }
}

//
// Init
//

static const char *omm_speedrun_lss_get_data(const char *line, const char *beginsWith, const char *endsWith) {
    static char data[1024];
    const char *begin = strstr(line, beginsWith);
    const char *end = strstr(line, endsWith);
    if (begin && end) {
        begin += strlen(beginsWith);
        s32 length = (s32) (end - begin);
        OMM_MEMSET(data, 0, sizeof(data));
        OMM_MEMCPY(data, begin, max_s(0, length));
        return data;
    }
    return NULL;
}

static s32 omm_speedrun_split_data_get_stars(const char *split) {
    static const char *sOpenBrackets[] = { "[", "(", NULL };
    static const char *sCloseBrackets[] = { "]", ")", NULL };
    for_each_until_null(const char *, openBracket, sOpenBrackets) {
        const char *closeBracket = sCloseBrackets[(s32) (openBracket - sOpenBrackets)];
        const char *p = split;
        while ((p = strstr(p, *openBracket)) != NULL) {
            if (strstr(++p, closeBracket)) {
                s32 numStars;
                if (sscanf(p, "%d", &numStars)) {
                    return numStars;
                }
            } else break;
        }
    }
    return -1;
}

static bool omm_speedrun_split_data_is_bowser(const char *split) {
    char lower[1024] = { 0 };
    for (const char *c = split; *c; ++c) {
        lower[(s32) (c - split)] = tolower(*c);
    }
    return strstr(lower, "bowser") != NULL;
}

OMM_AT_STARTUP static void omm_speedrun_init() {
    OMM_STRING(filename, 256, "%s/%s", OMM_EXE_FOLDER, "splits.lss");
    FILE *f = fopen(filename, "r");
    if (f) {
        printf("\nExtracting split data from file: %s\n", filename);

        // Looking for game info and splits
        OmmArray splits = omm_array_zero;
        bool isSegment = false;
        char buffer[1024];
        while (fgets(buffer, 1024, f)) {

            // Game name
            const char *gameName = omm_speedrun_lss_get_data(buffer, "<GameName>", "</GameName>");
            if (gameName) {
                printf("Game: %s\n", gameName);
                continue;
            }
            
            // Category name
            const char *categoryName = omm_speedrun_lss_get_data(buffer, "<CategoryName>", "</CategoryName>");
            if (categoryName) {
                printf("Category: %s\n", categoryName);
                continue;
            }

            // Segment start
            if (omm_speedrun_lss_get_data(buffer, "<Segment>", "")) {
                isSegment = true;
                continue;
            }
            
            // Segment end
            if (omm_speedrun_lss_get_data(buffer, "</Segment>", "")) {
                isSegment = false;
                continue;
            }
            
            // Splits
            if (isSegment) {
                const char *splitName = omm_speedrun_lss_get_data(buffer, "<Name>", "</Name>");
                if (splitName) {
                    omm_array_add(splits, ptr, OMM_MEMDUP(splitName, strlen(splitName) + 1));
                }
            }
        }

        // Generating splits
        printf("Splits:\n");
        omm_array_for_each(splits, p) {
            const char *split = (const char *) p->as_ptr;
            s32 numStars = omm_speedrun_split_data_get_stars(split);
            if (numStars != -1) {
                omm_array_add(sOmmSplits, s32, numStars);
                printf("- %d Star split: %s\n", numStars, split);
            } else if (i_p == omm_array_count(splits) - 1) {
                omm_array_add(sOmmSplits, s32, -1);
                printf("- Grand Star split: %s\n", split);
            } else if (omm_speedrun_split_data_is_bowser(split)) {
                omm_array_add(sOmmSplits, s32, -1);
                printf("- Bowser key split: %s\n", split);
            } else {
                printf("[!] Invalid split format: %s\n", split);
            }
        }

        // Done
        printf("Data successfully extracted. Closing file.\n\n");
        fflush(stdout);
        fclose(f);
    }
}

#pragma GCC pop_options

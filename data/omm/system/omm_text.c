#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

const char SM64_TO_ASCII_CHAR[] = {
/*        x0    x1    x2    x3    x4    x5    x6    x7    x8    x9    xA    xB    xC    xD    xE    xF  */
/* 0x */  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  'A',  'B',  'C',  'D',  'E',  'F',
/* 1x */  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',  'U',  'V',
/* 2x */  'W',  'X',  'Y',  'Z',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',
/* 3x */  'm',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\'',  '.',
/* 4x */    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
/* 5x */  '|',  '~',  '{',  '}',  'A',  'B',  'C',  'Z',  'R',    0,    0,    0,    0,    0,    0,    0,
/* 6x */    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  ',',
/* 7x */    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
/* 8x */    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
/* 9x */    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  ' ',  '-',
/* Ax */    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
/* Bx */    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
/* Cx */    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
/* Dx */  '/',    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
/* Ex */    0,  '(',    0,  ')',  '+',  '&',  ':',    0,    0,    0,    0,    0,    0,    0,    0,    0,
/* Fx */    0,    0,  '!',  '%',  '?',    0,  '"',  '-',    0,  '@',  '*',  'x',    0,    0, '\n',    0,
};

//
// String conversion (very basic)
//

#if !OMM_GAME_IS_R96X
static const struct { const char *str; u8 c64[2]; } ASCII_TO_SM64_CHAR[] = {
#include "data/omm/system/omm_text_charmap.inl"
{NULL, {0xFF, 0}}, // terminator
};

static const char *omm_text_add_char(u8 *str64, const char *str, s32 *i) {
    for (s32 j = 0; ASCII_TO_SM64_CHAR[j].str; ++j) {
        if (strstr(str, ASCII_TO_SM64_CHAR[j].str) == str) {
            str64[(*i)++] = *(ASCII_TO_SM64_CHAR[j].c64);
            return str + strlen(ASCII_TO_SM64_CHAR[j].str);
        }
    }
    str64[(*i)++] = 0x9E;
    return str + 1;
}
#endif

static u8 *omm_text_alloc(s32 length, bool heapAlloc) {
    u8 *allocated = omm_memory_new(heapAlloc ? NULL : gOmmMemoryPoolStrings, length + 1, NULL);
    mem_set(allocated, 0xFF, length + 1);
    return allocated;
}

u8 *omm_text_convert(const char *str, bool heapAlloc) {
    u8 *str64 = NULL;
#if OMM_GAME_IS_R96X
    u8 *translated = get_key_string(str);
    if (translated == get_language()->none) {
        translated = getTranslatedText((char *) str);
        s32 length = omm_text_length(translated);
        str64 = mem_cpy(omm_text_alloc(length, heapAlloc), translated, length);
        free(translated);
    } else {
        s32 length = omm_text_length(translated);
        str64 = mem_cpy(omm_text_alloc(length, heapAlloc), translated, length);
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

u8 *omm_text_duplicate(const u8 *str64, bool heapAlloc) {
    s32 length = omm_text_length(str64);
    u8 *strCopy = omm_text_alloc(length, heapAlloc);
    mem_cpy(strCopy, str64, length);
    return strCopy;
}

u8 *omm_text_copy(u8 *dest, s32 size, const u8 *str64) {
    mem_set(dest, 0xFF, size);
    for (s32 i = 0; i < (size - 1) && *str64 != 0xFF; ++i, ++str64) {
        dest[i] = *str64;
    }
    return dest;
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

s32 omm_text_length(const u8 *str64) {
    s32 length = 0;
    for (; str64 && *str64 != 0xFF; str64++, length++);
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
// Replace Mario/Cappy in text by the character's name/their hat name
//

typedef struct { const char *str; s32 offset; const u8 *str64; s32 length; } TextToSkip;
static TextToSkip sTextToSkip[] = {
    { "Mario 64",      0, NULL, 0 },
    { "Mario Bros.",   0, NULL, 0 },
    { "Super Mario",   6, NULL, 0 },
    { "Mario is red",  0, NULL, 0 },
    { "Odyssey Mario", 8, NULL, 0 },
    { "FastMario",     4, NULL, 0 },
};

static bool omm_text_starts_with(const u8 *str64, const u8 *subStr64, s32 lenSubStr) {
    for (s32 i = 0; i != lenSubStr; ++i) {
        if (str64[i] == 0xFF || str64[i] != subStr64[i]) {
            return false;
        }
    }
    return true;
}

// Assumes `from` and `to` have the same length
static void omm_text_replace(u8 *str64, const char *from, const char *to) {
    const u8 *from64 = omm_text_convert(from, false);
    const u8 *to64 = omm_text_convert(to, false);
    s32 lenFrom = omm_text_length(from64);
    s32 lenStr = omm_text_length(str64);
    for (s32 i = 0; i != lenStr; ++i) {
        if (omm_text_starts_with(str64 + i, from64, lenFrom)) {
            u32 numTextsToSkip = array_length(sTextToSkip);
            for_each_(TextToSkip, textToSkip, sTextToSkip, numTextsToSkip) {
                if (!textToSkip->str64) {
                    textToSkip->str64 = omm_text_convert(textToSkip->str, true);
                    textToSkip->length = omm_text_length(textToSkip->str64);
                }
                if (i >= textToSkip->offset && omm_text_starts_with(str64 + i - textToSkip->offset, textToSkip->str64, textToSkip->length)) {
                    i += textToSkip->length - textToSkip->offset - 1;
                    break;
                }
            }
            if (i_textToSkip == numTextsToSkip) {
                mem_cpy(str64 + i, to64, lenFrom);
                i += lenFrom - 1;
            }
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

static void omm_text_replace_mario_by_yoshi(u8 *str64) {
    omm_text_replace(str64, OMM_TEXT_MARIO, OMM_TEXT_YOSHI);
    omm_text_replace(str64, OMM_TEXT_MARIO_UPPER, OMM_TEXT_YOSHI_UPPER);
    omm_text_replace(str64, OMM_TEXT_MARIO_LOWER, OMM_TEXT_YOSHI_LOWER);
}

static void (*omm_text_replace_func[])(u8 *) = {
    omm_text_replace_mario_by_mario,
    omm_text_replace_mario_by_peach,
    omm_text_replace_mario_by_luigi,
    omm_text_replace_mario_by_wario,
    omm_text_replace_mario_by_yoshi,
};

u8 *omm_text_get_string_for_selected_player(u8 *str64) {
    static OmmArray sPlayersStrings = omm_array_zero;
    s32 lenWithFFterm = omm_text_length(str64) + 1;
    s32 playerIndex = gOmmGlobals->yoshiMode ? OMM_PLAYER_YOSHI : omm_player_get_selected_index_model_and_sounds();

    // Try to find the string in the list
    omm_array_for_each(sPlayersStrings, p) {
        const u8 **pstr = (const u8 **) p->as_ptr;
        for (s32 i = 0; i != OMM_NUM_PLAYABLE_CHARACTERS; ++i) {
            if (omm_text_compare(str64, pstr[i]) == 0) {
                mem_cpy(str64, pstr[playerIndex], lenWithFFterm);
                return str64;
            }
        }
    }

    // It doesn't exist yet, lets add an entry
    u8 **pstr = mem_new(u8 *, array_length(omm_text_replace_func));
    for (s32 i = 0; i != array_length(omm_text_replace_func); ++i) {
        pstr[i] = mem_dup(str64, lenWithFFterm);
        omm_text_replace_func[i](pstr[i]);
    }
    omm_array_add(sPlayersStrings, ptr, pstr);
    mem_cpy(str64, pstr[playerIndex], lenWithFFterm);
    return str64;
}

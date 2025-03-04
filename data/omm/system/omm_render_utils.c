#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define FONT_TEXT_MISSING OMM_TEXTURE_FONT_TEXT_MISSING
#define FONT_HUD_MISSING OMM_TEXTURE_FONT_HUD_QMARK

//
// Fonts
//

static const char OMM_TEXTURE_FONT_TEXT_MISSING[] = "levels/menu/main_menu_seg7_us.0B700.ia8";

const char *gOmmFontText[0x100] = {
    /* 0x00 */ "levels/menu/main_menu_seg7_us.0AC40.ia8",
    /* 0x01 */ "levels/menu/main_menu_seg7_us.0AC80.ia8",
    /* 0x02 */ "levels/menu/main_menu_seg7_us.0ACC0.ia8",
    /* 0x03 */ "levels/menu/main_menu_seg7_us.0AD00.ia8",
    /* 0x04 */ "levels/menu/main_menu_seg7_us.0AD40.ia8",
    /* 0x05 */ "levels/menu/main_menu_seg7_us.0AD80.ia8",
    /* 0x06 */ "levels/menu/main_menu_seg7_us.0ADC0.ia8",
    /* 0x07 */ "levels/menu/main_menu_seg7_us.0AE00.ia8",
    /* 0x08 */ "levels/menu/main_menu_seg7_us.0AE40.ia8",
    /* 0x09 */ "levels/menu/main_menu_seg7_us.0AE80.ia8",
    /* 0x0A */ "levels/menu/main_menu_seg7_us.0AEC0.ia8",
    /* 0x0B */ "levels/menu/main_menu_seg7_us.0AF00.ia8",
    /* 0x0C */ "levels/menu/main_menu_seg7_us.0AF40.ia8",
    /* 0x0D */ "levels/menu/main_menu_seg7_us.0AF80.ia8",
    /* 0x0E */ "levels/menu/main_menu_seg7_us.0AFC0.ia8",
    /* 0x0F */ "levels/menu/main_menu_seg7_us.0B000.ia8",
    /* 0x10 */ "levels/menu/main_menu_seg7_us.0B040.ia8",
    /* 0x11 */ "levels/menu/main_menu_seg7_us.0B080.ia8",
    /* 0x12 */ "levels/menu/main_menu_seg7_us.0B0C0.ia8",
    /* 0x13 */ "levels/menu/main_menu_seg7_us.0B100.ia8",
    /* 0x14 */ "levels/menu/main_menu_seg7_us.0B140.ia8",
    /* 0x15 */ "levels/menu/main_menu_seg7_us.0B180.ia8",
    /* 0x16 */ "levels/menu/main_menu_seg7_us.0B1C0.ia8",
    /* 0x17 */ "levels/menu/main_menu_seg7_us.0B200.ia8",
    /* 0x18 */ "levels/menu/main_menu_seg7_us.0B240.ia8",
    /* 0x19 */ "levels/menu/main_menu_seg7_us.0B280.ia8",
    /* 0x1A */ "levels/menu/main_menu_seg7_us.0B2C0.ia8",
    /* 0x1B */ "levels/menu/main_menu_seg7_us.0B300.ia8",
    /* 0x1C */ "levels/menu/main_menu_seg7_us.0B340.ia8",
    /* 0x1D */ "levels/menu/main_menu_seg7_us.0B380.ia8",
    /* 0x1E */ "levels/menu/main_menu_seg7_us.0B3C0.ia8",
    /* 0x1F */ "levels/menu/main_menu_seg7_us.0B400.ia8",
    /* 0x20 */ "levels/menu/main_menu_seg7_us.0B440.ia8",
    /* 0x21 */ "levels/menu/main_menu_seg7_us.0B480.ia8",
    /* 0x22 */ "levels/menu/main_menu_seg7_us.0B4C0.ia8",
    /* 0x23 */ "levels/menu/main_menu_seg7_us.0B500.ia8",
    /* 0x24 */ "levels/menu/main_menu_seg7_us.0AEC0.ia8",
    /* 0x25 */ "levels/menu/main_menu_seg7_us.0AF00.ia8",
    /* 0x26 */ "levels/menu/main_menu_seg7_us.0AF40.ia8",
    /* 0x27 */ "levels/menu/main_menu_seg7_us.0AF80.ia8",
    /* 0x28 */ "levels/menu/main_menu_seg7_us.0AFC0.ia8",
    /* 0x29 */ "levels/menu/main_menu_seg7_us.0B000.ia8",
    /* 0x2A */ "levels/menu/main_menu_seg7_us.0B040.ia8",
    /* 0x2B */ "levels/menu/main_menu_seg7_us.0B080.ia8",
    /* 0x2C */ "levels/menu/main_menu_seg7_us.0B0C0.ia8",
    /* 0x2D */ "levels/menu/main_menu_seg7_us.0B100.ia8",
    /* 0x2E */ "levels/menu/main_menu_seg7_us.0B140.ia8",
    /* 0x2F */ "levels/menu/main_menu_seg7_us.0B180.ia8",
    /* 0x30 */ "levels/menu/main_menu_seg7_us.0B1C0.ia8",
    /* 0x31 */ "levels/menu/main_menu_seg7_us.0B200.ia8",
    /* 0x32 */ "levels/menu/main_menu_seg7_us.0B240.ia8",
    /* 0x33 */ "levels/menu/main_menu_seg7_us.0B280.ia8",
    /* 0x34 */ "levels/menu/main_menu_seg7_us.0B2C0.ia8",
    /* 0x35 */ "levels/menu/main_menu_seg7_us.0B300.ia8",
    /* 0x36 */ "levels/menu/main_menu_seg7_us.0B340.ia8",
    /* 0x37 */ "levels/menu/main_menu_seg7_us.0B380.ia8",
    /* 0x38 */ "levels/menu/main_menu_seg7_us.0B3C0.ia8",
    /* 0x39 */ "levels/menu/main_menu_seg7_us.0B400.ia8",
    /* 0x3A */ "levels/menu/main_menu_seg7_us.0B440.ia8",
    /* 0x3B */ "levels/menu/main_menu_seg7_us.0B480.ia8",
    /* 0x3C */ "levels/menu/main_menu_seg7_us.0B4C0.ia8",
    /* 0x3D */ "levels/menu/main_menu_seg7_us.0B500.ia8",
    /* 0x3E */ "levels/menu/main_menu_seg7_us.0B680.ia8",
    /* 0x3F */ "levels/menu/main_menu_seg7_us.0B7C0.ia8",
    /* 0x40 */ "levels/menu/main_menu_seg7_us.0B740.ia8",
    /* 0x41 */ "levels/menu/main_menu_seg7_us.0B780.ia8",
    /* 0x42 */ FONT_TEXT_MISSING,
    /* 0x43 */ FONT_TEXT_MISSING,
    /* 0x44 */ FONT_TEXT_MISSING,
    /* 0x45 */ FONT_TEXT_MISSING,
    /* 0x46 */ FONT_TEXT_MISSING,
    /* 0x47 */ FONT_TEXT_MISSING,
    /* 0x48 */ FONT_TEXT_MISSING,
    /* 0x49 */ FONT_TEXT_MISSING,
    /* 0x4A */ FONT_TEXT_MISSING,
    /* 0x4B */ FONT_TEXT_MISSING,
    /* 0x4C */ FONT_TEXT_MISSING,
    /* 0x4D */ FONT_TEXT_MISSING,
    /* 0x4E */ FONT_TEXT_MISSING,
    /* 0x4F */ FONT_TEXT_MISSING,
    /* 0x50 */ OMM_TEXTURE_FONT_TEXT_ARROW_UP,
    /* 0x51 */ OMM_TEXTURE_FONT_TEXT_ARROW_DOWN,
    /* 0x52 */ OMM_TEXTURE_FONT_TEXT_ARROW_LEFT,
    /* 0x53 */ OMM_TEXTURE_FONT_TEXT_ARROW_RIGHT,
    /* 0x54 */ OMM_TEXTURE_FONT_TEXT_A_BUTTON,
    /* 0x55 */ OMM_TEXTURE_FONT_TEXT_B_BUTTON,
    /* 0x56 */ OMM_TEXTURE_FONT_TEXT_C_BUTTON,
    /* 0x57 */ OMM_TEXTURE_FONT_TEXT_Z_BUTTON,
    /* 0x58 */ OMM_TEXTURE_FONT_TEXT_R_BUTTON,
    /* 0x59 */ OMM_TEXTURE_FONT_TEXT_L_BUTTON,
    /* 0x5A */ OMM_TEXTURE_FONT_TEXT_X_BUTTON,
    /* 0x5B */ OMM_TEXTURE_FONT_TEXT_Y_BUTTON,
    /* 0x5C */ OMM_TEXTURE_FONT_TEXT_D_BUTTON,
    /* 0x5D */ FONT_TEXT_MISSING,
    /* 0x5E */ FONT_TEXT_MISSING,
    /* 0x5F */ FONT_TEXT_MISSING,
    /* 0x60 */ FONT_TEXT_MISSING,
    /* 0x61 */ FONT_TEXT_MISSING,
    /* 0x62 */ FONT_TEXT_MISSING,
    /* 0x63 */ FONT_TEXT_MISSING,
    /* 0x64 */ FONT_TEXT_MISSING,
    /* 0x65 */ FONT_TEXT_MISSING,
    /* 0x66 */ FONT_TEXT_MISSING,
    /* 0x67 */ FONT_TEXT_MISSING,
    /* 0x68 */ FONT_TEXT_MISSING,
    /* 0x69 */ FONT_TEXT_MISSING,
    /* 0x6A */ FONT_TEXT_MISSING,
    /* 0x6B */ FONT_TEXT_MISSING,
    /* 0x6C */ FONT_TEXT_MISSING,
    /* 0x6D */ FONT_TEXT_MISSING,
    /* 0x6E */ FONT_TEXT_MISSING,
    /* 0x6F */ "levels/menu/main_menu_seg7_us.0B640.ia8",
    /* 0x70 */ FONT_TEXT_MISSING,
    /* 0x71 */ FONT_TEXT_MISSING,
    /* 0x72 */ FONT_TEXT_MISSING,
    /* 0x73 */ FONT_TEXT_MISSING,
    /* 0x74 */ FONT_TEXT_MISSING,
    /* 0x75 */ FONT_TEXT_MISSING,
    /* 0x76 */ FONT_TEXT_MISSING,
    /* 0x77 */ FONT_TEXT_MISSING,
    /* 0x78 */ FONT_TEXT_MISSING,
    /* 0x79 */ FONT_TEXT_MISSING,
    /* 0x7A */ FONT_TEXT_MISSING,
    /* 0x7B */ FONT_TEXT_MISSING,
    /* 0x7C */ FONT_TEXT_MISSING,
    /* 0x7D */ FONT_TEXT_MISSING,
    /* 0x7E */ FONT_TEXT_MISSING,
    /* 0x7F */ FONT_TEXT_MISSING,
    /* 0x80 */ FONT_TEXT_MISSING,
    /* 0x81 */ FONT_TEXT_MISSING,
    /* 0x82 */ FONT_TEXT_MISSING,
    /* 0x83 */ FONT_TEXT_MISSING,
    /* 0x84 */ FONT_TEXT_MISSING,
    /* 0x85 */ FONT_TEXT_MISSING,
    /* 0x86 */ FONT_TEXT_MISSING,
    /* 0x87 */ FONT_TEXT_MISSING,
    /* 0x88 */ FONT_TEXT_MISSING,
    /* 0x89 */ FONT_TEXT_MISSING,
    /* 0x8A */ FONT_TEXT_MISSING,
    /* 0x8B */ FONT_TEXT_MISSING,
    /* 0x8C */ FONT_TEXT_MISSING,
    /* 0x8D */ FONT_TEXT_MISSING,
    /* 0x8E */ FONT_TEXT_MISSING,
    /* 0x8F */ FONT_TEXT_MISSING,
    /* 0x90 */ FONT_TEXT_MISSING,
    /* 0x91 */ FONT_TEXT_MISSING,
    /* 0x92 */ FONT_TEXT_MISSING,
    /* 0x93 */ FONT_TEXT_MISSING,
    /* 0x94 */ FONT_TEXT_MISSING,
    /* 0x95 */ FONT_TEXT_MISSING,
    /* 0x96 */ FONT_TEXT_MISSING,
    /* 0x97 */ FONT_TEXT_MISSING,
    /* 0x98 */ FONT_TEXT_MISSING,
    /* 0x99 */ FONT_TEXT_MISSING,
    /* 0x9A */ FONT_TEXT_MISSING,
    /* 0x9B */ FONT_TEXT_MISSING,
    /* 0x9C */ FONT_TEXT_MISSING,
    /* 0x9D */ FONT_TEXT_MISSING,
    /* 0x9E */ FONT_TEXT_MISSING,
    /* 0x9F */ "levels/menu/main_menu_seg7_us.0B600.ia8",
    /* 0xA0 */ FONT_TEXT_MISSING,
    /* 0xA1 */ FONT_TEXT_MISSING,
    /* 0xA2 */ FONT_TEXT_MISSING,
    /* 0xA3 */ FONT_TEXT_MISSING,
    /* 0xA4 */ FONT_TEXT_MISSING,
    /* 0xA5 */ FONT_TEXT_MISSING,
    /* 0xA6 */ FONT_TEXT_MISSING,
    /* 0xA7 */ FONT_TEXT_MISSING,
    /* 0xA8 */ FONT_TEXT_MISSING,
    /* 0xA9 */ FONT_TEXT_MISSING,
    /* 0xAA */ FONT_TEXT_MISSING,
    /* 0xAB */ FONT_TEXT_MISSING,
    /* 0xAC */ FONT_TEXT_MISSING,
    /* 0xAD */ FONT_TEXT_MISSING,
    /* 0xAE */ FONT_TEXT_MISSING,
    /* 0xAF */ FONT_TEXT_MISSING,
    /* 0xB0 */ FONT_TEXT_MISSING,
    /* 0xB1 */ FONT_TEXT_MISSING,
    /* 0xB2 */ FONT_TEXT_MISSING,
    /* 0xB3 */ FONT_TEXT_MISSING,
    /* 0xB4 */ FONT_TEXT_MISSING,
    /* 0xB5 */ FONT_TEXT_MISSING,
    /* 0xB6 */ FONT_TEXT_MISSING,
    /* 0xB7 */ FONT_TEXT_MISSING,
    /* 0xB8 */ FONT_TEXT_MISSING,
    /* 0xB9 */ FONT_TEXT_MISSING,
    /* 0xBA */ FONT_TEXT_MISSING,
    /* 0xBB */ FONT_TEXT_MISSING,
    /* 0xBC */ FONT_TEXT_MISSING,
    /* 0xBD */ FONT_TEXT_MISSING,
    /* 0xBE */ FONT_TEXT_MISSING,
    /* 0xBF */ FONT_TEXT_MISSING,
    /* 0xC0 */ FONT_TEXT_MISSING,
    /* 0xC1 */ FONT_TEXT_MISSING,
    /* 0xC2 */ FONT_TEXT_MISSING,
    /* 0xC3 */ FONT_TEXT_MISSING,
    /* 0xC4 */ FONT_TEXT_MISSING,
    /* 0xC5 */ FONT_TEXT_MISSING,
    /* 0xC6 */ FONT_TEXT_MISSING,
    /* 0xC7 */ FONT_TEXT_MISSING,
    /* 0xC8 */ FONT_TEXT_MISSING,
    /* 0xC9 */ FONT_TEXT_MISSING,
    /* 0xCA */ FONT_TEXT_MISSING,
    /* 0xCB */ FONT_TEXT_MISSING,
    /* 0xCC */ FONT_TEXT_MISSING,
    /* 0xCD */ FONT_TEXT_MISSING,
    /* 0xCE */ FONT_TEXT_MISSING,
    /* 0xCF */ FONT_TEXT_MISSING,
    /* 0xD0 */ OMM_TEXTURE_FONT_TEXT_SLASH,
    /* 0xD1 */ FONT_TEXT_MISSING,
    /* 0xD2 */ FONT_TEXT_MISSING,
    /* 0xD3 */ FONT_TEXT_MISSING,
    /* 0xD4 */ FONT_TEXT_MISSING,
    /* 0xD5 */ FONT_TEXT_MISSING,
    /* 0xD6 */ FONT_TEXT_MISSING,
    /* 0xD7 */ FONT_TEXT_MISSING,
    /* 0xD8 */ FONT_TEXT_MISSING,
    /* 0xD9 */ FONT_TEXT_MISSING,
    /* 0xDA */ FONT_TEXT_MISSING,
    /* 0xDB */ FONT_TEXT_MISSING,
    /* 0xDC */ FONT_TEXT_MISSING,
    /* 0xDD */ FONT_TEXT_MISSING,
    /* 0xDE */ FONT_TEXT_MISSING,
    /* 0xDF */ FONT_TEXT_MISSING,
    /* 0xE0 */ FONT_TEXT_MISSING,
    /* 0xE1 */ OMM_TEXTURE_FONT_TEXT_BRACKET_LEFT,
    /* 0xE2 */ FONT_TEXT_MISSING,
    /* 0xE3 */ OMM_TEXTURE_FONT_TEXT_BRACKET_RIGHT,
    /* 0xE4 */ OMM_TEXTURE_FONT_TEXT_PLUS,
    /* 0xE5 */ "levels/menu/main_menu_seg7_us.0B800.ia8",
    /* 0xE6 */ OMM_TEXTURE_FONT_TEXT_COLON,
    /* 0xE7 */ FONT_TEXT_MISSING,
    /* 0xE8 */ FONT_TEXT_MISSING,
    /* 0xE9 */ FONT_TEXT_MISSING,
    /* 0xEA */ FONT_TEXT_MISSING,
    /* 0xEB */ FONT_TEXT_MISSING,
    /* 0xEC */ FONT_TEXT_MISSING,
    /* 0xED */ FONT_TEXT_MISSING,
    /* 0xEE */ FONT_TEXT_MISSING,
    /* 0xEF */ FONT_TEXT_MISSING,
    /* 0xF0 */ FONT_TEXT_MISSING,
    /* 0xF1 */ FONT_TEXT_MISSING,
    /* 0xF2 */ "levels/menu/main_menu_seg7_us.0B6C0.ia8",
    /* 0xF3 */ OMM_TEXTURE_FONT_TEXT_PERCENT,
    /* 0xF4 */ "levels/menu/main_menu_seg7_us.0B700.ia8",
    /* 0xF5 */ FONT_TEXT_MISSING,
    /* 0xF6 */ OMM_TEXTURE_FONT_TEXT_QUOTES,
    /* 0xF7 */ OMM_TEXTURE_FONT_TEXT_TILDE,
    /* 0xF8 */ FONT_TEXT_MISSING,
    /* 0xF9 */ "levels/menu/main_menu_seg7_us.0B540.ia8",
    /* 0xFA */ "levels/menu/main_menu_seg7_us.0B5C0.ia8",
    /* 0xFB */ "levels/menu/main_menu_seg7_us.0B580.ia8",
    /* 0xFC */ OMM_TEXTURE_FONT_TEXT_CAPPY,
    /* 0xFD */ OMM_TEXTURE_FONT_TEXT_STAR_EMPTY,
    /* 0xFE */ FONT_TEXT_MISSING,
    /* 0xFF */ FONT_TEXT_MISSING,
};

const char *gOmmFontHud[0x100] = {
    /* 0x00 */ "textures/segment2/segment2.00000.rgba16",
    /* 0x01 */ "textures/segment2/segment2.00200.rgba16",
    /* 0x02 */ "textures/segment2/segment2.00400.rgba16",
    /* 0x03 */ "textures/segment2/segment2.00600.rgba16",
    /* 0x04 */ "textures/segment2/segment2.00800.rgba16",
    /* 0x05 */ "textures/segment2/segment2.00A00.rgba16",
    /* 0x06 */ "textures/segment2/segment2.00C00.rgba16",
    /* 0x07 */ "textures/segment2/segment2.00E00.rgba16",
    /* 0x08 */ "textures/segment2/segment2.01000.rgba16",
    /* 0x09 */ "textures/segment2/segment2.01200.rgba16",
    /* 0x0A */ "textures/segment2/segment2.01400.rgba16",
    /* 0x0B */ "textures/segment2/segment2.01600.rgba16",
    /* 0x0C */ "textures/segment2/segment2.01800.rgba16",
    /* 0x0D */ "textures/segment2/segment2.01A00.rgba16",
    /* 0x0E */ "textures/segment2/segment2.01C00.rgba16",
    /* 0x0F */ "textures/segment2/segment2.01E00.rgba16",
    /* 0x10 */ "textures/segment2/segment2.02000.rgba16",
    /* 0x11 */ "textures/segment2/segment2.02200.rgba16",
    /* 0x12 */ "textures/segment2/segment2.02400.rgba16",
    /* 0x13 */ OMM_TEXTURE_FONT_HUD_J,
    /* 0x14 */ "textures/segment2/segment2.02800.rgba16",
    /* 0x15 */ "textures/segment2/segment2.02A00.rgba16",
    /* 0x16 */ "textures/segment2/segment2.02C00.rgba16",
    /* 0x17 */ "textures/segment2/segment2.02E00.rgba16",
    /* 0x18 */ "textures/segment2/segment2.03000.rgba16",
    /* 0x19 */ "textures/segment2/segment2.03200.rgba16",
    /* 0x1A */ OMM_TEXTURE_FONT_HUD_Q,
    /* 0x1B */ "textures/segment2/segment2.03600.rgba16",
    /* 0x1C */ "textures/segment2/segment2.03800.rgba16",
    /* 0x1D */ "textures/segment2/segment2.03A00.rgba16",
    /* 0x1E */ "textures/segment2/segment2.03C00.rgba16",
    /* 0x1F */ OMM_TEXTURE_FONT_HUD_V,
    /* 0x20 */ "textures/segment2/segment2.04000.rgba16",
    /* 0x21 */ OMM_TEXTURE_FONT_HUD_X,
    /* 0x22 */ "textures/segment2/segment2.04400.rgba16",
    /* 0x23 */ OMM_TEXTURE_FONT_HUD_Z,
    /* 0x24 */ FONT_HUD_MISSING,
    /* 0x25 */ FONT_HUD_MISSING,
    /* 0x26 */ FONT_HUD_MISSING,
    /* 0x27 */ FONT_HUD_MISSING,
    /* 0x28 */ FONT_HUD_MISSING,
    /* 0x29 */ FONT_HUD_MISSING,
    /* 0x2A */ FONT_HUD_MISSING,
    /* 0x2B */ FONT_HUD_MISSING,
    /* 0x2C */ FONT_HUD_MISSING,
    /* 0x2D */ FONT_HUD_MISSING,
    /* 0x2E */ FONT_HUD_MISSING,
    /* 0x2F */ FONT_HUD_MISSING,
    /* 0x30 */ FONT_HUD_MISSING,
    /* 0x31 */ FONT_HUD_MISSING,
    /* 0x32 */ "textures/segment2/segment2.05600.rgba16",
    /* 0x33 */ "textures/segment2/segment2.05800.rgba16",
    /* 0x34 */ "textures/segment2/segment2.05A00.rgba16",
#if OMM_GAME_IS_SMMS
    /* 0x35 */ "menu/smms/moon.rgba32",
#else
    /* 0x35 */ "textures/segment2/segment2.05C00.rgba16",
#endif
    /* 0x36 */ FONT_HUD_MISSING,
    /* 0x37 */ FONT_HUD_MISSING,
    /* 0x38 */ "textures/segment2/segment2.04800.rgba16",
    /* 0x39 */ "textures/segment2/segment2.04A00.rgba16",
    /* 0x3A */ FONT_HUD_MISSING,
    /* 0x3B */ FONT_HUD_MISSING,
    /* 0x3C */ FONT_HUD_MISSING,
    /* 0x3D */ FONT_HUD_MISSING,
    /* 0x3E */ FONT_HUD_MISSING,
    /* 0x3F */ FONT_HUD_MISSING,
    /* 0x40 */ FONT_HUD_MISSING,
    /* 0x41 */ FONT_HUD_MISSING,
    /* 0x42 */ FONT_HUD_MISSING,
    /* 0x43 */ FONT_HUD_MISSING,
    /* 0x44 */ FONT_HUD_MISSING,
    /* 0x45 */ FONT_HUD_MISSING,
    /* 0x46 */ FONT_HUD_MISSING,
    /* 0x47 */ FONT_HUD_MISSING,
    /* 0x48 */ FONT_HUD_MISSING,
    /* 0x49 */ FONT_HUD_MISSING,
    /* 0x4A */ FONT_HUD_MISSING,
    /* 0x4B */ FONT_HUD_MISSING,
    /* 0x4C */ FONT_HUD_MISSING,
    /* 0x4D */ FONT_HUD_MISSING,
    /* 0x4E */ FONT_HUD_MISSING,
    /* 0x4F */ FONT_HUD_MISSING,
    /* 0x50 */ FONT_HUD_MISSING,
    /* 0x51 */ FONT_HUD_MISSING,
    /* 0x52 */ FONT_HUD_MISSING,
    /* 0x53 */ FONT_HUD_MISSING,
    /* 0x54 */ FONT_HUD_MISSING,
    /* 0x55 */ FONT_HUD_MISSING,
    /* 0x56 */ FONT_HUD_MISSING,
    /* 0x57 */ FONT_HUD_MISSING,
    /* 0x58 */ FONT_HUD_MISSING,
    /* 0x59 */ FONT_HUD_MISSING,
    /* 0x5A */ FONT_HUD_MISSING,
    /* 0x5B */ FONT_HUD_MISSING,
    /* 0x5C */ FONT_HUD_MISSING,
    /* 0x5D */ FONT_HUD_MISSING,
    /* 0x5E */ FONT_HUD_MISSING,
    /* 0x5F */ FONT_HUD_MISSING,
    /* 0x60 */ FONT_HUD_MISSING,
    /* 0x61 */ FONT_HUD_MISSING,
    /* 0x62 */ FONT_HUD_MISSING,
    /* 0x63 */ FONT_HUD_MISSING,
    /* 0x64 */ FONT_HUD_MISSING,
    /* 0x65 */ FONT_HUD_MISSING,
    /* 0x66 */ FONT_HUD_MISSING,
    /* 0x67 */ FONT_HUD_MISSING,
    /* 0x68 */ FONT_HUD_MISSING,
    /* 0x69 */ FONT_HUD_MISSING,
    /* 0x6A */ FONT_HUD_MISSING,
    /* 0x6B */ FONT_HUD_MISSING,
    /* 0x6C */ FONT_HUD_MISSING,
    /* 0x6D */ FONT_HUD_MISSING,
    /* 0x6E */ FONT_HUD_MISSING,
    /* 0x6F */ FONT_HUD_MISSING,
    /* 0x70 */ FONT_HUD_MISSING,
    /* 0x71 */ FONT_HUD_MISSING,
    /* 0x72 */ FONT_HUD_MISSING,
    /* 0x73 */ FONT_HUD_MISSING,
    /* 0x74 */ FONT_HUD_MISSING,
    /* 0x75 */ FONT_HUD_MISSING,
    /* 0x76 */ FONT_HUD_MISSING,
    /* 0x77 */ FONT_HUD_MISSING,
    /* 0x78 */ FONT_HUD_MISSING,
    /* 0x79 */ FONT_HUD_MISSING,
    /* 0x7A */ FONT_HUD_MISSING,
    /* 0x7B */ FONT_HUD_MISSING,
    /* 0x7C */ FONT_HUD_MISSING,
    /* 0x7D */ FONT_HUD_MISSING,
    /* 0x7E */ OMM_TEXTURE_STAR_EMPTY_CLASSIC,
    /* 0x7F */ OMM_TEXTURE_STAR_EMPTY,
    /* 0x80 */ OMM_TEXTURE_STAR_FULL_0,
    /* 0x81 */ OMM_TEXTURE_STAR_FULL_1,
    /* 0x82 */ OMM_TEXTURE_STAR_FULL_2,
    /* 0x83 */ OMM_TEXTURE_STAR_FULL_3,
    /* 0x84 */ OMM_TEXTURE_STAR_FULL_4,
    /* 0x85 */ OMM_TEXTURE_STAR_FULL_5,
    /* 0x86 */ OMM_TEXTURE_STAR_FULL_6,
    /* 0x87 */ OMM_TEXTURE_STAR_FULL_7,
    /* 0x88 */ OMM_TEXTURE_STAR_FULL_8,
    /* 0x89 */ OMM_TEXTURE_STAR_FULL_9,
    /* 0x8A */ OMM_TEXTURE_STAR_FULL_10,
    /* 0x8B */ OMM_TEXTURE_STAR_FULL_11,
    /* 0x8C */ OMM_TEXTURE_STAR_FULL_12,
    /* 0x8D */ OMM_TEXTURE_STAR_FULL_13,
    /* 0x8E */ OMM_TEXTURE_STAR_FULL_14,
    /* 0x8F */ OMM_TEXTURE_STAR_FULL_15,
    /* 0x90 */ OMM_TEXTURE_STAR_FULL_16,
    /* 0x91 */ OMM_TEXTURE_STAR_FULL_17,
    /* 0x92 */ OMM_TEXTURE_STAR_FULL_18,
    /* 0x93 */ OMM_TEXTURE_STAR_FULL_19,
    /* 0x94 */ FONT_HUD_MISSING,
    /* 0x95 */ FONT_HUD_MISSING,
    /* 0x96 */ FONT_HUD_MISSING,
    /* 0x97 */ FONT_HUD_MISSING,
    /* 0x98 */ FONT_HUD_MISSING,
    /* 0x99 */ FONT_HUD_MISSING,
    /* 0x9A */ FONT_HUD_MISSING,
    /* 0x9B */ FONT_HUD_MISSING,
    /* 0x9C */ FONT_HUD_MISSING,
    /* 0x9D */ FONT_HUD_MISSING,
    /* 0x9E */ FONT_HUD_MISSING,
    /* 0x9F */ OMM_TEXTURE_FONT_HUD_HYPHEN,
    /* 0xA0 */ FONT_HUD_MISSING,
    /* 0xA1 */ FONT_HUD_MISSING,
    /* 0xA2 */ FONT_HUD_MISSING,
    /* 0xA3 */ FONT_HUD_MISSING,
    /* 0xA4 */ FONT_HUD_MISSING,
    /* 0xA5 */ FONT_HUD_MISSING,
    /* 0xA6 */ FONT_HUD_MISSING,
    /* 0xA7 */ FONT_HUD_MISSING,
    /* 0xA8 */ FONT_HUD_MISSING,
    /* 0xA9 */ FONT_HUD_MISSING,
    /* 0xAA */ FONT_HUD_MISSING,
    /* 0xAB */ FONT_HUD_MISSING,
    /* 0xAC */ FONT_HUD_MISSING,
    /* 0xAD */ FONT_HUD_MISSING,
    /* 0xAE */ FONT_HUD_MISSING,
    /* 0xAF */ FONT_HUD_MISSING,
    /* 0xB0 */ FONT_HUD_MISSING,
    /* 0xB1 */ FONT_HUD_MISSING,
    /* 0xB2 */ FONT_HUD_MISSING,
    /* 0xB3 */ FONT_HUD_MISSING,
    /* 0xB4 */ FONT_HUD_MISSING,
    /* 0xB5 */ FONT_HUD_MISSING,
    /* 0xB6 */ FONT_HUD_MISSING,
    /* 0xB7 */ FONT_HUD_MISSING,
    /* 0xB8 */ FONT_HUD_MISSING,
    /* 0xB9 */ FONT_HUD_MISSING,
    /* 0xBA */ FONT_HUD_MISSING,
    /* 0xBB */ FONT_HUD_MISSING,
    /* 0xBC */ FONT_HUD_MISSING,
    /* 0xBD */ FONT_HUD_MISSING,
    /* 0xBE */ FONT_HUD_MISSING,
    /* 0xBF */ FONT_HUD_MISSING,
    /* 0xC0 */ FONT_HUD_MISSING,
    /* 0xC1 */ FONT_HUD_MISSING,
    /* 0xC2 */ FONT_HUD_MISSING,
    /* 0xC3 */ FONT_HUD_MISSING,
    /* 0xC4 */ FONT_HUD_MISSING,
    /* 0xC5 */ FONT_HUD_MISSING,
    /* 0xC6 */ FONT_HUD_MISSING,
    /* 0xC7 */ FONT_HUD_MISSING,
    /* 0xC8 */ FONT_HUD_MISSING,
    /* 0xC9 */ FONT_HUD_MISSING,
    /* 0xCA */ FONT_HUD_MISSING,
    /* 0xCB */ FONT_HUD_MISSING,
    /* 0xCC */ FONT_HUD_MISSING,
    /* 0xCD */ FONT_HUD_MISSING,
    /* 0xCE */ FONT_HUD_MISSING,
    /* 0xCF */ FONT_HUD_MISSING,
    /* 0xD0 */ FONT_HUD_MISSING,
    /* 0xD1 */ FONT_HUD_MISSING,
    /* 0xD2 */ FONT_HUD_MISSING,
    /* 0xD3 */ FONT_HUD_MISSING,
    /* 0xD4 */ FONT_HUD_MISSING,
    /* 0xD5 */ FONT_HUD_MISSING,
    /* 0xD6 */ FONT_HUD_MISSING,
    /* 0xD7 */ FONT_HUD_MISSING,
    /* 0xD8 */ FONT_HUD_MISSING,
    /* 0xD9 */ FONT_HUD_MISSING,
    /* 0xDA */ FONT_HUD_MISSING,
    /* 0xDB */ FONT_HUD_MISSING,
    /* 0xDC */ FONT_HUD_MISSING,
    /* 0xDD */ FONT_HUD_MISSING,
    /* 0xDE */ FONT_HUD_MISSING,
    /* 0xDF */ FONT_HUD_MISSING,
    /* 0xE0 */ FONT_HUD_MISSING,
    /* 0xE1 */ FONT_HUD_MISSING,
    /* 0xE2 */ FONT_HUD_MISSING,
    /* 0xE3 */ FONT_HUD_MISSING,
    /* 0xE4 */ FONT_HUD_MISSING,
    /* 0xE5 */ FONT_HUD_MISSING,
    /* 0xE6 */ FONT_HUD_MISSING,
    /* 0xE7 */ FONT_HUD_MISSING,
    /* 0xE8 */ FONT_HUD_MISSING,
    /* 0xE9 */ FONT_HUD_MISSING,
    /* 0xEA */ FONT_HUD_MISSING,
    /* 0xEB */ FONT_HUD_MISSING,
    /* 0xEC */ FONT_HUD_MISSING,
    /* 0xED */ FONT_HUD_MISSING,
    /* 0xEE */ FONT_HUD_MISSING,
    /* 0xEF */ FONT_HUD_MISSING,
    /* 0xF0 */ FONT_HUD_MISSING,
    /* 0xF1 */ FONT_HUD_MISSING,
    /* 0xF2 */ FONT_HUD_MISSING,
    /* 0xF3 */ FONT_HUD_MISSING,
    /* 0xF4 */ OMM_TEXTURE_FONT_HUD_QMARK,
    /* 0xF5 */ FONT_HUD_MISSING,
    /* 0xF6 */ FONT_HUD_MISSING,
    /* 0xF7 */ FONT_HUD_MISSING,
    /* 0xF8 */ FONT_HUD_MISSING,
    /* 0xF9 */ FONT_HUD_MISSING,
    /* 0xFA */ FONT_HUD_MISSING,
    /* 0xFB */ FONT_HUD_MISSING,
    /* 0xFC */ FONT_HUD_MISSING,
    /* 0xFD */ FONT_HUD_MISSING,
    /* 0xFE */ FONT_HUD_MISSING,
    /* 0xFF */ FONT_HUD_MISSING,
};

//
//       COORDS
//
//   |
//   |   _    _
//   |  | \  / |
//   |  ||\\//||
// y--- || \/ ||
//   |
//   +--|---------
//      x
///////////////////

void omm_render_update_scroll(Scroll *scroll, s32 count, s16 stickValue) {
    s32 val = 2 * (stickValue > +OMM_SCROLL_STICK_VALUE_MIN) + 1 * (stickValue < -OMM_SCROLL_STICK_VALUE_MIN);
    s32 inc = (((val ^ scroll->val) & val) == 2) - (((val ^ scroll->val) & val) == 1);
    if (inc != 0) {
        scroll->idx += inc;
        play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundArgs);
    }
    if (scroll->tmr == 10) {
        scroll->tmr = 8;
        scroll->val = 0;
    } else {
        scroll->tmr = (scroll->tmr + 1) * (val != 0);
        scroll->val = val;
    }
    scroll->idx = ((scroll->idx + count) % count);
    scroll->inc = inc;
}

void omm_render_create_dl_ortho_matrix() {
    gDisplayListHead = gfx_create_ortho_matrix(gDisplayListHead);
}

void omm_render_shade_screen(u8 alpha) {
    Vtx *vtx = omm_alloc_vtx(4);
    vtx[0] = (Vtx) {{{ GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 0, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF }}};
    vtx[1] = (Vtx) {{{ GFX_DIMENSIONS_FROM_RIGHT_EDGE(0), 0, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF }}};
    vtx[2] = (Vtx) {{{ GFX_DIMENSIONS_FROM_RIGHT_EDGE(0), SCREEN_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF }}};
    vtx[3] = (Vtx) {{{ GFX_DIMENSIONS_FROM_LEFT_EDGE(0), SCREEN_HEIGHT, 0 }, 0, { 0, 0 }, { 0x00, 0x00, 0x00, 0xFF }}};
    omm_render_create_dl_ortho_matrix();
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, alpha);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
}

void omm_render_texrect(s16 x, s16 y, s16 w, s16 h, s16 texw, s16 texh, u8 red, u8 green, u8 blue, u8 alpha, const void *texture, bool shadow) {
    if (w <= 0 || h <= 0) return;
    if (shadow) omm_render_texrect(x + 1, y - 1, w, h, texw, texh, 0, 0, 0, alpha, texture, false);
    omm_render_create_dl_ortho_matrix();
    gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, texw, texh, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gSPTextureRectangle(gDisplayListHead++, (x) << 2, (SCREEN_HEIGHT - h - y) << 2, (x + w) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, ((0x400 * texw) / w), ((0x400 * texh) / h));
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
}

void omm_render_rect_window_coords(f32 x, f32 y, f32 w, f32 h, u8 red, u8 green, u8 blue, u8 alpha) {
    Vtx *vtx = omm_alloc_vtx(4);
    omm_render_create_dl_ortho_matrix();
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
    vtx[0] = (Vtx) {{{ X_WINDOW_TO_SCREEN(x),     Y_WINDOW_TO_SCREEN(y),     0 }, 0, { 0, 0 }, { red, green, blue, alpha }}};
    vtx[1] = (Vtx) {{{ X_WINDOW_TO_SCREEN(x + w), Y_WINDOW_TO_SCREEN(y),     0 }, 0, { 0, 0 }, { red, green, blue, alpha }}};
    vtx[2] = (Vtx) {{{ X_WINDOW_TO_SCREEN(x + w), Y_WINDOW_TO_SCREEN(y + h), 0 }, 0, { 0, 0 }, { red, green, blue, alpha }}};
    vtx[3] = (Vtx) {{{ X_WINDOW_TO_SCREEN(x),     Y_WINDOW_TO_SCREEN(y + h), 0 }, 0, { 0, 0 }, { red, green, blue, alpha }}};
}

void omm_render_number(s16 x, s16 y, s16 w, s16 h, s16 xStep, u8 alpha, s32 number, s32 digits, bool makeZerosTransparent, bool shadow) {
    static const char *OMM_HUD_NUMBER_GLYPHS[] = {
        OMM_TEXTURE_HUD_0,
        OMM_TEXTURE_HUD_1,
        OMM_TEXTURE_HUD_2,
        OMM_TEXTURE_HUD_3,
        OMM_TEXTURE_HUD_4,
        OMM_TEXTURE_HUD_5,
        OMM_TEXTURE_HUD_6,
        OMM_TEXTURE_HUD_7,
        OMM_TEXTURE_HUD_8,
        OMM_TEXTURE_HUD_9,
    };

    s32 glyphs[16];
    number = clamp_s(number, 0, 0x7FFFFFFF);
    digits = clamp_s(digits, 1, 15);
    for (s32 i = 0; i != digits; ++i) {
        glyphs[i] = (number % 10);
        number /= 10;
    }
    for (s32 i = digits, j = 0, n = 0, a = 0; i != 0; --i, x += xStep) {
        n = glyphs[i - 1];
        j = max_s(j, n);
        a = (alpha * (0xFF - 0xAA * (makeZerosTransparent && ((digits == 1) || (i > 1)) && (j == 0)))) / 0xFF;
        omm_render_texrect(x, y, w, h, 16, 16, 0xFF, 0xFF, 0xFF, a, OMM_HUD_NUMBER_GLYPHS[n], shadow);
    }
}

static void omm_render_string_char(u8 c, s16 x, s16 y, s16 w, s16 h, u8 red, u8 green, u8 blue, u8 alpha, bool useDialogFontForMissingChars) {
    f32 wRatio = (f32) w / 8.f;
    f32 hRatio = (f32) h / 8.f;
    const char *tex = gOmmFontText[c];
    if (tex == FONT_TEXT_MISSING && useDialogFontForMissingChars) {
        tex = ((const char **) main_font_lut)[c];
        gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
        gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
        create_dl_translation_matrix(MENU_MTX_PUSH, x + wRatio, y - 3 * hRatio, 0);
        create_dl_scale_matrix(MENU_MTX_NOPUSH, wRatio, hRatio, 1.f);
        gDPPipeSync(gDisplayListHead++);
        gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, tex);
        gSPDisplayList(gDisplayListHead++, dl_ia_text_tex_settings);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
        gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
        gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
    } else {
        gDPLoadTextureBlock(gDisplayListHead++, tex, G_IM_FMT_IA, G_IM_SIZ_8b, 8, 8, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
        gSPTextureRectangle(gDisplayListHead++, x << 2, (SCREEN_HEIGHT - h - y) << 2, (x + w) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, (s32) ((1 << 10) / wRatio), (s32) ((1 << 10) / hRatio));
    }
}

void omm_render_string(s16 x, s16 y, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow) {
    if (shadow) omm_render_string(x + 1, y - 1, 0, 0, 0, alpha, str64, false);
    omm_render_create_dl_ortho_matrix();
    u8 *temp = omm_text_capitalize(omm_text_duplicate(str64, false));
    gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
    for (; *temp != DIALOG_CHAR_TERMINATOR; temp++) {
        if (*temp != DIALOG_CHAR_SPACE && *temp != DIALOG_CHAR_STAR_COUNT) {
            omm_render_string_char(*temp, x, y, 8, 8, red, green, blue, alpha, OMM_GAME_IS_R96X);
        }
        x += omm_render_get_char_width(*temp);
    }
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

void omm_render_string_sized(s16 x, s16 y, s16 w, s16 h, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow) {
    if (w <= 0 || h <= 0) return;
    if (shadow) omm_render_string_sized(x + 1, y - 1, w, h, 0, 0, 0, alpha, str64, false);
    f32 wRatio = (f32) w / 8.f;
    omm_render_create_dl_ortho_matrix();
    u8 *temp = omm_text_capitalize(omm_text_duplicate(str64, false));
    gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
    for (; *temp != DIALOG_CHAR_TERMINATOR; temp++) {
        if (*temp != DIALOG_CHAR_SPACE && *temp != DIALOG_CHAR_STAR_COUNT) {
            omm_render_string_char(*temp, x, y, w, h, red, green, blue, alpha, OMM_GAME_IS_R96X);
        }
        x += (s16) ((f32) omm_render_get_char_width(*temp) * wRatio + 0.5f);
    }
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

void omm_render_string_hud(s16 x, s16 y, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow) {
    if (shadow) omm_render_string_hud(x + 1, y - 1, 0, 0, 0, alpha, str64, false);
    omm_render_create_dl_ortho_matrix();
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
    for (; *str64 != 0xFF; str64++) {
        if (*str64 != GLOBAL_CHAR_SPACE) {
            gDPLoadTextureBlock(gDisplayListHead++, gOmmFontHud[*str64], G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
            gSPTextureRectangle(gDisplayListHead++, x << 2, (SCREEN_HEIGHT - 16 - y) << 2, (x + 16) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        }
        x += omm_render_get_char_width_hud(*str64);
    }
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

s32 omm_render_get_char_width(u8 c) {
    return ((c == DIALOG_CHAR_SPACE) ? 4 : max_s(4, gDialogCharWidths[c - (26 * (c >= 0x24 && c <= 0x3D))]));
}

s32 omm_render_get_char_width_hud(u8 c) {
    return ((c == DIALOG_CHAR_SPACE) ? 8 : 12);
}

s32 omm_render_get_string_width(const u8 *str64) {
    s32 width = 0;
    for (; *str64 != 0xFF; str64++) {
        width += omm_render_get_char_width(*str64);
    }
    return width;
}

s32 omm_render_get_string_width_sized(const u8 *str64, s16 w) {
    s32 width = 0;
    f32 wRatio = (f32) w / 8.f;
    for (; *str64 != 0xFF; str64++) {
        width += (s16) ((f32) omm_render_get_char_width(*str64) * wRatio + 0.5f);
    }
    return width;
}

s32 omm_render_get_string_width_hud(const u8 *str64) {
    s32 width = 0;
    for (; *str64 != 0xFF; str64++) {
        width += omm_render_get_char_width_hud(*str64);
    }
    return width;
}

const void *omm_render_get_star_glyph(s32 index, MODE_INDEX s32 modeIndex, bool colored, bool collected) {
    return (const void *) gOmmFontHud[colored ?
        (collected ? 0x80 + OMM_STAR_COLOR_[index + OMM_STAR_COLOR_OFFSET(modeIndex)] : 0x7F) :
        (collected ? GLYPH_STAR : 0x7E)
    ];
}

const u8 *omm_render_get_star_rgb(s32 modeIndex, bool colored, bool collected) {
    static const u8 sStarRGB[][3] = { { 0xFF, 0xFF, 0xFF }, OMM_STAR_COLOR_CLASSIC };
    return sStarRGB[(!colored && collected) * (modeIndex + 1)];
}

OMM_AT_STARTUP static void omm_patch_dialog_char_widths() {
    gDialogCharWidths[0x50] = 8; // up arrow
    gDialogCharWidths[0x51] = 8; // down arrow
    gDialogCharWidths[0x52] = 8; // left arrow
    gDialogCharWidths[0x53] = 8; // right arrow
    gDialogCharWidths[0x54] = 7; // A button
    gDialogCharWidths[0x55] = 7; // B button
    gDialogCharWidths[0x56] = 7; // C button
    gDialogCharWidths[0x57] = 7; // Z button
    gDialogCharWidths[0x58] = 7; // R button
    gDialogCharWidths[0x59] = 6; // L button
    gDialogCharWidths[0x5A] = 9; // X button
    gDialogCharWidths[0x5B] = 7; // Y button
    gDialogCharWidths[0x5C] = 7; // D button
}

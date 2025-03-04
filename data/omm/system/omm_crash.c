#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#if WINDOWS_BUILD && IS_64_BIT
#include "pc/gfx/gfx_window_manager_api.h"
#include "pc/gfx/gfx_dxgi.h"
#include "pc/gfx/gfx_sdl.h"
#include <windows.h>

#define CHAR_WIDTH   (3)
#define CHAR_HEIGHT  (5)
#define LINE_SPACING (CHAR_HEIGHT)

static const struct {
    u32 code;
    const char *error;
    const char *message;
} OMM_CRASH_HANDLER_ERRORS[] = {
    { EXCEPTION_ACCESS_VIOLATION,       "Segmentation Fault",       "The game tried to %s at address 0x%016llX." },
    { EXCEPTION_ARRAY_BOUNDS_EXCEEDED,  "Array Out Of Bounds",      "The game tried to access an element out of the array bounds." },
    { EXCEPTION_DATATYPE_MISALIGNMENT,  "Data Misalignment",        "The game tried to access misaligned data." },
    { EXCEPTION_BREAKPOINT,             "Breakpoint",               "The game reached a breakpoint." },
    { EXCEPTION_FLT_DENORMAL_OPERAND,   "Float Denormal Operand",   "The game tried to perform a floating point operation with a denormal operand." },
    { EXCEPTION_FLT_DIVIDE_BY_ZERO,     "Float Division By Zero",   "The game tried to divide a floating point number by zero." },
    { EXCEPTION_FLT_INEXACT_RESULT,     "Float Inexact Result",     "The game couldn't represent the result of a floating point operation as a decimal fraction." },
    { EXCEPTION_FLT_INVALID_OPERATION,  "Float Invalid Operation",  "The game tried to perform an invalid floating point operation." },
    { EXCEPTION_FLT_OVERFLOW,           "Float Overflow",           "An overflow occurred with a floating point number." },
    { EXCEPTION_FLT_STACK_CHECK,        "Float Stack Overflow",     "The game performed a floating point operation resulting in a stack overflow." },
    { EXCEPTION_FLT_UNDERFLOW,          "Float Underflow",          "An underflow occurred with a floating point number." },
    { EXCEPTION_ILLEGAL_INSTRUCTION,    "Illegal Instruction",      "The game tried to execute an invalid instruction." },
    { EXCEPTION_IN_PAGE_ERROR,          "Page Error",               "The game tried to %s at address 0x%016llX." },
    { EXCEPTION_INT_DIVIDE_BY_ZERO,     "Integer Division By Zero", "The game tried to divide an integer by zero." },
    { EXCEPTION_INT_OVERFLOW,           "Integer Overflow",         "An overflow occurred with an integer." },
    { EXCEPTION_PRIV_INSTRUCTION,       "Instruction Not Allowed",  "The game tried to execute an invalid instruction." },
    { EXCEPTION_STACK_OVERFLOW,         "Stack Overflow",           "The game performed an operation resulting in a stack overflow." },
    { 0,                                "Unknown Exception",        "An unknown exception occurred." },
};

typedef struct {
    s32 x, y;
    u8 r, g, b;
    char s[128];
} OmmCrashHandlerText;
static OmmCrashHandlerText sOmmCrashHandlerText[256];

#define omm_crash_handler_set_text(_x_, _y_, _r_, _g_, _b_, _fmt_, ...) \
{                                                                       \
    if (_x_ == -1) {                                                    \
        pText->x = ((pText - 1)->x + strlen((pText - 1)->s));           \
    } else {                                                            \
        pText->x = _x_;                                                 \
    }                                                                   \
    pText->y = _y_;                                                     \
    pText->r = _r_;                                                     \
    pText->g = _g_;                                                     \
    pText->b = _b_;                                                     \
    str_fmt(pText->s, 128, _fmt_, __VA_ARGS__);                         \
    pText++;                                                            \
}

extern void gfx_clear_frame_mtx();
extern void gfx_clear_frame_transition();
extern void gfx_clear_frame_dialog();
extern void gfx_clear_frame_hud();
extern void gfx_clear_frame_effects();
extern void gfx_clear_frame_paintings();
static void omm_crash_handler_produce_one_frame() {
    static Vp sViewport = { {
        { 640, 480, 511, 0 },
        { 640, 480, 511, 0 },
    } };

    // Start frame
    gfx_start_frame();
    gFrameInterpolation = false;
    config_gfx_pool();
    init_render_image();
    gSPViewport(gDisplayListHead++, &sViewport);

    // Clear screen
    create_dl_translation_matrix(MENU_MTX_PUSH, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 240.f, 0.f);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, (GFX_DIMENSIONS_ASPECT_RATIO * SCREEN_HEIGHT) / 130.f, 3.f, 1.f);
    gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, 0xFF);
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // Print background
    f32 s = gfx_current_dimensions.aspect_ratio;
    s32 w = SCREEN_HEIGHT * (s / 3.5f);
    s32 x = GFX_DIMENSIONS_FROM_RIGHT_EDGE(8 * sqr_f(s) + w);
    s32 y = 12 * s - 16;
    omm_render_create_dl_ortho_matrix();
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock(gDisplayListHead++, (const void *) "misc/omm_texture_misc_crash.rgba32", G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gSPTextureRectangle(gDisplayListHead++, (x) << 2, (SCREEN_HEIGHT - w - y) << 2, (x + w) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, (0x4000 / w), (0x4000 / w));
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, 0xFF);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);

    // Print text
    for (OmmCrashHandlerText *text = sOmmCrashHandlerText; text->s[0] != 0; ++text) {
        s32 x = text->x;
        s32 y = text->y;
        if (gfx_current_dimensions.aspect_ratio < 1.6f && x >= 105) {
            x -= 30;
            y += 14;
        }
        gDisplayListHead = gfx_font_init(gDisplayListHead, OMM_TEXTURE_FONT_DISPLAY, 16, 768, 480, 48, 80);
        for (char *c = text->s; *c != 0; ++c, ++x) {
            gDisplayListHead = gfx_font_render_char(
                gDisplayListHead, *c,
                (s32) GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE((x + 1) * CHAR_WIDTH),
                (s32) (SCREEN_HEIGHT - CHAR_HEIGHT - (4 + LINE_SPACING * y)),
                CHAR_WIDTH, CHAR_HEIGHT, 0, 0,
                text->r, text->g, text->b, 0xFF
            );
        }
        gDisplayListHead = gfx_font_end(gDisplayListHead);
    }

    // Clear interpolated stuff
    gfx_clear_frame_mtx();
    gfx_clear_frame_transition();
    gfx_clear_frame_dialog();
    gfx_clear_frame_hud();
    gfx_clear_frame_effects();
    gfx_clear_frame_paintings();

    // Render frame
    end_master_display_list();
    alloc_display_list(0);
    display_and_vsync();
    gfx_end_frame();
}

static LONG omm_crash_handler(EXCEPTION_POINTERS *ExceptionInfo) {
    mem_zero(sOmmCrashHandlerText, sizeof(sOmmCrashHandlerText));
    OmmCrashHandlerText *pText = &sOmmCrashHandlerText[0];

    // Oops, the game crashed
    omm_crash_handler_set_text(0, 0, 0xFF, 0x80, 0x00, "%s", "Odyssey Mario's Moveset Crash Handler");
    omm_crash_handler_set_text(0, 1, 0xFF, 0x80, 0x00, "%s", "The game crashed. Please report this crash with a consistent way to reproduce it.");

    // Exception address, code, type and info
    if (ExceptionInfo && ExceptionInfo->ExceptionRecord) {
        PEXCEPTION_RECORD er = ExceptionInfo->ExceptionRecord;
        omm_crash_handler_set_text( 0, 3, 0xFF, 0x00, 0x00, "%s", "Exception occurred at address ");
        omm_crash_handler_set_text(-1, 3, 0xFF, 0xFF, 0x00, "0x%016llX", (uintptr_t) er->ExceptionAddress);
        omm_crash_handler_set_text(-1, 3, 0xFF, 0x00, 0x00, "%s", " with error code ");
        omm_crash_handler_set_text(-1, 3, 0xFF, 0x00, 0xFF, "0x%08X", (u32) er->ExceptionCode);
        omm_crash_handler_set_text(-1, 3, 0xFF, 0x00, 0x00, "%s", ":");
        for (s32 i = 0; i != array_length(OMM_CRASH_HANDLER_ERRORS); ++i) {
            if (OMM_CRASH_HANDLER_ERRORS[i].code == (u32) er->ExceptionCode || OMM_CRASH_HANDLER_ERRORS[i].code == 0) {
                omm_crash_handler_set_text( 0, 4, 0xFF, 0x00, 0x00, "%s", OMM_CRASH_HANDLER_ERRORS[i].error);
                omm_crash_handler_set_text(-1, 4, 0xFF, 0xFF, 0xFF, "%s", " - ");
                if (er->ExceptionCode == EXCEPTION_ACCESS_VIOLATION || er->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) {
                    omm_crash_handler_set_text(-1, 4, 0xFF, 0xFF, 0xFF, OMM_CRASH_HANDLER_ERRORS[i].message, (er->ExceptionInformation[0] ? "write" : "read"), (uintptr_t) er->ExceptionInformation[1]);
                } else {
                    omm_crash_handler_set_text(-1, 4, 0xFF, 0xFF, 0xFF, "%s", OMM_CRASH_HANDLER_ERRORS[i].message);
                }
                break;
            }
        }
    } else {
        omm_crash_handler_set_text(0, 3, 0xFF, 0x00, 0x00, "%s", "An unknown exception occurred somewhere in the game's code.");
        omm_crash_handler_set_text(0, 4, 0x80, 0x80, 0x80, "%s", "Unable to retrieve the exception info.");
    }

    // Useful info
    omm_crash_handler_set_text(105, 0, 0xFF, 0xFF, 0x80, "%s", "Ver.: ");
    omm_crash_handler_set_text( -1, 0, 0x00, 0xC0, 0xFF, "%s", OMM_VERSION);
    omm_crash_handler_set_text(105, 1, 0xFF, 0xFF, 0x80, "%s", "Game: ");
    omm_crash_handler_set_text( -1, 1, 0x00, 0xC0, 0xFF, "%s", OMM_GAME_CODE);
    omm_crash_handler_set_text(105, 2, 0xFF, 0xFF, 0x80, "%s", "Arch: ");
#if IS_64_BIT
    omm_crash_handler_set_text( -1, 2, 0x00, 0xC0, 0xFF, "%s", "64-bit");
#else
    omm_crash_handler_set_text( -1, 2, 0x00, 0xC0, 0xFF, "%s", "32-bit");
#endif
    omm_crash_handler_set_text(105, 3, 0xFF, 0xFF, 0x80, "%s", "RAPI: ");
#if defined(RAPI_GL)
    omm_crash_handler_set_text( -1, 3, 0x00, 0xC0, 0xFF, "%s", "OpenGL 2.1");
#elif defined(RAPI_GL_LEGACY)
    omm_crash_handler_set_text( -1, 3, 0x00, 0xC0, 0xFF, "%s", "OpenGL 1.1");
#elif defined(RAPI_D3D11)
    omm_crash_handler_set_text( -1, 3, 0x00, 0xC0, 0xFF, "%s", "DirectX 11");
#elif defined(RAPI_D3D12)
    omm_crash_handler_set_text( -1, 3, 0x00, 0xC0, 0xFF, "%s", "DirectX 12");
#else
    omm_crash_handler_set_text( -1, 3, 0x00, 0xC0, 0xFF, "%s", "Unknown");
#endif
#define STUB_LEVEL(_1, lvl, ...) STRINGIFY(lvl),
#define DEFINE_LEVEL(_1, lvl, ...) STRINGIFY(lvl),
    static const char *sLevelNames[] = {
        "LEVEL_NONE",
#include "levels/level_defines.h"
    };
#undef STUB_LEVEL
#undef DEFINE_LEVEL
    omm_crash_handler_set_text(105, 4, 0xFF, 0xFF, 0x80, "%s", "Char: ");
    omm_crash_handler_set_text( -1, 4, 0x00, 0xC0, 0xFF, "%s", omm_player_properties_get_selected_name_upper());
    omm_crash_handler_set_text(105, 5, 0xFF, 0xFF, 0x80, "%s", "File: ");
    omm_crash_handler_set_text( -1, 5, 0x00, 0xC0, 0xFF, "Mario %c", 'A' + gCurrSaveFileNum - 1);
    omm_crash_handler_set_text(105, 6, 0xFF, 0xFF, 0x80, "%s", "Lev.: ");
    omm_crash_handler_set_text( -1, 6, 0x00, 0xC0, 0xFF, "%s", sLevelNames[gCurrLevelNum] + 6);
    omm_crash_handler_set_text(105, 7, 0xFF, 0xFF, 0x80, "%s", "Area: ");
    omm_crash_handler_set_text( -1, 7, 0x00, 0xC0, 0xFF, "%d", gCurrAreaIndex);
    omm_crash_handler_set_text(105, 8, 0xFF, 0xFF, 0x80, "%s", "Star: ");
    omm_crash_handler_set_text( -1, 8, 0x00, 0xC0, 0xFF, "%d", gCurrActNum);
    omm_crash_handler_set_text(105, 9, 0xFF, 0xFF, 0x80, "%s", "Room: ");
    omm_crash_handler_set_text( -1, 9, 0x00, 0xC0, 0xFF, "%d", gMarioCurrentRoom);

    // Registers
    omm_crash_handler_set_text(0, 6, 0xFF, 0xFF, 0xFF, "%s", "Registers:");
    if (ExceptionInfo && ExceptionInfo->ContextRecord) {
        PCONTEXT cr = ExceptionInfo->ContextRecord;
        omm_crash_handler_set_text( 0,  7, 0xFF, 0xFF, 0xFF,   "RSP: 0x%016llX", cr->Rsp);
        omm_crash_handler_set_text(-1,  7, 0xFF, 0xFF, 0xFF, "  RBP: 0x%016llX", cr->Rbp);
        omm_crash_handler_set_text(-1,  7, 0xFF, 0xFF, 0xFF, "  RIP: 0x%016llX", cr->Rip);
        omm_crash_handler_set_text( 0,  8, 0xFF, 0xFF, 0xFF,   "RAX: 0x%016llX", cr->Rax);
        omm_crash_handler_set_text(-1,  8, 0xFF, 0xFF, 0xFF, "  RBX: 0x%016llX", cr->Rbx);
        omm_crash_handler_set_text(-1,  8, 0xFF, 0xFF, 0xFF, "  RCX: 0x%016llX", cr->Rcx);
        omm_crash_handler_set_text(-1,  8, 0xFF, 0xFF, 0xFF, "  RDX: 0x%016llX", cr->Rdx);
        omm_crash_handler_set_text( 0,  9, 0xFF, 0xFF, 0xFF,   "R08: 0x%016llX", cr->R8);
        omm_crash_handler_set_text(-1,  9, 0xFF, 0xFF, 0xFF, "  R09: 0x%016llX", cr->R9);
        omm_crash_handler_set_text(-1,  9, 0xFF, 0xFF, 0xFF, "  R10: 0x%016llX", cr->R10);
        omm_crash_handler_set_text(-1,  9, 0xFF, 0xFF, 0xFF, "  R11: 0x%016llX", cr->R11);
        omm_crash_handler_set_text( 0, 10, 0xFF, 0xFF, 0xFF,   "R12: 0x%016llX", cr->R12);
        omm_crash_handler_set_text(-1, 10, 0xFF, 0xFF, 0xFF, "  R13: 0x%016llX", cr->R13);
        omm_crash_handler_set_text(-1, 10, 0xFF, 0xFF, 0xFF, "  R14: 0x%016llX", cr->R14);
        omm_crash_handler_set_text(-1, 10, 0xFF, 0xFF, 0xFF, "  R15: 0x%016llX", cr->R15);
        omm_crash_handler_set_text( 0, 11, 0xFF, 0xFF, 0xFF,   "RSI: 0x%016llX", cr->Rsi);
        omm_crash_handler_set_text(-1, 11, 0xFF, 0xFF, 0xFF, "  RDI: 0x%016llX", cr->Rdi);
    } else {
        omm_crash_handler_set_text(0, 7, 0x80, 0x80, 0x80, "%s", "Unable to access the registers.");
    }

    // Stack trace
    omm_crash_handler_set_text(0, 13, 0xFF, 0xFF, 0xFF, "%s", "Stack trace:");
    if (ExceptionInfo && ExceptionInfo->ContextRecord) {
        typedef struct Symbol { uintptr_t offset; char name[128]; struct Symbol *next; } Symbol;
        Symbol *symbols = NULL;
        Symbol *symbol0 = NULL;

        // Load symbols
        fs_file_t *file = fs_open("omm." OMM_GAME_CODE ".map");
        if (file) {
            char buffer[1024];
            while (fs_readline(file, buffer, 1024)) {
                s32 length = strlen(buffer);
                if (length > 17) {
                    buffer[strcspn(buffer, "\r\n")] = 0;
                    const char *addr = buffer;
                    const char *name = buffer + 17;

                    // New symbol
                    Symbol *newSymbol = mem_new(Symbol, 1);
                    str_fmt(newSymbol->name, 128, "%s", name);
                    sscanf(addr, "%016llX", &newSymbol->offset);
                    newSymbol->next = NULL;

                    // Store symbol
                    if (!symbols) {
                        symbols = newSymbol;
                    } else {
                        for (Symbol *symbol = symbols;; symbol = symbol->next) {
                            if (!symbol->next) {
                                symbol->next = newSymbol;
                                break;
                            }
                            if (symbol->next->offset > newSymbol->offset) {
                                newSymbol->next = symbol->next;
                                symbol->next = newSymbol;
                                break;
                            }
                        }
                    }

                    // Reference
                    if (mem_eq(newSymbol->name, "set_mario_action", sizeof("set_mario_action"))) {
                        symbol0 = newSymbol;
                    }
                }
            }
            fs_close(file);
        }
        uintptr_t addr0 = (symbol0 ? ((uintptr_t) set_mario_action - symbol0->offset) : 0);

        // Unwind and print call stack
        void *stack[64];
        s32 frames = CaptureStackBackTrace(0, 64, stack, NULL);
        for (s32 i = 1, j = 0; i < frames && j < 30; ++i) {
            s32 y = 14 + j;
            omm_crash_handler_set_text( 0, y, 0xFF, 0xFF, 0x00, "0x%016llX", (uintptr_t) stack[i]);
            omm_crash_handler_set_text(-1, y, 0xFF, 0xFF, 0xFF, "%s", ": ");
            for (Symbol *symbol = symbols;; symbol = symbol->next) {
                if (!symbol || !symbol->next) {
                    if (j != 0) {
                        omm_crash_handler_set_text(-1, y, 0x00, 0xFF, 0xFF, "%s", "????");
                        ++j;
                    }
                    break;
                } else {
                    uintptr_t offset = (uintptr_t) stack[i] - addr0;
                    if (symbol->next->offset > offset) {
                        omm_crash_handler_set_text(-1, y, 0x00, 0xFF, 0xFF, "%s", symbol->name);
                        omm_crash_handler_set_text(-1, y, 0xFF, 0xFF, 0xFF, " + 0x%llX", offset - symbol->offset);
                        ++j;
                        break;
                    }
                }
            }
        }
    } else {
        omm_crash_handler_set_text(0, 14, 0x80, 0x80, 0x80, "%s", "Unable to unwind the call stack.");
    }

    // Main loop
#if OMM_GAME_IS_R96X
    dynos_music_stop();
    dynos_jingle_stop();
    dynos_sound_stop(1);
#endif
    while (true) {
#if defined(WAPI_SDL2)
        gfx_sdl.main_loop(omm_crash_handler_produce_one_frame);
#elif defined(WAPI_DXGI)
        gfx_dxgi.main_loop(omm_crash_handler_produce_one_frame);
#else
        break;
#endif
    }
    exit(0);
}

OMM_AT_STARTUP static void omm_init_crash_handler() {
    SetUnhandledExceptionFilter(omm_crash_handler);
}

#endif

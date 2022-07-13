#if defined(WAPI_SDL1)
#error "SDL1 is no longer supported. Please use SDL2 instead."
#elif defined(WAPI_SDL2)
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include <unistd.h>

#ifdef __MINGW32__
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#else
#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#ifdef OSX_BUILD
#include <SDL2/SDL_opengl.h>
#else
#include <SDL2/SDL_opengles2.h>
#endif
#endif

#define IS_FULLSCREEN()                 ((SDL_GetWindowFlags(sWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
#define TRANSLATE_SCANCODE(scancode)    ((scancode) < 0x200 ? sInvertedScancodeTable[scancode] : 0)

static SDL_Window   *sWindow    = NULL;
static SDL_GLContext sGLContext = NULL;
static kb_callback_t sOnKeyDown = NULL;
static kb_callback_t sOnKeyUp   = NULL;
static s32 sInvertedScancodeTable[0x200];

static const SDL_Scancode sWindowsScancodeTable[] = {
    /*  0                        1                            2                         3                            4                     5                            6                            7  */
    /*  8                        9                            A                         B                            C                     D                            E                            F  */
    SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_ESCAPE,         SDL_SCANCODE_1,           SDL_SCANCODE_2,              SDL_SCANCODE_3,       SDL_SCANCODE_4,              SDL_SCANCODE_5,              SDL_SCANCODE_6,          /* 0 */
    SDL_SCANCODE_7,              SDL_SCANCODE_8,              SDL_SCANCODE_9,           SDL_SCANCODE_0,              SDL_SCANCODE_MINUS,   SDL_SCANCODE_EQUALS,         SDL_SCANCODE_BACKSPACE,      SDL_SCANCODE_TAB,        /* 0 */

    SDL_SCANCODE_Q,              SDL_SCANCODE_W,              SDL_SCANCODE_E,           SDL_SCANCODE_R,              SDL_SCANCODE_T,       SDL_SCANCODE_Y,              SDL_SCANCODE_U,              SDL_SCANCODE_I,          /* 1 */
    SDL_SCANCODE_O,              SDL_SCANCODE_P,              SDL_SCANCODE_LEFTBRACKET, SDL_SCANCODE_RIGHTBRACKET,   SDL_SCANCODE_RETURN,  SDL_SCANCODE_LCTRL,          SDL_SCANCODE_A,              SDL_SCANCODE_S,          /* 1 */

    SDL_SCANCODE_D,              SDL_SCANCODE_F,              SDL_SCANCODE_G,           SDL_SCANCODE_H,              SDL_SCANCODE_J,       SDL_SCANCODE_K,              SDL_SCANCODE_L,              SDL_SCANCODE_SEMICOLON,  /* 2 */
    SDL_SCANCODE_APOSTROPHE,     SDL_SCANCODE_GRAVE,          SDL_SCANCODE_LSHIFT,      SDL_SCANCODE_BACKSLASH,      SDL_SCANCODE_Z,       SDL_SCANCODE_X,              SDL_SCANCODE_C,              SDL_SCANCODE_V,          /* 2 */

    SDL_SCANCODE_B,              SDL_SCANCODE_N,              SDL_SCANCODE_M,           SDL_SCANCODE_COMMA,          SDL_SCANCODE_PERIOD,  SDL_SCANCODE_SLASH,          SDL_SCANCODE_RSHIFT,         SDL_SCANCODE_PRINTSCREEN,/* 3 */
    SDL_SCANCODE_LALT,           SDL_SCANCODE_SPACE,          SDL_SCANCODE_CAPSLOCK,    SDL_SCANCODE_F1,             SDL_SCANCODE_F2,      SDL_SCANCODE_F3,             SDL_SCANCODE_F4,             SDL_SCANCODE_F5,         /* 3 */

    SDL_SCANCODE_F6,             SDL_SCANCODE_F7,             SDL_SCANCODE_F8,          SDL_SCANCODE_F9,             SDL_SCANCODE_F10,     SDL_SCANCODE_NUMLOCKCLEAR,   SDL_SCANCODE_SCROLLLOCK,     SDL_SCANCODE_HOME,       /* 4 */
    SDL_SCANCODE_UP,             SDL_SCANCODE_PAGEUP,         SDL_SCANCODE_KP_MINUS,    SDL_SCANCODE_LEFT,           SDL_SCANCODE_KP_5,    SDL_SCANCODE_RIGHT,          SDL_SCANCODE_KP_PLUS,        SDL_SCANCODE_END,        /* 4 */

    SDL_SCANCODE_DOWN,           SDL_SCANCODE_PAGEDOWN,       SDL_SCANCODE_INSERT,      SDL_SCANCODE_DELETE,         SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_NONUSBACKSLASH, SDL_SCANCODE_F11,        /* 5 */
    SDL_SCANCODE_F12,            SDL_SCANCODE_PAUSE,          SDL_SCANCODE_UNKNOWN,     SDL_SCANCODE_LGUI,           SDL_SCANCODE_RGUI,    SDL_SCANCODE_APPLICATION,    SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN,    /* 5 */

    SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN,     SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_F13,     SDL_SCANCODE_F14,            SDL_SCANCODE_F15,            SDL_SCANCODE_F16,        /* 6 */
    SDL_SCANCODE_F17,            SDL_SCANCODE_F18,            SDL_SCANCODE_F19,         SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN,    /* 6 */

    SDL_SCANCODE_INTERNATIONAL2, SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN,     SDL_SCANCODE_INTERNATIONAL1, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN,    /* 7 */
    SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_INTERNATIONAL4, SDL_SCANCODE_UNKNOWN,     SDL_SCANCODE_INTERNATIONAL5, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_INTERNATIONAL3, SDL_SCANCODE_UNKNOWN,        SDL_SCANCODE_UNKNOWN     /* 7 */
};

static const SDL_Scancode sScancodeRmappingExtended[][2] = {
    { SDL_SCANCODE_KP_ENTER, SDL_SCANCODE_RETURN },
    { SDL_SCANCODE_RALT, SDL_SCANCODE_LALT },
    { SDL_SCANCODE_RCTRL, SDL_SCANCODE_LCTRL },
    { SDL_SCANCODE_KP_DIVIDE, SDL_SCANCODE_SLASH },
};

static const SDL_Scancode sScancodeRmappingNonExtended[][2] = {
    { SDL_SCANCODE_KP_7, SDL_SCANCODE_HOME },
    { SDL_SCANCODE_KP_8, SDL_SCANCODE_UP },
    { SDL_SCANCODE_KP_9, SDL_SCANCODE_PAGEUP },
    { SDL_SCANCODE_KP_4, SDL_SCANCODE_LEFT },
    { SDL_SCANCODE_KP_6, SDL_SCANCODE_RIGHT },
    { SDL_SCANCODE_KP_1, SDL_SCANCODE_END },
    { SDL_SCANCODE_KP_2, SDL_SCANCODE_DOWN },
    { SDL_SCANCODE_KP_3, SDL_SCANCODE_PAGEDOWN },
    { SDL_SCANCODE_KP_0, SDL_SCANCODE_INSERT },
    { SDL_SCANCODE_KP_PERIOD, SDL_SCANCODE_DELETE },
    { SDL_SCANCODE_KP_MULTIPLY, SDL_SCANCODE_PRINTSCREEN }
};

static void gfx_sdl_set_fullscreen(void) {
    if (configWindow.reset) {
        configWindow.fullscreen = false;
    }
    if (configWindow.fullscreen == IS_FULLSCREEN()) {
        return;
    }
    if (configWindow.fullscreen) {
        SDL_SetWindowFullscreen(sWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(SDL_DISABLE);
    } else {
        SDL_SetWindowFullscreen(sWindow, 0);
        SDL_ShowCursor(SDL_ENABLE);
        configWindow.exiting_fullscreen = true;
    }
}

static void gfx_sdl_reset_dimension_and_pos(void) {
    if (configWindow.exiting_fullscreen) {
        configWindow.exiting_fullscreen = false;
    }
    if (configWindow.reset) {
        configWindow.x = WAPI_WIN_CENTERPOS;
        configWindow.y = WAPI_WIN_CENTERPOS;
        configWindow.w = DESIRED_SCREEN_WIDTH;
        configWindow.h = DESIRED_SCREEN_HEIGHT;
        configWindow.reset = false;
    } else if (!configWindow.settings_changed) {
        return;
    }
    s32 x = (configWindow.x == WAPI_WIN_CENTERPOS) ? SDL_WINDOWPOS_CENTERED : configWindow.x;
    s32 y = (configWindow.y == WAPI_WIN_CENTERPOS) ? SDL_WINDOWPOS_CENTERED : configWindow.y;
    SDL_SetWindowSize(sWindow, configWindow.w, configWindow.h);
    SDL_SetWindowPosition(sWindow, x, y);
    SDL_GL_SetSwapInterval(configWindow.vsync);
}

static void gfx_sdl_init(const char *title) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef USE_GLES
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);  // These attributes allow for hardware acceleration on RPis.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif
    s32 x = (configWindow.x == WAPI_WIN_CENTERPOS) ? SDL_WINDOWPOS_CENTERED : configWindow.x;
    s32 y = (configWindow.y == WAPI_WIN_CENTERPOS) ? SDL_WINDOWPOS_CENTERED : configWindow.y;
    sWindow = SDL_CreateWindow(title, x, y, configWindow.w, configWindow.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    sGLContext = SDL_GL_CreateContext(sWindow);
    SDL_GL_SetSwapInterval(configWindow.vsync);
    gfx_sdl_set_fullscreen();
    for (u64 i = 0; i < sizeof(sWindowsScancodeTable) / sizeof(sWindowsScancodeTable[0]); i++) {
        sInvertedScancodeTable[sWindowsScancodeTable[i]] = i;
    }
    for (u64 i = 0; i < sizeof(sScancodeRmappingExtended) / sizeof(sScancodeRmappingExtended[0]); i++) {
        sInvertedScancodeTable[sScancodeRmappingExtended[i][0]] = sInvertedScancodeTable[sScancodeRmappingExtended[i][1]] + 0x100;
    }
    for (u64 i = 0; i < sizeof(sScancodeRmappingNonExtended) / sizeof(sScancodeRmappingNonExtended[0]); i++) {
        sInvertedScancodeTable[sScancodeRmappingNonExtended[i][0]] = sInvertedScancodeTable[sScancodeRmappingNonExtended[i][1]];
        sInvertedScancodeTable[sScancodeRmappingNonExtended[i][1]] += 0x100;
    }
}

static void gfx_sdl_main_loop(void (*run_one_game_iter)(void)) {
    run_one_game_iter();
}

static void gfx_sdl_get_dimensions(u32 *width, u32 *height) {
    s32 w, h;
    SDL_GetWindowSize(sWindow, &w, &h);
    if (width) *width = w;
    if (height) *height = h;
}

static void gfx_sdl_onkeydown(s32 scancode) {
    if (sOnKeyDown) {
        sOnKeyDown(TRANSLATE_SCANCODE(scancode));
    }
    const u8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN]) {
        configWindow.fullscreen = !configWindow.fullscreen;
        configWindow.settings_changed = true;
    }
}

static void gfx_sdl_onkeyup(s32 scancode) {
    if (sOnKeyUp) {
        sOnKeyUp(TRANSLATE_SCANCODE(scancode));
    }
}

static void gfx_sdl_handle_events(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN: {
                gfx_sdl_onkeydown(event.key.keysym.scancode);
            } break;
            
            case SDL_KEYUP: {
                gfx_sdl_onkeyup(event.key.keysym.scancode);
            } break;
            
            case SDL_WINDOWEVENT: {
                if (!IS_FULLSCREEN()) {
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_MOVED: {
                            if (!configWindow.exiting_fullscreen) {
                                if (event.window.data1 >= 0) configWindow.x = event.window.data1;
                                if (event.window.data2 >= 0) configWindow.y = event.window.data2;
                            }
                        } break;
                        
                        case SDL_WINDOWEVENT_SIZE_CHANGED: {
                            configWindow.w = event.window.data1;
                            configWindow.h = event.window.data2;
                        } break;
                    }
                }
            } break;

            case SDL_QUIT: {
                game_exit();
            } break;
        }
    }
    if (configWindow.settings_changed) {
        gfx_sdl_set_fullscreen();
        gfx_sdl_reset_dimension_and_pos();
        configWindow.settings_changed = false;
    }
}

static void gfx_sdl_set_keyboard_callbacks(kb_callback_t on_key_down, kb_callback_t on_key_up, UNUSED void (*on_all_keys_up)(void)) {
    sOnKeyDown = on_key_down;
    sOnKeyUp = on_key_up;
}

static bool gfx_sdl_start_frame(void) {
    return true;
}

static void gfx_sdl_swap_buffers_begin(void) {
    SDL_GL_SwapWindow(sWindow);
}

static void gfx_sdl_swap_buffers_end(void) {
    static f64 prev = 0;
    f64 curr = SDL_GetPerformanceCounter();
    f64 freq = SDL_GetPerformanceFrequency();
    f64 rate = freq / (30.0 * gNumInterpolatedFrames);
    f64 diff = curr - prev;
    if (diff < rate) {
        usleep((1000000.0 * (rate - diff)) / freq);
        prev += rate;
    } else {
        prev = curr;
    }
}

static double gfx_sdl_get_time(void) {
    return 0.0;
}

static void gfx_sdl_shutdown(void) {
    if (SDL_WasInit(0)) {
        if (sGLContext) {
            SDL_GL_DeleteContext(sGLContext);
            sGLContext = NULL;
        }
        if (sWindow) {
            SDL_DestroyWindow(sWindow);
            sWindow = NULL;
        }
        SDL_Quit();
    }
}

struct GfxWindowManagerAPI gfx_sdl = {
    gfx_sdl_init,
    gfx_sdl_set_keyboard_callbacks,
    gfx_sdl_main_loop,
    gfx_sdl_get_dimensions,
    gfx_sdl_handle_events,
    gfx_sdl_start_frame,
    gfx_sdl_swap_buffers_begin,
    gfx_sdl_swap_buffers_end,
    gfx_sdl_get_time,
    gfx_sdl_shutdown
};

#endif

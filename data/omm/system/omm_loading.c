#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "pc/gfx/gfx_window_manager_api.h"
#include "pc/gfx/gfx_dxgi.h"
#include "pc/gfx/gfx_sdl.h"
extern f32 gfx_texture_do_precache(void);

#define CHAR_WIDTH 4
#define CHAR_HEIGHT 8
#define LINE_SPACING (CHAR_HEIGHT)

enum OmmLoadingState {
    OMM_LOADING_START,
    OMM_LOADING_FILE_SYSTEM,
    OMM_LOADING_MEMORY_POOLS,
    OMM_LOADING_WINDOW,
    OMM_LOADING_RENDER_ENGINE,
    OMM_LOADING_AUDIO_ENGINE,
    OMM_LOADING_TEXTURES,
    OMM_LOADING_DONE,
};

static struct {
    s32 state;
    s32 value;
} sOmmLoadingState[1] = { { 0, 0 } };

typedef struct {
    s32 x, y;
    u8 r, g, b;
    char s[128];
} OmmLoadingScreenText;
static OmmLoadingScreenText sOmmLoadingScreenText[256];

#define omm_loading_screen_set_text(_x_, _y_, _r_, _g_, _b_, _fmt_, ...) \
{                                                                        \
    if (_x_ == -1) {                                                     \
        pText->x = ((pText - 1)->x + strlen((pText - 1)->s));            \
    } else {                                                             \
        pText->x = _x_;                                                  \
    }                                                                    \
    pText->y = _y_;                                                      \
    pText->r = _r_;                                                      \
    pText->g = _g_;                                                      \
    pText->b = _b_;                                                      \
    snprintf(pText->s, 128, _fmt_, __VA_ARGS__);                         \
    pText++;                                                             \
}

static void omm_loading_state_update() {
    switch (sOmmLoadingState->state) {
        case OMM_LOADING_START: {
            sOmmLoadingState->state = OMM_LOADING_FILE_SYSTEM;
            sOmmLoadingState->value = 0;
        } break;

        case OMM_LOADING_FILE_SYSTEM: {
            if (sOmmLoadingState->value++ == 1) {
                sOmmLoadingState->state = OMM_LOADING_MEMORY_POOLS;
                sOmmLoadingState->value = 0;
            }
        } break;

        case OMM_LOADING_MEMORY_POOLS: {
            if (sOmmLoadingState->value++ == 1) {
                sOmmLoadingState->state = OMM_LOADING_WINDOW;
                sOmmLoadingState->value = 0;
            }
        } break;

        case OMM_LOADING_WINDOW: {
            if (sOmmLoadingState->value++ == 1) {
                sOmmLoadingState->state = OMM_LOADING_RENDER_ENGINE;
                sOmmLoadingState->value = 0;
            }
        } break;

        case OMM_LOADING_RENDER_ENGINE: {
            if (sOmmLoadingState->value++ == 1) {
                sOmmLoadingState->state = OMM_LOADING_AUDIO_ENGINE;
                sOmmLoadingState->value = 0;
            }
        } break;

        case OMM_LOADING_AUDIO_ENGINE: {
            if (sOmmLoadingState->value++ == 1) {
                sOmmLoadingState->state = OMM_LOADING_TEXTURES;
                sOmmLoadingState->value = 0;
            }
        } break;

        case OMM_LOADING_TEXTURES: {
            if (!gOmmPreloadTextures || (sOmmLoadingState->value = (s32) (gfx_texture_do_precache() * 100)) == 100) {
                sOmmLoadingState->state = OMM_LOADING_DONE;
                sOmmLoadingState->value = 0;
            }
        } break;

        case OMM_LOADING_DONE: {
        } break;
    }
}

static void omm_loading_state_text() {
    omm_zero(sOmmLoadingScreenText, sizeof(sOmmLoadingScreenText));
    OmmLoadingScreenText *pText = &sOmmLoadingScreenText[0];
    if (sOmmLoadingState->state >= OMM_LOADING_FILE_SYSTEM) {
        omm_loading_screen_set_text(0, 0, 0xFF, 0xFF, 0xFF, "%s", "> Initializing File system... ");
        if (sOmmLoadingState->state > OMM_LOADING_FILE_SYSTEM) omm_loading_screen_set_text(-1, 0, 0x00, 0xFF, 0x00, "%s", "Done");
    }
    if (sOmmLoadingState->state >= OMM_LOADING_MEMORY_POOLS) {
        omm_loading_screen_set_text(0, 1, 0xFF, 0xFF, 0xFF, "%s", "> Initializing Memory pools... ");
        if (sOmmLoadingState->state > OMM_LOADING_MEMORY_POOLS) omm_loading_screen_set_text(-1, 1, 0x00, 0xFF, 0x00, "%s", "Done");
    }
    if (sOmmLoadingState->state >= OMM_LOADING_WINDOW) {
        omm_loading_screen_set_text(0, 2, 0xFF, 0xFF, 0xFF, "%s", "> Initializing Window... ");
        if (sOmmLoadingState->state > OMM_LOADING_WINDOW) omm_loading_screen_set_text(-1, 2, 0x00, 0xFF, 0x00, "%s", "Done");
    }
    if (sOmmLoadingState->state >= OMM_LOADING_RENDER_ENGINE) {
        omm_loading_screen_set_text(0, 3, 0xFF, 0xFF, 0xFF, "%s", "> Initializing Rendering engine... ");
        if (sOmmLoadingState->state > OMM_LOADING_RENDER_ENGINE) omm_loading_screen_set_text(-1, 3, 0x00, 0xFF, 0x00, "%s", "Done");
    }
    if (sOmmLoadingState->state >= OMM_LOADING_AUDIO_ENGINE) {
        omm_loading_screen_set_text(0, 4, 0xFF, 0xFF, 0xFF, "%s", "> Initializing Audio engine... ");
        if (sOmmLoadingState->state > OMM_LOADING_AUDIO_ENGINE) omm_loading_screen_set_text(-1, 4, 0x00, 0xFF, 0x00, "%s", "Done");
    }
    if (sOmmLoadingState->state >= OMM_LOADING_TEXTURES) {
        if (gOmmPreloadTextures) {
            omm_loading_screen_set_text(0, 5, 0xFF, 0xFF, 0xFF, "%s", "> Pre-loading Textures... ");
            if (sOmmLoadingState->state > OMM_LOADING_TEXTURES) omm_loading_screen_set_text(-1, 5, 0x00, 0xFF, 0x00, "%s", "Done")
            else omm_loading_screen_set_text(-1, 5, 0xFF, 0xFF, 0x00, "%d%%", sOmmLoadingState->value);
        } else {
            omm_loading_screen_set_text(0, 5, 0xFF, 0xFF, 0xFF, "%s", "> Skipping Texture pre-loading...");
        }
    }
    if (sOmmLoadingState->state >= OMM_LOADING_DONE) {
        omm_loading_screen_set_text(0, 6, 0xFF, 0xFF, 0xFF, "%s", "> Starting game...");
    }
}

static void omm_loading_screen_produce_one_frame() {

    // Start frame
    gfx_start_frame();
    gFrameInterpolation = false;
    load_gfx_memory_pool();
    init_scene_rendering();

    // Load stuff and update text
    omm_loading_state_update();
    omm_loading_state_text();

    // Clear screen
    create_dl_translation_matrix(MENU_MTX_PUSH, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 240.f, 0.f);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, (GFX_DIMENSIONS_ASPECT_RATIO * SCREEN_HEIGHT) / 130.f, 3.f, 1.f);
    gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, 0xFF);
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // Print text
    for (OmmLoadingScreenText *text = sOmmLoadingScreenText; text->s[0] != 0; ++text) {
        s32 x = text->x;
        s32 y = text->y;
        for (char *c = text->s; *c != 0; ++c, ++x) {
            if (*c > 0x20 && *c < 0x7F) {
                omm_render_texrect(
                    GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE((x + 3) * CHAR_WIDTH),
                    SCREEN_HEIGHT - CHAR_HEIGHT - (8 + LINE_SPACING * y),
                    CHAR_WIDTH, CHAR_HEIGHT, G_IM_FMT_RGBA, G_IM_SIZ_32b,
                    64, 128, text->r, text->g, text->b, 0xFF,
                    OMM_TEXTURE_MENU_FONT_[*c - 0x20], false
                );
            }
        }
    }

    // Render frame
    end_master_display_list();
    alloc_display_list(0);
    display_and_vsync();
    gfx_end_frame();
}

static void omm_loading_screen_update() {
    while (sOmmLoadingState->state != OMM_LOADING_DONE) {
        omm_loading_screen_produce_one_frame();
    }
}

void omm_loading_screen_start() {
    omm_loading_screen_update();
}

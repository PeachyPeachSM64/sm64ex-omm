#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/system/omm_thread.h"
#include "pc/gfx/gfx_window_manager_api.h"
#include "pc/gfx/gfx_dxgi.h"
#include "pc/gfx/gfx_sdl.h"
#include <time.h>

#define OMM_LOADING_SCREEN_TEXTURE_W                    (512)
#define OMM_LOADING_SCREEN_TEXTURE_H                    (256)
#define OMM_LOADING_SCREEN_TEXTURE_TILE_W               (36)
#define OMM_LOADING_SCREEN_TEXTURE_TILE_H               (36)
#define OMM_LOADING_SCREEN_TEXTURE_TILES_PER_ROW        (14)

#define OMM_LOADING_SCREEN_MARIO_W                      (SCREEN_HEIGHT / 4)
#define OMM_LOADING_SCREEN_MARIO_H                      (SCREEN_HEIGHT / 4)
#define OMM_LOADING_SCREEN_MARIO_X                      ((SCREEN_WIDTH / 2) - (OMM_LOADING_SCREEN_MARIO_W / 2))
#define OMM_LOADING_SCREEN_MARIO_Y                      ((SCREEN_HEIGHT / 2) - (OMM_LOADING_SCREEN_MARIO_H / 2))

#define OMM_LOADING_SCREEN_PROGRESS_W                   (SCREEN_HEIGHT / 3)
#define OMM_LOADING_SCREEN_PROGRESS_H                   (1)
#define OMM_LOADING_SCREEN_PROGRESS_Y                   (OMM_LOADING_SCREEN_MARIO_Y - (OMM_LOADING_SCREEN_PROGRESS_H / 2) - (SCREEN_HEIGHT / 16))
#define OMM_LOADING_SCREEN_PROGRESS_COLOR               0xFF, 0xFF, 0xFF
#define OMM_LOADING_SCREEN_PROGRESS_COLOR_OUTLINE       0x80, 0x80, 0x80

typedef struct {
    const char *texture;
    s32 start;
    s32 length;
} OmmLoadingScreenAnimation;

static const OmmLoadingScreenAnimation OMM_LOADING_SCREEN_ANIMATIONS[] = {
    { OMM_TEXTURE_MISC_LOADING_SCREEN_0, 0, 87 },
    { OMM_TEXTURE_MISC_LOADING_SCREEN_1, 0, 40 },
    { OMM_TEXTURE_MISC_LOADING_SCREEN_1, 40, 50 },
};

static Vp OMM_LOADING_SCREEN_VIEWPORT = {{
    { 640, 480, 511, 0 },
    { 640, 480, 511, 0 },
}};

static struct {
    const OmmLoadingScreenAnimation *animation;
    s32 frame;
    u64 loaded_bytes;
    u64 total_bytes;
} sOmmLoadingScreen[1];

static void omm_loading_screen_produce_one_frame() {

    // Start frame
    gfx_start_frame();
    gFrameInterpolation = false;
    config_gfx_pool();
    init_render_image();
    gSPViewport(gDisplayListHead++, &OMM_LOADING_SCREEN_VIEWPORT);

    // Clear screen
    create_dl_translation_matrix(MENU_MTX_PUSH, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 240.f, 0.f);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, (GFX_DIMENSIONS_ASPECT_RATIO * SCREEN_HEIGHT) / 130.f, 3.f, 1.f);
    gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, 0xFF);
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // Draw loading screen
    f32 progress = (f32) (sOmmLoadingScreen->loaded_bytes < sOmmLoadingScreen->total_bytes ? (f64) sOmmLoadingScreen->loaded_bytes / (f64) sOmmLoadingScreen->total_bytes : 1.0);
    s32 frame = sOmmLoadingScreen->animation->start + sOmmLoadingScreen->frame;
    sOmmLoadingScreen->frame = (sOmmLoadingScreen->frame + 1) % sOmmLoadingScreen->animation->length;
    gDisplayListHead = gfx_font_init(gDisplayListHead, sOmmLoadingScreen->animation->texture, OMM_LOADING_SCREEN_TEXTURE_TILES_PER_ROW, OMM_LOADING_SCREEN_TEXTURE_W, OMM_LOADING_SCREEN_TEXTURE_H, OMM_LOADING_SCREEN_TEXTURE_TILE_W, OMM_LOADING_SCREEN_TEXTURE_TILE_H);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, 0x20 + frame, OMM_LOADING_SCREEN_MARIO_X, OMM_LOADING_SCREEN_MARIO_Y, OMM_LOADING_SCREEN_MARIO_W, OMM_LOADING_SCREEN_MARIO_H, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_end(gDisplayListHead);
    f32 w = X_SCREEN_TO_WINDOW(GFX_DIMENSIONS_FROM_LEFT_EDGE(OMM_LOADING_SCREEN_PROGRESS_W));
    f32 h = Y_SCREEN_TO_WINDOW(OMM_LOADING_SCREEN_PROGRESS_H);
    f32 x = X_SCREEN_TO_WINDOW(SCREEN_WIDTH / 2) - w / 2.f;
    f32 y = Y_SCREEN_TO_WINDOW(OMM_LOADING_SCREEN_PROGRESS_Y);
    omm_render_rect_window_coords(x - 2, y - 2, w + 4, h + 4, OMM_LOADING_SCREEN_PROGRESS_COLOR_OUTLINE, 0xFF);
    omm_render_rect_window_coords(x - 1, y - 1, w + 2, h + 2, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(x, y, w * progress, h, OMM_LOADING_SCREEN_PROGRESS_COLOR, 0xFF);

    // Render frame
    end_master_display_list();
    alloc_display_list(0);
    display_and_vsync();
    gfx_end_frame();
}

static void omm_loading_screen_render(pthread_t *thread) {
    while (sOmmLoadingScreen->loaded_bytes < sOmmLoadingScreen->total_bytes) {
        omm_loading_screen_produce_one_frame();
    }
    pthread_join(*thread, NULL);
}

static void *omm_load_textures_and_model_packs(UNUSED void *unused) {

    // Load textures
    gfx_texture_precache_start(&sOmmLoadingScreen->loaded_bytes);

    // Load models packs
    omm_models_load_all(&sOmmLoadingScreen->loaded_bytes);

    // Make sure to exit the loading screen loop
    sOmmLoadingScreen->loaded_bytes = sOmmLoadingScreen->total_bytes;
    return NULL;
}

void omm_loading_screen_start() {

    // Select the animation to display
    srand(time(NULL));
    sOmmLoadingScreen->animation = &OMM_LOADING_SCREEN_ANIMATIONS[rand() % 3];
    sOmmLoadingScreen->frame = 0;

    // Compute total size of textures and model packs
    sOmmLoadingScreen->loaded_bytes = 0;
    sOmmLoadingScreen->total_bytes = (
        gfx_texture_precache_get_size() +
        omm_models_precache_get_size()
    );

    // Run this now to render the loading screen at least once before threading starts
    // Needed to load the loading screen texture and to avoid displaying a black screen if threading fails
    omm_loading_screen_produce_one_frame();

    // Attempt to start thread
    pthread_t thread;
    if (sOmmLoadingScreen->total_bytes > 0 && pthread_create(&thread, NULL, omm_load_textures_and_model_packs, NULL) == 0) {
        omm_loading_screen_render(&thread);
    } else {
        omm_load_textures_and_model_packs(NULL);
    }
    gfx_texture_precache_end();
    rom_asset_copy_baserom_to_appdata();
}

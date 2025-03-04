#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "level_commands.h"
#if OMM_GFX_API_DX
#include <windows.h>
#endif
#define MARIO_BASE_SCALE 2.f

#define OMM_PE_WHITE                    (0xF4)
#define OMM_PE_WHITE_3                  OMM_PE_WHITE, OMM_PE_WHITE, OMM_PE_WHITE
#define OMM_PE_GRAY                     (0xD0)
#define OMM_PE_GRAY_3                   OMM_PE_GRAY, OMM_PE_GRAY, OMM_PE_GRAY
#define OMM_PE_DROPDOWN_LINE(_y_)       (!pe->event->dropDown || (pe->hover->y <= (_y_) && (_y_) < pe->hover->y + pe->hover->h))
#define OMM_PE_DROPDOWN_DARKEN          (0.65f)

#define OMM_PE_CHAR_WIDTH               (4)
#define OMM_PE_CHAR_HEIGHT              (8)

#define OMM_PE_INPUT_SAVE_AND_EXIT      ( gPlayer1Controller->buttonPressed & START_BUTTON                                        )
#define OMM_PE_INPUT_A_PRESSED          ((gPlayer1Controller->buttonPressed & A_BUTTON) || (pe->mouse->pressed & SDL_BUTTON_LMASK))
#define OMM_PE_INPUT_A_DOWN             ((gPlayer1Controller->buttonDown    & A_BUTTON) || (pe->mouse->down    & SDL_BUTTON_LMASK))
#define OMM_PE_INPUT_B_PRESSED          ( gPlayer1Controller->buttonPressed & B_BUTTON                                            )
#define OMM_PE_INPUT_X_DOWN             ( gPlayer1Controller->buttonDown    & X_BUTTON                                            )
#define OMM_PE_INPUT_Z_PRESSED          ((gPlayer1Controller->buttonPressed & Z_TRIG  ) || (pe->mouse->pressed & SDL_BUTTON_RMASK))
#define OMM_PE_INPUT_Z_DOWN             ((gPlayer1Controller->buttonDown    & Z_TRIG  ) || (pe->mouse->down    & SDL_BUTTON_RMASK))
#define OMM_PE_INPUT_UP_PRESSED         ( gPlayer1Controller->buttonPressed & U_JPAD                                              )
#define OMM_PE_INPUT_DOWN_PRESSED       ( gPlayer1Controller->buttonPressed & D_JPAD                                              )
#define OMM_PE_INPUT_LEFT_PRESSED       ( gPlayer1Controller->buttonPressed & L_JPAD                                              )
#define OMM_PE_INPUT_RIGHT_PRESSED      ( gPlayer1Controller->buttonPressed & R_JPAD                                              )
#define OMM_PE_INPUT_ZOOM_IN            ( gPlayer1Controller->buttonDown    & R_TRIG                                              )
#define OMM_PE_INPUT_ZOOM_OUT           ( gPlayer1Controller->buttonDown    & L_TRIG                                              )

#define OMM_PE_DISPLAY_X                (5)
#define OMM_PE_DISPLAY_W                (2 * OMM_PE_DISPLAY_X + OMM_PE_CHAR_WIDTH * (OMM_PE_STR_MAX_LENGTH_LEFT + OMM_PE_STR_MAX_LENGTH_RIGHT))

#define OMM_PE_STR_MAX_LENGTH_LEFT      (13)
#define OMM_PE_STR_MAX_LENGTH_RIGHT     (20)
#define OMM_PE_STR_MAX_LENGTH_PALETTES  (30)

#define OMM_PE_TEXT_X                   ((s32) (GFX_DIMENSIONS_FROM_LEFT_EDGE(OMM_PE_DISPLAY_X)))
#define OMM_PE_TEXT_X2                  (OMM_PE_TEXT_X + OMM_PE_CHAR_WIDTH * OMM_PE_STR_MAX_LENGTH_LEFT)
#define OMM_PE_TEXT_X3                  (OMM_PE_TEXT_X2 + OMM_PE_CHAR_WIDTH * 10.5f)
#define OMM_PE_TEXT_Y                   (SCREEN_HEIGHT - 12)
#define OMM_PE_TEXT_H                   (OMM_PE_CHAR_HEIGHT)

#define OMM_PE_TEXT_PLAYER_Y            (OMM_PE_TEXT_Y)
#define OMM_PE_TEXT_PALETTE_Y           (OMM_PE_TEXT_PLAYER_Y - OMM_PE_TEXT_H)
#define OMM_PE_TEXT_GROUP_Y             (OMM_PE_TEXT_PALETTE_Y - OMM_PE_TEXT_H)
#define OMM_PE_TEXT_COLOR_Y             (k ? OMM_PE_TEXT_COLOR_2_Y : OMM_PE_TEXT_COLOR_1_Y)
#define OMM_PE_TEXT_COLOR_1_Y           (OMM_PE_TEXT_GROUP_Y - OMM_PE_TEXT_H)
#define OMM_PE_TEXT_COLOR_2_Y           (pe->colormaps[0]->y - 2 - OMM_PE_TEXT_H)
#define OMM_PE_TEXT_BACKGROUND_Y        (4 + 2 * OMM_PE_TEXT_H)
#define OMM_PE_TEXT_CAP_STATE_Y         (4 + OMM_PE_TEXT_H)
#define OMM_PE_TEXT_ANIMATIONS_Y        (4)

#define OMM_PE_HOVER_BOX_MARGIN         (2)
#define OMM_PE_HOVER_BOX_X_0            (OMM_PE_TEXT_X - OMM_PE_HOVER_BOX_MARGIN)
#define OMM_PE_HOVER_BOX_X_1            (OMM_PE_TEXT_X2 - OMM_PE_HOVER_BOX_MARGIN)
#define OMM_PE_HOVER_BOX_X_2            (OMM_PE_TEXT_X3 - OMM_PE_HOVER_BOX_MARGIN)
#define OMM_PE_HOVER_BOX_Y(_y_)         ((_y_) + OMM_PE_CHAR_HEIGHT / 4 - OMM_PE_HOVER_BOX_MARGIN)
#define OMM_PE_HOVER_BOX_W_0            (2 * OMM_PE_HOVER_BOX_MARGIN + OMM_PE_CHAR_WIDTH * (OMM_PE_STR_MAX_LENGTH_LEFT - 1))
#define OMM_PE_HOVER_BOX_W_1            ((2 * OMM_PE_HOVER_BOX_MARGIN + OMM_PE_CHAR_WIDTH * OMM_PE_STR_MAX_LENGTH_RIGHT) / 2 - 2)
#define OMM_PE_HOVER_BOX_W_2            ((2 * OMM_PE_HOVER_BOX_MARGIN + OMM_PE_CHAR_WIDTH * OMM_PE_STR_MAX_LENGTH_RIGHT) / 2 - 2)
#define OMM_PE_HOVER_BOX_W_1_2          (OMM_PE_HOVER_BOX_W_1 + OMM_PE_HOVER_BOX_W_2 + 4)
#define OMM_PE_HOVER_BOX_H              (OMM_PE_CHAR_HEIGHT / 2 + 2 * OMM_PE_HOVER_BOX_MARGIN)

#define OMM_PE_ITEM_X_0                 (OMM_PE_HOVER_BOX_X_0 + OMM_PE_HOVER_BOX_W_0 - 8)
#define OMM_PE_ITEM_X_1                 (OMM_PE_HOVER_BOX_X_1 + OMM_PE_HOVER_BOX_W_1 - 8)
#define OMM_PE_ITEM_X_2                 (OMM_PE_HOVER_BOX_X_2 + OMM_PE_HOVER_BOX_W_2 - 8)

#define OMM_PE_PALETTES_TEXT_W          (OMM_PE_CHAR_WIDTH * OMM_PE_STR_MAX_LENGTH_PALETTES)
#define OMM_PE_PALETTES_TEXT_H          (OMM_PE_CHAR_HEIGHT + 2)
#define OMM_PE_PALETTES_TEXT_X(_i_)     (OMM_PE_PALETTES_HOVER_X(_i_) + OMM_PE_PALETTES_HOVER_W / 2)
#define OMM_PE_PALETTES_TEXT_Y(_j_)     (OMM_PE_PALETTES_HOVER_Y(_j_) + OMM_PE_PALETTES_HOVER_MARGIN - OMM_PE_CHAR_HEIGHT / 4)

#define OMM_PE_PALETTES_HOVER_MARGIN    (2)
#define OMM_PE_PALETTES_HOVER_X(_i_)    (OMM_PE_PALETTES_X + OMM_PE_PALETTES_MARGIN_X + (_i_) * (OMM_PE_PALETTES_HOVER_W + OMM_PE_CHAR_WIDTH))
#define OMM_PE_PALETTES_HOVER_Y(_j_)    (OMM_PE_PALETTES_Y + OMM_PE_PALETTES_MARGIN_Y + ((pe->dropdown->count / 2) - 1 - (_j_)) * OMM_PE_PALETTES_TEXT_H)
#define OMM_PE_PALETTES_HOVER_W         (2 * OMM_PE_PALETTES_HOVER_MARGIN + OMM_PE_PALETTES_TEXT_W)
#define OMM_PE_PALETTES_HOVER_H         (OMM_PE_CHAR_HEIGHT / 2 + 2 * OMM_PE_PALETTES_HOVER_MARGIN)

#define OMM_PE_PALETTES_MARGIN_X        (4)
#define OMM_PE_PALETTES_MARGIN_Y        (4)
#define OMM_PE_PALETTES_X               ((SCREEN_WIDTH + OMM_PE_DISPLAY_W - OMM_PE_PALETTES_W) / 2)
#define OMM_PE_PALETTES_Y               ((SCREEN_HEIGHT - OMM_PE_PALETTES_H) / 2)
#define OMM_PE_PALETTES_W               (2 * OMM_PE_PALETTES_MARGIN_X + 2 * OMM_PE_PALETTES_HOVER_W + OMM_PE_CHAR_WIDTH)
#define OMM_PE_PALETTES_H               (2 * OMM_PE_PALETTES_MARGIN_Y + (pe->dropdown->count / 2) * OMM_PE_PALETTES_TEXT_H + OMM_PE_CHAR_HEIGHT)

#define OMM_PE_INPUT_BOX_MARGIN         (4)
#define OMM_PE_INPUT_BOX_CHAR_MARGIN    (1)
#define OMM_PE_INPUT_BOX_CHAR_OFFSET    (OMM_PE_CHAR_HEIGHT / 8)
#define OMM_PE_INPUT_BOX_CHAR_WIDTH     (OMM_PE_CHAR_WIDTH + 2 * OMM_PE_INPUT_BOX_CHAR_MARGIN)
#define OMM_PE_INPUT_BOX_CHAR_HEIGHT    (OMM_PE_CHAR_HEIGHT - 2 * OMM_PE_INPUT_BOX_CHAR_OFFSET + 2 * OMM_PE_INPUT_BOX_CHAR_MARGIN)

#define OMM_PE_DROPDOWN_HOVER_MARGIN    (2)
#define OMM_PE_DROPDOWN_HOVER_X         (OMM_PE_TEXT_X2 - OMM_PE_DROPDOWN_HOVER_MARGIN)
#define OMM_PE_DROPDOWN_HOVER_Y(_y_)    ((_y_) + OMM_PE_CHAR_HEIGHT / 4 - OMM_PE_DROPDOWN_HOVER_MARGIN)
#define OMM_PE_DROPDOWN_HOVER_W         (2 * OMM_PE_DROPDOWN_HOVER_MARGIN + OMM_PE_CHAR_WIDTH * OMM_PE_STR_MAX_LENGTH_RIGHT)
#define OMM_PE_DROPDOWN_HOVER_H         (OMM_PE_CHAR_HEIGHT / 2 + 2 * OMM_PE_DROPDOWN_HOVER_MARGIN)

#define OMM_PE_DROPDOWN_TEXT_Y(_i_)     (pe->dropdown->y + OMM_PE_DROPDOWN_MARGIN - OMM_PE_CHAR_HEIGHT / 4 + (pe->dropdown->count - 1 - (_i_)) * OMM_PE_TEXT_H)

#define OMM_PE_DROPDOWN_MARGIN          (OMM_PE_DROPDOWN_HOVER_MARGIN + 1)
#define OMM_PE_DROPDOWN_Y(_y_)          (OMM_PE_DROPDOWN_HOVER_Y(_y_))
#define OMM_PE_DROPDOWN_H               (OMM_PE_DROPDOWN_HOVER_H + 2 * (OMM_PE_DROPDOWN_MARGIN - OMM_PE_DROPDOWN_HOVER_MARGIN))

#define OMM_PE_COLOR_BOX_X              (OMM_PE_TEXT_X2 + 7 * OMM_PE_CHAR_WIDTH)
#define OMM_PE_COLOR_BOX_Y              ((OMM_PE_CHAR_HEIGHT - OMM_PE_COLOR_BOX_H) / 2)
#define OMM_PE_COLOR_BOX_W              (OMM_PE_CHAR_HEIGHT)
#define OMM_PE_COLOR_BOX_H              (OMM_PE_CHAR_HEIGHT)

#define OMM_PE_COLOR_MAP_DEFAULT_W      (OMM_PE_DISPLAY_W - 2 * OMM_PE_DISPLAY_X)
#define OMM_PE_COLOR_MAP_DEFAULT_H      (80)
#define OMM_PE_COLOR_MAP_DEFAULT_X      (OMM_PE_TEXT_X)
#define OMM_PE_COLOR_MAP_1_DEFAULT_Y    (OMM_PE_TEXT_COLOR_1_Y - 2 - OMM_PE_COLOR_MAP_DEFAULT_H)
#define OMM_PE_COLOR_MAP_2_DEFAULT_Y    (OMM_PE_TEXT_COLOR_2_Y - 2 - OMM_PE_COLOR_MAP_DEFAULT_H)

#define OMM_PE_COLOR_PICKER_X(_x_)      (pe->colormaps[k]->x + 1.5f + (pe->colormaps[k]->w - 2) * (_x_))
#define OMM_PE_COLOR_PICKER_Y(_y_)      (pe->colormaps[k]->y + 1 + (pe->colormaps[k]->h - 2) * (_y_))
#define OMM_PE_COLOR_PICKER_THICKNESS   (1)
#define OMM_PE_COLOR_PICKER_WIDTH       (2)
#define OMM_PE_COLOR_PICKER_LENGTH      (5)

#define OMM_PE_SAVE_BUTTON_X            (GFX_DIMENSIONS_FROM_RIGHT_EDGE(4) - OMM_PE_SAVE_BUTTON_W)
#define OMM_PE_SAVE_BUTTON_Y            (4)
#define OMM_PE_SAVE_BUTTON_W            (textSaveLen * OMM_PE_CHAR_WIDTH + 8)
#define OMM_PE_SAVE_BUTTON_H            (OMM_PE_CHAR_HEIGHT + 8)

#define OMM_PE_CURSOR_W                 (12)
#define OMM_PE_CURSOR_H                 (12)

static const char *OMM_PALETTE_EDITOR_PALETTE_CHOICES[4];

static const char *OMM_PALETTE_EDITOR_GROUP_CHOICES[9];

static const char *OMM_PALETTE_EDITOR_COLOR_CHOICES[] = {
    OMM_TEXT_PE_COLOR_EDIT,
    OMM_TEXT_PE_COLOR_COPY,
    OMM_TEXT_PE_COLOR_PASTE,
    OMM_TEXT_PE_COLOR_LIGHTEN,
    OMM_TEXT_PE_COLOR_DARKEN,
};

static const char *OMM_PALETTE_EDITOR_BACKGROUND_CHOICES[] = {
    [BACKGROUND_OCEAN_SKY] = OMM_TEXT_PE_BACKGROUND_OCEAN_SKY,
    [BACKGROUND_FLAMING_SKY] = OMM_TEXT_PE_BACKGROUND_FLAMING_SKY,
    [BACKGROUND_UNDERWATER_CITY] = OMM_TEXT_PE_BACKGROUND_UNDERWATER_CITY,
    [BACKGROUND_BELOW_CLOUDS] = OMM_TEXT_PE_BACKGROUND_BELOW_CLOUDS,
    [BACKGROUND_SNOW_MOUNTAINS] = OMM_TEXT_PE_BACKGROUND_SNOW_MOUNTAINS,
    [BACKGROUND_DESERT] = OMM_TEXT_PE_BACKGROUND_DESERT,
    [BACKGROUND_HAUNTED] = OMM_TEXT_PE_BACKGROUND_HAUNTED,
    [BACKGROUND_GREEN_SKY] = OMM_TEXT_PE_BACKGROUND_GREEN_SKY,
    [BACKGROUND_ABOVE_CLOUDS] = OMM_TEXT_PE_BACKGROUND_ABOVE_CLOUDS,
    [BACKGROUND_PURPLE_SKY] = OMM_TEXT_PE_BACKGROUND_PURPLE_SKY,
};

static const char *OMM_PALETTE_EDITOR_CAP_CHOICES[] = {
    OMM_TEXT_PE_CAP_OFF,
    OMM_TEXT_PE_CAP_ON,
    OMM_TEXT_PE_CAP_WING,
    OMM_TEXT_PE_CAP_METAL,
    OMM_TEXT_PE_CAP_WINGED_METAL,
};

static const s16 OMM_PALETTE_EDITOR_CAP_BODY_STATES[][2] = {
    { MARIO_HAS_DEFAULT_CAP_OFF, 0 },
    { MARIO_HAS_DEFAULT_CAP_ON, 0 },
    { MARIO_HAS_WING_CAP_ON, 0 },
    { MARIO_HAS_DEFAULT_CAP_ON, MODEL_STATE_METAL },
    { MARIO_HAS_WING_CAP_ON, MODEL_STATE_METAL },
};

static const u8 OMM_PALETTE_EDITOR_INPUT_COLORS[6][3] = {
    { 0xFF, 0xC0, 0xC0 },
    { 0xFF, 0xC0, 0xC0 },
    { 0xC0, 0xFF, 0xC0 },
    { 0xC0, 0xFF, 0xC0 },
    { 0xC0, 0xC0, 0xFF },
    { 0xC0, 0xC0, 0xFF },
};

//
// Mario colors functions for the palette editor
//

static bool omm_palette_editor_use_emblem() {
    s32 index = omm_models_get_actor_index(mario_geo);
    if (index != -1) {
        OmmGfxData *gfxData = ((const OmmActorGfx *) omm_array_get(gOmmActorList, ptr, index))->gfx_data;
        if (gfxData) {
            return gfxData->use_emblem;
        }
    }
    return false;
}

static s32 omm_palette_editor_lights_count(bool peach, bool emblem) {
    return omm_mario_colors_lights_count(peach) - (2 * !peach * !emblem);
}

static s32 omm_palette_editor_light_groups_count(bool peach, bool emblem) {
    return omm_palette_editor_lights_count(peach, emblem) / 2;
}

//
// Palette editor
//

#define OMM_PE_STATE_CLOSED     (0)
#define OMM_PE_STATE_OPENING    (1)
#define OMM_PE_STATE_OPEN       (2)
#define OMM_PE_STATE_CLOSING    (3)

typedef struct { bool colorPreview, pickingColor, selectedBox, dropDown, inputColor; s32 kmin, kmax, color; } OmmPaletteEditorEvent;
typedef const OmmPaletteEditorEvent ev;
static ev OMM_PE_EVENT_NONE[1]                = { { true,  false, false, false, false, 0, 1, 0 } };
static ev OMM_PE_EVENT_COPY_PALETTE[1]        = { { false, false, true,  false, false, 0, 1, 0 } };
static ev OMM_PE_EVENT_MOVING_CAMERA[1]       = { { false, false, false, false, false, 0, 1, 0 } };
static ev OMM_PE_EVENT_PICKING_COLOR_1[1]     = { { true,  true,  false, false, false, 0, 0, 0 } };
static ev OMM_PE_EVENT_PICKING_COLOR_2[1]     = { { true,  true,  false, false, false, 1, 1, 1 } };
static ev OMM_PE_EVENT_INPUT_COLOR_1[1]       = { { false, false, true,  false, true,  0, 1, 0 } };
static ev OMM_PE_EVENT_INPUT_COLOR_2[1]       = { { false, false, true,  false, true,  0, 1, 1 } };
static ev OMM_PE_EVENT_DROPDOWN_PALETTE[1]    = { { false, false, true,  true,  false, 0, 1, 0 } };
static ev OMM_PE_EVENT_DROPDOWN_GROUP[1]      = { { false, false, true,  true,  false, 0, 1, 0 } };
static ev OMM_PE_EVENT_DROPDOWN_COLOR_1[1]    = { { false, false, true,  true,  false, 0, 1, 0 } };
static ev OMM_PE_EVENT_DROPDOWN_COLOR_2[1]    = { { false, false, true,  true,  false, 0, 1, 1 } };
static ev OMM_PE_EVENT_DROPDOWN_BACKGROUND[1] = { { false, false, true,  true,  false, 0, 1, 0 } };
static ev OMM_PE_EVENT_DROPDOWN_CAP_STATE[1]  = { { false, false, true,  true,  false, 0, 1, 0 } };

static struct {
    s32 state;      // editor state
    ev *event;      // editor event
    s32 peach;      // selected Peach
    s32 emblem;     // show "emblem"
    s32 cap;        // cap state
    s32 anims;      // turn on or off animations
    s32 palette;    // custom palette index
    s32 group;      // selected light group
    s32 subgroup;   // selected light group in "All" groups mode
    s32 pitch;      // model pitch
    s32 yaw;        // model yaw
    s32 zoom;       // model zoom
    s32 skybox;     // background skybox
    s32 bright;     // colormap brightness mode
    struct {
    s32 x, y;       // screen coords
    s32 w, h;       // width, height
    } colormaps[2][1]; // colormaps
    struct {
    s32 count;      // number of items
    s32 items[16][3][2]; // screen coords of items
    } items[1];     // items
    struct {
    f32 x, y;       // floating screen coords
    f32 cx, cy;     // anchor pos (floating screen coords) for moving camera
    s32 down;       // hand state
    s32 hindex;     // last item index (horizontal)
    s32 vindex;     // last item index (vertical)
    } cursor[1];    // cursor (Mario hand)
    struct {
    s32 wx, wy;     // window coords
    u32 pressed;    // pressed buttons
    u32 down;       // held buttons
    } mouse[1];     // mouse
    struct {
    u32 *p;         // pointer to light color
    f32 r, g, b;    // color (RGB floats)
    f32 u, v, w;    // color (UVW floats)
    } lights[2][1]; // current light group lights
    struct {
    f32 r, g, b;    // color (RGB floats)
    } copy[1];      // color copy
    struct {
    s32 x, y;       // screen coords
    s32 w, h;       // width, height
    } hover[1];     // hover box
    struct {
    s32 value;      // input value
    s32 index;      // current index
    } input[1];     // input
    struct {
    s32 x, y;       // screen coords
    s32 w, h;       // width, height
    s32 count;      // number of choices
    s32 selected;   // selected choice index
    const char **choices; // selectable choices
    } dropdown[1];  // dropdown
} pe[1]; // sOmmPaletteEditor

static void omm_palette_editor_rgb_to_uvw(f32 r, f32 g, f32 b, f32 *u, f32 *v, f32 *w) {

    // Brightness (w)
    *w = max_3_f(r, g, b);
    if (*w > 0.f) {
        r /= *w;
        g /= *w;
        b /= *w;

        // Saturation (v)
        *v = min_3_f(r, g, b);
        if (*v < 1.f) {

            // Raw color
            r = invlerp_f(r, *v, 1);
            g = invlerp_f(g, *v, 1);
            b = invlerp_f(b, *v, 1);

            // Hue (u)
            f32 m = max_3_f(r, g, b);
            if      (m == r) *u = fmodf(g - b + 6, 6) / 6.f;
            else if (m == g) *u = fmodf(b - r + 2, 6) / 6.f;
            else if (m == b) *u = fmodf(r - g + 4, 6) / 6.f;
        } else {
            *u = 0;
        }
    } else {
        *u = 0;
        *v = 0;
    }
}

static void omm_palette_editor_uvw_to_rgb(f32 u, f32 v, f32 w, f32 *r, f32 *g, f32 *b) {

    // Raw color (u)
    f32 f = fmodf(u * 6.f, 6);
    f32 i = f - floorf(f);
    f32 j = 1.f - i;
    if      (f < 1.f) { *r = 1; *g = i; *b = 0; }
    else if (f < 2.f) { *r = j; *g = 1; *b = 0; }
    else if (f < 3.f) { *r = 0; *g = 1; *b = i; }
    else if (f < 4.f) { *r = 0; *g = j; *b = 1; }
    else if (f < 5.f) { *r = i; *g = 0; *b = 1; }
    else if (f < 6.f) { *r = 1; *g = 0; *b = j; }

    // Saturation (v)
    *r = lerp_f(v, *r, 1);
    *g = lerp_f(v, *g, 1);
    *b = lerp_f(v, *b, 1);

    // Brightness (w)
    *r *= w;
    *g *= w;
    *b *= w;
}

static void omm_palette_editor_set_current() {
    pe->group = clamp_s(pe->group, 0, omm_palette_editor_light_groups_count(pe->peach, pe->emblem));
    s32 group = (pe->group ? pe->group - 1 : pe->subgroup);
    s32 palette = omm_mario_colors_count() - 4 + pe->palette;
    pe->lights[0]->p = omm_mario_colors_get_light(pe->peach, palette, group * 2 + 0);
    pe->lights[1]->p = omm_mario_colors_get_light(pe->peach, palette, group * 2 + 1);
    for (s32 k = 0; k != 2; ++k) {
        pe->lights[k]->r = ((*pe->lights[k]->p >> 16) & 0xFF) / 255.f;
        pe->lights[k]->g = ((*pe->lights[k]->p >>  8) & 0xFF) / 255.f;
        pe->lights[k]->b = ((*pe->lights[k]->p >>  0) & 0xFF) / 255.f;
        omm_palette_editor_rgb_to_uvw(
            pe->lights[k]->r,
            pe->lights[k]->g,
            pe->lights[k]->b,
            &pe->lights[k]->u,
            &pe->lights[k]->v,
            &pe->lights[k]->w
        );
    }
}

static void omm_palette_editor_update_current() {
    for (s32 k = 0; k != 2; ++k) {
        omm_palette_editor_uvw_to_rgb(
            pe->lights[k]->u,
            pe->lights[k]->v,
            pe->lights[k]->w,
            &pe->lights[k]->r,
            &pe->lights[k]->g,
            &pe->lights[k]->b
        );
        *pe->lights[k]->p = (
            (((u32) (clamp_0_1_f(pe->lights[k]->r) * 255.9f)) << 16) |
            (((u32) (clamp_0_1_f(pe->lights[k]->g) * 255.9f)) <<  8) |
            (((u32) (clamp_0_1_f(pe->lights[k]->b) * 255.9f)) <<  0)
        );
    }
}

static s32 omm_palette_editor_get_pointed_color(f32 *u, f32 *v, f32 *w) {
    for (s32 k = pe->event->kmin; k <= pe->event->kmax; ++k) {
        f32 pu = invlerp_f(pe->cursor->x, pe->colormaps[k]->x + 1, pe->colormaps[k]->x + pe->colormaps[k]->w - 1);
        f32 pv = invlerp_f(pe->cursor->y, pe->colormaps[k]->y + 1, pe->colormaps[k]->y + pe->colormaps[k]->h - 1);
        if (pe->event->pickingColor || (pu >= 0.f && pu <= 1.f && pv >= 0.f && pv <= 1.f)) {
            if (pe->bright) {
                *u = pe->lights[k]->u;
                *v = clamp_0_1_f(1 - pv);
                *w = clamp_0_1_f(pu);
            } else {
                *u = clamp_0_1_f(pu);
                *v = clamp_0_1_f(1 - pv);
                *w = pe->lights[k]->w;
            }
            return k;
        }
    }
    return -1;
}

static s32 omm_palette_editor_get_number_from_keyboard_key() {
    static const u32 sKeyToNumber[][2] = {
                                  //   Q       A       N
        { 11, 0x0 }, { 82, 0x0 }, //  [)]  |  [à]  |  [0]
        {  2, 0x1 }, { 79, 0x1 }, //  [!]  |  [&]  |  [1]
        {  3, 0x2 }, { 80, 0x2 }, //  [@]  |  [é]  |  [2]
        {  4, 0x3 }, { 81, 0x3 }, //  [#]  |  ["]  |  [3]
        {  5, 0x4 }, { 75, 0x4 }, //  [$]  |  [']  |  [4]
        {  6, 0x5 }, { 76, 0x5 }, //  [%]  |  [(]  |  [5]
        {  7, 0x6 }, { 77, 0x6 }, //  [^]  |  [-]  |  [6]
        {  8, 0x7 }, { 71, 0x7 }, //  [&]  |  [è]  |  [7]
        {  9, 0x8 }, { 72, 0x8 }, //  [*]  |  [_]  |  [8]
        { 10, 0x9 }, { 73, 0x9 }, //  [(]  |  [ç]  |  [9]
        { 16, 0xA }, { 30, 0xA }, // [Q/A] | [A/Q]
        { 48, 0xB },              //  [B]  |  [B]
        { 46, 0xC },              //  [C]  |  [C]
        { 32, 0xD },              //  [D]  |  [D]
        { 18, 0xE },              //  [E]  |  [E]
        { 33, 0xF },              //  [F]  |  [F]
    };
    u32 keyPressed = controller_get_raw_key();
    for (s32 i = 0; i != array_length(sKeyToNumber); ++i) {
        if (sKeyToNumber[i][0] == keyPressed) {
            return sKeyToNumber[i][1];
        }
    }
    return -1;
}

OMM_INLINE bool omm_palette_editor_is_inside_box(f32 x, f32 y, f32 boxx, f32 boxy, f32 boxw, f32 boxh) {
    return x >= boxx && x < boxx + boxw && y >= boxy && y < boxy + boxh;
}

OMM_INLINE void omm_palette_editor_set_item(s32 vindex, s32 hindex, s32 x, s32 y) {
    pe->items->items[vindex][hindex][0] = x;
    pe->items->items[vindex][hindex][1] = y;
}

//
// Render
//

static void omm_palette_editor_render_string(const u8 *str, s32 maxlen, s32 x, s32 y, s32 w, s32 h, u8 r, u8 g, u8 b, u8 a, bool shadow, bool darkenIfDropDown) {
    if (darkenIfDropDown && !OMM_PE_DROPDOWN_LINE(y)) {
        r *= OMM_PE_DROPDOWN_DARKEN;
        g *= OMM_PE_DROPDOWN_DARKEN;
        b *= OMM_PE_DROPDOWN_DARKEN;
    }
    gDisplayListHead = gfx_font_init(gDisplayListHead, OMM_TEXTURE_FONT_PALETTE_EDITOR, 16, 512, 384, 32, 64);
    for (const u8 *c = str; *c != 0xFF && maxlen; ++c, --maxlen) {
        if (shadow) {
            gDisplayListHead = gfx_font_render_char(gDisplayListHead, SM64_TO_ASCII_CHAR[*c], x + 1, y - 1, w, h, 0, 0, 0x00, 0x00, 0x00, a);
        }
        gDisplayListHead = gfx_font_render_char(gDisplayListHead, SM64_TO_ASCII_CHAR[*c], x, y, w, h, 0, 0, r, g, b, a);
        x += w;
    }
    gDisplayListHead = gfx_font_end(gDisplayListHead);
}

static void omm_palette_editor_render_dropdown_background(s32 x, s32 y, s32 w, s32 h) {
    f32 wx = X_SCREEN_TO_WINDOW(x);
    f32 wy = Y_SCREEN_TO_WINDOW(y);
    f32 ww = X_SCREEN_TO_WINDOW(GFX_DIMENSIONS_FROM_LEFT_EDGE(w));
    f32 wh = Y_SCREEN_TO_WINDOW(h);
    omm_render_texrect(x, y, w, h, 32, 32, 0x20, 0x20, 0x20, 0xF0, OMM_TEXTURE_MISC_WHITE, false);
    omm_render_rect_window_coords(wx, wy, 1, wh, 0xA0, 0xA0, 0xA0, 0xC0);
    omm_render_rect_window_coords(wx + ww - 1, wy, 1, wh, 0xA0, 0xA0, 0xA0, 0xC0);
    omm_render_rect_window_coords(wx, wy, ww, 1, 0xA0, 0xA0, 0xA0, 0xC0);
    omm_render_rect_window_coords(wx, wy + wh - 1, ww, 1, 0xA0, 0xA0, 0xA0, 0xC0);
}

static void omm_palette_editor_render_color_picker(f32 wx, f32 wy) {
    f32 dn0 = -(OMM_PE_COLOR_PICKER_WIDTH + OMM_PE_COLOR_PICKER_THICKNESS + 1);
    f32 dp0 = +(OMM_PE_COLOR_PICKER_WIDTH);
    f32 dn1 = dn0 - (OMM_PE_COLOR_PICKER_LENGTH + 3);
    f32 dp1 = dp0 + (OMM_PE_COLOR_PICKER_THICKNESS + 3);
    f32 dn2 = -(OMM_PE_COLOR_PICKER_THICKNESS + 1) / 2;
    f32 w0 = 2 * (OMM_PE_COLOR_PICKER_WIDTH + OMM_PE_COLOR_PICKER_THICKNESS + 2) - 1;
    f32 w1 = 2 + OMM_PE_COLOR_PICKER_THICKNESS;
    f32 w2 = 2 + OMM_PE_COLOR_PICKER_LENGTH;
    u8 white = (OMM_PE_DROPDOWN_LINE(-1) ? 1.f : OMM_PE_DROPDOWN_DARKEN) * OMM_PE_WHITE;
    omm_render_rect_window_coords(wx + dn0, wy + dn0, w0, w1, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + dn0, wy + dp0, w0, w1, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + dn0, wy + dn0, w1, w0, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + dp0, wy + dn0, w1, w0, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + dn1, wy + dn2, w2, w1, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + dp1, wy + dn2, w2, w1, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + dn2, wy + dn1, w1, w2, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + dn2, wy + dp1, w1, w2, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + dn0 + 1, wy + dn0 + 1, w0 - 2, w1 - 2, white, white, white, 0xFF);
    omm_render_rect_window_coords(wx + dn0 + 1, wy + dp0 + 1, w0 - 2, w1 - 2, white, white, white, 0xFF);
    omm_render_rect_window_coords(wx + dn0 + 1, wy + dn0 + 1, w1 - 2, w0 - 2, white, white, white, 0xFF);
    omm_render_rect_window_coords(wx + dp0 + 1, wy + dn0 + 1, w1 - 2, w0 - 2, white, white, white, 0xFF);
    omm_render_rect_window_coords(wx + dn1 + 1, wy + dn2 + 1, w2 - 2, w1 - 2, white, white, white, 0xFF);
    omm_render_rect_window_coords(wx + dp1 + 1, wy + dn2 + 1, w2 - 2, w1 - 2, white, white, white, 0xFF);
    omm_render_rect_window_coords(wx + dn2 + 1, wy + dn1 + 1, w1 - 2, w2 - 2, white, white, white, 0xFF);
    omm_render_rect_window_coords(wx + dn2 + 1, wy + dp1 + 1, w1 - 2, w2 - 2, white, white, white, 0xFF);
}

static void omm_palette_editor_render_color_preview(f32 u, f32 v, f32 w) {
    f32 cw = OMM_PE_CHAR_WIDTH - 1;
    f32 ch = OMM_PE_CHAR_HEIGHT - 2;
    f32 x = pe->cursor->x + 0.95f * OMM_PE_CURSOR_W - 1;
    f32 y = pe->cursor->y - 0.70f * OMM_PE_CURSOR_H + 1;
    f32 wx = X_SCREEN_TO_WINDOW(x) - 4;
    f32 wy = Y_SCREEN_TO_WINDOW(y);
    f32 ww = X_SCREEN_TO_WINDOW(GFX_DIMENSIONS_FROM_LEFT_EDGE(2 + 6 * cw + ch));
    f32 wh = Y_SCREEN_TO_WINDOW(ch);
    f32 r, g, b; omm_palette_editor_uvw_to_rgb(u, v, w, &r, &g, &b);
    str_t rgb;
    str_fmt(rgb, sizeof(rgb), "%02X%02X%02X", (u8) (r * 255.9f), (u8) (g * 255.9f), (u8) (b * 255.9f));
    omm_render_rect_window_coords(wx, wy - wh, ww, wh, 0xFF, 0xFF, 0xFF, 0xFF);
    omm_render_rect_window_coords(wx + 1, wy - wh + 1, ww - wh - 2, wh - 2, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + ww - wh, wy - wh + 1, wh - 1, wh - 2, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + ww - wh + 1, wy - wh + 2, wh - 3, wh - 4, (u8) (r * 255.9f), (u8) (g * 255.9f), (u8) (b * 255.9f), 0xFF);
    gDisplayListHead = gfx_font_init(gDisplayListHead, OMM_TEXTURE_FONT_PALETTE_EDITOR, 16, 512, 384, 32, 64);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[0], x + 0 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[1], x + 1 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[2], x + 2 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[3], x + 3 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[4], x + 4 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[5], x + 5 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_end(gDisplayListHead);
}

static void omm_palette_editor_render_paste_color_preview(s32 x, s32 y, f32 r, f32 g, f32 b) {
    f32 wx = X_SCREEN_TO_WINDOW(x);
    f32 wy = Y_SCREEN_TO_WINDOW(y);
    f32 cw = OMM_PE_CHAR_WIDTH;
    f32 ch = OMM_PE_CHAR_HEIGHT;
    f32 ww = X_SCREEN_TO_WINDOW(GFX_DIMENSIONS_FROM_LEFT_EDGE(2 + 6 * cw + ch));
    f32 wh = Y_SCREEN_TO_WINDOW(ch);
    str_t rgb;
    str_fmt(rgb, sizeof(rgb), "%02X%02X%02X", (u8) (r * 255.9f), (u8) (g * 255.9f), (u8) (b * 255.9f));
    omm_render_rect_window_coords(wx, wy - wh, ww, wh, 0xFF, 0xFF, 0xFF, 0xFF);
    omm_render_rect_window_coords(wx + 1, wy - wh + 1, ww - wh - 2, wh - 2, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + ww - wh, wy - wh + 1, wh - 1, wh - 2, 0x00, 0x00, 0x00, 0xFF);
    omm_render_rect_window_coords(wx + ww - wh + 1, wy - wh + 2, wh - 3, wh - 4, (u8) (r * 255.9f), (u8) (g * 255.9f), (u8) (b * 255.9f), 0xFF);
    gDisplayListHead = gfx_font_init(gDisplayListHead, OMM_TEXTURE_FONT_PALETTE_EDITOR, 16, 512, 384, 32, 64);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[0], x + 1 + 0 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[1], x + 1 + 1 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[2], x + 1 + 2 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[3], x + 1 + 3 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[4], x + 1 + 4 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_render_char(gDisplayListHead, rgb[5], x + 1 + 5 * cw, y - ch, cw, ch, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDisplayListHead = gfx_font_end(gDisplayListHead);
}

//
// Geo functions and layout
//

static Gfx *omm_geo_palette_editor_switch_background(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER) {
        ((struct GraphNodeSwitchCase *) node)->selectedCase = pe->skybox;
    }
    return NULL;
}

static void omm_geo_palette_editor_render_colormap(s32 k) {
    u8 OxFF = (OMM_PE_DROPDOWN_LINE(-1) ? 1.f : OMM_PE_DROPDOWN_DARKEN) * 0xFF;
    u8 white = (OMM_PE_DROPDOWN_LINE(-1) ? 1.f : OMM_PE_DROPDOWN_DARKEN) * OMM_PE_WHITE;
    omm_render_texrect(pe->colormaps[k]->x, pe->colormaps[k]->y, pe->colormaps[k]->w, pe->colormaps[k]->h, 32, 32, white, white, white, 0xFF, OMM_TEXTURE_MISC_WHITE, false);
    if (pe->bright) {
        Vtx *vtx = omm_alloc_vtx(4);
        s32 x0 = pe->colormaps[k]->x + 1, x1 = pe->colormaps[k]->x + pe->colormaps[k]->w - 1;
        s32 y0 = pe->colormaps[k]->y + 1, y1 = pe->colormaps[k]->y + pe->colormaps[k]->h - 1;
        vtx[0] = (Vtx) {{{ x0, y0, 0 }, 0, { (0x400 * pe->lights[k]->u + 0) * 32.f, 0x2000 }, { 0x00, 0x00, 0x00, 0xFF }}};
        vtx[1] = (Vtx) {{{ x1, y0, 0 }, 0, { (0x400 * pe->lights[k]->u + 1) * 32.f, 0x2000 }, { OxFF, OxFF, OxFF, 0xFF }}};
        vtx[2] = (Vtx) {{{ x0, y1, 0 }, 0, { (0x400 * pe->lights[k]->u + 0) * 32.f, 0x0000 }, { 0x00, 0x00, 0x00, 0xFF }}};
        vtx[3] = (Vtx) {{{ x1, y1, 0 }, 0, { (0x400 * pe->lights[k]->u + 1) * 32.f, 0x0000 }, { OxFF, OxFF, OxFF, 0xFF }}};
        omm_render_create_dl_ortho_matrix();
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING | G_CULL_BOTH);
        gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(gDisplayListHead++, OMM_TEXTURE_MISC_COLOR_MAP, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1024, 256, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, vtx, 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 2, 1, 3, 0);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        omm_palette_editor_render_color_picker((s32) X_SCREEN_TO_WINDOW(OMM_PE_COLOR_PICKER_X(pe->lights[k]->w)), (s32) Y_SCREEN_TO_WINDOW(OMM_PE_COLOR_PICKER_Y(1 - pe->lights[k]->v)) - 1);
    } else {
        omm_render_texrect(pe->colormaps[k]->x + 1, pe->colormaps[k]->y + 1, pe->colormaps[k]->w - 2, pe->colormaps[k]->h - 2, 1024, 256, pe->lights[k]->w * OxFF, pe->lights[k]->w * OxFF, pe->lights[k]->w * OxFF, 0xFF, OMM_TEXTURE_MISC_COLOR_MAP, false);
        omm_palette_editor_render_color_picker((s32) X_SCREEN_TO_WINDOW(OMM_PE_COLOR_PICKER_X(pe->lights[k]->u)), (s32) Y_SCREEN_TO_WINDOW(OMM_PE_COLOR_PICKER_Y(1 - pe->lights[k]->v)) - 1);
    }
}

static Gfx *omm_geo_palette_editor_render(s32 callContext, UNUSED struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER && pe->state == OMM_PE_STATE_OPEN) {
        omm_render_texrect(GFX_DIMENSIONS_FROM_LEFT_EDGE(-1), -1, OMM_PE_DISPLAY_W + 1, SCREEN_HEIGHT + 2, 32, 32, 0x00, 0x00, 0x00, 0xA0, OMM_TEXTURE_MISC_WHITE, false);

        // Save button background
        u8 *textSave = omm_text_convert(OMM_TEXT_PE_SAVE, false);
        s32 textSaveLen = omm_text_length(textSave);
        omm_render_texrect(OMM_PE_SAVE_BUTTON_X, OMM_PE_SAVE_BUTTON_Y, OMM_PE_SAVE_BUTTON_W, OMM_PE_SAVE_BUTTON_H, 32, 32, 0x00, 0x00, 0x00, 0xA0, OMM_TEXTURE_MISC_WHITE, false);

        // Hover box
        if (pe->event == OMM_PE_EVENT_NONE && pe->hover->w > 0.f && pe->hover->h > 0.f) {
            u8 c = (pe->cursor->down ? 0xC0 : 0x80);
            omm_render_texrect(pe->hover->x, pe->hover->y, pe->hover->w, pe->hover->h, 32, 32, c, c, c, 0xC0, OMM_TEXTURE_MISC_WHITE, false);
        } else if (pe->event->selectedBox) {
            omm_render_texrect(pe->hover->x, pe->hover->y, pe->hover->w, pe->hover->h, 32, 32, 0xC0, 0xC0, 0xC0, 0xC0, OMM_TEXTURE_MISC_WHITE, false);
        }

        // Save button text
        omm_palette_editor_render_string(textSave, textSaveLen, OMM_PE_SAVE_BUTTON_X + 4, OMM_PE_SAVE_BUTTON_Y + 4, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3, 0xFF, true, false);

        // Player
        u8 *textPlayer = omm_text_convert(OMM_TEXT_PE_PLAYER, false);
        u8 *textPlayerName = omm_text_convert(omm_player_properties_get_name(pe->peach), false);
        omm_palette_editor_render_string(textPlayer, OMM_PE_STR_MAX_LENGTH_LEFT, OMM_PE_TEXT_X, OMM_PE_TEXT_PLAYER_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, true);
        omm_palette_editor_render_string(textPlayerName, OMM_PE_STR_MAX_LENGTH_RIGHT, OMM_PE_TEXT_X2, OMM_PE_TEXT_PLAYER_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3, 0xFF, true, true);

        // Palette
        const char *paletteStr = omm_mario_colors_choices(pe->peach)[omm_mario_colors_count() - 4 + pe->palette];
        u8 *textPalette = omm_text_convert(OMM_TEXT_PE_PALETTE, false);
        u8 *textPaletteName = omm_text_convert(paletteStr, false);
        omm_palette_editor_render_string(textPalette, OMM_PE_STR_MAX_LENGTH_LEFT, OMM_PE_TEXT_X, OMM_PE_TEXT_PALETTE_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, true);
        omm_palette_editor_render_string(textPaletteName, OMM_PE_STR_MAX_LENGTH_RIGHT, OMM_PE_TEXT_X2, OMM_PE_TEXT_PALETTE_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3, 0xFF, true, true);

        // Group
        u8 *textGroup = omm_text_convert(OMM_TEXT_PE_GROUP, false);
        u8 *textGroupName = omm_text_convert(OMM_PALETTE_EDITOR_GROUP_CHOICES[pe->group], false);
        omm_palette_editor_render_string(textGroup, OMM_PE_STR_MAX_LENGTH_LEFT, OMM_PE_TEXT_X, OMM_PE_TEXT_GROUP_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, true);
        omm_palette_editor_render_string(textGroupName, OMM_PE_STR_MAX_LENGTH_RIGHT, OMM_PE_TEXT_X2, OMM_PE_TEXT_GROUP_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3, 0xFF, true, true);

        // Lights
        if (pe->group == 0) {
            s32 palette = omm_mario_colors_count() - 4 + pe->palette;
            s32 numGroups = omm_palette_editor_light_groups_count(pe->peach, pe->emblem);

            // For each group
            for (s32 group = 0; group != numGroups; ++group) {
                s32 y = OMM_PE_TEXT_COLOR_1_Y - group * OMM_PE_TEXT_H;

                // Group name
                textGroupName = omm_text_convert(OMM_PALETTE_EDITOR_GROUP_CHOICES[1 + group], false);
                if (group == pe->subgroup) {
                    omm_palette_editor_render_string(textGroupName, OMM_PE_STR_MAX_LENGTH_LEFT, OMM_PE_TEXT_X, y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, 0xFF, 0xFF, 0x00, 0xFF, true, true);
                } else {
                    omm_palette_editor_render_string(textGroupName, OMM_PE_STR_MAX_LENGTH_LEFT, OMM_PE_TEXT_X, y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, true);
                }

                // For each color
                for (s32 k = 0; k != 2; ++k) {
                    u32 color = *omm_mario_colors_get_light(pe->peach, palette, group * 2 + k);
                    s32 dx = k * (OMM_PE_CHAR_WIDTH * 10.5f);
                    str_t colorValueStr;
                    str_fmt(colorValueStr, sizeof(colorValueStr), "%06X", color);
                    u8 *textColorValue = omm_text_convert(colorValueStr, false);
                    u8 white = (OMM_PE_DROPDOWN_LINE(y) ? 1.f : OMM_PE_DROPDOWN_DARKEN) * OMM_PE_WHITE;
                    f32 mult = (OMM_PE_DROPDOWN_LINE(y) ? 1.f : OMM_PE_DROPDOWN_DARKEN);
                    omm_palette_editor_render_string(textColorValue, OMM_PE_STR_MAX_LENGTH_RIGHT, OMM_PE_TEXT_X2 + dx, y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3 * (group != pe->subgroup), 0xFF, true, true);
                    omm_render_texrect(OMM_PE_COLOR_BOX_X + dx, y + OMM_PE_COLOR_BOX_Y, OMM_PE_COLOR_BOX_W, OMM_PE_COLOR_BOX_H, 32, 32, white, white, white, 0xFF, OMM_TEXTURE_MISC_WHITE, false);
                    omm_render_texrect(OMM_PE_COLOR_BOX_X + dx + 1, y + OMM_PE_COLOR_BOX_Y + 1, OMM_PE_COLOR_BOX_W - 2, OMM_PE_COLOR_BOX_H - 2, 32, 32, ((color >> 16) & 0xFF) * mult, ((color >> 8) & 0xFF) * mult, ((color >> 0) & 0xFF) * mult, 0xFF, OMM_TEXTURE_MISC_WHITE, false);
                }
            }

            // Color maps
            for (s32 k = 0; k != 2; ++k) {
                s32 y = OMM_PE_TEXT_COLOR_1_Y - numGroups * OMM_PE_TEXT_H - 2 - k * (OMM_PE_TEXT_H + pe->colormaps[0]->h + 4);

                // Light name
                u8 *textLightName = omm_text_convert(omm_mario_colors_light_name(pe->peach, pe->subgroup * 2 + k), false);
                omm_palette_editor_render_string(textLightName, OMM_PE_STR_MAX_LENGTH_RIGHT, OMM_PE_TEXT_X, y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, true);

                // Color map
                omm_geo_palette_editor_render_colormap(k);
            }

        } else for (s32 k = 0; k != 2; ++k) {

            // Color
            str_t colorValueStr;
            str_fmt(colorValueStr, sizeof(colorValueStr), "%06X", *pe->lights[k]->p);
            u8 *textColor = omm_text_convert(k ? OMM_TEXT_PE_COLOR_2 : OMM_TEXT_PE_COLOR_1, false);
            u8 *textColorValue = omm_text_convert(colorValueStr, false);
            u8 OxFF = (OMM_PE_DROPDOWN_LINE(OMM_PE_TEXT_COLOR_Y) ? 1.f : OMM_PE_DROPDOWN_DARKEN) * 0xFF;
            u8 white = (OMM_PE_DROPDOWN_LINE(OMM_PE_TEXT_COLOR_Y) ? 1.f : OMM_PE_DROPDOWN_DARKEN) * OMM_PE_WHITE;
            omm_palette_editor_render_string(textColor, OMM_PE_STR_MAX_LENGTH_LEFT, OMM_PE_TEXT_X, OMM_PE_TEXT_COLOR_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, true);
            omm_palette_editor_render_string(textColorValue, OMM_PE_STR_MAX_LENGTH_RIGHT, OMM_PE_TEXT_X2, OMM_PE_TEXT_COLOR_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3, 0xFF, true, true);
            omm_render_texrect(OMM_PE_COLOR_BOX_X, OMM_PE_TEXT_COLOR_Y + OMM_PE_COLOR_BOX_Y, OMM_PE_COLOR_BOX_W, OMM_PE_COLOR_BOX_H, 32, 32, white, white, white, 0xFF, OMM_TEXTURE_MISC_WHITE, false);
            omm_render_texrect(OMM_PE_COLOR_BOX_X + 1, OMM_PE_TEXT_COLOR_Y + OMM_PE_COLOR_BOX_Y + 1, OMM_PE_COLOR_BOX_W - 2, OMM_PE_COLOR_BOX_H - 2, 32, 32, pe->lights[k]->r * OxFF, pe->lights[k]->g * OxFF, pe->lights[k]->b * OxFF, 0xFF, OMM_TEXTURE_MISC_WHITE, false);

            // Color map
            omm_geo_palette_editor_render_colormap(k);
        }

        // Background
        u8 *textBackground = omm_text_convert(OMM_TEXT_PE_BACKGROUND, false);
        u8 *textBackgroundName = omm_text_convert(OMM_PALETTE_EDITOR_BACKGROUND_CHOICES[pe->skybox], false);
        omm_palette_editor_render_string(textBackground, OMM_PE_STR_MAX_LENGTH_LEFT, OMM_PE_TEXT_X, OMM_PE_TEXT_BACKGROUND_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, true);
        omm_palette_editor_render_string(textBackgroundName, OMM_PE_STR_MAX_LENGTH_RIGHT, OMM_PE_TEXT_X2, OMM_PE_TEXT_BACKGROUND_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3, 0xFF, true, true);

        // Cap state
        u8 *textCapState = omm_text_convert(OMM_TEXT_PE_CAP_STATE, false);
        u8 *textCapStateName = omm_text_convert(OMM_PALETTE_EDITOR_CAP_CHOICES[pe->cap], false);
        omm_palette_editor_render_string(textCapState, OMM_PE_STR_MAX_LENGTH_LEFT, OMM_PE_TEXT_X, OMM_PE_TEXT_CAP_STATE_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, true);
        omm_palette_editor_render_string(textCapStateName, OMM_PE_STR_MAX_LENGTH_RIGHT, OMM_PE_TEXT_X2, OMM_PE_TEXT_CAP_STATE_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3, 0xFF, true, true);

        // Animations
        u8 *textAnimations = omm_text_convert(OMM_TEXT_PE_ANIMATIONS, false);
        u8 *textAnimationsName = omm_text_convert(pe->anims ? OMM_TEXT_PE_ON : OMM_TEXT_PE_OFF, false);
        omm_palette_editor_render_string(textAnimations, OMM_PE_STR_MAX_LENGTH_LEFT, OMM_PE_TEXT_X, OMM_PE_TEXT_ANIMATIONS_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, true);
        omm_palette_editor_render_string(textAnimationsName, OMM_PE_STR_MAX_LENGTH_RIGHT, OMM_PE_TEXT_X2, OMM_PE_TEXT_ANIMATIONS_Y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, pe->anims ? 0x00 : 0xFF, pe->anims ? 0xFF : 0x00, 0x00, 0xFF, true, true);

        // Events
        if (pe->event->dropDown) {
            omm_palette_editor_render_dropdown_background(pe->dropdown->x, pe->dropdown->y, pe->dropdown->w, pe->dropdown->h);
            for (s32 i = 0; i != pe->dropdown->count; ++i) {
                s32 y = OMM_PE_DROPDOWN_TEXT_Y(i);
                if (i == pe->dropdown->selected) {
                    omm_render_texrect(pe->dropdown->x, OMM_PE_DROPDOWN_HOVER_Y(y), pe->dropdown->w, OMM_PE_DROPDOWN_HOVER_H, 32, 32, 0x80, 0x80, 0x80, 0xC0, OMM_TEXTURE_MISC_WHITE, false);
                }
                u8 *textChoice = omm_text_convert(pe->dropdown->choices[i], false);
                omm_palette_editor_render_string(textChoice, OMM_PE_STR_MAX_LENGTH_RIGHT, pe->dropdown->x + OMM_PE_DROPDOWN_HOVER_MARGIN, y, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3, 0xFF, true, false);
            }
        } else if (pe->event->inputColor) {
            u8 *textInput = omm_text_convert(OMM_TEXT_PE_INPUT_COLOR, false);
            s32 textInputLength = omm_text_length(textInput);
            s32 inputBoxW = OMM_PE_INPUT_BOX_MARGIN * 2 + OMM_PE_CHAR_WIDTH * (textInputLength + 2) + 6 * OMM_PE_INPUT_BOX_CHAR_WIDTH;
            s32 inputBoxH = OMM_PE_INPUT_BOX_MARGIN * 2 + OMM_PE_INPUT_BOX_CHAR_HEIGHT;
            s32 inputBoxX = (SCREEN_WIDTH + OMM_PE_DISPLAY_W - inputBoxW) / 2;
            s32 inputBoxY = (SCREEN_HEIGHT - inputBoxH) / 2;
            s32 textInputX = inputBoxX + OMM_PE_INPUT_BOX_MARGIN;
            s32 textInputY = inputBoxY + OMM_PE_INPUT_BOX_MARGIN + OMM_PE_INPUT_BOX_CHAR_MARGIN - OMM_PE_INPUT_BOX_CHAR_OFFSET;
            omm_render_texrect(inputBoxX, inputBoxY, inputBoxW, inputBoxH, 32, 32, 0x20, 0x20, 0x20, 0xE0, OMM_TEXTURE_MISC_WHITE, false);
            omm_palette_editor_render_string(textInput, textInputLength, textInputX, textInputY, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, false);
            for (s32 i = 0; i != 6; ++i) {
                const u8 *inputBoxColor = OMM_PALETTE_EDITOR_INPUT_COLORS[i];
                inputBoxX = textInputX + OMM_PE_CHAR_WIDTH * (textInputLength + 2) + i * OMM_PE_INPUT_BOX_CHAR_WIDTH;
                omm_render_texrect(inputBoxX, inputBoxY + OMM_PE_INPUT_BOX_MARGIN, OMM_PE_INPUT_BOX_CHAR_WIDTH, OMM_PE_INPUT_BOX_CHAR_HEIGHT, 32, 32, inputBoxColor[0], inputBoxColor[1], inputBoxColor[2], 0xFF, OMM_TEXTURE_MISC_WHITE, false);
                if (i < pe->input->index) {
                    u8 textDigit[] = { ((pe->input->value >> (4 * (5 - i))) & 0xF), 0xFF };
                    omm_palette_editor_render_string(textDigit, 1, inputBoxX + OMM_PE_INPUT_BOX_CHAR_MARGIN, textInputY, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, 0x00, 0x00, 0x00, 0xFF, false, false);
                }
            }
        } else if (pe->event == OMM_PE_EVENT_COPY_PALETTE) {
            omm_render_texrect(OMM_PE_PALETTES_X, OMM_PE_PALETTES_Y, OMM_PE_PALETTES_W, OMM_PE_PALETTES_H, 32, 32, 0x20, 0x20, 0x20, 0xE0, OMM_TEXTURE_MISC_WHITE, false);
            for (s32 palette = 0; palette != pe->dropdown->count; ++palette) {
                s32 i = palette / (pe->dropdown->count / 2);
                s32 j = palette % (pe->dropdown->count / 2);
                s32 paletteX = OMM_PE_PALETTES_HOVER_X(i);
                s32 paletteY = OMM_PE_PALETTES_HOVER_Y(j);
                s32 paletteC = (palette == pe->dropdown->selected ? 0x80 : 0x40);
                omm_render_texrect(paletteX, paletteY, OMM_PE_PALETTES_HOVER_W, OMM_PE_PALETTES_HOVER_H, 32, 32, paletteC, paletteC, paletteC, 0xE0, OMM_TEXTURE_MISC_WHITE, false);
                const char *paletteStr = omm_mario_colors_choices(pe->peach)[palette];
                u8 *textPaletteName = omm_text_convert(paletteStr, false);
                s32 textPaletteNameX = OMM_PE_PALETTES_TEXT_X(i) - (OMM_PE_CHAR_WIDTH * min_s(OMM_PE_STR_MAX_LENGTH_PALETTES, omm_text_length(textPaletteName))) / 2;
                s32 textPaletteNameY = OMM_PE_PALETTES_TEXT_Y(j);
                omm_palette_editor_render_string(textPaletteName, OMM_PE_STR_MAX_LENGTH_PALETTES, textPaletteNameX, textPaletteNameY, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_WHITE_3, 0xFF, true, false);
            }
            const char *paletteStr = omm_mario_colors_choices(pe->peach)[omm_mario_colors_count() - 4 + pe->palette];
            u8 *textCopyPalette1 = omm_text_convert(OMM_TEXT_PE_COPY_PALETTE, false);
            s32 textCopyPaletteLength1 = omm_text_length(textCopyPalette1);
            u8 *textCopyPalette2 = omm_text_convert(paletteStr, false);
            s32 textCopyPaletteLength2 = omm_text_length(textCopyPalette2);
            s32 textCopyPalette1X = (SCREEN_WIDTH + OMM_PE_DISPLAY_W - OMM_PE_CHAR_WIDTH * (textCopyPaletteLength1 + textCopyPaletteLength2)) / 2;
            s32 textCopyPalette2X = textCopyPalette1X + OMM_PE_CHAR_WIDTH * textCopyPaletteLength1;
            s32 textCopyPaletteY = OMM_PE_PALETTES_TEXT_Y(-1) + 2;
            omm_palette_editor_render_string(textCopyPalette1, textCopyPaletteLength1, textCopyPalette1X, textCopyPaletteY, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, false);
            omm_palette_editor_render_string(textCopyPalette2, textCopyPaletteLength2, textCopyPalette2X, textCopyPaletteY, OMM_PE_CHAR_WIDTH, OMM_PE_CHAR_HEIGHT, OMM_PE_GRAY_3, 0xFF, true, false);
        }

        // Color preview
        if (pe->event->colorPreview) {
            f32 u, v, w;
            if (omm_palette_editor_get_pointed_color(&u, &v, &w) != -1) {
                omm_palette_editor_render_color_preview(u, v, w);
            }
        } else if ((pe->event == OMM_PE_EVENT_DROPDOWN_COLOR_1 || pe->event == OMM_PE_EVENT_DROPDOWN_COLOR_2) && pe->dropdown->selected == 2) {
            omm_palette_editor_render_paste_color_preview(
                pe->dropdown->x + pe->dropdown->w,
                OMM_PE_DROPDOWN_HOVER_Y(OMM_PE_DROPDOWN_TEXT_Y(1)),
                pe->copy->r, pe->copy->g, pe->copy->b
            );
        }

        // Cursor
        Vtx *vtx = omm_alloc_vtx(4);
        omm_render_create_dl_ortho_matrix();
        OMM_RENDER_ENABLE_ALPHA(gDisplayListHead++);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(gDisplayListHead++, pe->cursor->down ? OMM_ASSET_MENU_MARIO_HAND_CLOSED : OMM_ASSET_MENU_MARIO_HAND_OPEN, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, vtx, 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
        vtx[0] = (Vtx) {{{ pe->cursor->x - 0.05f * OMM_PE_CURSOR_W, pe->cursor->y - 0.70f * OMM_PE_CURSOR_H, 0 }, 0, { 0x000, 0x400 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
        vtx[1] = (Vtx) {{{ pe->cursor->x + 0.95f * OMM_PE_CURSOR_W, pe->cursor->y - 0.70f * OMM_PE_CURSOR_H, 0 }, 0, { 0x400, 0x400 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
        vtx[2] = (Vtx) {{{ pe->cursor->x + 0.95f * OMM_PE_CURSOR_W, pe->cursor->y + 0.30f * OMM_PE_CURSOR_H, 0 }, 0, { 0x400, 0x000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
        vtx[3] = (Vtx) {{{ pe->cursor->x - 0.05f * OMM_PE_CURSOR_W, pe->cursor->y + 0.30f * OMM_PE_CURSOR_H, 0 }, 0, { 0x000, 0x000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    }
    return NULL;
}

static const GeoLayout omm_geo_palette_editor[] = {
    GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
    GEO_OPEN_NODE(),
        GEO_ZBUFFER(0),
        GEO_OPEN_NODE(),
            GEO_NODE_ORTHO(100),
            GEO_OPEN_NODE(),
                GEO_SWITCH_CASE(10, omm_geo_palette_editor_switch_background),
                GEO_OPEN_NODE(),
                    GEO_BACKGROUND(BACKGROUND_OCEAN_SKY, geo_skybox_main),
                    GEO_BACKGROUND(BACKGROUND_FLAMING_SKY, geo_skybox_main),
                    GEO_BACKGROUND(BACKGROUND_UNDERWATER_CITY, geo_skybox_main),
                    GEO_BACKGROUND(BACKGROUND_BELOW_CLOUDS, geo_skybox_main),
                    GEO_BACKGROUND(BACKGROUND_SNOW_MOUNTAINS, geo_skybox_main),
                    GEO_BACKGROUND(BACKGROUND_DESERT, geo_skybox_main),
                    GEO_BACKGROUND(BACKGROUND_HAUNTED, geo_skybox_main),
                    GEO_BACKGROUND(BACKGROUND_GREEN_SKY, geo_skybox_main),
                    GEO_BACKGROUND(BACKGROUND_ABOVE_CLOUDS, geo_skybox_main),
                    GEO_BACKGROUND(BACKGROUND_PURPLE_SKY, geo_skybox_main),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
        GEO_ZBUFFER(1),
        GEO_OPEN_NODE(),
            GEO_CAMERA_FRUSTUM(45, 100, 25000),
            GEO_OPEN_NODE(),
                GEO_CAMERA(0, 0, 0, 1000, 0, 0, 0, 0),
                GEO_OPEN_NODE(),
                    GEO_RENDER_OBJ(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
        GEO_ZBUFFER(0),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_geo_palette_editor_render),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Init
//

static s32 omm_level_palette_editor_init(UNUSED s32 arg, UNUSED s32 unused) {
    gMarioState->unk00 = 0;
    gMarioState->flags = 0;
    gMarioState->action = 0;
    gMarioState->spawnInfo = &gPlayerSpawnInfos[0];
    gMarioState->statusForCamera = &gPlayerCameraState[0];
    gMarioState->marioBodyState = &gBodyStates[0];
    gMarioState->controller = &gControllers[0];
    gMarioState->marioBodyState->capState = MARIO_HAS_DEFAULT_CAP_ON;
    gMarioState->marioBodyState->eyeState = MARIO_EYES_OPEN;
    gMarioState->marioBodyState->handState = MARIO_HAND_FISTS;
    gMarioAnimations = &gMarioAnimsData;
    pe->event = OMM_PE_EVENT_NONE;
    pe->peach = false;
    pe->emblem = omm_palette_editor_use_emblem();
    pe->cap = 1;
    pe->anims = true;
    pe->palette = 0;
    pe->group = 1;
    pe->subgroup = 0;
    pe->pitch = 0;
    pe->yaw = 0;
    pe->zoom = 20;
    pe->skybox = BACKGROUND_BELOW_CLOUDS;
    pe->bright = false;
    pe->cursor->x = SCREEN_WIDTH / 2;
    pe->cursor->y = SCREEN_HEIGHT / 2;
    pe->cursor->cx = 0;
    pe->cursor->cy = 0;
    pe->cursor->down = false;
    pe->cursor->hindex = -1;
    pe->cursor->vindex = -1;
    pe->mouse->pressed = 0;
    pe->mouse->down = SDL_GetMouseState(&pe->mouse->wx, &pe->mouse->wy);
    pe->hover->x = 0;
    pe->hover->y = 0;
    pe->hover->w = 0;
    pe->hover->h = 0;
    pe->input->value = 0;
    pe->input->index = 0;
    pe->dropdown->x = 0;
    pe->dropdown->y = 0;
    pe->dropdown->w = 0;
    pe->dropdown->h = 0;
    pe->dropdown->count = 0;
    pe->dropdown->selected = -1;
    pe->dropdown->choices = NULL;
    omm_palette_editor_set_current();
    return 0;
}

//
// Utils
//

static bool omm_palette_editor_update_toggle(s32 y, s32 *toggle) {
    s32 boxx = OMM_PE_HOVER_BOX_X_1;
    s32 boxy = OMM_PE_HOVER_BOX_Y(y);
    s32 boxw = OMM_PE_HOVER_BOX_W_1_2;
    s32 boxh = OMM_PE_HOVER_BOX_H;
    if (pe->event == OMM_PE_EVENT_NONE && omm_palette_editor_is_inside_box(pe->cursor->x, pe->cursor->y, boxx, boxy, boxw, boxh)) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (OMM_PE_INPUT_A_PRESSED) {
            *toggle = !*toggle;
            return true;
        }
    }
    return false;
}

static bool omm_palette_editor_update_dropdown(ev *event, s32 y, s32 *choice, const char **choices, s32 count) {
    s32 boxx = OMM_PE_HOVER_BOX_X_1;
    s32 boxy = OMM_PE_HOVER_BOX_Y(y);
    s32 boxw = OMM_PE_HOVER_BOX_W_1_2;
    s32 boxh = OMM_PE_HOVER_BOX_H;
    if (pe->event == event) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (OMM_PE_INPUT_UP_PRESSED || OMM_PE_INPUT_DOWN_PRESSED) {
            if (OMM_PE_INPUT_UP_PRESSED) pe->dropdown->selected = (pe->dropdown->selected + pe->dropdown->count - 1) % pe->dropdown->count;
            if (OMM_PE_INPUT_DOWN_PRESSED) pe->dropdown->selected = (pe->dropdown->selected + 1) % pe->dropdown->count;
            s32 y = OMM_PE_DROPDOWN_TEXT_Y(pe->dropdown->selected);
            s32 hx = OMM_PE_DROPDOWN_HOVER_X;
            s32 hy = OMM_PE_DROPDOWN_HOVER_Y(y);
            s32 hw = OMM_PE_DROPDOWN_HOVER_W;
            s32 hh = OMM_PE_DROPDOWN_HOVER_H;
            pe->cursor->x = hx + hw * 0.9f;
            pe->cursor->y = hy + hh / 2.f;
        }
        s32 selected = -1;
        for (s32 i = 0; i != pe->dropdown->count; ++i) {
            s32 y = OMM_PE_DROPDOWN_TEXT_Y(i);
            s32 hx = OMM_PE_DROPDOWN_HOVER_X;
            s32 hy = OMM_PE_DROPDOWN_HOVER_Y(y);
            s32 hw = OMM_PE_DROPDOWN_HOVER_W;
            s32 hh = OMM_PE_DROPDOWN_HOVER_H;
            if (omm_palette_editor_is_inside_box(pe->cursor->x, pe->cursor->y, hx, hy, hw, hh)) {
                pe->dropdown->selected = i;
                selected = i;
                break;
            }
        }
        if (OMM_PE_INPUT_A_PRESSED || OMM_PE_INPUT_B_PRESSED) {
            gPlayer1Controller->buttonPressed &= ~A_BUTTON;
            pe->mouse->pressed &= ~SDL_BUTTON_LMASK;
            pe->event = OMM_PE_EVENT_NONE;
            if (!OMM_PE_INPUT_B_PRESSED && selected >= 0 && selected < pe->dropdown->count) {
                *choice = selected;
                return true;
            }
        }
    } else if (pe->event == OMM_PE_EVENT_NONE && omm_palette_editor_is_inside_box(pe->cursor->x, pe->cursor->y, boxx, boxy, boxw, boxh)) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (OMM_PE_INPUT_A_PRESSED) {
            pe->event = event;
            pe->dropdown->count = count;
            pe->dropdown->choices = choices;
            pe->dropdown->selected = *choice;
            pe->dropdown->w = pe->hover->w;
            pe->dropdown->h = OMM_PE_DROPDOWN_H + (pe->dropdown->count - 1) * OMM_PE_TEXT_H;
            pe->dropdown->x = pe->hover->x;
            pe->dropdown->y = OMM_PE_DROPDOWN_Y(y) - pe->dropdown->h;
            if (pe->dropdown->y < 0) {
                pe->dropdown->y = y + OMM_PE_HOVER_BOX_H;
            }
        }
    }
    return false;
}

static bool omm_palette_editor_select_group(s32 group, s32 boxx, s32 y, s32 boxw, s32 boxh) {
    s32 boxy = OMM_PE_HOVER_BOX_Y(y);
    if (pe->event == OMM_PE_EVENT_NONE && omm_palette_editor_is_inside_box(pe->cursor->x, pe->cursor->y, boxx, boxy, boxw, boxh)) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (OMM_PE_INPUT_A_PRESSED) {
            pe->input->value = 0;
            pe->input->index = 0;
            pe->subgroup = group;
            return true;
        }
    }
    return false;
}

static void omm_palette_editor_update_color_dropdown(ev *event1, ev *event2, s32 group, s32 boxx, s32 y, s32 boxw, s32 boxh, const char **choices, s32 count) {
    s32 boxy = OMM_PE_HOVER_BOX_Y(y);

    // Dropdown
    if (pe->event == event1 && (pe->group != 0 || pe->subgroup == group)) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (OMM_PE_INPUT_UP_PRESSED || OMM_PE_INPUT_DOWN_PRESSED) {
            if (OMM_PE_INPUT_UP_PRESSED) pe->dropdown->selected = (pe->dropdown->selected + pe->dropdown->count - 1) % pe->dropdown->count;
            if (OMM_PE_INPUT_DOWN_PRESSED) pe->dropdown->selected = (pe->dropdown->selected + 1) % pe->dropdown->count;
            s32 y = OMM_PE_DROPDOWN_TEXT_Y(pe->dropdown->selected);
            s32 hx = pe->dropdown->x;
            s32 hy = OMM_PE_DROPDOWN_HOVER_Y(y);
            s32 hw = pe->dropdown->w;
            s32 hh = OMM_PE_DROPDOWN_HOVER_H;
            pe->cursor->x = hx + hw * 0.9f;
            pe->cursor->y = hy + hh / 2.f;
        }
        s32 selected = -1;
        for (s32 i = 0; i != pe->dropdown->count; ++i) {
            s32 y = OMM_PE_DROPDOWN_TEXT_Y(i);
            s32 hx = pe->dropdown->x;
            s32 hy = OMM_PE_DROPDOWN_HOVER_Y(y);
            s32 hw = pe->dropdown->w;
            s32 hh = OMM_PE_DROPDOWN_HOVER_H;
            if (omm_palette_editor_is_inside_box(pe->cursor->x, pe->cursor->y, hx, hy, hw, hh)) {
                pe->dropdown->selected = i;
                selected = i;
                break;
            }
        }
        if (OMM_PE_INPUT_A_PRESSED || OMM_PE_INPUT_B_PRESSED) {
            s32 k = pe->event->color;
            gPlayer1Controller->buttonPressed &= ~A_BUTTON;
            pe->mouse->pressed &= ~SDL_BUTTON_LMASK;
            pe->event = OMM_PE_EVENT_NONE;
            if (!OMM_PE_INPUT_B_PRESSED && selected >= 0 && selected < pe->dropdown->count) {
                switch (selected) {

                    // Input color
                    case 0: {
                        controller_get_raw_key();
                        pe->event = event2;
                        pe->input->value = 0;
                        pe->input->index = 0;
                    } break;

                    // Copy color
                    case 1: {
                        omm_palette_editor_uvw_to_rgb(
                            pe->lights[k]->u,
                            pe->lights[k]->v,
                            pe->lights[k]->w,
                            &pe->copy->r,
                            &pe->copy->g,
                            &pe->copy->b
                        );
                    } break;

                    // Paste color
                    case 2: {
                        omm_palette_editor_rgb_to_uvw(
                            pe->copy->r,
                            pe->copy->g,
                            pe->copy->b,
                            &pe->lights[k]->u,
                            &pe->lights[k]->v,
                            &pe->lights[k]->w
                        );
                        omm_palette_editor_update_current();
                    } break;

                    // Lighten color
                    case 3: {
                        omm_palette_editor_rgb_to_uvw(
                            min_f(1.f, pe->lights[k]->r * 2.f),
                            min_f(1.f, pe->lights[k]->g * 2.f),
                            min_f(1.f, pe->lights[k]->b * 2.f),
                            &pe->lights[k]->u,
                            &pe->lights[k]->v,
                            &pe->lights[k]->w
                        );
                        omm_palette_editor_update_current();
                    } break;

                    // Darken color
                    case 4: {
                        omm_palette_editor_rgb_to_uvw(
                            min_f(1.f, pe->lights[k]->r * 0.5f),
                            min_f(1.f, pe->lights[k]->g * 0.5f),
                            min_f(1.f, pe->lights[k]->b * 0.5f),
                            &pe->lights[k]->u,
                            &pe->lights[k]->v,
                            &pe->lights[k]->w
                        );
                        omm_palette_editor_update_current();
                    } break;
                }
            }
            return;
        }
    }

    // Color input
    else if (pe->event == event2 && (pe->group != 0 || pe->subgroup == group)) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (pe->input->index == 6) {
            *pe->lights[pe->event->color]->p = pe->input->value;
            omm_palette_editor_set_current();
            pe->event = OMM_PE_EVENT_NONE;
            return;
        }
        s32 number = omm_palette_editor_get_number_from_keyboard_key();
        if (number != -1) {
            pe->input->value |= (number << (4 * (5 - pe->input->index)));
            pe->input->index++;
            return;
        }
        if (OMM_PE_INPUT_A_PRESSED || OMM_PE_INPUT_B_PRESSED) {
            gPlayer1Controller->buttonPressed &= ~A_BUTTON;
            pe->mouse->pressed &= ~SDL_BUTTON_LMASK;
            pe->event = OMM_PE_EVENT_NONE;
            return;
        }
    }

    // Hover
    else if (pe->event == OMM_PE_EVENT_NONE && omm_palette_editor_is_inside_box(pe->cursor->x, pe->cursor->y, boxx, boxy, boxw, boxh)) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (OMM_PE_INPUT_A_PRESSED) {
            gPlayer1Controller->buttonPressed &= ~A_BUTTON;
            pe->mouse->pressed &= ~SDL_BUTTON_LMASK;
            pe->event = event1;
            pe->dropdown->count = count;
            pe->dropdown->choices = choices;
            pe->dropdown->selected = 0;
            pe->dropdown->w = boxw;
            pe->dropdown->h = OMM_PE_DROPDOWN_H + (pe->dropdown->count - 1) * OMM_PE_TEXT_H;
            pe->dropdown->x = boxx;
            pe->dropdown->y = OMM_PE_DROPDOWN_Y(y) - pe->dropdown->h;
            if (pe->dropdown->y < 0) {
                pe->dropdown->y = y + OMM_PE_HOVER_BOX_H;
            }
            if (pe->group == 0) {
                pe->subgroup = group;
            }
            omm_palette_editor_set_current();
        }
    }
}

//
// Update
//

static void omm_palette_editor_update_colormaps() {
    if (pe->group == 0) {
        s32 numGroups = omm_palette_editor_light_groups_count(pe->peach, pe->emblem);
        s32 y0 = OMM_PE_TEXT_COLOR_1_Y - numGroups * OMM_PE_TEXT_H - 4;
        s32 h = (174 - (numGroups + 2) * OMM_PE_TEXT_H) / 2;
        pe->colormaps[0]->x = OMM_PE_COLOR_MAP_DEFAULT_X;
        pe->colormaps[0]->y = y0 - h;
        pe->colormaps[0]->w = OMM_PE_COLOR_MAP_DEFAULT_W;
        pe->colormaps[0]->h = h;
        pe->colormaps[1]->x = OMM_PE_COLOR_MAP_DEFAULT_X;
        pe->colormaps[1]->y = y0 - OMM_PE_TEXT_H - 4 - 2 * h;
        pe->colormaps[1]->w = OMM_PE_COLOR_MAP_DEFAULT_W;
        pe->colormaps[1]->h = h;
    } else {
        pe->colormaps[0]->x = OMM_PE_COLOR_MAP_DEFAULT_X;
        pe->colormaps[0]->y = OMM_PE_COLOR_MAP_1_DEFAULT_Y;
        pe->colormaps[0]->w = OMM_PE_COLOR_MAP_DEFAULT_W;
        pe->colormaps[0]->h = OMM_PE_COLOR_MAP_DEFAULT_H;
        pe->colormaps[1]->x = OMM_PE_COLOR_MAP_DEFAULT_X;
        pe->colormaps[1]->y = OMM_PE_COLOR_MAP_2_DEFAULT_Y;
        pe->colormaps[1]->w = OMM_PE_COLOR_MAP_DEFAULT_W;
        pe->colormaps[1]->h = OMM_PE_COLOR_MAP_DEFAULT_H;
    }
}

static void omm_palette_editor_update_items() {
    mem_zero(pe->items->items, sizeof(pe->items->items));
    if (pe->group == 0) {
        s32 numGroups = omm_palette_editor_light_groups_count(pe->peach, pe->emblem);
        omm_palette_editor_set_item(0, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_PLAYER_Y);
        omm_palette_editor_set_item(1, 0, OMM_PE_ITEM_X_0, OMM_PE_TEXT_PALETTE_Y);
        omm_palette_editor_set_item(1, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_PALETTE_Y);
        omm_palette_editor_set_item(2, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_GROUP_Y);
        for (s32 group = 0; group != numGroups; ++group) {
            s32 y = OMM_PE_TEXT_COLOR_1_Y - group * OMM_PE_TEXT_H;
            omm_palette_editor_set_item(3 + group, 0, OMM_PE_ITEM_X_0, y);
            omm_palette_editor_set_item(3 + group, 1, OMM_PE_ITEM_X_1, y);
            omm_palette_editor_set_item(3 + group, 2, OMM_PE_ITEM_X_2, y);
        }
        omm_palette_editor_set_item(3 + numGroups, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_BACKGROUND_Y);
        omm_palette_editor_set_item(4 + numGroups, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_CAP_STATE_Y);
        omm_palette_editor_set_item(5 + numGroups, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_ANIMATIONS_Y);
        pe->items->count = 6 + numGroups;
    } else {
        omm_palette_editor_set_item(0, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_PLAYER_Y);
        omm_palette_editor_set_item(1, 0, OMM_PE_ITEM_X_0, OMM_PE_TEXT_PALETTE_Y);
        omm_palette_editor_set_item(1, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_PALETTE_Y);
        omm_palette_editor_set_item(2, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_GROUP_Y);
        omm_palette_editor_set_item(3, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_COLOR_1_Y);
        omm_palette_editor_set_item(4, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_COLOR_2_Y);
        omm_palette_editor_set_item(5, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_BACKGROUND_Y);
        omm_palette_editor_set_item(6, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_CAP_STATE_Y);
        omm_palette_editor_set_item(7, 1, OMM_PE_ITEM_X_2, OMM_PE_TEXT_ANIMATIONS_Y);
        pe->items->count = 8;
    }
}

static void omm_palette_editor_update_mouse() {
    s32 mx, my;
    u32 ms = SDL_GetMouseState(&mx, &my);
    if (mx != pe->mouse->wx || my != pe->mouse->wy) {
        pe->cursor->x = X_WINDOW_TO_SCREEN(mx);
        pe->cursor->y = Y_WINDOW_TO_SCREEN((s32) gfx_current_dimensions.height - my);
        pe->mouse->wx = mx;
        pe->mouse->wy = my;
        pe->cursor->hindex = -1;
        pe->cursor->vindex = -1;
    }
    pe->mouse->pressed = ms & (ms ^ pe->mouse->down);
    pe->mouse->down = ms;
}

static void omm_palette_editor_update_controller() {

    // Select next item
    if (pe->event == OMM_PE_EVENT_NONE && (OMM_PE_INPUT_UP_PRESSED || OMM_PE_INPUT_DOWN_PRESSED || OMM_PE_INPUT_LEFT_PRESSED || OMM_PE_INPUT_RIGHT_PRESSED)) {
        if (OMM_PE_INPUT_UP_PRESSED) {
            pe->cursor->hindex = max_s(0, pe->cursor->hindex);
            pe->cursor->vindex = (max_s(0, pe->cursor->vindex) + pe->items->count - 1) % pe->items->count;
        } else if (OMM_PE_INPUT_DOWN_PRESSED) {
            pe->cursor->hindex = max_s(0, pe->cursor->hindex);
            pe->cursor->vindex = (pe->cursor->vindex + 1) % pe->items->count;
        } else if (OMM_PE_INPUT_LEFT_PRESSED) {
            pe->cursor->vindex = max_s(0, pe->cursor->vindex);
            pe->cursor->hindex = (pe->cursor->hindex + 2) % 3;
        } else if (OMM_PE_INPUT_RIGHT_PRESSED) {
            pe->cursor->vindex = max_s(0, pe->cursor->vindex);
            pe->cursor->hindex = (pe->cursor->hindex + 1) % 3;
        }
        while (!pe->items->items[pe->cursor->vindex][pe->cursor->hindex][0]) {
            pe->cursor->hindex = (pe->cursor->hindex + 1) % 3;
        }
        return;
    }

    // Move cursor, unselect item
    if (!pe->mouse->pressed && !pe->mouse->down && (gPlayer1Controller->stickX != 0.f || gPlayer1Controller->stickY != 0.f)) {
        f32 dx = gPlayer1Controller->stickX / 64.f;
        f32 dy = gPlayer1Controller->stickY / 64.f;
        pe->cursor->x = clamp_f(pe->cursor->x + dx * (OMM_PE_INPUT_X_DOWN ? 16.f : 4.f), GFX_DIMENSIONS_FROM_LEFT_EDGE(0), GFX_DIMENSIONS_FROM_RIGHT_EDGE(0));
        pe->cursor->y = clamp_f(pe->cursor->y + dy * (OMM_PE_INPUT_X_DOWN ? 16.f : 4.f), 0, SCREEN_HEIGHT - 1);
        pe->cursor->hindex = -1;
        pe->cursor->vindex = -1;
        return;
    }

    // Move cursor to selected item
    if (pe->event == OMM_PE_EVENT_NONE && pe->cursor->hindex != -1 && pe->cursor->vindex != -1) {
        pe->cursor->x = pe->items->items[pe->cursor->vindex][pe->cursor->hindex][0];
        pe->cursor->y = OMM_PE_HOVER_BOX_Y(pe->items->items[pe->cursor->vindex][pe->cursor->hindex][1]) + OMM_PE_HOVER_BOX_H / 2.f;
        return;
    }
}

static void omm_palette_editor_update_hand() {
    pe->cursor->down = OMM_PE_INPUT_A_DOWN;
    pe->hover->x = 0;
    pe->hover->y = 0;
    pe->hover->w = 0;
    pe->hover->h = 0;
}

static void omm_palette_editor_update_brightness_mode() {
    if (OMM_PE_INPUT_Z_PRESSED) {
        pe->bright = !pe->bright;
    }
}

static void omm_palette_editor_move_camera() {
    if (pe->event == OMM_PE_EVENT_NONE) {
        if (OMM_PE_INPUT_A_PRESSED && pe->cursor->x >= GFX_DIMENSIONS_FROM_LEFT_EDGE(OMM_PE_DISPLAY_W + 1)) {
            pe->cursor->cx = pe->cursor->x;
            pe->cursor->cy = pe->cursor->y;
            pe->event = OMM_PE_EVENT_MOVING_CAMERA;
        }
    } else if (pe->event == OMM_PE_EVENT_MOVING_CAMERA) {
        if (pe->cursor->down) {
            pe->yaw += (pe->cursor->x - pe->cursor->cx) * 0x100;
            pe->pitch -= (pe->cursor->y - pe->cursor->cy) * 0x100;
            pe->cursor->cx = pe->cursor->x;
            pe->cursor->cy = pe->cursor->y;
        } else {
            pe->event = OMM_PE_EVENT_NONE;
        }
    }
    pe->yaw += (abs_s(gPlayer1Controller->extStickX) > 32) * gPlayer1Controller->extStickX * 0x10;
    pe->pitch -= (abs_s(gPlayer1Controller->extStickY) > 32) * gPlayer1Controller->extStickY * 0x10;
}

static void omm_palette_editor_zoom_in_out() {
    if (OMM_PE_INPUT_ZOOM_IN) {
        pe->zoom = clamp_s(pe->zoom + 1, 1, 60);
    } else if (OMM_PE_INPUT_ZOOM_OUT) {
        pe->zoom = clamp_s(pe->zoom - 1, 1, 60);
    } else {
        pe->zoom = clamp_s(pe->zoom - 2 * gOmmGlobals->mouseWheelY, 1, 60);
    }
}

static void omm_palette_editor_copy_palette() {
    s32 boxx = (OMM_PE_TEXT_X - OMM_PE_HOVER_BOX_MARGIN);
    s32 boxy = OMM_PE_HOVER_BOX_Y(OMM_PE_TEXT_PALETTE_Y);
    s32 boxw = (2 * OMM_PE_HOVER_BOX_MARGIN + OMM_PE_CHAR_WIDTH * (OMM_PE_STR_MAX_LENGTH_LEFT - 1));
    s32 boxh = OMM_PE_HOVER_BOX_H;
    if (pe->event == OMM_PE_EVENT_COPY_PALETTE) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (OMM_PE_INPUT_UP_PRESSED || OMM_PE_INPUT_DOWN_PRESSED || OMM_PE_INPUT_LEFT_PRESSED || OMM_PE_INPUT_RIGHT_PRESSED) {
            if (pe->dropdown->selected != -1) {
                if (OMM_PE_INPUT_UP_PRESSED) pe->dropdown->selected = (pe->dropdown->selected + pe->dropdown->count - 1) % pe->dropdown->count;
                if (OMM_PE_INPUT_DOWN_PRESSED) pe->dropdown->selected = (pe->dropdown->selected + 1) % pe->dropdown->count;
                if (OMM_PE_INPUT_LEFT_PRESSED) pe->dropdown->selected = (pe->dropdown->selected + pe->dropdown->count / 2) % pe->dropdown->count;
                if (OMM_PE_INPUT_RIGHT_PRESSED) pe->dropdown->selected = (pe->dropdown->selected + pe->dropdown->count / 2) % pe->dropdown->count;
            } else {
                if (OMM_PE_INPUT_UP_PRESSED) pe->dropdown->selected = pe->dropdown->count - 1;
                if (OMM_PE_INPUT_DOWN_PRESSED) pe->dropdown->selected = 0;
                if (OMM_PE_INPUT_LEFT_PRESSED) pe->dropdown->selected = pe->dropdown->count / 2;
                if (OMM_PE_INPUT_RIGHT_PRESSED) pe->dropdown->selected = 0;
            }
            s32 i = pe->dropdown->selected / (pe->dropdown->count / 2);
            s32 j = pe->dropdown->selected % (pe->dropdown->count / 2);
            s32 hx = OMM_PE_PALETTES_HOVER_X(i);
            s32 hy = OMM_PE_PALETTES_HOVER_Y(j);
            s32 hw = OMM_PE_PALETTES_HOVER_W;
            s32 hh = OMM_PE_PALETTES_HOVER_H;
            pe->cursor->x = hx + hw * 0.9f;
            pe->cursor->y = hy + hh / 2.f;
        }
        pe->dropdown->selected = -1;
        for (s32 palette = 0; palette != pe->dropdown->count; ++palette) {
            s32 i = palette / (pe->dropdown->count / 2);
            s32 j = palette % (pe->dropdown->count / 2);
            s32 hx = OMM_PE_PALETTES_HOVER_X(i);
            s32 hy = OMM_PE_PALETTES_HOVER_Y(j);
            s32 hw = OMM_PE_PALETTES_HOVER_W;
            s32 hh = OMM_PE_PALETTES_HOVER_H;
            if (omm_palette_editor_is_inside_box(pe->cursor->x, pe->cursor->y, hx, hy, hw, hh)) {
                pe->dropdown->selected = palette;
                break;
            }
        }
        if (OMM_PE_INPUT_A_PRESSED || OMM_PE_INPUT_B_PRESSED) {
            gPlayer1Controller->buttonPressed &= ~A_BUTTON;
            pe->mouse->pressed &= ~SDL_BUTTON_LMASK;
            pe->event = OMM_PE_EVENT_NONE;
            if (!OMM_PE_INPUT_B_PRESSED && pe->dropdown->selected >= 0 && pe->dropdown->selected < pe->dropdown->count) {
                for (s32 i = 0; i != omm_palette_editor_lights_count(pe->peach, pe->emblem); ++i) {
                    mem_cpy(
                        omm_mario_colors_get_light(pe->peach, pe->dropdown->count - 4 + pe->palette, i),
                        omm_mario_colors_get_light(pe->peach, pe->dropdown->selected, i),
                        sizeof(u32)
                    );
                }
                omm_palette_editor_set_current();
            }
        }
    } else if (pe->event == OMM_PE_EVENT_NONE && omm_palette_editor_is_inside_box(pe->cursor->x, pe->cursor->y, boxx, boxy, boxw, boxh)) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (OMM_PE_INPUT_A_PRESSED) {
            pe->event = OMM_PE_EVENT_COPY_PALETTE;
            pe->dropdown->selected = -1;
            pe->dropdown->count = omm_mario_colors_count();
        }
    }
}

static void omm_palette_editor_pick_color() {
    if (pe->event == OMM_PE_EVENT_NONE || pe->event->pickingColor) {
        if (pe->cursor->down && (pe->event != OMM_PE_EVENT_NONE || OMM_PE_INPUT_A_PRESSED)) {
            f32 u, v, w; s32 k = omm_palette_editor_get_pointed_color(&u, &v, &w);
            if (k != -1) {
                pe->event = k ? OMM_PE_EVENT_PICKING_COLOR_2 : OMM_PE_EVENT_PICKING_COLOR_1;
                pe->lights[k]->u = u;
                pe->lights[k]->v = v;
                pe->lights[k]->w = w;
                omm_palette_editor_update_current();
            }
        } else {
            pe->event = OMM_PE_EVENT_NONE;
        }
    }
}

static void omm_palette_editor_update_player() {
    if (OMM_REWARD_IS_PLAYABLE_PEACH_UNLOCKED && omm_palette_editor_update_toggle(OMM_PE_TEXT_PLAYER_Y, &pe->peach)) {
        omm_palette_editor_set_current();
    }
}

static void omm_palette_editor_update_palette() {
    for (s32 i = 0; i != 4; ++i) {
        OMM_PALETTE_EDITOR_PALETTE_CHOICES[i] = omm_mario_colors_choices(pe->peach)[omm_mario_colors_count() - 4 + i];
    }
    if (omm_palette_editor_update_dropdown(OMM_PE_EVENT_DROPDOWN_PALETTE, OMM_PE_TEXT_PALETTE_Y, &pe->palette, OMM_PALETTE_EDITOR_PALETTE_CHOICES, 4)) {
        omm_palette_editor_set_current();
    }
}

static void omm_palette_editor_update_group() {
    s32 numGroups = omm_palette_editor_light_groups_count(pe->peach, pe->emblem);
    for (s32 i = 0; i != numGroups; ++i) {
        OMM_PALETTE_EDITOR_GROUP_CHOICES[1 + i] = omm_mario_colors_light_group_name(pe->peach, i);
    }
    OMM_PALETTE_EDITOR_GROUP_CHOICES[0] = OMM_TEXT_PE_ALL;
    if (omm_palette_editor_update_dropdown(OMM_PE_EVENT_DROPDOWN_GROUP, OMM_PE_TEXT_GROUP_Y, &pe->group, OMM_PALETTE_EDITOR_GROUP_CHOICES, numGroups + 1)) {
        omm_palette_editor_set_current();
    }
}

static void omm_palette_editor_update_color_inputs() {
    if (pe->group == 0) {
        s32 numGroups = omm_palette_editor_light_groups_count(pe->peach, pe->emblem);
        for (s32 group = 0; group != numGroups; ++group) {
            s32 y = OMM_PE_TEXT_COLOR_1_Y - group * OMM_PE_TEXT_H;

            // Select group
            if (omm_palette_editor_select_group(group, OMM_PE_HOVER_BOX_X_0, y, OMM_PE_HOVER_BOX_W_0, OMM_PE_HOVER_BOX_H)) {
                omm_palette_editor_update_current();
                return;
            }

            // Edit group color 1
            omm_palette_editor_update_color_dropdown(
                OMM_PE_EVENT_DROPDOWN_COLOR_1, OMM_PE_EVENT_INPUT_COLOR_1, group,
                OMM_PE_HOVER_BOX_X_1, y, OMM_PE_HOVER_BOX_W_1, OMM_PE_HOVER_BOX_H,
                OMM_PALETTE_EDITOR_COLOR_CHOICES, array_length(OMM_PALETTE_EDITOR_COLOR_CHOICES)
            );

            // Edit group color 2
            omm_palette_editor_update_color_dropdown(
                OMM_PE_EVENT_DROPDOWN_COLOR_2, OMM_PE_EVENT_INPUT_COLOR_2, group,
                OMM_PE_HOVER_BOX_X_2, y, OMM_PE_HOVER_BOX_W_2, OMM_PE_HOVER_BOX_H,
                OMM_PALETTE_EDITOR_COLOR_CHOICES, array_length(OMM_PALETTE_EDITOR_COLOR_CHOICES)
            );
        }
    } else {

        // Edit current group color 1
        omm_palette_editor_update_color_dropdown(
            OMM_PE_EVENT_DROPDOWN_COLOR_1, OMM_PE_EVENT_INPUT_COLOR_1, pe->group,
            OMM_PE_HOVER_BOX_X_1, OMM_PE_TEXT_COLOR_1_Y, OMM_PE_HOVER_BOX_W_1_2, OMM_PE_HOVER_BOX_H,
            OMM_PALETTE_EDITOR_COLOR_CHOICES, array_length(OMM_PALETTE_EDITOR_COLOR_CHOICES)
        );

        // Edit current group color 2
        omm_palette_editor_update_color_dropdown(
            OMM_PE_EVENT_DROPDOWN_COLOR_2, OMM_PE_EVENT_INPUT_COLOR_2, pe->group,
            OMM_PE_HOVER_BOX_X_1, OMM_PE_TEXT_COLOR_2_Y, OMM_PE_HOVER_BOX_W_1_2, OMM_PE_HOVER_BOX_H,
            OMM_PALETTE_EDITOR_COLOR_CHOICES, array_length(OMM_PALETTE_EDITOR_COLOR_CHOICES)
        );
    }
}

static void omm_palette_editor_update_background() {
    omm_palette_editor_update_dropdown(OMM_PE_EVENT_DROPDOWN_BACKGROUND, OMM_PE_TEXT_BACKGROUND_Y, &pe->skybox, OMM_PALETTE_EDITOR_BACKGROUND_CHOICES, array_length(OMM_PALETTE_EDITOR_BACKGROUND_CHOICES));
}

static void omm_palette_editor_update_capless() {
    omm_palette_editor_update_dropdown(OMM_PE_EVENT_DROPDOWN_CAP_STATE, OMM_PE_TEXT_CAP_STATE_Y, &pe->cap, OMM_PALETTE_EDITOR_CAP_CHOICES, array_length(OMM_PALETTE_EDITOR_CAP_CHOICES));
}

static void omm_palette_editor_update_animations() {
    omm_palette_editor_update_toggle(OMM_PE_TEXT_ANIMATIONS_Y, &pe->anims);
}

static bool omm_palette_editor_check_save_button() {
    u8 *textSave = omm_text_convert(OMM_TEXT_PE_SAVE, false);
    s32 textSaveLen = omm_text_length(textSave);
    s32 boxx = OMM_PE_SAVE_BUTTON_X + 2;
    s32 boxy = OMM_PE_SAVE_BUTTON_Y + 2;
    s32 boxw = OMM_PE_SAVE_BUTTON_W - 4;
    s32 boxh = OMM_PE_SAVE_BUTTON_H - 4;
    if (pe->event == OMM_PE_EVENT_NONE && omm_palette_editor_is_inside_box(pe->cursor->x, pe->cursor->y, boxx, boxy, boxw, boxh)) {
        pe->hover->x = boxx;
        pe->hover->y = boxy;
        pe->hover->w = boxw;
        pe->hover->h = boxh;
        if (OMM_PE_INPUT_A_PRESSED) {
            return true;
        }
    }
    return false;
}

static s32 omm_level_palette_editor_update(UNUSED s32 arg, UNUSED s32 unused) {
    if (pe->state == OMM_PE_STATE_OPEN) {

        // Quit the palette editor by pressing Start
        if (OMM_PE_INPUT_SAVE_AND_EXIT) {
            omm_palette_editor_close();
            return 0;
        }

        // Update values
        omm_palette_editor_update_colormaps();
        omm_palette_editor_update_items();

        // Update inputs
        omm_palette_editor_update_mouse();
        omm_palette_editor_update_controller();
        omm_palette_editor_update_hand();

        // Quit the palette editor by clicking the Save button
        if (omm_palette_editor_check_save_button()) {
            omm_palette_editor_close();
            return 0;
        }

        // Update view
        omm_palette_editor_update_brightness_mode();
        omm_palette_editor_move_camera();
        omm_palette_editor_zoom_in_out();

        // Update parameters
        omm_palette_editor_copy_palette();
        omm_palette_editor_pick_color();
        omm_palette_editor_update_player();
        omm_palette_editor_update_palette();
        omm_palette_editor_update_group();
        omm_palette_editor_update_color_inputs();
        omm_palette_editor_update_background();
        omm_palette_editor_update_capless();
        omm_palette_editor_update_animations();

        // Update the floating Mario object
        struct Object *mario = obj_get_first_with_behavior(bhvStaticObject);
        if (mario) {
            struct MarioState *m = gMarioState;
            m->marioObj = gMarioObject = mario;

            // Set attributes
            obj_scale(mario, MARIO_BASE_SCALE * (pe->zoom / 20.f));
            obj_set_xyz(mario, OMM_PE_DISPLAY_W * 1.5f, -100, 0);
            obj_set_angle(mario, pe->pitch, pe->yaw, 0);
            obj_set_always_rendered(mario, true);
            obj_update_gfx(mario);
            vec3f_transform(mario->oGfxPos, (Vec3f) { 0, -50, 0 }, &mario->oPosX, mario->oGfxAngle, mario->oGfxScale);

            // Update animation
            switch (m->actionState) {
                case 0: obj_anim_play(mario, MARIO_ANIM_IDLE_HEAD_LEFT, 1.f); break;
                case 1: obj_anim_play(mario, MARIO_ANIM_IDLE_HEAD_RIGHT, 1.f); break;
                case 2: obj_anim_play(mario, MARIO_ANIM_IDLE_HEAD_CENTER, 1.f); break;
            }
            obj_anim_set_frame(mario, m->actionTimer);
            m->actionTimer += pe->anims;
            if (obj_anim_is_at_end(mario)) {
                m->actionState = (m->actionState + 1) % 3;
                m->actionTimer = 0;
            }

            // Cap state
            const s16 *capBodyState = OMM_PALETTE_EDITOR_CAP_BODY_STATES[pe->cap];
            struct MarioBodyState *bodyState = m->marioBodyState;
            bodyState->capState = capBodyState[0];
            bodyState->modelState = capBodyState[1];
            bodyState->eyeState = MARIO_EYES_OPEN;
            bodyState->handState = MARIO_HAND_FISTS;

            // Mario/Peach
            if (pe->peach) {
                mario->oGraphNode = gLoadedGraphNodes[MODEL_PEACH_OMM] = geo_layout_to_graph_node(NULL, omm_geo_peach);
            } else {
                mario->oGraphNode = gLoadedGraphNodes[MODEL_MARIO] = geo_layout_to_graph_node(NULL, mario_geo);
            }
            m->marioObj = gMarioObject = NULL;
            geo_preprocess_object_graph_node(mario);
        }
    }
    return 0;
}

OMM_ROUTINE_UPDATE(omm_palette_editor_update) {
    if (pe->state && omm_is_transition_active()) {
        gPlayer1Controller->buttonPressed = 0;
        gPlayer2Controller->buttonPressed = 0;
        gPlayer3Controller->buttonPressed = 0;
        gPlayer1Controller->buttonDown = 0;
        gPlayer2Controller->buttonDown = 0;
        gPlayer3Controller->buttonDown = 0;
    }
}

//
// Level script
//

const LevelScript omm_level_palette_editor[] = {
    CALL(0, omm_level_palette_editor_init),
    INIT_LEVEL(),
    ALLOC_LEVEL_POOL(),
    LOAD_MODEL_FROM_GEO(MODEL_MARIO, mario_geo),
    AREA(1, omm_geo_palette_editor),
        OBJECT(MODEL_MARIO, 0, 0, 0, 0, 0, 0, 0, bhvStaticObject),
        TERRAIN(main_menu_seg7_collision),
        SET_MENU_MUSIC(OMM_SEQ_PALETTE_EDITOR),
    END_AREA(),
    FREE_LEVEL_POOL(),
    LOAD_AREA(1),
    TRANSITION(WARP_TRANSITION_FADE_FROM_COLOR, 0x10, 0xFF, 0xFF, 0xFF),
    CALL_LOOP(0, omm_level_palette_editor_update),
};

//
// Public
//

void omm_palette_editor_open() {
    play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
    play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 30, 0xFF, 0xFF, 0xFF);
    pe->state = OMM_PE_STATE_OPENING;
}

bool omm_palette_editor_is_opening() {
    return pe->state == OMM_PE_STATE_OPENING;
}

void omm_palette_editor_set_open() {
#if OMM_GFX_API_DX
    ShowCursor(FALSE);
#elif OMM_GFX_API_GL
    SDL_ShowCursor(SDL_DISABLE);
#endif
    pe->state = OMM_PE_STATE_OPEN;
}

bool omm_palette_editor_is_open() {
    return pe->state == OMM_PE_STATE_OPEN;
}

void omm_palette_editor_close() {
    play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
    play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 30, 0x00, 0x00, 0x00);
    pe->state = OMM_PE_STATE_CLOSING;
    omm_save_file_do_save();
}

bool omm_palette_editor_is_closing() {
    return pe->state == OMM_PE_STATE_CLOSING;
}

void omm_palette_editor_set_closed() {
    if (!configWindow.fullscreen) {
#if OMM_GFX_API_DX
        ShowCursor(TRUE);
#elif OMM_GFX_API_GL
        SDL_ShowCursor(SDL_ENABLE);
#endif
    }
    pe->state = OMM_PE_STATE_CLOSED;
}

bool omm_palette_editor_is_closed() {
    return pe->state == OMM_PE_STATE_CLOSED;
}

s32 omm_palette_editor_get_current_palette() {
    if (pe->state == OMM_PE_STATE_OPEN || pe->state == OMM_PE_STATE_CLOSING) {
        return pe->palette;
    }
    return -1;
}

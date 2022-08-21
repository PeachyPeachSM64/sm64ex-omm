#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define MARIO_BASE_SCALE 2.f

#if WINDOWS_BUILD
#include <windows.h>
#define open_clipboard()                OpenClipboard(NULL)
#define get_clipboard_data()            GetClipboardData(CF_TEXT)
#define close_clipboard()               CloseClipboard()
#else
#define open_clipboard()                false
#define get_clipboard_data()            NULL
#define close_clipboard()
#endif

#define OMM_PE_SAVE_AND_EXIT            (gPlayer1Controller->buttonPressed & START_BUTTON)
#define OMM_PE_TOGGLE_INPUT_MODE        (gPlayer1Controller->buttonPressed & A_BUTTON)
#define OMM_PE_SWITCH_CHARACTER         (gPlayer1Controller->buttonPressed & Y_BUTTON)
#define OMM_PE_ENABLE_BRIGHTNESS_MODE   (gPlayer1Controller->buttonDown    & Z_TRIG)
#define OMM_PE_PREV_PALETTE             (gPlayer1Controller->buttonPressed & L_JPAD)
#define OMM_PE_NEXT_PALETTE             (gPlayer1Controller->buttonPressed & R_JPAD)
#define OMM_PE_PREV_COLOR               (gPlayer1Controller->buttonPressed & U_JPAD)
#define OMM_PE_NEXT_COLOR               (gPlayer1Controller->buttonPressed & D_JPAD)
#define OMM_PE_NEXT_SKYBOX              (gPlayer1Controller->buttonPressed & B_BUTTON)
#define OMM_PE_TOGGLE_CAP               (gPlayer1Controller->buttonPressed & X_BUTTON)
#define OMM_PE_ZOOM_IN                  (gPlayer1Controller->buttonDown    & R_TRIG)
#define OMM_PE_ZOOM_OUT                 (gPlayer1Controller->buttonDown    & L_TRIG)

#define OMM_PE_DISPLAY_X                (GFX_DIMENSIONS_FROM_LEFT_EDGE(1))
#define OMM_PE_DISPLAY_Y                (1)
#define OMM_PE_DISPLAY_W                (180)
#define OMM_PE_DISPLAY_H                (SCREEN_HEIGHT - 2)

#define OMM_PE_TEXT_X                   (OMM_PE_DISPLAY_X + 4)
#define OMM_PE_TEXT_X2                  (OMM_PE_DISPLAY_X + 60)
#define OMM_PE_TEXT_Y                   (SCREEN_HEIGHT - 13)
#define OMM_PE_TEXT_H                   (14)

#define OMM_PE_COLOR_BOX_X              (OMM_PE_DISPLAY_X + OMM_PE_DISPLAY_W - OMM_PE_COLOR_BOX_W - 3)
#define OMM_PE_COLOR_BOX_Y              (-2)
#define OMM_PE_COLOR_BOX_W              (48)
#define OMM_PE_COLOR_BOX_H              (12)

#define OMM_PE_COLOR_MAP_X              (OMM_PE_DISPLAY_X + 4)
#define OMM_PE_COLOR_MAP_Y              (OMM_PE_DISPLAY_Y + 4)
#define OMM_PE_COLOR_MAP_W              (OMM_PE_DISPLAY_W - 8)
#define OMM_PE_COLOR_MAP_H              (OMM_PE_COLOR_MAP_W)

#define OMM_PE_COLOR_PICKER_X(x)        (OMM_PE_COLOR_MAP_X + 1 - OMM_PE_COLOR_PICKER_W / 2 + (OMM_PE_COLOR_MAP_W - 2) * (x))
#define OMM_PE_COLOR_PICKER_Y(y)        (OMM_PE_COLOR_MAP_Y + 1 - OMM_PE_COLOR_PICKER_H / 2 + (OMM_PE_COLOR_MAP_H - 2) * (y))
#define OMM_PE_COLOR_PICKER_W           (4)
#define OMM_PE_COLOR_PICKER_H           (4)

//
// Palette editor
//

static struct {
    s32 peach;
    s32 capless;
    s32 palette;
    s32 color;
    s32 pitch;
    s32 yaw;
    s32 zoom;
    s32 skybox;
    s32 bright;
    s32 input;
    u32 *p;
    f32 r, g, b;
    f32 u, v, w;
} pe[1]; // sOmmPaletteEditor
s32 gOmmPaletteEditorState = OMM_PALETTE_EDITOR_STATE_CLOSED;

static void omm_palette_editor_rgb_to_uvw() {

    // Fill r, g, b
    f32 r = pe->r = ((*pe->p >> 16) & 0xFF) / 255.f;
    f32 g = pe->g = ((*pe->p >>  8) & 0xFF) / 255.f;
    f32 b = pe->b = ((*pe->p >>  0) & 0xFF) / 255.f;

    // Brightness (w)
    pe->w = max_3_f(r, g, b);
    if (pe->w > 0.f) {
        r /= pe->w;
        g /= pe->w;
        b /= pe->w;

        // Saturation (v)
        pe->v = min_3_f(r, g, b);
        if (pe->v < 1.f) {

            // Raw color
            r = invlerp_f(r, pe->v, 1);
            g = invlerp_f(g, pe->v, 1);
            b = invlerp_f(b, pe->v, 1);

            // Hue (u)
            f32 m = max_3_f(r, g, b);
            if      (m == r) pe->u = fmodf(g - b + 6, 6) / 6.f;
            else if (m == g) pe->u = fmodf(b - r + 2, 6) / 6.f;
            else if (m == b) pe->u = fmodf(r - g + 4, 6) / 6.f;
        } else {
            pe->u = 0;
        }
    } else {
        pe->u = 0;
        pe->v = 0;
    }
}

static void omm_palette_editor_uvw_to_rgb() {

    // Raw color (u)
    f32 f = fmodf(pe->u * 6.f, 6);
    f32 i = f - floorf(f);
    f32 j = 1.f - i;
    if      (f < 1.f) { pe->r = 1; pe->g = i; pe->b = 0; }
    else if (f < 2.f) { pe->r = j; pe->g = 1; pe->b = 0; }
    else if (f < 3.f) { pe->r = 0; pe->g = 1; pe->b = i; }
    else if (f < 4.f) { pe->r = 0; pe->g = j; pe->b = 1; }
    else if (f < 5.f) { pe->r = i; pe->g = 0; pe->b = 1; }
    else if (f < 6.f) { pe->r = 1; pe->g = 0; pe->b = j; }

    // Saturation (v)
    pe->r = lerp_f(pe->v, pe->r, 1);
    pe->g = lerp_f(pe->v, pe->g, 1);
    pe->b = lerp_f(pe->v, pe->b, 1);

    // Brightness (w)
    pe->r *= pe->w;
    pe->g *= pe->w;
    pe->b *= pe->w;

    // Update u32 color
    *pe->p = (
        (((u32) (clamp_0_1_f(pe->r) * 255.9f)) << 16) |
        (((u32) (clamp_0_1_f(pe->g) * 255.9f)) <<  8) |
        (((u32) (clamp_0_1_f(pe->b) * 255.9f)) <<  0)
    );
}

static void omm_palette_editor_set_current() {
    s32 palette = omm_mario_colors_count() - 4 + pe->palette;
    pe->color = clamp_s(pe->color, 0, omm_mario_colors_lights_count(pe->peach) - 1);
    pe->p = omm_mario_colors_get_light(pe->peach, palette, pe->color);
    omm_palette_editor_rgb_to_uvw();
}

static void omm_palette_editor_update_current() {
    omm_palette_editor_uvw_to_rgb();
}

static s32 omm_palette_editor_get_input_length() {
    for (s32 i = 0, input = pe->input; i != 6; ++i, input >>= 5) {
        if (input <= 0) {
            return i;
        }
    }
    return 6;
}

static u32 omm_palette_editor_get_input_color() {
    u32 color = 0;
    for (s32 i = 0, input = pe->input; i != 6 && input > 0; ++i, input >>= 5) {
        color |= ((input - 1) & 0x0F) << (4 * i);
    }
    return color;
}

static void omm_palette_editor_update_input(s32 number) {
    pe->input <<= 5;
    pe->input += (number + 1);
}

static void omm_palette_editor_update_input_from_clipboard() {
    if (open_clipboard()) {
        for (const char *p = (const char *) get_clipboard_data(); p && *p && omm_palette_editor_get_input_length() < 6; ++p) {
            switch (*p) {
                case '0': omm_palette_editor_update_input(0x0); break;
                case '1': omm_palette_editor_update_input(0x1); break;
                case '2': omm_palette_editor_update_input(0x2); break;
                case '3': omm_palette_editor_update_input(0x3); break;
                case '4': omm_palette_editor_update_input(0x4); break;
                case '5': omm_palette_editor_update_input(0x5); break;
                case '6': omm_palette_editor_update_input(0x6); break;
                case '7': omm_palette_editor_update_input(0x7); break;
                case '8': omm_palette_editor_update_input(0x8); break;
                case '9': omm_palette_editor_update_input(0x9); break;
                case 'a': omm_palette_editor_update_input(0xA); break;
                case 'b': omm_palette_editor_update_input(0xB); break;
                case 'c': omm_palette_editor_update_input(0xC); break;
                case 'd': omm_palette_editor_update_input(0xD); break;
                case 'e': omm_palette_editor_update_input(0xE); break;
                case 'f': omm_palette_editor_update_input(0xF); break;
                case 'A': omm_palette_editor_update_input(0xA); break;
                case 'B': omm_palette_editor_update_input(0xB); break;
                case 'C': omm_palette_editor_update_input(0xC); break;
                case 'D': omm_palette_editor_update_input(0xD); break;
                case 'E': omm_palette_editor_update_input(0xE); break;
                case 'F': omm_palette_editor_update_input(0xF); break;
            }
        }
        close_clipboard();
    }
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

static Gfx *omm_geo_palette_editor_render(s32 callContext, UNUSED struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER && gOmmPaletteEditorState == OMM_PALETTE_EDITOR_STATE_OPEN) {
        omm_render_texrect(OMM_PE_DISPLAY_X, OMM_PE_DISPLAY_Y, OMM_PE_DISPLAY_W, OMM_PE_DISPLAY_H, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0x00, 0x00, 0x00, 0x80, OMM_TEXTURE_WHITE, false);

        // Player
        u8 *textPlayer = omm_text_convert(OMM_TEXT_PLAYER, false);
        u8 *textPlayerName = omm_text_convert(omm_player_properties_get_name(pe->peach), false);
        u32 textPlayerColor = omm_player_properties_get_color(pe->peach);
        omm_render_string(OMM_PE_TEXT_X, OMM_PE_TEXT_Y, 0xFF, 0xFF, 0xFF, 0xFF, textPlayer, true);
        omm_render_string(OMM_PE_TEXT_X2, OMM_PE_TEXT_Y, (textPlayerColor >> 24) & 0xFF, (textPlayerColor >> 16) & 0xFF, (textPlayerColor >> 8) & 0xFF, 0xFF, textPlayerName, true);

        // Palette
        const char *palette_str = omm_mario_colors_choices(pe->peach)[omm_mario_colors_count() - 4 + pe->palette];
        u8 *textPalette = omm_text_convert(OMM_TEXT_PALETTE, false);
        u8 *textPaletteName = omm_text_convert(palette_str, false);
        omm_render_string(OMM_PE_TEXT_X, OMM_PE_TEXT_Y - OMM_PE_TEXT_H, 0xFF, 0xFF, 0xFF, 0xFF, textPalette, true);
        omm_render_string(OMM_PE_TEXT_X2, OMM_PE_TEXT_Y - OMM_PE_TEXT_H, 0xFF, 0xFF, 0xFF, 0xFF, textPaletteName, true);

        // Color
        u8 *textColor = omm_text_convert(OMM_TEXT_COLOR, false);
        u8 *textColorName = omm_text_convert(omm_mario_colors_get_light_name(pe->peach, pe->color), false);
        omm_render_string(OMM_PE_TEXT_X, OMM_PE_TEXT_Y - 2 * OMM_PE_TEXT_H, 0xFF, 0xFF, 0xFF, 0xFF, textColor, true);
        omm_render_string(OMM_PE_TEXT_X2, OMM_PE_TEXT_Y - 2 * OMM_PE_TEXT_H, 0xFF, 0xFF, 0xFF, 0xFF, textColorName, true);

        // Value
        u8 *textValue = omm_text_convert(OMM_TEXT_VALUE, false);
        omm_render_string(OMM_PE_TEXT_X, OMM_PE_TEXT_Y - 3 * OMM_PE_TEXT_H, 0xFF, 0xFF, 0xFF, 0xFF, textValue, true);
        omm_render_texrect(OMM_PE_COLOR_BOX_X, OMM_PE_TEXT_Y - 3 * OMM_PE_TEXT_H + OMM_PE_COLOR_BOX_Y, OMM_PE_COLOR_BOX_W, OMM_PE_COLOR_BOX_H, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_WHITE, false);
        if (pe->input != -1) {
            char value_str[16]; u32 color = omm_palette_editor_get_input_color();
            omm_render_texrect(OMM_PE_TEXT_X2 - 2, OMM_PE_TEXT_Y - 3 * OMM_PE_TEXT_H - 3, 46, 14, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0x00, 0x00, 0x00, 0xFF, OMM_TEXTURE_WHITE, false);
            omm_render_texrect(OMM_PE_TEXT_X2 - 1, OMM_PE_TEXT_Y - 3 * OMM_PE_TEXT_H - 2, 44, 12, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0xE0, 0xE0, 0xE0, 0xFF, OMM_TEXTURE_WHITE, false);
            switch (omm_palette_editor_get_input_length()) {
                case 0: snprintf(value_str, 16, " "); break;
                case 1: snprintf(value_str, 16, "%01X", color); break;
                case 2: snprintf(value_str, 16, "%02X", color); break;
                case 3: snprintf(value_str, 16, "%03X", color); break;
                case 4: snprintf(value_str, 16, "%04X", color); break;
                case 5: snprintf(value_str, 16, "%05X", color); break;
                case 6: snprintf(value_str, 16, "%06X", color); break;
            }
            u8 *textValueHex = omm_text_convert(value_str, false);
            omm_render_string(OMM_PE_TEXT_X2, OMM_PE_TEXT_Y - 3 * OMM_PE_TEXT_H, 0x00, 0x00, 0x00, 0xFF, textValueHex, false);
            omm_render_texrect(OMM_PE_COLOR_BOX_X + 1, OMM_PE_TEXT_Y - 3 * OMM_PE_TEXT_H + 1 + OMM_PE_COLOR_BOX_Y, OMM_PE_COLOR_BOX_W - 2, OMM_PE_COLOR_BOX_H - 2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, (color >> 16) & 0xFF, (color >> 8) & 0xFF, (color >> 0) & 0xFF, 0xFF, OMM_TEXTURE_WHITE, false);
        } else {
            omm_sprintf(value_str, 256, "%06X", *pe->p);
            u8 *textValueHex = omm_text_convert(value_str, false);
            omm_render_string(OMM_PE_TEXT_X2, OMM_PE_TEXT_Y - 3 * OMM_PE_TEXT_H, 0xFF, 0xFF, 0xFF, 0xFF, textValueHex, true);
            omm_render_texrect(OMM_PE_COLOR_BOX_X + 1, OMM_PE_TEXT_Y - 3 * OMM_PE_TEXT_H + 1 + OMM_PE_COLOR_BOX_Y, OMM_PE_COLOR_BOX_W - 2, OMM_PE_COLOR_BOX_H - 2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, pe->r * 0xFF, pe->g * 0xFF, pe->b * 0xFF, 0xFF, OMM_TEXTURE_WHITE, false);
        }

        // Color map
        f32 lum = 0.2126f * pe->r + 0.7152f * pe->g + 0.0722f * pe->b;
        u8 pickerColor = 0xFF * (lum < 0.8f);
        omm_render_texrect(OMM_PE_COLOR_MAP_X, OMM_PE_COLOR_MAP_Y, OMM_PE_COLOR_MAP_W, OMM_PE_COLOR_MAP_H, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_WHITE, false);
        if (pe->bright) {
            static Vtx vtx[4];
            s32 x0 = OMM_PE_COLOR_MAP_X + 1, x1 = OMM_PE_COLOR_MAP_X + OMM_PE_COLOR_MAP_W;
            s32 y0 = OMM_PE_COLOR_MAP_Y + 1, y1 = OMM_PE_COLOR_MAP_Y + OMM_PE_COLOR_MAP_H - 1;
            vtx[0] = (Vtx) { { { x0, y0, 0 }, 0, { (1024 * pe->u + 0) * 32, 8192 }, { 0x00, 0x00, 0x00, 0xFF } } };
            vtx[1] = (Vtx) { { { x1, y0, 0 }, 0, { (1024 * pe->u + 1) * 32, 8192 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
            vtx[2] = (Vtx) { { { x0, y1, 0 }, 0, { (1024 * pe->u + 0) * 32,    0 }, { 0x00, 0x00, 0x00, 0xFF } } };
            vtx[3] = (Vtx) { { { x1, y1, 0 }, 0, { (1024 * pe->u + 1) * 32,    0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING | G_CULL_BOTH);
            gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPLoadTextureBlock(gDisplayListHead++, OMM_TEXTURE_COLOR_MAP, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1024, 256, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, vtx, 4, 0);
            gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 2, 1, 3, 0);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
            gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
            omm_render_texrect(OMM_PE_COLOR_PICKER_X(pe->w), OMM_PE_COLOR_PICKER_Y(1 - pe->v), OMM_PE_COLOR_PICKER_W, OMM_PE_COLOR_PICKER_H, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, pickerColor, pickerColor, pickerColor, 0xFF, OMM_TEXTURE_COLOR_PICKER, false);
        } else {
            omm_render_texrect(OMM_PE_COLOR_MAP_X + 1, OMM_PE_COLOR_MAP_Y + 1, OMM_PE_COLOR_MAP_W - 2, OMM_PE_COLOR_MAP_H - 2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1024, 256, pe->w * 0xFF, pe->w * 0xFF, pe->w * 0xFF, 0xFF, OMM_TEXTURE_COLOR_MAP, false);
            omm_render_texrect(OMM_PE_COLOR_PICKER_X(pe->u), OMM_PE_COLOR_PICKER_Y(1 - pe->v), OMM_PE_COLOR_PICKER_W, OMM_PE_COLOR_PICKER_H, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, pickerColor, pickerColor, pickerColor, 0xFF, OMM_TEXTURE_COLOR_PICKER, false);
        }
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
// Level functions and script
//

static s32 omm_level_palette_editor_init(UNUSED s32 arg, UNUSED s32 unused) {
    gMarioState->unk00 = 0;
    gMarioState->flags = 0;
    gMarioState->action = 0;
    gMarioState->spawnInfo = &gPlayerSpawnInfos[0];
    gMarioState->statusForCamera = &gPlayerCameraState[0];
    gMarioState->marioBodyState = &gBodyStates[0];
    gMarioState->controller = &gControllers[0];
    gMarioState->unkB0 = -60;
    gMarioState->marioBodyState->capState = MARIO_HAS_DEFAULT_CAP_ON;
    gMarioState->marioBodyState->eyeState = MARIO_EYES_OPEN;
    gMarioState->marioBodyState->handState = MARIO_HAND_FISTS;
    gMarioAnimations = &gMarioAnimsData;
    pe->peach = OMM_PLAYER_IS_PEACH;
    pe->capless = false;
    pe->palette = 0;
    pe->color = 0;
    pe->pitch = 0;
    pe->yaw = 0;
    pe->zoom = 20;
    pe->skybox = BACKGROUND_BELOW_CLOUDS;
    pe->bright = false;
    pe->input = -1;
    omm_palette_editor_set_current();
    return 0;
}

static s32 omm_level_palette_editor_func(UNUSED s32 arg, UNUSED s32 unused) {
    return 0;
}

const LevelScript omm_level_palette_editor[] = {
    CALL(0, omm_level_palette_editor_init),
    INIT_LEVEL(),
    ALLOC_LEVEL_POOL(),
    LOAD_MODEL_FROM_GEO(MODEL_MARIO, mario_geo),
    AREA(1, omm_geo_palette_editor),
        OBJECT(MODEL_MARIO, 0, 0, 0, 0, 0, 0, 0, bhvStaticObject),
        TERRAIN(main_menu_seg7_collision),
        SET_MENU_MUSIC(OMM_PALETTE_EDITOR_MUSIC),
    END_AREA(),
    FREE_LEVEL_POOL(),
    LOAD_AREA(1),
    TRANSITION(WARP_TRANSITION_FADE_FROM_COLOR, 0x10, 0xFF, 0xFF, 0xFF),
    CALL_LOOP(0, omm_level_palette_editor_func),
};

//
// Update
//

OMM_ROUTINE_UPDATE(omm_palette_editor_update) {
    if (gOmmPaletteEditorState) {

        // Suppress inputs during transitions
        if (omm_is_transition_active()) {
            gPlayer1Controller->buttonPressed = 0;
            gPlayer2Controller->buttonPressed = 0;
            gPlayer3Controller->buttonPressed = 0;
            gPlayer1Controller->buttonDown = 0;
            gPlayer2Controller->buttonDown = 0;
            gPlayer3Controller->buttonDown = 0;
        }

        // Palette editor
        if (gOmmPaletteEditorState == OMM_PALETTE_EDITOR_STATE_OPEN) {

            // Quit the palette editor by pressing Start
            if (OMM_PE_SAVE_AND_EXIT) {
                omm_palette_editor_close();
                return;
            }

            // Enter a color code by pressing A
            if (OMM_PE_TOGGLE_INPUT_MODE) {
                pe->input = -1 * !(pe->input == -1);
            }
            
            // Input mode
            if (pe->input != -1) {
                static const u32 keyToNumber[][2] = {
                                              //   Q       A      N
                    { 11, 0x0 }, { 82, 0x0 }, //  [)]  |  [à]  | [0]
                    {  2, 0x1 }, { 79, 0x1 }, //  [!]  |  [&]  | [1]
                    {  3, 0x2 }, { 80, 0x2 }, //  [@]  |  [é]  | [2]
                    {  4, 0x3 }, { 81, 0x3 }, //  [#]  |  ["]  | [3]
                    {  5, 0x4 }, { 75, 0x4 }, //  [$]  |  [']  | [4]
                    {  6, 0x5 }, { 76, 0x5 }, //  [%]  |  [(]  | [5]
                    {  7, 0x6 }, { 77, 0x6 }, //  [^]  |  [-]  | [6]
                    {  8, 0x7 }, { 71, 0x7 }, //  [&]  |  [è]  | [7]
                    {  9, 0x8 }, { 72, 0x8 }, //  [*]  |  [_]  | [8]
                    { 10, 0x9 }, { 73, 0x9 }, //  [(]  |  [ç]  | [9]
                    { 16, 0xA }, { 30, 0xA }, // [Q/A] | [A/Q]
                    { 48, 0xB },              //  [B]  |  [B]
                    { 46, 0xC },              //  [C]  |  [C]
                    { 32, 0xD },              //  [D]  |  [D]
                    { 18, 0xE },              //  [E]  |  [E]
                    { 33, 0xF },              //  [F]  |  [F]
                    { 47, 0x10 },             // clipboard data
                };
                u32 keyPressed = controller_get_raw_key();
                for (s32 i = 0; i != omm_static_array_length(keyToNumber); ++i) {
                    if (keyToNumber[i][0] == keyPressed) {
                        if (keyToNumber[i][1] < 0x10) {
                            omm_palette_editor_update_input(keyToNumber[i][1]);
                        } else {
                            omm_palette_editor_update_input_from_clipboard();
                        }
                        if (omm_palette_editor_get_input_length() == 6) {
                            *pe->p = omm_palette_editor_get_input_color();
                            omm_palette_editor_set_current();
                            pe->input = -1;
                        }
                        break;
                    }
                }
            } else {

                // Switch character by pressing Y
                if (OMM_PE_SWITCH_CHARACTER) {
                    pe->peach = !pe->peach;
                    omm_palette_editor_set_current();
                }

                // Hold Z to change brightness instead of color
                if (OMM_PE_ENABLE_BRIGHTNESS_MODE) {
                    pe->bright = true;
                } else {
                    pe->bright = false;
                }
                
                // Change the palette with D-pad left and right
                if (OMM_PE_PREV_PALETTE) {
                    pe->palette = (pe->palette + 3) % 4;
                    omm_palette_editor_set_current();
                } else if (OMM_PE_NEXT_PALETTE) {
                    pe->palette = (pe->palette + 1) % 4;
                    omm_palette_editor_set_current();
                }

                // Change the current color with D-pad up and down
                if (OMM_PE_PREV_COLOR) {
                    pe->color = (pe->color + omm_mario_colors_lights_count(pe->peach) - 1) % omm_mario_colors_lights_count(pe->peach);
                    omm_palette_editor_set_current();
                } else if (OMM_PE_NEXT_COLOR) {
                    pe->color = (pe->color + 1) % omm_mario_colors_lights_count(pe->peach);
                    omm_palette_editor_set_current();
                }
                
                // Change the current color with the L-stick
                if (gPlayer1Controller->stickMag > 1.f) {
                    if (pe->bright) pe->w = clamp_0_1_f(pe->w + gPlayer1Controller->stickX / 2048.f);
                    else pe->u = clamp_0_1_f(pe->u + gPlayer1Controller->stickX / 2048.f);
                    pe->v = clamp_0_1_f(pe->v - gPlayer1Controller->stickY / 2048.f);
                    omm_palette_editor_update_current();
                }
                
                // Select a color with a mouse left-click
                else {
                    s32 mx, my;
                    u32 ms = SDL_GetMouseState(&mx, &my);
                    if (ms & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        f32 fx = mx / (f32) gfx_current_dimensions.width;
                        f32 fy = my / (f32) gfx_current_dimensions.height;
                        s32 sw = GFX_DIMENSIONS_FROM_RIGHT_EDGE(0) - GFX_DIMENSIONS_FROM_LEFT_EDGE(0);
                        s32 sh = SCREEN_HEIGHT;
                        f32 x = GFX_DIMENSIONS_FROM_LEFT_EDGE(fx * sw);
                        f32 y = (1 - fy) * sh;
                        f32 u = invlerp_f(x, OMM_PE_COLOR_MAP_X + 1, OMM_PE_COLOR_MAP_X + OMM_PE_COLOR_MAP_W - 1);
                        f32 v = invlerp_f(y, OMM_PE_COLOR_MAP_Y + 1, OMM_PE_COLOR_MAP_Y + OMM_PE_COLOR_MAP_H - 1);
                        if (u >= -0.02f && u <= 1.02f && v >= -0.02f && v <= 1.02f) {
                            pe->u = clamp_0_1_f(u);
                            pe->v = clamp_0_1_f(1 - v);
                            omm_palette_editor_update_current();
                        }
                    }
                }
            }

            // Change the skybox by pressing B
            if (OMM_PE_NEXT_SKYBOX) {
                pe->skybox = (pe->skybox + 1) % 10;
            }

            // Switch cap/capless by pressing X
            if (OMM_PE_TOGGLE_CAP) {
                pe->capless = !pe->capless;
            }

            // Zoom in/out by holding R or L
            if (OMM_PE_ZOOM_IN) {
                pe->zoom = clamp_s(pe->zoom + 1, 1, 60);
            } else if (OMM_PE_ZOOM_OUT) {
                pe->zoom = clamp_s(pe->zoom - 1, 1, 60);
            }

            // Rotate the camera with the R-stick
            if (abs_s(gPlayer1Controller->extStickX) > abs_s(gPlayer1Controller->extStickY)) {
                pe->yaw += gPlayer1Controller->extStickX * 0x10;
            } else {
                pe->pitch += gPlayer1Controller->extStickY * 0x10;
            }
            
            // Update the floating Mario object
            struct Object *mario = obj_get_first_with_behavior(bhvStaticObject);
            if (mario) {
                struct MarioState *m = gMarioState;
                m->marioObj = gMarioObject = mario;
                
                // Set attributes
                obj_scale(mario, MARIO_BASE_SCALE * (pe->zoom / 20.f));
                obj_set_pos(mario, OMM_PE_DISPLAY_W * 1.5f, -100, 0);
                obj_set_angle(mario, pe->pitch, pe->yaw, 0);
                obj_update_gfx(mario);

                // Update animation
                switch (m->actionState) {
                    case 0: obj_anim_play(mario, MARIO_ANIM_IDLE_HEAD_LEFT, 1.f); break;
                    case 1: obj_anim_play(mario, MARIO_ANIM_IDLE_HEAD_RIGHT, 1.f); break;
                    case 2: obj_anim_play(mario, MARIO_ANIM_IDLE_HEAD_CENTER, 1.f); break;
                }
                obj_anim_set_frame(mario, m->actionTimer++);
                if (obj_anim_is_at_end(mario)) {
                    m->actionState = (m->actionState + 1) % 3;
                    m->actionTimer = 0;
                }

                // Cap/Capless
                if (pe->capless) {
                    gMarioState->marioBodyState->capState = MARIO_HAS_DEFAULT_CAP_OFF;
                } else {
                    gMarioState->marioBodyState->capState = MARIO_HAS_DEFAULT_CAP_ON;
                }

                // Mario/Peach
                if (pe->peach) {
                    mario->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_peach);
                } else {
                    mario->oGraphNode = geo_layout_to_graph_node(NULL, mario_geo);
                }
                m->marioObj = gMarioObject = NULL;
                geo_preprocess_object_graph_node(mario);

                // Palette
                if (pe->peach) {
                    gOmmExtrasPeachColors = omm_mario_colors_count() - 4 + pe->palette;
                } else {
                    gOmmExtrasMarioColors = omm_mario_colors_count() - 4 + pe->palette;
                }
            }
        }
    }
}


//
// Public
//

void omm_palette_editor_open() {
    play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
    play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 30, 0xFF, 0xFF, 0xFF);
    gOmmPaletteEditorState = OMM_PALETTE_EDITOR_STATE_OPENING;
}

void omm_palette_editor_close() {
    play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
    play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 30, 0x00, 0x00, 0x00);
    gOmmPaletteEditorState = OMM_PALETTE_EDITOR_STATE_CLOSING;
    gSaveFileModified = true;
}

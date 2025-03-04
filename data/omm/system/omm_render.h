#ifndef OMM_RENDER_H
#define OMM_RENDER_H

#include "types.h"

// Fonts
extern const char *gOmmFontText[0x100];
extern const char *gOmmFontHud[0x100];

// HUD flags
#define HUD_DISPLAY_NONE                                    (0x0000)
#define HUD_DISPLAY_FLAG_COIN_COUNT                         (0x0002)
#define HUD_DISPLAY_FLAG_STAR_COUNT                         (0x0004)
#define HUD_DISPLAY_FLAG_CAMERA                             (0x0008)
#define HUD_DISPLAY_FLAG_KEYS                               (0x0010)
#define HUD_DISPLAY_FLAG_POWER_METER                        (0x0020)
#define HUD_DISPLAY_FLAG_TIMER                              (0x0040)
#define HUD_DISPLAY_FLAG_BREATH_METER                       (0x0080)
#define HUD_DISPLAY_FLAG_VIBE_GAUGE                         (0x0100)
#define HUD_DISPLAY_FLAG_CAP_TIMER                          (0x0200)
#define HUD_DISPLAY_FLAG_OBJECTS_RADAR                      (0x0400)
#define HUD_DISPLAY_FLAG_SPARKLY_COUNT                      (0x0800)
#define HUD_DISPLAY_FLAG_SPARKLY_OBJECTIVE                  (0x1000)
#define HUD_DISPLAY_FLAG_EFFECTS                            (0x2000)
#define HUD_DISPLAY_DEFAULT ( \
    HUD_DISPLAY_FLAG_COIN_COUNT | \
    HUD_DISPLAY_FLAG_STAR_COUNT | \
    HUD_DISPLAY_FLAG_CAMERA | \
    HUD_DISPLAY_FLAG_KEYS | \
    HUD_DISPLAY_FLAG_POWER_METER | \
    HUD_DISPLAY_FLAG_BREATH_METER | \
    HUD_DISPLAY_FLAG_VIBE_GAUGE | \
    HUD_DISPLAY_FLAG_CAP_TIMER | \
    HUD_DISPLAY_FLAG_OBJECTS_RADAR | \
    HUD_DISPLAY_FLAG_SPARKLY_COUNT | \
    HUD_DISPLAY_FLAG_SPARKLY_OBJECTIVE | \
    HUD_DISPLAY_FLAG_EFFECTS \
)

#define HUD_DISPLAY_HUD                                     (gHudDisplay.flags != HUD_DISPLAY_NONE)
#define HUD_DISPLAY_COIN_COUNT                              ((gHudDisplay.flags & HUD_DISPLAY_FLAG_COIN_COUNT) != 0)
#define HUD_DISPLAY_STAR_COUNT                              ((gHudDisplay.flags & HUD_DISPLAY_FLAG_STAR_COUNT) != 0)
#define HUD_DISPLAY_CAMERA                                  ((gHudDisplay.flags & HUD_DISPLAY_FLAG_CAMERA) != 0)
#define HUD_DISPLAY_KEYS                                    ((gHudDisplay.flags & HUD_DISPLAY_FLAG_KEYS) != 0)
#define HUD_DISPLAY_POWER_METER                             ((gHudDisplay.flags & HUD_DISPLAY_FLAG_POWER_METER) != 0)
#define HUD_DISPLAY_TIMER                                   ((gHudDisplay.flags & HUD_DISPLAY_FLAG_TIMER) != 0)
#define HUD_DISPLAY_BREATH_METER                            ((gHudDisplay.flags & HUD_DISPLAY_FLAG_BREATH_METER) != 0)
#define HUD_DISPLAY_VIBE_GAUGE                              ((gHudDisplay.flags & HUD_DISPLAY_FLAG_VIBE_GAUGE) != 0)
#define HUD_DISPLAY_CAP_TIMER                               ((gHudDisplay.flags & HUD_DISPLAY_FLAG_CAP_TIMER) != 0)
#define HUD_DISPLAY_OBJECTS_RADAR                           ((gHudDisplay.flags & HUD_DISPLAY_FLAG_OBJECTS_RADAR) != 0)
#define HUD_DISPLAY_SPARKLY_COUNT                           ((gHudDisplay.flags & HUD_DISPLAY_FLAG_SPARKLY_COUNT) != 0)
#define HUD_DISPLAY_SPARKLY_OBJECTIVE                       ((gHudDisplay.flags & HUD_DISPLAY_FLAG_SPARKLY_OBJECTIVE) != 0)
#define HUD_DISPLAY_EFFECTS                                 ((gHudDisplay.flags & HUD_DISPLAY_FLAG_EFFECTS) != 0)

// Screen/Window
#define X_WINDOW_TO_SCREEN(_x_)                             relerp_0_1_f(_x_, 0, gfx_current_dimensions.width, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), GFX_DIMENSIONS_FROM_RIGHT_EDGE(0))
#define X_SCREEN_TO_WINDOW(_x_)                             relerp_0_1_f(_x_, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), GFX_DIMENSIONS_FROM_RIGHT_EDGE(0), 0, gfx_current_dimensions.width)
#define Y_WINDOW_TO_SCREEN(_y_)                             relerp_0_1_f(_y_, 0, gfx_current_dimensions.height, 0, SCREEN_HEIGHT)
#define Y_SCREEN_TO_WINDOW(_y_)                             relerp_0_1_f(_y_, 0, SCREEN_HEIGHT, 0, gfx_current_dimensions.height)

// Commons
#define OMM_RENDER_DEFINE_GLYPH_SIZE(value)                 s32 _GLYPH_SIZE_ = (s32) (value)
#define OMM_INVSQRT2                                        (0.70710678118)
#define OMM_SCROLL_STICK_VALUE_MIN                          (56)
#define OMM_RENDER_GLYPH_SIZE                               (_GLYPH_SIZE_)
#define OMM_RENDER_GLYPH_SIZE_DEFAULT                       (10)
#define OMM_RENDER_LEFT_X                                   GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(12)
#define OMM_RENDER_RIGHT_X                                  GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(12)
#define OMM_RENDER_TOP_Y                                    (SCREEN_HEIGHT - 12)
#define OMM_RENDER_BOTTOM_Y                                 (12)
#define OMM_RENDER_NUMBER_OFFSET_X                          ((OMM_RENDER_GLYPH_SIZE * 3) / 4)
#define OMM_RENDER_STAR_OFFSET_X                            ((OMM_RENDER_GLYPH_SIZE * 9) / 8)
#define OMM_RENDER_OFFSET_Y                                 ((OMM_RENDER_GLYPH_SIZE * 5) / 4)
#define OMM_RENDER_ENABLE_ALPHA(p)                          gSPSetOtherMode(p, G_SETOTHERMODE_L, 18, 1, G_BL_A_MEM)
#define OMM_RENDER_BACKUP_DL_HEAD(p)                        Gfx *dlh = gDisplayListHead; gDisplayListHead = p;
#define OMM_RENDER_RESTORE_DL_HEAD(p)                       p = gDisplayListHead; gDisplayListHead = dlh;

// Values
#define OMM_RENDER_VALUE_GLYPH_X                            (OMM_RENDER_LEFT_X)
#define OMM_RENDER_VALUE_NUMBER_X                           (OMM_RENDER_VALUE_GLYPH_X + ((OMM_RENDER_GLYPH_SIZE * 5) / 4))
#define OMM_RENDER_VALUE_Y                                  (OMM_RENDER_TOP_Y - OMM_RENDER_GLYPH_SIZE)

// Coin sparkles (Vanishing HUD only)
#define OMM_RENDER_HUD_COIN_SPARKLE_SIZE                    ((OMM_RENDER_GLYPH_SIZE * 3) / 2)
#define OMM_RENDER_HUD_COIN_SPARKLE_X                       (OMM_RENDER_VALUE_GLYPH_X - ((OMM_RENDER_HUD_COIN_SPARKLE_SIZE - OMM_RENDER_GLYPH_SIZE) / 2))
#define OMM_RENDER_HUD_COIN_SPARKLE_Y                       (y - ((OMM_RENDER_HUD_COIN_SPARKLE_SIZE - OMM_RENDER_GLYPH_SIZE) / 2))
#define OMM_RENDER_HUD_COIN_SPARKLE_FRAMES                  (6)
#define OMM_RENDER_HUD_COIN_SPARKLE_DELAY                   (2)
#define OMM_RENDER_HUD_COIN_SPARKLE_COUNT                   (8)

// Collectibles
#define OMM_RENDER_COLLECTIBLE_X                            (OMM_RENDER_LEFT_X)
#define OMM_RENDER_COLLECTIBLE_Y                            (OMM_RENDER_BOTTOM_Y)

// Dark mode
#define OMM_RENDER_DARK_MODE_NUM_POINTS                     (256)
#define OMM_RENDER_DARK_MODE_RADIUS_MIN                     (0.f)
#define OMM_RENDER_DARK_MODE_RADIUS_MAX                     (0.4f)
#define OMM_RENDER_DARK_MODE_RADIUS_OUT                     (1.5f)
#define OMM_RENDER_DARK_MODE_HALF_WIDTH                     ((GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0)) / 2.f)
#define OMM_RENDER_DARK_MODE_HALF_HEIGHT                    (SCREEN_HEIGHT / 2.f)
#define OMM_RENDER_DARK_MODE_CENTER_X                       (SCREEN_WIDTH / 2.f)
#define OMM_RENDER_DARK_MODE_CENTER_Y                       (SCREEN_HEIGHT / 2.f)

// Power meter
#define OMM_RENDER_POWER_SIZE                               (OMM_RENDER_GLYPH_SIZE * 0.8f)
#define OMM_RENDER_POWER_X                                  (OMM_RENDER_RIGHT_X - OMM_RENDER_POWER_SEGMENT_RADIUS_3)
#define OMM_RENDER_POWER_Y                                  (OMM_RENDER_TOP_Y - OMM_RENDER_POWER_SEGMENT_RADIUS_3)
#define OMM_RENDER_POWER_ANIM_X                             (SCREEN_WIDTH * 0.6f)
#define OMM_RENDER_POWER_ANIM_Y                             (SCREEN_HEIGHT * 0.55f)
#define OMM_RENDER_POWER_BACKGROUND_NUM_TRIS                (32)
#define OMM_RENDER_POWER_BACKGROUND_R                       (0x40)
#define OMM_RENDER_POWER_BACKGROUND_G                       (0x40)
#define OMM_RENDER_POWER_BACKGROUND_B                       (0x40)
#define OMM_RENDER_POWER_SEGMENT_NUM_QUADS                  (16)
#define OMM_RENDER_POWER_SEGMENT_RADIUS_0                   (OMM_RENDER_POWER_SIZE * 1.125f)
#define OMM_RENDER_POWER_SEGMENT_RADIUS_1                   (OMM_RENDER_POWER_SIZE * 1.250f)
#define OMM_RENDER_POWER_SEGMENT_RADIUS_2                   (OMM_RENDER_POWER_SIZE * 1.375f)
#define OMM_RENDER_POWER_SEGMENT_RADIUS_3                   (OMM_RENDER_POWER_SIZE * 1.500f)
#define OMM_RENDER_POWER_SEGMENT_COLOR_R(index)             ((0xFFF00000 >> (8 * (index))) & 0xFF)
#define OMM_RENDER_POWER_SEGMENT_COLOR_G(index)             ((0x00F0E8E0 >> (8 * (index))) & 0xFF)
#define OMM_RENDER_POWER_SEGMENT_COLOR_B(index)             ((0x000000FF >> (8 * (index))) & 0xFF)
#define OMM_RENDER_POWER_SEGMENT_DELTA                      (OMM_RENDER_POWER_SIZE / 16.f)
#define OMM_RENDER_POWER_HEART_NUM_PIECES                   (32)
#define OMM_RENDER_POWER_HEART_RADIUS_1_X                   (OMM_RENDER_POWER_SIZE * 1.250f)
#define OMM_RENDER_POWER_HEART_RADIUS_1_Y                   (OMM_RENDER_POWER_SIZE * 1.125f)
#define OMM_RENDER_POWER_HEART_RADIUS_2_X                   (OMM_RENDER_POWER_SIZE * 1.375f)
#define OMM_RENDER_POWER_HEART_RADIUS_2_Y                   (OMM_RENDER_POWER_SIZE * 1.250f)
#define OMM_RENDER_POWER_HEART_COLOR_RED                    (Vec3s) { 0xFF, 0x3F, 0x3F }
#define OMM_RENDER_POWER_HEART_COLOR_WHITE                  (Vec3s) { 0xFF, 0xFF, 0xFF }
#define OMM_RENDER_POWER_HEART_PULSE(ticks)                 (((0x2100012 >> (4 * ((ticks) <= OMM_RENDER_POWER_TICKS_CRITICAL ? min_s(gGlobalTimer % 30, 6) : 0))) & 0xF) / 2.f)
#define OMM_RENDER_POWER_HEART_OFFSET_Y                     (OMM_RENDER_POWER_SIZE / 8.f)
#define OMM_RENDER_POWER_LIFE_UP_OFFSET_X                   (OMM_RENDER_POWER_SIZE / 2.f)
#define OMM_RENDER_POWER_NUMBER_OFFSET_X                    (OMM_RENDER_POWER_SIZE / 2)
#define OMM_RENDER_POWER_NUMBER_OFFSET_Y                    ((OMM_RENDER_POWER_SIZE * 11) / 16)
#define OMM_RENDER_POWER_FULL_SEGMENTS                      ((0x06060308 >> (8 * gOmmMovesetType)) & 0xFF) // 8,  3,  6,  6
#define OMM_RENDER_POWER_TICKS_PER_SEGMENT                  ((0x0A0A0A01 >> (8 * gOmmMovesetType)) & 0xFF) // 1, 10, 10, 10
#define OMM_RENDER_POWER_TICKS_MAX                          ((0x3C3C1E06 >> (8 * gOmmMovesetType)) & 0xFF) // 6, 30, 60, 60
#define OMM_RENDER_POWER_TICKS_NORMAL                       ((0x3C1E1E06 >> (8 * gOmmMovesetType)) & 0xFF) // 6, 30, 30, 60
#define OMM_RENDER_POWER_TICKS_LOW                          ((0x14141404 >> (8 * gOmmMovesetType)) & 0xFF) // 4, 20, 20, 20
#define OMM_RENDER_POWER_TICKS_CRITICAL                     ((0x0A0A0A02 >> (8 * gOmmMovesetType)) & 0xFF) // 2, 10, 10, 10
#define OMM_RENDER_POWER_TICKS_TO_SEGMENTS(ticks)           (((ticks) + OMM_RENDER_POWER_TICKS_PER_SEGMENT / 2) / OMM_RENDER_POWER_TICKS_PER_SEGMENT)

// Breath meter
#define OMM_RENDER_BREATH_NUM_QUADS                         (180)
#define OMM_RENDER_BREATH_COLOR_R                           ((i >= j) ? min_s((      (t) * 0x1FF), 0xFF) :          (0xFF))
#define OMM_RENDER_BREATH_COLOR_G                           ((i >= j) ? min_s(((1.f - t) * 0x1FF), 0xFF) : (!blink * 0xFF))
#define OMM_RENDER_BREATH_COLOR_B                           ((i >= j) ?                           (0x00) : (!blink * 0xFF))
#define OMM_RENDER_BREATH_RADIUS_IN                         (3.f)
#define OMM_RENDER_BREATH_RADIUS_OUT                        (5.f)

// Camera
#define OMM_RENDER_CAMERA_X                                 (OMM_RENDER_RIGHT_X - (2 * OMM_RENDER_GLYPH_SIZE))
#define OMM_RENDER_CAMERA_Y                                 (OMM_RENDER_BOTTOM_Y)
#define OMM_RENDER_CAMERA_OFFSET_X                          (OMM_RENDER_GLYPH_SIZE)
#define OMM_RENDER_CAMERA_OFFSET_Y                          ((OMM_RENDER_GLYPH_SIZE * 3) / 4)
#define OMM_RENDER_CAMERA_STEP_Y                            (OMM_RENDER_GLYPH_SIZE / 4)

// Single object radar
#define OMM_RENDER_OBJECT_RADAR_X                           (OMM_RENDER_LEFT_X + (OMM_RENDER_OBJECT_RADAR_RADIUS + OMM_RENDER_OBJECT_RADAR_ARROW_SIZE / 2))
#define OMM_RENDER_OBJECT_RADAR_Y                           (OMM_RENDER_BOTTOM_Y + (OMM_RENDER_OBJECT_RADAR_RADIUS + OMM_RENDER_OBJECT_RADAR_ARROW_SIZE / 2))
#define OMM_RENDER_OBJECT_RADAR_OBJ_SIZE                    (OMM_RENDER_GLYPH_SIZE)
#define OMM_RENDER_OBJECT_RADAR_COIN_SIZE                   (OMM_RENDER_GLYPH_SIZE)
#define OMM_RENDER_OBJECT_RADAR_SECRET_SIZE                 (OMM_RENDER_GLYPH_SIZE)
#define OMM_RENDER_OBJECT_RADAR_STAR_SIZE                   ((OMM_RENDER_GLYPH_SIZE * 5) / 4)
#define OMM_RENDER_OBJECT_RADAR_ARROW_SIZE                  ((OMM_RENDER_GLYPH_SIZE * 7) / 8)
#define OMM_RENDER_OBJECT_RADAR_RADIUS                      (((OMM_RENDER_OBJECT_RADAR_OBJ_SIZE + OMM_RENDER_OBJECT_RADAR_ARROW_SIZE) * 7) / 10)
#define OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_DURATION        (30)
#define OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_NEXT            (10)
#define OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_TOTAL           (OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_NEXT + OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_NEXT + OMM_RENDER_OBJECT_RADAR_TIMER_CYCLE_DURATION)

// Red coins
#define OMM_RENDER_RED_COINS_X                              (OMM_RENDER_RIGHT_X - (3 * OMM_RENDER_GLYPH_SIZE) - (4 * OMM_RENDER_NUMBER_OFFSET_X))
#define OMM_RENDER_RED_COINS_Y                              (OMM_RENDER_BOTTOM_Y)

// Timer
#define OMM_RENDER_TIMER_OFFSET_X                           (OMM_RENDER_NUMBER_OFFSET_X)
#define OMM_RENDER_TIMER_X                                  ((SCREEN_WIDTH - 7 * OMM_RENDER_TIMER_OFFSET_X) / 2)
#define OMM_RENDER_TIMER_Y                                  (OMM_RENDER_TOP_Y - OMM_RENDER_GLYPH_SIZE - (OMM_RENDER_OFFSET_Y / 2))

// Sparkly Stars timer
#define OMM_RENDER_SPARKLY_TIMER_GLYPH_SIZE                 (10)
#define OMM_RENDER_SPARKLY_TIMER_OFFSET_X(glyphSize)        (((glyphSize * 3) + 3) / 4)
#define OMM_RENDER_SPARKLY_TIMER_X(glyphSize)               ((SCREEN_WIDTH - 8 * OMM_RENDER_SPARKLY_TIMER_OFFSET_X(glyphSize)) / 2)
#define OMM_RENDER_SPARKLY_TIMER_Y                          (8)

// Pause (Course)
#define OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X                ((SCREEN_WIDTH * 2) / 5)
#define OMM_RENDER_PAUSE_COURSE_RIGHT_ALIGN_X               (OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X - 16)
#define OMM_RENDER_PAUSE_COURSE_OFFSET_Y                    (16)
#define OMM_RENDER_PAUSE_COURSE_OPTIONS_Y                   (SCREEN_HEIGHT / 2 - 16)
#define OMM_RENDER_PAUSE_COURSE_INFO_Y                      (OMM_RENDER_PAUSE_COURSE_OPTIONS_Y + OMM_RENDER_PAUSE_COURSE_OFFSET_Y * 5)

// Pause (Castle)
#define OMM_RENDER_PAUSE_CASTLE_PAUSE_Y                     (SCREEN_HEIGHT / 2)
#define OMM_RENDER_PAUSE_CASTLE_BOX_W                       (180)
#define OMM_RENDER_PAUSE_CASTLE_BOX_H                       (48)
#define OMM_RENDER_PAUSE_CASTLE_BOX_Y                       (SCREEN_HEIGHT / 4)
#define OMM_RENDER_PAUSE_CASTLE_BOX_TRIANGLE_UP_Y           (OMM_RENDER_PAUSE_CASTLE_BOX_Y + OMM_RENDER_PAUSE_CASTLE_BOX_H / 2 + 4)
#define OMM_RENDER_PAUSE_CASTLE_BOX_TRIANGLE_DOWN_Y         (OMM_RENDER_PAUSE_CASTLE_BOX_Y - OMM_RENDER_PAUSE_CASTLE_BOX_H / 2 - 4)
#define OMM_RENDER_PAUSE_CASTLE_BOX_TRIANGLE_LEFT_X         ((SCREEN_WIDTH - OMM_RENDER_PAUSE_CASTLE_BOX_W) / 2 - 4)
#define OMM_RENDER_PAUSE_CASTLE_BOX_TRIANGLE_RIGHT_X        ((SCREEN_WIDTH + OMM_RENDER_PAUSE_CASTLE_BOX_W) / 2 + 4)
#define OMM_RENDER_PAUSE_CASTLE_BOX_LINE_1_Y                (OMM_RENDER_PAUSE_CASTLE_BOX_Y + 10)
#define OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y                (OMM_RENDER_PAUSE_CASTLE_BOX_Y -  4)
#define OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y                (OMM_RENDER_PAUSE_CASTLE_BOX_Y - 18)

// Course complete
#define OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X             ((SCREEN_WIDTH * 2) / 5)
#define OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X            (OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X - 16)
#define OMM_RENDER_COURSE_COMPLETE_HI_SCORE_X               (OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X + ((OMM_RENDER_GLYPH_SIZE * 9) / 2))
#define OMM_RENDER_COURSE_COMPLETE_OFFSET_Y                 (16)

// Star select
#define OMM_RENDER_STAR_SELECT_BACKGROUND_BOTTOM_Y0         (0)
#define OMM_RENDER_STAR_SELECT_BACKGROUND_BOTTOM_Y1         (90)
#define OMM_RENDER_STAR_SELECT_BACKGROUND_TOP_Y0            (SCREEN_HEIGHT - 70)
#define OMM_RENDER_STAR_SELECT_BACKGROUND_TOP_Y1            (SCREEN_HEIGHT)
#define OMM_RENDER_STAR_SELECT_COURSE_NAME_Y                (SCREEN_HEIGHT - 56)
#define OMM_RENDER_STAR_SELECT_ACT_STAR_X                   (0)
#define OMM_RENDER_STAR_SELECT_ACT_STAR_Y                   (25)
#define OMM_RENDER_STAR_SELECT_ACT_STAR_Z                   (-300)
#define OMM_RENDER_STAR_SELECT_ACT_STAR_DX                  (300)
#define OMM_RENDER_STAR_SELECT_ACT_STAR_DZ                  (600)
#define OMM_RENDER_STAR_SELECT_ACT_NAME_Y                   (64)
#define OMM_RENDER_STAR_SELECT_SCORE_Y                      (24)
#define OMM_RENDER_STAR_SELECT_100_COINS_STAR_X             ((SCREEN_WIDTH / 2) + 44)
#define OMM_RENDER_STAR_SELECT_100_COINS_STAR_SIZE          (16)

// You got a Star!
#define OMM_RENDER_YOU_GOT_A_STAR_Y                         (32)
#define OMM_RENDER_YOU_GOT_A_STAR_W                         (200)
#define OMM_RENDER_YOU_GOT_A_STAR_H                         (64)
#define OMM_RENDER_YOU_GOT_A_STAR_LINE_1_Y                  (OMM_RENDER_YOU_GOT_A_STAR_Y + 46)
#define OMM_RENDER_YOU_GOT_A_STAR_LINE_2_Y                  (OMM_RENDER_YOU_GOT_A_STAR_Y + 28)
#define OMM_RENDER_YOU_GOT_A_STAR_LINE_3_Y                  (OMM_RENDER_YOU_GOT_A_STAR_Y + 10)

//
// Utils
//

typedef struct { s32 idx; s32 inc; s32 tmr; s32 val; } Scroll;
void omm_render_update_scroll(Scroll *scroll, s32 count, s16 stickValue);
void omm_render_create_dl_ortho_matrix();
void omm_render_shade_screen(u8 alpha);
void omm_render_texrect(s16 x, s16 y, s16 w, s16 h, s16 texw, s16 texh, u8 red, u8 green, u8 blue, u8 alpha, const void *texture, bool shadow);
void omm_render_rect_window_coords(f32 x, f32 y, f32 w, f32 h, u8 red, u8 green, u8 blue, u8 alpha);
void omm_render_number(s16 x, s16 y, s16 w, s16 h, s16 xStep, u8 alpha, s32 number, s32 digits, bool makeZerosTransparent, bool shadow);
void omm_render_string(s16 x, s16 y, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow);
void omm_render_string_sized(s16 x, s16 y, s16 w, s16 h, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow);
void omm_render_string_hud(s16 x, s16 y, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow);
s32  omm_render_get_char_width(u8 c);
s32  omm_render_get_char_width_hud(u8 c);
s32  omm_render_get_string_width(const u8 *str64);
s32  omm_render_get_string_width_sized(const u8 *str64, s16 w);
s32  omm_render_get_string_width_hud(const u8 *str64);
const void *omm_render_get_star_glyph(s32 index, MODE_INDEX s32 modeIndex, bool colored, bool collected);
const u8 *omm_render_get_star_rgb(s32 modeIndex, bool colored, bool collected);

#define omm_render_string_left_align(x, y, red, green, blue, alpha, str64, shadow)          omm_render_string(x, y, red, green, blue, alpha, str64, shadow)
#define omm_render_string_right_align(x, y, red, green, blue, alpha, str64, shadow)         omm_render_string(x - omm_render_get_string_width(str64), y, red, green, blue, alpha, str64, shadow)
#define omm_render_string_centered(y, red, green, blue, alpha, str64, shadow)               omm_render_string((SCREEN_WIDTH - omm_render_get_string_width(str64)) / 2, y, red, green, blue, alpha, str64, shadow)
#define omm_render_string_hud_centered(y, red, green, blue, alpha, str64, shadow)           omm_render_string_hud((SCREEN_WIDTH - omm_render_get_string_width_hud(str64)) / 2, y, red, green, blue, alpha, str64, shadow)
#define omm_render_glyph(x, y, w, h, red, green, blue, alpha, texture, shadow)              omm_render_texrect(x, y, w, h, 16, 16, red, green, blue, alpha, texture, shadow)
#define omm_render_glyph_hud(x, y, red, green, blue, alpha, texture, shadow)                omm_render_texrect(x, y, OMM_RENDER_GLYPH_SIZE, OMM_RENDER_GLYPH_SIZE, 16, 16, red, green, blue, alpha, texture, shadow)
#define omm_render_glyph_hud_small(x, y, red, green, blue, alpha, texture, shadow)          omm_render_texrect(x, y, OMM_RENDER_GLYPH_SIZE / 2, OMM_RENDER_GLYPH_SIZE / 2, 8, 8, red, green, blue, alpha, texture, shadow)
#define omm_render_number_hud(x, y, alpha, number, digits, makeZerosTransparent, shadow)    omm_render_number(x, y, OMM_RENDER_GLYPH_SIZE, OMM_RENDER_GLYPH_SIZE, OMM_RENDER_NUMBER_OFFSET_X, alpha, number, digits, makeZerosTransparent, shadow)
#define omm_render_counter(y, red, green, blue, alpha, glyph, count)                        { const u8 textX[] = { glyph, 0xFB, 0xFF }; str_t textCount; str_fmt(textCount, sizeof(textCount), "%d", count); omm_render_string_left_align((SCREEN_WIDTH / 2) - 2 - omm_render_get_string_width(textX), y, red, green, blue, alpha, textX, false); omm_render_string_left_align((SCREEN_WIDTH / 2) + 2, y, red, green, blue, alpha, omm_text_convert(textCount, false), false); }

//
// Effects
//

void omm_render_effect_freeze();
void omm_render_effect_dark_mode();
void omm_render_effect_vibe();
void omm_render_effect_you_got_a_star_begin(const char *title, const u8 *courseName, const u8 *starName);
void omm_render_effect_you_got_a_star_end();
void omm_render_effect_you_got_a_star_stop();
void omm_render_effect_you_got_a_star();

//
// HUD
//

void omm_render_hud_stars(s16 x, s16 y, s16 w, u8 alpha, s32 levelNum);
void omm_render_hud_coins(s16 x, s16 y, s16 w, u8 alpha, s32 coins);
void omm_render_hud();

//
// Pause
//

void omm_render_pause_init();
void omm_render_pause_course();
void omm_render_pause_castle();
s32  omm_render_pause();

//
// Course complete
//

void omm_render_course_complete_init();
s32  omm_render_course_complete();

//
// Cake
//

void omm_render_cake_ending_screen();
bool omm_render_cake_ending_screen_play_sound();

#endif

#ifndef OMM_RENDER_H
#define OMM_RENDER_H

// Commons
#define OMM_INVSQRT2                                        0.70710678118
#define OMM_HUD_DISPLAY_FLAG_POWER_UP                       HUD_DISPLAY_FLAG_STAR_COUNT
#define OMM_HUD_DISPLAY_FLAG_VIBE_GAUGE                     HUD_DISPLAY_FLAG_CAMERA_AND_POWER
#define OMM_HUD_DISPLAY_FLAG_O2_METER                       HUD_DISPLAY_FLAG_CAMERA_AND_POWER
#define OMM_HUD_DISPLAY_FLAG_RED_COINS_RADAR                HUD_DISPLAY_FLAG_STAR_COUNT
#define OMM_HUD_DISPLAY_FLAG_SPARKLY_STARS_CONDITIONS       HUD_DISPLAY_FLAG_STAR_COUNT
#define OMM_RENDER_GLYPH_SIZE                               (12)
#define OMM_RENDER_LEFT_X                                   GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(16)
#define OMM_RENDER_RIGHT_X                                  GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(16)
#define OMM_RENDER_TOP_Y                                    (SCREEN_HEIGHT - 16)
#define OMM_RENDER_BOTTOM_Y                                 (16)
#define OMM_RENDER_NUMBER_OFFSET_X                          ((OMM_RENDER_GLYPH_SIZE * 3) / 4)
#define OMM_RENDER_STAR_OFFSET_X                            ((OMM_RENDER_GLYPH_SIZE * 9) / 8)
#define OMM_RENDER_OFFSET_Y                                 ((OMM_RENDER_GLYPH_SIZE * 5) / 4)
#define OMM_RENDER_ENABLE_ALPHA(p)                          gSPSetOtherMode(p, G_SETOTHERMODE_L, 18, 1, G_BL_A_MEM)

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
#define OMM_RENDER_DARK_MODE_RADIUS_MAX                     (0.3f)
#define OMM_RENDER_DARK_MODE_RADIUS_OUT                     (1.5f)
#define OMM_RENDER_DARK_MODE_HALF_WIDTH                     ((GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0)) / 2.f)
#define OMM_RENDER_DARK_MODE_HALF_HEIGHT                    (SCREEN_HEIGHT / 2.f)
#define OMM_RENDER_DARK_MODE_CENTER_X                       (SCREEN_WIDTH / 2.f)
#define OMM_RENDER_DARK_MODE_CENTER_Y                       (SCREEN_HEIGHT / 2.f)

// Power meter
#define OMM_RENDER_POWER_X                                  (OMM_RENDER_RIGHT_X - ((OMM_RENDER_GLYPH_SIZE * 11) / 8))
#define OMM_RENDER_POWER_Y                                  (OMM_RENDER_TOP_Y - ((OMM_RENDER_GLYPH_SIZE * 11) / 8))
#define OMM_RENDER_POWER_BACKGROUND_NUM_TRIS                (64)
#define OMM_RENDER_POWER_BACKGROUND_R                       (0x30)
#define OMM_RENDER_POWER_BACKGROUND_G                       (0x30)
#define OMM_RENDER_POWER_BACKGROUND_B                       (0x30)
#define OMM_RENDER_POWER_SEGMENT_NUM_QUADS                  (32)
#define OMM_RENDER_POWER_SEGMENT_RADIUS_0                   (OMM_RENDER_GLYPH_SIZE * 1.000f)
#define OMM_RENDER_POWER_SEGMENT_RADIUS_1                   (OMM_RENDER_GLYPH_SIZE * 1.125f)
#define OMM_RENDER_POWER_SEGMENT_RADIUS_2                   (OMM_RENDER_GLYPH_SIZE * 1.250f)
#define OMM_RENDER_POWER_SEGMENT_RADIUS_3                   (OMM_RENDER_GLYPH_SIZE * 1.375f)
#define OMM_RENDER_POWER_SEGMENT_CENTER_R                   ((OMM_ARRAY_OF(const u8) { 0xFF, 0xFF, 0xFF, 0x00, 0x00 })[index])
#define OMM_RENDER_POWER_SEGMENT_CENTER_G                   ((OMM_ARRAY_OF(const u8) { 0xFF, 0x00, 0xC8, 0xE0, 0xC8 })[index])
#define OMM_RENDER_POWER_SEGMENT_CENTER_B                   ((OMM_ARRAY_OF(const u8) { 0xFF, 0x00, 0x00, 0x00, 0xFF })[index])
#define OMM_RENDER_POWER_SEGMENT_BORDER_R                   ((OMM_ARRAY_OF(const u8) { 0xFF, 0xD0, 0xD0, 0x00, 0x00 })[index])
#define OMM_RENDER_POWER_SEGMENT_BORDER_G                   ((OMM_ARRAY_OF(const u8) { 0xFF, 0x00, 0xA0, 0xB8, 0xA0 })[index])
#define OMM_RENDER_POWER_SEGMENT_BORDER_B                   ((OMM_ARRAY_OF(const u8) { 0xFF, 0x00, 0x00, 0x00, 0xD0 })[index])
#define OMM_RENDER_POWER_SEGMENT_DELTA                      (OMM_RENDER_GLYPH_SIZE / 16.f)
#define OMM_RENDER_POWER_HEART_NUM_PIECES                   (64)
#define OMM_RENDER_POWER_HEART_RADIUS_1_X                   (OMM_RENDER_GLYPH_SIZE * 1.250f)
#define OMM_RENDER_POWER_HEART_RADIUS_1_Y                   (OMM_RENDER_GLYPH_SIZE * 1.125f)
#define OMM_RENDER_POWER_HEART_RADIUS_2_X                   (OMM_RENDER_GLYPH_SIZE * 1.375f)
#define OMM_RENDER_POWER_HEART_RADIUS_2_Y                   (OMM_RENDER_GLYPH_SIZE * 1.250f)
#define OMM_RENDER_POWER_HEART_CENTER_R                     (0xFF)
#define OMM_RENDER_POWER_HEART_CENTER_G                     (0x3F + index * 0x60)
#define OMM_RENDER_POWER_HEART_CENTER_B                     (0x3F + index * 0x60)
#define OMM_RENDER_POWER_HEART_BORDER_R                     (0xD0)
#define OMM_RENDER_POWER_HEART_BORDER_G                     (0x30 + index * 0x50)
#define OMM_RENDER_POWER_HEART_BORDER_B                     (0x30 + index * 0x50)
#define OMM_RENDER_POWER_HEART_PULSE                        ((OMM_ARRAY_OF(const s32) { 2, 1, 0, 0, 0, 1, 2 })[hp < OMM_RENDER_POWER_HP_CRITICAL ? min_s(gGlobalTimer % 30, 6) : 0])
#define OMM_RENDER_POWER_HEART_OFFSET_Y                     (OMM_RENDER_GLYPH_SIZE / 8.f)
#define OMM_RENDER_POWER_LIFE_UP_OFFSET_X                   (OMM_RENDER_GLYPH_SIZE / 2.f)
#define OMM_RENDER_POWER_NUMBER_OFFSET_X                    (OMM_RENDER_GLYPH_SIZE / 2)
#define OMM_RENDER_POWER_NUMBER_OFFSET_Y                    ((OMM_RENDER_GLYPH_SIZE * 11) / 16)
#define OMM_RENDER_POWER_HP_FULL_SEGMENTS                   ((OMM_ARRAY_OF(const s32) { 8,  3,  6 })[gOmmMovesetType])
#define OMM_RENDER_POWER_HP_PER_SEGMENT                     ((OMM_ARRAY_OF(const s32) { 1, 10, 10 })[gOmmMovesetType])
#define OMM_RENDER_POWER_HP_NORMAL                          ((OMM_ARRAY_OF(const s32) { 7, 31, 61 })[gOmmMovesetType])
#define OMM_RENDER_POWER_HP_LOW                             ((OMM_ARRAY_OF(const s32) { 5, 25, 25 })[gOmmMovesetType])
#define OMM_RENDER_POWER_HP_CRITICAL                        ((OMM_ARRAY_OF(const s32) { 3, 15, 15 })[gOmmMovesetType])
#define OMM_RENDER_POWER_HP_VALUE                           ((hp + OMM_RENDER_POWER_HP_PER_SEGMENT / 2) / OMM_RENDER_POWER_HP_PER_SEGMENT)

// Oxygen meter
#define OMM_RENDER_O2_NUM_QUADS                             (180)
#define OMM_RENDER_O2_COLOR_R                               ((i >= j) ? min_s((      (t) * 0x1FF), 0xFF) :          (0xFF))
#define OMM_RENDER_O2_COLOR_G                               ((i >= j) ? min_s(((1.f - t) * 0x1FF), 0xFF) : (!blink * 0xFF))
#define OMM_RENDER_O2_COLOR_B                               ((i >= j) ?                               (0x00) : (!blink * 0xFF))
#define OMM_RENDER_O2_RADIUS_IN                             (3.f)
#define OMM_RENDER_O2_RADIUS_OUT                            (5.f)

// Camera
#define OMM_RENDER_CAMERA_X                                 (OMM_RENDER_RIGHT_X - (2 * OMM_RENDER_GLYPH_SIZE))
#define OMM_RENDER_CAMERA_Y                                 (19)
#define OMM_RENDER_CAMERA_OFFSET_X                          (OMM_RENDER_GLYPH_SIZE)
#define OMM_RENDER_CAMERA_OFFSET_Y                          ((OMM_RENDER_GLYPH_SIZE * 3) / 4)
#define OMM_RENDER_CAMERA_STEP_Y                            (OMM_RENDER_GLYPH_SIZE / 4)

// Red coins radar
#define OMM_RENDER_RADAR_X                                  (OMM_RENDER_LEFT_X + OMM_RENDER_RADAR_RADIUS + OMM_RENDER_RADAR_ARROW_SIZE / 2)
#define OMM_RENDER_RADAR_Y                                  (19 + OMM_RENDER_RADAR_RADIUS)
#define OMM_RENDER_RADAR_COIN_SIZE                          (OMM_RENDER_GLYPH_SIZE)
#define OMM_RENDER_RADAR_STAR_SIZE                          ((OMM_RENDER_GLYPH_SIZE * 5) / 4)
#define OMM_RENDER_RADAR_ARROW_SIZE                         ((OMM_RENDER_GLYPH_SIZE * 7) / 8)
#define OMM_RENDER_RADAR_RADIUS                             (((OMM_RENDER_RADAR_COIN_SIZE + OMM_RENDER_RADAR_ARROW_SIZE) * 7) / 10)

// Red coins
#define OMM_RENDER_RED_COINS_X                              (OMM_RENDER_RIGHT_X - (3 * OMM_RENDER_GLYPH_SIZE) - (4 * OMM_RENDER_NUMBER_OFFSET_X))
#define OMM_RENDER_RED_COINS_Y                              (19)

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
#define OMM_RENDER_PAUSE_CASTLE_BOX_LINE_1_Y                (OMM_RENDER_PAUSE_CASTLE_BOX_Y + 10)
#define OMM_RENDER_PAUSE_CASTLE_BOX_LINE_2_Y                (OMM_RENDER_PAUSE_CASTLE_BOX_Y -  4)
#define OMM_RENDER_PAUSE_CASTLE_BOX_LINE_3_Y                (OMM_RENDER_PAUSE_CASTLE_BOX_Y - 18)

// Course complete
#define OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X             ((SCREEN_WIDTH * 2) / 5)
#define OMM_RENDER_COURSE_COMPLETE_RIGHT_ALIGN_X            (OMM_RENDER_PAUSE_COURSE_LEFT_ALIGN_X - 16)
#define OMM_RENDER_COURSE_COMPLETE_HI_SCORE_X               (OMM_RENDER_COURSE_COMPLETE_LEFT_ALIGN_X + ((OMM_RENDER_GLYPH_SIZE * 9) / 2))
#define OMM_RENDER_COURSE_COMPLETE_OFFSET_Y                 (16)

// Star select
#define OMM_RENDER_STAR_SELECT_J                            (0x13)
#define OMM_RENDER_STAR_SELECT_Q                            (0x1A)
#define OMM_RENDER_STAR_SELECT_V                            (0x1F)
#define OMM_RENDER_STAR_SELECT_X                            (0x21)
#define OMM_RENDER_STAR_SELECT_Z                            (0x23)
#define OMM_RENDER_STAR_SELECT_SPACE                        (DIALOG_CHAR_SPACE)
#define OMM_RENDER_STAR_SELECT_COMMA                        (DIALOG_CHAR_COMMA)
#define OMM_RENDER_STAR_SELECT_HYPHEN                       (0x9F)
#define OMM_RENDER_STAR_SELECT_APOSTROPHE                   (0x3E)
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
void omm_render_texrect(s16 x, s16 y, s16 w, s16 h, s32 fmt, s32 siz, s16 texw, s16 texh, u8 red, u8 green, u8 blue, u8 alpha, const void *texture, bool shadow);
void omm_render_number(s16 x, s16 y, s16 w, s16 h, s16 xStep, u8 alpha, s32 number, s32 digits, bool makeZerosTransparent, bool shadow);
void omm_render_string(s16 x, s16 y, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow);
void omm_render_string_hud(s16 x, s16 y, u8 red, u8 green, u8 blue, u8 alpha, const u8 *str64, bool shadow);
s32  omm_render_get_char_width(u8 c);
s32  omm_render_get_char_width_hud(u8 c);
s32  omm_render_get_string_width(const u8 *str64);
s32  omm_render_get_string_width_hud(const u8 *str64);
u8  *omm_render_get_star_glyph(s32 index, bool colored);

#define omm_render_string_left_align(x, y, red, green, blue, alpha, str64, shadow)          omm_render_string(x, y, red, green, blue, alpha, str64, shadow)
#define omm_render_string_right_align(x, y, red, green, blue, alpha, str64, shadow)         omm_render_string(x - omm_render_get_string_width(str64), y, red, green, blue, alpha, str64, shadow)
#define omm_render_string_centered(y, red, green, blue, alpha, str64, shadow)               omm_render_string((SCREEN_WIDTH - omm_render_get_string_width(str64)) / 2, y, red, green, blue, alpha, str64, shadow)
#define omm_render_string_hud_centered(y, red, green, blue, alpha, str64, shadow)           omm_render_string_hud((SCREEN_WIDTH - omm_render_get_string_width_hud(str64)) / 2, y, red, green, blue, alpha, str64, shadow)
#define omm_render_glyph(x, y, w, h, red, green, blue, alpha, texture, shadow)              omm_render_texrect(x, y, w, h, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, red, green, blue, alpha, texture, shadow)
#define omm_render_glyph_hud(x, y, red, green, blue, alpha, texture, shadow)                omm_render_texrect(x, y, OMM_RENDER_GLYPH_SIZE, OMM_RENDER_GLYPH_SIZE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, red, green, blue, alpha, texture, shadow)
#define omm_render_glyph_hud_small(x, y, red, green, blue, alpha, texture, shadow)          omm_render_texrect(x, y, OMM_RENDER_GLYPH_SIZE / 2, OMM_RENDER_GLYPH_SIZE / 2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, red, green, blue, alpha, texture, shadow)
#define omm_render_number_hud(x, y, alpha, number, digits, makeZerosTransparent, shadow)    omm_render_number(x, y, OMM_RENDER_GLYPH_SIZE, OMM_RENDER_GLYPH_SIZE, OMM_RENDER_NUMBER_OFFSET_X, alpha, number, digits, makeZerosTransparent, shadow)
#define omm_render_counter(y, red, green, blue, alpha, glyph, count)                        { const u8 textX[] = { glyph, 0xFB, 0xFF }; OMM_STRING(textCount, 8, "%d", count); omm_render_string_left_align((SCREEN_WIDTH / 2) - 2 - omm_render_get_string_width(textX), y, red, green, blue, alpha, textX, false); omm_render_string_left_align((SCREEN_WIDTH / 2) + 2, y, red, green, blue, alpha, omm_text_convert(textCount, false), false); }

//
// Effects
//

void omm_render_effect_freeze();
void omm_render_effect_dark_mode();
void omm_render_effect_vibe();
void omm_render_effect_you_got_a_star_begin(const char *title, const u8 *courseName, const u8 *starName);
void omm_render_effect_you_got_a_star_end();
void omm_render_effect_you_got_a_star();

//
// HUD
//

bool omm_render_hud_stars(s16 x, s16 y, u8 alpha, s32 level, bool cond, bool shadow);
void omm_render_hud_coins(s16 x, s16 y, u8 alpha, s32 coins, bool shadow);
void omm_render_hud_camera(struct MarioState *m);
void omm_render_hud_power_meter(struct MarioState *m);
void omm_render_hud_o2_meter(struct MarioState *m);
void omm_render_hud_timer();
void omm_render_hud_cannon_reticle();
void omm_render_hud_values(struct MarioState *m);
void omm_render_hud_red_coins_radar(struct MarioState *m);
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

s32 omm_render_course_complete();

//
// Star select
//

s32  omm_render_star_select_get_string_width(const u8 *str64, s32 glyphSize, s32 spacing);
void omm_render_star_select_string(const u8 *str64, s32 x, s32 y, s32 glyphSize, s32 spacing, bool outline, bool shadow);
void omm_render_star_select_background_rect(s32 x0, s32 y0, u8 r0, u8 g0, u8 b0, u8 a0, s32 x1, s32 y1, u8 r1, u8 g1, u8 b1, u8 a1, bool vrt);
s32  omm_render_star_select_update(s32 action);

//
// Main menu
//

void omm_render_menu_update_inputs(struct Object *characterSelectButton, struct Object *sparklyStarsScore, f32 *cursorPos);
void omm_render_menu_update(f32 *cursorPos, s8 selectedButtonId, u8 alpha);
void omm_render_menu(struct Object *characterSelectButton, struct Object *sparklyStarsScore, u8 alpha);

#endif

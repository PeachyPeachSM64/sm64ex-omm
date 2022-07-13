#ifndef OMM_SPARKLY_H
#define OMM_SPARKLY_H

#include "data/omm/omm_includes.h"

#define gOmmSSM                                 gOmmData->mario->sparkly.mode
#define gOmmSSF                                 gOmmData->mario->sparkly.flags
#define gOmmSSE                                 gOmmData->mario->sparkly.ending
#define gOmmSSH                                 gOmmData->mario->sparkly.cheats
#define omm_ssf_set(flag, value)                OMM_SET_FLAG(gOmmSSF, flag, value)

typedef struct OmmSSD {
    s32 index, game, level, area;
    s32 starX, starY, starZ;
    s32 signX, signY, signZ, signA, showOnEntry;
    s32 coinX, coinY, coinZ, coinA, spawnCoinArrow;
    s32 timer;
    u64 flags;
} OmmSSD;
extern const OmmSSD gOmmSSD[4][30];

//
// Sparkly Stars constants
// OMM_SSX_* -> misc data
// OMM_SSS_* -> save data
// OMM_SSM_* -> modes
// OMM_SSE_* -> endings
// OMM_SSF_* -> OMM data flags
// OMM_SSD_* -> sparkly stars data
// OMM_SSC_* -> context
//

#define OMM_SSX_REQUIRED                        DEF(120, 50, 120, 120, 157, 130)
#define OMM_SSX_BLOCK_LEVEL                     DEF(LEVEL_GROUNDS, LEVEL_CASTLE, LEVEL_GROUNDS, LEVEL_GROUNDS, LEVEL_COURT, LEVEL_COURT)
#define OMM_SSX_BLOCK_AREA                      DEF(1, 1, 1, 1, 2, 1)
#define OMM_SSX_BLOCK_COUNT                     DEF(3, 1, 3, 3, 1, 1)
#define OMM_SSX_BLOCK_AVAILABLE                 DEF(0b1111, 0b1001, 0b1111, 0b1111, 0b1001, 0b1001)
#define OMM_SSX_BLOCK_X                         DEF(     0,      0,      0,      0,   6464,   -990)
#define OMM_SSX_BLOCK_Y                         DEF(   820,  -2780,    820,    820,   -920,    720)
#define OMM_SSX_BLOCK_Z                         DEF(   480,   1700,    480,    480,   6300,  -2340)
#define OMM_SSX_BLOCK_ANGLE                     DEF(0x4000, 0x0000, 0x4000, 0x4000, 0x0000, 0x0000)
#define OMM_SSX_BLOCK_GEO                       (OMM_ARRAY_OF(const GeoLayout *) { NULL, omm_geo_sparkly_star_1_block, omm_geo_sparkly_star_2_block, omm_geo_sparkly_star_3_block })
#define OMM_SSX_BLOCK_BHV                       (OMM_ARRAY_OF(const BehaviorScript *) { NULL, omm_bhv_sparkly_star_1_block, omm_bhv_sparkly_star_2_block, omm_bhv_sparkly_star_3_block })
#define OMM_SSX_STAR_GEO_OPAQUE                 (OMM_ARRAY_OF(const GeoLayout *) { NULL, omm_geo_sparkly_star_1_opaque, omm_geo_sparkly_star_2_opaque, omm_geo_sparkly_star_3_opaque })
#define OMM_SSX_STAR_GEO_TRANSPARENT            (OMM_ARRAY_OF(const GeoLayout *) { NULL, omm_geo_sparkly_star_1_transparent, omm_geo_sparkly_star_2_transparent, omm_geo_sparkly_star_3_transparent })
#define OMM_SSX_SPARKLE_GEO                     (OMM_ARRAY_OF(const GeoLayout *) { NULL, omm_geo_sparkly_star_1_sparkle, omm_geo_sparkly_star_2_sparkle, omm_geo_sparkly_star_3_sparkle })
#define OMM_SSX_HINT_GEO                        (OMM_ARRAY_OF(const GeoLayout *) { NULL, omm_geo_sparkly_star_1_hint, omm_geo_sparkly_star_2_hint, omm_geo_sparkly_star_3_hint })
#define OMM_SSX_TEXT_STAR                       (OMM_ARRAY_OF(const char *) { NULL, OMM_TEXT_SPARKLY_STAR_1, OMM_TEXT_SPARKLY_STAR_2, OMM_TEXT_SPARKLY_STAR_3 })
#define OMM_SSX_TEXT_STARS                      (OMM_ARRAY_OF(const char *) { NULL, OMM_TEXT_SPARKLY_STARS_1, OMM_TEXT_SPARKLY_STARS_2, OMM_TEXT_SPARKLY_STARS_3 })
#define OMM_SSX_HUD_GLYPH                       (OMM_ARRAY_OF(const char *) { NULL, OMM_TEXTURE_STAR_FULL_17, OMM_TEXTURE_STAR_FULL_18, OMM_TEXTURE_STAR_FULL_19 })
#define OMM_SSX_HUD_COLOR                       (OMM_ARRAY_OF(Vec3s) { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xE0, 0x80 }, { 0xC0, 0xE0, 0xFF }, { 0xFF, 0x40, 0x80 } })

#define OMM_SSM_DISABLED                        0
#define OMM_SSM_NORMAL                          1
#define OMM_SSM_HARD                            2
#define OMM_SSM_LUNATIC                         3
#define OMM_SSM_COUNT                           3

#define OMM_SSE_REGULAR                         0
#define OMM_SSE_BAD                             1
#define OMM_SSE_PEACH                           2

#define OMM_SSF_STAR_BLOCK                      (1 << 0)
#define OMM_SSF_GRAND_STAR                      (1 << 1)
#define OMM_SSF_GAME_PAUSED                     (1 << 2)
#define OMM_SSF_TIME_STOP                       (1 << 3)
#define OMM_SSF_TRANSITION                      (1 << 4)
#define OMM_SSF_MARIO_UPDATED                   (1 << 5)
#define OMM_SSF_CHEAT_DETECTED                  (1 << 6)

#define OMM_SSD_COINS                           0x00000000000000FFllu
#define OMM_SSD_RED_COINS                       0x000000000000FF00llu
#define OMM_SSD_BUTTONS                         0x00000000FFFF0000llu
#define OMM_SSD_MUSHROOMS                       0x0000000F00000000llu
#define OMM_SSD_CONDITION                       0x0000001000000000llu
#define OMM_SSD_ONLY_COIN_Y                     0x0000002000000000llu
#define OMM_SSD_ONLY_COIN_R                     0x0000004000000000llu
#define OMM_SSD_ONLY_COIN_B                     0x0000008000000000llu
#define OMM_SSD_NO_MUSHROOM                     0x0000010000000000llu
#define OMM_SSD_NO_CAPTURE                      0x0000020000000000llu
#define OMM_SSD_NO_VIBE                         0x0000040000000000llu
#define OMM_SSD_NO_VIBE_AUTO_REFILL             0x0000080000000000llu
#define OMM_SSD_WING_CAP                        0x0000100000000000llu
#define OMM_SSD_VANISH_CAP                      0x0000200000000000llu
#define OMM_SSD_METAL_CAP                       0x0000400000000000llu
#define OMM_SSD_SHARDS                          0x0000800000000000llu
#define OMM_SSD_DARK_MODE                       0x0001000000000000llu
#define OMM_SSD_FLAMES                          0x0002000000000000llu
#define OMM_SSD_BOXES                           0x0004000000000000llu
#define OMM_SSD_RINGS                           0x0008000000000000llu
#define OMM_SSD_GET_COINS(flags)                (((flags) & OMM_SSD_COINS)     >>  0llu)
#define OMM_SSD_GET_RED_COINS(flags)            (((flags) & OMM_SSD_RED_COINS) >>  8llu)
#define OMM_SSD_GET_BUTTONS(flags)              (((flags) & OMM_SSD_BUTTONS)   >> 16llu)
#define OMM_SSD_GET_MUSHROOMS(flags)            (((flags) & OMM_SSD_MUSHROOMS) >> 32llu)
#define OMM_SSD_SET_COINS(value)                ((((u64) value) <<  0llu) & OMM_SSD_COINS    )
#define OMM_SSD_SET_RED_COINS(value)            ((((u64) value) <<  8llu) & OMM_SSD_RED_COINS)
#define OMM_SSD_SET_BUTTONS(value)              ((((u64) value) << 16llu) & OMM_SSD_BUTTONS  )
#define OMM_SSD_SET_MUSHROOMS(value)            ((((u64) value) << 32llu) & OMM_SSD_MUSHROOMS)
#define OMM_SSD_CAPS                            (OMM_SSD_WING_CAP | OMM_SSD_VANISH_CAP | OMM_SSD_METAL_CAP)

#define OMM_SSC_OK                              0
#define OMM_SSC_FAIL                            1
#define OMM_SSC_INVALID                         2

#define OMM_SSC_F_INITED                        (1 << 0)
#define OMM_SSC_F_SUCCESSFUL                    (1 << 1)
#define OMM_SSC_F_STAR_SPAWNED                  (1 << 2)
#define OMM_SSC_F_COIN_ARROW_SPAWNED            (1 << 3)
#define OMM_SSC_F_OBJECTS_SPAWNED               (1 << 4)
#define OMM_SSC_F_CHEAT_DETECTED                (1 << 5)
#define OMM_SSC_F_ENTRY_DIALOG                  (1 << 6)
#define OMM_SSC_F_TIMER_STARTED                 (1 << 7)
#define OMM_SSC_F_EVENT_STARTED                 (1 << 8)
#define OMM_SSC_F_ZONES                         (1 << 9)

#define OMM_SSC_C_COIN_Y                        0
#define OMM_SSC_C_COIN_L                        1
#define OMM_SSC_C_COIN_R                        2
#define OMM_SSC_C_COIN_B                        3
#define OMM_SSC_C_MUSHROOM                      4
#define OMM_SSC_C_SECRET                        5
#define OMM_SSC_C_COUNT                         6

//
// Shortcuts
//

#define OMM_SSM_IS_DISABLED                     (gOmmSSM == OMM_SSM_DISABLED)
#define OMM_SSM_IS_ENABLED                      (gOmmSSM != OMM_SSM_DISABLED)
#define OMM_SSM_IS_NORMAL                       (gOmmSSM == OMM_SSM_NORMAL)
#define OMM_SSM_IS_HARD                         (gOmmSSM == OMM_SSM_HARD)
#define OMM_SSM_IS_LUNATIC                      (gOmmSSM == OMM_SSM_LUNATIC)

#define OMM_SSF_IS_STAR_BLOCK                   OMM_GET_FLAG(gOmmSSF, OMM_SSF_STAR_BLOCK)
#define OMM_SSF_IS_GRAND_STAR                   OMM_GET_FLAG(gOmmSSF, OMM_SSF_GRAND_STAR)
#define OMM_SSF_IS_GAME_PAUSED                  OMM_GET_FLAG(gOmmSSF, OMM_SSF_GAME_PAUSED)
#define OMM_SSF_IS_TIME_STOP                    OMM_GET_FLAG(gOmmSSF, OMM_SSF_TIME_STOP)
#define OMM_SSF_IS_TRANSITION                   OMM_GET_FLAG(gOmmSSF, OMM_SSF_TRANSITION)
#define OMM_SSF_IS_MARIO_UPDATED                OMM_GET_FLAG(gOmmSSF, OMM_SSF_MARIO_UPDATED)
#define OMM_SSF_IS_CHEAT_DETECTED               OMM_GET_FLAG(gOmmSSF, OMM_SSF_CHEAT_DETECTED)

#define OMM_SSC_IS_OK                           omm_ssc_state_ok()
#define OMM_SSC_IS_INITED                       omm_ssc_get(OMM_SSC_F_INITED)
#define OMM_SSC_IS_SUCCESSFUL                   omm_ssc_get(OMM_SSC_F_SUCCESSFUL)
#define OMM_SSC_IS_STAR_SPAWNED                 omm_ssc_get(OMM_SSC_F_STAR_SPAWNED)
#define OMM_SSC_IS_COIN_ARROW_SPAWNED           omm_ssc_get(OMM_SSC_F_COIN_ARROW_SPAWNED)
#define OMM_SSC_IS_OBJECTS_SPAWNED              omm_ssc_get(OMM_SSC_F_OBJECTS_SPAWNED)
#define OMM_SSC_IS_CHEAT_DETECTED               omm_ssc_get(OMM_SSC_F_CHEAT_DETECTED)
#define OMM_SSC_IS_ENTRY_DIALOG                 omm_ssc_get(OMM_SSC_F_ENTRY_DIALOG)
#define OMM_SSC_IS_TIMER_STARTED                omm_ssc_get(OMM_SSC_F_TIMER_STARTED)
#define OMM_SSC_IS_EVENT_STARTED                omm_ssc_get(OMM_SSC_F_EVENT_STARTED)

//
// Sparkly Stars functions
// omm_ssd_* -> data
// omm_ssc_* -> context
// omm_ssa_* -> actions
// omm_ssi_* -> interactions
// omm_sse_* -> endings
// omm_ssu_* -> update
//

bool omm_ssd_is_available(s32 mode);
bool omm_ssd_is_unlocked(s32 mode);
bool omm_ssd_is_selectible(s32 mode);
bool omm_ssd_is_selected(s32 mode);
bool omm_ssd_is_completed(s32 mode);
bool omm_ssd_is_timer_started(s32 mode);
bool omm_ssd_is_star_collected(s32 mode, s32 index);
bool omm_ssd_is_grand_star_collected(s32 mode);
bool omm_ssd_is_bowser_4_unlocked(s32 mode);
bool omm_ssd_is_bowser_4();

s32  omm_ssd_get_timer(s32 mode);
s32  omm_ssd_get_star_index(s32 mode, s32 level, s32 area);
s32  omm_ssd_get_star_level(s32 mode, s32 index);
s32  omm_ssd_get_star_area(s32 mode, s32 index);
u8  *omm_ssd_get_star_name(s32 mode, s32 index);
u8  *omm_ssd_get_star_level_name(s32 mode, s32 index);
s32  omm_ssd_get_star_count(s32 mode);

void omm_ssd_disable();
void omm_ssd_set_mode(s32 mode);
void omm_ssd_unlock_mode(s32 mode);
void omm_ssd_start_timer(s32 mode);
void omm_ssd_collect_star(s32 mode, s32 index);
void omm_ssd_collect_grand_star(s32 mode);
void omm_ssd_unlock_bowser_4(s32 mode);
void omm_ssd_clear_mode(s32 mode);
bool omm_ssd_read(const char **tokens);
void omm_ssd_write(char **buffer);
bool omm_ssd_check_cheats(struct MarioState *m);
void omm_ssd_turn_off_cheats();

bool omm_ssc_get(s32 flag);
void omm_ssc_set(s32 flag, bool cond);
void omm_ssc_clr(s32 flag);
s32  omm_ssc_cnt(s32 counter);
void omm_ssc_mov(s32 counter, s32 value);
void omm_ssc_inc(s32 counter, s32 inc);
s32  omm_ssc_dif(s32 counter);

bool omm_ssc_state_ok();
void omm_ssc_state_set(s32 state, bool sound);

s32  omm_ssc_data_flags(u64 flags);
s32  omm_ssc_data_flames();
s32  omm_ssc_data_boxes();
s32  omm_ssc_data_rings();
void omm_ssc_data_reset();

bool omm_ssc_check_star(struct Object *star);
void omm_ssc_spawn_star(struct MarioState *m);
void omm_ssc_spawn_sign(struct MarioState *m);
void omm_ssc_update_level(struct MarioState *m, const OmmSSD *data);
void omm_ssc_update(struct MarioState *m);
void omm_ssc_reset();

s32  omm_ssa_star_dance(struct MarioState *m);
s32  omm_ssa_end_cutscene_1(struct MarioState *m);
s32  omm_ssa_end_cutscene_2(struct MarioState *m);

bool omm_ssi_star(struct MarioState *m, struct Object *o);
void omm_ssi_grand_star(struct MarioState *m, struct Object *o);

void omm_sse_update_cutscene();
void omm_sse_render_screen();

#endif

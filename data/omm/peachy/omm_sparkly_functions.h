#ifndef OMM_SPARKLY_FUNCTIONS_H
#define OMM_SPARKLY_FUNCTIONS_H

#include "types.h"

#define OMM_SPARKLY_STATE_IS_OK         (gOmmSparklyContext->state == OMM_SPARKLY_STATE_OK)
#define OMM_SPARKLY_MODE_IS_ENABLED     (gOmmSparklyMode != OMM_SPARKLY_MODE_DISABLED)
#define OMM_SPARKLY_MODE_IS_NORMAL      (gOmmSparklyMode == OMM_SPARKLY_MODE_NORMAL)
#define OMM_SPARKLY_MODE_IS_HARD        (gOmmSparklyMode == OMM_SPARKLY_MODE_HARD)
#define OMM_SPARKLY_MODE_IS_LUNATIC     (gOmmSparklyMode == OMM_SPARKLY_MODE_LUNATIC)

s32  omm_sparkly_get_star_index(s32 sparklyMode, s32 levelNum, s32 areaIndex);
s32  omm_sparkly_get_star_index_in_level(s32 sparklyMode, s32 levelNum, s32 firstAreaIndex);
u8  *omm_sparkly_get_level_name(ustr_t dst, s32 sparklyMode, s32 starIndex);
u8  *omm_sparkly_get_star_name(ustr_t dst, s32 sparklyMode, s32 starIndex);
s32  omm_sparkly_get_collected_count(s32 sparklyMode);
s32  omm_sparkly_get_bowser_4_index(s32 sparklyMode);
s32  omm_sparkly_get_num_stars(s32 sparklyMode);

bool omm_sparkly_is_available(s32 sparklyMode);
bool omm_sparkly_is_unlocked(s32 sparklyMode);
bool omm_sparkly_is_selectible(s32 sparklyMode);
bool omm_sparkly_is_selected(s32 sparklyMode);
bool omm_sparkly_is_completed(s32 sparklyMode);
bool omm_sparkly_is_timer_started(s32 sparklyMode);
bool omm_sparkly_is_star_collected(s32 sparklyMode, s32 starIndex);
bool omm_sparkly_is_grand_star_collected(s32 sparklyMode);
bool omm_sparkly_is_bowser_4_unlocked(s32 sparklyMode);
bool omm_sparkly_is_bowser_4_battle();
s32  omm_sparkly_get_timer(s32 sparklyMode);

void omm_sparkly_disable();
void omm_sparkly_set_mode(s32 sparklyMode);
void omm_sparkly_unlock_mode(s32 sparklyMode);
void omm_sparkly_start_timer(s32 sparklyMode);
void omm_sparkly_collect_star(s32 sparklyMode, s32 starIndex);
void omm_sparkly_collect_grand_star(s32 sparklyMode);
void omm_sparkly_set_completed(s32 sparklyMode);
void omm_sparkly_unlock_bowser_4(s32 sparklyMode);
void omm_sparkly_clear_mode(s32 sparklyMode);
void omm_sparkly_clear_all();
bool omm_sparkly_read(const char *name, const char *value, bool *invalid);
void omm_sparkly_write();
bool omm_sparkly_are_regular_cheats_enabled(struct MarioState *m, bool skipIfCheatsDisabled);
bool omm_sparkly_check_cheats(struct MarioState *m);
void omm_sparkly_turn_off_cheats();

bool omm_sparkly_check_requirements(struct MarioState *m);
bool omm_sparkly_check_game_state(struct MarioState *m);
bool omm_sparkly_check_star_block(struct MarioState *m);
bool omm_sparkly_check_anti_cheat(struct MarioState *m);
bool omm_sparkly_check_bowser_4(struct MarioState *m);

void omm_sparkly_state_set(s32 state, bool sound);
s32  omm_sparkly_context_get_data(u64 flags);
s32  omm_sparkly_context_get_remaining_flames();
s32  omm_sparkly_context_get_remaining_boxes();
s32  omm_sparkly_context_get_remaining_star_rings();
s32  omm_sparkly_context_get_remaining_enemies();
void omm_sparkly_context_reset_data();
bool omm_sparkly_context_check_star(struct Object *star);
void omm_sparkly_context_spawn_star(struct MarioState *m);
void omm_sparkly_context_spawn_sign(struct MarioState *m);
void omm_sparkly_context_update_level(struct MarioState *m, const OmmSparklyData *data);
void omm_sparkly_context_update(struct MarioState *m);
void omm_sparkly_context_reset();

bool omm_sparkly_level__all_secrets(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__all_mushrooms(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__all_boxes(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__all_flames(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__eight_shards(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__eight_shards_and_lava(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__secrets_in_order(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__ten_goombas_stack(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__launch_cappy_at_target(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__dont_touch_floor(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__toads_on_pillars(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__toads_hide_and_seek(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__invalidate_if_not_flooded(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__invalidate_if_not_frozen(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__inc_timer_with_coins(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__shell_ride(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__koopa_the_quick_race(struct MarioState *m, const s32 *params);
bool omm_sparkly_level__all_star_rings(struct MarioState *m, const s32 *params);

bool omm_sparkly_level_jrb__flooded_frozen(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_ddd__switch_and_boxes(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_sl__snowmen_at_top(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_wdw__secret(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_wdw__star_box_red_coins(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_bitdw__goomba_and_coins(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_bitfs__six_secrets(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_bits__bobomb_and_enemies(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_bits__dark_all_star_rings(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_vcutm__five_corners(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_basement__catch_mips(struct MarioState *m, const s32 *params);
bool omm_sparkly_level_upstairs__toads_hide_and_seek(struct MarioState *m, const s32 *params);

s32  omm_sparkly_act_star_dance(struct MarioState *m);
s32  omm_sparkly_act_ending_1(struct MarioState *m);
s32  omm_sparkly_act_ending_2(struct MarioState *m);

bool omm_sparkly_interact_star(struct MarioState *m, struct Object *o);
void omm_sparkly_interact_grand_star(struct MarioState *m, struct Object *o);

void omm_sparkly_ending_dialog();
void omm_sparkly_ending_cutscene();

#endif
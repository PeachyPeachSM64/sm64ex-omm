#ifndef OMM_SPARKLY_FUNCTIONS_H
#define OMM_SPARKLY_FUNCTIONS_H

#include "types.h"

#define OMM_SPARKLY_STATE_IS_OK         (gOmmSparklyContext->state == OMM_SPARKLY_STATE_OK)
#define OMM_SPARKLY_MODE_IS_DISABLED    (gOmmSparklyMode == OMM_SPARKLY_MODE_DISABLED)
#define OMM_SPARKLY_MODE_IS_ENABLED     (gOmmSparklyMode != OMM_SPARKLY_MODE_DISABLED)
#define OMM_SPARKLY_MODE_IS_NORMAL      (gOmmSparklyMode == OMM_SPARKLY_MODE_NORMAL)
#define OMM_SPARKLY_MODE_IS_HARD        (gOmmSparklyMode == OMM_SPARKLY_MODE_HARD)
#define OMM_SPARKLY_MODE_IS_LUNATIC     (gOmmSparklyMode == OMM_SPARKLY_MODE_LUNATIC)
#define OMM_SPARKLY_LUNATIC_HEALTH      (OMM_SPARKLY_MODE_IS_LUNATIC && !omm_sparkly_is_completed(OMM_SPARKLY_MODE_LUNATIC))

s32  omm_sparkly_get_index(s32 mode, s32 level, s32 area);
u8  *omm_sparkly_get_level_name(s32 mode, s32 index);
u8  *omm_sparkly_get_star_name(s32 mode, s32 index);
s32  omm_sparkly_get_collected_count(s32 mode);
s32  omm_sparkly_get_bowser_4_index(s32 mode);

bool omm_sparkly_is_available(s32 mode);
bool omm_sparkly_is_unlocked(s32 mode);
bool omm_sparkly_is_selectible(s32 mode);
bool omm_sparkly_is_selected(s32 mode);
bool omm_sparkly_is_completed(s32 mode);
bool omm_sparkly_is_timer_started(s32 mode);
bool omm_sparkly_is_star_collected(s32 mode, s32 index);
bool omm_sparkly_is_grand_star_collected(s32 mode);
bool omm_sparkly_is_bowser_4_unlocked(s32 mode);
bool omm_sparkly_is_bowser_4_battle();
s32  omm_sparkly_get_timer(s32 mode);

void omm_sparkly_disable();
void omm_sparkly_set_mode(s32 mode);
void omm_sparkly_unlock_mode(s32 mode);
void omm_sparkly_start_timer(s32 mode);
void omm_sparkly_collect_star(s32 mode, s32 index);
void omm_sparkly_collect_grand_star(s32 mode);
void omm_sparkly_unlock_bowser_4(s32 mode);
void omm_sparkly_clear_mode(s32 mode);
bool omm_sparkly_read(const char **tokens);
void omm_sparkly_write(char **buffer);
bool omm_sparkly_check_cheats(struct MarioState *m);
void omm_sparkly_turn_off_cheats();

bool omm_sparkly_check_requirements(struct MarioState *m);
bool omm_sparkly_check_game_state(struct MarioState *m);
bool omm_sparkly_check_star_block(struct MarioState *m);
bool omm_sparkly_check_anti_cheat(struct MarioState *m);
bool omm_sparkly_check_bowser_4(struct MarioState *m);
void omm_sparkly_bowser_4_process_graph_node(struct Object *obj, bool enable, void (*geoFunc)(struct GraphNodeGenerated *));

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

s32  omm_sparkly_act_star_dance(struct MarioState *m);
s32  omm_sparkly_act_ending_1(struct MarioState *m);
s32  omm_sparkly_act_ending_2(struct MarioState *m);

bool omm_sparkly_interact_star(struct MarioState *m, struct Object *o);
void omm_sparkly_interact_grand_star(struct MarioState *m, struct Object *o);

void omm_sparkly_ending_cutscene();
void omm_sparkly_ending_screen();

#endif
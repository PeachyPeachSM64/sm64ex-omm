#ifndef OMM_MARIO_H
#define OMM_MARIO_H

#include "types.h"

#define HANG_STEP_NONE                          (0)
#define HANG_STEP_HIT_WALL                      (1)
#define HANG_STEP_LEFT_CEIL                     (2)

#define OMM_MARIO_ACTION_RESULT_BREAK           (0)
#define OMM_MARIO_ACTION_RESULT_CANCEL          (1)
#define OMM_MARIO_ACTION_RESULT_CONTINUE        (2)

#define OMM_MARIO_POSSESS_CHECK_TANGIBILITY     (1 << 0)
#define OMM_MARIO_POSSESS_FORCE_CAPTURE         (1 << 1)
#define OMM_MARIO_POSSESS_IGNORE_PRESS_HOLD     (1 << 2)

#define OMM_MARIO_UNPOSSESS_ACT_NONE            (0)
#define OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT        (1)
#define OMM_MARIO_UNPOSSESS_ACT_FORWARD_KB      (2)
#define OMM_MARIO_UNPOSSESS_ACT_BACKWARD_KB     (3)
#define OMM_MARIO_UNPOSSESS_ACT_BURNT           (4)
#define OMM_MARIO_UNPOSSESS_ACT_GRABBED         (5)
#define OMM_MARIO_UNPOSSESS_ACT_EATEN           (6)
#define OMM_MARIO_UNPOSSESS_ACT_BLOWN           (7)
#define OMM_MARIO_UNPOSSESS_ACT_TORNADO         (8)

//
// Movement
//

bool omm_mario_update_walking_speed             (struct MarioState *m);
bool omm_mario_update_air_without_turn          (struct MarioState *m);
bool omm_mario_update_air_with_turn             (struct MarioState *m);
bool omm_mario_update_hanging_speed             (struct MarioState *m);

//
// Interactions
//

bool omm_mario_interact_hoot                    (struct MarioState *m, struct Object *o);
bool omm_mario_interact_grabbable               (struct MarioState *m, struct Object *o);
bool omm_mario_interact_door                    (struct MarioState *m, struct Object *o);
bool omm_mario_interact_damage                  (struct MarioState *m, struct Object *o);
bool omm_mario_interact_coin                    (struct MarioState *m, struct Object *o);
bool omm_mario_interact_cap                     (struct MarioState *m, struct Object *o);
bool omm_mario_interact_pole                    (struct MarioState *m, struct Object *o);
bool omm_mario_interact_koopa                   (struct MarioState *m, struct Object *o);
bool omm_mario_interact_unknown_08              (struct MarioState *m, struct Object *o);
bool omm_mario_interact_breakable               (struct MarioState *m, struct Object *o);
bool omm_mario_interact_strong_wind             (struct MarioState *m, struct Object *o);
bool omm_mario_interact_warp_door               (struct MarioState *m, struct Object *o);
bool omm_mario_interact_star_or_key             (struct MarioState *m, struct Object *o);
bool omm_mario_interact_warp                    (struct MarioState *m, struct Object *o);
bool omm_mario_interact_cannon_base             (struct MarioState *m, struct Object *o);
bool omm_mario_interact_bounce_top              (struct MarioState *m, struct Object *o);
bool omm_mario_interact_water_ring              (struct MarioState *m, struct Object *o);
bool omm_mario_interact_bully                   (struct MarioState *m, struct Object *o);
bool omm_mario_interact_flame                   (struct MarioState *m, struct Object *o);
bool omm_mario_interact_koopa_shell             (struct MarioState *m, struct Object *o);
bool omm_mario_interact_bounce_top_2            (struct MarioState *m, struct Object *o);
bool omm_mario_interact_mr_blizzard             (struct MarioState *m, struct Object *o);
bool omm_mario_interact_hit_from_below          (struct MarioState *m, struct Object *o);
bool omm_mario_interact_text                    (struct MarioState *m, struct Object *o);
bool omm_mario_interact_tornado                 (struct MarioState *m, struct Object *o);
bool omm_mario_interact_whirlpool               (struct MarioState *m, struct Object *o);
bool omm_mario_interact_clam_or_bubba           (struct MarioState *m, struct Object *o);
bool omm_mario_interact_bbh_entrance            (struct MarioState *m, struct Object *o);
bool omm_mario_interact_snufit_bullet           (struct MarioState *m, struct Object *o);
bool omm_mario_interact_shock                   (struct MarioState *m, struct Object *o);
bool omm_mario_interact_igloo_barrier           (struct MarioState *m, struct Object *o);
bool omm_mario_interact_unknown_31              (struct MarioState *m, struct Object *o);
void omm_mario_process_interactions             (struct MarioState *m);

//
// Actions
//

bool omm_mario_set_action                       (struct MarioState *m, u32 action, u32 actionArg, u16 buttons);
bool omm_mario_throw_cappy                      (struct MarioState *m, u32 action, u32 actionArg, u16 buttons);
void omm_mario_init_next_action                 (struct MarioState *m);
bool omm_mario_check_wall_slide                 (struct MarioState *m);
bool omm_mario_can_perform_wall_slide           (struct MarioState *m);
bool omm_mario_try_to_perform_wall_slide        (struct MarioState *m);
bool omm_mario_check_and_perform_wall_slide     (struct MarioState *m);
bool omm_mario_should_stop_wall_slide           (struct MarioState *m);
bool omm_mario_is_idling                        (struct MarioState *m);
bool omm_mario_is_jumping                       (struct MarioState *m);
bool omm_mario_is_punching                      (struct MarioState *m);
bool omm_mario_is_kicking                       (struct MarioState *m);
bool omm_mario_is_attacking                     (struct MarioState *m);
bool omm_mario_is_ground_pounding               (struct MarioState *m);
bool omm_mario_is_ground_pound_landing          (struct MarioState *m);
bool omm_mario_is_roll_landing                  (struct MarioState *m);
bool omm_mario_is_reading                       (struct MarioState *m);
bool omm_mario_is_picking_up                    (struct MarioState *m);
bool omm_mario_is_holding                       (struct MarioState *m);
bool omm_mario_is_hanging                       (struct MarioState *m);
bool omm_mario_is_ledge_climbing                (struct MarioState *m);
bool omm_mario_is_burning                       (struct MarioState *m);
bool omm_mario_is_sliding                       (struct MarioState *m);
bool omm_mario_is_grabbed                       (struct MarioState *m);
bool omm_mario_is_stuck_in_ground_after_fall    (struct MarioState *m);
bool omm_mario_is_star_dancing                  (struct MarioState *m);
bool omm_mario_is_emerging_from_pipe            (struct MarioState *m);
bool omm_mario_is_ready_for_dialog              (struct MarioState *m);
bool omm_mario_is_ready_to_speak                (struct MarioState *m);
bool omm_mario_is_capture                       (struct MarioState *m);
bool omm_mario_is_invisible                     (struct MarioState *m);
bool omm_mario_allow_first_person               (struct MarioState *m);
bool omm_mario_should_walk                      (struct MarioState *m);
bool omm_mario_should_run                       (struct MarioState *m);
bool omm_mario_has_wing_cap                     (struct MarioState *m);
bool omm_mario_has_vanish_cap                   (struct MarioState *m);
bool omm_mario_has_metal_cap                    (struct MarioState *m);
bool omm_mario_cap_is_flickering                (struct MarioState *m);
bool omm_mario_is_dead                          (struct MarioState *m);
bool omm_mario_check_dead                       (struct MarioState *m, s32 health);
bool omm_mario_check_death_warp                 (struct MarioState *m, s32 warpOp);
bool omm_mario_check_grab                       (struct MarioState *m, struct Object *o, bool ignoreAngles);
bool omm_mario_check_npc_dialog                 (struct MarioState *m, s32 actionArg, s32 *dialogState);
bool omm_mario_check_flooded                    (struct MarioState *m);
#if OMM_GAME_IS_R96X
bool omm_mario_is_milk                          (struct MarioState *m);
#else
#define omm_mario_is_milk(m)                    false
#endif

//
// Update
//

void omm_mario_update_platform                  (struct MarioState *m);
void omm_mario_unset_cap                        (struct MarioState *m);
void omm_mario_lock_camera                      (struct MarioState *m, bool isStarCutscene);
bool omm_mario_start_dialog                     (struct MarioState *m, struct Object *o, s16 dialogId, bool dialogWithChoice, bool freezeObject, s16 angleVelTurnObjectTowardsMario);
void omm_mario_update_l_actions                 (struct MarioState *m);

//
// Capture
//

bool omm_mario_possess_object                   (struct MarioState *m, struct Object *o, u32 possessFlags);
bool omm_mario_possess_object_after_warp        (struct MarioState *m);
bool omm_mario_unpossess_object                 (struct MarioState *m, s32 unpossessAct, u32 objIntangibleFrames);
bool omm_mario_unpossess_object_with_yaw        (struct MarioState *m, s32 unpossessAct, u32 objIntangibleFrames, s16 faceAngleYaw);
bool omm_mario_unpossess_object_before_warp     (struct MarioState *m);
bool omm_mario_lock                             (struct MarioState *m, s32 duration);
bool omm_mario_lock_once                        (struct MarioState *m, s32 duration);
bool omm_mario_unlock                           (struct MarioState *m);
bool omm_mario_is_locked                        (struct MarioState *m);
s32  omm_act_possession                         (struct MarioState *m);

//
// Colors
//

bool omm_mario_colors_read                      (const char *name, const char *value1, const char *value2, bool *invalid);
void omm_mario_colors_write                     ();
void omm_mario_colors_reset                     (bool peach);
s32  omm_mario_colors_count                     ();
s32  omm_mario_colors_lights_count              (bool peach);
s32  omm_mario_colors_light_groups_count        (bool peach);
u32 *omm_mario_colors_get_light                 (bool peach, s32 palette, s32 index);
const char **omm_mario_colors_choices           (bool peach);
const char  *omm_mario_colors_light_name        (bool peach, s32 index);
const char  *omm_mario_colors_light_group_name  (bool peach, s32 index);
const void  *omm_mario_colors_part_to_light     (s32 part);

#endif // OMM_MARIO_H

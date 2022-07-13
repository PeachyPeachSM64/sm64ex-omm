#ifndef OMM_MARIO_H
#define OMM_MARIO_H

#include "data/omm/omm_includes.h"

#define HANG_STEP_NONE 0
#define HANG_STEP_HIT_WALL 1
#define HANG_STEP_LEFT_CEIL 2

enum OmmActionResult {
    OMM_MARIO_ACTION_RESULT_BREAK,
    OMM_MARIO_ACTION_RESULT_CANCEL,
    OMM_MARIO_ACTION_RESULT_CONTINUE
};

enum OmmUnpossessAction {
    OMM_MARIO_UNPOSSESS_ACT_NONE,
    OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT,
    OMM_MARIO_UNPOSSESS_ACT_KNOCKED_BACK,
    OMM_MARIO_UNPOSSESS_ACT_BURNT,
    OMM_MARIO_UNPOSSESS_ACT_GRABBED,
    OMM_MARIO_UNPOSSESS_ACT_EATEN,
    OMM_MARIO_UNPOSSESS_ACT_BLOWN,
    OMM_MARIO_UNPOSSESS_ACT_TORNADO,
    OMM_MARIO_UNPOSSESS_ACT_COUNT
};

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

bool omm_mario_interact_coin                    (struct MarioState *m, struct Object *o);
bool omm_mario_interact_star_or_key             (struct MarioState *m, struct Object *o);
bool omm_mario_interact_cap                     (struct MarioState *m, struct Object *o);
void omm_mario_preprocess_interactions          (struct MarioState *m);

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
bool omm_mario_is_idling                        (struct MarioState *m);
bool omm_mario_is_punching                      (struct MarioState *m);
bool omm_mario_is_kicking                       (struct MarioState *m);
bool omm_mario_is_ground_pounding               (struct MarioState *m);
bool omm_mario_is_ground_pound_landing          (struct MarioState *m);
bool omm_mario_is_roll_landing                  (struct MarioState *m);
bool omm_mario_is_reading                       (struct MarioState *m);
bool omm_mario_is_picking_up                    (struct MarioState *m);
bool omm_mario_is_holding                       (struct MarioState *m);
bool omm_mario_is_hanging                       (struct MarioState *m);
bool omm_mario_is_stuck_in_ground_after_fall    (struct MarioState *m);
bool omm_mario_is_star_dancing                  (struct MarioState *m);
bool omm_mario_should_walk                      (struct MarioState *m);
bool omm_mario_should_run                       (struct MarioState *m);
bool omm_mario_has_wing_cap                     (struct MarioState *m);
bool omm_mario_has_vanish_cap                   (struct MarioState *m);
bool omm_mario_has_metal_cap                    (struct MarioState *m);
bool omm_mario_check_dead                       (struct MarioState *m, s16 hp);
bool omm_mario_check_death_warp                 (struct MarioState *m, s32 warpOp);
bool omm_mario_check_grab                       (struct MarioState *m, struct Object *o, bool ignoreAngles);

//
// Update
//

void omm_mario_lock_camera                      (struct MarioState *m, bool isStarCutscene);
bool omm_mario_start_dialog                     (struct MarioState *m, struct Object *o, s16 dialogId, bool dialogWithChoice, bool freezeObject, s16 angleVelTurnObjectTowardsMario);
void omm_mario_update_dialog                    (struct MarioState *m);
void omm_mario_update_grab                      (struct MarioState *m);
void omm_mario_update_spin                      (struct MarioState *m);
void omm_mario_update_fall                      (struct MarioState *m);
void omm_mario_update_caps                      (struct MarioState *m);
void omm_mario_update_burn                      (struct MarioState *m);
void omm_mario_update_action                    (struct MarioState *m);
void omm_mario_update_camera_mode               (struct MarioState *m);
void omm_mario_update_castle_collisions         (struct MarioState *m);

//
// Capture
//

bool omm_mario_possess_object                   (struct MarioState *m, struct Object *o, bool checkTangibility);
bool omm_mario_unpossess_object                 (struct MarioState *m, u8 unpossessAct, bool isBackwards, u32 objIntangibleFrames);
bool omm_mario_lock                             (struct MarioState *m, s32 duration);
bool omm_mario_lock_star_grab                   (struct MarioState *m);
bool omm_mario_unlock                           (struct MarioState *m);
bool omm_mario_is_locked                        (struct MarioState *m);
s32  omm_act_possession                         (struct MarioState *m);

#endif // OMM_MARIO_H

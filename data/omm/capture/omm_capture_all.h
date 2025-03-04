#ifndef OMM_CAPTURE_ALL_H
#define OMM_CAPTURE_ALL_H

#include "types.h"

//
// Possessed object constants and shortcuts
//

#define POBJ_RESULT_NONE                        (0)
#define POBJ_RESULT_JUMP_START                  (1)
#define POBJ_RESULT_JUMP_HOLD                   (2)

#define POBJ_NONZERO_ANALOG                     (gOmmMario->capture.stickMag != 0)
#define POBJ_A_BUTTON_PRESSED                   ((gOmmMario->capture.buttonPressed & A_BUTTON) != 0)
#define POBJ_A_BUTTON_DOWN                      ((gOmmMario->capture.buttonDown & A_BUTTON) != 0)
#define POBJ_B_BUTTON_PRESSED                   ((gOmmMario->capture.buttonPressed & B_BUTTON) != 0)
#define POBJ_B_BUTTON_DOWN                      ((gOmmMario->capture.buttonDown & B_BUTTON) != 0)
#define POBJ_X_BUTTON_PRESSED                   ((gOmmMario->capture.buttonPressed & X_BUTTON) != 0)
#define POBJ_X_BUTTON_DOWN                      ((gOmmMario->capture.buttonDown & X_BUTTON) != 0)
#define POBJ_IS_STAR_DANCING                    (gOmmMario->capture.starDance)
#define POBJ_IS_OPENING_DOORS                   (gOmmMario->capture.openDoors)
#define POBJ_IS_EMERGING_FROM_PIPE              (gOmmWarp->state == POBJ_WARP_STATE_EMERGING_FROM_PIPE)
#define POBJ_ABS_FORWARD_VEL                    (abs_f(o->oForwardVel))
#define POBJ_IS_WALKING                         (POBJ_ABS_FORWARD_VEL > 2.f)
#define POBJ_IS_RUNNING                         (POBJ_ABS_FORWARD_VEL > pobj_get_walk_speed(o))
#define POBJ_IS_DASHING                         (POBJ_ABS_FORWARD_VEL > pobj_get_run_speed(o))

#define POBJ_PHYSICS_GROUND                     omm_player_physics_get_ground(omm_player_get_selected_index(), true)
#define POBJ_PHYSICS_AIR                        omm_player_physics_get_air(omm_player_get_selected_index(), true)
#define POBJ_PHYSICS_SWIM                       omm_player_physics_get_swim(omm_player_get_selected_index(), true)
#define POBJ_PHYSICS_JUMP                       omm_player_physics_get_jump(omm_player_get_selected_index(), true)
#define POBJ_PHYSICS_GRAVITY                    omm_player_physics_get_gravity(omm_player_get_selected_index(), true)

#define POBJ_SET_ABOVE_WATER                    gOmmObject->state._properties |= (1 << 0)
#define POBJ_SET_UNDER_WATER                    gOmmObject->state._properties |= (1 << 1)
#define POBJ_SET_AFFECTED_BY_WATER              gOmmObject->state._properties |= (1 << 2)
#define POBJ_SET_AFFECTED_BY_VERTICAL_WIND      gOmmObject->state._properties |= (1 << 3)
#define POBJ_SET_AFFECTED_BY_CANNON             gOmmObject->state._properties |= (1 << 4)
#define POBJ_SET_FLOATING                       gOmmObject->state._properties |= (1 << 5)
#define POBJ_SET_INVULNERABLE                   gOmmObject->state._properties |= (1 << 6)
#define POBJ_SET_INTANGIBLE                     gOmmObject->state._properties |= (1 << 7)
#define POBJ_SET_IMMUNE_TO_FIRE                 gOmmObject->state._properties |= (1 << 8)
#define POBJ_SET_IMMUNE_TO_LAVA                 gOmmObject->state._properties |= (1 << 9)
#define POBJ_SET_IMMUNE_TO_QUICKSAND            gOmmObject->state._properties |= (1 << 10)
#define POBJ_SET_IMMUNE_TO_STRONG_WINDS         gOmmObject->state._properties |= (1 << 11)
#define POBJ_SET_ABLE_TO_MOVE_ON_WATER          gOmmObject->state._properties |= (1 << 12)
#define POBJ_SET_ABLE_TO_MOVE_ON_SLOPES         gOmmObject->state._properties |= (1 << 13)
#define POBJ_SET_ABLE_TO_MOVE_THROUGH_WALLS     gOmmObject->state._properties |= (1 << 14)
#define POBJ_SET_ABLE_TO_OPEN_DOORS             gOmmObject->state._properties |= (1 << 15)
#define POBJ_SET_ATTACKING                      gOmmObject->state._properties |= (1 << 16)
#define POBJ_SET_ATTACKING_BREAKABLE            gOmmObject->state._properties |= (1 << 17)
#define POBJ_SET_GROUND_POUNDING                gOmmObject->state._properties |= (1 << 18)
#define POBJ_SET_TALKING                        gOmmObject->state._properties |= (1 << 19)
#define POBJ_SET_BOUNCING                       gOmmObject->state._properties |= (1 << 20) // Special property set when bouncing on an enemy

#define POBJ_IS_ABOVE_WATER                     ((gOmmObject->state._properties & (1 << 0)) != 0)
#define POBJ_IS_UNDER_WATER                     ((gOmmObject->state._properties & (1 << 1)) != 0)
#define POBJ_IS_AFFECTED_BY_WATER               ((gOmmObject->state._properties & (1 << 2)) != 0)
#define POBJ_IS_AFFECTED_BY_VERTICAL_WIND       ((gOmmObject->state._properties & (1 << 3)) != 0)
#define POBJ_IS_AFFECTED_BY_CANNON              ((gOmmObject->state._properties & (1 << 4)) != 0)
#define POBJ_IS_FLOATING                        ((gOmmObject->state._properties & (1 << 5)) != 0)
#define POBJ_IS_INVULNERABLE                    ((gOmmObject->state._properties & (1 << 6)) != 0)
#define POBJ_IS_INTANGIBLE                      ((gOmmObject->state._properties & (1 << 7)) != 0)
#define POBJ_IS_IMMUNE_TO_FIRE                  ((gOmmObject->state._properties & (1 << 8)) != 0)
#define POBJ_IS_IMMUNE_TO_LAVA                  ((gOmmObject->state._properties & (1 << 9)) != 0)
#define POBJ_IS_IMMUNE_TO_QUICKSAND             ((gOmmObject->state._properties & (1 << 10)) != 0)
#define POBJ_IS_IMMUNE_TO_STRONG_WINDS          ((gOmmObject->state._properties & (1 << 11)) != 0)
#define POBJ_IS_ABLE_TO_MOVE_ON_WATER           ((gOmmObject->state._properties & (1 << 12)) != 0)
#define POBJ_IS_ABLE_TO_MOVE_ON_SLOPES          ((gOmmObject->state._properties & (1 << 13)) != 0)
#define POBJ_IS_ABLE_TO_MOVE_THROUGH_WALLS      ((gOmmObject->state._properties & (1 << 14)) != 0)
#define POBJ_IS_ABLE_TO_OPEN_DOORS              ((gOmmObject->state._properties & (1 << 15)) != 0)
#define POBJ_IS_ATTACKING                       ((gOmmObject->state._properties & (1 << 16)) != 0)
#define POBJ_IS_ATTACKING_BREAKABLE             ((gOmmObject->state._properties & (1 << 17)) != 0)
#define POBJ_IS_GROUND_POUNDING                 ((gOmmObject->state._properties & (1 << 18)) != 0)
#define POBJ_IS_TALKING                         ((gOmmObject->state._properties & (1 << 19)) != 0)
#define POBJ_IS_BOUNCING                        ((gOmmObject->state._properties & (1 << 20)) != 0)

#define POBJ_STEP_FLAGS                         (OBJ_STEP_UPDATE_HOME | OBJ_STEP_UPDATE_PREV_POS | (OBJ_STEP_MOVE_THROUGH_WALLS * POBJ_IS_ABLE_TO_MOVE_THROUGH_WALLS) | (OBJ_STEP_STICKY_FEET * POBJ_IS_ABLE_TO_MOVE_ON_SLOPES) | OBJ_STEP_CHECK_ON_GROUND)
#define POBJ_INT_PRESET_COLLECTIBLES            (OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_STARS)
#define POBJ_INT_PRESET_CHAIN_CHOMP             (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE)
#define POBJ_INT_PRESET_MAD_PIANO               (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE)
#define POBJ_INT_PRESET_AMP                     (OBJ_INT_ATTRACT_COINS_WEAK | OBJ_INT_ATTRACT_COINS_RANGE(500))
#define POBJ_INT_PRESET_CHEEP_CHEEP_SPIN        (OBJ_INT_ATTRACT_COINS_STRONG | OBJ_INT_ATTRACT_COINS_RANGE(600))

#define POBJ_WARP_STATE_CANNOT_WARP             (-1)
#define POBJ_WARP_STATE_NOT_WARPING             (0)
#define POBJ_WARP_STATE_WARPING                 (1)
#define POBJ_WARP_STATE_OPENING_DOOR            (2)
#define POBJ_WARP_STATE_FADING_OUT              (3)
#define POBJ_WARP_STATE_FADING_IN               (4)
#define POBJ_WARP_STATE_EMERGING_FROM_PIPE      (5)
#define POBJ_WARP_STATE_HAS_WARPED              (6)

#define POBJ_CAN_WARP(warpObj)                  (gOmmWarp->state == POBJ_WARP_STATE_NOT_WARPING && ((warpObj) == NULL || (warpObj) != gOmmWarp->object))

//
// Sounds
//

// Walk
#define POBJ_SOUND_WALK_GOOMBA                  (SOUND_OBJ_GOOMBA_WALK)
#define POBJ_SOUND_WALK_KOOPA                   (SOUND_OBJ_KOOPA_WALK)
#define POBJ_SOUND_WALK_BOBOMB                  (SOUND_OBJ_BOBOMB_WALK)
#define POBJ_SOUND_WALK_BULLY_1                 (SOUND_OBJ_BULLY_WALK)
#define POBJ_SOUND_WALK_BULLY_2                 (SOUND_OBJ_BULLY_WALKING)
#define POBJ_SOUND_WALK_SCUTTLEBUG              (SOUND_OBJ2_SCUTTLEBUG_WALK)
#define POBJ_SOUND_WALK_SKEETER                 (SOUND_OBJ_SKEETER_WALK)
#define POBJ_SOUND_WALK_PENGUIN                 (SOUND_OBJ_BABY_PENGUIN_WALK)
#define POBJ_SOUND_WALK_HEAVE_HO                (SOUND_AIR_HEAVEHO_MOVE)
#define POBJ_SOUND_WALK_CHUCKYA                 (SOUND_AIR_CHUCKYA_MOVE)
#define POBJ_SOUND_WALK_UKIKI                   (SOUND_OBJ_UKIKI_STEP_DEFAULT)
#define POBJ_SOUND_WALK_MIPS                    (SOUND_OBJ_MIPS_RABBIT)
#define POBJ_SOUND_WALK_MIPS_WATER              (SOUND_OBJ_MIPS_RABBIT_WATER)
#define POBJ_SOUND_WALK_YOSHI                   (SOUND_GENERAL_YOSHI_WALK)
#define POBJ_SOUND_WALK_METAL                   (SOUND_ACTION_METAL_STEP)
#define POBJ_SOUND_WALK_METAL_WATER             (SOUND_ACTION_METAL_STEP_WATER)

// Jump
#define POBJ_SOUND_JUMP_1                       (SOUND_OBJ_WIGGLER_JUMP)
#define POBJ_SOUND_JUMP_2                       (SOUND_OBJ2_SCUTTLEBUG_ALERT)
#define POBJ_SOUND_JUMP_BOING_1                 (SOUND_GENERAL_BOING1)
#define POBJ_SOUND_JUMP_BOING_2                 (SOUND_GENERAL_BOING2)
#define POBJ_SOUND_JUMP_KING_BOBOMB             (SOUND_OBJ_KING_BOBOMB_JUMP)
#define POBJ_SOUND_JUMP_MR_BLIZZARD             (SOUND_OBJ_MR_BLIZZARD_ALERT | 0x4000)
#define POBJ_SOUND_JUMP_SWOOP                   (SOUND_OBJ2_SWOOP | 0x4000)
#define POBJ_SOUND_JUMP_UKIKI                   (SOUND_OBJ_UKIKI_CHATTER_LONG | 0x4000)
#define POBJ_SOUND_JUMP_MIPS                    (SOUND_OBJ_MIPS_RABBIT | 0x4000)
#define POBJ_SOUND_JUMP_METAL                   (SOUND_ACTION_METAL_JUMP)
#define POBJ_SOUND_JUMP_METAL_WATER             (SOUND_ACTION_METAL_JUMP_WATER)

// Air
#define POBJ_SOUND_WING_FLAP_1                  (SOUND_GENERAL_SWISH_WATER)
#define POBJ_SOUND_WING_FLAP_2                  (SOUND_OBJ_UNKNOWN6)
#define POBJ_SOUND_FLYING_FAST                  (SOUND_ACTION_FLYING_FAST | 0x8100)

// Water
#define POBJ_SOUND_SWIMMING                     (SOUND_ACTION_UNKNOWN434)
#define POBJ_SOUND_WATER_SPLASH                 (SOUND_ACTION_UNKNOWN430)
#define POBJ_SOUND_SHALLOW_WATER_SPLASH         (SOUND_OBJ_WALKING_WATER | 0x3000)

// Actions
#define POBJ_SOUND_SHOOT                        (SOUND_OBJ_MRI_SHOOT)
#define POBJ_SOUND_THROW_1                      (SOUND_ACTION_THROW)
#define POBJ_SOUND_THROW_2                      (SOUND_OBJ_UNKNOWN4)
#define POBJ_SOUND_THROW_LAKITU                 (SOUND_OBJ_EVIL_LAKITU_THROW)
#define POBJ_SOUND_POUNDING                     (SOUND_OBJ_POUNDING1)
#define POBJ_SOUND_VANISH                       (SOUND_GENERAL_VANISH_SFX)

// Effects
#define POBJ_SOUND_DEATH                        (SOUND_OBJ_DEFAULT_DEATH)
#define POBJ_SOUND_STAR                         (SOUND_MENU_STAR_SOUND)
#define POBJ_SOUND_WRONG                        (SOUND_MENU_CAMERA_BUZZ)
#define POBJ_SOUND_BREAK_BOX                    (SOUND_GENERAL_BREAK_BOX)
#define POBJ_SOUND_WALL_EXPLOSION               (SOUND_GENERAL_WALL_EXPLOSION | 0x7F00)
#define POBJ_SOUND_LAVA_BURN                    (SOUND_MOVING_LAVA_BURN)

// Terrain
#define POBJ_SOUND_WALKING_SNOW_SAND            (SOUND_OBJ_SNOW_SAND1)
#define POBJ_SOUND_SLIDING_GROUND               (SOUND_MOVING_TERRAIN_SLIDE + gMarioState->terrainSoundAddend)
#define POBJ_SOUND_RIDING_SHELL_GROUND          (SOUND_MOVING_TERRAIN_RIDING_SHELL + gMarioState->terrainSoundAddend)
#define POBJ_SOUND_RIDING_SHELL_WATER           (SOUND_MOVING_TERRAIN_RIDING_SHELL + SOUND_TERRAIN_WATER)
#define POBJ_SOUND_RIDING_SHELL_LAVA            (SOUND_MOVING_RIDING_SHELL_LAVA)
#define POBJ_SOUND_LANDING_METAL                (SOUND_ACTION_METAL_LANDING)
#define POBJ_SOUND_LANDING_METAL_WATER          (SOUND_ACTION_METAL_LAND_WATER)

// Object specific
#define POBJ_SOUND_CHAIN                        (SOUND_GENERAL_CHAIN_CHOMP1)
#define POBJ_SOUND_CHAIN_CHOMP_BARK             (SOUND_GENERAL_CHAIN_CHOMP2)
#define POBJ_SOUND_WHOMP                        (SOUND_OBJ_WHOMP_LOWPRIO)
#define POBJ_SOUND_THWOMP                       (SOUND_OBJ_THWOMP)
#define POBJ_SOUND_PENGUIN_DIVE                 (SOUND_OBJ_BABY_PENGUIN_DIVE)
#define POBJ_SOUND_MONTY_MOLE_DIG               (SOUND_OBJ2_MONTY_MOLE_APPEAR)
#define POBJ_SOUND_BULLY                        (SOUND_OBJ2_BULLY_ATTACKED)
#define POBJ_SOUND_MOTOS_GRAB                   (SOUND_GENERAL_CANNON_UP)
#define POBJ_SOUND_AMP_BUZZ                     (SOUND_AIR_AMP_BUZZ)
#define POBJ_SOUND_HEAVE_HO_GRAB                (SOUND_OBJ_HEAVEHO_TOSSED + 0x4000)
#define POBJ_SOUND_CHUCKYA_GRAB                 (SOUND_OBJ_UNKNOWN3)
#define POBJ_SOUND_UKIKI_IDLE                   (SOUND_OBJ_UKIKI_CHATTER_IDLE)
#define POBJ_SOUND_UKIKI_TAUNT                  (SOUND_OBJ_UKIKI_CHATTER_SHORT)
#define POBJ_SOUND_LAKITU_FLY                   (SOUND_AIR_LAKITU_FLY)
#define POBJ_SOUND_YOSHI                        (SOUND_GENERAL_YOSHI_TALK)
#define POBJ_SOUND_YOSHI_THROW                  (SOUND_OBJ_MRI_SHOOT | 0xFF00)
#define POBJ_SOUND_YOSHI_EGG                    (SOUND_ACTION_UNSTUCK_FROM_GROUND | 0xFF00)
#define POBJ_SOUND_YOSHI_EGG_THROW              (SOUND_OBJ_EVIL_LAKITU_THROW | 0xFF00)

//
// Captures
//

#define OMM_CAPTURE_GOOMBA                      (1llu << 0llu)
#define OMM_CAPTURE_GOOMBA_HUGE                 (1llu << 1llu)
#define OMM_CAPTURE_GOOMBA_TINY                 (1llu << 2llu)
#define OMM_CAPTURE_KOOPA                       (1llu << 3llu)
#define OMM_CAPTURE_KOOPA_SHELL                 (1llu << 4llu)
#define OMM_CAPTURE_KOOPA_SHELL_WATER           (1llu << 5llu)
#define OMM_CAPTURE_BOBOMB                      (1llu << 6llu)
#define OMM_CAPTURE_BOBOMB_BUDDY                (1llu << 7llu)
#define OMM_CAPTURE_BOBOMB_BUDDY_CANNON         (1llu << 8llu)
#define OMM_CAPTURE_CHAIN_CHOMP                 (1llu << 9llu)
#define OMM_CAPTURE_BULLET_BILL                 (1llu << 10llu)
#define OMM_CAPTURE_HOOT                        (1llu << 11llu)
#define OMM_CAPTURE_WHOMP                       (1llu << 12llu)
#define OMM_CAPTURE_THWOMP                      (1llu << 13llu)
#define OMM_CAPTURE_SPINDRIFT                   (1llu << 14llu)
#define OMM_CAPTURE_SMALL_PENGUIN               (1llu << 15llu)
#define OMM_CAPTURE_MR_BLIZZARD                 (1llu << 16llu)
#define OMM_CAPTURE_SNOWMAN                     (1llu << 17llu)
#define OMM_CAPTURE_BOO                         (1llu << 18llu)
#define OMM_CAPTURE_BIG_BOO                     (1llu << 19llu)
#define OMM_CAPTURE_SCUTTLEBUG                  (1llu << 20llu)
#define OMM_CAPTURE_MAD_PIANO                   (1llu << 21llu)
#define OMM_CAPTURE_CRAZY_BOX                   (1llu << 22llu)
#define OMM_CAPTURE_MR_I                        (1llu << 23llu)
#define OMM_CAPTURE_SWOOP                       (1llu << 24llu)
#define OMM_CAPTURE_SNUFIT                      (1llu << 25llu)
#define OMM_CAPTURE_MONTY_MOLE                  (1llu << 26llu)
#define OMM_CAPTURE_DORRIE                      (1llu << 27llu)
#define OMM_CAPTURE_BULLY                       (1llu << 28llu)
#define OMM_CAPTURE_FLY_GUY                     (1llu << 29llu)
#define OMM_CAPTURE_POKEY                       (1llu << 30llu)
#define OMM_CAPTURE_AMP                         (1llu << 31llu)
#define OMM_CAPTURE_GRINDEL                     (1llu << 32llu)
#define OMM_CAPTURE_FIRE_SPITTER                (1llu << 33llu)
#define OMM_CAPTURE_CHEEP_CHEEP                 (1llu << 34llu)
#define OMM_CAPTURE_SUSHI_SHARK                 (1llu << 35llu)
#define OMM_CAPTURE_MONEY_BAG                   (1llu << 36llu)
#define OMM_CAPTURE_SKEETER                     (1llu << 37llu)
#define OMM_CAPTURE_HEAVE_HO                    (1llu << 38llu)
#define OMM_CAPTURE_CHUCKYA                     (1llu << 39llu)
#define OMM_CAPTURE_HAT_UKIKI                   (1llu << 40llu)
#define OMM_CAPTURE_CAGE_UKIKI                  (1llu << 41llu)
#define OMM_CAPTURE_LAKITU                      (1llu << 42llu)
#define OMM_CAPTURE_SPINY                       (1llu << 43llu)
#define OMM_CAPTURE_TOAD                        (1llu << 44llu)
#define OMM_CAPTURE_MIPS                        (1llu << 45llu)
#define OMM_CAPTURE_FLAMING_BOBOMB              (1llu << 46llu)
#define OMM_CAPTURE_YOSHI                       (1llu << 47llu)

#define OMM_MAX_CAPTURES                        (48)
#define OMM_ALL_CAPTURES                        ((1llu << OMM_MAX_CAPTURES) - 1llu)
#define OMM_ALL_CAPTURES_BUT_YOSHI              (OMM_ALL_CAPTURES & ~OMM_CAPTURE_YOSHI)

//
// Attributes
//

f32 pobj_get_walk_speed(struct Object *o);
f32 pobj_get_run_speed(struct Object *o);
f32 pobj_get_dash_speed(struct Object *o);
f32 pobj_get_max_speed(struct Object *o, bool run, bool dash, bool stop);
f32 pobj_get_jump_velocity(struct Object *o);
f32 pobj_get_terminal_velocity(struct Object *o);
f32 pobj_get_gravity(struct Object *o);

//
// Movement
//

bool pobj_process_inputs(struct Object *o);
void pobj_move(struct Object *o, bool run, bool dash, bool stop);
void pobj_move_max_vel(struct Object *o, f32 maxVel);
void pobj_move_3d(struct Object *o, bool run, bool dash, bool stop, s16 pitchMax, s16 pitchVel, s16 yawVel);
s32  pobj_jump(struct Object *o, s32 numMaxJumps);
bool pobj_hop(struct Object *o, f32 hopDiv);
void pobj_decelerate(struct Object *o);
void pobj_apply_gravity(struct Object *o, f32 mult);
void pobj_handle_special_floors(struct Object *o);

//
// Attacks
//

f32 pobj_check_charge_attack(s32 tmax, struct Object *origin, f32 radius, f32 offset);
f32 pobj_check_release_attack(s32 tmax);

//
// Doors
//

bool pobj_update_door(struct Object *o);

//
// Hold
//

bool pobj_try_to_hold_object(struct Object *o, f32 radius, f32 height, f32 offset, u16 angleRange);
bool pobj_update_held_object(struct Object *o, f32 dx, f32 dy, f32 dz, f32 dh, f32 dt);
bool pobj_throw_held_object(struct Object *o, f32 forwardVel, f32 yVel);
bool pobj_release_held_object();

//
// Interactions
//

bool pobj_process_interaction(struct Object *o, struct Object *obj, u32 interactType);

#define pobj_return_ok                                          return 0
#define pobj_return_unpossess                                   return 1
#define pobj_return_retry                                       return 2
#define pobj_stop_if_unpossessed()                              { if (!omm_mario_is_capture(gMarioState)) { pobj_return_unpossess; } }
#define pobj_reset_home()                                       { o->oHomeX = gOmmObject->state._initialHome[0]; o->oHomeY = gOmmObject->state._initialHome[1]; o->oHomeZ = gOmmObject->state._initialHome[2]; }
#define pobj_freeze_gfx_during_star_dance()                     { if (POBJ_IS_STAR_DANCING) { o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP; } }
#define pobj_charge_attack(tmax, origin, radius, offset, ...)   { f32 _power_ = pobj_check_charge_attack(tmax, origin, radius, offset); if (_power_) { __VA_ARGS__ } }
#define pobj_release_attack(tmax, ...)                          { f32 _power_ = pobj_check_release_attack(tmax); if (_power_) { __VA_ARGS__ } }
#define pobj_process_interactions(...) {                                                \
    gOmmObject->state._invincTimer--;                                                   \
    if (!POBJ_IS_TALKING) {                                                             \
        omm_obj_process_interactions(o, POBJ_INT_PRESET_COLLECTIBLES);                  \
        if (gOmmObject->state._invincTimer <= 0 && !omm_mario_is_locked(gMarioState)) { \
            for_each_object_in_interaction_lists(obj) {                                 \
                if (obj != o && obj->activeFlags) {                                     \
                    { __VA_ARGS__ }                                                     \
                    if (pobj_process_interaction(o, obj, obj->oInteractType)) {         \
                        break;                                                          \
                    }                                                                   \
                }                                                                       \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
}

//
// Capture data
//

bool omm_capture_init(struct Object *o);
void omm_capture_end(struct Object *o);
s32  omm_capture_update(struct Object *o);
void omm_capture_update_gfx(struct Object *o);
u64  omm_capture_get_type(struct Object *o);
f32  omm_capture_get_top(struct Object *o);
f32  omm_capture_get_walk_speed(struct Object *o);
f32  omm_capture_get_run_speed(struct Object *o);
f32  omm_capture_get_dash_speed(struct Object *o);
f32  omm_capture_get_jump_velocity(struct Object *o);
f32  omm_capture_get_terminal_velocity(struct Object *o);
f32  omm_capture_get_gravity(struct Object *o);
f32  omm_capture_get_hitbox_radius(struct Object *o);
f32  omm_capture_get_hitbox_height(struct Object *o);
f32  omm_capture_get_hitbox_down_offset(struct Object *o);
f32  omm_capture_get_wall_hitbox_radius(struct Object *o);
bool omm_capture_should_reference_object(struct Object *o);
void omm_capture_apply_squish(struct Object *o);
void omm_capture_reset_squish(struct Object *o);
void omm_capture_set_camera_behind_mario();
void omm_capture_reset_camera();

//
// Behaviors
//

#define CAPTURE_BEHAVIOR(name) \
bool omm_cappy_##name##_init(struct Object *o); \
void omm_cappy_##name##_end(struct Object *o); \
s32  omm_cappy_##name##_update(struct Object *o); \
void omm_cappy_##name##_update_gfx(struct Object *o); \
u64  omm_cappy_##name##_get_type(struct Object *o); \
f32  omm_cappy_##name##_get_top(struct Object *o)

// Bob-omb Battlefield
CAPTURE_BEHAVIOR(goomba);               // bhvGoomba
CAPTURE_BEHAVIOR(koopa);                // bhvKoopa
CAPTURE_BEHAVIOR(koopa_shell);          // bhvKoopaShell, bhvKoopaShellUnderwater
CAPTURE_BEHAVIOR(bobomb);               // bhvBobomb
CAPTURE_BEHAVIOR(npc_message);          // bhvBobombBuddy
CAPTURE_BEHAVIOR(bobomb_buddy);         // bhvBobombBuddyOpensCannon
CAPTURE_BEHAVIOR(chain_chomp);          // bhvChainChomp, bhvOmmChainChompFree

// Whomp Fortress
CAPTURE_BEHAVIOR(bullet_bill);          // bhvBulletBill
CAPTURE_BEHAVIOR(hoot);                 // bhvHoot
CAPTURE_BEHAVIOR(whomp);                // bhvSmallWhomp
CAPTURE_BEHAVIOR(thwomp);               // bhvThwomp, bhvThwomp2, bhvGrindel, bhvHorizontalGrindel

// Jolly Roger Bay
// None

// Cool, Cool Mountain
CAPTURE_BEHAVIOR(spindrift);            // bhvSpindrift
CAPTURE_BEHAVIOR(penguin_small);        // bhvSmallPenguin, bhvPenguinBaby, bhvUnused20E0
CAPTURE_BEHAVIOR(mr_blizzard);          // bhvMrBlizzard
CAPTURE_BEHAVIOR(snowmans_body);        // bhvSnowmansBottom

// Big Boo's Haunt
CAPTURE_BEHAVIOR(boo);                  // bhvBoo, bhvGhostHuntBoo, bhvMerryGoRoundBoo, bhvBooWithCage
CAPTURE_BEHAVIOR(scuttlebug);           // bhvScuttlebug
CAPTURE_BEHAVIOR(mad_piano);            // bhvMadPiano
CAPTURE_BEHAVIOR(crazy_box);            // bhvJumpingBox
CAPTURE_BEHAVIOR(mr_i);                 // bhvMrI

// Hazy Maze Cave
CAPTURE_BEHAVIOR(swoop);                // bhvSwoop
CAPTURE_BEHAVIOR(snufit);               // bhvSnufit
CAPTURE_BEHAVIOR(monty_mole);           // bhvMontyMole
CAPTURE_BEHAVIOR(dorrie);               // bhvDorrie
CAPTURE_BEHAVIOR(omm_dorrie);           // bhvOmmDorrie

// Lethal Lava Land
CAPTURE_BEHAVIOR(bully);                // bhvSmallBully
CAPTURE_BEHAVIOR(motos);                // bhvMotos
CAPTURE_BEHAVIOR(blargg);               // bhvBlargg
CAPTURE_BEHAVIOR(friendly_blargg);      // bhvFriendlyBlargg

// Shifting Sand Land
CAPTURE_BEHAVIOR(fly_guy);              // bhvFlyGuy
CAPTURE_BEHAVIOR(pokey);                // bhvPokey, bhvPokeyBodyPart
CAPTURE_BEHAVIOR(amp);                  // bhvHomingAmp, bhvCirclingAmp

// Dire, Dire Docks
CAPTURE_BEHAVIOR(fire_spitter);         // bhvFireSpitter
CAPTURE_BEHAVIOR(cheep_cheep);          // bhvBub
CAPTURE_BEHAVIOR(sushi_shark);          // bhvSushiShark

// Snowman's Land
CAPTURE_BEHAVIOR(moneybag);             // bhvMoneybag

// Wet-Dry World
CAPTURE_BEHAVIOR(skeeter);              // bhvSkeeter
CAPTURE_BEHAVIOR(heave_ho);             // bhvHeaveHo
CAPTURE_BEHAVIOR(chuckya);              // bhvChuckya

// Tall, Tall Mountain
CAPTURE_BEHAVIOR(ukiki);                // bhvUkiki, bhvMacroUkiki

// Tiny-Huge Island
CAPTURE_BEHAVIOR(lakitu);               // bhvEnemyLakitu
CAPTURE_BEHAVIOR(spiny);                // bhvSpiny

// Tick Tock Clock
// None

// Rainbow Ride
// None

// Misc
CAPTURE_BEHAVIOR(toad);                 // bhvToadMessage
CAPTURE_BEHAVIOR(mips);                 // bhvMips
CAPTURE_BEHAVIOR(yoshi);                // bhvYoshi
struct Object *omm_cappy_yoshi_get_object_in_mouth(struct Object *o);

// Extra
CAPTURE_BEHAVIOR(bee);                  // bhvBee, bhvCustomSMSRSmallBee

// OMM Bowser
CAPTURE_BEHAVIOR(flaming_bobomb);       // bhvOmmFlamingBobomb

#undef CAPTURE_BEHAVIOR
#endif // OMM_CAPTURE_ALL_H

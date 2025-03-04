#ifndef OMM_MARIO_ACTIONS_H
#define OMM_MARIO_ACTIONS_H

//
// Mario
//

#define OMM_MARIO_FALL_DAMAGE_HEIGHT                (1800.f)
#define OMM_MARIO_WALL_SLIDE_JUMP_ANGLE_MAX         (0x1400)
#define OMM_MARIO_HANG_MAX_SPEED                    (16.f)
#define OMM_MARIO_SWIMMING_MAX_SPEED(hold)          (hold ? 25.f : 30.f)
#define OMM_MARIO_FLUTTER_KICK_MAX_SPEED(hold)      (hold ? 20.f : 24.f)
#define OMM_MARIO_ROLL_MIN_SPEED                    (30.f)
#define OMM_MARIO_ROLL_MAX_SPEED                    (120.f)
#define OMM_MARIO_ROLL_DECEL                        (1.5f)
#define OMM_MARIO_ROLL_YAW_VEL                      (0x180)
#define OMM_MARIO_SPIN_DURATION                     (30)
#define OMM_MARIO_SPIN_BUFFER_DURATION              (6)
#define OMM_MARIO_SPIN_MIN_HIT_CHECKPOINTS          (5)
#define OMM_MARIO_SPIN_JUMP_DECEL_INC               (10)
#define OMM_MARIO_SPIN_JUMP_DECEL_DEC               (1)
#define OMM_MARIO_SPIN_JUMP_DECEL_MAX               (40)
#define OMM_MARIO_SHELL_RIDE_MIN_SPEED              (24.f)
#define OMM_MARIO_SHELL_RIDE_BONK_SPEED             (36.f)
#define OMM_MARIO_SHELL_RIDE_MAX_SPEED              (72.f)
#define OMM_MARIO_WING_FLYING_MAX_SPEED             (56.f)
#define OMM_MARIO_WATER_SURFACE_OFFSET              (80.f)
#define OMM_MARIO_WATER_JUMP_SURFACE_OFFSET         (81.5f)
#define OMM_MARIO_WATER_PARTICLES_SURFACE_OFFSET    (130.f)
#define OMM_MARIO_WATER_BREATH_RECOVERY_OFFSET      (140.f)
#define OMM_MARIO_WATER_SHELL_MAX_SPEED             (60.f)
#define OMM_MARIO_WATER_DASH_MAX_SPEED              (56.f)
#define OMM_MARIO_WATER_SUPER_JUMP_SPEED_THRESHOLD  (40.f)
#define OMM_MARIO_WATER_SUPER_JUMP_FRAMES           (3)
#define OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED     (28.f)
#define OMM_MARIO_METAL_WATER_LONG_JUMP_MIN_VEL     (OMM_MARIO_METAL_WATER_WALKING_MAX_SPEED / 4.f)

//
// Attacks
//

#ifndef INT_PUNCH
#define INT_GROUND_POUND_OR_TWIRL                   (1 << 0) // 0x01
#define INT_PUNCH                                   (1 << 1) // 0x02
#define INT_KICK                                    (1 << 2) // 0x04
#define INT_TRIP                                    (1 << 3) // 0x08
#define INT_SLIDE_KICK                              (1 << 4) // 0x10
#define INT_FAST_ATTACK_OR_SHELL                    (1 << 5) // 0x20
#define INT_HIT_FROM_ABOVE                          (1 << 6) // 0x40
#define INT_HIT_FROM_BELOW                          (1 << 7) // 0x80
#define INT_ATTACK_NOT_FROM_BELOW                   (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_SLIDE_KICK | INT_FAST_ATTACK_OR_SHELL | INT_HIT_FROM_ABOVE)
#define INT_ANY_ATTACK                              (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_SLIDE_KICK | INT_FAST_ATTACK_OR_SHELL | INT_HIT_FROM_ABOVE | INT_HIT_FROM_BELOW)
#define INT_ATTACK_NOT_WEAK_FROM_ABOVE              (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_HIT_FROM_BELOW)
#endif

//
// Health
//

#define OMM_HEALTH_STATE_NONE                       (0)
#define OMM_HEALTH_STATE_HEAL                       (1)
#define OMM_HEALTH_STATE_DAMAGE                     (2)
#define OMM_HEALTH_STATE_LIFE_UP                    (3)

#define OMM_HEALTH_CLASSIC_MAX                      (0x880)
#define OMM_HEALTH_CLASSIC_DEAD                     (0xFF)
#define OMM_HEALTH_CLASSIC_1_SEGMENT                (0x180)
#define OMM_HEALTH_CLASSIC_NUM_SEGMENTS             (8)
#define OMM_HEALTH_CLASSIC_PER_SEGMENT              ((OMM_HEALTH_CLASSIC_MAX - OMM_HEALTH_CLASSIC_DEAD) / OMM_HEALTH_CLASSIC_NUM_SEGMENTS)

#define OMM_HEALTH_ODYSSEY_MAX                      (0x87F)
#define OMM_HEALTH_ODYSSEY_DEAD                     (0xFF)
#define OMM_HEALTH_ODYSSEY_1_SEGMENT                (OMM_HEALTH_ODYSSEY_DEAD + OMM_HEALTH_ODYSSEY_PER_SEGMENT)
#define OMM_HEALTH_ODYSSEY_3_SEGMENTS               (OMM_HEALTH_ODYSSEY_DEAD + (OMM_HEALTH_ODYSSEY_PER_SEGMENT * OMM_HEALTH_ODYSSEY_NUM_SEGMENTS_HALF))
#define OMM_HEALTH_ODYSSEY_NUM_SEGMENTS             (6)
#define OMM_HEALTH_ODYSSEY_NUM_SEGMENTS_HALF        (OMM_HEALTH_ODYSSEY_NUM_SEGMENTS / 2)
#define OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT    (0xA)
#define OMM_HEALTH_ODYSSEY_PER_SEGMENT              ((OMM_HEALTH_ODYSSEY_MAX - OMM_HEALTH_ODYSSEY_DEAD) / OMM_HEALTH_ODYSSEY_NUM_SEGMENTS)
#define OMM_HEALTH_ODYSSEY_PER_TICK                 (OMM_HEALTH_ODYSSEY_PER_SEGMENT / OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT)
#define OMM_HEALTH_ODYSSEY_NUM_COINS_PER_HEAL       (10)

#define OMM_HEALTH_MAX                              (OMM_MOVESET_ODYSSEY ? OMM_HEALTH_ODYSSEY_MAX : OMM_HEALTH_CLASSIC_MAX)
#define OMM_HEALTH_DEAD                             (OMM_MOVESET_ODYSSEY ? OMM_HEALTH_ODYSSEY_DEAD : OMM_HEALTH_CLASSIC_DEAD)
#define OMM_HEALTH_1_SEGMENT                        (OMM_MOVESET_ODYSSEY ? OMM_HEALTH_ODYSSEY_1_SEGMENT : OMM_HEALTH_CLASSIC_1_SEGMENT)
#define OMM_HEALTH_PER_SEGMENT                      (OMM_MOVESET_ODYSSEY ? OMM_HEALTH_ODYSSEY_PER_SEGMENT : OMM_HEALTH_CLASSIC_PER_SEGMENT)

#define OMM_HEALTH_ANIM_HEAL_MOVE_START             (45)
#define OMM_HEALTH_ANIM_HEAL_MOVE_END               (OMM_HEALTH_ANIM_HEAL_MOVE_START + 15)

#define OMM_HEALTH_ANIM_DAMAGE_SHAKE_END            (OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT)
#define OMM_HEALTH_ANIM_DAMAGE_MOVE_START           (45)
#define OMM_HEALTH_ANIM_DAMAGE_MOVE_END             (OMM_HEALTH_ANIM_DAMAGE_MOVE_START + 15)
#define OMM_HEALTH_ANIM_DAMAGE_SHAKE_Y              (OMM_RENDER_POWER_SIZE * 0.8f)
#define OMM_HEALTH_ANIM_DAMAGE_SHAKE_FREQ           (0x8000)

#define OMM_HEALTH_ANIM_LIFE_UP_MOVE_START          (OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT * 3)
#define OMM_HEALTH_ANIM_LIFE_UP_MOVE_END            (OMM_HEALTH_ANIM_LIFE_UP_MOVE_START + 15)
#define OMM_HEALTH_ANIM_LIFE_UP_SHAKE_END           (OMM_HEALTH_ANIM_LIFE_UP_MOVE_END + 16)
#define OMM_HEALTH_ANIM_LIFE_UP_SHAKE_X             (OMM_RENDER_POWER_SIZE * 0.3f)
#define OMM_HEALTH_ANIM_LIFE_UP_SHAKE_Y             (OMM_RENDER_POWER_SIZE * 0.6f)
#define OMM_HEALTH_ANIM_LIFE_UP_SHAKE_FREQ          (0x4000)

//
// Breath
//

#define OMM_BREATH_MAX_DURATION                     (1200) // 40 seconds (1200 frames) in water, ~13.3 seconds (400 frames) in poison gas, 10 seconds (300 frames) when water-dashing
#define OMM_BREATH_HEAL_INC                         (30)
#define OMM_BREATH_WATER_DEC                        (1)
#define OMM_BREATH_WATER_POUND_DEC                  (6)
#define OMM_BREATH_WATER_DASH_DEC                   (4)
#define OMM_BREATH_POISON_DEC                       (3)
#define OMM_BREATH_REFILL                           (OMM_BREATH_MAX_DURATION / OMM_BREATH_HEAL_INC)

//
// Freezing water
//

#define OMM_FREEZING_WATER_INC                      (2)
#define OMM_FREEZING_WATER_INC_OUT_OF_BREATH        (4)
#define OMM_FREEZING_WATER_DEC_AIR                  (1)
#define OMM_FREEZING_WATER_DEC_GROUND               (3)
#define OMM_FREEZING_WATER_DURATION                 (150) // 5 seconds
#define OMM_FREEZING_WATER_TIMER_MAX                (OMM_FREEZING_WATER_DURATION * OMM_FREEZING_WATER_INC)
#define OMM_FREEZING_WATER_TIMER_CRITICAL           ((OMM_FREEZING_WATER_DURATION * OMM_FREEZING_WATER_INC * 7) / 10)

//
// Caps
//

#define OMM_IMPROVED_WING_CAP_DURATION              (1800) // 60 seconds
#define OMM_IMPROVED_VANISH_CAP_DURATION            (1200) // 40 seconds
#define OMM_IMPROVED_METAL_CAP_DURATION             (900)  // 30 seconds

//
// Actions
//

#define BUTTONS_PRESSED(_buttons_)                  ((m->controller->buttonPressed & (_buttons_)) == (_buttons_))
#define BUTTONS_DOWN(_buttons_)                     ((m->controller->buttonDown & (_buttons_)) == (_buttons_))
#define BUTTONS_RELEASED(_buttons_)                 ((m->controller->buttonDown & (_buttons_)) == 0)
#define ANM(_anim_, _speed_)                        { obj_anim_play(m->marioObj, _anim_, _speed_); }
#define SFX(_sound_)                                { obj_play_sound(m->marioObj, (_sound_)); }
#define PFX(_particles_)                            { m->particleFlags |= (_particles_); }
#define NO_RETURN
#define RETURN_BREAK                                return OMM_MARIO_ACTION_RESULT_BREAK
#define RETURN_CANCEL                               return OMM_MARIO_ACTION_RESULT_CANCEL
#define RETURN_CONTINUE                             return OMM_MARIO_ACTION_RESULT_CONTINUE

// Init an action
// Set Mario's forward and upwards velocities, spawn some particles,
// play a sound effect, and set specific values depending on his action
#define action_init(forwardVel, upwardsVel, particles, soundBits, ...) \
if (m->actionState == 0) { \
    __VA_ARGS__ \
    mario_set_forward_vel(m, forwardVel); \
    m->vel[1] = (upwardsVel) * omm_player_physics_get_selected_jump(); \
    PFX(particles); \
    SFX(soundBits); \
    m->actionState = 1; \
}

// Set Mario's forward velocity, spawn some particles,
// play a sound effect, and set specific values depending on his action
#define action_init_no_y_vel(forwardVel, particles, soundBits, ...) \
if (m->actionState == 0) { \
    __VA_ARGS__ \
    mario_set_forward_vel(m, forwardVel); \
    PFX(particles); \
    SFX(soundBits); \
    m->actionState = 1; \
}

// Change Mario's action and init his next action based on the actionFunc arg
// result can be either NO_RETURN, RETURN_BREAK, RETURN_CANCEL or RETURN_CONTINUE
#define action_set(actionFunc, nextAction, actionArg, buttons, result, ...) \
if (actionFunc(m, nextAction, actionArg, buttons)) { \
    __VA_ARGS__ \
    omm_mario_init_next_action(m); \
    result; \
}

// Trigger an action change based on a condition
#define action_condition(cond, action, actionArg, result, ...) \
if (cond) { \
    action_set(omm_mario_set_action, action, actionArg, 0, result, __VA_ARGS__); \
}

// Trigger an action change if Cappy is thrown
#define action_cappy(cond, action, actionArg, result, ...) \
if (BUTTONS_PRESSED(X_BUTTON) && (cond)) { \
    action_set(omm_mario_throw_cappy, action, actionArg, Z_TRIG | A_BUTTON | B_BUTTON, result, __VA_ARGS__); \
}

// Trigger an action change if Z and A are pressed
#define action_za_pressed(cond, action, actionArg, result, ...) \
if (BUTTONS_PRESSED(A_BUTTON) && BUTTONS_DOWN(Z_TRIG) && (cond)) { \
    action_set(omm_mario_set_action, action, actionArg, Z_TRIG | A_BUTTON, result, __VA_ARGS__); \
}

// Trigger an action change if Z and B are pressed
#define action_zb_pressed(cond, action, actionArg, result, ...) \
if (BUTTONS_PRESSED(B_BUTTON) && BUTTONS_DOWN(Z_TRIG) && (cond)) { \
    action_set(omm_mario_set_action, action, actionArg, Z_TRIG | B_BUTTON, result, __VA_ARGS__); \
}

// Trigger an action change if A is pressed
#define action_a_pressed(cond, action, actionArg, result, ...) \
if (BUTTONS_PRESSED(A_BUTTON) && (cond)) { \
    action_set(omm_mario_set_action, action, actionArg, A_BUTTON, result, __VA_ARGS__); \
}

// Trigger an action change if B is pressed
#define action_b_pressed(cond, action, actionArg, result, ...) \
if (BUTTONS_PRESSED(B_BUTTON) && (cond)) { \
    action_set(omm_mario_set_action, action, actionArg, B_BUTTON, result, __VA_ARGS__); \
}

// Trigger an action change if Z is pressed
#define action_z_pressed(cond, action, actionArg, result, ...) \
if (BUTTONS_PRESSED(Z_TRIG) && (cond)) { \
    action_set(omm_mario_set_action, action, actionArg, Z_TRIG, result, __VA_ARGS__); \
}

// Trigger an action change if a midair spin is performed
// For Peach, the midair spin is replaced by the float/glide
#define action_midair_spin(cond, action, actionArg, result, ...) \
if (BUTTONS_PRESSED(A_BUTTON) && (m->vel[1] <= 0.f) && (cond)) { \
    if (OMM_PLAYER_IS_PEACH && !omm_mario_is_milk(m)) { \
        action_set(omm_mario_set_action, ACT_OMM_PEACH_FLOAT, actionArg, 0, result, __VA_ARGS__); \
    } else if (gOmmMario->midairSpin.timer == 0) { \
        action_set(omm_mario_set_action, action, actionArg, A_BUTTON, result, __VA_ARGS__); \
    } \
}

// Trigger an action change if an air spin is performed
#define action_air_spin(cond, action, actionArg, result, ...) \
if ((gOmmMario->spin.timer != 0) && (m->vel[1] <= 12.f) && (cond)) { \
    action_set(omm_mario_set_action, action, actionArg, 0, result, __VA_ARGS__); \
}

// Trigger an action change if a spin is performed
#define action_spin(cond, action, actionArg, result, ...) \
if ((gOmmMario->spin.timer != 0) && (cond)) { \
    action_set(omm_mario_set_action, action, actionArg, 0, result, __VA_ARGS__); \
}

// Trigger an action change if Mario leaves the ground
#define action_off_floor(cond, action, actionArg, result, ...) \
if ((m->input & INPUT_OFF_FLOOR) && (cond)) { \
    action_set(omm_mario_set_action, action, actionArg, 0, result, __VA_ARGS__); \
}

// Trigger an action change if the left stick is value is non-zero
#define action_moving(cond, action, actionArg, result, ...) \
if ((m->input & INPUT_NONZERO_ANALOG) && (cond)) { \
    action_set(omm_mario_set_action, action, actionArg, 0, result, __VA_ARGS__); \
}

//
// Groups
//

#define ACT_GROUP_METAL_WATER                       (7 << 6) /* 0x000001C0 */

//
// Action flags
//

#define ACT_FLAG_OPEN_HANDS                         ACT_FLAG_SWIMMING_OR_FLYING
#define ACT_FLAG_ENABLE_HEAD_ROTATION               ACT_FLAG_WATER_OR_TEXT
#define ACT_FLAG_OMM_ACTION                         (1 << 30) /* 0x40000000 */

//
// Stationary
//

#define ACT_OMM_SHOCKED_FROM_HIGH_FALL              (0x00 | ACT_GROUP_STATIONARY | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
#define ACT_OMM_SPIN_POUND_LAND                     (0x01 | ACT_GROUP_STATIONARY | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY)

//
// Moving
//

#define ACT_OMM_ROLL                                (0x00 | ACT_GROUP_MOVING | ACT_FLAG_OMM_ACTION | ACT_FLAG_MOVING | ACT_FLAG_SHORT_HITBOX)
#define ACT_OMM_SPIN_GROUND                         (0x01 | ACT_GROUP_MOVING | ACT_FLAG_OMM_ACTION | ACT_FLAG_MOVING)
#define ACT_OMM_CAPPY_THROW_GROUND                  (0x02 | ACT_GROUP_MOVING | ACT_FLAG_OMM_ACTION | ACT_FLAG_MOVING)

//
// Airborne
//

#define ACT_OMM_CAPPY_BOUNCE                        (0x00 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
#define ACT_OMM_CAPPY_VAULT                         (0x01 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
#define ACT_OMM_GROUND_POUND_JUMP                   (0x02 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
#define ACT_OMM_LEAVE_OBJECT_JUMP                   (0x03 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
#define ACT_OMM_WALL_SLIDE                          (0x04 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR)
#define ACT_OMM_CAPPY_THROW_AIRBORNE                (0x05 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR)
#define ACT_OMM_ROLL_AIR                            (0x06 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_SHORT_HITBOX)
#define ACT_OMM_SPIN_AIR                            (0x07 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
#define ACT_OMM_SPIN_JUMP                           (0x08 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
#define ACT_OMM_SPIN_POUND                          (0x09 | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_ATTACKING)
#define ACT_OMM_MIDAIR_SPIN                         (0x0A | ACT_GROUP_AIRBORNE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)

//
// Submerged
//

#define ACT_OMM_WATER_FIRST_PERSON                  (0x00 | ACT_GROUP_SUBMERGED | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING)
#define ACT_OMM_WATER_GROUND_POUND                  (0x01 | ACT_GROUP_SUBMERGED | ACT_FLAG_OMM_ACTION | ACT_FLAG_SWIMMING)
#define ACT_OMM_WATER_GROUND_POUND_LAND             (0x02 | ACT_GROUP_SUBMERGED | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING)
#define ACT_OMM_WATER_GROUND_POUND_JUMP             (0x03 | ACT_GROUP_SUBMERGED | ACT_FLAG_OMM_ACTION | ACT_FLAG_SWIMMING | ACT_FLAG_OPEN_HANDS)
#define ACT_OMM_WATER_DASH                          (0x04 | ACT_GROUP_SUBMERGED | ACT_FLAG_OMM_ACTION | ACT_FLAG_SWIMMING | ACT_FLAG_OPEN_HANDS)
#define ACT_OMM_CAPPY_THROW_WATER                   (0x05 | ACT_GROUP_SUBMERGED | ACT_FLAG_OMM_ACTION | ACT_FLAG_SWIMMING | ACT_FLAG_OPEN_HANDS)
#define ACT_OMM_LEAVE_OBJECT_WATER                  (0x06 | ACT_GROUP_SUBMERGED | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_OPEN_HANDS | ACT_FLAG_ENABLE_HEAD_ROTATION)
#define ACT_OMM_GRABBED_WATER                       (0x07 | ACT_GROUP_SUBMERGED | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE | ACT_FLAG_SWIMMING | ACT_FLAG_OPEN_HANDS)

//
// Cutscene
//

#define ACT_OMM_POSSESSION                          (0x00 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
#define ACT_OMM_POSSESSION_UNDERWATER               (0x00 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE | ACT_FLAG_METAL_WATER) // Needed for the camera to work properly underwater
#define ACT_OMM_STAR_DANCE                          (0x01 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
#define ACT_OMM_DEATH                               (0x02 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
#define ACT_OMM_DEATH_WATER                         (0x03 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_SWIMMING | ACT_FLAG_OPEN_HANDS)
#define ACT_OMM_DEATH_FALL                          (0x04 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
#define ACT_OMM_DEATH_FROZEN                        (0x05 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_SWIMMING | ACT_FLAG_OPEN_HANDS)
#define ACT_OMM_DEATH_EATEN_BY_BUBBA                (0x06 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
#define ACT_OMM_DEATH_QUICKSAND                     (0x07 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
#define ACT_OMM_DEATH_SQUISHED                      (0x08 | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
#define ACT_OMM_SPARKLY_STAR_DANCE                  (0x0A | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
#define ACT_OMM_WARPING                             (0x0B | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_INTANGIBLE)
#if OMM_GAME_IS_R96X
#define ACT_OMM_MILK_POWER_UP                       (0x0C | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
#define ACT_OMM_MILK_POWER_DOWN                     (0x0D | ACT_GROUP_CUTSCENE | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
#endif

//
// Automatic
//

#define ACT_OMM_GRAB_BOWSER                         (0x00 | ACT_GROUP_AUTOMATIC | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
#define ACT_OMM_HOLD_BOWSER                         (0x01 | ACT_GROUP_AUTOMATIC | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
#define ACT_OMM_RELEASE_BOWSER                      (0x02 | ACT_GROUP_AUTOMATIC | ACT_FLAG_OMM_ACTION | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)

//
// Metal water
//

#define ACT_OMM_METAL_WATER_IDLE                    (0x00 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_FIRST_PERSON            (0x01 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_WALKING                 (0x02 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_MOVING)
#define ACT_OMM_METAL_WATER_START_CROUCHING         (0x03 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_CROUCHING               (0x04 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_STOP_CROUCHING          (0x05 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_START_CRAWLING          (0x06 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_CRAWLING                (0x07 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_MOVING)
#define ACT_OMM_METAL_WATER_STOP_CRAWLING           (0x08 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_JUMP                    (0x09 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_CONTROL_JUMP_HEIGHT)
#define ACT_OMM_METAL_WATER_JUMP_LAND               (0x0A | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_DOUBLE_JUMP             (0x0B | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_CONTROL_JUMP_HEIGHT)
#define ACT_OMM_METAL_WATER_DOUBLE_JUMP_LAND        (0x0C | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_TRIPLE_JUMP             (0x0D | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_TRIPLE_JUMP_LAND        (0x0E | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_BACKFLIP                (0x0F | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_BACKFLIP_LAND           (0x10 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_SIDE_FLIP               (0x11 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_SIDE_FLIP_LAND          (0x12 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_LONG_JUMP               (0x13 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_LONG_JUMP_LAND          (0x14 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_FREEFALL                (0x15 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_FREEFALL_LAND           (0x16 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_WALL_SLIDE              (0x17 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_WALL_KICK_AIR           (0x18 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_CONTROL_JUMP_HEIGHT)
#define ACT_OMM_METAL_WATER_PUNCHING                (0x19 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_ATTACKING)
#define ACT_OMM_METAL_WATER_JUMP_KICK               (0x1A | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_ATTACKING)
#define ACT_OMM_METAL_WATER_DIVE                    (0x1B | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_DIVING | ACT_FLAG_ATTACKING)
#define ACT_OMM_METAL_WATER_GROUND_POUND            (0x1C | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_ATTACKING)
#define ACT_OMM_METAL_WATER_GROUND_POUND_LAND       (0x1D | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_ATTACKING)
#define ACT_OMM_METAL_WATER_GROUND_POUND_LAND_STOP  (0x1E | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY)
#define ACT_OMM_METAL_WATER_GROUND_POUND_JUMP       (0x1F | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_BACKWARD_GROUND_KB      (0x20 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_INVULNERABLE)
#define ACT_OMM_METAL_WATER_BACKWARD_AIR_KB         (0x21 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE)
#define ACT_OMM_METAL_WATER_CAPPY_THROW_GROUND      (0x22 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_MOVING)
#define ACT_OMM_METAL_WATER_CAPPY_THROW_AIRBORNE    (0x23 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_CAPPY_BOUNCE            (0x24 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_SPIN_GROUND             (0x25 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_MOVING)
#define ACT_OMM_METAL_WATER_SPIN_AIR                (0x26 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
#define ACT_OMM_METAL_WATER_SPIN_JUMP               (0x27 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
#define ACT_OMM_METAL_WATER_SPIN_POUND              (0x28 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_ATTACKING)
#define ACT_OMM_METAL_WATER_SPIN_POUND_LAND         (0x29 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY)
#define ACT_OMM_METAL_WATER_HOLD_IDLE               (0x2A | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_HOLD_WALKING            (0x2B | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_MOVING)
#define ACT_OMM_METAL_WATER_HOLD_JUMP               (0x2C | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR | ACT_FLAG_CONTROL_JUMP_HEIGHT)
#define ACT_OMM_METAL_WATER_HOLD_JUMP_LAND          (0x2D | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#define ACT_OMM_METAL_WATER_HOLD_FREEFALL           (0x2E | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#define ACT_OMM_METAL_WATER_HOLD_FREEFALL_LAND      (0x2F | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
#if OMM_GAME_IS_R96X
#define ACT_OMM_METAL_WATER_WARIO_CHARGE            (0x30 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_MOVING | ACT_FLAG_ATTACKING | ACT_FLAG_INVULNERABLE)
#define ACT_OMM_METAL_WATER_WARIO_TRIPLE_JUMP       (0x31 | ACT_GROUP_METAL_WATER | ACT_FLAG_OMM_ACTION | ACT_FLAG_METAL_WATER | ACT_FLAG_AIR)
#endif

//
// SMO Animations (credits: Sharen#1569)
//

#define MARIO_ANIM_OMM_CAPPY_VAULT                  (0xD1)
#define MARIO_ANIM_OMM_CAPPY_UP_THROW               (0xD2)
#define MARIO_ANIM_OMM_CAPPY_DOWN_THROW             (0xD3)
#define MARIO_ANIM_OMM_CAPPY_SPIN_THROW             (0xD4)
#define MARIO_ANIM_OMM_CAPPY_THROW                  (0xD5)
#define MARIO_ANIM_OMM_CAPPY_RAINBOW_SPIN           (0xD6) // Cappy Twirl but by it's actual SMO name
#define MARIO_ANIM_OMM_DROWN                        (0xD7)
#define MARIO_ANIM_OMM_DEATH                        (0xD8)
#define MARIO_ANIM_OMM_FREEZE                       (0xD9)
#define MARIO_ANIM_OMM_STARDANCE                    (0xDA)

#endif // OMM_MARIO_ACTIONS_H

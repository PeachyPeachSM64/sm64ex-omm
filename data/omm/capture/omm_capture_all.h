#ifndef OMM_CAPTURE_ALL_H
#define OMM_CAPTURE_ALL_H

#include "data/omm/omm_includes.h"

//
// Possessed object shortcuts
//

#define POBJ_RETURN_OK                          return 0
#define POBJ_RETURN_UNPOSSESS                   return 1
#define POBJ_RETURN_RETRY                       return 2
#define POBJ_STOP_IF_UNPOSSESSED                { if (gMarioState->action != ACT_OMM_POSSESSION) { POBJ_RETURN_UNPOSSESS; } }

#define POBJ_RESULT_NONE                        0
#define POBJ_RESULT_JUMP_START                  1
#define POBJ_RESULT_JUMP_HOLD                   2
#define POBJ_RESULT_HOP_SMALL                   3
#define POBJ_RESULT_HOP_LARGE                   4

#define POBJ_A_BUTTON_PRESSED                   (gOmmData->mario->capture.buttonPressed & A_BUTTON)
#define POBJ_A_BUTTON_DOWN                      (gOmmData->mario->capture.buttonDown & A_BUTTON)
#define POBJ_B_BUTTON_PRESSED                   (gOmmData->mario->capture.buttonPressed & B_BUTTON)
#define POBJ_B_BUTTON_DOWN                      (gOmmData->mario->capture.buttonDown & B_BUTTON)

#define POBJ_GROUND_SPEED_MULTIPLIER            (1.f / omm_player_get_selected_ground_speed_multiplier())
#define POBJ_AIR_SPEED_MULTIPLIER               (1.f / omm_player_get_selected_air_speed_multiplier())
#define POBJ_JUMP_MULTIPLIER                    (1.f / sqrtf(omm_player_get_selected_jump_multiplier()))

#define POBJ_SET_ABOVE_WATER                    gOmmData->object->state.properties |= (1 << 0)
#define POBJ_SET_UNDER_WATER                    gOmmData->object->state.properties |= (1 << 1)
#define POBJ_SET_INVULNERABLE                   gOmmData->object->state.properties |= (1 << 2)
#define POBJ_SET_IMMUNE_TO_FIRE                 gOmmData->object->state.properties |= (1 << 3)
#define POBJ_SET_IMMUNE_TO_LAVA                 gOmmData->object->state.properties |= (1 << 4)
#define POBJ_SET_IMMUNE_TO_SAND                 gOmmData->object->state.properties |= (1 << 5)
#define POBJ_SET_IMMUNE_TO_WIND                 gOmmData->object->state.properties |= (1 << 6)
#define POBJ_SET_ABLE_TO_MOVE_ON_WATER          gOmmData->object->state.properties |= (1 << 7)
#define POBJ_SET_ABLE_TO_MOVE_ON_SLOPES         gOmmData->object->state.properties |= (1 << 8)
#define POBJ_SET_ABLE_TO_MOVE_THROUGH_WALLS     gOmmData->object->state.properties |= (1 << 9)
#define POBJ_SET_ATTACKING                      gOmmData->object->state.properties |= (1 << 10)

#define POBJ_IS_ABOVE_WATER                     ((gOmmData->object->state.properties & (1 << 0)) != 0)
#define POBJ_IS_UNDER_WATER                     ((gOmmData->object->state.properties & (1 << 1)) != 0)
#define POBJ_IS_INVULNERABLE                    ((gOmmData->object->state.properties & (1 << 2)) != 0)
#define POBJ_IS_IMMUNE_TO_FIRE                  ((gOmmData->object->state.properties & (1 << 3)) != 0)
#define POBJ_IS_IMMUNE_TO_LAVA                  ((gOmmData->object->state.properties & (1 << 4)) != 0)
#define POBJ_IS_IMMUNE_TO_SAND                  ((gOmmData->object->state.properties & (1 << 5)) != 0)
#define POBJ_IS_IMMUNE_TO_WIND                  ((gOmmData->object->state.properties & (1 << 6)) != 0)
#define POBJ_IS_ABLE_TO_MOVE_ON_WATER           ((gOmmData->object->state.properties & (1 << 7)) != 0)
#define POBJ_IS_ABLE_TO_MOVE_ON_SLOPES          ((gOmmData->object->state.properties & (1 << 8)) != 0)
#define POBJ_IS_ABLE_TO_MOVE_THROUGH_WALLS      ((gOmmData->object->state.properties & (1 << 9)) != 0)
#define POBJ_IS_ATTACKING                       ((gOmmData->object->state.properties & (1 << 10)) != 0)

#define POBJ_STEP_FLAGS                         (OBJ_STEP_UPDATE_HOME | (OBJ_STEP_MOVE_THROUGH_WALLS * POBJ_IS_ABLE_TO_MOVE_THROUGH_WALLS) | (OBJ_STEP_STICKY_FEET * POBJ_IS_ABLE_TO_MOVE_ON_SLOPES) | OBJ_STEP_CHECK_ON_GROUND)

#define POBJ_INTERACTIONS(...)                                                                         \
    if ((gOmmData->object->state.invincTimer-- <= 0) && !omm_mario_is_locked(gMarioState)) {           \
        for_each_object_in_interaction_lists(obj) {                                                    \
            if (obj != o) {                                                                            \
                __VA_ARGS__;                                                                           \
                u32 interactType = obj->oInteractType;                                                 \
                if (!(obj->oInteractStatus & INT_STATUS_INTERACTED)) {                                 \
                    pobj_process_interaction(o, obj, interactType);                                    \
                }                                                                                      \
            }                                                                                          \
        }                                                                                              \
    }

//
// Possessed object stuff
//

void pobj_move(struct Object *o, bool run, bool dash, bool stop);
void pobj_move_3d(struct Object *o, bool run, bool dash);
s32  pobj_jump(struct Object *o, f32 hopDiv, s32 numMaxJumps);
void pobj_decelerate(struct Object *o, f32 decel, f32 decelIce);
void pobj_apply_gravity(struct Object *o, f32 mult);
void pobj_handle_special_floors(struct Object *o);
bool pobj_process_interaction(struct Object *o, struct Object *obj, u32 interactType);

//
// Capture data
//

const void *omm_capture_get_data(struct Object *o);
bool omm_capture_init(struct Object *o);
void omm_capture_end(struct Object *o);
s32  omm_capture_update(struct Object *o);
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
void omm_capture_set_camera_behind_mario();
void omm_capture_reset_camera();

//
// Behaviors
//

#define CAPTURE_BEHAVIOR(name)                  \
bool cappy_##name##_init(struct Object *o);     \
void cappy_##name##_end(struct Object *o);      \
s32 cappy_##name##_update(struct Object *o);    \
f32 cappy_##name##_get_top(struct Object *o)

// Bob-omb Battlefield
CAPTURE_BEHAVIOR(goomba);               // bhvGoomba
CAPTURE_BEHAVIOR(koopa);                // bhvKoopa
CAPTURE_BEHAVIOR(koopa_shell);          // bhvKoopaShell
CAPTURE_BEHAVIOR(bobomb);               // bhvBobomb
CAPTURE_BEHAVIOR(npc_message);          // bhvBobombBuddy
CAPTURE_BEHAVIOR(bobomb_buddy);         // bhvBobombBuddyOpensCannon
CAPTURE_BEHAVIOR(chain_chomp);          // bhvChainChomp, bhvChainChompFreed

// Whomp Fortress
CAPTURE_BEHAVIOR(bullet_bill);          // bhvBulletBill
CAPTURE_BEHAVIOR(hoot);                 // bhvHoot
CAPTURE_BEHAVIOR(whomp);                // bhvSmallWhomp

// Jolly Roger Bay

// Cool, Cool Mountain
CAPTURE_BEHAVIOR(spindrift);            // bhvSpindrift
CAPTURE_BEHAVIOR(penguin_small);        // bhvPenguinBaby, bhvSmallPenguin
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

// Lethal Lava Land
CAPTURE_BEHAVIOR(bully);                // bhvSmallBully
CAPTURE_BEHAVIOR(motos);                // bhvMotos
CAPTURE_BEHAVIOR(blargg);               // bhvBlargg
CAPTURE_BEHAVIOR(friendly_blargg);      // bhvFriendlyBlargg

// Shifting Sand Land
CAPTURE_BEHAVIOR(fly_guy);              // bhvFlyGuy
CAPTURE_BEHAVIOR(pokey);                // bhvPokey, bhvPokeyBodyPart
CAPTURE_BEHAVIOR(amp);                  // bhvHomingAmp, bhvCirclingAmp
CAPTURE_BEHAVIOR(klepto);               // bhvKlepto

// Dire, Dire Docks
CAPTURE_BEHAVIOR(fire_spitter);         // bhvFireSpitter
CAPTURE_BEHAVIOR(sushi_shark);          // bhvSushiShark
// underwater shell?

// Snowman's Land
CAPTURE_BEHAVIOR(moneybag);             // bhvMoneybag, bhvMoneybagHidden
CAPTURE_BEHAVIOR(sl_walking_penguin);   // bhvSLWalkingPenguin

// Wet-Dry World
CAPTURE_BEHAVIOR(skeeter);              // bhvSkeeter
CAPTURE_BEHAVIOR(heave_ho);             // bhvHeaveHo
CAPTURE_BEHAVIOR(chuckya);              // bhvChuckya

// Tall, Tall Mountain
CAPTURE_BEHAVIOR(ukiki);                // bhvUkiki, bhvMacroUkiki
//extern const BehaviorScript bhvCloud[];
//extern const BehaviorScript bhvCloudPart[];

// Tiny-Huge Island
CAPTURE_BEHAVIOR(enemy_lakitu);         // bhvEnemyLakitu
CAPTURE_BEHAVIOR(spiny);                // bhvSpiny

// Tick Tock Clock

// Rainbow Ride

// Misc
CAPTURE_BEHAVIOR(toad);                 // bhvToadMessage
CAPTURE_BEHAVIOR(mips);                 // bhvMips
CAPTURE_BEHAVIOR(yoshi);                // bhvYoshi

// OMM Bowser
CAPTURE_BEHAVIOR(flaming_bobomb);       // omm_bhv_flaming_bobomb

#undef CAPTURE_BEHAVIOR
#endif // OMM_CAPTURE_ALL_H

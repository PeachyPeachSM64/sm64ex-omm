#ifndef OMM_BEHAVIOR_DATA_H
#define OMM_BEHAVIOR_DATA_H

#include "types.h"

//
// Constants
//

#define BHV_TYPE_COIN                               (1llu << 0) // Coin
#define BHV_TYPE_WATER_RING                         (1llu << 1) // Water ring
#define BHV_TYPE_MUSHROOM_1UP                       (1llu << 2) // Mushroom 1up
#define BHV_TYPE_STAR_OR_KEY                        (1llu << 3) // Interactable Star or key
#define BHV_TYPE_CAP                                (1llu << 4) // Cap object
#define BHV_TYPE_SECRET                             (1llu << 5) // Secret
#define BHV_TYPE_WEAK                               (1llu << 6) // Vulnerable to weak attacks
#define BHV_TYPE_STRONG                             (1llu << 7) // Vulnerable to strong attacks (snowballs, beams)
#define BHV_TYPE_DESTRUCTIBLE                       (1llu << 8) // Vulnerable to destructible attacks (explosions)
#define BHV_TYPE_BREAKABLE                          (1llu << 9) // Fragile/Breakable
#define BHV_TYPE_BOSS                               (1llu << 10) // Boss flag, affected by OBJ_INT_ATTACK_ONE_HIT
#define BHV_TYPE_INVULNERABLE                       (1llu << 11) // Invulnerable enemies
#define BHV_TYPE_GRABBABLE                          (1llu << 12) // Grabbable
#define BHV_TYPE_BULLY                              (1llu << 13) // Bully-type
#define BHV_TYPE_FLAME                              (1llu << 14) // Flame-type objects
#define BHV_TYPE_NO_CAPPY                           (1llu << 15) // Intangible to Cappy
#define BHV_TYPE_KICKABLE_BOARD                     (1llu << 16) // Kickable board
#define BHV_TYPE_WF_STAR_WALL                       (1llu << 17) // Whomp Fortress Star inside wall
#define BHV_TYPE_EXCLAMATION_BOX                    (1llu << 18) // Yellow exclamation box
#define BHV_TYPE_UNAGIS_TAIL                        (1llu << 19) // Unagi's tail
#define BHV_TYPE_WATER_DIAMOND                      (1llu << 20) // WDW Water diamond
#define BHV_TYPE_INTERACTABLE                       (BHV_TYPE_UNIMPORTANT - 1)
#define BHV_TYPE_COLLECTIBLE                        (BHV_TYPE_COIN | BHV_TYPE_WATER_RING | BHV_TYPE_MUSHROOM_1UP | BHV_TYPE_STAR_OR_KEY | BHV_TYPE_CAP | BHV_TYPE_SECRET)
#define BHV_TYPE_ENEMY                              (BHV_TYPE_WEAK | BHV_TYPE_STRONG | BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_BOSS | BHV_TYPE_BULLY)

#define BHV_TYPE_UNIMPORTANT                        (1llu << 21) // Unimportant object to unload if out of object slots
#define BHV_TYPE_STAR_MODEL                         (1llu << 22) // Star or key model
#define BHV_TYPE_NO_CAM_COL                         (1llu << 23) // No camera collision
#define BHV_TYPE_GOOMBA                             (1llu << 24) // Goomba-like behavior
#define BHV_TYPE_BOO                                (1llu << 25) // Boo-like behavior
#define BHV_TYPE_METAL_BALL                         (1llu << 26) // Balls of steel
#define BHV_TYPE_DOOR                               (1llu << 27) // Door, reject these behaviors when the camera checks for pillars
#define BHV_TYPE_TREASURE_CHEST                     (1llu << 28) // Treasure chest, cannot be moved by other objects
#define BHV_TYPE_PLAYER                             (1llu << 29) // Player behavior
#define BHV_TYPE_BOWSER                             (1llu << 30) // Bowser behavior
#define BHV_TYPE_NOT_ENEMY                          (1llu << 31) // Doesn't count as an enemy
#define BHV_TYPE_SPARKLY_ENEMY                      (1llu << 32) // Enemy behavior for Sparkly Stars missions
#define BHV_TYPE_METAL_DESTRUCTIBLE                 (1llu << 33) // Destroyed by touching Metal Mario
#define BHV_TYPE_DEFEATED_IF_DESTROYED              (1llu << 34) // Count as defeated only if destroyed

#define BOSS_TYPE_KING_BOBOMB                       (0)
#define BOSS_TYPE_KING_WHOMP                        (1)
#define BOSS_TYPE_BIG_BOO                           (2)
#define BOSS_TYPE_BIG_BULLY                         (3)
#define BOSS_TYPE_EYEROK_HAND                       (4)
#define BOSS_TYPE_WIGGLER                           (5)
#define BOSS_TYPE_BOWSER                            (6)

#define HOLD_ACTIONS_ALL                            (0)
#define HOLD_ACTIONS_0                              (1) // 0
#define HOLD_ACTIONS_1                              (2) // 1
#define HOLD_ACTIONS_KING_BOBOMB                    (3) // 1, 2, 6
#define HOLD_ANIM_NONE                              (-1)
#define HOLD_ANIM_FREEZE                            (0)
#define HOLD_ANIM_SET_FRAME(x)                      (-(x)-1)
#define HOLD_HIT_FLOOR                              (1 << 0)
#define HOLD_HIT_WALL                               (1 << 1)
#define HOLD_HIT_WATER                              (1 << 2)
#define HOLD_HIT_FLOOR_OR_WALL                      (HOLD_HIT_FLOOR | HOLD_HIT_WALL)
#define HOLD_HIT_FLOOR_OR_WATER                     (HOLD_HIT_FLOOR | HOLD_HIT_WATER)
#define HOLD_HIT_ANY                                (HOLD_HIT_FLOOR | HOLD_HIT_WALL | HOLD_HIT_WATER)
#define HOLD_ACT_SPECIAL                            (99)
#define HOLD_ACT_ATTACKED                           (1000)
#define HOLD_ACT_ATTACKED_BIG_BOO                   (1001)
#define HOLD_ACT_DESTROY                            (1002)
#define HOLD_ACT_BOUNCE                             (1003)
#define HOLD_ACT_BOUNCE_TOAD                        (1004)
#define HOLD_ACT_BOUNCE_MIPS                        (1005)
#define HOLD_ACT_BOUNCE_SPARKLY_STAR_BOX            (1006)

#define YOSHI_TONGUE_TYPE_DEFAULT                   (0) // Eatable, throwable
#define YOSHI_TONGUE_TYPE_EATABLE                   (1) // Eatable only
#define YOSHI_TONGUE_TYPE_THROWABLE                 (2) // Throwable only
#define YOSHI_TONGUE_TYPE_FIRE                      (3) // Spits fire spitter fireball if eaten/thrown
#define YOSHI_TONGUE_TYPE_COIN                      (4) // Coin, collected immediately
#define YOSHI_TONGUE_TYPE_MUSHROOM_1UP              (5) // Mushroom 1up, eaten immediately, no egg
#define YOSHI_TONGUE_TYPE_STAR_OR_KEY               (6) // Star or key, eaten immediately, no egg
#define YOSHI_TONGUE_TYPE_CAP                       (7) // Cap, eaten immediately, no egg
#define YOSHI_TONGUE_TYPE_SECRET                    (8) // Secret, collected immediately
#define YOSHI_TONGUE_TYPE_ATTACK                    (9) // Attack object when colliding with the tip of the tongue
#define YOSHI_TONGUE_TYPE_DESTROY                   (10) // Destroyed immediately

#define DESTROY_TYPE_WHITE_PUFF                     (0)
#define DESTROY_TYPE_WHITE_PUFF_AND_CHILDREN        (1)
#define DESTROY_TYPE_BREAK_PARTICLES                (2)
#define DESTROY_TYPE_TRIANGLE_PARTICLES             (3)
#define DESTROY_TYPE_SNOW_PARTICLES                 (4)
#define DESTROY_TYPE_EXPLOSION                      (5)
#define DESTROY_TYPE_EXPLOSION_AND_RESPAWN          (6)
#define DESTROY_TYPE_WATER_BOMB                     (7)
#define DESTROY_TYPE_WHOMP_KING                     (8)
#define DESTROY_TYPE_BIG_SNOWBALL                   (9)
#define DESTROY_TYPE_BOO                            (10)
#define DESTROY_TYPE_BIG_MR_I                       (11)
#define DESTROY_TYPE_BULLY                          (12)
#define DESTROY_TYPE_POKEY                          (13)
#define DESTROY_TYPE_EYEROK_HAND                    (14)
#define DESTROY_TYPE_UKIKI_CAGE                     (15)
#define DESTROY_TYPE_FIRE_PIRANHA_PLANT             (16)
#define DESTROY_TYPE_WOODEN_POST                    (17)
#define DESTROY_COND_NONE                           (0)
#define DESTROY_COND_REGULAR_GOOMBA                 (1)
#define DESTROY_COND_TINY_GOOMBA                    (2)
#define DESTROY_COND_HUGE_GOOMBA                    (3)
#define DESTROY_COND_BIG_MR_I                       (4)
#define DESTROY_COINS_BLUE                          (-1)
#define DESTROY_COINS_BLUE_ONCE                     (-2)
#define DESTROY_COINS_REMAINING                     (-3)

//
// Structs
//

struct OmmArray;

typedef struct {
    s32 type;
} OmmBhvDataBoss;

typedef struct {
    s32 vibeSparkles;
    s32 deathAction;
} OmmBhvDataPerryAttack;

typedef struct {
    s32 actions;
    s32 heldAnim;
    f32 heldAnimAccel;
    s32 thrownAnim;
    f32 thrownAnimAccel;
    s32 hitSurface;
    s32 nextAction;
    u32 interactionFlags;
} OmmBhvDataHoldable;

typedef struct {
    f32 hitboxRadius;
    f32 hitboxHeight;
    f32 hurtboxRadius;
    f32 hurtboxHeight;
    f32 hitboxDownOffset;
    bool scale;
} OmmBhvDataHitbox;

typedef struct {
    bool (*init)(struct Object *);
    s32 (*update)(struct Object *);
    void (*update_gfx)(struct Object *);
    void (*end)(struct Object *);
    u64 (*get_type)(struct Object *);
    f32 (*get_top)(struct Object *);
    f32 walkSpeed;
    f32 runSpeed;
    f32 dashSpeed;
    f32 jumpVelocity;
    f32 terminalVelocity;
    f32 gravity;
    f32 hitboxRadius;
    f32 hitboxHeight;
    f32 hitboxDownOffset;
    f32 wallHitboxRadius;
    f32 (*scale_modifier)(f32);
    bool referenceObject; // if true, reference the object as previous capture after leaving it
} OmmBhvDataCapture;

typedef struct {
    s32 type;
} OmmBhvDataYoshiTongue;

typedef struct {
    s32 type;
    s32 cond;
    s64 args[6];
    void *next;
} OmmBhvDataDestroy;

typedef struct {
    const BehaviorScript *bhv;
#if OMM_CODE_DEBUG
    const GeoLayout *geo;
    const char *bhvName;
#endif
    OmmBhvDataBoss *boss;
    OmmBhvDataPerryAttack *perryAttack;
    OmmBhvDataHoldable *holdable;
    OmmBhvDataHitbox *hitbox;
    OmmBhvDataCapture *capture;
    OmmBhvDataYoshiTongue *yoshiTongue;
    OmmBhvDataDestroy *destroy;
    u64 types;
} OmmBhvData;

//
// Getters
//

const struct OmmArray *omm_get_behaviors_list();
const OmmBhvData *omm_behavior_data_get(const BehaviorScript *bhv);
const OmmBhvDataBoss *omm_behavior_data_get_boss(const BehaviorScript *bhv);
const OmmBhvDataPerryAttack *omm_behavior_data_get_perry_attack(const BehaviorScript *bhv);
const OmmBhvDataHoldable *omm_behavior_data_get_holdable(const BehaviorScript *bhv);
const OmmBhvDataHitbox *omm_behavior_data_get_hitbox(const BehaviorScript *bhv);
const OmmBhvDataCapture *omm_behavior_data_get_capture(const BehaviorScript *bhv);
const OmmBhvDataYoshiTongue *omm_behavior_data_get_yoshi_tongue(const BehaviorScript *bhv);
const OmmBhvDataDestroy *omm_behavior_data_get_destroy(const BehaviorScript *bhv);
u64 omm_behavior_data_get_types(const BehaviorScript *bhv);
u64 omm_behavior_data_get_capture_type(struct Object *o);

#endif
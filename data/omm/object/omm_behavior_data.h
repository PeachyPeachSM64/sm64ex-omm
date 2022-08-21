#ifndef OMM_BEHAVIOR_DATA_H
#define OMM_BEHAVIOR_DATA_H

#include "types.h"

//
// Constants
//

#define BHV_TYPE_COIN                       (1 << 0) // Coin
#define BHV_TYPE_WATER_RING                 (1 << 1) // Water ring
#define BHV_TYPE_MUSHROOM_1UP               (1 << 2) // Mushroom 1up
#define BHV_TYPE_STAR_OR_KEY                (1 << 3) // Interactible Star or key
#define BHV_TYPE_CAP                        (1 << 4) // Cap object
#define BHV_TYPE_SECRET                     (1 << 5) // Secret
#define BHV_TYPE_WEAK                       (1 << 6) // Vulnerable to weak attacks
#define BHV_TYPE_STRONG                     (1 << 7) // Vulnerable to strong attacks (snowballs, beams)
#define BHV_TYPE_DESTRUCTIBLE               (1 << 8) // Vulnerable to destructible attacks (explosions)
#define BHV_TYPE_BREAKABLE                  (1 << 9) // Fragile/Breakable
#define BHV_TYPE_BOSS                       (1 << 10) // Boss flag, affected by OBJ_INT_ATTACK_ONE_HIT
#define BHV_TYPE_INVULNERABLE               (1 << 11) // Invulnerable enemies
#define BHV_TYPE_GRABBABLE                  (1 << 12) // Grabbable
#define BHV_TYPE_KNOCKABLE_0                (1 << 13) // Knockback 0
#define BHV_TYPE_KNOCKABLE_1                (1 << 14) // Knockback 1
#define BHV_TYPE_GOOMBA                     (1 << 15) // Goomba-like behavior
#define BHV_TYPE_BULLY                      (1 << 16) // Bully-type
#define BHV_TYPE_FLAME                      (1 << 17) // Flame-type objects
#define BHV_TYPE_NO_CAPPY                   (1 << 18) // Intangible to Cappy
#define BHV_TYPE_KICKABLE_BOARD             (1 << 19) // Kickable board
#define BHV_TYPE_WF_STAR_WALL               (1 << 20) // Whomp Fortress Star inside wall
#define BHV_TYPE_EXCLAMATION_BOX            (1 << 21) // Yellow exclamation box
#define BHV_TYPE_UNAGIS_TAIL                (1 << 22) // Unagi's tail
#define BHV_TYPE_INTERACTIBLE               ((1 << 23) - 1)
#define BHV_TYPE_COLLECTIBLE                (BHV_TYPE_COIN | BHV_TYPE_WATER_RING | BHV_TYPE_MUSHROOM_1UP | BHV_TYPE_STAR_OR_KEY | BHV_TYPE_CAP | BHV_TYPE_SECRET)
#define BHV_TYPE_KNOCKABLE                  (BHV_TYPE_KNOCKABLE_0 | BHV_TYPE_KNOCKABLE_1)

#define BHV_TYPE_STAR_MODEL                 (1 << 23) // Star or key model
#define BHV_TYPE_PLAYER                     (1 << 24) // Player behavior
#define BHV_TYPE_BOWSER                     (1 << 25) // Bowser behavior
#define BHV_TYPE_SPARKLY_ENEMY              (1 << 26) // Enemy behavior for Sparkly Stars missions

#define HOLD_ALL_ACTIONS                    -1
#define HOLD_HIT_FLOOR                      (1 << 0)
#define HOLD_HIT_WALL                       (1 << 1)
#define HOLD_HIT_ANY                        (HOLD_HIT_FLOOR | HOLD_HIT_WALL)
#define HOLD_ACT_ATTACKED                   1000
#define HOLD_ACT_DESTROY                    1001
#define HOLD_ACT_BOUNCE                     1002

//
// Structs
//

typedef struct {
    s32 vibeSparkles;
    s32 deathAction;
} OmmBhvDataPerryAttack;

typedef struct {
    s32 actions[8];
    s32 actionsCount;
    s32 heldAnim;
    s32 thrownAnim;
    s32 hitSurface;
    s32 nextAction;
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
    void (*end)(struct Object *);
    f32 (*getTop)(struct Object *);
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
    f32 (*scaleModifier)(f32);
    bool referenceObject; // if true, reference the object as previous capture after leaving it
} OmmBhvDataCapture;

typedef struct {
    const BehaviorScript *bhv;
    OmmBhvDataPerryAttack *perryAttack;
    OmmBhvDataHoldable *holdable;
    OmmBhvDataHitbox *hitbox;
    OmmBhvDataCapture *capture;
    u32 types;
} OmmBhvData;

//
// Getters
//

const BehaviorScript **omm_get_behaviors_list();
const OmmBhvData *omm_behavior_data_get(const BehaviorScript *bhv);
const OmmBhvDataPerryAttack *omm_behavior_data_get_perry_attack(const BehaviorScript *bhv);
const OmmBhvDataHoldable *omm_behavior_data_get_holdable(const BehaviorScript *bhv);
const OmmBhvDataHitbox *omm_behavior_data_get_hitbox(const BehaviorScript *bhv);
const OmmBhvDataCapture *omm_behavior_data_get_capture(const BehaviorScript *bhv);
u32 omm_behavior_data_get_types(const BehaviorScript *bhv);

#endif
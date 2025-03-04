#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

// Bowser animations
// 0 Getting up from belly
// 1 Getting up from belly with small hops
// 2 Grabbed by the tail/Flying
// 3 Start grabbed
// 4 -
// 5 Fall on belly, Stunned?
// 6 Big fire breath
// 7 Jump and land
// 8 Landing
// 9 Jump
// 10 Dance
// 11 Big upwards fire breath
// 12 Idle
// 13 Walk
// 14 Stop walk
// 15 Start walk
// 16 Bombed, land on back
// 17 Defeated
// 18 Start run
// 19 Run
// 20 Stop run (played after braking when speed is 0)
// 21 Braking
// 22 Small fire breath
// 23 Standing on edge
// 24 Standing on edge stop
// 25 Bombed, land on belly
// 26 Getting up after bombed (25)

#define OMM_BOWSER_ANIM_IDLE                        (12)
#define OMM_BOWSER_ANIM_WALK                        (13)
#define OMM_BOWSER_ANIM_JUMP                        (9)
#define OMM_BOWSER_ANIM_LAND                        (8)
#define OMM_BOWSER_ANIM_THROW_ROCK                  (23)
#define OMM_BOWSER_ANIM_RAISE_ARM                   (21)
#define OMM_BOWSER_ANIM_THROW_BOMB                  (20)
#define OMM_BOWSER_ANIM_SURPRISED                   (10)
#define OMM_BOWSER_ANIM_EXHAUSTED                   (5)
#define OMM_BOWSER_ANIM_GET_UP                      (26)
#define OMM_BOWSER_ANIM_DAMAGED_BELLY               (25)
#define OMM_BOWSER_ANIM_DAMAGED_BACK                (16)
#define OMM_BOWSER_ANIM_FURIOUS                     (11)
#define OMM_BOWSER_ANIM_GIVE_UP                     (2)
#define OMM_BOWSER_ANIM_DEFEATED                    (17)
#define OMM_BOWSER_ANIM_THROW_FIRE_BALL             (22)
#define OMM_BOWSER_ANIM_INHALE_AND_THROW            (6)

#define OMM_BOWSER_ACTION_JUMP                      (0)
#define OMM_BOWSER_ACTION_JUMP_MINE                 (1)
#define OMM_BOWSER_ACTION_FIRE_BALLS                (2)
#define OMM_BOWSER_ACTION_BOMBS                     (3)
#define OMM_BOWSER_ACTION_FLAMETHROWER              (4)
#define OMM_BOWSER_ACTION_CLONE_START               (5)
#define OMM_BOWSER_ACTION_CLONE_END                 (6)
#define OMM_BOWSER_ACTION_GET_MAD                   (7)
#define OMM_BOWSER_ACTION_EXHAUSTED                 (8)
#define OMM_BOWSER_ACTION_DAMAGED                   (9)

#define OMM_BOWSER_SUB_ACTION_DEFEATED              (99)

#define OMM_BOWSER_COMBO_ATTACK_AND_THROW_BOMBS     (0)
#define OMM_BOWSER_COMBO_GET_MAD                    (1)
#define OMM_BOWSER_COMBO_ATTACK_UNTIL_EXHAUSTED     (2)
#define OMM_BOWSER_COMBO_DAMAGED                    (3)

//
// Combos
//

typedef struct OmmBowserAction {
    s32 action;
    s32 duration;
    s32 count;
    s32 delay;
} OmmBowserAction;

#define ACTION_JUMP(jumpDuration, numShockwaves, delayBetweenShockwaves)        { OMM_BOWSER_ACTION_JUMP, jumpDuration, numShockwaves, delayBetweenShockwaves }
#define ACTION_JUMP_MINE(jumpDuration, numShockwaves, delayBetweenShockwaves)   { OMM_BOWSER_ACTION_JUMP_MINE, jumpDuration, numShockwaves, delayBetweenShockwaves }
#define ACTION_FIRE_BALLS(numFireBalls, delayBetweenFireBalls, delayDec)        { OMM_BOWSER_ACTION_FIRE_BALLS, delayDec, numFireBalls, delayBetweenFireBalls }
#define ACTION_BOMBS(numBombs, delayBetweenBombs)                               { OMM_BOWSER_ACTION_BOMBS, 0, numBombs, delayBetweenBombs }
#define ACTION_FLAMETHROWER(flameDuration)                                      { OMM_BOWSER_ACTION_FLAMETHROWER, flameDuration, 0, 0 }
#define ACTION_CLONE_START()                                                    { OMM_BOWSER_ACTION_CLONE_START, 0, 0, 0 }
#define ACTION_CLONE_END()                                                      { OMM_BOWSER_ACTION_CLONE_END, 0, 0, 0 }
#define ACTION_GET_MAD()                                                        { OMM_BOWSER_ACTION_GET_MAD, 0, 0, 0 }
#define ACTION_EXHAUSTED(exhaustDuration)                                       { OMM_BOWSER_ACTION_EXHAUSTED, exhaustDuration, 0, 0 }
#define ACTION_DAMAGED()                                                        { OMM_BOWSER_ACTION_DAMAGED, 0, 0, 0 }
#define ACTION_DEFEATED()                                                       { OMM_BOWSER_ACTION_DAMAGED, 0, -1, 0 }

#define ACTION_START()                                                          ACTION_JUMP_MINE(30, 1, 30)
#define ACTION_END_LOOP()                                                       { -1, 0, 0, 0 }
#define ACTION_END_NEXT_COMBO()                                                 { -1, 0, 1, 0 }
#define ACTION_END_NEXT_PHASE()                                                 { -1, 0, 2, 0 }

// Level, Phase, Combo, OmmBowserAction
static const OmmBowserAction OMM_BOWSER_ACTIONS[5][3][4][36] = {

//////////////
// Bowser 1 //
//////////////

{
// Phase 1 //
{
{
ACTION_START(),
ACTION_JUMP(30, 0, 35),
ACTION_BOMBS(1, 45),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(30, 1, 35),
ACTION_JUMP_MINE(30, 2, 30),
ACTION_EXHAUSTED(90),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 2 //
{
{
ACTION_START(),
ACTION_JUMP(30, 1, 35),
ACTION_BOMBS(2, 45),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(30, 1, 30),
ACTION_FIRE_BALLS(3, 45, 0),
ACTION_JUMP_MINE(30, 2, 25),
ACTION_EXHAUSTED(75),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 3 //
{
{
ACTION_START(),
ACTION_JUMP(30, 2, 30),
ACTION_BOMBS(3, 45),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(30, 1, 30),
ACTION_FIRE_BALLS(2, 30, 0),
ACTION_JUMP(30, 2, 25),
ACTION_FIRE_BALLS(3, 30, 10),
ACTION_JUMP_MINE(30, 3, 20),
ACTION_EXHAUSTED(60),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DEFEATED(),
ACTION_END_LOOP(),
},
}
},

//////////////
// Bowser 2 //
//////////////

{
// Phase 1 //
{
{
ACTION_START(),
ACTION_JUMP(30, 1, 20),
ACTION_BOMBS(2, 45),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(30, 1, 20),
ACTION_FIRE_BALLS(2, 25, 0),
ACTION_JUMP_MINE(20, 2, 20),
ACTION_FIRE_BALLS(3, 20, 0),
ACTION_EXHAUSTED(90),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 2 //
{
{
ACTION_START(),
ACTION_JUMP(30, 2, 20),
ACTION_BOMBS(3, 45),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(30, 1, 20),
ACTION_FLAMETHROWER(120),
ACTION_JUMP_MINE(25, 2, 15),
ACTION_EXHAUSTED(75),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 3 //
{
{
ACTION_START(),
ACTION_JUMP(30, 3, 20),
ACTION_BOMBS(4, 40),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(30, 1, 15),
ACTION_FIRE_BALLS(4, 20, 2),
ACTION_JUMP(25, 2, 15),
ACTION_FLAMETHROWER(120),
ACTION_JUMP_MINE(20, 3, 15),
ACTION_EXHAUSTED(60),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DEFEATED(),
ACTION_END_LOOP(),
},
}
},

//////////////
// Bowser 3 //
//////////////

{
// Phase 1 //
{
{
ACTION_START(),
ACTION_JUMP(25, 1, 15),
ACTION_BOMBS(3, 45),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(30, 1, 20),
ACTION_JUMP(27, 1, 20),
ACTION_JUMP(24, 1, 20),
ACTION_JUMP(21, 1, 20),
ACTION_JUMP(18, 1, 20),
ACTION_JUMP(15, 1, 20),
ACTION_JUMP(12, 1, 20),
ACTION_JUMP_MINE(9, 5, 15),
ACTION_EXHAUSTED(90),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 2 //
{
{
ACTION_START(),
ACTION_JUMP(25, 3, 12),
ACTION_BOMBS(4, 40),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(25, 1, 20),
ACTION_FIRE_BALLS(3, 25, 5),
ACTION_JUMP(20, 2, 20),
ACTION_FIRE_BALLS(4, 22, 4),
ACTION_JUMP_MINE(15, 3, 15),
ACTION_FIRE_BALLS(5, 20, 3),
ACTION_EXHAUSTED(75),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 3 //
{
{
ACTION_START(),
ACTION_JUMP(20, 5, 8),
ACTION_BOMBS(5, 35),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(25, 1, 20),
ACTION_FLAMETHROWER(80),
ACTION_JUMP(20, 2, 20),
ACTION_FLAMETHROWER(100),
ACTION_JUMP(15, 3, 20),
ACTION_FLAMETHROWER(120),
ACTION_JUMP_MINE(10, 4, 15),
ACTION_FIRE_BALLS(12, 30, 2),
ACTION_EXHAUSTED(60),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DEFEATED(),
ACTION_END_LOOP(),
},
}
},

//////////////
// Bowser 4 //
//  Normal  //
//////////////

{
// Phase 1 //
{
{
ACTION_START(),
ACTION_JUMP(25, 1, 15),
ACTION_JUMP(15, 1, 15),
ACTION_JUMP(5, 1, 15),
ACTION_BOMBS(5, 45),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP_MINE(30, 20, 15),
ACTION_EXHAUSTED(120),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 2 //
{
{
ACTION_START(),
ACTION_JUMP(25, 1, 15),
ACTION_FIRE_BALLS(10, 15, 0),
ACTION_JUMP(20, 2, 20),
ACTION_FIRE_BALLS(20, 10, 0),
ACTION_JUMP(15, 3, 25),
ACTION_BOMBS(7, 40),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(20, 1, 15),
ACTION_FIRE_BALLS(3, 10, 0),
ACTION_JUMP(15, 2, 12),
ACTION_FIRE_BALLS(4, 8, 0),
ACTION_JUMP_MINE(10, 3, 9),
ACTION_FIRE_BALLS(5, 6, 0),
ACTION_EXHAUSTED(105),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 3 //
{
{
ACTION_START(),
ACTION_JUMP(20, 3, 25),
ACTION_FLAMETHROWER(90),
ACTION_JUMP(15, 5, 20),
ACTION_FLAMETHROWER(180),
ACTION_JUMP(10, 7, 15),
ACTION_BOMBS(9, 35),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(25, 1, 15),
ACTION_JUMP(23, 1, 15),
ACTION_JUMP(21, 1, 15),
ACTION_JUMP(19, 1, 15),
ACTION_JUMP(17, 1, 15),
ACTION_JUMP(15, 1, 15),
ACTION_JUMP(13, 1, 15),
ACTION_JUMP(11, 1, 15),
ACTION_JUMP(9, 1, 15),
ACTION_JUMP(7, 1, 15),
ACTION_JUMP(5, 1, 15),
ACTION_JUMP(3, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP_MINE(1, 1, 15),
ACTION_EXHAUSTED(90),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DEFEATED(),
ACTION_END_LOOP(),
},
}
},

//////////////
// Bowser 4 //
//   Hard   //
//////////////

{
// Phase 1 //
{
{
ACTION_START(),
ACTION_JUMP(20, 1, 15),
ACTION_JUMP(16, 1, 15),
ACTION_JUMP(12, 1, 15),
ACTION_JUMP(8, 1, 15),
ACTION_JUMP(4, 3, 15),
ACTION_BOMBS(7, 40),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP_MINE(30, 40, 12),
ACTION_EXHAUSTED(120),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 2 //
{
{
ACTION_START(),
ACTION_JUMP(25, 1, 15),
ACTION_FIRE_BALLS(15, 10, 0),
ACTION_JUMP(20, 3, 20),
ACTION_FIRE_BALLS(30, 5, 0),
ACTION_JUMP(15, 5, 25),
ACTION_BOMBS(9, 35),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_JUMP(20, 1, 12),
ACTION_FIRE_BALLS(3, 10, 1),
ACTION_JUMP(18, 2, 15),
ACTION_FIRE_BALLS(4, 10, 1),
ACTION_JUMP(16, 2, 9),
ACTION_FIRE_BALLS(5, 10, 1),
ACTION_JUMP(14, 3, 15),
ACTION_FIRE_BALLS(6, 10, 1),
ACTION_JUMP(12, 3, 6),
ACTION_FIRE_BALLS(7, 10, 1),
ACTION_JUMP(10, 4, 15),
ACTION_FIRE_BALLS(8, 10, 1),
ACTION_JUMP_MINE(8, 4, 5),
ACTION_FIRE_BALLS(9, 10, 1),
ACTION_EXHAUSTED(105),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DAMAGED(),
ACTION_END_NEXT_PHASE(),
},
},
// Phase 3 //
{
{
ACTION_START(),
ACTION_JUMP(30, 0, 15),
ACTION_CLONE_START(),
ACTION_JUMP(25, 3, 15),
ACTION_FLAMETHROWER(150),
ACTION_JUMP(20, 6, 20),
ACTION_FLAMETHROWER(300),
ACTION_JUMP(15, 10, 25),
ACTION_CLONE_END(),
ACTION_BOMBS(11, 30),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_GET_MAD(),
ACTION_END_NEXT_COMBO(),
},
{
ACTION_START(),
ACTION_CLONE_START(),
ACTION_JUMP(20, 1, 15),
ACTION_JUMP(19, 1, 15),
ACTION_JUMP(18, 1, 15),
ACTION_JUMP(17, 1, 15),
ACTION_JUMP(16, 1, 15),
ACTION_JUMP(15, 1, 15),
ACTION_JUMP(14, 1, 15),
ACTION_JUMP(13, 1, 15),
ACTION_JUMP(12, 1, 15),
ACTION_JUMP(11, 1, 15),
ACTION_JUMP(10, 1, 15),
ACTION_JUMP(9, 1, 15),
ACTION_JUMP(8, 1, 15),
ACTION_JUMP(7, 1, 15),
ACTION_JUMP(6, 1, 15),
ACTION_JUMP(5, 1, 15),
ACTION_JUMP(4, 1, 15),
ACTION_JUMP(3, 1, 15),
ACTION_JUMP(2, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP(1, 1, 15),
ACTION_JUMP_MINE(1, 1, 15),
ACTION_CLONE_END(),
ACTION_EXHAUSTED(90),
ACTION_END_LOOP(),
},
{
ACTION_START(),
ACTION_DEFEATED(),
ACTION_END_LOOP(),
},
}
},

};

//
// Behavior
//

typedef struct OmmBowser {
    s32 index;
    s32 bowserType;
    s32 numMines;
    s32 setupDuration;
    s16 setupAngleVel;
    f32 landRadius;
    f32 mineRadius;
    f32 floorHeight;
    f32 mineHeight;
    f32 bombMaxHeight;
    f32 jumpMaxHeight;
    s16 jumpDeltaAngle;
    f32 grabMultiplier;
    s32 phase;
    s32 combo;
    s32 action;
    s32 timer;
    s32 subAction;
    s32 jumpIndex;
    f32 jumpPosX;
    f32 jumpPosZ;
    f32 jumpVelY;
    s32 jumpWaves;
    s16 jumpAngle;
    s32 jumpShake;
    s32 counter;
    s32 hint;
    s32 hintTimer;
    s32 hintDialogTimer;
    struct Object *mines[8];
    s32 flamethrowerTimer;
    s16 flamethrowerYaws[32];
    bool flamethrowerCanChangeDir;
    struct Object *obj;
    struct OmmBowser *real;  // Reference to the real Bowser
    struct OmmBowser *clone; // Reference to the Bowser clone
} OmmBowser;
static OmmBowser sOmmBowsers[2];

static void bhv_omm_bowser_init(OmmBowser *bowser) {
    if (gCurrLevelNum == LEVEL_BOWSER_1) {
        bowser->index = 0;
        bowser->bowserType = 0;
        bowser->numMines = 8;
        bowser->setupDuration = 20;
        bowser->landRadius = 2175;
        bowser->mineRadius = 3075;
        bowser->grabMultiplier = 1.10f;
    } else if (gCurrLevelNum == LEVEL_BOWSER_2) {
        bowser->index = 1;
        bowser->bowserType = 1;
        bowser->numMines = 8;
        bowser->setupDuration = 15;
        bowser->landRadius = 2200;
        bowser->mineRadius = 3600;
        bowser->grabMultiplier = 1.08f;
    } else if (gCurrLevelNum == LEVEL_BOWSER_3) {
        bowser->index = 2;
        bowser->bowserType = 2;
        bowser->numMines = 5;
        bowser->setupDuration = 10;
        bowser->landRadius = 1600;
        bowser->mineRadius = 3600;
        bowser->grabMultiplier = 1.08f;
    } else {
        bowser->index = gOmmSparklyMode + 2;
        bowser->bowserType = 2;
        bowser->numMines = 8;
        bowser->setupDuration = 5;
        bowser->landRadius = 2200;
        bowser->mineRadius = 3600;
        bowser->grabMultiplier = 1.08f;
    }
    bowser->setupAngleVel = (s16) (0xC000 / bowser->setupDuration);
    bowser->floorHeight = find_floor_height(0, CELL_HEIGHT_LIMIT, 0);
    bowser->mineHeight = bowser->floorHeight + 300.f;
    bowser->bombMaxHeight = bowser->floorHeight + 3000.f;
    bowser->jumpMaxHeight = bowser->floorHeight + 1000.f;
    bowser->jumpDeltaAngle = (s16)(0x10000 / bowser->numMines);
    bowser->phase = 0;
    bowser->combo = OMM_BOWSER_COMBO_ATTACK_AND_THROW_BOMBS;
    bowser->action = 0;
    bowser->timer = 0;
    bowser->subAction = 0;
    bowser->jumpIndex = 0;
    bowser->jumpPosX = 0;
    bowser->jumpPosZ = 0;
    bowser->jumpVelY = 0;
    bowser->jumpWaves = 0;
    bowser->jumpAngle = 0;
    bowser->jumpShake = 0;
    bowser->counter = 0;
    bowser->hint = 0;
    bowser->hintTimer = 0;
    bowser->hintDialogTimer = 0;
    bowser->real = bowser;
    bowser->clone = NULL;
    bowser->flamethrowerTimer = 0;
    bowser->flamethrowerCanChangeDir = false;
    for (s32 i = 0; i != bowser->numMines; ++i) {
        s16 angle = (s16)(i * bowser->jumpDeltaAngle);
        bowser->mines[i] = omm_obj_spawn_bowser_mine(bowser->obj, bowser->mineRadius * +sins(angle), bowser->mineHeight, bowser->mineRadius * -coss(angle), -angle);
        if (gCurrLevelNum == LEVEL_BOWSER_3) {
            struct Object *thing = spawn_object(bowser->obj, MODEL_LEVEL_GEOMETRY_03, bhvStaticObject);
            thing->oPosX = bowser->mineRadius * +sins(angle);
            thing->oPosY = bowser->mineHeight - 800.f;
            thing->oPosZ = bowser->mineRadius * -coss(angle);
            thing->oFaceAngleYaw = -angle;
            thing->oMoveAngleYaw = -angle;
        }
    }
}

#define omm_bowser_current_action (&OMM_BOWSER_ACTIONS[bowser->index][bowser->phase][bowser->combo][bowser->action])
#define is_bowser_clone (bowser != bowser->real)

#define OMM_BOWSER_CLONE_CODE(...) \
__VA_ARGS__; \
if (bowser->clone) { \
    bowser = bowser->clone; \
    __VA_ARGS__; \
    bowser = bowser->real; \
} \

#if OMM_CODE_DEV
u8 *gOmmBowsersData = (u8 *) &sOmmBowsers;
const s32 gOmmBowsersSize = sizeof(sOmmBowsers);
#endif

//
// Sub-actions
//

static s32 bhv_omm_bowser_get_jump_index(OmmBowser *bowser, bool ignoreMines) {
    for (;;) {
        s32 i = (random_u16() % bowser->numMines);

        // Not the same spot
        if (i == bowser->jumpIndex) {
            continue;
        }

        // Bowser's clone cannot jump on Bowser's spot
        if (is_bowser_clone && i == bowser->real->jumpIndex) {
            continue;
        }

        // Is there a mine?
        // Bowser's clone treats JUMP_MINE actions as JUMP
        if (!is_bowser_clone && !ignoreMines && !bowser->mines[i]) {
            continue;
        }

        // Is Mario near this point?
        f32 x = bowser->landRadius * +sins(i * bowser->jumpDeltaAngle);
        f32 z = bowser->landRadius * -coss(i * bowser->jumpDeltaAngle);
        f32 distToMario = sqrtf(sqr_f(gMarioState->pos[0] - x) + sqr_f(gMarioState->pos[2] - z));
        if (distToMario < 800.f) {
            continue;
        }

        // OK
        return i;
    }
    return 0;
}

static void bhv_omm_bowser_turn_to_target(OmmBowser *bowser, f32 x, f32 z, s16 angleVel, s32 animIdle, s32 animWalk) {
    static s32 sSoundWalkTimer = 0;
    s16 angleToTarget = atan2s(z - bowser->obj->oPosZ, x - bowser->obj->oPosX);
    u16 diff = (u16) abs_s(bowser->obj->oFaceAngleYaw - angleToTarget);
    if (diff > 0x40) {
        if (animWalk != -1) {
            obj_anim_play_with_sound(bowser->obj, animWalk, 1.f, 0, false);
        }
        if (!is_bowser_clone) {
            if (animWalk == OMM_BOWSER_ANIM_WALK || animWalk == OMM_BOWSER_ANIM_SURPRISED) {
                if ((sSoundWalkTimer++ % 21) == 20) {
                    obj_play_sound(bowser->obj, SOUND_OBJ_BOWSER_WALK);
                }
            }
        }
    } else {
        if (animIdle != -1) {
            obj_anim_play_with_sound(bowser->obj, animIdle, 1.f, 0, false);
        }
    }
    bowser->obj->oFaceAngleYaw = angleToTarget - approach_s32((s16) (angleToTarget - bowser->obj->oFaceAngleYaw), 0, angleVel, angleVel);
}

//
// Update
//

static bool omm_mario_is_capture_flaming_bobomb(struct MarioState *m) {
    return omm_mario_is_capture(m) && omm_capture_get_type(gOmmCapture) == OMM_CAPTURE_FLAMING_BOBOMB;
}

// Increase current sub-action index and init the next sub-action
static void bhv_omm_bowser_update_action(OmmBowser *bowser, s32 nextSubAction) {
    bowser->subAction = nextSubAction;
    bowser->timer = -1;
}

// Increase current combo action index and init the next action
static void bhv_omm_bowser_update_combo(OmmBowser *bowser) {

    // Increase action index
    bowser->action++;

    // Make Bowser surprised if Mario possesses a flaming bob-omb
    if (bowser->combo == OMM_BOWSER_COMBO_ATTACK_AND_THROW_BOMBS && omm_mario_is_capture_flaming_bobomb(gMarioState)) {
        bowser->combo = OMM_BOWSER_COMBO_GET_MAD;
        bowser->action = 1;
        bowser->clone = NULL;
    }

    // Start the damaged combo if Bowser's health is 0
    if (bowser->combo == OMM_BOWSER_COMBO_ATTACK_UNTIL_EXHAUSTED && bowser->obj->oHealth == 0) {
        bowser->obj->oHealth = -1;
        bowser->combo = OMM_BOWSER_COMBO_DAMAGED;
        bowser->action = 1;
        bowser->clone = NULL;
    }

    // Make Bowser return to its first combo if Mario no longer possesses a flaming bob-omb
    if (bowser->combo == OMM_BOWSER_COMBO_ATTACK_UNTIL_EXHAUSTED && !omm_mario_is_capture_flaming_bobomb(gMarioState)) {
        bowser->combo = OMM_BOWSER_COMBO_ATTACK_AND_THROW_BOMBS;
        bowser->action = 1;
        bowser->clone = NULL;
    }

    // End of combo
    if (omm_bowser_current_action->action == -1) {
        switch (omm_bowser_current_action->count) {

            // Loop back
            case 0: {
                bowser->action = 1;
            } break;

            // Next combo
            case 1: {
                bowser->combo++;
                bowser->action = 1;
            } break;

            // Next phase
            case 2: {
                bowser->phase++;
                bowser->combo = OMM_BOWSER_COMBO_ATTACK_AND_THROW_BOMBS;
                bowser->action = 1;
            } break;
        }
    }

    // Init action
    switch (omm_bowser_current_action->action) {
        case OMM_BOWSER_ACTION_JUMP: {
OMM_BOWSER_CLONE_CODE(
            bowser->jumpPosX = bowser->obj->oPosX;
            bowser->jumpPosZ = bowser->obj->oPosZ;
            bowser->jumpIndex = bhv_omm_bowser_get_jump_index(bowser, true);
            bowser->jumpWaves = omm_bowser_current_action->count;
            bowser->jumpAngle = atan2s(
                (bowser->landRadius * -coss(bowser->jumpIndex * bowser->jumpDeltaAngle)) - bowser->obj->oPosZ,
                (bowser->landRadius * +sins(bowser->jumpIndex * bowser->jumpDeltaAngle)) - bowser->obj->oPosX
            );
);
        } break;

        case OMM_BOWSER_ACTION_JUMP_MINE: {
OMM_BOWSER_CLONE_CODE(
            bowser->jumpPosX = bowser->obj->oPosX;
            bowser->jumpPosZ = bowser->obj->oPosZ;
            bowser->jumpIndex = bhv_omm_bowser_get_jump_index(bowser, false);
            bowser->jumpWaves = omm_bowser_current_action->count;
            bowser->jumpAngle = atan2s(
                (bowser->landRadius * -coss(bowser->jumpIndex * bowser->jumpDeltaAngle)) - bowser->obj->oPosZ,
                (bowser->landRadius * +sins(bowser->jumpIndex * bowser->jumpDeltaAngle)) - bowser->obj->oPosX
            );
);
        } break;

        case OMM_BOWSER_ACTION_FIRE_BALLS: {
            bowser->counter = omm_bowser_current_action->count;
        } break;

        case OMM_BOWSER_ACTION_BOMBS: {
            bowser->counter = omm_bowser_current_action->count;
        } break;
    }

    // Init sub-action and timer
    bhv_omm_bowser_update_action(bowser, 0);
}

// Cancel current action to jump away from Mario if he's too close
static void bhv_omm_bowser_check_emergency_jump(OmmBowser *bowser) {
    if (obj_get_horizontal_distance(bowser->obj, gMarioObject) > 600.f) {
        return;
    }

    // Doesn't trigger if Mario is possessing a flaming bob-omb
    if (omm_mario_is_capture_flaming_bobomb(gMarioState)) {
        return;
    }

    // Deinit action
    switch (omm_bowser_current_action->action) {

        // Don't cancel during a jump
        case OMM_BOWSER_ACTION_JUMP:
        case OMM_BOWSER_ACTION_JUMP_MINE: {
            if (bowser->subAction < 2) {
                return;
            }
        } break;

        // Delete all inactive bombs
        case OMM_BOWSER_ACTION_BOMBS: {
            for (bool ok = false; !ok;) {
                ok = true;
                for_each_object_with_behavior(bobomb, bhvOmmFlamingBobomb) {
                    if (bobomb->oAction == 0) {
                        obj_mark_for_deletion(bobomb->oFlamingBobombAura);
                        obj_destroy(bobomb);
                        ok = false;
                        break;
                    }
                }
            }
        } break;

        // Extinguish the flames
        case OMM_BOWSER_ACTION_FLAMETHROWER: {
            for_each_object_with_behavior(flame, bhvOmmBowserFlame) {
                flame->oAction = 2;
                flame->oTimer = 0;
            }
        } break;
    }

    // Remove the clone
    bowser->clone = NULL;

    // Trigger an emergency jump
    bowser->action = -1;
    bhv_omm_bowser_update_combo(bowser);
}

//
// Actions
//

// Bowser jumps to a distant point
// Launches a fire shockwave when landing
static void bhv_omm_bowser_jump(OmmBowser *bowser) {
    switch (bowser->subAction) {

        // Bowser turns to the direction of the landing point
        case 0: {
            if (bowser->timer < 6) {
OMM_BOWSER_CLONE_CODE(
                s32 angleVel = abs_s(bowser->jumpAngle - bowser->obj->oFaceAngleYaw) / (6 - bowser->timer);
                bowser->obj->oFaceAngleYaw = bowser->jumpAngle - approach_s32((s16) (bowser->jumpAngle - bowser->obj->oFaceAngleYaw), 0, angleVel, angleVel);
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_JUMP, 1.f, 0, false);
);
            }

            // Starts jumping
            else {
OMM_BOWSER_CLONE_CODE(
                bowser->obj->oFaceAngleYaw = bowser->jumpAngle;
);
                obj_play_sound(bowser->obj, SOUND_OBJ2_BOWSER_ROAR);
                bhv_omm_bowser_update_action(bowser, 1);
            }
        } break;

        // Jumps
        case 1: {
OMM_BOWSER_CLONE_CODE(
            obj_reset_hitbox(bowser->obj, 0, 0, 0, 0, 0, 0);
            f32 t = clamp_0_1_f((f32) bowser->timer / (f32) omm_bowser_current_action->duration);
            bowser->obj->oPosX = bowser->jumpPosX + ((bowser->landRadius * +sins(bowser->jumpIndex * bowser->jumpDeltaAngle)) - bowser->jumpPosX) * t;
            bowser->obj->oPosZ = bowser->jumpPosZ + ((bowser->landRadius * -coss(bowser->jumpIndex * bowser->jumpDeltaAngle)) - bowser->jumpPosZ) * t;
            bowser->obj->oPosY = bowser->floorHeight + (bowser->jumpMaxHeight - bowser->floorHeight) * sins(0x8000 * t);
);

            // About to land
            if (t == 1.f) {
                if (bowser->jumpShake == 0) {
                    gPlayerCameraState->cameraEvent = CAM_EVENT_BOWSER_JUMP;
                    gSecondCameraFocus = bowser->obj;
                }
                set_environmental_camera_shake(SHAKE_ENV_BOWSER_THROW_BOUNCE);
                bowser->jumpShake = 60;
OMM_BOWSER_CLONE_CODE(
                bowser->obj->oPosY = bowser->floorHeight;
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_LAND, 1.f, 0, false);
                obj_spawn_white_puff(bowser->obj, SOUND_OBJ_BOWSER_WALK);
);
                bhv_omm_bowser_update_action(bowser, 2);
            }
        } break;

        // Shockwaves
        case 2: {
            if (bowser->jumpWaves > 0) {
OMM_BOWSER_CLONE_CODE(
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_LAND, 1.f, 0, true);
                if ((bowser->timer % omm_bowser_current_action->delay) == 0) {
                    omm_obj_spawn_bowser_shockwave_fire(bowser->obj, 100, 100, 150, 40, bowser->landRadius + bowser->mineRadius, OMM_TEXTURE_BOWSER_FIRE_RED_1, OMM_TEXTURE_BOWSER_FIRE_RED_2);
                    bowser->jumpWaves--;
                }
);
            } else {
                bhv_omm_bowser_update_action(bowser, 3);
            }
        } break;

        // Landed, next action
        case 3: {
            if (obj_anim_is_near_end(bowser->obj)) {
                bhv_omm_bowser_update_combo(bowser);
            }
        } break;
    }
}

// Bowser inhales and throw large fast fire balls at Mario
// These fire balls cannot be captured or deflected, and Bowser will try to predict Mario's movement to hit him
static void bhv_omm_bowser_fireballs(OmmBowser *bowser) {
    switch (bowser->subAction) {

        // Targets Mario
        case 0: {
            if (bowser->timer < bowser->setupDuration) {
                bhv_omm_bowser_turn_to_target(bowser, gMarioState->pos[0], gMarioState->pos[2], bowser->setupAngleVel, OMM_BOWSER_ANIM_IDLE, OMM_BOWSER_ANIM_WALK);
            } else {
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_INHALE_AND_THROW, 1.5f, SOUND_OBJ_BOWSER_INHALING, true);
                bhv_omm_bowser_update_action(bowser, 1);
            }
        } break;

        // Inhales (fast)
        case 1: {
            bhv_omm_bowser_turn_to_target(bowser, gMarioState->pos[0], gMarioState->pos[2], 0x800, -1, -1);
            if (bowser->obj->oAnimFrame >= 42) {
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_THROW_FIRE_BALL, 1.f, 0, true);
                bhv_omm_bowser_update_action(bowser, 2);
            }
        } break;

        // Shoots a fire ball
        case 2: {
            if (bowser->timer >= 4) {

                // Predicts Mario's future position if he's on ground
                // Shoots a faster fire ball directly at Mario if he's airborne
                // Choose randomly between the two states if Mario is not possessing a flaming bob-omb
                f32 pt = 0.f;
                f32 vf = 80.f;
                bool possess = omm_mario_is_capture_flaming_bobomb(gMarioState);
                bool onFloor = (gMarioState->pos[1] <= bowser->floorHeight + 8.f);
                bool predict = (possess ? onFloor : (random_u16() & 1));
                if (predict) {
                    f32 vx = gMarioState->vel[0];
                    f32 vz = gMarioState->vel[2];
                    f32 dx = gMarioState->pos[0] - bowser->obj->oPosX;
                    f32 dz = gMarioState->pos[2] - bowser->obj->oPosZ;
                    f32 A = sqr_f(vx) + sqr_f(vz) - sqr_f(vf);
                    f32 B = 2.f * (vx * dx + vz * dz);
                    f32 C = sqr_f(dx) + sqr_f(dz);
                    f32 D = (B * B) - (4.f * A * C);
                    if (A != 0.f && D >= 0.f) {
                        f32 t1 = (-B - sqrtf(D)) / (2.f * A);
                        f32 t2 = (-B + sqrtf(D)) / (2.f * A);
                        if (max_f(t1, t2) >= 0.f) {
                            if (t1 < 0.f) t1 = 10000.f;
                            if (t2 < 0.f) t2 = 10000.f;
                            pt = min_f(t1, t2);
                        }
                    }
                } else {
                    vf = 120.f;
                }
                f32 px = gMarioState->pos[0] + pt * gMarioState->vel[0];
                f32 pz = gMarioState->pos[2] + pt * gMarioState->vel[2];
                bowser->obj->oFaceAngleYaw = atan2s(pz - bowser->obj->oPosZ, px - bowser->obj->oPosX);

                // Shoots a fire ball
                omm_obj_spawn_bowser_fireball(bowser->obj,
                    bowser->obj->oPosX + bowser->obj->hitboxRadius * sins(bowser->obj->oFaceAngleYaw),
                    bowser->obj->oPosY + 100.f,
                    bowser->obj->oPosZ + bowser->obj->hitboxRadius * coss(bowser->obj->oFaceAngleYaw),
                    vf,
                    bowser->landRadius + bowser->mineRadius,
                    bowser->obj->oFaceAngleYaw
                );
                bowser->counter--;
                bhv_omm_bowser_update_action(bowser, 3);
            }
        } break;

        // Waits between fire balls and keeps on focusing Mario
        // The delay decreases after each thrown fire ball
        case 3: {
            s32 delay = omm_bowser_current_action->delay - (omm_bowser_current_action->count - (bowser->counter + 1)) * omm_bowser_current_action->duration;
            if (bowser->timer < delay) {
                if (bowser->obj->oAnimID == OMM_BOWSER_ANIM_THROW_FIRE_BALL && obj_anim_is_near_end(bowser->obj)) {
                    obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_IDLE, 1.f, 0, false);
                }
                s16 angleVel = (s16) (0xC000 / delay);
                s32 animIdle = (bowser->obj->oAnimID == OMM_BOWSER_ANIM_THROW_FIRE_BALL ? -1 : OMM_BOWSER_ANIM_IDLE);
                s32 animWalk = (bowser->obj->oAnimID == OMM_BOWSER_ANIM_THROW_FIRE_BALL ? -1 : OMM_BOWSER_ANIM_WALK);
                bhv_omm_bowser_turn_to_target(bowser, gMarioState->pos[0], gMarioState->pos[2], angleVel, animIdle, animWalk);
            } else if (bowser->counter <= 0) {
                bhv_omm_bowser_update_action(bowser, 4);
            } else {
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_THROW_FIRE_BALL, 1.f, 0, true);
                bhv_omm_bowser_update_action(bowser, 2);
            }
        } break;

        // Next action
        case 4: {
            bhv_omm_bowser_update_combo(bowser);
        } break;
    }
}

// Bowser summons and throw flaming bob-ombs at Mario
// When landing, bombs explode in a blue fire shockwave
static void bhv_omm_bowser_bombs(OmmBowser *bowser) {
    switch (bowser->subAction) {

        // Targets Mario
        case 0: {
            if (bowser->timer < bowser->setupDuration) {
                bhv_omm_bowser_turn_to_target(bowser, gMarioState->pos[0], gMarioState->pos[2], bowser->setupAngleVel, OMM_BOWSER_ANIM_IDLE, OMM_BOWSER_ANIM_WALK);
            } else {
                bhv_omm_bowser_update_action(bowser, 1);
            }
        } break;

        // Summons bombs
        case 1: {
            for (s32 i = 0; i != omm_bowser_current_action->count; ++i) {
                s32 angle = (((f32) i - (f32) (omm_bowser_current_action->count / 2.f)) * 0x1740) + bowser->obj->oFaceAngleYaw;
                omm_obj_spawn_flaming_bobomb(
                    bowser->obj,
                    bowser->obj->oPosX + 500.f * sins(angle),
                    bowser->obj->oPosY + 360.f,
                    bowser->obj->oPosZ + 500.f * coss(angle),
                    omm_bowser_current_action->count - i,
                    omm_bowser_current_action->count,
                    bowser->landRadius,
                    bowser->bombMaxHeight
                );
            }
            obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_RAISE_ARM, 1.f, SOUND_OBJ2_BOWSER_TELEPORT, true);
            bhv_omm_bowser_update_action(bowser, 2);
        } break;

        // Wait
        case 2: {
            obj_anim_extend(bowser->obj);
            if (bowser->timer >= 20) {
                bhv_omm_bowser_update_action(bowser, 3);
            }
        } break;

        // Throw bombs
        case 3: {
            if (bowser->counter > 0) {

                // If Mario manages to capture a flaming bob-omb during its ascending state,
                // Bowser gets mad instantly and throw all other bombs at once
                if (omm_mario_is_capture_flaming_bobomb(gMarioState) && gOmmObject->flaming_bobomb.captureDuringAscent) {
                    for_each_object_with_behavior(bobomb, bhvOmmFlamingBobomb) {
                        if (bobomb->oAction <= 1) {
                            bobomb->oAction = 5;
                            bobomb->oTimer = 0;
                        }
                    }
                    bhv_omm_bowser_update_combo(bowser);
                    return;
                }

                // Throw a bomb every delay frames
                if ((bowser->timer % omm_bowser_current_action->delay) == 0) {
                    obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_THROW_BOMB, 1.f, SOUND_OBJ2_BOWSER_ROAR | 0x8100, true);
                    struct Object *bobomb = obj_get_first_with_behavior_and_field_s32(bhvOmmFlamingBobomb, _FIELD(oFlamingBobombIndex), bowser->counter);
                    bobomb->oAction = 1;
                    bobomb->oTimer = 0;
                    bowser->counter--;
                }
                if (bowser->obj->oAnimID == OMM_BOWSER_ANIM_THROW_BOMB && obj_anim_is_near_end(bowser->obj)) {
                    obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_RAISE_ARM, 1.f, 0, true);
                } else {
                    obj_anim_extend(bowser->obj);
                }
            } else {
                bhv_omm_bowser_update_action(bowser, 4);
            }
        } break;

        // Wait for the last throw anim to end
        case 4: {
            if (obj_anim_is_near_end(bowser->obj)) {
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_IDLE, 1.f, 0, true);
                bhv_omm_bowser_update_action(bowser, 5);
            }
        } break;

        // Wait for the bobombs to land, next action
        case 5: {
            if (omm_mario_is_capture_flaming_bobomb(gMarioState) || !obj_get_first_with_behavior(bhvOmmFlamingBobomb)) {
                bhv_omm_bowser_update_combo(bowser);
            }
        } break;
    }
}

// Bowser inhales and throw a burst of flames at Mario, then rotates towards him for a few seconds
// As Bowser gets faster and faster, Mario must jump over the flamethrower to avoid it
static void bhv_omm_bowser_flamethrower(OmmBowser *bowser) {
    switch (bowser->subAction) {

        // Targets Mario
        case 0: {
            if (bowser->timer < bowser->setupDuration) {
OMM_BOWSER_CLONE_CODE(
                bowser->flamethrowerCanChangeDir = true;
                bhv_omm_bowser_turn_to_target(bowser, gMarioState->pos[0], gMarioState->pos[2], bowser->setupAngleVel, OMM_BOWSER_ANIM_IDLE, OMM_BOWSER_ANIM_WALK);
);
            } else {
OMM_BOWSER_CLONE_CODE(
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_INHALE_AND_THROW, 1.f, SOUND_OBJ_BOWSER_INHALING, true);
);
                bhv_omm_bowser_update_action(bowser, 1);
            }
        } break;

        // Inhales (slow)
        case 1: {
OMM_BOWSER_CLONE_CODE(
            bhv_omm_bowser_turn_to_target(bowser, gMarioState->pos[0], gMarioState->pos[2], 0x400, -1, -1);
);
            if (bowser->obj->oAnimFrame >= 54) {
                bhv_omm_bowser_update_action(bowser, 2);
            }
        } break;

        // Flame-thrower
        case 2: {

            // Locks the animation on frame 54 (mouth open, facing forward)
OMM_BOWSER_CLONE_CODE(
            obj_anim_set_frame(bowser->obj, 54);
);

            // Spawn 30 flames, one per frame
            if (bowser->timer < 30) {
OMM_BOWSER_CLONE_CODE(
                f32 dist = bowser->obj->hitboxRadius + 160.f * (bowser->timer + 1);
                struct Object *flame = omm_obj_spawn_bowser_flame(bowser->obj,
                    bowser->obj->oPosX + dist * sins(bowser->obj->oFaceAngleYaw),
                    bowser->floorHeight,
                    bowser->obj->oPosZ + dist * coss(bowser->obj->oFaceAngleYaw),
                    omm_bowser_current_action->duration
                );
                flame->oHomeX = bowser->obj->oPosX;
                flame->oHomeZ = bowser->obj->oPosZ;
                flame->oForwardVel = dist;
                flame->oIsBowserClone = is_bowser_clone;
);
            }

            // Next sub-action
            else {
OMM_BOWSER_CLONE_CODE(
                bowser->flamethrowerTimer = 0;
                bowser->obj->oAngleVelYaw = 0;
);
                bhv_omm_bowser_update_action(bowser, 3);
            }
        } break;

        // Follows Mario
        case 3: {

            // Locks the animation on frame 54 (mouth open, facing forward)
OMM_BOWSER_CLONE_CODE(
            obj_anim_set_frame(bowser->obj, 54);
);

            // Rotates faster and faster
            // Decelerates to u-turn
            if (bowser->timer < omm_bowser_current_action->duration) {
OMM_BOWSER_CLONE_CODE(
                s16 prevYaw = bowser->obj->oFaceAngleYaw;
                s16 prevVel = bowser->obj->oAngleVelYaw;
                bhv_omm_bowser_turn_to_target(bowser, gMarioState->pos[0], gMarioState->pos[2], 0x20, -1, -1);
                s16 currAcc = (bowser->obj->oFaceAngleYaw - prevYaw);
                s16 signVel = sign_0_s(bowser->obj->oAngleVelYaw);
                s16 signAcc = sign_0_s(currAcc);
                s16 signDir = (signAcc != 0) ? signAcc : signVel;

                // Deceleration/Acceleration
                if (signVel * signAcc < 0) {
                    if (bowser->flamethrowerCanChangeDir) {
                        bowser->obj->oAngleVelYaw += 0x40 * signDir;
                    } else {
                        bowser->obj->oAngleVelYaw -= 0x20 * signDir;
                    }
                } else {
                    bowser->obj->oAngleVelYaw += 0x20 * signDir;
                }
                bowser->obj->oAngleVelYaw = clamp_s(bowser->obj->oAngleVelYaw, -0x400, +0x400);

                // Direction
                if (abs_s(bowser->obj->oAngleVelYaw) > 0x200) {
                    bowser->flamethrowerCanChangeDir = true;
                } else if (bowser->obj->oAngleVelYaw * prevVel < 0) {
                    bowser->flamethrowerCanChangeDir = false;
                }

                // Angle
                bowser->obj->oFaceAngleYaw = prevYaw + bowser->obj->oAngleVelYaw;
                if (bowser->flamethrowerTimer < (s32) array_length(bowser->flamethrowerYaws)) {
                    bowser->flamethrowerYaws[bowser->flamethrowerTimer] = bowser->obj->oFaceAngleYaw;
                } else {
                    mem_mov(bowser->flamethrowerYaws, bowser->flamethrowerYaws + 1, sizeof(s16) * (array_length(bowser->flamethrowerYaws) - 1));
                    bowser->flamethrowerYaws[array_length(bowser->flamethrowerYaws) - 1] = bowser->obj->oFaceAngleYaw;
                }
                bowser->flamethrowerTimer++;
);
            }

            // Next sub-action
            else {
                bhv_omm_bowser_update_action(bowser, 4);
            }
        } break;

        // Stops and next action
        case 4: {
            if (obj_anim_is_near_end(bowser->obj)) {
                bhv_omm_bowser_update_combo(bowser);
            }
        } break;
    }

    // Update flames
    for_each_object_with_behavior(flame, bhvOmmBowserFlame) {
        if (flame->oAction == 1) {
OMM_BOWSER_CLONE_CODE(
            if (flame->oIsBowserClone == is_bowser_clone) {
                s32 index = clamp_s(flame->oTimer - 1, 0, bowser->flamethrowerTimer - 1) - max_s(0, bowser->flamethrowerTimer - array_length(bowser->flamethrowerYaws));
                s16 angle = bowser->flamethrowerYaws[index];
                flame->oPosX = flame->oHomeX + flame->oForwardVel * sins(angle);
                flame->oPosZ = flame->oHomeZ + flame->oForwardVel * coss(angle);
            }
);
        }
    }
}

// Bowser clones himself
static void bhv_omm_bowser_clone_start(OmmBowser *bowser) {
    switch (bowser->subAction) {

        // Turns to face center and spawn a clone
        case 0: {
            s16 prevAngle = bowser->obj->oFaceAngleYaw;
            bhv_omm_bowser_turn_to_target(bowser, 0, 0, 0x800, OMM_BOWSER_ANIM_IDLE, OMM_BOWSER_ANIM_WALK);
            if (bowser->obj->oFaceAngleYaw == prevAngle) {
                OmmBowser *clone = &sOmmBowsers[1];
                clone->real = bowser;
                clone->clone = clone;
                clone->obj = spawn_object(bowser->obj, MODEL_BOWSER, bhvOmmBowserClone);
                clone->obj->oAction = 0;
                clone->obj->oHealth = 99;
                obj_set_always_rendered(clone->obj, true);
                obj_set_xyz(clone->obj, bowser->obj->oPosX, bowser->obj->oPosY, bowser->obj->oPosZ);
                obj_set_home(clone->obj, bowser->obj->oPosX, bowser->obj->oPosY, bowser->obj->oPosZ);
                obj_set_angle(clone->obj, bowser->obj->oFaceAnglePitch, bowser->obj->oFaceAngleYaw, bowser->obj->oFaceAngleRoll);
                obj_scale_xyz(clone->obj, bowser->obj->oScaleX, bowser->obj->oScaleY, bowser->obj->oScaleZ);
                obj_play_sound(bowser->obj, SOUND_OBJ2_BOWSER_TELEPORT);
                bhv_omm_bowser_update_action(bowser, 1);
                bowser->clone = clone;
            }
        } break;

        // Moves laterally with the clone, then start the clone
        case 1: {
            if (bowser->timer < 45) {
OMM_BOWSER_CLONE_CODE(
                bowser->obj->oFaceAngleYaw += 0x30 * (is_bowser_clone ? -1 : +1);
                vec3f_rotate_zxy(&bowser->obj->oPosX, &bowser->obj->oPosX, 0, 0x30 * (is_bowser_clone ? -1 : +1), 0);
                obj_anim_play(bowser->obj, OMM_BOWSER_ANIM_IDLE, 1.f);
);
            } else {
                bowser->clone->obj->oAction = 1;
                bhv_omm_bowser_update_combo(bowser);
            }
        } break;
    }
}

// Bowser's clone vanishes
static void bhv_omm_bowser_clone_end(OmmBowser *bowser) {
    bowser->clone = NULL;
    bhv_omm_bowser_update_combo(bowser);
}

// Bowser gets mad when seeing his minion controlled by Mario
static void bhv_omm_bowser_get_mad(OmmBowser *bowser) {
    switch (bowser->subAction) {

        // Gets mad
        case 0: {
            obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_SURPRISED, 1.25f, SOUND_OBJ_BOWSER_TAIL_PICKUP, true);
            bhv_omm_bowser_update_action(bowser, 1);
        } break;

        // Faces Mario, next action
        case 1: {
            bhv_omm_bowser_turn_to_target(bowser, gMarioState->pos[0], gMarioState->pos[2], 0x800, -1, -1);
            if (obj_anim_is_near_end(bowser->obj)) {
                bhv_omm_bowser_update_combo(bowser);
            }
        } break;
    }
}

// Bowser is exhausted after attacking
static void bhv_omm_bowser_exhausted(OmmBowser *bowser) {

    // Bowser was hit
    if (bowser->obj->oBowserHit) {
        bowser->obj->oHealth--;
        bowser->obj->oBowserHit = 0;

        // Bowser is sent flying into a mine
        if (bowser->obj->oHealth == 0) {
            bhv_omm_bowser_update_combo(bowser);
            return;
        }

        // Bowser does a roll
        // The height depends on his remaining health
        bowser->jumpVelY = 45.f + 15.f * (3 - bowser->obj->oHealth);
        obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_DAMAGED_BELLY, 1.25f, SOUND_OBJ_BOWSER_TAIL_PICKUP, true);
        bhv_omm_bowser_update_action(bowser, 4);
    }

    // Process sub-action
    switch (bowser->subAction) {

        // Turns to face center
        case 0: {
            s16 prevAngle = bowser->obj->oFaceAngleYaw;
            bhv_omm_bowser_turn_to_target(bowser, 0, 0, 0x400, OMM_BOWSER_ANIM_WALK, OMM_BOWSER_ANIM_WALK);
            if (bowser->obj->oFaceAngleYaw == prevAngle) {
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_EXHAUSTED, 1.f, SOUND_OBJ_BOWSER_DEFEATED, true);
                bhv_omm_bowser_update_action(bowser, 1);
            }
        } break;

        // Falls on belly
        case 1: {
            bowser->obj->oHealth = (bowser->phase + 1); // Bowser is now vulnerable to flaming bob-omb explosions
            if (obj_anim_is_near_end(bowser->obj)) {
                bhv_omm_bowser_update_action(bowser, 2);
            }
        } break;

        // Waits some time before moving again
        case 2: {
            bowser->obj->oBowserEyesShut = 1;
            obj_anim_extend(bowser->obj);
            if (bowser->timer >= omm_bowser_current_action->duration || !omm_mario_is_capture_flaming_bobomb(gMarioState)) {
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_GET_UP, 1.5f, 0, true);
                bhv_omm_bowser_update_action(bowser, 3);
            }
        } break;

        // Gets up, next action
        case 3: {
            bowser->obj->oHealth = -1; // Bowser is no longer vulnerable to flaming bob-omb explosions
            bowser->obj->oBowserEyesShut = 0;
            if (obj_anim_is_near_end(bowser->obj)) {
                bhv_omm_bowser_update_combo(bowser);
            }
        } break;

        // Hit by a flaming bob-omb, but not enough to be sent flying into a mine
        case 4: {
            bowser->obj->oPosY += bowser->jumpVelY;
            bowser->obj->oBowserEyesShut = 1;
            obj_anim_extend(bowser->obj);

            // Bounces
            if (bowser->obj->oPosY > bowser->floorHeight) {
                bowser->jumpVelY -= 5.f;
            } else {
                bowser->obj->oPosY = bowser->floorHeight;
                bowser->jumpVelY /= -2.f;
                if (abs_f(bowser->jumpVelY) > 5.f) {
                    obj_spawn_white_puff(bowser->obj, SOUND_OBJ_BOWSER_WALK);
                    set_environmental_camera_shake(SHAKE_ENV_BOWSER_THROW_BOUNCE);
                } else {
                    bhv_omm_bowser_update_action(bowser, 2);
                }
            }
        } break;
    }
}

// Mario hits Bowser, Bowser hits a mine
static void bhv_omm_bowser_damaged(OmmBowser *bowser) {
    switch (bowser->subAction) {

        // Knocked back
        case 0: {
            obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_DAMAGED_BELLY, 1.25f, SOUND_OBJ_BOWSER_TAIL_PICKUP, true);
            bhv_omm_bowser_update_action(bowser, 1);
        } break;

        // Flies into a mine
        case 1: {
            f32 t = clamp_0_1_f(bowser->timer / 30.f);
            bowser->obj->oPosX = (bowser->landRadius + (bowser->mineRadius - bowser->landRadius) * t) * +sins(bowser->jumpIndex * bowser->jumpDeltaAngle);
            bowser->obj->oPosY = (bowser->floorHeight + (bowser->mineHeight + 100.f - bowser->floorHeight) * t + (bowser->jumpMaxHeight - bowser->floorHeight) * 0.5f * sins(0x8000 * t));
            bowser->obj->oPosZ = (bowser->landRadius + (bowser->mineRadius - bowser->landRadius) * t) * -coss(bowser->jumpIndex * bowser->jumpDeltaAngle);
            bowser->obj->oBowserEyesShut = 1;
            obj_anim_extend(bowser->obj);

            // About to hit the mine
            if (t == 1.f) {

                // Explode mine
                struct Object *mine = bowser->mines[bowser->jumpIndex];
                spawn_object(mine, MODEL_BOWSER_FLAMES, bhvBowserBombExplosion);
                create_sound_spawner(SOUND_GENERAL_BOWSER_BOMB_EXPLOSION);
                set_environmental_camera_shake(SHAKE_ENV_BOWSER_JUMP);
                obj_mark_for_deletion(mine);
                bowser->mines[bowser->jumpIndex] = NULL;

                // The blast ejects Mario from his possession...
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 0);

                // ...and destroy falling platforms on Bowser 3
                if (gCurrLevelNum == LEVEL_BOWSER_3 && bowser->phase == 1) {
                    struct Object *b = obj_get_first_with_behavior(bhvBowser);
                    b->oHealth = 1;
                    b->oAction = 3;
                    for_each_object_with_behavior(platform, bhvFallingBowserPlatform) {
                        platform->oBitsPlatformBowserObject = b;
                    }
                }

                // Hits the mine
                bowser->jumpVelY = +100.f;
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_DAMAGED_BELLY, 1.25f, SOUND_OBJ_BOWSER_TAIL_PICKUP, true);
                bhv_omm_bowser_update_action(bowser, 2);
            }
        } break;

        // Hits the mine
        case 2: {
            f32 q = (bowser->phase == 2 ? bowser->grabMultiplier : 1.f);
            f32 t = sqrtf(clamp_0_1_f(bowser->timer / 90.f));
            bowser->obj->oPosX = (bowser->mineRadius * (1.f - t * q)) * +sins(bowser->jumpIndex * bowser->jumpDeltaAngle);
            bowser->obj->oPosZ = (bowser->mineRadius * (1.f - t * q)) * -coss(bowser->jumpIndex * bowser->jumpDeltaAngle);
            bowser->obj->oPosY += bowser->jumpVelY;
            bowser->obj->oBowserEyesShut = 1;
            obj_anim_extend(bowser->obj);

            // Bounces
            if (bowser->obj->oPosY > bowser->floorHeight) {
                bowser->jumpVelY -= 5.f;
            } else {
                bowser->obj->oPosY = bowser->floorHeight;
                bowser->jumpVelY /= -2.f;
                if (abs_f(bowser->jumpVelY) > 5.f) {
                    obj_spawn_white_puff(bowser->obj, SOUND_OBJ_BOWSER_WALK);
                    set_environmental_camera_shake(SHAKE_ENV_BOWSER_THROW_BOUNCE);
                }
            }

            // About to land on center
            if (t == 1.f) {

                // Defeated
                if (omm_bowser_current_action->count == -1) {
                    bowser->obj->oPosY = bowser->floorHeight;
                    bhv_omm_bowser_update_action(bowser, 6);
                }

                // Gets up
                else {
                    bowser->obj->oPosY = bowser->floorHeight;
                    obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_GET_UP, 1.5f, SOUND_OBJ2_BOWSER_ROAR, true);
                    bhv_omm_bowser_update_action(bowser, 3);
                }
            }
        } break;

        // Next phase
        case 3: {
            if (obj_anim_is_near_end(bowser->obj)) {
                bowser->obj->oBowserEyesShut = 0;
                bhv_omm_bowser_update_combo(bowser);
            }
        } break;

        // Gives up, wait for Mario to grab his tail
        case 6: {
            bowser->obj->oBowserEyesShut = 1;
            obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_GIVE_UP, 1.f, 0, false);
            f32 distMarioFromCenter = sqrtf(sqr_f(gMarioObject->oPosX) + sqr_f(gMarioObject->oPosZ));
            if (distMarioFromCenter <= 150.f && omm_mario_is_punching(gMarioState)) {
                omm_mario_set_action(gMarioState, ACT_OMM_GRAB_BOWSER, 0, B_BUTTON);
                gMarioState->pos[0] = 0.f;
                gMarioState->pos[1] = bowser->floorHeight;
                gMarioState->pos[2] = 0.f;
                gMarioState->faceAngle[0] = 0;
                gMarioState->faceAngle[1] = bowser->obj->oFaceAngleYaw;
                gMarioState->faceAngle[2] = 0;
                gMarioObject->oGfxPos[0] = 0.f;
                gMarioObject->oGfxPos[1] = bowser->floorHeight;
                gMarioObject->oGfxPos[2] = 0.f;
                gMarioObject->oGfxAngle[0] = 0;
                gMarioObject->oGfxAngle[1] = bowser->obj->oFaceAngleYaw;
                gMarioObject->oGfxAngle[2] = 0;
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_GIVE_UP, 1.f, SOUND_OBJ_BOWSER_TAIL_PICKUP, true);
                bhv_omm_bowser_update_action(bowser, 7);
            }
        } break;

        // Spins (Mario updates Bowser's position and angle)
        case 7: {
            if (gMarioState->action == ACT_OMM_RELEASE_BOWSER) {
                bowser->jumpPosX = bowser->obj->oPosX;
                bowser->jumpPosZ = bowser->obj->oPosZ;
                bhv_omm_bowser_update_action(bowser, 8);
            }
        } break;

        // "So long-a, Bowser"
        case 8: {
            f32 t = clamp_0_1_f((bowser->timer + 1) / 40.f);
            bowser->obj->oPosX = (bowser->jumpPosX + (bowser->mineRadius - bowser->jumpPosX) * sqrtf(t)) * sins(bowser->obj->oFaceAngleYaw);
            bowser->obj->oPosY = (bowser->floorHeight + (bowser->mineHeight + 100.f - bowser->floorHeight) * t + (bowser->jumpMaxHeight - bowser->floorHeight) * 0.75f * sins(0x8000 * t));
            bowser->obj->oPosZ = (bowser->jumpPosZ + (bowser->mineRadius - bowser->jumpPosZ) * sqrtf(t)) * coss(bowser->obj->oFaceAngleYaw);
            bowser->obj->oBowserEyesShut = 1;

            // About to hit the mine
            if (t == 1.f) {

                // Explode mine
                struct Object *mine = bowser->obj->oBowserMine;
                spawn_object(mine, MODEL_BOWSER_FLAMES, bhvBowserBombExplosion);
                create_sound_spawner(SOUND_GENERAL_BOWSER_BOMB_EXPLOSION);
                set_environmental_camera_shake(SHAKE_ENV_BOWSER_JUMP);
                obj_mark_for_deletion(mine);

                // Hits the mine
                bowser->jumpVelY = +90.f;
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_DAMAGED_BACK, 1.25f, SOUND_OBJ_BOWSER_TAIL_PICKUP, true);
                bhv_omm_bowser_update_action(bowser, 9);
            }
        } break;

        // Hits the mine
        case 9: {
            f32 t = sqrtf(clamp_0_1_f(bowser->timer / 120.f));
            bowser->obj->oPosX = (bowser->mineRadius * (1.f - t)) * sins(bowser->obj->oFaceAngleYaw);
            bowser->obj->oPosZ = (bowser->mineRadius * (1.f - t)) * coss(bowser->obj->oFaceAngleYaw);
            bowser->obj->oPosY += bowser->jumpVelY;
            bowser->obj->oBowserEyesShut = 1;
            obj_anim_extend(bowser->obj);

            // Bounces
            if (bowser->obj->oPosY > bowser->floorHeight) {
                bowser->jumpVelY -= 4.f;
            } else {
                bowser->obj->oPosY = bowser->floorHeight;
                bowser->jumpVelY /= -2.f;
                if (abs_f(bowser->jumpVelY) > 4.f) {
                    obj_spawn_white_puff(bowser->obj, SOUND_OBJ_BOWSER_WALK);
                    set_environmental_camera_shake(SHAKE_ENV_BOWSER_THROW_BOUNCE);
                }
            }

            // About to land on center
            if (t == 1.f) {
                bowser->obj->oPosY = bowser->floorHeight;
                bhv_omm_bowser_update_action(bowser, 10);
            }
        } break;

        // Defeated
        // Awaken the old Bowser, put this one to sleep
        case 10: {
            struct Object *b = obj_get_first_with_behavior(bhvBowser);
            obj_set_dormant(b, false);
            b->oBehParams2ndByte = bowser->bowserType;
            b->oAction = 4; // bowser_act_dead
            b->oPrevAction = 4; // must be the same as b->oAction
            b->oSubAction = 2; // bowser_dead_wait_for_mario
            b->oHeldState = 0; // bowser_free_update
            b->oBowserEyesShut = 1; // closed eyes
            b->oPosX = bowser->obj->oPosX;
            b->oPosY = bowser->obj->oPosY;
            b->oPosZ = bowser->obj->oPosZ;
            b->oVelX = 0;
            b->oVelY = 0;
            b->oVelZ = 0;
            b->oFaceAnglePitch = bowser->obj->oFaceAnglePitch;
            b->oFaceAngleYaw = bowser->obj->oFaceAngleYaw;
            b->oFaceAngleRoll = bowser->obj->oFaceAngleRoll;
            b->oMoveAnglePitch = bowser->obj->oFaceAnglePitch;
            b->oMoveAngleYaw = bowser->obj->oFaceAngleYaw;
            b->oMoveAngleRoll = bowser->obj->oFaceAngleRoll;
            b->oGfxPos[0] = bowser->obj->oPosX;
            b->oGfxPos[1] = bowser->obj->oPosY;
            b->oGfxPos[2] = bowser->obj->oPosZ;
            b->oGfxAngle[0] = bowser->obj->oFaceAnglePitch;
            b->oGfxAngle[1] = bowser->obj->oFaceAngleYaw;
            b->oGfxAngle[2] = bowser->obj->oFaceAngleRoll;
            b->oGfxScale[0] = bowser->obj->oScaleX;
            b->oGfxScale[1] = bowser->obj->oScaleY;
            b->oGfxScale[2] = bowser->obj->oScaleZ;
            obj_anim_play_with_sound(b, OMM_BOWSER_ANIM_DEFEATED, 1.f, SOUND_OBJ_BOWSER_DEFEATED | 0xFF00, true);
            if (bowser->index > 2) {
                obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_DEFEATED, 1.f, NO_SOUND, true);
                obj_scale(bowser->obj, 1.01f); // Avoid Z-fighting
                bowser->obj->oFlags |= OBJ_FLAG_NO_SHADOW;
                bowser->obj->oOpacity = 0xFE;
                bhv_omm_bowser_update_action(bowser, OMM_BOWSER_SUB_ACTION_DEFEATED);
            } else {
                obj_set_dormant(bowser->obj, true);
                bhv_omm_bowser_update_action(bowser, 11);
            }
        } break;

        // There is no come back
        case 11: {
        } break;

        // Bowser lost the power of the Sparkly Grand Star
        case OMM_BOWSER_SUB_ACTION_DEFEATED: {
            struct Object *b = obj_get_first_with_behavior(bhvBowser);
            obj_anim_play(bowser->obj, b->oAnimID, (f32) b->oAnimInfo.animAccel / (f32) ANIM_ACCEL_ONE);
            obj_anim_set_frame(bowser->obj, b->oAnimFrame);
            bowser->obj->oFlags |= OBJ_FLAG_NO_SHADOW;
            bowser->obj->oOpacity = relerp_0_1_f(bowser->obj->oAnimFrame, 0, bowser->obj->oCurrAnim->mLoopEnd - 4, 0xFE, 0x00);
            if (bowser->obj->oOpacity == 0) {
                obj_set_dormant(bowser->obj, true);
                bhv_omm_bowser_update_action(bowser, 11);
            }
        } break;
    }
}

//
// Main loop
//

static void bhv_omm_bowser_spawn_sparkly_sparkles(struct Object *o) {
    if (omm_sparkly_is_bowser_4_battle() && (o->oTimer & 1)) {
        omm_obj_spawn_sparkly_star_sparkle(o, gOmmSparklyMode, 150.f, 20.f, 1.f, 100.f);
    }
}

static bool bhv_omm_bowser_update_hint(OmmBowser *bowser, struct MarioState *m, s32 hint, bool skip, bool condition, s32 duration) {

    // Should skip hint?
    if (skip) {
        bowser->hint = hint + 1;
        bowser->hintTimer = 0;
        bowser->hintDialogTimer = 0;
        return false;
    }

    // Update hint timer
    bowser->hintTimer += condition;
    if (bowser->hintTimer >= duration) {

        // Update hint dialog timer only when Bowser is on ground
        if (bowser->obj->oPosY <= bowser->floorHeight + 5) {
            bowser->hintDialogTimer++;

            // Should trigger hint?
            if (bowser->hintDialogTimer >= 30 && omm_mario_is_ready_for_dialog(m)) {
                omm_mario_start_dialog(m, bowser->obj, OMM_DIALOG_BOWSER_1_HINT_1 + hint, false, true, 0);
                bowser->hint = hint + 1;
                bowser->hintTimer = 0;
                bowser->hintDialogTimer = 0;
                return true;
            }
        }
    }
    return false;
}

static void bhv_omm_bowser_update_bowser(OmmBowser *bowser) {
    struct Object *b = obj_get_first_with_behavior(bhvBowser);
    if (!obj_is_dormant(b) && bowser->subAction != OMM_BOWSER_SUB_ACTION_DEFEATED) {
        bhv_omm_bowser_spawn_sparkly_sparkles(b);

        // Cappy Capture must be enabled to load the OMM Bowser fight
        if (!bowser->obj->oBehParams) {
            return;
        }

        // If Bowser has not finished to talk, don't init OMM Bowser
        if (b->oAction == 5 || b->oAction == 6) {
            return;
        }

        // The Bowser intro is done, it's time to init and switch to OMM Bowser
        bowser->obj->oPosX = b->oPosX;
        bowser->obj->oPosY = b->oPosY;
        bowser->obj->oPosZ = b->oPosZ;
        bowser->obj->oVelX = 0;
        bowser->obj->oVelY = 0;
        bowser->obj->oVelZ = 0;
        bowser->obj->oFaceAnglePitch = 0;
        bowser->obj->oFaceAngleYaw = b->oFaceAngleYaw;
        bowser->obj->oFaceAngleRoll = 0;
        bowser->obj->oBowserHit = 0;
        bowser->obj->oOpacity = 0xFF;
        bowser->obj->oIntangibleTimer = 0;
        bowser->obj->oBowserRainbowLightEffect = (gCurrLevelNum == LEVEL_BOWSER_3);
        bowser->obj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
        obj_anim_play_with_sound(bowser->obj, OMM_BOWSER_ANIM_IDLE, 1.f, 0, true);
        obj_set_params(bowser->obj, INTERACT_DAMAGE, 3, -1, 0, true);
        obj_reset_hitbox(bowser->obj, 200, 400, 200, 400, 0, 0);

        // Deactivate old Bowser objects
        omm_array_for_each(omm_obj_get_bowser_behaviors(), p_bhv) {
            const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
            for_each_object_with_behavior(obj, bhv) {
                obj_set_dormant(obj, true);
            }
        }

        // Deactivate the pointy things below mines in Bowser 3
        if (gCurrLevelNum == LEVEL_BOWSER_3) {
            for_each_object_with_behavior(obj, bhvStaticObject) {
                if (obj->oGraphNode == gLoadedGraphNodes[MODEL_LEVEL_GEOMETRY_03]) {
                    obj_set_dormant(obj, true);
                }
            }
        }

        // Init OMM Bowser
        bhv_omm_bowser_init(bowser);
        bhv_omm_bowser_update_combo(bowser);
        b->oBehParams2ndByte = bowser->bowserType;
    }

    // Update
    bowser->jumpShake = max_s(bowser->jumpShake - 1, 0);
OMM_BOWSER_CLONE_CODE(
    obj_reset_hitbox(bowser->obj, 200, 400, 200, 400, 0, 0);
);

    // Hints update (BITDW only)
    if (bowser->index == 0) {
        struct MarioState *m = gMarioState;
        s32 bowserAction = omm_bowser_current_action->action;

        switch (bowser->hint) {

            // Hint 1: Tells the player to capture a flaming bob-omb
            // - Triggers after 20 seconds
            // - Skipped if the player captures a flaming bob-omb
            case 0: {
                if (bhv_omm_bowser_update_hint(bowser, m, 0, omm_mario_is_capture_flaming_bobomb(m), true, 600)) {
                    return;
                }
            } break;

            // Hint 2: Tells the player to wait the moment when Bowser is the most vulnerable to attack
            // - Triggers if Mario gets out of his possession before damaging Bowser
            // - Skipped if the player damages Bowser with a flaming bob-omb
            case 1: {
                if (bhv_omm_bowser_update_hint(bowser, m, 1, bowserAction == OMM_BOWSER_ACTION_DAMAGED, m->prevAction == ACT_OMM_POSSESSION || m->prevAction == ACT_OMM_POSSESSION_UNDERWATER, 1)) {
                    return;
                }
            } break;

            // Hint 3: Tells the player to grab Bowser by the tail at the end of the fight
            // - Triggers after 5 seconds
            // - Skipped if the player grabs Bowser's tail
            case 2: {
                if (bhv_omm_bowser_update_hint(bowser, m, 2, m->action == ACT_OMM_GRAB_BOWSER, bowserAction == OMM_BOWSER_ACTION_DAMAGED && bowser->subAction >= 6, 120)) {
                    return;
                }
            } break;
        }
    }

    // Actions
    switch (omm_bowser_current_action->action) {
        case OMM_BOWSER_ACTION_JUMP:
        case OMM_BOWSER_ACTION_JUMP_MINE:
            bhv_omm_bowser_jump(bowser);
            bhv_omm_bowser_check_emergency_jump(bowser);
            bowser->obj->oBowserHit = 0;
            bowser->obj->oHealth = -1;
            break;

        case OMM_BOWSER_ACTION_FIRE_BALLS:
            bhv_omm_bowser_fireballs(bowser);
            bhv_omm_bowser_check_emergency_jump(bowser);
            bowser->obj->oBowserHit = 0;
            bowser->obj->oHealth = -1;
            break;

        case OMM_BOWSER_ACTION_BOMBS:
            bhv_omm_bowser_bombs(bowser);
            bhv_omm_bowser_check_emergency_jump(bowser);
            bowser->obj->oBowserHit = 0;
            bowser->obj->oHealth = -1;
            break;

        case OMM_BOWSER_ACTION_FLAMETHROWER:
            bhv_omm_bowser_flamethrower(bowser);
            bhv_omm_bowser_check_emergency_jump(bowser);
            bowser->obj->oBowserHit = 0;
            bowser->obj->oHealth = -1;
            break;

        case OMM_BOWSER_ACTION_CLONE_START:
            bhv_omm_bowser_clone_start(bowser);
            bhv_omm_bowser_check_emergency_jump(bowser);
            bowser->obj->oBowserHit = 0;
            bowser->obj->oHealth = -1;
            break;

        case OMM_BOWSER_ACTION_CLONE_END:
            bhv_omm_bowser_clone_end(bowser);
            bhv_omm_bowser_check_emergency_jump(bowser);
            bowser->obj->oBowserHit = 0;
            bowser->obj->oHealth = -1;
            break;

        case OMM_BOWSER_ACTION_GET_MAD:
            bhv_omm_bowser_get_mad(bowser);
            bowser->obj->oBowserHit = 0;
            bowser->obj->oHealth = -1;
            break;

        case OMM_BOWSER_ACTION_EXHAUSTED:
            bhv_omm_bowser_exhausted(bowser);
            break;

        case OMM_BOWSER_ACTION_DAMAGED:
            bhv_omm_bowser_damaged(bowser);
            bowser->obj->oBowserHit = 0;
            bowser->obj->oHealth = -1;
            break;
    }

    // Fix Bowser's blinking
OMM_BOWSER_CLONE_CODE(
    bowser->obj->oMoveAngleYaw = bowser->obj->oFaceAngleYaw;
    bowser->obj->oAngleToMario = bowser->obj->oMoveAngleYaw;
);

    // Update timer and sync Bowser's clone
    bowser->timer++;
    bowser->real = bowser;
    if (bowser->clone) {
        bowser->clone->index = bowser->index;
        bowser->clone->bowserType = bowser->bowserType;
        bowser->clone->numMines = bowser->numMines;
        bowser->clone->setupDuration = bowser->setupDuration;
        bowser->clone->setupAngleVel = bowser->setupAngleVel;
        bowser->clone->landRadius = bowser->landRadius;
        bowser->clone->mineRadius = bowser->mineRadius;
        bowser->clone->floorHeight = bowser->floorHeight;
        bowser->clone->mineHeight = bowser->mineHeight;
        bowser->clone->bombMaxHeight = bowser->bombMaxHeight;
        bowser->clone->jumpMaxHeight = bowser->jumpMaxHeight;
        bowser->clone->jumpDeltaAngle = bowser->jumpDeltaAngle;
        bowser->clone->grabMultiplier = bowser->grabMultiplier;
        bowser->clone->phase = bowser->phase;
        bowser->clone->combo = bowser->combo;
        bowser->clone->action = bowser->action;
        bowser->clone->timer = bowser->timer;
        bowser->clone->subAction = bowser->subAction;
        bowser->clone->obj = obj_get_first_with_behavior(bhvOmmBowserClone);
        bowser->clone->real = bowser;
        bowser->clone->clone = bowser->clone;
    }

    // Update Bowser's aura
OMM_BOWSER_CLONE_CODE(
    struct Object *aura = NULL;
    for_each_object_with_behavior(obj, bhvOmmBowserMadAura) {
        if (obj->parentObj == bowser->obj) {
            aura = obj;
            break;
        }
    }
    if (!aura) {
        aura = obj_spawn_from_geo(bowser->obj, omm_geo_bowser_mad_aura, bhvOmmBowserMadAura);
        aura->parentObj = bowser->obj;
        aura->oOpacity = 0;
    }
    s32 inc = 4 * (((bowser->combo == OMM_BOWSER_COMBO_GET_MAD || bowser->combo == OMM_BOWSER_COMBO_ATTACK_UNTIL_EXHAUSTED) && omm_bowser_current_action->action != OMM_BOWSER_ACTION_EXHAUSTED) ? +1 : -1);
    aura->oOpacity = ((clamp_s(aura->oOpacity + inc, 0x00, 0x80) * bowser->obj->oOpacity) / 0xFF);
    f32 dxz = sqrtf(sqr_f(gCamera->focus[2] - gCamera->pos[2]) + sqr_f(gCamera->focus[0] - gCamera->pos[0]));
    f32 dy = abs_f(gCamera->focus[1] - gCamera->pos[1]) / dxz;
    obj_set_xyz(aura, bowser->obj->oPosX, bowser->obj->oPosY + dy * bowser->obj->oScaleY * 100.f, bowser->obj->oPosZ);
    obj_set_angle(aura, 0, atan2s(bowser->obj->oPosZ - gCamera->pos[2], bowser->obj->oPosX - gCamera->pos[0]), 0);
    obj_set_scale(aura, (aura->oOpacity != 0) * bowser->obj->oScaleX, (aura->oOpacity != 0) * bowser->obj->oScaleY, (aura->oOpacity != 0) * bowser->obj->oScaleZ);
);

    // Spawn sparkles
OMM_BOWSER_CLONE_CODE(
    bhv_omm_bowser_spawn_sparkly_sparkles(bowser->obj);
);
}

static void bhv_omm_bowser_update() {
    sOmmBowsers[0].obj = gCurrentObject;
    bhv_omm_bowser_update_bowser(&sOmmBowsers[0]);
}

const BehaviorScript bhvOmmBowser[] = {
    OBJ_TYPE_GENACTOR,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_SET_INT(oInteractType, INTERACT_DAMAGE),
    BHV_LOAD_ANIMATIONS(oAnimations, bowser_seg6_anims_06057690),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_bowser_update),
    BHV_END_LOOP(),
};

static void bhv_omm_bowser_clone_update() {
    struct Object *o = gCurrentObject;
    if (!sOmmBowsers[0].clone || o != sOmmBowsers[1].obj) {
        o->oAction = 2;
    }
    switch (o->oAction) {

        // Slowly appears
        case 0: {
            f32 t = invlerp_0_1_f(o->oSubAction++, 0, 30);
            o->oOpacity = 0xFF * t;
        } break;

        // Started and updated by OMM Bowser
        case 1: {
            o->oSubAction = 0;
            o->oOpacity = 0xFF;
        } break;

        // Slowly disappears
        case 2: {
            f32 t = 1.f - invlerp_0_1_f(o->oSubAction++, 0, 15);
            o->oOpacity = 0xFF * t;
            if (t == 0.f) {
                for_each_object_with_behavior(obj, bhvOmmBowserMadAura) {
                    if (obj->parentObj == o) {
                        obj_mark_for_deletion(obj);
                        break;
                    }
                }
                obj_mark_for_deletion(o);
            }
        } break;
    }
}

const BehaviorScript bhvOmmBowserClone[] = {
    OBJ_TYPE_GENACTOR,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_SET_INT(oInteractType, INTERACT_DAMAGE),
    BHV_LOAD_ANIMATIONS(oAnimations, bowser_seg6_anims_06057690),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_bowser_clone_update),
    BHV_END_LOOP(),
};

//
// Spawner (auto)
//

OMM_ROUTINE_LEVEL_ENTRY(omm_obj_reset_bowser) {
    mem_zero(sOmmBowsers, sizeof(sOmmBowsers));
}

OMM_ROUTINE_UPDATE(omm_obj_spawn_bowser) {
    if (gMarioObject && (
#if OMM_GAME_IS_SM64
        OMM_LEVEL_IS_BOWSER_FIGHT(gCurrLevelNum) ||
#endif
        omm_sparkly_is_bowser_4_battle()))
    {
        struct Object *o = obj_get_first_with_behavior(bhvOmmBowser);
        if (!o) {
            o = obj_spawn_from_geo(gMarioObject, bowser_geo, bhvOmmBowser);
            o->oPosX = 0;
            o->oPosY = 0;
            o->oPosZ = 0;
            o->oBehParams = omm_sparkly_is_bowser_4_battle() || (OMM_CAP_CAPPY_CAPTURE && !gOmmGlobals->yoshiMode);
            o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
            obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
        }
    }
}

bool omm_bowser_is_active() {
    struct Object *bowser = obj_get_first_with_behavior(bhvOmmBowser);
    return bowser && bowser->oBehParams;
}

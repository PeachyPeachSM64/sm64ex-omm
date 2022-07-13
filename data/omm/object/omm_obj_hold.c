#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define ALL_ACTIONS -2

enum {
    HIT_FLOOR       = (1 << 0),
    HIT_WALL        = (1 << 1),
    HIT_ANY         = (HIT_WALL | HIT_FLOOR),
};

enum {
    ACT_ATTACKED    = 1000,
    ACT_DESTROY     = 1001,
    ACT_BOUNCE      = 1002,
};

//
// Data
//

typedef struct {
    const BehaviorScript *behavior;
    s32 available[8];
    s32 heldAnim;
    s32 thrownAnim;
    s32 hitSurface;
    s32 nextAction;
} OmmHoldBehavior;

static const OmmHoldBehavior sOmmHoldBehaviorList[] = {
    { bhvBalconyBigBoo,             { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvBigBully,                  { ALL_ACTIONS },  3,  1, HIT_ANY,   ACT_ATTACKED },
    { bhvBigBullyWithMinions,       { ALL_ACTIONS },  3,  1, HIT_ANY,   ACT_ATTACKED },
    { bhvBigChillBully,             { ALL_ACTIONS },  3,  1, HIT_ANY,   ACT_ATTACKED },
    { bhvBobomb,                    { ALL_ACTIONS },  1,  1, HIT_ANY,   BOBOMB_ACT_EXPLODE },
    { bhvBobombBuddy,               { ALL_ACTIONS },  0,  0, HIT_FLOOR, ACT_BOUNCE },
    { bhvBobombBuddyOpensCannon,    { ALL_ACTIONS },  0,  0, HIT_FLOOR, ACT_BOUNCE },
    { bhvBoo,                       { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvBooWithCage,               { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvBowlingBall,               { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_DESTROY },
    { bhvBreakableBox,              { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_DESTROY },
    { bhvBreakableBoxSmall,         { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_DESTROY },
    { bhvChuckya,                   { ALL_ACTIONS },  2,  2, HIT_ANY,   ACT_DESTROY },
    { bhvEnemyLakitu,               { ALL_ACTIONS },  3,  3, HIT_ANY,   ACT_ATTACKED },
    { bhvFirePiranhaPlant,          { 1, -1 },        9,  9, HIT_FLOOR, ACT_ATTACKED },
    { bhvFlyGuy,                    { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_ATTACKED },
    { bhvFlyingBookend,             { ALL_ACTIONS },  1,  1, HIT_ANY,   ACT_ATTACKED },
    { bhvFreeBowlingBall,           { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_DESTROY },
    { bhvGhostHuntBigBoo,           { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvGhostHuntBoo,              { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvGoomba,                    { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_DESTROY },
    { bhvHauntedChair,              { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_DESTROY },
    { bhvHeaveHo,                   { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_DESTROY },
    { bhvJumpingBox,                { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_DESTROY },
    { bhvKingBobomb,                { 1, 2, 6, -1 },  6,  6, HIT_FLOOR, 4 },
    { bhvKoopa,                     { ALL_ACTIONS },  2,  3, HIT_ANY,   ACT_ATTACKED },
    { bhvKoopaShell,                { ALL_ACTIONS }, -1, -1, HIT_FLOOR, ACT_BOUNCE },
    { bhvMacroUkiki,                { ALL_ACTIONS }, -1, -1, HIT_FLOOR, ACT_BOUNCE },
    { bhvMerryGoRoundBigBoo,        { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvMerryGoRoundBoo,           { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvMips,                      { ALL_ACTIONS },  4,  1, HIT_FLOOR, ACT_BOUNCE },
    { bhvMoneybag,                  { ALL_ACTIONS },  4,  2, HIT_ANY,   ACT_ATTACKED },
    { bhvMontyMole,                 { ALL_ACTIONS },  1,  7, HIT_ANY,   ACT_ATTACKED },
    { bhvMrBlizzard,                { ALL_ACTIONS },  1,  0, HIT_ANY,   ACT_DESTROY },
    { bhvMrI,                       { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_DESTROY },
    { bhvPenguinBaby,               { ALL_ACTIONS },  3,  1, HIT_FLOOR, ACT_BOUNCE },
    { bhvPiranhaPlant,              { ALL_ACTIONS },  9,  9, HIT_ANY,   ACT_DESTROY },
    { bhvPitBowlingBall,            { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_DESTROY },
    { bhvScuttlebug,                { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_ATTACKED },
    { bhvSkeeter,                   { ALL_ACTIONS },  3,  2, HIT_ANY,   ACT_ATTACKED },
    { bhvSmallBully,                { ALL_ACTIONS },  3,  1, HIT_ANY,   ACT_ATTACKED },
    { bhvSmallChillBully,           { ALL_ACTIONS },  3,  1, HIT_ANY,   ACT_ATTACKED },
    { bhvSmallPenguin,              { ALL_ACTIONS },  3,  1, HIT_FLOOR, ACT_BOUNCE },
    { bhvSnufit,                    { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvSpiny,                     { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_DESTROY },
    { bhvSpindrift,                 { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_ATTACKED },
    { bhvSwoop,                     { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_ATTACKED },
    { bhvTuxiesMother,              { ALL_ACTIONS },  3,  1, HIT_FLOOR, ACT_BOUNCE },
    { bhvToadMessage,               { ALL_ACTIONS },  6,  6, HIT_FLOOR, ACT_BOUNCE },
    { bhvUkiki,                     { ALL_ACTIONS }, 12,  3, HIT_FLOOR, ACT_BOUNCE },
    { bhvUnused20E0,                { ALL_ACTIONS },  3,  1, HIT_FLOOR, ACT_BOUNCE },
    { bhvYoshi,                     { ALL_ACTIONS },  1,  1, HIT_FLOOR, ACT_BOUNCE },
#if OMM_GAME_IS_SMMS
    { bhvBee,                       { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvgoombone,                  { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_DESTROY },
#endif
#if OMM_GAME_IS_R96A
    { bhvMotos,                     { ALL_ACTIONS },  1,  1, HIT_FLOOR, 5 },
#endif
#if OMM_GAME_IS_SMSR
    { bhvCustomSMSRShyGuy,          { ALL_ACTIONS },  0,  0, HIT_ANY,   ACT_DESTROY },
    { bhvCustomSMSRSmallBee,        { ALL_ACTIONS }, -1, -1, HIT_ANY,   ACT_ATTACKED },
    { bhvCustomSMSRPeachMessage,    { ALL_ACTIONS },  4,  4, HIT_FLOOR, ACT_BOUNCE },
    { bhvCustomSMSRYoshiMessage,    { ALL_ACTIONS },  1,  1, HIT_FLOOR, ACT_BOUNCE },
    { bhvCustomSMSRStaticMessage,   { ALL_ACTIONS }, -1, -1, HIT_FLOOR, ACT_BOUNCE },
    { bhvCustomSMSRMipsMessage,     { ALL_ACTIONS },  4,  1, HIT_FLOOR, ACT_BOUNCE },
#endif
    { omm_bhv_mips,                 { ALL_ACTIONS },  4,  1, HIT_FLOOR, ACT_BOUNCE },
};

static const OmmHoldBehavior *omm_obj_get_hold_behavior(struct Object *o) {
    for (s32 i = 0; o && i != (s32) OMM_ARRAY_SIZE(sOmmHoldBehaviorList); ++i) {
        if (sOmmHoldBehaviorList[i].behavior == o->behavior) {
            return &sOmmHoldBehaviorList[i];
        }
    }
    return NULL;
} 

static void omm_bhv_hold_update() {
    struct Object *o = gCurrentObject;
    const OmmHoldBehavior *bhv = omm_obj_get_hold_behavior(o);
    o->oIntangibleTimer = -1;
    o->oRoom = -1;
    switch (o->oHeldState) {

        // Held (the holder updates its position and angle)
        case HELD_HELD: {
            if (bhv->heldAnim != -1) {
                obj_anim_play(o, bhv->heldAnim, 1.f);
            }
            obj_update_gfx(o);
        } break;

        // Thrown
        case HELD_THROWN: {
            if (bhv->thrownAnim != -1) {
                obj_anim_play(o, bhv->thrownAnim, 1.f);
            }
            if (o->oWallHitboxRadius == 0.f) {
                o->oWallHitboxRadius = max_s(o->hitboxRadius, o->hurtboxRadius);
            }
            f32 velY = o->oVelY;
            perform_object_step(o, 0);
            o->oVelY -= 4.f;
            obj_update_gfx(o);
            
            // Check hit (floor or wall)
            o->oMoveFlags &= OBJ_MOVE_ON_GROUND;
            o->oMoveFlags |= (OBJ_MOVE_LANDED   * (o->oFloor != NULL && o->oPosY <= o->oFloorHeight));
            o->oMoveFlags |= (OBJ_MOVE_HIT_WALL * (o->oWall  != NULL));
            if (((bhv->hitSurface & HIT_FLOOR) && (o->oMoveFlags & OBJ_MOVE_LANDED)) ||
                ((bhv->hitSurface & HIT_WALL ) && (o->oMoveFlags & OBJ_MOVE_HIT_WALL))) {
                
                // Perform next action
                switch (bhv->nextAction) {

                    // Attack the object
                    case ACT_ATTACKED: {
                        o->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                        gCurBhvCommand = ((const BehaviorScript *) o->oBehaviorCommand) - 2;
                        o->curBhvCommand = ((const BehaviorScript *) o->oBehaviorCommand);
                        o->bhvStackIndex = o->oBehaviorStackIndex;
                        o->oIntangibleTimer = 0;
                        o->oHeldState = HELD_FREE;
                        obj_set_vel(o, 0, 0, 0);
                    } break;

                    // Destroy the object
                    case ACT_DESTROY: {
                        o->oIntangibleTimer = 0;
                        o->oHeldState = HELD_FREE;
                        obj_destroy(o);
                    } break;

                    // Make the object bounce
                    case ACT_BOUNCE: {
                        obj_set_forward_and_y_vel(o, o->oForwardVel / 3.f, abs_f(velY) / 3.f);
                        o->oHeldState = HELD_DROPPED;
                    } break;
                    
                    // Return to the object behavior, but set a specific action
                    default: {
                        gCurBhvCommand = ((const BehaviorScript *) o->oBehaviorCommand) - 2;
                        o->curBhvCommand = ((const BehaviorScript *) o->oBehaviorCommand);
                        o->bhvStackIndex = o->oBehaviorStackIndex;
                        o->oAction = bhv->nextAction;
                        o->oIntangibleTimer = 0;
                        o->oHeldState = HELD_FREE;
                        obj_set_vel(o, 0, velY - 4.f, 0);
                    } break;
                }
            }
        } break;

        // Dropped (bounce)
        case HELD_DROPPED: {
            if (bhv->thrownAnim != -1) {
                obj_anim_play(o, bhv->thrownAnim, 1.f);
            }
            if (o->oWallHitboxRadius == 0.f) {
                o->oWallHitboxRadius = max_s(o->hitboxRadius, o->hurtboxRadius);
            }
            f32 velY = o->oVelY;
            perform_object_step(o, 0);
            o->oVelY -= 4.f;
            obj_update_gfx(o);

            // Check floor collision
            if (o->oPosY <= o->oFloorHeight) {
                obj_set_forward_and_y_vel(o, o->oForwardVel / 2.f, abs_f(velY) / 2.f);
                if (o->oVelY < 4.f || o->oForwardVel < 1.f) {
                    gCurBhvCommand = ((const BehaviorScript *) o->oBehaviorCommand) - 2;
                    o->curBhvCommand = ((const BehaviorScript *) o->oBehaviorCommand);
                    o->bhvStackIndex = o->oBehaviorStackIndex;
                    o->oIntangibleTimer = 0;
                    o->oHeldState = HELD_FREE;
                    obj_set_vel(o, 0, 0, 0);
                }
            }
        } break;
    }
}

static const BehaviorScript omm_bhv_hold[] = {
    OBJ_TYPE_DEFAULT,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_hold_update,
    0x09000000,
};

//
// Public
//

bool omm_obj_is_holdable(struct Object *o) {
    const OmmHoldBehavior *bhv = omm_obj_get_hold_behavior(o);
    for (s32 i = 0; bhv && bhv->available[i] != -1; ++i) {
        if (bhv->available[i] == ALL_ACTIONS ||
            bhv->available[i] == o->oAction) {
            return true;
        }
    }
    return false;
}

bool omm_obj_hold(struct Object *o) {
    if (o->oHeldState == HELD_FREE && omm_obj_is_holdable(o)) {
        o->oBehaviorCommand = o->curBhvCommand;
        o->oBehaviorStackIndex = o->bhvStackIndex;
        o->curBhvCommand = omm_bhv_hold;
        o->bhvStackIndex += 1;
        o->oHeldState = HELD_HELD;
        o->oIntangibleTimer = -1;
        o->oRoom = -1;
        return true;
    }
    return false;
}

bool omm_obj_update_held_object(struct Object *o, struct Object *holder, f32 xOffset, f32 yOffset, f32 zOffset) {
    if (omm_obj_is_holdable(o)) {
        Vec3f offset = { xOffset * holder->oScaleX, yOffset * holder->oScaleY, zOffset * holder->oScaleZ };
        vec3f_rotate_zxy(offset, offset, holder->oFaceAnglePitch, holder->oFaceAngleYaw, holder->oFaceAngleRoll);
        obj_set_pos(o, holder->oPosX + offset[0], holder->oPosY + offset[1], holder->oPosZ + offset[2]);
        obj_set_angle(o, holder->oFaceAnglePitch, holder->oFaceAngleYaw, holder->oFaceAngleRoll);
        obj_update_gfx(o);
        o->oOpacity = 0xFF;
        o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
        o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
        o->oIntangibleTimer = -1;
        o->oRoom = -1;
        return true;
    }
    return false;
}

bool omm_obj_throw(struct Object *o, f32 forwardVel, f32 yVel) {
    if (omm_obj_is_holdable(o)) {
        o->oHeldState = HELD_THROWN;
        o->oIntangibleTimer = -1;
        o->oRoom = -1;
        obj_set_forward_and_y_vel(o, forwardVel, yVel);
        return true;
    }
    return false;
}

OmmArray omm_obj_get_holdable_behaviors() {
    static OmmArray sOmmHoldBehaviors = omm_array_zero;
    if (!sOmmHoldBehaviors.p) {
        for (s32 i = 0; i != (s32) OMM_ARRAY_SIZE(sOmmHoldBehaviorList); ++i) {
            omm_array_add(sOmmHoldBehaviors, ptr, sOmmHoldBehaviorList[i].behavior);
        }
    }
    return sOmmHoldBehaviors;
}

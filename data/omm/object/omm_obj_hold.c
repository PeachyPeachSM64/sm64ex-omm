#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void bhv_omm_hold_update() {
    struct Object *o = gCurrentObject;
    const OmmBhvDataHoldable *h = omm_behavior_data_get_holdable(o->behavior);
    if (h) {
        o->oIntangibleTimer = -1;
        o->oRoom = -1;
        switch (o->oHeldState) {

            // Held (the holder updates its position and angle)
            case HELD_HELD: {
                if (h->heldAnim != -1) {
                    obj_anim_play(o, h->heldAnim, 1.f);
                }
                obj_update_gfx(o);
            } break;

            // Thrown
            case HELD_THROWN: {
                if (h->thrownAnim != -1) {
                    obj_anim_play(o, h->thrownAnim, 1.f);
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
                if (((h->hitSurface & HOLD_HIT_FLOOR) && (o->oMoveFlags & OBJ_MOVE_LANDED)) ||
                    ((h->hitSurface & HOLD_HIT_WALL ) && (o->oMoveFlags & OBJ_MOVE_HIT_WALL))) {
                    
                    // Perform next action
                    switch (h->nextAction) {

                        // Attack the object
                        case HOLD_ACT_ATTACKED: {
                            o->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                            gCurBhvCommand = ((const BehaviorScript *) o->oBhvCommand) - 2;
                            o->curBhvCommand = ((const BehaviorScript *) o->oBhvCommand);
                            o->bhvStackIndex = (u32) o->oBhvStackIndex;
                            o->oIntangibleTimer = 0;
                            o->oHeldState = HELD_FREE;
                            obj_set_vel(o, 0, 0, 0);
                        } break;

                        // Destroy the object
                        case HOLD_ACT_DESTROY: {
                            o->oIntangibleTimer = 0;
                            o->oHeldState = HELD_FREE;
                            obj_destroy(o);
                        } break;

                        // Make the object bounce
                        case HOLD_ACT_BOUNCE: {
                            obj_set_forward_and_y_vel(o, o->oForwardVel / 3.f, abs_f(velY) / 3.f);
                            o->oHeldState = HELD_DROPPED;
                        } break;
                        
                        // Return to the object behavior, but set a specific action
                        default: {
                            gCurBhvCommand = ((const BehaviorScript *) o->oBhvCommand) - 2;
                            o->curBhvCommand = ((const BehaviorScript *) o->oBhvCommand);
                            o->bhvStackIndex = (u32) o->oBhvStackIndex;
                            o->oAction = h->nextAction;
                            o->oIntangibleTimer = 0;
                            o->oHeldState = HELD_FREE;
                            obj_set_vel(o, 0, velY - 4.f, 0);
                        } break;
                    }
                }
            } break;

            // Dropped (bounce)
            case HELD_DROPPED: {
                if (h->thrownAnim != -1) {
                    obj_anim_play(o, h->thrownAnim, 1.f);
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
                        gCurBhvCommand = ((const BehaviorScript *) o->oBhvCommand) - 2;
                        o->curBhvCommand = ((const BehaviorScript *) o->oBhvCommand);
                        o->bhvStackIndex = (u32) o->oBhvStackIndex;
                        o->oIntangibleTimer = 0;
                        o->oHeldState = HELD_FREE;
                        obj_set_vel(o, 0, 0, 0);
                    }
                }
            } break;
        }
    }
}

static const BehaviorScript bhvOmmHold[] = {
    OBJ_TYPE_DEFAULT,
    0x08000000,
    0x0C000000, (uintptr_t) bhv_omm_hold_update,
    0x09000000,
};

//
// Public
//

bool omm_obj_is_holdable(struct Object *o) {
    const OmmBhvDataHoldable *h = omm_behavior_data_get_holdable(o->behavior);
    for (s32 i = 0; h && i != h->actionsCount; ++i) {
        if (h->actions[i] == HOLD_ALL_ACTIONS ||
            h->actions[i] == o->oAction) {
            return true;
        }
    }
    return false;
}

bool omm_obj_hold(struct Object *o) {
    if (o->oHeldState == HELD_FREE && omm_obj_is_holdable(o)) {
        o->oBhvCommand = (void *) o->curBhvCommand;
        o->oBhvStackIndex = (s32) o->bhvStackIndex;
        o->curBhvCommand = bhvOmmHold;
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
    static OmmArray sOmmHoldableBehaviors = omm_array_zero;
    if (!sOmmHoldableBehaviors.p) {
        for_each_until_null(const BehaviorScript *, bhv, omm_get_behaviors_list()) {
            if (omm_behavior_data_get_holdable(*bhv)) {
                omm_array_add(sOmmHoldableBehaviors, ptr, *bhv);
            }
        }
    }
    return sOmmHoldableBehaviors;
}
